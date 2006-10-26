/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.26 $ */
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

# include "incls/_precompiled.incl"
# include "incls/_block_prims.cpp.incl"

TRACE_FUNC(TraceBlockPrims, "block")

static inline void inc_calls() {}

static inline void inc_block_counter() {
#ifdef ASSERT
  NumberOfBlockAllocations++;
#endif
}

static inline void inc_context_counter() {
#ifdef ASSERT
  NumberOfContextAllocations++;
#endif
}

extern "C" blockClosureOop allocateTenuredBlock(smiOop nofArgs) {
  PROLOGUE_1("allocateBlock", nofArgs);
  blockClosureOop blk =
    as_blockClosureOop(Universe::allocate_tenured(sizeof(blockClosureOopDesc)/oopSize));
  blk->init_mark();
  blk->set_klass_field(blockClosureKlass::blockKlassFor(nofArgs->value()));
  inc_block_counter();
  return blk;
}

// TODO: Implement the following function (gri)
extern "C" blockClosureOop allocateBlock(smiOop nofArgs) {
  PROLOGUE_1("allocateBlock", nofArgs);
  blockClosureOop blk =
    as_blockClosureOop(Universe::allocate(sizeof(blockClosureOopDesc)/oopSize));
  blk->init_mark();
  blk->set_klass_field(blockClosureKlass::blockKlassFor(nofArgs->value()));
  inc_block_counter();
  return blk;
}

/*
extern "C" blockClosureOop allocateBlock0() {
  PROLOGUE_0("allocateBlock0");
  blockClosureOop blk =
    as_blockClosureOop(Universe::allocate(sizeof(blockClosureOopDesc)/oopSize));
  blk->init_mark();
  blk->set_klass_field(Universe::zeroArgumentBlockKlassObj());
  inc_block_counter();
  return oop(blk);
}

extern "C" blockClosureOop allocateBlock1() {
  PROLOGUE_0("allocateBlock1");
  blockClosureOop blk =
    as_blockClosureOop(Universe::allocate(sizeof(blockClosureOopDesc)/oopSize));
  blk->init_mark();
  blk->set_klass_field(Universe::oneArgumentBlockKlassObj());
  inc_block_counter();
  return oop(blk);
}

extern "C" blockClosureOop allocateBlock2() {
  PROLOGUE_0("allocateBlock2");
  blockClosureOop blk =
    as_blockClosureOop(Universe::allocate(sizeof(blockClosureOopDesc)/oopSize));
  blk->init_mark();
  blk->set_klass_field(Universe::twoArgumentBlockKlassObj());
  inc_block_counter();
  return oop(blk);
}
*/

/*
extern "C" contextOop allocateContext(smiOop nofVars) {
  PROLOGUE_1("allocateContext", nofVars);
  contextKlass* ok = (contextKlass*) contextKlassObj->klass_part();
  inc_context_counter();
  return ok->allocateObjectSize(nofVars->value());
}

extern "C" contextOop allocateContext0() {
  PROLOGUE_0("allocateContext0");
  // allocate
  contextOop obj = as_contextOop(Universe::allocate(contextOopDesc::header_size()));
  // header
  obj->set_klass_field(contextKlassObj);
  //%clean this up later
  //  hash value must by convention be different from 0 (check markOop.hpp)
  obj->set_mark(markOopDesc::tagged_prototype()->set_hash(0 + 1));

  inc_context_counter();

  return obj;
}

extern "C" contextOop allocateContext1() {
  PROLOGUE_0("allocateContext1");
  contextKlass* ok = (contextKlass*) contextKlassObj->klass_part();
  inc_context_counter();
  return ok->allocateObjectSize(1);
}
extern "C" contextOop allocateContext2() {
  PROLOGUE_0("allocateContext2");
  contextKlass* ok = (contextKlass*) contextKlassObj->klass_part();
  inc_context_counter();
  return ok->allocateObjectSize(2);
} 

*/

extern "C" bool have_nlr_through_C;
extern "C" oop  nlr_result;

PRIM_DECL_2(unwindprotect, oop receiver, oop protectBlock) {
  PROLOGUE_2("unwindprotect", receiver, protectBlock);
  oop res = Delta::call(receiver, vmSymbols::value());

  if (have_nlr_through_C) {
    unwindInfo enabler;
    res  = Delta::call(protectBlock, vmSymbols::value(), nlr_result);
    // Now since we have to continue the first non-local-return
    // the nlr_result must be correct.
  }

  return res;
}

PRIM_DECL_1(blockRepeat, oop receiver) {
  PROLOGUE_1("blockRepeat", receiver);
  do Delta::call(receiver, vmSymbols::value());
  while (!have_nlr_through_C);
  return smiOop_zero; // The return value is ignored by the NLR
}

PRIM_DECL_1(block_method, oop receiver){
  PROLOGUE_1("block_method", receiver);
  return blockClosureOop(receiver)->method();
}

PRIM_DECL_1(block_is_optimized, oop receiver){
  PROLOGUE_1("blockRepeat", receiver);
  return blockClosureOop(receiver)->isCompiledBlock() ? trueObj : falseObj;
}
