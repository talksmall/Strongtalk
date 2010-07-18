/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.13 $ */
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
# include "incls/_loopOpt.cpp.incl"

GrowableArray<BB*>* CompiledLoop::_bbs;

CompiledLoop::CompiledLoop() {
  _bbs = NULL;
  _startOfLoop = _endOfLoop = _startOfBody = _endOfBody = _startOfCond = _endOfCond = NULL;
  _loopVar = NULL;
  _lowerBound = NULL; _upperBound = NULL;
  _isIntegerLoop = false;
  _hoistableTests = NULL;
  _loopBranch = NULL;
  _isCountingUp = true;	  // initial guess
  _scope = NULL; // in case loop creationg is aborted
}


void CompiledLoop::set_startOfLoop(LoopHeaderNode* current) {
  assert(_startOfLoop == NULL, "already set");
  assert(current->isLoopHeaderNode(), "expected loop header");
  _startOfLoop = _loopHeader = current;
  _beforeLoop = current->firstPrev();
}


void CompiledLoop::set_endOfLoop(Node* end) {
  assert(_endOfLoop == NULL, "already set");
  _endOfLoop = end;
  _startOfLoop = _startOfLoop->next();	  // because original start is node *before* loop
  _firstNodeID = _startOfLoop->id() + 1;
  _lastNodeID = BasicNode::currentID - 1;  // -1 because currentID is id of next node to be allocated
  _scope = _startOfLoop->scope();
}


void CompiledLoop::set_startOfBody(Node* current) {
  assert(_startOfBody == NULL, "already set");
  _startOfBody = current;
}


void CompiledLoop::set_endOfBody(Node* current) {
  assert(_endOfBody == NULL, "already set");
  _endOfBody = current;
  // correct startOfBody -- merge node is created before cond
  if (isInLoopCond(_startOfBody)) _startOfBody = _startOfBody->next();
  assert(!isInLoopCond(_startOfBody), "oops!");
}


void CompiledLoop::set_startOfCond(Node* current) {
  assert(_startOfCond == NULL, "already set");
  _startOfCond = current;
}


void CompiledLoop::set_endOfCond(Node* current) {
  assert(_endOfCond == NULL, "already set");
  _endOfCond = current;
}


bool CompiledLoop::isInLoop(Node* n) const {
  return _firstNodeID <= n->id() && n->id() <= _lastNodeID;
}


bool CompiledLoop::isInLoopCond(Node* n) const {
  return _startOfCond->id() <= n->id() && n->id() <= _endOfCond->id();
}


bool CompiledLoop::isInLoopBody(Node* n) const {
  return _startOfBody->id() <= n->id() && n->id() <= _endOfBody->id();
}


char* CompiledLoop::recognize() {
  // Recognize integer loop.
  // We're looking for a loop where the loopVariable is initialized just before
  // the loop starts, is defined only once in the loop (increment/decrement),
  // and the loop condition is a comparison against a loop invariant.
  discoverLoopNesting();
  if (!OptimizeIntegerLoops) return "!OptimizeIntegerLoops";
  char* msg;
  if ((msg = findUpperBound()) != NULL) return msg;
  if ((msg = checkLoopVar()) != NULL) return msg;
  if ((msg = checkUpperBound()) != NULL) return msg;
  _isIntegerLoop = true;
  findLowerBound();	// don't need to know to optimize loop; result may be non-NULL
  return NULL;
}

void CompiledLoop::discoverLoopNesting() {
  // discover enclosing loop (if any) and set up loop header links
  for (InlinedScope* s = _scope; s != NULL; s = s->sender()) {
    GrowableArray<CompiledLoop*>* loops = s->loops();
    for (int i = loops->length() - 1; i >= 0; i--) {
      CompiledLoop* l = loops->at(i);
      if (l->isInLoop(_loopHeader)) {
	// this is out enclosing loop
	_loopHeader->set_enclosingLoop(l->loopHeader());
	l->loopHeader()->addNestedLoop(_loopHeader);
	return;
      }
    }
  }
}


char* CompiledLoop::findLowerBound() {
  // find lower bound; is assigned to loop var (temp) before loop
  // NB: throughout this code, "lower" and "upper" bound are used to denote the 
  // starting and ending values of the loop; in the case of a downward-counting loop,
  // lowerBound will actually be the highest value.
  // search for assignment to loopVar that reaches loop head
  Node* defNode;
  for (defNode = _beforeLoop; 
       defNode && defNode->nPredecessors() < 2 && 
	 (!defNode->hasDest() || ((NonTrivialNode*)defNode)->dest() != _loopVar); 
       defNode = defNode->firstPrev()) ;
  if (defNode && defNode->isAssignNode()) {
    // ok, loopVar is assigned here
    _lowerBound = ((NonTrivialNode*)defNode)->src();
  }
  return "lower bound not found";
}


// Helper class to find the last send preceding a certain bci
class SendFinder: public SpecializedMethodClosure {
 public:
  int theBCI, lastSendBCI;

  SendFinder(methodOop m, int bci) : SpecializedMethodClosure() {
    theBCI = bci; lastSendBCI = IllegalBCI;
    MethodIterator iter(m, this);
  }
  void send() { 
    int b = bci();
    if (b <= theBCI && b > lastSendBCI) lastSendBCI = b; 
  }
  virtual void normal_send(InterpretedIC* ic) { send(); }
  virtual void self_send  (InterpretedIC* ic) { send(); }
  virtual void super_send (InterpretedIC* ic) { send(); }
};


int CompiledLoop::findStartOfSend(int bci) {
  // find send preceding bci; slow but safe
  SendFinder f(_scope->method(), bci);
  return f.lastSendBCI;
}


char* CompiledLoop::findUpperBound() {
  // find upper bound and loop variable
  int condBCI = findStartOfSend(_endOfCond->bci() - InterpretedIC::size);
  if (condBCI == IllegalBCI) return "loop condition: no send found";
  // first find comparison in loop condition
  Expr* loopCond = _scope->exprStackElems()->at(condBCI);
  if (loopCond == NULL) return "loop condition: no expr (possible compiler bug)";
  if (loopCond->isMergeExpr()) {
    MergeExpr* cond = (MergeExpr*)loopCond;
    if (cond->isSplittable()) {
      Node* first = cond->exprs->first()->node();
      if (first->nPredecessors() == 1) {
        Node* prev = first->firstPrev();
        if (prev->isBranchNode()) _loopBranch = (BranchNode*)prev;
      }
    }
  }
  if (!_loopBranch) return "loop condition: conditional branch not found";
  NonTrivialNode* n = (NonTrivialNode*)_loopBranch->firstPrev();
  PReg* operand;
  if (n->isTArithNode()) {
    operand = ((TArithRRNode*)n)->operand();
  } else if (n->isArithNode()) {
    operand = ((ArithRRNode*)n)->operand();
  } else {
    return "loop condition: comparison not found";
  }
  BranchOpCode op = _loopBranch->op();

  // Now see if it's a simple comparison involving the loop variable
  // and make an initial guess about the loop variable.
  // NB: code generator inverts loop condition, i.e., branch leaves loop
  if (op == LTBranchOp || op == LEBranchOp) {
    // upperBound > loopVar
    _loopVar = operand;
    _upperBound = n->src();
  } else if (op == GTBranchOp || op == GEBranchOp) {
    // loopVar < upperBound
    _loopVar = n->src();
    _upperBound = operand;
  } else {
    return "loop condition: not oneof(<, <=, >, >=)";
  }

  // The above code assumes a loop counting up; check the loopVar now and switch
  // directions if it doesn't work.
  if (checkLoopVar()) {
    // the reverse guess may be wrong too, but it doesn't hurt to try
    PReg* temp = _loopVar; _loopVar = _upperBound; _upperBound = temp;
  }
  return NULL;
}


// closure for finding defs in a loop
class LoopClosure : public Closure<Def*> {
public:
  NonTrivialNode* defNode;
  CompiledLoop* theLoop;

  LoopClosure(CompiledLoop* l) { defNode = NULL; theLoop = l; }
};


// count all defs in loop
class LoopDefCounter : public LoopClosure {
public:
  int defCount;
  LoopDefCounter(CompiledLoop* l) : LoopClosure(l) { defCount = 0; }

  void do_it(Def* d) { 
    if (theLoop->isInLoop(d->node)) {
      defCount++;
      defNode = (NonTrivialNode*)d->node;
    } 
  }
};


int CompiledLoop::defsInLoop(PReg* r, NonTrivialNode** defNode) {
  // returns the number of definitions of r within the loop
  // also sets defNode to the last definition
  // BUG: won't work if loop has sends -- will ignore possible defs to inst vars etc.
  LoopDefCounter ldc(this);
  r->forAllDefsDo(&ldc);
  if (defNode) *defNode = ldc.defNode;
  return ldc.defCount;
}


class LoopDefFinder : public LoopClosure {
public:
  LoopDefFinder(CompiledLoop* l) : LoopClosure(l) {}	  // don't ask why this is necessary... (MS C++ 4.0)
  void do_it(Def* d) { if (theLoop->isInLoop(d->node)) defNode = d->node; }
};


NonTrivialNode* CompiledLoop::findDefInLoop(PReg* r) {
  assert(defsInLoop(r) == 1, "must have single definition in loop");
  LoopDefFinder ldf(this);
  r->forAllDefsDo(&ldf);
  return ldf.defNode;
}


char* CompiledLoop::checkLoopVar() {
#ifdef unnecessary
  // first check if loop var is initialized to lower bound
  BB* beforeLoopBB = _beforeLoop->bb();
  // search for assignment preceeding loop
  for (Node* n = _beforeLoop; 
       n && n->bb() == beforeLoopBB && (!n->isAssignNode() || ((AssignNode*)n)->dest() != loopVar); 
       n = n->firstPrev()) ;
  if (n && n->bb() == beforeLoopBB) {
    // ok, found the assignment
    assert(n->isAssignNode() && ((AssignNode*)n)->dest() == loopVar, "oops");
  } else {
    return "no lower-bound assignment to loop var before loop";
  }
#endif
  // check usage of loop var within loop

  // quick check: must have at least 2 defs (initialization plus increment)
  if (_loopVar->isSinglyAssigned()) return "loopVar has single definition globally";

  // loop variable must have exactly one def in loop, and def must be result of an addition
  // usual code pattern: SAPn = add(loopVar, const); ...; loopVar := SAPn
  if (defsInLoop(_loopVar) != 1) return "loopVar has != 1 defs in loop";
  NonTrivialNode* n1 = findDefInLoop(_loopVar);
  if (defsInLoop(n1->src()) != 1) return "loopVar has != 1 defs in loop (2)";
  NonTrivialNode* n2 = findDefInLoop(n1->src());
  PReg* operand;
  ArithOpCode op;
  if (n2->isTArithNode()) {
    operand = ((TArithRRNode*)n2)->operand();
    op = ((TArithRRNode*)n2)->op();
  } else if (n2->isArithNode()) {
    operand = ((ArithRRNode*)n2)->operand();
    op = ((ArithRRNode*)n2)->op();
  } else {
    return "loopVar def not an arithmetic operation";
  }
  _incNode = n2;
  if (op != tAddArithOp && op != tSubArithOp) return "loopVar def isn't an add/sub";
  if (_incNode->src() == _loopVar) {
    if (!isIncrement(operand, op)) return "loopVar isn't incremented by constant/loop-invariant";
  } else if (operand == _loopVar) {
    if (!isIncrement(_incNode->src(), op)) return "loopVar isn't incremented by constant/loop-invariant";
  } else {
    return "loopVar def isn't an increment";
  }

  // at this point, we finally know for sure whether the loop is counting up or down
  // check that loop is bounded at all 
  BranchOpCode branchOp = _loopBranch->op();
  bool loopVarMustBeLeft = (branchOp == GTBranchOp || branchOp == GEBranchOp) ^ !_isCountingUp;
  NonTrivialNode* compare = (NonTrivialNode*)_loopBranch->firstPrev();
  if (loopVarMustBeLeft != (compare->src() == _loopVar)) {
    return "loopVar is on wrong side of comparison (loop not bounded)";
  }

  return NULL;
}


bool CompiledLoop::isIncrement(PReg* p, ArithOpCode op) {
  // is p a suitable increment (i.e., a positive constant or loop-invariant variable)?
  _increment = p;
  if (p->isConstPReg()) {
    oop c = ((ConstPReg*)p)->constant;
    if (!c->is_smi()) return false;
    _isCountingUp = (smiOop(c)->value() > 0) ^ (op == tSubArithOp);
    return true;
  } else {
    // fix this: need to check sign in loop header
    return defsInLoop(p) == 0;
  }
}


char* CompiledLoop::checkUpperBound() {
  // upper bound must not be defined in loop (loop invariant)
  _loopArray = NULL;
  _loopSizeLoad = NULL;
  int ndefs = defsInLoop(_upperBound, NULL);
  if (ndefs > 0) return "upper bound isn't loop-invariant";
  // ok, no assignments in loop; check if upper bound is size of an array
  // search for assignment that reaches loop head
  Node* defNode;
  for (defNode = _beforeLoop; 
       defNode && defNode->nPredecessors() < 2 && 
	 (!defNode->hasDest() || ((NonTrivialNode*)defNode)->dest() != _upperBound); 
       defNode = defNode->firstPrev()) ;
  if (defNode && defNode->isArraySizeLoad()) {
    // ok, upper bound is an array; can optimize array accesses if it is loop-invariant
    if (defsInLoop(_loopArray) == 0) {
      _loopSizeLoad = (LoadOffsetNode*)defNode;
      _loopArray = _loopSizeLoad->base();
    }
  }
  return NULL;
}


void CompiledLoop::optimizeIntegerLoop() {
  // need general loop opts as well (for array type checks)
  if (!OptimizeLoops) fatal("if OptimizeIntegerLoops is set, OptimizeLoops must be set too");
  if (!isIntegerLoop()) return; 
  // activate loop header (will check upper bound for smi-ness etc.)
  PReg* u = _loopSizeLoad ? NULL : _upperBound;
  _loopHeader->activate(_loopVar, _lowerBound, u, _loopSizeLoad);
  _loopHeader->_integerLoop = true;
  removeTagChecks();
  checkForArraysDefinedInLoop();
  if (_loopArray) {
    // upper bound is loopArray's size --> counter can't overflow
    removeLoopVarOverflow();
    // also, array accesses need no bounds check
    removeBoundsChecks(_loopArray, _loopVar);
    // potential performance bug: should do bounds check for all array accesses with loop-invariant index
  } else {
    // fix this: must check upper bound against maxInt
    removeLoopVarOverflow();
  }
}


// closure for untagging defs/uses of loop var
class UntagClosure : public Closure<Use*> {
public:
  CompiledLoop* theLoop;
  PReg* theLoopPReg;
  GrowableArray<klassOop>* smi_type;

  UntagClosure(CompiledLoop* l, PReg* r) { 
    theLoop = l; theLoopPReg = r; smi_type = new GrowableArray<klassOop>(1); smi_type->append(smiKlassObj); 
  }
  void do_it(Use* u) {
    if (theLoop->isInLoop(u->node)) {
      u->node->assert_preg_type(theLoopPReg, smi_type, theLoop->loopHeader());
    }
  }
};


void CompiledLoop::removeTagChecks() {
  // Eliminate all tag checks for loop variable and upper bound
  // As a side effect, this will add all arrays indexed by the loop variable to the LoopHeaderNode
  UntagClosure uc(this, _loopVar);
  _loopVar->forAllUsesDo(&uc);
  if (_lowerBound) {
    uc.theLoopPReg = _lowerBound;
    _lowerBound->forAllUsesDo(&uc);
  }
  uc.theLoopPReg = _upperBound;
  _upperBound->forAllUsesDo(&uc);
}


void CompiledLoop::removeLoopVarOverflow() {
  // bug: should remove overflow only if increment is constant and not too large -- fix this
  Node* n = _incNode->next();
  assert(n->isBranchNode(), "must be branch");
  BranchNode* overflowCheck = (BranchNode*)n;
  assert(overflowCheck->op() == VSBranchOp, "should be overflow check");
  if (CompilerDebug || PrintLoopOpts) {
    cout(PrintLoopOpts)->print("*removing overflow check at node N%d\n", overflowCheck->id());
  }
  Node* taken = overflowCheck->next(1);	  // overflow handling code
  taken->removeUpToMerge();
  overflowCheck->removeNext(taken);	  // so overflowCheck can be eliminated
  overflowCheck->eliminate(overflowCheck->bb(), NULL, true, false);

  // since increment cannot fail anymore, directly increment loop counter if possible
  // need to search for assignment of incremented value to loop var
  Node* a = overflowCheck->next();
  while (1) {
    if (a->nPredecessors() > 1) break;	  // can't search beyond merge
    if (a->nSuccessors()   > 1) break;	  // can't search beyond branches
    if (a->isAssignNode()) {
      if (!a->deleted) {
	AssignNode* assign = (AssignNode*)a;
	if (assign->src() == _incNode->dest() && assign->dest() == _loopVar) {
	  if (CompilerDebug || PrintLoopOpts) {
	    cout(PrintLoopOpts)->print("*optimizing loopVar increment at N%d\n", _incNode->id());
	  }
	  _incNode->setDest(_incNode->bb(), _loopVar);
	  assign->eliminate(assign->bb(), NULL, true, false);
	}
      }
    } else if (!a->isTrivial()) {
      compiler_warning("unexpected nontrivial node N%d after loopVar increment\n", a->id());
    }
    a = a->next();
  }
}


void CompiledLoop::checkForArraysDefinedInLoop() {
  // remove all arrays from loopHeader's list which are defined in the loop
  GrowableArray<AbstractArrayAtNode*> arraysToRemove(10);
  int len = _loopHeader->_arrayAccesses->length();
  for (int i = 0; i < len; i++) {
    AbstractArrayAtNode* n = _loopHeader->_arrayAccesses->at(i);
    if (defsInLoop(n->src())) arraysToRemove.append(n);
  }
  while (arraysToRemove.nonEmpty()) {
    AbstractArrayAtNode* n = arraysToRemove.pop();
    _loopHeader->_arrayAccesses->remove(n);
  }
}


void CompiledLoop::optimize() {
  // general loop optimizations
  hoistTypeTests();
  findRegCandidates();
}


class TTHoister : public Closure<InlinedScope*> {
 public:
  GrowableArray<HoistedTypeTest*>* hoistableTests;
  CompiledLoop* theLoop;

  TTHoister(CompiledLoop* l, GrowableArray<HoistedTypeTest*>* h) { hoistableTests = h; theLoop = l; }

  void do_it(InlinedScope* s) {
    GrowableArray<NonTrivialNode*>* tests = s->typeTests();
    int len = tests->length();
    for (int i = 0; i < len; i++) {
      NonTrivialNode* n = tests->at(i);
      assert(n->doesTypeTests(), "shouldn't be in list");
      if (n->deleted) continue;
      if (n->hasUnknownCode()) continue;	  // can't optimize - expects other klasses, so would get uncommon trap at run-time
      if (!theLoop->isInLoop(n)) continue;	  // not in this loop
      GrowableArray<PReg*> regs(4);
      GrowableArray<GrowableArray<klassOop>*> klasses(4);
      n->collectTypeTests(regs, klasses);
      for (int j = 0; j < regs.length(); j++) {
	PReg* r = regs.at(j);
	if (theLoop->defsInLoop(r) == 0) {
	  // this test can be hoisted
	  if (CompilerDebug || PrintLoopOpts) cout(PrintLoopOpts)->print("*moving type test of %s at N%d out of loop\n", r->name(), n->id());
	  hoistableTests->append(new HoistedTypeTest(n, r, klasses.at(j)));
	}
      }
    }
  }
};


void CompiledLoop::hoistTypeTests() {
  // collect all type tests that can be hoisted out of the loop
  _loopHeader->_tests = _hoistableTests = new GrowableArray<HoistedTypeTest*>(10);
  TTHoister tth(this, _hoistableTests);
  _scope->subScopesDo(&tth);

  // add type tests to loop header for testing (avoid duplicates)
  // (currently quadratic algorithm but there should be very few)
  GrowableArray<HoistedTypeTest*>* headerTests = new GrowableArray<HoistedTypeTest*>(_hoistableTests->length());
  for (int i = _hoistableTests->length() - 1; i >= 0; i--) {
    HoistedTypeTest* t = _hoistableTests->at(i);
    PReg* tested = t->testedPR;
    for (int j = headerTests->length() - 1; j >= 0; j--) {
      if (headerTests->at(j)->testedPR == tested) {
	// already testing this PReg
	if (isEquivalentType(headerTests->at(j)->klasses, t->klasses)) {
	  // nothing to do
	} else {
	  // Whoa!  The same PReg is tested for different types in different places.
	  // Possible but rare.
	  headerTests->at(j)->invalid = t->invalid = true;
	  if (WizardMode) {
	    compiler_warning("CompiledLoop::hoistTypeTests: PReg tested for different types\n");
	    t->print();
	    headerTests->at(j)->print();
	  }
	}
	tested = NULL;    // don't add it to list
	break;
      }
    }
    if (tested) headerTests->append(t);
  }

  // now delete all hoisted type tests from loop body
  for (i = _hoistableTests->length() - 1; i >= 0; i--) {
    HoistedTypeTest* t = _hoistableTests->at(i);
    if (!t->invalid) {
      t->node->assert_preg_type(t->testedPR, t->klasses, _loopHeader);
    }
  }
  if (!_loopHeader->isActivated()) _loopHeader->activate();
}

bool CompiledLoop::isEquivalentType(GrowableArray<klassOop>* klasses1, GrowableArray<klassOop>* klasses2) {
  // are the two lists klasses1 and klasses2 equivalent (i.e., contain the same set of klasses)?
  if (klasses1->length() != klasses2->length()) return false;
  for (int i = klasses2->length() - 1; i >= 0; i--) {
    if (klasses1->at(i) != klasses2->at(i)    // quick check
        && (!klasses1->contains(klasses2->at(i)) || !klasses2->contains(klasses1->at(i))) ) { // slow check
      return false;
    }
  }
  return true;
}


class BoundsCheckRemover : public Closure<Use*> {
public:
  CompiledLoop* theLoop;
  PReg* theLoopPReg;
  GrowableArray<AbstractArrayAtNode*>* theArrayList;

  BoundsCheckRemover(CompiledLoop* l, PReg* r, GrowableArray<AbstractArrayAtNode*>* arrays) {
    theLoop = l; theLoopPReg = r; theArrayList = arrays;
  }
  void do_it(Use* u) {
    if (theLoop->isInLoop(u->node) &&
	// the cast to AbstractArrayAtNode* isn't correct (u->node may be something else),
	// but it's safe since we're only searching in the array list using pointer identity
	theArrayList->contains((AbstractArrayAtNode*)u->node)) {
      u->node->assert_in_bounds(theLoopPReg, theLoop->loopHeader());
    }
  }
};


void CompiledLoop::removeBoundsChecks(PReg* array, PReg* var) {
  // Remove all bounds checks for nodes where var indexes into array.
  // This means that the arrays must be type- and bounds-checked in the loop header.
  // Thus, only do it for arrays that aren't defined within the loop, i.e.,
  // those in the loop header's list of arrays.
  BoundsCheckRemover bcr(this, var, _loopHeader->_arrayAccesses);
  array->forAllUsesDo(&bcr);
}

void CompiledLoop::findRegCandidates() {
  // Find the PRegs with the highest number of defs & uses in this loop.
  // Problem: we don't have a list of all PRegs used in the loop; in fact, we don't even have
  // a list of all nodes.
  // Solution: iterate through all BBs between start and end of loop (in code generation order)
  // and collect the defs and uses.  The BB ordering algorithm should make sure that the BBs
  // of the loop are consecutive.
  if (_bbs == NULL) _bbs = bbIterator->code_generation_order();
  GrowableArray<LoopRegCandidate*> candidates(PReg::currentNo, PReg::currentNo, NULL);
  const int len = _bbs->length();
  const BB* startBB = _startOfLoop->bb();
  int i;
  for (i = 0; _bbs->at(i) != startBB; i++) ;	// search for first BB
  const BB* endBB = _endOfLoop->bb();
  int ncalls = 0;

  // iterate through all BBs in the loop
  for (BB* bb = _bbs->at(i); bb != endBB; i++, bb = _bbs->at(i)) {
    const int n = bb->duInfo.info->length();
    if (bb->last->isCallNode()) ncalls++;
    for (int j = 0; j < n; j++) {
      DUInfo* info = bb->duInfo.info->at(j);
      PReg* r = info->reg;
      if (candidates.at(r->id()) == NULL) candidates.at_put(r->id(), new LoopRegCandidate(r));
      LoopRegCandidate* c = candidates.at(r->id());
      c->incDUs(info->uses.length(), info->defs.length());
    }
  }
  loopHeader()->set_nofCallsInLoop(ncalls);

  // find the top 2 candidates...
  LoopRegCandidate* first  = new LoopRegCandidate(NULL);
  LoopRegCandidate* second = new LoopRegCandidate(NULL);
  for (int j = candidates.length() - 1; j >= 0; j--) {
    LoopRegCandidate* c = candidates.at(j);
    if (c == NULL) continue;
    if (c->weight() > first->weight()) {
      second = first; first = c;
    } else if (c->weight() > second->weight()) {
      second = c;
    }
  }

  // ...and add them to the loop header 
  if (first->preg() != NULL) {
    loopHeader()->addRegisterCandidate(first);
    if (second->preg() != NULL) {
      loopHeader()->addRegisterCandidate(second);
    }
  } else {
    assert(second->preg() == NULL, "bad sorting");
  }
}

void CompiledLoop::print() {
  std->print("((CompiledLoop*)%#x) = [N%d..N%d], cond = [N%d..N%d], body = [N%d..N%d] (bci %d..%d)\n", 
          this, _firstNodeID, _lastNodeID, _startOfCond->id(), _endOfCond->id(), 
	  _startOfBody->id(), _endOfBody->id(),
	  _startOfLoop->bci(), _endOfLoop->bci());
  std->print("\tloopVar=%s, lower=%s, upper=%s\n", _loopVar->safeName(), _lowerBound->safeName(), _upperBound->safeName());
}


HoistedTypeTest::HoistedTypeTest(NonTrivialNode* node, PReg* testedPR, GrowableArray<klassOop>* klasses) {
  this->node = node; this->testedPR = testedPR; this->klasses = klasses; invalid = false;
}


void HoistedTypeTest::print_test_on(outputStream* s) {
  s->print("%s = {", testedPR->name());
  int len = klasses->length();
  for (int j = 0; j < len; j++) {
    klassOop m = klasses->at(j);
    m->print_value_on(s);
    if (j < len - 1) s->print(",");
  }
  s->print("}");
}


void HoistedTypeTest::print() {
  std->print("((HoistedTypeTest*)%#x): ", this);
  print_test_on(std);
  std->cr();
}

void LoopRegCandidate::print() {
  std->print("((LoopRegCandidate*)%#x): %s, %d uses, %d defs\n", this, _preg->name(), _nuses, _ndefs);
}

# endif
