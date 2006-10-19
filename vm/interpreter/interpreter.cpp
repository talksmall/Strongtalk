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
extern "C" int interpreter_loop_counter;		// see interpreter_asm.asm
extern "C" int interpreter_loop_counter_limit;	// see interpreter_asm.asm


// Boundaries of assembled interpreter code
extern "C" char* interpreter_begin_addr;
extern "C" char* interpreter_end_addr;

bool Interpreter::contains(char* pc) {
  return
    (interpreter_begin_addr <= pc && pc < interpreter_end_addr) ||
    (_code_begin_addr       <= pc && pc < _code_end_addr);
}


extern "C" char* InterpreterCodeStatus();	// see interpreter_asm.asm

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
  if (TraceBytecodes) {
    u_char* ip = DeltaProcess::active()->last_frame().hp();
    char* name = Bytecodes::name((Bytecodes::Code)*ip);
    std->print("%9d 0x%x: %02x %s\n", NumberOfBytecodesExecuted, ip, *ip, name);
  } 
}


void Interpreter::wrong_eax() {
  fatal("interpreter bug: eax doesn't contain the right value");
}


void Interpreter::wrong_ebx() {
  fatal("interpreter bug: high 3 bytes of ebx # 0");
}


void Interpreter::wrong_esp() {
  fatal("interpreter bug: esp doesn't contain the right value");
}


void Interpreter::wrong_obj() {
  fatal("interpreter bug: register doesn't contain a valid oop");
}


void Interpreter::wrong_primitive_result() {
  fatal("interpreter bug: primitive failed that is not supposed to fail");
}


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
  MacroAssembler*	_masm;		// used to generate code
  bool			_debug;		// indicates debug mode

  Label		_method_entry;		// entry point to activate method execution
  Label		_block_entry;		// entry point to activate block execution (primitiveValue)
  Label		_inline_cache_miss;	// inline cache misses handling
  Label		_smi_send_failure;	// handles predicted smi send failures
  Label		_issue_NLR;		// the starting point for NLRs in interpreted code
  Label		_nlr_testpoint;		// the return point for NLRs in interpreted sends

  Label		_boolean_expected;	// boolean expected error
  Label		_float_expected;	// float expected error
  Label		_NLR_to_dead_frame;	// NLR error
  Label		_halted;		// halt executed

  // Debugging
  void check_ebx();
  void check_oop(Register reg);
  void should_not_reach_here();
  
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

  void save_esi()		{ _masm->movl(esi_addr(), esi); }
  void restore_esi()		{ _masm->movl(esi, esi_addr()); }
  void restore_ebx()		{ _masm->xorl(ebx, ebx); }

  // Constant addresses
  Address nil_addr()		{ return Address(int(&nilObj), relocInfo::external_word_type); }
  Address true_addr()		{ return Address(int(&trueObj), relocInfo::external_word_type); }
  Address false_addr()		{ return Address(int(&falseObj), relocInfo::external_word_type); }
  Address smiKlass_addr()	{ return Address(int(&smiKlassObj), relocInfo::external_word_type); }
  Address doubleKlass_addr()	{ return Address(int(&doubleKlassObj), relocInfo::external_word_type); }
  Address contextKlass_addr()	{ return Address(int(&symbolKlassObj), relocInfo::external_word_type); }

  // C calls
  void call_C(Label& L);
  void call_C(char* entry);
  void call_C(Register entry);
  
  // Parameter passing/returns
  void load_recv(Bytecodes::ArgumentSpec arg_spec);
  void return_tos(Bytecodes::ArgumentSpec arg_spec);

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
  char* push_context_temp(int contextNo, int tempNo);

  char* float_allocate();
  char* float_floatify();
  char* float_oopify();
  char* float_move();
  char* float_set();
  char* float_op(int nof_args, bool returns_float = false);

  char* push_closure(char* allocation_routine, bool use_context);
  char* install_context(char* allocation_routine, bool for_method);

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
  char* normal_send(Bytecodes::Code code, bool allow_methodOop, bool allow_nmethod);

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

  char* special_primitive_send_hint();
  char* halt();

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
    Label L;
    _masm->testl(ebx, 0xFFFFFF00);
    _masm->jcc(Assembler::zero, L);
    _masm->call_C((char*)Interpreter::wrong_ebx, relocInfo::runtime_call_type);
    _masm->bind(L);
  }
}


void InterpreterGenerator::check_oop(Register reg) {
  // check if reg contains an oop
  if (_debug) {
    Label L;
    _masm->testb(reg, Mark_Tag_Bit);
    _masm->jcc(Assembler::zero, L);
    _masm->call_C((char*)Interpreter::wrong_obj, relocInfo::runtime_call_type);
    _masm->bind(L);
  }
}


void InterpreterGenerator::should_not_reach_here() {
  // make sure the Interpreter traps if this point is ever reached
  _masm->hlt();
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
  _masm->addl(esi, (n+1)*oopSize);	// advance
  _masm->andl(esi, -oopSize);		// align
}


void InterpreterGenerator::advance_aligned(int n) {
  _masm->addl(esi, n + oopSize - 1);	// advance
  _masm->andl(esi, -oopSize);		// align
}


void InterpreterGenerator::load_ebx() {
  check_ebx();
  _masm->movb(ebx, Address(esi));
}


void InterpreterGenerator::next_ebx() {
  check_ebx();
  _masm->movb(ebx, Address(esi, 1));
  _masm->incl(esi);
}


void InterpreterGenerator::jump_ebx() {
  if (TraceBytecodes || CountBytecodes || StopInterpreterAt > 0) {
    _masm->incl(Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type));
    if (StopInterpreterAt > 0) {
      Label cont;
      _masm->cmpl(Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type), StopInterpreterAt);
      _masm->jcc(Assembler::less, cont);
      _masm->int3();
      _masm->bind(cont);
    }
  }
  if (TraceBytecodes) {
    _masm->pushl(eax);	// save tos
    call_C((char*)Interpreter::trace_bytecode);
    _masm->popl(eax);	// restore tos
    load_ebx();
  }
  check_oop(eax);
  _masm->jmp(Address(noreg, ebx, Address::times_4, (int)dispatchTable::table()));
}


void InterpreterGenerator::load_edi() {
  _masm->movl(edi, Address(noreg, ebx, Address::times_4, (int)dispatchTable::table()));
}


void InterpreterGenerator::jump_edi() {
  if (TraceBytecodes || CountBytecodes || StopInterpreterAt > 0) {
    _masm->incl(Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type));
    if (StopInterpreterAt > 0) {
      Label cont;
      _masm->cmpl(Address(int(&NumberOfBytecodesExecuted), relocInfo::external_word_type), StopInterpreterAt);
      _masm->jcc(Assembler::less, cont);
      _masm->int3();
      _masm->bind(cont);
    }
  }
  if (TraceBytecodes) {
    _masm->pushl(eax);	// save tos
    call_C((char*)Interpreter::trace_bytecode);
    _masm->popl(eax);	// restore tos
    load_ebx();
  }
  check_oop(eax);
  _masm->jmp(edi);
}


char* InterpreterGenerator::entry_point() {
  char* ep = _masm->pc();
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
  _masm->call_C(L, _nlr_testpoint);
  restore_esi();
  restore_ebx();
}


void InterpreterGenerator::call_C(char* entry) {
  save_esi();
  _masm->call_C(entry, relocInfo::runtime_call_type, _nlr_testpoint);
  restore_esi();
  restore_ebx();
}


void InterpreterGenerator::call_C(Register entry) {
  save_esi();
  _masm->call_C(entry, _nlr_testpoint);
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
  _masm->pushl(eax); // make sure receiver & all arguments are on the stack
  switch (arg_spec) {
    case Bytecodes::recv_0_args: break; // recv already in eax
    case Bytecodes::recv_1_args: _masm->movl(eax, Address(esp, 1*oopSize)); break;
    case Bytecodes::recv_2_args: _masm->movl(eax, Address(esp, 2*oopSize)); break;
    case Bytecodes::recv_n_args:
      // byte after send byte code specifies the number of arguments (0..255)
      _masm->movb(ebx, Address(esi, 1));
      _masm->movl(eax, Address(esp, ebx, Address::times_4));
      break;
    case Bytecodes::args_only  : _masm->movl(eax, self_addr()); break;
    default                    : ShouldNotReachHere();
  }
}


//-----------------------------------------------------------------------------------------
// Instructions

char* InterpreterGenerator::push_temp(int i) {
  char* ep = entry_point();
  next_ebx();
  _masm->pushl(eax);
  load_edi();
  _masm->movl(eax, temp_addr(i));
  jump_edi();
  return ep;
}


char* InterpreterGenerator::push_temp_n() {
  char* ep = entry_point();
  _masm->addl(esi, 2);
  _masm->movb(ebx, Address(esi, -1));
  _masm->pushl(eax);
  _masm->movl(eax, temp_addr(ebx));
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_arg(int i) {
  char* ep = entry_point();
  next_ebx();
  _masm->pushl(eax);
  load_edi();
  _masm->movl(eax, arg_addr(i));
  jump_edi();
  return ep;
}


char* InterpreterGenerator::push_arg_n() {
  char* ep = entry_point();
  _masm->addl(esi, 2);
  _masm->movb(ebx, Address(esi, -1));
  _masm->pushl(eax);
  _masm->movl(eax, arg_addr(ebx));
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_smi(bool negative) {
  char* ep = entry_point();
  _masm->movb(ebx, Address(esi, 1));	// get b
  _masm->addl(esi, 2);			// advance to next bytecode
  _masm->pushl(eax);			// save tos
  if (negative) {
    _masm->leal(eax, Address(noreg, ebx, Address::times_4));
    _masm->negl(eax);
  } else {
    _masm->leal(eax, Address(noreg, ebx, Address::times_4, 4, relocInfo::none));
  }
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_literal() {
  char* ep = entry_point();
  _masm->pushl(eax);
  skip_words(1);
  load_ebx();
  _masm->movl(eax, Address(esi, -4));
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_tos() {
  char* ep = entry_point();
  next_ebx();
  _masm->pushl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_self() {
  char* ep = entry_point();
  next_ebx();
  _masm->pushl(eax);
  load_edi();
  _masm->movl(eax, self_addr());
  jump_edi();
  return ep;
}


char* InterpreterGenerator::push_const(Address obj_addr) {
  char* ep = entry_point();
  _masm->pushl(eax);
  next_ebx();
  _masm->movl(eax, obj_addr);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_instVar() {
  char* ep = entry_point();
  advance_aligned(1 + oopSize);
  _masm->movl(ecx, self_addr());
  _masm->movl(edx, Address(esi, -oopSize));
  _masm->pushl(eax);
  load_ebx();
  _masm->movl(eax, field_addr(ecx, edx));
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::store_instVar(bool pop) {
  char* ep = entry_point();
  advance_aligned(1 + oopSize);
  _masm->movl(ecx, self_addr());
  _masm->movl(edx, Address(esi, -oopSize));
  load_ebx();
  _masm->movl(field_addr(ecx, edx), eax);
  _masm->store_check(ecx, edx);
  if (pop) _masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::return_instVar() {
  char* ep = entry_point();
  advance_aligned(1 + oopSize);
  _masm->movl(ecx, self_addr());
  _masm->movl(edx, Address(esi, -oopSize));
  _masm->movl(eax, field_addr(ecx, edx));
  return_tos(Bytecodes::recv_0_args);
  return ep;
}


char* InterpreterGenerator::only_pop() {
  char* ep = entry_point();
  next_ebx();
  _masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::store_temp(int i, bool pop) {
  char* ep = entry_point();
  next_ebx();
  _masm->movl(temp_addr(i), eax);
  if (pop) _masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::store_temp_n(bool pop) {
  char* ep = entry_point();
  _masm->addl(esi, 2);
  _masm->movb(ebx, Address(esi, -1));
  _masm->movl(temp_addr(ebx), eax);
  load_ebx();
  if (pop) _masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_global() {
  char* ep = entry_point();
  skip_words(1);
  _masm->pushl(eax);
  _masm->movl(ecx, Address(esi, -oopSize));					// get association
  load_ebx();
  _masm->movl(eax, field_addr(ecx, associationOopDesc::value_offset()));	// get value via association
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::store_global(bool pop) {
  char* ep = entry_point();
  skip_words(1);
  _masm->movl(ecx, Address(esi, -oopSize));					// get association
  load_ebx();
  _masm->movl(field_addr(ecx, associationOopDesc::value_offset()), eax);	// store value via association
  _masm->store_check(ecx, edx);
  if (pop) _masm->popl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::allocate_temps(int n) {
  char* ep = entry_point();
  assert(n > 0, "just checkin'");
  next_ebx();
  _masm->pushl(eax);
  _masm->movl(eax, nil_addr());
  while (--n > 0) _masm->pushl(eax);
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::allocate_temps_n() {
  Label entry, loop;

  _masm->bind(loop);
  _masm->pushl(eax);
  _masm->bind(entry);
  _masm->decb(ebx);
  _masm->jcc(Assembler::notZero, loop);
  load_ebx();
  jump_ebx();

  char* ep = entry_point();
  _masm->movb(ebx, Address(esi, 1));		// get n (n = 0 ==> 256 temps)
  _masm->addl(esi, 2);				// advance to next bytecode
  _masm->pushl(eax);
  _masm->movl(eax, nil_addr());
  _masm->jmp(entry);

  return ep;
}


//-----------------------------------------------------------------------------------------
// Context temporaries
//
// Note that eax must always be pushed in the beginning since it may hold the context (temp0).

char* InterpreterGenerator::set_self_via_context() {
  Label loop;
  char* ep = entry_point();
  next_ebx();
  _masm->movl(edx, self_addr());		// get incoming context (stored in receiver)
  _masm->bind(loop);				// search for home context
  _masm->movl(ecx, edx);			// save current context
  _masm->movl(edx, Address(edx, contextOopDesc::parent_byte_offset()));
  _masm->test(edx, Mem_Tag);			// check if parent is_smi
  _masm->jcc(Assembler::notZero, loop);		// if not, current context is not home context
  _masm->movl(edx, Address(ecx, contextOopDesc::temp0_byte_offset()));
  _masm->movl(self_addr(), edx);		// set self in activation frame
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::push_context_temp(int contextNo, int tempNo) {
  assert(contextNo >= 0, "illegal context no.");
  assert(tempNo >= 0, "illegal temporary no.");
  char* ep = entry_point();
  _masm->pushl(eax);
  _masm->movl(ecx, context_addr());
  next_ebx();
  for (int i = 0; i < contextNo; i++) _masm->movl(ecx, Address(ecx, contextOopDesc::parent_byte_offset()));
  _masm->movl(eax, Address(ecx, contextOopDesc::temp0_byte_offset() + tempNo*oopSize));
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

extern "C" oop allocateBlock(smiOop nofArgs);	// Note: needs last Delta frame setup!

// Note: The following routines don't need the last Delta frame to be setup
extern "C" oop allocateBlock0();
extern "C" oop allocateBlock1();
extern "C" oop allocateBlock2();


char* InterpreterGenerator::push_closure(char* allocation_routine, bool use_context) {
  char* ep = entry_point();
  _masm->pushl(eax);							// save tos
  if (allocation_routine == (char*)allocateBlock) {
    // no. of arguments specified by 2nd byte
    _masm->movb(ebx, Address(esi, 1));					// get no. of arguments
    advance_aligned(2 + oopSize);					// go to next instruction
    _masm->shll(ebx, Tag_Size);						// convert into smi (pushed on the stack!)
    save_esi();								// save vital registers
    _masm->pushl(ebx);							// pass as argument
    _masm->set_last_Delta_frame_before_call();				// allocateBlock needs last Delta frame!
    _masm->call((char*)allocateBlock, relocInfo::runtime_call_type);	// eax := block closure(nof. args)
    _masm->reset_last_Delta_frame();
    _masm->popl(ebx);							// get rid of argument
  } else {
    // no. of arguments implied by 1st byte
    advance_aligned(1 + oopSize);					// go to next instruction
    save_esi();								// no last Delta frame setup needed => save vital registers
    _masm->call(allocation_routine, relocInfo::runtime_call_type);	// eax := block closure
  }
  restore_esi();							// returning from C land => restore esi (ebx is restored later)
  _masm->movl(ecx, Address(esi, -oopSize));				// get block methodOop
  if (use_context) {							// if full block then
    _masm->movl(edx, context_addr());					//   get context of this activation
    if (_debug) {
      // should check if edx is really pointing to a context
      // (can it ever happen that temp0 is not holding a context
      // but push_closure is used with the use_context attribute?)
    }
  } else {								// else
    _masm->popl(edx);							//   use tos as context information
  }
  // install methodOop and context in block closure and increment
  // the invocation counter of the parent (= enclosing) methodOop
  //
  // eax: block closure
  // ecx: block methodOop
  // edx: context
  _masm->movl(ebx, Address(ecx, methodOopDesc::selector_or_method_byte_offset()));	// get parent (= running) methodOop
  _masm->movl(Address(eax, blockClosureOopDesc::method_or_entry_byte_offset()), ecx);	// set block method
  _masm->movl(ecx, Address(ebx, methodOopDesc::counters_byte_offset()));// get counter of parent methodOop
  _masm->movl(Address(eax, blockClosureOopDesc::context_byte_offset()), edx);		// set context
  _masm->addl(ecx, 1 << methodOopDesc::_invocation_count_offset);	// increment invocation counter of parent methodOop
  _masm->movl(edx, eax);						// make sure eax is not destroyed
  _masm->movl(Address(ebx, methodOopDesc::counters_byte_offset()), ecx);// store counter of parent methodOop
  restore_ebx();
  load_ebx();								// get next instruction
  _masm->store_check(edx, ecx);						// do a store check on edx, use ecx as scratch register
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

// Note: The following routines don't need the last Delta frame to be setup
extern "C" oop allocateContext(smiOop nofVars);
extern "C" oop allocateContext0();
extern "C" oop allocateContext1();
extern "C" oop allocateContext2();

char* InterpreterGenerator::install_context(char* allocation_routine, bool for_method) {
  char* ep = entry_point();
  _masm->pushl(eax);				// save tos
  if (allocation_routine == (char*)allocateContext) {
    // no. of variables specified by 2nd byte
    _masm->movb(ebx, Address(esi, 1));		// get no. of variables
    _masm->addl(esi, 2);			// go to next instruction
    _masm->shll(ebx, Tag_Size);			// convert into smi (pushed on the stack!)
    save_esi();					// no last Delta frame setup needed => save vital registers
    _masm->pushl(ebx);				// pass as argument
    _masm->call((char*)allocateContext, relocInfo::runtime_call_type);		// eax := context(nof. vars)
    _masm->popl(ebx);				// get rid of argument
  } else {
    // no. of variables implied by 1st byte
    _masm->incl(esi);				// go to next instruction
    save_esi();					// no last Delta frame setup needed => save vital registers
    _masm->call(allocation_routine, relocInfo::runtime_call_type);		// eax := context
  }
  restore_esi();				// returning from C land => restore vital registers
  restore_ebx();
  if (for_method) {				// if method context then
    _masm->movl(Address(eax, contextOopDesc::parent_byte_offset()), ebp);	// parent points to method frame
  } else {					// else
    _masm->movl(ecx, context_addr());		// get (incoming) enclosing context
    if (_debug) {
      // should check if ecx is really pointing to a context
      // (can it ever happen that temp0 is not holding a context
      // but install_context is used with the use_context attribute?)
    }
    _masm->movl(Address(eax, contextOopDesc::parent_byte_offset()), ecx);	// parent points to enclosing context
  }
  load_ebx();					// get next instruction
  _masm->movl(context_addr(), eax);		// install context
  _masm->store_check(eax, ecx);			// store check on eax, use ecx as scratch register
  _masm->popl(eax);				// restore tos
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
    _masm->leal(ecx, Address(ebp, esi_offset));
    _masm->cmpl(esp, ecx);
    _masm->jcc(Assembler::equal, L1);
    _masm->call_C((char*)Interpreter::wrong_esp, relocInfo::runtime_call_type);
    should_not_reach_here();
    _masm->bind(L1);

    // check eax (corresponds now to temp0, must be initialized to nil)
    _masm->cmpl(eax, nil_addr());
    _masm->jcc(Assembler::equal, L2);
    _masm->call_C((char*)Interpreter::wrong_eax, relocInfo::runtime_call_type);
    should_not_reach_here();
    _masm->bind(L2);
  }
  _masm->addl(esi, 4);				// advance to next bytecode
  _masm->pushl(eax);				// save tos (i.e. temp0)
  _masm->pushl(Floats::magic);			// initialize temp1 (indicates a float section)

  // allocate additional temps in multiples of 2 (to compensate for one float)
  _masm->movb(ebx, Address(esi, -3));		// get nofTemps
  _masm->testl(ebx, ebx);			// allocate no additional temps if nofTemps = 0
  _masm->jcc(Assembler::zero, tDone);
  _masm->movl(eax, nil_addr());
  _masm->bind(tLoop);
  _masm->pushl(eax);				// push nil
  _masm->pushl(eax);				// push nil
  _masm->decl(ebx);
  _masm->jcc(Assembler::notZero, tLoop);
  _masm->bind(tDone);
  
  // allocate floats
  _masm->movb(ebx, Address(esi, -2));		// get nofFloats
  _masm->testl(ebx, ebx);			// allocate no additional floats if nofFloats = 0
  _masm->jcc(Assembler::zero, fDone);
  _masm->xorl(eax, eax);			// use 0 to initialize the stack with 0.0
  _masm->bind(fLoop);
  _masm->pushl(eax);				// push 0.0 (allocate a double)
  _masm->pushl(eax);
  _masm->decb(ebx);
  _masm->jcc(Assembler::notZero, fLoop);
  _masm->bind(fDone);

  // allocate floats expression stack
  assert(floatSize == 8, "change the constant for shhl below");
  _masm->movb(ebx, Address(esi, -1));		// get floats expression stack size
  _masm->shll(ebx, 3);				// multiply with floatSize
  _masm->subl(esp, ebx);			// adjust esp
  restore_ebx();

  // continue with next instruction
  load_ebx();					// continue with next instruction
  _masm->popl(eax);				// restore tos in eax
  jump_ebx();

  return ep;
}
    
  
char* InterpreterGenerator::float_floatify() {
  Label is_smi;
  char* ep = entry_point();
  _masm->addl(esi, 2);				// advance to next instruction
  _masm->testb(eax, Mem_Tag);			// check if smi
  _masm->jcc(Assembler::zero, is_smi);
  _masm->movl(ecx, Address(eax, memOopDesc::klass_byte_offset()));	// check if float
  _masm->cmpl(ecx, doubleKlass_addr());
  _masm->jcc(Assembler::notEqual, _float_expected);

  // unbox doubleOop
  _masm->movb(ebx, Address(esi, -1));		// get float number
  _masm->fld_d(Address(eax, byteOffset(doubleOopDesc::value_offset()))); // unbox float
  _masm->fstp_d(float_addr(ebx));		// store float
  load_ebx();
  _masm->popl(eax);				// discard argument
  jump_ebx();

  // convert smi
  _masm->bind(is_smi);
  _masm->movb(ebx, Address(esi, -1));		// get float number
  _masm->leal(ecx, float_addr(ebx));
  _masm->sarl(eax, Tag_Size);			// convert smi argument into int
  _masm->movl(Address(ecx), eax);		// store it in memory (use float target location)
  _masm->fild_s(Address(ecx));			// convert it into float
  _masm->fstp_d(Address(ecx));			// store float
  load_ebx();
  _masm->popl(eax);				// discard argument
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
  _masm->popl(eax);				// discard return address
  _masm->fpop();				// pop ST (in order to avoid FPU stack overflows) -> get rid of argument
  call_C((char*)Interpreter::oopify_FloatValue);// eax := oopify_FloatValue() (gets its argument by looking at the last bytecode)
  load_ebx();
  jump_ebx();
  return ep;
}
    
  
char* InterpreterGenerator::float_move() {
  char* ep = entry_point();
  _masm->pushl(eax);				// make sure last float is completely in memory
  _masm->addl(esi, 3);				// advance to next instruction
  _masm->xorl(ecx, ecx);			// clear ecx
  _masm->movb(ebx, Address(esi, -1));		// get source float number
  _masm->movb(ecx, Address(esi, -2));		// get destination float number
  _masm->fld_d(float_addr(ebx));		// load source
  load_ebx();
  _masm->fstp_d(float_addr(ecx));		// store at destination
  _masm->popl(eax);				// re-adjust esp
  jump_ebx();
  return ep;
}
    
  
char* InterpreterGenerator::float_set() {
  char* ep = entry_point();
  _masm->pushl(eax);				// make sure last float is completely in memory
  _masm->movb(ebx, Address(esi, 1));		// get float number
  advance_aligned(2 + oopSize);			// advance to next instruction
  _masm->movl(ecx, Address(esi, -oopSize));	// get doubleOop address
  _masm->fld_d(Address(ecx, byteOffset(doubleOopDesc::value_offset()))); // unbox float
  _masm->fstp_d(float_addr(ebx));		// store it
  load_ebx();
  _masm->popl(eax);				// re-adjust esp
  jump_ebx();
  return ep;
}
    
  
char* InterpreterGenerator::float_op(int nof_args, bool returns_float) {
  assert(0 <= nof_args && nof_args <= 8, "illegal nof_args specification");
  char* ep = entry_point();
  _masm->pushl(eax);				// make sure all floats are completely in memory
  _masm->addl(esi, 3);				// advance to next instruction
  _masm->movb(ebx, Address(esi, -2));		// get float number
  _masm->leal(edx, float_addr(ebx));		// get float address
  _masm->movb(ebx, Address(esi, -1));		// get function number
  _masm->movl(ecx, Address(noreg, ebx, Address::times_4, int(Floats::_function_table), relocInfo::external_word_type));
  for (int i = 0; i < nof_args; i++) _masm->fld_d(Address(edx, -i*floatSize));
  _masm->call(ecx);				// invoke operation
  load_ebx();					// get next byte code
  if (returns_float) {
    _masm->fstp_d(Address(edx));		// store result
    _masm->popl(eax);				// re-adjust esp
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

char* InterpreterGenerator::lookup_primitive() {
  char* ep = entry_point();
  _masm->pushl(eax);				// push last argument
  call_C((char*)primitives::lookup_and_patch);	// do the lookup and patch call site appropriately
  load_ebx();
  _masm->popl(eax);				// restore last argument
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::call_primitive() {
  char* ep = entry_point();
  advance_aligned(1 + oopSize);
  _masm->pushl(eax);				// push last argument
  _masm->movl(eax, Address(esi, -oopSize));	// get primitive entry point
  call_C(eax);					// eax := primitive call(...)
  if (_debug) {					// (Pascal calling conv. => args are popped by callee)
    Label L;
    _masm->testb(eax, Mark_Tag_Bit);
    _masm->jcc(Assembler::zero, L);
    _masm->call_C((char*)Interpreter::wrong_primitive_result, relocInfo::runtime_call_type);
    _masm->bind(L);
  }
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::call_primitive_can_fail() {
  Label failed;
  char* ep = entry_point();
  advance_aligned(1 + 2*oopSize);
  _masm->pushl(eax);				// push last argument
  _masm->movl(eax, Address(esi, -2*oopSize));	// get primitive entry point
  call_C(eax);					// eax := primitive call(...) (Pascal calling conv.)
  _masm->testb(eax, Mark_Tag_Bit);		// if not marked then
  _masm->jcc(Assembler::notZero, failed);
  _masm->movl(ecx, Address(esi, -oopSize));	// get jump offset
  _masm->addl(esi, ecx);			// jump over failure block
  load_ebx();
  jump_ebx();

  _masm->bind(failed);
  _masm->andl(eax, ~Mark_Tag_Bit);		// unmark result
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
  _masm->incl(esi);				// advance to next instruction (skip no. of arguments byte)
  _masm->pushl(eax);				// push last argument
  _masm->movl(edx, Address(esi, -1 - oopSize));	// get dll function ptr
  _masm->testl(edx, edx);			// test if function has been looked up already
  _masm->jcc(Assembler::notZero, L); 		// and continue - otherwise lookup dll function & patch
  call_C((char*)DLLs::lookup_and_patch_InterpretedDLL_Cache);	// eax := returns dll function ptr
  _masm->movl(edx, eax);			// move dll function ptr into right register
  _masm->bind(L);				// and continue
  _masm->movb(ebx, Address(esi, -1));		// get no. of arguments
  _masm->movl(ecx, esp);			// get address of last argument
  save_esi();					// don't use call_C because no last_Delta_frame setup needed
  _masm->call(StubRoutines::call_DLL_entry(async), relocInfo::runtime_call_type);// eax := DLL call via a separate frame (parameter conversion)
  _masm->ic_info(_nlr_testpoint, 0);
  restore_esi();
  restore_ebx();
  _masm->movb(ebx, Address(esi, -1));		// get no. of arguments
  _masm->leal(esp, Address(esp, ebx, Address::times_4)); // pop arguments
  _masm->popl(ecx);				// get proxy object
  _masm->movl(Address(ecx, pointer_offset), eax); // box result
  load_ebx();
  _masm->movl(eax, ecx);			// return proxy
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
  Interpreter::_redo_send_entry = _masm->pc();
  restore_esi();				// has been saved by call_native
  restore_ebx();				// possibly destroyed
  load_ebx();
  _masm->popl(eax);				// get last argument into eax again
  jump_ebx();					// restart send
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
  save_esi();
  _masm->call(_method_entry);
  _masm->ic_info(_nlr_testpoint, 0);
  restore_esi();
}


char* Interpreter::_last_native_called = NULL;		// debugging only - see comment in header file

void InterpreterGenerator::call_native(Register entry) {
  // trace_send code should come here - fix this
  save_esi();
  _masm->movl(Address(int(&Interpreter::_last_native_called), relocInfo::external_word_type), entry);
  _masm->call(entry);
  _masm->ic_info(_nlr_testpoint, 0);			// ordinary inline cache info
  restore_esi();
  restore_ebx();
}


extern "C" char* method_entry_point = NULL;		// for interpreter_asm.asm (remove if not used anymore)
extern "C" char* block_entry_point  = NULL;		// for interpreter_asm.asm (remove if not used anymore)

void InterpreterGenerator::generate_method_entry_code() {
// This generates the code sequence called to activate methodOop execution.
// It is usually called via the call_method() macro, which saves the old
// instruction counter (esi) and provides the ic_info word for NLRs.

  const int counter_offset = methodOopDesc::counters_byte_offset();
  const int code_offset    = methodOopDesc::codes_byte_offset();

  assert(!_method_entry.is_bound(), "code has been generated before");
  Label start_setup, counter_overflow, start_execution, handle_counter_overflow, is_interpreted;

  // eax: receiver
  // ebx: 000000xx
  // ecx: methodOop
  // parameters on the stack
  method_entry_point = _masm->pc();
  _masm->bind(_method_entry);
  _masm->movl(edi, nil_addr());

  // eax: receiver
  // ebx: 000000xx
  // ecx: methodOop
  // edi: initial value for temp0
  // parameters on the stack
  _masm->bind(start_setup);
  _masm->enter();							// setup new stack frame
  _masm->pushl(eax);							// install receiver
  _masm->movl(edx, Address(ecx, counter_offset));			// get method invocation counter
  _masm->leal(esi, Address(ecx, code_offset));				// set bytecode pointer to first instruction
  _masm->addl(edx, 1 << methodOopDesc::_invocation_count_offset);	// increment invocation counter (only upper word)
  _masm->pushl(esi);							// initialize esi stack location for profiler
  _masm->movl(Address(ecx, counter_offset), edx);			// store method invocation counter
  load_ebx();								// get first byte code of method
  _masm->cmpl(edx, 0xFFFF << methodOopDesc::_invocation_count_offset);	// make sure cmpl uses imm32 field
  Interpreter::_invocation_counter_addr = (int*)(_masm->pc() - oopSize);// compute invocation counter address
  _masm->jcc(Assembler::aboveEqual, counter_overflow);			// treat invocation counter overflow
  _masm->bind(start_execution);						// continuation point after overflow
  _masm->movl(eax, edi);						// initialize temp0
  jump_ebx();								// start execution

  // invocation counter overflow
  _masm->bind(counter_overflow);
  // not necessary to store esi since it has been just initialized
  _masm->pushl(edi);							// move tos on stack (temp0, always here)
  _masm->set_last_Delta_frame_before_call();
  _masm->call(handle_counter_overflow);					// introduce extra frame to pass arguments
  _masm->reset_last_Delta_frame();
  _masm->popl(edi);							// restore edi, used to initialize eax
  // Should check here if recompilation created a nmethod for this
  // methodOop. If so, one should redo the send and thus start the
  // nmethod.
  // If an nmethod has been created, invocation_counter_overflow
  // returns the continuation pc, otherwise it returns NULL.
  // For now: simply continue with interpreted version.
  restore_esi();
  restore_ebx();
  load_ebx();
  _masm->jmp(start_execution);

  // handle invocation counter overflow, use extra frame to pass arguments
  // eax: receiver
  // ecx: methodOop
  _masm->bind(handle_counter_overflow);
  _masm->pushl(ecx);							// pass methodOop argument
  _masm->pushl(eax);							// pass receiver argument
  _masm->call((char*)Recompilation::methodOop_invocation_counter_overflow, relocInfo::runtime_call_type); // methodOop_invocation_counter_overflow(receiver, methodOop)
  _masm->addl(esp, 2*oopSize);						// discard arguments
  _masm->ret(0);


// This generates the code sequence called to activate block execution.
// It is jumped to from one of the primitiveValue primitives. eax is
// expected to hold the receiver (i.e., the block closure).

  // eax: receiver (block closure)
  // primitiveValue parameters on the stack
  block_entry_point = _masm->pc();
  _masm->bind(_block_entry);
  _masm->movl(ecx, Address(eax, blockClosureOopDesc::method_or_entry_byte_offset()));	// get methodOop/jump table entry out of closure
  _masm->reset_last_Delta_frame();					// if called from the interpreter, the last Delta frame is setup
  _masm->test(ecx, Mem_Tag);						// if methodOop then
  _masm->jcc(Assembler::notZero, is_interpreted);			//   start methodOop execution
  _masm->jmp(ecx);							// else jump to jump table entry

  _masm->bind(is_interpreted);
  // eax: receiver (block closure)
  // ecx: block methodOop
  restore_ebx();							// if value... is called from compiled code, ebx may be not zero
  _masm->movl(eax, Address(eax, blockClosureOopDesc::context_byte_offset()));		// get context out of closure
  _masm->movl(edi, eax);						// initial value for temp0 is (incoming) context/value
  // eax: context (= receiver)
  // ebx: 00000000
  // ecx: block methodOop
  // edi: context (= initial value for temp0)
  // parameters on stack
  _masm->jmp(start_setup);
}


//-----------------------------------------------------------------------------------------
// Inline cache misses

void InterpreterGenerator::generate_inline_cache_miss_handler() {
  assert(!_inline_cache_miss.is_bound(), "code has been generated before");
  _masm->bind(_inline_cache_miss);
  // We need an inline cache for NLR evaluation.
  // This can happen because the inline cache miss may call "doesNotUnderstand:"
  call_C((char*)InterpretedIC::inline_cache_miss);
  load_ebx();
  _masm->popl(eax);
  jump_ebx();
}


//-----------------------------------------------------------------------------------------
// smi predicted sends


void InterpreterGenerator::generate_predicted_smi_send_failure_handler() {
  assert(!_smi_send_failure.is_bound(), "code has been generated before");
  char* ep = normal_send(Bytecodes::interpreted_send_1, true, false);
  // Note: Has to jump to normal_send entry point because the entry point is
  //       not necessarily in the beginning of the normal send code pattern.
  _masm->bind(_smi_send_failure);
  _masm->pushl(edx);					// push receiver back on tos
  _masm->jmp(ep, relocInfo::runtime_call_type);
}


void InterpreterGenerator::check_smi_tags() {
  // tos: receiver
  // eax: argument
  _masm->popl(edx);					// get receiver
  _masm->movl(ecx, eax);				// copy it to ecx
  _masm->orl(ecx, edx);					// or tag bits
  _masm->test(ecx, Mem_Tag);				// if one of them is set then
  _masm->jcc(Assembler::notZero, _smi_send_failure);	// arguments are not bot smis
  // edx: receiver
  // eax: argument
}


char* InterpreterGenerator::smi_add() {
  Label overflow;
  char* ep = entry_point();
  check_smi_tags();
  _masm->addl(eax, edx);
  _masm->jcc(Assembler::overflow, overflow);
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  jump_ebx();

  _masm->bind(overflow);
  // eax: argument + receiver
  // edx: receiver
  _masm->subl(eax, edx);
  _masm->jmp(_smi_send_failure);
  return ep;
}


char* InterpreterGenerator::smi_sub() {
  Label overflow;
  char* ep = entry_point();
  check_smi_tags();
  _masm->subl(edx, eax);
  _masm->jcc(Assembler::overflow, overflow);
  advance_aligned(1 + 2*oopSize);
  _masm->movl(eax, edx);
  load_ebx();
  jump_ebx();

  _masm->bind(overflow);
  // eax: argument
  // edx: receiver - argument
  _masm->addl(edx, eax);
  _masm->jmp(_smi_send_failure);
  return ep;
}


char* InterpreterGenerator::smi_mul() {
  Label overflow;
  char* ep = entry_point();
  check_smi_tags();
  _masm->movl(ecx, eax);				// save argument for overflow case
  _masm->sarl(edx, Tag_Size);
  _masm->imull(eax, edx);
  _masm->jcc(Assembler::overflow, overflow);
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  jump_ebx();

  _masm->bind(overflow);
  // eax: argument * (receiver >> Tag_Size)
  // ecx: argument
  // edx: receiver >> Tag_Size
  _masm->movl(eax, ecx);				// restore argument
  _masm->shll(edx, Tag_Size);				// undo shift
  _masm->jmp(_smi_send_failure);
  return ep;
}


char* InterpreterGenerator::smi_compare_op(Bytecodes::Code code) {
  Label is_true;
  char* ep = entry_point();
  check_smi_tags();
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  _masm->cmpl(edx, eax);
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
  _masm->jcc(cc, is_true);
  _masm->movl(eax, false_addr());
  jump_ebx();

  _masm->bind(is_true);
  _masm->movl(eax, true_addr());
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::smi_logical_op(Bytecodes::Code code) {
  char* ep = entry_point();
  check_smi_tags();
  advance_aligned(1 + 2*oopSize);
  load_ebx();
  switch (code) {
    case Bytecodes::smi_and: _masm->andl(eax, edx); break;
    case Bytecodes::smi_or : _masm->orl (eax, edx); break;
    case Bytecodes::smi_xor: _masm->xorl(eax, edx); break;
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
  _masm->sarl(eax, Tag_Size);				// convert argument (shift count) into int (sets zero flag)
  _masm->movl(ecx, eax);				// move shift count into CL
  _masm->jcc(Assembler::negative, shift_right);		// shift right or shift left?

  // shift left
  _masm->shll(edx);					// else receiver << (argument mod 32)
  _masm->movl(eax, edx);				// set result
  jump_ebx();
  
  // shift right
  _masm->bind(shift_right);
  _masm->negl(ecx);
  _masm->sarl(edx);					// receiver >> (argument mod 32)
  _masm->andl(edx, -1 << Tag_Size);			// clear tag bits
  _masm->movl(eax, edx);				// set result
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
  _masm->leave();
  switch (arg_spec) {
    case Bytecodes::recv_0_args: _masm->ret(0*oopSize); break;
    case Bytecodes::recv_1_args: _masm->ret(1*oopSize); break;
    case Bytecodes::recv_2_args: _masm->ret(2*oopSize); break;
    case Bytecodes::recv_n_args: {
      // no. of arguments is in the next byte
      _masm->movb(ebx, Address(esi, 1));			// get no. of arguments
      _masm->popl(ecx);						// get return address
      _masm->leal(esp, Address(esp, ebx, Address::times_4));	// adjust esp (remove arguments)
      _masm->jmp(ecx);						// return
      break;
    }
    default: ShouldNotReachHere();
  }
}


//-----------------------------------------------------------------------------------------
// Error handling
//
// Entry points for run-time errors. eax must contain the value it had
// before starting to execute the instruction that issued the error,
// esi must point to the next instruction.

extern "C" void suspend_on_error(InterpreterErrorConstants error_code);

void InterpreterGenerator::generate_error_handler_code() {
  assert(!_boolean_expected.is_bound(), "code has been generated before");

  Label suspend, call_suspend;
  // eax: top of expression stack
  // ecx: error code
  // esi: points to next instruction
  _masm->bind(suspend);
  _masm->pushl(eax);				// save tos
  call_C(call_suspend);
  should_not_reach_here();

  _masm->bind(call_suspend);			// extra stack frame to pass error code in C land
  _masm->pushl(ecx);				// pass error code
  _masm->call((char*)suspend_on_error, relocInfo::runtime_call_type);
  should_not_reach_here();
  
  _masm->bind(_boolean_expected);
  _masm->movl(ecx, boolean_expected);
  _masm->jmp(suspend);

  _masm->bind(_float_expected);
  _masm->movl(ecx, float_expected);
  _masm->jmp(suspend);

  _masm->bind(_NLR_to_dead_frame);
  _masm->movl(ecx, nonlocal_return_error);
  _masm->jmp(suspend);

  _masm->bind(_halted);
  _masm->movl(ecx, halted);
  _masm->jmp(suspend);
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
  _masm->bind(zapped_context);
  _masm->popl(eax);				// get NLR result back
  _masm->addl(esi, 2);				// adjust esi (must point to next instruction)
  _masm->jmp(_NLR_to_dead_frame);

  _masm->bind(_issue_NLR);
  _masm->pushl(eax);				// make sure context (temp0) is in memory
  _masm->movl(edi, context_addr());		// get context
  if (_debug) {
    // should check here if edx is really a context
  }

  // find home stack frame by following the context chain
  // edi: current context in chain
  _masm->bind(loop);				// repeat
  _masm->movl(eax, edi);			//   eax := last context used
  _masm->movl(edi, Address(edi, contextOopDesc::parent_byte_offset()));
  _masm->test(edi, Mem_Tag);			//   edi := edi.home
  _masm->jcc(Assembler::notZero, loop);		// until is_smi(edi)
  _masm->testl(edi, edi);			// if edi = 0 then
  _masm->jcc(Assembler::zero, zapped_context);	//   context has been zapped
  _masm->movl(Address(int(&nlr_home_context), relocInfo::external_word_type), eax); 
                                                // else save the context containing the home (edi points to home stack frame)
  _masm->movb(ebx, Address(esi, 1));		// get no. of arguments to pop
  _masm->popl(eax);				// get NLR result back
  _masm->movl(esi, ebx);			// keep no. of arguments in esi
  _masm->notl(esi);				// make negative to distinguish from compiled NLRs
  
  // entry point for all methods to do NLR test & continuation,
  // first check if context zap is necessary
  //
  // eax: NLR result
  // edi: NLR home
  // esi: no. of arguments to pop (1s complement)
  nlr_testpoint_entry = _masm->pc();
  _masm->bind(_nlr_testpoint);

  // check if esi is indeed negative, otherwise this would be a NLR with
  // target in compiled code, which would be a bug - leave it in here to
  // find deoptimization bugs (request from Lars)
  //_masm->testl(esi, esi);
  //_masm->jcc(Assembler::positive, compiled_code_NLR);

  _masm->movl(ecx, context_addr());		// get potential context
  _masm->test(ecx, Mem_Tag);			// if is_smi(ecx) then
  _masm->jcc(Assembler::zero, no_zapping);	//   can't be a context pointer
  _masm->movl(edx, Address(ecx, memOopDesc::klass_byte_offset()));	// else isOop: get its class
  _masm->cmpl(edx, contextKlass_addr());	// if class # contextKlass then
  _masm->jcc(Assembler::notEqual, no_zapping);	//   is not a context
  _masm->movl(ebx, Address(ecx, contextOopDesc::parent_byte_offset()));	// else is context: get home
  _masm->cmpl(ebx, ebp);			// if home # ebp then
  _masm->jcc(Assembler::notEqual, no_zapping);	//   is not a methoc context
  _masm->movl(Address(ecx, contextOopDesc::parent_byte_offset()), 0);	// else method context: zap home

  _masm->bind(no_zapping);
  _masm->cmpl(edi, ebp);
  _masm->jcc(Assembler::notEqual, StubRoutines::continue_NLR_entry());

  // home found
  // eax: NLR result
  // edi: NLR home
  // esi: no. of arguments to pop (1s complement)
  restore_ebx();				// make sure ebx = 0
  _masm->leave();				// remove stack frame
  _masm->popl(ecx);				// get return address
  _masm->notl(esi);				// make positive again
  _masm->leal(esp, Address(esp, esi, Address::times_4));	// pop arguments
  _masm->jmp(ecx);				// return

  // error handler for compiled code NLRs - can be removed as soon
  // as that test has been removed. For now just use magic imm32 to
  // indicate this problem.
  //_masm->bind(compiled_code_NLR);
  //_masm->hlt();
  //_masm->testl(eax, 0x0badcafe);
  //_masm->testl(eax, 0x0badcafe);
  //_masm->testl(eax, 0x0badcafe);
  //_masm->testl(eax, 0x0badcafe);
}


char* InterpreterGenerator::nonlocal_return_tos() {
  char* ep = entry_point();
  _masm->jmp(_issue_NLR);
  return ep;
}


char* InterpreterGenerator::nonlocal_return_self() {
  char* ep = entry_point();
  _masm->pushl(eax);
  _masm->movl(eax, self_addr());
  _masm->jmp(_issue_NLR);
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

char* InterpreterGenerator::normal_send(Bytecodes::Code code, bool allow_methodOop, bool allow_nmethod) {
  assert(allow_methodOop || allow_nmethod, "must allow at least one method representation");

  Label is_smi, compare_class, is_methodOop, is_nmethod;

  Bytecodes::ArgumentSpec arg_spec = Bytecodes::argument_spec(code);
  bool pop_tos = Bytecodes::pop_tos(code);

  // inline cache layout
  int     length      = (arg_spec == Bytecodes::recv_n_args ? 2 : 1) + 2*oopSize;
  Address method_addr = Address(esi, -2*oopSize);
  Address klass_addr  = Address(esi, -1*oopSize);
  
  _masm->bind(is_smi);				// smi case (assumed to be infrequent)
  _masm->movl(edi, smiKlass_addr());		// load smi klass
  _masm->jmp(compare_class);

  char* ep = entry_point();
  load_recv(arg_spec);
  advance_aligned(length);
  _masm->test(eax, Mem_Tag);			// check if smi
  _masm->movl(ecx, method_addr);		// get cached method (assuming infrequent cache misses)
  _masm->movl(edx, klass_addr);			// get cached klass
  _masm->jcc(Assembler::zero, is_smi);
  _masm->movl(edi, Address(eax, memOopDesc::klass_byte_offset()));	// get recv class

  _masm->bind(compare_class);
  // eax: receiver
  // ebx: 000000xx (load_recv may modify bl)
  // ecx: cached selector/methodOop/nmethod
  // edx: cached klass
  // edi: receiver klass
  // esi: next instruction
  _masm->cmpl(edx, edi);			// compare with inline cache
  _masm->jcc(Assembler::notEqual, _inline_cache_miss);

  if (allow_methodOop && allow_nmethod) {
    // make case distinction at run-time
    _masm->test(ecx, Mem_Tag);			// check if nmethod
    _masm->jcc(Assembler::zero, is_nmethod);	// nmethods (jump table entries) are 4-byte alligned
  }

  if (allow_methodOop) {
    _masm->bind(is_methodOop);
    call_method();
    if (arg_spec != Bytecodes::args_only) _masm->popl(ecx);// discard receiver if on stack
    load_ebx();
    if (pop_tos) _masm->popl(eax);		// discard result if not used
    jump_ebx();
  }

  if (allow_nmethod) {
    _masm->bind(is_nmethod);
    call_native(ecx);
    if (arg_spec != Bytecodes::args_only) _masm->popl(ecx);// discard receiver if on stack
    load_ebx();
    if (pop_tos) _masm->popl(eax);		// discard result if not used
    jump_ebx();
  }

  return ep;
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

  _masm->bind(is_smi);				// smi case (assumed to be infrequent)
  _masm->movl(ecx, smiKlass_addr());		// load smi klass
  _masm->jmp(probe_primary_cache);

  char* ep = entry_point();
  load_recv(arg_spec);
  advance_aligned(length);
  _masm->test(eax, Mem_Tag);			// check if smi
  _masm->jcc(Assembler::zero, is_smi);		// otherwise
  _masm->movl(ecx, Address(eax, memOopDesc::klass_byte_offset()));	// get recv class

  // probe primary cache
  //
  // eax: receiver
  // ebx: 000000xx
  // ecx: receiver klass
  // esi: next instruction
  _masm->bind(probe_primary_cache);		// compute hash value
  _masm->movl(edx, selector_addr);		// get selector
  // compute hash value
  _masm->movl(edi, ecx);
  _masm->xorl(edi, edx);
  _masm->andl(edi, (primary_cache_size - 1) << 4);
  // probe cache
  _masm->cmpl(ecx, Address(edi, lookupCache::primary_cache_address() + 0*oopSize));
  _masm->jcc(Assembler::notEqual, probe_secondary_cache);
  _masm->cmpl(edx, Address(edi, lookupCache::primary_cache_address() + 1*oopSize));
  _masm->jcc(Assembler::notEqual, probe_secondary_cache);
  _masm->movl(ecx, Address(edi, lookupCache::primary_cache_address() + 2*oopSize));
  _masm->test(ecx, Mem_Tag);			// check if nmethod
  _masm->jcc(Assembler::zero, is_nmethod);	// nmethods (jump table entries) are 4-byte aligned

  _masm->bind(is_methodOop);
  call_method();
  if (arg_spec != Bytecodes::args_only) _masm->popl(ecx);// discard receiver if on stack
  load_ebx();
  if (pop_tos) _masm->popl(eax);		// discard result if not used
  jump_ebx();

  _masm->bind(is_nmethod);
  call_native(ecx);
  if (arg_spec != Bytecodes::args_only) _masm->popl(ecx);// discard receiver if on stack
  load_ebx();
  if (pop_tos) _masm->popl(eax);		// discard result if not used
  jump_ebx();

  // probe secondary cache
  //
  // eax: receiver
  // ebx: 000000xx
  // ecx: receiver klass
  // edx: selector
  // edi: primary cache index
  // esi: next instruction
  _masm->bind(probe_secondary_cache);
  _masm->andl(edi, (secondary_cache_size - 1) << 4);
  // probe cache
  _masm->cmpl(ecx, Address(edi, lookupCache::secondary_cache_address() + 0*oopSize));
  _masm->jcc(Assembler::notEqual, _inline_cache_miss);
  _masm->cmpl(edx, Address(edi, lookupCache::secondary_cache_address() + 1*oopSize));
  _masm->jcc(Assembler::notEqual, _inline_cache_miss);
  _masm->movl(ecx, Address(edi, lookupCache::secondary_cache_address() + 2*oopSize));
  _masm->test(ecx, Mem_Tag);			// check if nmethod
  _masm->jcc(Assembler::zero, is_nmethod);	// nmethods (jump table entries) are 4-byte aligned
  _masm->jmp(is_methodOop);

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
  _masm->movl(ebx, pic_addr);			// get pic
  _masm->movl(ecx, Address(ebx, length_offset));// get pic length (smi)
  _masm->sarl(ecx, Tag_Size + 1);		// get pic length (int)
  // verifyPIC here

  _masm->movl(edx, smiKlass_addr());		// preload smi klass
  _masm->testl(eax, Mem_Tag);			// check if smi
  _masm->jcc(Assembler::zero, loop);		// otherwise
  _masm->movl(edx, Address(eax, memOopDesc::klass_byte_offset()));	// get receiver klass

  // search pic for appropriate entry
  _masm->bind(loop);
  // eax: receiver
  // ebx: pic (objArrayOop)
  // ecx: counter
  // edx: receiver class
  // esi: next instruction
  _masm->cmpl(edx, Address(ebx, ecx, Address::times_8, data_offset - 1*oopSize, relocInfo::none));
  _masm->jcc(Assembler::equal, found);
  _masm->decl(ecx);
  _masm->jcc(Assembler::notZero, loop);

  // cache miss
  _masm->jmp(_inline_cache_miss);

  _masm->bind(found);
  // eax: receiver
  // ebx: pic (objArrayOop)
  // ecx: counter (> 0)
  // edx: receiver class
  // esi: next instruction
  _masm->movl(ecx, Address(ebx, ecx, Address::times_8, data_offset - 2*oopSize, relocInfo::none));
  _masm->testl(ecx, Mem_Tag);
  _masm->jcc(Assembler::zero, is_nmethod);
  restore_ebx();

  // methodOop found
  call_method();
  if (arg_spec != Bytecodes::args_only) _masm->popl(ecx);	// discard receiver if on stack
  load_ebx();
  if (pop_tos) _masm->popl(eax);				// discard result if not used
  jump_ebx();

  // nmethod found
  _masm->bind(is_nmethod);
  call_native(ecx);
  if (arg_spec != Bytecodes::args_only) _masm->popl(ecx);	// discard receiver if on stack
  load_ebx();
  if (pop_tos) _masm->popl(eax);				// discard result if not used
  jump_ebx();

  return ep;
}


//-----------------------------------------------------------------------------------------
// Miscellaneous

char* InterpreterGenerator::special_primitive_send_hint() {
  char* ep = entry_point();
  assert(Bytecodes::format(Bytecodes::special_primitive_send_1_hint) == Bytecodes::BB, "unexpected format");
  _masm->addl(esi, 2);				// simply skip this instruction
  load_ebx();
  jump_ebx();
  return ep;
}


char* InterpreterGenerator::halt() {
  char* ep = entry_point();
  _masm->incl(esi);				// advance to next instruction
  _masm->jmp(_halted);
  return ep;
}


//-----------------------------------------------------------------------------------------
// Instruction generator

char* InterpreterGenerator::generate_instruction(Bytecodes::Code code) {
  const bool pop		= true; // for readability
  const bool returns_float	= true; // for readability

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
    case Bytecodes::push_temp_0_context_0		: return push_context_temp(0, 0);
    case Bytecodes::push_temp_1_context_0		: return push_context_temp(0, 1);
    case Bytecodes::push_temp_2_context_0		: return push_context_temp(0, 2);

    case Bytecodes::push_temp_0_context_1		: return push_context_temp(1, 0);
    case Bytecodes::push_temp_1_context_1		: return push_context_temp(1, 1);
    case Bytecodes::push_temp_2_context_1		: return push_context_temp(1, 2);
    
    // self/context initialization
    case Bytecodes::set_self_via_context		: return set_self_via_context();

    // block closure allocation
    case Bytecodes::push_new_closure_context_0		: return push_closure((char*)allocateBlock0, true);
    case Bytecodes::push_new_closure_context_1		: return push_closure((char*)allocateBlock1, true);
    case Bytecodes::push_new_closure_context_2		: return push_closure((char*)allocateBlock2, true);
    case Bytecodes::push_new_closure_context_n		: return push_closure((char*)allocateBlock , true);

    case Bytecodes::push_new_closure_tos_0		: return push_closure((char*)allocateBlock0, false);
    case Bytecodes::push_new_closure_tos_1		: return push_closure((char*)allocateBlock1, false);
    case Bytecodes::push_new_closure_tos_2		: return push_closure((char*)allocateBlock2, false);
    case Bytecodes::push_new_closure_tos_n		: return push_closure((char*)allocateBlock , false);
    
    // context allocation
    case Bytecodes::install_new_context_method_0	: return install_context((char*)allocateContext0, true);
    case Bytecodes::install_new_context_method_1	: return install_context((char*)allocateContext1, true);
    case Bytecodes::install_new_context_method_2	: return install_context((char*)allocateContext2, true);
    case Bytecodes::install_new_context_method_n	: return install_context((char*)allocateContext , true);

    case Bytecodes::install_new_context_block_1		: return install_context((char*)allocateContext1, false);
    case Bytecodes::install_new_context_block_2		: return install_context((char*)allocateContext2, false);
    case Bytecodes::install_new_context_block_n		: return install_context((char*)allocateContext , false);

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

    // returns
    case Bytecodes::non_local_return_tos_pop_n		: return nonlocal_return_tos();
    case Bytecodes::non_local_return_self_pop_n		: return nonlocal_return_self();

    // globals
    case Bytecodes::push_global				: return push_global();
    case Bytecodes::store_global_pop			: return store_global(pop);
    case Bytecodes::store_global			: return store_global();

    case Bytecodes::push_classVar			: return push_global();		// same as for globals
    case Bytecodes::store_classVar_pop			: return store_global(pop);	// same as for globals
    case Bytecodes::store_classVar			: return store_global();	// same as for globals

    case Bytecodes::halt				: return halt();

    // miscellaneous
    case Bytecodes::special_primitive_send_1_hint	: return special_primitive_send_hint();
    default						: return NULL;
  }
  ShouldNotReachHere();
}


void InterpreterGenerator::info(char* name) {
  if (!Disclaimer::is_product() && PrintInterpreter) {
    std->print("%s\n", name);
    _masm->code()->decode();
    std->cr();
  }
}


void InterpreterGenerator::generate_all() {
  Interpreter::_code_begin_addr = _masm->pc();

  // generate code for Floats
  Floats::init(_masm);
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

  // generate individual instructions
  for (int i = 0; i < Bytecodes::number_of_codes; i++) {
    char* start_point = _masm->pc();
    char* entry_point = generate_instruction((Bytecodes::Code)i);
    if (entry_point != NULL) {
      // bytecode implemented
      Bytecodes::set_entry_point(Bytecodes::Code(i), entry_point);
      if (!Disclaimer::is_product() && PrintInterpreter) {
      #ifndef PRODUCT
        int length = _masm->pc() - start_point;
	char* name = Bytecodes::name((Bytecodes::Code)i);
        std->print("Bytecode 0x%02x: %s (%d bytes), entry point = 0x%x\n", i, name, length, entry_point);
	_masm->code()->decode();
	std->cr();
      #endif
      }
    }
  }

  _masm->finalize();
  Interpreter::_code_end_addr = _masm->pc();
}


InterpreterGenerator::InterpreterGenerator(CodeBuffer* code, bool debug) {
  _masm = new MacroAssembler(code);
  _debug = debug;
  generate_all();
}


// Interpreter initialization

static const int interpreter_size = 40000;
static char interpreter_code[interpreter_size];


void interpreter_init() {
  const bool debug = false; // change this to switch between debug/optimized version
  if (!Disclaimer::is_product() && PrintInterpreter) {
    std->print("\nBytecode Interpreter\n\n");
  }

  CodeBuffer* code = new CodeBuffer(&interpreter_code[0], interpreter_size);
  InterpreterGenerator g(code, debug);

  if (!Disclaimer::is_product() && PrintInterpreter) {
    std->print("%d bytes generated for the interpreter\n", code->code_size());
    exit(0);
  }

  Interpreter::init();
}
