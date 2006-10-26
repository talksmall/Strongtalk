/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.44 $ */
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
# include "incls/_stubRoutines.cpp.incl"

const int max_fast_allocate_size = 9;

// entry points
char* StubRoutines::_ic_normal_lookup_entry	= NULL;
char* StubRoutines::_ic_super_lookup_entry	= NULL;
char* StubRoutines::_zombie_nmethod_entry	= NULL;
char* StubRoutines::_zombie_block_nmethod_entry	= NULL;
char* StubRoutines::_megamorphic_ic_entry	= NULL;
char* StubRoutines::_compile_block_entry	= NULL;
char* StubRoutines::_continue_NLR_entry		= NULL;
char* StubRoutines::_call_sync_DLL_entry	= NULL;
char* StubRoutines::_call_async_DLL_entry	= NULL;
char* StubRoutines::_lookup_sync_DLL_entry	= NULL;
char* StubRoutines::_lookup_async_DLL_entry	= NULL;
char* StubRoutines::_recompile_stub_entry	= NULL;
char* StubRoutines::_used_uncommon_trap_entry	= NULL;
char* StubRoutines::_unused_uncommon_trap_entry	= NULL;
char* StubRoutines::_verify_context_chain_entry = NULL;
char* StubRoutines::_deoptimize_block_entry	= NULL;
char* StubRoutines::_call_inspector_entry	= NULL;

char* StubRoutines::_PIC_stub_entries[PIC::max_nof_entries + 1];	// entry 0 ignored
char* StubRoutines::_allocate_entries[max_fast_allocate_size + 1];


// tracing

void StubRoutines::trace_DLL_call_1(dll_func function, oop* last_argument, int nof_arguments) {
  if (!TraceDLLCalls) return; // in case it has been turned off during run-time
  frame f = DeltaProcess::active()->last_frame();
  if (f.is_interpreted_frame()) {
    // called from within interpreter -> InterpretedDLL_Cache available
    methodOop m = f.method();
    CodeIterator it(m, m->bci_from(f.hp()));
    InterpretedDLL_Cache* cache = it.dll_cache();
    assert(cache->entry_point() == function, "inconsistency with InterpretedDLL_Cache");
    assert(cache->number_of_arguments() == nof_arguments, "inconsistency with InterpretedDLL_Cache");
    cache->print();
  } else {
    // called from within compiled code -> CompiledDLL_Cache available
    CompiledDLL_Cache* cache = compiledDLL_Cache_from_return_address(f.pc());
    assert(cache->entry_point() == function, "inconsistency with CompiledDLL_Cache");
    cache->print();
  }
  // print arguments
  int i = 1;
  oop* arg_ptr = last_argument + (nof_arguments - 1);
  for (i = 1; i <= nof_arguments; i++, arg_ptr--) {
    oop arg = *arg_ptr;
    std->print("%6d. ", i);
    if (arg->is_smi()) {
      std->print("smi   value = %d\n", ((smiOop)arg)->value());
    } else {
      std->print("proxy value = 0x%08x (proxy = 0x%08x)\n", ((proxyOop)arg)->get_pointer(), arg);
    }
  }
}


void StubRoutines::trace_DLL_call_2(int result) {
  if (!TraceDLLCalls) return; // in case it has been turned off during run-time
  std->print("    result = 0x%08x\n", result);
}


void StubRoutines::wrong_DLL_call() {
  { ResourceMark rm;
    std->print("DLL call error: number of arguments probably wrong\n");
  }
  if (DeltaProcess::active()->is_scheduler()) {
    DeltaProcess::active()->trace_stack();
    fatal("DLL error in scheduler");
  } else {
    DeltaProcess::active()->suspend(::DLL_lookup_error);
  }
  ShouldNotReachHere();
}


// generators

char* StubRoutines::generate_ic_lookup(MacroAssembler* masm, char* lookup_routine_entry) {
// Stub routine that calls icLookup which patches the icache.
// After returning from icLookup the send is continued. The
// icLookupStub is called from empty iCaches within compiled
// code.
//
// Note that so far, the lookup routine doesn't do allocation,
// therefore the receiver can be saved/restored on the C stack.
// (This has to be fixed as soon as 'message not understood' is
// handled correctly from within compiled code.
  
  // eax: receiver
  // tos: return address
  char* entry_point = masm->pc();
  masm->set_last_Delta_frame_after_call();
  masm->movl(ebx, Address(esp));		// get return address (= ic address)
  masm->pushl(eax);				// save receiver
  masm->pushl(ebx);				// pass ic
  masm->pushl(eax);				// pass receiver
  masm->call(lookup_routine_entry, relocInfo::runtime_call_type);	// eax = lookup_routine(receiver, ic)
  masm->movl(ebx, eax);				// ebx = method code
  masm->popl(eax);				// get rid of receiver argument
  masm->popl(eax);				// get rid of ic argument
  masm->popl(eax);				// restore receiver (don't use argument, might be overwritten)
  masm->reset_last_Delta_frame();
  masm->jmp(ebx);				// jump to target
  return entry_point;
}


extern "C" char* icNormalLookup(oop recv, CompiledIC* ic);
extern "C" char* icSuperLookup (oop recv, CompiledIC* ic);
extern "C" char* zombie_nmethod(CompiledIC* ic);

char* StubRoutines::generate_ic_normal_lookup(MacroAssembler* masm) {
  return generate_ic_lookup(masm, (char*)icNormalLookup);
}


char* StubRoutines::generate_ic_super_lookup(MacroAssembler* masm) {
  return generate_ic_lookup(masm, (char*)icSuperLookup);
}


char* StubRoutines::generate_zombie_nmethod(MacroAssembler* masm) {
// Called from zombie nmethods immediately after they are called.
// Does cleanup of interpreted/compiled ic and redoes the send.
//
// Note: zombie_nmethod() doesn't do scavenge (otherwise this code
//       has to change - the receiver cannot be saved on the stack).

  // eax  : receiver
  // tos  : return address to zombie nmethod (which called this stub)
  // tos-4: return address to caller nmethod (which called the zombie nmethod)
  char* entry_point = masm->pc();
  masm->popl(ebx);				// get rid of return address to zombie nmethod
  // eax: receiver
  // tos: return address to caller nmethod (which called the zombie nmethod)
  masm->set_last_Delta_frame_after_call();
  masm->movl(ebx, Address(esp));		// get return address (= ic address) - don't pop! (needed for correct Delta frame)
  masm->pushl(eax);				// save receiver
  masm->pushl(ebx);				// pass ic
  masm->call((char*)zombie_nmethod, relocInfo::runtime_call_type);	// eax = zombie_nmethod(return_address)
  masm->movl(ecx, eax);				// ecx = entry point to redo send
  masm->popl(ebx);				// get rid of ic argument
  masm->popl(eax);				// restore receiver
  masm->popl(ebx);				// get rid of return address
  masm->reset_last_Delta_frame();
  masm->jmp(ecx);				// redo send
  return entry_point;
}

// The code below (validateContextChain(...) and generate_verify_context_chain(...)
// can be removed if it turns out that the new solution (using deoptimize_block()
// and generate_deoptimize_block(...)) is working correctly. See also call site
// (code for PrologueNode). - gri 6/25/96

static bool validateContextChain(blockClosureOop block) {
  assert(block->is_block(), "must be block");
  bool is_valid = true;

  { contextOop con = block->lexical_scope();
    // verify entire context chain
    assert(con->is_context(), "expecting a context");

    while (true) {
      if (con->unoptimized_context() != NULL) {
        is_valid = false;
        break;
      }
      if (!con->has_outer_context()) break; 
      con = con->outer_context();
    }
  }

  if (is_valid) return true;

  assert(block->isCompiledBlock(), "we should be in a compiled block");

  // Patch the blockClosure
  methodOop method = block->method();
  nmethod*  nm     = block->jump_table_entry()->block_nmethod();

  LOG_EVENT1("Deoptimized context in blockClosure -> switch to methodOop 0x%lx", nm);
  {
    block->set_method(method);
    contextOop con    = block->lexical_scope();
    contextOop un_con = con->unoptimized_context();
    if (un_con) {
      block->set_lexical_scope(un_con);
    } else {
      Unimplemented();
    }
  }
  return false;
}

static void deoptimize_context_and_patch_block(blockClosureOop block) {
  assert(block->is_block(), "must be block");
  assert(block->isCompiledBlock(), "we should be in a compiled block");

  // Patch the blockClosure
  methodOop method = block->method();
  nmethod*  nm     = block->jump_table_entry()->block_nmethod();

  LOG_EVENT1("Deoptimized context in blockClosure -> switch to methodOop 0x%lx", nm);

  contextOop con = block->lexical_scope();
  block->set_method(method);  
  if (method->expectsContext()) {
    guarantee(con && con->is_context(), "Optimized context must be present");

    // we have (nm, con) the deoptimize the context
    contextOop unoptimized_con = con->unoptimized_context();
    guarantee(unoptimized_con && unoptimized_con->is_context(), "Unoptimized context must be present");
    block->set_lexical_scope(unoptimized_con);
  } else {
    guarantee(!con->is_context(), "Cannot be a context");
  }
}

extern "C" void restart_primitiveValue();

char* StubRoutines::generate_zombie_block_nmethod(MacroAssembler* masm) {
// Called from zombie nmethods immediately after they are called.
// Does cleanup of interpreted/compiled ic and redoes the send.
//
// Note: zombie_nmethod() doesn't do scavenge (otherwise this code
//       has to change - the receiver cannot be saved on the stack).

  // eax  : receiver
  // tos  : return address to zombie nmethod (which called this stub)
  // tos-4: return address to caller nmethod (which called the zombie nmethod)
  char* entry_point = masm->pc();

  masm->set_last_Delta_frame_after_call();
  masm->pushl(self_reg); // pass argument (C calling convention)
  masm->call((char*)deoptimize_context_and_patch_block, relocInfo::runtime_call_type);
  masm->movl(ebx, eax);
  masm->popl(self_reg);
  masm->reset_last_Delta_frame();
  masm->addl(esp, 4);
  masm->jmp((char*)restart_primitiveValue, relocInfo::runtime_call_type);
  return entry_point;
}

extern "C" char* method_entry_point;

char* StubRoutines::generate_megamorphic_ic(MacroAssembler* masm) {
// Called from within a MIC (megamorphic inline cache), the special
// variant of PICs for compiled code (see compiledPIC.hpp/cpp).
// The MIC layout is as follows:
//
// call <this stub routine>
// selector			<--- return address (tos)
//
// Note: Don't use this for megamorphic super sends!

  Label is_smi, probe_primary_cache, probe_secondary_cache, call_method, is_methodOop, do_lookup;

  masm->bind(is_smi);				// smi case (assumed to be infrequent)
  masm->movl(ecx, Address((int)&smiKlassObj, relocInfo::external_word_type));
  masm->jmp(probe_primary_cache);

  // eax    : receiver
  // tos    : return address pointing to selector in MIC
  // tos + 4: return address of megamorphic send in compiled code
  // tos + 8: last argument/receiver
  char* entry_point = masm->pc();
  masm->popl(ebx);				// get return address (MIC cache)
  masm->test(eax, Mem_Tag);			// check if smi
  masm->jcc(Assembler::zero, is_smi);		// if so, get smi class directly
  masm->movl(ecx, Address(eax, memOopDesc::klass_byte_offset()));	// otherwise, load receiver class

  // probe primary cache
  //
  // eax: receiver
  // ebx: MIC cache pointer
  // ecx: receiver klass
  // tos: return address of megamorphic send in compiled code (ic)
  masm->bind(probe_primary_cache);		// compute hash value
  masm->movl(edx, Address(ebx));		// get selector
  // compute hash value
  masm->movl(edi, ecx);
  masm->xorl(edi, edx);
  masm->andl(edi, (primary_cache_size - 1) << 4);
  // probe cache
  masm->cmpl(ecx, Address(edi, lookupCache::primary_cache_address() + 0*oopSize));
  masm->jcc(Assembler::notEqual, probe_secondary_cache);
  masm->cmpl(edx, Address(edi, lookupCache::primary_cache_address() + 1*oopSize));
  masm->jcc(Assembler::notEqual, probe_secondary_cache);
  masm->movl(ecx, Address(edi, lookupCache::primary_cache_address() + 2*oopSize));

  // call method
  //
  // eax: receiver
  // ecx: methodOop/nmethod
  // tos: return address of megamorphic send in compiled code (ic)
  masm->bind(call_method);
  masm->test(ecx, Mem_Tag);			// check if methodOop
  masm->jcc(Assembler::notZero, is_methodOop);	// otherwise
  masm->jmp(ecx);				// call nmethod

  // call methodOop - setup registers
  masm->bind(is_methodOop);
  masm->xorl(ebx, ebx);				// clear ebx for interpreter
  masm->movl(edx, Address(int(&method_entry_point), relocInfo::external_word_type));
  // (Note: cannot use value in method_entry_point directly since interpreter is generated afterwards)
  //
  // eax: receiver
  // ebx: 00000000
  // ecx: methodOop
  // edx: entry point
  // tos: return address of megamorphic send in compiled code (ic)
  masm->jmp(edx);				// call method_entry

  // probe secondary cache
  //
  // eax: receiver
  // ebx: MIC cache pointer
  // ecx: receiver klass
  // edx: selector
  // edi: primary cache index
  // tos: return address of megamorphic send in compiled code (ic)
  masm->bind(probe_secondary_cache);		// compute hash value
  masm->andl(edi, (secondary_cache_size - 1) << 4);
  // probe cache
  masm->cmpl(ecx, Address(edi, lookupCache::secondary_cache_address() + 0*oopSize));
  masm->jcc(Assembler::notEqual, do_lookup);
  masm->cmpl(edx, Address(edi, lookupCache::secondary_cache_address() + 1*oopSize));
  masm->jcc(Assembler::notEqual, do_lookup);
  masm->movl(ecx, Address(edi, lookupCache::secondary_cache_address() + 2*oopSize));
  masm->jmp(call_method);
  
  // do lookup
  //
  // eax: receiver
  // ebx: MIC cache pointer
  // ecx: receiver klass
  // edx: selector
  // edi: secondary cache index
  // tos: return address of megamorphic send in compiled code (ic)
  masm->bind(do_lookup);
  masm->set_last_Delta_frame_after_call();
  masm->pushl(eax);				// save receiver
  masm->pushl(edx);				// pass 2nd argument: selector
  masm->pushl(ecx);				// pass 1st argument: receiver klass
  masm->call((char*)lookupCache::normal_lookup, relocInfo::runtime_call_type);
  masm->movl(ecx, eax);				// ecx: method
  masm->popl(ebx);				// pop 1st argument
  masm->popl(ebx);				// pop 2nd argument
  masm->popl(eax);				// restore receiver
  masm->reset_last_Delta_frame();
  masm->testl(ecx, ecx);			// test if method has been found in lookup cache
  masm->jcc(Assembler::notZero, call_method);

  // method not found in the lookup cache - full lookup needed (message not understood may happen)
  // eax: receiver
  // ebx: points to MIC cache
  // tos: return address of megamorphic send in compiled code
  //
  // Note: This should not happen right now, since normal_lookup always returns a value
  //       if the method exists (and 'message not understood' is not yet supported in
  //       compiled code). However, this should change at some point, and normal_lookup_cache_probe
  //       should be used instead of normal_lookup.
  masm->hlt();

  return entry_point;
}


char* StubRoutines::generate_compile_block(MacroAssembler* masm) {
// Stub routine that is called from a jumptable entry for a block closure.
// The recipe:
//   - compile the toplevelblock nmethod
//   - patch the jump entry with the entry point of the compiler nmethod
//   - jump to the new nmethod.
//
// Note that compile_new_block doesn't do allocation,
// therefore the receiver can be saved on the C stack.

  // eax: receiver
  char* entry_point = masm->pc();
  masm->set_last_Delta_frame_after_call();
  masm->pushl(eax);				// save receiver
  masm->pushl(eax);				// pass receiver
  masm->call((char*)jumpTable::compile_new_block, relocInfo::runtime_call_type);	// eax = block_closure_compile(receiver)
  masm->movl(ebx, eax);				// ebx = block code
  masm->popl(eax);				// get rid of receiver argument
  masm->popl(eax);				// restore receiver (don't use argument, might be overwritten)
  masm->reset_last_Delta_frame();
  masm->jmp(ebx);				// jump to target
  return entry_point;
}


char* StubRoutines::generate_continue_NLR(MacroAssembler* masm) {
// Entry point jumped to from compiled code. Initiates (or continues an ongoing) NLR.
// Originally this code has been generated in nmethods, using a stub reduces code size.

  Register ret_addr = temp1;
  Register offset   = temp2;

  char* entry_point = masm->pc();
  masm->leave();						// remove stack frame
  masm->popl(ret_addr);						// get (local) return address
  masm->movl(offset, Address(ret_addr, IC_Info::info_offset));	// get ic info
  if (IC_Info::number_of_flags > 0) {
    masm->sarl(offset, IC_Info::number_of_flags);		// shift ic info flags out
  }
  masm->addl(ret_addr, offset);					// compute non-local return address
  masm->jmp(ret_addr);						// do NLR
  return entry_point;
}


char* StubRoutines::generate_call_DLL(MacroAssembler* masm, bool async) {
// The following routine provides the extra frame for DLL calls.
// Note: 1. Its code has to be *outside* the interpreters code! (see also: DLL calls in interpreter)
//       2. This routine is also used by the compiler! Make sure to adjust the parameter
//          passing in the compiler as well (x86_node.cpp, codeGenerator.cpp), when changing this code!
//
// Stack layout immediately after calling the DLL:
// (The DLL state word is used for asynchronous DLL/interrupted DLL calls)
//
//	  ...					DLL land
// esp->[ return addr	] ------------------------------------------------------------------------
//	[ unboxed arg 1	]			C land
//	  ...
// 	[ unboxed arg n	]
// 	[ ptr to itself	] <----	ptr to itself	to check that the right no. of arguments is used
// 	[ DLL state	]			used for DLL/interrupted DLL calls
// 	[ return addr	] ------------------------------------------------------------------------
// 	[ argument n	] <----	last_Delta_sp	Delta land
// 	[ argument n-1	]
// 	  ...
// 	[ argument 1	]
//	[ return proxy	]
//	  ...
// ebp->[ previous ebp	] <----	last_Delta_fp
//
// The routine expects 3 arguments to be passed in registers as follows:
//
// ebx: number of arguments
// ecx: address of last argument
// edx: DLL function entry point

  Label loop_entry, no_arguments, smi_argument, next_argument, wrong_call;

  char* entry_point = masm->pc();
  masm->set_last_Delta_frame_after_call();
  masm->pushl(0);				// initial value for DLL state
  masm->movl(esi, esp);				// save DLL state address
  masm->pushl(esp);				// to check that the right no. of arguments is used
  if (TraceDLLCalls) {				// call trace routine (C to C call, no special setup required)
    masm->pushl(esi);				// save DLL state address
    masm->pushl(ebx);				// pass arguments in reverse order
    masm->pushl(ecx);
    masm->pushl(edx);
    masm->call((char*)trace_DLL_call_1, relocInfo::runtime_call_type);
    masm->popl(edx);				// restore registers
    masm->popl(ecx);
    masm->popl(ebx);
    masm->popl(esi);				// restore DLL state address
  }
  masm->testl(ebx, ebx);			// if number of arguments != 0 then
  masm->jcc(MacroAssembler::notZero, loop_entry);// convert arguments

  // done with all the arguments
  masm->bind(no_arguments);
  if (async) {
    masm->pushl(edx);
    masm->pushl(esi);				// pass DLL state address
    masm->call((char*)DLLs::enter_async_call, relocInfo::runtime_call_type);
    masm->popl(esi);				// discard argument
    masm->popl(edx);				// restore registers
  }

  // do DLL call
  masm->call(edx);				// eax := dll call (pops arguments itself)

  // check no. of arguments
  masm->popl(ebx);				// must be the same as esp after popping
  masm->cmpl(ebx, esp);
  masm->jcc(Assembler::notEqual, wrong_call);
  
  // top of stack contains DLL state
  masm->movl(ebx, esp);				// get DLL state address
  masm->pushl(eax);				// save result
  masm->pushl(ebx);				// pass DLL state address
  char* exit_dll = (char*)(async ? DLLs::exit_async_call : DLLs::exit_sync_call);
  masm->call(exit_dll, relocInfo::runtime_call_type);
  masm->popl(ebx);				// discard argument
  masm->popl(eax);				// restore result

  if (TraceDLLCalls) {				// call trace routine (C to C call, no special setup required)
    masm->pushl(eax);				// pass result
    masm->call((char*)trace_DLL_call_2, relocInfo::runtime_call_type);
    masm->popl(eax);				// restore result
  }
  masm->popl(ebx);				// discard DLL state word
  masm->reset_last_Delta_frame();
  masm->ret(0);

  // wrong DLL has been called (no. of popped arguments is incorrect)
  masm->bind(wrong_call);
  masm->call((char*)wrong_DLL_call, relocInfo::runtime_call_type);
  masm->hlt();					// should never reach here

  // smi argument -> convert it to int
  masm->bind(smi_argument);
  masm->sarl(eax, Tag_Size);			// convert smi into C int

  // next argument
  masm->bind(next_argument);
  masm->pushl(eax);				// push converted argument
  masm->addl(ecx, oopSize);			// go to previous argument
  masm->decl(ebx);				// decrement argument counter
  masm->jcc(MacroAssembler::zero, no_arguments);// continue until no arguments

  // loop
  masm->bind(loop_entry);
  // ebx: argument count
  // ecx: current argument address
  masm->movl(eax, Address(ecx));		// get argument
  masm->testb(eax, Mem_Tag);			// check if smi or proxy
  masm->jcc(MacroAssembler::zero, smi_argument);

  // boxed argument -> unbox it
  masm->movl(eax, Address(eax, pointer_offset));// unbox proxy
  masm->jmp(next_argument);

  return entry_point;
}


char* StubRoutines::generate_lookup_DLL(MacroAssembler* masm, bool async) {
// Lookup routine called from "empty" DLL caches in compiled code only.
// Calls a lookup & patch routine which updates the DLL cache and then
// continues with call_DLL.

  // ebx: number of arguments
  // ecx: address of last argument
  // edx: some initial value for DLL function entry point
  assert(call_DLL_entry(async) != NULL, "call_DLL_entry must have been generated before");
  char* entry_point = masm->pc();
  masm->set_last_Delta_frame_after_call();
  masm->pushl(ebx);				// save registers (edx has no valid value yet)
  masm->pushl(ecx);
  masm->call((char*)DLLs::lookup_and_patch_CompiledDLL_Cache, relocInfo::runtime_call_type);	// eax := function entry point
  masm->popl(ecx);				// restore registers
  masm->popl(ebx);
  masm->movl(edx, eax);				// setup edx
  masm->reset_last_Delta_frame();
  masm->jmp(call_DLL_entry(async), relocInfo::runtime_call_type);	// now jump to real target
  return entry_point;
}


char* StubRoutines::generate_recompile_stub(MacroAssembler* masm) {
// Recompilation; called by nmethod prologue of recompilation
// trigger. Sets up stack frame and passes on receiver + caller
// to C. Using the stub reduces the code size of nmethod prologues.

  // eax: receiver
  char* entry_point = masm->pc();
  //masm->int3();
  masm->set_last_Delta_frame_after_call();
  masm->call((char*)SavedRegisters::save_registers, relocInfo::runtime_call_type);
  masm->movl(ebx, Address(esp));		// get return address (trigger nmethod)
  masm->pushl(eax);				// save receiver
  masm->pushl(ebx);				// pass 2nd argument (pc)
  masm->pushl(eax);				// pass 1st argument (recv)
  masm->call((char*)Recompilation::nmethod_invocation_counter_overflow, relocInfo::runtime_call_type);	// eax = nmethod_invocation_counter_overflow(receiver, pc)
  masm->movl(ecx, eax);				// save continuation address in ecx
  masm->popl(eax);				// pop 1st argument
  masm->popl(ebx);				// pop 2nd argument
  masm->popl(eax);				// restore receiver
  masm->reset_last_Delta_frame();
  masm->leave();				// remove trigger nmethod's stack frame
  masm->jmp(ecx);				// continue
  return entry_point;
}


/* Old code - keep around for a while - gri 9/18/96

extern "C" char* recompileNMethod(oop rcvr, char* retpc);

char* StubRoutines::generate_recompile_stub(MacroAssembler* masm) {
// Recompilation; called by nmethod prologue of recompilation
// trigger. Sets up stack frame and passes on receiver + caller
// to C. Using the stub reduces the code size of nmethod prologues.

  char* entry_point = masm->pc();
  masm->movl(ebx, Address(esp));		// get return address (trigger nmethod)
  masm->enter();				// create normal stack frame with link (for stub)
  masm->pushl(eax);				// save receiver
  masm->call_C((char*)recompileNMethod, eax, ebx);	// eax = recompileNMethod(receiver, pc)
  masm->movl(ecx, eax);				// save continuation address in ecx
  masm->popl(eax);				// restore receiver
  masm->leave();				// remove this stack frame
  masm->leave();				// remove trigger's stack frame
  masm->jmp(ecx);				// continue
  return entry_point;
}
*/


char* StubRoutines::generate_uncommon_trap(MacroAssembler* masm) {
  char* entry_point = masm->pc();
  masm->set_last_Delta_frame_after_call();
  masm->call((char*)SavedRegisters::save_registers, relocInfo::runtime_call_type);
  masm->call((char*)uncommon_trap, relocInfo::runtime_call_type);
  masm->reset_last_Delta_frame();
  masm->ret(0);
  return entry_point;
}



char* StubRoutines::generate_verify_context_chain(MacroAssembler* masm) {
// Verify the context chain for a block nmethod, if there is
// an unoptimized context in the chain the block must be deoptimized
// and reevaluated.
//
// Called as first instruction for a block nmethod if 
// the method is expecting a contextOop
//
// Stack at entry:
//   [nmethod caller return address]  @ esp - 4
//   [callee nmethod return address]  @ esp
//
// Registers at entry:
//   ebp points to nmethod callers frame
//   eax contains the block closure.

  Label deoptimize;

  char* entry_point = masm->pc();
  masm->set_last_Delta_frame_after_call();
  masm->pushl(self_reg); // pass argument (C calling convention)
  masm->call((char*)validateContextChain, relocInfo::runtime_call_type);
  masm->movl(ebx, eax);
  masm->popl(self_reg);
  masm->reset_last_Delta_frame();
  masm->testl(ebx,ebx);
  masm->jcc(MacroAssembler::zero, deoptimize);
  masm->ret(0);

  masm->bind(deoptimize);
  masm->addl(esp, 4);
  masm->jmp((char*)restart_primitiveValue, relocInfo::runtime_call_type);
  return entry_point;
}


static blockClosureOop deoptimize_block(blockClosureOop block) {
  VerifyNoAllocation vna;
  assert(block->is_block(),        "must be a block");
  assert(block->isCompiledBlock(), "we should be in a compiled block");

  #ifdef ASSERT
    // just checking if the context chain really contains an unoptimized context
    bool has_unoptimized_context = false;
    { contextOop con = block->lexical_scope();
      // verify entire context chain
      assert(con->is_context(), "expecting a context");
      while (true) {
        if (con->unoptimized_context() != NULL) {
          has_unoptimized_context = true;
          break;
        }
        if (!con->has_outer_context()) break; 
        con = con->outer_context();
      }
    }
    assert(has_unoptimized_context, "should have an unoptimized context");
  #endif

  block->deoptimize();
  return block;
}


char* StubRoutines::generate_deoptimize_block(MacroAssembler* masm) {
// Called if there's an unoptimized context in the incoming context
// chain of a block nmethod. The block must be deoptimized and the
// value send has to be restarted.
//
// Called from block nmethods with incoming contexts, immediately
// after checking the context chain and before any stack frame
// has been set up.

  // eax: block closure
  // tos: callee nmethod return address (returning to caller)
  char* entry_point = masm->pc();
  masm->set_last_Delta_frame_after_call();	// nmethod is treated as C routine
  masm->pushl(self_reg);			// pass argument
  masm->call((char*)deoptimize_block, relocInfo::runtime_call_type);	// eax := deoptimize_block(self_reg)
  masm->popl(ebx);				// get rid of argument
  masm->reset_last_Delta_frame();		// return & restart the primitive (eax must contain the block)
  masm->jmp((char*)restart_primitiveValue, relocInfo::runtime_call_type);
  return entry_point;
}


char* StubRoutines::generate_call_inspector(MacroAssembler* masm) {
// Called for each MacroAssembler::inspect(...) - used for debugging only
  char* entry_point = masm->pc();
  masm->set_last_Delta_frame_after_call();	// just in case somebody wants to look at the stack
  masm->pushad();
  masm->call((char*)MacroAssembler::inspector, relocInfo::runtime_call_type);
  masm->popad();
  masm->reset_last_Delta_frame();
  masm->ret(0);
  return entry_point;
}


char* StubRoutines::generate_PIC_stub(MacroAssembler* masm, int pic_size) {
// Called from within a PIC (polymorphic inline cache).
// The stub interprets the methodOop section of compiled PICs.
// The methodOop section layout is as follows:
//
// call <this stub routine>
// cached klass 1	<--- return address (tos)
// cached methodOop 1
// cached klass 2
// cached methodOop2
// ...
//
// cached klass n
// cached methodOop n
//
// Note: Don't use this for polymorphic super sends!

  Label found, loop;

  // entry found at index
  //
  // eax: receiver
  // ebx: PIC table pointer
  // ecx: methodOop
  // edx: receiver klass
  // tos: return address of polymorphic send in compiled code
  masm->bind(found);
  masm->movl(edx, Address(int(&method_entry_point), relocInfo::external_word_type));
  // (Note: cannot use value in method_entry_point directly since interpreter is generated afterwards)
  masm->xorl(ebx, ebx);
  // eax: receiver
  // ebx: 000000xx
  // ecx: methodOop
  masm->jmp(edx);

  // eax    : receiver
  // tos    : return address pointing to table in PIC
  // tos + 4: return address of polymorphic send in compiled code
  // tos + 8: last argument/receiver
  char* entry_point = masm->pc();
  masm->popl(ebx);				// get return address (PIC table pointer)
  masm->movl(edx, Address((int)&smiKlassObj, relocInfo::external_word_type));
  masm->test(eax, Mem_Tag);			// check if smi
  masm->jcc(Assembler::zero, loop);		// if so, class is already in ecx
  masm->movl(edx, Address(eax, memOopDesc::klass_byte_offset()));	// otherwise, load receiver class

  // eax: receiver
  // ebx: PIC table pointer
  // edx: receiver klass
  // tos: return address of polymorphic send in compiled code
  masm->bind(loop);
  for (int i = 0; i < pic_size; i++) {
    // compare receiver klass with klass in PIC table at index
    masm->cmpl(edx, Address(ebx, i * PIC::PIC_methodOop_entry_size + PIC::PIC_methodOop_klass_offset));
    masm->movl(ecx, Address(ebx, i * PIC::PIC_methodOop_entry_size + PIC::PIC_methodOop_offset));
    masm->jcc(Assembler::equal, found);
  }
  assert(ic_normal_lookup_entry() != NULL, "ic_normal_lookup_entry must be generated before");
  masm->jmp(ic_normal_lookup_entry(), relocInfo::runtime_call_type);

  return entry_point;
}


char* StubRoutines::generate_allocate(MacroAssembler* masm, int size) {
  char* entry_point = masm->pc();
  masm->hlt();
  return entry_point;
}


// Parametrized accessors

char* StubRoutines::PIC_stub_entry(int pic_size) {
  assert(_is_initialized, "StubRoutines not initialized yet");
  assert(1 <= pic_size && pic_size <= PIC::max_nof_entries, "pic size out of range")
  return _PIC_stub_entries[pic_size];
}


char* StubRoutines::allocate_entry(int size) {
  assert(_is_initialized, "StubRoutines not initialized yet");
  assert(0 <= size && size <= max_fast_allocate_size, "size out of range")
  return _allocate_entries[size];
}


// Initialization

bool StubRoutines::_is_initialized = false;
char StubRoutines::_code[StubRoutines::_code_size];

char* StubRoutines::generate(MacroAssembler* masm, char* title, char* gen(MacroAssembler*)) {
  char* old_pc = masm->pc();
  char* entry_point = gen(masm);
  char* new_pc = masm->pc();
  if (!Disclaimer::is_product() && PrintStubRoutines) {
    std->print("Stub routine: %s", title);
    std->print(" (%d bytes), entry point = 0x%x\n", new_pc - old_pc, entry_point);
    masm->code()->decode();
    std->cr();
  }
  return entry_point;
}


char* StubRoutines::generate(MacroAssembler* masm, char* title, char* gen(MacroAssembler*, int argument), int argument) {
  char* old_pc = masm->pc();
  char* entry_point = gen(masm, argument);
  char* new_pc = masm->pc();
  if (!Disclaimer::is_product() && PrintStubRoutines) {
    std->print("Stub routine: %s %d", title, argument);
    std->print(" (%d bytes), entry point = 0x%x\n", new_pc - old_pc, entry_point);
    masm->code()->decode();
    std->cr();
  }
  return entry_point;
}


void StubRoutines::init() {
  if (_is_initialized) return;

  ResourceMark rm;
  CodeBuffer* code = new CodeBuffer(_code, _code_size);
  MacroAssembler* masm = new MacroAssembler(code);

  // add generators here
  _ic_normal_lookup_entry	= generate(masm, "ic_normal_lookup",	 generate_ic_normal_lookup	);
  _ic_super_lookup_entry	= generate(masm, "ic_super_lookup",	 generate_ic_super_lookup	);
  _zombie_nmethod_entry		= generate(masm, "zombie_nmethod",	 generate_zombie_nmethod	);
  _zombie_block_nmethod_entry	= generate(masm, "zombie_block_nmethod", generate_zombie_block_nmethod	);
  _megamorphic_ic_entry		= generate(masm, "megamorphic_ic",	 generate_megamorphic_ic	);
  _compile_block_entry		= generate(masm, "compile_block", 	 generate_compile_block		);
  _continue_NLR_entry		= generate(masm, "continue_NLR",	 generate_continue_NLR		);
  _call_sync_DLL_entry		= generate(masm, "call_sync_DLL",	 generate_call_sync_DLL		);
  _call_async_DLL_entry		= generate(masm, "call_async_DLL",	 generate_call_async_DLL	);
  _lookup_sync_DLL_entry	= generate(masm, "lookup_sync_DLL",	 generate_lookup_sync_DLL	);
  _lookup_async_DLL_entry	= generate(masm, "lookup_async_DLL",	 generate_lookup_async_DLL	);
  _recompile_stub_entry		= generate(masm, "recompile_stub",	 generate_recompile_stub	);
  _used_uncommon_trap_entry	= generate(masm, "used_uncommon_trap",	 generate_uncommon_trap		);
  _unused_uncommon_trap_entry	= generate(masm, "unused_uncommon_trap", generate_uncommon_trap		);
  _verify_context_chain_entry	= generate(masm, "verify_context_chain", generate_verify_context_chain	);
  _deoptimize_block_entry	= generate(masm, "deoptimize_block",	 generate_deoptimize_block	);
  _call_inspector_entry		= generate(masm, "call_inspector",	 generate_call_inspector	);

  for (int pic_size = 1; pic_size <= PIC::max_nof_entries; pic_size++) {
    _PIC_stub_entries[pic_size] = generate(masm, "PIC stub", generate_PIC_stub, pic_size);
  }

  for (int size = 0; size <= max_fast_allocate_size; size++) {
    _allocate_entries[size] = generate(masm, "allocate", generate_allocate, size);
  }

  masm->finalize();
  _is_initialized = true;
  if (!Disclaimer::is_product() && PrintStubRoutines) {
    std->print("%d bytes generated for stub routines\n", masm->offset());
    exit(0);
  }
};


void stubRoutines_init() {
  StubRoutines::init();
}