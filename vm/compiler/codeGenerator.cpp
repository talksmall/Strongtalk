/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.78 $ */
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
# include "incls/_codeGenerator.cpp.incl"


// Sometimes a little stub has to be generated if a merge between two execution
// paths requires that the PRegMapping of one path is made conformant with the
// mapping of the other path. If this code cannot be emitted in place (because
// it would destroy the mapping in use) a conditional jump to the stub instructions
// is generated instead (note that in case of an absolute jump, merge code can
// always be emitted in place). A Stub holds the necessary information to generate
// the merge instructions.

// Stub routines should eventually be canonicalized if possible -> saves space. FIX THIS

class Stub: public ResourceObj {
 private:
  PRegMapping*	_mapping;
  Node*		_dst;
  Label		_stub_code;
  
  Stub(PRegMapping* mapping, Node* dst) {
    assert(dst->hasMapping() && !mapping->isConformant(dst->mapping()), "no stub required");
    _mapping = mapping;
    _dst     = dst;
  }

 public:
  static Stub* new_jcc_stub(PRegMapping* mapping, Node* dst, Assembler::Condition cc) {
    Stub* s = new Stub(mapping, dst);
    // generate conditional jump to stub code
    mapping->assembler()->jcc(cc, s->_stub_code);
    return s;
  }

  static Stub* new_NLR_stub(PRegMapping* mapping, Node* dst, int flags) {
    Stub* s = new Stub(mapping, dst);
    // generate inline cache with NLR jumping to stub code
    mapping->assembler()->ic_info(s->_stub_code, flags);
    return s;
  }

  void generateMergeStub() {
    _mapping->assembler()->bind(_stub_code);
    _mapping->makeConformant(_dst->mapping());
    _mapping->assembler()->jmp(_dst->label);
  }
};


class DebugInfoWriter: public PRegClosure {
 private:
  GrowableArray<PReg*>*	_pregs;			// maps index -> preg
  GrowableArray<int>*	_locations;		// previous preg location or illegalLocation
  GrowableArray<bool>*	_present;		// true if preg is currently present

  Location location_at(int i)			{ return Location(_locations->at(i)); }
  void location_at_put(int i, Location loc)	{ _locations->at_put(i, loc._loc); }

 public:
  DebugInfoWriter(int number_of_pregs) {
    _pregs     = new GrowableArray<PReg*>(number_of_pregs, number_of_pregs, NULL                );
    _locations = new GrowableArray<int  >(number_of_pregs, number_of_pregs, illegalLocation._loc);
    _present   = new GrowableArray<bool >(number_of_pregs, number_of_pregs, false               );
  }


  void preg_do(PReg* preg) {
    if (preg->logicalAddress() != NULL && !preg->loc.isContextLocation()) {
      // record only debug-visible PRegs & ignore context PRegs
      // Note: ContextPRegs appear in the mapping only because
      //       their values might also be cached in a register.
      int i = preg->id();
      _pregs  ->at_put(i, preg);		// make sure preg is available
      _present->at_put(i, true);		// mark it as present
    }
  }

  
  void write_debug_info(PRegMapping* mapping, int pc_offset) {
    // record current pregs in mapping
    mapping->iterate(this);
    // determine changes & notify ScopeDescRecorder if necessary
    ScopeDescRecorder* rec = theCompiler->scopeDescRecorder();
    for (int i = _locations->length(); i-- > 0; ) {
      PReg*    preg    = _pregs->at(i);
      bool     present = _present->at(i);
      Location old_loc = location_at(i);
      Location new_loc = present ? mapping->locationFor(preg) : illegalLocation;
      if ((!present && old_loc != illegalLocation) ||	// preg not present anymore but has been there before
	  ( present && old_loc == illegalLocation) ||	// preg present but has not been there before
	  ( present && old_loc != new_loc)) {		// preg present but has changed location
        // preg location has changed => notify ScopeDescRecorder
	NameNode* nameNode;
	if (new_loc == illegalLocation) {
	  nameNode = new IllegalName();
	} else {
	  nameNode = new LocationName(new_loc);
	}
	// debugging
	if (PrintDebugInfoGeneration) {
	  std->print_cr("%5d: %-20s @ %s", pc_offset, preg->name(), new_loc.name());
	}
	rec->changeLogicalAddress(preg->logicalAddress(), nameNode, pc_offset);
      }
      location_at_put(i, new_loc);		// record current location
      _present->at_put(i, false);		// mark as not present for next round
    }
  }


  void print() {
    std->print_cr("a DebugInfoWriter");
  }
};


// Implementation of CodeGenerator

CodeGenerator::CodeGenerator(MacroAssembler* masm, PRegMapping* mapping) : _mergeStubs(16) {
  assert(masm == mapping->assembler(), "should be the same");
  PRegLocker::initialize();
  _masm            = masm;
  _currentMapping  = mapping;
  _debugInfoWriter = new DebugInfoWriter(bbIterator->pregTable->length());
  _maxNofStackTmps = 0;
  _previousNode    = NULL;
  _nilReg          = noreg;
  _pushCode        = NULL;
}


void CodeGenerator::setMapping(PRegMapping* mapping) {
  maxNofStackTmps(); // enforce adjustement of _maxNofStackTmps
  _currentMapping = mapping;
}


int CodeGenerator::maxNofStackTmps() {
  if (_currentMapping != NULL) {
    _maxNofStackTmps = max(_maxNofStackTmps, _currentMapping->maxNofStackTmps());
  }
  return _maxNofStackTmps;
}


Register CodeGenerator::def(PReg* preg) const {
  assert(!preg->isConstPReg(), "cannot assign to ConstPReg");
  assert(!preg->loc.isContextLocation(), "cannot assign into context yet");
  return _currentMapping->def(preg);
}


bool CodeGenerator::isLiveRangeBoundary(Node* a, Node* b) const {
  return a->scope() != b->scope() || a->bci() != b->bci();
}


void CodeGenerator::jmp(Node* from, Node* to, bool to_maybe_nontrivial) {
  // keep only PRegs that are still alive at dst
  if (from != NULL && isLiveRangeBoundary(from, to)) _currentMapping->killDeadsAt(to);
  // make mappings conformant if necessary
  if (to_maybe_nontrivial || (to->isMergeNode() && !to->isTrivial())) {
    // dst has more than one predecessor
    if (!to->hasMapping()) {
      // first jump to dst, use current mapping, must be injective
      _currentMapping->makeInjective();
      to->setMapping(_currentMapping);
    } else {
      // not the first mapping => make mapping conformant
      _currentMapping->makeConformant(to->mapping());
    }
  } else {
    // dst has exactly one predecessor => use current mapping
    assert(!to->hasMapping(), "more than one predecessor?");
    to->setMapping(_currentMapping);
  }
  _masm->jmp(to->label);
  setMapping(NULL);
}


void CodeGenerator::jcc(Assembler::Condition cc, Node* from, Node* to, bool to_maybe_nontrivial) {
  // make mappings conformant if necessary
  if (to_maybe_nontrivial || (to->isMergeNode() && !to->isTrivial())) {
    // dst has more than one predecessor
    if (!to->hasMapping()) {
      // first jump to dst, use current mapping, must be injective
      _currentMapping->makeInjective(); // may generate code => must be applied to current mapping
      PRegMapping* copy = new PRegMapping(_currentMapping);
      // eliminate PRegs that are not alive anymore at dst
      if (isLiveRangeBoundary(from, to)) copy->killDeadsAt(to);
      to->setMapping(copy);
      _masm->jcc(cc, to->label);
    } else {
      // not the first mapping
      PRegMapping* copy = new PRegMapping(_currentMapping);
      if (isLiveRangeBoundary(from, to)) copy->killDeadsAt(to);
      if (copy->isConformant(to->mapping())) {
        // everythink ok, simply jump (must use a copy with dead PRegs removed for comparison)
	_masm->jcc(cc, to->label);
      } else {
        // must make mappings conformant, use stub routine
	_mergeStubs.push(Stub::new_jcc_stub(copy, to, cc));
      }
    }
  } else {
    // dst has exactly one predecessor
    assert(!to->hasMapping(), "more than one predecessor?");
    PRegMapping* copy = new PRegMapping(_currentMapping);
    if (isLiveRangeBoundary(from, to)) copy->killDeadsAt(to);
    to->setMapping(copy);
    _masm->jcc(cc, to->label);
  }
}


void CodeGenerator::bindLabel(Node* node) {
  if (_currentMapping == NULL) {
    // continue with mapping at node, live ranges already adjusted
    assert(node->hasMapping(), "must have a mapping");
    setMapping(node->mapping());
  } else {
    // current mapping exists
    if (!node->hasMapping()) {
      // node is approached the first time
      if (node->isMergeNode() && !node->isTrivial()) {
        // more than one predecessor => store injective version of current mapping at node
        // (if only one predecessor => simply continue to use the current mapping)
	if (_previousNode != NULL && isLiveRangeBoundary(_previousNode, node)) _currentMapping->killDeadsAt(node);
	_currentMapping->makeInjective();
        node->setMapping(_currentMapping);
      }
    } else {
      // merge current mapping with node mapping
      if (_previousNode != NULL && isLiveRangeBoundary(_previousNode, node)) _currentMapping->killDeadsAt(node);
      _currentMapping->makeConformant(node->mapping());
      setMapping(node->mapping());
    }
  }
  assert(_currentMapping != NULL, "must have a mapping");
  _masm->bind(node->label);
}


void CodeGenerator::inlineCache(Node* call, MergeNode* nlrTestPoint, int flags) {
  assert(_currentMapping != NULL, "mapping must exist");
  assert(call->scope() == nlrTestPoint->scope(), "should be in the same scope");
  // make mappings conformant if necessary
  if (nlrTestPoint->isMergeNode() && !nlrTestPoint->isTrivial()) {
    // dst has more than one predecessor
    if (!nlrTestPoint->hasMapping()) {
      // first jump to dst, use current mapping, must be injective
      PRegMapping* copy = new PRegMapping(_currentMapping);
      if (isLiveRangeBoundary(call, nlrTestPoint)) copy->killDeadsAt(nlrTestPoint);
      assert(_currentMapping->isInjective(), "must be injective");
      copy->acquireNLRRegisters();
      nlrTestPoint->setMapping(copy);
      _masm->ic_info(nlrTestPoint->label, flags);
    } else {
      // not the first mapping
      PRegMapping* copy = new PRegMapping(_currentMapping);
      if (isLiveRangeBoundary(call, nlrTestPoint)) copy->killDeadsAt(nlrTestPoint);
      copy->acquireNLRRegisters();
      if (copy->isConformant(nlrTestPoint->mapping())) {
        // everything ok, simply jump (must use a copy with dead PRegs removed for comparison)
	_masm->ic_info(nlrTestPoint->label, flags);
      } else {
        // must make mappings conformant, use stub routine
	_mergeStubs.push(Stub::new_NLR_stub(copy, nlrTestPoint, flags));
      }
    }
  } else {
    // dst has exactly one predecessor
    assert(!nlrTestPoint->hasMapping(), "more than one predecessor?");
    PRegMapping* copy = new PRegMapping(_currentMapping);
    if (isLiveRangeBoundary(call, nlrTestPoint)) copy->killDeadsAt(nlrTestPoint);
    copy->acquireNLRRegisters();
    nlrTestPoint->setMapping(copy);
    _masm->ic_info(nlrTestPoint->label, flags);
  }
}


void CodeGenerator::generateMergeStubs() {
  char* start_pc = _masm->pc();
  while (_mergeStubs.nonEmpty()) _mergeStubs.pop()->generateMergeStub();
  if (PrintCodeGeneration && _masm->pc() > start_pc) {
    std->print("---\n");
    std->print("fixup merge stubs\n");
    _masm->code()->decode();
  }
}


// Code generation for statistical information on nmethods

char* CodeGenerator::nmethodAddress() const {
  // hack to compute hypothetical nmethod address
  // should be fixed at some point
  return (char*)(((nmethod*) (_masm->code()->code_begin())) - 1);
}


void CodeGenerator::incrementInvocationCounter() {
  // Generates code to increment the nmethod execution counter
  char* addr = nmethodAddress() + nmethod::invocationCountOffset();
  _masm->incl(Address(int(addr), relocInfo::internal_word_type));
}


// Initialization / Finalization

void CodeGenerator::initialize(InlinedScope* scope) {
  // This routine is called at the very beginning of code generation
  // for one nmethod (after creation of the CodeGenerator of course).
  // It sets up the initial mapping and thus makes sure that the correct
  // debugging information is written out before the code generation for
  // the PrologueNode.

  // setup arguments
  int i;
  for (i = 0; i < scope->nofArguments(); i++) {
    _currentMapping->mapToArgument(scope->argument(i)->preg(), i);
  }
  // setup temporaries (finalize() generates initialization code)
  for (i = 0; i < scope->nofTemporaries(); i++) {
    _currentMapping->mapToTemporary(scope->temporary(i)->preg(), i);
  }
  // setup receiver
  _currentMapping->mapToRegister(scope->self()->preg(), self_reg);
}


void CodeGenerator::finalize(InlinedScope* scope) {
  // first generate stubs if there are any
  generateMergeStubs();

  // patch 'initialize locals' code
  int n = maxNofStackTmps();
  int frame_size = 2 + n;	// return address, old ebp + stack temps
  // make sure frame is big enough for deoptimization
  if (frame_size < minimum_size_for_deoptimized_frame) {
    // add the difference to
    n += minimum_size_for_deoptimized_frame - frame_size;
  }

  Assembler masm(_pushCode);
  if (_pushCode->code_begin() + n <= _pushCode->code_limit()) {
    while (n-- > 0) masm.pushl(_nilReg);
  } else {
    masm.jmp(_masm->pc(), relocInfo::none);
    while (n-- > 0) _masm->pushl(_nilReg);
    _masm->jmp(_pushCode->code_limit(), relocInfo::none);
  }

   // store nofCompilations at end of code for easier debugging
  if (CompilerDebug) _masm->movl(eax, nofCompilations);

  if (PrintCodeGeneration) {
    std->print("---\n");
    std->print("merge stubs\n");
    _masm->code()->decode();
    std->print("---\n");
  }
}


/*
void CodeGenerator::finalize(InlinedScope* scope) {
  // This routine is called at the very end of code generation for one
  // nmethod; it provides the entry points & sets up the stack frame
  // (i.e., this is the first code executed when entering a nmethod).
  // Note: _currentMapping is not used since this is one fixed code pattern.

  // first generate stubs if there are any
  generateMergeStubs();

  // set unverified entry point
  _masm->align(oopSize);
  theCompiler->set_entry_point_offset(_masm->offset());

  // verify receiver
  if (scope->isMethodScope()) {
    // check class
    klassOop klass = scope->selfKlass();
    if (klass == smiKlassObj) {
      // receiver must be a smi, check smi tag only
      _masm->testl(self_reg, Mem_Tag);			// testl instead of test => no alignment nop's needed later
      _masm->jcc(Assembler::notZero, CompiledIC::normalLookupRoutine());
    } else {
      assert(self_reg != temp1, "choose another register");
      _masm->testl(self_reg, Mem_Tag);			// testl instead of test => no alignment nop's needed later
      _masm->jcc(Assembler::zero, CompiledIC::normalLookupRoutine());
      _masm->cmpl(Address(self_reg, memOopDesc::klass_byte_offset()), klass);
      _masm->jcc(Assembler::notEqual, CompiledIC::normalLookupRoutine());
    }
  } else {
    // If this is a block method and we expect a context
    // then the incoming context chain must be checked.
    // The context chain may contain a deoptimized contextOop.
    // (see StubRoutines::verify_context_chain for details)
    if (scope->method()->block_info() == methodOopDesc::expects_context) {
      const bool use_fast_check = false;
      if (use_fast_check) {
        // look in old backend for this code
        Unimplemented();
      } else {
        _masm->call(StubRoutines::verify_context_chain(), relocInfo::runtime_call_type);
      }
    }
  }

  // set verified entry point (for callers who know the receiver is correct)
  _masm->align(oopSize);
  theCompiler->set_verified_entry_point_offset(_masm->offset());

  // build stack frame & initialize locals
  _masm->enter();
  int n = maxNofStackTmps();
  int frame_size = 2 + n;	// return address, old ebp + stack temps
  // make sure frame is big enough for deoptimization
  if (frame_size < minimum_size_for_deoptimized_frame) {
    // add the difference to
    n += minimum_size_for_deoptimized_frame - frame_size;
  }
  if (n == 1) {
    _masm->pushl(nilObj);
  } else if (n > 1) {
    _masm->movl(temp1, nilObj);
    while (n-- > 0) _masm->pushl(temp1);
  }

  // increment invocation counter & check for overflow (trigger recompilation)
  Label recompile_stub_call;
  if (RecompilationPolicy::needRecompileCounter(theCompiler)) {
    char* addr = nmethodAddress() + nmethod::invocationCountOffset();
    _masm->movl(temp1, Address(int(addr), relocInfo::internal_word_type));
    _masm->incl(temp1);
    _masm->cmpl(temp1, theCompiler->get_invocation_counter_limit());
    _masm->movl(Address(int(addr), relocInfo::internal_word_type), temp1);
    _masm->jcc(Assembler::greaterEqual, recompile_stub_call);
    //
    // need to fix this:
    // 1. put call to recompiler at end (otherwise we cannot provide debugging info easily)
    // 2. check if everything is still ok (e.g. does the recompiler call ever return? if not, no jump needed)
    // 3. check recompiler call stub routine (should not setup stack frame because registers cannot be seen!) - seems to be fixed
  }

  // jump to start of code

  // call to recompiler - if the nmethod turns zombie, this will be overwritten by a call to the zombie handler
  // (see also comment in nmethod)
  _masm->bind(recompile_stub_call);
  // write debug info
  theCompiler->set_special_handler_call_offset(theMacroAssm->offset());
  _masm->call(StubRoutines::recompile_stub_entry(), relocInfo::runtime_call_type);

  
  // store nofCompilations at end of code for easier debugging
  if (CompilerDebug) _masm->movl(eax, nofCompilations);

  if (PrintCodeGeneration) {
    std->print("---\n");
    std->print("entry point\n");
    _masm->code()->decode();
    std->print("---\n");
  }
}
*/


void CodeGenerator::zapContext(PReg* context) {
  _masm->movl(Address(use(context), contextOopDesc::parent_byte_offset()), 0);
}


void CodeGenerator::storeCheck(Register obj) {
  // Does a store check for the oop in register obj.
  //
  // Note: Could be optimized by hardwiring the byte map base address
  // in the code - however relocation would be necessary whenever the
  // base changes. Advantage: only one instead of two instructions.
  Temporary base(_currentMapping);
  Temporary indx(_currentMapping);
  _masm->movl(base.reg(), Address(int(&byte_map_base), relocInfo::external_word_type));
  _masm->movl(indx.reg(), obj);						// do not destroy obj (a preg may be mapped to it)
  _masm->shrl(indx.reg(), card_shift);					// divide obj by card_size
  _masm->movb(Address(base.reg(), indx.reg(), Address::times_1), 0);	// clear entry
}


void CodeGenerator::assign(PReg* dst, PReg* src, bool needsStoreCheck) {
  PRegLocker lock(src);		// make sure src stays in register if it's in a register
  enum { is_const, is_loaded, is_mapped, is_undefined } state = is_undefined;
  oop value;			// valid if state == is_const
  Register reg;			// valid if state == is_loaded
  PReg* preg;			// valid if state == is_mapped

  { Temporary t1(_currentMapping, NLR_result_reg);
    if (t1.reg() != NLR_result_reg) {
      reg = t1.reg();
    } else {
      Temporary t2(_currentMapping);
      reg = t2.reg();
    }
  }
  Temporary t(_currentMapping, reg);
  assert(reg != NLR_result_reg, "fix this");
  assert(t.reg() == reg, "should be the same");

  // get/load source
  if (src->isConstPReg()) {
    value = ((ConstPReg*)src)->constant;
    state = is_const;
  } else if (src->loc == resultOfNLR) {
    _currentMapping->mapToRegister(src, NLR_result_reg);
    preg = src;
    state = is_mapped;
  } else if (src->loc.isContextLocation()) {
    PReg* context = theCompiler->contextList->at(src->loc.contextNo())->context();
    Address addr = Address(use(context), Mapping::contextOffset(src->loc.tempNo()));
    _masm->movl(reg, addr);
    state = is_loaded;
  } else {
    assert(!src->loc.isSpecialLocation(), "what's this?");
    preg = src;
    state = is_mapped;
  }

  // map/store to dest
  if (dst->loc == topOfStack) {
    switch (state) {
      case is_const : _masm->pushl(value);		break;
      case is_loaded: _masm->pushl(reg);		break;
      case is_mapped: _masm->pushl(use(preg));		break;
      default       : ShouldNotReachHere();
    }
  } else if (dst->loc.isContextLocation()) {
    PReg* context = theCompiler->contextList->at(dst->loc.contextNo())->context();
    PRegLocker lock(context);
    Address addr = Address(use(context), Mapping::contextOffset(dst->loc.tempNo()));
    switch (state) {
      case is_const : _masm->movl(addr, value);		break;
      case is_loaded: _masm->movl(addr, reg);		break;
      case is_mapped: _masm->movl(addr, use(preg));	break;
      default       : ShouldNotReachHere();
    }
    if (needsStoreCheck) storeCheck(use(context));
  } else {
    assert(!dst->loc.isSpecialLocation(), "what's this?");
    switch (state) {
      case is_const : _masm->movl(def(dst), value);	break;
      case is_loaded: _masm->movl(def(dst), reg);	break;
      case is_mapped: _currentMapping->move(dst, preg);	break;
      default       : ShouldNotReachHere();
    }
  }
}


// Debugging

static int _callDepth		= 0;
static int _numberOfCalls	= 0;
static int _numberOfReturns	= 0;
static int _numberOfNLRs	= 0;


void CodeGenerator::indent() {
  const int maxIndent = 40;
  if (_callDepth <= maxIndent) {
    std->print("%*s", _callDepth, "");
  } else {
    std->print("%*d: ", maxIndent-2, _callDepth);
  }
}


char* CodeGenerator::nmethodName() {
  deltaVFrame* f = DeltaProcess::active()->last_delta_vframe();
  return f->method()->selector()->as_string();
}


void CodeGenerator::verifyObj(oop obj) {
  if (!obj->is_smi() && !obj->is_mem()) fatal("should be an ordinary oop");
  klassOop klass = obj->klass();
  if (klass == NULL || !klass->is_mem()) fatal("should be an ordinary memOop");
  if (obj->is_block()) blockClosureOop(obj)->verify();
}


void CodeGenerator::verifyContext(oop obj) {
  if (obj->is_mark()) error("context should never be mark");
  if (!Universe::is_heap((oop*)obj)) error("context outside of heap");
  if (!obj->is_context()) error("should be a context");
  oop c = (oop)(contextOop(obj)->parent());
  if (c->is_mem()) verifyContext(c);
}


void CodeGenerator::verifyArguments(oop recv, oop* ebp, int nofArgs) {
  bool print_args_long = true;
  ResourceMark rm;
  _numberOfCalls++;
  _callDepth++;
  if (TraceCalls) { 
    ResourceMark rm;
    indent(); std->print("( %s %s ", recv->print_value_string(), nmethodName());
  }
  verifyObj(recv);
  int i = nofArgs;
  oop* arg = ebp + (nofArgs + 2);
  while (i-- > 0) {
    arg--;
    verifyObj(*arg);
    if (TraceCalls) {
      ResourceMark rm;
      if (print_args_long || (*arg)->is_smi()) {
        std->print("%s ", (*arg)->print_value_string());
      } else {
        std->print("0x%x ", *arg);
      }
    }
  }
  if (TraceCalls) std->cr();
  if (VerifyDebugInfo) { 
    deltaVFrame* f = DeltaProcess::active()->last_delta_vframe();
    while (f != NULL) {
      f->verify_debug_info();
      f = f->sender_delta_frame();
    }
  }
}


void CodeGenerator::verifyReturn(oop result) {
  _numberOfReturns++;
  result->verify();
  if (TraceCalls) {
    ResourceMark rm;
    indent(); std->print(") %s -> %s\n", nmethodName(), result->print_value_string());
  }
  _callDepth--;
}


void CodeGenerator::verifyNLR(char* fp, char* nlrFrame, int nlrScopeID, oop result) {
  _numberOfNLRs++;
  LOG_EVENT3("verifyNLR(%#x, %#x, %d, %#x)", fp, nlrFrame, result);
  if (nlrFrame <= fp) error("NLR went too far: %#x <= %#x", nlrFrame, fp);
  // treat >99 scopes as likely error -- might actually be ok
  if (nlrScopeID < 0 || nlrScopeID > 99) error("illegal NLR scope ID %#x", nlrScopeID);
  if (result->is_mark()) error("NLR result is a markOop");
  result->verify();
  if (TraceCalls) {
    ResourceMark rm;
    indent(); std->print(") %s  ^ %s\n", nmethodName(), result->print_value_string());
  }
  _callDepth--;
}


void CodeGenerator::callVerifyObj(Register obj) {
  // generates transparent check code which verifies that obj is
  // a legal oop and halts if not - for debugging purposes only
  if (!VerifyCode) warning(": verifyObj should not be called");
  _masm->pushad();
  _masm->call_C((char*)CodeGenerator::verifyObj, obj);
  _masm->popad();
}


void CodeGenerator::callVerifyContext(Register context) {
  // generates transparent check code which verifies that context is
  // a legal context and halts if not - for debugging purposes only
  if (!VerifyCode) warning(": verifyContext should not be called");
  _masm->pushad();
  _masm->call_C((char*)CodeGenerator::verifyContext, context);
  _masm->popad();
}


void CodeGenerator::callVerifyArguments(Register recv, int nofArgs) {
  // generates transparent check code which verifies that all arguments
  // are legal oops and halts if not - for debugging purposes only
  if (!VerifyCode && !TraceCalls && !TraceResults) warning(": performance bug: verifyArguments should not be called");
  assert(recv != temp1, "use another temporary register");
  _masm->pushad();
  _masm->movl(temp1, nofArgs);
  _masm->call_C((char*)CodeGenerator::verifyArguments, recv, ebp, temp1);
  _masm->popad();
}


void CodeGenerator::callVerifyReturn() {
  // generates transparent check code which verifies that result contains
  // a legal oop and halts if not - for debugging purposes only
  if (!VerifyCode && !TraceCalls && !TraceResults) warning(": verifyReturn should not be called");
  _masm->pushad();
  _masm->call_C((char*)CodeGenerator::verifyReturn, result_reg);
  _masm->popad();
}


void CodeGenerator::callVerifyNLR() {
  // generates transparent check code which verifies NLR check & continuation
  if (!VerifyCode && !TraceCalls && !TraceResults) warning(": verifyNLR should not be called");
  _masm->pushad();
  _masm->call_C((char*)CodeGenerator::verifyNLR, ebp, NLR_home_reg, NLR_homeId_reg, NLR_result_reg);
  _masm->popad();
}


// Basic blocks

static bool bb_needs_jump;
// true if basic block needs a jump at the end to its successor, false otherwise
// Note: most gen() nodes with more than one successor are implemented such that
//       next() is the fall-through case. If that's not the case, an extra jump
//       has to be generated (via endOfBasicBlock()). However, some of the nodes
//       do explicit jumps to all successors to accomodate for arbitrary node
//       reordering, in which case they may set the flag to false (it is auto-
//       matically set to true for each node).
//
// This flag should go away at soon as all node with more than one exit are
// implemented correctly (i.e., do all the jumping themselves).


void CodeGenerator::beginOfBasicBlock(Node* node) {
  if (PrintCodeGeneration && WizardMode) std->print("--- begin of basic block (N%d) ---\n", node->id());
  bindLabel(node);
}


void CodeGenerator::endOfBasicBlock(Node* node) {
  if (bb_needs_jump && node->next() != NULL) {
    Node* from = node;
    Node* to   = node->next();
    if (PrintCodeGeneration) {
      std->print("branch from N%d to N%d\n", from->id(), to->id());
      if (PrintPRegMapping) _currentMapping->print();
    }
    jmp(from, to);
    _previousNode = NULL;
    if (PrintCodeGeneration) _masm->code()->decode();
  }

  if (PrintCodeGeneration && WizardMode) std->print("--- end of basic block (N%d) ---\n", node->id());
}


void CodeGenerator::updateDebuggingInfo(Node* node) {
  ScopeDescRecorder* rec = theCompiler->scopeDescRecorder();
  int pc_offset = assembler()->offset();
  rec->addPcDesc(pc_offset, node->scope()->scopeInfo(), node->bci());
  _debugInfoWriter->write_debug_info(_currentMapping, pc_offset);
}


// For all nodes
void CodeGenerator::beginOfNode(Node* node) {
  assert(_currentMapping != NULL, "must have a valid mapping");
  // adjust mapping to liveness of PRegs
  if (_previousNode != NULL && isLiveRangeBoundary(_previousNode, node)) _currentMapping->killDeadsAt(node);
  _currentMapping->cleanupContextReferences();
  // adjust debugging information if desired (e.g., when using disassembler with full symbolic support)
  if (GenerateFullDebugInfo) updateDebuggingInfo(node);
  // debugging
  if (PrintCodeGeneration) {
    std->print("---\n");
    std->print("N%d: ", node->id());
    node->print();
    std->print(" (bci = %d)\n", node->bci());
    if (PrintPRegMapping) _currentMapping->print();
  }
  bb_needs_jump = true;
};


void CodeGenerator::endOfNode(Node* node) {
  if (PrintCodeGeneration) _masm->code()->decode();
  // if _currentMapping == NULL there's no previous node & the next node will be
  // reached via a jump and it's mapping is already set up the right way
  // (i.e., no PReg killing required => set _previousNode to NULL)
  _previousNode = _currentMapping == NULL ? NULL : node;
};


// Individual nodes

void CodeGenerator::aPrologueNode(PrologueNode* node) {
  // set unverified entry point
  _masm->align(oopSize);
  theCompiler->set_entry_point_offset(_masm->offset());

  // verify receiver
  InlinedScope* scope = node->scope();
  PReg* recv = scope->self()->preg();
  PRegLocker lock(recv);
  if (scope->isMethodScope()) {
    // check class
    klassOop klass = scope->selfKlass();
    if (klass == smiKlassObj) {
      // receiver must be a smi, check smi tag only
      _masm->test(use(recv), Mem_Tag);
      _masm->jcc(Assembler::notZero, CompiledIC::normalLookupRoutine());
    } else {
      _masm->test(use(recv), Mem_Tag);
      _masm->jcc(Assembler::zero, CompiledIC::normalLookupRoutine());
      _masm->cmpl(Address(use(recv), memOopDesc::klass_byte_offset()), klass);
      _masm->jcc(Assembler::notEqual, CompiledIC::normalLookupRoutine());
    }
  } else {
    // If this is a block method and we expect a context
    // then the incoming context chain must be checked.
    // The context chain may contain a deoptimized contextOop.
    // (see StubRoutines::verify_context_chain for details)
    if (scope->method()->block_info() == methodOopDesc::expects_context) {
      const bool use_fast_check = false;
      if (use_fast_check) {
        // look in old backend for this code
        Unimplemented();
      } else {
        _masm->call(StubRoutines::verify_context_chain(), relocInfo::runtime_call_type);
      }
    }
  }

  // set verified entry point (for callers who know the receiver is correct)
  _masm->align(oopSize);
  theCompiler->set_verified_entry_point_offset(_masm->offset());

  // build stack frame & initialize locals
  _masm->enter();
  { Temporary t(_currentMapping);
    _masm->movl(t.reg(), Universe::nilObj());
    _nilReg = t.reg();
    char* beg = _masm->pc();
    int i = 10;
    while (i-- > 0) _masm->nop();
    char* end = _masm->pc();
    _pushCode = new CodeBuffer(beg, end - beg);
  }

  if (scope->isBlockScope()) {
    // initialize context for blocks; recv is block closure => get context out of it
    // and store it in self & temp0 (which holds the context in the interpreter model).
    // Note: temp0 has been mapped to the stack when setting up temporaries.
    assert(scope->context() == scope->temporary(0)->preg(), "should be the same");
    Register reg = use(recv);
    _masm->movl(def(recv), Address(reg, blockClosureOopDesc::context_byte_offset()));
    assign(scope->context(), recv);
  }
  // debugging
  if (VerifyCode || VerifyDebugInfo || TraceCalls) callVerifyArguments(use(recv), scope->method()->number_of_arguments());

  // increment invocation counter & check for overflow (trigger recompilation)
  Label recompile_stub_call;
  if (RecompilationPolicy::needRecompileCounter(theCompiler)) {
    char* addr = nmethodAddress() + nmethod::invocationCountOffset();
    _masm->movl(temp1, Address(int(addr), relocInfo::internal_word_type));
    _masm->incl(temp1);
    _masm->cmpl(temp1, theCompiler->get_invocation_counter_limit());
    _masm->movl(Address(int(addr), relocInfo::internal_word_type), temp1);
    _masm->jcc(Assembler::greaterEqual, recompile_stub_call);
    //
    // need to fix this:
    // 1. put call to recompiler at end (otherwise we cannot provide debugging info easily)
    // 2. check if everything is still ok (e.g. does the recompiler call ever return? if not, no jump needed)
    // 3. check recompiler call stub routine (should not setup stack frame because registers cannot be seen!) - seems to be fixed
  }
  Label start;
  _masm->jmp(start);

  // call to recompiler - if the nmethod turns zombie, this will be overwritten
  // by a call to the zombie handler (see also comment in nmethod)
  _masm->bind(recompile_stub_call);
  updateDebuggingInfo(node);
  theCompiler->set_special_handler_call_offset(theMacroAssm->offset());
  _masm->call(StubRoutines::recompile_stub_entry(), relocInfo::runtime_call_type);

  _masm->bind(start);
}


void CodeGenerator::aLoadIntNode(LoadIntNode* node) {
  _masm->movl(def(node->dst()), node->value());
}


void CodeGenerator::aLoadOffsetNode(LoadOffsetNode* node) {
  PRegLocker lock(node->base(), node->dst());
  _masm->movl(def(node->dst()), Address(use(node->base()), byteOffset(node->offset)));
}


int CodeGenerator::byteOffset(int offset) {
  // Computes the byte offset from the beginning of an oop
  assert(offset >= 0, "wrong offset");
  return offset*oopSize - Mem_Tag;
}


void CodeGenerator::uplevelBase(PReg* startContext, int nofLevels, Register base) {
  // Compute uplevel base into register base; nofLevels is number of indirections (0 = in this context).
  _masm->movl(base, use(startContext));
  if (VerifyCode) callVerifyContext(base);
  while (nofLevels-- > 0) _masm->movl(base, Address(base, contextOopDesc::parent_byte_offset()));
}


void CodeGenerator::aLoadUplevelNode(LoadUplevelNode* node) {
  PRegLocker lock(node->context0());
  Temporary base(_currentMapping);
  uplevelBase(node->context0(), node->nofLevels(), base.reg());
  Register dst = def(node->dst());
  _masm->movl(dst, Address(base.reg(), byteOffset(node->offset())));
  if (VerifyCode) callVerifyObj(dst);
}


void CodeGenerator::anAssignNode(AssignNode* node) {
  assign(node->dst(), node->src());
}


void CodeGenerator::aStoreOffsetNode(StoreOffsetNode* node) {
  PRegLocker lock(node->base(), node->src());
  Register base = use(node->base());
  _masm->movl(Address(base, byteOffset(node->offset())), use(node->src()));
  if (node->needsStoreCheck()) storeCheck(base);
}


void CodeGenerator::aStoreUplevelNode(StoreUplevelNode* node) {
  PRegLocker lock(node->context0(), node->src());
  Temporary base(_currentMapping);
  uplevelBase(node->context0(), node->nofLevels(), base.reg());
  _masm->movl(Address(base.reg(), byteOffset(node->offset())), use(node->src()));
  if (node->needsStoreCheck()) storeCheck(base.reg());
}


void CodeGenerator::moveConstant(ArithOpCode op, PReg*& x, PReg*& y, bool& x_attr, bool& y_attr) {
  if (x->isConstPReg() && ArithOpIsCommutative[op]) {
    PReg* t1 = x     ; x      = y     ; y      = t1;
    bool  t2 = x_attr; x_attr = y_attr; y_attr = t2;
  }
}


void CodeGenerator::arithRROp(ArithOpCode op, Register x, Register y) { // x := x op y
  assert(Int_Tag == 0, "check this code");
  switch (op) {
    case TestArithOp  : _masm->testl(x, y);		break;
    case tAddArithOp  : // fall through
    case  AddArithOp  : _masm->addl(x, y);		break;
    case tSubArithOp  : // fall through
    case  SubArithOp  : _masm->subl(x, y);		break;
    case tMulArithOp  : _masm->sarl(x, Tag_Size);
    case  MulArithOp  : _masm->imull(x, y);		break;
    case tDivArithOp  : // fall through
    case  DivArithOp  : Unimplemented();		break;
    case tModArithOp  : // fall through
    case  ModArithOp  : Unimplemented();		break;
    case tAndArithOp  : // fall through
    case  AndArithOp  : _masm->andl(x, y);		break;
    case tOrArithOp   : // fall through
    case  OrArithOp   : _masm->orl(x, y);		break;
    case tXOrArithOp  : // fall through
    case  XOrArithOp  : _masm->xorl(x, y);		break;
    case tShiftArithOp: Unimplemented();
    case  ShiftArithOp: Unimplemented();
    case tCmpArithOp  : // fall through
    case  CmpArithOp  : _masm->cmpl(x, y);		break;
    default: ShouldNotReachHere();
  }
}


void CodeGenerator::arithRCOp(ArithOpCode op, Register x, int y) { // x := x op y
  assert(Int_Tag == 0, "check this code");
  switch (op) {
    case TestArithOp  : _masm->testl(x, y);		break;
    case tAddArithOp  : // fall through
    case  AddArithOp  :
      if (y == 0) {
        warning("code generated to add 0 (no load required)");
      } else {
        _masm->addl(x, y);
      }
      break;
    case tSubArithOp  : // fall through
    case  SubArithOp  :
      if (y == 0) {
        warning("code generated to subtract 0 (no load required)");
      } else {
        _masm->subl(x, y);
      }
      break;
    case tMulArithOp  : y = arithmetic_shift_right(y, Tag_Size);
    case  MulArithOp  :
      // catch a few trivial cases (since certain optimizations happen
      // after inlining of primitives, these cases cannot be handled in
      // the primitive inliner alone => phase ordering problem).
      // Note that overflow check must still remain possible (i.e.,
      // cannot easily substitute *4 with 2 adds without saving CC).
      switch (y) {
        case -1:
          _masm->negl(x);
	  break;
	case  0:
	  warning("code generated to multiply with 0 (no load required)");
          _masm->xorl(x, x);
	  break;
	case  1:
	  warning("code generated to multiply with 1 (no load required)");
          // do nothing
	  break;
	case  2:
	  _masm->addl(x, x);
	  break;
	default:
	  _masm->imull(x, x, y);
	  break;
      }
      break;
    case tDivArithOp  : // fall through
    case  DivArithOp  : Unimplemented();		break;
    case tModArithOp  : // fall through
    case  ModArithOp  : Unimplemented();		break;
    case tAndArithOp  : // fall through
    case  AndArithOp  : _masm->andl(x, y);		break;
    case tOrArithOp   : // fall through
    case  OrArithOp   : _masm->orl(x, y);		break;
    case tXOrArithOp  : // fall through
    case  XOrArithOp  : _masm->xorl(x, y);		break;
    case tShiftArithOp:
      if (y < 0) {
        // shift right
        int shift_count = ((-y) >> Tag_Size) % 32;
        _masm->sarl(x, shift_count);
        _masm->andl(x, -1 << Tag_Size);			// clear Tag bits
      } else if (y > 0) {
        // shift left
        int shift_count = ((+y) >> Tag_Size) % 32;
        _masm->shll(x, shift_count);
      }
      break;
    case  ShiftArithOp: Unimplemented();
    case tCmpArithOp  : // fall through
    case  CmpArithOp  : _masm->cmpl(x, y);		break;
    default: ShouldNotReachHere();
  }
}


void CodeGenerator::arithROOp(ArithOpCode op, Register x, oop y) { // x := x op y
  assert(!y->is_smi(), "check this code");
  switch (op) {
    case  CmpArithOp  : _masm->cmpl(x, y);		break;
    default           : ShouldNotReachHere();
  }
}


void CodeGenerator::arithRXOp(ArithOpCode op, Register x, oop y) { // x := x op y
  if (y->is_smi()) {
    arithRCOp(op, x, int(y));				// y is smiOop -> needs no relocation info
  } else {
    arithROOp(op, x, y);
  }
}


bool CodeGenerator::producesResult(ArithOpCode op) {
  return (op != TestArithOp) && (op != CmpArithOp) && (op != tCmpArithOp);
}


Register CodeGenerator::targetRegister(ArithOpCode op, PReg* z, PReg* x) {
  assert(PRegLocker::locks(z) && PRegLocker::locks(x), "should be locked");
  Register reg = noreg;
  if (producesResult(op)) {
    // result produced -> use a copy of x as register for z
    Register x_reg = use(x);
    // x is guaranteed to be in a register
    if (_currentMapping->onStack(x)) {
      // x is also on stack -> release register location from mapping and use it as copy
      _currentMapping->killRegister(x);
      reg = _currentMapping->def(z, x_reg);
    } else {
      // preg is only in register -> need to allocate a new register & copy it explicitly
      reg = def(z);
      _masm->movl(reg, x_reg);
    }
  } else {
    // no result produced -> can use x directly as register for z
    reg = use(x);
  }
  return reg;
}


void CodeGenerator::anArithRRNode(ArithRRNode* node) {
  ArithOpCode op = node->op();
  PReg* z = node->dst();
  PReg* x = node->src();
  PReg* y = node->operand();
  bool dummy;
  moveConstant(op, x, y, dummy, dummy);
  PRegLocker lock(z, x, y);
  Register reg = targetRegister(op, z, x);
  if (y->isConstPReg()) {
    arithRXOp(op, reg, ((ConstPReg*)y)->constant);
  } else {
    arithRROp(op, reg, use(y));
  }
}


void CodeGenerator::anArithRCNode(ArithRCNode* node) {
  ArithOpCode op = node->op();
  PReg* z = node->dst();
  PReg* x = node->src();
  int   y = node->operand();
  PRegLocker lock(z, x);
  Register reg = targetRegister(op, z, x);
  arithRCOp(op, reg, y);
}


void CodeGenerator::aTArithRRNode(TArithRRNode* node) {
  ArithOpCode op = node->op();
  PReg* z = node->dst();
  PReg* x = node->src();
  PReg* y = node->operand();
  bool x_is_int = node->arg1IsInt();
  bool y_is_int = node->arg2IsInt();
  moveConstant(op, x, y, x_is_int, y_is_int);
  PRegLocker lock(z, x, y);
  Register tags = noreg;
  if (x_is_int) {
    if (y_is_int) {
      // both x & y are smis => no tag check necessary
    } else {
      // x is smi => check y
      tags = use(y);
    }
  } else {
    if (y_is_int) {
      // y is smi => check x
      tags = use(x);
    } else {
      // check both x & y
      Temporary t(_currentMapping);
      tags = t.reg();
      _masm->movl(tags, use(x));
      _masm->orl (tags, use(y));
    }
  }
  if (tags != noreg) {
    // check tags
    _masm->test(tags, Mem_Tag);
    jcc(Assembler::notZero, node, node->next(1));
  }
  Register reg = targetRegister(op, z, x);
  if (y->isConstPReg()) {
    arithRXOp(op, reg, ((ConstPReg*)y)->constant);
  } else {
    arithRROp(op, reg, use(y));
  }
}


void CodeGenerator::aFloatArithRRNode(FloatArithRRNode* node) {
  Unimplemented();
}


void CodeGenerator::aFloatUnaryArithNode(FloatUnaryArithNode* node) {
  Unimplemented();
}


void CodeGenerator::aContextCreateNode(ContextCreateNode* node) {
  // node->dst() has been pre-allocated (temp0) in the prologue node -> remove it from
  // mapping. Note that in cases where there's an incoming context (which serves as parent (node->src())),
  // node->src() and node->dst() differ because the NodeBuilder allocates a new SAPReg in this case.
  assert(node->src() != node->dst(), "should not be the same");
  assert(node->dst() == node->scope()->context(), "should assign to scope context");
  _currentMapping->kill(node->dst());		// kill it so that aPrimNode(node) can map the result to it
  switch (node->sizeOfContext()) {
    case 0 : // fall through for now - fix this
    case 1 : // fall through for now - fix this
    case 2 : // fall through for now - fix this
    default:
      _masm->pushl(int(as_smiOop(node->nofTemps())));
      aPrimNode(node);
      _masm->addl(esp, oopSize);	// pop argument, this is not a Pascal call - change this as some point?
      break;
  }
  PRegLocker lock(node->dst());		// once loaded, make sure context stays in register
  Register context_reg = use(node->dst());
  if (node->src() == NULL) {
    assert(node->scope()->isMethodScope() || node->scope()->method()->block_info() == methodOopDesc::expects_nil, "inconsistency");
    _masm->movl(Address(context_reg, contextOopDesc::parent_byte_offset()), NULL);
    // NULL for now; the interpreter uses nil. However, some of the
    // context verification code called from compiled code checks for
    // parents that are either a frame pointer, NULL or a context.
    // This should be unified at some point. (gri 5/9/96)
  } else if (_currentMapping->isDefined(node->src())) {
    // node->src() holds incoming context as parent and has been defined before
    _masm->movl(Address(context_reg, contextOopDesc::parent_byte_offset()), use(node->src()));
  } else {
    // node->src()->loc is pointing to the current stack frame (method context) and has not been explicitly defined
    assert(node->src()->loc == frameLoc, "parent should point to current stack frame");
    _masm->movl(Address(context_reg, contextOopDesc::parent_byte_offset()), frame_reg);
  }
  storeCheck(context_reg);
}


void CodeGenerator::aContextInitNode(ContextInitNode* node) {
  // initialize context temporaries (parent has been initialized in the ContextCreateNode)
  for (int i = node->nofTemps(); i-- > 0; ) {
    PReg* src = node->initialValue(i)->preg();
    PReg* dst;
    if (src->isBlockPReg()) {
      // Blocks aren't actually assigned (at the PReg level) so that the inlining info isn't lost.
      if (node->wasEliminated()) {
        continue;				// there's no assignment (context was eliminated)
      } else {
	dst = node->contextTemp(i)->preg();	// fake destination created by compiler
      }
    } else {
      dst = node->contextTemp(i)->preg();
    }
    assign(dst, src, false);
  }
  // NB: no store check necessary (done in ContextCreateNode)
  // init node must follow create immediately (since fields are uninitialized)
  assert(node->firstPrev()->isContextCreateNode(), "should be immediatly after a ContextCreateNode");
}


void CodeGenerator::aContextZapNode(ContextZapNode* node) {
  if (!node->isActive()) return;
  assert(node->scope()->needsContextZapping() && node->src() == node->scope()->context(), "no zapping needed or wrong context");
  // Make sure these registers are not used within zapContext
  // because they are used for return/non-local return
  // -> allocate them as temporaries for now
  Temporary t1(_currentMapping, NLR_result_reg);
  Temporary t2(_currentMapping, NLR_home_reg);
  Temporary t3(_currentMapping, NLR_homeId_reg);
  // zap context
  _masm->movl(Address(use(node->src()), contextOopDesc::parent_byte_offset()), 0);
  // A better solution should be found here: There should be a mechanism
  // to exclude certain register from beeing taken.
}


void CodeGenerator::copyIntoContexts(BlockCreateNode* node) {
  // Copy newly created block into all contexts that have a copy.
  // The BlockPReg has a list of all contexts containing the block.  It should
  // be stored into those that are allocated (weren't eliminated) and are in
  // a sender scope.  
  // Why not copy into contexts in a sibling scope?  There are two cases:
  //   (1) The sibling scope never created the block(s) that uplevel-access this
  //       block.  The context location still contains 0 but that doesn't matter
  //       because that context location is now inaccessible.
  //   (2) The sibling scope did create these block(s).  In this case, the receiver
  //       must already exist since it was materialized when the first uplevel-
  //       accessing block was created.
  // Urs 4/96
  BlockPReg* blk = node->block();
  GrowableArray<Location*>* copies = blk->contextCopies();
  if (copies != NULL) {
    for (int i = copies->length(); i-- > 0; ) {
      Location* l = copies->at(i);
      InlinedScope* scopeWithContext = theCompiler->scopes->at(l->scopeID());
      PReg* r = scopeWithContext->contextTemporaries()->at(l->tempNo())->preg();
      if (r->loc == unAllocated) continue;	  // not uplevel-accessed (eliminated)
      if (r->isBlockPReg()) continue;		  // ditto
      if (!r->loc.isContextLocation()) fatal("expected context location");
      if (scopeWithContext->isSenderOrSame(node->scope())) {
        assign(r, node->block());
      }
    }
  }
}


void CodeGenerator::materializeBlock(BlockCreateNode* node) {
  CompileTimeClosure* closure = node->block()->closure();
  // allocate closure
  _currentMapping->kill(node->dst());	// kill it so that aPrimNode(node) can map the result to it
  int nofArgs = closure->nofArgs();
  switch (nofArgs) {
    case 0 : // fall through for now - fix this
    case 1 : // fall through for now - fix this
    case 2 : // fall through for now - fix this
    default:
      _masm->pushl(int(as_smiOop(nofArgs)));
      aPrimNode(node);			// Note: primitive calls are called via call_C - also necessary for primitiveValue calls?
      _masm->addl(esp, oopSize);	// pop argument, this is not a Pascal call - change this at some point?
      break;
  }
  // copy into all contexts that have a copy
  if (node->block()->isMemoized()) copyIntoContexts(node);
  // initialize closure fields
  PRegLocker lock(node->block());	// once loaded, make sure closure stays in register
  Register closure_reg = use(node->block());
  // assert(theCompiler->jumpTableID == closure->parent_id(), "nmethod id must be the same");
  // fix this: RELOCATION INFORMATION IS NEEDED WHEN MOVING THE JUMPTABLE (Snapshot reading etc.)
  _masm->movl(Address(closure_reg, blockClosureOopDesc::context_byte_offset()        ),	use(closure->context()));
  _masm->movl(Address(closure_reg, blockClosureOopDesc::method_or_entry_byte_offset()),	(int)closure->jump_table_entry());
  storeCheck(closure_reg);
}


void CodeGenerator::aBlockCreateNode(BlockCreateNode* node) {
  if (node->block()->closure()->method()->is_clean_block()) {
    // create the block now (doesn't need to be copied at run-time
    CompileTimeClosure* closure = node->block()->closure();
    blockClosureOop blk = blockClosureOopDesc::create_clean_block(closure->nofArgs(), closure->jump_table_entry());
    _masm->movl(def(node->dst()), blk);
  } else if (node->block()->isMemoized()) {
    // initialize block variable
    _masm->movl(def(node->dst()), MemoizedBlockNameDesc::uncreatedBlockValue());
  } else {
    // actually create block
    materializeBlock(node);
  }
}


void CodeGenerator::aBlockMaterializeNode(BlockMaterializeNode* node) {
  assert(node->next() == node->likelySuccessor(), "code pattern is not optimal");
  if (node->block()->closure()->method()->is_clean_block()) {
    // no need to create the block (already exists)
  } else if (node->block()->isMemoized()) {
    // materialize block if it is not already materialized
    // (nothing to do in case of non-memoized blocks)
    Register closure_reg = use(node->block());
    assert(MemoizedBlockNameDesc::uncreatedBlockValue() == oop(0), "change the code generation here");
    _masm->testl(closure_reg, closure_reg);
    jcc(Assembler::notZero, node, node->next(), true);
    materializeBlock(node);
    jmp(node, node->next(), true);			// will be eliminated since next() is the likely successor
    bb_needs_jump = false;
  }
}


void CodeGenerator::aSendNode(SendNode* node) {
  // Question concerning saveRegisters() below: is it really needed to also save the
  // recv (it is a parameter passed in)? If it happens to be also a visible value of
  // the caller and if it has not been stored before we would get an "intermediate"
  // frame with a unsaved register value => we should save the recv as well. However
  // this is only true, if the recv value is not explicitly assigned (and the assignment
  // has not been eliminated). Otherwise this is an unneccessary save instr.
  // For now: be conservative & save it always.
  if (node->isCounting()) incrementInvocationCounter();
  char* entry = node->isSuperSend() ? CompiledIC::superLookupRoutine() : CompiledIC::normalLookupRoutine();
  PReg* recv = node->recv();
  _currentMapping->killDeadsAt(node->next(), recv);	// free mapping of unused pregs
  _currentMapping->makeInjective();			// make injective because NLR cannot deal with non-injective mappings yet
  _currentMapping->saveRegisters();			// make sure none of the remaining preg values are lost
  _currentMapping->killRegisters(recv);			// so PRegMapping::use can safely allocate receiverLoc if necessary
  _currentMapping->use(recv, receiver_reg);		// make sure recv is in the right register
  updateDebuggingInfo(node);
  _masm->call(entry, relocInfo::ic_type);
  _currentMapping->killRegisters();
  // compute flag settings
  int flags = 0;
  if (node->isSuperSend())	setNth(flags, super_send_bit_no);
  if (node->isUninlinable())	setNth(flags, uninlinable_bit_no);
  if (node->staticReceiver())	setNth(flags, receiver_static_bit_no);
  // inline cache
  inlineCache(node, node->scope()->nlrTestPoint(), flags);
  _currentMapping->mapToRegister(node->dst(), result_reg);	// NLR mapping of node->dst is handled in NLRTestNode
}


void CodeGenerator::aPrimNode(PrimNode* node) {
  MergeNode* nlr = node->pdesc()->can_perform_NLR() ? node->scope()->nlrTestPoint() : NULL;
  _currentMapping->killDeadsAt(node->next());		// free mapping of unused pregs
  _currentMapping->makeInjective();			// make injective because NLR cannot deal with non-injective mappings yet
  _currentMapping->saveRegisters();			// make sure none of the remaining preg values are lost
  _currentMapping->killRegisters();
  updateDebuggingInfo(node);
  // Note: cannot use call_C because inline cache code has to come immediately after call instruction!
  _masm->set_last_Delta_frame_before_call();
  _masm->call((char*)(node->pdesc()->fn()), relocInfo::prim_type);
  _currentMapping->killRegisters();
  if (nlr != NULL) inlineCache(node, nlr);
  _masm->reset_last_Delta_frame();
  _currentMapping->mapToRegister(node->dst(), result_reg);	// NLR mapping of node->dst is handled in NLRTestNode
}


void CodeGenerator::aDLLNode(DLLNode* node) {
  // determine entry point depending on whether a run-time lookup is needed or not.
  // Note: do not do a DLL lookup at compile time since this may cause a call back.
  char* entry = (node->function() == NULL)
                ? StubRoutines::lookup_DLL_entry(node->async())
		: StubRoutines::call_DLL_entry(node->async());
  // pass arguments for DLL lookup/parameter conversion routine in registers
  // (change this code if the corresponding routines change (StubRoutines))
  // ebx: no. of arguments
  // ecx: address of last argument
  // edx: dll function entry point (backpatched, belongs to CompiledDLL_Cache)
  _currentMapping->saveRegisters();
  _currentMapping->killRegisters();
  updateDebuggingInfo(node);
  _masm->movl(ebx, node->nofArguments());
  _masm->movl(ecx, esp);
  // CompiledDLL_Cache
  // This code pattern must correspond to the CompiledDLL_Cache layout
  // (make sure assembler is not optimizing mov reg, 0 into xor reg, reg!)
  _masm->movl(edx, int(node->function()));		// part of CompiledDLL_Cache
  _masm->inline_oop(node->dll_name());			// part of CompiledDLL_Cache
  _masm->inline_oop(node->function_name());		// part of CompiledDLL_Cache
  _masm->call(entry, relocInfo::runtime_call_type);	// call lookup/parameter conversion routine
  _currentMapping->killRegisters();
  // For now: ordinary inline cache even though NLRs through DLLs are not allowed yet
  // (make sure somebody is popping arguments if NLRs are used).
  inlineCache(node, node->scope()->nlrTestPoint());
  _currentMapping->mapToRegister(node->dst(), result_reg);
  _masm->addl(esp, node->nofArguments()*oopSize);	// get rid of arguments
}


/*
static void testForSingleKlass(Register obj, klassOop klass, Register klassReg, Label& success, Label& failure) {
  if (klass == Universe::smiKlassObj()) {
    // check tag
    theMacroAssm->test(obj, Mem_Tag);
  } else if (klass == Universe::trueObj()->klass()) {
    // only one instance: compare with trueObj
    theMacroAssm->cmpl(obj, Universe::trueObj());
  } else if (klass == Universe::falseObj()->klass()) {
    // only one instance: compare with falseObj
    theMacroAssm->cmpl(obj, Universe::falseObj());
  } else if (klass == Universe::nilObj()->klass()) {
    // only one instance: compare with nilObj
    theMacroAssm->cmpl(obj, Universe::nilObj());
  } else {
    // compare against obj's klass - must check if smi first
    theMacroAssm->test(obj, Mem_Tag);
    theMacroAssm->jcc(Assembler::zero, failure);
    theMacroAssm->movl(klassReg, Address(obj, memOopDesc::klass_byte_offset()));
    theMacroAssm->cmpl(klassReg, klass);
  }
  theMacroAssm->jcc(Assembler::notEqual, failure);
  theMacroAssm->jmp(success);	// this jump will be eliminated since this is the likely successor
}
*/
void CodeGenerator::testForSingleKlass(Register obj, klassOop klass, Register klassReg, Label& success, Label& failure) {
  if (klass == Universe::smiKlassObj()) {
    // check tag
    _masm->test(obj, Mem_Tag);
  } else if (klass == Universe::trueObj()->klass()) {
    // only one instance: compare with trueObj
    _masm->cmpl(obj, Universe::trueObj());
  } else if (klass == Universe::falseObj()->klass()) {
    // only one instance: compare with falseObj
    _masm->cmpl(obj, Universe::falseObj());
  } else if (klass == Universe::nilObj()->klass()) {
    // only one instance: compare with nilObj
    _masm->cmpl(obj, Universe::nilObj());
  } else {
    // compare against obj's klass - must check if smi first
    _masm->test(obj, Mem_Tag);
    _masm->jcc(Assembler::zero, failure);
    _masm->movl(klassReg, Address(obj, memOopDesc::klass_byte_offset()));
    _masm->cmpl(klassReg, klass);
  }
  _masm->jcc(Assembler::notEqual, failure);
  _masm->jmp(success);	// this jump will be eliminated since this is the likely successor
}


void CodeGenerator::generateTypeTests(LoopHeaderNode* node, Label& failure) {
  Unimplemented();

  int last = 0;
  for (int i = 0; i <= last; i++) {
    HoistedTypeTest* t = node->tests()->at(i);
    if (t->testedPR->loc == unAllocated) continue;	// optimized away, or ConstPReg
    if (t->testedPR->isConstPReg()) {
      guarantee(t->testedPR->loc == unAllocated, "code assumes ConstPRegs are unallocated");
      //handleConstantTypeTest((ConstPReg*)t->testedPR, t->klasses);
    } else {


    }
  }
}


/*
void LoopHeaderNode::generateTypeTests(Label& cont, Label& failure) {
  // test all values against expected classes
  Label* ok;
  const Register klassReg = temp2;
  const int len = _tests->length() - 1;
  int last;						// last case that generates a test
  for (last = len; last >= 0 && _tests->at(last)->testedPR->loc == unAllocated; last--) ;
  if (last < 0) return;					// no tests at all   
  for (int i = 0; i <= last; i++) {
    HoistedTypeTest* t = _tests->at(i);
    if (t->testedPR->loc == unAllocated) continue;	// optimized away, or ConstPReg
    if (t->testedPR->isConstPReg()) { 
      guarantee(t->testedPR->loc == unAllocated, "code assumes ConstPRegs are unallocated");
      handleConstantTypeTest((ConstPReg*)t->testedPR, t->klasses);
    } else {
      const Register obj = movePRegToReg(t->testedPR, temp1);
      Label okLabel;
      ok = (i == last) ? &cont : &okLabel;
      if (t->klasses->length() == 1) {
	testForSingleKlass(obj, t->klasses->at(0), klassReg, *ok, failure);
      } else if (t->klasses->length() == 2 &&
		 testForBoolKlasses(obj, t->klasses->at(0), t->klasses->at(1), klassReg, true, 
		 *ok, *ok, failure)) {
	// ok, was a bool test
      } else {
	const int len = t->klasses->length();
	GrowableArray<Label*> labels(len + 1);
	labels.append(&failure);
	for (int i = 0; i < len; i++) labels.append(ok);
	generalTypeTest(obj, klassReg, true, t->klasses, &labels);
      }
      if (i != last) theMacroAssm->bind(*ok);
    }
  }
}
*/


/*
void CodeGenerator::handleConstantTypeTest(ConstPReg* r, GrowableArray<klassOop>* klasses) {
  // constant r is tested against klasses (efficiency hack: klasses == NULL means {smi})
  if ((klasses == NULL && r->constant->is_smi()) || (klasses && klasses->contains(r->constant->klass()))) {
    // always ok, no need to test
  } else {
    compiler_warning("loop header type test will always fail!");
    // don't jump to failure because that would make subsequent LoopHeader code unreachable (--> breaks back end)
    theMacroAssm->call(StubRoutines::unused_uncommon_trap_entry(), relocInfo::uncommon_type);
  }
}
*/


void CodeGenerator::generateIntegerLoopTest(PReg* preg, LoopHeaderNode* node, Label& failure) {
  if (preg != NULL) {
    if (preg->isConstPReg()) {
      // no run-time test necessary
      //handleConstantTypeTest((ConstPReg*)preg, NULL);
    } else if (preg->loc == unAllocated) {
      // preg is never used in loop => no test needed
      guarantee(preg->cpReg() == preg, "should use cpReg()");
    } else {
      // generate run-time test
      /*
      if (prev.is_unbound()) theMacroAssm->bind(prev);
      Label ok;
      const Register obj = movePRegToReg(p, temp1);
      testForSingleKlass(obj, Universe::smiKlassObj(), klassReg, ok, failure);
      theMacroAssm->bind(ok);
      */
    }
  }
}


/*
void LoopHeaderNode::generateIntegerLoopTest(PReg* p, Label& prev, Label& failure) {
  const Register klassReg = temp2;
  if (p != NULL) {
    if (p->isConstPReg()) {
      // no run-time test necessary
      handleConstantTypeTest((ConstPReg*)p, NULL);
    } else if (p->loc == unAllocated) {
      // p is never used in loop, so no test needed
      guarantee(p->cpReg() == p, "should use cpReg()");
    } else {
      // generate run-time test
      if (prev.is_unbound()) theMacroAssm->bind(prev);
      Label ok;
      const Register obj = movePRegToReg(p, temp1);
      testForSingleKlass(obj, Universe::smiKlassObj(), klassReg, ok, failure);
      theMacroAssm->bind(ok);
    }
  }
}
*/


void CodeGenerator::generateIntegerLoopTests(LoopHeaderNode* node, Label& failure) {
  assert(node->isIntegerLoop(), "must be integer loop");
  generateIntegerLoopTest(node->lowerBound(), node, failure);
  generateIntegerLoopTest(node->upperBound(), node, failure);
  generateIntegerLoopTest(node->loopVar   (), node, failure);
}


/*
void LoopHeaderNode::generateIntegerLoopTests(Label& prev, Label& failure) {
  if (!_integerLoop) return;
  generateIntegerLoopTest(_lowerBound, prev, failure);
  generateIntegerLoopTest(_upperBound, prev, failure);
  generateIntegerLoopTest(_loopVar   , prev, failure);
}
*/


void CodeGenerator::generateArrayLoopTests(LoopHeaderNode* node, Label& failure) {
  assert(node->isIntegerLoop(), "must be integer loop");
  if (node->upperLoad() != NULL) {
    // The loop variable iterates from lowerBound...array size; if any of the array accesses
    // use the loop variable without an index range check, we need to check it here.
    PReg* loopArray = node->upperLoad()->src();
    AbstractArrayAtNode* atNode;
    int i = node->arrayAccesses()->length();
    while (i-- > 0) {
      atNode = node->arrayAccesses()->at(i);
      if (atNode->src() == loopArray && !atNode->needsBoundsCheck()) break;
    }
    if (i >= 0) {
      // loopVar is used to index into array; make sure lower & upper bound is within array range
      PReg* lo = node->lowerBound();
      PReg* hi = node->upperBound();
      if (lo != NULL && lo->isConstPReg() && ((ConstPReg*)lo)->constant->is_smi() && ((ConstPReg*)lo)->constant >= as_smiOop(1)) {

      } else {
        // test lower bound
        //
	if (lo->loc == unAllocated) {
	  
	} else {
	  //
	}
      }
      // test upper bound
      
    }
  }
}


/*
void LoopHeaderNode::generateArrayLoopTests(Label& prev, Label& failure) {
  if (!_integerLoop) return;
  Register boundReg = temp1;
  const Register tempReg  = temp2;
  if (_upperLoad != NULL) {
    // The loop variable iterates from lowerBound...array size; if any of the array accesses use the loop variable
    // without an index range check, we need to check it here.
    PReg* loopArray = _upperLoad->src();
    AbstractArrayAtNode* atNode;
    for (int i = _arrayAccesses->length() - 1; i >= 0; i--) {
      atNode = _arrayAccesses->at(i);
      if (atNode->src() == loopArray && !atNode->needsBoundsCheck()) break;
    }
    if (i >= 0) {
      // loopVar is used to index into array; make sure lower & upper bound is within array range
      if (_lowerBound != NULL && _lowerBound->isConstPReg() && ((ConstPReg*)_lowerBound)->constant->is_smi() && ((ConstPReg*)_lowerBound)->constant >= as_smiOop(1)) {
	// loopVar iterates from smi_const to array size, so no test necessary
      } else {
	// test lower bound
       if (prev.is_unbound()) theMacroAssm->bind(prev);
       if (_lowerBound->loc == unAllocated) {
	 guarantee(_lowerBound->cpReg() == _lowerBound, "should use cpReg()");
       } else {
	 const Register t = movePRegToReg(_lowerBound ? _lowerBound : _loopVar, tempReg);
	 theMacroAssm->cmpl(boundReg, as_smiOop(1));
	 theMacroAssm->jcc(Assembler::less, failure);
       }
      }

      // test upper bound
      boundReg = movePRegToReg(_upperBound, boundReg);
      const Register t = movePRegToReg(atNode->src(), tempReg);
      theMacroAssm->movl(t, Address(t, byteOffset(atNode->sizeOffset())));
      theMacroAssm->cmpl(boundReg, t);
      theMacroAssm->jcc(Assembler::above, failure);
    }
  }
}
*/


void CodeGenerator::aLoopHeaderNode(LoopHeaderNode* node) {
  if (node->isActivated()) {
    warning("loop header node not yet implemented");
    return;

    // the loop header node performs all checks hoisted out of the loop:
    // for general loops:
    //   - do all type tests in the list, uncommon branch if they fail
    //     (common case: true/false tests, single-klass tests) 
    // additionally for integer loops:
    //   - test lowerBound (may be NULL), upperBound, loopVar for smi-ness (the first two may be ConstPRegs)
    //   - if upperBound is NULL, upperLoad is load of the array size
    //   - if loopArray is non-NULL, check lowerBound (if non-NULL) or initial value of loopVar against 1
    Label failure;
    generateTypeTests(node, failure);
    if (node->isIntegerLoop()) {
      generateIntegerLoopTests(node, failure);
      generateArrayLoopTests(node, failure);
    }
    _masm->bind(failure);
    updateDebuggingInfo(node);
    _masm->call(StubRoutines::unused_uncommon_trap_entry(), relocInfo::uncommon_type);
    bb_needs_jump = false;
    setMapping(NULL);
  }
}


/*
void LoopHeaderNode::gen() {
  if (!_activated) return;    // loop wasn't optimized
  // the loop header node performs all checks hoisted out of the loop:
  // for general loops:
  //   - do all type tests in the list, uncommon branch if they fail
  //     (common case: true/false tests, single-klass tests) 
  // additionally for integer loops:
  //   - test lowerBound (may be NULL), upperBound, loopVar for smi-ness (the first two may be ConstPRegs)
  //   - if upperBound is NULL, upperLoad is load of the array size
  //   - if loopArray is non-NULL, check lowerBound (if non-NULL) or initial value of loopVar against 1
  TrivialNode::gen();
  Label ok;
  Label failure;
  generateTypeTests(ok, failure);
  generateIntegerLoopTests(ok, failure);
  generateArrayLoopTests(ok, failure);
  if (ok.is_unbound()) theMacroAssm->bind(ok);
  theMacroAssm->jmp(next()->label);
  // above 2 lines could be eliminated with: if (ok.is_unbound()) ok.redirectTo(next()->label)
  bb_needs_jump = false;  // we generate all jumps explicitly
  theMacroAssm->bind(failure);
  theMacroAssm->call(StubRoutines::unused_uncommon_trap_entry(), relocInfo::uncommon_type);
}
*/


void CodeGenerator::aReturnNode(ReturnNode* node) {
  InlinedScope* scope = node->scope();
  if (scope->needsContextZapping()) zapContext(scope->context());	// <<< still needed? What about ContextZapNode?
  // make sure result is in result_reg, no other pregs are used anymore
  PReg* result = scope->resultPR;
  _currentMapping->killRegisters(result);
  _currentMapping->use(result, result_reg);
  // remove stack frame & return
  if (VerifyCode || TraceCalls || TraceResults) callVerifyReturn();
  int no_of_args_to_pop = scope->nofArguments();
  if (scope->method()->is_blockMethod()) {
    // blocks are called via primitiveValue => need to pop first argument
    // of primitiveValue (= block closure) as well since return happens
    // directly (and not through primitiveValue).
    no_of_args_to_pop++;
  }
  _masm->leave();
  _masm->ret(no_of_args_to_pop * oopSize);
  // no pregs accessible anymore
  setMapping(NULL);
}


void CodeGenerator::aNLRSetupNode(NLRSetupNode* node) {
  InlinedScope* scope = node->scope();
  // compute home into a temporary register (NLR_home_reg might still be in use - but try to use it if possible)
  // and check if context has been  zapped
  //
  // QUESTION: Who is popping the arguments (for an ordinary return it happens automatically in the return).
  // Couldn't that be a problem in loops? How's this done in the old backend? In the interpreter?
  Label NLR_error;							// for NLRs to non-existing frames
  Temporary home(_currentMapping, NLR_home_reg);			// try to allocate temporary home into right register
  uplevelBase(scope->context(), scope->homeContext() + 1, home.reg());	// compute home
  _masm->testl(home.reg(), home.reg());					// check if zapped
  _masm->jcc(Assembler::zero, NLR_error);				// zero -> home has been zapped
  // load result into temporary register (NLR_result_reg might still be in use - but try to use it if possible)
  PReg* resultPReg = scope->resultPR;
  _currentMapping->killRegisters(resultPReg);				// no PRegs are used anymore except result
  Register result;							// temporary result register
  { Temporary t(_currentMapping, NLR_result_reg); result = t.reg(); }	// try to allocate temporary result into right register
  _currentMapping->use(resultPReg, result);				// load result into temporary result register
  // finally assign result and home to the right registers, make sure that they
  // don't overwrite each other (home could be in the result register & vice versa).
  // For now push them and pop them back into the right registers.
  if (result != NLR_result_reg || home.reg() != NLR_home_reg) {
    _masm->pushl(result);
    _masm->pushl(home.reg());
    _masm->popl(NLR_home_reg);
    _masm->popl(NLR_result_reg);
  }
  // assign home id
  _masm->movl(NLR_homeId_reg, scope->home()->scopeID());
  // issue NLR
  if (VerifyCode || TraceCalls || TraceResults) callVerifyNLR();
  _masm->jmp(StubRoutines::continue_NLR_entry(), relocInfo::runtime_call_type);
  // call run-time routine in failure case
  // what about the debugging information? FIX THIS
  _masm->bind(NLR_error);
  _masm->call_C((char*)suspend_on_NLR_error, relocInfo::runtime_call_type);
  // no pregs accessible anymore
  setMapping(NULL);
}


void CodeGenerator::anInlinedReturnNode(InlinedReturnNode* node) {
  // Not generated anymore for new backend
  ShouldNotReachHere();
}


void CodeGenerator::aNLRContinuationNode(NLRContinuationNode* node) {
  guarantee(_currentMapping->NLRinProgress(), "NLR must be in progress");
  InlinedScope* scope = node->scope();
  if (scope->needsContextZapping()) zapContext(scope->context());
  // continue with NLR
  if (VerifyCode || TraceCalls || TraceResults) callVerifyNLR();
  _masm->jmp(StubRoutines::continue_NLR_entry(), relocInfo::runtime_call_type);
  // no pregs accessible anymore
  setMapping(NULL);
}


Assembler::Condition CodeGenerator::mapToCC(BranchOpCode op) {
  switch (op) {
    case EQBranchOp : return Assembler::equal;
    case NEBranchOp : return Assembler::notEqual;
    case LTBranchOp : return Assembler::less;
    case LEBranchOp : return Assembler::lessEqual;
    case GTBranchOp : return Assembler::greater;
    case GEBranchOp : return Assembler::greaterEqual;
    case LTUBranchOp: return Assembler::below;
    case LEUBranchOp: return Assembler::belowEqual;
    case GTUBranchOp: return Assembler::above;
    case GEUBranchOp: return Assembler::aboveEqual;
    case VSBranchOp : return Assembler::overflow;
    case VCBranchOp : return Assembler::noOverflow;
  }
  ShouldNotReachHere();
  return Assembler::zero;
}


void CodeGenerator::aBranchNode(BranchNode* node) {
  jcc(mapToCC(node->op()), node, node->next(1));
}


void CodeGenerator::aTypeTestNode(TypeTestNode* node) {
  // Note 1: This code pattern requires *no* particular order
  //         of the the classes of the TypeTestNode.
  //
  // Note 2: In case of a TypeTestNode without unknown case,
  //         the last case would not have to be conditional.
  //         However, for debugging purposes right now all
  //         cases are always explicitly checked.
  const int len = node->classes()->length();
  
  if (ReorderBBs) {
    PRegLocker lock(node->src());
    Register obj = use(node->src());
      
    if (len == 1) {
      // handle all cases where only one klass is involved
      assert(node->hasUnknown(), "should be eliminated if there's no unknown case");
      assert(node->likelySuccessor() == node->next(1), "code pattern is not optimal");
      klassOop klass = node->classes()->at(0);
      if (klass == Universe::smiKlassObj()) {
        // check tag
        _masm->test(obj, Mem_Tag);
      } else if (klass == Universe::trueObj()->klass()) {
        // only one instance: compare with trueObj
        _masm->cmpl(obj, Universe::trueObj());
      } else if (klass == Universe::falseObj()->klass()) {
        // only one instance: compare with falseObj
        _masm->cmpl(obj, Universe::falseObj());
      } else if (klass == Universe::nilObj()->klass()) {
        // only one instance: compare with nilObj
        _masm->cmpl(obj, Universe::nilObj());
      } else {
        // compare against obj's klass - must check if smi first
        Temporary objKlass(_currentMapping);
        _masm->test(obj, Mem_Tag);
        _masm->jcc(Assembler::zero, node->next()->label);
        _masm->movl(objKlass.reg(), Address(obj, memOopDesc::klass_byte_offset()));
        _masm->cmpl(objKlass.reg(), klass);
      }
      jcc(Assembler::notEqual, node, node->next());
      jmp(node, node->next(1));			// this jump will be eliminated since this is the likely successor
      bb_needs_jump = false;			// no jump necessary at end of basic block
      return;
    }

    if (len == 2) {
      // handle pure boolean cases (ifTrue:/ifFalse:)
      klassOop klass1 = node->classes()->at(0);
      klassOop klass2 = node->classes()->at(1);
      oop      bool1  = Universe::trueObj();
      oop      bool2  = Universe::falseObj();
      if (klass1 == bool2->klass() && klass2 == bool1->klass()) {
        oop t = bool1; bool1 = bool2; bool2 = t;
      }
      if (klass1 == bool1->klass() && klass2 == bool2->klass()) {
        const bool ignoreNoUnknownForNow = true;
        // Note: Uncommon case: A TypeTestNode with no uncommon case has a successor
        //       at next(0) anyhow (because there are no "holes" (= NULLs) in the
        //       successor list of a node). That is, for now we have to jump to that
        //       point somehow (even though it can never happen), because otherwise
        //       the PRegMapping is not set for that node. (Maybe one should detect
        //       this case and then set a "dummy" PRegMapping, since it is not used
        //       anyhow but needs to be there only for assertion checking).
        if (ignoreNoUnknownForNow || node->hasUnknown()) {
	  assert(ignoreNoUnknownForNow || node->likelySuccessor() == node->next(2), "code pattern is not optimal");
	  _masm->cmpl(obj, bool1);
	  jcc(Assembler::equal, node, node->next(1));
          _masm->cmpl(obj, bool2);
          jcc(Assembler::notEqual, node, node->next());
	  jmp(node, node->next(2));		// this jump will be eliminated since this is the likely successor
	} else {
	  assert(node->likelySuccessor() == node->next(1), "code pattern is not optimal");
          _masm->cmpl(obj, bool2);
	  jcc(Assembler::equal, node, node->next(2));
	  jmp(node, node->next(1));		// this jump will be eliminated since this is the likely successor
	}
	bb_needs_jump = false;			// no jump necessary at end of basic block
        return;
      }
    }
  }

  // general case
  Label unknownCase;
  Temporary objKlass(_currentMapping);
  bool klassHasBeenLoaded = false;
  bool smiHasBeenChecked = false;
  PRegLocker lock(node->src());
  Register obj = use(node->src());
  for (int i = 0; i < len; i++) {
    klassOop klass = node->classes()->at(i);
    if (klass == trueObj->klass()) {
      // only one instance: compare with trueObj
      _masm->cmpl(obj, trueObj);
      jcc(Assembler::equal, node, node->next(i+1));
    } else if (klass == falseObj->klass()) {
      // only one instance: compare with falseObj
      _masm->cmpl(obj, falseObj);
      jcc(Assembler::equal, node, node->next(i+1));
    } else if (klass == nilObj->klass()) {
      // only one instance: compare with nilObj
      _masm->cmpl(obj, nilObj);
      jcc(Assembler::equal, node, node->next(i+1));
    } else if (klass == smiKlassObj) {
      // check smi tag only if not checked already, otherwise ignore
      if (!smiHasBeenChecked) {
        _masm->test(obj, Mem_Tag);
        jcc(Assembler::zero, node, node->next(i+1));
        smiHasBeenChecked = true;
      }
    } else {
      // compare with klass
      if (!klassHasBeenLoaded) {
        if (!smiHasBeenChecked) {
	  Node* smiCase = node->smiCase();
	  if (smiCase != NULL || node->hasUnknown()) {
	    // smi can actually appear => check for it
            _masm->test(obj, Mem_Tag);
	    if (smiCase != NULL) {
	      // jump to smiCase if there's one
              jcc(Assembler::zero, node, smiCase);
	    } else {
	      // node hasUnknown & smiCase cannot happen => jump to unknown case (end of typetest)
	      _masm->jcc(Assembler::zero, unknownCase);
	    }
	  }
	  smiHasBeenChecked = true;
	}
	_masm->movl(objKlass.reg(), Address(obj, memOopDesc::klass_byte_offset()));
	klassHasBeenLoaded = true;
      }
      _masm->cmpl(objKlass.reg(), klass);
      jcc(Assembler::equal, node, node->next(i+1));
    }
  }
  // bind label in any case to avoid unbound label assertion bug
  _masm->bind(unknownCase);

  // Note: Possible problem: if the smi case is checked before the class
  //       is loaded, there's possibly a jump to the end of the TypeTestNode
  //       from the smi case. However, then the klass register isn't defined.
  //       if later there's the uncommon case, the klass register is defined.
  //       What if one refers to that register? Or is it not possible because
  //       it's not a regular PReg but a temporary? Think about this!
  //
  // >>>>> IS A PROBLEM! The temporary is likely to throw out another PReg from a register! FIX THIS!
}


// Note: Maybe should reorganize the way NLRs are treated in the intermediate representation;
// may be able to avoid some jumps. For example, continuing the NLR is done via a stub routine,
// maybe one can jump to that routine conditionally and thereby save a jump around a jump (that
// stub routine could also do the zapping if necessary (could come in two versions)).

void CodeGenerator::aNLRTestNode(NLRTestNode* node) {
  assert(_currentMapping->NLRinProgress(), "NLR must be in progress");
  InlinedScope* scope = node->scope();
  // check if arrived at the right frame
  Label L;
  _masm->cmpl(NLR_home_reg, frame_reg);
  _masm->jcc(Assembler::notEqual, L);
  // check if arrived at the right scope within the frame
  int id = scope->scopeID();
  if (id == 0) {
    // use test instruction to compare against 0 (smaller code than with cmp)
    _masm->testl(NLR_homeId_reg, NLR_homeId_reg);
  } else {
    _masm->cmpl(NLR_homeId_reg, id);
  }
  _currentMapping->releaseNLRRegisters();
  jcc(Assembler::equal, node, node->next(1));
  _currentMapping->acquireNLRRegisters();
  // otherwise continue NLR
  _masm->bind(L);
}


void CodeGenerator::aMergeNode(MergeNode* node) {
  assert(node->isTrivial() || _currentMapping->isInjective(), "must be injective if more than one predecessor");
}


void CodeGenerator::jcc_error(Assembler::Condition cc, AbstractBranchNode* node, Label& label) {
  assert(node->canFail(), "should not be called if node cannot fail");
  Node* failure_start = node->next(1);
  if (failure_start->isUncommonNode()) {
    jcc(cc, node, failure_start, true);
  } else {
    _masm->jcc(cc, label);
  }
}


void CodeGenerator::anArrayAtNode(ArrayAtNode* node) {
  PReg* array  = node->array();
  PReg* index  = node->index();
  PReg* result = node->dst();
  PReg* error  = node->error();
  PRegLocker lock(array, index);
  Register array_reg = use(array);
  // use temporary register for index - will be modified
  Temporary offset(_currentMapping, index);
  // first element is at index 1 => subtract smi(1) (doesn't change smi/oop property)
  theMacroAssm->subl(offset.reg(), int(smiOop_one));
  // do index smi check if necessary (still possible, even after subtracting smi(1))
  Label indexNotSmi;
  if (!node->index_is_smi()) {
    _masm->test(offset.reg(), Mem_Tag);
    jcc_error(Assembler::notZero, node, indexNotSmi);
  }
  // do bounds check if necessary
  Label indexOutOfBounds;
  if (node->index_needs_bounds_check()) {
    const int size_offset = byteOffset(node->size_word_offset());
    _masm->cmpl(offset.reg(), Address(array_reg, size_offset));
    jcc_error(Assembler::aboveEqual, node, indexOutOfBounds);
  }
  // load element
  assert(Tag_Size == 2, "check this code");
  const int data_offset = byteOffset(node->data_word_offset());
  switch (node->access_type()) {
    case ArrayAtNode::byte_at:
      { Register result_reg = def(result);
        _masm->sarl(offset.reg(), Tag_Size);	// adjust index
	if (result_reg.hasByteRegister()) {
	  // result_reg has byte register -> can use byte load instruction
          _masm->xorl(result_reg, result_reg);	// clear destination register
	  _masm->movb(result_reg, Address(array_reg, offset.reg(), Address::times_1, data_offset));
	} else {
	  // result_reg has no byte register -> cannot use byte load instruction
	  // instead of doing better register selection use word load & mask for now
	  _masm->movl(result_reg, Address(array_reg, offset.reg(), Address::times_1, data_offset));
	  _masm->andl(result_reg, 0x000000FF);	// clear uppper 3 bytes
	}
	_masm->shll(result_reg, Tag_Size);	// make result a smi
      }
      break;
    case ArrayAtNode::double_byte_at:
      { Register result_reg = def(result);
        _masm->sarl(offset.reg(), Tag_Size - 1);// adjust index
        _masm->movl(result_reg, Address(array_reg, offset.reg(), Address::times_1, data_offset));
        _masm->andl(result_reg, 0x0000FFFF);	// clear upper 2 bytes
        _masm->shll(result_reg, Tag_Size);	// make result a smi
      }
      break;
    case ArrayAtNode::character_at:
      { Register result_reg = def(result);
        _masm->sarl(offset.reg(), Tag_Size - 1);// adjust index
        _masm->movl(result_reg, Address(array_reg, offset.reg(), Address::times_1, data_offset));
        _masm->andl(result_reg, 0x0000FFFF);	// clear upper 2 bytes
        // use result_reg as index into asciiCharacters()
        // check index first, must be 0 <= result_reg < asciiCharacters()->length()
        objArrayOop chars = Universe::asciiCharacters();
        _masm->cmpl(result_reg, chars->length());
	jcc_error(Assembler::aboveEqual, node, indexOutOfBounds);
	// get character out of chars array
	_masm->movl(offset.reg(), chars);
	_masm->movl(result_reg, Address(offset.reg(), result_reg, Address::times_4, byteOffset(chars->klass()->klass_part()->non_indexable_size() + 1)));
      }
      break;
    case ArrayAtNode::object_at:
      // offset already shifted => no scaling necessary
      _masm->movl(def(result), Address(array_reg, offset.reg(), Address::times_1, data_offset));
      break;
    default:
      ShouldNotReachHere();
      break;
  }
  // handle error cases if not uncommon
  if (node->canFail() && !node->next(1)->isUncommonNode()) {
    Label exit;
    _masm->jmp(exit);
    // error messages
    if (!node->index_is_smi()) {
      _masm->bind(indexNotSmi);
      _masm->hlt();
    }
    if (node->index_needs_bounds_check()) {
      _masm->bind(indexOutOfBounds);
      _masm->hlt();
    }
    // hack for now - jcc so mapping stays alive
    // must do all the mapping in the program path taken - otherwise
    // mappings are inconsistent
    _masm->bind(exit);
    Register r = def(error);
    _masm->test(r, 0);
    jcc(Assembler::notZero, node, node->next(1));
  }
}


void CodeGenerator::anArrayAtPutNode(ArrayAtPutNode* node) {
  PReg* array   = node->array();
  PReg* index   = node->index();
  PReg* element = node->element();
  PReg* error   = node->error();
  PRegLocker lock(array, index, element);
  Register array_reg = use(array);
  // use temporary register for index - will be modified
  Temporary offset(_currentMapping, index);
  // first element is at index 1 => subtract smi(1) (doesn't change smi/oop property)
  theMacroAssm->subl(offset.reg(), int(smiOop_one));
  // do index smi check if necessary (still possible, even after subtracting smi(1))
  Label indexNotSmi;
  if (!node->index_is_smi()) {
    _masm->test(offset.reg(), Mem_Tag);
    jcc_error(Assembler::notZero, node, indexNotSmi);
  }
  // do bounds check if necessary
  Label indexOutOfBounds;
  if (node->index_needs_bounds_check()) {
    const int size_offset = byteOffset(node->size_word_offset());
    _masm->cmpl(offset.reg(), Address(array_reg, size_offset));
    jcc_error(Assembler::aboveEqual, node, indexOutOfBounds);
  }
  // store element
  assert(Tag_Size == 2, "check this code");
  const int data_offset = byteOffset(node->data_word_offset());
  Label elementNotSmi, elementOutOfRange;
  switch (node->access_type()) {
    case ArrayAtPutNode::byte_at_put:
      { // use temporary register for element - will be modified
        Temporary elt(_currentMapping, element);
	_masm->sarl(offset.reg(), Tag_Size);	// adjust index
	// do element smi check if necessary
        if (!node->element_is_smi()) {
          _masm->test(elt.reg(), Mem_Tag);
	  jcc_error(Assembler::notZero, node, elementNotSmi);
        }
	_masm->sarl(elt.reg(), Tag_Size);	// convert element into (int) byte
	// do element range check if necessary
	if (node->element_needs_range_check()) {
	  _masm->cmpl(elt.reg(), 0x100);
	  jcc_error(Assembler::aboveEqual, node, elementOutOfRange);
        }
	// store the element
	if (elt.reg().hasByteRegister()) {
	  // elt.reg() has byte register -> can use byte store instruction
	  _masm->movb(Address(array_reg, offset.reg(), Address::times_1, data_offset), elt.reg());
	} else {
	  // elt.reg() has no byte register -> cannot use byte store instruction
	  // instead of doing a better register selection use word load/store & mask for now
	  Temporary field(_currentMapping);
	  _masm->movl(field.reg(), Address(array_reg, offset.reg(), Address::times_1, data_offset));
          _masm->andl(field.reg(), 0xFFFFFF00);	// mask out lower byte
	  _masm->orl(field.reg(), elt.reg());	// move elt (elt < 0x100 => no masking of elt needed)
	  _masm->movl(Address(array_reg, offset.reg(), Address::times_1, data_offset), field.reg());
	}
	assert(!node->needs_store_check(), "just checking");
      }
      break;
    case ArrayAtPutNode::double_byte_at_put:
      { // use temporary register for element - will be modified
        Temporary elt(_currentMapping, element);
	_masm->sarl(offset.reg(), Tag_Size - 1);// adjust index
	// do element smi check if necessary
        if (!node->element_is_smi()) {
          _masm->test(elt.reg(), Mem_Tag);
	  jcc_error(Assembler::notZero, node, elementNotSmi);
        }
	_masm->sarl(elt.reg(), Tag_Size);	// convert element into (int) double byte
	// do element range check if necessary
	if (node->element_needs_range_check()) {
	  _masm->cmpl(elt.reg(), 0x10000);
	  jcc_error(Assembler::aboveEqual, node, elementOutOfRange);
        }
	// store the element
	if (elt.reg().hasByteRegister()) {
	  // elt.reg() has byte register -> can use byte store instructions
	  _masm->movb(Address(array_reg, offset.reg(), Address::times_1, data_offset + 0), elt.reg());
	  _masm->shrl(elt.reg(), 8);		// shift 2nd byte into low-byte position
          _masm->movb(Address(array_reg, offset.reg(), Address::times_1, data_offset + 1), elt.reg());
	} else {
	  // elt.reg() has no byte register -> cannot use byte store instructions
	  // instead of doing a better register selection use word load/store & mask for now
	  Temporary field(_currentMapping);
	  _masm->movl(field.reg(), Address(array_reg, offset.reg(), Address::times_1, data_offset));
          _masm->andl(field.reg(), 0xFFFF0000);	// mask out lower two bytes
	  _masm->orl(field.reg(), elt.reg());	// move elt (elt < 0x10000 => no masking of elt needed)
	  _masm->movl(Address(array_reg, offset.reg(), Address::times_1, data_offset), field.reg());
	}
	assert(!node->needs_store_check(), "just checking");
      }
      break;
    case ArrayAtPutNode::object_at_put:
      // offset already shifted => no scaling necessary
      if (node->needs_store_check()) {
        _masm->leal(offset.reg(), Address(array_reg, offset.reg(), Address::times_1, data_offset));
	_masm->movl(Address(offset.reg()), use(element));
	storeCheck(offset.reg());
      } else {
        _masm->movl(Address(array_reg, offset.reg(), Address::times_1, data_offset), use(element));
      }
      break;
    default:
      ShouldNotReachHere();
      break;
  }
  // handle error cases if not uncommon
  if (node->canFail() && !node->next(1)->isUncommonNode()) {
    Label exit;
    _masm->jmp(exit);
    // error messages
    if (!node->index_is_smi()) {
      _masm->bind(indexNotSmi);
      _masm->hlt();
    }
    if (node->index_needs_bounds_check()) {
      _masm->bind(indexOutOfBounds);
      _masm->hlt();
    }
    if (!node->element_is_smi()) {
      _masm->bind(elementNotSmi);
      _masm->hlt();
    }
    if (node->element_needs_range_check()) {
      _masm->bind(elementOutOfRange);
      _masm->hlt();
    }
    // hack for now - jcc so mapping stays alive
    // must do all the mapping in the program path taken - otherwise
    // mappings are inconsistent
    _masm->bind(exit);
    Register r = def(error);
    _masm->test(r, 0);
    jcc(Assembler::notZero, node, node->next(1));
  }
}


void CodeGenerator::anInlinedPrimitiveNode(InlinedPrimitiveNode* node) {
  switch (node->op()) {
    case InlinedPrimitiveNode::obj_klass:
      { Label is_smi;
        PRegLocker lock(node->src());
	Register obj_reg   = use(node->src());
	Register klass_reg = def(node->dst());
	_masm->movl(klass_reg, Universe::smiKlassObj());
	_masm->test(obj_reg, Mem_Tag);
	_masm->jcc(Assembler::zero, is_smi);
	_masm->movl(klass_reg, Address(obj_reg, memOopDesc::klass_byte_offset()));
	_masm->bind(is_smi);
      };
      break;
    case InlinedPrimitiveNode::obj_hash:
      { Unimplemented();
        // Implemented for the smi klass only by now - can be resolved in
	// the PrimInliner for that case without using an InlinedPrimitiveNode.
      };
      break;
    case InlinedPrimitiveNode::proxy_byte_at:
      { PReg* proxy  = node->src();
	PReg* index  = node->arg1();
	PReg* result = node->dst();
	PReg* error  = node->error();
	PRegLocker lock(proxy, index);
	// use Temporary register for proxy & index - will be modified
	Temporary base  (_currentMapping, proxy);
	Temporary offset(_currentMapping, index);
	// do index smi check if necessary
	Label indexNotSmi;
	if (!node->arg1_is_smi()) {
          _masm->test(offset.reg(), Mem_Tag);
	  jcc_error(Assembler::notZero, node, indexNotSmi);
	}
	// load element
	assert(Tag_Size == 2, "check this code");
	Register result_reg = def(result);
        _masm->movl(base.reg(), Address(base.reg(), pointer_offset));	// unbox proxy
	_masm->sarl(offset.reg(), Tag_Size);				// adjust index
	if (result_reg.hasByteRegister()) {
	  // result_reg has byte register -> can use byte load instruction
	  _masm->xorl(result_reg, result_reg);				// clear destination register
	  _masm->movb(result_reg, Address(base.reg(), offset.reg(), Address::times_1, 0));
	} else {
	  // result_reg has no byte register -> cannot use byte load instruction
	  // instead of doing better register selection use word load & mask for now
	  _masm->movl(result_reg, Address(base.reg(), offset.reg(), Address::times_1, 0));
	  _masm->andl(result_reg, 0x000000FF);				// clear uppper 3 bytes
	}
	_masm->shll(result_reg, Tag_Size);				// make result a smi
	// handle error cases if not uncommon
	if (node->canFail() && !node->next(1)->isUncommonNode()) {
          Label exit;
	  _masm->jmp(exit);
          // error messages
	  if (!node->arg1_is_smi()) {
            _masm->bind(indexNotSmi);
	    _masm->hlt();
	  }
	  // hack for now - jcc so mapping stays alive
          // must do all the mapping in the program path taken - otherwise
          // mappings are inconsistent
          _masm->bind(exit);
          Register r = def(error);
          _masm->test(r, 0);
          jcc(Assembler::notZero, node, node->next(1));
	}
      }
      break;
    case InlinedPrimitiveNode::proxy_byte_at_put:
      { bool const_val = node->arg2()->isConstPReg();
        PReg* proxy = node->src();
	PReg* index = node->arg1();
	PReg* value = node->arg2();
	PReg* error = node->error();
	// Locking turned off for now -> blocks too many registers for
	// this code (however may add unnecessary moves) -> find a better
	// solution for this
	//
	// PRegLocker lock(proxy, index, value);
	// use Temporary register for proxy & index - will be modified
	Temporary base  (_currentMapping, proxy);
	Temporary offset(_currentMapping, index);
	// use temporary register for value - will be modified
	// (actually only needed if not const_val - however right now
	// we can only allocate temps via constructors (i.e., they have
	// to be allocated/deallocated in a nested manner)).
	Temporary val(_currentMapping);
	if (const_val) {
	  // value doesn't have to be loaded -> do nothing here
          if (!node->arg2_is_smi()) fatal("proxy_byte_at_put: should not happen - internal error");
          //if (!node->arg2_is_smi()) fatal("proxy_byte_at_put: should not happen - tell Robert");
	} else {
	  _masm->movl(val.reg(), use(value));
	}
	// do index smi check if necessary
	Label indexNotSmi;
	if (!node->arg1_is_smi()) {
          _masm->test(offset.reg(), Mem_Tag);
	  jcc_error(Assembler::notZero, node, indexNotSmi);
	}
	// do value smi check if necessary
	Label valueNotSmi;
	if (!node->arg2_is_smi()) {
          assert(!const_val, "constant shouldn't need a smi check");
          _masm->test(val.reg(), Mem_Tag);
	  jcc_error(Assembler::notZero, node, valueNotSmi);
	}
	// store element
        assert(Tag_Size == 2, "check this code");
        _masm->movl(base.reg(), Address(base.reg(), pointer_offset));	// unbox proxy
	_masm->sarl(offset.reg(), Tag_Size);				// adjust index
	if (const_val) {
          smiOop constant = smiOop(((ConstPReg*)value)->constant);
	  assert(constant->is_smi(), "should be a smi");
	  _masm->movb(Address(base.reg(), offset.reg(), Address::times_1, 0), constant->value() & 0xFF);
	} else {
	  _masm->sarl(val.reg(), Tag_Size);				// convert (smi)value into int
	  if (val.reg().hasByteRegister()) {
	    // val.reg() has byte register -> can use byte store instruction
	    _masm->movb(Address(base.reg(), offset.reg(), Address::times_1, 0), val.reg());
	  } else {
	    // val.reg() has no byte register -> cannot use byte store instruction
	    // instead of doing a better register selection use word load/store & mask for now
	    Temporary field(_currentMapping);
	    _masm->movl(field.reg(), Address(base.reg(), offset.reg(), Address::times_1, 0));
	    _masm->andl(val  .reg(), 0x000000FF);			// make sure value is one byte only
	    _masm->andl(field.reg(), 0xFFFFFF00);			// mask out lower byte of target field
	    _masm->orl (field.reg(), val.reg());			// move value byte into target field
	    _masm->movl(Address(base.reg(), offset.reg(), Address::times_1, 0), field.reg());
	  }
	}
	// handle error cases if not uncommon
	if (node->canFail() && !node->next(1)->isUncommonNode()) {
          Label exit;
	  _masm->jmp(exit);
          // error messages
	  if (!node->arg1_is_smi()) {
            _masm->bind(indexNotSmi);
	    _masm->hlt();
	  }
	  if (!node->arg2_is_smi()) {
	    _masm->bind(valueNotSmi);
	    _masm->hlt();
	  }
	  // hack for now - jcc so mapping stays alive
          // must do all the mapping in the program path taken - otherwise
          // mappings are inconsistent
          _masm->bind(exit);
          Register r = def(error);
          _masm->test(r, 0);
          jcc(Assembler::notZero, node, node->next(1));
	}
      }
      break;
    default: ShouldNotReachHere();
  }
}


void CodeGenerator::anUncommonNode(UncommonNode* node) {
  //_currentMapping->saveRegisters();
  //_currentMapping->killRegisters();
  updateDebuggingInfo(node);
  _masm->call(StubRoutines::unused_uncommon_trap_entry(), relocInfo::uncommon_type);
  setMapping(NULL);
}


void CodeGenerator::aFixedCodeNode(FixedCodeNode* node) {
  switch(node->kind()) {
    case FixedCodeNode::dead_end:     _masm->hlt(); setMapping(NULL); break;
    case FixedCodeNode::inc_counter:  incrementInvocationCounter(); break;
    default:			      fatal1("unexpected FixedCodeNode kind %d", node->kind());
  }
}


# endif


