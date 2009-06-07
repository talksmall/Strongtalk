/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.143 $ */
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

# include "incls/_compiler.cpp.incl"

# ifdef ASSERT
    bool verifyOften = false;
# else
    bool verifyOften = false;
# endif

int		nofCompilations	= 0;
Compiler*	theCompiler	= NULL;
Compiler*	lastCompiler	= NULL;	    // for debugging
BBIterator*	last_bbIterator;

void compiler_init() {
#ifdef ASSERT
  CompilerDebug = true;	  
#endif
}

ScopeDescRecorder* Compiler::scopeDescRecorder() { return rec; }

CodeBuffer* Compiler::code() const { return _code; }

Compiler::Compiler(LookupKey* k, methodOop m, CompiledIC* i) {
  key		= k;
  method	= m;
  ic		= i;
  parentNMethod	= NULL;
  blockScope	= NULL;

  main_jumpTable_id     = jumpTableID();
  promoted_jumpTable_id = jumpTableID();

  initialize();
}


Compiler::Compiler(RScope* scope) {
  assert(scope != NULL, "scope must exist");

  key		= scope->key();
  method	= scope->method();
  ic		= NULL;
  parentNMethod	= NULL;
  blockScope	= NULL;

  main_jumpTable_id     = jumpTableID();
  promoted_jumpTable_id = jumpTableID();

  initialize(scope);
}

Compiler::Compiler(blockClosureOop blk, NonInlinedBlockScopeDesc* scope) : _scopeStack(10) {
  // Create a valid key for the compiled method.
  // {receiver class, block method} see key.hpp
  key = LookupKey::allocate(scope->parent()->selfKlass(), scope->method());

  assert(blk->isCompiledBlock(), "must be compiled block");
  jumpTableEntry* e = blk->jump_table_entry();
  int sub_index;
  parentNMethod = e->parent_nmethod(sub_index);

  short main_index = parentNMethod->main_id.is_block() 
                     ? parentNMethod->promoted_id.major()
                     : parentNMethod->main_id.major() ;

  main_jumpTable_id     = jumpTableID(main_index, sub_index);
  promoted_jumpTable_id = jumpTableID();

  blockScope = scope;
  method = scope->method(); 
  ic = NULL;

  // Check if the inlining database is active
  RScope* rs = NULL;
  if (UseInliningDatabase) {
    LookupKey* outer = &parentNMethod->outermost()->key;
    rs = InliningDatabase::lookup_and_remove(outer, key);
    if (rs && TraceInliningDatabase) {
      std->print("ID block compile: ");
      key->print();
      std->cr();
    }
  }
  initialize(rs);
}


void Compiler::finalize() {
  assert(theMacroAssm == NULL, "shouldn't have an assembler anymore");
  _code = NULL;
  last_bbIterator = bbIterator;
  bbIterator = NULL;
  theCompiler = NULL;
}


int Compiler::level() const { 
  return _noInlinableSends ? MaxRecompilationLevels - 1 : _nextLevel; 
}


int Compiler::version() const { 
  if (recompilee) {
    // don't increment version number when uncommon-recompiling
    // (otherwise limit is reached too quickly)
    return recompilee->version() + (is_uncommon_compile() ?  0 : 1); 
  } else {
    return 0;
  }
}


int Compiler::estimatedSize() const {
  // estimated target nmethod size (bytes)
  return NodeFactory::cumulCost;
}


InlinedScope* Compiler::currentScope() const 	{ return _scopeStack.top(); }
void Compiler::enterScope(InlinedScope* s) 	{ _scopeStack.push(s); }
void Compiler::exitScope (InlinedScope* s) 	{ assert(s == _scopeStack.top(), "bad nesting"); _scopeStack.pop(); }


void Compiler::initialize(RScope* remote_scope) {
  //assert(VMProcess::vm_operation() != NULL, "must be in vmProcess to compile");
  if (VMProcess::vm_operation() == NULL)
    warning("should be in vmProcess to compile"); //softened to a warning to support testing
  nofCompilations++;
#ifdef DEBUG
  messages = new stringStream(250 * K);
#endif

  if (remote_scope) {
    _uses_inlining_database = true;
    recompileeRScope = remote_scope;
  } else {
    _uses_inlining_database = false;
  }

  recompileeRScope = remote_scope;
  assert(theCompiler == NULL, "shouldn't have but one compiler at a time");
  assert(theMacroAssm == NULL, "shouldn't have an assembler yet");
  PReg::initPRegs();	// must come early (before any PReg allocation)
  initNodes();		// same here (before creating nodes)
  initLimits();
  theCompiler	= lastCompiler = this;
  _code         = new CodeBuffer(CompilerInstrsSize, CompilerInstrsSize / 2);
  countID	= -1;
  topScope	= NULL;
  bbIterator 	= new BBIterator;
  /* theAllocator = */ new RegisterAllocator();
  assert(method, "must have method");
  Scope::initialize();
  _totalNofBytes = 0;
  _special_handler_call_offset = -1;
  _entry_point_offset = -1;
  _verified_entry_point_offset = -1;
  _totalNofFloatTemporaries = -1;
  _float_section_size = 0;
  _float_section_start_offset = 0;
  rec = new ScopeDescRecorder(CompilerScopesSize, CompilerPCsSize);
  // Save dependency information in the scopeDesc recorder.
  rec->add_dependant(key);

  nlrTestPoints = new GrowableArray<NLRTestNode*>(50); 
  contextList = NULL;
  scopes = new GrowableArray<InlinedScope*>(50);
  blockClosures = new GrowableArray<BlockPReg*>(50);
  firstNode = NULL;
  reporter = new PerformanceDebugger(this);
  initTopScope();
}


void Compiler::initLimits() {
  if (recompileeRScope) {
    // We're compiling from the inlining data base
    _nextLevel = MaxRecompilationLevels - 1;
  } else if (recompilee) {
    if (DeltaProcess::active()->isUncommon()) {
      // when recompiling because of an uncommon trap, reset level
      _nextLevel = 0;
    } else {
      _nextLevel = recompilee->level() + 1;
      if (_nextLevel >= MaxRecompilationLevels) {
	warning("recompilation level too high -- should not happen");
	_nextLevel = MaxRecompilationLevels;
      }
    }
  } else {
    // new nmethod
    _nextLevel = 0;
  }
  _noInlinableSends = true;

#ifdef LATER
  inlineLimit[NormalFnLimit] 	     = getLimit(limits[NormalFnLimit],		level);
  inlineLimit[BlockFnLimit] 	     = getLimit(limits[BlockFnLimit],		level);
  inlineLimit[BlockArgFnLimit]       = getLimit(limits[BlockArgFnLimit],	level);
  inlineLimit[NormalFnInstrLimit]    = getLimit(limits[NormalFnInstrLimit],	level);
  inlineLimit[BlockFnInstrLimit]     = getLimit(limits[BlockFnInstrLimit],	level);
  inlineLimit[BlockArgFnInstrLimit]  = getLimit(limits[BlockArgFnInstrLimit],	level);
  inlineLimit[SplitCostLimit]        = getLimit(limits[SplitCostLimit],		level);
  inlineLimit[NmInstrLimit]          = getLimit(limits[NmInstrLimit],		level);

  if (CompilerAdjustLimits) {
    // adjust NmInstrLimit if top-level method is large
    int cost = sicCost((methodKlass*)method->klass(), topScope, costP);
    if (cost > NormalMethodLen) {
      float l = (float)cost / NormalMethodLen * inlineLimit[NmInstrLimit];
      inlineLimit[NmInstrLimit] = min(int(l), CompilerInstructionsSize / 3);
    }
  }
#endif
}


bool Compiler::registerUninlinable(Inliner* inliner) {
  // All sends that aren't inlined for some reason are registered here
  // to determine the minimum optimization level needed for recompilation
  // (i.e. if the send wouldn't be inlined even at the highest optimization
  // level there's no point in recompiling).
  // At the end of compilation, _nextLevel will contain the lowest
  // optimization level that will generate better code than the current level.
  // Return true if the send is considered non-inlinable.
  if (!Inline) return true;		    // no point recompiling
  SendInfo* info = inliner->info();
  if (is_database_compile()) {
    info->counting = false;
    info->uninlinable = true;		    // for now, never inline if not inlined in DB 
    // (would need to change DB format to allow counting and uninlinable sends)
  }

  if (!UseRecompilation) {
    // make sure we're not using counting sends
    info->counting = false;
  }
  if (info->uninlinable) {
    info->counting = false;
    return true;			    // won't be inlined, ever
  }
  if (is_uncommon_compile()) {
    info->counting = true;		    // make sure the uncommon nmethod is recompiled eventually
  }
  if (inliner->msg() == NULL) {
    info->counting = true;		    // for now
    _noInlinableSends = false;		    // unknown receiver (?)
    return false;
  } else {
    assert(!info->rcvr->isUnknownExpr(), "oops");
    return true;
  }
}


bool Compiler::is_uncommon_compile() const {
  return DeltaProcess::active()->isUncommon();
}


// NewBackendGuard is used only to set the right flags to enable the
// new backend (enabled via TryNewBackend) instead of setting them
// all manually. At some point all the bugs should be fixed and this
// class and its use can simply be removed.
//
// This class basically simplifies Dave's (or whoever's) life since
// only one flag (TryNewBackend) needs to be set and everything else
// is setup automatically. Eventually UseNewBackend should do the job.
//
// gri 10/2/96

class NewBackendGuard: StackObj {
 private:
  static bool _first_use;

  bool _UseNewBackend;
  bool _LocalCopyPropagate;
  bool _OptimizeLoops;
  bool _OptimizeIntegerLoops;

 public:
  NewBackendGuard() {
    // save original settings in any case
    _UseNewBackend        = UseNewBackend;
    _LocalCopyPropagate   = LocalCopyPropagate;
    _OptimizeLoops        = OptimizeLoops;
    _OptimizeIntegerLoops = OptimizeIntegerLoops;

    if (TryNewBackend) {
      // print out a warning if this class is used
      if (_first_use) {
        warning("TryNewBackend automatically changes some flags for compilation - for temporary use only");
	_first_use = false;
      }

      // switch to right settings
      UseNewBackend        = true;
      LocalCopyPropagate   = false;
      OptimizeLoops        = false;
      OptimizeIntegerLoops = false;
    }
  }

  ~NewBackendGuard() {
    // restore original settings in any case
    UseNewBackend        = _UseNewBackend;
    LocalCopyPropagate   = _LocalCopyPropagate;
    OptimizeLoops        = _OptimizeLoops;
    OptimizeIntegerLoops = _OptimizeIntegerLoops;
  }
};

bool NewBackendGuard::_first_use = true;

nmethod* Compiler::compile() {
  NewBackendGuard guard;

  if ((PrintProgress > 0) && (nofCompilations % PrintProgress == 0)) std->print(".");
  const char* compiling;
  if (DeltaProcess::active()->isUncommon()) {
    compiling = recompilee ? "Uncommon-Recompiling " : "Uncommon-Compiling ";
  } else {
    if (_uses_inlining_database) {
      compiling = recompilee ? "Recompiling (database)" : "Compiling (database)";
    } else {
      compiling = recompilee ? "Recompiling " : "Compiling ";
    }
  }
  EventMarker em("%s%#lx %#lx", compiling, key->selector(), NULL);

  // don't use uncommon traps when recompiling because of trap
  useUncommonTraps = DeferUncommonBranches && !is_uncommon_compile();
  if (is_uncommon_compile()) reporter->report_uncommon(false);
  if (recompilee && recompilee->isUncommonRecompiled()) reporter->report_uncommon(true);
  // don't use counters when compiling from DB
  FlagSetting fs(UseRecompilation, UseRecompilation && !is_database_compile());

  bool should_trace = _uses_inlining_database ? PrintInliningDatabaseCompilation : PrintCompilation;
  TraceTime t(compiling, should_trace);
    
  if (should_trace || PrintCode) {
    print_key(std);
    if (PrintCode || PrintInlining) std->print("\n");
  }

  topScope->genCode();
  fixupNLRTestPoints();
  buildBBs();

  if (PrintCode) print_code(false);
  if (verifyOften) bbIterator->verify();

  // compute escaping blocks and up-level accessed vars
  bbIterator->computeEscapingBlocks();
  bbIterator->computeUplevelAccesses();
  if (verifyOften) bbIterator->verify();
  //if (PrintCode) print_code(false);

  // construct def & use information
  bbIterator->makeUses();
  if (verifyOften) bbIterator->verify();
  //if (PrintCode) print_code(false);

  if (LocalCopyPropagate) {
    bbIterator->localCopyPropagate();
    if (verifyOften) bbIterator->verify();
  }
  //if (PrintCode) print_code(false);
  if (GlobalCopyPropagate) {
    bbIterator->globalCopyPropagate();
    if (verifyOften) bbIterator->verify();
  }
  //if (PrintCode) print_code(false);
  if (BruteForcePropagate) {
    bbIterator->bruteForceCopyPropagate();
    if (verifyOften) bbIterator->verify();
  }
  //if (PrintCode) print_code(false);
  if (EliminateUnneededNodes) {
    bbIterator->eliminateUnneededResults();
    if (verifyOften) bbIterator->verify();
  }
  //if (PrintCode) print_code(false);
  if (OptimizeIntegerLoops) {
    // run after copy propagation so that loop increment is easier to recognize
    // also run after eliminateUnneededResults so that cpInfo is set for eliminated PRegs
    topScope->optimizeLoops();
    if (verifyOften) bbIterator->verify();
  }
  //if (PrintCode) print_code(false);
 
  // compute existence & format of run-time context objects and blocks
  computeBlockInfo();

  // allocate floats
  _totalNofFloatTemporaries = topScope->allocateFloatTemporaries(0);
  
  // HACK: Fix preallocation
  // Necessary because a few primitives (allocateContext/Closure) need self or
  // the previous context after calling a primitive; i.e., self or the previous
  // context should not be allocated to a register. Currently not working correctly
  // -> allocated to stack as a temporary fix for the problem.
  theAllocator->preAllocate(topScope->self()->preg());
  bbIterator->localAlloc();		// allocate regs within basic blocks
  theAllocator->allocate(bbIterator->globals);

  if (PrintCode) print_code(false);
#ifdef ASSERT
  bbIterator->verify();
#endif

  if (PrintDebugInfoGeneration) {
    std->cr();
    std->cr();
    std->print_cr("Start of debugging info.");
  }
  topScope->generateDebugInfo();	// must come before gen to set scopeInfo
  topScope->generateDebugInfoForNonInlinedBlocks();

  // generate machine code
  theMacroAssm  = new MacroAssembler(_code);
  if (UseNewBackend) {
    PRegMapping* mapping = new PRegMapping(theMacroAssm, topScope->nofArguments(), 6, topScope->nofTemporaries());
    CodeGenerator* cgen = new CodeGenerator(theMacroAssm, mapping);
    cgen->initialize(topScope);
    bbIterator->apply(cgen);
    cgen->finalize(topScope);
  } else {
    // use a node visitor to generate code
    OldCodeGenerator* cgen = new OldCodeGenerator();
    bbIterator->apply(cgen);
  }
  theMacroAssm->finalize();
  theMacroAssm = NULL;

#ifndef ASSERT
  if (verifyOften) {
#endif
    bool ok = bbIterator->verifyLabels();
    if (!ok) print_code(false);
#ifndef ASSERT
  }
#endif

  rec->generate();			// write debugging info
  nmethod* nm = new_nmethod(this);	// construct new nmethod
  em.event.args[1] = nm;

  if (PrintAssemblyCode) Disassembler::decode(nm);

  reporter->finish_reporting();
  if (should_trace) {
    lprintf(": %#lx (%d bytes; level %ld v%d)\n", nm, nm->instsLen(), nm->level(), nm->version());
    flush_logFile();
  }

  if (verifyOften) nm->verify();

  if (PrintDebugInfo) nm->print_inlining(std, true);

  return nm;
}


void Compiler::buildBBs() {		// build the basic block graph
  bbIterator->build(firstNode);
}


void Compiler::fixupNLRTestPoints() {
  // the NLRTest nodes didn't get their correct successors during node generation because
  // their sender scopes' nlrTestPoints may not yet have been created; fix them up now
  int i = nlrTestPoints->length();
  while (i-- > 0) nlrTestPoints->at(i)->fixup();
}

void Compiler::computeBlockInfo() {
  FlagSetting(EliminateUnneededNodes, true);  // unused context nodes must be eliminated
  GrowableArray<InlinedScope*>* allContexts = new GrowableArray<InlinedScope*>(25);
  topScope->collectContextInfo(allContexts);
  // for now, just allocate all contexts as in interpreter
  // fix this later: collect all uplevel-accessed PRegs at same loop depth, form physical
  // contexts for these
  // also, if uplevel-read and single def --> could copy into context and keep
  // stack/register copy


  // remove all unused contexts 
  // need to iterate because removing a nested context may enable removal of a parent context
  // (could avoid iteration with topo sort, but there are few contexts anyway)
  bool changed = EliminateContexts;
  while (changed) {
    changed = false;
    for (int i = allContexts->length() - 1; i >= 0; i--) {
      InlinedScope* s = allContexts->at(i);
      if (s == NULL) continue;
      PReg* contextPR = s->context();
      assert(contextPR->isSinglyAssigned(), "should have exactly one def");
      GrowableArray<Expr*>* temps = s->contextTemporaries();
      bool noUplevelAccesses = true;
      // check if all context temps can be stack-allocated
      for (int j = temps->length() - 1; j >= 0; j--) {
	PReg* r = temps->at(j)->preg();
	if (r->uplevelR() || r->uplevelW()	    // this temp is still uplevel-accessed, so can't eliminate context
	    || (r->isBlockPReg() && !r->isUnused()) // this block still forces a context
	    ) {
	  noUplevelAccesses = false;
	  break;
	}
      }
      // TO DO: check if context is needed for NLRs
      // (noUplevelAccesses alone does not allow elimination)
      if (/*noUplevelAccesses || */contextPR->isSinglyUsed()) {
        // can eliminate context -- no uplevel-accessed vars
	// (single use is context initializer)
  	if (CompilerDebug) cout(PrintEliminateContexts)->print("%*s*eliminating context %s\n", s->depth, "", contextPR->safeName());
        contextPR->scope()->gen()->removeContextCreation();
	allContexts->at_put(i, NULL);	  // make code generator break if it tries to access this context
	changed = true;
      }
    }
  }

  // now collect all remaining contexts
  int i = allContexts->length();
  contextList = new GrowableArray<InlinedScope*>(i, i, NULL);
  while (i-- > 0) {
    // should merge several contexts into one physical context if possible
    // fix this later
    InlinedScope* s = allContexts->at(i);
    if (s == NULL) continue;
    PReg* contextPR = s->context();
    if (CompilerDebug) {
      cout(PrintEliminateContexts)->print("%*s*could not eliminate context %s in scope %s\n", 
      					  s->depth, "", contextPR->safeName(), s->key()->print_string());
    }
    reporter->report_context(s);
    contextList->at_put(i, s);
    ContextCreateNode* c = s->contextInitializer()->creator();
    c->set_contextNo(i);
    GrowableArray<Expr*>* temps = s->contextTemporaries();
    // allocate the temps in this context (but only if they're used)
    int ntemps = temps->length();
    int size = 0;
    for (int j = 0; j < ntemps; j++) {
      PReg* p = temps->at(j)->preg();
// should be:
//     if (p->isUsed() && (p->uplevelR() || p->uplevelW())) {
// but doesn't work yet (probably must fix set_self_via_context etc.)
// -Urs 6/96
      if (p->isUsed()) {
	// allocate p to context temp
	assert(p->scope() == s || p->isBlockPReg(), "oops");
	Location loc = Mapping::contextTemporary(i, size, s->scopeID());
	if (p->isBlockPReg()) {
	  // Blocks aren't actually assigned (at the PReg level) so that the inlining info
	  // isn't lost.  Thus we need to create a fake destination here if the context exists.
	  SAPReg* dest = new SAPReg(s, loc, true, true, PrologueBCI, EpilogueBCI);
	  Expr* e = new UnknownExpr(dest, NULL);
	  //contextPR->scope()->contextInitializer()->initialize(j, init);
	  temps->at_put(j, e);
	} else {
	  p->allocateTo(loc);
	}
	size++;
      }
    }
    c->set_sizeOfContext(size);
    if (size < ntemps && c->scope()->number_of_noninlined_blocks() > 0) {
      // this hasn't been exercised much 
      compiler_warning("while compiling %s: eliminated some context temps", key->print_string());
    }
  }

  // Compute the number of noninlined blocks for the nmethod and allocate 
  const int nblocks = topScope->number_of_noninlined_blocks();

  if (is_method_compile() || nblocks > 0) {
    // allocate nblocks+1 jumpTable entries
    const jumpTableID id = Universe::code->jump_table()->allocate(nblocks + 1);

    if (is_method_compile()) {
      main_jumpTable_id = id;
    } else {
      promoted_jumpTable_id = id;
    }

    // first is for nmethod itself
    int block_index = 1;
    for (int i = bbIterator->exposedBlks->length() - 1; i >= 0; i--) {
      BlockPReg* blk = bbIterator->exposedBlks->at(i);
      if (blk->isUsed()) {
        assert(block_index <= nblocks, "nblocks too small");
        blk->closure()->set_id(id.sub(block_index++));
      }
    }
    assert(nblocks + 1 == block_index, "just checking");
  }
}

void Compiler::initTopScope() {
  if (recompileeRScope == NULL) {
    if (TypeFeedback) {
      recompileeRScope = 
        recompilee ?
	  (RScope*) RNonDummyScope::constructRScopes(recompilee) : 
	  (RScope*) new RInterpretedScope(NULL, -1, key, method, 0, true);
    } else {
      recompileeRScope = new RNullScope;
    }
  }
  if (PrintRScopes) recompileeRScope->printTree(0, 0);

  countID = Universe::code->nextNMethodID(); 
  Scope* parentScope = NULL;
  SendInfo* info = new SendInfo(NULL, key, NULL);
  InlinedScope* sender = NULL;	// no sender -- top scope in nmethod
    
  if (is_block_compile()) {
    // block method
    assert(parentNMethod != NULL, "parentNMethod must be set for block compile");
    assert(blockScope->parent() != NULL, "must know parent");
    parentScope = new_OutlinedScope(parentNMethod, blockScope->parent());
    topScope = BlockScope::new_BlockScope(method, parentScope->methodHolder(), parentScope, sender, recompileeRScope, info);
  } else {
    // normal method
    klassOop methodHolder = key->klass()->klass_part()->lookup_method_holder_for(method);
    topScope = MethodScope::new_MethodScope(method, methodHolder, sender, recompileeRScope, info);
  }
  // make sure home exists always
  assert(topScope->home() != NULL, "no home");
}


void Compiler::print() {
  print_short(); lprintf(":");
  key->print();
  lprintf("\tmethod: %s\n", method->print_string());
  lprintf("\tp ((Compiler*)%#lx)->print_code()\n", this);
}


void Compiler::print_short() {
  lprintf("(Compiler*) %#lx", this);
}


void Compiler::print_key(outputStream* str) {
  key->print_on(str);
  if (topScope == NULL) return; // print_key may be used during fatals where the compiler isn't set up yet

  str->print(" (no. %d, method %#x", nofCompilations, method);
  // print the parent scope offset for block compiles.
  if (blockScope) {
    std->print(", parent offset %d", blockScope->parent()->offset());
  }
  str->print(")...");
}

void Compiler::print_code(bool suppressTrivial) {
  if (theCompiler == NULL) {
    // This will not work, another indication that firstNode should be stored with the BBIterator
    // anyway, not fixed for now (gri 6/6/96)
    last_bbIterator->print_code(suppressTrivial);
    last_bbIterator->print();
  } else {
    bool hadBBs = bbIterator != NULL;
    if (! hadBBs) {
      // need BBs for printing
      bbIterator = new BBIterator;
      buildBBs();
    }
    bbIterator->print_code(suppressTrivial);
    bbIterator->print();
    if (!hadBBs) {
      bbIterator->clear();
      bbIterator = NULL;
    }
  }
  lprintf("\n\n");
}


int Compiler::get_invocation_counter_limit() const {
  if (is_uncommon_compile()) {
    return RecompilationPolicy::uncommonNMethodInvocationLimit(version());
  } else {
    return Interpreter::get_invocation_counter_limit();
  }
}


void Compiler::set_special_handler_call_offset(int offset) {
  // doesn't need to be aligned since called rarely and from within the nmethod only
  _special_handler_call_offset = offset;
}


void Compiler::set_entry_point_offset(int offset) {
  assert(offset % oopSize == 0, "entry point must be aligned");
  _entry_point_offset = offset;
}


void Compiler::set_verified_entry_point_offset(int offset) {
  assert(offset % oopSize == 0, "verified entry point must be aligned");
  _verified_entry_point_offset = offset;
}


void Compiler::set_float_section_size(int size) {
  assert(size >= 0, "size cannot be negative");
  _float_section_size = size;
}


void Compiler::set_float_section_start_offset(int offset) {
  _float_section_start_offset = offset;
}


int Compiler::number_of_noninlined_blocks() const {
  return topScope->number_of_noninlined_blocks();
}


void Compiler::copy_noninlined_block_info(nmethod* nm) {
  topScope->copy_noninlined_block_info(nm);
}

#ifdef DEBUG
outputStream* cout(bool flag) {
  return (flag || theCompiler == NULL) ? std : theCompiler->messages;
}

void print_cout() {
  ResourceMark rm;
  lputs(theCompiler->messages->as_string());
}

#endif  // DEBUG

#endif  // DELTA_COMPILER
