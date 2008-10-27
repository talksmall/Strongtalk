/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.20 $ */
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

// A generation is a bunch of spaces of similarly-aged objects

class generation: ValueObj {
  friend class rSet;
  friend class Universe;
  friend class MarkSweep;
  friend class memOopDesc;
  friend class byteArrayOopDesc;
  friend class oldGeneration;

 protected:
  // Minimum and maximum addresses, used by card marking code.
  // Must not overlap with address ranges of other generation(s).
  char *low_boundary;
  char *high_boundary;
  VirtualSpace virtual_space;

 public:
  // space enquiries
  virtual int capacity() = 0;
  virtual int used() = 0;
  virtual int free() = 0;

  void print();
};


class newGeneration: public generation {
  friend class rSet;
  friend class Universe;
  friend class MarkSweep;
  friend class OopNCode;

 private:
  edenSpace eden_space;
  survivorSpace* from_space;
  survivorSpace* to_space;

 public:
  edenSpace*      eden() { return &eden_space; }
  survivorSpace*  from() { return from_space;  }
  survivorSpace*  to()   { return to_space;    }

  // space enquiries
  int capacity();
  int used();
  int free();

  void print();

  void object_iterate(ObjectClosure* blk);

  void verify();
  bool would_fit(int size) { return to_space->would_fit(size); }

  void swap_spaces();

  bool contains(void* p) {
    return (char*)p >= low_boundary && (char*)p < high_boundary; }

  oop* object_start(oop* p);

  oop* allocate(int size) { return eden()->allocate(size); }
  oop* allocate_in_survivor_space(int size) {
    return to_space->allocate(size); }

  char *boundary() { return high_boundary; }
 protected:
  inline bool is_new(memOop p, char *boundary); // inlined in generation.dcl.h
  inline bool is_new(oop p,    char *boundary); // ditto

 private:
  // called by Universe
  void initialize(ReservedSpace rs, int eden_size, int surv_size);

  // phase2 of mark sweep
  void prepare_for_compaction(OldWaterMark* mark);
  // phase3 of mark sweep
  void compact(OldWaterMark* mark);

  void switch_pointers(oop from, oop to);
};


class oldGeneration: public generation {
  friend class rSet;
  friend class Universe;
  friend class MarkSweep;
  friend class oldSpace;
  friend class symbolKlass;

  oop* allocate_in_next_space(int size);

 private:
  // OldGeneration consists of a linked lists of spaces.
  // [ ] -> [ ] -> [ ] -> [ ] -> [ ] 
  //  ^             ^             ^
  // first         current       last

  oldSpace *first_space;
  oldSpace *current_space;
  oldSpace *last_space;

 public:
  int expand(int size);
  int shrink(int size);

  oop* allocate(int size, bool allow_expansion=true) {
    return current_space->allocate(size, allow_expansion);
  }

  // called by Universe
  void initialize(ReservedSpace rs, int initial_size);

 public:
  // space enquiries
  int capacity();
  int used();
  int free();

  void print();
  void print_remembered_set();

  // Returns the number of dirty pages in old space.
  // ie. # of pages marked as dirty
  int number_of_dirty_pages();

  // Returns the number of pages with dirty objects
  // ie. # of pages with object pointing to new objects.
  int number_of_pages_with_dirty_objects();

  void object_iterate(ObjectClosure* blk);
  void object_iterate_from(OldWaterMark* mark, ObjectClosure* blk);

  void verify();

  bool contains(void* p);
  oop* object_start(oop* p);

  OldWaterMark top_mark()    { return current_space->top_mark(); }
  OldWaterMark bottom_mark() { return first_space->bottom_mark(); }
  OldSpaceMark memo() { return OldSpaceMark(current_space); }
 private:
  void scavenge_contents_from(OldWaterMark* mark);

  void switch_pointers(oop from, oop to);
  void switch_pointers_by_card(oop from, oop to);

  void sorted_space_list(oldSpace *sp[], int (*cmp)(oldSpace**, oldSpace**));

  // phase2 of mark sweep
  void prepare_for_compaction(OldWaterMark* mark);
  // phase3 of mark sweep
  void compact(OldWaterMark* mark);

  void append_space(oldSpace *last);
};

// ensure that you surround the call with {} to prevent s leaking out!
#define FOR_EACH_OLD_SPACE(s) \
  for (oldSpace *s= Universe::old_gen.first_space;               \
       s != NULL;                                               \
       s= s->next_space)



