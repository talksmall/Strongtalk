/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.57 $ */
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

# include "incls/_rscope.cpp.incl"

RScope::RScope(RNonDummyScope* s, int bci) : _senderBCI(bci) {
  _sender = s; 
  if (s) {
    s->addScope(bci, this);
    nsends = s->nsends;
  } else {
    nsends = -1;
  }
}

GrowableArray<RScope*>* RNullScope::subScopes(int bci) const {
  return new GrowableArray<RScope*>(1);
}

static int compare_pcDescs(PcDesc** a, PcDesc** b) {
  // to sort by descending scope and ascending bci
  int diff = (*b)->scope - (*a)->scope;
  return diff ? diff : (*a)->byteCode - (*b)->byteCode;
}
  
int RNonDummyScope::compare(RNonDummyScope** a,  RNonDummyScope** b) {
  return (*b)->scopeID() - (*a)->scopeID();
}
  
RNonDummyScope::RNonDummyScope(RNonDummyScope* s, int bci, methodOop m, int level)
: RScope(s, bci),  
  _level(level), 
  uncommon(1),
  ncodes(m == NULL ? 1 : m->size_of_codes() * oopSize) {
  _subScopes = NEW_RESOURCE_ARRAY(GrowableArray<RScope*>*, ncodes + 1);
  for (int i = 0; i <= ncodes; i++) _subScopes[i] = NULL;
}

RInlinedScope::RInlinedScope(RNonDummyScope* s, int bci, const nmethod* n, ScopeDesc* d, int level)
: RNonDummyScope(s, bci, d->method(), level), desc(d), nm(n) {}


RPICScope::RPICScope(const nmethod* c, PcDesc* pc, CompiledIC* s, klassOop k,
		     ScopeDesc* dsc, nmethod* n, methodOop m, int ns, int lev, bool tr)
  : RNonDummyScope(NULL, pc->byteCode, m, lev), 
    caller(c), _sd(s), pcDesc(pc), klass(k), nm(n), _method(m), trusted(tr), desc(dsc) {
  nsends = ns; 
  extended = false;
}

RDatabaseScope::RDatabaseScope(RNonDummyScope* sender, int bci, klassOop receiver_klass, methodOop method, int level)
: RNonDummyScope(sender, bci, method, level) {
  _receiver_klass = receiver_klass;
  _method         = method;
  _key            = LookupKey::allocate(receiver_klass, method->is_blockMethod() ? oop(method) : oop(_method->selector()));
  _uncommon       = new GrowableArray<bool>(ncodes);
  for (int i = 0; i <= ncodes; i++) _uncommon->append(false);
}

RUntakenScope::RUntakenScope(RNonDummyScope* sender, PcDesc* p, bool u)
  : RNonDummyScope(sender, p->byteCode, NULL, 0), isUncommon(u), pc(p) { 
  int i = 0;	// to allow setting breakpoints
}

RUninlinableScope::RUninlinableScope(RNonDummyScope* sender, int bci) : RNullScope(sender, bci) { 
  int i = 0;	// to allow setting breakpoints
}

RInterpretedScope::RInterpretedScope(RNonDummyScope* sender, int bci, LookupKey* key, 
				     methodOop m, int level, bool trusted) 
  : RNonDummyScope(sender, bci, m, level) {
  _key = key; _method = m; this->nsends = m->invocation_count(); this->trusted = trusted;
  extended = false;
}

LookupKey* RInlinedScope::key() const {
  return desc->key();
}

LookupKey* RPICScope::key() const {
  // If we have a nmethod, return the key of the nmethod
  if (nm)
    return (LookupKey*) &nm->key;

  // If we have a scope desc, return the key of the scope desc
  if (desc)
    return desc->key();

  // If we have a send desc, return an allocated lookup key
  if (sd()) {
    return sd()->isSuperSend()
         ? LookupKey::allocate(klass, _method)
         : LookupKey::allocate(klass, _method->selector());
  }
  ShouldNotReachHere();
  // return nm ? (LookupKey*)&nm->key : LookupKey::allocate(klass, _method->selector()); 
  //			// potential bug -- is key correct?  (super sends) -- fix this
  return NULL;
}

methodOop RInlinedScope::method() const { return desc->method(); }

bool RScope::wasNeverExecuted() const {
  methodOop m = method();
  symbolOop sel = m->selector();
  if (InliningPolicy::isInterpreterPredictedSmiSelector(sel) ||
      InliningPolicy::isInterpreterPredictedBoolSelector(sel)) {
    // predicted methods aren't called by interpreter if prediction succeeds
    return false;
  } else {
    return m->was_never_executed();
  }
}


// equivalent: test whether receiver scope and argument (a InlinedScope or a LookupKey) denote the same source-level scope

bool RInterpretedScope::equivalent(LookupKey* l) const {
  return _key->equal(l);
}
  
bool RInterpretedScope::equivalent(InlinedScope* s) const {
  return _key->equal(s->key());
}
  
bool RInlinedScope::equivalent(LookupKey* l) const {
  return desc->l_equivalent(l);
}
  
bool RInlinedScope::equivalent(InlinedScope* s) const {
  if (!s->isInlinedScope()) return false;
  InlinedScope* ss = (InlinedScope*)s;
  // don't use ss->rscope because it may not be set yet; but ss's sender
  // must have an rscope if ss is equivalent to this.
  return ss->senderBCI() == desc->senderBCI() &&
         ss->sender()->rscope == sender();
}
  
bool RPICScope::equivalent(InlinedScope* s) const {
  Unused(s);
  // an RPICScope represents a non-inlined scope, so it can't be equivalent
  // to any InlinedScope
  return false;
}

bool RPICScope::equivalent(LookupKey* l) const {
  if (desc != NULL) return desc->l_equivalent(l);   // compiled case
  assert(!_sd->isSuperSend(), "this code probably doesn't work for super sends");
  return klass == l->klass() && _method->selector() == l->selector();
}
  
RScope* RNonDummyScope::subScope(int bci, LookupKey* k) const {
  // return the subscope matching the lookup
  assert(bci >= 0 && bci < ncodes, "bci out of range");
  GrowableArray<RScope*>* list = _subScopes[bci];
  if (list == NULL) return new RNullScope;
  for (int i = 0; i < list->length(); i++) {
    RScope* rs = list->at(i);
    if (rs->equivalent(k))
      return rs;
  }
  return new RNullScope;
}

GrowableArray<RScope*>* RNonDummyScope::subScopes(int bci) const {
  // return all subscopes at bci
  assert(bci >= 0 && bci < ncodes, "bci out of range");
  GrowableArray<RScope*>* list = _subScopes[bci];
  if (list == NULL) return new GrowableArray<RScope*>(1);
  return list;
}

bool RNonDummyScope::hasSubScopes(int bci) const {
  assert(bci >= 0 && bci < ncodes, "bci out of range");
  return _subScopes[bci] != NULL;
}

void RNonDummyScope::addScope(int bci, RScope* s) {
  assert(bci >= 0 && bci < ncodes, "bci out of range");
  if (_subScopes[bci] == NULL) _subScopes[bci] = new GrowableArray<RScope*>(5);
  assert(!_subScopes[bci]->contains(s), "already there");
  // remove uninlineble markers if real scopes are added
  if (_subScopes[bci]->length() == 1 && _subScopes[bci]->first()->isUninlinableScope()) {
    _subScopes[bci]->pop();
  }
  _subScopes[bci]->append(s);
}

bool RInterpretedScope::isUncommonAt(int bci) const { return DeferUncommonBranches; }

bool RNonDummyScope::isUncommonAt(int bci) const {
  if (_subScopes[bci]) {
    RScope* s = _subScopes[bci]->first();
    if (s && s->isUntakenScope()) {
      // send was never executed - make it uncommon
      return true;
    }
  }
  return false;
}

bool RNonDummyScope::isNotUncommonAt(int bci) const {
  assert(bci >= 0 && bci < ncodes, "bci out of range");

  // check if program got uncommon trap in the past
  for (int i = 0; i < uncommon.length(); i++) {
    if (uncommon.at(i)->bci() == bci) return true;
  }

  if (_subScopes[bci]) {
    RScope* s = _subScopes[bci]->first();
    if (s && !s->isUntakenScope()) {
      // send was executed at least once - don't make it uncommon
      return true;
    }
  }
  return false;
}

Expr* RScope::receiverExpr(PReg* p) const {
  // guess that true/false map really means true/false object
  // (gives more efficient testing code)
  klassOop k = receiverKlass();
  if (k == trueObj->klass()) {
    return new ConstantExpr(trueObj, p, NULL);
  } else if (k == falseObj->klass()) {
    return new ConstantExpr(falseObj, p, NULL);
  } else {
    return new KlassExpr(k, p, NULL);
  }
}

Expr* RUntakenScope::receiverExpr(PReg* p) const {
  return new UnknownExpr(p, NULL, true);
}

bool RInlinedScope::isLite() const 	{ return desc->is_lite(); }
bool RPICScope::isLite() const 	{ return desc && desc->is_lite(); }
klassOop RInterpretedScope::receiverKlass() const { return _key->klass(); }
klassOop RInlinedScope::receiverKlass() const { return desc->selfKlass(); }

void RNonDummyScope::unify(RNonDummyScope* s) {
  assert(ncodes == s->ncodes, "should be the same");
  for (int i = 0; i < ncodes; i++) {
    _subScopes[i] = s->_subScopes[i];
    if (_subScopes[i]) {
      for (int j = _subScopes[i]->length() - 1; j >= 0; j--) {
	_subScopes[i]->at(j)->_sender = this;
      }
    }
  }
}

void RPICScope::unify(RNonDummyScope* s) {
  RNonDummyScope::unify(s);
  if (s->isPICScope()) {
    uncommon.appendAll(&((RPICScope*)s)->uncommon);
  }

}

const int UntrustedPICLimit = 2;
const int PICTrustLimit = 2;
  
static void getCallees(const nmethod* nm, 
		       GrowableArray<PcDesc*>*&   taken_uncommon,
		       GrowableArray<PcDesc*>*& untaken_uncommon,
		       GrowableArray<PcDesc*>*& uninlinable,
		       GrowableArray<RNonDummyScope*>*& sends, 
		       bool trusted, 
		       int level) {
  // return a list of all uncommon branches of nm, plus a list
  // of all nmethods called by nm (in the form of PICScopes)
  // all lists are sorted by scope (biggest offset first)
  if (theCompiler && CompilerDebug) {
    cout(PrintRScopes)->print("%*s*searching nm %#lx \"%s\" (%strusted; %ld callers)\n", 2 * level, "",
			      nm, nm->key.selector()->as_string(), trusted ? "" : "not ", nm->ncallers());
  }
  taken_uncommon = new GrowableArray<PcDesc*>(1);
  untaken_uncommon = new GrowableArray<PcDesc*>(16);
  uninlinable = new GrowableArray<PcDesc*>(16);
  sends = new GrowableArray<RNonDummyScope*>(10);
  relocIterator iter(nm);
  while (iter.next()) {
    if (iter.type() == relocInfo::uncommon_type) {
      GrowableArray<PcDesc*>* l = iter.wasUncommonTrapExecuted() ? taken_uncommon : untaken_uncommon;
      l->append(nm->containingPcDesc((char*)iter.word_addr()));
    }
  }

  taken_uncommon->sort(&compare_pcDescs);
  untaken_uncommon->sort(&compare_pcDescs);

  if (TypeFeedback) {
    relocIterator iter(nm);
    while (iter.next()) {
      if (iter.type() != relocInfo::ic_type)  continue;
      CompiledIC* sd = iter.ic();
      PcDesc* p = nm->containingPcDesc((char*)sd);
      if (sd->wasNeverExecuted()) {
	// this send was never taken
	sends->append(new RUntakenScope(NULL, p, false));
      } else if (sd->isUninlinable() || sd->isMegamorphic()) {
	// don't inline this send
	uninlinable->append(p);
      } else {
	bool useInfo  = trusted || sd->ntargets() <= UntrustedPICLimit;
	if (useInfo) {
	  CompiledIC_Iterator it(sd);
	  while (!it.at_end()) {
	    nmethod* callee = it.compiled_method();
	    methodOop m = it.interpreted_method();
	    ScopeDesc* desc;
	    int count;
            if (callee != NULL) {
	      // compiled target
	      desc = callee->scopes()->root();
	      count = callee->invocation_count() / max(1, callee->ncallers());
	    } else {
	      // interpreted target
	      desc = NULL;
	      count = m->invocation_count();
	    }
	    sends->append(new RPICScope(nm, p, sd, it.klass(), desc, callee, m, count, level, trusted));
	    it.advance();
	  }
	} else if (theCompiler && CompilerDebug) {
	  cout(PrintRScopes)->print("%*s*not trusting PICs in sd %#lx \"%s\" (%ld cases)\n",
	    			    2*level, "", sd, sd->selector()->as_string(), sd->ntargets());
	}
      }
    }
    sends->sort(&RPICScope::compare);
    uninlinable->sort(&compare_pcDescs);
  }
}

RNonDummyScope* RNonDummyScope::constructRScopes(const nmethod* nm, bool trusted, int level) {
  // construct nm's RScope tree and return the root
  // level > 0 means recursive invocation through a RPICScope (level
  // is the recursion depth); trusted means PICs info is considered accurate
  RNonDummyScope* current = NULL;
  RNonDummyScope* root = NULL;
  GrowableArray<PcDesc*>* taken_uncommon;
  GrowableArray<PcDesc*>* untaken_uncommon;
  GrowableArray<PcDesc*>* uninlinable;
  GrowableArray<RNonDummyScope*>* sends;
  getCallees(nm, taken_uncommon, untaken_uncommon, uninlinable, sends, trusted, level);

  // visit each scope in the debug info and enter it into the tree
  FOR_EACH_SCOPE(nm->scopes(), s) {
    // search s' sender RScope
    ScopeDesc* sender = s->sender();
    for (RNonDummyScope* rsender = current; rsender; rsender = rsender->sender()) {
      if (rsender->isInlinedScope() &&
	  ((RInlinedScope*)rsender)->desc->is_equal(sender))
	break;
    }
    int bci = sender ? s->senderBCI() : IllegalBCI;
    current = new RInlinedScope((RInlinedScope*)rsender, bci, nm, s, level);
    if (!root) {
      root = current;
      root->nsends = nm->invocation_count();
    }

    // enter bcis with taken uncommon branches
    while (taken_uncommon->nonEmpty() && taken_uncommon->top()->scope == s->offset()) {
      current->uncommon.push(new RUncommonBranch(current, taken_uncommon->pop()));
    }
    // enter info from PICs
    while (sends->nonEmpty() && sends->top()->scopeID() == s->offset()) {
      RNonDummyScope* s = sends->pop();
      s->_sender = current; 
      current->addScope(s->senderBCI(), s);
    }
    // enter untaken uncommon branches
    PcDesc* u;
    while (untaken_uncommon->nonEmpty() && (u = untaken_uncommon->top())->scope == s->offset()) {
      new RUntakenScope(current, u, true);    // will add it as subscope of current
      untaken_uncommon->pop();
    }
    // enter uninlinable sends
    while (uninlinable->nonEmpty() && (u = uninlinable->top())->scope == s->offset()) {
      // only add uninlinable markers for sends that have no inlined cases
      int bci = u->byteCode;
      if (!current->hasSubScopes(bci)) {
        new RUninlinableScope(current, bci);    // will add it as subscope of current
      }
      uninlinable->pop();
    }
  }
  assert(sends           ->isEmpty(), "sends should have been connected to rscopes");
  assert(  taken_uncommon->isEmpty(), "taken uncommon branches should have been connected to rscopes");
  assert(untaken_uncommon->isEmpty(), "untaken uncommon branches should have been connected to rscopes");
  assert(uninlinable     ->isEmpty(), "uninlinable sends should have been connected to rscopes");
  return root;
}

void RNonDummyScope::constructSubScopes(bool trusted) {
  // construct all our (immediate) subscopes
  methodOop m = method();
  if (m->is_accessMethod()) return;
  CodeIterator iter(m);
  do {
    RNonDummyScope* s = NULL;
    InterpretedIC* ic = iter.ic();
    switch (iter.send()) {
      case Bytecodes::interpreted_send:
      case Bytecodes::compiled_send   :
      case Bytecodes::predicted_send  :
      case Bytecodes::accessor_send   :
      case Bytecodes::polymorphic_send:
      case Bytecodes::primitive_send  :
        { for (InterpretedIC_Iterator it(ic); !it.at_end(); it.advance()) {
            if (it.is_compiled()) {
	      nmethod* nm = it.compiled_method();
	      RNonDummyScope* s = constructRScopes(nm, trusted && trustPICs(m), _level + 1);
	      addScope(iter.bci(), s);
	    } else {
	      methodOop m = it.interpreted_method();
              LookupKey* k = LookupKey::allocate(it.klass(), it.selector());
	      new RInterpretedScope(this, iter.bci(), k, m, _level + 1, trusted && trustPICs(m)); 
	      // NB: constructor adds callee to our subScope list
	    }
	  }
	}
	break;
      case Bytecodes::megamorphic_send:
        new RUninlinableScope(this, iter.bci());  // constructor adds callee to our subScope list
	break;
      case Bytecodes::no_send         :
        break;
      default:
        fatal1("unexpected send type %d", iter.send());
    }
  } while (iter.advance());
}


bool RNonDummyScope::trustPICs(methodOop m) {
  // should the PICs in m be trusted?
  symbolOop sel = m->selector();
  if (sel == vmSymbols::plus() || sel == vmSymbols::minus() ||
      sel == vmSymbols::multiply() || sel == vmSymbols::divide()) {
    // code space optimization: try to avoid unnecessary mixed-type arithmetic
    return false;
  } else {
    return true;	// can't easily determine number of callers
  }
}

bool RPICScope::trustPICs(const nmethod* nm) {
  // should the PICs in nm be trusted?
  int invoc = nm->invocation_count();
  if (invoc < MinInvocationsBeforeTrust) return false;
  int ncallers = nm->ncallers();
  symbolOop sel = nm->key.selector();
  if (sel == vmSymbols::plus() || sel == vmSymbols::minus() ||
      sel == vmSymbols::multiply() || sel == vmSymbols::divide()) {
    // code space optimization: try to avoid unnecessary mixed-type arithmetic
    return ncallers <= 1;
  } else {
    return ncallers <= PICTrustLimit;
  }
}

void RPICScope::extend() {
  // try to follow PIC info one level deeper (i.e. extend rscope tree)
  if (extended) return;
  if (nm && !nm->isZombie()) {
    // search the callee for type info
    RNonDummyScope* s = constructRScopes(nm, trusted && trustPICs(nm), _level + 1);
    // s and receiver represent the same scope - unify them
    unify(s);
  } else {
    constructSubScopes(false);    // search interpreted inline caches but don't trust their info
  }
  extended = true;
}

void RInterpretedScope::extend() {
  // try to follow PIC info one level deeper (i.e. extend rscope tree)
  if (!extended) {
    // search the inline caches for type info
    constructSubScopes(trusted);
    if (PrintRScopes) printTree(_senderBCI, _level);
  }
  extended = true;
}

void RScope::print() { 
  std->print("; sender: %#lx@%ld; count %ld\n", PrintHexAddresses ? _sender : 0, _senderBCI, nsends);
}

void RNonDummyScope::printSubScopes() const {
  for (int i = 0; i < ncodes && _subScopes[i] == NULL; i++) ;
  if (i < ncodes) {
    std->print("{ ");
    for (i = 0; i < ncodes; i++) {
      std->print("%#lx ", PrintHexAddresses ? _subScopes[i] : 0);
    }
    std->print("}");
  } else {
    std->print("none");
  }
}
  
void RInterpretedScope::print_short() {
  std->print("((RInterpretedScope*)%#lx) \"%s\" #%ld", PrintHexAddresses ? this : 0, _key->print_string(), nsends);
}
  
void RInlinedScope::print_short() {
  std->print("((RInlinedScope*)%#lx) \"%s\" #%ld", PrintHexAddresses ? this : 0, desc->selector()->as_string(), nsends);
}
  
void RInlinedScope::print() {
  print_short();
  std->print(": scope %#lx; subScopes: ", PrintHexAddresses ? desc : 0);
  printSubScopes();
  if (uncommon.nonEmpty()) { std->print("; uncommon "); uncommon.print(); }
  RScope::print();
}

void RPICScope::print_short() {
  std->print("((RPICScope*)%#lx) \"%s\" #%ld", PrintHexAddresses ? this : 0, method()->selector()->as_string(), nsends);
}
  
void RPICScope::print() {
  print_short();
  std->print(": IC %#lx; subScopes: ", PrintHexAddresses ? _sd : 0);
  printSubScopes();
  if (uncommon.nonEmpty()) { std->print("; uncommon "); uncommon.print(); }
}

void RUntakenScope::print_short() {
  std->print("((RUntakenScope*)%#lx) \"%s\"", PrintHexAddresses ? this : 0);
}

void RUntakenScope::print() {
  print_short();
  assert(!*_subScopes, "should have no subscopes");
  assert(uncommon.isEmpty(), "should have no uncommon branches");
}

void RUncommonBranch::print() {
  std->print("((RUncommonScope*)%#lx) : %#lx@%ld\n", PrintHexAddresses ? this : 0, PrintHexAddresses ? scope : 0, bci());
}

void RUninlinableScope::print_short()	{ std->print("((RUninlinableScope*)%#lx)", PrintHexAddresses ? this : 0); }
void RNullScope::print_short() 		{ std->print("((RNullScope*)%#lx)", PrintHexAddresses ? this : 0); }

void RNullScope::printTree(int bci, int level) const { }

void RScope::printTree(int bci, int level) const {
  std->print("%*s%3ld: ", level * 2, "", bci); ((RScope*)this)->print_short(); std->print("\n");
}

void RNonDummyScope::printTree(int senderBCI, int level) const {
  RScope::printTree(senderBCI, level);
  int u = 0;	      // current position in uncommon
  for (int bci = 0; bci < ncodes; bci++) {
    if (_subScopes[bci]) {
      for (int j = 0; j < _subScopes[bci]->length(); j++) {
	_subScopes[bci]->at(j)->printTree(bci, level + 1);
      }
    }
    for (int j = u; j < uncommon.length() && uncommon.at(j)->bci() < bci; u++, j++) ;
    if (j < uncommon.length() && uncommon.at(j)->bci() == bci) {
      std->print("  %*s%3ld: uncommon\n", level * 2, "", bci);
    }
  }
}

void RDatabaseScope::print() {
  print_short();
  printSubScopes();
}

void RDatabaseScope::print_short() {
  std->print("((RDatabaseScope*)%#lx)  \"%s\"", PrintHexAddresses ? this : 0, _key->print_string());
}

bool RDatabaseScope::equivalent(InlinedScope* s) const {
  Unimplemented();
  return false;
}

bool RDatabaseScope::equivalent(LookupKey* l) const {
  return _key->equal(l);
}

bool RDatabaseScope::isUncommonAt(int bci) const {
  // if the DB has an uncommon branch at bci, treat it as untaken
  return _uncommon->at(bci);  
}

bool RDatabaseScope::isNotUncommonAt(int bci) const {
  // if there's no uncommon branch in the DB, don't use it here either
  return !_uncommon->at(bci);  
}

int  RInlinedScope::inlining_database_size() {
  int result = 1; // Count this node

  for (int i = 0; i < ncodes; i++) {
    if (_subScopes[i]) {
      for (int j = 0; j < _subScopes[i]->length(); j++) {
        result += _subScopes[i]->at(j)->inlining_database_size();
      }
    }
  }
  return result;
}



// don't file out PIC scopes in the output since they're not inlined into the current nmethod;
// same for interpreted scopes 

inline PcDesc* next_uncommon(int scope, int u, GrowableArray<PcDesc*>* uncommon) {
  if (uncommon == NULL || u >= uncommon->length()) return NULL;   // none left
  PcDesc* pc = uncommon->at(u);
  return (pc->scope == scope) ? pc : NULL;
}

void RUninlinableScope::print_inlining_database_on(outputStream* st, GrowableArray<PcDesc*>* uncommon, int bci, int level) {
  // not necessary to actually write out this info since DB-driven compilation won't inline anything not inlined in DB
  // st->print_cr("%*s%d uninlinable", level * 2, "", bci);
}

void RInlinedScope::print_inlining_database_on(outputStream* st, GrowableArray<PcDesc*>* uncommon, int bci, int level) {
  // File out level and bci
  if (bci != -1) {
    st->print("%*s%d ", level * 2, "", bci);
  }

  LookupKey* k = key();
  k->print_inlining_database_on(st);
  st->cr();

  // find scope in uncommon list
  int scope = desc->offset();
  for (int u = 0; uncommon && u < uncommon->length() - 1 && uncommon->at(u)->scope < scope; u++) ;
  PcDesc* current_uncommon = next_uncommon(scope, u, uncommon);

  // File out subscopes
  for (int i = 0; i < ncodes; i++) {
    if (_subScopes[i]) {
      for (int j = 0; j < _subScopes[i]->length(); j++) {
        _subScopes[i]->at(j)->print_inlining_database_on(st, uncommon, i, level + 1);
      }
    }
    if (current_uncommon && current_uncommon->byteCode == i) {
      // nmethod has an uncommon branch at this bci
      st->print_cr("%*s%d uncommon", (level + 1) * 2, "", i);
      // advance to next uncommon branch
      u++;
      current_uncommon = next_uncommon(scope, u, uncommon);
    }
  }
}

# endif
