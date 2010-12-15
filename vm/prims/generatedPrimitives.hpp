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


// GeneratedPrimitives contains the assembly primitives.
// Instead of relying on an assembler, they are generated
// during system initialization.
//
// Steps to add a new stub primitive:
//
// 1. add a new entry point (class variable)
// 2. add the corresponding entry point accessor (class method)
// 3. implement the primitive code generator
// 4. call the generator in init()

class PrimitivesGenerator: StackObj {

  MacroAssembler* masm;			
  Address nil_addr()		{ return Address(int(&nilObj), relocInfo::external_word_type); }
  Address true_addr()		{ return Address(int(&trueObj), relocInfo::external_word_type); }
  Address false_addr()		{ return Address(int(&falseObj), relocInfo::external_word_type); }
  Address smiKlass_addr()	{ return Address(int(&smiKlassObj), relocInfo::external_word_type); }
  Address doubleKlass_addr()	{ return Address(int(&doubleKlassObj), relocInfo::external_word_type); }
  Address contextKlass_addr()	{ return Address(int(&contextKlassObj), relocInfo::external_word_type); }

  Label error_receiver_has_wrong_type;
  Label error_first_argument_has_wrong_type;
  Label error_division_by_zero;
  Label error_overflow;
  Label allocation_failure;

  void scavenge(int size);
  void test_for_scavenge(Register dst, int size, Label& need_scavenge);
  
 protected:
  PrimitivesGenerator(MacroAssembler* _masm) { masm = _masm; }

  enum arith_op {
      op_add,
      op_sub,
      op_mul,
      op_div
  };
  
  // add generators here
  void  error_jumps();

  char* smiOopPrimitives_add();
  char* smiOopPrimitives_subtract();
  char* smiOopPrimitives_multiply();
  char* smiOopPrimitives_mod();
  char* smiOopPrimitives_div();
  char* smiOopPrimitives_quo();
  char* smiOopPrimitives_remainder();
  
  char* double_op(arith_op op);
  char* double_from_smi();
  
  char* primitiveNew(int n);
  char* allocateBlock(int n);
  char* allocateContext_var();
  char* allocateContext(int n);
// slr perf testing
  char* inline_allocation();
//
  friend class GeneratedPrimitives;
};

class GeneratedPrimitives: AllStatic {
 private:
  enum { _code_size = 8192 };			// simply increase if too small (assembler will crash if too small)
  static bool _is_initialized;			// true if GeneratedPrimitives has been initialized
//  static char _code[_code_size];		// the code buffer for the primitives
  static char* _code;		// the code buffer for the primitives

  // add entry points here
  static char* _allocateContext_var;

  static char* _smiOopPrimitives_add;
  static char* _smiOopPrimitives_subtract;
  static char* _smiOopPrimitives_multiply;
  static char* _smiOopPrimitives_mod;
  static char* _smiOopPrimitives_div;
  static char* _smiOopPrimitives_quo;
  static char* _smiOopPrimitives_remainder;
  
  static char* _double_add;
  static char* _double_subtract;
  static char* _double_multiply;
  static char* _double_divide;
  static char* _double_from_smi;

  static char* _primitiveValue[];
  static char* _primitiveNew[];
  static char* _allocateBlock[];
  static char* _allocateContext[];
  static char* _primitiveInlineAllocations;

  // helpers for generation and patch
  static char* patch(char* name, char* entry_point);
  static char* patch(char* name, char* entry_point, int argument);
  static oop   invoke(char* op, oop receiver, oop argument);
  friend class PrimitivesGenerator;

 public:

  static void set_primitiveValue(int n, char* entry_point);

  // add entry point accessors here
  static char* primitiveValue(int n);
  static char* primitiveNew(int n);
  static char* allocateBlock(int n);
  static char* allocateContext(int n); 	// -1 for variable size

  // Support for profiling
  static bool contains(char* pc)            	{ return (_code <= pc) && (pc < &_code[_code_size]); }

  // Support for compiler constant folding
  static oop smiOopPrimitives_add(oop receiver, oop argument);
  static oop smiOopPrimitives_subtract(oop receiver, oop argument);
  static oop smiOopPrimitives_multiply(oop receiver, oop argument);
  static oop smiOopPrimitives_mod(oop receiver, oop argument);
  static oop smiOopPrimitives_div(oop receiver, oop argument);
  static oop smiOopPrimitives_quo(oop receiver, oop argument);
  static oop smiOopPrimitives_remainder(oop receiver, oop argument);

  static void patch_primitiveValue();
  static void init();				// must be called in system initialization phase
};
