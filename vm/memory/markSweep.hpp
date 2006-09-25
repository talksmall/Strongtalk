/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.1 $ */
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

// MarkSweep takes care of garbage collection
class MarkSweep : AllStatic {
 public:
  static oop collect(oop p = NULL);

  // Call backs
  static void follow_root(oop* p);
  static void reverse_and_push(oop* p);
  static void reverse_and_follow(oop* p);

  static void add_hcode_offset(int offset);
  static int  next_hcode_offset();
 private:
  // the traversal stack used during phase1.
  static GrowableArray<memOop>* stack;
  // the hcode pointer offsets saved before and
  // and retrieved after the garbage collection.
  static GrowableArray<int>* hcode_offsets;
  static int                 hcode_pos;

 private:
  static void mark_sweep_phase1(oop* p);
  static void mark_sweep_phase2();
  static void mark_sweep_phase3();

  static inline memOop reverse(oop* p);

  static void allocate();
  static void deallocate();
  static void trace(char* msg);
};
