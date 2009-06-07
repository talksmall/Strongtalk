/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.28 $ */
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

// Primitives for debugging

class debugPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <NoReceiver> primitiveBooleanFlagAt: name      <Symbol>
  //                              ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { 
  //      error = #(NotFound)
  //      name  = 'debugPrimitives::boolAt' }
  //%
  static PRIM_DECL_1(boolAt,    oop name);

  //%prim
  // <NoReceiver> primitiveBooleanFlagAt: name      <Symbol>
  //                                 put: value     <Boolean>
  //                              ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { 
  //      error = #(NotFound)
  //      name  = 'debugPrimitives::boolAtPut' }
  //%
  static PRIM_DECL_2(boolAtPut, oop name, oop value);

  //%prim
  // <NoReceiver> primitiveSmallIntegerFlagAt: name      <Symbol>
  //                                   ifFail: failBlock <PrimFailBlock> ^<SmallInteger> =
  //   Internal { 
  //      error = #(NotFound)
  //      name  = 'debugPrimitives::smiAt' }
  //%
  static PRIM_DECL_1(smiAt,     oop name);

  //%prim
  // <NoReceiver> primitiveSmallIntegerFlagAt: name      <Symbol>
  //                                      put: value     <Boolean>
  //                                   ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { 
  //      error = #(NotFound)
  //      name  = 'debugPrimitives::smiAtPut' }
  //%
  static PRIM_DECL_2(smiAtPut,  oop name, oop value);

  //%prim
  // <NoReceiver> primitiveClearLookupCache ^<Object> =
  //   Internal { name  = 'debugPrimitives::clearLookupCache' }
  //%
  static PRIM_DECL_0(clearLookupCache);

  //%prim
  // <NoReceiver> primitiveClearLookupCacheStatistics ^<Object> =
  //   Internal { name  = 'debugPrimitives::clearLookupCacheStatistics' }
  //%
  static PRIM_DECL_0(clearLookupCacheStatistics);

  //%prim
  // <NoReceiver> primitivePrintLookupCacheStatistics ^<Object> =
  //   Internal { name  = 'debugPrimitives::printLookupCacheStatistics' }
  //%
  static PRIM_DECL_0(printLookupCacheStatistics);

  //%prim
  // <NoReceiver> primitivePrintLayout ^<Object> =
  //   Internal { name  = 'debugPrimitives::printMemoryLayout' }
  //%
  static PRIM_DECL_0(printMemoryLayout);

  //%prim
  // <NoReceiver> primitiveDecodeAllMethods ^<Object> =
  //   Internal { name  = 'debugPrimitives::decodeAllMethods' }
  //%
  static PRIM_DECL_0(decodeAllMethods);

  //%prim
  // <Object> primitivePrintMethodCodes: selector <Symbol>
  //                             ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NotFound)
  //              name  = 'debugPrimitives::printMethodCodes' }
  //%
  static PRIM_DECL_2(printMethodCodes, oop receiver, oop sel);

  //%prim
  // <Object> primitiveGenerateIR: selector <Symbol>
  //                       ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NotFound)
  //              name  = 'debugPrimitives::generateIR' }
  //%
  static PRIM_DECL_2(generateIR, oop receiver, oop sel);

  //%prim
  // <Object> primitiveOptimizeMethod: selector <Symbol>
  //                           ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NotFound)
  //              name  = 'debugPrimitives::optimizeMethod' }
  //%
  static PRIM_DECL_2(optimizeMethod, oop receiver, oop sel);

  //%prim
  // <Object> primitiveDecodeMethod: selector <Symbol>
  //                         ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { error = #(NotFound)
  //              name  = 'debugPrimitives::decodeMethod' }
  //%
  static PRIM_DECL_2(decodeMethod, oop receiver, oop sel);

  //%prim
  // <NoReceiver> primitiveTimerStart ^<Object> =
  //   Internal { name  = 'debugPrimitives::timerStart' }
  //%
  static PRIM_DECL_0(timerStart);

  //%prim
  // <NoReceiver> primitiveTimerStop ^<Object> =
  //   Internal { name  = 'debugPrimitives::timerStop' }
  //%
  static PRIM_DECL_0(timerStop);

  //%prim
  // <NoReceiver> primitiveTimerPrintBuffer ^<Object> =
  //   Internal { name  = 'debugPrimitives::timerPrintBuffer' }
  //%
  static PRIM_DECL_0(timerPrintBuffer);

  //%prim
  // <NoReceiver> primitiveInterpreterInvocationCounterLimit ^<SmallInteger> =
  //   Internal { name = 'debugPrimitives::interpreterInvocationCounterLimit' }
  //%
  static PRIM_DECL_0(interpreterInvocationCounterLimit);

  //%prim
  // <NoReceiver> primitiveSetInterpreterInvocationCounterLimitTo: limit <SmallInteger>
  //                                                       ifFail: failBlock <PrimFailBlock> ^ <Object> =
  //   Internal { name = 'debugPrimitives::setInterpreterInvocationCounterLimit' }
  //%
  static PRIM_DECL_1(setInterpreterInvocationCounterLimit, oop limit);

  //%prim
  // <NoReceiver> primitiveClearInvocationCounters ^<Object> =
  //   Internal { name  = 'debugPrimitives::clearInvocationCounters' }
  //%
  static PRIM_DECL_0(clearInvocationCounters);

  //%prim
  // <NoReceiver> primitivePrintInvocationCounterHistogram: size <SmallInteger> 
  //                                                ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name  = 'debugPrimitives::printInvocationCounterHistogram' }
  //%
  static PRIM_DECL_1(printInvocationCounterHistogram, oop size);

  //%prim
  // <NoReceiver> primitivePrintObjectHistogram ^<Object> =
  //   Internal { name  = 'debugPrimitives::printObjectHistogram' }
  //%
  static PRIM_DECL_0(printObjectHistogram);

  //%prim
  // <NoReceiver> primitiveClearInlineCaches ^<Object> =
  //   Internal {name  = 'debugPrimitives::clearInlineCaches' }
  //%
  static PRIM_DECL_0(clearInlineCaches);

  //%prim
  // <NoReceiver> primitiveClearNMethodCounters ^<Object> =
  //   Internal { name  = 'debugPrimitives::clearNMethodCounters' }
  //%
  static PRIM_DECL_0(clearNMethodCounters);

  //%prim
  // <NoReceiver> primitivePrintNMethodCounterHistogram: size <SmallInteger> 
  //                                        ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { name  = 'debugPrimitives::printNMethodCounterHistogram' }
  //%
  static PRIM_DECL_1(printNMethodCounterHistogram, oop size);

  //%prim
  // <NoReceiver> primitiveNumberOfMethodInvocations ^<SmallInteger> =
  //   Internal { name  = 'debugPrimitives::numberOfMethodInvocations' }
  //%
  static PRIM_DECL_0(numberOfMethodInvocations);

  //%prim
  // <NoReceiver> primitiveNumberOfNMethodInvocations ^<SmallInteger> =
  //   Internal { name  = 'debugPrimitives::numberOfNMethodInvocations' }
  //%
  static PRIM_DECL_0(numberOfNMethodInvocations);

  // Accessors to LookupCache statistics

  //%prim
  // <NoReceiver> primitiveNumberOfPrimaryLookupCacheHits ^<SmallInteger> =
  //   Internal { name  = 'debugPrimitives::numberOfPrimaryLookupCacheHits' }
  //%
  static PRIM_DECL_0(numberOfPrimaryLookupCacheHits);

  //%prim
  // <NoReceiver> primitiveNumberOfSecondaryLookupCacheHits ^<SmallInteger> =
  //   Internal { name  = 'debugPrimitives::numberOfSecondaryLookupCacheHits' }
  //%
  static PRIM_DECL_0(numberOfSecondaryLookupCacheHits);

  //%prim
  // <NoReceiver> primitiveNumberOfLookupCacheMisses ^<SmallInteger> =
  //   Internal { name  = 'debugPrimitives::numberOfLookupCacheMisses' }
  //%
  static PRIM_DECL_0(numberOfLookupCacheMisses);

  //%prim
  // <NoReceiver> primitiveClearPrimitiveCounters ^<Object> =
  //   Internal { name  = 'debugPrimitives::clearPrimitiveCounters' }
  //%
  static PRIM_DECL_0(clearPrimitiveCounters);

  //%prim
  // <NoReceiver> primitivePrintPrimitiveCounters ^<Object> =
  //   Internal { name  = 'debugPrimitives::printPrimitiveCounters' }
  //%
  static PRIM_DECL_0(printPrimitiveCounters);

  //%prim
  // <NoReceiver> primitiveDeoptimizeStacks ^<Object> =
  //   Internal { doc   = 'Deoptimizes all stack to the canonical form'
  //              flags = #(NLR)
  //              name  = 'debugPrimitives::deoptimizeStacks' }
  //%
  static PRIM_DECL_0(deoptimizeStacks);

  //%prim
  // <NoReceiver> primitiveVerify ^<Object> =
  //   Internal { doc   = 'Verify the system'
  //              name  = 'debugPrimitives::verify' }
  //%
  static PRIM_DECL_0(verify);
};
