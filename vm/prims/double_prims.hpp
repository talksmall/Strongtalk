/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.25 $ */
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

// Primitives for doubles
//
//  alias PrimFailBlock : <[Symbol, ^BottomType]>

class doubleOopPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <Float> primitiveFloatLessThan:  aNumber   <Float>
  //                         ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is less than the argument'
  //              flags = #(Pure DoubleCompare LastDeltaFrameNotNeeded)
  //              name  = 'doubleOopPrimitives::lessThan' }
  //%
  static PRIM_DECL_2(lessThan, oop receiver, oop argument);
  
  //%prim
  // <Float> primitiveFloatGreaterThan: aNumber   <Float>
  //                            ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is greater than the argument'
  //              flags = #(Pure DoubleCompare LastDeltaFrameNotNeeded)
  //              name  = 'doubleOopPrimitives::greaterThan' }
  //%
  static PRIM_DECL_2(greaterThan, oop receiver, oop argument);
  
  //%prim
  // <Float> primitiveFloatLessThanOrEqual: aNumber   <Float>
  //                                ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is less than or equal to the argument'
  //              flags = #(Pure DoubleCompare LastDeltaFrameNotNeeded)
  //              name  = 'doubleOopPrimitives::lessThanOrEqual' }
  //%
  static PRIM_DECL_2(lessThanOrEqual, oop receiver, oop argument);
  
  //%prim
  // <Float> primitiveFloatGreaterThanOrEqual: aNumber   <Float>
  //                                   ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is greater than or equal to the argument'
  //              flags = #(Pure DoubleCompare LastDeltaFrameNotNeeded)
  //              name  = 'doubleOopPrimitives::greaterThanOrEqual' }
  //%
  static PRIM_DECL_2(greaterThanOrEqual, oop receiver, oop argument);
  
  //%prim
  // <Float> primitiveFloatEqual: aNumber   <Float>
  //                      ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is equal to the argument'
  //              flags = #(Pure DoubleCompare LastDeltaFrameNotNeeded)
  //              name  = 'doubleOopPrimitives::equal' }
  //%
  static PRIM_DECL_2(equal, oop receiver, oop argument);
  
  //%prim
  // <Float> primitiveFloatNotEqual: aNumber   <Float>
  //                         ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is not equal to the argument'
  //              flags = #(Pure DoubleCompare LastDeltaFrameNotNeeded)
  //              name  = 'doubleOopPrimitives::notEqual' }
  //%
  static PRIM_DECL_2(notEqual, oop receiver, oop argument);
  
  //%prim
  // <Float> primitiveFloatMod: aNumber   <Float>
  //                    ifFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the result of dividing the receiver by the argument'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::mod' }
  //%
  static PRIM_DECL_2(mod, oop receiver, oop argument);
  
  //%prim
  // <Float> primitiveFloatCosine ^<Float> =
  //   Internal { doc   = 'Returns the cosine of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::cosine' }
  //%
  static PRIM_DECL_1(cosine, oop receiver);
  
  //%prim
  // <Float> primitiveFloatSine ^<Float> =
  //   Internal { doc   = 'Returns the sine of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::sine' }
  //%
  static PRIM_DECL_1(sine, oop receiver);
  
  //%prim
  // <Float> primitiveFloatTangentIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the tangent of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::tangent' }
  //%
  static PRIM_DECL_1(tangent, oop receiver);

  //%prim
  // <Float> primitiveFloatArcCosineIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the arc-cosine of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::arcCosine' }
  //%
  static PRIM_DECL_1(arcCosine, oop receiver);
  
  //%prim
  // <Float> primitiveFloatArcSineIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the arc-sine of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::arcSine' }
  //%
  static PRIM_DECL_1(arcSine, oop receiver);
  
  //%prim
  // <Float> primitiveFloatArcTangentIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the arc-tangent of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::arcTangent' }
  //%
  static PRIM_DECL_1(arcTangent, oop receiver);
  
  //%prim
  // <Float> primitiveFloatHyperbolicCosineIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the hyperbolic-cosine of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::hyperbolicCosine' }
  //%
  static PRIM_DECL_1(hyperbolicCosine, oop receiver);
  
  //%prim
  // <Float> primitiveFloatHyperbolicSineIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the hyperbolic-sine of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::hyperbolicSine' }
  //%
  static PRIM_DECL_1(hyperbolicSine, oop receiver);
  
  //%prim
  // <Float> primitiveFloatHyperbolicTangentIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the hyperbolic-tangent of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::hyperbolicTangent' }
  //%
  static PRIM_DECL_1(hyperbolicTangent, oop receiver);
  
  //%prim
  // <Float> primitiveFloatSqrtIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the square root of the receiver'
  //              error = #(ReceiverNegative)
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::sqrt' }
  //%
  static PRIM_DECL_1(sqrt, oop receiver);
  
  //%prim
  // <Float> primitiveFloatSquared ^<Float> =
  //   Internal { doc   = 'Returns the result of multiplying the receiver by it self'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::squared' }
  //%
  static PRIM_DECL_1(squared, oop receiver);
  
  //%prim
  // <Float> primitiveFloatLnIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the logarithm of the receiver'
  //              error = #(ReceiverNotStrictlyPositive)
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::ln' }
  //%
  static PRIM_DECL_1(ln, oop receiver);
  
  //%prim
  // <Float> primitiveFloatExp ^<Float> =
  //   Internal { doc   = 'Returns the exponential value of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::exp' }
  //%
  static PRIM_DECL_1(exp, oop receiver);
  
  //%prim
  // <Float> primitiveFloatLog10IfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the base 10 logarithm of the receiver'
  //              error = #(ReceiverNotStrictlyPositive)
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::log10' }
  //%
  static PRIM_DECL_1(log10, oop receiver);
  
  //%prim
  // <Float> primitiveFloatIsNan ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is NaN (Not a Number)'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::isNan' }
  //%
  static PRIM_DECL_1(isNan, oop receiver);
  
  //%prim
  // <Float> primitiveFloatIsFinite ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is finite (not NaN)'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::isFinite' }
  //%
  static PRIM_DECL_1(isFinite, oop receiver);
  
  //%prim
  // <Float> primitiveFloatFloor ^<Float> =
  //   Internal { doc   = 'Returns the largest integral Float that is less than or equal to the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::floor' }
  //%
  static PRIM_DECL_1(floor, oop receiver);

  //%prim
  // <Float> primitiveFloatSmallIntegerFloorIfFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { doc   = 'Returns the largest SmallInteger that is less than or equal to the receiver'
  //              flags = #(Pure DoubleArith LastDeltaFrameNotNeeded)
  //              error = #(ConversionFailed)
  //              name  = 'doubleOopPrimitives::smi_floor' }
  //%
  static PRIM_DECL_1(smi_floor, oop receiver);
  
  //%prim
  // <Float> primitiveFloatCeiling ^<Float> =
  //   Internal { doc   = 'Returns the smallest integral Float that is greater than or equal to the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::ceiling' }
  //%
  static PRIM_DECL_1(ceiling, oop receiver);
  
  //%prim
  // <Float> primitiveFloatExponent ^<SmallInteger> =
  //   Internal { doc   = 'Returns the exponent part of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::exponent' }
  //%
  static PRIM_DECL_1(exponent, oop receiver);
  
  //%prim
  // <Float> primitiveFloatMantissa ^<Float> =
  //   Internal { doc   = 'Returns the mantissa part of the receiver'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::mantissa' }
  //%
  static PRIM_DECL_1(mantissa, oop receiver);

  //%prim
  // <Float> primitiveFloatTruncated  ^<Float> =
  //   Internal { doc   = 'Returns the receiver truncated'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::truncated' }
  //%
  static PRIM_DECL_1(truncated, oop receiver);
  
  //%prim
  // <Float> primitiveFloatTimesTwoPower: aNumber   <SmallInteger>
  //                              ifFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { doc   = 'Returns the receiver multiplied with 2 to the power of aNumber'
  //              error = #(RangeError)
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::timesTwoPower' }
  //%
  static PRIM_DECL_2(timesTwoPower, oop receiver, oop argument);
  
  //%prim
  // <Float> primitiveFloatRoundedAsSmallIntegerIfFail: failBlock <PrimFailBlock>  ^<SmallInteger> =
  //   Internal { doc   = 'Returns the receiver converted to a SmallInteger'
  //              error = #(SmallIntegerConversionFailed)
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::roundedAsSmallInteger' }
  //%
  static PRIM_DECL_1(roundedAsSmallInteger, oop receiver);
 
  //%prim
  // <Float> primitiveFloatAsSmallIntegerIfFail: failBlock <PrimFailBlock>  ^<SmallInteger> =
  //   Internal { doc   = 'Returns the receiver as a SmallInteger'
  //              error = #(SmallIntegerConversionFailed)
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::asSmallInteger' }
  //%
  static PRIM_DECL_1(asSmallInteger, oop receiver);

  //%prim
  // <Float> primitiveFloatPrintFormat: format    <IndexedByteInstanceVariables>
  //                            ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { doc   = 'Prints the receiver using the format and returns the recever'
  //              flags = #Function
  //              name  = 'doubleOopPrimitives::printFormat' }
  //%
  static PRIM_DECL_2(printFormat, oop receiver, oop argument); 

  //%prim
  // <Float> primitiveFloatPrintString ^<IndexedByteInstanceVariables> =
  //   Internal { doc   = 'Returns the print string for the receiver'
  //              flags = #Function
  //              name  = 'doubleOopPrimitives::printString' }
  //%
  static PRIM_DECL_1(printString, oop receiver);

  //%prim
  // <NoReceiver> primitiveFloatMaxValue ^<Float> =
  //   Internal { doc   = 'Returns the maximum Float value'
  //              flags = #(Pure)
  //              name  = 'doubleOopPrimitives::max_value' }
  //%
  static PRIM_DECL_0(max_value);

  //%prim
  // <NoReceiver> primitiveFloatMinPositiveValue ^<Float> =
  //   Internal { doc   = 'Returns the minimum positive Float value'
  //              flags = #(Pure)
  //              name  = 'doubleOopPrimitives::min_positive_value' }
  //%
  static PRIM_DECL_0(min_positive_value);

  //%prim
  // <Float> primitiveFloatStoreString ^<ByteArray> =
  //   Internal { flags = #(Function)
  //              name  = 'doubleOopPrimitives::store_string' }
  //%
  static PRIM_DECL_1(store_string, oop receiver);

  //%prim
  // <NoReceiver> primitiveMandelbrotAtRe: re        <Float>
  //                                   im: im        <Float>
  //                              iterate: n         <SmallInteger>
  //                               ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { doc   = 'Returns no. of iterations used for Mandelbrot value at (re, im)'
  //              flags = #(Pure LastDeltaFrameNotNeeded)
  //              name  = 'doubleOopPrimitives::mandelbrot' }
  //%
  static PRIM_DECL_3(mandelbrot, oop re, oop im, oop n);
  
  //%prim
  // <Float> primitiveFloatIsInfinity ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver is +-Infinity'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::isInfinity' }
  //%
  static PRIM_DECL_1(isInfinity, oop receiver);

  //%prim
  // <Float> primitiveFloatIsNormal ^<Boolean> =
  //   Internal { doc   = 'Returns whether the receiver has a normal value and not a Infinity, NaN or a non-zero normalized number)'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::isNormal' }
  //%
  static PRIM_DECL_1(isNormal, oop receiver);

  //%prim
  // <Float> primitiveFloatSign ^<Float> =
  //   Internal { doc   = 'Returns whether the receiver is negative positive, zero or a NaN(-1,1,0, NaN)'
  //              flags = #(Pure DoubleArith)
  //              name  = 'doubleOopPrimitives::sign' }
  //%
  static PRIM_DECL_1(sign, oop receiver);


};

//%prim
// <Float> primitiveFloatSubtract: aNumber   <Float>
//                         ifFail: failBlock <PrimFailBlock> ^<Float> =
//   Internal { doc   = 'Returns the result of subtracting the argument from the receiver'
//              flags = #(Pure DoubleArith LastDeltaFrameNotNeeded)
//              name  = 'double_subtract' }
//%
extern "C" oop PRIM_API double_subtract(oop receiver, oop argument);

//%prim
// <Float> primitiveFloatDivide: aNumber   <Float>
//                       ifFail: failBlock <PrimFailBlock> ^<Float> =
//   Internal { doc   = 'Returns the modulus of the receiver by the argument'
//              flags = #(Pure DoubleArith LastDeltaFrameNotNeeded)
//              name  = 'double_divide' } 
//%
extern "C" oop PRIM_API double_divide(oop receiver, oop argument);
  
//%prim
// <Float> primitiveFloatAdd: aNumber   <Float>
//                    ifFail: failBlock <PrimFailBlock> ^<Float> =
//   Internal { doc   = 'Returns the sum of the receiver and the argument'
//              flags = #(Pure DoubleArith LastDeltaFrameNotNeeded)
//              name  = 'double_add' }
//%
extern "C" oop PRIM_API double_add(oop receiver, oop argument);
    
//%prim
// <Float> primitiveFloatMultiply: aNumber   <Float>
//                         ifFail: failBlock <PrimFailBlock> ^<Float> =
//   Internal { doc   = 'Returns the multiply of the receiver and the argument'
//              flags = #(Pure DoubleArith LastDeltaFrameNotNeeded)
//              name  = 'double_multiply' }
//%
extern "C" oop PRIM_API double_multiply(oop receiver, oop argument);
  
