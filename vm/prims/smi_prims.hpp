/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.18 $ */
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


// Primitives for small integers

class smiOopPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <SmallInteger> primitiveLessThan: aNumber   <SmallInteger>
  //                           ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { flags = #(Pure SmiCompare LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::lessThan' }
  //%
  static PRIM_DECL_2(lessThan, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveGreaterThan: aNumber   <SmallInteger>
  //                              ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { flags = #(Pure SmiCompare LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::greaterThan' }
  //%
  static PRIM_DECL_2(greaterThan, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveLessThanOrEqual: aNumber   <SmallInteger>
  //                                  ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { flags = #(Pure SmiCompare LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::lessThanOrEqual' }
  //%
  static PRIM_DECL_2(lessThanOrEqual, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveGreaterThanOrEqual: aNumber   <SmallInteger>
  //                                     ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { flags = #(Pure SmiCompare LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::greaterThanOrEqual' }
  //%
  static PRIM_DECL_2(greaterThanOrEqual, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveSmallIntegerEqual: aNumber   <SmallInteger>
  //                                    ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { flags = #(Pure SmiCompare LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::equal' }
  //%
  static PRIM_DECL_2(equal, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveSmallIntegerNotEqual: aNumber   <SmallInteger>
  //                                       ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { flags = #(Pure SmiCompare LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::notEqual' }
  //%
  static PRIM_DECL_2(notEqual, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveBitAnd: aNumber   <SmallInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::bitAnd' }
  //%
  static PRIM_DECL_2(bitAnd, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveBitOr: aNumber   <SmallInteger>
  //                        ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::bitOr' }
  //%
  static PRIM_DECL_2(bitOr, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveBitXor: aNumber   <SmallInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::bitXor' }
  //%
  static PRIM_DECL_2(bitXor, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveBitShift: aNumber   <SmallInteger>
  //                           ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::bitShift' }
  //%
  static PRIM_DECL_2(bitShift, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveRawBitShift: aNumber   <SmallInteger>
  //                              ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
  //              name  = 'smiOopPrimitives::rawBitShift' }
  //%
  static PRIM_DECL_2(rawBitShift, oop receiver, oop argument);

  //%prim
  // <SmallInteger> primitiveAsObjectIfFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #Function
  //              name  = 'smiOopPrimitives::asObject' }
  //%
  static PRIM_DECL_1(asObject, oop receiver);

  // For debugging only
  //%prim
  // <SmallInteger> primitivePrintCharacterIfFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #Function
  //              name  = 'smiOopPrimitives::printCharacter' }
  //%
  static PRIM_DECL_1(printCharacter, oop receiver);
};

// Assembler optimized primitives

//%prim
// <SmallInteger> primitiveAdd: aNumber   <SmallInteger>
//                      ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
//   Internal { error = #(Overflow)
//              flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
//              name  = 'smiOopPrimitives_add' }
//%
extern "C" oop PRIM_API smiOopPrimitives_add(oop receiver, oop argument);

//%prim
// <SmallInteger> primitiveSubtract: aNumber   <SmallInteger>
//                           ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
//   Internal { error = #(Overflow)
//              flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
//              name  = 'smiOopPrimitives_subtract' }
//%
extern "C" oop PRIM_API smiOopPrimitives_subtract(oop receiver, oop argument);

//%prim
// <SmallInteger> primitiveMultiply: aNumber   <SmallInteger>
//                           ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
//   Internal { error = #(Overflow)
//              flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
//              name  = 'smiOopPrimitives_multiply' }
//%
extern "C" oop PRIM_API smiOopPrimitives_multiply(oop receiver, oop argument);

//%prim
// <SmallInteger> primitiveMod: aNumber   <SmallInteger>
//                      ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
//   Internal { error = #(Overflow DivisionByZero)
//              flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
//              name  = 'smiOopPrimitives_mod' }
//%
extern "C" oop PRIM_API smiOopPrimitives_mod(oop receiver, oop argument);

//%prim
// <SmallInteger> primitiveDiv: aNumber   <SmallInteger>
//                      ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
//   Internal { error = #(Overflow DivisionByZero)
//              flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
//              name  = 'smiOopPrimitives_div' }
//%
extern "C" oop PRIM_API smiOopPrimitives_div(oop receiver, oop argument);

//%prim
// <SmallInteger> primitiveQuo: aNumber   <SmallInteger>
//                      ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
//   Internal { error = #(NotImplementedYet)
//            flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
//              name  = 'smiOopPrimitives_quo' }
//%
extern "C" oop PRIM_API smiOopPrimitives_quo(oop receiver, oop argument);

//%prim
// <SmallInteger> primitiveRemainder: aNumber   <SmallInteger>
//                            ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
//   Internal { error = #(DivisionByZero)
//              flags = #(Pure SmiArith LastDeltaFrameNotNeeded)
//              name  = 'smiOopPrimitives_remainder' }
//%
extern "C" oop PRIM_API smiOopPrimitives_remainder(oop receiver, oop argument);

 //%prim
// <SmallInteger> primitiveAsFloat ^<Float> =
//   Internal { flags = #(Pure SmiArith)
//              name  = 'double_from_smi' }
//%
extern "C" oop PRIM_API double_from_smi(oop receiver);
