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

# include "incls/_precompiled.incl"
# include "incls/_callBack.cpp.incl"

void callBack::initialize(oop receiver, symbolOop selector) {
  assert(selector->is_symbol(), "must be symbol");
  Universe::set_callBack(receiver, selector);
}

static char* store_byte(char* chunk, char b) {
  *chunk = b;
  return chunk + sizeof(char);
}

static char* store_long(char* chunk, long l) {
  *((long*) chunk) = l;
  return chunk + sizeof(long);
}

void* callBack::registerPascalCall(int index, int nofArgs) {
  void* result = malloc(15);
  char* chunk = (char*) result;

  // MOV ECX, index
  chunk = store_byte(chunk, '\xB9');
  chunk = store_long(chunk, index);

  // MOV EDX, number of bytes for parameters
  chunk = store_byte(chunk, '\xBA');
  chunk = store_long(chunk, nofArgs * sizeof(int));

  // JMP _handleCCallStub
  chunk = store_byte(chunk, '\xE9');
  chunk = store_long(chunk, ((long)StubRoutines::handle_pascal_callback_stub()) - (4 + (long) chunk));

  return result;
}

void* callBack::registerCCall(int index) {
  void* result = malloc(10);
  char* chunk = (char*) result;

  // MOV ECX, index
  chunk = store_byte(chunk, '\xB9');
  chunk = store_long(chunk, index);

  // JMP _handleCCallStub
  chunk = store_byte(chunk, '\xE9');
  chunk = store_long(chunk, ((long)StubRoutines::handle_C_callback_stub()) - (4 + (long) chunk));

  return result;
}

void callBack::unregister(void* block) {
  assert(block, "block is not valid");
  free((char*) block);
}

// handleCallBack is called from the two assembly stubs:
// - handlePascalCallBackStub
// - handleCCallBackStub

typedef void* (__stdcall *call_out_func_4)(int a, int b, int c, int d);

extern "C" bool have_nlr_through_C;

extern "C" volatile void* handleCallBack(int index, int params) {
  DeltaProcess* proc = NULL;

  if (Universe::callBack_receiver()->is_nil()) {
    warning("callBack receiver is not set");
  }

  int low  = get_unsigned_bitfield(params,  0, 16);
  int high = get_unsigned_bitfield(params, 16, 16);

  if (DeltaProcess::active()->thread_id() != os::current_thread_id()) {
    // We'are now back in a asynchronous DLL call so give up the control
    // Fix this:
    //   remove warning when it has been tested
    proc = Processes::find_from_thread_id(os::current_thread_id());
    assert(proc, "process must be present");
    DLLs::exit_async_call(&proc);
  }
 

  oop res = Delta::call(Universe::callBack_receiver(),
                        Universe::callBack_selector(),
			as_smiOop(index),
                        as_smiOop(high),
                        as_smiOop(low));

  assert(DeltaProcess::active()->thread_id() == os::current_thread_id(), "check for process torch");

  void* result;

  // convert return result

  if (have_nlr_through_C) {
    // Continues the NLR after at the next Delta frame
    ErrorHandler::continue_nlr_in_delta();
  }

  if (res->is_smi()) {
    result = (void*) smiOop(res)->value();
  } else if (res->is_proxy()) {
    result = (void*) proxyOop(res)->get_pointer();
  } else {
    warning("Wrong return type for call back, returning NULL");
    result = NULL;
  }

  // Return value has to be converted before we transfer control to another 
  // thread.

  if (proc) {
    // We'are now back in a asynchronous DLL call so give up the control
    proc->transfer_and_continue();
  }

  // Call Delta level error routine
  return result;
}
