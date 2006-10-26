/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.14 $ */
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


// remembered set for GC, implemented as a card marking byte array with one byte / card
// Card size is 512 bytes
// NB: Card size must be >= 512 because of the offset_array in oldspace

const int card_shift = 9; 			// wired in to scavenge_contents
const int card_size  = 1 << card_shift;
const int card_size_in_oops = card_size / oopSize;

class rSet: public CHeapObj {
  friend class oldSpace;
  friend class oldGeneration;
  friend class SetOopClosure;

 private:
  char*    low_boundary; 	// duplicate of old_gen var so byte_for can be inlined
  char*    high_boundary;
  char     byte_map[1];  	// size is a lie
  
  // friend void oldSpace::switch_pointers_by_card(oop, oop);
  char* byte_for(void *p) const { return (char*)&byte_map[int((char*)p - low_boundary) >> card_shift]; }
  oop*  oop_for(char* p) const 	{ return (oop*)(low_boundary  +  ((p - byte_map) <<  card_shift)); }

  friend oop*  card_for(oop* p) { return (oop*)(int(p) & ~(card_size - 1)); }
  
  inline char* byte_map_end() const;
 
 public:
  int byte_map_size() const 	{ return (high_boundary - low_boundary) / card_size; }
  rSet();
  void* operator new(size_t size);
  
  inline void clear();
  char* byte_map_base() const	{ return byte_for(NULL); }
  void record_store(void* p)    { *byte_for(p) = 0; }
  bool is_dirty(void* p) const	{ return *byte_for(p) == 0; }

  // Tells is any card for obj is dirty
  bool is_object_dirty(memOop obj);

  void  scavenge_contents(oldSpace* s);
  char* scavenge_contents(oldSpace* s, char* begin, char* limit);
  bool verify(bool postScavenge);

  void print_set_for_space(oldSpace* sp);
  void print_set_for_object(memOop obj);

  // Returns the number of dirty pages in an old segment
  int number_of_dirty_pages_in(oldSpace* sp);

  // Return the number of pages with dirty objects.
  int number_of_pages_with_dirty_objects_in(oldSpace* sp);

  // Operations used during garbage collection
  void set_size(memOop obj, int size);
  int  get_size(memOop obj);

 private:
  void fixup(char *start, char *end);
  void clear(char *start, char *end);
  rSet(rSet *old, char *start, char *end);
  bool has_page_dirty_objects(oldSpace* sp, char* page);
};
