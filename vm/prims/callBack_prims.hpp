/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.8 $ */
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

// Primitives for call back

class callBackPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <NoReceiver> primitiveCallBackReceiver: receiver <Object>
  //                               selector: selector <Symbol>
  //                                 ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'callBackPrimitives::initialize' }
  //%
  static PRIM_DECL_2(initialize, oop receiver, oop selector);

  //%prim
  // <NoReceiver> primitiveCallBackRegisterPascalCall: index   <SmallInteger>
  //                                numberOfArguments: nofArgs <SmallInteger>
  //                                           result: proxy   <Proxy>
  //                                           ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'callBackPrimitives::registerPascalCall' }
  //%
  static PRIM_DECL_3(registerPascalCall, oop index, oop nofArgs, oop result);

  //%prim
  // <NoReceiver> primitiveCallBackRegisterCCall: index   <SmallInteger>
  //                                      result: proxy   <Proxy>
  //                                      ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'callBackPrimitives::registerCCall' }
  //%
  static PRIM_DECL_2(registerCCall, oop index, oop result);

  //%prim
  // <Object> primitiveCallBackUnregister: proxy <Proxy>
  //                               ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'callBackPrimitives::unregister' }
  //%
  static PRIM_DECL_1(unregister, oop proxy);

  //%prim
  // <Object> primitiveCallBackInvokePascal2: proxy <Proxy>
  //                                  ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'callBackPrimitives::invokePascal' }
  //%
  static PRIM_DECL_1(invokePascal, oop proxy);

  //%prim
  // <Object> primitiveCallBackInvokeC2: proxy <Proxy>
  //                             ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name = 'callBackPrimitives::invokeC' }
  //%
  static PRIM_DECL_1(invokeC, oop proxy);

 }; 
