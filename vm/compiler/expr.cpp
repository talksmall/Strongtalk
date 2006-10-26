/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.47 $ */
/* Copyright (c) 2006, Sun Microsystems, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
	  disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Sun Microsystems nor the names of its contributors may be used to endorse or promote products derived 
	  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT 
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE


*/


# include "incls/_precompiled.incl"

# ifdef DELTA_COMPILER
# include "incls/_expr.cpp.incl"

const int UnknownExpr::UnlikelyBit 	  = 1;
const int MergeExpr::SplittableBit        = 2;
const int MergeExpr::UnknownSetBit        = 4;
const int MergeExpr::ContainingUnknownBit = 8;

const int MaxMergeExprSize = 5;	// max. # exprs in a merge expression

Expr::Expr(PReg* p, Node* n) { 
  assert(p, "must have PReg");
  _preg = p; _node = n; next = NULL; flags = 0; 
  assert(p->scope()->isInlinedScope(), "should be InlinedScope");
  assert(n != NodeBuilder::EndOfCode, "should be a real node");
}


MergeExpr::MergeExpr(Expr* e1, Expr* e2, PReg* p, Node* nod) : Expr(p, nod) {
  initialize();
  if (!p) _preg = e1->preg();
  mergeInto(e1, nod); 
  mergeInto(e2, nod);
#ifdef ASSERT
  verify();
#endif
}

MergeExpr::MergeExpr(PReg* p, Node* nod) : Expr(p, nod) { initialize(); }

void MergeExpr::initialize() {
  exprs = new GrowableArray<Expr*>(MaxMergeExprSize); // NB: won't grow beyond MaxMergeExprSize
  setSplittable(_node != NULL);
}

NoResultExpr::NoResultExpr(Node* n) : Expr(new NoPReg(n ? n->scope() : theCompiler->currentScope()), n) {}

ContextExpr::ContextExpr(PReg* r) : Expr(r, NULL) {}

KlassExpr::KlassExpr(klassOop k, PReg* p, Node* n) : Expr(p, n) {
  _klass = k;
  assert(k, "must have klass");
}
  
BlockExpr::BlockExpr(BlockPReg* p, Node* n) 
  : KlassExpr(blockClosureKlass::blockKlassFor(p->closure()->nofArgs()), p, n) {
  assert(n, "must have a node");
  _blockScope = p->scope();
}

Expr* Expr::asReceiver() const { 
  // the receiver is the Expr* for a newly created InlinedScope; return the Expr that
  // should be used as the new scope's receiver
  assert(hasKlass(), "must have klass");
  return (Expr*)this; 
}

Expr* MergeExpr::asReceiver() const {
  return new KlassExpr(klass(), _preg, _node);
}

// equals: do two expression denote the same type information?

bool UnknownExpr::equals(Expr* other) const {
  return other->isUnknownExpr();
}

bool NoResultExpr::equals(Expr* other) const {
  return other->isNoResultExpr();
}

bool KlassExpr::equals(Expr* other) const {
  return (other->isKlassExpr() || other->isConstantExpr()) &&
    other->klass() == klass();
}

bool BlockExpr::equals(Expr* other) const {
  return other->isBlockExpr() && other->klass() == klass();
}

bool ConstantExpr::equals(Expr* other) const {
  return other->isConstantExpr() && other->constant() == constant() ||
    other->isKlassExpr() && other->klass() == klass();
}

bool MergeExpr::equals(Expr* other) const {
  Unused(other);
  return false; // for now -- fix this later
}  

// mergeWith: return receiver merged with arg; functional (does not modify receiver or arg expr)
Expr* UnknownExpr::mergeWith(Expr* other, Node* n) {
  if (other->isNoResultExpr()) return this;
  if (other->isUnknownExpr()) {
    if (n && node() && other->node()) {
      // preserve splitting info
      MergeExpr* e = new MergeExpr(this, other, preg(), n);
      assert(e->isSplittable(), "wasted effort");
      return e;
    } else {
      _node = NULL; 	// prevent future splitting
      return this;
    }
  } else {
    PReg* r = _preg == other->preg() ? _preg : NULL;
    return new MergeExpr(this, other, r, n);
  }
}

Expr* NoResultExpr::mergeWith(Expr* other, Node* n)  {
  Unused(n);
  return other; 
}

Expr* KlassExpr::mergeWith(Expr* other, Node* n)  {
  if (other->isNoResultExpr()) return this;
  if ((other->isKlassExpr() || other->isConstantExpr())
      && other->klass() == klass()) {
    // generalize klass + constant in same clone family --> klass
    _node = NULL; 	// prevent future splitting
    return this;
  } else {
    PReg* r = _preg == other->preg() ? _preg : NULL;
    return new MergeExpr(this, other, r, n);
  }
}

Expr* BlockExpr::mergeWith(Expr* other, Node* n) {
  if (other->isNoResultExpr()) return this;
  if (equals(other)) {
    if (n && node() && other->node()) {
      // preserve splitting info
      MergeExpr* e = new MergeExpr(this, other, preg(), n);
      assert(e->isSplittable(), "wasted effort");
      return e;
    } else {
      _node = NULL; 	// prevent future splitting
      return this;
    }
  } else {
    PReg* r = _preg == other->preg() ? _preg : NULL;
    return new MergeExpr(this, other, r, n);
  }
}

Expr* ConstantExpr::mergeWith(Expr* other, Node* n)  {
  // NB: be careful not to turn true & false into klasses
  if (other->isNoResultExpr()) return this;
  if (other->isConstantExpr()
      && other->constant() == constant()) {
    if (n && node() && other->node()) {
      // preserve splitting info
      MergeExpr* e = new MergeExpr(this, other, preg(), n);
      assert(e->isSplittable(), "wasted effort");
      return e;
    } else {
      _node = NULL; 	// prevent future splitting
      return this;
    }
  } else if (other->isKlassExpr()) {
    return other->mergeWith(this, n);
  } else {
    PReg* r = _preg == other->preg() ? _preg : NULL;
    return new MergeExpr(this, other, r, n);
  }
}

Expr* MergeExpr::mergeWith(Expr* other, Node* n) {
  assert(_preg == other->preg() || other->isNoResultExpr(), "mismatched PRegs");
  return new MergeExpr(this, other, _preg, n);
}

// mergeInto: merge other expr into receiver; modifies receiver
void MergeExpr::mergeInto(Expr* other, Node* n) {
  if (other->isNoResultExpr()) return;
  setUnknownSet(false);
  if (n == NULL) setSplittable(false);
  _node = n;
  if (other->isMergeExpr()) {
    MergeExpr* o = other->asMergeExpr();
    if (o->isSplittable() && !isSplittable()) {
      int i = 0;
    }
    for (int i = 0; i < o->exprs->length(); i++) {
      // must be careful when adding splittable exprs (e->next != NULL)
      // to avoid creating loops in the ->next chain
      Expr* e = o->exprs->at(i);
      Expr* nexte;
      for ( ; e; e = nexte) {
	nexte = e->next;
	e->next = NULL;
	add(e);
      }
    }
  } else {
    add(other);
  }
#ifdef ASSERT
  int len = exprs->length();
  for (int i = 0; i < len; i++) {
    Expr* e = exprs->at(i);
    for (int j = i + 1; j < len; j++) {
      Expr* e2 = exprs->at(j);
      assert(! e->equals(e2), "duplicate expr");
      assert(! (e->hasKlass() && e2->hasKlass() && e->klass() == e2->klass()),
	     "duplicate klasses");
    }
  }
#endif
  assert(!isSplittable() || _node, "splittable mergeExpr must have a node");
}

// add a new expression to the receiver
void MergeExpr::add(Expr* e) {
  assert(e->next == NULL, "shouldn't be set");
  setUnknownSet(false);
  if (exprs->isFull()) {
    setSplittable(false);
    return;
  }
  if (!e->node()) setSplittable(false);
  for (int i = 0; i < exprs->length(); i++) {
    Expr* e1 = exprs->at(i);
    if (e->hasKlass() && e1->hasKlass() && e->klass() == e1->klass() ||
	e->equals(e1)) {
      // an equivalent expression is already in our list
      // if unsplittable we don't need to do anything except
      // if e is a klass and the expr we already have is a constant
      // (otherwise: might later make unknown unlikely and rely on
      // constant value)
      if (!isSplittable() && !e1->isConstantExpr()) return;
      
      // even though the klass is already in our list, we care about
      // the new entry because we might have to copy the nodes between
      // it and the split send
      // Therefore, we keep lists of equivalent Exprs (linked via the
      // "next" field).
      Node* n = e->node();
      if (n) {
	for (Expr* e2 = exprs->at(i); e2; e2 = e2->next) {
	  if (n == e2->node()) {
	    // node already in list; this can happen if we're merging an expression
	    // with itself (e.g. we inlined 2 cases, both return the same argument)
	    // can't treat as splittable anymore
	    setSplittable(false);
	    break;
	  }
	}
     }

      // generalize different constants to klasses
      if (e->isConstantExpr() && e1->isConstantExpr() &&
	  e->constant() == e1->constant()) {
	// leave them as constants
      } else {
	if (e->isConstantExpr()) {
	  e = e->convertToKlass(e->preg(), e->node());
	}
	if (e1->isConstantExpr()) {
	  // convertToKlass e1 and replace it in receiver
	  Expr* ee = e1->convertToKlass(e1->preg(), e1->node());
	  ee->next = e1->next;
	  exprs->at_put(i, ee);
	}
      }
      if (!isSplittable()) return;
      // append e at end of e1's next chain
      for (e1 = exprs->at(i); e1->next; e1 = e1->next) ;
      e1->next = e;
      return;
    }
  }
  if (exprs->length() == MaxMergeExprSize) {
    // our capacity overflows, so make sure we've got at least one Unknown
    // type in our set
    if (findUnknown() == NULL) exprs->append(new UnknownExpr(e->preg(), NULL));
  } else {
    exprs->append(e);
  }
}

int MergeExpr::nklasses() const {
  int n = 0;
  for (int i = 0; i < exprs->length(); i++) {
    n += exprs->at(i)->nklasses();
  }
  return n;
}


// copyWithout: return receiver w/o the argument expression

Expr* KlassExpr::copyWithout(Expr* e) const {
  assert(e->klass() == klass(), "don't have this klass");
  return new NoResultExpr(node());
}


Expr* ConstantExpr::copyWithout(Expr* e) const {
  assert(e->constant() == constant(), "don't have this constant");
  return new NoResultExpr(node());
}

Expr* MergeExpr::copyWithout(Expr* e) const {
  MergeExpr* res = (MergeExpr*)shallowCopy(preg(), node());
  res->exprs->remove(e);
  return res;
}



bool MergeExpr::really_hasKlass(InlinedScope* s) const {
  // Check if receiver really has only one klass.  Specifically, if we're
  // at the place that made the receiver's unknown part unlikely, the
  // receiver should *not* be considered a KlassExpr because the unknown
  // part hasn't been tested yet.
  return hasKlass() && !(s == unlikelyScope && s->bci() <= unlikelyBCI);
}

bool MergeExpr::hasKlass() const {
  // treat a merge expr like a single klass if it contains only one klass and
  // possibly an unlikely unknown
  if (exprs->length() > 2) return false;
  Expr* e1 = exprs->at(0);
  bool haveKlass1 = e1->hasKlass();
  if (exprs->length() == 1) return haveKlass1;	// only one expr
  UnknownExpr* u1 = e1->findUnknown();
  if (u1 && !u1->isUnlikely()) return false;  // 1st = likely unknown
  Expr* e2 = exprs->at(1);
  bool haveKlass2 = e2->hasKlass();
  UnknownExpr* u2 = e2->findUnknown();
  if (u2 && !u2->isUnlikely()) return false;  // 2nd = likely unknown
  if (haveKlass1 && haveKlass2) return false; 	// 2 klasses
  // success!  one expr may have klass, one is unlikely unknown
  return haveKlass1 || haveKlass2;
}

KlassExpr* MergeExpr::asKlassExpr() const {
  assert(hasKlass(), "don't have a klass");
  Expr* e = exprs->at(0);
  return e->hasKlass() ? e->asKlassExpr() : exprs->at(1)->asKlassExpr();
}

klassOop MergeExpr::klass() const {
  assert(hasKlass(), "don't have a klass");
  Expr* e = exprs->at(0);
  return e->hasKlass() ? e->klass() : exprs->at(1)->klass();
}

bool MergeExpr::hasConstant() const {
  // see hasKlass()...also, must be careful about different constants with
  // same klass (i.e. expr->next is set)
  return false;
}

oop MergeExpr::constant() const {
  ShouldNotCallThis();
  return 0;
}

Expr* ConstantExpr::convertToKlass(PReg* p, Node* n) const {
  return new KlassExpr(_c->klass(), p, n);
}

KlassExpr* ConstantExpr::asKlassExpr() const {
  return new KlassExpr(_c->klass(), _preg, _node);
}

Expr* MergeExpr::convertToKlass(PReg* p, Node* n) const {
  MergeExpr* e = new MergeExpr(p, n);
  for (int i = 0; i < exprs->length(); i++) {
    Expr* expr = exprs->at(i)->convertToKlass(p, n);
    e->add(expr);
  }
  // result is non-splittable because nodes aren't correct and expr->next
  // is ignored for the components of the receiver
  e->setSplittable(false);
  return e;
}

bool MergeExpr::containsUnknown() {
  if (isUnknownSet()) {
    assert ((findUnknown() == NULL) != isContainingUnknown(), "isContainingUnknown wrong");
    return isContainingUnknown();
  }
  setUnknownSet(true);
  for (int i = 0; i < exprs->length(); i++) {
    if (exprs->at(i)->isUnknownExpr()) {
      setContainingUnknown(true);
      return true;
    }
  }
  setContainingUnknown(false);
  return false;
}

UnknownExpr* MergeExpr::findUnknown() const {
  for (int i = 0; i < exprs->length(); i++) {
    if (exprs->at(i)->isUnknownExpr()) return (UnknownExpr*)exprs->at(i);
  }
  return NULL;
}

Expr* MergeExpr::findKlass(klassOop klass) const {
  for (int i = 0; i < exprs->length(); i++) {
    Expr* e = exprs->at(i);
    if (e->hasKlass() && e->klass() == klass) return e;
  }
  return NULL;
}

Expr* UnknownExpr::makeUnknownUnlikely(InlinedScope* s)    {
  Unused(s);
  assert(DeferUncommonBranches, "shouldn't make unlikely");
  // called on an UnknownExpr itself, this is a no-op; works only
  // with merge exprs
  return this;
}

Expr* MergeExpr::makeUnknownUnlikely(InlinedScope* s) {
  assert(DeferUncommonBranches, "shouldn't make unlikely");
  unlikelyScope = s; unlikelyBCI = s->bci();
  for (int i = 0; i < exprs->length(); i++) {
    Expr* e;
    if ((e = exprs->at(i))->isUnknownExpr()) {
      if (!((UnknownExpr*)e)->isUnlikely()) {
	// must make a copy to avoid side-effecting e.g. incoming arg
	UnknownExpr* u = (UnknownExpr*)e;
	UnknownExpr* new_u = new UnknownExpr(u->preg(), u->node(), true);
	exprs->at_put(i, new_u);
	for (u = (UnknownExpr*)u->next; u;
					u = (UnknownExpr*)u->next, new_u = (UnknownExpr*)new_u->next){
	  new_u->next = new UnknownExpr(u->preg(), u->node(), true);
	}
      }
      return this;
    }
  }
  ShouldNotReachHere(); // contains no unknown expression
  return NULL;
}

Expr* ConstantExpr::findKlass(klassOop m) const { return klass() == m ? (Expr*)this : NULL; }

// needsStoreCheck: when storing the expr into the heap, do we need a GC store check?
bool KlassExpr::needsStoreCheck() const {
  return _klass != smiKlassObj;
}
  
bool ConstantExpr::needsStoreCheck() const {
  // don't need a check if either
  // - it's a smi, or
  // - it's an old object (old objects never become young again)
  return ! (_c->is_smi() || _c->is_old());
}
  
Expr* UnknownExpr::shallowCopy(PReg* p, Node* n) const {
  return new UnknownExpr(p, n, isUnlikely());
}

Expr* NoResultExpr::shallowCopy(PReg* p, Node* n) const {
  Unused(p); Unused(n);
  return new NoResultExpr();
}

Expr* KlassExpr::shallowCopy(PReg* p, Node* n) const {
  return new KlassExpr(_klass, p, n);
}

Expr* BlockExpr::shallowCopy(PReg* p, Node* n) const {
  if (p->isBlockPReg()) {
    return new BlockExpr((BlockPReg*)p, n);
  } else {
    // remove block info (might be a performance bug -- should keep
    // around info so can inline (but: would have to check for non-LIFO
    // cases, e.g. when non-clean block is returned from a method)
    return new UnknownExpr(p, n);
  }
}

Expr* ConstantExpr::shallowCopy(PReg* p, Node* n) const {
  return new ConstantExpr(_c, p, n);
}

Expr* MergeExpr::shallowCopy(PReg* p, Node* n) const {
  MergeExpr* e = new MergeExpr(p, n);
  e->exprs = exprs->copy();
  e->setSplittable(isSplittable());
  return e;
}

InlinedScope* Expr::scope() const { 
  assert(_preg->scope()->isInlinedScope(), "oops");
  return (InlinedScope*)_preg->scope(); 
}

NameNode* Expr::nameNode(bool mustBeLegal) const {
  return preg()->nameNode(mustBeLegal); }
  
NameNode* ConstantExpr::nameNode(bool mustBeLegal) const {
  Unused(mustBeLegal);
//c    return newValueName(constant()); }
  return 0;
}

void Expr::print_helper(char* type) {
  lprintf(" (Node %#lx)", node());
  if (next) lprintf(" (next %#lx)", next);
  lprintf("    ((%s*)%#x)\n", type, this);
}
  
void UnknownExpr::print() {
  lprintf("UnknownExpr %s", isUnlikely() ? "unlikely" : "");
  Expr::print_helper("UnknownExpr");
}

void NoResultExpr::print() { lprintf("NoResultExpr "); Expr::print_helper("NoResultExpr"); }
void ContextExpr::print()  { lprintf("ContextExpr %s", preg()->safeName()); Expr::print_helper("ContextExpr"); }

void ConstantExpr::print() {
  lprintf("ConstantExpr %s", constant()->print_value_string()); Expr::print_helper("ConstantExpr");
}

void KlassExpr::print() {
  lprintf("KlassExpr %s", klass()->print_value_string()); Expr::print_helper("KlassExpr"); 
}

void BlockExpr::print() {
  lprintf("BlockExpr %s", preg()->name()); Expr::print_helper("BlockExpr"); 
}

void MergeExpr::print() {
  lprintf("MergeExpr %s(\n", isSplittable() ? "splittable " : "");
  for (int i = 0; i < exprs->length(); i++) {
    lprintf("\t%#lx%s ", exprs->at(i), exprs->at(i)->next ? "*" : "");
    exprs->at(i)->print();
  }
  lprintf(")");
  Expr::print_helper("MergeExpr");
}

void Expr::verify() const {
  if (_preg == NULL) {
    error("Expr %#lx: no preg", this);
  } else {
    _preg->verify();
  }
  //if (_node) _node->verify();
//  if (unlikelyScope) unlikelyScope->verify();
}
  
void KlassExpr::verify() const {
  Expr::verify();
  _klass->verify();
  if (!_klass->is_klass()) error("KlassExpr %#lx: _klass %#lx isn't a klass", this, _klass);
}

void BlockExpr::verify() const {
  Expr::verify();
  if (_blockScope != preg()->creationScope()) 
    error("BlockExpr %#lx: inconsistent parent scope", this, _blockScope, preg()->creationScope());
}

void ConstantExpr::verify() const {
  Expr::verify();
  _c->verify();
}

void MergeExpr::verify() const {
  GrowableArray<Node*> nodes(10);
  for (int i = 0; i < exprs->length(); i++) {
    Expr* e = exprs->at(i);
    e->verify();
    if (e->isMergeExpr()) error("MergeExpr %#lx contains nested MergeExpr %#lx", this, e);
    Node* n = e->node();
    if (n) {
      if (nodes.contains(n)) error("MergeExpr %#lx contains 2 expressions with same node %#lx", this, n);
      nodes.append(n);
    }
  }
  Expr::verify();
}

void ContextExpr::verify()  const {
  Expr::verify();
}


ExprStack::ExprStack(InlinedScope* scope, int size) : GrowableArray<Expr*>(size) {
  _scope = scope;
}


void ExprStack::push(Expr* expr, InlinedScope* currentScope, int bci) {
  assert(!expr->isContextExpr(), "shouldn't push contexts");
  // Register expression e for bci
  currentScope->setExprForBCI(bci, expr);
  // Set r's startBCI if it is an expr stack entry and not already set,
  // currentScope is the scope doing the push.
  PReg* r = expr->preg();
  if (r->isSAPReg()) {
    SAPReg* sr = (SAPReg*)r;
    if (sr->scope() == _scope) {
      if (sr->begBCI() == IllegalBCI)
	sr->_begBCI = sr->creationStartBCI = _scope->bci();
    } else {
      assert(sr->scope()->isSenderOf(_scope), "preg scope too low");
    }
  }
  GrowableArray<Expr*>::push(expr);
}


void ExprStack::push2nd(Expr* expr, InlinedScope* currentScope, int bci) {
  assert(!expr->isContextExpr(), "shouldn't push contexts");
  // Register expression e for current BCI.
  currentScope->set2ndExprForBCI(bci, expr);
  // Set r's startBCI if it is an expr stack entry and not already set,
  // currentScope is the scope doing the push.
  PReg* r = expr->preg();
  if (r->isSAPReg()) {
    SAPReg* sr = (SAPReg*)r;
    if (sr->scope() == _scope) {
      if (sr->begBCI() == IllegalBCI)
	sr->_begBCI = sr->creationStartBCI = _scope->bci();
    } else {
      assert(sr->scope()->isSenderOf(_scope), "preg scope too low");
    }
  }
  GrowableArray<Expr*>::push(expr);
}


void ExprStack::assign_top(Expr* expr) {
  assert(!expr->isContextExpr(), "shouldn't push contexts");
  GrowableArray<Expr*>::at_put(len - 1, expr);
}


Expr* ExprStack::pop() {
  Expr* e = GrowableArray<Expr*>::pop();
  PReg* r = e->preg();
  if (r->isSAPReg()) {
    SAPReg* sr = (SAPReg*)r;
    if (sr->scope() == _scope) {
      // endBCI may be assigned several times
      int newBCI =
	_scope->bci() == EpilogueBCI ? _scope->nofBytes() - 1 : _scope->bci();
      if (bciLT(sr->endBCI(), newBCI)) sr->_endBCI = newBCI;
    } else {
      assert(sr->scope()->isSenderOf(_scope), "preg scope too low");
    }
  }
  return e;
}


void ExprStack::pop(int nofExprsToPop) {
  for (int i = 0; i < nofExprsToPop; i++) pop();
}


void ExprStack::print() {
  const int len = length();
  for (int i = 0; i < len; i++) { 
    lprintf("[TOS - %2d]:  ", len - i - 1);
    at(i)->print();
  }
}

  
# endif
