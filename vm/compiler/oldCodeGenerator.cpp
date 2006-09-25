/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.27 $ */
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
# include "incls/_oldCodeGenerator.cpp.incl"


static bool bb_needs_jump;
// true if basic block needs a jump at the end to its successor, false otherwise
// Note: most gen() nodes with more than one successor are implemented such that
//       next() is the fall-through case. If that's not the case, an extra jump
//       has to be generated (via endOfBasicBlock()). However, some of the nodes
//       do explicit jumps to all successors to accomodate for arbitrary node
//       reordering, in which case they may set the flag to false (it is auto-
//       matically set to true for each node).
// This flag should go away at soon as all node with more than one exit are
// implemented correctly (i.e., do all the jumping themselves).


void OldCodeGenerator::beginOfBasicBlock(Node* node) {
  theMacroAssm->bind(node->label);
}


void OldCodeGenerator::endOfBasicBlock(Node* node) {
  if (bb_needs_jump && node->next() != NULL) theMacroAssm->jmp(node->next()->label);
}


void OldCodeGenerator::beginOfNode(Node* node) {
  // assume that all nodes that may terminate a basic block need a jump at the end
  // (turned off for individual nodes by their gen() methods if no jump is needed
  // because they generate code patterns that generate the jumps already)
  bb_needs_jump = true;
}


void OldCodeGenerator::endOfNode(Node* node) {
  // nothing to do
}


void OldCodeGenerator::aPrologueNode(PrologueNode* node)		{ node->gen(); }
void OldCodeGenerator::aLoadIntNode(LoadIntNode* node)			{ node->gen(); }
void OldCodeGenerator::aLoadOffsetNode(LoadOffsetNode* node)		{ node->gen(); }
void OldCodeGenerator::aLoadUplevelNode(LoadUplevelNode* node)		{ node->gen(); }
void OldCodeGenerator::anAssignNode(AssignNode* node)			{ node->gen(); }
void OldCodeGenerator::aStoreOffsetNode(StoreOffsetNode* node)		{ node->gen(); }
void OldCodeGenerator::aStoreUplevelNode(StoreUplevelNode* node)	{ node->gen(); }
void OldCodeGenerator::anArithRRNode(ArithRRNode* node)			{ node->gen(); }
void OldCodeGenerator::aFloatArithRRNode(FloatArithRRNode* node)	{ node->gen(); }
void OldCodeGenerator::aFloatUnaryArithNode(FloatUnaryArithNode* node)	{ node->gen(); }
void OldCodeGenerator::anArithRCNode(ArithRCNode* node)			{ node->gen(); }
void OldCodeGenerator::aTArithRRNode(TArithRRNode* node)		{ node->gen(); }
void OldCodeGenerator::aContextCreateNode(ContextCreateNode* node)	{ node->gen(); }
void OldCodeGenerator::aContextInitNode(ContextInitNode* node)		{ node->gen(); }
void OldCodeGenerator::aContextZapNode(ContextZapNode* node)		{ node->gen(); }
void OldCodeGenerator::aBlockCreateNode(BlockCreateNode* node)		{ node->gen(); }
void OldCodeGenerator::aBlockMaterializeNode(BlockMaterializeNode* node){ node->gen(); }
void OldCodeGenerator::aSendNode(SendNode* node)			{ node->gen(); }
void OldCodeGenerator::aPrimNode(PrimNode* node)			{ node->gen(); }
void OldCodeGenerator::aDLLNode(DLLNode* node)				{ node->gen(); }
void OldCodeGenerator::aLoopHeaderNode(LoopHeaderNode* node)		{ node->gen(); }
void OldCodeGenerator::aReturnNode(ReturnNode* node)			{ node->gen(); }
void OldCodeGenerator::aNLRSetupNode(NLRSetupNode* node)		{ node->gen(); }
void OldCodeGenerator::anInlinedReturnNode(InlinedReturnNode* node)	{ node->gen(); }
void OldCodeGenerator::aNLRContinuationNode(NLRContinuationNode* node)	{ node->gen(); }
void OldCodeGenerator::aBranchNode(BranchNode* node)			{ node->gen(); }
void OldCodeGenerator::aTypeTestNode(TypeTestNode* node)		{ node->gen(); }
void OldCodeGenerator::aNLRTestNode(NLRTestNode* node)			{ node->gen(); }
void OldCodeGenerator::aMergeNode(MergeNode* node)			{ node->gen(); }
void OldCodeGenerator::anArrayAtNode(ArrayAtNode* node)			{ node->gen(); }
void OldCodeGenerator::anArrayAtPutNode(ArrayAtPutNode* node)		{ node->gen(); }
void OldCodeGenerator::anInlinedPrimitiveNode(InlinedPrimitiveNode* node){ node->gen(); }
void OldCodeGenerator::anUncommonNode(UncommonNode* node)		{ node->gen(); }
void OldCodeGenerator::aFixedCodeNode(FixedCodeNode* node)		{ node->gen(); }
void OldCodeGenerator::aNopNode(NopNode* node)				{ node->gen(); }
void OldCodeGenerator::aCommentNode(CommentNode* node)			{ node->gen(); }


//----------------------------------------------------------------------------------------------------
//
// Implementation of gen() nodes


// Inline caches
//
// An inline cache is implemented via a dummy instruction that
// follows the call immediately. The instruction's 32bit immediate
// value provides the icache information. The instruction itself
// does not modify the CPU state except the flags which are in
// an undefined state after a call, anyway.

static void inlineCache(Label& nlrTestPoint, SendInfo* info, bool super) {
  // generates the inline cache information (must follow a call instruction immediately)
  char flags = 0;
  if (super)				setNth(flags, super_send_bit_no);
  if (info && info->uninlinable)	setNth(flags, uninlinable_bit_no);
  if (info && info->receiverStatic)	setNth(flags, receiver_static_bit_no);
  theMacroAssm->ic_info(nlrTestPoint, flags);
}


// Calls to C land
//
// When entering C land, the ebp & esp of the last Delta frame have to be recorded.
// When leaving C land, last_Delta_fp has to be reset to 0. This is required to
// allow proper stack traversal.

static void call_C(char* dest, relocInfo::relocType relocType, bool needsDeltaFPCode) {
  if (needsDeltaFPCode) theMacroAssm->set_last_Delta_frame_before_call();
  theMacroAssm->call(dest, relocType);
  if (needsDeltaFPCode) theMacroAssm->reset_last_Delta_frame();
}


static void call_C(char* dest, relocInfo::relocType relocType, bool needsDeltaFPCode, Label& nlrTestPoint) {
  if (needsDeltaFPCode) theMacroAssm->set_last_Delta_frame_before_call();
  theMacroAssm->call(dest, relocType);
  inlineCache(nlrTestPoint, NULL, false);
  if (needsDeltaFPCode) theMacroAssm->reset_last_Delta_frame();
}


// Routines for debugging
//
// The verifyXXX routines are called from within compiled code if the
// VerifyCode flag is set. The routines do plausibility checks on objects
// and trap in case of an error. The verifyXXXCode routines are used to
// generate the transparent call stubs for the verifyXXX's.

static int callDepth = 0;	// to indent tracing messages
static int numberOfCalls = 0;	// # of traced calls since start

static void indent() { 
  const int maxIndent = 30;
  if (callDepth < maxIndent) {
    lprintf("%*s", callDepth, " ");
  } else {
    lprintf("%*s <%5d>", maxIndent - 9, " ", callDepth);
  }
}


static char* nmethodName() {
  deltaVFrame* f = DeltaProcess::active()->last_delta_vframe();
   return f->method()->selector()->as_string();
}


static void breakpointCode() {
  // generates a transparent call to a breakpoint routine where
  // a breakpoint can be set - for debugging purposes only
  if (!VerifyCode) warning(": breakpoint should not be called");
  theMacroAssm->pushad();
  call_C((char*)breakpoint, relocInfo::runtime_call_type, true);
  theMacroAssm->popad();
}


static void verifyOopCode(Register reg) {
  // generates transparent check code which test the contents of
  // reg for the mark bit and halts if set - for debugging purposes only
  if (!VerifyCode) warning(": verifyOop should not be called");
  Label L;
  theMacroAssm->test(reg, Mark_Tag_Bit);
  theMacroAssm->jcc(Assembler::zero, L);
  theMacroAssm->hlt();
  theMacroAssm->bind(L);
}


extern "C" void verifyContext(oop obj) {
  // verify entire context chain
  contextOop ctx = contextOop(obj); 
  while (1) {
    if (ctx->is_mark()) error("context should never be mark");
    if (!Universe::is_heap((oop*)ctx)) error("context outside of heap");
    if (!ctx->is_context()) error("should be a context");
    if (ctx->unoptimized_context() != NULL) {
      error("context has been deoptimized -- shouldn't use in compiled code");
    }
    if (!ctx->has_outer_context()) break; 
    ctx = ctx->outer_context();
  }
}


static void verifyContextCode(Register reg) {
  // generates transparent check code which verifies that reg contains
  // a legal context and halts if not - for debugging purposes only
  if (!VerifyCode) warning(": verifyContext should not be called");
  theMacroAssm->pushad();
  theMacroAssm->pushl(reg);	// pass argument (C calling convention)
  call_C((char*)verifyContext, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, oopSize);   // get rid of argument
  theMacroAssm->popad();
}


extern "C" void verifyNilOrContext(oop obj) {
  if (obj != nilObj) verifyContext(obj);
}


static void verifyNilOrContextCode(Register reg) {
  // generates transparent check code which verifies that reg contains
  // a legal context and halts if not - for debugging purposes only
  if (!VerifyCode) warning(": verifyNilOrContext should not be called");
  theMacroAssm->pushad();
  theMacroAssm->pushl(reg);	// pass argument (C calling convention)
  call_C((char*)verifyNilOrContext, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, oopSize);   // get rid of argument
  theMacroAssm->popad();
}


extern "C" void verifyBlock(blockClosureOop blk) {
  blk->verify();
}


static void verifyBlockCode(Register reg) {
  // generates transparent check code which verifies that reg contains
  // a legal context and halts if not - for debugging purposes only
  if (!VerifyCode) warning(": verifyBlockCode should not be called");
  theMacroAssm->pushad();
  theMacroAssm->pushl(reg);	// pass argument (C calling convention)
  call_C((char*)verifyBlock, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, oopSize);   // get rid of argument
  theMacroAssm->popad();
}


static int NumberOfReturns = 0;	  // for debugging (conditional breakpoints)

extern "C" void verifyReturn(oop obj) {
  NumberOfReturns++;
  obj->verify();
  if (TraceCalls) {
    ResourceMark rm;
    callDepth--;
    indent(); lprintf("return %s from %s\n", obj->print_value_string(), nmethodName()); 
  }
}


static void verifyReturnCode(Register reg) {
  // generates transparent check code which verifies that reg contains
  // a legal context and halts if not - for debugging purposes only
  if (!VerifyCode && !GenTraceCalls) warning(": verifyReturn should not be called");
  theMacroAssm->pushad();
  theMacroAssm->pushl(reg);	// pass argument (C calling convention)
  call_C((char*)verifyReturn, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, oopSize);   // get rid of argument
  theMacroAssm->popad();
}


extern "C" void verifyNLR(char* fp, char* nlrFrame, int nlrScopeID, oop nlrResult) {
  LOG_EVENT3("verifyNLR(%#x, %#x, %d, %#x)", fp, nlrFrame, nlrResult);
  if (nlrFrame <= fp) error("NLR went too far: %#x <= %#x", nlrFrame, fp);
  // treat >99 scopes as likely error -- might actually be ok
  if (nlrScopeID < 0 || nlrScopeID > 99) error("illegal NLR scope ID %#x", nlrScopeID);
  if (nlrResult->is_mark()) error("NLR result is a markOop");
  if (TraceCalls) {
    ResourceMark rm;
    callDepth--;
    indent(); lprintf("NLR %s from/thru %s\n", nlrResult->print_value_string(), nmethodName()); 
  }
}


static void verifyNLRCode() {
  // generates transparent check code which verifies NLR check & continuation
  if (!VerifyCode) warning(": verifyNLRCode should not be called");
  theMacroAssm->pushad();
  theMacroAssm->pushl(Mapping::asRegister(NLRResultLoc));	// pass argument (C calling convention)
  theMacroAssm->pushl(Mapping::asRegister(NLRHomeIdLoc));
  theMacroAssm->pushl(Mapping::asRegister(NLRHomeLoc));
  theMacroAssm->pushl(ebp);
  call_C((char*)verifyNLR, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, 4*oopSize);   // get rid of arguments
  theMacroAssm->popad();
}


extern "C" void verifySmi(oop obj) {
  if (!obj->is_smi()) fatal("should be a smi");
}


static void verifySmiCode(Register reg) {
  // generates transparent check code which verifies that reg contains
  // a legal smi and halts if not - for debugging purposes only
  if (!VerifyCode) warning(": verifySmi should not be called");
  theMacroAssm->pushad();
  theMacroAssm->pushl(reg);	// pass argument (C calling convention)
  call_C((char*)verifySmi, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, oopSize);   // get rid of argument
  theMacroAssm->popad();
}


extern "C" void verifyObj(oop obj) {
  if (!obj->is_smi() && !obj->is_mem()) fatal("should be an ordinary oop");
  klassOop klass = obj->klass();
  if (klass == NULL || !klass->is_mem()) fatal("should be an ordinary memOop");
  if (obj->is_block()) blockClosureOop(obj)->verify();
}


static void verifyObjCode(Register reg) {
  // generates transparent check code which verifies that reg contains
  // a legal oop and halts if not - for debugging purposes only
  if (!VerifyCode) warning(": verifyObj should not be called");
  theMacroAssm->pushad();
  theMacroAssm->pushl(reg);	// pass argument (C calling convention)
  call_C((char*)verifyObj, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, oopSize);   // get rid of argument
  theMacroAssm->popad();
}


extern "C" void verifyArguments(oop recv, int ebp, int nofArgs) {
  ResourceMark rm;
  numberOfCalls++;
  if (TraceCalls) { 
    callDepth++;
    indent(); 
    lprintf("calling %s %s ", nmethodName(), recv->print_value_string());
  }
  verifyObj(recv);
  int i = nofArgs;
  oop* arg = (oop*)(ebp + (nofArgs + 2)*oopSize);
  while (i-- > 0) {
    arg--;
    verifyObj(*arg);
    if (TraceCalls) {
      ResourceMark rm;
      lprintf("%s, ", (*arg)->print_value_string());
    }
  }
  if (VerifyDebugInfo) { 
    deltaVFrame* f = DeltaProcess::active()->last_delta_vframe();
    while (f != NULL) {
      f->verify_debug_info();
      f = f->sender_delta_frame();
    }
  }
}


static void verifyArgumentsCode(Register recv, int nofArgs) {
  // generates transparent check code which verifies that all arguments
  // are legal oops and halts if not - for debugging purposes only
  assert(VerifyCode || GenTraceCalls || VerifyDebugInfo, "performance bug: verifyArguments should not be called");
  theMacroAssm->pushad();
  theMacroAssm->pushl(nofArgs);	// pass arguments (C calling convention)
  theMacroAssm->pushl(ebp);
  theMacroAssm->pushl(recv);
  call_C((char*)verifyArguments, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, 3*oopSize);   // get rid of arguments
  theMacroAssm->popad();
}


static int result_counter = 0;

static void trace_result(int compilation, methodOop method, oop result) {
  ResourceMark rm;
  std->print("%6d: 0x%08x (compilation %4d, ", result_counter++, int(result), compilation);
  method->selector()->print_value();
  std->print(")\n", compilation);
}


static void call_trace_result(Register result) {
  theMacroAssm->pushad();
  theMacroAssm->pushl(result);
  theMacroAssm->pushl(theCompiler->method);
  theMacroAssm->pushl(nofCompilations);
  call_C((char*)trace_result, relocInfo::runtime_call_type, true);
  theMacroAssm->addl(esp, 3*oopSize);   // get rid of arguments
  theMacroAssm->popad();
}


// helper functions for loading/storing

static Register moveLocToReg(Location src, Register temp) {
  // Returns the register corresponding to src if src is a register location,
  // otherwise loads the value at loc into register temp and returns temp.
  if (src.isRegisterLocation()) {
    return Mapping::asRegister(src);
  } else {
    Mapping::load(src, temp);
    return temp;
  }
}


static Register movePRegToReg(PReg* src, Register temp) {
  // Returns the src register if a register has been assigned to src,
  // otherwise loads the value src into register temp and returns temp.
  if (src->isConstPReg()) {
    theMacroAssm->movl(temp, ((ConstPReg*)src)->constant);
    return temp;
  } else {
    return moveLocToReg(src->loc, temp);
  }
}


static inline Register answerLocReg(Location src, Register temp) {
  // Returns the register corresponding to src if src is a register location,
  // otherwise returns temp.
  return src.isRegisterLocation() ? Mapping::asRegister(src) : temp;
}


static inline Register answerPRegReg(PReg* src, Register temp) {
  // Returns the src register if a register has been assigned to src,
  // otherwise returns temp.
  return answerLocReg(src->loc, temp);
}


static void load(PReg* src, Register dst) {
  // Loads src into register dst.
  if (src->isConstPReg()) {
    theMacroAssm->movl(dst, ((ConstPReg*)src)->constant);
  } else {
    Mapping::load(src->loc, dst);
  }
}


static void fload(PReg* src, Register base, Register temp) {
  assert(base != temp, "registers must be different");
  // Loads src into FPU ST
  if (src->isConstPReg()) {
    theMacroAssm->movl(temp, ((ConstPReg*)src)->constant);
    theMacroAssm->fld_d(Address(temp, byteOffset(doubleOopDesc::value_offset()))); // unbox float
  } else {
    Mapping::fload(src->loc, base);
  }
}


static void store(Register src, PReg* dst, Register temp1, Register temp2, bool needsStoreCheck = true) {
  // Stores register src to dst.
  assert(!dst->isConstPReg(), "destination cannot be a constant");
  Mapping::store(src, dst->loc, temp1, temp2, needsStoreCheck);
}


static void fstore(PReg* dst, Register base) {
  // Stores FPU ST to dst and pops ST
  assert(!dst->isConstPReg(), "destination cannot be a constant");
  Mapping::fstore(dst->loc, base);
}


static void storeO(ConstPReg* src, PReg* dst, Register temp1, Register temp2, bool needsStoreCheck = true) {
  // Stores constant src to dst.
  assert(!dst->isConstPReg(), "destination cannot be a constant");
  Mapping::storeO(src->constant, dst->loc, temp1, temp2, needsStoreCheck);
}


// The float section on the stack is 8byte-aligned. In order to access it, a base register
// instead of ebp is used. This base register holds the 8byte aligned ebp value. For now,
// all node accessing floats are using the same base register (temp3); this allows us to
// get rid of unneccessary base register setup code if the previous node set it up already.

static void set_floats_base(Node* node, Register base, bool enforce = false) {
  // Stores aligned ebp value into base
  assert(floatSize == 8, "check this code");
  assert(node->isAccessingFloats(), "must be a node accessing floats");
  assert(base == temp3, "check this code");
  if (node->hasSinglePredecessor() && node->firstPrev()->isAccessingFloats() && !enforce) {
    // previous node is also accessing floats => base register is
    // already set => no extra code necessary
  } else {
    theMacroAssm->movl(base, ebp);
    theMacroAssm->andl(base, -floatSize); 
  }
}


static void assign(Node* node, PReg* src, PReg* dst, Register temp1, Register temp2, Register temp3, bool needsStoreCheck = true) {
  // General assignment
  assert(temp1 != temp2 && temp1 != temp3 && temp2 != temp3, "registers must be different");
  if (src->loc != dst->loc) {
    if (node->isAccessingFloats()) {
      Register base = temp3;
      set_floats_base(node, base);
      fload(src, base, temp1);
      fstore(dst, base);
    } else if (src->isConstPReg()) {
      // assign constants directly without loading into temporary register first
      storeO((ConstPReg*)src, dst, temp1, temp2, needsStoreCheck);
    } else {
      Register t = movePRegToReg(src, answerPRegReg(dst, temp1));
      store(t, dst, temp2, temp3, needsStoreCheck);
    }
  }
}


static Register uplevelBase(PReg* startContext, int nofLevels, Register temp) {
  // Compute uplevel base; nofLevels is number of indirections (0 = in this context)
  Register b = nofLevels > 0 ? temp : answerPRegReg(startContext, temp);
  load(startContext, b);
  while (nofLevels-- > 0) { 
   if (VerifyCode) verifyContextCode(b);
   theMacroAssm->Load(b, contextOopDesc::parent_byte_offset(), b); 
  }
  return b;
}


// Code generation for statistical information on nmethods

static char* nmethodAddr() {
  // hack to compute hypothetical nmethod address
  // should be fixed at some point
  return (char*)(((nmethod*) (theMacroAssm->code()->code_begin())) - 1);
}


static void incCounter() {
  // Generates code to increment the nmethod execution counter
  char* addr = nmethodAddr() + nmethod::invocationCountOffset();
  theMacroAssm->incl(Address(int(addr), relocInfo::internal_word_type));
}


// Helper functions for general code generation

static Assembler::Condition mapToCC(BranchOpCode op) {
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
    default: ShouldNotReachHere(); return Assembler::zero;
  }
}


static void primitiveCall(InlinedScope* scope, primitive_desc* pdesc) {
  if (pdesc->can_perform_NLR()) {
    call_C((char*)(&pdesc->fn()), relocInfo::prim_type, pdesc->needs_delta_fp_code(), scope->nlrTestPoint()->label);
  } else {
    call_C((char*)(&pdesc->fn()), relocInfo::prim_type, pdesc->needs_delta_fp_code());
  }
}


static void zapContext(PReg* context, Register temp) {
  Register c = movePRegToReg(context, temp);
  theMacroAssm->movl(Address(c, contextOopDesc::parent_byte_offset()), 0);
}


static void continueNLR(Register temp1, Register temp2) {
  assert(temp1 == ::temp1 && temp2 == ::temp2, "different register usage than stub routine - check this");
  if (VerifyCode) verifyNLRCode();
  theMacroAssm->jmp(StubRoutines::continue_NLR_entry(), relocInfo::runtime_call_type);
}


// Code generation for individual nodes

void BasicNode::gen() {
  ScopeDescRecorder* rec = theCompiler->scopeDescRecorder();
  rec->addPcDesc(theMacroAssm->offset(), _scope->scopeInfo(), _bci);
}


static void checkRecompilation(Label& recompile_stub_call, Register t) {
  if (RecompilationPolicy::needRecompileCounter(theCompiler)) {
    // increment the nmethod execution counter and check limit
    char* addr = nmethodAddr() + nmethod::invocationCountOffset();
    theMacroAssm->movl(t, Address(int(addr), relocInfo::internal_word_type));
    theMacroAssm->incl(t);
    theMacroAssm->cmpl(t, theCompiler->get_invocation_counter_limit());
    theMacroAssm->movl(Address(int(addr), relocInfo::internal_word_type), t);
    theMacroAssm->jcc(Assembler::greaterEqual, recompile_stub_call);
  }
}


static void verify_context_chain(Register closure, int chain_length, Register temp1, Register temp2) {
  // Generates code to verify the context chain of a block closure. If the chain
  // contains deoptimized contextOops, the block has to be deoptimized as well.
  // Method: A bit in the mark field of each context indicates whether it has
  //         been deoptimized or not. All mark fields of the contexts in the
  //         context chain are or-ed together and the bit is checked at the end.
  assert(closure != temp1 && closure != temp2 && temp1 != temp2, "registers must be different");
  assert(chain_length >= 1, "must have at least one context in the chain");
  const Register context = temp1;
  const Register sum     = temp2;
  // initialize sum with mark of first context
  theMacroAssm->movl(context, Address(closure, blockClosureOopDesc::context_byte_offset()));
  theMacroAssm->movl(sum, Address(context, memOopDesc::mark_byte_offset()));
  // 'or' the mark fields of the remaining contexts in the chain to sum
  for (int i = chain_length - 1; i-- > 0; ) {
    theMacroAssm->movl(context, Address(context, contextOopDesc::parent_byte_offset()));
    theMacroAssm->orl(sum, Address(context, memOopDesc::mark_byte_offset()));
  }
  // check if there was any context that has been deoptimized
  theMacroAssm->testl(sum, markOopDesc::context_forward_bit_mask());
  theMacroAssm->jcc(Assembler::notZero, StubRoutines::deoptimize_block_entry(), relocInfo::runtime_call_type);
  // otherwise continue
}


void PrologueNode::gen() {
  BasicNode::gen();

  // call to recompiler - if the nmethod turns zombie, this will be overwritten by a call to the zombie handler
  // (see also comment in nmethod)
  Label recompile_stub_call;
  theMacroAssm->bind(recompile_stub_call);
  theCompiler->set_special_handler_call_offset(theMacroAssm->offset());
  theMacroAssm->call(StubRoutines::recompile_stub_entry(), relocInfo::runtime_call_type);

  // entry point for callers who need to verify receiver klass or if block
  theMacroAssm->align(oopSize);
  theCompiler->set_entry_point_offset(theMacroAssm->offset());
  Register recv = moveLocToReg(selfLoc, temp1);
  if (scope()->isMethodScope()) {
    // check class
    klassOop klass = _scope->selfKlass();
    if (klass == smiKlassObj) {
      // receiver must be a smi, check smi tag only
      theMacroAssm->test(recv, Mem_Tag);
      theMacroAssm->jcc(Assembler::notZero, CompiledIC::normalLookupRoutine());
    } else {
      // receiver could be a smi, check smi tag before loading class
      theMacroAssm->test(recv, Mem_Tag);
      theMacroAssm->jcc(Assembler::zero, CompiledIC::normalLookupRoutine());
      theMacroAssm->cmpl(Address(recv, memOopDesc::klass_byte_offset()), klass);
      theMacroAssm->jcc(Assembler::notEqual, CompiledIC::normalLookupRoutine());
    }
  } else {
    // If this is a block method and we expect a context
    // then the incoming context chain must be checked.
    // The context chain may contain a deoptimized contextOop.
    // (see StubRoutines::verify_context_chain for details)
    if (scope()->method()->block_info() == methodOopDesc::expects_context) {
      const bool use_fast_check = false;			// turn this off if it doesn't work
      if (use_fast_check) {
        // What happens if the context chain is not anchored in a method?
	// Probably doesn't work correctly - think about this - gri 6/26/96
	// Turned off for now - because of problems. Should fix this.
        int length = _scope->homeContext() + 1;			// includes context created within this scope
        if (scope()->allocatesCompiledContext()) length--;	// context has not been created yet -> adjust length
        verify_context_chain(recv, length, temp2, temp3);
      } else {
        theMacroAssm->call(StubRoutines::verify_context_chain(), relocInfo::runtime_call_type);
      }
    }
  }

  // callers who know the receiver class (e.g., PICs) should enter here
  theMacroAssm->align(oopSize);
  theCompiler->set_verified_entry_point_offset(theMacroAssm->offset());
  // build stack frame
  int frame_size = 2;	// return address & old ebp
  theMacroAssm->enter();
  // allocate float temporaries
  int nofFloats = theCompiler->totalNofFloatTemporaries();
  if (nofFloats > 0) {
    assert(floatSize == oopSize*2, "check this code");
    assert(first_float_offset == -4, "check this code");
    int float_section_size = nofFloats*(floatSize/oopSize) + 2;	// 2 additional words for filler & float alignment
    frame_size += 1 + float_section_size;			// magic word & floats
    theMacroAssm->pushl(Floats::magic);				// magic word
    theMacroAssm->subl(esp, float_section_size * oopSize);	// add one word for float alignment
    theCompiler->set_float_section_size(float_section_size);
    theCompiler->set_float_section_start_offset(-2);		// float_section after ebp & magic word
  }
  // allocate normal temporaries
  int nofTemps = theAllocator->nofStackTemps();
  if (nofTemps > 0) {
    assert(first_temp_offset == -1, "check this code");
    frame_size += nofTemps;
    theMacroAssm->movl(temp2, nilObj);
    for (int i = 0; i < nofTemps; i++) theMacroAssm->pushl(temp2);
  }
  // make sure frame is big enough for deoptimization
  if (frame_size < minimum_size_for_deoptimized_frame) {
    if (nofTemps == 0) theMacroAssm->movl(temp2, nilObj);	// make sure temp2 holds nil
    while (frame_size < minimum_size_for_deoptimized_frame) {
      frame_size++;
      theMacroAssm->pushl(temp2);
    }
  }

  if (VerifyCode || VerifyDebugInfo || GenTraceCalls) verifyArgumentsCode(recv, scope()->method()->number_of_arguments());

  // initialize self and context (for blocks)
  // recv has already been loaded (possibly into temp1)
  if (scope()->isMethodScope()) {
    store(recv, scope()->self()->preg(), temp2, temp3);
  } else {
    // recv contains block closure -> get context out of it
    Register c = answerPRegReg(scope()->self()->preg(), temp2);
    theMacroAssm->Load(recv, blockClosureOopDesc::context_byte_offset(), c);
    store(c, scope()->self()->preg(), temp1, temp3);
    store(c, scope()->context(), temp1, temp3);

    if (VerifyCode) {
      switch (scope()->method()->block_info()) {
        case methodOopDesc::expects_nil      : verifyNilOrContextCode(c); break;
	case methodOopDesc::expects_self     : // fall through
	case methodOopDesc::expects_parameter: verifyOopCode(c);          break;
	case methodOopDesc::expects_context  : verifyContextCode(c);      break;
	default: ShouldNotReachHere();
      }
    }
  }
  // check for recompilation (do this last so stack frame is initialized properly)
  checkRecompilation(recompile_stub_call, temp2);
}


void LoadIntNode::gen() {
  BasicNode::gen();
  Register t = answerPRegReg(_dest, temp1);
  theMacroAssm->movl(t, _value);
  store(t, _dest, temp2, temp3);
}


void LoadOffsetNode::gen() {
  BasicNode::gen();
  Register b = movePRegToReg(_src, temp1);
  Register t = answerPRegReg(_dest, temp2);
  theMacroAssm->Load(b, byteOffset(offset), t);
  store(t, _dest, temp1, temp3);
}


void LoadUplevelNode::gen() {
  BasicNode::gen();
  Register b = uplevelBase(_context0, _nofLevels, temp1);
  Register t = answerPRegReg(_dest, temp2);
  theMacroAssm->Load(b, byteOffset(_offset), t);
  if (VerifyCode) verifyObjCode(t);
  store(t, _dest, temp1, temp3);
}


void StoreOffsetNode::gen() {
  BasicNode::gen();
  Register b = movePRegToReg(_base, temp1);
  Register t = movePRegToReg(_src, temp2);
  theMacroAssm->Store(t, b, byteOffset(_offset));
  if (_needsStoreCheck) {
    // NB: make sure b is a copy of base because storeCheck overwrites it (was bug 3/9/96 -Urs)
    if (b != temp1) load(_base, temp1);
    theMacroAssm->store_check(temp1, temp2);
  }
}


void StoreUplevelNode::gen() {
  StoreNode::gen();
  Register b = uplevelBase(_context0, _nofLevels, temp1);
  Register t = movePRegToReg(_src, temp2);
  theMacroAssm->Store(t, b, byteOffset(_offset));
  if (_needsStoreCheck) {
    // NB: make sure b is a copy of _context0 because storeCheck overwrites it
    if (b != temp1) load(_context0, temp1);
    theMacroAssm->store_check(temp1, temp2);
  }
}


void MergeNode::gen() {
  BasicNode::gen();
  // nothing to do
}


void SendNode::gen() {
  BasicNode::gen();
  if (isCounting()) incCounter();
  char* entry = _superSend ? CompiledIC::superLookupRoutine() : CompiledIC::normalLookupRoutine();
  theMacroAssm->call(entry, relocInfo::ic_type);
  inlineCache(scope()->nlrTestPoint()->label, _info, _superSend);
  assert(_dest->loc == resultLoc, "assignment missing");
}


void PrimNode::gen() {
  BasicNode::gen();
  primitiveCall(scope(), _pdesc);
  // assign result
  Register t = moveLocToReg(resultLoc, answerPRegReg(_dest, temp1));
  store(t, _dest, temp2, temp3);
}


void DLLNode::gen() {
  BasicNode::gen();
  assert(temp1 == ebx && temp2 == ecx && temp3 == edx, "registers are no temps anymore -> fix parameter passing");
  // determine entry point depending on whether a run-time lookup is needed or not
  // Note: do not do a DLL lookup at compile time since this may cause a call back.
  char* entry = (function() == NULL) ? StubRoutines::lookup_DLL_entry(async()) : StubRoutines::call_DLL_entry(async());
  // pass arguments for DLL_C_frame in registers
  // adjust this code if DLL_C_frame changes:
  // ebx: no. of arguments
  // ecx: address of last argument
  // edx: dll function entry point
  theMacroAssm->movl(ebx, nofArguments());	// setup no. of arguments
  theMacroAssm->movl(ecx, esp);			// setup address of last argument
  // CompiledDLL_Cache
  // This code pattern must correspond to the CompiledDLL_Cache layout
  // (make sure assembler is not optimizing mov reg, 0 into xor reg, reg!)
  theMacroAssm->movl(edx, int(function()));	// part of CompiledDLL_Cache
  theMacroAssm->inline_oop(dll_name());		// part of CompiledDLL_Cache
  theMacroAssm->inline_oop(function_name());	// part of CompiledDLL_Cache
  theMacroAssm->call(entry, relocInfo::runtime_call_type);
  // Note: should also pop arguments in case of a NLR, could become a problem
  //       if DLL is called within a loop - fix this at some point.
  inlineCache(scope()->nlrTestPoint()->label, NULL, false);
  // assign result
  theMacroAssm->addl(esp, nofArguments()*oopSize);	// get rid of arguments
  Register t = moveLocToReg(resultLoc, answerPRegReg(_dest, temp1));
  store(t, _dest, temp2, temp3);
}


static bool producesResult(ArithOpCode op) {
  return (op != TestArithOp) && (op != CmpArithOp) && (op != tCmpArithOp);
}


static bool setupRegister(PReg* dst, PReg* arg, ArithOpCode op, Register& x, Register t) {
  // Sets up register x such that x := x op <some constant> corresponds to dst := arg op <some constant>.
  // If the temporary register t is used at all, x will be in t.
  // Returns true if op generated a result in x; returns false otherwise.
  bool result = producesResult(op);
  if (result) {
    // operation generates result, try to use as few registers as possible
    if ((dst->loc == arg->loc) /* || lastUsageOf(arg) */) {
      // arg is not used anymore afterwards, can be overwritten
      x = movePRegToReg(arg, t);
    } else {
      // have to load arg into a temporary register
      x = t; load(arg, t);
    }
  } else {
    // operation generates no result, use argument register directly
    assert(dst->isNoPReg(), "dst should be a noPReg");
    x = movePRegToReg(arg, t);
  }
  return result;
}


static bool setupRegisters(PReg* dst, PReg* arg1, ArithOpCode op, PReg* arg2, Register& x, Register& y, Register t1, Register t2) {
  // Sets up registers x & y such that x := x op y corresponds to dst := arg1 op arg2.
  // If the temporary registers t1 & t2 are used at all, x will be in t1 and y in t2.
  // Returns true if op generated a result in x; returns false otherwise.
  assert(t1 != t2, "registers should be different");
  bool result = producesResult(op);
  if (result) {
    // operation generates result, try to use as few registers as possible
    if ((dst->loc == arg1->loc) /* || lastUsageOf(arg1) */) {
      // arg1 is not used anymore afterwards, can be overwritten
      x = movePRegToReg(arg1, t1);
      y = movePRegToReg(arg2, t2);
    } else {
      // have to load arg1 into a temporary register
      x = t1; load(arg1, t1);
      y = movePRegToReg(arg2, t2);
    }
  } else {
    // operation generates no result, use argument registers directly
    assert(dst->isNoPReg(), "dst should be a noPReg");
    x = movePRegToReg(arg1, t1);
    y = movePRegToReg(arg2, t2);
  }
  return result;
}


static void arithRROp(ArithOpCode op, Register x, Register y) {
  assert(Int_Tag == 0, "check this code");
  switch (op) {
    case TestArithOp  : theMacroAssm->testl(x, y);	break;
    case tAddArithOp  : // fall through
    case  AddArithOp  : theMacroAssm->addl(x, y);	break;
    case tSubArithOp  : // fall through
    case  SubArithOp  : theMacroAssm->subl(x, y);	break;
    case tMulArithOp  : theMacroAssm->sarl(x, Tag_Size);
    case  MulArithOp  : theMacroAssm->imull(x, y);	break;
    case tDivArithOp  : // fall through
    case  DivArithOp  : Unimplemented();		break;
    case tModArithOp  : // fall through
    case  ModArithOp  : Unimplemented();		break;
    case tAndArithOp  : // fall through
    case  AndArithOp  : theMacroAssm->andl(x, y);	break;
    case tOrArithOp   : // fall through
    case  OrArithOp   : theMacroAssm->orl(x, y);	break;
    case tXOrArithOp  : // fall through
    case  XOrArithOp  : theMacroAssm->xorl(x, y);	break;
    case tShiftArithOp: Unimplemented();
    case  ShiftArithOp: Unimplemented();
    case tCmpArithOp  : // fall through
    case  CmpArithOp  : theMacroAssm->cmpl(x, y);	break;
    default           : ShouldNotReachHere();
  }
}


static void arithRCOp(ArithOpCode op, Register x, int y) {
  assert(Int_Tag == 0, "check this code");
  switch (op) {
    case TestArithOp  : theMacroAssm->testl(x, y);	break;
    case tAddArithOp  : // fall through
    case  AddArithOp  : theMacroAssm->addl(x, y);	break;
    case tSubArithOp  : // fall through
    case  SubArithOp  : theMacroAssm->subl(x, y);	break;
    case tMulArithOp  : y = arithmetic_shift_right(y, Tag_Size);
    case  MulArithOp  : theMacroAssm->imull(x, x, y);	break;
    case tDivArithOp  : // fall through
    case  DivArithOp  : Unimplemented();		break;
    case tModArithOp  : // fall through
    case  ModArithOp  : Unimplemented();		break;
    case tAndArithOp  : // fall through
    case  AndArithOp  : theMacroAssm->andl(x, y);	break;
    case tOrArithOp   : // fall through
    case  OrArithOp   : theMacroAssm->orl(x, y);	break;
    case tXOrArithOp  : // fall through
    case  XOrArithOp  : theMacroAssm->xorl(x, y);	break;
    case tShiftArithOp:
      if (y < 0) {
        // shift right
        int shift_count = ((-y) >> Tag_Size) % 32;
        theMacroAssm->sarl(x, shift_count);
        theMacroAssm->andl(x, -1 << Tag_Size);		// clear Tag bits
      } else if (y > 0) {
        // shift left
        int shift_count = ((+y) >> Tag_Size) % 32;
        theMacroAssm->shll(x, shift_count);
      }
      break;
    case  ShiftArithOp: Unimplemented();
    case tCmpArithOp  : // fall through
    case  CmpArithOp  : theMacroAssm->cmpl(x, y);	break;
    default           : ShouldNotReachHere();
  }
}


static void arithROOp(ArithOpCode op, Register x, oop y) {
  assert(!y->is_smi(), "check this code");
  switch (op) {
    case  CmpArithOp  : theMacroAssm->cmpl(x, y);	break;
    default           : ShouldNotReachHere();
  }
}


void TArithRRNode::gen() {
  BasicNode::gen();
  PReg* arg1 = _src;
  PReg* arg2 = _oper;
  if (arg2->isConstPReg()) {
    oop y = ((ConstPReg*)arg2)->constant;
    assert(y->is_smi() == _arg2IsInt, "flag value inconsistent");
    if (_arg2IsInt) {
      // perform operation
      Register x;
      bool result = setupRegister(_dest, arg1, _op, x, temp1);
      if (!_arg1IsInt) {
        // tag check necessary for arg1
        theMacroAssm->test(x, Mem_Tag);
        theMacroAssm->jcc(Assembler::notZero, next(1)->label);
      }
      arithRCOp(_op, x, int(y));			// y is smiOop -> needs no relocation info
      if (result) store(x, _dest, temp2, temp3);
    } else {
      // operation fails always
      theMacroAssm->jmp(next(1)->label);
    }
  } else {
    Register x, y;
    bool result = setupRegisters(_dest, arg1, _op, arg2, x, y, temp1, temp2);
    // check argument tags
    Register tags = noreg;
    if (_arg1IsInt) {
      if (_arg2IsInt) {
        // both x & y are smis => no tag check necessary
      } else {
        // x is smi => check y
        tags = y;
      }
    } else {
      if (_arg2IsInt) {
        // y is smi => check x
        tags = x;
      } else {
        // check both x & y
	tags = temp3;
        theMacroAssm->movl(tags, x);
        theMacroAssm->orl (tags, y);
      }
    }
    if (tags != noreg) {
      // check tags
      theMacroAssm->test(tags, Mem_Tag);
      theMacroAssm->jcc(Assembler::notZero, next(1)->label);
    }
    // perform operation
    arithRROp(_op, x, y);
    if (result) {
      Register t = (x == temp1) ? temp2 : temp1;
      store(x, _dest, t, temp3);
    }
  }
}


void ArithRRNode::gen() {
  BasicNode::gen();
  PReg* arg1 = _src;
  PReg* arg2 = _oper;
  if (arg2->isConstPReg()) {
    oop y = ((ConstPReg*)arg2)->constant;
    Register x;
    bool result = setupRegister(_dest, arg1, _op, x, temp1);
    if (y->is_smi()) {
      arithRCOp(_op, x, int(y));		// y is smiOop -> needs no relocation info
    } else {
      arithROOp(_op, x, y);
    }
    if (result) store(x, _dest, temp2, temp3);
  } else {
    Register x, y;
    bool result = setupRegisters(_dest, arg1, _op, arg2, x, y, temp1, temp2);
    arithRROp(_op, x, y);
    if (result) {
      Register t = (x == temp1) ? temp2 : temp1;
      store(x, _dest, t, temp3);
    }
  }
}


void ArithRCNode::gen() {
  BasicNode::gen();
  PReg* arg1 = _src;
  int y = _oper;
  Register x;
  bool result = setupRegister(_dest, arg1, _op, x, temp1);
  arithRCOp(_op, x, y);
  if (result) store(x, _dest, temp2, temp3);
}


static void floatArithRROp(ArithOpCode op) {
  switch (op) {
    case fAddArithOp: theMacroAssm->faddp();	break;
    case fSubArithOp: theMacroAssm->fsubp();	break;
    case fMulArithOp: theMacroAssm->fmulp();	break;
    case fDivArithOp: theMacroAssm->fdivp();	break;
    case fModArithOp: theMacroAssm->fprem();	break;
    case fCmpArithOp: theMacroAssm->fcompp();	break;
    default         : ShouldNotReachHere();
  }
}


void FloatArithRRNode::gen() {
  BasicNode::gen();
  bool noResult = (_op == fCmpArithOp);
  bool exchange = (_op == fModArithOp || _op == fCmpArithOp);
  Register base = temp3;
  set_floats_base(this, base);
  fload(_src, base, temp1);
  fload(_oper, base, temp2);
  if (exchange) theMacroAssm->fxch();	// is paired with next instruction => no extra cycles
  floatArithRROp(_op);
  if (_op == fCmpArithOp) {
    // operation set FPU condition codes -> result is FPU status word
    assert(!Mapping::isFloatTemporary(_dest->loc), "fCmpArithOp doesn't produce a float");
    if (_dest->loc.isRegisterLocation() && _dest->loc.number() == eax.number()) {
      // store FPU status word in eax
      theMacroAssm->fwait();
      theMacroAssm->fnstsw_ax();
    } else {
      // store FPU status word not in eax
      Unimplemented();
    }
  } else {
    // store result (must be a float)
    fstore(_dest, base);
  }
}


static Address doubleKlass_addr() {
  return Address((int)&doubleKlassObj, relocInfo::external_word_type);
}


static oop oopify_float() {
  double x;
  __asm fstp x							// get top of FPU stack
  BlockScavenge bs;						// because all registers are saved on the stack
  return oopFactory::new_double(x);				// box the FloatValue
}


static void floatArithROp(ArithOpCode op, Register reg, Register temp) {
  assert(reg != temp, "registers must be different");
  switch (op) {
    case fNegArithOp   : theMacroAssm->fchs();	break;
    case fAbsArithOp   : theMacroAssm->fabs();	break;
    case fSqrArithOp   : theMacroAssm->fmul(0);	break;
    case f2OopArithOp  :
      { theMacroAssm->pushl(reg);				// reserve space for the result
        theMacroAssm->pushad();					// make sure no register is destroyed (no scavenge)
        theMacroAssm->call((char*)oopify_float, relocInfo::runtime_call_type);
	theMacroAssm->movl(Address(esp, nofRegisters * oopSize), eax);	// store result at reserved stack location
	theMacroAssm->popad();					// restore register contents
	theMacroAssm->popl(reg);				// get result
      }
      break;
    case f2FloatArithOp:
      { Label is_smi, is_float, done;
        theMacroAssm->test(reg, Mem_Tag);			// check if smi
	theMacroAssm->jcc(Assembler::zero, is_smi);
        theMacroAssm->movl(temp, Address(reg, memOopDesc::klass_byte_offset()));	// get object klass
        theMacroAssm->cmpl(temp, doubleKlass_addr());		// check if floatOop
	theMacroAssm->jcc(Assembler::equal, is_float);
	theMacroAssm->hlt(); // not yet implemented		// cannot be converted

	// convert smi
	theMacroAssm->bind(is_smi);
	theMacroAssm->sarl(reg, Tag_Size);			// convert smi into int
	theMacroAssm->movl(Address(esp, -oopSize), reg);	// store it at end of stack
	theMacroAssm->fild_s(Address(esp, -oopSize));		// load & convert into FloatValue
	theMacroAssm->jmp(done);

        // unbox doubleOop
	theMacroAssm->bind(is_float);
        theMacroAssm->fld_d(Address(reg, byteOffset(doubleOopDesc::value_offset()))); // unbox float

	theMacroAssm->bind(done);
      }
      break;
    default            : ShouldNotReachHere();
  }
}


void FloatUnaryArithNode::gen() {
  BasicNode::gen();
  Register reg;
  Register base = temp3;
  set_floats_base(this, base);
  if (Mapping::isFloatTemporary(_src->loc) || _src->loc == topOfFloatStack) {
    // load argument on FPU stack & setup reg if result is an oop
    fload(_src, base, temp1);
    reg = temp1;
  } else {
    // load argument into reg
    reg = movePRegToReg(_src, temp1);
  }
  floatArithROp(_op, reg, temp2);
  if (Mapping::isFloatTemporary(_dest->loc) || _dest->loc == topOfFloatStack) {
    // result is on FPU stack
    fstore(_dest, base);
  } else {
    // result is in reg
    store(reg, _dest, temp2, temp3);
    set_floats_base(this, base, true);	// store may overwrite base -> make sure it is set again
  }
}


void AssignNode::gen() {
  StoreNode::gen();
  assign(this, _src, _dest, temp1, temp2, temp3);
}


void BranchNode::gen() {
  BasicNode::gen();
  theMacroAssm->jcc(mapToCC(_op), next(1)->label);
}


/*
void BranchNode::gen() {
  BasicNode::gen();
  theMacroAssm->jcc(mapToCC(_op), next(1)->label);
  theMacroAssm->jmp(next()->label);	// this jump will be eliminated since this is the likely successor
  bb_needs_jump = false;		// no jump necessary at end of basic block
}
*/


void ContextCreateNode::gen() {
  BasicNode::gen();

  switch (_contextSize) {
    case 0:  primitiveCall(scope(), primitives::context_allocate0()); break;
    case 1:  primitiveCall(scope(), primitives::context_allocate1()); break;
    case 2:  primitiveCall(scope(), primitives::context_allocate2()); break;
    default: assert(_pdesc == primitives::context_allocate(), "bad context create prim");
	     theMacroAssm->pushl((int)as_smiOop(_contextSize));
	     primitiveCall(scope(), _pdesc);
	     theMacroAssm->addl(esp, oopSize);	// pop argument, this is not a Pascal call - should fix this
  }
  Register context = Mapping::asRegister(resultLoc);
  if (_src == NULL) {
    assert(scope()->isMethodScope() || scope()->method()->block_info() == methodOopDesc::expects_nil, "inconsistency");
    theMacroAssm->movl(Address(context, contextOopDesc::parent_byte_offset()), NULL);
    // NULL for now; the interpreter uses nil. However, some of the
    // context verification code called from compiled code checks for
    // parents that are either a frame pointer, NULL or a context.
    // This should be unified at some point. (gri 5/9/96)
  } else {
    // parent is in _src (incoming)
    Register parent = movePRegToReg(_src, temp1);
    theMacroAssm->movl(Address(context, contextOopDesc::parent_byte_offset()), parent);
  }
  store(context, _dest, temp2, temp3);
  theMacroAssm->store_check(context, temp2);
}


void ContextInitNode::gen() {
  BasicNode::gen();
  // initialize context fields
  for (int i = nofTemps() - 1; i >= 0; i--) {
    PReg* src = _initializers->at(i)->preg();
    PReg* dest;
    if (src->isBlockPReg()) {
      // Blocks aren't actually assigned (at the PReg level) so that the inlining info isn't lost.
      if (wasEliminated()) {
        continue;				// there's no assignment (context was eliminated)
      } else {
	dest = contents()->at(i)->preg();	// fake destination created by compiler
      }
    } else {
      dest = contents()->at(i)->preg();
    }
    assign(this, src, dest, temp1, temp2, temp3, false);
  }
  // NB: no store check necessary (done in ContextCreateNode)
  // init node must follow create immediately (since fields are uninitialized)
}


void ContextZapNode::gen() {
  // Only generated for new backend yet
  ShouldNotReachHere();
}


void FixedCodeNode::gen() {
  BasicNode::gen();
  switch(_kind) {
    case dead_end:    theMacroAssm->hlt(); break;
    case inc_counter: incCounter(); break;
    default:	      fatal1("unexpected FixedCodeNode kind %d", _kind);
  }
}


void InlinedReturnNode::gen() {
  BasicNode::gen();
  assign(this, _src, _dest, temp1, temp2, temp3);
  if (scope()->needsContextZapping()) {
    zapContext(scope()->context(), temp1);
  }
}


void ReturnNode::gen() {
  BasicNode::gen();
  assert(_src->loc == resultLoc, "result in wrong location");
  if (scope()->needsContextZapping()) {
    zapContext(scope()->context(), temp1);
  }
  // remove stack frame
  if (VerifyCode || GenTraceCalls) verifyReturnCode(Mapping::asRegister(resultLoc));
  if (TraceResults) call_trace_result(result_reg);
  int no_of_args_to_pop = scope()->nofArguments();
  if (scope()->method()->is_blockMethod()) {
    // blocks are called via primitiveValue => need to pop first argument
    // of primitiveValue (= block closure) as well since return happens
    // directly (and not through primitiveValue).
    no_of_args_to_pop++;
  }
  theMacroAssm->leave();
  theMacroAssm->ret(no_of_args_to_pop * oopSize);
}


void NLRSetupNode::gen() {
  BasicNode::gen();
  assert(_src->loc == NLRResultLoc, "result in wrong location");
  // get, test if not zapped & assign home
  Label NLR_error;
  Register home = uplevelBase(scope()->context(), scope()->homeContext() + 1, temp1);
  theMacroAssm->testl(home, home);
  theMacroAssm->jcc(Assembler::zero, NLR_error); // zero -> home has been zapped
  if (home != Mapping::asRegister(NLRHomeLoc)) {
    theMacroAssm->movl(Mapping::asRegister(NLRHomeLoc), home);
  }
  // assign home id
  theMacroAssm->movl(NLR_homeId_reg, scope()->home()->scopeID());
  if (TraceResults) call_trace_result(NLR_result_reg);
  continueNLR(temp1, temp2);
  // call run-time routine in failure case
  theMacroAssm->bind(NLR_error);
  call_C((char*)suspend_on_NLR_error, relocInfo::runtime_call_type, true);
  theMacroAssm->hlt();
}


void NLRContinuationNode::gen() {
  BasicNode::gen();
  if (scope()->needsContextZapping()) {
    zapContext(scope()->context(), temp1);
  }
  continueNLR(temp1, temp2);
}


void NLRTestNode::gen() {
  BasicNode::gen();
  //theMacroAssm->bind(scope()->_nlrTestPointLabel);
  // arrived at the right frame?
  Label L;
  theMacroAssm->cmpl(Mapping::asRegister(NLRHomeLoc), ebp);
  theMacroAssm->jcc(Assembler::notEqual, L);
  // arrived at the right scope within a frame?
  int id = scope()->scopeID();
  if (id == 0) {
    // use x86 test to compare with 0 (smaller code than with cmp)
    theMacroAssm->testl(Mapping::asRegister(NLRHomeIdLoc), Mapping::asRegister(NLRHomeIdLoc));
  } else {
    theMacroAssm->cmpl(Mapping::asRegister(NLRHomeIdLoc), id);
  }
  theMacroAssm->jcc(Assembler::equal, next1()->label);
  // continue non-local return
  theMacroAssm->bind(L);
}


void InterruptCheckNode::gen() {
  BasicNode::gen();
  Unimplemented();
}


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


static bool testForBoolKlasses(Register obj, klassOop klass1, klassOop klass2, Register klassReg, 
			       bool hasUnknown, Label& success1, Label& success2, Label& failure) {
  oop bool1  = Universe::trueObj();
  oop bool2  = Universe::falseObj();
  if (klass1 == bool2->klass() && klass2 == bool1->klass()) {
    oop t = bool1; bool1 = bool2; bool2 = t;
  }
  if (klass1 == bool1->klass() && klass2 == bool2->klass()) {
    if (hasUnknown) {
      theMacroAssm->cmpl(obj, bool1);
      theMacroAssm->jcc(Assembler::equal, success1);
      theMacroAssm->cmpl(obj, bool2);
      theMacroAssm->jcc(Assembler::notEqual, failure);
      theMacroAssm->jmp(success2);	// this jump will be eliminated since this is the likely successor
    } else {
      theMacroAssm->cmpl(obj, bool2);
      theMacroAssm->jcc(Assembler::equal, success2);
      theMacroAssm->jmp(success1);	// this jump will be eliminated since this is the likely successor
    }
    return true;
  }
  return false;
}


static void generalTypeTest(Register obj, Register klassReg, bool hasUnknown,
			    GrowableArray<klassOop>* classes, GrowableArray<Label*>* next) {
  // handle general case: N klasses, N+1 labels (first label = unknown case)
  int smi_case = -1;			// index of smi case in next array (if there)
  const int len = classes->length();
  GrowableArray<klassOop> klasses(len);	// list of classes excluding smi case
  GrowableArray<Label*>    labels(len);	// list of nodes   excluding smi case

  // compute klasses & nodes list without smi case
  for (int i = 0; i < len; i++) {
    const klassOop klass = classes->at(i);
    if (klass == Universe::smiKlassObj()) {
      smi_case = i + 1;
    } else {
      klasses.append(klass);
      labels.append(next->at(i+1));
    }
  }

  if (smi_case == -1 && hasUnknown) {
    // smi case is also unknown case
    smi_case = 0;
  }

  // generate code
  if (smi_case >= 0) {
    theMacroAssm->test(obj, Mem_Tag);
    theMacroAssm->jcc(Assembler::zero, *next->at(smi_case));
  }

  bool klassHasBeenLoaded = false;
  const int nof_cmps = hasUnknown ? klasses.length() : klasses.length() - 1;
  for (i = 0; i < nof_cmps; i++) {
    const klassOop klass = klasses.at(i);
    if (klass == Universe::trueObj()->klass()) {
      // only one instance: compare with trueObj
      theMacroAssm->cmpl(obj, Universe::trueObj());
    } else if (klass == Universe::falseObj()->klass()) {
      // only one instance: compare with falseObj
      theMacroAssm->cmpl(obj, Universe::falseObj());
    } else if (klass == Universe::nilObj()->klass()) {
      // only one instance: compare with nilObj
      theMacroAssm->cmpl(obj, Universe::nilObj());
    } else {
      // compare with class
      assert(klass != Universe::smiKlassObj(), "should have been excluded");
      if (!klassHasBeenLoaded) {
        theMacroAssm->movl(klassReg, Address(obj, memOopDesc::klass_byte_offset()));
	klassHasBeenLoaded = true;
      }
      theMacroAssm->cmpl(klassReg, klass);
    }
    theMacroAssm->jcc(Assembler::equal, *labels.at(i));
  }
  if (hasUnknown) {
    theMacroAssm->jmp(*(next->first()));
  } else {
    // must be last case, no test required
    theMacroAssm->jmp(*labels.at(i));
  }
}


void TypeTestNode::gen() {
  BasicNode::gen();
  const int len = classes()->length();
  const Register obj = movePRegToReg(_src, temp1);
  const Register klassReg = temp2;
  bb_needs_jump = false;  // we generate all jumps explicitly

  if (ReorderBBs) {
    // generate better code for some of the frequent cases

    if (len == 1) {
      // handle all cases where only one klass is involved
      assert(hasUnknown(), "should be eliminated if there's no unknown case");
      assert(likelySuccessor() == next(1), "code pattern is not optimal");
      klassOop klass = classes()->at(0);
      testForSingleKlass(obj, klass, klassReg, next(1)->label, next()->label);
      return;
    }

    if (len == 2) {
      // handle pure boolean cases (ifTrue:/ifFalse:)
      klassOop klass1 = classes()->at(0);
      klassOop klass2 = classes()->at(1);
      if (testForBoolKlasses(obj, klass1, klass2, klassReg, hasUnknown(), next(1)->label, next(2)->label, next()->label)) {
        return;
      }
    }
  }

  // handle general case
  GrowableArray<Label*> labels(len + 1);
  for (int i = 0; i <= len; i++) labels.append(&next(i)->label);
  generalTypeTest(obj, klassReg, hasUnknown(), classes(), &labels);
}

/* old code
void TypeTestNode::gen() {
  BasicNode::gen();
  const int len = classes()->length();
  const Register obj = movePRegToReg(_src, temp1);
  const Register klassReg = temp2;

  if (ReorderBBs) {
    // generate better code for some of the frequent cases

    if (len == 1) {
      // handle all cases where only one klass is involved
      assert(hasUnknown(), "should be eliminated if there's no unknown case");
      assert(likelySuccessor() == next(1), "code pattern is not optimal");
      klassOop klass = classes()->at(0);
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
	theMacroAssm->jcc(Assembler::zero, next()->label);
        theMacroAssm->movl(klassReg, Address(obj, memOopDesc::klass_byte_offset()));
        theMacroAssm->cmpl(klassReg, klass);
      }
      theMacroAssm->jcc(Assembler::notEqual, next()->label);
      theMacroAssm->jmp(next(1)->label);	// this jump will be eliminated since this is the likely successor
      bb_needs_jump = false;			// no jump necessary at end of basic block
      return;
    }

    if (len == 2) {
      // handle pure boolean cases (ifTrue:/ifFalse:)
      klassOop klass1 = classes()->at(0);
      klassOop klass2 = classes()->at(1);
      oop      bool1  = Universe::trueObj();
      oop      bool2  = Universe::falseObj();
      if (klass1 == bool2->klass() && klass2 == bool1->klass()) {
        oop t = bool1; bool1 = bool2; bool2 = t;
      }
      if (klass1 == bool1->klass() && klass2 == bool2->klass()) {
	if (hasUnknown()) {
	  assert(likelySuccessor() == next(2), "code pattern is not optimal");
	  theMacroAssm->cmpl(obj, bool1);
	  theMacroAssm->jcc(Assembler::equal, next(1)->label);
          theMacroAssm->cmpl(obj, bool2);
          theMacroAssm->jcc(Assembler::notEqual, next()->label);
	  theMacroAssm->jmp(next(2)->label);	// this jump will be eliminated since this is the likely successor
	} else {
	  assert(likelySuccessor() == next(1), "code pattern is not optimal");
          theMacroAssm->cmpl(obj, bool2);
	  theMacroAssm->jcc(Assembler::equal, next(2)->label);
	  theMacroAssm->jmp(next(1)->label);	// this jump will be eliminated since this is the likely successor
	}
	bb_needs_jump = false;			// no jump necessary at end of basic block
        return;
      }
    } 

  // handle general case
  Node* smi_case = NULL;		// smi case if there
  GrowableArray<klassOop> klasses(len);	// list of classes excluding smi case
  GrowableArray<Node*>    nodes(len);	// list of nodes   excluding smi case

  // compute klasses & nodes list without smi case
  for (int i = 0; i < len; i++) {
    const klassOop klass = classes()->at(i);
    if (klass == Universe::smiKlassObj()) {
      smi_case = next(i+1);
    } else {
      klasses.append(klass);
      nodes.append(next(i+1));
    }
  }

  if (smi_case == NULL && hasUnknown()) {
    // smi case is also unknown case
    smi_case = next();
  }

  // generate code
  if (smi_case != NULL) {
    theMacroAssm->test(obj, Mem_Tag);
    theMacroAssm->jcc(Assembler::zero, smi_case->label);
  }

  bool klassHasBeenLoaded = false;
  const int nof_cmps = hasUnknown() ? klasses.length() : klasses.length() - 1;
  for (i = 0; i < nof_cmps; i++) {
    const klassOop klass = klasses.at(i);
    if (klass == Universe::trueObj()->klass()) {
      // only one instance: compare with trueObj
      theMacroAssm->cmpl(obj, Universe::trueObj());
    } else if (klass == Universe::falseObj()->klass()) {
      // only one instance: compare with falseObj
      theMacroAssm->cmpl(obj, Universe::falseObj());
    } else if (klass == Universe::nilObj()->klass()) {
      // only one instance: compare with nilObj
      theMacroAssm->cmpl(obj, Universe::nilObj());
    } else {
      // compare with class
      assert(klass != Universe::smiKlassObj(), "should have been excluded");
      if (!klassHasBeenLoaded) {
        theMacroAssm->movl(klassReg, Address(obj, memOopDesc::klass_byte_offset()));
	klassHasBeenLoaded = true;
      }
      theMacroAssm->cmpl(klassReg, klass);
    }
    theMacroAssm->jcc(Assembler::equal, nodes.at(i)->label);
  }
  if (hasUnknown()) {
    theMacroAssm->jmp(next()->label);
  } else {
    // must be last case, no test required
    theMacroAssm->jmp(nodes.at(i)->label);
  }
  bb_needs_jump = false;
}
*/


void UncommonNode::gen() {
  BasicNode::gen();
  theMacroAssm->call(StubRoutines::unused_uncommon_trap_entry(), relocInfo::uncommon_type);
}


void BlockCreateNode::copyIntoContexts(Register val, Register t1, Register t2) {
  // Copy newly created block (in val) into all contexts that have a copy;
  // registers t1 and t2 can be used as scratch registers.
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
  BlockPReg* blk = block();
  GrowableArray<Location*>* copies = blk->contextCopies();
  if (copies == NULL) return;
  for (int i = copies->length() - 1; i >= 0; i--) {
    Location* l = copies->at(i);
    InlinedScope* scopeWithContext = theCompiler->scopes->at(l->scopeID());
    PReg* r = scopeWithContext->contextTemporaries()->at(l->tempNo())->preg();
    if (r->loc == unAllocated) continue;	  // not uplevel-accessed (eliminated)
    if (r->isBlockPReg()) continue;		  // ditto
    if (!r->loc.isContextLocation()) fatal("expected context location");
    if (scopeWithContext->isSenderOrSame(_scope)) {
      store(val, r, t1, t2);
    }
  }
}


void BlockCreateNode::materialize() {
  CompileTimeClosure* closure = block()->closure();
  if (closure->context()->loc.isRegisterLocation()) {
    // should not be allocated to a register, since the register will
    // be destroyed after the call - push it on the stack as temporary
    // fix - take this out after register allocation has been fixed
    theMacroAssm->pushl(Mapping::asRegister(closure->context()->loc));
  }
  int nofArgs = closure->nofArgs();
  switch (nofArgs) {
    case 0:   primitiveCall(scope(), primitives::block_allocate0()); break;
    case 1:   primitiveCall(scope(), primitives::block_allocate1()); break;
    case 2:   primitiveCall(scope(), primitives::block_allocate2()); break;
    default:  assert(_pdesc == primitives::block_allocate(), "bad block clone prim");
	      theMacroAssm->pushl((int)as_smiOop(nofArgs));
	      primitiveCall(scope(), _pdesc);
	      theMacroAssm->addl(esp, oopSize); // pop argument, this is not a Pascal call - should fix this
  }
  // assign result
  Register t = moveLocToReg(resultLoc, answerPRegReg(_dest, temp1));
  store(t, _dest, temp2, temp3);
  // copy into all contexts that have a copy
  if (block()->isMemoized()) copyIntoContexts(t, temp2, temp3);
  // initialize block closure fields
  Register closureReg = Mapping::asRegister(resultLoc); // fix this: should refer to _dest->loc
  Register contextReg;
  if (closure->context()->loc.isRegisterLocation()) {
    // context value is on the stack -- see also comment above
    // take this out after register allocation has been fixed
    contextReg = temp1;
    theMacroAssm->popl(contextReg);
  } else {
    contextReg = movePRegToReg(closure->context(), temp1);
  }
  if (VerifyCode) {
    switch (closure->method()->block_info()) {
      case methodOopDesc::expects_nil      : verifyNilOrContextCode(contextReg); break;
      case methodOopDesc::expects_self     : // fall through
      case methodOopDesc::expects_parameter: verifyOopCode(contextReg); break;
      case methodOopDesc::expects_context  : verifyContextCode(contextReg); break;
      default: ShouldNotReachHere();
    }
  }
  theMacroAssm->Store(contextReg, closureReg, blockClosureOopDesc::context_byte_offset());
  // assert(theCompiler->jumpTableID == closure->parent_id(), "nmethod id must be the same");
  // fix this: RELOCATION INFORMATION IS NEEDED WHEN MOVING THE JUMPTABLE (Snapshot reading etc.)
  theMacroAssm->movl(Address(closureReg, blockClosureOopDesc::method_or_entry_byte_offset()), int(closure->jump_table_entry()));
  if (VerifyCode) verifyBlockCode(closureReg);
  theMacroAssm->store_check(closureReg, temp1);
}


void BlockCreateNode::gen() {
  BasicNode::gen();
  if (block()->closure()->method()->is_clean_block()) {
    // create the block now (doesn't need to be copied at run-time)
    CompileTimeClosure* closure = block()->closure();
    blockClosureOop blk = blockClosureOopDesc::create_clean_block(closure->nofArgs(), closure->jump_table_entry());
    Mapping::storeO(blk, _dest->loc, temp1, temp3, true);
  } else if (block()->isMemoized()) {
    // initialize block variable
    Mapping::storeO(MemoizedBlockNameDesc::uncreatedBlockValue(), _dest->loc, temp1, temp3, true);
  } else {
    // actually create block
    materialize();
  }
}


void BlockMaterializeNode::gen() {
  BasicNode::gen();
  if (block()->isMemoized() && !block()->closure()->method()->is_clean_block()) {
    // materialize block if it is not already materialized
    // (nothing to do in case of non-memoized or clean blocks)
    Label L;
    Register t = movePRegToReg(block(), temp1);
    assert(MemoizedBlockNameDesc::uncreatedBlockValue() == oop(0), "change the code generation here");
    theMacroAssm->testl(t, t);
    theMacroAssm->jcc(Assembler::notZero, L);
    materialize();
    theMacroAssm->bind(L);
  }
}

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
  // above 2 lines could be eliminated with if (ok.is_unbound()) ok.redirectTo(next()->label)
  bb_needs_jump = false;  // we generate all jumps explicitly
  theMacroAssm->bind(failure);
  theMacroAssm->call(StubRoutines::unused_uncommon_trap_entry(), relocInfo::uncommon_type);
}


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

void LoopHeaderNode::generateIntegerLoopTests(Label& prev, Label& failure) {
  if (!_integerLoop) return;
  generateIntegerLoopTest(_lowerBound, prev, failure);
  generateIntegerLoopTest(_upperBound, prev, failure);
  generateIntegerLoopTest(_loopVar   , prev, failure);
}

void LoopHeaderNode::handleConstantTypeTest(ConstPReg* r, GrowableArray<klassOop>* klasses) {
  // constant r is tested against klasses (efficiency hack: klasses == NULL means {smi})
  if ((klasses == NULL && r->constant->is_smi()) || (klasses && klasses->contains(r->constant->klass()))) {
    // always ok, no need to test
  } else {
    compiler_warning("loop header type test will always fail!");
    // don't jump to failure because that would make subsequent LoopHeader code unreachable (--> breaks back end)
    theMacroAssm->call(StubRoutines::unused_uncommon_trap_entry(), relocInfo::uncommon_type);
  }
}

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


static void jcc_error(Node* node, Assembler::Condition cond, Label& label) {
// Used in code pattern generators that also generate code to setup error messages.
// If an uncommon trap is issued in the error situation anyway, the error message
// setup code is not needed and we can jump to the uncommon node directly => saves
// code & a jump in the commom case.
  Node* failure_start = node->next(1);
  if (failure_start->isUncommonNode()) {
    // error handling causes uncommon trap anyway, jump to uncommon node directly
    theMacroAssm->jcc(cond, failure_start->label);
  } else {
    // failure case is not uncommon, jump to label
    theMacroAssm->jcc(cond, label);
  }
}


void ArrayAtNode::gen() {
  BasicNode::gen();
  // load registers in an order that reduces load delays
  Register size  = temp3;
  Register index = temp2; load(_arg, index);	// index is modified -> load always into register
  Register array = movePRegToReg(_src, temp1);	// array is read_only
  // first element is at index 1 => subtract smi(1) (doesn't change smi/oop property)
  theMacroAssm->subl(index, int(smiOop_one));
  // preload size for bounds check if necessary
  if (_needBoundsCheck) {
    theMacroAssm->movl(size, Address(array, byteOffset(_sizeOffset)));
  }
  // do index smi check if necessary (still possible, even after subtracting smi(1))
  Label indexNotSmi;
  if (!_intArg) {
    theMacroAssm->test(index, Mem_Tag);
    jcc_error(this, Assembler::notZero, indexNotSmi);
  }
  // do bounds check if necessary
  Label indexOutOfBounds;
  if (_needBoundsCheck) {
    theMacroAssm->cmpl(index, size);
    jcc_error(this, Assembler::aboveEqual, indexOutOfBounds);
  }
  // load element
  Register t = answerPRegReg(_dest, temp3);
  assert(Tag_Size == 2, "check this code");
  switch (_access_type) {
    case byte_at:
      theMacroAssm->sarl(index, Tag_Size);	// adjust index
      theMacroAssm->xorl(t, t);			// clear destination register
      theMacroAssm->movb(t, Address(array, index, Address::times_1, byteOffset(_dataOffset)));
      theMacroAssm->shll(t, Tag_Size);		// make result a smi
      break;
    case double_byte_at:
      theMacroAssm->sarl(index, Tag_Size - 1);	// adjust index
      theMacroAssm->movl(t, Address(array, index, Address::times_1, byteOffset(_dataOffset)));
      theMacroAssm->andl(t, 0x0000FFFF);	// clear upper 2 bytes
      theMacroAssm->shll(t, Tag_Size);		// make result a smi
      break;
    case character_at:
      { theMacroAssm->sarl(index, Tag_Size - 1);// adjust index
        theMacroAssm->movl(t, Address(array, index, Address::times_1, byteOffset(_dataOffset)));
        theMacroAssm->andl(t, 0x0000FFFF);	// clear upper 2 bytes
        // use t as index into asciiCharacters()
        // check index first, must be 0 <= t < asciiCharacters()->length()
        objArrayOop chars = Universe::asciiCharacters();
        theMacroAssm->cmpl(t, chars->length());
        jcc_error(this, Assembler::aboveEqual, indexOutOfBounds);
	// get character out of chars array
        theMacroAssm->movl(temp1, chars);
        theMacroAssm->movl(t, Address(temp1, t, Address::times_4, byteOffset(chars->klass()->klass_part()->non_indexable_size() + 1)));
      }
      break;
    case object_at:
      // smi index is already shifted the right way => no index adjustment necessary
      theMacroAssm->movl(t, Address(array, index, Address::times_1, byteOffset(_dataOffset)));
      break;
    default:
      ShouldNotReachHere();
      break;
  }
  assert(t != temp1 && t != temp2, "just checking");
  store(t, _dest, temp1, temp2);
  // handle error cases if not uncommon
  if (canFail() && !next(1)->isUncommonNode()) {
    Label exit;
    theMacroAssm->jmp(exit);
    // error messages
    if (!_intArg) {
      theMacroAssm->bind(indexNotSmi);
      theMacroAssm->movl(temp1, vmSymbols::first_argument_has_wrong_type());
      store(temp1, _error, temp2, temp3);
      theMacroAssm->jmp(next(1)->label);
    }
    if (_needBoundsCheck) {
      theMacroAssm->bind(indexOutOfBounds);
      theMacroAssm->movl(temp1, vmSymbols::out_of_bounds());
      store(temp1, _error, temp2, temp3);
      theMacroAssm->jmp(next(1)->label);
    }
    theMacroAssm->bind(exit);
  }
}


void ArrayAtPutNode::gen() {
  BasicNode::gen();
  // load registers in an order that reduces load delays
  Register size  = temp3;
  Register index = temp2; load(_arg, index);	// index is modified -> load always into register
  Register array = temp1; load(_src, array);	// array may be modified -> load always into register
  // first element is at index 1 => subtract smi(1) (doesn't change smi/oop property)
  theMacroAssm->subl(index, int(smiOop_one));
  // preload size for bounds check if necessary
  if (_needBoundsCheck) {
    theMacroAssm->movl(size, Address(array, byteOffset(_sizeOffset)));
  }
  // do index smi check if necessary (still possible, even after subtracting smi(1))
  Label indexNotSmi;
  if (!_intArg) {
    theMacroAssm->test(index, Mem_Tag);
    jcc_error(this, Assembler::notZero, indexNotSmi);
  }
  // do bounds check if necessary
  Label indexOutOfBounds;
  if (_needBoundsCheck) {
    theMacroAssm->cmpl(index, size);
    jcc_error(this, Assembler::aboveEqual, indexOutOfBounds);
  }
  // store element
  Label elementNotSmi, elementOutOfRange;
  Register element = temp3; load(elem, element);// element may be modified -> load always into register
  assert(Tag_Size == 2, "check this code");
  switch (_access_type) {
    case byte_at_put:
      if (!_smi_element) {
        theMacroAssm->test(element, Mem_Tag);
        jcc_error(this, Assembler::notZero, elementNotSmi);
      }
      theMacroAssm->sarl(element, Tag_Size);	// convert element into (int) byte
      if (_needs_element_range_check) {
        theMacroAssm->cmpl(element, 0x100);
	jcc_error(this, Assembler::aboveEqual, elementOutOfRange);
      }
      theMacroAssm->sarl(index, Tag_Size);	// adjust index
      theMacroAssm->movb(Address(array, index, Address::times_1, byteOffset(_dataOffset)), element);
      assert(!_needs_store_check, "just checking");
      break;
    case double_byte_at_put:
      if (!_smi_element) {
        theMacroAssm->test(element, Mem_Tag);
        jcc_error(this, Assembler::notZero, elementNotSmi);
      }
      theMacroAssm->sarl(element, Tag_Size);	// convert element into (int) double byte
      if (_needs_element_range_check) {
        theMacroAssm->cmpl(element, 0x10000);
	jcc_error(this, Assembler::aboveEqual, elementOutOfRange);
      }
      theMacroAssm->sarl(index, Tag_Size - 1);	// adjust index
      theMacroAssm->leal(array, Address(array, index, Address::times_1, byteOffset(_dataOffset)));
      assert(temp2 != array && temp2 != element, "check this code");
      theMacroAssm->movl(temp2, element);	// copy element (since element might be used afterwards)
      theMacroAssm->shrl(temp2, 8);		// shift 2nd byte into low-byte position
      theMacroAssm->movb(Address(array, 0), element);
      theMacroAssm->movb(Address(array, 1), temp2);
      assert(!_needs_store_check, "just checking");
      // Note: could use a better code sequence without introducing the extra movl & shrl
      //       instruction here - however, currently the assembler doesn't support addressing
      //       of the the 2nd byte in a register (otherwise two movb instructions would do).
      break;
    case object_at_put:
      // smi index is already shifted the right way => no index adjustment necessary
      if (_needs_store_check) {
        theMacroAssm->leal(array, Address(array, index, Address::times_1, byteOffset(_dataOffset)));
        theMacroAssm->movl(Address(array), element);
        theMacroAssm->store_check(array, temp3);
      } else {
        theMacroAssm->movl(Address(array, index, Address::times_1, byteOffset(_dataOffset)), element);
      }
      break;
    default:
      ShouldNotReachHere();
      break;
  }
  // handle error cases if not uncommon
  if (canFail() && !next(1)->isUncommonNode()) {
    Label exit;
    theMacroAssm->jmp(exit);
    // error messages
    if (!_intArg) {
      theMacroAssm->bind(indexNotSmi);
      theMacroAssm->movl(temp1, vmSymbols::first_argument_has_wrong_type());
      store(temp1, _error, temp2, temp3);
      theMacroAssm->jmp(next(1)->label);
    }
    if (_needBoundsCheck) {
      theMacroAssm->bind(indexOutOfBounds);
      theMacroAssm->movl(temp1, vmSymbols::out_of_bounds());
      store(temp1, _error, temp2, temp3);
      theMacroAssm->jmp(next(1)->label);
    }
    if (!_smi_element) {
      theMacroAssm->bind(elementNotSmi);
      theMacroAssm->movl(temp1, vmSymbols::second_argument_has_wrong_type());
      store(temp1, _error, temp2, temp3);
      theMacroAssm->jmp(next(1)->label);
    }
    if (_needs_element_range_check) {
      theMacroAssm->bind(elementOutOfRange);
      theMacroAssm->movl(temp1, vmSymbols::value_out_of_range());
      store(temp1, _error, temp2, temp3);
      theMacroAssm->jmp(next(1)->label);
    }
    theMacroAssm->bind(exit);
  }
}


void InlinedPrimitiveNode::gen() {
  BasicNode::gen();
  switch (_op) {
    case InlinedPrimitiveNode::obj_klass:
      { Register obj   = movePRegToReg(_src, temp1);			// obj is read_only
        Register klass = temp2;
	Label is_smi;
	theMacroAssm->movl(klass, Universe::smiKlassObj());
	theMacroAssm->test(obj, Mem_Tag);
	theMacroAssm->jcc(Assembler::zero, is_smi);
	theMacroAssm->movl(klass, Address(obj, memOopDesc::klass_byte_offset()));
	theMacroAssm->bind(is_smi);
        store(klass, _dest, temp1, temp3);
      }
      break;
    case InlinedPrimitiveNode::obj_hash:
      { Unimplemented();
        // Implemented for the smi klass only by now - can be resolved in
	// the PrimInliner for that case without using an InlinedPrimitiveNode.
      };
      break;
    case InlinedPrimitiveNode::proxy_byte_at:
      { Register proxy  = temp1; load(_src,  proxy);			// proxy is modified
        Register index  = temp2; load(_arg1, index);			// index is modified
	Register result = answerPRegReg(_dest, temp3);
	Label indexNotSmi;
	// do index smi check if necessary
	if (!_arg1_is_smi) {
	  theMacroAssm->test(index, Mem_Tag);
	  jcc_error(this, Assembler::notZero, indexNotSmi);
	}
	// load element
	theMacroAssm->movl(proxy, Address(proxy, pointer_offset));	// unbox proxy
	theMacroAssm->sarl(index, Tag_Size);				// adjust index
	theMacroAssm->xorl(result, result);				// clear destination register
	theMacroAssm->movb(result, Address(proxy, index, Address::times_1, 0));
	theMacroAssm->shll(result, Tag_Size);				// make result a smi
	// continue
	assert(result != temp1 && result != temp2, "just checking");
        store(result, _dest, temp1, temp2);
	// handle error cases if not uncommon
        if (canFail() && !next(1)->isUncommonNode()) {
	  Label exit;
          theMacroAssm->jmp(exit);
          // error messages
          if (!_arg1_is_smi) {
	    theMacroAssm->bind(indexNotSmi);
            theMacroAssm->movl(temp1, vmSymbols::first_argument_has_wrong_type());
            store(temp1, _error, temp2, temp3);
            theMacroAssm->jmp(next(1)->label);
          }
          theMacroAssm->bind(exit);
	}
      }
      break;
    case InlinedPrimitiveNode::proxy_byte_at_put:
      { bool const_val = _arg2->isConstPReg();
        Register proxy = temp1; load(_src,  proxy);			// proxy is modified
        Register index = temp2; load(_arg1, index);			// index is modified
	Register value; 
	if (const_val) {
	  // value doesn't have to be loaded -> do nothing here
	  if (!_arg2_is_smi) fatal("proxy_byte_at_put: should not happen - internal error");
	  //if (!_arg2_is_smi) fatal("proxy_byte_at_put: should not happen - tell Robert");
	} else {
	  value = temp3; load(_arg2, value);				// value is modified
	}
	Label indexNotSmi, valueNotSmi;
	// do index smi check if necessary
	if (!_arg1_is_smi) {
	  theMacroAssm->test(index, Mem_Tag);
	  jcc_error(this, Assembler::notZero, indexNotSmi);
	}
	// do value smi check if necessary
	if (!_arg2_is_smi) {
	  assert(!const_val, "constant shouldn't need a smi check");
	  theMacroAssm->test(value, Mem_Tag);
	  jcc_error(this, Assembler::notZero, valueNotSmi);
	}
        // store element
        theMacroAssm->movl(proxy, Address(proxy, pointer_offset));	// unbox proxy
	theMacroAssm->sarl(index, Tag_Size);				// adjust index
	if (const_val) {
	  smiOop constant = smiOop(((ConstPReg*)_arg2)->constant);
	  assert(constant->is_smi(), "should be a smi");
          theMacroAssm->movb(Address(proxy, index, Address::times_1, 0), constant->value() & 0xFF);
	} else {
	  theMacroAssm->sarl(value, Tag_Size);				// adjust value
	  theMacroAssm->movb(Address(proxy, index, Address::times_1, 0), value);
	}
        // handle error cases if not uncommon
        if (canFail() && !next(1)->isUncommonNode()) {
	  Label exit;
          theMacroAssm->jmp(exit);
          // error messages
          if (!_arg1_is_smi) {
            theMacroAssm->bind(indexNotSmi);
	    theMacroAssm->movl(temp1, vmSymbols::first_argument_has_wrong_type());
            store(temp1, _error, temp2, temp3);
            theMacroAssm->jmp(next(1)->label);
          }
          if (!_arg2_is_smi) {
	    theMacroAssm->bind(valueNotSmi);
            theMacroAssm->movl(temp1, vmSymbols::second_argument_has_wrong_type());
            store(temp1, _error, temp2, temp3);
            theMacroAssm->jmp(next(1)->label);
          }
          theMacroAssm->bind(exit);
	}
      };
      break;
    default: ShouldNotReachHere();
  }
}


#endif
