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
const int max_fast_alien_call_size = 7;

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
char* StubRoutines::_call_delta			= NULL;
char* StubRoutines::_return_from_Delta		= NULL;
char* StubRoutines::_single_step_stub		= NULL;
char* StubRoutines::_single_step_continuation	= NULL;
char* StubRoutines::_unpack_unoptimized_frames	= NULL;
char* StubRoutines::_provoke_nlr_at		= NULL;
char* StubRoutines::_continue_nlr_in_delta	= NULL;
char* StubRoutines::_handle_pascal_callback_stub= NULL;
char* StubRoutines::_handle_C_callback_stub	= NULL;
char* StubRoutines::_oopify_float		= NULL;
char* StubRoutines::_alien_call_with_args_entry = NULL;

char* StubRoutines::_PIC_stub_entries[PIC::max_nof_entries + 1];	// entry 0 ignored
char* StubRoutines::_allocate_entries[max_fast_allocate_size + 1];
char* StubRoutines::_alien_call_entries[max_fast_alien_call_size + 1];

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

  { 
    contextOop con = block->lexical_scope();
    contextOop prev_con = NULL;
    // verify entire context chain
    assert(con->is_context(), "expecting a context");

    //while (true) {
    //  if (con->unoptimized_context() != NULL) {
    //    is_valid = false;
    //    break;
    //  }
    //  if (!con->has_outer_context()) break; 
    //  con = con->outer_context();
    //}

    // slr fixup unoptimized context refs as we go
    while (true) {
      if (con->unoptimized_context() != NULL) {
        is_valid = false;
        if (prev_con) {
          prev_con->set_parent(con->parent());
        } else {
          prev_con = con;
        }
        //break;
      } else {
        prev_con = con;
      }
      if (!con->has_outer_context()) break; 
      con = con->outer_context();
    }
    // slr fixup unoptimized context refs as we go
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

//extern "C" void restart_primitiveValue();
//extern "C" char* restart_primitiveValue;

char* StubRoutines::generate_zombie_block_nmethod(MacroAssembler* masm) {
//  assert(restart_primitiveValue, "restart_primitiveValue must have been generated before");
  // %hack indirect load

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
  masm->movl(edx, Address((int)Interpreter::restart_primitiveValue(), relocInfo::external_word_type));
  masm->jmp(edx);
//  masm->jmp(restart_primitiveValue, relocInfo::runtime_call_type);
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

  Label loop_entry, no_arguments, smi_argument, next_argument, wrong_call,
    align_stack, convert_args;

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
  //slr mod: push a fake stack frame to support cdecl calls
  masm->enter();
  //slr mod end
  // following is to allow 16-byte stack alignment for Darwin (OSX)
  masm->movl(eax, ebx);
  masm->negl(eax);
  masm->leal(eax, Address(esp, eax, Address::times_4)); // esp - 4 x nargs
  masm->andl(eax, 0xf); // padding required for 16-byte alignment
  masm->bind(align_stack);
  masm->cmpl(eax, 0);
  masm->jcc(MacroAssembler::lessEqual, convert_args);
  masm->pushl(0);
  masm->subl(eax, 4); // align stack
  masm->jmp(align_stack);
  masm->bind(convert_args);
  // end stack alignment mod

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

  //slr mod: pop the fake stack frame for cdecl
  masm->leave();
  //slr mod end
  
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
//  masm->call((char*)SavedRegisters::save_registers, relocInfo::runtime_call_type);
  SavedRegisters::generate_save_registers(masm);
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
//  masm->call((char*)SavedRegisters::save_registers, relocInfo::runtime_call_type);
  SavedRegisters::generate_save_registers(masm);
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
  masm->pushl(self_reg); // save self (argument can get corrupted in called function)
  masm->pushl(self_reg); // pass argument (C calling convention)
  masm->call((char*)validateContextChain, relocInfo::runtime_call_type);
  masm->movl(ebx, eax);
  masm->popl(self_reg);
  masm->popl(self_reg);
  masm->reset_last_Delta_frame();
  masm->testl(ebx,ebx);
  masm->jcc(MacroAssembler::zero, deoptimize);
  masm->ret(0);

  masm->bind(deoptimize);
  masm->addl(esp, 4);
  masm->jmp((char*)Interpreter::restart_primitiveValue(), relocInfo::runtime_call_type);
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
  masm->jmp((char*)Interpreter::restart_primitiveValue(), relocInfo::runtime_call_type);
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

//-----------------------------------------------------------------------------------------
// extern "C" oop call_delta(void* method, oop receiver, int nofArgs, oop* args)

extern "C" char*    method_entry_point;

extern "C" bool	    have_nlr_through_C;
extern "C" oop	    nlr_result;
extern "C" int	    nlr_home;
extern "C" int	    nlr_home_id;

extern "C" char*    C_frame_return_addr;

extern "C" int*	    last_Delta_fp;	// ebp of the last Delta frame before a C call
extern "C" oop*	    last_Delta_sp;	// esp of the last Delta frame before a C call
extern "C" void popStackHandles(char* nextFrame);

char* StubRoutines::generate_call_delta(MacroAssembler* masm) {
// This is the general Delta entry point. All code that is calling the interpreter or
// compiled code is entering via this entry point. In case of an NLR leaving Delta code,
// the global NLR variables are set. Note: Needs to be *outside* the interpreters code.
//
// method must be a methodOop or a non-zombie nmethod (zombie nmethods must be filtered
// out before - calling call_delta with a zombie nmethod crashes the system).
//
// Note: Shouldn't we preserve some C registers before entering Delta and restore
// them when coming back? At least that's what C calling convention would
// require. Might not be a problem because the calling C funtions don't do
// anything anymore calling call_delta. CHECK THIS! (gri 6/10/96)
//
// indeed we have to preserve EDI & ESI or else the debug mode stack
// check assertions will fail (cmp esp, esi)				-Marc 04/07

  char* nlr_return_from_Delta_entry = generate_nlr_return_from_Delta(masm);
  
  Label _loop, _no_args, _is_compiled, _return, _nlr_test, _nlr_setup, _stack_ok;

  // extern "C" oop call_delta(void* method, oop receiver, int nofArgs, oop* args)
  // incoming arguments
  Address method	= Address(ebp, +2*oopSize);
  Address receiver	= Address(ebp, +3*oopSize);
  Address nofArgs	= Address(ebp, +4*oopSize);
  Address args		= Address(ebp, +5*oopSize);
  
  char* entry_point = masm->pc();

  // setup stack frame
  masm->enter();

  // last_Delta_fp & last_Delta_sp must be the first two words in
  // the stack frame; i.e. at ebp - 4 and ebp - 8. See also frame.hpp.
  masm->pushl(Address((int)&last_Delta_fp, relocInfo::external_word_type));
  masm->pushl(Address((int)&last_Delta_sp, relocInfo::external_word_type));

  masm->pushl(edi);	// save registers for C calling convetion
  masm->pushl(esi);
  masm->movl(edi, Address(esp, 12));

  // reset last Delta frame
  masm->reset_last_Delta_frame();
  // test for stack corruption
  masm->testl(edi, edi);
  masm->jcc(Assembler::equal, _stack_ok);
  masm->cmpl(esp, edi);
  masm->jcc(Assembler::less, _stack_ok);
  // break because of stack corruption
  //masm->int3();
  
  masm->bind(_stack_ok);      
  // setup calling stack frame with arguments
  masm->movl(ebx, nofArgs);	// get no. of arguments
  masm->movl(ecx, args);	// pointer to first argument
  masm->testl(ebx, ebx);
  masm->jcc(Assembler::zero, _no_args);

 masm->bind(_loop);		    
  masm->movl(edx, Address(ecx));	// get argument
  masm->addl(ecx, oopSize);		// advance to next argument
  masm->pushl(edx);			// push argument on stack
  masm->decl(ebx);			// decrement argument counter
  masm->jcc(Assembler::notZero, _loop);	// until no arguments
  
  // call Delta method
 masm->bind(_no_args);
  masm->movl(eax, receiver);
  masm->xorl(ebx, ebx);		    // _restore_ebx
  masm->movl(edx, method);
  masm->test(edx, Mem_Tag);
  masm->jcc(Assembler::zero, _is_compiled);
  masm->movl(ecx, edx);
  masm->movl(edx, Address((int)&method_entry_point, relocInfo::external_word_type));

  // eax: receiver
  // ebx: 0
  // ecx: methodOop (if not compiled)
  // edx: calling address
  // Note: no zombie nmethods possible -> no 2nd ic_info word required
 masm->bind(_is_compiled);
  masm->call(edx);
 _return_from_Delta = masm->pc();
  masm->ic_info(_nlr_test, 0);
  masm->movl(Address((int)&have_nlr_through_C, relocInfo::external_word_type), 0);

 masm->bind(_return);
  masm->leal(esp, Address(ebp, -4*oopSize));
  masm->popl(esi);	// restore registers for C calling convetion
  masm->popl(edi);
  masm->popl(Address((int)&last_Delta_sp, relocInfo::external_word_type)); // reset _last_Delta_sp
  masm->popl(Address((int)&last_Delta_fp, relocInfo::external_word_type)); // reset _last_Delta_fp
  masm->popl(ebp);
  masm->ret(0);	// remove stack frame & return

  // When returning from Delta to C via a NLR, the following code
  // sets up the global NLR variables and patches the return address
  // of the first C frame in the last_C_chunk of the stack (see below).
 masm->bind(_nlr_test);
  masm->movl(ecx, Address(ebp, -2*oopSize));	// get pushed value of _last_Delta_sp
  masm->testl(ecx, ecx);
  masm->jcc(Assembler::zero, _nlr_setup);

  masm->movl(edx, Address(ecx, -oopSize)); // get return address of the first C function called
  // store return address for nlr_return_from_Delta
  masm->movl(Address((int)&C_frame_return_addr, relocInfo::external_word_type), edx);
//  masm->hlt();

//  char* nlr_return_from_Delta_addr = StubRoutines::nlr_return_from_Delta();
//  assert(nlr_return_from_Delta_addr, "nlr_return_from_Delta not initialized yet");
//  masm->movl(Address(ecx, -oopSize), (int)nlr_return_from_Delta_addr);  // patch return address
  masm->movl(Address(ecx, -oopSize), (int)nlr_return_from_Delta_entry);  // patch return address
  masm->pushl(eax);
  masm->pushl(ebx);
  masm->pushl(edx);
  masm->pushl(edi);
  masm->pushl(esi);
  masm->pushl(ecx);
  masm->call((char*)&popStackHandles, relocInfo::external_word_type);
  masm->popl(ecx);
  masm->popl(esi);
  masm->popl(edi);
  masm->popl(edx);
  masm->popl(ebx);
  masm->popl(eax);

 masm->bind(_nlr_setup);
  // setup global NLR variables
  masm->movl(Address((int)&have_nlr_through_C,    relocInfo::external_word_type), 1);
  masm->movl(Address((int)&nlr_result,		  relocInfo::external_word_type), eax);
  masm->movl(Address((int)&nlr_home,		  relocInfo::external_word_type), edi);
  masm->movl(Address((int)&nlr_home_id,		  relocInfo::external_word_type), esi);
  masm->jmp(_return);
  
  return entry_point;
}

char* StubRoutines::generate_nlr_return_from_Delta(MacroAssembler* masm) {
  // When returning from C to Delta via a NLR, the following code
  // continues an ongoing NLR. In case of a NLR, the return address of
  // the first C frame in the last_C_chunk of the stack is patched such
  // that C will return to _nlr_setup, which in turn returns to the
  // NLR testpoint of the primitive that called C.
  
  char* entry_point = masm->pc();
  
  masm->reset_last_Delta_frame();
  masm->movl(eax, Address((int)&nlr_result,	relocInfo::external_word_type));
  masm->movl(edi, Address((int)&nlr_home,	relocInfo::external_word_type));
  masm->movl(esi, Address((int)&nlr_home_id,	relocInfo::external_word_type));
  
  // get return address
  masm->movl(ebx, Address((int)&C_frame_return_addr,	relocInfo::external_word_type));
  masm->movl(ecx, Address(ebx, IC_Info::info_offset));	    // get nlr_offset
  masm->sarl(ecx, IC_Info::number_of_flags);		    // shift ic info flags out
  masm->addl(ebx, ecx);			    		    // compute NLR test point address
  masm->jmp(ebx);			    		    // return to nlr test point
  
  return entry_point;
}

//-----------------------------------------------------------------------------------------
// single_step_stub

extern "C" int* frame_breakpoint;   // dispatch table
extern "C" doFn original_table[Bytecodes::number_of_codes];
extern "C" void single_step_handler();

void (*StubRoutines::single_step_fn)() = NULL;
//extern "C" void nlr_single_step_continuation();

//extern "C" char* single_step_handler;
//extern "C" char* nlr_single_step_continuation;

char* StubRoutines::generate_single_step_stub(MacroAssembler* masm) {
// slr mod:
//   first the single step continuation
//		- used as the return address of the single step code below
    _single_step_continuation = masm->pc();
	// inline cache for non local return
    int offset = Interpreter::nlr_single_step_continuation_entry() - _single_step_continuation;
    int ic_info = (offset << 8) + 0;
    masm->testl(eax, ic_info);
	//masm->ic_info(Interpreter::nlr_single_step_continuation(), 0);

	masm->reset_last_Delta_frame();
	masm->popl(eax);
	// restore bytecode pointer
	masm->movl(esi, Address(ebp, -2*oopSize));
	// reload bytecode
	masm->xorl(ebx, ebx);
	masm->movb(ebx, Address(esi));
	// execute bytecode
	masm->jmp(Address(noreg, ebx, Address::times_4, (int)original_table));

//   then the calling stub
// end slr mod
//   parameters:
//     ebx = byte code
//     esi = byte code pointer
//

  Label is_break;

  char* entry_point = masm->pc();

//  masm->int3();
  masm->cmpl(ebp, Address((int)&frame_breakpoint, relocInfo::external_word_type));
  masm->jcc(Assembler::greaterEqual, is_break);	
  masm->jmp(Address(noreg, ebx, Address::times_4, (int)original_table));

 masm->bind(is_break);
  masm->movl(Address(ebp, -2*oopSize), esi);	// save esi
  masm->pushl(eax);				// save tos
  masm->set_last_Delta_frame_before_call();
      
//  masm->pushl(Address((int)&nlr_single_step_continuation, relocInfo::external_word_type));
//  assert(nlr_single_step_continuation, "%fix this");
  // %hack: indirect load
//  masm->int3();
//  masm->hlt();
//  slr mod: masm->movl(edx, (int)Interpreter::nlr_single_step_continuation());
//  slr mod: masm->pushl(Address(edx));
  masm->pushl((int)_single_step_continuation);
//  end slr mod
  
//  assert(single_step_handler, "%fix this");
  masm->jmp(Address(noreg, noreg, Address::no_scale, (int)&single_step_fn));
//  masm->jmp((char*)single_step_handler, relocInfo::runtime_call_type);
//  masm->jmp(single_step_handler, relocInfo::runtime_call_type);
  //	Should not reach here
  masm->hlt();

  return entry_point;
}
//-----------------------------------------------------------------------------------------
// unpack_unoptimized_frames

extern "C" oop* setup_deoptimization_and_return_new_sp(oop* old_sp, int* old_fp, objArrayOop frame_array, int* current_frame);
extern "C" void unpack_frame_array();
extern "C" bool nlr_through_unpacking;
extern "C" oop  result_through_unpacking;

char* StubRoutines::generate_unpack_unoptimized_frames(MacroAssembler* masm) {
// Invoked when returning to an unoptimized frame. 
// Deoptimizes the frame_array into a stack stretch of interpreter frames
//
// _unpack_unoptimized_frames must look like a compiled inline cache
// so NLR works across unoptimized frames.
// Since a NLR may have its home inside the optimized frames we have to deoptimize
// and then continue the NLR.

  Address real_sender_sp    = Address(ebp, -2*oopSize);
  Address frame_array	    = Address(ebp, -1*oopSize);
  Address real_fp	    = Address(ebp);

  Register nlr_result_reg  = eax; // holds the result of the method
  Register nlr_home_reg    = edi; // the home frame ptr
  Register nlr_home_id_reg = esi; // used to pass esi
  
  Label wrapper_for_unpack_frame_array, _return;

 masm->bind(wrapper_for_unpack_frame_array);
  masm->enter();	
  masm->call((char*)unpack_frame_array, relocInfo::runtime_call_type);
  // Restore the nlr state
  masm->cmpl(Address((int)&nlr_through_unpacking,		relocInfo::external_word_type), 0);
  masm->jcc(Assembler::equal, _return);
  masm->movl(Address((int)&nlr_through_unpacking,		relocInfo::external_word_type), 0);
  masm->movl(nlr_result_reg,	Address((int)&nlr_result,	relocInfo::external_word_type));
  masm->movl(nlr_home_reg,	Address((int)&nlr_home,		relocInfo::external_word_type));
  masm->movl(nlr_home_id_reg, 	Address((int)&nlr_home_id,	relocInfo::external_word_type));

 masm->bind(_return);
  masm->popl(ebp);
  masm->ret(0);

  Label common_unpack_unoptimized_frames;

 masm->bind(common_unpack_unoptimized_frames);
  masm->pushl(ebp);			// Push the old   frame pointer
  masm->pushl(frame_array);		// Push the array with the packed frames
  masm->pushl(real_fp);		// Push the frame pointer link
  masm->pushl(real_sender_sp);	// Push the stack pointer of the calling activation

  // Compute the new stack pointer
  masm->call((char*)setup_deoptimization_and_return_new_sp, relocInfo::runtime_call_type);
  masm->movl(esp, eax);			// Set the new stack pointer
  masm->movl(ebp, real_fp);	    		// Set the frame pointer to the link
  masm->pushl(-1);				// Push invalid return address
  masm->jmp(wrapper_for_unpack_frame_array);	// Call the unpacking function

  Label nlr_unpack_unoptimized_frames;

 masm->bind(nlr_unpack_unoptimized_frames);
  masm->movl(Address((int)&nlr_through_unpacking,   relocInfo::external_word_type), 1);
  masm->movl(Address((int)&nlr_result,		    relocInfo::external_word_type), nlr_result_reg);
  masm->movl(Address((int)&nlr_home,	  	    relocInfo::external_word_type), nlr_home_reg);
  masm->movl(Address((int)&nlr_home_id,		    relocInfo::external_word_type), nlr_home_id_reg);
  masm->jmp(common_unpack_unoptimized_frames);

  char* entry_point = masm->pc();
  masm->ic_info(nlr_unpack_unoptimized_frames, 0);
  masm->movl(Address((int)&nlr_through_unpacking,	relocInfo::external_word_type), 0);
  masm->movl(Address((int)&result_through_unpacking,	relocInfo::external_word_type), eax);
  masm->jmp(common_unpack_unoptimized_frames);

  return entry_point;
}

char* StubRoutines::generate_provoke_nlr_at(MacroAssembler* masm) {
  // extern "C" void provoke_nlr_at(int* frame_pointer, oop* stack_pointer);
  Address old_ret_addr    = Address(esp, -1*oopSize);
  Address frame_pointer   = Address(esp, +1*oopSize);
  Address stack_pointer   = Address(esp, +2*oopSize);

  Register nlr_result_reg  = eax; // holds the result of the method
  Register nlr_home_reg    = edi; // the home frame ptr
  Register nlr_home_id_reg = esi; // used to pass esi

  char* entry_point = masm->pc();

  masm->movl(ebp, frame_pointer);			// set new frame pointer
  masm->movl(esp, stack_pointer);			// set new stack pointer
  masm->movl(ebx, old_ret_addr);			// find old return address
  
  masm->movl(nlr_result_reg,	Address((int)&nlr_result,   relocInfo::external_word_type));
  masm->movl(nlr_home_reg,	Address((int)&nlr_home,	    relocInfo::external_word_type));
  masm->movl(nlr_home_id_reg,	Address((int)&nlr_home_id,  relocInfo::external_word_type));

  masm->movl(ecx, Address(ebx, IC_Info::info_offset));    // get nlr_offset
  masm->sarl(ecx, IC_Info::number_of_flags);		    // shift ic info flags out
  masm->addl(ebx, ecx);			    	    // compute NLR test point address
  masm->jmp(ebx);			    		    // return to nlr test point

  return entry_point;
}

char* StubRoutines::generate_continue_nlr_in_delta(MacroAssembler* masm) {
  // extern "C" void continue_nlr_in_delta(int* frame_pointer, oop* stack_pointer);
  Address old_ret_addr    = Address(esp, -1*oopSize);
  Address frame_pointer   = Address(esp, +1*oopSize);
  Address stack_pointer   = Address(esp, +2*oopSize);

  Register nlr_result_reg  = eax; // holds the result of the method
  Register nlr_home_reg    = edi; // the home frame ptr
  Register nlr_home_id_reg = esi; // used to pass esi
  
  char* entry_point = masm->pc();
  
  masm->movl(ebp, frame_pointer);			// set new frame pointer
  masm->movl(esp, stack_pointer);			// set new stack pointer
  masm->movl(ebx, old_ret_addr);			// find old return address

  masm->movl(nlr_result_reg,	Address((int)&nlr_result,   relocInfo::external_word_type));
  masm->movl(nlr_home_reg,	Address((int)&nlr_home,	    relocInfo::external_word_type));
  masm->movl(nlr_home_id_reg,	Address((int)&nlr_home_id,  relocInfo::external_word_type));

  masm->jmp(ebx);			    		// continue

  return entry_point;
}

//-------------------------------------------------------------------------------
// Stub routines for callbacks (see callBack.cpp)
extern "C" volatile void* handleCallBack(int index, int params);

char* StubRoutines::generate_handle_pascal_callback_stub(MacroAssembler* masm) {
  // Stub routines called from a "Pascal" callBack chunk (see callBack.cpp)
  // Incomming arguments:
  //  ecx, ecx = index           (passed on to Delta)
  //  edx, edx = number of bytes (to be deallocated before returning)
  
  char* entry_point = masm->pc();

  // create link
  masm->enter();

  // save registers for Pascal calling convention
  masm->pushl(ebx);
  masm->pushl(edi);
  masm->pushl(esi);
    
  // save number of bytes in parameter list
  masm->pushl(edx);

  // compute parameter start
  masm->movl(edx, esp);
  masm->addl(edx, 24); 		// (esi, edi, ebx, edx, fp, return address)
  
  // eax = handleCallBack(index, &params)
  masm->pushl(edx); 		// &params
  masm->pushl(ecx); 		// index
  masm->call((char*)handleCallBack, relocInfo::runtime_call_type);
  masm->addl(esp, 2*oopSize); 	// pop the arguments

  // restore number of bytes in parameter list
  masm->popl(edx);

  // restore registers for Pascal calling convention
  masm->popl(esi);
  masm->popl(edi);
  masm->popl(ebx);

  // destroy link
  masm->leave();

  // get return address
  masm->popl(ecx);

  // deallocate the callers parameters
  masm->addl(esp, edx);

  // jump to caller
  masm->jmp(ecx);
		
  return entry_point;
}

char* StubRoutines::generate_handle_C_callback_stub(MacroAssembler* masm) {
  // Stub routines called from a "C" callBack chunk (see callBack.cpp)
  // Incomming arguments:
  // eax = index               (passed on to Delta)

  Label stackOK;
   char* entry_point = masm->pc();

   // create link
   masm->enter();
   
   // save registers for C calling convention
   masm->pushl(ebx);
   masm->pushl(edi);
   masm->pushl(esi);

   // compute parameter start
   masm->movl(edx, esp);
   masm->addl(edx, 20); //  (esi, edi, ebx, fp, return address)

   // eax = handleCallBack(index, &params)
   masm->pushl(esp);
   masm->pushl(edx); // &params
   masm->pushl(ecx); // index
   masm->call((char*)handleCallBack, relocInfo::runtime_call_type);
   masm->addl(esp, 2*oopSize); 	// pop the arguments
   masm->popl(esi);
   masm->cmpl(esi, esp);
   masm->jcc(Assembler::equal, stackOK);
   masm->int3();
   masm->bind(stackOK);
   // restore registers for Pascal calling convention
   masm->popl(esi);
   masm->popl(edi);
   masm->popl(ebx);

   // destroy link
   masm->leave();

   // jump to caller
   masm->ret(0);

  return entry_point;
}

/*
static oop oopify_float() {
  double x;
  __asm fstp x							// get top of FPU stack
    BlockScavenge bs;						// because all registers are saved on the stack
  return oopFactory::new_double(x);				// box the FloatValue
}
*/

char* StubRoutines::generate_oopify_float(MacroAssembler* masm) {
  char* entry_point = masm->pc();
  
 // masm->int3();
 // masm->hlt();
 // masm->call((char*)::oopify_float, relocInfo::runtime_call_type);
  masm->enter();
  masm->subl(esp, 8);
  masm->fstp_d(Address(esp));
  masm->incl(Address((int)BlockScavenge::counter_addr(), relocInfo::external_word_type));
  masm->call((char*)oopFactory::new_double, relocInfo::runtime_call_type);
  masm->decl(Address((int)BlockScavenge::counter_addr(), relocInfo::external_word_type));
  masm->leave();
  masm->ret();
  
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
void StubRoutines::alien_arg_size(MacroAssembler* masm, Label &nextArg) {
    Label isPointer, isDirect, isSMI, isUnsafe;

    masm->movl(ecx, Address(eax));                        // load current arg
    masm->testl(ecx, Mem_Tag);
    masm->jcc(Assembler::equal, isSMI);

    masm->movl(esi, Address(ecx, memOopDesc::klass_byte_offset()));// get class
    masm->movl(esi, Address(esi, klassOopDesc::nonIndexableSizeOffset()));// get non-indexable size
                                                          // start of the byte array's bytes
    masm->leal(ecx, Address(ecx, esi, Address::times_1, - Mem_Tag));
    masm->testl(ecx, Mem_Tag);
    masm->jcc(Assembler::notEqual, isUnsafe);
    masm->addl(ecx, 4);

    masm->movl(esi, Address(ecx));                        // load the size
    masm->testl(esi, esi);                                // direct?
    masm->jcc(Assembler::equal, isPointer);               // pointer == 0
    masm->jcc(Assembler::greater, isDirect);              // direct > 0
    
    masm->addl(edx, esi);                                 // indirect so size negative  
    masm->jmp(nextArg);
    
    masm->bind(isDirect);
    masm->subl(edx, esi);
    masm->jmp(nextArg);

    masm->bind(isUnsafe);
    masm->bind(isPointer);
    masm->bind(isSMI);
    masm->subl(edx, 4);
}
void StubRoutines::push_alignment_spacers(MacroAssembler* masm) {
  Label pushArgs;
  masm->andl(edx, 0xf);                                   // push spacers to align stack
  masm->jcc(Assembler::equal, pushArgs);
  masm->subl(edx, 4);
  masm->pushl(0);
  masm->jcc(Assembler::equal, pushArgs);
  masm->subl(edx, 4);
  masm->pushl(0);
  masm->jcc(Assembler::equal, pushArgs);
  masm->subl(edx, 4);
  masm->pushl(0);
  masm->bind(pushArgs);                                   // end of stack alignment spacers
}
void StubRoutines::push_alien_arg(MacroAssembler *masm, Label &nextArg) {
  Label isSMI, isPointer, isDirect, isUnsafe, startMove, moveLoopTest, moveLoopHead;
    masm->testl(eax, Mem_Tag);
    masm->jcc(Assembler::equal, isSMI);

    masm->movl(esi, Address(eax, sizeof(memOopDesc) - Mem_Tag));
                                                          // load the first ivar
                                                          //   either smiOop for normal alien
                                                          //   or memOop for unsafe alien
    masm->testl(esi, Mem_Tag);                            
    masm->jcc(Assembler::notEqual, isUnsafe);

    masm->movl(esi, Address(eax, memOopDesc::klass_byte_offset()));
    masm->movl(esi, Address(esi, klassOopDesc::nonIndexableSizeOffset()));
    masm->leal(eax, Address(eax, esi, Address::times_1, 4 - Mem_Tag)); // start of the oops in the array

    masm->movl(esi, Address(eax));                        // load the size
    masm->testl(esi, esi);                                // direct?
    masm->jcc(Assembler::equal, isPointer);               // pointer == 0
    masm->jcc(Assembler::greater, isDirect);              // direct > 0
                                                          // indirect < 0
    masm->movl(eax, Address(eax, 4));                     // load the indirect start address
    masm->jmp(startMove);

    masm->bind(isPointer);                                // push the pointer
    masm->movl(eax, Address(eax, 4));
    masm->pushl(eax);
    masm->jmp(nextArg);
    
    masm->bind(isDirect);
    masm->negl(esi);                                      // negate the size to lower esp
    masm->addl(eax, 4);                                   // start of direct contents
    
    masm->bind(startMove);
    masm->pushl(0);                                       // pad odd sizes with zero
    masm->leal(esp, Address(esp, esi, Address::times_1, 4)); // move stack pointer by size of data
    masm->andl(esp, -4);                                  // ensure stack 4-byte aligned

    masm->negl(esi);                                      // negate size to use as offset
    masm->jmp(moveLoopTest);
      masm->bind(moveLoopHead);
      masm->movl(ecx, Address(eax, esi, Address::times_1));
      masm->movl(Address(esp, esi, Address::times_1), ecx);

      masm->bind(moveLoopTest);                             // continue?
      masm->subl(esi, 4);
      masm->cmpl(esi, 0);
      masm->jcc(Assembler::greaterEqual, moveLoopHead);     // y - next iteration
                                                            // n - less than four bytes remain
      masm->addl(esi, 3);                                   // adjust offset for byte moves
      masm->jcc(Assembler::less, nextArg);                  // y - exact multiple of four bytes
                                                            // n - 1-3 bytes need to be moved
      masm->movb(ecx, Address(eax, esi, Address::times_1));
      masm->movb(Address(esp, esi, Address::times_1), ecx);
      masm->decl(esi);
      masm->jcc(Assembler::less, nextArg);                  // y - one extra byte
                                                            // n - 2-3 extra bytes
      masm->movb(ecx, Address(eax, esi, Address::times_1));
      masm->movb(Address(esp, esi, Address::times_1), ecx);
      masm->decl(esi);
      masm->jcc(Assembler::less, nextArg);                  // y - two extra bytes
                                                            // n - three extra bytes
      masm->movb(ecx, Address(eax, esi, Address::times_1));
      masm->movb(Address(esp, esi, Address::times_1), ecx);
      masm->jmp(nextArg);                                   // finished

    masm->bind(isUnsafe);

    masm->movl(eax, Address(esi, memOopDesc::klass_byte_offset()));
    masm->movl(eax, Address(eax, klassOopDesc::nonIndexableSizeOffset()));
    masm->leal(esi, Address(esi, eax, Address::times_1, 4 - Mem_Tag)); // start of the bytes in the array
    masm->pushl(esi);
    masm->jmp(nextArg);

    masm->bind(isSMI);
    masm->sarl(eax, 2);
    masm->pushl(eax);

    masm->bind(nextArg);
}
extern "C" {
  void PRIM_API enter_async_call(DeltaProcess**);
  void PRIM_API exit_async_call(DeltaProcess**);
}
char* StubRoutines::generate_alien_call_with_args(MacroAssembler* masm) {
  Label no_result, ptr_result, short_ptr_result, short_result, argLoopStart;
  Label isSMI, isDirect, startMove, isPointer, nextArg, moveLoopHead, moveLoopEnd, 
    moveLoopTest, argLoopExit, argLoopTest, pushArgs;
  Label sizeLoopTest, sizeLoopStart;

  Address fnptr(ebp,8);
  Address result(ebp,12);
  Address argCount(ebp,16);
  Address argArray(ebp,20);
  Address proc(ebp,-16);
  char* entry_point = masm->pc();

  masm->enter();
  masm->pushl(esi);                                       // preserve registers
  masm->pushl(edi);
  masm->pushl(ebx);
  masm->subl(esp, 4);                                     // process pointer

  masm->movl(edi, argArray);                              // start of the oops in the array
  masm->movl(ebx, argCount);
  masm->leal(ebx, Address(edi, ebx, Address::times_1));   // upper bounds of array

  masm->movl(edx, esp);                                   // start of size calculation

  //masm->int3();

  masm->movl(eax, ebx);                                   // eax is pointer to current arg
  masm->jmp(sizeLoopTest);
    masm->bind(sizeLoopStart);
    
    alien_arg_size(masm, sizeLoopTest);

    masm->bind(sizeLoopTest);                             // end of size calculation
    masm->subl(eax, 4);
    masm->cmpl(eax, edi);
  masm->jcc(Assembler::greaterEqual, sizeLoopStart);

  push_alignment_spacers(masm);
  //masm->int3();

  masm->jmp(argLoopTest);
    masm->bind(argLoopStart);
    masm->movl(eax, Address(ebx));

    push_alien_arg(masm, argLoopTest);

    masm->subl(ebx, 4);
    masm->cmpl(ebx, edi);
    masm->jcc(Assembler::less, argLoopExit);

    masm->jmp(argLoopStart);
  masm->bind(argLoopExit);
#ifdef ASYNC_ALIEN
  masm->leal(eax, proc);
  masm->pushl(eax);
  masm->call((char*)enter_async_call, relocInfo::external_word_type);
#endif
  masm->call(fnptr);                            // call the alien function

#ifdef ASYNC_ALIEN
  masm->pushl(eax);
  masm->leal(eax, proc);
  masm->pushl(edx);
  masm->pushl(eax);
  masm->call((char*) exit_async_call, relocInfo::external_word_type);
  masm->popl(edx);
  masm->popl(eax);
#endif
  masm->movl(ecx, result);                      // result alien
  
  masm->testl(ecx, ecx);
  masm->jcc(Assembler::equal, no_result);       // is a result required?
  
  //masm->int3(); //debug
                                                // get the start of the alien area
  masm->movl(ecx, Address(ecx));
  masm->movl(esi, Address(ecx, memOopDesc::klass_byte_offset()));

  masm->movl(esi, Address(esi, klassOopDesc::nonIndexableSizeOffset()));
  masm->leal(ecx, Address(ecx, esi, Address::times_1, 4 - Mem_Tag));
                                                // ecx now points to start of alien contents
  masm->movl(esi, Address(ecx));
  masm->testl(esi, esi);                        
  masm->jcc(Assembler::less, ptr_result);
  masm->jcc(Assembler::equal, short_result);
  
  masm->cmpl(esi, 4);                           // direct result
  masm->jcc(Assembler::equal, short_result);
  masm->movl(Address(ecx,8), edx);              
  masm->bind(short_result);
  masm->movl(Address(ecx,4), eax);              

  masm->jmp(no_result);

  masm->bind(ptr_result);                       // indirect result
  masm->cmpl(esi, -4);
  masm->movl(esi, Address(ecx, 4));
  masm->jcc(Assembler::equal, short_ptr_result);
  masm->movl(Address(esi, 4), edx);             
  masm->bind(short_ptr_result);
  masm->movl(Address(esi), eax);                
  
  masm->bind(no_result);
  masm->movl(esi, Address(ebp, -4));            // restore registers
  masm->movl(edi, Address(ebp, -8));
  masm->movl(ebx, Address(ebp, -12));
  masm->leave();

  masm->ret(16);
  return entry_point;
}

char* StubRoutines::generate_alien_call(MacroAssembler* masm, int args) {
  Label no_result, ptr_result, short_ptr_result, short_result, pushArgs;
  Address fnptr(ebp,8);
  Address result(ebp,12);
  Address proc(ebp,-8);
  char* entry_point = masm->pc();

  masm->enter();
  masm->pushl(esi);                             // preserve registers
  masm->subl(esp, 4);                           // make space for process pointer

  masm->movl(edx, esp);
  for (int arg = 0; arg < args; arg++) {
    Address argAddress(ebp, 16 + (args - arg - 1) * 4);
    Label nextArg;
    masm->leal(eax, argAddress);
    alien_arg_size(masm, nextArg);
      
    masm->bind(nextArg);
  }

  masm->andl(edx, 0xf);
  masm->jcc(Assembler::equal, pushArgs);
  masm->pushl(0);
  masm->subl(edx, 4);
  masm->jcc(Assembler::equal, pushArgs);
  masm->pushl(0);
  masm->subl(edx, 4);
  masm->jcc(Assembler::equal, pushArgs);
  masm->pushl(0);
  masm->subl(edx, 4);
  masm->bind(pushArgs);
  //if (args > 0) masm->int3();

  for (int arg1 = 0; arg1 < args; arg1++) {
    Address argAddress(ebp, 16 + (args - arg1 - 1) * 4);
    Label moveLoopEnd;
    masm->movl(eax, argAddress);

    push_alien_arg(masm, moveLoopEnd);
  }
#ifdef ASYNC_ALIEN
  masm->leal(eax, proc);
  masm->pushl(eax);
  masm->call((char*)enter_async_call, relocInfo::external_word_type);
#endif

  masm->call(fnptr);                            // call the alien function

#ifdef ASYNC_ALIEN
  masm->pushl(eax);
  masm->leal(eax, proc);
  masm->pushl(edx);
  masm->pushl(eax);
  masm->call((char*) exit_async_call, relocInfo::external_word_type);
  masm->popl(edx);
  masm->popl(eax);
#endif

  masm->movl(ecx, result);                      // result alien
  
  masm->testl(ecx, ecx);
  masm->jcc(Assembler::equal, no_result);       // is a result required?
  
  //masm->int3(); //debug
                                                // get the start of the alien area
  masm->movl(ecx, Address(ecx));
  masm->movl(esi, Address(ecx, memOopDesc::klass_byte_offset()));

  masm->movl(esi, Address(esi, klassOopDesc::nonIndexableSizeOffset()));
  masm->leal(ecx, Address(ecx, esi, Address::times_1, 4 - Mem_Tag));
                                                // ecx now points to start of alien contents
  masm->movl(esi, Address(ecx));
  masm->testl(esi, esi);                        
  masm->jcc(Assembler::less, ptr_result);
  masm->jcc(Assembler::equal, short_result);
  
  masm->cmpl(esi, 4);                           // direct result
  masm->jcc(Assembler::equal, short_result);
  masm->movl(Address(ecx,8), edx);              
  masm->bind(short_result);
  masm->movl(Address(ecx,4), eax);              

  masm->jmp(no_result);

  masm->bind(ptr_result);                       // indirect/pointer result
  masm->cmpl(esi, -4);
  masm->movl(esi, Address(ecx, 4));
  masm->jcc(Assembler::equal, short_ptr_result);
  masm->movl(Address(esi, 4), edx);             
  masm->bind(short_ptr_result);
  masm->movl(Address(esi), eax);                
  
  masm->bind(no_result);
  
  masm->movl(esi, Address(ebp, -4));            // restore registers
  masm->leave();
  masm->ret(8 + (args << 2));
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

char* StubRoutines::alien_call_entry(int args) {
  assert(_is_initialized, "StubRoutines not initialized yet");
  assert(0 <= args && args <= max_fast_alien_call_size, "size out of range")
  return _alien_call_entries[args];
}


// Initialization

bool StubRoutines::_is_initialized = false;
//char StubRoutines::_code[StubRoutines::_code_size];
char* StubRoutines::_code = NULL;
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

  _code = os::exec_memory(_code_size);
  
  ResourceMark rm;
  CodeBuffer* code = new CodeBuffer(_code, _code_size);
  MacroAssembler* masm = new MacroAssembler(code);

  // add generators here
  _ic_normal_lookup_entry	= generate(masm, "ic_normal_lookup",		generate_ic_normal_lookup		);
  _ic_super_lookup_entry	= generate(masm, "ic_super_lookup",		generate_ic_super_lookup		);
  _zombie_nmethod_entry		= generate(masm, "zombie_nmethod",		generate_zombie_nmethod			);
  _zombie_block_nmethod_entry	= generate(masm, "zombie_block_nmethod",	generate_zombie_block_nmethod		);
  _megamorphic_ic_entry		= generate(masm, "megamorphic_ic",		generate_megamorphic_ic			);
  _compile_block_entry		= generate(masm, "compile_block", 		generate_compile_block			);
  _continue_NLR_entry		= generate(masm, "continue_NLR",		generate_continue_NLR			);
  _call_sync_DLL_entry		= generate(masm, "call_sync_DLL",	 	generate_call_sync_DLL			);
  _call_async_DLL_entry		= generate(masm, "call_async_DLL",	 	generate_call_async_DLL			);
  _lookup_sync_DLL_entry	= generate(masm, "lookup_sync_DLL",	 	generate_lookup_sync_DLL		);
  _lookup_async_DLL_entry	= generate(masm, "lookup_async_DLL",	 	generate_lookup_async_DLL		);
  _recompile_stub_entry		= generate(masm, "recompile_stub",	 	generate_recompile_stub			);
  _used_uncommon_trap_entry	= generate(masm, "used_uncommon_trap",	 	generate_uncommon_trap			);
  _unused_uncommon_trap_entry	= generate(masm, "unused_uncommon_trap", 	generate_uncommon_trap			);
  _verify_context_chain_entry	= generate(masm, "verify_context_chain", 	generate_verify_context_chain		);
  _deoptimize_block_entry	= generate(masm, "deoptimize_block",	 	generate_deoptimize_block		);
  _call_inspector_entry		= generate(masm, "call_inspector",	 	generate_call_inspector			);
  _call_delta			= generate(masm, "call_delta",		  	generate_call_delta			);
  _single_step_stub		= generate(masm, "single_step_stub",	  	generate_single_step_stub		);
  _unpack_unoptimized_frames	= generate(masm, "unpack_unoptimized_frames",	generate_unpack_unoptimized_frames	);
  _provoke_nlr_at		= generate(masm, "provoke_nlr_at",	 	generate_provoke_nlr_at			);
  _continue_nlr_in_delta	= generate(masm, "continue_nlr_in_delta",	generate_continue_nlr_in_delta		);
  _handle_pascal_callback_stub	= generate(masm, "handle_pascal_callback_stub",	generate_handle_pascal_callback_stub	);
  _handle_C_callback_stub	= generate(masm, "handle_C_callback_stub",	generate_handle_C_callback_stub		);
  _oopify_float			= generate(masm, "oopify_float",		generate_oopify_float			);
  _alien_call_with_args_entry   = generate(masm, "alien_call_with_args",        generate_alien_call_with_args           );
  for (int pic_size = 1; pic_size <= PIC::max_nof_entries; pic_size++) {
    _PIC_stub_entries[pic_size] = generate(masm, "PIC stub", generate_PIC_stub, pic_size);
  }

  for (int size = 0; size <= max_fast_allocate_size; size++) {
    _allocate_entries[size] = generate(masm, "allocate", generate_allocate, size);
  }

  for (int args = 0; args <= max_fast_alien_call_size; args++) {
    _alien_call_entries[args] = generate(masm, "alien_call", generate_alien_call, args);
  }

  masm->finalize();
  assert(code->code_size() < _code_size, "Stub routines too large for allocated space");
  _is_initialized = true;
  if (!Disclaimer::is_product() && PrintStubRoutines) {
    std->print("%d bytes generated for stub routines\n", masm->offset());
    exit(0);
  }
};

/*

  %note: initialisation is in InterpreterGenerator now -Marc 04/07

void stubRoutines_init() {
  StubRoutines::init();
}
*/
