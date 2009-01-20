/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.118 $ */
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
# include "incls/_scope.cpp.incl"
# include <string.h>

smi Scope::_currentScopeID;

// SendInfo implementation

SendInfo::SendInfo(InlinedScope* sen, LookupKey* lkup, Expr* r) {
  senderScope = sen;
  rcvr = r; 
  sel = lkup->selector(); 
  key = lkup;
  init();
}


void SendInfo::computeNSends(RScope* rscope, int bci) {
  GrowableArray<RScope*>* lst = rscope->subScopes(bci);
  nsends = 0;
  for (int i = lst->length() - 1; i >= 0; i--) {
    nsends += lst->at(i)->nsends;
  }
}

void SendInfo::init() {
  resReg = NULL; needRealSend = counting = false; 
  nsends = -1; receiverStatic = predicted = uninlinable = false;
  inPrimFailure = senderScope && senderScope->gen()->in_prim_failure_block();
}

// Scopes
// NB: constructors are protected to avoid stupid "call-of-virtual-in-constructor" bugs

InlinedScope::InlinedScope() {}

void InlinedScope::initialize(methodOop method, klassOop methodHolder, InlinedScope* sender, RScope* rs, SendInfo* info) {
  _scopeID 		= currentScopeID();
  theCompiler->scopes->append(this);
  assert(theCompiler->scopes->at(_scopeID) == this, "bad list");
  _sender 		= sender;
  _scopeInfo 		= NULL;
  if (sender) {
    _senderBCI 		= sender->bci();
    sender->addSubScope(this);
    depth 		= _sender->depth + 1;
    loopDepth 		= _sender->loopDepth;
  } else {
    _senderBCI 		= IllegalBCI;
    depth = loopDepth 	= 0;
  }
  result = nlrResult 	= NULL;	// these are set during compilation
  if (info && info->resReg) {
    resultPR 		= info->resReg;
  } else {
    // potential bug: live range of resultPR is bogus
    assert(isTop(), "should have resReg for inlined scope");
    resultPR 		= new SAPReg(this, resultLoc, false, false, PrologueBCI, EpilogueBCI);
  }
  rscope 		= rs;
  rs->extend();
      
  predicted 		= info ? info->predicted : false;

  assert(info->key->klass(), "must have klass");
  _key			= info->key;	
  _method		= method;
  _methodHolder		= methodHolder;	// NB: can be NULL if method is in Object
  _nofSends		= 0;
  _nofInterruptPoints	= 0;
  _primFailure		= sender ? sender->_primFailure : false;
  _endsDead		= false;
  _self			= NULL;		// initialized by createTemps or by sender scope
  _gen.initialize(this);

  _temporaries		= NULL;		// allocated by createTemporaries
  _floatTemporaries	= NULL;		// allocated by createFloatTemporaries
  _contextTemporaries	= NULL;		// allocated by createContextTemporaries
  _context		= NULL;		// set for blocks and used/set by createContextTemporaries
  _exprStackElems	= new GrowableArray<Expr*>(nofBytes());
  _subScopes		= new GrowableArray<InlinedScope*>(5);
  _loops		= new GrowableArray<CompiledLoop*>(5);
  _typeTests		= new GrowableArray<NonTrivialNode*>(10);

  _pregsBegSorted	= new GrowableArray<PReg*>(5);
  _pregsEndSorted	= new GrowableArray<PReg*>(5);
  _firstFloatIndex	= -1;		// set during float allocation
  _hasBeenGenerated     = false;

  theCompiler->nofBytesCompiled(nofBytes());
  if (!rs->isNullScope() && rs->method() != method) {
    // wrong rscope (could happen after programming change)
    rs = new RNullScope;
  }
}

bool InlinedScope::isLite() const {
  // A scope is light (doesn't need its locals/expression stack described) if it has no interrupt
  // points, i.e., if the program can never stop while the PC is in this scope.
  // The top scope of an nmethod can't be light (at least the receiver is needed).
  return GenerateLiteScopeDescs && (sender() != NULL) && (_nofInterruptPoints == 0);
}

void MethodScope::initialize(methodOop method, klassOop methodHolder, InlinedScope* sen, RScope* rs, SendInfo* info) {
  InlinedScope::initialize(method, methodHolder, sen, rs, info);
}


MethodScope::MethodScope() {}
BlockScope::BlockScope() {}


void BlockScope::initialize(methodOop method, klassOop methodHolder, Scope* p, InlinedScope* s, RScope* rs, SendInfo* info) {
  InlinedScope::initialize(method, methodHolder, s, rs, info);
  _parent = p; 
  _self_is_initialized = false;
  if (s == NULL) {
    // top scope: create a context (currently always initialized for blocks)
    // (context is set up by the prologue node)
    _context = new SAPReg(this, PrologueBCI, EpilogueBCI);
  } else {
    // set up for context passed in by caller
    // (_context may be changed later if this scope allocates its own context)
    switch (method->block_info()) {
      case methodOopDesc::expects_nil:		// no context needed
   	_context = NULL; break;
      case methodOopDesc::expects_self:
   	_context = self()->preg(); fatal("self not known yet -- fix this"); break;
      case methodOopDesc::expects_parameter:	// fix this -- should find which
	Unimplemented();
	break;
      case methodOopDesc::expects_context:
   	if (p->isInlinedScope()) {
	  _context = ((InlinedScope*)p)->context(); 
	} else {
	  fatal("shouldn't inline");  	// shouldn't inline block unless parent was inlined, too
	}
	break;
      default:
   	fatal("unexpected incoming info");
    }
  }
}


MethodScope* MethodScope::new_MethodScope(methodOop method, klassOop methodHolder, InlinedScope* sen, RScope* rs, SendInfo* info) {
  MethodScope* new_scope = new MethodScope;
  new_scope->initialize(method, methodHolder, sen, rs, info);
  new_scope->initializeArguments();
  return new_scope;
}	   


BlockScope* BlockScope::new_BlockScope(methodOop method, klassOop methodHolder, Scope* p, InlinedScope* s, RScope* rs, SendInfo* info) {
  BlockScope* new_scope = new BlockScope;
  new_scope->initialize(method, methodHolder, p, s, rs, info);
  new_scope->initializeArguments();
  return new_scope;
}	   


void InlinedScope::addSubScope(InlinedScope* s) {
  // assert(_subScopes->isEmpty() || _subScopes->last()->senderBCI() <= s->senderBCI(),
  //	 "scopes not ordered by bci");
  // NB: subScopes are not in bci order when generating while loops -- condition is generated
  // first but has higher bcis than body.  Ugh.   -Urs 10/95
  _subScopes->append(s);
}

void InlinedScope::subScopesDo(Closure<InlinedScope*>* f) {
  f->do_it(this);
  _subScopes->apply(f);
}

MergeNode* InlinedScope::nlrTestPoint() {
  /* Generate code handling NLRs coming from below.
     There are several factors influencing what happens:

     1. NLR-Target:  A scope is a potential NLR target if the scope is a method scope and
        there is at least one block with a NLR in the scope.
	NLR targets must test the target of the current NLR and do a normal method return 
	if the NLR comes from one of their blocks.
	
     2. Context: does this scope have a context object?  If so, it must be zapped before
        going further (whether the scope was the NLR-target or not).

     3. Top scope:  If the scope is the top scope of an nmethod, it must continue with a
        MethodReturn or NLReturn rather than an InlinedMethodReturn/InlinedNLReturn.

     Unfortunately, at this point in the compilation we can't answer questions 1 and 2 yet,
     because we don't know yet which blocks (and context objects) have been optimized away.
     (We could know, if we kept track of things during the node generation.  However, even
     then we'd be wrong sometimes because some optimizations can eliminate blocks later.)
     On the other hand, we have to generate *something* here to correctly model the control
     flow.  Thus we insert a NLRTestNode now and defer the exact decision about what it
     does to the machine code generation phase.
  */
  if (!_nlrTestPoint) {
    int end_bci = nofBytes();
    // Note: src has incorrect def because it is implicitly assigned to via a call
    // (in sender scopes (when inlining), there will be an assignment when fixing up the connections).
    //SAPReg* src = new SAPReg(this, NLRResultLoc, false, true, end_bci, end_bci);
    //SAPReg* dst = new SAPReg(this, NLRResultLoc, false, false, end_bci, end_bci);
    _nlrTestPoint = NodeFactory::new_MergeNode(end_bci);
    _nlrTestPoint->append(NodeFactory::new_NLRTestNode(end_bci));
  }
  return _nlrTestPoint;
}


void InlinedScope::addResult(Expr* e) {
  // e is a possible return value; add it to our return expression to keep track of all possible return types
  assert(e->preg() == resultPR || resultPR == NULL || e->isNoResultExpr(), "bad result PReg");
  if (result == NULL) {
    result = e;
  } else {
    result = result->mergeWith(e, result->node());
  }
}


void InlinedScope::initializeArguments() {
  const int nofArgs = _method->number_of_arguments();
  _arguments = new GrowableArray<Expr*>(nofArgs, nofArgs, NULL);
  if (isTop()) {
    // create expr for self but do not allocate a location yet
    // (self is setup by the prologue node)
    _self =
      new KlassExpr(klassOop(selfKlass()),
		    new SAPReg(this, unAllocated, false, false, PrologueBCI, EpilogueBCI),
		    NULL);
    // preallocate incoming arguments, i.e., create their expressions
    // using SAPRegs that are already allocated
    for (int i = 0; i < nofArgs; i++) {
      SAPReg* arg = new SAPReg(this, Mapping::incomingArg(i, nofArgs), false, false, PrologueBCI, EpilogueBCI);
      _arguments->at_put(i, new UnknownExpr(arg));
    }
  } else {
    _self = NULL;	// will be initialized by sender
    // get args from sender's expression stack; top of expr stack = last arg, etc.
    const int top = sender()->exprStack()->length();
    for (int i = 0; i < nofArgs; i++) {
      _arguments->at_put(i, sender()->exprStack()->at(top - nofArgs + i));
    }
  }
}


void BlockScope::initializeSelf() {
  // for non-inlined top-level scope, make sure self is initialized
  // NB: bytecode compiler generates set_self_via_context, but only if self is needed in this
  // particular block and block expects a context (e.g., can have nested blocks with
  // set_self_via_context but not have one here).
  // Thus, load self in top-level block and ignore all nested set_self_via_context bytecodes.
  if (_parent->isInlinedScope() && method()->hasNestedBlocks()) {
    _gen.set_self_via_context();
  }
}


void InlinedScope::createTemporaries(int nofTemps) {
  // add nofTemps temporaries (may be called multiple times)
  int firstNew;
  if (!hasTemporaries()) {
    // first time we're called
    _temporaries = new GrowableArray<Expr*>(nofTemps, nofTemps, NULL);
    // The canonical model has the context in the first temporary.
    // To preserve this model the first temporary is aliased to _context.
    // Lars, 3/8/96
    if (_context) {
      _temporaries->at_put(0, new ContextExpr(_context));
      firstNew = 1;
    } else {
      firstNew = 0;
    }
 } else {
    // grow existing temp array
    const GrowableArray<Expr*>* oldTemps = _temporaries;
    const int n = nofTemps + oldTemps->length();
    _temporaries = new GrowableArray<Expr*>(n, n, NULL);
    firstNew = oldTemps->length();
    nofTemps += oldTemps->length();
    for (int i = 0; i < firstNew; i++) _temporaries->at_put(i, oldTemps->at(i));
  }
  // initialize new temps
 ConstPReg* nil = new_ConstPReg(this, nilObj);
  for (int i = firstNew; i < nofTemps; i++) {
    PReg* r = new PReg(this);
    _temporaries->at_put(i, new UnknownExpr(r, NULL));
    if (isTop()) {
      // temps are initialized by PrologueNode
    } else {
      gen()->append(NodeFactory::new_AssignNode(nil, r));
    }
  }
}


void InlinedScope::createFloatTemporaries(int nofFloats) {
  assert(!hasFloatTemporaries(), "cannot be called twice");
  _floatTemporaries = new GrowableArray<Expr*>(nofFloats, nofFloats, NULL);
  // initialize float temps
  for (int i = 0; i < nofFloats; i++) {
    PReg* preg = new PReg(this, Location::floatLocation(scopeID(), i), false, false);
    _floatTemporaries->at_put(i, new UnknownExpr(preg, NULL));
    if (isTop()) {
      // floats are initialized by PrologueNode
    } else {
      warning("float initialization of floats in inlined scopes not implemented yet");
    } 
  }
}


void InlinedScope::createContextTemporaries(int nofTemps) {
  // create _contextTemporaries and initialize all elements immediately;
  // after creation, some of the elements may be overwritten
  // (e.g., copying self or a method argument to the context)
  assert(_contextTemporaries == NULL, "more than one context created");
  assert(allocatesInterpretedContext(), "inconsistent context info");
  _contextTemporaries = new GrowableArray<Expr*>(nofTemps, nofTemps, NULL);
  for (int i = 0; i < nofTemps; i++) {
    PReg* r = new PReg(this);
    _contextTemporaries->at_put(i, new UnknownExpr(r, NULL));
  }
  // create context if not there yet
  if (_context == NULL) {
    // assert(isMethodScope(), "BlockScope should have correct context already");
    // replaced old assertion with the one below. Since assert disappears in the
    // fast version, put in a warning so that we can look at this if it happens
    // again (couldn't re-create the situation yet) - gri 5/10/96
    assert(isMethodScope() || isBlockScope() && method()->block_info() == methodOopDesc::expects_nil, "check this");
    //if (isBlockScope()) warning("possibly a bug in InlinedScope::createContextTemporaries - tell Robert");
    _context = new SAPReg(this, PrologueBCI, EpilogueBCI);
  }
  // The canonical model has the context in the first temporary.
  // To preserve this model the first temporary is aliased to _context.
  // Lars, 10/9/95
  _temporaries->at_put(0, new ContextExpr(_context));
}

void InlinedScope::contextTemporariesAtPut(int no, Expr* e) { 
  assert(!e->preg()->isSAPReg() || e->preg()->isBlockPReg() || ((SAPReg*)e->preg())->isInContext(), "not in context");
  _contextTemporaries->at_put(no, e); 
}


bool InlinedScope::allocatesCompiledContext() const {
  if (!allocatesInterpretedContext()) return false;
  ContextCreateNode* c = _contextInitializer->creator();
  if (bbIterator->usesBuilt && c->deleted) {
    // logically has a context, but it has been optimized away
    return false;
  } else {
    return true;
  }
}


void InlinedScope::prologue() {
  theCompiler->enterScope(this);
  initializeSelf();
}


static int compare_scopeBCIs(InlinedScope** a, InlinedScope** b) {
  // put unused scopes at the end so they can be deleted 
  if ((*a)->hasBeenGenerated() == (*b)->hasBeenGenerated()) {
    return (*a)->senderBCI() - (*b)->senderBCI();
  } else {
    return (*a)->hasBeenGenerated() ? -1 : 1;
  }
}


void InlinedScope::epilogue() {
  // generate epilogue code (i.e., everything after last byte code has been processed)
  assert(exprStack()->isEmpty(), "expr. stack should be empty now");

  // first make sure subScopes are sorted by bci
  _subScopes->sort(compare_scopeBCIs);

  // now remove all subscopes that were created but not used (not inlined)
  while (! _subScopes->isEmpty() && !_subScopes->last()->hasBeenGenerated()) _subScopes->pop();
#ifdef ASSERT
  for (int i = 0; i < _subScopes->length(); i++) {
    if (!_subScopes->at(i)->hasBeenGenerated()) fatal("unused scopes should be at end");
  }
#endif

  if (_nofSends > 0 && containsNLR()) {
    // this scope *could* be the target of a non-inlined NLR; add an UnknownExpr to
    // the scope's result expression to account for this possibility
    // note: to be sure, we'd have to know that at least one nested block wasn't inlined,
    // but this analysis isn't performed until later
    addResult(new UnknownExpr(resultPR, NULL));
    // also make sure we have an NLR test point to catch the NLR
    (void)nlrTestPoint();
    assert(has_nlrTestPoint(), "should have a NLR test point now");
  }

  // generate NLR code if needed
  if (has_nlrTestPoint()) {
    // NB: assertion below is too strict -- may have an nlr node that will be connected
    // only during fixupNLRPoints()
    // assert(nlrTestPoint()->nPredecessors() > 0, "nlr node is unused??");
  } else if (isTop() && theCompiler->nlrTestPoints->nonEmpty()) {
    // the top scope doesn't have an NLR point, but needs one anyway so that inlined
    // scopes have somewhere to jump to
    (void)nlrTestPoint();
  }
  if (!result) result = new NoResultExpr;
  theCompiler->exitScope(this);
}


bool InlinedScope::isSenderOf(InlinedScope* callee) const {
  assert(callee, "should have a scope");
  if (depth > callee->depth) return false;
  int d = callee->depth - 1;
  for (InlinedScope* s = callee->sender(); s && d >= depth; s = s->sender(), d--) {
    if (this == s) return true;
  }
  return false;
}


void InlinedScope::addSend(GrowableArray<PReg*>* expStk, bool isSend) {
  // add send or prim. call / uncommon branch to this scope and mark locals as debug-visible
  if (!expStk) return;    		// not an exposing send
  for (InlinedScope* s = this; s && s->isInlinedScope(); s = s->sender()) {
    if (isSend) s->_nofSends++;
    s->_nofInterruptPoints++;
    s->markLocalsDebugVisible(expStk);	// mark locals as debug-visible
  }
}


void InlinedScope::markLocalsDebugVisible(GrowableArray<PReg*>* exprStack) {
  // this scope has at least one send - mark params & locals as debug-visible
  int i;
  if (_nofSends <= 1) {
    // first time we're called
    self()->preg()->debug = true;
    for (i = nofArguments() - 1; i >= 0; i--) {
      argument(i)->preg()->debug = true;
    }
    for (i = nofTemporaries() - 1; i >= 0; i--) {
      temporary(i)->preg()->debug = true;
    }
    // if there's a context, mark all context variables as debug-visible too.
    GrowableArray<Expr*>*  ct = contextTemporaries();
    if (ct != NULL) {
      for (i = 0; i < ct->length(); i++) {
        ct->at(i)->preg()->debug = true;
      }
    }
  }
  // also mark expression stack as debug-visible (excluding arguments to
  // current send) (the args are already excluded from the CallNode's
  // expression stack, so just use that one instead of this->exprStack)
  for (i = 0; i < exprStack->length(); i++) {
    exprStack->at(i)->debug = true;
  }
}


void InlinedScope::setExprForBCI(int bci, Expr* expr) {
  assert(_exprStackElems->at_grow(bci) == NULL, "only one expr per BCI allowed");
  _exprStackElems->at_put_grow(bci, expr);
}


void InlinedScope::set2ndExprForBCI(int bci, Expr* expr) {
  _exprStackElems->at_put_grow(bci, expr);
}


void InlinedScope::set_self(Expr* e) { 
  assert(!_self, "self already set"); 
  assert(e->scope()->isSenderOrSame(this), "must be in sender scope");
  _self = e; 
}


int InlinedScope::homeContext() const {
  // count the number of logical (i.e. interpreter) contexts from here up to the home method
  // contexts are numbered starting with zero and there is at least one context
  int context = -1;
  methodOop method = _method;
  while (method != NULL) {
    if (method->allocatesInterpretedContext()) { context++; }
    method = method->parent();
  }
  assert(context >= 0, "there must be at least one context");
  return context;
}


InlinedScope* InlinedScope::find_scope(int c, int& nofIndirections, OutlinedScope*& out) {
  // return the InlinedScope that contains context c
  // IN : context no. c for this scope (in interpreter terms)
  // OUT: number of indirections required at run time (-1 = in same stack frame, 
  //      0 = in context of this frame, 1 = in parent context of this frame's context, etc.)
  // if the inlined scope is found (nofIndirections = -1) it is returned as the result
  // if the inlined scope is not found (nofIndirections >= 0), the highest possible scope
  // is returned and out is set to the outlined scope containing the context
  assert(c >= 0, "context must be >= 0");
  int distance = _method->lexicalDistance(c);
  nofIndirections = -1;
  Scope* 	s = this;
  out = NULL;
  // first, go up as far as possible
  for (int d = distance; d > 0 && s->parent()->isInlinedScope(); d--, s = s->parent()) ; 
  if (d == 0) {
    // found scope in our nmethod
    return (InlinedScope*)s;
  }

  // InlinedScope not found; go up the rest of the scopes and count how many 
  // stack frames are traversed
  InlinedScope* top = (InlinedScope*)s;
  if (top->allocatesCompiledContext()) nofIndirections++;
  Scope* prev = s;
  for (s = s->parent(); d > 0; d--, prev = s, s = s->parent()) {
    if (s->allocatesCompiledContext()) nofIndirections++;
  }
  assert(prev->isOutlinedScope(), "must be outlined scope");
  out = (OutlinedScope*)prev;
  assert(nofIndirections >= 0, "must have at least one context access");
  return top;
}


void InlinedScope::collectContextInfo(GrowableArray<InlinedScope*>* contextList) {
  // collect all scopes with contexts
  if (allocatesInterpretedContext()) contextList->append(this);
  for (int i = _subScopes->length() - 1; i >= 0; i--) {
    _subScopes->at(i)->collectContextInfo(contextList);
  }
}


int InlinedScope::number_of_noninlined_blocks() {
  // return the number of non-inlined blocks in this scope or its callees
  int nblocks = 0;
  for (int i = bbIterator->exposedBlks->length() - 1; i >= 0; i--) {
    BlockPReg* blk = bbIterator->exposedBlks->at(i);
    if (blk->isUsed() && isSenderOrSame(blk->scope())) nblocks++;
  }
  return nblocks;
}


void InlinedScope::generateDebugInfoForNonInlinedBlocks() {
  for (int i = bbIterator->exposedBlks->length() - 1; i >= 0; i--) {
    BlockPReg* blk = bbIterator->exposedBlks->at(i);
    if (blk->isUsed()) blk->closure()->generateDebugInfo();
  }
}


void InlinedScope::copy_noninlined_block_info(nmethod* nm) {
  for (int i = bbIterator->exposedBlks->length() - 1; i >= 0; i--) {
    BlockPReg* blk = bbIterator->exposedBlks->at(i);
    if (blk->isUsed()) {
      int offset = theCompiler->scopeDescRecorder()->
                     offset_for_noninlined_scope_node(blk->closure()->noninlined_block_scope());
      nm->noninlined_block_at_put(blk->closure()->id().minor(), offset);
    }
  }
}


// loop optimizations

void InlinedScope::addTypeTest(NonTrivialNode* t) {
  assert(t->doesTypeTests(), "shouldn't add");
  _typeTests->append(t);
}

CompiledLoop* InlinedScope::addLoop() {
  CompiledLoop* l = new CompiledLoop;
  _loops->append(l);
  return l;
}

void InlinedScope::optimizeLoops() {
  for (int i = _loops->length() - 1; i >= 0; i--) {
    CompiledLoop* loop = _loops->at(i);
    char* msg = loop->recognize();
    if (msg) {
      cout(PrintLoopOpts)->print("*loop %d in scope %s not an integer loop: %s\n", i, key()->print_string(), msg);
    } else {
      cout(PrintLoopOpts)->print("*optimizing integer loop %d in scope %s\n", i, key()->print_string());
      loop->optimizeIntegerLoop();
    }
    if (OptimizeLoops) loop->optimize();
  }
  for (i = _subScopes->length() - 1; i >= 0; i--) {
    _subScopes->at(i)->optimizeLoops();
  }
}



// register allocation

void InlinedScope::addToPRegsBegSorted(PReg* r)	{
  assert(PrologueBCI <= r->begBCI() && r->begBCI() <= EpilogueBCI, "illegal bci");
  assert(_pregsBegSorted->isEmpty() || _pregsBegSorted->last()->begBCI() <= r->begBCI(), "sort order wrong");
  _pregsBegSorted->append(r);
}


void InlinedScope::addToPRegsEndSorted(PReg* r)	{
  assert(PrologueBCI <= r->endBCI() && r->endBCI() <= EpilogueBCI, "illegal bci");
  assert(_pregsEndSorted->isEmpty() || _pregsEndSorted->last()->endBCI() <= r->endBCI(), "sort order wrong");
  _pregsEndSorted->append(r);
}


void InlinedScope::allocatePRegs(IntFreeList* f) {
  int bci  = PrologueBCI;
  int bi   = 0;
  int si   = 0;
  int ei   = 0;
  int blen = _pregsBegSorted->length();
  int slen = _subScopes->length();
  int elen = _pregsEndSorted->length();
  int n    = f->allocated();
  assert(blen == elen, "should be the same");
  while (bci <= EpilogueBCI) {
    // allocate registers that begin at bci
    while (bi < blen && _pregsBegSorted->at(bi)->begBCI() == bci) {
      _pregsBegSorted->at(bi)->allocateTo(Mapping::localTemporary(f->allocate()));
      bi++;
      assert(bi == blen || 
      	     _pregsBegSorted->at(bi)->begBCI() >= _pregsBegSorted->at(bi-1)->begBCI(),
	     "_pregsBegSorted not sorted");
    }
    // allocate registers for subscopes that begin at bci
    while (si < slen && _subScopes->at(si)->senderBCI() == bci) {
      _subScopes->at(si)->allocatePRegs(f);
      si++;
      assert(si == slen || 
      	     _subScopes->at(si)->senderBCI() >= _subScopes->at(si-1)->senderBCI(),
	     "_subScopes not sorted");
    }
    // release registers that end at bci
    while (ei < elen && _pregsEndSorted->at(ei)->endBCI() == bci) {
      f->release(Mapping::localTemporaryIndex(_pregsEndSorted->at(ei)->loc));
      ei++;
      assert(ei == elen || 
      	     _pregsEndSorted->at(ei)->endBCI() >= _pregsEndSorted->at(ei-1)->endBCI(),
	     "_pregsEndSorted not sorted");
    }
    // advance bci
    bci = min(bi < blen ? _pregsBegSorted->at(bi)->begBCI() : EpilogueBCI + 1,
              si < slen ? _subScopes->at(si)->senderBCI()   : EpilogueBCI + 1,
              ei < elen ? _pregsEndSorted->at(ei)->endBCI() : EpilogueBCI + 1);
  }
  assert(f->allocated() == n, "inconsistent allocation/release");
}


int InlinedScope::allocateFloatTemporaries(int firstFloatIndex) {
  assert(firstFloatIndex >= 0, "illegal firstFloatIndex");
  _firstFloatIndex = firstFloatIndex;				// start index for first float of this scope
  int nofFloatTemps = hasFloatTemporaries() ? nofFloatTemporaries() : 0;
  // convert floatLocs into stackLocs
  for (int k = 0; k < nofFloatTemps; k++) {
    PReg* preg = floatTemporary(k)->preg();
    Location loc = preg->loc;
    assert(loc.scopeNo() == scopeID() && loc.floatNo() == k, "inconsistency");
    preg->loc = Mapping::floatTemporary(scopeID(), k);
    assert(preg->loc.isStackLocation(), "must be a stack location");
  }
  int startFloatIndex = firstFloatIndex + nofFloatTemps;	// start index for first float in subscopes
  int totalNofFloatsInSubscopes = 0;
  // allocate float temporaries of subscopes
  int len = _subScopes->length();
  for (int i = 0; i < len; i++) {
    InlinedScope* scope = _subScopes->at(i);
    totalNofFloatsInSubscopes = max(totalNofFloatsInSubscopes, scope->allocateFloatTemporaries(startFloatIndex));
  }
  return nofFloatTemps + totalNofFloatsInSubscopes;
}


bool MethodScope::isRecursiveCall(methodOop method, klassOop rcvrKlass, int depth) {
  // test is method/rcvrKlass would be a recursive invocation of this scope
  if (method == _method && rcvrKlass == selfKlass()) {
    if (depth <= 1) {
      return true;	// terminate recursion here
    } else {
      // it's recursive, but the unrolling depth hasn't been reached yet
      depth--;
    }
  }
  // check sender
  if (isTop()) {
    return false;
  } else {
    return sender()->isRecursiveCall(method, rcvrKlass, depth);
  }
}


bool BlockScope::isRecursiveCall(methodOop method, klassOop rcvrKlass, int depth) {
  if (method == _method) {
    if (depth <= 1) {
      return true;	// terminate recursion here
    } else {
      // it's recursive, but the unrolling depth hasn't been reached yet
      // NB: don't just fall through to parent check below because of
      // x := [ x value ]. x value
      return sender()->isRecursiveCall(method, rcvrKlass, --depth);
    }
  }
  // don't check sender, only check parent and it's senders
  // otherwise, in code like [ x foo. y do: [ z foo ]] the z foo send
  // would be considered a recursive invocation of x foo if x class = z class
  if (parent()->isOutlinedScope()) {
    return false;
  } else {
    return parent()->isRecursiveCall(method, rcvrKlass, depth);
  }
}

void BlockScope::setContext(PReg* newContext) {
  _context = newContext;
  if (_temporaries->first()->isContextExpr()) {
    _temporaries->at_put(0, new ContextExpr(newContext));
  } else {
    GrowableArray<Expr*>* oldTemps = _temporaries;
    _temporaries = new GrowableArray<Expr*>(oldTemps->length() + 1);
    _temporaries->append(new ContextExpr(newContext));
    _temporaries->appendAll(oldTemps);
  }
}

void InlinedScope::genCode() {
  _hasBeenGenerated = true;
  prologue();
  // always generate (shared) entry points for ordinary & non-local return
  _returnPoint        = NodeFactory::new_MergeNode(EpilogueBCI);
  _NLReturnPoint      = NodeFactory::new_MergeNode(EpilogueBCI);
  _nlrTestPoint       = NULL;
  _contextInitializer = NULL;
  int nofTemps = method()->number_of_stack_temporaries();
  if (isTop()) {
    _returnPoint->append(NodeFactory::new_ReturnNode(resultPR, EpilogueBCI));
    _NLReturnPoint->append(NodeFactory::new_NLRSetupNode(resultPR, EpilogueBCI));
    Node* first = NodeFactory::new_PrologueNode(key(), nofArguments(), nofTemps);
    theCompiler->firstNode = first;
    gen()->setCurrent(first);
  }
  // allocate space for temporaries - initialization done in the prologue code
  assert(!hasTemporaries(), "should have no temporaries yet\n");
  createTemporaries(nofTemps);
  // allocate space for float temporaries
  int nofFloats = method()->total_number_of_floats();
  if (UseFPUStack) {
    const int FPUStackSize = 8;
    if (method()->float_expression_stack_size() <= FPUStackSize) {
      // float expression stack fits in FPU stack, use it instead
      // and allocate only space for the real float temporaries
      nofFloats = method()->number_of_float_temporaries();
    } else {
      warning("possible performance bug: cannot use FPU stack for float expressions");
    }
  }
  createFloatTemporaries(nofFloats);
  // build the intermediate representation
  assert(gen()->current() != NULL, "current() should have been set before");
  MethodIterator iter(method(), gen());
  if (gen()->aborting()) {
    // ends with dead code -- clean up expression stack
    while (!exprStack()->isEmpty()) exprStack()->pop();
  }
  epilogue();
}


// debugging info
void print_selector_cr(symbolOop selector) {
  char buffer[100];
  int length = selector->length();
  strncpy(buffer, selector->chars(), length);
  buffer[length] = '\0';
  std->print_cr("%s", buffer);
}

void InlinedScope::generateDebugInfo() {
  // Generate debug info for the common parts of methods and blocks

  if (PrintDebugInfoGeneration) {
    if (isMethodScope()) {
      std->print("Method: ");
      print_selector_cr(method()->selector());
      std->print_cr("self: %s", _self->preg()->name());
    } else {
      methodOop m;
      std->print("Method: ");
      for (m = method(); m->is_blockMethod(); m = m->parent()) {
        std->print("[] in ");
      }
      print_selector_cr(m->selector());
      method()->print_codes();
      std->print_cr("no receiver (block method)");
    }
  }

  ScopeDescRecorder* rec = theCompiler->scopeDescRecorder();
  int len, i;

  if (!isLite()) {
    // temporaries
    if (hasTemporaries()) {
      len = _temporaries->length();
      for (i = 0; i < len; i++) {
        PReg* preg = _temporaries->at(i)->preg();
        rec->addTemporary(_scopeInfo, i, preg->createLogicalAddress());
	if (PrintDebugInfoGeneration) std->print_cr("temp[%2d]: %s", i, preg->name());
      }
    }
    // float temporaries
    if (hasFloatTemporaries()) {
      len = _floatTemporaries->length();
      for (i = 0; i < len; i++) {
        PReg* preg = _floatTemporaries->at(i)->preg();
        rec->addTemporary(_scopeInfo, i, preg->createLogicalAddress());
	if (PrintDebugInfoGeneration) std->print_cr("float[%2d]: %s", i, preg->name());
      }
    }
    // context temporaries
    if (allocatesInterpretedContext()) {
      len = _contextTemporaries->length();
      for (i = 0; i < len; i++) {
        PReg* preg = _contextTemporaries->at(i)->preg();
        rec->addContextTemporary(_scopeInfo, i, preg->createLogicalAddress());
	if (PrintDebugInfoGeneration) std->print_cr("c_temp[%2d]: %s", i, preg->name());
      }
    }
    // expr stack
    len = _exprStackElems->length();
    for (i = 0; i < len; i++) {
      Expr* elem = _exprStackElems->at(i);
      if (elem != NULL) {
        PReg* r = elem->preg()->cpReg();
        if (r->scope()->isSenderOrSame(this)) {
	  // Note: Is it still needed to create this info here, since the
	  //       PReg locations may change over time and thus produce more
	  //       debug info than actually needed for the new backend. Discuss
	  //       this with Lars.
          rec->addExprStack(_scopeInfo, i, r->createLogicalAddress());
          if (PrintDebugInfoGeneration) std->print_cr("expr[%2d]: %s", i, r->name());
        } else {
  	  // r's scope is too low (i.e. it's not actually live anymore)
	  // this can only happen if the expression is discarded; thus it's safe not to describe this entry
	  // Urs 5/96
	  // fix this: should check that bci (i) is statement end (or r is NoPReg)
        }
      }
    }
  }

  // subscopes
  len = _subScopes->length();
  for (i = 0; i < len; i++) {
    InlinedScope* s = _subScopes->at(i);
    if (PrintDebugInfoGeneration) std->print_cr("Subscope %d (id = %d):", i, s->scopeID());
    s->generateDebugInfo();
  }
}


void MethodScope::generateDebugInfo() {
  ScopeDescRecorder* rec = theCompiler->scopeDescRecorder();
  const bool visible = true;
  _scopeInfo =
    rec->addMethodScope(
      _key,
      _method,
      _self->preg()->createLogicalAddress(),
      allocatesCompiledContext(),
      isLite(), 
      _scopeID,
      _sender ? _sender->scopeInfo() : NULL,
      _senderBCI,
      visible
    );
  InlinedScope::generateDebugInfo();
}


void BlockScope::generateDebugInfo() {
  ScopeDescRecorder* rec = theCompiler->scopeDescRecorder();
  if (_parent->isOutlinedScope()) {
    _scopeInfo =
      rec->addTopLevelBlockScope(
    	_method,
	_self->preg()->createLogicalAddress(),
    	_self->klass(),
    	allocatesCompiledContext()
      );
  } else {
    assert(_parent->isInlinedScope(), "oops");
    const bool visible = true;
    _scopeInfo =
      rec->addBlockScope(
    	_method,
	((InlinedScope*)_parent)->scopeInfo(),
    	allocatesCompiledContext(),
	isLite(),
	_scopeID,
	_sender->scopeInfo(),
	_senderBCI,
    	visible
      );
  }
  InlinedScope::generateDebugInfo();
}


// Outlined scopes

OutlinedScope* new_OutlinedScope(nmethod* nm, ScopeDesc* sc) {
  if (sc->isMethodScope()) {
    return new OutlinedMethodScope(nm, sc);
  } else {
    return new OutlinedBlockScope(nm, sc);
  }
}


OutlinedScope::OutlinedScope(nmethod* nm, ScopeDesc* scope) {
  _nm = nm;
  _scope = scope;
}


OutlinedBlockScope::OutlinedBlockScope(nmethod* nm, ScopeDesc* sc) : OutlinedScope(nm, sc) {
  ScopeDesc* parent = sc->parent(true);
  if (parent) {
    _parent = new_OutlinedScope(nm, parent);
  } else {
    _parent = NULL;
  }
}


Expr* OutlinedScope::receiverExpr(PReg* p) const { return _scope->selfExpr(p); }
methodOop OutlinedScope::method() const { return _scope->method(); }
klassOop OutlinedMethodScope::methodHolder() const { 
  return selfKlass()->klass_part()->lookup_method_holder_for(method()); 
}


// printing code

void SendInfo::print() {
  lprintf("SendInfo %#lx \"", this);
  sel->print_symbol_on();
  lprintf("\" (rcvr = %#lx, nsends = %ld)\n", rcvr, nsends);
}


void InlinedScope::printTree() {
  print();
  for (int i = 0; i < _subScopes->length(); i++) {
    _subScopes->at(i)->printTree();
  }
}

void InlinedScope::print() {
  lprintf(" method: %#lx\n\tid: %ld", method(), scopeID());
  lprintf("\nself:   ");
  self()->print();
  for (int i = 0; i < nofArguments(); i++) {
    lprintf("arg %2d: ", i);
    argument(i)->print();
  }
  lprintf("\n");
}


void MethodScope::print_short() {
  lprintf("(MethodScope*)%#lx (", this); selector()->print_symbol_on(); lprintf(")");
}


void MethodScope::print() {
  print_short();
  InlinedScope::print();
  if (sender()) { lprintf("  sender: "); sender()->print_short(); }
  lprintf(" @ %ld\n", senderBCI());
  method()->pretty_print();
}


void BlockScope::print() {
  print_short();
  InlinedScope::print();
  if (parent()) { printf("\tparent: ");  parent()->print_short(); }
  if (sender()) { lprintf("  sender: "); sender()->print_short(); }
  lprintf(" @ %ld\n", senderBCI());
  method()->pretty_print();
}


void BlockScope::print_short() {
  lprintf("(BlockScope*)%#lx (", this); selector()->print_symbol_on(); 
  lprintf(" %#lx)", method());
}


void OutlinedScope::print_short(char* name) {
  lprintf("(%s*)%#lx (", name, this); 
  _scope->selector()->print_symbol_on();
  lprintf(")");
}


void OutlinedScope::print(char* name) {
  print_short(name);
  lprintf("  _nm = %#lx, _scope = %#lx", _nm, _scope);
}


void OutlinedMethodScope::print_short() { OutlinedScope::print_short("OutlinedMethodScope"); }
void OutlinedMethodScope::print() { OutlinedScope::print("OutlinedMethodScope"); }

void OutlinedBlockScope::print() {
  OutlinedScope::print("OutlinedMethodScope");
  if (parent()) { lprintf("\n    parent: "); parent()->print_short(); }
  lprintf("\n");
}

void OutlinedBlockScope::print_short() { OutlinedScope::print_short("OutlinedMethodScope"); }

# endif
