/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.9 $ */
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

// Floats describes the floating point operations of the interpreter
// and implements stub routines used to execute this operations.
// Note: The Floats stub routines are *inside* the interpreter code.

class Floats: AllStatic {
 public:
  enum Some_constants {
    magic			= 0xbadbabe0,	// indicates floats in the frame if stored in temp1
    max_number_of_functions	= 256,
  };

  // Function codes for float operations.
  // Note: When changing these codes, make sure that the bytecode compiler
  //       is updated as well! (use delta +GenerateSmalltalk to generate
  //       an updated file-in file for Smalltalk).
  enum Function {
    // nullary functions
    zero,			// float(i) := 0.0
    one,			// float(i) := 1.0

    // unary functions
    abs,			// float(i) := |float(i)|
    negated,			// float(i) := -float(i)
    squared,			// float(i) := float(i) * float(i)
    sqrt,			// float(i) := sqrt(float(i))
    sin,			// float(i) := sin(float(i))
    cos,			// float(i) := cos(float(i))
    tan,			// float(i) := tan(float(i))
    exp,			// float(i) := exp(float(i))
    ln,				// float(i) := ln(float(i))

    // binary functions
    add,			// float(i) := float(i) + float(i + 1)
    subtract,			// float(i) := float(i) - float(i + 1)
    multiply,			// float(i) := float(i) * float(i + 1)
    divide,			// float(i) := float(i) / float(i + 1)
    modulo,			// float(i) := float(i) \ float(i + 1)

    // unary functions to oop
    is_zero,			// tos := float(i) =  0.0
    is_not_zero,		// tos := float(i) ~= 1.0
    oopify,			// tos := oop(float(i))
    
    // binary functions to oop
    is_equal,			// tos := float(i) =  float(i + 1)
    is_not_equal,		// tos := float(i) ~= float(i + 1)
    is_less,			// tos := float(i) <  float(i + 1)
    is_less_equal,		// tos := float(i) <= float(i + 1)
    is_greater,			// tos := float(i) >  float(i + 1)
    is_greater_equal,		// tos := float(i) >= float(i + 1)

    number_of_functions,

    // debugging & other purposes only (not actually generated in the bytecodes)
    undefined,			// error handling
    floatify			// used only to find selectors (MethodIterator)
  };

 private:
  private:
  static bool  _is_initialized;	// true if Floats has been initialized
  static char* _function_names[];

  static void  generate_tst(MacroAssembler* masm, Assembler::Condition cc);
  static void  generate_cmp(MacroAssembler* masm, Assembler::Condition cc);
  static void  generate(MacroAssembler* masm, Function f);

 public:
  // Dispatch (interpreter)
  // Note: _function_table is bigger than number_of_functions to catch illegal (byte) indices
  static char* _function_table[max_number_of_functions];

  // Debugging/Printing
  static char* function_name_for(Function f);

  // Tells if there is a selector for the float operation
  static bool  has_selector_for(Function f);

  // Returns the selector for the float operation; NULL is there is no selector
  static symbolOop selector_for(Function f);

  static oop magic_value() { return smiOop(magic); }

  // Initialization/debugging
  static bool  is_initialized()	{ return _is_initialized; }
  static void  init(MacroAssembler* masm);
  static void  print();
};
