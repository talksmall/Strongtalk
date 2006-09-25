/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.70 $ */
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
# include "incls/_preg.cpp.incl"

int PReg::currentNo = 0;
int BlockPReg::_numBlocks = 0;
static GrowableArray<ConstPReg*>* constants = 0;
static PReg* dummyPR;
const int PReg::AvgBBIndexLen = 10;
const int PReg::VeryNegative = -9999;		// fix this -- should be int16, really

#define BAD_SCOPE  ((InlinedScope*)1)


/*
LogicalAddress* PReg::createLogicalAddress() {
  if (_logicalAddress == NULL) {
    _logicalAddress = theCompiler->scopeDescRecorder()->createLogicalAddress(nameNode());
  };
  return _logicalAddress;
}
*/


LogicalAddress* PReg::createLogicalAddress() {
  PReg* r = cpReg();
  if (r->_logicalAddress == NULL) {
    r->_logicalAddress = theCompiler->scopeDescRecorder()->createLogicalAddress(nameNode());
  };
  return r->_logicalAddress;
}


// weights indexed by loop depth
static int udWeight[] = { 1, 8, 8*8, 8*8*8, 8*8*8*8 };
const  int udWeightLen = sizeof(udWeight) / sizeof(int) - 1;


void PReg::initPRegs() {
  PReg::currentNo = 0; BlockPReg::_numBlocks = 0;
  constants = new GrowableArray<ConstPReg*>(50);
  dummyPR = new PReg(BAD_SCOPE);
}


SAPReg::SAPReg(InlinedScope* s, int st, int en, bool inContext) : PReg(s), _isInContext(inContext) {
  creationStartBCI = _begBCI 	= st == IllegalBCI ? s->bci() : st;
  _endBCI  			= en == IllegalBCI ? s->bci() : en;
  _creationScope = s;
}


BlockPReg::BlockPReg(InlinedScope* scope, CompileTimeClosure* closure, int beg, int end) : SAPReg(scope, beg, end) {
  _closure = closure; assert(closure, "need a closure");
  _memoized = _escapes = false;
  _escapeNodes = NULL; _uplevelRead = _uplevelWritten = NULL; _contextCopies = NULL;
  _numBlocks++;
  theCompiler->blockClosures->append(this);
  if (MemoizeBlocks) memoize();	    
}

void BlockPReg::addContextCopy(Location* l) {
  if (!_contextCopies) _contextCopies = new GrowableArray<Location*>(3);
  _contextCopies->append(l);
}

void PReg::makeIncorrectDU(bool incU, bool incD) {
  if (incU) _nuses = VeryNegative;
  if (incD) _ndefs = VeryNegative;
}


bool PReg::isLocalTo(BB* bb) const {
  // is this a preg local to bb? (i.e. can it be allocated to temp regs?)
  // treat ConstPRegs as non-local so they don't get allocated prematurely
  // (possible performance bug)
  return
    loc.equals(unAllocated) && !uplevelR() && !debug && !incorrectDU() &&
    !isConstPReg() && dus.length() == 1 && dus.first()->bb == bb;
}


// check basic conditions for global CP
bool PReg::canCopyPropagate() const {
  if (nuses() == 0 || ndefs() != 1) return false;
  // don't propagate if register has incorrect def info or does not
  // survive calls (i.e. is local to BB)
  if (incorrectD() || loc.isTrashedRegister()) return false;
  return true;    	// looks good
}

// NB: _uplevelR/W are initialized lazily to reduce memory consumption
void PReg::addUplevelAccessor(BlockPReg* blk, bool read, bool write) {
  if (read) {
    if (!_uplevelR) _uplevelR = new GrowableArray<BlockPReg*>(5);
    if (!_uplevelR->contains(blk)) _uplevelR->append(blk);
  } 
  if (write) {
    if (!_uplevelW) _uplevelW = new GrowableArray<BlockPReg*>(5);
    if (!_uplevelW->contains(blk)) _uplevelW->append(blk);
  } 
}

void PReg::removeUplevelAccessor(BlockPReg* blk) {
  if (_uplevelR) {
    if (_uplevelR->contains(blk)) _uplevelR->remove(blk);
    if (_uplevelR->isEmpty()) _uplevelR = NULL;
  }
  if (_uplevelW) {
    if (_uplevelW->contains(blk)) _uplevelW->remove(blk);
    if (_uplevelW->isEmpty()) _uplevelW = NULL;
  }
}

void PReg::removeAllUplevelAccessors() {
  _uplevelR = _uplevelW = NULL;
}

ConstPReg* new_ConstPReg(InlinedScope* s, oop c) {
  for (int i = 0; i < constants->length(); i++) {
    ConstPReg* r = constants->at(i);
    if (r->constant == c) {
      // needed to ensure high enough scope (otherwise will break assertions)
      // but in reality it's not needed since ConstPRegs aren't register-allocated right now
      r->extendLiveRange(s);
      return r;
    }
  }
  // constant not found, create new ConstPReg*
  ConstPReg* r = new ConstPReg(s, c);
  constants->append(r);
  r->_ndefs = 1;   	// fake def
  return r;
}


ConstPReg* findConstPReg(Node* n, oop c) {
  // return const preg for oop or NULL if none exists
  for (int i = 0; i < constants->length(); i++) {
    ConstPReg* r = constants->at(i);
    if (r->constant == c) {
      return r->covers(n) ? r : NULL;
    }
  }
  return NULL;    	    	    // constant not found
}


bool ConstPReg::needsRegister() const {
  // register only pays off if we're used more than once and aren't a
  // small immediate constant
//c    return CompilerCSEConstants && weight > 1 && 
//c      (int(constant) > maxImmediate || int(constant) < -maxImmediate);
  return false;
}


void ConstPReg::allocateTo(Location reg) {
  assert(reg.isRegisterLocation(), "should be a register");
  loc = reg;
  assert(_scope->isInlinedScope(), "expected non-access scope");
  //c    ((InlinedScope*)_scope)->allocateConst(this);
  Unimplemented();
}


inline int computeWeight(InlinedScope* s) {
  const int scale = 16;	// normal use counts scale, uncommon use is 1
  if (s && s->isInlinedScope() && ((InlinedScope*)s)->primFailure()) {
    return 1 * udWeight[min(udWeightLen, s->loopDepth)];
  } else {
    return scale * udWeight[min(udWeightLen, s ? s->loopDepth : 0)];
  }
}


void PReg::incUses(Use* use) {
  _nuses++;
  if (use->isSoft()) _nsoftUses++;
  InlinedScope* s = use->node->scope();
  weight += computeWeight(s);
  assert(weight >= _nuses + _ndefs || isConstPReg(), "weight too small");
}     


void PReg::decUses(Use* use) {
  _nuses--;
  if (use->isSoft()) _nsoftUses--;
  InlinedScope* s = use->node->scope();
  weight -= computeWeight(s);
  assert(weight >= _nuses + _ndefs || isConstPReg(), "weight too small");
}


void PReg::incDefs(Def* def) {
  _ndefs++;
  InlinedScope* s = def->node->scope();
  weight += computeWeight(s);
  assert(weight >= _nuses + _ndefs || isConstPReg(), "weight too small");
}


void PReg::decDefs(Def* def) {
  _ndefs--;
  InlinedScope* s = def->node->scope();
  weight -= computeWeight(s);
  assert(weight >= _nuses + _ndefs || isConstPReg(), "weight too small");
}


void PReg::removeUse(DUInfo* info, Use* use) {
  assert(info->reg == this, "wrong reg");
  info->uses.remove(use);
  decUses(use);
}


void PReg::removeUse(BB* bb, Use* use) {
  if (use == NULL) return;
  for (int i = 0; i < dus.length(); i++) {
    PRegBBIndex* index = dus.at(i);
    if (index->bb == bb) {
      DUInfo* info = bb->duInfo.info->at(index->index);
      removeUse(info, use);
      return;
    }
  }
  ShouldNotReachHere(); // info not found
}


void PReg::removeDef(DUInfo* info, Def* def) {
  assert(info->reg == this, "wrong reg");
  info->defs.remove(def);
  _ndefs--;
  InlinedScope* s = def->node->scope();
  weight -= computeWeight(s);
  assert(weight >= _nuses + _ndefs, "weight too small");
}


void PReg::removeDef(BB* bb, Def* def) {
  if (def == NULL) return;
  for (int i = 0; i < dus.length(); i++) {
    PRegBBIndex* index = dus.at(i);
    if (index->bb == bb) {
      DUInfo* info = bb->duInfo.info->at(index->index);
      removeDef(info, def);
      return;
    }
  }
  ShouldNotReachHere(); // info not found
}


void PReg::addDUHelper(Node* n, SList<DefUse*>* l, DefUse* el) {
  int myNum = n->num();
  SListElem<DefUse*>* prev = NULL;
  for (SListElem<DefUse*>* e = l->head();
       e && e->data()->node->num() < myNum;
       prev = e, e = e->next()) ;
  l->insertAfter(prev, el);
}


Use* PReg::addUse(DUInfo* info, NonTrivialNode* n) {
  assert(info->reg == this, "wrong reg");
  Use* u = new Use(n);
  addDUHelper(n, (SList<DefUse*>*)&info->uses, u);
  incUses(u);
  return u;
}


Use* PReg::addUse(BB* bb, NonTrivialNode* n) {
  for (int i = 0; i < dus.length(); i++) {
    PRegBBIndex* index = dus.at(i);
    if (index->bb == bb) {
      DUInfo* info = bb->duInfo.info->at(index->index);
      return addUse(info, n);
    }
  }
  return bb->addUse(n, this);
}


Def* PReg::addDef(DUInfo* info, NonTrivialNode* n) {
  assert(info->reg == this, "wrong reg");
  Def* d = new Def(n);
  addDUHelper(n, (SList<DefUse*>*)&info->defs, d);
  incDefs(d);
  return d;
}


Def* PReg::addDef(BB* bb, NonTrivialNode* n) {
  for (int i = 0; i < dus.length(); i++) {
    PRegBBIndex* index = dus.at(i);
    if (index->bb == bb) {
    DUInfo* info = bb->duInfo.info->at(index->index);
    return addDef(info, n);
    }
  }
  return bb->addDef(n, this);
}

void PReg::forAllDefsDo(Closure<Def*>* c) {
  ::forAllDefsDo(&dus, c);
}

void PReg::forAllUsesDo(Closure<Use*>* c) {
  ::forAllUsesDo(&dus, c);
}

void PReg::allocateTo(Location r) {
  if (CompilerDebug) cout(PrintRegAlloc)->print("*allocating %s to %s\n", name(), r.name());
  assert(loc.equals(unAllocated), "already allocated");
  loc = r;
}


bool PReg::extendLiveRange(Node* n) {
  // the receiver is being copy-propagated to n
  // PRegs currently can't be propagated outside their scope
  // should fix CP: treat all PRegs like SAPReg so can propagate anywhere?
  return extendLiveRange(n->scope(), n->bci());
}

bool PReg::extendLiveRange(InlinedScope* s, int bci) {
  // the receiver is being copy-propagated to n
  // PRegs currently can't be propagated outside their scope
  // should fix CP: treat all PRegs like SAPReg so can propagate anywhere?
  if (s == _scope) {
    return true;	// ok, same scope
  } else if (_scope->isSenderOf(s)) {
    return true;	// scope is caller; already covers n
  } else {
    return false;
  }
}


bool SAPReg::extendLiveRange(Node* n) {
  // the receiver is being copy-propagated to n; try to extend its live range
  return extendLiveRange(n->scope(), n->bci());
}

bool SAPReg::extendLiveRange(InlinedScope* s, int bci) {
  // the receiver is being copy-propagated to scope s at bci; try to extend its live range
  assert(_begBCI != IllegalBCI && creationStartBCI != IllegalBCI &&
	 _endBCI != IllegalBCI, "live range not set");
  if (isInContext()) {
    // context locations cannot be propagated beyond their scope
    // (otherwise the context pointer's live range would have to be extended)
    // was bug 5/3/96  -Urs
    return PReg::extendLiveRange(s, bci);
  }
  bool ok = true;
  if (s == _scope) {
    if (bciLT(bci, _begBCI)) {
      // seems like we're propagating backwards!  happens because of the non-source
      // order of the byte codes in while loops (condition comes after body), so
      // when propagating from code in the condition into code in the body it looks
      // like we're going backwards
      // can't handle this yet -- Urs 7/95
      return false;
    }
    if (bciGT(bci, _endBCI)) _endBCI = bci;
  } else if (s->isSenderOf(_scope)) {
    // propagating upwards - promote receiver to higher scope
    for (InlinedScope* ss = _scope; ss->sender() != s; ss = ss->sender());
    _scope = s;
    _begBCI = ss->senderBCI();
    _endBCI = bci;
  } else if (_scope->isSenderOf(s)) {
    // scope is callee; check if already covered
    for (InlinedScope* ss = s; ss->sender() != _scope; ss = ss->sender()) ;
    int bci = ss->senderBCI();
    if (bciLT(bci, _begBCI)) {
      // seems like we're propagating backwards!  happens because of the non-source
      // order of the byte codes in while loops (condition comes after body), so
      // when propagating from code in the condition into code in the body it looks
      // like we're going backwards
      // can't handle this yet -- Urs 7/95
      return false;
    }
    if (bciGT(bci, _endBCI)) _endBCI = bci;
  } else {
    // can't propagate between siblings yet
    ok = false;
  }
  assert(bciLE(_begBCI, _endBCI) && _begBCI != IllegalBCI &&
	 (bciLE(_endBCI, scope()->nofBytes()) || _endBCI == EpilogueBCI), "invalid start/endBCI");
  return ok;
}


void ConstPReg::extendLiveRange(InlinedScope* s) {
  // make sure the constant reg is in a high enough scope
  if (_scope->isSenderOrSame(s)) {
    // scope is caller of s
  } else if (s->isSenderOf(_scope)) {
    // s is caller of scope, so promote receiver to s
    _scope = s;
  } else {
    // scope and s are siblings of some sort - go up to common sender
    do { s = s->sender(); } while (!s->isSenderOf(_scope));
    _scope = s;
  }
}


bool ConstPReg::covers(Node* n) const {
  // does receiver cover node n (is it live at n)?
  InlinedScope* s = n->scope();
  if (_scope->isSenderOrSame(s)) {
    // ok, scope is caller of s
    return true;
  } 
  return false;
}


bool ConstPReg::extendLiveRange(Node* n) {
  extendLiveRange(n->scope());
  return true;
}


bool PReg::checkEquivalentDefs() const {
  // check if all defs are equivalent, i.e. assign the same preg
  if (ndefs() == 1) return true;
  PReg* rhs = NULL;
  for (int i = 0; i < dus.length(); i++) {
    PRegBBIndex* index = dus.at(i);
    BB* bb = index->bb;
    DUInfo* info = bb->duInfo.info->at(index->index);
    for (SListElem<Def*>* e = info->defs.head(); e; e = e->next()) {
      NonTrivialNode* n = e->data()->node;
      if (!n->isAssignmentLike()) return false;
      if (rhs) {
	if (rhs != n->src()) return false;
      } else {
	rhs = n->src();
      }
    }
  }
  // yup, rhs is the only preg ever assigned to me
  return true;
}


bool PReg::canBeEliminated(bool withUses) const {
  // can this PReg be eliminated without compromising the debugging info?
  assert(_nuses == 0 || withUses, "still has uses");
  if (_ndefs + _nuses == 0) return false; 	// nothing to eliminate
  
  // check if reg can be eliminated
  if (incorrectDU()) { // don't elim if uses are incorrect (hardwired pregs)
    return false;
  }
  
  assert(!_nsoftUses || debug, "nsoftUses should imply debug");

  if (isBlockPReg() && !withUses && !uplevelR()) {
    // blocks can always be eliminated - can describe with BlockValueDesc
    return true;
  }
  
  if (debug) {
    // debug-visible or uplevel-read: eliminate only if run-time value
    // can be reconstructed
    if (cpInfo) {
      // already computed cpInfo, thus can be eliminated 
      assert(!cpReg()->loc.isLocalRegister(), "shouldn't be eliminated");
      //assert(!cpReg()->loc.isLocalRegister(), "shouldn't be eliminated (was bug 4/27  -Urs)");
      return true;
    }
    if (_ndefs > 1) {
      if (isBlockPReg()) {
	// ok; we know all defs of a block are equivalent
      } else if (isSAPReg() && checkEquivalentDefs()) {
	// ok, all defs are the same
      } else {
	if (!checkEquivalentDefs()) {
	  if (CompilerDebug) cout(PrintEliminateUnnededNodes)->print("*not eliminating %s: >1 def && debug-visible\n", name());
	  return false;
	}
      }
    }
    PRegBBIndex* index = dus.first();
    DUInfo* info = index->bb->duInfo.info->at(index->index);
    SListElem<Def*>* e = info->defs.head();
    if (!e) {
      // info not in first elem - would have to search
      if (CompilerDebug) cout(PrintEliminateUnnededNodes)->print("*not eliminating %s: def not in first info\n", name());
      return false;
    }
    NonTrivialNode* defNode = e->data()->node;
    PReg* defSrc;
    bool ok;
    if (defNode->hasConstantSrc()) {
      // constant assignment - easy to handle
      ok = true;
    } else if (defNode->hasSrc() && (defSrc = defNode->src())->isSAPReg() &&
	       !defSrc->loc.isRegisterLocation()) {
      // can substitute defSrc if its lifetime encompasses ours and if
      // it is singly-assigned and not a temp reg (last cond. is necessary to
      // prevent e.g. result of a send (in eax) to be used as the receiver of
      // subsequent scopes that have nsends > 0; fix this if it becomes a
      // performance problem)
      ok = defSrc->scope()->isSenderOf(_scope);
      if (!ok && defSrc->scope() == _scope && isSAPReg()) {
	// same scope, ok if defSrc lives long enough
	ok = bciGE(((SAPReg*)defSrc)->endBCI(), endBCI());
      }
      if (!ok) {
	// try to extend defSrc's live range to cover ours
	ok = defSrc->extendLiveRange(_scope, endBCI());
      }
    } else {
      ok = false;
    }
    if (!ok) {
      if (CompilerDebug) cout(PrintEliminateUnnededNodes)->print("*not eliminating %s: can't recover debug info\n", name());
      return false;	    	    // can't eliminate this PReg
    }
  }
  return true;
}


bool BlockPReg::canBeEliminated(bool withUses) const {
  if (!PReg::canBeEliminated(withUses)) return false;
  if (!_escapes) return true;
  if (uplevelR()) return false;
  
  // escaping, unused block; can be eliminated
  // also, the block doesn't escape anymore
  // _escapes = false;
  assert(nuses() == 0, "still has uses");
  return true;
}


// eliminate all nodes defining me (if possible)
void PReg::eliminate(bool withUses) {
  if (!canBeEliminated(withUses)) return;
  for (int i = 0; i < dus.length(); i++) {
    PRegBBIndex* index = dus.at(i);
    BB* bb = index->bb;
    DUInfo* info = bb->duInfo.info->at(index->index);
    eliminateDefs(info, bb, withUses);
    if (withUses) eliminateUses(info, bb);
  }
}

void PReg::eliminateUses(DUInfo* info, BB* bb) {
  // eliminate all use nodes in info
  SListElem<Use*>* ue = info->uses.head();
  while (ue) {
#ifdef ASSERT
    int oldlen = info->uses.length();	  // for debugging
#endif
    Node* n = ue->data()->node;
    if (CompilerDebug) {
      char buf[1024];
      cout(PrintEliminateUnnededNodes)->print("*%seliminating node N%ld: %s\n", 
	n->canBeEliminated() ? "" : "not ", n->id(), n->print_string(buf)); 
    }
    assert(n->canBeEliminated(), "must be able to eliminate this");
    n->eliminate(bb, this);
    assert(info->uses.length() < oldlen, "didn't remove use");
    ue = info->uses.head();
  }
}

void PReg::eliminateDefs(DUInfo* info, BB* bb, bool removing) {
  // eliminate all defining nodes in info
  SListElem<Def*>* e = info->defs.head();
  while (e) {
#ifdef ASSERT
    int oldlen = info->defs.length();	  // for debugging
#endif
    NonTrivialNode* n = e->data()->node;
    if (n->canBeEliminated()) {
      updateCPInfo(n);
      n->eliminate(bb, this);
      assert(info->defs.length() < oldlen, "didn't remove def");
	e = info->defs.head();	    // simple, but may rescan some uneliminatable nodes
    } else {
      if (CompilerDebug) {
	char buf[1024];
	cout(PrintEliminateUnnededNodes)->print("*not eliminating node N%ld: %s\n", n->id(), n->print_string(buf)); 
      }
      assert(!removing, "cannot eliminate this?");
      e = e->next();
    }
  }
}

void BlockPReg::eliminate(bool withUses) {
  PReg::eliminate(withUses);
  if (nuses() == 0) {
    // the block has been eliminated; remove the uplevel accesses
    // (needed to enable eliminating the accessed contexts)
    if (_uplevelRead) {
      for (int i = _uplevelRead->length() - 1; i >= 0; i--) _uplevelRead->at(i)->removeUplevelAccessor(this);
    }
    if (_uplevelWritten) {
      for (int i = _uplevelWritten->length() - 1; i >= 0; i--) _uplevelWritten->at(i)->removeUplevelAccessor(this);
    }
  }
}

void PReg::updateCPInfo(NonTrivialNode* n) {
  // update/compute cpInfo to keep track of copy-propagation effects for debugging info
  if (cpInfo) {
    if (debug) {
      // canBeEliminated assures that all defs are equivalent
#ifdef ASSERT
      CPInfo* cpi = new_CPInfo(n);
      assert(cpi && cpi->r->cpReg() == cpReg(), "can't handle this");
#endif
    } else {
      // can't really handle CP w/multiple defs; make sure we don't use
      // bad information
      cpInfo->r = dummyPR;
    }
  } else {
    cpInfo = new_CPInfo(n);
    assert(!debug || cpInfo || isBlockPReg(), "couldn't create info");
    if (cpInfo) {
      PReg* r = cpInfo->r;
      // if we're eliminating a debug-visible PReg, the replacement
      // must be debug-visible, too (so that it isn't allocated to
      // a temp reg)
      r->debug |= debug;
      if (r->cpRegs == NULL) r->cpRegs = new GrowableArray<PReg*>(5);
      r->cpRegs->append(this);
    }
  }
}
 

// for efficiency, node n in isLiveAt() must be "plausible", i.e. in a
// scope somewhere below the receiver's scope
// otherwise, use slow_isLiveAt

bool PReg::slow_isLiveAt(Node* n) const {
  if (_scope->isSenderOrSame(n->scope())) {
    return isLiveAt(n);
  } else {
    return false;
  }
}


bool PReg::isLiveAt(Node* n) const {
  // pregs are live in the entire scope (according to Urs, 2/24/96)
  if (!_scope->isSenderOrSame(n->scope())) return false; // cannot be live anymore if s is outside subscopes of _scope
  assert(PrologueBCI == begBCI() && endBCI() == EpilogueBCI, "must be live in the entire scope");
  return true;
}


InlinedScope* findAncestor(InlinedScope* s1, int& bci1, InlinedScope* s2, int& bci2) {
  // find closest common ancestor of s1 and s2, and the
  // respective sender bcis in that scope
  if (s1->depth > s2->depth) {
    while (s1->depth > s2->depth) {
      bci1 = s1->senderBCI(); s1 = s1->sender();
    }
  } else {
    while (s2->depth > s1->depth) {
      bci2 = s2->senderBCI(); s2 = s2->sender();
    }
  }
  assert(s1->depth == s2->depth, "just checkin'...");
  while (s1 != s2) {
    bci1 = s1->senderBCI(); s1 = s1->sender();
    bci2 = s2->senderBCI(); s2 = s2->sender();
  }
  assert(s1->isInlinedScope(), "oops");
  return (InlinedScope*)s1;
}


bool SAPReg::isLiveAt(Node* n) const {
  // is receiver live at Node n?  (may give conservative answer; i.e., it's ok to
  // return true even if the receiver is provably dead)
  // check if receiver is live in source-level terms; if that says
  // dead it really means dead
  InlinedScope* s = n->scope();
  bool live = basic_isLiveAt(s, n->bci());
  if (!live || !loc.isTemporaryRegister()) return live;
  fatal("cannot handle temp registers");
  return false;
}


bool SAPReg::basic_isLiveAt(InlinedScope* s, int bci) const {
  int id = this->id();
  if (!_scope->isSenderOrSame(s)) return false; // cannot be live anymore if s is outside subscopes of _scope
  assert(bciLE(bci, s->nofBytes()) || bci == EpilogueBCI, "bci too high");
  assert(_scope->isSenderOrSame(s), "s is not below my scope");

  // find closest common ancestor of s and creationScope, and the
  // respective bcis in that scope
  int bs = bci;
  int bc = creationStartBCI;
  InlinedScope* ss = findAncestor(s, bs, creationScope(), bc);
  if (!_scope->isSenderOrSame(ss)) fatal("bad scope arg in basic_isLiveAt");

  // Attention: Originally, the live range of a PReg excluded its defining node.
  // The new backend however requires them to be live at the beginning as well.
  // The problem comes from situations where a PReg is used over a sequence of
  // nodes that all belong to the same bci. In general this has been solved in
  // the backend by killing PRegs only at bci boundaries. However, with inlining
  // turned on, this became more difficult: resultPRs are defined and used over
  // bci boundaries (from callee to caller), but when actually testing for liveness,
  // the test sees only the range in the caller, which is all in the same bci.
  // By extending the live range so that it includes its definition this is not
  // a problem anymore.
  //
  // Note: the isLiveAt methods are only used by the new backend (gri 3/27/96).
  if (ss == _scope) {
    // live range = [startBCI, endBCI]			// originally: ]startBCI, endBCI]
    assert(_begBCI == bc ||
	   ss == creationScope() && creationStartBCI == bc, "oops");
    return bciLE(_begBCI, bs) && bciLE(bs, _endBCI);	// originally: bciLT(_begBCI, bs) && bciLE(bs, _endBCI);
  } else {
    // live range = [bc, end of scope]			// originally: ]bc, end of scope]
    return bciLE(bc, bs);				// originally: bciLT(bc, bs);
  }
}


bool PReg::isCPEquivalent(PReg* r) const {
  // is receiver in same register as argument?
  if (this == r) return true;
  // try receiver's CP info
  for (CPInfo* i = cpInfo; i && i->r; i = i->r->cpInfo) {
    if (i->r == r) return true;
  }
  // now try the other way
  for (i = r->cpInfo; i && i->r; i = i->r->cpInfo) {
    if (i->r == this) return true;
  }
  return false;
}
  

// all the nameNode() functions translate the PReg info into debugging info for
// the scopeDescRecorder

NameNode* PReg::locNameNode(bool mustBeLegal) const {
  assert(!loc.isTemporaryRegister() || !mustBeLegal, "shouldn't be in temp reg");
  if (loc.isTemporaryRegister() && !debug) {
    return new IllegalName;
  } else {
    // debug-visible PRegs may have temp regs if they're only visible 
    // from uncommon branches
    return new LocationName(loc);
  }
}


InlinedScope* BlockPReg::parent() const {
  return _closure->parent_scope();
}

  
NameNode* BlockPReg::locNameNode(bool mustBeLegal) const {
  Unused(mustBeLegal);
  assert(!loc.isTemporaryRegister(), "shouldn't be in temp reg");    
  // for now, always use MemoizedName to describe block (even if always created)
  // makes debugging info easier to read (can see which locs must be blocks)
  return new MemoizedName(loc, closure()->method(), closure()->parent_scope()->scopeInfo());
}
  

NameNode* PReg::nameNode(bool mustBeLegal) const {
  PReg* r = cpReg();
  if (!(r->loc.equals(unAllocated))) {
    return r->locNameNode(mustBeLegal);
  } else if (r->isConstPReg()) {
    return r->nameNode(mustBeLegal);
  } else if (r->isBlockPReg()) {
    CompileTimeClosure* c = ((BlockPReg*) r)->closure();
    return new BlockValueName(c->method(), c->parent_scope()->scopeInfo());
  } else {
    // hack: initial nilling of locals isn't represented yet
    // fix this -- should at least check if it's a local
    // also, when entire scopes are removed (e.g. when deleting branches of
    // a type test) the scope should be marked so its debugging info isn't written
    // currently, the contents of such scopes would break several assertions
    return newValueName(nilObj);
#ifdef how_it_should_be
    assert(!debug, "couldn't recover debug info");
    return new IllegalName;
#endif
  }
}

NameNode* ConstPReg::nameNode(bool mustBeLegal) const {
  return newValueName(constant);
}

NameNode* NoPReg::nameNode(bool mustBeLegal) const {
  return new IllegalName;
}


PReg* PReg::cpReg() const {
  // assert(!cpInfo || loc.equals(unAllocated), "allocated regs shouldn't have cpInfo");
  // NB: the above assertion looks tempting but can be wrong: some unused PRegs may still
  // retain their definition because the defining node cannot be eliminated (because it might fail
  // or have other side effects)
  // e.g.: result of ArrayAt operation
  if (cpInfo == NULL) {
    return (PReg*)this;
  } else {
    PReg* r;
    for (CPInfo* i = cpInfo; i; r = i->r, i = r->cpInfo) ;
    return r == dummyPR ? (PReg*)this : r;
  }
}


void BlockPReg::memoize() {
  _memoized = true;
}


void BlockPReg::markEscaped() {
  if (!_escapes) {
    _escapes = true;
    if (CompilerDebug) cout(PrintExposed)->print("*exposing %s\n", name());
    if (MemoizeBlocks) memoize();
  }
}

  
void BlockPReg::markEscaped(Node* n) {
  markEscaped();
  if (_escapeNodes == NULL) _escapeNodes = new GrowableArray<Node*>(5);
  if (! _escapeNodes->contains(n)) _escapeNodes->append(n);
}


// A helper class for BlockPRegs to compute their uplevel accesses
// Note: Uplevel accesses in all branches are taken into account,
// even if it is known at compile-time that a branch of an ifTrue/ifFalse
// is never generated due to a constant condition (the same holds of course
// for whileTrue/whileFalse and failure blocks of primitive calls).
// -> conservative computation of uplevel accesses
class UplevelComputer: public SpecializedMethodClosure {
 public:
  BlockPReg* r;				// the block whose accesses we're computing
  InlinedScope* scope;			// r's scope (i.e., scope creating the block)
  GrowableArray<PReg*>* read;		// list of rscope's temps read by r
  GrowableArray<PReg*>* written;	// same for written temps 
  int nestingLevel;			// nesting level (0 = block itself, 1 = block within block, etc)
  int enclosingDepth;			// depth to which we're nested within outer method
  GrowableArray<Scope*>* enclosingScopes; // 0 = scope immediately enclosing block (= this->scope), etc.
  methodOop method;			// the method currently being scanned for uplevel-accesses; either
   					// r's block method or a nested block method

  UplevelComputer(BlockPReg* reg) {
    r = reg; scope = r->scope();
    read    = new GrowableArray<PReg*>(10);
    written = new GrowableArray<PReg*>(10);
    nestingLevel = 0;
    method = r->closure()->method();
    enclosingDepth = 0;
    enclosingScopes = new GrowableArray<Scope*>(5);
    for (Scope* s = scope; s != NULL; s = s->parent(), enclosingDepth++) enclosingScopes->push(s);
  }


  void record_temporary (bool reading, int no, int ctx) {
    // distance is the lexical nesting distance in source-level terms (i.e., regardless of what the interpreter
    // does or whether the intermediate scopes have contexts or not) between r's scope and the scope 
    // resolving the access; e.g., 1 --> the scope creating r
    int distance = method->lexicalDistance(ctx) - nestingLevel;
    if (distance < 1) return;				// access is resolved in some nested block
    Scope* s = enclosingScopes->at(distance - 1);	// -1 because 0th element is enclosing scope, i.e., at distance 1
    if (s->isInlinedScope()) {
      // temporary is defined in this nmethod
      InlinedScope* target = (InlinedScope*)s;
      assert(target->allocatesInterpretedContext(), "find_scope returned bad scope");
      PReg* reg = target->contextTemporary(no)->preg();
      if (CompilerDebug) {
	cout(PrintExposed)->print("*adding %s to uplevel-%s of block %s\n", reg->name(), reading ? "read" : "written", r->name());
      }
      GrowableArray<PReg*>* list = reading ? read : written;
      list->append(reg);
    } else {
      // uplevel access goes to another nmethod 
    }
  }

  void push_temporary (int no, int context)		{ record_temporary(true, no, context); }
  void store_temporary(int no, int context)		{ record_temporary(false, no, context); }
  void allocate_closure(AllocationType type, int nofArgs, methodOop meth) {
    // recursively search nested blocks
    nestingLevel++;
    methodOop savedMethod = method;
    method = meth;
    MethodIterator iter(meth, this);
    method = savedMethod;
    nestingLevel--;
  }
 };


void BlockPReg::computeUplevelAccesses() {
  // compute _uplevelRead/_uplevelWritten
  assert(escapes(), "should escape");
  if (_uplevelRead) return;	// already computed
  UplevelComputer c(this);
  MethodIterator iter(_closure->method(), &c);
  assert(! _uplevelWritten, "shouldn't be there");
  _uplevelRead    = c.read;
  _uplevelWritten = c.written;
  for (int i = _uplevelRead->length() - 1; i >= 0; i--) _uplevelRead   ->at(i)->addUplevelAccessor(this, true, false);
  for (i = _uplevelWritten->length() - 1;  i >= 0; i--) _uplevelWritten->at(i)->addUplevelAccessor(this, false, true);
}


char* PReg::safeName() const {
  if (this == NULL) {
    return "(null)";     // for safer debugging
  } else {
    return name();
  }
}

char* PReg::name() const {
  char* n = NEW_RESOURCE_ARRAY(char, 25);
  if (loc.equals(unAllocated)) {
    sprintf(n, "%s%d%s%s%s", prefix(), id(),
	    uplevelR() || uplevelW() ? "^" : "",
	    uplevelR() ? "R" : "", uplevelW() ? "W" : "");
  } else {
    sprintf(n, "%s%d(%s)%s%s%s", prefix(), id(), loc.name(),
	    uplevelR() || uplevelW() ? "^" : "",
	    uplevelR() ? "R" : "", uplevelW() ? "W" : "");
  }
  return n;
}


void PReg::print() {
  lprintf("%s: ", name()); printDefsAndUses(&dus); lprintf("\n");
}


void BlockPReg::print() {
  print_short(); lprintf(": "); 
  printDefsAndUses(&dus);
  if (_uplevelRead) { lprintf("; uplevel-read: "); _uplevelRead->print(); }
  if (_uplevelWritten) { lprintf("; uplevel-written: "); _uplevelWritten->print(); }
  if (_escapeNodes) { 
    lprintf("; escapes at: "); 
    for (int i = 0; i < _escapeNodes->length(); i++) lprintf("N%d ", _escapeNodes->at(i)->id()); 
  }
  lprintf("\n");
}


char* BlockPReg::name() const {
  char* n = NEW_RESOURCE_ARRAY(char, 25);
  sprintf(n, "%s <%#lx>%s", PReg::name(), PrintHexAddresses ? this : 0, _memoized ? "#" : "");
  return n;
}


char* ConstPReg::name() const {
  char* n = NEW_RESOURCE_ARRAY(char, 25);
  sprintf(n, "%s <%#lx>", PReg::name(), constant);
  return n;
}


#ifdef not_yet_used
  char* SplitPReg::name() const {
    char* n = NEW_RESOURCE_ARRAY(char, 25);
//c    char buf[MaxSplitDepth+1];
//c    sprintf(n, "%s <%s>", PReg::name(), sig->prefix(buf));
    sprintf(n, "%s", PReg::name());
    return n;
  }
#endif


bool PReg::verify() const {
  bool ok = true;
  if (_id < 0 || _id >= currentNo) {
    ok = false;
    error("PReg %#lx %s: invalid ID %ld", this, name(), _id);
  }
  int uses = 0, defs = 0;
  for (int i = 0; i < dus.length(); i++) {
    PRegBBIndex* index = dus.at(i);
    DUInfo* info = index->bb->duInfo.info->at(index->index);
    defs += info->defs.length();
    uses += info->uses.length();
  }
  if (defs != _ndefs && !incorrectD() && !isConstPReg()) {
    // ConstPRegs have fake def
    ok = false;
    error("PReg %#lx %s: wrong def count (%ld instead of %ld)",
	  this, name(), _ndefs, defs);
  }
  if (uses != _nuses && !incorrectU()) {
    ok = false;
    error("PReg %#lx %s: wrong use count (%ld instead of %ld)",
	  this, name(), _nuses, uses);
  }
  if (!incorrectD() && _ndefs == 0 && _nuses > 0) {
    ok = false;
    error("PReg %#lx %s: used but not defined", this, name());
  }
# ifdef fixthis  // fix this - may still be needed
  if (debug && !incorrectDU() && isTrashedReg(loc)) {
    ok = false;
    error("PReg %#lx %s: debug-visible but allocated to temp reg", this, name());
  }
# endif
  return ok;
}


bool SAPReg::verify() const {
  bool ok = PReg::verify();
  if (ok) {
    if (_begBCI == IllegalBCI) {
      if (creationStartBCI != IllegalBCI || _endBCI != IllegalBCI) {
	ok = false;
	error("SAPReg %#lx %s: live range only partially set", this, name());
      }
    } else if (_scope->isInlinedScope()) {
      int ncodes = scope()->nofBytes();
      if (creationStartBCI < PrologueBCI ||
	  creationStartBCI > creationScope()->nofBytes()) {
	ok = false;
	error("SAPReg %#lx %s: invalid creationStartBCI %ld", this, name(), creationStartBCI);
      }
      if (_begBCI < PrologueBCI || _begBCI > ncodes) {
	ok = false;
	error("SAPReg %#lx %s: invalid startBCI %ld", this, name(), _begBCI);
      }
      if (_endBCI < PrologueBCI || _endBCI > ncodes && _endBCI != EpilogueBCI) {
	ok = false;
	error("SAPReg %#lx %s: invalid endBCI %ld", this, name(), _endBCI);
      }
    }
  }
  return ok;
}


bool BlockPReg::verify() const {
  bool ok = SAPReg::verify() && _closure->verify();
  // check uplevel-accessed vars: if they are blocks, they must be exposed
  if (_uplevelRead) {
    for (int i = 0; i < _uplevelRead->length(); i++) {
      if (_uplevelRead->at(i)->isBlockPReg()) {
	BlockPReg* blk = (BlockPReg*)_uplevelRead->at(i);
	if (!blk->escapes()) {
	  error("BlockPReg %#lx is uplevel-accessed by escaping BlockPReg %#lx but isn't marked escaping itself",
	         blk, this);
	  ok = false;
	}
      }
    }
    for (i = 0; i < _uplevelWritten->length(); i++) {
      if (_uplevelWritten->at(i)->isBlockPReg()) {
	BlockPReg* blk = (BlockPReg*)_uplevelRead->at(i);
	error("BlockPReg %#lx is uplevel-written by escaping BlockPReg %#lx, but BlockPRegs should never be assigned",
	       blk, this);
	ok = false;
      }
    }
  }
  return ok;
}


bool NoPReg::verify() const { 
  if (_nuses != 0) {
    error("NoPReg %#lx: has uses", this);
    return false;
  } else {
    return true;
  } 
}


bool ConstPReg::verify() const {
  bool ok = PReg::verify() && constant->is_klass() || constant->verify();
  /*
  if (int(constant) < maxImmediate && int(constant) > -maxImmediate
    && loc != UnAllocated) {
    error("ConstPReg %#lx: could use load immediate to load oop %#lx",
	   this, constant);
    ok = false;
  }
  */
  if (!(loc.equals(unAllocated)) && !loc.isRegisterLocation()) {
    error("ConstPReg %#lx: was allocated to stack", this);
    ok = false;
  }
  if (!(loc.equals(unAllocated)) && loc.isTrashedRegister()) {
    error("ConstPReg %#lx: was allocated to trashed reg", this);
    ok = false;
  }
  return ok;
}


# endif
