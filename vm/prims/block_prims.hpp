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

// Primitives for blocks

// The following 9 primitives are implemented in the interpreter
// but we still need the interface!

//%prim
// <BlockWithoutArguments> primitiveValue ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue0' }
//%
extern "C" oop primitiveValue0(oop blk);

//%prim
// <BlockWithOneArgument> primitiveValue: arg1 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue1' }
//%
extern "C" oop primitiveValue1(oop blk, oop arg1);

//%prim
// <BlockWithTwoArguments> primitiveValue: arg1 <Object> value: arg2 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue2' }
//%
extern "C" oop primitiveValue2(oop blk, oop arg1, oop arg2);

//%prim
// <BlockWithThreeArguments> primitiveValue: arg1 <Object> value: arg2 <Object> value: arg3 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue3' }
//%
extern "C" oop primitiveValue3(oop blk, oop arg1, oop arg2, oop arg3);

//%prim
// <BlockWithFourArguments> primitiveValue: arg1 <Object> value: arg2 <Object> value: arg3 <Object>
//                                   value: arg4 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue4' }
//%
extern "C" oop primitiveValue4(oop blk, oop arg1, oop arg2, oop arg3, oop arg4);

//%prim
// <BlockWithFiveArguments> primitiveValue: arg1 <Object> value: arg2 <Object> value: arg3 <Object>
//                                   value: arg4 <Object> value: arg5 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue5' }
//%
extern "C" oop primitiveValue5(oop blk, oop arg1, oop arg2, oop arg3, oop arg4, oop arg5);

//%prim
// <BlockWithSixArguments> primitiveValue: arg1 <Object> value: arg2 <Object> value: arg3 <Object>
//                                  value: arg4 <Object> value: arg5 <Object> value: arg6 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue6' }
//%
extern "C" oop primitiveValue6(oop blk, oop arg1, oop arg2, oop arg3, oop arg4, oop arg5,
                               oop arg6);

//%prim
// <BlockWithSevenArguments> primitiveValue: arg1 <Object> value: arg2 <Object> value: arg3 <Object>
//                                    value: arg4 <Object> value: arg5 <Object> value: arg6 <Object>
//                                    value: arg7 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue7' }
//%
extern "C" oop primitiveValue7(oop blk, oop arg1, oop arg2, oop arg3, oop arg4, oop arg5,
                               oop arg6, oop arg7);

//%prim
// <BlockWithEightArguments> primitiveValue: arg1 <Object> value: arg2 <Object> value: arg3 <Object>
//                                    value: arg4 <Object> value: arg5 <Object> value: arg6 <Object>
//                                    value: arg7 <Object> value: arg8 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue8' }
//%
extern "C" oop primitiveValue8(oop blk, oop arg1, oop arg2, oop arg3, oop arg4, oop arg5,
                               oop arg6, oop arg7, oop arg8);

//%prim
// <BlockWithNineArguments> primitiveValue: arg1 <Object> value: arg2 <Object> value: arg3 <Object>
//                                   value: arg4 <Object> value: arg5 <Object> value: arg6 <Object>
//                                   value: arg7 <Object> value: arg8 <Object> value: arg9 <Object> ^<Object> =
//   Internal {
//     flags = #(NLR Block LastDeltaFrameNotNeeded)
//     name  = 'primitiveValue9' }
//%
extern "C" oop primitiveValue9(oop blk, oop arg1, oop arg2, oop arg3, oop arg4, oop arg5,
                               oop arg6, oop arg7, oop arg8, oop arg9);

// Instead we should come up with a
// generic solution for up to 255 arguments at some point. (gri)

// The following primitives are called directly by the interpreter.

extern "C" blockClosureOop allocateBlock(smiOop nofArgs);
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate: size <SmallInteger> ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate)
//     name  = 'allocateBlock' }
//%
extern "C" blockClosureOop allocateTenuredBlock(smiOop nofArgs);  // for compiler


extern "C" blockClosureOop allocateBlock0();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate0 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock0' }
//%

extern "C" blockClosureOop allocateBlock1();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate1 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock1' }
//%

extern "C" blockClosureOop allocateBlock2();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate2 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock2' }
//%

extern "C" blockClosureOop allocateBlock3();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate3 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock3' }
//%

extern "C" blockClosureOop allocateBlock4();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate4 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock4' }
//%

extern "C" blockClosureOop allocateBlock5();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate5 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock5' }
//%

extern "C" blockClosureOop allocateBlock6();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate6 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock6' }
//%

extern "C" blockClosureOop allocateBlock7();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate7 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock7' }
//%

extern "C" blockClosureOop allocateBlock8();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate8 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock8' }
//%

extern "C" blockClosureOop allocateBlock9();
//%prim
// <NoReceiver> primitiveCompiledBlockAllocate9 ^<Block> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateBlock9' }
//%

extern "C" contextOop allocateContext(smiOop nofVars);
//%prim
// <NoReceiver> primitiveCompiledContextAllocate: size <SmallInteger> ^<Object> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateContext' }
//%

extern "C" contextOop allocateContext0();
//%prim
// <NoReceiver> primitiveCompiledContextAllocate0 ^<Object> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateContext0' }
//%
extern "C" contextOop allocateContext1();
//%prim
// <NoReceiver> primitiveCompiledContextAllocate1 ^<Object> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateContext1' }
//%
extern "C" contextOop allocateContext2();
//%prim
// <NoReceiver> primitiveCompiledContextAllocate2 ^<Object> =
//   Internal {
//     flags = #(Internal Block Allocate LastDeltaFrameNotNeeded)
//     name  = 'allocateContext2' }
//%

PRIM_DECL_2(unwindprotect, oop receiver, oop protectBlock);
//%prim
// <BlockWithoutArguments> primitiveUnwindProtect: protect   <BlockWithoutArguments> 
//                                         ifFail: failBlock <PrimFailBlock> ^<Object> =
//   Internal {
//     doc   = 'Evaluates the receiver block and if it returns via a non-local-return'
//     doc   = 'the protect block is invoked.'
//     doc   = 'The original non-local-return continues after evaluation of the protect block.'
//     flags = #(NLR)
//     name  = 'unwindprotect' }
//%

PRIM_DECL_1(blockRepeat, oop receiver);
//%prim
// <BlockWithoutArguments> primitiveRepeat ^<BottomType> =
//   Internal {
//     doc   = 'Repeats evaluating the receiver block'
//     flags = #(NLR)
//     name  = 'blockRepeat' }
//%

PRIM_DECL_1(block_method, oop receiver);
//%prim
// <Block> primitiveBlockMethod ^<Method> =
//   Internal {
//     doc   = 'Returns the block method'
//     name  = 'block_method' }
//%

PRIM_DECL_1(block_is_optimized, oop receiver);
//%prim
// <Block> primitiveBlockIsOptimized ^<Boolean> =
//   Internal {
//     doc   = 'Tells whether the block has optimized code'
//     name  = 'block_is_optimized' }
//%
