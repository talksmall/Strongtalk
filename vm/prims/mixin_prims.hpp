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

// Primitives for mixins

class mixinOopPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  // METHODS

  //%prim
  // <NoReceiver> primitiveMixinNumberOfMethodsOf: mixin <Mixin>
  //                                       ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name  = 'mixinOopPrimitives::number_of_methods' }
  //%
  static PRIM_DECL_1(number_of_methods, oop mixin);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //                    methodAt: index     <SmallInteger>
  //                      ifFail: failBlock <PrimFailBlock> ^<Method> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'mixinOopPrimitives::method_at' }
  //%
  static PRIM_DECL_2(method_at, oop mixin, oop index);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //                   addMethod: method    <Method>
  //                      ifFail: failBlock <PrimFailBlock> ^<Method> =
  //   Internal { error = #(IsInstalled)
  //              name  = 'mixinOopPrimitives::add_method' }
  //%
  static PRIM_DECL_2(add_method, oop mixin, oop method);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //              removeMethodAt: index     <SmallInteger> 
  //                      ifFail: failBlock <PrimFailBlock> ^<Method> =
  //   Internal {  error = #(IsInstalled OutOfBounds)
  //               name  = 'mixinOopPrimitives::remove_method_at' }
  //%
  static PRIM_DECL_2(remove_method_at, oop mixin, oop index);


  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //               methodsIfFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { name  = 'mixinOopPrimitives::methods' }
  //%
  static PRIM_DECL_1(methods, oop mixin);


  // INSTANCE VARIABLES

  //%prim
  // <NoReceiver> primitiveMixinNumberOfInstanceVariablesOf: mixin <Mixin>
  //                                                 ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name = 'mixinOopPrimitives::number_of_instance_variables' }
  //%
  static PRIM_DECL_1(number_of_instance_variables, oop mixin);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //          instanceVariableAt: index     <SmallInteger>
  //                      ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'mixinOopPrimitives::instance_variable_at' }
  //%
  static PRIM_DECL_2(instance_variable_at, oop mixin, oop index);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //         addInstanceVariable: name      <Symbol>
  //                      ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { error = #(IsInstalled OutOfBounds)
  //              name = 'mixinOopPrimitives::add_instance_variable' }
  //%
  static PRIM_DECL_2(add_instance_variable, oop mixin, oop name);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //    removeInstanceVariableAt: index     <SmallInteger> 
  //                      ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { error = #(IsInstalled OutOfBounds)
  //              name  = 'mixinOopPrimitives::remove_instance_variable_at' }
  //%
  static PRIM_DECL_2(remove_instance_variable_at, oop mixin, oop index);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //     instanceVariablesIfFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { name  = 'mixinOopPrimitives::instance_variables' }
  //%
  static PRIM_DECL_1(instance_variables, oop mixin);

  // CLASS VARIABLES

  //%prim
  // <NoReceiver> primitiveMixinNumberOfClassVariablesOf: mixin <Mixin>
  //                                              ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name  = 'mixinOopPrimitives::number_of_class_variables' }
  //%
  static PRIM_DECL_1(number_of_class_variables, oop mixin);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //             classVariableAt: index     <SmallInteger>
  //                      ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { error = #(OutOfBounds)
  //              name  = 'mixinOopPrimitives::class_variable_at' }
  //%
  static PRIM_DECL_2(class_variable_at, oop mixin, oop index);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //            addClassVariable: name      <Symbol>
  //                      ifFail: failBlock <PrimFailBlock> ^<Mixin> =
  //   Internal {  error = #(IsInstalled OutOfBounds)
  //               name = 'mixinOopPrimitives::add_class_variable' }
  //%
  static PRIM_DECL_2(add_class_variable, oop mixin, oop name);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //       removeClassVariableAt: index     <SmallInteger> 
  //                      ifFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { error = #(IsInstalled OutOfBounds)
  //              name  = 'mixinOopPrimitives::remove_class_variable_at' }
  //%
  static PRIM_DECL_2(remove_class_variable_at, oop mixin, oop index);

  //%prim
  // <NoReceiver> primitiveMixin: mixin     <Mixin>
  //        classVariablesIfFail: failBlock <PrimFailBlock> ^<Symbol> =
  //   Internal { name  = 'mixinOopPrimitives::class_variables' }
  //%
  static PRIM_DECL_1(class_variables, oop mixin);


  // PRIMARY INVOCATION

  //%prim
  // <NoReceiver> primitiveMixinPrimaryInvocationOf: mixin <Mixin>
  //                                         ifFail: failBlock <PrimFailBlock> ^<Class> =
  //   Internal { name = 'mixinOopPrimitives::primary_invocation' }
  //%
  static PRIM_DECL_1(primary_invocation, oop mixin);

  //%prim
  // <NoReceiver> primitiveMixinSetPrimaryInvocationOf: mixin <Mixin>
  //                                                to: class <Class>
  //                                            ifFail: failBlock <PrimFailBlock> ^<Class> =
  //   Internal { error = #(IsInstalled)
  //              name  = 'mixinOopPrimitives::set_primary_invocation' }
  //%
  static PRIM_DECL_2(set_primary_invocation, oop mixin, oop klass);

  // CLASS MIXIN

  //%prim
  // <NoReceiver> primitiveMixinClassMixinOf: mixin <Mixin>
  //                                  ifFail: failBlock <PrimFailBlock> ^<Mixin> =
  //   Internal { name = 'mixinOopPrimitives::class_mixin' }
  //%
  static PRIM_DECL_1(class_mixin, oop mixin);

  //%prim
  // <NoReceiver> primitiveMixinSetClassMixinOf: mixin      <Mixin>
  //                                         to: classMixin <Mixin>
  //                                     ifFail: failBlock <PrimFailBlock> ^<Mixin> =
  //   Internal { error = #(IsInstalled)
  //              name  = 'mixinOopPrimitives::set_class_mixin' }
  //%
  static PRIM_DECL_2(set_class_mixin, oop mixin, oop class_mixin);

  //%prim
  // <NoReceiver> primitiveMixinIsInstalled: mixin <Mixin>
  //                                 ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { name = 'mixinOopPrimitives::is_installed' }
  //%
  static PRIM_DECL_1(is_installed, oop mixin);

  //%prim
  // <NoReceiver> primitiveMixinSetInstalled: mixin <Mixin>
  //                                  ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { name = 'mixinOopPrimitives::set_installed' }
  //%
  static PRIM_DECL_1(set_installed, oop mixin);

    //%prim
  // <NoReceiver> primitiveMixinSetUnInstalled: mixin <Mixin>
  //                                    ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { name = 'mixinOopPrimitives::set_uninstalled' }
  //%
  static PRIM_DECL_1(set_uninstalled, oop mixin);

};
