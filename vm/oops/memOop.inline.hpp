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

inline smi memOopDesc::identity_hash() {
  // don't clean up the addr()->_mark below to mark(), 
  // since hash_markOop can modify its argument
  return hash_markOop(addr()->_mark);
}

inline void memOopDesc::scavenge_header() { 
  // Not needed since klas is in old space
  //   scavenge_oop(klass_addr());
  // this may not be correct if running with the recompiler. Investigate. slr 29/09/2008
}

inline void memOopDesc::scavenge_body(int begin, int end) { 
  oop* p = (oop*) addr();
  oop* q = p + end;
  p     += begin;
  while (p < q) scavenge_oop(p++);
}

inline void memOopDesc::scavenge_tenured_body(int begin, int end) {
  oop* p = (oop*) addr();
  oop* q = p + end;
  p     += begin;
  while (p < q) scavenge_tenured_oop(p++);
}

inline void memOopDesc::follow_header() { 
  MarkSweep::reverse_and_push(klass_addr());
}

inline void memOopDesc::follow_body(int begin, int end) { 
  oop* p = (oop*) addr();
  oop* q = p + end;
  p     += begin;
  while (p < q) MarkSweep::reverse_and_push(p++);
}

inline void memOopDesc::layout_iterate_header(ObjectLayoutClosure* blk) {
  blk->do_mark(&addr()->_mark);
  blk->do_oop("klass", (oop*) &addr()->_klass_field);
}

inline void memOopDesc::oop_iterate_header(OopClosure* blk) {
  blk->do_oop((oop*) &addr()->_klass_field);
}

inline void memOopDesc::oop_iterate_body(OopClosure* blk,  int begin, int end) {
  oop* p = (oop*) addr();
  oop* q = p + end;
  p     += begin;
  while (p < q) blk->do_oop(p++); 
}

inline void memOopDesc::initialize_header(bool has_untagged_contents, klassOop klass) {
  set_klass_field(klass);
  if (has_untagged_contents)
    init_untagged_contents_mark();
  else
    init_mark();
}

inline void memOopDesc::initialize_body(int begin, int end) {
  oop value = nilObj;
  oop* p = (oop*) addr();
  oop* q = p + end;
  p     += begin;
  while (p < q) Universe::store(p++, value, false);
}

inline void memOopDesc::raw_at_put(int which, oop contents, bool cs) {
  Universe::store(oops(which), contents, cs); }

inline int memOopDesc::size() const {
  return blueprint()->oop_size(memOop(this));
}

inline int memOopDesc::scavenge_contents() {
 return blueprint()->oop_scavenge_contents(this);
}

inline int memOopDesc::scavenge_tenured_contents() {
  return blueprint()->oop_scavenge_tenured_contents(this);
}

// Store object size in age field and remembered set
inline void memOopDesc::gc_store_size() {
  int s = size();
  if (s < markOopDesc::max_age) {
    // store size in age field
    set_mark(mark()->set_age(s));
    assert(mark()->age() == s, "Check size");
  } else {
    // store size in remembered set
    // first set overflow value in age field
    set_mark(mark()->set_age(0));
    assert(mark()->age() == 0, "Check size");

    Universe::remembered_set->set_size(this, s);
    assert(Universe::remembered_set->get_size(this) == s, "Check size");
  }
}

// Retrieve object size from age field and remembered set       
inline int memOopDesc::gc_retrieve_size() {
  if (mark()->age() == 0)
    return Universe::remembered_set->get_size(this);
  return mark()->age();
}
