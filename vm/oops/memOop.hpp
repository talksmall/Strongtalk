/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.38 $ */
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

// memOops are all oops that actually take up space in the heap (i.e.,
// aren't immediates like smis)

// Again (see oop.hpp), memOops are tagged (ST oops) but memOopDesc*s are
// untagged C pointers.  

// should clean up order of member functions  -Urs

class memOopDesc: public oopDesc {
 protected:
  // instance variable
  // markOop _mark;			// see comment in oop.hpp
  klassOop _klass_field;		// the receiver's class
  
 public:
  // returns the header size of a memOop
  static int header_size()		{ return sizeof(memOopDesc)/oopSize; }

  // field offsets for code generation
  static int mark_byte_offset()		{ return (0 * oopSize) - Mem_Tag; }
  static int klass_byte_offset()	{ return (1 * oopSize) - Mem_Tag; }

  // coercions
  friend memOop as_memOop(void* p);

  // conversion from memOop to memOopDesc*
  memOopDesc* addr() const		{ return (memOopDesc*) (int(this) - Mem_Tag); }
  
  // space operations, is_old/new work w/o conversion to memOopDesc*
  // since oop > pointer (Mem_Tag >= 0)!
  bool is_old() const			{ return (char*)this >= Universe::old_gen.low_boundary;  }
  bool is_new() const			{ return (char*)this <  Universe::new_gen.high_boundary; }

  // mark accessors
  markOop mark() const			{ return addr()->_mark; }

  void set_mark(markOop m)		{ addr()->_mark = m; }
  void set_mark(memOop p)		{ set_mark(markOop(p)); }
  void set_mark(oop* p)			{ set_mark(markOop(p)); }

  oop* klass_addr() const		{ return (oop*) &addr()->_klass_field; }

  void set_klass_field(klassOop k, bool cs = true) {
    // %optimization
    //   since klasses are tenured the store check can be avoided
    addr()->_klass_field = k;
  }

  klassOop klass_field() const		{ return addr()->_klass_field; }

  Klass* blueprint() const {
    // %include-conflict "return klass_field()->klass_part();"
    // To avoid include problems the above code has be translated into:
    return (Klass*) (((char*) klass_field()) + sizeof(memOopDesc) - Mem_Tag);
  }

  // mark operations
  
  // use this after a copy to get a new mark
  void init_mark() { 
    set_mark(markOopDesc::tagged_prototype());
  }
  void init_untagged_contents_mark() {
    set_mark(markOopDesc::untagged_prototype());
  }

  void mark_as_dying() { 
    set_mark(mark()->set_near_death());
  }

  // Notification queue support
  bool is_queued() const		{ return mark()->is_queued();       }
  void set_queued()			{ set_mark(mark()->set_queued());   }
  void clear_queued()			{ set_mark(mark()->clear_queued()); }

  // mark operations
  inline smi identity_hash();		// inlined in memOop.inline.hpp
  void set_identity_hash(smi);

  // memory operations
  oop scavenge();
  void follow_contents();

  // scavenge the header part (see oop_scavenge_contents)
  inline void scavenge_header();
  // scavenge the body [begin..[end
  inline void scavenge_body(int begin, int end);

  inline void scavenge_tenured_header()	{}
  inline void scavenge_tenured_body(int begin, int end);

  // Scavenge all pointers in this object and return the oop size
  inline int scavenge_contents();
  // Scavenge all pointers in this object and return the oop size
  // has_new_pointers reports if the object has pointers to new space.
  inline int scavenge_tenured_contents();

  // Copy this object to survivor space and return the new address
  // (called by scavenge)
  oop copy_to_survivor_space();

  // MarkSweep support
  inline void follow_header();
  inline void follow_body(int begin, int end);

  // support for iterating through all oops of an object (see oop_oop_iterate).
  inline void oop_iterate_header(OopClosure* blk);
  inline void oop_iterate_body(OopClosure* blk, int begin, int end);

  // support for iterate the layout of an object (see oop_layout_iterate).
  inline void layout_iterate_header(ObjectLayoutClosure* blk);
         void layout_iterate_body(ObjectLayoutClosure* blk, int begin, int end);

  // support for initializing objects (see allocateObject[Size]).
  inline void initialize_header(bool has_untagged, klassOop klass);
  inline void initialize_body(int begin, int end);

  bool verify();

  // forwarding operations
  bool is_forwarded()			{ return mark()->is_mem(); }
  void forward_to(memOop p) {
    assert(p->is_mem(), "forwarding to something that's not a memOop");
    set_mark(p);
  }
  memOop forwardee()			{ return memOop(mark()); }
  
  // marking operations
  bool is_gc_marked()			{ return !mark()->has_sentinel(); } // Changed from mark()->is_smi(), Lars

  // GC operations (see discussion in universe_more.cpp for rational)
  void gc_store_size();			// Store object size in age field and remembered set
  int  gc_retrieve_size();		// Retrieve object size from age field and remembered set

  // accessors
  oop* oops(int which = 0)		{ return &((oop*) addr())[which]; }
  
  oop raw_at(int which)			{ return *oops(which); }
  inline void raw_at_put(int which, oop contents, bool cs = true);

  // accessing instance variables 
  bool is_within_instVar_bounds(int index);
  oop instVarAt(int index);
  oop instVarAtPut(int index, oop value);
  
  // iterators
  void oop_iterate(OopClosure* blk);   
  void layout_iterate(ObjectLayoutClosure* blk);

  // Returns the oop size of this object
  int size() const;
  
  // printing operation
  void print_id_on(outputStream* st);
  void print_on(outputStream* st);

  // bootstrapping operations
  void bootstrap_object(bootstrap* st);
  void bootstrap_header(bootstrap* st);
  void bootstrap_body(bootstrap* st, int h_size);
  
  friend memOopKlass;
};

inline memOop as_memOop(void* p)
{ 
    assert((int(p) & Tag_Mask) == 0, "not an aligned C pointer");
    return memOop(int(p) + Mem_Tag);
}
