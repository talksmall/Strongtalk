/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.34 $ */
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
# include "incls/_memOop.cpp.incl"

void memOopDesc::layout_iterate_body(ObjectLayoutClosure* blk, int begin, int end) {
  oop* p = (oop*) addr();
  oop* q = p + end;
  p     += begin;
  while (p < q) {
    int offset = p - (oop*) addr();
    // Compute the instance variable name at the current offset
    symbolOop name = blueprint()->inst_var_name_at(offset);
    char* n = "instVar?";
    if (name) {
      stringStream st(50);
      name->print_symbol_on(&st);
      n = st.as_string();
    }
    blk->do_oop(n, p++);
  }
}

oop memOopDesc::scavenge() {
  assert((!Universe::should_scavenge(this)) == 
         (((char*) this > Universe::old_gen.low_boundary) || Universe::new_gen.to()->contains(this)),
	 "just checking");

  if (((char*) this > Universe::old_gen.low_boundary) || Universe::new_gen.to()->contains(this)) {
    return this;
  } else if (this->is_forwarded()) {
    return oop(this->forwardee());
  } else {
    return copy_to_survivor_space();
  }
}

void memOopDesc::follow_contents() {
  assert(is_gc_marked(), "pointer reversal should have taken place");
  // lprintf("[%s, 0x%lx, 0x%lx]", blueprint()->name(), this, klass());
  blueprint()->oop_follow_contents(this);
}

oop memOopDesc::copy_to_survivor_space() {
  int s = size();
  assert(Universe::should_scavenge(this) && !is_forwarded(), 
	 "shouldn't be scavenging"); 
  bool is_new;
  oop* x = Universe::allocate_in_survivor_space(this, s, is_new);

# ifdef VERBOSE_SCAVENGING
  lprintf("{copy %s %#lx -> %#lx (%d)}\n",
	  blueprint()->name(), oops(), x, s);
# endif

  memOop p= as_memOop(x);
  copy_oops(oops(), x, s);

  if (is_new) { 
    p->set_mark(p->mark()->incr_age()); 
    Universe::age_table->add(p, s); 
  } else {
  # ifdef VERBOSE_SCAVENGING
    lprintf("{tenuring %s %#lx -> %#lx (%d)}\n",
	    blueprint()->name(), oops(), x, s);
  # endif
  }
  forward_to(p); 
  return p;
}

void memOopDesc::oop_iterate(OopClosure* blk) {
  blueprint()->oop_oop_iterate(this, blk);
}

void memOopDesc::layout_iterate(ObjectLayoutClosure* blk) {
  blueprint()->oop_layout_iterate(this, blk);
}

bool memOopDesc::verify() {
  bool flag = true;
  if (flag) {
    markOop m = mark();
    if (! oop(m)->is_mark()) {
      error("mark of memOop %#lx isn't a markOop", this);
      if (! m->verify())
        error(" mark of memOop %#lx isn't even a legal oop", this);
      flag = false;
    }
    klassOop p = klass();
    if (! p->is_klass()) {
      error("map of memOop %#lx isn't a klassOop", this);
      flag = false;
    }
  }
  return flag;
}

void memOopDesc::set_identity_hash(smi h) {
  set_mark(mark()->set_hash(h));
}

void memOopDesc::bootstrap_header(bootstrap* st) {
  if (st->new_format()) {
    st->read_oop((oop*)&addr()->_klass_field);
    set_mark(blueprint()->has_untagged_contents() 
           ? markOopDesc::untagged_prototype()
	   : markOopDesc::tagged_prototype());
  } else {
    st->read_mark(&addr()->_mark);
    st->read_oop((oop*)&addr()->_klass_field);
  }
}

void memOopDesc::bootstrap_object(bootstrap* st) {
  bootstrap_header(st);
  bootstrap_body(st, header_size());
}

void memOopDesc::bootstrap_body(bootstrap* st, int h_size) {
  int offset = h_size;
  int s      = blueprint()->non_indexable_size();
  while (offset < s) {
    st->read_oop((oop*)addr() + offset);
    offset++;
  }
}

bool memOopDesc::is_within_instVar_bounds(int index) {
  return index >= blueprint()->oop_header_size()
      && index <  blueprint()->non_indexable_size();
}

oop memOopDesc::instVarAt(int index) {
  return raw_at(index );
}


oop memOopDesc::instVarAtPut(int index, oop value) {
  raw_at_put(index, value);
  return this;
}

void memOopDesc::print_on(outputStream* st) {
  blueprint()->oop_print_on(this, st);
}

void memOopDesc::print_id_on(outputStream* st) {
  int id;
  if (GCInProgress || !(id = objectIDTable::insert(memOop(this))))
    st->print("(%#-6lx)", addr());
  else
    st->print("%d",id);
}
