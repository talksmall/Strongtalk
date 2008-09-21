/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.32 $ */
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

# include <string.h>

# include "incls/_rSet.cpp.incl"

// values of bytes in byte map: during normal operation (incl. scavenge),
// bytes are either -1 (clean) or 0 (dirty); i.e., the interpreter / compiled code
// clear a byte when storing into that card.
// More precisely, the card being marked corrsponds to the object start, not
// necessarily to the word being updated (e.g., if the object starts towards the
// end of a card, the updated instance variable may be on the next card).  But for
// objArrays, the precise card is marked so that we don't need to scavenge the
// entire array.
// During GC, the bytes are used to remember object sizes, see comment further below

// To do list for the remembered set
//
// 1. Optimization suggested by Urs 9/30/95
//    Speed up card scanning by comparing words for frequent case
//    (A profile of Mark Sweep is necessary to make the call).
// 2. Handle objArrays in a more effficient way. Scavenge only
//    parts of the objArray with dirty cards.
//    The current implementation is REALLY slow for huge tenured objArrays
//    with few new pointers.

rSet::rSet() {
   low_boundary = Universe::new_gen.low_boundary;
  high_boundary = Universe::old_gen.high_boundary;
  clear(); 
  Set_Byte_Map_Base(byte_for(NULL));
  assert(byte_for(low_boundary) == byte_map, "Checking start of map");
}

void* rSet::operator new(size_t size) {
  assert((int(Universe::new_gen.low_boundary) & (card_size - 1)) == 0,
	 "new must start at card boundary");
  assert((int(Universe::old_gen.low_boundary) & (card_size - 1)) == 0,
	 "old must start at card boundary");
  assert((int(Universe::old_gen.high_boundary) & (card_size - 1)) == 0,
	 "old must end at card boundary");
  assert(card_size >= 512, "card_size must be at least 512");
  int bmsize =
    (Universe::old_gen.high_boundary - Universe::new_gen.low_boundary)
      / card_size;
  return AllocateHeap(size + bmsize, "rSet");
}

// copy the bits from an older, smaller bitmap, add area [start,end)
rSet::rSet(rSet *old, char *start, char *end) {
  ShouldNotReachHere();
  /*
   low_boundary= Universe::new_gen.low_boundary;
  high_boundary= Universe::old_gen.high_boundary;
  char *old_low=  old->low_boundary;
  char *old_high= old->high_boundary;
  Set_Byte_Map_Base(byte_for(NULL));
  memcpy(byte_for(old_low),
	 old->byte_for(old_low),
	 old->byte_for(old_high) - old->byte_for(old_low));
  clear(byte_for(start), byte_for(end));
  delete old;
  */
}  

char* rSet::scavenge_contents(oldSpace* sp, char* begin, char* limit) {
  // make sure we are staring with a dirty page
  assert(!*begin, "check for dirty page");

  // Find object at page start

  oop* s = oop_for(begin);

  // Return if we're at the end.
  if (s >= sp->top()) return begin + 1;

  s = sp->object_start(s);

  char* end = begin+1;

  oop* object_end = NULL;

  while (!*end && end < limit) {
    while (!*end && end < limit) end++;

    // We now have a string of dirty pages [begin..end[
    oop* e = min(oop_for(end), (oop*)sp->top());

    if (e < (oop*)sp->top()) {
      // Find the object crossing the last dirty page
      object_end = sp->object_start(e);
      if (object_end != e) {
        // object starts on page boundary
        int size = as_memOop(object_end)->size();
        object_end += size;
      }
      end = byte_for(object_end);
    }
  }

  // Clear the cards
  for (char* i = begin; i < end; i++) *i = -1;

  // Find the end
  oop* e = min(oop_for(end), (oop*)sp->top());

  while (s < e) {
    memOop m = as_memOop(s);
    int size = m->scavenge_tenured_contents();
    assert(size = m->size(), "just checking");
    s += size;
  }
  return end;
}

void rSet::scavenge_contents(oldSpace* sp) {
  char* current_byte = byte_for(sp->bottom());
  char* end_byte     = byte_for(sp->top());
  // set sentinel for scan (dirty page)
  *(end_byte + 1) = 0;

  // scan over clean pages
  while (*current_byte) current_byte++;

  while (current_byte <= end_byte) {
    // Pass the dirty page on to scavenge_contents
    current_byte = scavenge_contents(sp, current_byte, end_byte);

    // scan over clean pages
    while (*current_byte) current_byte++;
  }
}

void rSet::print_set_for_space(oldSpace* sp) {
  char* current_byte = byte_for(sp->bottom());
  char* end_byte     = byte_for(sp->top());
  lprintf("%s: [%#lx, %#lx]\n", sp->name(), current_byte, end_byte);
  while (current_byte <= end_byte) {
    if (*current_byte) {
      lprintf("_");
    } else {
      lprintf("*");
    }
    current_byte++;
  }
  lprintf("\n");
}

int rSet::number_of_dirty_pages_in(oldSpace* sp) {
  int count = 0;
  char* current_byte = byte_for(sp->bottom());
  char* end_byte     = byte_for(sp->top());
  while (current_byte <= end_byte) {
    if (!*current_byte) count++;
    current_byte++;
  }
  return count;
}

class CheckDirtyClosure : public OopClosure {
 public:
  bool is_dirty;

  void clear() {
    is_dirty = false; 
  }

  void do_oop(oop* o) {
    if ((*o)->is_new()) {
      is_dirty = true;
      /*
      { FlagSetting fs(PrintObjectID, false);
        std->print("0x%lx ", o);
        (*o)->print_value();
        std->cr();
      }
      */
    }
  }
};

bool rSet::has_page_dirty_objects(oldSpace* sp, char* page) {
  // Find object at page start
  oop* s = sp->object_start(oop_for(page));
  // Find the end
  oop* e = min(oop_for(page+1), (oop*)sp->top());

  CheckDirtyClosure blk;

  while (s < e) {
    memOop m = as_memOop(s);
    blk.clear();
    m->oop_iterate(&blk);
    if (blk.is_dirty) return true;
    s += m->size();
  }
  return false;
}

int rSet::number_of_pages_with_dirty_objects_in(oldSpace* sp) {
  int count = 0;
  char* current_byte = byte_for(sp->bottom());
  char* end_byte     = byte_for(sp->top());
  while (current_byte <= end_byte) {
    if (has_page_dirty_objects(sp, current_byte))
      count++;
    current_byte++;
  }
  return count;
}

void rSet::print_set_for_object(memOop obj) {
  std->print("Remember set for 0x%lx ", obj);
  obj->print_value();
  std->cr();
  if (obj->is_new()) {
    std->print_cr(" object is in new space!");
  } else {
    std->sp();
    char* current_byte = byte_for(obj->addr());
    char* end_byte     = byte_for(obj->addr() + obj->size());
    while (current_byte <= end_byte) {
      if (*current_byte) {
        std->print("_");
      } else {
        std->print("*");
      }
      current_byte++;
    }
    std->cr();
  }
}

bool rSet::is_object_dirty(memOop obj) {
  assert(!obj->is_new(), "just checking");
  char* current_byte = byte_for(obj->addr());
  char* end_byte     = byte_for(obj->addr() + obj->size());
  while (current_byte <= end_byte) {
    if (*current_byte == 0) return true;
    current_byte++;
  }
  return false;
}

void rSet::clear(char *start, char *end) {
  int* from  = (int*) start;
  int  count = (int*) end - from;
  set_words(from, count, AllBits);
}

bool rSet::verify(bool postScavenge) {
  return true;
}

// Scheme for storing the size of objects during pointer-reversal phase of GC.
// (Can't get object size via klass because pointers are reversed! :-)
// size must be >= 128.
// 1. byte     Size            Size
// [0..128[ -> [128..256[
// 128      -> 1 extra byte   [256      .. 256   + 2^8 [
// 129      -> 2 extra bytes  [512      .. 512   + 2^16[
// 130      -> 4 extra bytes  [66048    ..         2^32[

const int lim_0 =         markOopDesc::max_age;
const int lim_1 =         (1 <<  8);
const int lim_2 = lim_1 + (1 <<  8);
const int lim_3 = lim_2 + (1 << 16);

void rSet::set_size(memOop obj, int size) {
  unsigned char* p = (unsigned char*) byte_for(obj->addr());
  assert(size >= lim_0, "size must be >= max_age");
  if (size < lim_1) { 		// use 1 byte
    *p = (unsigned char) (size - lim_0);
  } else if (size < lim_2) { 	// use 1 + 1 bytes
    *p++ = lim_0 + 2;
    *p   = (unsigned char) (size - lim_1);
  } else if (size < lim_3) { 	// use 1 + 2 bytes
    *p++ = lim_0 + 3;
    *(unsigned short*)p = (unsigned short) (size - lim_2);
  } else { 			// use 1 + 4 bytes
    *p++ = lim_0 + 4;
    *(unsigned int*)p = (unsigned int) (size - lim_3);
  }
}

int rSet::get_size(memOop obj) {
  unsigned char* p = (unsigned char*) byte_for(obj->addr());
  unsigned char h = *p++;
  if (h <= lim_0 + 1) return h + lim_0;
  if (h == lim_0 + 2) return (*(unsigned char*)  p) + lim_1;
  if (h == lim_0 + 3) return (*(unsigned short*) p) + lim_2;
  if (h == lim_0 + 4) return (*(unsigned int*)   p) + lim_3;
  ShouldNotReachHere();
  return 0;
}

// new old space added; fix the cards
void rSet::fixup(char *start, char *end) {
  if (end > high_boundary) {
    Universe::remembered_set = new rSet(this, start, end);
  } else {
    clear(byte_for(start), byte_for(end));
  }
}
