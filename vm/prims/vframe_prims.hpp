/* Copyright 1996 LongView Technologies L.L.C. $Revision: 1.2 $ */
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


// Primitives for vframe

class vframeOopPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <Activation> primitiveActivationProcess ^<SmallInteger> =
  //   Internal { flags = #(Pure)
  //              name  = 'vframeOopPrimitives::index' }
  //%
  static PRIM_DECL_1(process, oop receiver);

  //%prim
  // <Activation> primitiveActivationIndex ^<SmallInteger> =
  //   Internal { flags = #(Pure)
  //              name  = 'vframeOopPrimitives::index' }
  //%
  static PRIM_DECL_1(index, oop receiver);

  //%prim
  // <Activation> primitiveActivationTimeStamp ^<SmallInteger> =
  //   Internal { flags = #(Pure)
  //              name  = 'vframeOopPrimitives::time_stamp' }
  //%
  static PRIM_DECL_1(time_stamp, oop receiver);

  //%prim
  // <Activation> primitiveActivationIsSmalltalkActivationIfFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { flags = #(Pure)
  //              errors = #(ActivationIsInvalid)
  //              name  = 'vframeOopPrimitives::is_smalltalk_activation' }
  //%
  static PRIM_DECL_1(is_smalltalk_activation, oop receiver);

  //%prim
  // <Activation> primitiveActivationByteCodeIndexIfFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags  = #(Pure)
  //              errors = #(ActivationIsInvalid ExternalActivation)
  //              name   = 'vframeOopPrimitives::byte_code_index' }
  //%
  static PRIM_DECL_1(byte_code_index, oop receiver);

  //%prim
  // <Activation> primitiveActivationExpressionStackIfFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { flags  = #(Pure)
  //              errors = #(ActivationIsInvalid ExternalActivation)
  //              name   = 'vframeOopPrimitives::expression_stack' }
  //%
  static PRIM_DECL_1(expression_stack, oop receiver);

  //%prim
  // <Activation> primitiveActivationMethodIfFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags  = #(Pure)
  //              errors = #(ActivationIsInvalid ExternalActivation)
  //              name   = 'vframeOopPrimitives::method' }
  //%
  static PRIM_DECL_1(method, oop receiver);

  //%prim
  // <Activation> primitiveActivationReceiverIfFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags  = #(Pure)
  //              errors = #(ActivationIsInvalid ExternalActivation)
  //              name   = 'vframeOopPrimitives::receiver' }
  //%
  static PRIM_DECL_1(receiver, oop recv);

  //%prim
  // <Activation> primitiveActivationTemporariesIfFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { flags  = #(Pure)
  //              errors = #(ActivationIsInvalid ExternalActivation)
  //              name   = 'vframeOopPrimitives::temporaries' }
  //%
  static PRIM_DECL_1(temporaries, oop receiver);

  //%prim
  // <Activation> primitiveActivationArgumentsIfFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { flags  = #(Pure)
  //              errors = #(ActivationIsInvalid ExternalActivation)
  //              name   = 'vframeOopPrimitives::arguments' }
  //%
  static PRIM_DECL_1(arguments, oop receiver);

  //%prim
  // <Activation> primitiveActivationPrettyPrintIfFail: failBlock <PrimFailBlock> ^<IndexedByteInstanceVariables> =
  //   Internal { flags  = #(Pure)
  //              errors = #(ActivationIsInvalid ExternalActivation)
  //              name   = 'vframeOopPrimitives::pretty_print' }
  //%
  static PRIM_DECL_1(pretty_print, oop receiver);

};
