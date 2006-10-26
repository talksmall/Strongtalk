/* Copyright 1996, LongView Technologies L.L.C. $Revision: 1.2 $ */
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
# include "incls/_systemAverage.cpp.incl"

char         SlidingSystemAverage::buffer[buffer_size];
unsigned int SlidingSystemAverage::stat[number_of_cases];
unsigned int SlidingSystemAverage::pos;

void SlidingSystemAverage::reset() {
  for (int index = 0; index < buffer_size; index++) {
    buffer[index] = nowhere;
  }
}

unsigned int* SlidingSystemAverage::update() {
  // clear the array;
  for (unsigned int index = 0; index < number_of_cases; index++) {
    stat[index] = 0;
  }

  index = pos;
  do {
    stat[buffer[index]]++;
    index = (index + 1) % buffer_size;
  } while (index != pos);
  return stat;
}

void SlidingSystemAverage::add(char type) {
  buffer[pos] = type;
  pos = (pos + 1) % buffer_size;
}

// The sweeper task is activated every second (1000 milliseconds).
class SystemAverageTask : public PeriodicTask {
 public:
  SystemAverageTask() : PeriodicTask(10) { }

  void task() {
    char type;
    if (last_Delta_fp) {
      if (theCompiler) { 
        type = SlidingSystemAverage::in_compiler;
      } else if (GCInProgress) {
        type = SlidingSystemAverage::in_garbage_collect; 
      } else if (DeltaProcess::is_idle()) {
        type = SlidingSystemAverage::is_idle;
      } else {
        type = SlidingSystemAverage::in_vm;
      }
    } else {
      // interpreted code / compiled code / runtime routine
      frame fr = DeltaProcess::active()->profile_top_frame();
      if (fr.is_interpreted_frame()) {
	type = SlidingSystemAverage::in_interpreted_code;
      } else if (fr.is_compiled_frame()) {
        type = SlidingSystemAverage::in_compiled_code;
      } else if (PIC::in_heap(fr.pc())) {
	type = SlidingSystemAverage::in_pic_code;
      } else if (StubRoutines::contains(fr.pc())) {
	type = SlidingSystemAverage::in_stub_code;
      }
    }
    SlidingSystemAverage::add(type);
  }
};

void systemAverage_init() {
  SlidingSystemAverage::reset();
  if (UseSlidingSystemAverage) {
    (new SystemAverageTask)->enroll();
  }
}