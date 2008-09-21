/* Copyright 1994 - 1996, LongView Technologies, L.L.C. $Revision: 1.49 $ */
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

class Interpreter: AllStatic {
 private:
  static bool  _is_initialized;		// true if Interpreter has been initialized
  static char* _code_begin_addr;	// the first byte of the interpreter's code
  static char* _code_end_addr;		// the first byte after the interpreter's code
  static int*  _invocation_counter_addr;// the address of the invocation counter (used in method entry code)

  // entry points
  static char* _redo_send_entry;	// entry point to redo an interpreted send that called a zombie nmethod

  // deoptimized returns
  static char* _dr_from_send_without_receiver;
  static char* _dr_from_send_without_receiver_restore;
  
  static char* _dr_from_send_without_receiver_pop;
  static char* _dr_from_send_without_receiver_pop_restore;
  
  static char* _dr_from_send_with_receiver;
  static char* _dr_from_send_with_receiver_restore;
  
  static char* _dr_from_send_with_receiver_pop;
  static char* _dr_from_send_with_receiver_pop_restore;
  
  static char* _dr_from_primitive_call_without_failure_block;
  static char* _dr_from_primitive_call_without_failure_block_restore;
  
  static char* _dr_from_primitive_call_with_failure_block;
  static char* _dr_from_primitive_call_with_failure_block_restore;
  
  static char* _dr_from_dll_call;
  static char* _dr_from_dll_call_restore;

  static char* _restart_primitiveValue;
  //static char* _nlr_single_step_continuation;
  static Label _nlr_single_step_continuation; // used by single step stub routine
  static char* _redo_bytecode_after_deoptimization;
  static char* _illegal;

  // Run-time routines
  static void trace_bytecode();
  static void warning_illegal(int ebx, int esi);
  static void wrong_eax();		// called in debug mode only
  static void wrong_esp();		// called in debug mode only

  // Floats
  static doubleOop oopify_FloatValue();

 public:
  // Note: The variable below has been introduced for debugging only: It seems that sometimes a nmethod
  //       is called from the interpreter (last time it was via a megamorphic self send) that is invalid,
  //       i.e., the nmethod reports a cache miss by calling the lookup routines. In order to backtrack
  //       called entry point is stored. Remove this variable if the bug has been found! (gri 7-24-96)
  static char* _last_native_called;	// last nmethod entry point called by the interpreter

  static bool contains(char* pc);	// true if pc is within interpreter code; false otherwise

  // Properties of the interpreter code (depends on defines in the assembler code)
  static bool is_optimized();
  static bool can_trace_bytecodes();
  static bool can_trace_sends();
  static bool has_assertions();
  static bool has_stack_checks();
  static bool has_timers();

  static void print_code_status();

  // Loops
  static void loop_counter_overflow();	// this routine gets called when the loop counter overflows
  static int  loop_counter();		// the number of loop iterations since the last reset
  static void reset_loop_counter();	// resets the loop counter to 0
  static int  loop_counter_limit();	// the loop counter limit
  static void set_loop_counter_limit(int limit);

  static int* loop_counter_addr();
  static int* loop_counter_limit_addr();

  // Invocation counters
  static void set_invocation_counter_limit(int new_limit);  // set invocation limit
  static int  get_invocation_counter_limit();		    // return invocation limit

  // entry points accessors
  static char* access(char* entry_point);
    
  static char* redo_send_entry();
  static char* restart_primitiveValue();
  static Label& nlr_single_step_continuation();
  static char* redo_bytecode_after_deoptimization();
  static char* illegal();

  static char* deoptimized_return_from_send_without_receiver();
  static char* deoptimized_return_from_send_without_receiver_restore();
  static char* deoptimized_return_from_send_without_receiver_pop();
  static char* deoptimized_return_from_send_without_receiver_pop_restore();
  static char* deoptimized_return_from_send_with_receiver();
  static char* deoptimized_return_from_send_with_receiver_restore();
  static char* deoptimized_return_from_send_with_receiver_pop();
  static char* deoptimized_return_from_send_with_receiver_pop_restore();
  static char* deoptimized_return_from_primitive_call_without_failure_block();
  static char* deoptimized_return_from_primitive_call_without_failure_block_restore();
  static char* deoptimized_return_from_primitive_call_with_failure_block();
  static char* deoptimized_return_from_primitive_call_with_failure_block_restore();
  static char* deoptimized_return_from_dll_call();
  static char* deoptimized_return_from_dll_call_restore();

  // Initialization
  static bool is_initialized()		{ return _is_initialized; }
  static void init();

  friend class InterpreterGenerator;
};
