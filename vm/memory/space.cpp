/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.43 $ */
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
# include "incls/_space.cpp.incl"

oop* OldWaterMark::pseudo_allocate(int size) {
  oop* p = _point;
  if (p + size  < _space->end()) {
    _point = p + size;
  } else {
    lprintf("crossing space\n");
    fatal("not implemented yet");
  }
  return p;
}

void space::clear() {
  set_top(bottom());
# ifdef ASSERT 
  // to detect scavenging bugs
  set_oops(bottom(), capacity()/oopSize, oop(1));
# endif
}

void space::switch_pointers(oop from, oop to) {
  // FIX LATER
  fatal("not implemented yet");
}

void space::initialize(char* name, oop* bottom, oop* end) {
  assert(Universe::on_page_boundary(bottom) && Universe::on_page_boundary(end), "invalid space boundaries");

  set_name(name);
  set_bottom(bottom);
  set_top(bottom);
  set_end(end);
}

void space::prepare_for_compaction(OldWaterMark* mark) {
  // compute the new addresses for the live objects and update all
  // pointers to these objects.
  // Used by universe::mark_sweep_phase2()
  // %profiling note:
  //    the garbage collectior spends 55% of its time in this function
  oop* q          = bottom();
  oop* t          = top();
  oop* new_top    = mark->_point;
  memOop first_free = NULL;
  while (q < t) {
    memOop m = as_memOop(q);
    if (m->is_gc_marked()) {
      if (first_free) { 
        first_free->set_mark(q);
        // lprintf("[%#lx] = %#lx, %#lx\n", first_free, first_free->mark(), q);
        first_free = NULL;
      }

      // Reverse the list with the mark at the end
      oop* root_or_mark = (oop*) m->mark();
      while (is_oop_root(root_or_mark)) {
        oop* next = (oop*) *root_or_mark;
        *root_or_mark = (oop) as_memOop(new_top);
        root_or_mark = next;
      }
      m->set_mark(markOop(root_or_mark));

      int size = m->gc_retrieve_size(); // The mark has to be restored before 
      // the size is retrieved
      new_top += size;
      q       += size;
    } else {
      if (!first_free) {
        first_free = m;
        // lprintf("First free %#lx\n", q);
      }
      q += m->size();
    }
  }
  if (first_free) { 
    first_free->set_mark(q);
    // lprintf("[%#lx] = %#lx, %#lx\n", first_free, first_free->mark(), q);
  }
  mark->_point = new_top;
}

void space::compact(OldWaterMark* mark) {
  // compute the new addresses for the live objects
  // Used by universe::mark_sweep_phase3()
  // %profiling note:
  //    the garbage collectior spends 23% of its time in this function
  oop* q       = bottom();
  oop* t       = top();
  oop* new_top = mark->_point;
  while (q < t) {
    memOop m = as_memOop(q);
    if (m->mark()->is_smi()) {
      // lprintf("Expanding %#lx -> %#lx\n", q, *q);
      q = (oop*) *q;
    } else {
      int size = m->gc_retrieve_size();
      if (q != new_top) {
        copy_oops(q, new_top, size);
        // lprintf("copy %#lx -> %#lx (%d)\n", q, new_top, size);
        assert((*new_top)->is_mark(), "should be header");
      }
      mark->_space->update_offsets(new_top, new_top + size);
      q       += size;
      new_top += size;
    }
  }
  mark->_point = new_top;
  mark->_space->set_top(new_top);
  set_top(new_top);
}

oop* newSpace::object_start(oop* p) {
  assert (bottom() <= p && p < top(), "p must be in space");
  oop* q = bottom();
  oop* t = top();
  while (q < t) {
    oop* prev = q;
    q += as_memOop(q)->size();
    if (q > p) return prev;
  }
  fatal("should never reach this point");
  return NULL;
}

void newSpace::object_iterate_from(NewWaterMark* mark, ObjectClosure* blk) {
  blk->begin_space(this);
  oop* p = mark->_point;
  oop* t = top();
  while (p < t) {
    memOop m = as_memOop(p);
    blk->do_object(m);
    p += m->size();
  }
  mark->_point = p;
  blk->end_space(this);
}

edenSpace::edenSpace() {}
survivorSpace::survivorSpace() {}

void survivorSpace::scavenge_contents_from(NewWaterMark* mark) {
# ifdef VERBOSE_SCAVENGING  
  lprintf("{scavenge_contents [ %#lx <= %#lx <= %#lx]}\n",
    bottom(), mark->_point, top());
# endif

  if (top() == mark->_point) return;
  assert(mark->_point < top(), "scavenging past top");

  oop* p = mark->_point; // for performance
  oop* t = top();

  do {
    memOop m = as_memOop(p);

# ifdef VERBOSE_SCAVENGING  
    lprintf("{scavenge %#lx (%#lx)} ", p, m->klass());
    lprintf("%s\n", m->klass()->name());
    oop *prev = p;
# endif

    assert((*p)->is_mark(), "Header should be mark");
    p += m->scavenge_contents();

# ifdef VERBOSE_SCAVENGING  
    if (p - prev != m->size())
      fatal("scavenge_contents is not returning the right size");
# endif

  } while (p < t);
  mark->_point = p;
}

void oldSpace::initialize_threshold() {
  next_offset_index                 = 0;
  offset_array[next_offset_index++] = 0;
  next_offset_treshold = _bottom + card_size_in_oops;
}

oldSpace::oldSpace(char *name, int &size) {
  next_space= NULL;

  offset_array = NEW_C_HEAP_ARRAY(u_char, Universe::old_gen.virtual_space.reserved_size()/card_size);
  set_name(name);
  set_bottom((oop*) Universe::old_gen.virtual_space.low());
  set_top((oop*)    Universe::old_gen.virtual_space.low());
  set_end((oop*)    Universe::old_gen.virtual_space.high());
  initialize_threshold();
}

void oldSpace::update_offset_array(oop* p, oop* p_end) {
  assert( p_end >= next_offset_treshold, "should be past threshold");
  //  [    ][    ][   ]       "card pages"
  //    ^p  ^t     ^p_end
  assert((next_offset_treshold - p) <= card_size_in_oops,
    "Offset should be <= card_size_in_oops");
  offset_array[next_offset_index++] = next_offset_treshold - p;
  next_offset_treshold += card_size_in_oops;
  while (next_offset_treshold < p_end) {
    offset_array[next_offset_index++] = card_size_in_oops;
    next_offset_treshold += card_size_in_oops;
  }
}

int oldSpace::expand(int size) {
  int min_size = ReservedSpace::page_align_size(size);
  int expand_size = ReservedSpace::align_size(min_size, ObjectHeapExpandSize * K);
  Universe::old_gen.virtual_space.expand(expand_size);
  set_end((oop*) Universe::old_gen.virtual_space.high());
  return expand_size;
}

oop* oldSpace::expand_and_allocate(int size) {
  expand(size);
  return allocate(size);
}

void oldSpace::scavenge_recorded_stores() {
  Universe::remembered_set->scavenge_contents(this);
}

void oldSpace::scavenge_contents_from(OldWaterMark* mark) {
  assert(this == mark->_space, "Match does not match space");
  oop* p = mark->_point;
  while (p < _top) {
    assert(oop(*p)->is_mark(),"must be mark");
    memOop x = as_memOop(p);
    p += x->scavenge_tenured_contents();
  }
  assert(p == _top, "p should be top");
  mark->_point = _top;
}

void oldSpace::object_iterate_from(OldWaterMark* mark, ObjectClosure* blk) {
  blk->begin_space(this);
  oop* p = mark->_point;
  oop* t = top();
  while (p < t) {
    memOop m = as_memOop(p);
    blk->do_object(m);
    p += m->size();
  }
  mark->_point = p;
  blk->end_space(this);
}


void space::oops_do(oopsDoFn f) {
  oop* p = bottom();
  oop* t = top();
  while (p < t) {
    memOop m = as_memOop(p);
    f((oop*) &m);
    p += m->size();
  }
}

void space::print() {
  std->print(" %5s %6dK, %d%% used", name(), capacity() / K, used() * 100 / capacity());
  if (WizardMode) {
    std->print(" [%#-6lx,%#-6lx[", bottom(), top());
  }
  std->cr();
}

void space::object_iterate(ObjectClosure* blk) {
  if (is_empty()) return;
  blk->begin_space(this);
  oop* p = bottom();
  oop* t = top();
  while (p < t) {
    memOop m = as_memOop(p);
    blk->do_object(m);
    p += m->size();
  }
  blk->end_space(this);
}

void newSpace::verify() {
  lprintf("%s ", name());
  oop* p = bottom();
  oop* t = top();

  memOop m;
  while (p < t) {
    assert(oop(*p)->is_mark(), "First word must be mark");
    m = as_memOop(p);
    m->verify();
    p += m->size();
  }
  assert(p == top(), "end of last object must match end of space");
}

class VerifyOldOopClosure : public OopClosure {
public:
  memOop the_obj;
  void do_oop(oop* o) {
    oop obj = *o;
    if (obj->is_new()) {
      // Make sure the the_obj is in the remembered set
      if (!Universe::remembered_set->is_object_dirty(the_obj)) {
        std->cr();
        std->print_cr("New obj reference found in non dirty page.");
        std->print_cr("- object containing the reference:");
        the_obj->print();
        std->print_cr("- the referred object:");
        std->print("[0x%lx]: 0x%lx = ", o, obj);
        obj->print_value();
        std->cr();
        Universe::remembered_set->print_set_for_object(the_obj);
        warning("gc problem");
      }
    }
  }
};

void oldSpace::verify() {
  lprintf("%s ", name());
  oop*   p = _bottom;
  memOop m;
  VerifyOldOopClosure blk;
  while (p < _top) {
    assert(oop(*p)->is_mark(), "First word must be mark");
    m = as_memOop(p);

    int size = m->size();
    assert(m == as_memOop(Universe::object_start(p + (size/2))), "check offset computation");

    m->verify();
    blk.the_obj = m;
    m->oop_iterate(&blk);
    p += m->size();
  }
  assert(p == _top, "end of last object must match end of space");
}

oop* oldSpace::object_start(oop* p) {
  // Find the page start
  oop* q = p;
  int b = (int) q;
  clearBits(b, nthMask(card_shift));
  q = (oop*) b;
  assert(contains(q), "q must be in this space");
  int index  = (q - bottom()) / card_size_in_oops;

  int offset = offset_array[index--];
  while(offset == card_size_in_oops) {
    q -= card_size_in_oops;
    offset = offset_array[index--];
  }
  q -= offset;
  oop* n = q;
  assert((*n)->is_mark(), "check for header");
  while (n <= p) {
    q = n;
    n += as_memOop(n)->size();
  }
  assert( as_memOop(q)->mark()->is_mark(), "Must be mark");
  return q;
}

extern "C" oop* eden_bottom = NULL;
extern "C" oop* eden_top    = NULL;
extern "C" oop* eden_end    = NULL;
