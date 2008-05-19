/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.50 $ */
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
# include "incls/_universe.more.cpp.incl"

bool GCInProgress = false;

oop Universe::tenure(oop p) {
  tenuring_threshold = 0;		// tenure everything
  scavenge(&p);
# define checkIt(s) assert(s->used() == 0, "new spaces should be empty");
  APPLY_TO_YOUNG_SPACES(checkIt)
# undef checkIt
  return p;
}

bool Universe::can_scavenge() {
  // don't scavenge if we're in critical vm operation
  if (processSemaphore) return false;

  if (BlockScavenge::is_blocked()) return false;

  // don't scavenge if we're allocating in the VM process.
  if (DeltaProcess::active()->in_vm_operation()) return false;

  return true;
}

extern "C" void scavenge_and_allocate(int size) {
  Universe::scavenge_and_allocate(size, NULL);
}

oop* Universe::scavenge_and_allocate(int size, oop* p) {
  // Fix this:
  //  If it is a huge object we are allocating we should
  //  allocate it in old_space and return without doing a scavenge  
  if (!can_scavenge()) return allocate_tenured(size);

  VM_Scavenge op(p);
  VMProcess::execute(&op);
  assert(DeltaProcess::active()->last_Delta_fp() != NULL, "last Delta fp should be present");
  assert(DeltaProcess::active()->last_Delta_sp() != NULL, "last Delta fp should be present");
  return allocate_without_scavenge(size);
}

void Universe::scavenge_oop(oop* p) { *p = (*p)->scavenge(); }

bool Universe::needs_garbage_collection() {
  return old_gen.free() < new_gen.to()->capacity();
}

void os_dump_context();

void Universe::scavenge(oop* p) {
  // %note
  //   the symbol_table can be ignored during scavenge since all
  //   all symbols are tenured.
  FlagSetting fl(GCInProgress, true);
  ResourceMark rm;
  scavengeCount++;
  assert(!processSemaphore, "processSemaphore shouldn't be set");
  {
    EventMarker m("scavenging");
    TraceTime   t("Scavenge", PrintScavenge);

    if (PrintScavenge && WizardMode) {
      std->print(" %d",tenuring_threshold);
    }
    
    if (VerifyBeforeScavenge) verify();

    WeakArrayRegister::begin_scavenge();

    // Getting ready for scavenge
    age_table->clear();

    new_gen.to_space->clear();

    // Save top of to_space and old_gen
    NewWaterMark to_mark  = new_gen.to_space->top_mark();
    OldWaterMark old_mark =           old_gen.top_mark();

    // Scavenge all roots
    if (p) SCAVENGE_TEMPLATE(p);

    Universe::roots_do(scavenge_oop);
    Handles::oops_do(scavenge_oop);

    {FOR_EACH_OLD_SPACE(s) s->scavenge_recorded_stores();}

    Processes::scavenge_contents();
    NotificationQueue::oops_do(&Universe::scavenge_oop);

    // Scavenge promoted contents in to_space and old_gen until done.

    while (   (old_mark != old_gen.top_mark()) 
           || (to_mark != new_gen.to_space->top_mark())) {
      old_gen.scavenge_contents_from(&old_mark);
      new_gen.to_space->scavenge_contents_from(&to_mark);
    }

    WeakArrayRegister::check_and_scavenge_contents();

    new_gen.swap_spaces();

    // Set the desired survivor size to half the real survivor space
    int desired_survivor_size = new_gen.to()->capacity()/2;
    tenuring_threshold = age_table->tenuring_threshold(desired_survivor_size/oopSize);
    
    if (VerifyAfterScavenge) verify(true);
    
    // do this at end so an overflow during a scavenge doesnt cause another one
    NeedScavenge = false;
  }
}

space* Universe::spaceFor(void* p) {
  if (new_gen.from()->contains(p)) return new_gen.from();
  if (new_gen.eden()->contains(p)) return new_gen.eden();
  {FOR_EACH_OLD_SPACE(s) if (s->contains(p)) return s;}
  ShouldNotReachHere(); // not in any space
  return NULL;
}
