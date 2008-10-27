/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.15 $ */
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

// Primitives for obj arrays

class objArrayPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <NoReceiver>
  //   primitiveIndexedObjectNew: class <IndexedInstanceVariables class>
  //                        size: size <SmallInteger> 
  //                     tenured: tenured <Boolean>
  //                      ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NegativeSize)
  //              flags = #(Allocate)
  //              name  = 'objArrayPrimitives::allocateSize2' }
  //%
  static PRIM_DECL_3(allocateSize2, oop receiver, oop argument, oop tenured);

  //%prim
  // <IndexedInstanceVariables class>
  //   primitiveIndexedObjectNew: size <SmallInteger> 
  //                      ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NegativeSize)
  //              flags = #(Allocate)
  //              name  = 'objArrayPrimitives::allocateSize' }
  //%
  static PRIM_DECL_2(allocateSize, oop receiver, oop argument);

  //%prim
  // <IndexedInstanceVariables>
  //   primitiveIndexedObjectSize =
  //   Internal { flags = #(Pure IndexedObject)
  //              name  = 'objArrayPrimitives::size' }
  //%
  static PRIM_DECL_1(size, oop receiver); 

  //%prim
  // <IndexedInstanceVariables> 
  //   primitiveIndexedObjectAt: index     <SmallInteger>
  //                     ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #(Function IndexedObject)
  //              name  = 'objArrayPrimitives::at' }
  //%
  static PRIM_DECL_2(at, oop receiver, oop index);
  
  //%prim
  // <IndexedInstanceVariables>
  //   primitiveIndexedObjectAt: index     <SmallInteger>
  //                        put: c         <Object>
  //                     ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #(Function IndexedObject)
  //              name  = 'objArrayPrimitives::atPut' }
  //%
  static PRIM_DECL_3(atPut, oop receiver, oop index, oop value);

  //%prim
  // <IndexedInstanceVariables>
  //   primitiveIndexedObjectAtAllPut: obj <Object> ^<Self> =
  //   Internal { flags = #(Function IndexedObject)
  //              name  = 'objArrayPrimitives::at_all_put' }
  //%
  static PRIM_DECL_2(at_all_put, oop receiver, oop obj);

  //%prim
  // <IndexedInstanceVariables>
  //   primitiveIndexedObjectReplaceFrom: from      <SmallInteger>
  //                                  to: to        <SmallInteger>
  //                                with: source    <IndexedInstanceVariables>
  //                          startingAt: start     <SmallInteger>
  //                              ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { error = #(OutOfBounds)
  //              flags = #(IndexedObject)
  //              name  = 'objArrayPrimitives::replace_from_to' }
  //%
  static PRIM_DECL_5(replace_from_to, oop receiver, oop from, oop to, oop source, oop start);

  //%prim
  // <IndexedInstanceVariables>
  //   primitiveIndexedObjectCopyFrom: from      <SmallInteger>
  //                       startingAt: start     <SmallInteger>
  //                             size: size      <SmallInteger>
  //                           ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { error = #(OutOfBounds NegativeSize)
  //              flags = #(IndexedObject)
  //              name  = 'objArrayPrimitives::copy_size' }
  //%
  static PRIM_DECL_4(copy_size, oop receiver, oop from, oop start, oop size);

};

