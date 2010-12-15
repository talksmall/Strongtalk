/* Copyright 1994 - 1996, LongView Technologies, L.L.C. $Revision: 1.107 $ */
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
# include "incls/_interpreter.cpp.incl"


// Interpreter stack frame
//
// An interpreter stack frame always provides space for at least one temporary (temp_0).
// This speeds up method activation for methods with less than 2 temporaries. More
// temporaries are allocated via special Bytecodes. Furthermore: Since there is always
// (at least) one temporary, which in case of context extensions holds the pointer to
// the heap-allocated context, non-local returns become simpler, because they can assume
// at least temp_0 to be there (if there is a chance of having no temporaries and a self
// send without args, the temp0 location would contain the return address of the callee
// (a non-oop), if there were no temp_0 always).

static const int float_0_offset = oopSize * (frame_temp_offset - 3);
static const int temp_1_offset	= oopSize * (frame_temp_offset - 1);
static const int temp_0_offset	= oopSize * frame_temp_offset;
static const int esi_offset	= oopSize * frame_hp_offset;
static const int self_offset	= oopSize * frame_receiver_offset;
static const int link_offset	= oopSize * frame_link_offset;
static const int ret_addr_offset= oopSize * frame_return_addr_offset;
static const int arg_n_offset	= oopSize * (frame_arg_offset - 1);

static const int max_nof_temps	= 256;
static const int max_nof_floats	= 256;


// Interpreter boundaries
bool  Interpreter::_is_initialized  = false;
char* Interpreter::_code_begin_addr = NULL;
char* Interpreter::_code_end_addr   = NULL;

#define TEST_GENERATION_

#ifdef TEST_GENERATION_
extern "C" int interpreter_loop_counter = 0;
extern "C" int interpreter_loop_counter_limit = 0;

bool Interpreter::contains(char* pc) {
  return
    (_code_begin_addr       <= pc && pc < _code_end_addr) || 
    (pc == StubRoutines::single_step_continuation());
}

extern "C" char* InterpreterCodeStatus() { return "\x01\x00\x00\x00\x00\x00"; };
//extern "C" char* InterpreterCodeStatus() { return "\x00\x01\x01\x01\x01\x01"; };

#else

extern "C" int interpreter_loop_counter;		// see interpreter_asm.asm
extern "C" int interpreter_loop_counter_limit;	  	// see interpreter_asm.asm

// Boundaries of assembled interpreter code
extern "C" char* interpreter_begin_addr;
extern "C" char* interpreter_end_addr;

bool Interpreter::contains(char* pc) {
  return
    (interpreter_begin_addr <= pc && pc < interpreter_end_addr) ||
    (_code_begin_addr       <= pc && pc < _code_end_addr);
}

extern "C" char* InterpreterCodeStatus();

#endif // TEST_GENERATION

bool Interpreter::is_optimized()        { return InterpreterCodeStatus()[0] == 1; }
bool Interpreter::can_trace_bytecodes() { return InterpreterCodeStatus()[1] == 1; }
bool Interpreter::can_trace_sends()     { return InterpreterCodeStatus()[2] == 1; }
bool Interpreter::has_assertions()      { return InterpreterCodeStatus()[3] == 1; }
bool Interpreter::has_stack_checks()    { return InterpreterCodeStatus()[4] == 1; }
bool Interpreter::has_timers()          { return InterpreterCodeStatus()[5] == 1; }

void Interpreter::print_code_status() {
  if (is_optimized()) {
    std->print_cr("- Interpreter code is optimized");
  } else {
    std->print("- Interpreter code is in debug mode: ");
    if (can_trace_bytecodes()) std->print("trace_bytecodes ");
    if (can_trace_sends())     std->print("trace_sends ");
    if (has_assertions())      std->print("assertions ");
    if (has_stack_checks())    std->print("stack_checks ");
    if (has_timers())          std->print("timers ");
    std->cr();
  }
}


// Loops

extern "C" void loop_counter_overflow() {
  // This routine can go away if the interpreter is generated
  Interpreter::loop_counter_overflow();
}


void Interpreter::loop_counter_overflow() {
  const bool debug = false;
  methodOop method = DeltaProcess::active()->last_frame().method();
  method->set_invocation_count(method->invocation_count() + loop_counter_limit());
  reset_loop_counter();

  if (debug) {
    ResourceMark rm;
    int l = loop_counter_limit();
    std->print("loop counter > %d in ", l);
    method->print_value();
    std->cr();
  }
}


int Interpreter::loop_counter() {
  return interpreter_loop_counter;
}


void Interpreter::reset_loop_counter() {
  interpreter_loop_counter = 0;
}


int Interpreter::loop_counter_limit() {
  return interpreter_loop_counter_limit;
}


void Interpreter::set_loop_counter_limit(int limit) {
  assert(0 <= limit, "loop counter limit must be positive");
  interpreter_loop_counter_limit = limit;
}


// Runtime routines called from interpreter_asm.asm
//
// The following routine is for inline_cache_miss calls from
// within interpreter_asm.asm. Can go away as soon as not needed
// anymore (because interpreter is generated).

extern "C" void inline_cache_miss() {
  InterpretedIC::inline_cache_miss();
}


extern "C" void verifyPIC(oop pic) {
  if (!Universe::is_heap((oop*) pic)) fatal("pic should be in heap");
  if (!pic->is_objArray()) fatal("pic should be an objArray");
  int length = objArrayOop(pic)->length();
  if (!(2*size_of_smallest_interpreterPIC <= length && length <= 2*size_of_largest_interpreterPIC)) fatal("pic has wrong length field");
}


extern "C" void trace_send(oop receiver, methodOop method) {
  if (TraceMessageSend) {
    ResourceMark rm;
    std->print("Trace ");
    receiver->print_value();
    method->selector()->print_value();
    std->cr();
  }
}


// Runtime routines called from the generated interpreter

void Interpreter::trace_bytecode() {
  if (TraceInterpreterFramesAt) {
    if (TraceInterpreterFramesAt < NumberOfBytecodesExecuted) 
    {
      frame f = DeltaProcess::active()->last_frame();
      lprintf("Frame: fp = %#lx, sp = %#lx]\n", f.fp(), f.sp());
      for (oop* p = f.sp(); p <= f.temp_addr(0); p++) {
	lprintf("\t[%#lx]: ", p);
	(*p)->print_value();
	lprintf("\n");
      }
      u_char* ip = DeltaProcess::active()->last_frame().hp();
      char* name = Bytecodes::name((Bytecodes::Code)*ip);
      std->print("%9d 0x%x: %02x %s\n", NumberOfBytecodesExecuted, ip, *ip, name);
    }
  }
  else if (TraceBytecodes) {
    u_char* ip = DeltaProcess::active()->last_frame().hp();
    char* name = Bytecodes::name((Bytecodes::Code)*ip);
    std->print("%9d 0x%x: %02x %s\n", NumberOfBytecodesExecuted, ip, *ip, name);
  }
}


void Interpreter::warning_illegal(int ebx, int esi) {
  warning("illegal instruction (ebx = 0x%x, esi = 0x%x)", ebx, esi);
}

void Interpreter::wrong_eax() {
  fatal("interpreter bug: eax doesn't contain the right value");
}

void Interpreter::wrong_esp() {
  fatal("interpreter bug: esp doesn't contain the right value");
}

/*
void Interpreter::wrong_ebx() {
  fatal("interpreter bug: high 3 bytes of ebx # 0");
}

void Interpreter::wrong_obj() {
  fatal("interpreter bug: register doesn't contain a valid oop");
}

void Interpreter::wrong_primitive_result() {
  fatal("interpreter bug: primitive failed that is not supposed to fail");
}
*/


doubleOop Interpreter::oopify_FloatValue() {
  // Called from float_oopify. Get the float argument by inspecting
  // the stack and the argument of the Floats::oopify operation.
  frame f = DeltaProcess::active()->last_frame();
  assert(*(f.hp() - 3) == Bytecodes::float_unary_op_to_oop && *(f.hp() - 1) == Floats::oopify, "not called by Floats::oopify");
  int float_index = *(f.hp() - 2);
  assert(0 <= float_index && float_index < max_nof_floats, "illegal float index");
  double* float_address = (double*)((char*)f.fp() + (float_0_offset - (max_nof_floats - 1)*floatSize) + float_index*floatSize);
  return oopFactory::new_double(*float_address);
}


int* Interpreter::_invocation_counter_addr = NULL;

void Interpreter::set_invocation_counter_limit(int new_limit) {
  assert(_invocation_counter_addr != NULL, "invocation counter address unknown");
  assert(0 <= new_limit && new_limit <= methodOopDesc::_invocation_count_max, "illegal counter limit");
  assert(*((u_char*)_invocation_counter_addr - 2) == 0x81, "not a cmp edx, imm32 instruction anymore?")
  *_invocation_counter_addr = new_limit << methodOopDesc::_invocation_count_offset;
}


int Interpreter::get_invocation_counter_limit() {
  assert(_invocation_counter_addr != NULL, "invocation counter address unknown");
  return get_unsigned_bitfield(*_invocation_counter_addr, methodOopDesc::_invocation_count_offset, methodOopDesc::_invocation_count_width);
}

static int* loop_counter_addr() {  }
static int* loop_counter_limit_addr();

// entry points accessors

char* Interpreter::access(char* entry_point) {
  assert(entry_point, "code not generated yet");
  return entry_point;
}
extern "C" void restart_primitiveValue();
extern "C" void nlr_single_step_continuation();
extern "C" void redo_bytecode_after_deoptimization();
extern "C" void illegal();

char* Interpreter::redo_send_entry() 				{ return access(_redo_send_entry); }
//char* Interpreter::restart_primitiveValue() 			{ return access((char*)::restart_primitiveValue); }
char* Interpreter::nlr_single_step_continuation_entry()		{ return access(Interpreter::_nlr_single_step_continuation_entry); }
//char* Interpreter::redo_bytecode_after_deoptimization()		{ return access((char*)::redo_bytecode_after_deoptimization); }
//char* Interpreter::illegal()					{ return access((char*)::illegal); }

char* Interpreter::restart_primitiveValue() 			{ return access(_restart_primitiveValue); }
Label& Interpreter::nlr_single_step_continuation()		{ 
	assert(_nlr_single_step_continuation.is_bound(), "code not generated yet");
	return _nlr_single_step_continuation; 
}
char* Interpreter::redo_bytecode_after_deoptimization()		{ return access(_redo_bytecode_after_deoptimization); }
char* Interpreter::illegal()					{ return access(_illegal); }


char* Interpreter::deoptimized_return_from_send_without_receiver() 				{ return access(_dr_from_send_without_receiver); }
char* Interpreter::deoptimized_return_from_send_without_receiver_restore() 			{ return access(_dr_from_send_without_receiver_restore); }
char* Interpreter::deoptimized_return_from_send_without_receiver_pop() 				{ return access(_dr_from_send_without_receiver_pop); }
char* Interpreter::deoptimized_return_from_send_without_receiver_pop_restore() 			{ return access(_dr_from_send_without_receiver_pop_restore); }
char* Interpreter::deoptimized_return_from_send_with_receiver() 				{ return access(_dr_from_send_with_receiver); }
char* Interpreter::deoptimized_return_from_send_with_receiver_restore() 			{ return access(_dr_from_send_with_receiver_restore); }
char* Interpreter::deoptimized_return_from_send_with_receiver_pop() 				{ return access(_dr_from_send_with_receiver_pop); }
char* Interpreter::deoptimized_return_from_send_with_receiver_pop_restore() 			{ return access(_dr_from_send_with_receiver_pop_restore); }
char* Interpreter::deoptimized_return_from_primitive_call_without_failure_block() 		{ return access(_dr_from_primitive_call_without_failure_block); }
char* Interpreter::deoptimized_return_from_primitive_call_without_failure_block_restore() 	{ return access(_dr_from_primitive_call_without_failure_block_restore); }
char* Interpreter::deoptimized_return_from_primitive_call_with_failure_block() 			{ return access(_dr_from_primitive_call_with_failure_block); }
char* Interpreter::deoptimized_return_from_primitive_call_with_failure_block_restore() 		{ return access( _dr_from_primitive_call_with_failure_block_restore); }
char* Interpreter::deoptimized_return_from_dll_call() 						{ return access(_dr_from_dll_call); }
char* Interpreter::deoptimized_return_from_dll_call_restore() 					{ return access(_dr_from_dll_call_restore); }


/*
extern "C" void deoptimized_return_from_send_without_receiver();
extern "C" void deoptimized_return_from_send_without_receiver_restore();
extern "C" void deoptimized_return_from_send_without_receiver_pop();
extern "C" void deoptimized_return_from_send_without_receiver_pop_restore();
extern "C" void deoptimized_return_from_send_with_receiver();
extern "C" void deoptimized_return_from_send_with_receiver_restore();
extern "C" void deoptimized_return_from_send_with_receiver_pop();
extern "C" void deoptimized_return_from_send_with_receiver_pop_restore();
extern "C" void deoptimized_return_from_primitive_call_without_failure_block();
extern "C" void deoptimized_return_from_primitive_call_without_failure_block_restore();
extern "C" void deoptimized_return_from_primitive_call_with_failure_block();
extern "C" void deoptimized_return_from_primitive_call_with_failure_block_restore();
extern "C" void deoptimized_return_from_dll_call();
extern "C" void deoptimized_return_from_dll_call_restore();

char* Interpreter::deoptimized_return_from_send_without_receiver() 				{ return access((char*)::deoptimized_return_from_send_without_receiver); }
char* Interpreter::deoptimized_return_from_send_without_receiver_restore() 			{ return access((char*)::deoptimized_return_from_send_without_receiver_restore); }
char* Interpreter::deoptimized_return_from_send_without_receiver_pop() 				{ return access((char*)::deoptimized_return_from_send_without_receiver_pop); }
char* Interpreter::deoptimized_return_from_send_without_receiver_pop_restore() 			{ return access((char*)::deoptimized_return_from_send_without_receiver_pop_restore); }
char* Interpreter::deoptimized_return_from_send_with_receiver() 				{ return access((char*)::deoptimized_return_from_send_with_receiver); }
char* Interpreter::deoptimized_return_from_send_with_receiver_restore() 			{ return access((char*)::deoptimized_return_from_send_with_receiver_restore); }
char* Interpreter::deoptimized_return_from_send_with_receiver_pop() 				{ return access((char*)::deoptimized_return_from_send_with_receiver_pop); }
char* Interpreter::deoptimized_return_from_send_with_receiver_pop_restore() 			{ return access((char*)::deoptimized_return_from_send_with_receiver_pop_restore); }
char* Interpreter::deoptimized_return_from_primitive_call_without_failure_block() 		{ return access((char*)::deoptimized_return_from_primitive_call_without_failure_block); }
char* Interpreter::deoptimized_return_from_primitive_call_without_failure_block_restore() 	{ return access((char*)::deoptimized_return_from_primitive_call_without_failure_block_restore); }
char* Interpreter::deoptimized_return_from_primitive_call_with_failure_block() 			{ return access((char*)::deoptimized_return_from_primitive_call_with_failure_block); }
char* Interpreter::deoptimized_return_from_primitive_call_with_failure_block_restore() 		{ return access((char*)::deoptimized_return_from_primitive_call_with_failure_block_restore); }
char* Interpreter::deoptimized_return_from_dll_call() 						{ return access((char*)::deoptimized_return_from_dll_call); }
char* Interpreter::deoptimized_return_from_dll_call_restore() 					{ return access((char*)::deoptimized_return_from_dll_call_restore); }
*/
// Interpreter initialization

void Interpreter::init() {
  if (_is_initialized) return;
  reset_loop_counter();
  set_loop_counter_limit(LoopCounterLimit);
  set_invocation_counter_limit(InvocationCounterLimit);
  _is_initialized = true;
}


// The InterpreterGenerator contains the functionality to generate
// the interpreter in the system initialization phase.

class InterpreterGenerator: StackObj {
 private:
  MacroAssembler*	masm;		// used to generate code
  bool			_debug;		// indicates debug mode

  bool			_stack_check;

  Label		_method_entry;		// entry point to activate method execution
  Label		_block_entry;		// entry point to activate block execution (primitiveValue)
  Label		_inline_cache_miss;	// inline cache misses handling
  Label		_smi_send_failure;	// handles predicted smi send failures
  Label		_issue_NLR;		// the starting point for NLRs in interpreted code
  Label		_nlr_testpoint;		// the return point for NLRs in interpreted sends
  Label		_C_nlr_testpoint;	// the return point for NLRs in C

  Label		_boolean_expected;	// boolean expected error
  Label		_float_expected;	// float expected error
  Label		_NLR_to_dead_frame;	// NLR error
  Label		_halted;		// halt executed

  Label		_stack_missaligned;	  // assertion errors
  Label		_ebx_wrong;		  //
  Label		_obj_wrong;		  //
  Label		_last_Delta_fp_wrong;	  //
  Label		_primitive_result_wrong;  //
  char*		_illegal;


  // Debugging
  void check_ebx();
  void check_oop(Register reg);
  void should_not_reach_here();

  void stack_check_push();
  void stack_check_pop();

  // Arguments, temporaries & instance variables
  Address arg_addr(int i);
  Address arg_addr(Register arg_no);
  Address temp_addr(int i);
  Address temp_addr(Register temp_no);
  Address float_addr(Register float_no);
  Address field_addr(Register obj, int i);
  Address field_addr(Register obj, Register smi_offset);

  // Instruction sequencing
  void skip_words(int n);
  void advance_aligned(int n);

  void load_ebx();
  void next_ebx();
  void jump_ebx();

  void load_edi();
  void jump_edi();

  char* entry_point();

  // Frame addresses
  Address self_addr()		{ return Address(ebp, self_offset); }
  Address esi_addr()		{ return Address(ebp, esi_offset); }
  Address context_addr()	{ return Address(ebp, temp_0_offset); }

  void save_esi()		{ masm->movl(esi_addr(), esi); }
  void restore_esi()		{ masm->movl(esi, esi_addr()); }
  void restore_ebx()		{ masm->xorl(ebx, ebx); }

  // Constant addresses
  Address nil_addr()		{ return Address(int(&nilObj), relocInfo::external_word_type); }
  Address true_addr()		{ return Address(int(&trueObj), relocInfo::external_word_type); }
  Address false_addr()		{ return Address(int(&falseObj), relocInfo::external_word_type); }
  Address smiKlass_addr()	{ return Address(int(&smiKlassObj), relocInfo::external_word_type); }
  Address doubleKlass_addr()	{ return Address(int(&doubleKlassObj), relocInfo::external_word_type); }
  Address contextKlass_addr()	{ return Address(int(&contextKlassObj), relocInfo::external_word_type); }

  // C calls
  void call_C(Label& L);
  void call_C(char* entry);
  void call_C(Register entry);

  // Parameter passing/returns
  void load_recv(Bytecodes::ArgumentSpec arg_spec);
  void return_tos(Bytecodes::ArgumentSpec arg_spec);
  void zap_context();

  // Debugging
  void generateStopInterpreterAt();

  // Instructions
  char* push_temp(int i);
  char* push_temp_n();
  char* push_arg(int i);
  char* push_arg_n();
  char* push_smi(bool negative);
  char* push_literal();
  char* push_tos();
  char* push_self();
  char* push_const(Address obj_addr);
  char* push_global();
  char* push_instVar();
  char* return_instVar();
  char* only_pop();

  char* store_temp(int i, bool pop = false);
  char* store_temp_n(bool pop = false);
  char* store_global(bool pop = false);
  char* store_instVar(bool pop = false);

  char* allocate_temps(int n);
  char* allocate_temps_n();

  char* set_self_via_context();
  char* with_context_temp(bool store, int tempNo, int contextNo);
  char* copy_params_into_context(bool self, int paramsCount);

  char* float_allocate();
  char* float_floatify();
  char* float_oopify();
  char* float_move();
  char* float_set();
  char* float_op(int nof_args, bool returns_float = false);

  char* push_closure(int nofArgs, bool use_context);
  char* install_context(int nofArgs, bool for_method);

  char* predict_prim(bool canFail);
  char* lookup_primitive();
  char* call_primitive();
  char* call_primitive_can_fail();
  char* call_DLL(bool async);

  void  call_method();
  void  call_native(Register entry);
  void  generate_error_handler_code();
  void  generate_nonlocal_return_code();
  void  generate_method_entry_code();
  void  generate_inline_cache_miss_handler();
  void  generate_predicted_smi_send_failure_handler();
  void  generate_redo_send_code();
  void  generate_deoptimized_return_restore();
  void  generate_deoptimized_return_code();
  void	generate_primitiveValue(int i);
  void	generate_forStubRountines();
  char* normal_send(Bytecodes::Code code, bool allow_methodOop, bool allow_nmethod, bool primitive_send = false);

  char* control_cond(Bytecodes::Code code);
  char* control_while(Bytecodes::Code code);
  char* control_jump(Bytecodes::Code code);

  char* access_send(bool self);

  char* primitive_send  (Bytecodes::Code code);
  char* interpreted_send(Bytecodes::Code code);
  char* compiled_send   (Bytecodes::Code code);
  char* polymorphic_send(Bytecodes::Code code);
  char* megamorphic_send(Bytecodes::Code code);

  void  check_smi_tags();
  char* smi_add();
  char* smi_sub();
  char* smi_mul();
  char* smi_compare_op(Bytecodes::Code code);
  char* smi_logical_op(Bytecodes::Code code);
  char* smi_shift();

  char* objArray_size();
  char* objArray_at();
  char* objArray_at_put();

  char* compare(bool equal);
  char* special_primitive_send_hint();
  char* halt();
  char* illegal() { return _illegal; }

  char* local_return(bool push_self, int nofArgs, bool zap = false);

  // Non-local returns
  char* nonlocal_return_tos();
  char* nonlocal_return_self();

  // Instruction generation
  char* generate_instruction(Bytecodes::Code code);

  // Generation helper
  void  info(char* name);

  // Generation
  void  generate_all();

 public:
  InterpreterGenerator(CodeBuffer* code, bool debug);
};


// Debugging

void InterpreterGenerator::check_ebx() {
  // check if ebx is 000000xx
  if (_debug) {
    masm->testl(ebx, 0xFFFFFF00);
    masm->jcc(Assembler::notZero, _ebx_wrong);
  }
}


void InterpreterGenerator::check_oop(Register reg) {
  // check if reg contains an oop
  if (_debug) {
    masm->testb(reg, Mark_Tag_Bit);
    masm->jcc(Assembler::notZero, _obj_wrong);
  }
}

//-----------------------------------------------------------------------------------------
// Stack checker
//
// The stack checker pushes a magic number on the stack and
// checks for it when it is popped. The stack checker is helpfull
// to detect inconsistent numbers of pushes and pops within a
// structured construct (e.g., such as loops). Check code is only
// generated if stack_checks are enabled.

static const int magic		= 0x0FCFCFCFC;			// must be a smi

void InterpreterGenerator::stack_check_push() {
  if (_stack_check) {
    masm->pushl(eax);
    masm->movl(eax, magic);
  }
}

void InterpreterGenerator::stack_check_pop() {
  if (_stack_check) {
    Label L;
    // ;_print "pop : esp = 0x%x", esp, 0
    // ;_print "      tos = 0x%x", eax, 0
    masm->cmpl(eax, magic);
    masm->jcc(Assembler::notEqual, _stack_missaligned);
    masm->bind(L);
    masm->popl(eax);
  }
}

void InterpreterGenerator::should_not_reach_here() {
  // make sure the Interpreter traps if this point is ever reached
  masm->hlt();
}


// Arguments, temporaries & instance variables

Address InterpreterGenerator::arg_addr(int i) {
  assert(1 <= i, "argument number must be positive");
  return Address(ebp, arg_n_offset + i*oopSize);
}


Address InterpreterGenerator::arg_addr(Register arg_no) {
  return Address(ebp, arg_no, Address::times_4, arg_n_offset + 1*oopSize, relocInfo::none);
}


Address InterpreterGenerator::temp_addr(int i) {
  assert(0 <= i, "temporary number must be positive");
  return Address(ebp, temp_0_offset - i*oopSize);
}


Address InterpreterGenerator::temp_addr(Register temp_no) {
  return Address(ebp, temp_no, Address::times_4, temp_0_offset - (max_nof_temps - 1)*oopSize, relocInfo::none);
}


Address InterpreterGenerator::float_addr(Register float_no) {
  return Address(ebp, float_no, Address::times_8, float_0_offset - (max_nof_floats - 1)*floatSize, relocInfo::none);
}


Address InterpreterGenerator::field_addr(Register obj, int i) {
  assert(2 <= i, "illegal field offset");
  return Address(obj, byteOffset(i));
}


Address InterpreterGenerator::field_addr(Register obj, Register smi_offset) {
  return Address(obj, smi_offset, Address::times_1, -Mem_Tag, relocInfo::none);
}


// Instruction sequencing

void InterpreterGenerator::skip_words(int n) {
  masm->addl(esi, (n+1)*oopSize);	// advance
  masm->andl(esi, -oopSize);		// align
}


void InterpreterGenerator::advance_aligned(int n) {
  masm->addl(esi, n + oopSize - 1);	// advance
  masm->andl(esi, -oopSize);		// align
}


void InterpreterGenerator::load_ebx() {
  check_ebx();
  masm->movb(ebx, Address(esi));
}


void InterpreterGenerator::next_ebx() {
  check_ebx();
  masm->movb(ebx, Address(esi, 1));
  masm->incl(esi);
}

void InterpreterGenerator::generateStopInterpreterAt() {
    if (StopInterpreterAt > 0) {
      Label cont;
	  masm->pushl(edx);
	  masm->movl(edx, Address(int(&StopInterpreterAt), relocInfo::external_word_type));
      masm->cmpl(edx, Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type));
	  masm->popl(edx);
	  masm->jcc(Assembler::above, cont);
      masm->int3();
      masm->bind(cont);
    }
}
void InterpreterGenerator::jump_ebx() {
  if (TraceBytecodes || CountBytecodes || StopInterpreterAt > 0) {
    masm->incl(Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type));
	generateStopInterpreterAt();
    /*if (StopInterpreterAt > 0) {
      Label cont;
      masm->cmpl(Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type), StopInterpreterAt);
      masm->jcc(Assembler::less, cont);
      masm->int3();
      masm->bind(cont);
    }*/
  }
  if (TraceBytecodes) {
    masm->pushl(eax);	// save tos
    call_C((char*)Interpreter::trace_bytecode);
    masm->popl(eax);	// restore tos
    load_ebx();
  }
  check_oop(eax);
  masm->jmp(Address(noreg, ebx, Address::times_4, (int)dispatchTable::table()));
}


void InterpreterGenerator::load_edi() {
  masm->movl(edi, Address(noreg, ebx, Address::times_4, (int)dispatchTable::table()));
}


void InterpreterGenerator::jump_edi() {
  if (TraceBytecodes || CountBytecodes || StopInterpreterAt > 0) {
    masm->incl(Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type));
	generateStopInterpreterAt();
	/*    if (StopInterpreterAt > 0) {
      Label cont;
      masm->cmpl(Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type), StopInterpreterAt);
      masm->jcc(Assembler::less, cont);
      masm->int3();
      masm->bind(cont);
    }*/
  }
  if (TraceBytecodes) {
    masm->pushl(eax);	// save tos
    call_C((char*)Interpreter::trace_bytecode);
    masm->popl(eax);	// restore tos
    load_ebx();
  }
  check_oop(eax);
  masm->jmp(edi);
}


char* InterpreterGenerator::entry_point() {
  char* ep = masm->pc();
  if (_debug) {
    check_ebx();
    check_oop(eax);
  }
  return ep;
}


// C calls
//
// Always use an ic info so that NLRs can be handled since
// an NLR may be issued via an abort to terminate a thread.

void InterpreterGenerator::call_C(Label& L) {
  save_esi();
  masm->call_C(L, _C_nlr_testpoint);
  restore_esi();
  restore_ebx();
}


void InterpreterGenerator::call_C(char* entry) {
  save_esi();
  masm->call_C(entry, relocInfo::runtime_call_type, _C_nlr_testpoint);
  restore_esi();
  restore_ebx();
}


void InterpreterGenerator::call_C(Register entry) {
  save_esi();
  masm->call_C(entry, _C_nlr_testpoint);
  restore_esi();
  restore_ebx();
}


// Calling conventions for sends
//
// For general sends the receiver and the arguments are pushed on the stack in the order
// in which they appear in the source code (i.e., receiver first, first argument, etc.).
// For self and super sends, the receiver is *not* pushed but taken directly from the
// callers stack frame, i.e. in these cases only the arguments are on the stack.
//
// The callee is responsible for removing the arguments from the stack, i.e., its return
// instructions have to know how many arguments there are.
//
// load_recv is loading the receiver into eax and makes sure that the receiver
// (except for self and super sends) as well as the arguments are on the stack.


void InterpreterGenerator::load_recv(Bytecodes::ArgumentSpec arg_spec) {
  masm->pushl(eax); // make sure receiver & all arguments are on the stack
  switch (arg_spec) {
    case Bytecodes::recv_0_args: break; // recv already in eax
    case Bytecodes::recv_1_args: masm->movl(eax, Address(esp, 1*oopSize)); break;
    case Bytecodes::recv_2_args: masm->movl(eax, Address(esp, 2*oopSize)); break;
    case Bytecodes::recv_n_args:
      // byte after send byte code specifies the number of arguments (0..255)
      masm->movb(ebx, Address(esi, 1));
      masm->movl(eax, Address(esp, ebx, Address::times_4));
      break;
    case Bytecodes::args_only  : masm->movl(eax, self_addr()); break;
    default                    : ShouldNotReachHere();
  }
}


//-----------------------------------------------------------------------------------------
// Instructions

char* InterpreterGenerator::push_temp(int i) {
  char* ep = entry_point();
  next_ebx();
  masm->pushl(eax);
  load_edi();
  masm->movl(eax, temp_addr(i));
  jump_edi();
  return ep;
}


char* InterpreterGenerator::push_temp_n() {
  char* ep = entry_point();
  masm->addl(esi, 2);
  masm->movb(ebx, Address(esi, -1));
  masm->pushl(eax);
  masm->movl(eax, temp_addr(ebx));
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_arg(int i) {
  char* ep = entry_point();
  next_ebx();
  masm->pushl(eax);
  load_edi();
  masm->movl(eax, arg_addr(i));
  jump_edi();
  return ep;
}


char* InterpreterGenerator::push_arg_n() {
  char* ep = entry_point();
  masm->addl(esi, 2);
  masm->movb(ebx, Address(esi, -1));
  masm->pushl(eax);
  masm->movl(eax, arg_addr(ebx));
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_smi(bool negative) {
  char* ep = entry_point();
  masm->movb(ebx, Address(esi, 1));	// get b
  masm->addl(esi, 2);			// advance to next bytecode
  masm->pushl(eax);			// save tos
  if (negative) {
    masm->leal(eax, Address(noreg, ebx, Address::times_4));
    masm->negl(eax);
  } else {
    masm->leal(eax, Address(noreg, ebx, Address::times_4, 4, relocInfo::none));
  }
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_literal() {
  char* ep = entry_point();
  masm->pushl(eax);
  skip_words(1);
  load_ebx();
  masm->movl(eax, Address(esi, -4));
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_tos() {
  char* ep = entry_point();
  next_ebx();
  masm->pushl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_self() {
  char* ep = entry_point();
  next_ebx();
  masm->pushl(eax);
  load_edi();
  masm->movl(eax, self_addr());
  jump_edi();
  return ep;
}


char* InterpreterGenerator::push_const(Address obj_addr) {
  char* ep = entry_point();
  masm->pushl(eax);
  next_ebx();
  masm->movl(eax, obj_addr);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_instVar() {
  char* ep = entry_point();
  advance_aligned(1 + oopSize);
  masm->movl(ecx, self_addr());
  masm->movl(edx, Address(esi, -oopSize));
  masm->pushl(eax);
  load_ebx();
  masm->movl(eax, field_addr(ecx, edx));
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::store_instVar(bool pop) {
  char* ep = entry_point();
  advance_aligned(1 + oopSize);
  masm->movl(ecx, self_addr());
  masm->movl(edx, Address(esi, -oopSize));
  load_ebx();
  masm->movl(field_addr(ecx, edx), eax);
  masm->store_check(ecx, edx);
  if (pop) masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::return_instVar() {
  char* ep = entry_point();
  advance_aligned(1 + oopSize);
  masm->movl(ecx, self_addr());
  masm->movl(edx, Address(esi, -oopSize));
  masm->movl(eax, field_addr(ecx, edx));
  return_tos(Bytecodes::recv_0_args);
  return ep;
}


char* InterpreterGenerator::only_pop() {
  char* ep = entry_point();
  next_ebx();
  masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::store_temp(int i, bool pop) {
  char* ep = entry_point();
  next_ebx();
  masm->movl(temp_addr(i), eax);
  if (pop) masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::store_temp_n(bool pop) {
  char* ep = entry_point();
  masm->addl(esi, 2);
  masm->movb(ebx, Address(esi, -1));
  masm->movl(temp_addr(ebx), eax);
  load_ebx();
  if (pop) masm->popl(eax);
  jump_ebx();
  return ep;
}

extern "C" void trace_push_global(oop assoc, oop value) {
  ResourceMark rm;
  std->print_cr("Trace push_global: ");
  assoc->print_value();
  std->cr();
  value->print_value();
  std->cr();
}

char* InterpreterGenerator::push_global() {
  char* ep = entry_point();
  skip_words(1);
  masm->pushl(eax);
  masm->movl(ecx, Address(esi, -oopSize));					// get association
  load_ebx();
  masm->movl(eax, field_addr(ecx, associationOopDesc::value_offset()));	// get value via association

  
  if (false) { // trace push_global 
    masm->pushad();
    masm->pushl(eax);	// pass arguments (C calling convention)
    masm->pushl(ecx);
    masm->call_C((char*)trace_push_global, relocInfo::runtime_call_type);
    masm->addl(esp, oopSize*2);   // get rid of arguments
    masm->popad();
  }

  jump_ebx();
  return ep;
}


char* InterpreterGenerator::store_global(bool pop) {
  char* ep = entry_point();
  skip_words(1);
  masm->movl(ecx, Address(esi, -oopSize));					// get association
  load_ebx();
  masm->movl(field_addr(ecx, associationOopDesc::value_offset()), eax);	// store value via association
  masm->store_check(ecx, edx);
  if (pop) masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::allocate_temps(int n) {
  char* ep = entry_point();
  assert(n > 0, "just checkin'");
  next_ebx();
  masm->pushl(eax);
  masm->movl(eax, nil_addr());
  while (--n > 0) masm->pushl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::allocate_temps_n() {
  Label entry, loop;

  masm->bind(loop);
  masm->pushl(eax);
  masm->bind(entry);
  masm->decb(ebx);
  masm->jcc(Assembler::notZero, loop);
  load_ebx();
  jump_ebx();

  char* ep = entry_point();
  masm->movb(ebx, Address(esi, 1));		// get n (n = 0 ==> 256 temps)
  masm->addl(esi, 2);				// advance to next bytecode
  masm->pushl(eax);
  masm->movl(eax, nil_addr());
  masm->jmp(entry);

  return ep;
}


//-----------------------------------------------------------------------------------------
// Context temporaries
//
// Note that eax must always be pushed in the beginning since it may hold the context (temp0).
// (tos is always in eax)

char* InterpreterGenerator::set_self_via_context() {
  Label loop;
  char* ep = entry_point();
  next_ebx();
  masm->movl(edx, self_addr());		// get incoming context (stored in receiver)
  masm->bind(loop);				// search for home context
  masm->movl(ecx, edx);			// save current context
  masm->movl(edx, Address(edx, contextOopDesc::parent_byte_offset()));
  masm->test(edx, Mem_Tag);			// check if parent is_smi
  masm->jcc(Assembler::notZero, loop);		// if not, current context is not home context
  masm->movl(edx, Address(ecx, contextOopDesc::temp0_byte_offset()));
  masm->movl(self_addr(), edx);		// set self in activation frame
  jump_ebx();
  return ep;
}

char* InterpreterGenerator::with_context_temp(bool store, int tempNo, int contextNo) {
  assert(contextNo >= -1, "illegal context no.");
  assert(tempNo >= -1, "illegal temporary no.");

  Label _loop;
  int codeSize = 1 + (contextNo == -1 ? 1 : 0) + (tempNo == -1 ? 1 : 0);

  char* ep = entry_point();

  if (!store) {
    masm->pushl(eax);
  }

  masm->movl(ecx, context_addr());

  if (contextNo == -1) {
    masm->movb(ebx, Address(esi, codeSize-1));
    masm->bind(_loop);
    masm->movl(ecx, Address(ecx, contextOopDesc::parent_byte_offset()));
    masm->decb(ebx);
    masm->jcc(Assembler::notZero, _loop);
  } else {
    for (int i = 0; i < contextNo; i++)
      masm->movl(ecx, Address(ecx, contextOopDesc::parent_byte_offset()));
  }
    //slr debugging
    //if (!store && tempNo == 1 && contextNo == 1) {
    //  Label not_frame;
    //  masm->testl(ecx, 1); // test for a frame in the context
    //  masm->jcc(Assembler::notZero, not_frame);
    //  masm->int3();
    //  masm->bind(not_frame);
    //}
    //

  Address slot;
  if (tempNo == -1) {
    masm->movb(ebx, Address(esi, 1));
    slot = Address(ecx, ebx, Address::times_4, contextOopDesc::temp0_byte_offset());
  } else {
    slot = Address(ecx, contextOopDesc::temp0_byte_offset() + tempNo*oopSize);
  }

  if (!store) {
    masm->movl(eax, slot);
  } else {
    masm->movl(slot, eax);
    masm->store_check(ecx, eax);
    masm->popl(eax);
  }

  masm->addl(esi, codeSize);
  load_ebx();
  jump_ebx();

  return ep;
}

//-----------------------------------------------------------------------------------------
// Copy parameters into context
//

char* InterpreterGenerator::copy_params_into_context(bool self, int paramsCount) {
  assert(paramsCount >= -1, "illegal params count.");

  Label _loop;
  int oneIfSelf = self ? 1 : 0;

  char* ep = entry_point();

  masm->pushl(eax);					// save tos (make sure temp0 is in memory)
  masm->movl(ecx, context_addr());
  masm->movl(eax, ecx);
  masm->store_check(eax, edx);

  if (self) {
    // store recv
    masm->movl(edx, self_addr());
    masm->movl(Address(ecx, contextOopDesc::temp0_byte_offset()), edx);
  }

  if (paramsCount == -1) {
    masm->addl(esi, 2);					// esi points to first parameter index
    masm->movb(eax, Address(esi, -1));			// get b (nof params)
   masm->bind(_loop);
    masm->movb(ebx, Address(esi));			// get parameter index
    masm->movl(edx, arg_addr(ebx));			// get parameter
    Address slot = Address(ecx, contextOopDesc::temp0_byte_offset() + oopSize*oneIfSelf);
    masm->movl(slot, edx);				// store in context variable
    masm->addl(ecx, 4);
    masm->incl(esi);
    masm->decb(eax);
    masm->jcc(Assembler::notZero, _loop);
  } else {
    for (int i = 0; i < paramsCount; i++) {
      masm->movb(ebx, Address(esi, 1+i));		// get i.th parameter index
      masm->movl(edx, arg_addr(ebx));			// get parameter
      Address slot = Address(ecx, contextOopDesc::temp0_byte_offset() + oopSize*(i + oneIfSelf));
      masm->movl(slot, edx);				// store (i+oneIfSelf).th in context variable
    }
    masm->addl(esi, 1+paramsCount);
  }

  load_ebx();
  masm->popl(eax);					// restore tos
  jump_ebx();

  return ep;
}
//-----------------------------------------------------------------------------------------
// Blocks
//
// push_closure allocates and pushes a (block) closure on tos. The closure's fields
// are initialized depending on the function arguments allocation_routine and use_context.
// allocation_routine is the entry point used to allocate the block. If use_context is true,
// this frame's context is used for the closure initialization, otherwise tos is used instead.
// Additionally, whenever a block is created, its surrounding block or method's invocation
// counter is incremented.

/*
extern "C" oop allocateBlock(smiOop nofArgs);	// Note: needs last Delta frame setup!

// Note: The following routines don't need the last Delta frame to be setup
extern "C" oop allocateBlock0();
extern "C" oop allocateBlock1();
extern "C" oop allocateBlock2();
*/

char* InterpreterGenerator::push_closure(int nofArgs, bool use_context) {
  char* ep = entry_point();
  masm->pushl(eax);							// save tos
  if (nofArgs == -1) {
    // no. of arguments specified by 2nd byte
    masm->movb(ebx, Address(esi, 1));					// get no. of arguments
    advance_aligned(2 + oopSize);					// go to next instruction
    masm->shll(ebx, Tag_Size);						// convert into smi (pushed on the stack!)
    save_esi();								// save vital registers
    masm->pushl(ebx);							// pass as argument
    masm->set_last_Delta_frame_before_call();				// allocateBlock needs last Delta frame!
    masm->call(GeneratedPrimitives::allocateBlock(nofArgs), relocInfo::runtime_call_type);	// eax := block closure(nof. args)
    masm->reset_last_Delta_frame();
    masm->popl(ebx);							// get rid of argument
  } else {
    // no. of arguments implied by 1st byte
    advance_aligned(1 + oopSize);					// go to next instruction
    save_esi();								// no last Delta frame setup needed => save vital registers
    masm->call(GeneratedPrimitives::allocateBlock(nofArgs), relocInfo::runtime_call_type);	// eax := block closure
  }
  restore_esi();							// returning from C land => restore esi (ebx is restored later)
  masm->movl(ecx, Address(esi, -oopSize));				// get block methodOop
  if (use_context) {							// if full block then
    masm->movl(edx, context_addr());					//   get context of this activation
    if (_debug) {
      // should check if edx is really pointing to a context
      // (can it ever happen that temp0 is not holding a context
      // but push_closure is used with the use_context attribute?)
    }
  } else {								// else
    masm->popl(edx);							//   use tos as context information
  }
  // install methodOop and context in block closure and increment
  // the invocation counter of the parent (= enclosing) methodOop
  //
  // eax: block closure
  // ecx: block methodOop
  // edx: context
  masm->movl(ebx, Address(ecx, methodOopDesc::selector_or_method_byte_offset()));	// get parent (= running) methodOop
  masm->movl(Address(eax, blockClosureOopDesc::method_or_entry_byte_offset()), ecx);	// set block method
  masm->movl(ecx, Address(ebx, methodOopDesc::counters_byte_offset()));// get counter of parent methodOop
  masm->movl(Address(eax, blockClosureOopDesc::context_byte_offset()), edx);		// set context
  masm->addl(ecx, 1 << methodOopDesc::_invocation_count_offset);	// increment invocation counter of parent methodOop
  masm->movl(edx, eax);						// make sure eax is not destroyed
  masm->movl(Address(ebx, methodOopDesc::counters_byte_offset()), ecx);// store counter of parent methodOop
  restore_ebx();
  load_ebx();								// get next instruction
  masm->store_check(edx, ecx);						// do a store check on edx, use ecx as scratch register
  jump_ebx();
  return ep;
}


//-----------------------------------------------------------------------------------------
// Contexts
//
// install_context allocates and installs a (heap) context in temp0. The context's
// fields are initialized depending on the function arguments allocation_routine and
// for_method. allocation_routine is the entry point used to allocate the context. If
// for_method is true, the current frame pointer (ebp) will be the context's parent,
// otherwise the (incoming) context will be used as parent context.

/*
// Note: The following routines don't need the last Delta frame to be setup
extern "C" oop allocateContext(smiOop nofVars);
extern "C" oop allocateContext0();
extern "C" oop allocateContext1();
extern "C" oop allocateContext2();
*/

char* InterpreterGenerator::install_context(int nofArgs, bool for_method) {
  char* ep = entry_point();
  masm->pushl(eax);				// save tos
  if (nofArgs == -1) {
    // no. of variables specified by 2nd byte
    masm->movb(ebx, Address(esi, 1));		// get no. of variables
    masm->addl(esi, 2);			// go to next instruction
    masm->shll(ebx, Tag_Size);			// convert into smi (pushed on the stack!)
    save_esi();					// no last Delta frame setup needed => save vital registers
    masm->pushl(ebx);				// pass as argument
    masm->call(GeneratedPrimitives::allocateContext(nofArgs), relocInfo::runtime_call_type);		// eax := context(nof. vars)
    masm->popl(ebx);				// get rid of argument
  } else {
    // no. of variables implied by 1st byte
    masm->incl(esi);				// go to next instruction
    save_esi();					// no last Delta frame setup needed => save vital registers
    masm->call(GeneratedPrimitives::allocateContext(nofArgs), relocInfo::runtime_call_type);		// eax := context
  }
  restore_esi();				// returning from C land => restore vital registers
  restore_ebx();
  if (for_method) {				// if method context then
    masm->movl(Address(eax, contextOopDesc::parent_byte_offset()), ebp);	// parent points to method frame
  } else {					// else
    masm->movl(ecx, context_addr());		// get (incoming) enclosing context
    if (_debug) {
      // should check if ecx is really pointing to a context
      // (can it ever happen that temp0 is not holding a context
      // but install_context is used with the use_context attribute?)
    }
    masm->movl(Address(eax, contextOopDesc::parent_byte_offset()), ecx);	// parent points to enclosing context
  }
  load_ebx();					// get next instruction
  masm->movl(context_addr(), eax);		// install context
  masm->store_check(eax, ecx);			// store check on eax, use ecx as scratch register
  masm->popl(eax);				// restore tos
  jump_ebx();
  return ep;
}

//-----------------------------------------------------------------------------------------
// Control structures and jumps
//
// Jump offsets are unsigned bytes/words. For forward jumps, the jump destination
// is the address of the next instruction + offset, for backward jumps (whileTrue
// and whileFalse) the jump destination is the current instruction's address - offset.
//

char* InterpreterGenerator::control_cond(Bytecodes::Code code) {

  bool isByte, isTrue, isCond;

  switch (code) {
    case Bytecodes::ifTrue_byte		: isByte = true;  isTrue = true;  isCond = false; break;
    case Bytecodes::ifTrue_word		: isByte = false; isTrue = true;  isCond = false; break;
    case Bytecodes::ifFalse_byte	: isByte = true;  isTrue = false; isCond = false; break;
    case Bytecodes::ifFalse_word	: isByte = false; isTrue = false; isCond = false; break;
    case Bytecodes::and_byte		: isByte = true;  isTrue = true;  isCond = true;  break;
    case Bytecodes::and_word		: isByte = false; isTrue = true;  isCond = true;  break;
    case Bytecodes::or_byte		: isByte = true;  isTrue = false; isCond = true;  break;
    case Bytecodes::or_word		: isByte = false; isTrue = false; isCond = true;  break;
    default               		: ShouldNotReachHere();
  }

  Label _else;
  Address cond	   =  isTrue ? true_addr() : false_addr();
  Address not_cond = !isTrue ? true_addr() : false_addr();
  int codeSize = (isCond ? 1 : 2) + (isByte ? 1 : 4);

  char* ep = entry_point();

  if (!isByte) {
    advance_aligned(codeSize);
  }
  masm->cmpl(eax, cond);					// if tos # cond
  masm->jcc(Assembler::notEqual, _else);			// then jump to else part
  if (isByte) {
    masm->addl(esi, codeSize);					// skip info & offset byte
  }
  load_ebx();
  masm->popl(eax);						// discard condition
  jump_ebx();

 masm->bind(_else);
  masm->cmpl(eax, not_cond);					// if tos # ~cond
  masm->jcc(Assembler::notEqual, _boolean_expected);		// then non-boolean arguments

  // jump relative to next instr (must happen after the check for non-booleans)
  if (isByte) {
    masm->movb(ebx, Address(esi, codeSize -1));
    masm->leal(esi, Address(esi, ebx, Address::times_1, codeSize));
  } else {
    masm->addl(esi, Address(esi, -oopSize));
  }
  load_ebx();
  if (!isCond) {
    masm->popl(eax);						// discard condition
  }
  jump_ebx();

  return ep;
}

char* InterpreterGenerator::control_while(Bytecodes::Code code) {

  bool isByte, isTrue;

  switch (code) {
    case Bytecodes::whileTrue_byte	: isByte = true;  isTrue = true;  break;
    case Bytecodes::whileTrue_word	: isByte = false; isTrue = true;  break;
    case Bytecodes::whileFalse_byte	: isByte = true;  isTrue = false; break;
    case Bytecodes::whileFalse_word	: isByte = false; isTrue = false; break;
    default               		: ShouldNotReachHere();
  }

  Label _exit, _overflow, _call_overflow;
  Address cond	   =  isTrue ? true_addr() : false_addr();
  Address not_cond = !isTrue ? true_addr() : false_addr();
  int codeSize = 1 + (isByte ? 1 : oopSize);

  char* ep = entry_point();

  masm->cmpl(eax, cond);					// if tos # cond
  masm->jcc(Assembler::notEqual, _exit);			// then jump to else part

  if (isByte) {
    masm->movb(ebx, Address(esi, codeSize - 1));
    masm->subl(esi, ebx);
  } else {
    masm->leal(edx, Address(esi, codeSize + 3));		// unaligned address of next instruction
    masm->andl(edx, ~3);     					// aligned address of next instruction
    masm->subl(esi, Address(edx, -oopSize));
  }

  masm->movl(edx, Address((int)&interpreter_loop_counter, relocInfo::external_word_type));
  load_ebx();
  masm->popl(eax);						// discard loop condition
  masm->incl(edx);
  masm->movl(Address((int)&interpreter_loop_counter, relocInfo::external_word_type), edx);
  masm->cmpl(edx, Address((int)&interpreter_loop_counter_limit, relocInfo::external_word_type));
  masm->jcc(Assembler::greater, _overflow);
  jump_ebx();

 masm->bind(_exit);
  masm->cmpl(eax, not_cond);					// if tos # ~cond
  masm->jcc(Assembler::notEqual, _boolean_expected);		// then non-boolean arguments

  // advance to next instruction (must happen after the check for non-booleans)
  if (isByte) {
    masm->addl(esi, codeSize);
  } else {
    masm->leal(edx, Address(esi, codeSize + 3));		// unaligned address of next instruction
    masm->andl(edx, ~3);     					// aligned address of next instruction
    masm->movl(esi, edx);
  }
  load_ebx();
  masm->popl(eax);						// discard loop condition
  stack_check_pop();
  jump_ebx();

 masm->bind(_overflow);
  call_C(_call_overflow);
  load_ebx();
  jump_ebx();

 masm->bind(_call_overflow);
  masm->pushl(eax);						// save eax
  masm->call((char*)&Interpreter::loop_counter_overflow, relocInfo::runtime_call_type);
  masm->popl(eax);						// restore eax
  masm->ret(0);

  return ep;
}

char* InterpreterGenerator::control_jump(Bytecodes::Code code) {

  bool isByte, isLoop;

  switch (code) {
    case Bytecodes::jump_else_byte	: isByte = true;  isLoop = false; break;
    case Bytecodes::jump_else_word	: isByte = false; isLoop = false; break;
    case Bytecodes::jump_loop_byte	: isByte = true;  isLoop = true;  break;
    case Bytecodes::jump_loop_word	: isByte = false; isLoop = true;  break;
    default               		: ShouldNotReachHere();
  }

  int codeSize = 1 + (isByte ? 1 : oopSize) * (isLoop ? 2 : 1);

  char* ep = entry_point();

  if (isLoop) {
    stack_check_push();
  }

  if (isByte) {
    masm->movb(ebx, Address(esi, codeSize -1));				// get jump offset
    masm->leal(esi, Address(esi, ebx, Address::times_1, codeSize));	// jump destination
  } else {
    advance_aligned(codeSize);
    masm->addl(esi, Address(esi, -oopSize));				// jump destination
  }

  load_ebx();
  jump_ebx();

  return ep;
}


//-----------------------------------------------------------------------------------------
// Floating-point operations
//
// These operations work in the float section of (interpreted) activation frames.
// Within that float section, floats are indexed from max_nof_floats - 1 to 0.
// Operations that work on floats have as 2nd byte the float index of the result.
//
// If there's a float section, the first 2 'normal' temporaries are always there,
// and temp1 holds a magic value which allows the GC to determine fast whether
// there is untagged data on the stack or not.
//
//
// Stack layout if there's a float section:
//
// eax	  top of stack
// esp->[ expressions	]
// 	  ...
// 	[ expressions	] <---- start of expression stack
//	[ float m-1	] <----	last float in float section
//	[ ...		]
// 	[ float 0	] <----	first float in float section
// 	[ temporary n-1	] <----	last 'normal' temporary (n is a multiple of 2 and n >= 2)
// 	  ...
// 	[ temporary 1	]	always here if there's a float section (holds Floats::magic)
//	[ temporary 0	]	always here if there's a float section
//	  ...
// ebp->[ previous ebp	]
//
//
// Note that (as always) the top of expression stack is hold in eax; i.e., if the
// expression stack is empty and there's a float section, the lower-half of the last
// float is kept in eax! This is not a problem, however one has to make sure to push
// eax before every float operation in order to have a floats completely in memory
// (this is not an extra burden since eax often has to be saved anyway).

char* InterpreterGenerator::float_allocate() {
// Allocates (additional) temps and floats in a stack frame.
// Bytecode format:
//
// <float_allocate code> <nofTemps> <nofFloats> <floatExprStackSize>
//
// <nofTemps>			no. of additional temps to allocate (in chunks of two) besides temp0 & temp1
// <nofFloats>			no. of initialized floats to allocate
// <floatExprStackSize>		no. of uninitialized floats to allocate

  Label tLoop, tDone, fLoop, fDone;
  assert(oop(Floats::magic)->is_smi(), "must be a smi");
  char* ep = entry_point();
  if (_debug) {
    // This instruction must be the first bytecode executed in a method (if there).
    Label L1, L2;
    // check stack pointer (must point to esi save location, temp0 is in eax)
    masm->leal(ecx, Address(ebp, esi_offset));
    masm->cmpl(esp, ecx);
    masm->jcc(Assembler::equal, L1);
    masm->call_C((char*)Interpreter::wrong_esp, relocInfo::runtime_call_type);
    should_not_reach_here();
    masm->bind(L1);

    // check eax (corresponds now to temp0, must be initialized to nil)
    masm->cmpl(eax, nil_addr());
    masm->jcc(Assembler::equal, L2);
    masm->call_C((char*)Interpreter::wrong_eax, relocInfo::runtime_call_type);
    should_not_reach_here();
    masm->bind(L2);
  }
  masm->addl(esi, 4);				// advance to next bytecode
  masm->pushl(eax);				// save tos (i.e. temp0)
  masm->pushl(Floats::magic);			// initialize temp1 (indicates a float section)

  // allocate additional temps in multiples of 2 (to compensate for one float)
  masm->movb(ebx, Address(esi, -3));		// get nofTemps
  masm->testl(ebx, ebx);			// allocate no additional temps if nofTemps = 0
  masm->jcc(Assembler::zero, tDone);
  masm->movl(eax, nil_addr());
  masm->bind(tLoop);
  masm->pushl(eax);				// push nil
  masm->pushl(eax);				// push nil
  masm->decl(ebx);
  masm->jcc(Assembler::notZero, tLoop);
  masm->bind(tDone);

  // allocate floats
  masm->movb(ebx, Address(esi, -2));		// get nofFloats
  masm->testl(ebx, ebx);			// allocate no additional floats if nofFloats = 0
  masm->jcc(Assembler::zero, fDone);
  masm->xorl(eax, eax);			// use 0 to initialize the stack with 0.0
  masm->bind(fLoop);
  masm->pushl(eax);				// push 0.0 (allocate a double)
  masm->pushl(eax);
  masm->decb(ebx);
  masm->jcc(Assembler::notZero, fLoop);
  masm->bind(fDone);

  // allocate floats expression stack
  assert(floatSize == 8, "change the constant for shhl below");
  masm->movb(ebx, Address(esi, -1));		// get floats expression stack size
  masm->shll(ebx, 3);				// multiply with floatSize
  masm->subl(esp, ebx);			// adjust esp
  restore_ebx();

  // continue with next instruction
  load_ebx();					// continue with next instruction
  masm->popl(eax);				// restore tos in eax
  jump_ebx();

  return ep;
}


char* InterpreterGenerator::float_floatify() {
  Label is_smi;
  char* ep = entry_point();
  masm->addl(esi, 2);				// advance to next instruction
  masm->testb(eax, Mem_Tag);			// check if smi
  masm->jcc(Assembler::zero, is_smi);
  masm->movl(ecx, Address(eax, memOopDesc::klass_byte_offset()));	// check if float
  masm->cmpl(ecx, doubleKlass_addr());
  masm->jcc(Assembler::notEqual, _float_expected);

  // unbox doubleOop
  masm->movb(ebx, Address(esi, -1));		// get float number
  masm->fld_d(Address(eax, byteOffset(doubleOopDesc::value_offset()))); // unbox float
  masm->fstp_d(float_addr(ebx));		// store float
  load_ebx();
  masm->popl(eax);				// discard argument
  jump_ebx();

  // convert smi
  masm->bind(is_smi);
  masm->movb(ebx, Address(esi, -1));		// get float number
  masm->leal(ecx, float_addr(ebx));
  masm->sarl(eax, Tag_Size);			// convert smi argument into int
  masm->movl(Address(ecx), eax);		// store it in memory (use float target location)
  masm->fild_s(Address(ecx));			// convert it into float
  masm->fstp_d(Address(ecx));			// store float
  load_ebx();
  masm->popl(eax);				// discard argument
  jump_ebx();

  return ep;
}


char* InterpreterGenerator::float_oopify() {
// Implements the Floats::oopify operation. It is implemented
// here rather than in Floats because it needs to do a C call.
// Instead of returning regularly, it directly continues with
// the next byte code.
  char* ep = entry_point();
  // here the return address to float_op is on the stack
  // discard it so that C routine can be called regularly.
  masm->popl(eax);				// discard return address
  masm->fpop();				// pop ST (in order to avoid FPU stack overflows) -> get rid of argument
  call_C((char*)Interpreter::oopify_FloatValue);// eax := oopify_FloatValue() (gets its argument by looking at the last bytecode)
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::float_move() {
  char* ep = entry_point();
  masm->pushl(eax);				// make sure last float is completely in memory
  masm->addl(esi, 3);				// advance to next instruction
  masm->xorl(ecx, ecx);			// clear ecx
  masm->movb(ebx, Address(esi, -1));		// get source float number
  masm->movb(ecx, Address(esi, -2));		// get destination float number
  masm->fld_d(float_addr(ebx));		// load source
  load_ebx();
  masm->fstp_d(float_addr(ecx));		// store at destination
  masm->popl(eax);				// re-adjust esp
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::float_set() {
  char* ep = entry_point();
  masm->pushl(eax);				// make sure last float is completely in memory
  masm->movb(ebx, Address(esi, 1));		// get float number
  advance_aligned(2 + oopSize);			// advance to next instruction
  masm->movl(ecx, Address(esi, -oopSize));	// get doubleOop address
  masm->fld_d(Address(ecx, byteOffset(doubleOopDesc::value_offset()))); // unbox float
  masm->fstp_d(float_addr(ebx));		// store it
  load_ebx();
  masm->popl(eax);				// re-adjust esp
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::float_op(int nof_args, bool returns_float) {
  assert(0 <= nof_args && nof_args <= 8, "illegal nof_args specification");
  char* ep = entry_point();
  masm->pushl(eax);				// make sure all floats are completely in memory
  masm->addl(esi, 3);				// advance to next instruction
  masm->movb(ebx, Address(esi, -2));		// get float number
  masm->leal(edx, float_addr(ebx));		// get float address
  masm->movb(ebx, Address(esi, -1));		// get function number
  masm->movl(ecx, Address(noreg, ebx, Address::times_4, int(Floats::_function_table), relocInfo::external_word_type));
  for (int i = 0; i < nof_args; i++) masm->fld_d(Address(edx, -i*floatSize));
  masm->call(ecx);				// invoke operation
  load_ebx();					// get next byte code
  if (returns_float) {
    masm->fstp_d(Address(edx));		// store result
    masm->popl(eax);				// re-adjust esp
  }						// otherwise: result in eax
  jump_ebx();
  return ep;
}

//-----------------------------------------------------------------------------------------
// Primitive calls
//
// Note: In general, when calling the VM, esi should point to the next bytecode
//       instruction. This is not the case when calling primitives::lookup_and_patch()
//       in lookup_primitive(). However, esi (i.e. f.hp()) is adjusted in the
//       lookup_and_patch routine.

char* InterpreterGenerator::predict_prim(bool canFail) {
  // _predict_prim & _predict_prim_ifFail are two bytecodes that are
  // used during lookup, during execution they can be simply ignored.
  char* ep = entry_point();
  advance_aligned(1 + (canFail ? 2 : 1)*oopSize);
  load_ebx();
  jump_ebx();
  return ep;
}

char* InterpreterGenerator::lookup_primitive() {
  char* ep = entry_point();
  masm->pushl(eax);				// push last argument
  call_C((char*)primitives::lookup_and_patch);	// do the lookup and patch call site appropriately
  load_ebx();
  masm->popl(eax);				// restore last argument
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::call_primitive() {
  char* ep = entry_point();
  advance_aligned(1 + oopSize);
  masm->pushl(eax);				// push last argument
  masm->movl(eax, Address(esi, -oopSize));	// get primitive entry point
  call_C(eax);					// eax := primitive call(...)
  if (_debug) {					// (Pascal calling conv. => args are popped by callee)
    masm->testb(eax, Mark_Tag_Bit);
    masm->jcc(Assembler::notZero, _primitive_result_wrong);
  }
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::call_primitive_can_fail() {
  Label failed;
  char* ep = entry_point();
  advance_aligned(1 + 2*oopSize);
  masm->pushl(eax);				// push last argument
  masm->movl(eax, Address(esi, -2*oopSize));	// get primitive entry point
  call_C(eax);					// eax := primitive call(...) (Pascal calling conv.)
  masm->testb(eax, Mark_Tag_Bit);		// if not marked then
  masm->jcc(Assembler::notZero, failed);
  masm->movl(ecx, Address(esi, -oopSize));	// get jump offset
  masm->addl(esi, ecx);			// jump over failure block
  load_ebx();
  jump_ebx();

  masm->bind(failed);
  masm->andl(eax, ~Mark_Tag_Bit);		// unmark result
  load_ebx();					// and execute failure block
  jump_ebx();
  return ep;
}

//-----------------------------------------------------------------------------------------
// DLL calls
//
// Byte code structure:
//
// 	[send		]	(1 byte)
//	....alignment....	(0..3 bytes)
//	[dll_name	]	(1 dword)
//	[function_name	]	(1 dword)
//	[0/function ptr	]	(1 dword)
//	[no. of Args	]	(1 byte)
//	...	  	  <---	esi
//
// The arguments of a dll call are either smis or proxys, i.e. boxed 32 bit words
// hold in a proxy object. Before calling the dll function, all arguments are con-
// verted into C arguments, i.e. smis are divided by 4 and boxed 32 bit words hold
// in proxys are unboxed. The arguments are then pushed in reversed order to comply
// with the C calling conventions. The result of the dll call is boxed again into
// a proxy object which has been pushed before the arguments.
//
// Because there are Delta and C frames on the same stack and the innards of the
// DLL call are unknown, its call is set up in an additional frame that behaves
// like a C frame (and that has its code outside the interpreters code) but that
// contains a frame pointer which can be used to properly set the last Delta frame.
//
// The call_DLL_entry routine (see StubRoutines) needs 3 arguments to be passed
// in registers as follows:
//
// ebx: no. of dll function arguments
// ecx: address of last dll function argument
// edx: dll function entry point
//
// NLRs through DLL calls: Note that if the DLL is returning via an NLR, the
// arguments don't need to be popped since the NLR is simply returning too
// (as for ordinary NLRs). Thus, NLRs are just propagated as usual.

char* InterpreterGenerator::call_DLL(bool async) {
  char* ep = entry_point();
  Label L;
  advance_aligned(1 + 3*oopSize);		// advance to no. of arguments byte
  masm->incl(esi);				// advance to next instruction (skip no. of arguments byte)
  masm->pushl(eax);				// push last argument
  masm->movl(edx, Address(esi, -1 - oopSize));	// get dll function ptr
  masm->testl(edx, edx);			// test if function has been looked up already
  masm->jcc(Assembler::notZero, L); 		// and continue - otherwise lookup dll function & patch
  call_C((char*)DLLs::lookup_and_patch_InterpretedDLL_Cache);	// eax := returns dll function ptr
  masm->movl(edx, eax);			// move dll function ptr into right register
  masm->bind(L);				// and continue
  masm->movb(ebx, Address(esi, -1));		// get no. of arguments
  masm->movl(ecx, esp);			// get address of last argument
  save_esi();					// don't use call_C because no last_Delta_frame setup needed
  masm->call(StubRoutines::call_DLL_entry(async), relocInfo::runtime_call_type);// eax := DLL call via a separate frame (parameter conversion)
  masm->ic_info(_nlr_testpoint, 0);
  restore_esi();
  restore_ebx();
  masm->movb(ebx, Address(esi, -1));		// get no. of arguments
  masm->leal(esp, Address(esp, ebx, Address::times_4)); // pop arguments
  masm->popl(ecx);				// get proxy object
  masm->movl(Address(ecx, pointer_offset), eax); // box result
  load_ebx();
  masm->movl(eax, ecx);			// return proxy
  jump_ebx();
  return ep;
}


//-----------------------------------------------------------------------------------------
// Redo send code
//
// Used to restart a send (issued via call_native) that called a zombie nmethod.
// The receiver (if not a self or super send) as well as the arguments are still
// on the stack and esi has been reset to the send byte code.

char* Interpreter::_redo_send_entry = NULL;

void InterpreterGenerator::generate_redo_send_code() {
  assert(Interpreter::_redo_send_entry == NULL, "code generated twice");
  Interpreter::_redo_send_entry = masm->pc();
  restore_esi();				// has been saved by call_native
  restore_ebx();				// possibly destroyed
  load_ebx();
  masm->popl(eax);				// get last argument into eax again
  jump_ebx();					// restart send
}


//-----------------------------------------------------------------------------------------
// Return entry points for deoptimized interpreter frames
//
//  There is may ways of returning from an interpreter frame.
//    - from send  (with or without pop) X (with or without receiver) X (with or without restoring result value)
//    - from primitive call (with or without failure block)           X (with or without restoring result value)
//    - from DLL call                                                   (with or without restoring result value)

char* Interpreter::_dr_from_send_without_receiver				= NULL;
char* Interpreter::_dr_from_send_without_receiver_restore			= NULL;
char* Interpreter::_dr_from_send_without_receiver_pop				= NULL;
char* Interpreter::_dr_from_send_without_receiver_pop_restore			= NULL;
char* Interpreter::_dr_from_send_with_receiver					= NULL;
char* Interpreter::_dr_from_send_with_receiver_restore				= NULL;
char* Interpreter::_dr_from_send_with_receiver_pop				= NULL;
char* Interpreter::_dr_from_send_with_receiver_pop_restore			= NULL;
char* Interpreter::_dr_from_primitive_call_without_failure_block		= NULL;
char* Interpreter::_dr_from_primitive_call_without_failure_block_restore	= NULL;
char* Interpreter::_dr_from_primitive_call_with_failure_block			= NULL;
char* Interpreter::_dr_from_primitive_call_with_failure_block_restore		= NULL;
char* Interpreter::_dr_from_dll_call						= NULL;
char* Interpreter::_dr_from_dll_call_restore					= NULL;

extern "C" int	 number_of_arguments_through_unpacking;
extern "C" oop   result_through_unpacking;

void InterpreterGenerator::generate_deoptimized_return_restore() {
  masm->movl(eax, Address((int)&number_of_arguments_through_unpacking,   relocInfo::external_word_type));
  masm->shll(eax, 2);
  masm->addl(esp, eax);
  masm->movl(eax, Address((int)&result_through_unpacking,   relocInfo::external_word_type));
}

void InterpreterGenerator::generate_deoptimized_return_code() {
  assert(Interpreter::_dr_from_dll_call == NULL, "code generated twice");

//#define  maybeINT3() masm->int3();
#define  maybeINT3()

  Label deoptimized_C_nlr_continuation;
  Label deoptimized_nlr_continuation;

 masm->bind(deoptimized_C_nlr_continuation);
  masm->reset_last_Delta_frame();
  // fall through
 masm->bind(deoptimized_nlr_continuation);
  // mov	eax, [_nlr_result]
  masm->jmp(_nlr_testpoint);

 Interpreter::_dr_from_send_without_receiver_restore = masm->pc();
  generate_deoptimized_return_restore();
  // fall through

 Interpreter::_dr_from_send_without_receiver = masm->pc();
  masm->ic_info(deoptimized_nlr_continuation, 0); // last part of the _call_method macro
  maybeINT3()
  restore_esi();
  restore_ebx();
  load_ebx();
  jump_ebx();

 Interpreter::_dr_from_send_without_receiver_pop_restore = masm->pc();
  generate_deoptimized_return_restore();
  // fall through

 Interpreter::_dr_from_send_without_receiver_pop = masm->pc();
  masm->ic_info(deoptimized_nlr_continuation, 0); // last part of the _call_method macro
  maybeINT3()
  restore_esi();
  restore_ebx();
  load_ebx();
  masm->popl(eax);		// pop result
  jump_ebx();

 Interpreter::_dr_from_send_with_receiver_restore = masm->pc();
  generate_deoptimized_return_restore();
  // fall through

 Interpreter::_dr_from_send_with_receiver = masm->pc();
  masm->ic_info(deoptimized_nlr_continuation, 0); // last part of the _call_method macro
  maybeINT3()
  restore_esi();
  restore_ebx();
  masm->popl(ecx);		// pop receiver
  load_ebx();
  jump_ebx();

 Interpreter::_dr_from_send_with_receiver_pop_restore = masm->pc();
  generate_deoptimized_return_restore();
  // fall through

 Interpreter::_dr_from_send_with_receiver_pop = masm->pc();
  masm->ic_info(deoptimized_nlr_continuation, 0); // last part of the _call_method macro
  maybeINT3()
  restore_esi();
  restore_ebx();
  masm->popl(ecx);		// pop receiver
  load_ebx();
  masm->popl(eax);		// pop result
  jump_ebx();

 Interpreter::_dr_from_primitive_call_without_failure_block_restore = masm->pc();
  generate_deoptimized_return_restore();
  // fall through

 Interpreter::_dr_from_primitive_call_without_failure_block = masm->pc();
  masm->ic_info(deoptimized_C_nlr_continuation, 0);
  maybeINT3()
  restore_esi();
  restore_ebx();				  // ebx := 0
  masm->reset_last_Delta_frame();
  if (_debug) {
    masm->test(eax, Mark_Tag_Bit);
    masm->jcc(Assembler::notZero, _primitive_result_wrong);
  }
  load_ebx();
  jump_ebx();

  Label _deoptimized_return_from_primitive_call_with_failure_block_failed;

 Interpreter::_dr_from_primitive_call_with_failure_block_restore = masm->pc();
  generate_deoptimized_return_restore();
  // fall through

 Interpreter::_dr_from_primitive_call_with_failure_block = masm->pc();
 masm->ic_info(deoptimized_C_nlr_continuation, 0);
  maybeINT3()
  restore_esi();
  restore_ebx();				  // ebx := 0
  masm->reset_last_Delta_frame();
  masm->test(eax, Mark_Tag_Bit);		  // if not marked then
  masm->jcc(Assembler::notZero, _deoptimized_return_from_primitive_call_with_failure_block_failed);
  masm->movl(ecx, Address(esi, -oopSize));	  // load jump offset
  masm->addl(esi, ecx);				  // and jump over failure block
  load_ebx();
  jump_ebx();

 masm->bind(_deoptimized_return_from_primitive_call_with_failure_block_failed);
  masm->andl(eax, ~Mark_Tag_Bit);		// else unmark result
  load_ebx();					// and execute failure block
  jump_ebx();					// the result will be stored
						// into a temp in the failure block

 Interpreter::_dr_from_dll_call_restore = masm->pc();
 masm->movl(eax, Address((int)&result_through_unpacking,   relocInfo::external_word_type));
  // fall through

 Interpreter::_dr_from_dll_call = masm->pc();
  masm->ic_info(deoptimized_C_nlr_continuation, 0);
  maybeINT3()
  masm->reset_last_Delta_frame();
  restore_esi();
  restore_ebx();				  // ebx := 0
  // eax: DLL result
  masm->movb(ebx, Address(esi, -1));				    // get no. of arguments
  masm->leal(esp, Address(esp, ebx, Address::times_4));		    // adjust sp (pop arguments)
  masm->popl(ecx);						    // get proxy object
  masm->movl(Address(ecx, proxyOopDesc::pointer_byte_offset()), eax);   // box result
  load_ebx();
  masm->movl(eax, ecx);	  					    // return proxy
  jump_ebx();
}

//-----------------------------------------------------------------------------------------
// Blocks
//
//  primitiveValue0..9 are the primitives called in block value messages.
//  i is the number of arguments for the block.

void InterpreterGenerator::generate_primitiveValue(int i) {
  GeneratedPrimitives::set_primitiveValue(i,masm->pc());
  masm->movl(eax, Address(esp, (i+1) * oopSize)); // load recv (= block)
  masm->jmp(_block_entry);
}


extern "C" int redo_send_offset;			      // offset when redoing send
extern "C" void verify_at_end_of_deoptimization();

char* Interpreter::_restart_primitiveValue		= NULL;
char* Interpreter::_redo_bytecode_after_deoptimization	= NULL;
char* Interpreter::_nlr_single_step_continuation_entry	= NULL;
Label Interpreter::_nlr_single_step_continuation = Label();

void InterpreterGenerator::generate_forStubRountines() {
  const int invocation_counter_inc = 0x10000;

 Interpreter::_restart_primitiveValue = masm->pc();
  masm->enter();
  masm->movl(ecx, Address(eax, blockClosureOopDesc::context_byte_offset()));
  masm->movl(edx, Address(eax, blockClosureOopDesc::method_or_entry_byte_offset()));
  masm->pushl(ecx);			// save recv (initialize with context)
  restore_ebx();			// if value... is called from compiled code
  masm->addl(Address(edx, methodOopDesc::counters_byte_offset()), invocation_counter_inc);
  masm->leal(esi, Address(edx, methodOopDesc::codes_byte_offset()));
  masm->movl(eax, ecx);			// initialize temp 1 with context
  masm->pushl(esi);			// initialize esi save
  load_ebx();
  jump_ebx();

 Interpreter::_redo_bytecode_after_deoptimization = masm->pc();

  // Call verify
  masm->call_C((char*)verify_at_end_of_deoptimization, relocInfo::runtime_call_type);

  // Redo the send
  restore_esi();
  restore_ebx();
  masm->movl(eax, Address((int)&redo_send_offset,   relocInfo::external_word_type));
  masm->subl(esi, eax);
  load_ebx();
  masm->popl(eax);		  // get top of stack
  jump_ebx();

 Interpreter::_nlr_single_step_continuation_entry = masm->pc();
  masm->bind(Interpreter::_nlr_single_step_continuation);
  masm->reset_last_Delta_frame();
  masm->jmp(_nlr_testpoint);
}

//-----------------------------------------------------------------------------------------
// Method invocation
//
// call_method calls a methodOop. The registers have
// to be set up as follows:
//
// eax: receiver
// ebx: 000000xx
// ecx: methodOop
// parameters on the stack

void InterpreterGenerator::call_method() {

  // trace_send code should come here - fix this
  if (TraceMessageSend) {
    masm->pushad();
    masm->pushl(ecx);	// pass arguments (C calling convention)
    masm->pushl(eax);
    masm->call_C((char*)trace_send, relocInfo::runtime_call_type);
    masm->addl(esp, oopSize*2);   // get rid of arguments
    masm->popad();
  }

  save_esi();
  masm->call(_method_entry);
  masm->ic_info(_nlr_testpoint, 0);
  restore_esi();
}

char* Interpreter::_last_native_called = NULL;		// debugging only - see comment in header file

void InterpreterGenerator::call_native(Register entry) {

  // trace_send code should come here - fix this
  if (TraceMessageSend) {
    masm->pushad();
    masm->pushl(eax);	// pass arguments (C calling convention)
    masm->pushl(ecx);
    masm->call_C((char*)trace_send, relocInfo::runtime_call_type);
    masm->addl(esp, oopSize*2);   // get rid of arguments
    masm->popad();
  }

  save_esi();
  masm->movl(Address(int(&Interpreter::_last_native_called), relocInfo::external_word_type), entry);
  masm->call(entry);
  masm->ic_info(_nlr_testpoint, 0);			// ordinary inline cache info
  restore_esi();
  restore_ebx();
}


extern "C" char* method_entry_point = NULL;		// for interpreter_asm.asm (remove if not used anymore)
extern "C" char* block_entry_point  = NULL;		// for interpreter_asm.asm (remove if not used anymore)
extern "C" char* active_stack_limit();                  // address of pointer to the current process' stack limit

extern "C" void check_stack_overflow();

void InterpreterGenerator::generate_method_entry_code() {
// This generates the code sequence called to activate methodOop execution.
// It is usually called via the call_method() macro, which saves the old
// instruction counter (esi) and provides the ic_info word for NLRs.

  const int counter_offset = methodOopDesc::counters_byte_offset();
  const int code_offset    = methodOopDesc::codes_byte_offset();

  assert(!_method_entry.is_bound(), "code has been generated before");
  Label start_setup, counter_overflow, start_execution, handle_counter_overflow, is_interpreted, 
    handle_stack_overflow, continue_from_stack_overflow;

  // eax: receiver
  // ebx: 000000xx
  // ecx: methodOop
  // parameters on the stack
  method_entry_point = masm->pc();
  masm->bind(_method_entry);
  masm->movl(edi, nil_addr());

  // eax: receiver
  // ebx: 000000xx
  // ecx: methodOop
  // edi: initial value for temp0
  // parameters on the stack
  masm->bind(start_setup);
  masm->enter();							// setup new stack frame
  masm->pushl(eax);							// install receiver
  masm->movl(edx, Address(ecx, counter_offset));			// get method invocation counter
  masm->leal(esi, Address(ecx, code_offset));				// set bytecode pointer to first instruction
  masm->addl(edx, 1 << methodOopDesc::_invocation_count_offset);	// increment invocation counter (only upper word)
  masm->pushl(esi);							// initialize esi stack location for profiler
  masm->movl(Address(ecx, counter_offset), edx);			// store method invocation counter
  load_ebx();								// get first byte code of method
  masm->cmpl(edx, 0xFFFF << methodOopDesc::_invocation_count_offset);	// make sure cmpl uses imm32 field
  Interpreter::_invocation_counter_addr = (int*)(masm->pc() - oopSize);// compute invocation counter address
  masm->jcc(Assembler::aboveEqual, counter_overflow);			// treat invocation counter overflow
  masm->bind(start_execution);						// continuation point after overflow
  masm->movl(eax, edi);						// initialize temp0
  masm->cmpl(esp, Address(int(active_stack_limit()), relocInfo::external_word_type));
  masm->jcc(Assembler::lessEqual, handle_stack_overflow);
  masm->bind(continue_from_stack_overflow);
  jump_ebx();								// start execution

  // invocation counter overflow
  masm->bind(counter_overflow);
  // not necessary to store esi since it has been just initialized
  masm->pushl(edi);							// move tos on stack (temp0, always here)
  masm->set_last_Delta_frame_before_call();
  masm->call(handle_counter_overflow);					// introduce extra frame to pass arguments
  masm->reset_last_Delta_frame();
  masm->popl(edi);							// restore edi, used to initialize eax
  // Should check here if recompilation created a nmethod for this
  // methodOop. If so, one should redo the send and thus start the
  // nmethod.
  // If an nmethod has been created, invocation_counter_overflow
  // returns the continuation pc, otherwise it returns NULL.
  // For now: simply continue with interpreted version.
  restore_esi();
  restore_ebx();
  load_ebx();
  masm->jmp(start_execution);

  // handle invocation counter overflow, use extra frame to pass arguments
  // eax: receiver
  // ecx: methodOop
  masm->bind(handle_counter_overflow);
  masm->pushl(ecx);							// pass methodOop argument
  masm->pushl(eax);							// pass receiver argument
  masm->call((char*)Recompilation::methodOop_invocation_counter_overflow, relocInfo::runtime_call_type); // methodOop_invocation_counter_overflow(receiver, methodOop)
  masm->addl(esp, 2*oopSize);						// discard arguments
  masm->ret(0);


// This generates the code sequence called to activate block execution.
// It is jumped to from one of the primitiveValue primitives. eax is
// expected to hold the receiver (i.e., the block closure).

  // eax: receiver (block closure)
  // primitiveValue parameters on the stack
  block_entry_point = masm->pc();
  masm->bind(_block_entry);
  masm->movl(ecx, Address(eax, blockClosureOopDesc::method_or_entry_byte_offset()));	// get methodOop/jump table entry out of closure
  masm->reset_last_Delta_frame();					// if called from the interpreter, the last Delta frame is setup
  masm->test(ecx, Mem_Tag);						// if methodOop then
  masm->jcc(Assembler::notZero, is_interpreted);			//   start methodOop execution
  masm->jmp(ecx);							// else jump to jump table entry

  masm->bind(is_interpreted);
  // eax: receiver (block closure)
  // ecx: block methodOop
  restore_ebx();							// if value... is called from compiled code, ebx may be not zero
  masm->movl(eax, Address(eax, blockClosureOopDesc::context_byte_offset()));		// get context out of closure
  masm->movl(edi, eax);						// initial value for temp0 is (incoming) context/value
  // eax: context (= receiver)
  // ebx: 00000000
  // ecx: block methodOop
  // edi: context (= initial value for temp0)
  // parameters on stack
  masm->jmp(start_setup);

  masm->bind(handle_stack_overflow);
  masm->pushl(eax);
  masm->call_C((char*)&check_stack_overflow, relocInfo::external_word_type);
  masm->popl(eax);
  restore_esi();
  restore_ebx();
  load_ebx();
  masm->jmp(continue_from_stack_overflow);
}


//-----------------------------------------------------------------------------------------
// Inline cache misses

void InterpreterGenerator::generate_inline_cache_miss_handler() {
  Label _normal_return;
  assert(!_inline_cache_miss.is_bound(), "code has been generated before");
  masm->bind(_inline_cache_miss);
  // We need an inline cache for NLR evaluation.
  // This can happen because the inline cache miss may call "doesNotUnderstand:"
  call_C((char*)InterpretedIC::inline_cache_miss);
  masm->testl(eax, eax);
  masm->jcc(Assembler::equal, _normal_return);
  masm->movl(ecx, Address(eax)); // pop arguments
  masm->movl(eax, Address(eax, oopSize)); // doesNotUnderstand: result
  masm->addl(esp, ecx); // pop arguments
  load_ebx();
  jump_ebx();
  masm->bind(_normal_return);
  load_ebx();
  masm->popl(eax);
  jump_ebx();
}


//-----------------------------------------------------------------------------------------
// smi predicted sends


void InterpreterGenerator::generate_predicted_smi_send_failure_handler() {
  assert(!_smi_send_failure.is_bound(), "code has been generated before");
  char* ep = normal_send(Bytecodes::interpreted_send_1, true, false);
  // Note: Has to jump to normal_send entry point because the entry point is
  //       not necessarily in the beginning of the normal send code pattern.
  masm->bind(_smi_send_failure);
  masm->pushl(edx);					// push receiver back on tos
  masm->jmp(ep, relocInfo::runtime_call_type);
}


void InterpreterGenerator::check_smi_tags() {
  // tos: receiver
  // eax: argument
  masm->popl(edx);					// get receiver
  masm->movl(ecx, eax);				// copy it to ecx
  masm->orl(ecx, edx);					// or tag bits
  masm->test(ecx, Mem_Tag);				// if one of them is set then
  masm->jcc(Assembler::notZero, _smi_send_failure);	// arguments are not bot smis
  // edx: receiver
  // eax: argument
}


char* InterpreterGenerator::smi_add() {
  Label overflow;
  char* ep = entry_point();
  check_smi_tags();
  masm->addl(eax, edx);
  masm->jcc(Assembler::overflow, overflow);
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  jump_ebx();

  masm->bind(overflow);
  // eax: argument + receiver
  // edx: receiver
  masm->subl(eax, edx);
  masm->jmp(_smi_send_failure);
  return ep;
}


char* InterpreterGenerator::smi_sub() {
  Label overflow;
  char* ep = entry_point();
  check_smi_tags();
  masm->subl(edx, eax);
  masm->jcc(Assembler::overflow, overflow);
  advance_aligned(1 + 2*oopSize);
  masm->movl(eax, edx);
  load_ebx();
  jump_ebx();

  masm->bind(overflow);
  // eax: argument
  // edx: receiver - argument
  masm->addl(edx, eax);
  masm->jmp(_smi_send_failure);
  return ep;
}


char* InterpreterGenerator::smi_mul() {
  Label overflow;
  char* ep = entry_point();
  check_smi_tags();
  masm->movl(ecx, eax);				// save argument for overflow case
  masm->sarl(edx, Tag_Size);
  masm->imull(eax, edx);
  masm->jcc(Assembler::overflow, overflow);
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  jump_ebx();

  masm->bind(overflow);
  // eax: argument * (receiver >> Tag_Size)
  // ecx: argument
  // edx: receiver >> Tag_Size
  masm->movl(eax, ecx);				// restore argument
  masm->shll(edx, Tag_Size);				// undo shift
  masm->jmp(_smi_send_failure);
  return ep;
}


char* InterpreterGenerator::smi_compare_op(Bytecodes::Code code) {
  Label is_true;
  char* ep = entry_point();
  check_smi_tags();
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  masm->cmpl(edx, eax);
  Assembler::Condition cc;
  switch (code) {
    case Bytecodes::smi_equal		: cc = Assembler::equal;	break;
    case Bytecodes::smi_not_equal	: cc = Assembler::notEqual;	break;
    case Bytecodes::smi_less		: cc = Assembler::less;		break;
    case Bytecodes::smi_less_equal	: cc = Assembler::lessEqual;	break;
    case Bytecodes::smi_greater		: cc = Assembler::greater;	break;
    case Bytecodes::smi_greater_equal	: cc = Assembler::greaterEqual;	break;
    default				: ShouldNotReachHere();
  }
  masm->jcc(cc, is_true);
  masm->movl(eax, false_addr());
  jump_ebx();

  masm->bind(is_true);
  masm->movl(eax, true_addr());
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::smi_logical_op(Bytecodes::Code code) {
  char* ep = entry_point();
  check_smi_tags();
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  switch (code) {
    case Bytecodes::smi_and: masm->andl(eax, edx); break;
    case Bytecodes::smi_or : masm->orl (eax, edx); break;
    case Bytecodes::smi_xor: masm->xorl(eax, edx); break;
    default                : ShouldNotReachHere();
  }
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::smi_shift() {
  // overflow is ignored for now (as in smi_prims.cpp)
  assert(Int_Tag == 0, "check this code");
  Label shift_right;

  char* ep = entry_point();
  check_smi_tags();
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  masm->sarl(eax, Tag_Size);				// convert argument (shift count) into int (sets zero flag)
  masm->movl(ecx, eax);				// move shift count into CL
  masm->jcc(Assembler::negative, shift_right);		// shift right or shift left?

  // shift left
  masm->shll(edx);					// else receiver << (argument mod 32)
  masm->movl(eax, edx);				// set result
  jump_ebx();

  // shift right
  masm->bind(shift_right);
  masm->negl(ecx);
  masm->sarl(edx);					// receiver >> (argument mod 32)
  masm->andl(edx, -1 << Tag_Size);			// clear tag bits
  masm->movl(eax, edx);				// set result
  jump_ebx();
  return ep;
}


//-----------------------------------------------------------------------------------------
// objArray predicted sends
//
// Problem: Implementation requires InterpretedICIterator to be adjusted: in case of
//          special predicted primitive methods, the (non-cached) receiver is not simply
//          a smi but can be an object array (or something else) depending on the primitive
//          that is predicted. Think about this.

char* InterpreterGenerator::objArray_size() {
  char* ep = entry_point();
  Unimplemented();
  return ep;
}


char* InterpreterGenerator::objArray_at() {
  char* ep = entry_point();
  Unimplemented();
  return ep;
}


char* InterpreterGenerator::objArray_at_put() {
  char* ep = entry_point();
  Unimplemented();
  return ep;
}


//-----------------------------------------------------------------------------------------
// Returns
//
// _return_tos pops the arguments and returns from a method or block.

void InterpreterGenerator::return_tos(Bytecodes::ArgumentSpec arg_spec) {
  masm->leave();
  switch (arg_spec) {
    case Bytecodes::recv_0_args: masm->ret(0*oopSize); break;
    case Bytecodes::recv_1_args: masm->ret(1*oopSize); break;
    case Bytecodes::recv_2_args: masm->ret(2*oopSize); break;
    case Bytecodes::recv_n_args: {
      // no. of arguments is in the next byte
      masm->movb(ebx, Address(esi, 1));			// get no. of arguments
      masm->popl(ecx);						// get return address
      masm->leal(esp, Address(esp, ebx, Address::times_4));	// adjust esp (remove arguments)
      masm->jmp(ecx);						// return
      break;
    }
    default: ShouldNotReachHere();
  }
}

//-----------------------------------------------------------------------------------------
// Zap context
//
// The context is zapped by storing 0 into the home field of the context.
// Only method contexts must be zapped.

void InterpreterGenerator::zap_context() {
  masm->pushl(eax); 			// make sure temp0 (context) is in memory
  masm->movl(ecx, context_addr());
  masm->popl(eax);
  masm->movl(Address(ecx, contextOopDesc::parent_byte_offset()), 0);
}

//-----------------------------------------------------------------------------------------
// Local returns
//

char* InterpreterGenerator::local_return(bool push_self, int nofArgs, bool zap) {
  char* ep = entry_point();
  if (zap) {
    zap_context();
  }
  if (push_self) {
    masm->movl(eax, self_addr());
  }

  // return bytecodes aren't sends, so they don't have any ArgumentSpec, yet
  // return_tos takes one as argument ... hence this weird device		-Marc 4/07
  Bytecodes::ArgumentSpec arg_spec;
  switch (nofArgs) {
    case  0: arg_spec = Bytecodes::recv_0_args; break;
    case  1: arg_spec = Bytecodes::recv_1_args; break;
    case  2: arg_spec = Bytecodes::recv_2_args; break;
    case -1: arg_spec = Bytecodes::recv_n_args; break;
    default: ShouldNotReachHere();
  }

  return_tos(arg_spec);
  return ep;
}

//-----------------------------------------------------------------------------------------
// Error handling
//
// Entry points for run-time errors. eax must contain the value it had
// before starting to execute the instruction that issued the error,
// esi must point to the next instruction.

extern "C" void suspend_on_error(InterpreterErrorConstants error_code);

char* Interpreter::_illegal  = NULL;

void InterpreterGenerator::generate_error_handler_code() {
  assert(!_boolean_expected.is_bound(), "code has been generated before");

  Label suspend, call_suspend;
  // eax: top of expression stack
  // ecx: error code
  // esi: points to next instruction
 masm->bind(suspend);
  masm->pushl(eax);				// save tos
  call_C(call_suspend);
  should_not_reach_here();

 masm->bind(call_suspend);			// extra stack frame to pass error code in C land
  masm->pushl(ecx);				// pass error code
  masm->call((char*)suspend_on_error, relocInfo::runtime_call_type);
  should_not_reach_here();

 masm->bind(_boolean_expected);
  masm->movl(ecx, boolean_expected);
  masm->jmp(suspend);

 masm->bind(_float_expected);
  masm->movl(ecx, float_expected);
  masm->jmp(suspend);

 masm->bind(_NLR_to_dead_frame);
  masm->movl(ecx, nonlocal_return_error);
  masm->jmp(suspend);

 masm->bind(_halted);
  masm->movl(ecx, halted);
  masm->jmp(suspend);

 masm->bind(_stack_missaligned);
  masm->movl(ecx, stack_missaligned);
  masm->jmp(suspend);

 masm->bind(_ebx_wrong);
  masm->movl(ecx, ebx_wrong);
  masm->jmp(suspend);

 masm->bind(_obj_wrong);
  masm->movl(ecx, obj_wrong);
  masm->jmp(suspend);

 masm->bind(_last_Delta_fp_wrong);
  masm->movl(ecx, last_Delta_fp_wrong);
  masm->jmp(suspend);

 masm->bind(_primitive_result_wrong);
  masm->movl(ecx, primitive_result_wrong);
  masm->jmp(suspend);

 _illegal = masm->pc();
  masm->movl(ecx,   illegal_code);
  masm->jmp(suspend);

  Interpreter::_illegal = _illegal;
}


//-----------------------------------------------------------------------------------------
// Non-local returns
//
// nonlocal_return does a non-local return starting in the current activation frame
// and returning through stack frames until it reaches the method activation frame
// that corresponds to the home stack frame of the (heap) context of the current
// activation. The heap contexts of all method frames (that have a heap context)
// encountered inbetween are zapped (incl. the context of the home stack frame).
// After determining the home stack frame via a loop, the NLR sequence is issued
// as in compiled code.
//
// Note: Non-local returns that start in an interpreted block must end in an interpreted
// method; i.e. even if the NLR goes through compiled (C) frames, it will eventually end
// in an interpreter frame. Furthermore, if a NLR is possible, there is always a chain of
// contexts that eventually will end with the context allocated for the NLR target method
// (since NLR require contexts to be there always).
//
//
// Registers used in compiled code to do NLRs (defined in Location.hpp):
//
// Note: When doing NLRs in compiled code, the nlr_home_id is used to identify the home
// scope in case of inlined methods. When doing NLR starting in interpreted code and
// therefore ending in interpreted code, this register is used to pass the the number of
// arguments to pop because this information is needed to correctly finish the NLR. In
// order to be able to distinguish between compiled and interpreted NLRs, this value is
// made negative (compiled NLR home ids are always >= 0).

extern "C" char* nlr_testpoint_entry = NULL;	// for interpreter_asm.asm (remove if not used anymore)
extern "C" contextOop nlr_home_context;

void InterpreterGenerator::generate_nonlocal_return_code() {
  assert(eax == NLR_result_reg, "NLR register use changed");
  assert(edi == NLR_home_reg  , "NLR register use changed");
  assert(esi == NLR_homeId_reg, "NLR register use changed");

  assert(!_issue_NLR.is_bound()    , "code has been generated before");
  assert(!_nlr_testpoint.is_bound(), "code has been generated before");

  Label zapped_context, loop, no_zapping, compiled_code_NLR;

  // context already zapped
  masm->bind(zapped_context);
  masm->popl(eax);				// get NLR result back
  masm->addl(esi, 2);				// adjust esi (must point to next instruction)
  masm->jmp(_NLR_to_dead_frame);

  masm->bind(_issue_NLR);
  masm->pushl(eax);				// make sure context (temp0) is in memory
  masm->movl(edi, context_addr());		// get context
  if (_debug) {
    // should check here if edx is really a context
  }

  // find home stack frame by following the context chain
  // edi: current context in chain
  masm->bind(loop);				// repeat
  masm->movl(eax, edi);			//   eax := last context used
  masm->movl(edi, Address(edi, contextOopDesc::parent_byte_offset()));
  masm->test(edi, Mem_Tag);			//   edi := edi.home
  masm->jcc(Assembler::notZero, loop);		// until is_smi(edi)
  masm->testl(edi, edi);			// if edi = 0 then
  masm->jcc(Assembler::zero, zapped_context);	//   context has been zapped
  masm->movl(Address(int(&nlr_home_context), relocInfo::external_word_type), eax);
                                                // else save the context containing the home (edi points to home stack frame)
  masm->movb(ebx, Address(esi, 1));		// get no. of arguments to pop
  masm->popl(eax);				// get NLR result back
  masm->movl(esi, ebx);			// keep no. of arguments in esi
  masm->notl(esi);				// make negative to distinguish from compiled NLRs

  // entry point for all methods to do NLR test & continuation,
  // first check if context zap is necessary
  //
  // eax: NLR result
  // edi: NLR home
  // esi: no. of arguments to pop (1s complement)

  masm->bind(_C_nlr_testpoint);
  masm->reset_last_Delta_frame();
  nlr_testpoint_entry = masm->pc();
  masm->bind(_nlr_testpoint);

  // check if esi is indeed negative, otherwise this would be a NLR with
  // target in compiled code, which would be a bug - leave it in here to
  // find deoptimization bugs (request from Lars)
  //masm->testl(esi, esi);
  //masm->jcc(Assembler::positive, compiled_code_NLR);

  masm->movl(ecx, context_addr());		// get potential context
  masm->test(ecx, Mem_Tag);			// if is_smi(ecx) then
  masm->jcc(Assembler::zero, no_zapping);	//   can't be a context pointer
  masm->movl(edx, Address(ecx, memOopDesc::klass_byte_offset()));	// else isOop: get its class
  masm->cmpl(edx, contextKlass_addr());	// if class # contextKlass then
  masm->jcc(Assembler::notEqual, no_zapping);	//   is not a context
  masm->movl(ebx, Address(ecx, contextOopDesc::parent_byte_offset()));	// else is context: get home
  masm->cmpl(ebx, ebp);			// if home # ebp then
  masm->jcc(Assembler::notEqual, no_zapping);	//   is not a methoc context
  masm->movl(Address(ecx, contextOopDesc::parent_byte_offset()), 0);	// else method context: zap home

  masm->bind(no_zapping);
  masm->cmpl(edi, ebp);
  masm->jcc(Assembler::notEqual, StubRoutines::continue_NLR_entry());

  // home found
  // eax: NLR result
  // edi: NLR home
  // esi: no. of arguments to pop (1s complement)
  restore_ebx();				// make sure ebx = 0
  masm->leave();				// remove stack frame
  masm->popl(ecx);				// get return address
  masm->notl(esi);				// make positive again
  masm->leal(esp, Address(esp, esi, Address::times_4));	// pop arguments
  masm->jmp(ecx);				// return

  // error handler for compiled code NLRs - can be removed as soon
  // as that test has been removed. For now just use magic imm32 to
  // indicate this problem.
  //masm->bind(compiled_code_NLR);
  //masm->hlt();
  //masm->testl(eax, 0x0badcafe);
  //masm->testl(eax, 0x0badcafe);
  //masm->testl(eax, 0x0badcafe);
  //masm->testl(eax, 0x0badcafe);
}


char* InterpreterGenerator::nonlocal_return_tos() {
  char* ep = entry_point();
  masm->jmp(_issue_NLR);
  return ep;
}


char* InterpreterGenerator::nonlocal_return_self() {
  char* ep = entry_point();
  masm->pushl(eax);
  masm->movl(eax, self_addr());
  masm->jmp(_issue_NLR);
  return ep;
}

//-----------------------------------------------------------------------------------------
// Normal sends, access methods
//
// HCode structure:
//
// 	[send		]	(1 byte)
// 	[method		]	(1 dword)
//	[icache (class)	]	(1 dword)
//	...	  	  <---	esi
//
// Access methods cannot exist for the smi class which simplifies
// the inline cache test: if the receiver is a smi it is always
// a cache miss because the cache never caches the smi class.
//
// Note: Currently _load_recv is used to get the receiver. This
// is suboptimal since the receiver is also pushed on the stack
// and has to be popped again at the end (except for self sends).
// Should change this at some point (optimization).
//

char* InterpreterGenerator::access_send(bool self) {
  char* ep = entry_point();

  Bytecodes::ArgumentSpec arg_spec;
  Address method_addr = Address(esi, -2*oopSize);
  Address klass_addr  = Address(esi, -1*oopSize);

  if (self) {
    arg_spec = Bytecodes::args_only;
  } else {
    arg_spec = Bytecodes::recv_0_args;
  }

  load_recv(arg_spec);
  advance_aligned(1 + 2*oopSize);

  // mov ecx, [method]		; get method
  // lea edx, [ecx._hcodes(4)]	; start address of hcode + 4
  // and edx, NOT 3			; align
  // mov edx, [edx]
  // mov eax, [eax + edx - Mem_Tag]	; load instVar at offset

  masm->test(eax, Mem_Tag);				// check if smi
  masm->movl(ecx, method_addr);				// get cached method (assuming infrequent cache misses)
  masm->movl(edx, klass_addr);				// get cached klass
  masm->jcc(Assembler::zero, _inline_cache_miss);	// if smi then it's a cache miss
  masm->movl(edi, Address(eax, memOopDesc::klass_byte_offset()));	// get recv class

  // eax: receiver
  // ebx: 000000xx (load_recv may modify bl)
  // ecx: cached methodOop
  // edx: cached klass
  // edi: receiver klass
  // esi: next instruction
  masm->cmpl(edx, edi);			// compare with inline cache
  masm->jcc(Assembler::notEqual, _inline_cache_miss);

  Address primitive_addr = Address(ecx, methodOopDesc::codes_byte_offset() + oopSize);
  masm->movl(edx, primitive_addr);		// get instVar offset
  masm->movl(eax, field_addr(eax, edx));

  if (!self) {
    masm->popl(ecx);				//    receiver still on stack: remove it
  }
  load_ebx();
  jump_ebx();

  return ep;
}


//-----------------------------------------------------------------------------------------
// Inline cache structure for non-polymorphic sends
//
// [send byte code ]      1 byte
// [no. of args    ]      1 byte, only if arg_spec == recv_n_args
// alignment .......      0..3 bytes
// [selector/method]      1 word
// [0/class        ]      1 word
// next instr ...... <--- esi, after advance
//
// normal_send generates the code for normal sends that can
// deal with either methodOop or nmethod entries, or both.
//
// Note: As of now (7/30/96) the method sweeper is running asynchronously and might modify
//       (cleanup) the inline cache while a send is in progress. Thus, the inline cache might
//       have changed in the meantime which may cause problems. Right now we try to minimize
//       the chance for this to happen by loading the cached method as soon as possible, thereby
//       reducing the time frame for the sweeper (gri).

char* InterpreterGenerator::normal_send(Bytecodes::Code code, bool allow_methodOop, bool allow_nmethod, bool primitive_send) {
  assert(allow_methodOop || allow_nmethod || primitive_send, "must allow at least one method representation");

  Label is_smi, compare_class, is_methodOop, is_nmethod;

  Bytecodes::ArgumentSpec arg_spec = Bytecodes::argument_spec(code);
  bool pop_tos = Bytecodes::pop_tos(code);

  // inline cache layout
  int     length      = (arg_spec == Bytecodes::recv_n_args ? 2 : 1) + 2*oopSize;
  Address method_addr = Address(esi, -2*oopSize);
  Address klass_addr  = Address(esi, -1*oopSize);

  masm->bind(is_smi);				// smi case (assumed to be infrequent)
  masm->movl(edi, smiKlass_addr());		// load smi klass
  masm->jmp(compare_class);

  char* ep = entry_point();
  load_recv(arg_spec);
  advance_aligned(length);
  masm->test(eax, Mem_Tag);			// check if smi
  masm->movl(ecx, method_addr);			// get cached method (assuming infrequent cache misses)
  masm->movl(edx, klass_addr);			// get cached klass
  masm->jcc(Assembler::zero, is_smi);
  masm->movl(edi, Address(eax, memOopDesc::klass_byte_offset()));	// get recv class

  masm->bind(compare_class);
  // eax: receiver
  // ebx: 000000xx (load_recv may modify bl)
  // ecx: cached selector/methodOop/nmethod
  // edx: cached klass
  // edi: receiver klass
  // esi: next instruction
  masm->cmpl(edx, edi);			// compare with inline cache
  masm->jcc(Assembler::notEqual, _inline_cache_miss);

  if (allow_methodOop && allow_nmethod) {
    // make case distinction at run-time
    masm->test(ecx, Mem_Tag);			// check if nmethod
    masm->jcc(Assembler::zero, is_nmethod);	// nmethods (jump table entries) are 4-byte alligned
  }

  if (allow_methodOop) {
    masm->bind(is_methodOop);
    call_method();
    if (arg_spec != Bytecodes::args_only) masm->popl(ecx);// discard receiver if on stack
    load_ebx();
    if (pop_tos) masm->popl(eax);		// discard result if not used
    jump_ebx();
  }

  if (allow_nmethod) {
    masm->bind(is_nmethod);
    call_native(ecx);
    if (arg_spec != Bytecodes::args_only) masm->popl(ecx);// discard receiver if on stack
    load_ebx();
    if (pop_tos) masm->popl(eax);		// discard result if not used
    jump_ebx();
  }

  if (primitive_send) {
    Label _failed;
    Address primitive_addr = Address(ecx, methodOopDesc::codes_byte_offset() + oopSize);

    masm->movl(edx, primitive_addr);		// get primitive address
    call_C(edx);				// eax := primitive call
    masm->test(eax, Mark_Tag_Bit);
    masm->jcc(Assembler::notZero, _failed);
    load_ebx();
    if (pop_tos) masm->popl(eax);		// discard result if not used
    jump_ebx();
    masm->bind(_failed);
    //_print 'predicted primitive failed - not yet implemented', 0, 0
    masm->hlt();
  }

  return ep;
}


char* InterpreterGenerator::primitive_send(Bytecodes::Code code) {
  return normal_send(code, false, false, true);
}

char* InterpreterGenerator::interpreted_send(Bytecodes::Code code) {
  return normal_send(code, true, false);
}


char* InterpreterGenerator::compiled_send(Bytecodes::Code code) {
  return normal_send(code, false, true);
}


char* InterpreterGenerator::megamorphic_send(Bytecodes::Code code) {
  // Handle super sends conventionally - most probably infrequent anyway
  if (Bytecodes::is_super_send(code)) return normal_send(code, true, true);

  Label is_smi, probe_primary_cache, is_methodOop, is_nmethod, probe_secondary_cache;
  Bytecodes::ArgumentSpec arg_spec = Bytecodes::argument_spec(code);

  // inline cache layout
  int     length        = (arg_spec == Bytecodes::recv_n_args ? 2 : 1) + 2*oopSize;
  bool    pop_tos       = Bytecodes::pop_tos(code);
  Address selector_addr = Address(esi, -2*oopSize);
  Address klass_addr    = Address(esi, -1*oopSize);

  masm->bind(is_smi);				// smi case (assumed to be infrequent)
  masm->movl(ecx, smiKlass_addr());		// load smi klass
  masm->jmp(probe_primary_cache);

  char* ep = entry_point();

  load_recv(arg_spec);
  advance_aligned(length);
  masm->test(eax, Mem_Tag);			// check if smi
  masm->jcc(Assembler::zero, is_smi);		// otherwise
  masm->movl(ecx, Address(eax, memOopDesc::klass_byte_offset()));	// get recv class

  // probe primary cache
  //
  // eax: receiver
  // ebx: 000000xx
  // ecx: receiver klass
  // esi: next instruction
  masm->bind(probe_primary_cache);		// compute hash value
  masm->movl(edx, selector_addr);		// get selector
  // compute hash value
  masm->movl(edi, ecx);
  masm->xorl(edi, edx);
  masm->andl(edi, (primary_cache_size - 1) << 4);
  // probe cache
  masm->cmpl(ecx, Address(edi, lookupCache::primary_cache_address() + 0*oopSize));
  masm->jcc(Assembler::notEqual, probe_secondary_cache);
  masm->cmpl(edx, Address(edi, lookupCache::primary_cache_address() + 1*oopSize));
  masm->jcc(Assembler::notEqual, probe_secondary_cache);
  masm->movl(ecx, Address(edi, lookupCache::primary_cache_address() + 2*oopSize));
  masm->test(ecx, Mem_Tag);			// check if nmethod
  masm->jcc(Assembler::zero, is_nmethod);	// nmethods (jump table entries) are 4-byte aligned

  masm->bind(is_methodOop);
  call_method();
  if (arg_spec != Bytecodes::args_only) masm->popl(ecx);// discard receiver if on stack
  load_ebx();
  if (pop_tos) masm->popl(eax);		// discard result if not used
  jump_ebx();

  masm->bind(is_nmethod);
  call_native(ecx);
  if (arg_spec != Bytecodes::args_only) masm->popl(ecx);// discard receiver if on stack
  load_ebx();
  if (pop_tos) masm->popl(eax);		// discard result if not used
  jump_ebx();

  // probe secondary cache
  //
  // eax: receiver
  // ebx: 000000xx
  // ecx: receiver klass
  // edx: selector
  // edi: primary cache index
  // esi: next instruction
  masm->bind(probe_secondary_cache);
  masm->andl(edi, (secondary_cache_size - 1) << 4);
  // probe cache
  masm->cmpl(ecx, Address(edi, lookupCache::secondary_cache_address() + 0*oopSize));
  masm->jcc(Assembler::notEqual, _inline_cache_miss);
  masm->cmpl(edx, Address(edi, lookupCache::secondary_cache_address() + 1*oopSize));
  masm->jcc(Assembler::notEqual, _inline_cache_miss);
  masm->movl(ecx, Address(edi, lookupCache::secondary_cache_address() + 2*oopSize));
  masm->test(ecx, Mem_Tag);			// check if nmethod
  masm->jcc(Assembler::zero, is_nmethod);	// nmethods (jump table entries) are 4-byte aligned
  masm->jmp(is_methodOop);

  return ep;
}

//-----------------------------------------------------------------------------------------
// Inline cache structure for polymorphic sends
//
// [send byte code ]      1 byte
// [no. of args    ]      1 byte, only if arg_spec == recv_n_args
// alignment .......      0..3 bytes
// [selector       ]      1 word
// [pic            ]      1 word
// next instr ...... <--- esi, after advance
//
// normal_send generates the code for normal sends that can
// deal with either methodOop or nmethod entries, or both.

char* InterpreterGenerator::polymorphic_send(Bytecodes::Code code) {
  Label loop, found, is_nmethod;

  Bytecodes::ArgumentSpec arg_spec = Bytecodes::argument_spec(code);
  bool pop_tos = Bytecodes::pop_tos(code);

  // inline cache layout
  int     length        = (arg_spec == Bytecodes::recv_n_args ? 2 : 1) + 2*oopSize;
  Address selector_addr = Address(esi, -2*oopSize);
  Address pic_addr      = Address(esi, -1*oopSize);

  // pic layout
  int length_offset = 2*oopSize - Mem_Tag;	// these constants should be mapped to the objectArrayOop definition
  int data_offset   = 3*oopSize - Mem_Tag;	// these constants should be mapped to the objectArrayOop definition

  char* ep = entry_point();
  load_recv(arg_spec);
  advance_aligned(length);
  masm->movl(ebx, pic_addr);			// get pic
  masm->movl(ecx, Address(ebx, length_offset));// get pic length (smi)
  masm->sarl(ecx, Tag_Size + 1);		// get pic length (int)
  // verifyPIC here

  masm->movl(edx, smiKlass_addr());		// preload smi klass
  masm->testl(eax, Mem_Tag);			// check if smi
  masm->jcc(Assembler::zero, loop);		// otherwise
  masm->movl(edx, Address(eax, memOopDesc::klass_byte_offset()));	// get receiver klass

  // search pic for appropriate entry
  masm->bind(loop);
  // eax: receiver
  // ebx: pic (objArrayOop)
  // ecx: counter
  // edx: receiver class
  // esi: next instruction
  masm->cmpl(edx, Address(ebx, ecx, Address::times_8, data_offset - 1*oopSize, relocInfo::none));
  masm->jcc(Assembler::equal, found);
  masm->decl(ecx);
  masm->jcc(Assembler::notZero, loop);

  // cache miss
  masm->jmp(_inline_cache_miss);

  masm->bind(found);
  // eax: receiver
  // ebx: pic (objArrayOop)
  // ecx: counter (> 0)
  // edx: receiver class
  // esi: next instruction
  masm->movl(ecx, Address(ebx, ecx, Address::times_8, data_offset - 2*oopSize, relocInfo::none));
  masm->testl(ecx, Mem_Tag);
  masm->jcc(Assembler::zero, is_nmethod);
  restore_ebx();

  // methodOop found
  call_method();
  if (arg_spec != Bytecodes::args_only) masm->popl(ecx);	// discard receiver if on stack
  load_ebx();
  if (pop_tos) masm->popl(eax);				// discard result if not used
  jump_ebx();

  // nmethod found
  masm->bind(is_nmethod);
  call_native(ecx);
  if (arg_spec != Bytecodes::args_only) masm->popl(ecx);	// discard receiver if on stack
  load_ebx();
  if (pop_tos) masm->popl(eax);				// discard result if not used
  jump_ebx();

  return ep;
}


//-----------------------------------------------------------------------------------------
// Miscellaneous

char* InterpreterGenerator::special_primitive_send_hint() {
  char* ep = entry_point();
  assert(Bytecodes::format(Bytecodes::special_primitive_send_1_hint) == Bytecodes::BB, "unexpected format");
  masm->addl(esi, 2);				// simply skip this instruction
  load_ebx();
  jump_ebx();
  return ep;
}

char* InterpreterGenerator::compare(bool equal) {
  Assembler::Condition cond = equal ? Assembler::equal : Assembler::notEqual;

  Label _return_true;

 char* ep = entry_point();
  next_ebx();
  masm->popl(edx);			// get receiver
  masm->cmpl(eax, edx);			// compare with argument
  masm->jcc(cond, _return_true);

  masm->movl(eax, false_addr());
  jump_ebx();

 masm->bind(_return_true);
  masm->movl(eax, true_addr());
  jump_ebx();

  return ep;
}

char* InterpreterGenerator::halt() {
  char* ep = entry_point();
  masm->incl(esi);				// advance to next instruction
  masm->jmp(_halted);
  return ep;
}

//-----------------------------------------------------------------------------------------
// Instruction generator

char* InterpreterGenerator::generate_instruction(Bytecodes::Code code) {
  // constants for readability
  const bool pop		= true;
  const bool returns_float	= true;
  const bool push		= false;
  const bool store_pop		= true;
  const int  n			= -1;

  switch (code) {
    // temporaries
    case Bytecodes::push_temp_0				: return push_temp(0);
    case Bytecodes::push_temp_1				: return push_temp(1);
    case Bytecodes::push_temp_2				: return push_temp(2);
    case Bytecodes::push_temp_3				: return push_temp(3);
    case Bytecodes::push_temp_4				: return push_temp(4);
    case Bytecodes::push_temp_5				: return push_temp(5);
    case Bytecodes::push_temp_n				: return push_temp_n();

    case Bytecodes::store_temp_0_pop			: return store_temp(0, pop);
    case Bytecodes::store_temp_1_pop			: return store_temp(1, pop);
    case Bytecodes::store_temp_2_pop			: return store_temp(2, pop);
    case Bytecodes::store_temp_3_pop			: return store_temp(3, pop);
    case Bytecodes::store_temp_4_pop			: return store_temp(4, pop);
    case Bytecodes::store_temp_5_pop			: return store_temp(5, pop);
    case Bytecodes::store_temp_n_pop			: return store_temp_n(pop);
    case Bytecodes::store_temp_n			: return store_temp_n();

    // arguments
    case Bytecodes::push_arg_1				: return push_arg(1);
    case Bytecodes::push_arg_2				: return push_arg(2);
    case Bytecodes::push_arg_3				: return push_arg(3);
    case Bytecodes::push_arg_n				: return push_arg_n();

    // space allocation
    case Bytecodes::allocate_temp_1			: return allocate_temps(1);
    case Bytecodes::allocate_temp_2			: return allocate_temps(2);
    case Bytecodes::allocate_temp_3			: return allocate_temps(3);
    case Bytecodes::allocate_temp_n			: return allocate_temps_n();

    // literals / expressions
    case Bytecodes::push_neg_n				: return push_smi(true);
    case Bytecodes::push_succ_n				: return push_smi(false);
    case Bytecodes::push_literal			: return push_literal();
    case Bytecodes::push_tos				: return push_tos();
    case Bytecodes::push_self				: return push_self();
    case Bytecodes::push_nil				: return push_const(nil_addr());
    case Bytecodes::push_true				: return push_const(true_addr());
    case Bytecodes::push_false				: return push_const(false_addr());
    case Bytecodes::only_pop				: return only_pop();

    // instance variables
    case Bytecodes::return_instVar			: return return_instVar();
    case Bytecodes::push_instVar			: return push_instVar();
    case Bytecodes::store_instVar_pop			: return store_instVar(pop);
    case Bytecodes::store_instVar			: return store_instVar();

    // context temporaries
    case Bytecodes::push_temp_0_context_0		: return with_context_temp(push, 0, 0);
    case Bytecodes::push_temp_1_context_0		: return with_context_temp(push, 1, 0);
    case Bytecodes::push_temp_2_context_0		: return with_context_temp(push, 2, 0);
    case Bytecodes::push_temp_n_context_0		: return with_context_temp(push, n, 0);

    case Bytecodes::push_temp_0_context_1		: return with_context_temp(push, 0, 1);
    case Bytecodes::push_temp_1_context_1		: return with_context_temp(push, 1, 1);
    case Bytecodes::push_temp_2_context_1		: return with_context_temp(push, 2, 1);
    case Bytecodes::push_temp_n_context_1		: return with_context_temp(push, n, 1);

    case Bytecodes::push_temp_0_context_n		: return with_context_temp(push, 0, n);
    case Bytecodes::push_temp_1_context_n		: return with_context_temp(push, 1, n);
    case Bytecodes::push_temp_2_context_n		: return with_context_temp(push, 2, n);
    case Bytecodes::push_temp_n_context_n		: return with_context_temp(push, n, n);

    case Bytecodes::store_temp_0_context_0_pop		: return with_context_temp(store_pop, 0, 0);
    case Bytecodes::store_temp_1_context_0_pop		: return with_context_temp(store_pop, 1, 0);
    case Bytecodes::store_temp_2_context_0_pop		: return with_context_temp(store_pop, 2, 0);
    case Bytecodes::store_temp_n_context_0_pop		: return with_context_temp(store_pop, n, 0);

    case Bytecodes::store_temp_0_context_1_pop		: return with_context_temp(store_pop, 0, 1);
    case Bytecodes::store_temp_1_context_1_pop		: return with_context_temp(store_pop, 1, 1);
    case Bytecodes::store_temp_2_context_1_pop		: return with_context_temp(store_pop, 2, 1);
    case Bytecodes::store_temp_n_context_1_pop		: return with_context_temp(store_pop, n, 1);

    case Bytecodes::store_temp_0_context_n_pop		: return with_context_temp(store_pop, 0, n);
    case Bytecodes::store_temp_1_context_n_pop		: return with_context_temp(store_pop, 1, n);
    case Bytecodes::store_temp_2_context_n_pop		: return with_context_temp(store_pop, 2, n);
    case Bytecodes::store_temp_n_context_n_pop		: return with_context_temp(store_pop, n, n);

    case Bytecodes::copy_1_into_context			: return copy_params_into_context(false, 1);
    case Bytecodes::copy_2_into_context			: return copy_params_into_context(false, 2);
    case Bytecodes::copy_n_into_context			: return copy_params_into_context(false, n);
    case Bytecodes::copy_self_into_context		: return copy_params_into_context(true, 0);
    case Bytecodes::copy_self_1_into_context		: return copy_params_into_context(true, 1);
    case Bytecodes::copy_self_2_into_context		: return copy_params_into_context(true, 2);
    case Bytecodes::copy_self_n_into_context		: return copy_params_into_context(true, n);

    // self/context initialization
    case Bytecodes::set_self_via_context		: return set_self_via_context();

    // block closure allocation
    case Bytecodes::push_new_closure_context_0		: return push_closure(0, true);
    case Bytecodes::push_new_closure_context_1		: return push_closure(1, true);
    case Bytecodes::push_new_closure_context_2		: return push_closure(2, true);
    case Bytecodes::push_new_closure_context_n		: return push_closure(n, true);

    case Bytecodes::push_new_closure_tos_0		: return push_closure(0, false);
    case Bytecodes::push_new_closure_tos_1		: return push_closure(1, false);
    case Bytecodes::push_new_closure_tos_2		: return push_closure(2, false);
    case Bytecodes::push_new_closure_tos_n		: return push_closure(n, false);

    // context allocation
    case Bytecodes::install_new_context_method_0	: return install_context(0, true);
    case Bytecodes::install_new_context_method_1	: return install_context(1, true);
    case Bytecodes::install_new_context_method_2	: return install_context(2, true);
    case Bytecodes::install_new_context_method_n	: return install_context(n, true);

    case Bytecodes::install_new_context_block_1		: return install_context(1, false);
    case Bytecodes::install_new_context_block_2		: return install_context(2, false);
    case Bytecodes::install_new_context_block_n		: return install_context(n, false);

    // primitive calls
    case Bytecodes::prim_call				: // fall through
    case Bytecodes::prim_call_self			: return call_primitive();
    case Bytecodes::prim_call_failure			: // fall through
    case Bytecodes::prim_call_self_failure		: return call_primitive_can_fail();

    case Bytecodes::prim_call_lookup			: // fall through
    case Bytecodes::prim_call_self_lookup		: // fall through
    case Bytecodes::prim_call_failure_lookup		: // fall through
    case Bytecodes::prim_call_self_failure_lookup	: return lookup_primitive();

    case Bytecodes::dll_call_sync			: return call_DLL(false);
    case Bytecodes::dll_call_async			: return call_DLL(true);

    case Bytecodes::predict_prim_call			: // fall through
    case Bytecodes::predict_prim_call_lookup		: // fall through
    case Bytecodes::predict_prim_call_failure		: // fall through
    case Bytecodes::predict_prim_call_failure_lookup	: return predict_prim(true);

    // control flow
    case Bytecodes::ifTrue_byte			  	: // fall through
    case Bytecodes::ifFalse_byte			: // fall through
    case Bytecodes::and_byte				: // fall through
    case Bytecodes::or_byte				: // fall through
    case Bytecodes::ifTrue_word				: // fall through
    case Bytecodes::ifFalse_word			: // fall through
    case Bytecodes::and_word				: // fall through
    case Bytecodes::or_word				: return control_cond(code);

    case Bytecodes::whileTrue_byte			: // fall through
    case Bytecodes::whileFalse_byte			: // fall through
    case Bytecodes::whileTrue_word			: // fall through
    case Bytecodes::whileFalse_word			: return control_while(code);

    case Bytecodes::jump_else_byte			: // fall through
    case Bytecodes::jump_loop_byte			: // fall through
    case Bytecodes::jump_else_word			: // fall through
    case Bytecodes::jump_loop_word			: return control_jump(code);

    // floating-point operations
    case Bytecodes::float_allocate			: return float_allocate();
    case Bytecodes::float_floatify_pop			: return float_floatify();
    case Bytecodes::float_move				: return float_move();
    case Bytecodes::float_set				: return float_set();
    case Bytecodes::float_nullary_op			: return float_op(0, returns_float);
    case Bytecodes::float_unary_op			: return float_op(1, returns_float);
    case Bytecodes::float_binary_op			: return float_op(2, returns_float);
    case Bytecodes::float_unary_op_to_oop		: return float_op(1);
    case Bytecodes::float_binary_op_to_oop		: return float_op(2);

    // accessor sends
    case Bytecodes::access_send_0			: return access_send(false);
    case Bytecodes::access_send_self			: return access_send(true);

    // primitive sends
    case Bytecodes::primitive_send_0			: // fall through
    case Bytecodes::primitive_send_1			: // fall through
    case Bytecodes::primitive_send_2			: // fall through
    case Bytecodes::primitive_send_n			: // fall through

    case Bytecodes::primitive_send_0_pop		: // fall through
    case Bytecodes::primitive_send_1_pop		: // fall through
    case Bytecodes::primitive_send_2_pop		: // fall through
    case Bytecodes::primitive_send_n_pop		: // fall through

    case Bytecodes::primitive_send_self			: // fall through
    case Bytecodes::primitive_send_self_pop		: // fall through

    case Bytecodes::primitive_send_super		: // fall through
    case Bytecodes::primitive_send_super_pop		: return primitive_send(code);

    // interpreted sends
    case Bytecodes::interpreted_send_0			: // fall through
    case Bytecodes::interpreted_send_1			: // fall through
    case Bytecodes::interpreted_send_2			: // fall through
    case Bytecodes::interpreted_send_n			: // fall through

    case Bytecodes::interpreted_send_0_pop		: // fall through
    case Bytecodes::interpreted_send_1_pop		: // fall through
    case Bytecodes::interpreted_send_2_pop		: // fall through
    case Bytecodes::interpreted_send_n_pop		: // fall through

    case Bytecodes::interpreted_send_self		: // fall through
    case Bytecodes::interpreted_send_self_pop		: // fall through

    case Bytecodes::interpreted_send_super		: // fall through
    case Bytecodes::interpreted_send_super_pop		: return interpreted_send(code);

    // compiled sends
    case Bytecodes::compiled_send_0			: // fall through
    case Bytecodes::compiled_send_1			: // fall through
    case Bytecodes::compiled_send_2			: // fall through
    case Bytecodes::compiled_send_n			: // fall through

    case Bytecodes::compiled_send_0_pop			: // fall through
    case Bytecodes::compiled_send_1_pop			: // fall through
    case Bytecodes::compiled_send_2_pop			: // fall through
    case Bytecodes::compiled_send_n_pop			: // fall through

    case Bytecodes::compiled_send_self			: // fall through
    case Bytecodes::compiled_send_self_pop		: // fall through

    case Bytecodes::compiled_send_super			: // fall through
    case Bytecodes::compiled_send_super_pop		: return compiled_send(code);

    // polymorphic sends
    case Bytecodes::polymorphic_send_0			: // fall through
    case Bytecodes::polymorphic_send_1			: // fall through
    case Bytecodes::polymorphic_send_2			: // fall through
    case Bytecodes::polymorphic_send_n			: // fall through

    case Bytecodes::polymorphic_send_0_pop		: // fall through
    case Bytecodes::polymorphic_send_1_pop		: // fall through
    case Bytecodes::polymorphic_send_2_pop		: // fall through
    case Bytecodes::polymorphic_send_n_pop		: // fall through

    case Bytecodes::polymorphic_send_self		: // fall through
    case Bytecodes::polymorphic_send_self_pop		: // fall through

    case Bytecodes::polymorphic_send_super		: // fall through
    case Bytecodes::polymorphic_send_super_pop		: return polymorphic_send(code);

    // megamorphic sends
    case Bytecodes::megamorphic_send_0			: // fall through
    case Bytecodes::megamorphic_send_1			: // fall through
    case Bytecodes::megamorphic_send_2			: // fall through
    case Bytecodes::megamorphic_send_n			: // fall through

    case Bytecodes::megamorphic_send_0_pop		: // fall through
    case Bytecodes::megamorphic_send_1_pop		: // fall through
    case Bytecodes::megamorphic_send_2_pop		: // fall through
    case Bytecodes::megamorphic_send_n_pop		: // fall through

    case Bytecodes::megamorphic_send_self		: // fall through
    case Bytecodes::megamorphic_send_self_pop		: // fall through

    case Bytecodes::megamorphic_send_super		: // fall through
    case Bytecodes::megamorphic_send_super_pop		: return megamorphic_send(code);

    // predicted smi sends
    case Bytecodes::smi_add				: return smi_add();
    case Bytecodes::smi_sub				: return smi_sub();
    case Bytecodes::smi_mult				: return smi_mul();

    case Bytecodes::smi_equal				: // fall through
    case Bytecodes::smi_not_equal			: // fall through
    case Bytecodes::smi_less				: // fall through
    case Bytecodes::smi_less_equal			: // fall through
    case Bytecodes::smi_greater				: // fall through
    case Bytecodes::smi_greater_equal			: return smi_compare_op(code);

    case Bytecodes::smi_and				: // fall through
    case Bytecodes::smi_or				: // fall through
    case Bytecodes::smi_xor				: return smi_logical_op(code);
    case Bytecodes::smi_shift				: return smi_shift();

    // local returns
    case Bytecodes::return_tos_pop_0			: return local_return(false, 0);
    case Bytecodes::return_tos_pop_1			: return local_return(false, 1);
    case Bytecodes::return_tos_pop_2			: return local_return(false, 2);
    case Bytecodes::return_tos_pop_n			: return local_return(false, n);

    case Bytecodes::return_self_pop_0			: return local_return(true, 0);
    case Bytecodes::return_self_pop_1			: return local_return(true, 1);
    case Bytecodes::return_self_pop_2			: return local_return(true, 2);
    case Bytecodes::return_self_pop_n			: return local_return(true, n);

    case Bytecodes::return_tos_zap_pop_n		: return local_return(false, n, true);
    case Bytecodes::return_self_zap_pop_n		: return local_return(true, n, true);

    // non-local returns
    case Bytecodes::non_local_return_tos_pop_n		: return nonlocal_return_tos();
    case Bytecodes::non_local_return_self_pop_n		: return nonlocal_return_self();

    // globals
    case Bytecodes::push_global				: return push_global();
    case Bytecodes::store_global_pop			: return store_global(pop);
    case Bytecodes::store_global			: return store_global();

    case Bytecodes::push_classVar			: return push_global();		// same as for globals
    case Bytecodes::store_classVar_pop			: return store_global(pop);	// same as for globals
    case Bytecodes::store_classVar			: return store_global();	// same as for globals

    // miscellaneous
    case Bytecodes::special_primitive_send_1_hint	: return special_primitive_send_hint();

    case Bytecodes::double_equal			: return compare(true);
    case Bytecodes::double_tilde			: return compare(false);

    case Bytecodes::halt				: return halt();

    // not implemented yet
    case Bytecodes::smi_div				: // fall through
    case Bytecodes::smi_mod				: // fall through
    case Bytecodes::smi_create_point			: // fall through

    case Bytecodes::objArray_at				: // fall through
    case Bytecodes::objArray_at_put			: // fall through

    case Bytecodes::return_instVar_name			: // fall through
    case Bytecodes::push_instVar_name			: // fall through
    case Bytecodes::store_instVar_pop_name		: // fall through
    case Bytecodes::store_instVar_name			: // fall through

    case Bytecodes::push_classVar_name			: // fall through
    case Bytecodes::store_classVar_pop_name		: // fall through
    case Bytecodes::store_classVar_name			: // fall through

    // unimplemented
    case Bytecodes::unimplemented_06			: // fall through

    case Bytecodes::unimplemented_20			: // fall through
    case Bytecodes::unimplemented_21			: // fall through
    case Bytecodes::unimplemented_22			: // fall through
    case Bytecodes::unimplemented_23			: // fall through
    case Bytecodes::unimplemented_24			: // fall through
    case Bytecodes::unimplemented_25			: // fall through
    case Bytecodes::unimplemented_26			: // fall through
    case Bytecodes::unimplemented_27			: // fall through

    case Bytecodes::unimplemented_39			: // fall through
    case Bytecodes::unimplemented_3a			: // fall through
    case Bytecodes::unimplemented_3b			: // fall through
    case Bytecodes::unimplemented_3c			: // fall through

    case Bytecodes::unimplemented_b7			: // fall through
    case Bytecodes::unimplemented_bc			: // fall through

    case Bytecodes::unimplemented_c7			: // fall through
    case Bytecodes::unimplemented_cc			: // fall through

    case Bytecodes::unimplemented_dc			: // fall through
    case Bytecodes::unimplemented_de			: // fall through
    case Bytecodes::unimplemented_df			: // fall through

    case Bytecodes::unimplemented_fa			: // fall through
    case Bytecodes::unimplemented_fb			: // fall through
    case Bytecodes::unimplemented_fc			: // fall through
    case Bytecodes::unimplemented_fd			: // fall through
    case Bytecodes::unimplemented_fe			: return NULL;

#ifdef TEST_GENERATION
    default						: ShouldNotReachHere();
#else
    default						: return NULL;
#endif // TEST_GENERATION

  }
  ShouldNotReachHere();
}

void InterpreterGenerator::info(char* name) {
  if (!Disclaimer::is_product() && PrintInterpreter) {
    std->print("%s\n", name);
    masm->code()->decode();
    std->cr();
  }
}


void InterpreterGenerator::generate_all() {
  Interpreter::_code_begin_addr = masm->pc();

  generate_forStubRountines();
  info("code for StubRountines");

  StubRoutines::init();

  // generate code for Floats
  Floats::init(masm);
  Floats::_function_table[Floats::oopify] = float_oopify();	// patch - no code generated in Floats for oopify
  info("Floats::oopify patch");

  // generate helper routines/code fragments
  generate_error_handler_code();
  info("error handler code");

  generate_nonlocal_return_code();
  info("non-local return code");

  generate_method_entry_code();
  info("method entry code");

  generate_inline_cache_miss_handler();
  info("inline cache miss handler");

  generate_predicted_smi_send_failure_handler();
  info("predicted smi send failure handler");

  generate_redo_send_code();
  info("redo send code");

  generate_deoptimized_return_code();
  info("deoptimized return code");

  for (int n = 0; n < 10; n++)
    generate_primitiveValue(n);
  info("primitiveValues");


  // generate individual instructions
  for (int i = 0; i < Bytecodes::number_of_codes; i++) {
    char* start_point = masm->pc();
    char* entry_point = generate_instruction((Bytecodes::Code)i);
    if (entry_point != NULL) {
      // bytecode implemented
      Bytecodes::set_entry_point(Bytecodes::Code(i), entry_point);
      if (!Disclaimer::is_product() && PrintInterpreter) {
      #ifndef PRODUCT
        int length = masm->pc() - start_point;
	char* name = Bytecodes::name((Bytecodes::Code)i);
        std->print("Bytecode 0x%02x: %s (%d bytes), entry point = 0x%x\n", i, name, length, entry_point);
	masm->code()->decode();
	std->cr();
      #endif
      }
    }
  }

  masm->finalize();
  Interpreter::_code_end_addr = masm->pc();
}


InterpreterGenerator::InterpreterGenerator(CodeBuffer* code, bool debug) {
  masm = new MacroAssembler(code);
  _debug = debug;
  _stack_check = Interpreter::has_stack_checks();
  generate_all();
}


// Interpreter initialization

static const int interpreter_size = 40000;
//static char interpreter_code[interpreter_size];
static char* interpreter_code;


void interpreter_init() {
  const bool debug = false; // change this to switch between debug/optimized version
  if (!Disclaimer::is_product() && PrintInterpreter) {
    std->print("\nBytecode Interpreter\n\n");
  }

  interpreter_code = os::exec_memory(interpreter_size);
  
  CodeBuffer* code = new CodeBuffer(&interpreter_code[0], interpreter_size);
  InterpreterGenerator g(code, debug);

  if (!Disclaimer::is_product() && PrintInterpreter) {
    std->print("%d bytes generated for the interpreter\n", code->code_size());
//    exit(0);
  }

  Interpreter::init();
}
