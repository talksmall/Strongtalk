/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.25 $ */
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
# include "incls/_generation.cpp.incl"

void generation::print() {
  std->print(" total %6dK, %d%% used ", capacity()/K, (100 * used())/capacity());
  std->print_cr(" [%#x, %#x[", low_boundary, high_boundary);
}

void newGeneration::swap_spaces() {
  eden()->clear();
  from()->clear();
  {
    survivorSpace* s = from();
    from_space = to();
    to_space    = s;
  }
  eden()->next_space = from();

  from()->set_name("from");
  from()->next_space = to();

  to()->set_name("to");
  to()->next_space = NULL;
}

void newGeneration::initialize(ReservedSpace rs, int eden_size, int surv_size) {  
  int new_size = eden_size + surv_size + surv_size;

  virtual_space.initialize(rs, rs.size());

  char *eden_start = virtual_space.low();
  char *from_start = eden_start + eden_size;
  char *to_start   = from_start + surv_size;
  char *to_end     = to_start   + surv_size;

  from_space= new survivorSpace();
  to_space  = new survivorSpace();

  eden()->initialize("eden", (oop*) eden_start, (oop*)from_start);
  from()->initialize("from", (oop*) from_start, (oop*) to_start);
    to()->initialize("to",   (oop*) to_start,   (oop*) to_end);

  eden()->next_space = from();
  from()->next_space =   to();
    to()->next_space =   NULL;

   low_boundary= virtual_space.low_boundary();
  high_boundary= virtual_space.high_boundary();
}

void newGeneration::prepare_for_compaction(OldWaterMark* mark) {
  // %note same order as in compact
  from()->prepare_for_compaction(mark);
  eden()->prepare_for_compaction(mark);
}

void newGeneration::compact(OldWaterMark* mark) {
  // %note same order as in prepare_for_compaction
  from()->compact(mark);
  from()->clear();
  eden()->compact(mark);
  eden()->clear();
}

oop* newGeneration::object_start(oop* p) {
  if (eden()->contains(p)) return eden()->object_start(p);
  return from()->object_start(p);
}

int newGeneration::capacity() {
  return eden()->capacity()
       + from()->capacity()
       +   to()->capacity();
}

int newGeneration::used() {
  return eden()->used()
       + from()->used()
       +   to()->used();
}

int newGeneration::free() {
  return eden()->free()
       + from()->free()
       +   to()->free();
}

void newGeneration::switch_pointers(oop f, oop t) {
  eden()->switch_pointers(f, t);
  from()->switch_pointers(f, t);
    to()->switch_pointers(f, t);
}

void newGeneration::print() {
  if (WizardMode) {
    std->print_cr(" New generation");
    generation::print();
  }
  eden()->print();
  from()->print();
  if (WizardMode) {
    to()->print();
  }
}

void newGeneration::object_iterate(ObjectClosure* blk) {
  eden()->object_iterate(blk);
  from()->object_iterate(blk);
}

void newGeneration::verify() {
  if (   eden()->next_space != from_space
      || from()->next_space !=   to_space
      || to()->next_space != NULL)
    error("misconnnected spaces in new gen");

  eden()->verify();
  from()->verify();
    to()->verify();
}

# undef FOR_EACH_OLD_SPACE
// this version used with old_gen
// ensure that you surround the call with {} to prevent s leaking out!
#define FOR_EACH_OLD_SPACE(s) 		\
  for (oldSpace *s= first_space;	\
       s != NULL;			\
       s= s->next_space)

void oldGeneration::initialize(ReservedSpace rs, int initial_size) {

  virtual_space.initialize(rs, initial_size);

  first_space = new oldSpace("old", initial_size);
  last_space  = current_space = first_space;

   low_boundary= virtual_space.low_boundary();
  high_boundary= virtual_space.high_boundary();
}

bool oldGeneration::contains(void* p) {
  FOR_EACH_OLD_SPACE(s) { if (s->contains(p)) return true; }
  return false;
}

int oldGeneration::capacity(){
  int sum= 0;
  FOR_EACH_OLD_SPACE(s) sum += s->capacity();
  return sum;
}

int oldGeneration::used(){
  int sum= 0;
  FOR_EACH_OLD_SPACE(s) sum += s->used();
  return sum;
}

int oldGeneration::free(){
  int sum= 0;
  FOR_EACH_OLD_SPACE(s) sum += s->free();
  return sum;
}

void oldGeneration::scavenge_contents_from(OldWaterMark* mark) {
  mark->_space->scavenge_contents_from(mark);
  while (mark->_space != current_space) {
    *mark = mark->_space->next_space->bottom_mark();
    mark->_space->scavenge_contents_from(mark);
  }
}

void oldGeneration::switch_pointers(oop from, oop to) {
 FOR_EACH_OLD_SPACE(space) space->switch_pointers(from, to);
}
    
int oldGeneration::expand(int size) {
  return first_space->expand(size);
}

void oldGeneration::prepare_for_compaction(OldWaterMark* mark) {
  // %note same order as in compact
  FOR_EACH_OLD_SPACE(s) s->prepare_for_compaction(mark);
}

void oldGeneration::compact(OldWaterMark* mark) {
  // %note same order as in prepare_for_compaction
  FOR_EACH_OLD_SPACE(s) s->compact(mark);
}

void oldGeneration::append_space(oldSpace *last) {
  last_space->next_space= last;
  last_space= last;
  last->next_space= NULL;
}  

oop* oldGeneration::allocate_in_next_space(int size) {
  // Scavenge breaks the there is more than one old space chunks
  // Fix this with VirtualSpace
  // 4/5/96 Lars
  warning("Second old space chunk allocated, this could mean trouble");
  if (current_space == last_space) {
    int space_size = current_space->capacity();
    oldSpace *s = new oldSpace("old", space_size);

    if ((char*) s->bottom() < Universe::new_gen.high_boundary)
      fatal("allocation of old space before new space");
        
    append_space(s);
  }
  current_space = current_space->next_space;

  char* sStart= (char*) current_space->bottom();
  char* sEnd  = (char*) current_space->end();
  if (sStart <  low_boundary)  low_boundary= sStart;
  if (sEnd   > high_boundary) high_boundary= sEnd;
  Universe::remembered_set->fixup(sStart, sEnd);
  Universe::current_sizes.old_size = capacity();

  return allocate(size);
}

void oldGeneration::print() {
  if (WizardMode) {
    std->print_cr(" Old generation");
    generation::print();
  }
  FOR_EACH_OLD_SPACE(s) s->print();
}

void oldGeneration::print_remembered_set() {
  lprintf("Remembered set\n");
  FOR_EACH_OLD_SPACE(s) Universe::remembered_set->print_set_for_space(s);
}

int oldGeneration::number_of_dirty_pages() {
  int count = 0;
  FOR_EACH_OLD_SPACE(s) {
    count += Universe::remembered_set->number_of_dirty_pages_in(s);
  }
  return count;
}

int oldGeneration::number_of_pages_with_dirty_objects() {
  int count = 0;
  FOR_EACH_OLD_SPACE(s) {
    count += Universe::remembered_set->number_of_pages_with_dirty_objects_in(s);
  }
  return count;
}

void oldGeneration::object_iterate(ObjectClosure* blk) {
  FOR_EACH_OLD_SPACE(s) s->object_iterate(blk);
}

void oldGeneration::object_iterate_from(OldWaterMark* mark, ObjectClosure* blk) {
  mark->_space->object_iterate_from(mark, blk);
  for (oldSpace *s = mark->_space->next_space; s != NULL; s = s->next_space) {
    *mark = s->bottom_mark();
    s->object_iterate_from(mark, blk);
  }
}

void oldGeneration::verify() {
  int n= 0;
  oldSpace *p;
  FOR_EACH_OLD_SPACE(s) {
    n++;
    p= s;
  }
  if (p != last_space) error("Wrong last_space in old gen");
  APPLY_TO_OLD_SPACES(SPACE_VERIFY_TEMPLATE);
}

static int addr_cmp(oldSpace **s1, oldSpace **s2) {
  char* s1start= (char*) (*s1)->bottom();
  char* s2start= (char*) (*s2)->bottom();
  if (s1start < s2start) return -1;
  else if (s1start > s2start) return 1;
  else return 0;
}

oop* oldGeneration::object_start(oop* p) {
  FOR_EACH_OLD_SPACE(s) {
    if (s->contains(p)) return s->object_start(p);
  }
  return NULL;
}



