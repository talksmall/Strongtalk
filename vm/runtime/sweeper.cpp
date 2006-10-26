/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.17 $ */
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
# include "incls/_sweeper.cpp.incl"
# include <math.h>

// The sweeper run at real_time ticks. We only swep if the interrupted
// Delta process is in a well-defined state (see SweeperTask).
// We might change the sweeper to sweep at preempt time like in the Self system.

Sweeper*  Sweeper::_head           = NULL;
int       Sweeper::sweep_seconds   = 0;
bool      Sweeper::_is_running     = false;
methodOop Sweeper::_active_method  = NULL;
nmethod*  Sweeper::_active_nmethod = NULL;

void Sweeper::print_all() {
  for (Sweeper* n = head(); n; n = n->next()) 
    n->print();
}

bool Sweeper::register_active_frame(frame fr) {
  if (fr.is_interpreted_frame()) {
    _active_method = fr.method();
    if (_active_method == NULL) return NULL;
    return true;
  } else if (fr.is_compiled_frame()) {
    _active_nmethod = findNMethod(fr.pc());
    return true;
  }
  return false;
}

void Sweeper::clear_active_frame() {
  _active_method  = NULL;
  _active_nmethod = NULL;
}

void Sweeper::step_all() {
  _is_running = true;
  ResourceMark rm;
  for (Sweeper* n = head(); n; n = n->next()) 
    n->step();
  sweep_seconds++;
  _is_running = false;
}

Sweeper::Sweeper() {
  _is_active   = false;
  _sweep_start = sweep_seconds;
}

void Sweeper::add(Sweeper* sweeper) {
  sweeper->_next = head();
  _head          = sweeper;
}

void Sweeper::step() {
  if (interval() == 0) return;
 
  if (!is_active() && (sweep_seconds - _sweep_start) >= interval()) {
    _sweep_start = sweep_seconds;
     activate();
  }
  if (is_active()) task();
}

void Sweeper::print() const {
  std->print_cr("%s", name());
}

void Sweeper::activate() {
  _is_active = true;
  LOG_EVENT1("Activating %s", name());
}

void Sweeper::deactivate() {
  _is_active = false;
  LOG_EVENT1("Deactivating %s", name());
}

// ---------------- HeapSweeper -----------------

void HeapSweeper::activate() {
  mark = Universe::old_gen.bottom_mark();
  Sweeper::activate();
}

void HeapSweeper::task() {}

// ---------------- CodeSweeper -----------------

inline void CodeSweeper::updateInterval() {
  if (oldHalfLifeTime != CounterHalfLifeTime) {
    oldHalfLifeTime = CounterHalfLifeTime;
    CodeSweeperInterval = 4;		    // for now, use fixed value; could adjust if necessary
    fractionPerTask = 8;
    const double log2 = 0.69314718055995;   // log(2)
    decayFactor = exp(log2 * CodeSweeperInterval * fractionPerTask / CounterHalfLifeTime);
    if (PrintCodeSweep) std->print("*method sweep: decay factor %f\n", decayFactor);
  }
}


int CodeSweeper::interval() const {
  ((CodeSweeper*)this)->updateInterval();
  return CodeSweeperInterval;
}

// ---------------- MethodSweeper -----------------


void MethodSweeper::method_task(methodOop method) {
  if (method != Sweeper::active_method()) {
    if (method->invocation_count() > 0) {
      method->decay_invocation_count(decayFactor);
    }
    method->cleanup_inline_caches();
  } else {
    // Save the nmethod for next round
    set_excluded_method(Sweeper::active_method());
  }
}

int MethodSweeper::method_dict_task(objArrayOop methods) {
  int length = methods->length();
  for (int index = 1; index <= length; index++) {
    methodOop method = methodOop(methods->obj_at(index));
    assert(method->is_method(), "just checking");
    method_task(method);
  }
  return length;
}

int MethodSweeper::klass_task(klassOop klass) {
  int result = 0;
  Klass* k = klass->klass_part();
  // Fix the customized methods 
  result += method_dict_task(k->methods());
  result += method_dict_task(klass->klass()->klass_part()->methods());
  if (k->is_named_class()) {
    // Fix the mixin parts
    result += method_dict_task(k->mixin()->methods());
    result += method_dict_task(klass->klass()->klass_part()->mixin()->methods());
  }

  if (!k->has_superKlass()) return result;
  if (k->superKlass()->klass_part()->is_named_class()) return result;
  
  // super class is an unnamed class so we have to handle it
  result += klass_task(k->superKlass());
  return result;
}

void MethodSweeper::task() {
  // Prologue: check is there is leftover from last sweep
  if (excluded_method()) {
    methodOop m = excluded_method();
    set_excluded_method(NULL);
    method_task(m);
  }

  objArrayOop array     = Universe::systemDictionaryObj();
  int          length   = array->length();
  int number_of_entries = length / fractionPerTask;
  if (PrintCodeSweep) std->print("*method sweep: %d entries...", number_of_entries);
  TraceTime t("MethodSweep ", PrintCodeSweep);

  int end = (index + number_of_entries);
  if (end > length)
    end = length;

  int begin = index;
  int result = 0;

  for (; index <= end; index++) {
     associationOop assoc =  associationOop(array->obj_at(index));
     assert(assoc->is_association(), "just checking");
     if (assoc->is_constant() && assoc->value()->is_klass()) {
       int result = klass_task(klassOop(assoc->value()));
     }
  }
  LOG_EVENT3("MethodSweeper task [%d, %d] #%d", begin, end, result);

  if (index > length) deactivate();
}

void MethodSweeper::activate() {
  index = 1;
  Sweeper::activate();
}

// ---------------- ZoneSweeper -----------------

void ZoneSweeper::nmethod_task(nmethod* nm) {
  if (nm != Sweeper::active_nmethod()) {
    nm->sweeper_step(decayFactor);
  } else {
    // Save the nmethod for next round
    set_excluded_nmethod(Sweeper::active_nmethod());
  }
}

void ZoneSweeper::task() {
  // Prologue: check is there is leftover from last sweep
  if (excluded_nmethod()) {
    nmethod* nm = excluded_nmethod();
    set_excluded_nmethod(NULL);
    nmethod_task(nm);
  }

  // %fix this:
  //    we need to validate next
  int total = Universe::code->numberOfNMethods();
  int todo = total / fractionPerTask;
  if (PrintCodeSweep) std->print("*zone sweep: %d of %d entries...", todo, total);
  TraceTime t("ZoneSweep ", PrintCodeSweep);
  
  for (int index = 0; index < todo; index++) {
    if (next == NULL) {
      deactivate(); 
      break;
    }
    nmethod_task(next);
    next = Universe::code->next_nm(next);
  }

  if (UseNMethodAging) {
    for (nmethod* nm = Universe::code->first_nm(); nm; nm = Universe::code->next_nm(nm)) {
      nm->incrementAge();
    }
  }
}

void ZoneSweeper::activate() {
  if (Universe::code->numberOfNMethods() > 0) {
    next = Universe::code->first_nm();
    _excluded_nmethod = NULL;
    Sweeper::activate();
  } else {
    deactivate();
  }
}

// The sweeper task is activated every second (1000 milliseconds).
class SweeperTask : public PeriodicTask {
 private:
   int counter;
 public:
  SweeperTask() : PeriodicTask(100) {
    counter = 0;
  }

  void task() {
    // If we're idle forget about the tick.
    if (DeltaProcess::is_idle()) return;
    if (++counter > 10) {
      if (processSemaphore) return;
      if (last_Delta_fp)    return;

      if (Sweeper::register_active_frame(DeltaProcess::active()->profile_top_frame())) {
        Sweeper::step_all();
        Sweeper::clear_active_frame();
        counter -= 10;
      }
    }
  }
};

MethodSweeper* methodSweeper;

void sweeper_init() {
  // Sweeper::add(new HeapSweeper());
  Sweeper::add(new ZoneSweeper());
  Sweeper::add(methodSweeper = new MethodSweeper());

  if (SweeperUseTimer) {
    SweeperTask* t = new SweeperTask;
    t->enroll();
  }
}