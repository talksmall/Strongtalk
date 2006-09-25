/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.17 $ */
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

// The following classes are C++ `closures` for iterating over
// Objects, Roots, and Frames.

// A Closure is used for iterating over data structures; use it instead
// of function pointers.
template <class T> class Closure : StackObj {
 public:
  virtual void do_it(T t) = 0;
};


// ObjectClosure is used for iterating through object space
// (see universe::object_iterate).
class ObjectClosure : StackObj {
 public:
  // Called when entering a space.
  virtual void begin_space(space* s) {}
  // Called when exiting a space.
  virtual void end_space(space* s)   {}
  // Called for each object.
  virtual void do_object(memOop obj) {}
};

// ObjectFilterClosure is an ObjectClosure with filtering.
class ObjectFilterClosure : public ObjectClosure {
  void do_object(memOop obj);
 public:
  // Called for each object and returns whether
  // do_filteres_objects should be called.
  virtual bool include_object(memOop obj) { return true; }
  // Called for each object where include_object returns true.
  virtual void do_filtered_object(memOop obj) {}
};

// ObjectLayoutClosure is a closure for iterating through the
// layout of a memOop (see memOop::layout_iterate).
class ObjectLayoutClosure : StackObj {
 public:
  // NON INDEXABLE PART
  // Called for the markOop
  virtual void do_mark(markOop *m) {}
  // Called for each oop
  virtual void do_oop(char* title, oop* o) {}
  // Called for each byte
  virtual void do_byte(char* title, u_char* b) {}
  // Called for each long
  virtual void do_long(char* title, void** p) {}
  // Called for each double
  virtual void do_double(char* title, double* d) {}

  // INDEXABLE PART
  // Called before iterating through the indexable part.
  // ONLY called if the object has an indexable part.
  virtual void begin_indexables() {}
  // Called after iterating through the indexable part.
  // ONLY called if the object has an indexable part.
  virtual void end_indexables() {}
  // Called for each indexable oop
  virtual void do_indexable_oop(int index, oop* o) {}
  // Called for each indexable byte
  virtual void do_indexable_byte(int index, u_char* b) {}
  // Called for each indexable double byte
  virtual void do_indexable_doubleByte(int index, doubleByte* b) {}
  // Called for each indexable long
  virtual void do_indexable_long(int index, long* l) {}
};

// A FrameClosure is used for iterating though frames
class FrameClosure : StackObj {
 public:
  // Called before iterating through a process.
  virtual void begin_process(Process* p) {}
  // Called after iterating through a process.
  virtual void end_process(Process* p) {}
  // Called for each frame
  virtual void do_frame(frame* f) {}
};

// A FrameLayoutClosure is used for iterating though the layout of frame
class FrameLayoutClosure : StackObj {
 public:
  // Called for each oop 
  virtual void do_stack(int index, oop* o) {}
  // Called for the hcode pointer
  virtual void do_hp(u_char** hp) {}
  // Called for the receiver 
  virtual void do_receiver(oop* o) {}
  // Called for the link 
  virtual void do_link(int** fp) {}
  // Called for the return address 
  virtual void do_return_addr(char** pc) {}
};

// A ProcessClosure is used for iterating over Delta processes
class ProcessClosure : StackObj {
 public:
  // Called for each process
  virtual void do_process(DeltaProcess* p) {}
};

// A OopClosure is used for iterating through oops
// (see memOop::oop_iterate, universe::root_iterate).
class OopClosure : StackObj {
 public:
  // Called for each oop
  virtual void do_oop(oop* o) {}
};

// A klassOopClosure is used for iterating through klassOops
// (see memOop::oop_iterate, universe::root_iterate).
class klassOopClosure : StackObj {
 public:
  // Called for each oop
  virtual void do_klass(klassOop klass) {}
};

