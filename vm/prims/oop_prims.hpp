/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.19 $ */
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

// Primitives applying to all objects

class oopPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <Object> primitiveBecome: anObject  <Object>
  //                   ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(RecieverHasWrongType)
  //              name  = 'oopPrimitives::become' }
  //%
  static PRIM_DECL_2(become, oop receiver, oop argument);

  //%prim
  // <Object> primitiveInstVarAt: index     <SmallInteger>
  //                      ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'oopPrimitives::instVarAt' }
  //%
  static PRIM_DECL_2(instVarAt, oop receiver, oop index);

  //%prim
  // <Reciever> primitiveInstVarNameFor: obj       <Object>
  //                                 at: index     <SmallInteger>
  //                             ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'oopPrimitives::instance_variable_name_at' }
  //%
  static PRIM_DECL_2(instance_variable_name_at, oop obj, oop index);

  //%prim
  // <Object> primitiveInstVarAt: index     <SmallInteger>
  //                         put: contents  <Object>
  //                      ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'oopPrimitives::instVarAtPut' }
  //%
  static PRIM_DECL_3(instVarAtPut, oop receiver, oop index, oop value);

  //%prim
  // <Object> primitiveHash ^<SmallInteger> =
  //   Internal { name = 'oopPrimitives::hash' }
  //%
  static PRIM_DECL_1(hash, oop receiver);

  //%prim
  // <NoReceiver> primitiveHashOf: obj <Object> ^<SmallInteger> =
  //   Internal { name = 'oopPrimitives::hash_of' }
  //%
  static PRIM_DECL_1(hash_of, oop obj);

  //%prim
  // <Object> primitiveShallowCopyIfFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(ReceiverHasWrongType)
  //              name  = 'oopPrimitives::shallowCopy' }
  //%
  static PRIM_DECL_1(shallowCopy, oop receiver);

  //%prim
  // <Object> primitiveCopyTenuredIfFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NotOops)
  //              name  = 'oopPrimitives::copy_tenured' }
  //%
  static PRIM_DECL_1(copy_tenured, oop receiver);

  //%prim
  // <Object> primitiveEqual: anObject <Object> ^<Boolean> =
  //   Internal { flags = #Pure
  //		  name  = 'oopPrimitives::equal' }
  //%
  static PRIM_DECL_2(equal, oop receiver, oop argument);

  //%prim
  // <Object> primitiveNotEqual: anObject <Object> ^<Boolean> =
  //   Internal { flags = #Pure
  //		  name  = 'oopPrimitives::not_equal' }
  //%
  static PRIM_DECL_2(not_equal, oop receiver, oop argument);

  //%prim
  // <Object> primitiveOopSize ^<SmallInteger> = 
  //   Internal {
  //     flags = #Pure
  //     name = 'oopPrimitives::oop_size'}
  //%
  static PRIM_DECL_1(oop_size, oop receiver);

  //%prim
  // <Object> primitiveClass ^<Self class> = 
  //   Internal {
  //     flags = #(Pure LastDeltaFrameNotNeeded)
  //     name = 'oopPrimitives::klass'}
  //%
  static PRIM_DECL_1(klass, oop receiver);

  //%prim
  // <NoReceiver> primitiveClassOf: obj <Object> ^<Behavior> = 
  //   Internal {
  //     flags = #(Pure LastDeltaFrameNotNeeded)
  //     name = 'oopPrimitives::klass_of'}
  //%
  static PRIM_DECL_1(klass_of, oop obj);

  //%prim
  // <Object> primitivePrint ^<Self> =
  //   Internal { name = 'oopPrimitives::print'}
  //%
  static PRIM_DECL_1(print, oop receiver);

  //%prim
  // <Object> primitivePrintValue ^<Self> =
  //   Internal { name = 'oopPrimitives::printValue'}
  //%
  static PRIM_DECL_1(printValue, oop receiver);

  //%prim
  // <Object> primitiveAsObjectID ^<SmallInteger> =
  //   Internal { name = 'oopPrimitives::asObjectID'}
  //%
  static PRIM_DECL_1(asObjectID, oop receiver);

  //%prim
  // <Object> primitivePerform: selector  <CompressedSymbol>
  //                    ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'oopPrimitives::perform'
  //              error = #(SelectorHasWrongNumberOfArguments)
  //            }
  //%
  static PRIM_DECL_2(perform, oop receiver, oop selector);

  //%prim
  // <Object> primitivePerform: selector  <CompressedSymbol>
  //                      with: arg1      <Object>
  //                    ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'oopPrimitives::performWith'
  //              error = #(SelectorHasWrongNumberOfArguments)
  //              flags = #(NLR)
  //            }
  //%
  static PRIM_DECL_3(performWith, oop receiver, oop selector, oop arg1);

  //%prim
  // <Object> primitivePerform: selector  <CompressedSymbol>
  //                      with: arg1      <Object>
  //                      with: arg2      <Object>
  //                    ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'oopPrimitives::performWithWith'
  //              error = #(SelectorHasWrongNumberOfArguments)
  //              flags = #(NLR)
  //            }
  //%
  static PRIM_DECL_4(performWithWith, oop receiver, oop selector, oop arg1, oop arg2);

  //%prim
  // <Object> primitivePerform: selector  <CompressedSymbol>
  //                      with: arg1      <Object>
  //                      with: arg2      <Object>
  //                      with: arg3      <Object>
  //                    ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'oopPrimitives::performWithWithWith'
  //              error = #(SelectorHasWrongNumberOfArguments)
  //              flags = #(NLR)
  //            }
  //%
  static PRIM_DECL_5(performWithWithWith, oop receiver, oop selector, oop arg1, oop arg2, oop arg3);

  //%prim
  // <Object> primitivePerform: selector  <CompressedSymbol>
  //                 arguments: args      <Array>
  //                    ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'oopPrimitives::performArguments'
  //              error = #(SelectorHasWrongNumberOfArguments)
  //              flags = #(NLR)
  //            }
  //%
  static PRIM_DECL_3(performArguments, oop receiver, oop selector, oop args);
}; 
