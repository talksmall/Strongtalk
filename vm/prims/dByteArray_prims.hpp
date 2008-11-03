/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.16 $ */
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

class doubleByteArrayPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <IndexedDoubleByteInstanceVariables class>
  //   primitiveIndexedDoubleByteNew: size      <SmallInteger> 
  //                          ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NegativeSize)
  //              flags = #(Allocate)
  //              name  = 'doubleByteArrayPrimitives::allocateSize' }
  //%
  static PRIM_DECL_2(allocateSize, oop receiver, oop argument);

  //%prim
  // <NoReceiver>
  //   primitiveIndexedDoubleByteNew: behaviour <IndexedDoubleByteInstanceVariables class>
  //                            size: size      <SmallInteger>
  //                         tenured: tenured   <Boolean>
  //                          ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NegativeSize)
  //              flags = #(Allocate)
  //              name  = 'doubleByteArrayPrimitives::allocateSize2' }
  //%
  static PRIM_DECL_3(allocateSize2, oop receiver, oop argument, oop tenured);

  //%prim
  // <IndexedDoubleByteInstanceVariables>
  //   primitiveIndexedDoubleByteSize ^<SmallInteger> =
  //   Internal { flags = #(Pure IndexedDoubleByte)
  //              name  = 'doubleByteArrayPrimitives::size' }
  //%
  static PRIM_DECL_1(size, oop receiver); 

  //%prim
  // <IndexedDoubleByteInstanceVariables>
  //   primitiveIndexedDoubleByteAt: index     <SmallInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #(Function IndexedDoubleByte)
  //              name  = 'doubleByteArrayPrimitives::at' }
  //%
  static PRIM_DECL_2(at, oop receiver, oop index);

  //%prim
  // <IndexedDoubleByteInstanceVariables>
  //   primitiveIndexedDoubleByteAt: index     <SmallInteger>
  //                            put: c         <SmallInteger>
  //                         ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(OutOfBounds ValueOutOfBounds)
  //              flags = #(Function IndexedDoubleByte)
  //              name  = 'doubleByteArrayPrimitives::atPut' }
  //%
  static PRIM_DECL_3(atPut, oop receiver, oop index, oop value);

  //%prim
  // <IndexedDoubleByteInstanceVariables>
  //   primitiveIndexedDoubleByteCompare: str       <String>
  //                              ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #Function
  //              name  = 'doubleByteArrayPrimitives::compare' }
  //%
  static PRIM_DECL_2(compare, oop receiver, oop argument);

  //%prim
  // <IndexedDoubleByteInstanceVariables>
  //   primitiveIndexedDoubleByteInternIfFail: failBlock <PrimFailBlock> ^<CompressedSymbol> =
  //   Internal { error = #(ValueOutOfBounds)
  //              name  = 'doubleByteArrayPrimitives::intern' }
  //%
  static PRIM_DECL_1(intern, oop receiver);

  //%prim
  // <IndexedDoubleByteInstanceVariables>
  //   primitiveIndexedDoubleByteCharacterAt: index <SmallInteger>
  //                                  ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #(Function IndexedDoubleByte)
  //              name  = 'doubleByteArrayPrimitives::characterAt' }
  //%
  static PRIM_DECL_2(characterAt, oop receiver, oop index);

  //%prim
  // <IndexedDoubleByteInstanceVariables>
  //   primitiveIndexedDoubleByteHash ^<SmallInteger> =
  //   Internal { name  = 'doubleByteArrayPrimitives::hash' }
  //%
  static PRIM_DECL_1(hash, oop receiver);
};

