/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.25 $ */
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

// primitives for classes

class behaviorPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <NoReceiver> primitiveNew: class <Behavior> 
  //              tenured: tenured <Boolean>
  //              ifFail: failBlock <PrimFailBlock> ^<Instance> =
  //   Internal { error = #(ReceiverIsIndexable)
  //              name  = 'behaviorPrimitives::allocate3'
  //              flags = #Allocate }
  //%
  static PRIM_DECL_2(allocate3, oop receiver, oop tenured);

  //%prim
  // <Behavior> primitiveNew2IfFail: failBlock <PrimFailBlock> ^<Instance> =
  //   Internal { error = #(ReceiverIsIndexable)
  //              name  = 'behaviorPrimitives::allocate2'
  //              flags = #Allocate }
  //%
  static PRIM_DECL_1(allocate2, oop receiver);

  //%prim
  // <Behavior> primitiveNewIfFail: failBlock <PrimFailBlock> ^<Instance> =
  //   Internal { error = #(ReceiverIsIndexable)
  //              name  = 'behaviorPrimitives::allocate'
  //              flags = #Allocate }
  //%
  static PRIM_DECL_1(allocate, oop receiver);

  // MIXIN

  //%prim
  // <NoReceiver> primitiveBehaviorMixinOf: behavior <Behavior> 
  //                                ifFail: failBlock <PrimFailBlock> ^<Mixin> =
  //   Internal { name = 'behaviorPrimitives::mixinOf' }
  //%
  static PRIM_DECL_1(mixinOf, oop behavior);

  //%prim
  // <NoReceiver> primitiveBehaviorHeaderSizeOf: behavior <Behavior>
  //                                     ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name = 'behaviorPrimitives::headerSize' }
  //%
  static PRIM_DECL_1(headerSize, oop behavior);

  //%prim
  // <NoReceiver> primitiveBehaviorNonIndexableSizeOf: behavior <Behavior>
  //                                           ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name = 'behaviorPrimitives::nonIndexableSize' }
  //%
  static PRIM_DECL_1(nonIndexableSize, oop behavior);

  //%prim
  // <NoReceiver> primitiveBehaviorIsSpecializedClass: behavior  <Behavior>
  //                                           ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { name  = 'behaviorPrimitives::is_specialized_class' }
  //%
  static PRIM_DECL_1(is_specialized_class, oop behavior);

  //%prim
  // <NoReceiver> primitiveBehaviorCanBeSubclassed: behavior  <Behavior>
  //                                        ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { name  = 'behaviorPrimitives::can_be_subclassed' }
  //%
  static PRIM_DECL_1(can_be_subclassed, oop behavior);

  //%prim
  // <NoReceiver> primitiveBehaviorCanHaveInstanceVariables: behavior <Behavior> 
  //                                                 ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { name  = 'behaviorPrimitives::can_have_instance_variables' }
  //%
  static PRIM_DECL_1(can_have_instance_variables, oop behavior);

  //%prim
  // <Behavior> primitiveSuperclass ^<Behavior|Nil> =
  //   Internal { name  = 'behaviorPrimitives::superclass' }
  //%
  static PRIM_DECL_1(superclass, oop receiver);

  //%prim
  // <NoReceiver> primitiveSuperclassOf: class <Behavior>
  //                             ifFail: failBlock <PrimFailBlock> ^<Behavior|Nil> =
  //   Internal { name  = 'behaviorPrimitives::superclass_of' }
  //%
  static PRIM_DECL_1(superclass_of, oop klass);

  // CLASS VARIABLES

  //%prim
  // <NoReceiver> primitiveBehavior: behavior  <Behavior> 
  //                classVariableAt: index     <SmallInteger> 
  //                         ifFail: failBlock <PrimFailBlock> ^<GlobalAssociation> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'behaviorPrimitives::classVariableAt' }
  //%
  static PRIM_DECL_2(classVariableAt, oop behavior, oop index);


  //%prim
  // <NoReceiver> primitiveBehavior: behavior  <Behavior> 
  //           classVariablesIfFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'behaviorPrimitives::classVariables' }
  //%
  static PRIM_DECL_1(classVariables, oop behavior);

  // METHODS

  //%prim
  // <Behavior> primitivePrintMethod: selector  <ByteArray>
  //                          ifFail: failBlock <PrimFailBlock> ^<Behavior> =
  //   Internal { name  = 'behaviorPrimitives::printMethod' }
  //%
  static PRIM_DECL_2(printMethod, oop receiver, oop name);

  //%prim
  // <Behavior> primitiveMethodFor: selector  <CompressedSymbol>
  //                        ifFail: failBlock <PrimFailBlock> ^<Method> =
  //   Internal { name = 'behaviorPrimitives::methodFor'
  //              error = #(NotFound)
  //            }
  //%
  static PRIM_DECL_2(methodFor, oop receiver, oop selector);

  //%prim
  // <NoReceiver> primitiveBehaviorFormat: behavior  <Behavior>
  //                               ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { name  = 'behaviorPrimitives::format' }
  //%
  static PRIM_DECL_1(format, oop behavior);

  //%prim
  // <NoReceiver> primitiveBehaviorVMType: behavior  <Behavior>
  //                               ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal {  doc   = 'Oops, SmallInteger, GlobalAssociation, Method, Float, Array, WeakArray, '
  //               doc   = 'DoubleByteArray, FloatValueArray, ByteArray, Symbol, '
  //               doc   = 'Block, Context, Process, Proxy, Activation, Mixin, and Class'
  //               name  = 'behaviorPrimitives::vm_type' }
  //%
  static PRIM_DECL_1(vm_type, oop behavior);

  //%prim
  // <Behavior> primitiveBehaviorIsClassOf: obj  <Object> ^<Boolean> =
  //   Internal { 
  //              flags = #(LastDeltaFrameNotNeeded)
  //              name  = 'behaviorPrimitives::is_class_of' }
  //%
  static PRIM_DECL_2(is_class_of, oop receiver, oop obj);
};


//%prim
// <NoReceiver> primitiveInlineAllocations: behavior <Behavior>
//                                   count: count <SmallInt> ^<Instance> = 
//   Internal { flags = #(Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveInlineAllocations' }
//%
extern "C" oop primitiveInlineAllocations(oop receiver, oop count);

//%prim
// <Behavior> primitiveNew0: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew0' }
//%
extern "C" oop primitiveNew0(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew1: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew1' }
//%
extern "C" oop primitiveNew1(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew2: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew2' }
//%
extern "C" oop primitiveNew2(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew3: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew3' }
//%
extern "C" oop primitiveNew3(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew4: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew4' }
//%
extern "C" oop primitiveNew4(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew5: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew5' }
//%
extern "C" oop primitiveNew5(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew6: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew6' }
//%
extern "C" oop primitiveNew6(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew7: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew7' }
//%
extern "C" oop primitiveNew7(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew8: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew8' }
//%
extern "C" oop primitiveNew8(oop receiver, oop tenured);

//%prim
// <Behavior> primitiveNew9: tenured <Boolean> ifFail: failBlock <PrimFailBlock> ^<Instance> = 
//   Internal { flags = #(Internal Allocate LastDeltaFrameNotNeeded)
//              name  = 'primitiveNew9' }
//%
extern "C" oop primitiveNew9(oop receiver, oop tenured);
