/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.21 $ */
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

// Primitives for proxy

class proxyOopPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  // Conversion

  //%prim
  // <Proxy> primitiveProxyGetIfFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(ConversionFailed)
  //              name  = 'proxyOopPrimitives::getSmi' }
  //%
  static PRIM_DECL_1(getSmi, oop receiver);

  //%prim
  // <Proxy> primitiveProxySet: value     <SmallInteger|Proxy>
  //                    ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { name = 'proxyOopPrimitives::set' }
  //%
  static PRIM_DECL_2(set, oop receiver, oop value);

  //%prim
  // <Proxy> primitiveProxySetHigh: high      <SmallInteger>
  //                           low: low       <SmallInteger>
  //                        ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { name = 'proxyOopPrimitives::setHighLow' }
  //%
  static PRIM_DECL_3(setHighLow, oop receiver, oop high, oop low);

  //%prim
  // <Proxy> primitiveProxyGetHigh ^<SmallInteger> =
  //   Internal { name = 'proxyOopPrimitives::getHigh' }
  //%
  static PRIM_DECL_1(getHigh, oop receiver);

  //%prim
  // <Proxy> primitiveProxyGetLow ^<SmallInteger> =
  //   Internal { name = 'proxyOopPrimitives::getLow' }
  //%
  static PRIM_DECL_1(getLow, oop receiver);

  // Testing

  //%prim
  // <Proxy> primitiveProxyIsNull ^<Boolean> =
  //   Internal { name = 'proxyOopPrimitives::isNull' }
  //%
  static PRIM_DECL_1(isNull, oop receiver);

  //%prim
  // <Proxy> primitiveProxyIsAllOnes ^<Boolean> =
  //   Internal { name = 'proxyOopPrimitives::isAllOnes' }
  //%
  static PRIM_DECL_1(isAllOnes, oop receiver);

  // Memory management

  //%prim
  // <Proxy> primitiveProxyMalloc: size      <SmallInteger>
  //                       ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { name = 'proxyOopPrimitives::malloc' }
  //%
  static PRIM_DECL_2(malloc, oop receiver, oop size);

  //%prim
  // <Proxy> primitiveProxyCalloc: size      <SmallInteger>
  //                       ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { name = 'proxyOopPrimitives::calloc' }
  //%
  static PRIM_DECL_2(calloc, oop receiver, oop size);

  //%prim
  // <Proxy> primitiveProxyFree ^<Self> =
  //   Internal { name = 'proxyOopPrimitives::free' }
  //%
  static PRIM_DECL_1(free, oop receiver);

  // The remaining primitives are used for
  // dereferencing the proxy value.

  // Byte

  //%prim
  // <Proxy> primitiveProxyByteAt: offset    <SmallInteger>
  //                       ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(LastDeltaFrameNotNeeded)
  //              name = 'proxyOopPrimitives::byteAt' }
  //%
  static PRIM_DECL_2(byteAt, oop receiver, oop offset);

  //%prim
  // <Proxy> primitiveProxyByteAt: offset    <SmallInteger>
  //                          put: value     <SmallInteger>
  //                       ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { flags = #(LastDeltaFrameNotNeeded)
  //              name  = 'proxyOopPrimitives::byteAtPut' }
  //%
  static PRIM_DECL_3(byteAtPut, oop receiver, oop offset, oop value);

  // Double byte

  //%prim
  // <Proxy> primitiveProxyDoubleByteAt: offset   <SmallInteger>
  //                            ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name = 'proxyOopPrimitives::doubleByteAt' }
  //%
  static PRIM_DECL_2(doubleByteAt, oop receiver, oop offset);

  //%prim
  // <Proxy> primitiveProxyDoubleByteAt: offset    <SmallInteger>
  //                                put: value     <SmallInteger>
  //                             ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name = 'proxyOopPrimitives::doubleByteAtPut' }
  //%
  static PRIM_DECL_3(doubleByteAtPut, oop receiver, oop offset, oop value); 

  // Smi

  //%prim
  // <Proxy> primitiveProxySmiAt: offset    <SmallInteger>
  //                      ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { error = #(ConversionFailed)
  //              name  = 'proxyOopPrimitives::smiAt' }
  //%
  static PRIM_DECL_2(smiAt, oop receiver, oop offset);

  //%prim
  // <Proxy> primitiveProxySmiAt: offset    <SmallInteger>
  //                         put: value     <SmallInteger>
  //                      ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { name = 'proxyOopPrimitives::smiAtPut' }
  //%
  static PRIM_DECL_3(smiAtPut, oop receiver, oop offset, oop value);

  // Proxy

  //%prim
  // <Proxy> primitiveProxySubProxyAt: offset    <SmallInteger>
  //                           result: result    <Proxy>
  //                           ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { name = 'proxyOopPrimitives::subProxyAt' }
  //%
  static PRIM_DECL_3(subProxyAt, oop receiver, oop offset, oop result);

  //%prim
  // <Proxy> primitiveProxyProxyAt: offset    <SmallInteger>
  //                        result: result    <Proxy>
  //                        ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { name = 'proxyOopPrimitives::proxyAt' }
  //%
  static PRIM_DECL_3(proxyAt, oop receiver, oop offset, oop result);

  //%prim
  // <Proxy> primitiveProxyProxyAt: offset    <SmallInteger>
  //                           put: value     <Proxy>
  //                        ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { name = 'proxyOopPrimitives::proxyAtPut' }
  //%
  static PRIM_DECL_3(proxyAtPut, oop receiver, oop offset, oop value);

  // Single precision floats

  //%prim
  // <Proxy> primitiveProxySinglePrecisionFloatAt: offset    <SmallInteger>
  //                                       ifFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { name  = 'proxyOopPrimitives::singlePrecisionFloatAt' }
  //%
  static PRIM_DECL_2(singlePrecisionFloatAt, oop receiver, oop offset);

  //%prim
  // <Proxy> primitiveProxySinglePrecisionFloatAt: offset    <SmallInteger>
  //                                          put: value     <Float>
  //                                       ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { error = #(ConversionFailed)
  //              name = 'proxyOopPrimitives::singlePrecisionFloatAtPut' }
  //%
  static PRIM_DECL_3(singlePrecisionFloatAtPut, oop receiver, oop offset, oop value);

  // Double precision floats

  //%prim
  // <Proxy> primitiveProxyDoublePrecisionFloatAt: offset    <SmallInteger>
  //                                       ifFail: failBlock <PrimFailBlock> ^<Float> =
  //   Internal { name  = 'proxyOopPrimitives::doublePrecisionFloatAt' }
  //%
  static PRIM_DECL_2(doublePrecisionFloatAt, oop receiver, oop offset);

  //%prim
  // <Proxy> primitiveProxyDoublePrecisionFloatAt: offset    <SmallInteger>
  //                                          put: value     <Float>
  //                                       ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { error = #(ConversionFailed)
  //              name = 'proxyOopPrimitives::doublePrecisionFloatAtPut' }
  //%
  static PRIM_DECL_3(doublePrecisionFloatAtPut, oop receiver, oop offset, oop value);

  // API Calls through proxies

  //%prim
  // <Proxy> primitiveAPICallResult: proxy <Proxy>
  //                         ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { flags = #NLR
  //              name  = 'proxyOopPrimitives::callOut0' }
  //%
  static PRIM_DECL_2(callOut0, oop receiver, oop result);

  //%prim
  // <Proxy> primitiveAPICallValue: arg1  <Proxy|SmallInteger>
  //                        result: proxy <Proxy>
  //                        ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { flags = #NLR
  //              name  = 'proxyOopPrimitives::callOut1' }
  //%
  static PRIM_DECL_3(callOut1, oop receiver, oop arg1, oop result);

  //%prim
  // <Proxy> primitiveAPICallValue: arg1      <Proxy|SmallInteger>
  //                         value: arg2      <Proxy|SmallInteger>
  //                        result: proxy     <Proxy>
  //                        ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { flags = #NLR
  //              name  = 'proxyOopPrimitives::callOut2' }
  //%
  static PRIM_DECL_4(callOut2, oop receiver, oop arg1, oop arg2, oop result);

  //%prim
  // <Proxy> primitiveAPICallValue: arg1      <Proxy|SmallInteger>
  //                         value: arg2      <Proxy|SmallInteger>
  //                         value: arg3      <Proxy|SmallInteger>
  //                        result: proxy     <Proxy>
  //                        ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { flags = #NLR
  //              name  = 'proxyOopPrimitives::callOut3' }
  //%
  static PRIM_DECL_5(callOut3, oop receiver, oop arg1, oop arg2, oop arg3, oop result);

  //%prim
  // <Proxy> primitiveAPICallValue: arg1      <Proxy|SmallInteger>
  //                         value: arg2      <Proxy|SmallInteger>
  //                         value: arg3      <Proxy|SmallInteger>
  //                         value: arg4      <Proxy|SmallInteger>
  //                        result: proxy     <Proxy>
  //                        ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { flags = #NLR
  //              name  = 'proxyOopPrimitives::callOut4' }
  //%
  static PRIM_DECL_6(callOut4, oop receiver, oop arg1, oop arg2, oop arg3, oop arg4, oop result);

  //%prim
  // <Proxy> primitiveAPICallValue: arg1      <Proxy|SmallInteger>
  //                         value: arg2      <Proxy|SmallInteger>
  //                         value: arg3      <Proxy|SmallInteger>
  //                         value: arg4      <Proxy|SmallInteger>
  //                         value: arg5      <Proxy|SmallInteger>
  //                        result: proxy     <Proxy>
  //                        ifFail: failBlock <PrimFailBlock> ^<Proxy> =
  //   Internal { flags = #NLR
  //              name  = 'proxyOopPrimitives::callOut5' }
  //%
  static PRIM_DECL_7(callOut5, oop receiver, oop arg1, oop arg2, oop arg3, oop arg4, oop arg5, oop result);


};
