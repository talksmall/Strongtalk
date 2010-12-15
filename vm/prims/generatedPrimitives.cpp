/* Copyright (c) 2007, Strongtalk Group
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
	  disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the Strongtalk Group nor the names of its contributors may be used to endorse or promote products derived 
	  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT 
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE


*/


# include "incls/_precompiled.incl"
# include "incls/_generatedPrimitives.cpp.incl"

// entry points
char* GeneratedPrimitives::_allocateContext_var			= NULL;

char* GeneratedPrimitives::_smiOopPrimitives_add		= NULL;
char* GeneratedPrimitives::_smiOopPrimitives_subtract		= NULL;
char* GeneratedPrimitives::_smiOopPrimitives_multiply		= NULL;
char* GeneratedPrimitives::_smiOopPrimitives_mod		= NULL;
char* GeneratedPrimitives::_smiOopPrimitives_div		= NULL;
char* GeneratedPrimitives::_smiOopPrimitives_quo		= NULL;
char* GeneratedPrimitives::_smiOopPrimitives_remainder		= NULL;

char* GeneratedPrimitives::_double_add				= NULL;
char* GeneratedPrimitives::_double_subtract			= NULL;
char* GeneratedPrimitives::_double_multiply			= NULL;
char* GeneratedPrimitives::_double_divide			= NULL;
char* GeneratedPrimitives::_double_from_smi			= NULL;

char* GeneratedPrimitives::_primitiveValue[10];
char* GeneratedPrimitives::_primitiveNew[10];
char* GeneratedPrimitives::_allocateBlock[10];
char* GeneratedPrimitives::_allocateContext[3];
char* GeneratedPrimitives::_primitiveInlineAllocations          = NULL;

extern "C" void scavenge_and_allocate(int size);

// macros

void PrimitivesGenerator::scavenge(int size) {
  masm->set_last_Delta_frame_after_call();
  masm->pushl(size);
  masm->call((char*)&scavenge_and_allocate, relocInfo::runtime_call_type);
  masm->addl(esp, 4);
  masm->reset_last_Delta_frame();
  masm->addl(eax, size * oopSize);
}

void PrimitivesGenerator::test_for_scavenge(Register dst, int size, Label& need_scavenge) {
  masm->movl(dst, Address((int)&eden_top, relocInfo::external_word_type));
  masm->addl(dst, size);
  masm->cmpl(dst, Address((int)&eden_end, relocInfo::external_word_type));
  masm->jcc(Assembler::greater, need_scavenge);
  masm->movl(Address((int)&eden_top, relocInfo::external_word_type), dst);
}

void PrimitivesGenerator::error_jumps() {
  
#define VMSYMBOL_POSTFIX  _enum
#define VMSYMBOL_ENUM_NAME(name) name##VMSYMBOL_POSTFIX
  
  Address _smi_overflow				= Address((int)&vm_symbols[VMSYMBOL_ENUM_NAME(smi_overflow)],			relocInfo::external_word_type);
  Address _division_by_zero			= Address((int)&vm_symbols[VMSYMBOL_ENUM_NAME(division_by_zero)],		relocInfo::external_word_type);
  Address _receiver_has_wrong_type		= Address((int)&vm_symbols[VMSYMBOL_ENUM_NAME(receiver_has_wrong_type)],	relocInfo::external_word_type);
  Address _division_not_exact			= Address((int)&vm_symbols[VMSYMBOL_ENUM_NAME(division_not_exact)],		relocInfo::external_word_type);
  Address _first_argument_has_wrong_type	= Address((int)&vm_symbols[VMSYMBOL_ENUM_NAME(first_argument_has_wrong_type)],	relocInfo::external_word_type);
  Address _allocation_failure           	= Address((int)&vm_symbols[VMSYMBOL_ENUM_NAME(failed_allocation)],  	        relocInfo::external_word_type);
  
#undef  VMSYMBOL_POSTFIX
#undef  VMSYMBOL_ENUM_NAME
  
  masm->bind(error_receiver_has_wrong_type);
  masm->movl(eax, _receiver_has_wrong_type);
  masm->addl(eax, 2);
  masm->ret(2 * oopSize);
  masm->bind(error_first_argument_has_wrong_type);
  masm->movl(eax, _first_argument_has_wrong_type);
  masm->addl(eax, 2);
  masm->ret(2 * oopSize);
  masm->bind(error_overflow);
  masm->movl(eax, _smi_overflow);
  masm->addl(eax, 2);
  masm->ret(2 * oopSize);
  masm->bind(error_division_by_zero);
  masm->movl(eax, _division_by_zero);
  masm->addl(eax, 2);
  masm->ret(2 * oopSize);
  masm->bind(allocation_failure);
  masm->movl(eax, _allocation_failure);
  masm->addl(eax, 2);
  masm->ret(2 * oopSize);
}

// generators are in xxx_prims_gen.cpp files

void GeneratedPrimitives::set_primitiveValue(int n, char* entry_point) {
  assert(0 <= n && n <= 9, "index out of range")
  _primitiveValue[n] = entry_point;
}

// Parametrized accessors


char* GeneratedPrimitives::primitiveValue(int n) {
  assert(0 <= n && n <= 9, "index out of range")
  assert(_primitiveValue[n], "primitiveValues not initialized yet");
  return _primitiveValue[n];
}


extern "C" oop primitiveNew0(oop);
extern "C" oop primitiveNew1(oop);
extern "C" oop primitiveNew2(oop);
extern "C" oop primitiveNew3(oop);
extern "C" oop primitiveNew4(oop);
extern "C" oop primitiveNew5(oop);
extern "C" oop primitiveNew6(oop);
extern "C" oop primitiveNew7(oop);
extern "C" oop primitiveNew8(oop);
extern "C" oop primitiveNew9(oop);

char* GeneratedPrimitives::primitiveNew(int n) {
  assert(_is_initialized, "GeneratedPrimitives not initialized yet");
  assert(0 <= n && n <= 9, "index out of range")
    return _primitiveNew[n];
  
  switch(n) {
  case 0: return (char*)::primitiveNew0;
  case 1: return (char*)::primitiveNew1;
  case 2: return (char*)::primitiveNew2;
  case 3: return (char*)::primitiveNew3;
  case 4: return (char*)::primitiveNew4;
  case 5: return (char*)::primitiveNew5;
  case 6: return (char*)::primitiveNew6;
  case 7: return (char*)::primitiveNew7;
  case 8: return (char*)::primitiveNew8;
  case 9: return (char*)::primitiveNew9;
  default: ShouldNotReachHere();
  }
  ShouldNotReachHere();
}

extern "C" blockClosureOop allocateBlock(smiOop nofArgs);
extern "C" blockClosureOop allocateBlock0();
extern "C" blockClosureOop allocateBlock1();
extern "C" blockClosureOop allocateBlock2();
extern "C" blockClosureOop allocateBlock3();
extern "C" blockClosureOop allocateBlock4();
extern "C" blockClosureOop allocateBlock5();
extern "C" blockClosureOop allocateBlock6();
extern "C" blockClosureOop allocateBlock7();
extern "C" blockClosureOop allocateBlock8();
extern "C" blockClosureOop allocateBlock9();

char* GeneratedPrimitives::allocateBlock(int n) {
  assert(_is_initialized, "GeneratedPrimitives not initialized yet");
  if (n == -1) return (char*)::allocateBlock;		// convenience
  assert(0 <= n && n <= 9, "index out of range")
    return _allocateBlock[n];
    
    switch(n) {
    case 0: return (char*)::allocateBlock0;
    case 1: return (char*)::allocateBlock1;
    case 2: return (char*)::allocateBlock2;
    case 3: return (char*)::allocateBlock3;
    case 4: return (char*)::allocateBlock4;
    case 5: return (char*)::allocateBlock5;
    case 6: return (char*)::allocateBlock6;
    case 7: return (char*)::allocateBlock7;
    case 8: return (char*)::allocateBlock8;
    case 9: return (char*)::allocateBlock9;
    default: ShouldNotReachHere();
  }
  ShouldNotReachHere();
}

extern "C" contextOop allocateContext(smiOop nofVars);
extern "C" contextOop allocateContext0();
extern "C" contextOop allocateContext1();
extern "C" contextOop allocateContext2();

char* GeneratedPrimitives::allocateContext(int n) {
  assert(_is_initialized, "GeneratedPrimitives not initialized yet");
    if (n == -1) return _allocateContext_var;		// convenience
  if (n == -1) return (char*)::allocateContext;		// convenience
  assert(0 <= n && n <= 2, "index out of range")
    return _allocateContext[n];
  
  switch(n) {
  case 0: return (char*)::allocateContext0;
  case 1: return (char*)::allocateContext1;
  case 2: return (char*)::allocateContext2;
  default: ShouldNotReachHere();
  }
  ShouldNotReachHere();
}

// Initialization

bool GeneratedPrimitives::_is_initialized = false;
//char GeneratedPrimitives::_code[GeneratedPrimitives::_code_size];
char* GeneratedPrimitives::_code = NULL;

char* GeneratedPrimitives::patch(char* name, char* entry_point) {
  primitives::patch(name, entry_point);
  return entry_point;
}

char* GeneratedPrimitives::patch(char* name, char* entry_point, int argument) {
  char formated_name[100];
  assert(strlen(name) < 100, "primitive name longer the 100 characters - buffer overrun");
  sprintf(formated_name, name, argument);
  primitives::patch(formated_name, entry_point);
  return entry_point;
}


void GeneratedPrimitives::init() {
  if (_is_initialized) return;

  int n;
  ResourceMark rm;
  _code = os::exec_memory(_code_size);
  
  CodeBuffer* code = new CodeBuffer(_code, _code_size);
  MacroAssembler* masm = new MacroAssembler(code);
  PrimitivesGenerator gen(masm);

  // add generators here

  gen.error_jumps();
  _smiOopPrimitives_add 	= patch("primitiveAdd:ifFail:", 		gen.smiOopPrimitives_add());
  _smiOopPrimitives_subtract 	= patch("primitiveSubtract:ifFail:", 		gen.smiOopPrimitives_subtract());
  _smiOopPrimitives_multiply 	= patch("primitiveMultiply:ifFail:", 		gen.smiOopPrimitives_multiply());
  _smiOopPrimitives_mod 	= patch("primitiveMod:ifFail:", 		gen.smiOopPrimitives_mod());
  _smiOopPrimitives_div 	= patch("primitiveDiv:ifFail:", 		gen.smiOopPrimitives_div());
  _smiOopPrimitives_quo 	= patch("primitiveQuo:ifFail:", 		gen.smiOopPrimitives_quo());
  _smiOopPrimitives_remainder 	= patch("primitiveRemainder:ifFail:", 		gen.smiOopPrimitives_remainder());
  
  PrimitivesGenerator::arith_op op_add = PrimitivesGenerator::op_add;
  PrimitivesGenerator::arith_op op_sub = PrimitivesGenerator::op_sub;
  PrimitivesGenerator::arith_op op_mul = PrimitivesGenerator::op_mul;
  PrimitivesGenerator::arith_op op_div = PrimitivesGenerator::op_div;
  _double_add 			= patch("primitiveFloatAdd:ifFail:", 		gen.double_op(op_add), op_add);
  _double_subtract 		= patch("primitiveFloatSubtract:ifFail:", 	gen.double_op(op_sub), op_sub);
  _double_multiply 		= patch("primitiveFloatMultiply:ifFail:", 	gen.double_op(op_mul), op_mul);
  _double_divide 		= patch("primitiveFloatDivide:ifFail:", 	gen.double_op(op_div), op_div);
  _double_from_smi 		= patch("primitiveAsFloat", 			gen.double_from_smi());

  for (n = 0; n <= 9; n++) {
    _primitiveNew[n] = patch("primitiveNew%1d:ifFail:", gen.primitiveNew(n), n);
  }

  for (n = 0; n <= 9; n++) {
    _allocateBlock[n] = patch("primitiveCompiledBlockAllocate%1d", gen.allocateBlock(n), n);
  }

  _allocateContext_var = patch("primitiveCompiledContextAllocate:", gen.allocateContext_var());

  for (n = 0; n <= 2; n++) {
    _allocateContext[n] = patch("primitiveCompiledContextAllocate%1d", gen.allocateContext(n), n);
  }
  
  _primitiveInlineAllocations = patch("primitiveInlineAllocations:count:", gen.inline_allocation());
  
  masm->finalize();
  _is_initialized = true;

};


// patch some primitives defined in the interpreter

void GeneratedPrimitives::patch_primitiveValue() {

  primitives::patch("primitiveValue", 							primitiveValue(0));
  primitives::patch("primitiveValue:", 							primitiveValue(1));
  primitives::patch("primitiveValue:value:", 						primitiveValue(2));
  primitives::patch("primitiveValue:value:value:", 					primitiveValue(3));
  primitives::patch("primitiveValue:value:value:value:", 				primitiveValue(4));
  primitives::patch("primitiveValue:value:value:value:value:", 				primitiveValue(5));
  primitives::patch("primitiveValue:value:value:value:value:value:",			primitiveValue(6));
  primitives::patch("primitiveValue:value:value:value:value:value:value:",		primitiveValue(7));
  primitives::patch("primitiveValue:value:value:value:value:value:value:value:",	primitiveValue(8));
  primitives::patch("primitiveValue:value:value:value:value:value:value:value:value:",	primitiveValue(9));  
  
}

typedef oop (__stdcall *smiOp)(oop,oop);
oop GeneratedPrimitives::invoke(char* op, oop receiver, oop argument) {
  assert(_is_initialized, "Generated primitives have not been initialized");
  return ((smiOp) op)(argument, receiver);
}
oop GeneratedPrimitives::smiOopPrimitives_add(oop receiver, oop argument) {
  return invoke(_smiOopPrimitives_add, receiver, argument);
}
oop GeneratedPrimitives::smiOopPrimitives_subtract(oop receiver, oop argument) {
  return invoke(_smiOopPrimitives_subtract, receiver, argument);
}
oop GeneratedPrimitives::smiOopPrimitives_multiply(oop receiver, oop argument) {
  return invoke(_smiOopPrimitives_multiply, receiver, argument);
}
oop GeneratedPrimitives::smiOopPrimitives_mod(oop receiver, oop argument) {
  return invoke(_smiOopPrimitives_mod, receiver, argument);
}
oop GeneratedPrimitives::smiOopPrimitives_div(oop receiver, oop argument) {
  return invoke(_smiOopPrimitives_div, receiver, argument);
}
oop GeneratedPrimitives::smiOopPrimitives_quo(oop receiver, oop argument) {
  return invoke(_smiOopPrimitives_quo, receiver, argument);
}
oop GeneratedPrimitives::smiOopPrimitives_remainder(oop receiver, oop argument) {
  return invoke(_smiOopPrimitives_remainder, receiver, argument);
}
void generatedPrimitives_init_before_interpreter() {
  GeneratedPrimitives::init();
}

void generatedPrimitives_init_after_interpreter() {
  GeneratedPrimitives::patch_primitiveValue();
}
