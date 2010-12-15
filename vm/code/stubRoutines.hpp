/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.19 $ */
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


// StubRoutines contains a set of little assembly run-time routines.
// Instead of relying on an assembler, these routines are generated
// during system initialization.
// Note: The stubroutines are *outside* of the interpreter code.
//
// Steps to add a new stub routine:
//
// 1. add a new entry point (class variable)
// 2. add the corresponding entry point accessor (class method)
// 3. implement the stub code generator
// 4. call the generator in init()

class StubRoutines: AllStatic {
 private:
  enum { _code_size = 12000 };			// simply increase if too small (assembler will crash if too small)
  static bool _is_initialized;			// true if StubRoutines has been initialized
//  static char _code[_code_size];		// the code buffer for the stub routines
  static char* _code;		// the code buffer for the stub routines
  static void (*single_step_fn)();              // pointer to the current single step function (used by evaluator and ST debugger)

  // add entry points here
  static char* _ic_normal_lookup_entry;
  static char* _ic_super_lookup_entry;
  static char* _zombie_nmethod_entry;
  static char* _zombie_block_nmethod_entry;
  static char* _megamorphic_ic_entry;
  static char* _compile_block_entry;
  static char* _continue_NLR_entry;
  static char* _call_sync_DLL_entry;
  static char* _call_async_DLL_entry;
  static char* _lookup_sync_DLL_entry;
  static char* _lookup_async_DLL_entry;
  static char* _recompile_stub_entry;
  static char* _used_uncommon_trap_entry;
  static char* _unused_uncommon_trap_entry;
  static char* _verify_context_chain_entry;
  static char* _deoptimize_block_entry;
  static char* _call_inspector_entry;

  static char* _call_delta;
  static char* _return_from_Delta;
  static char* _single_step_stub;
  static char* _single_step_continuation;
  static char* _unpack_unoptimized_frames;
  static char* _provoke_nlr_at;
  static char* _continue_nlr_in_delta;
  static char* _handle_pascal_callback_stub;
  static char* _handle_C_callback_stub;
  static char* _oopify_float;

  static char* _PIC_stub_entries[];
  static char* _allocate_entries[];
  static char* _alien_call_entries[];
  static char* _alien_call_with_args_entry;
  
  // add tracing routines here
  static void trace_DLL_call_1(dll_func function, oop* last_argument, int nof_arguments);
  static void trace_DLL_call_2(int result);
  static void wrong_DLL_call();


  // add generators here
  static char* generate_ic_lookup(MacroAssembler* masm, char* lookup_routine_entry);
  static char* generate_call_DLL(MacroAssembler* masm, bool async);
  static char* generate_lookup_DLL(MacroAssembler* masm, bool async);
  
  static char* generate_ic_normal_lookup		(MacroAssembler* masm);
  static char* generate_ic_super_lookup			(MacroAssembler* masm);
  static char* generate_zombie_nmethod	    		(MacroAssembler* masm);
  static char* generate_zombie_block_nmethod		(MacroAssembler* masm);
  static char* generate_megamorphic_ic			(MacroAssembler* masm);
  static char* generate_compile_block			(MacroAssembler* masm);
  static char* generate_continue_NLR			(MacroAssembler* masm);
  static char* generate_call_sync_DLL			(MacroAssembler* masm)	{ return generate_call_DLL  (masm, false); }
  static char* generate_call_async_DLL			(MacroAssembler* masm)	{ return generate_call_DLL  (masm, true ); }
  static char* generate_lookup_sync_DLL			(MacroAssembler* masm)	{ return generate_lookup_DLL(masm, false); }
  static char* generate_lookup_async_DLL		(MacroAssembler* masm)	{ return generate_lookup_DLL(masm, true ); }
  static char* generate_recompile_stub			(MacroAssembler* masm);
  static char* generate_uncommon_trap			(MacroAssembler* masm);
  static char* generate_verify_context_chain		(MacroAssembler* masm);
  static char* generate_deoptimize_block		(MacroAssembler* masm);
  static char* generate_call_inspector			(MacroAssembler* masm);

  static char* generate_nlr_return_from_Delta		(MacroAssembler* masm);
  static char* generate_call_delta			(MacroAssembler* masm);
  // generate_call_delta assigns _return_from_Delta
  static char* generate_single_step_stub		(MacroAssembler* masm);
  static char* generate_unpack_unoptimized_frames	(MacroAssembler* masm);
  static char* generate_provoke_nlr_at			(MacroAssembler* masm);
  static char* generate_continue_nlr_in_delta		(MacroAssembler* masm);
  static char* generate_handle_pascal_callback_stub	(MacroAssembler* masm);
  static char* generate_handle_C_callback_stub		(MacroAssembler* masm);
  static char* generate_oopify_float			(MacroAssembler* masm);
  
  static char* generate_PIC_stub			(MacroAssembler* masm, int pic_size	);
  static char* generate_allocate			(MacroAssembler* masm, int size		);
  static char* generate_alien_call			(MacroAssembler* masm, int args		);
  static char* generate_alien_call_with_args            (MacroAssembler* masm);

  // helpers for generation
  static char* generate(MacroAssembler* masm, char* title, char* gen(MacroAssembler*));
  static char* generate(MacroAssembler* masm, char* title, char* gen(MacroAssembler*, int argument), int argument);
  static void  alien_arg_size(MacroAssembler* masm, Label &nextArg);
  static void  push_alien_arg(MacroAssembler* masm, Label &nextArg);
  static void  push_alignment_spacers(MacroAssembler* masm);

 public:
  // add entry point accessors here
  static char* ic_normal_lookup_entry()		{ return _ic_normal_lookup_entry; }
  static char* ic_super_lookup_entry()		{ return _ic_super_lookup_entry; }
  static char* zombie_nmethod_entry()		{ return _zombie_nmethod_entry; }
  static char* zombie_block_nmethod_entry()     { return _zombie_block_nmethod_entry; }
  static char* megamorphic_ic_entry()		{ return _megamorphic_ic_entry; }
  static char* compile_block_entry() 		{ return _compile_block_entry; }
  static char* continue_NLR_entry()		{ return _continue_NLR_entry; }
  static char* call_DLL_entry(bool async)	{ return async ? _call_async_DLL_entry   : _call_sync_DLL_entry; }
  static char* lookup_DLL_entry(bool async)	{ return async ? _lookup_async_DLL_entry : _lookup_sync_DLL_entry; }
  static char* recompile_stub_entry()		{ return _recompile_stub_entry; }
  static char* used_uncommon_trap_entry()	{ return _used_uncommon_trap_entry; }
  static char* unused_uncommon_trap_entry()	{ return _unused_uncommon_trap_entry; }
  static char* verify_context_chain()		{ return _verify_context_chain_entry; }
  static char* deoptimize_block_entry()		{ return _deoptimize_block_entry; }
  static char* call_inspector_entry()		{ return _call_inspector_entry; }

  static char* call_delta()			{ return _call_delta; }
  static char* return_from_Delta()		{ return _return_from_Delta; }
  static char* single_step_stub()		{ return _single_step_stub; }
  static char* single_step_continuation()	{ return _single_step_continuation; }
  static char* unpack_unoptimized_frames()	{ return _unpack_unoptimized_frames; }
  static char* provoke_nlr_at()			{ return _provoke_nlr_at; }
  static char* continue_nlr_in_delta()		{ return _continue_nlr_in_delta; }
  static char* handle_pascal_callback_stub()	{ return _handle_pascal_callback_stub; }
  static char* handle_C_callback_stub()		{ return _handle_C_callback_stub; }
  static char* oopify_float()			{ return _oopify_float; }
  static char* alien_call_with_args_entry()     { return _alien_call_with_args_entry; }
  
  static char* PIC_stub_entry(int pic_size);	// PIC interpreter stubs: pic_size is the number of entries
  static char* allocate_entry(int size);	// allocation of memOops: size is words in addition to header
  static char* alien_call_entry(int args);      // alien call out       : args is the number of arguments passed to the function called

  // Support for profiling
  static bool contains(char* pc)                { return (_code <= pc) && (pc < &_code[_code_size]); }

  static void init();				// must be called in system initialization phase
  static void setSingleStepHandler(void (*fn)()) { single_step_fn = fn; }
};
