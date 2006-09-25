/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.22 $ */
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
# include "incls/_abort.cpp.incl"

// The following variables are used to do NLRs through C code
extern "C" bool       have_nlr_through_C;
extern "C" oop        nlr_result;
extern "C" int        nlr_home;
extern "C" int        nlr_home_id;
extern "C" contextOop nlr_home_context;

bool        have_nlr_through_C = false;
oop         nlr_result;
int         nlr_home;
int         nlr_home_id;
contextOop  nlr_home_context;

void ErrorHandler::abort_compilation() {
  Unimplemented();
}

extern "C" void provoke_nlr_at(int* frame_pointer, oop* stack_pointer);

void ErrorHandler::abort_current_process() {
  nlr_home    = 0;
  nlr_home_id = aborting_nlr_home_id();
  nlr_result  = smiOop_zero;
  provoke_nlr_at(DeltaProcess::active()->last_Delta_fp(), DeltaProcess::active()->last_Delta_sp());
  ShouldNotReachHere();
}

extern "C" void continue_nlr_in_delta(int* frame_pointer, oop* stack_pointer);

void ErrorHandler::continue_nlr_in_delta() {
  ::continue_nlr_in_delta(DeltaProcess::active()->last_Delta_fp(), DeltaProcess::active()->last_Delta_sp());
  ShouldNotReachHere();
}

void ErrorHandler::genesis() {
  Processes::kill_all();
  lookupCache::flush();
  Universe::flush_inline_caches_in_methods();
  Processes::start(new VMProcess);
}

