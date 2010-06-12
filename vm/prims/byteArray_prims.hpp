/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.24 $ */
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


// Primitives for byte arrays

class byteArrayPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
   static int number_of_calls;

  //%prim
  // <IndexedByteInstanceVariables class>
  //   primitiveIndexedByteNew: size      <SmallInteger> 
  //                    ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NegativeSize)
  //              flags = #(Allocate)
  //              name  = 'byteArrayPrimitives::allocateSize' }
  //%
  static PRIM_DECL_2(allocateSize, oop receiver, oop argument);

    //%prim
  // <NoReceiver>
  //   primitiveIndexedByteNew:  class     <IndexedByteInstanceVariables class>
  //                    size:    size      <SmallInteger>
  //                    tenured: tenured   <Boolean>
  //                    ifFail:  failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NegativeSize)
  //              flags = #(Allocate)
  //              name  = 'byteArrayPrimitives::allocateSize2' }
  //%
  static PRIM_DECL_3(allocateSize2, oop receiver, oop argument, oop tenured);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteSize ^<SmallInteger> =
  //   Internal { flags = #(Pure IndexedByte)
  //              name  = 'byteArrayPrimitives::size' }
  //%
  static PRIM_DECL_1(size, oop receiver); 
  
  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveSymbolNumberOfArguments ^<SmallInteger> =
  //   Internal { flags = #(Pure IndexedByte)
  //              name  = 'byteArrayPrimitives::numberOfArguments' }
  //%
  static PRIM_DECL_1(numberOfArguments, oop receiver); 
  
  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteAt: index <SmallInteger>
  //                   ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::at' }
  //%
  static PRIM_DECL_2(at, oop receiver, oop index);
  
  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteAt: index     <SmallInteger>
  //                      put: c         <SmallInteger>
  //                   ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(OutOfBounds ValueOutOfBounds)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::atPut' }
  //%
  static PRIM_DECL_3(atPut, oop receiver, oop index, oop value);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteCompare: index <String>
  //                        ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name = 'byteArrayPrimitives::compare' }
  //%
  static PRIM_DECL_2(compare, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteInternIfFail: failBlock <PrimFailBlock> ^<CompressedSymbol> =
  //   Internal { error = #(ValueOutOfBounds)
  //              name  = 'byteArrayPrimitives::intern' }
  //%
  static PRIM_DECL_1(intern, oop receiver);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteCharacterAt: index <SmallInteger>
  //                            ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::characterAt' }
  //%
  static PRIM_DECL_2(characterAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteAtAllPut: c <SmallInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { name  = 'byteArrayPrimitives::at_all_put' }
  //%
  static PRIM_DECL_2(at_all_put, oop receiver, oop c);


  // SUPPORT FOR LARGE INTEGER

  //%prim
  // <IndexedByteInstanceVariables class>
  //   primitiveIndexedByteLargeIntegerFromSmallInteger: number  <SmallInteger> 
  //                                             ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables> =
  //   Internal { flags = #(Function)
  //              name  = 'byteArrayPrimitives::largeIntegerFromSmallInteger' }
  //%
  static PRIM_DECL_2(largeIntegerFromSmallInteger, oop receiver, oop number);

  //%prim
  // <IndexedByteInstanceVariables class>
  //   primitiveIndexedByteLargeIntegerFromFloat: number  <Float> 
  //                                      ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables> =
  //   Internal { flags = #(Function)
  //              name  = 'byteArrayPrimitives::largeIntegerFromDouble' }
  //%
  static PRIM_DECL_2(largeIntegerFromDouble, oop receiver, oop number);

  //%prim
  // <IndexedByteInstanceVariables class>
  //   primitiveIndexedByteLargeIntegerFromString: argument  <String>
  //                                         base: base      <Integer> 
  //                                       ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables> =
  //   Internal { error = #(ConversionFailed)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerFromString' }
  //%
  static PRIM_DECL_3(largeIntegerFromString, oop receiver, oop argument, oop base);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerAdd: argument <IndexedByteInstanceVariables>
  //                                ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerAdd' }
  //%
  static PRIM_DECL_2(largeIntegerAdd, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerSubtract: argument <IndexedByteInstanceVariables>
  //                                     ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerSubtract' }
  //%
  static PRIM_DECL_2(largeIntegerSubtract, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerMultiply: argument <IndexedByteInstanceVariables>
  //                                     ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerMultiply' }
  //%
  static PRIM_DECL_2(largeIntegerMultiply, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerQuo: argument <IndexedByteInstanceVariables>
  //                                ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid DivisionByZero)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerQuo' }
  //%
  static PRIM_DECL_2(largeIntegerQuo, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerDiv: argument <IndexedByteInstanceVariables>
  //                                ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid DivisionByZero)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerDiv' }
  //%
  static PRIM_DECL_2(largeIntegerDiv, oop receiver, oop argument);


  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerMod: argument <IndexedByteInstanceVariables>
  //                                ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid DivisionByZero)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerMod' }
  //%
  static PRIM_DECL_2(largeIntegerMod, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerRem: argument <IndexedByteInstanceVariables>
  //                                ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid DivisionByZero)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerRem' }
  //%
  static PRIM_DECL_2(largeIntegerRem, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerAnd: argument <IndexedByteInstanceVariables>
  //                                ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid DivisionByZero)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerAnd' }
  //%
  static PRIM_DECL_2(largeIntegerAnd, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerXor: argument <IndexedByteInstanceVariables>
  //                                ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid DivisionByZero)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerXor' }
  //%
  static PRIM_DECL_2(largeIntegerXor, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerOr: argument <IndexedByteInstanceVariables>
  //                               ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid DivisionByZero)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerOr' }
  //%
  static PRIM_DECL_2(largeIntegerOr, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerShift: argument <SmallInt>
  //                                  ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables|SmallInteger> =
  //   Internal { error = #(ArgumentIsInvalid DivisionByZero)
  //              flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerShift' }
  //%
  static PRIM_DECL_2(largeIntegerShift, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerCompare: argument <IndexedByteInstanceVariables>
  //                                    ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerCompare' }
  //%
  static PRIM_DECL_2(largeIntegerCompare, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerAsFloatIfFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerToFloat' }
  //%
  static PRIM_DECL_1(largeIntegerToFloat, oop receiver);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteLargeIntegerToStringBase: base      <SmallInteger>
  //                                         ifFail: failBlock <PrimFailBlock> ^<String> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerToString' }
  //%
  static PRIM_DECL_2(largeIntegerToString, oop receiver, oop base);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveIndexedByteHash ^<SmallInteger> =
  //   Internal { flags = #(Pure IndexedByte)
  //              name  = 'byteArrayPrimitives::hash' }
  //%
  static PRIM_DECL_1(hash, oop receiver); 

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveLargeIntegerHash ^<SmallInteger> =
  //   Internal { flags = #(Pure IndexedByte)
  //              name  = 'byteArrayPrimitives::largeIntegerHash' }
  //%
  static PRIM_DECL_1(largeIntegerHash, oop receiver);

  // Aliens primitives

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienSizeIfFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienGetSize' }
  //%
  static PRIM_DECL_1(alienGetSize, oop receiver);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienSize: size <SmallInteger>
  //               ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienSetSize' }
  //%
  static PRIM_DECL_2(alienSetSize, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienAddressIfFail: failBlock <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienGetAddress' }
  //%
  static PRIM_DECL_1(alienGetAddress, oop receiver);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienAddress: address <Integer>
  //                  ifFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienSetAddress' }
  //%
  static PRIM_DECL_2(alienSetAddress, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienUnsignedByteAt: index  <SmallInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienUnsignedByteAt' }
  //%
  static PRIM_DECL_2(alienUnsignedByteAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienUnsignedByteAt: index  <SmallInteger>
  //                            put: value  <SmallInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienUnsignedByteAtPut' }
  //%
  static PRIM_DECL_3(alienUnsignedByteAtPut, oop receiver, oop index, oop value);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienSignedByteAt: index  <SmallInteger>
  //                       ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienSignedByteAt' }
  //%
  static PRIM_DECL_2(alienSignedByteAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienSignedByteAt: index  <SmallInteger>
  //                          put: value  <SmallInteger>
  //                       ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienSignedByteAtPut' }
  //%
  static PRIM_DECL_3(alienSignedByteAtPut, oop receiver, oop index, oop value);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienUnsignedShortAt: index  <SmallInteger>
  //                          ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienUnsignedShortAt' }
  //%
  static PRIM_DECL_2(alienUnsignedShortAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienUnsignedShortAt: index  <SmallInteger>
  //                             put: value  <SmallInteger>
  //                          ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienUnsignedShortAtPut' }
  //%
  static PRIM_DECL_3(alienUnsignedShortAtPut, oop receiver, oop argument1, oop argument2);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienSignedShortAt: index  <SmallInteger>
  //                        ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienSignedShortAt' }
  //%
  static PRIM_DECL_2(alienSignedShortAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienSignedShortAt: index  <SmallInteger>
  //                           put: value  <SmallInteger>
  //                        ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienSignedShortAtPut' }
  //%
  static PRIM_DECL_3(alienSignedShortAtPut, oop receiver, oop argument1, oop argument2);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienUnsignedLongAt: index  <SmallInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<SmallInteger|LargeInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienUnsignedLongAt' }
  //%
  static PRIM_DECL_2(alienUnsignedLongAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienUnsignedLongAt: index  <SmallInteger>
  //                            put: value  <SmallInteger|LargeInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<SmallInteger|LargeInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienUnsignedLongAtPut' }
  //%
  static PRIM_DECL_3(alienUnsignedLongAtPut, oop receiver, oop argument1, oop argument2);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienSignedLongAt: index  <SmallInteger>
  //                       ifFail: failBlock <PrimFailBlock> ^<SmallInteger|LargeInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienSignedLongAt' }
  //%
  static PRIM_DECL_2(alienSignedLongAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienSignedLongAt: index  <SmallInteger>
  //                          put: value  <SmallInteger|LargeInteger>
  //                       ifFail: failBlock <PrimFailBlock> ^<SmallInteger|LargeInteger> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienSignedLongAtPut' }
  //%
  static PRIM_DECL_3(alienSignedLongAtPut, oop receiver, oop argument1, oop argument2);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienDoubleAt: index  <SmallInteger>
  //                   ifFail: failBlock <PrimFailBlock> ^<Double> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienDoubleAt' }
  //%
  static PRIM_DECL_2(alienDoubleAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienDoubleAt: index  <SmallInteger>
  //                      put: value  <Double>
  //                   ifFail: failBlock <PrimFailBlock> ^<Double> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienDoubleAtPut' }
  //%
  static PRIM_DECL_3(alienDoubleAtPut, oop receiver, oop argument1, oop argument2);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienFloatAt: index  <SmallInteger>
  //                  ifFail: failBlock <PrimFailBlock> ^<Double> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienFloatAt' }
  //%
  static PRIM_DECL_2(alienFloatAt, oop receiver, oop index);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienFloatAt: index  <SmallInteger>
  //                     put: value  <Double>
  //                  ifFail: failBlock <PrimFailBlock> ^<Double> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienFloatAtPut' }
  //%
  static PRIM_DECL_3(alienFloatAtPut, oop receiver, oop argument1, oop argument2);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result  <IndexedByteInstanceVariables>
  //                     ifFail: failBlock <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResult0' }
  //%
  static PRIM_DECL_2(alienCallResult0, oop receiver, oop argument);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result    <IndexedByteInstanceVariables>
  //                       with: argument  <IndexedByteInstanceVariables|SmallInteger>
  //                     ifFail: failBlock <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResult1' }
  //%
  static PRIM_DECL_3(alienCallResult1, oop receiver, oop argument1, oop argument2);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result     <IndexedByteInstanceVariables>
  //                       with: argument1  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument2  <IndexedByteInstanceVariables|SmallInteger>
  //                     ifFail: failBlock  <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResult2' }
  //%
  static PRIM_DECL_4(alienCallResult2, oop receiver, oop argument1, oop argument2, oop argument3);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result     <IndexedByteInstanceVariables>
  //                       with: argument1  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument2  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument3  <IndexedByteInstanceVariables|SmallInteger>
  //                     ifFail: failBlock  <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResult3' }
  //%
  static PRIM_DECL_5(alienCallResult3, oop receiver, oop argument1, oop argument2, oop argument3, oop argument4);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result     <IndexedByteInstanceVariables>
  //                       with: argument1  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument2  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument3  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument4  <IndexedByteInstanceVariables|SmallInteger>
  //                     ifFail: failBlock  <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResult4' }
  //%
  static PRIM_DECL_6(alienCallResult4, oop receiver, oop argument1, oop argument2, oop argument3, oop argument4, oop argument5);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result     <IndexedByteInstanceVariables>
  //                       with: argument1  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument2  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument3  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument4  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument5  <IndexedByteInstanceVariables|SmallInteger>
  //                     ifFail: failBlock  <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResult5' }
  //%
  static PRIM_DECL_7(alienCallResult5, oop receiver, oop argument1, oop argument2, 
    oop argument3, oop argument4, oop argument5, oop argument6);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result     <IndexedByteInstanceVariables>
  //                       with: argument1  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument2  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument3  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument4  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument5  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument6  <IndexedByteInstanceVariables|SmallInteger>
  //                     ifFail: failBlock  <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResult6' }
  //%
  static PRIM_DECL_8(alienCallResult6, oop receiver, oop argument1, oop argument2, 
    oop argument3, oop argument4, oop argument5, oop argument6, oop argument7);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result     <IndexedByteInstanceVariables>
  //                       with: argument1  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument2  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument3  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument4  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument5  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument6  <IndexedByteInstanceVariables|SmallInteger>
  //                       with: argument7  <IndexedByteInstanceVariables|SmallInteger>
  //                     ifFail: failBlock  <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResult7' }
  //%
  static PRIM_DECL_9(alienCallResult7, oop receiver, oop argument1, oop argument2,
    oop argument3, oop argument4, oop argument5, oop argument6, oop argument7, oop argument8);

  //%prim
  // <IndexedByteInstanceVariables>
  //   primitiveAlienCallResult: result     <IndexedByteInstanceVariables>
  //              withArguments: argument1  <IndexedInstanceVariables>
  //                     ifFail: failBlock  <PrimFailBlock> ^<Integer> =
  //   Internal { flags = #(Function IndexedByte)
  //              name  = 'byteArrayPrimitives::alienCallResultWithArguments' }
  //%
  static PRIM_DECL_3(alienCallResultWithArguments, oop receiver, oop argument1, oop argument2);
};
