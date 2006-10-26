/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.12 $ */
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
# include "incls/_floats.cpp.incl"


char* Floats::_function_table[max_number_of_functions];
char* Floats::_function_names[] = {
  "zero", "one",
  "abs", "negated", "squared", "sqrt", "sin", "cos", "tan", "exp", "ln",
  "add", "subtract", "multiply", "divide", "modulo",
  "is_zero", "is_not_zero", "oopify",
  "is_equal", "is_not_equal", "is_less", "is_less_equal", "is_greater", "is_greater_equal"
};


char* Floats::function_name_for(Function f) {
  assert(0 <= f && f < number_of_functions, "illegal function");
  return _function_names[f];
}


bool Floats::has_selector_for(Function f) {
  if (f == undefined)	return false;
  if (f == zero)	return false;
  if (f == one)		return false;
  return true;
}


symbolOop Floats::selector_for(Function f) {
  switch(f) {
    case abs		 : return vmSymbols::abs();			break;
    case negated	 : return vmSymbols::negated();			break;
    case squared	 : return vmSymbols::squared();			break;
    case sqrt		 : return vmSymbols::sqrt();			break;
    case sin		 : return vmSymbols::sin();			break;
    case cos		 : return vmSymbols::cos();			break;
    case tan		 : return vmSymbols::tan();			break;
    case exp		 : return vmSymbols::exp();			break;
    case ln		 : return vmSymbols::ln();			break;
    case add		 : return vmSymbols::plus();			break;
    case subtract	 : return vmSymbols::minus();			break;
    case multiply	 : return vmSymbols::multiply();		break;
    case divide		 : return vmSymbols::divide();			break;
    case modulo		 : return vmSymbols::mod();			break;
    case is_zero	 : return vmSymbols::equal();			break;
    case is_not_zero	 : return vmSymbols::not_equal();		break;
    case oopify		 : return vmSymbols::as_float();		break;
    case is_equal	 : return vmSymbols::equal();			break;
    case is_not_equal	 : return vmSymbols::not_equal();		break;
    case is_less	 : return vmSymbols::less_than();		break;
    case is_less_equal	 : return vmSymbols::less_than_or_equal();	break;
    case is_greater	 : return vmSymbols::greater_than();		break;
    case is_greater_equal: return vmSymbols::greater_than_or_equal();	break;
    case floatify	 : return vmSymbols::as_float_value();		break;
  }
  return NULL;
}


// Initialization/debugging

void Floats::generate_tst(MacroAssembler* masm, Assembler::Condition cc) {
  int mask;
  Assembler::Condition cond;
  MacroAssembler::fpu_mask_and_cond_for(cc, mask, cond);
  Label L;
  masm->int3();	// remove this if it works
  masm->ftst();
  masm->fwait();
  masm->fnstsw_ax();
  masm->fpop();		// explicitly pop argument
  masm->testl(eax, mask);
  masm->movl(eax, Address((int)&trueObj, relocInfo::external_word_type));
  masm->jcc(cond, L);
  masm->movl(eax, Address((int)&falseObj, relocInfo::external_word_type));
  masm->bind(L);
}


void Floats::generate_cmp(MacroAssembler* masm, Assembler::Condition cc) {
  int mask;
  Assembler::Condition cond;
  MacroAssembler::fpu_mask_and_cond_for(cc, mask, cond);
  Label L;
  masm->fcompp();
  masm->fwait();
  masm->fnstsw_ax();
  masm->testl(eax, mask);
  masm->movl(eax, Address((int)&trueObj, relocInfo::external_word_type));
  masm->jcc(cond, L);
  masm->movl(eax, Address((int)&falseObj, relocInfo::external_word_type));
  masm->bind(L);
}


void Floats::generate(MacroAssembler* masm, Function f) {
  char* entry_point = masm->pc();
  switch (f) {
    // nullary functions
    case zero		 : masm->fldz();				break;
    case one		 : masm->fld1();				break;

    // unary functions
    case abs		 : masm->fabs();				break;
    case negated	 : masm->fchs();				break;
    case squared	 : masm->fmul(0);				break;
    case sqrt		 : masm->int3(); /* Unimplemented */		break;
    case sin		 : masm->int3(); /* Unimplemented */		break;
    case cos		 : masm->int3(); /* Unimplemented */		break;
    case tan		 : masm->int3(); /* Unimplemented */		break;
    case exp		 : masm->int3(); /* Unimplemented */		break;
    case ln		 : masm->int3(); /* Unimplemented */		break;

    // binary functions
    case add		 : masm->faddp();				break;
    case subtract	 : masm->fsubp();				break;
    case multiply	 : masm->fmulp();				break;
    case divide		 : masm->fdivp();				break;
    case modulo		 : masm->fxch(); masm->fprem();			break;

    // unary functions to oop
    case is_zero	 : generate_tst(masm, Assembler::zero);		break;
    case is_not_zero	 : generate_tst(masm, Assembler::notZero);	break;
    case oopify		 : masm->hlt();	/* see InterpreterGenerator */	break;

    // binary functions to oop
    // (Note: This is comparing ST(1) with ST while the bits in the FPU
    //        status word (assume comparison of ST with ST(1) -> reverse
    //        the conditions).
    case is_equal	 : generate_cmp(masm, Assembler::equal);	break;
    case is_not_equal	 : generate_cmp(masm, Assembler::notEqual);	break;
    case is_less	 : generate_cmp(masm, Assembler::greater);	break;
    case is_less_equal	 : generate_cmp(masm, Assembler::greaterEqual);	break;
    case is_greater	 : generate_cmp(masm, Assembler::less);		break;
    case is_greater_equal: generate_cmp(masm, Assembler::lessEqual);	break;

    default		 : ShouldNotReachHere();
  }
  masm->ret(0);
  _function_table[f] = entry_point;

  if (!Disclaimer::is_product() && PrintInterpreter) {
    int length = masm->pc() - entry_point;
    char* name = function_name_for(f);
    std->print("Float function %d: %s (%d bytes), entry point = 0x%x\n", f, name, length, entry_point);
    masm->code()->decode();
    std->cr();
  }
}


bool Floats::_is_initialized = false;

void Floats::init(MacroAssembler* masm) {
  if (is_initialized()) return;

  if (sizeof(_function_names) / sizeof(char*) != number_of_functions) {
    fatal("Floats: number of _functions_names not equal number_of_functions");
  }

  // pre-initialize whole table
  // (make sure there's an entry for each index so that illegal indices
  // can be caught during execution without additional index range check)
  for (int i = max_number_of_functions; i-- > 0;) _function_table[i] = masm->pc();
  masm->hlt();
  if (!Disclaimer::is_product() && PrintInterpreter) {
    std->print("Undefined float functions entry point\n");
    masm->code()->decode();
    std->cr();
  }

  // nullary functions
  generate(masm, zero);
  generate(masm, one);

  // unary functions
  generate(masm, abs);
  generate(masm, negated);
  generate(masm, squared);
  generate(masm, sqrt);
  generate(masm, sin);
  generate(masm, cos);
  generate(masm, tan);
  generate(masm, exp);
  generate(masm, ln);
  
  // binary functions
  generate(masm, add);
  generate(masm, subtract);
  generate(masm, multiply);
  generate(masm, divide);
  generate(masm, modulo);
  
  // unary functions to oop
  generate(masm, oopify);
  generate(masm, is_zero);
  generate(masm, is_not_zero);
  
  // binary functions to oop
  generate(masm, is_equal);
  generate(masm, is_not_equal);
  generate(masm, is_less);
  generate(masm, is_less_equal);
  generate(masm, is_greater);
  generate(masm, is_greater_equal);
  
  _is_initialized = true;
}


void Floats::print() {
  if (_is_initialized) {
    std->print_cr("Float functions:");
    for (int i = 0; i < number_of_functions; i++) {
      std->print("%3d: 0x%x %s\n", i, _function_table[i], function_name_for(Function(i)));
    }
  } else {
    std->print_cr("Floats not yet initialized");
  }
  std->cr();
}
