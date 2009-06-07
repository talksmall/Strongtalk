/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.12 $ */
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
# include "incls/_markSweep.cpp.incl"
typedef struct {
  oop anOop;
  oop* oopPointer;
} oopAssoc;

class OopChunk : public ResourceObj {
private:
  oopAssoc oop_start[1000];
  const oopAssoc* oop_end;;
  oopAssoc* next;

public:
  OopChunk() {
    oop_end = oop_start + 1000 - 1;// account for pre-increment in append
    next = oop_start - 1;
  }

  bool isFull() {
    return next >= oop_end;
  }

  oop* append(oop* anOop) {
    assert(!isFull(), "Cannot append to full OopChunk");
    assert((*anOop)->is_mem(), "Must be a mem oop");
    next++;
    next->anOop = *anOop;
    next->oopPointer = anOop;
    return &next->anOop;
  }

  void fixupOops() {
    oopAssoc* current = next;
    while (current >= oop_start) {
      assert(current->anOop->is_mem(), "Fixed up oop should be memOop");
      *(current->oopPointer) = current->anOop;
      current--;
    }
  }
};

class OopRelocations : public ResourceObj {
private:
  GrowableArray<OopChunk*>* chunks;
  OopChunk* current;

  void newChunk() {
      current = new OopChunk();
      chunks->append(current);
  }

public:
  OopRelocations() {
    chunks = new GrowableArray<OopChunk*>(10);
    newChunk();
  }

  oop* relocate(oop* toMove) {
    if (current->isFull()) {
      newChunk();
    }
    return current->append(toMove);
  }

  void fixupOops() {
    while(chunks->nonEmpty())
      chunks->pop()->fixupOops();
  }
};

GrowableArray<memOop>* MarkSweep::stack;
GrowableArray<int>*    MarkSweep::hcode_offsets;
int                    MarkSweep::hcode_pos;
OopRelocations*        MarkSweep::oopRelocations;


void oopVerify(oop* p) {
  (*p)->verify();
}

oop MarkSweep::collect(oop p) {
  FlagSetting  fl(GCInProgress, true);
  EventMarker  em("Garbage Collect");
  ResourceMark rm;
  TraceTime    t("Garbage collection", PrintGC);

  int old_used = Universe::old_gen.used();

  if (VerifyBeforeScavenge || VerifyBeforeGC) Universe::verify();

  // Clear all vm inline caches
  DeltaCallCache::clearAll();

  // clear remembered set; it is used for object sizes
  Universe::remembered_set->clear();

  allocate();	// allocate stack for traversal
  
  mark_sweep_phase1(&p);
  mark_sweep_phase2();
  mark_sweep_phase3();

  deallocate(); // clear allocated structures
  
  // clear the remember set; we have no pointers from old to new
  Universe::remembered_set->clear();

  lookupCache::flush();

  if (VerifyAfterScavenge || VerifyAfterGC) {
    Universe::verify();
    Universe::code->oops_do(&oopVerify);
  }

  if (PrintGC) {
    std->print(" %.1fM -> %.1fM",
               (double) old_used                 / (double) (1024 * 1024),
               (double) Universe::old_gen.used() / (double) (1024 * 1024));
  }

  return p;
}

void MarkSweep::allocate() {
  stack         = new GrowableArray<memOop>(200);
  hcode_offsets = new GrowableArray<int>(100);
  hcode_pos     = 0;
  oopRelocations = new OopRelocations();
}

void MarkSweep::deallocate() {
  stack         = NULL;
  hcode_offsets = NULL;
}

void MarkSweep::trace(char* msg) {
  if (TraceGC)
    std->print("%s", msg);
}

memOop MarkSweep::reverse(oop* p) {
  oop obj = *p;

  // Return NULL if non memOop
  if (!obj->is_mem()) return NULL;
  if (!oop(p)->is_smi()) {// ie. not word aligned
    p = oopRelocations->relocate(p);
  }
  
  if (memOop(obj)->is_gc_marked()) {
    // Reverse pointer
    *p = oop(memOop(obj)->mark());
    memOop(obj)->set_mark(p);

    return NULL;
  } else {
    // Before the pointer reversal takes place the object size must be made accessible
    // without using the klass pointer. We store the object size partially in the 
    // age field and partially in the remembered set.
    memOop(obj)->gc_store_size();

    assert(memOop(obj)->size() == memOop(obj)->gc_retrieve_size(), "checking real against stored size");

    // Reverse pointer
    *p = oop(memOop(obj)->mark());
    memOop(obj)->set_mark(p);

    assert(memOop(obj)->klass()->is_mem(), "just checking");

    return memOop(obj);
  }
}

void MarkSweep::reverse_and_push(oop* p) {
  memOop m = reverse(p);
  if (m) stack->push(m);
}

void MarkSweep::reverse_and_follow(oop* p) {
  memOop m = reverse(p);
  if (m) m->follow_contents(); // Follow contents of the marked object
}

void MarkSweep::follow_root(oop* p) {
  reverse_and_follow(p);
  while (!stack->isEmpty())
    stack->pop()->follow_contents();
}

void MarkSweep::add_hcode_offset(int offset) {
  hcode_offsets->push(offset);
}

int MarkSweep::next_hcode_offset() {
  return hcode_offsets->at(hcode_pos++);
}

void MarkSweep::mark_sweep_phase1(oop* p) {
  // Recursively traverse all live objects and mark them by reversing pointers.
  EventMarker em("1 reverse pointers");

  trace(" 1");

  WeakArrayRegister::begin_mark_sweep();
  Processes::convert_hcode_pointers();

  Universe::oops_do(&follow_root);

  // Hcode pointers point to the inside of methodOops and
  // have to be converted before we can follow the processes.
  Processes::follow_roots();

  WeakArrayRegister::check_and_follow_contents();
  NotificationQueue::oops_do(&follow_root);

  vmSymbols::follow_contents();

  // finalize unused objects - must be after all other gc_mark routines!
  Universe::symbol_table->follow_used_symbols();

  assert(stack->isEmpty(), "stack should be empty by now");
}

void MarkSweep::mark_sweep_phase2() {
  // Now all live objects are marked, compute the new object addresses.
  EventMarker em("2 compute new addresses");
  trace("2");

  OldWaterMark mark = Universe::old_gen.bottom_mark();
  // %note memory must be traversed in the same order as phase3
  Universe::old_gen.prepare_for_compaction(&mark);
  Universe::new_gen.prepare_for_compaction(&mark);
}

void MarkSweep::mark_sweep_phase3() {
  EventMarker em("3 compact heap");
  trace("3");
  OldWaterMark mark = Universe::old_gen.bottom_mark();
  mark._space->initialize_threshold();
  // %note memory must be traversed in the same order as phase2
  Universe::old_gen.compact(&mark);
  Universe::new_gen.compact(&mark);

  // update non-oop-aligned oop locations
  oopRelocations->fixupOops();

  // All hcode pointers can now be restored. Remember
  // we converted these pointers in phase1.
  Processes::restore_hcode_pointers();
}

