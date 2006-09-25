/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.6 $ */
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


// a simple flat profiler for Delta

class pnode;
class FlatProfilerTask;

enum TickPosition {
  in_code,
  in_primitive,
  in_compiler,
  in_pic,
  other
};

class FlatProfiler: AllStatic {
 private:
  static pnode**  table;
  static int table_size;

  static DeltaProcess*     proc;
  static FlatProfilerTask* task;
  static timer             time;

  static int          gc_ticks;		// total ticks in GC/scavenge
  static int   semaphore_ticks;
  static int        stub_ticks;
  static int    compiler_ticks;		// total ticks in compilation
  static int     unknown_ticks;

  static int total_ticks() {
    return gc_ticks
         + semaphore_ticks
         + stub_ticks
         + unknown_ticks;
  }

  friend class FlatProfilerTask;

  static void interpreted_update(methodOop method, klassOop klass, TickPosition where);
  static void compiled_update(nmethod* nm, TickPosition where);
  static int entry(int value);

  static void record_tick_for_running_frame(frame fr);
  static void record_tick_for_calling_frame(frame fr);

 public:
  static void allocate_table();

  static void reset();
  static void engage(DeltaProcess* p);
  static DeltaProcess* disengage();
  static bool is_active();
  static void print(int cutoff);
  static void record_tick();

  static DeltaProcess* process() { return proc; }
};
