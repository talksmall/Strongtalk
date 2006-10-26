/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.28 $ */
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
# include "incls/_fprofiler.cpp.incl"

pnode** FlatProfiler::table = NULL;
int     FlatProfiler::table_size = 1024;

DeltaProcess*     FlatProfiler::proc = NULL;
FlatProfilerTask* FlatProfiler::task = NULL;
timer             FlatProfiler::time;

int FlatProfiler::gc_ticks        = 0;
int FlatProfiler::semaphore_ticks = 0;
int FlatProfiler::stub_ticks      = 0;
int FlatProfiler::unknown_ticks   = 0;
int FlatProfiler::compiler_ticks  = 0;

static const int col2 = 11;	// position of output column 2
static const int col3 = 30;	// position of output column 3
static const int col4 = 55;	// position of output column 4

class tick_counter {
 public:
  int ticks_in_code;
  int ticks_in_primitives;
  int ticks_in_compiler;
  int ticks_in_pics;
  int ticks_in_other;

  tick_counter() { 
    ticks_in_code       = 0;
    ticks_in_primitives = 0;
    ticks_in_compiler   = 0;
    ticks_in_pics       = 0;
    ticks_in_other      = 0;
  }


  int total() const {
    return ticks_in_code
         + ticks_in_primitives
         + ticks_in_compiler
	 + ticks_in_pics
         + ticks_in_other;
  }

  void add(tick_counter* a) {
    ticks_in_code       += a->ticks_in_code;
    ticks_in_primitives += a->ticks_in_primitives;
    ticks_in_compiler   += a->ticks_in_compiler;
    ticks_in_pics       += a->ticks_in_pics;
    ticks_in_other      += a->ticks_in_other;
  }

  void update(TickPosition where) {
    switch(where) {
      case in_code:      ticks_in_code++;       break;
      case in_primitive: ticks_in_primitives++; break;
      case in_compiler:  ticks_in_compiler++;   break;
      case in_pic:       ticks_in_pics++;       break;
      case other:        ticks_in_other++;      break;
    }
  }

  void print_code(outputStream* st, int total_ticks) {
    st->print("%5.1f%% %3d ", total() * 100.0 / total_ticks, ticks_in_code);
  }

  void print_other(outputStream* st) {    
    if (ticks_in_primitives > 0) st->print("prim=%d ",  ticks_in_primitives);
    if (ticks_in_compiler > 0)   st->print("comp=%d ",  ticks_in_compiler);
    if (ticks_in_pics > 0)       st->print("pics=%d ",  ticks_in_pics);
    if (ticks_in_other > 0)      st->print("other=%d ", ticks_in_other);
  }
};

class pnode : public CHeapObj {
 private:
  pnode* _next;
 public:
  tick_counter ticks;

 public:
  pnode() {
    _next = NULL;
  }

  ~pnode() {
    if (_next)
      delete _next;
  }

  void set_next(pnode* n) { _next = n; }
  pnode* next()           { return _next; }

  void update(TickPosition where) { ticks.update(where);}
  int total_ticks() { return ticks.total(); }

  virtual bool is_interpreted() const { return false; }
  virtual bool is_compiled()    const { return false; }

  virtual bool match(methodOop m, klassOop k) { return false; }
  virtual bool match(nmethod* nm)             { return false; }


  static void print_title(outputStream* st) {
    st->fill_to(col2);
    st->print("Receiver");
    st->fill_to(col3);
    st->print("Method");
    st->fill_to(col4);
    st->print_cr("Leaf ticks");
  }

  static void print_total(outputStream* st, tick_counter* t, int total, char* msg) {
    t->print_code(st, total);
    st->print(msg);
    st->fill_to(col4);
    t->print_other(st);
    st->cr();
  }

  virtual methodOop method()         = 0;
  virtual klassOop  receiver_klass() = 0;
  
  void print_receiver_klass_on(outputStream* st) {
    receiver_klass()->klass_part()->print_name_on(st);
  }

  virtual void print_method_on(outputStream* st) {
    methodOop m = method();
    if (m->is_blockMethod()) {
      st->print("[] ");
      m->enclosing_method_selector()->print_symbol_on(st);
    } else {
      m->selector()->print_symbol_on(st);
    }

    if (ProfilerShowMethodHolder) {
      klassOop method_holder = receiver_klass()->klass_part()->lookup_method_holder_for(m);
      if (method_holder && (method_holder != receiver_klass())) {
        std->print(", in ");
        method_holder->klass_part()->print_name_on(std);
      }
    }
  }

  virtual void print(outputStream* st, int total_ticks) {
    ticks.print_code(st, total_ticks);
    st->fill_to(col2);
    print_receiver_klass_on(st);
    st->fill_to(col3);

    print_method_on(st);
    st->fill_to(col4);

    ticks.print_other(st);
    st->cr();
  }

  // for sorting
  static int compare(pnode** a, pnode** b) { return (*b)->total_ticks() - (*a)->total_ticks(); }
};

class interpretedNode : public pnode {
 private:
   methodOop _method;
   klassOop  _receiver_klass;
 public:
   interpretedNode(methodOop method, klassOop receiver_klass, TickPosition where) : pnode() {
     _method         = method;
     _receiver_klass = receiver_klass;
     update(where);
   }

   bool is_interpreted() const { return true; }

   bool match(methodOop m, klassOop k) {
      return   _method         == m 
            && _receiver_klass == k;
   }

   methodOop method()         { return _method;         }
   klassOop  receiver_klass() { return _receiver_klass; }

   static void print_title(outputStream* st) {
     st->print("       Int");
     pnode::print_title(st);
   }

   void print(outputStream* st, int total_ticks) {
     pnode::print(st, total_ticks);
   }
};

class compiledNode : public pnode {
 private:
   nmethod* nm;
 public:
   compiledNode(nmethod* nm, TickPosition where) : pnode() {
     this->nm = nm;
     update(where);
  }
  bool is_compiled()    const { return true; }

  bool match(nmethod* m) { return nm == m; }

  methodOop method()         { return nm->method();         }
  klassOop  receiver_klass() { return nm->receiver_klass(); }

  static void print_title(outputStream* st) {
    st->print("       Opt");
    pnode::print_title(st);
  }

  void print(outputStream* st, int total_ticks) {
    pnode::print(st, total_ticks);
  }

  void print_method_on(outputStream* st) {
    if (nm->isUncommonRecompiled()) {
      st->print("Uncommom recompiled ");
    }
    pnode::print_method_on(st);
    if (CompilerDebug) {
      st->print(" %#x ", nm);
    }
  }
};

int FlatProfiler::entry(int  value) {
  return value % table_size;
}

void FlatProfiler::interpreted_update(methodOop method, klassOop klass, TickPosition where) {
  int index = entry(method->selector_or_method()->identity_hash());
  if (!table[index]) {
    table[index] = new interpretedNode(method, klass, where);
  } else {
    for(pnode* node = table[index]; node; node = node->next()) {
      if (node->match(method, klass)) {
        node->update(where);
        return;
      }
      if (!node->next())
        node->set_next(new interpretedNode(method, klass, where));
    }
  }
}

void FlatProfiler::compiled_update(nmethod* nm, TickPosition where) {
  int index = entry(nm->main_id.major());
  if (!table[index]) {
    table[index] = new compiledNode(nm, where);
  } else {
    for(pnode* node = table[index]; node; node = node->next()) {
      if (node->match(nm)) {
        node->update(where);
        return;
      }
      if (!node->next())
        node->set_next(new compiledNode(nm, where));
    }
  }
}

class FlatProfilerTask : public PeriodicTask {
 public:
  FlatProfilerTask(int interval_time) : PeriodicTask(interval_time) {}
  void task();
};

void FlatProfilerTask::task() {
  // ignore of we're not in the right process
  if (FlatProfiler::proc == NULL) return; // profiler not active
  if (DeltaProcess::active() == FlatProfiler::proc || UseGlobalFlatProfiling) {
    FlatProfiler::record_tick();
  }
}

void FlatProfiler::record_tick_for_running_frame(frame fr) {
  // The tick happend in real code -> non VM code
  if (fr.is_interpreted_frame()) {
    methodOop method = fr.method();
    if (method == NULL) return;
    assert(method->is_method(), "must be method");
    FlatProfiler::interpreted_update(method, fr.receiver()->klass(), in_code);

  } else if (fr.is_compiled_frame()) {
    FlatProfiler::compiled_update(findNMethod(fr.pc()), in_code);

  } else if (PIC::in_heap(fr.pc())) {
    PIC* pic = PIC::find(fr.pc());
    FlatProfiler::compiled_update(findNMethod((char*) pic->compiled_ic()), in_pic);

  } else if (StubRoutines::contains(fr.pc())) {
    FlatProfiler::stub_ticks++;
  }
}

void FlatProfiler::record_tick_for_calling_frame(frame fr) {
  // The tick happend in VM code

  TickPosition where = other;
  if (theCompiler) { 
    where = in_compiler;
  }
  if (fr.is_interpreted_frame()) {
    methodOop method = fr.method();
    if (method == NULL) return;
    assert(method->is_method(), "must be method");
    int bci = method->bci_from(fr.hp());
    if (Bytecodes::code_type((Bytecodes::Code) *method->codes(bci)) == Bytecodes::primitive_call) {
      where = in_primitive;
    }
    FlatProfiler::interpreted_update(method, fr.receiver()->klass(), where);

  } else if (fr.is_compiled_frame()) {
    nmethod* nm = findNMethod(fr.pc());
    relocIterator iter(nm);
    while (iter.next()) {
      if (iter.is_call() && iter.call_end() == fr.pc()) {
        if (iter.type() == relocInfo::prim_type)
	  where = in_primitive;
      }
    }
    FlatProfiler::compiled_update(nm, where);

  } else {
    if (StubRoutines::contains(fr.pc())) {
      FlatProfiler::stub_ticks++;
    } else {
      FlatProfiler::unknown_ticks++;
    }
  }
}

void FlatProfiler::record_tick() {
  // If we're idle forget about the tick.
  if (DeltaProcess::is_idle()) return;

  // check for special vm flags
  if (theCompiler)      { FlatProfiler::compiler_ticks++; }
  if (GCInProgress)     { FlatProfiler::gc_ticks++;        return; }
  if (processSemaphore) { FlatProfiler::semaphore_ticks++; return; }
 
  { FlagSetting(processSemaphore, true);
    DeltaProcess* p = DeltaProcess::active();
    if (p->last_Delta_fp()) {
      record_tick_for_calling_frame(p->last_frame());
    } else {
      record_tick_for_running_frame(p->profile_top_frame());
    }
  }
}

void FlatProfiler::allocate_table() {
  table = NEW_C_HEAP_ARRAY(pnode*, table_size);
  for(int index = 0; index < table_size; index++) 
    table[index] = NULL;
}

void FlatProfiler::reset() {
  proc = NULL;
  task = NULL;

  for(int index = 0; index < table_size; index++) {
    pnode* n = table[index];
    if (n) { 
      delete n;
      table[index] = NULL;
    }
  }

  gc_ticks           = 0;
  unknown_ticks      = 0;
  semaphore_ticks    = 0;
  compiler_ticks     = 0;
  stub_ticks         = 0;
}

void FlatProfiler::engage(DeltaProcess* p) {
  proc = p;
  if (task == NULL) {
    task = new FlatProfilerTask(10);
    task->enroll();
    time.start();
  }
}

DeltaProcess* FlatProfiler::disengage() {
  if (!task) return NULL;
  task->deroll();
  delete task;
  task = NULL;
  time.stop();
  DeltaProcess* p = process();
  proc = NULL;
  return proc;
}

bool FlatProfiler::is_active() {
  return task != NULL;
}

static int compare_nodes(const void* p1,  const void* p2) {
  pnode** pn1 = (pnode**)p1;
  pnode** pn2 = (pnode**)p2;
  return (*pn2)->total_ticks() - (*pn1)->total_ticks();
}


void print_ticks(char* title, int ticks, int total) {
 if (ticks>0)
    std->print_cr("%5.1f%%  %3d %s", ticks * 100.0 / total, ticks, title);
}

void FlatProfiler::print(int cutoff) {
#ifdef PRODUCT
  // disable profiler
#else
  FlagSetting f(PrintObjectID, false);
  ResourceMark rm;
  double secs = time.seconds();

  GrowableArray <pnode*>* array = new GrowableArray<pnode*>(200);

  for(int index = 0; index < table_size; index++) {
    for(pnode* node = table[index]; node; node = node->next())
      array->append(node);
  }

  array->sort(&pnode::compare);

  // compute total
  int total = total_ticks();
  for (index = 0; index < array->length(); index++) {
    total += array->at(index)->ticks.total();
  }

  std->cr();
  std->print_cr("Flatprofile %3.2f secs (%d ticks):", secs, total);
  std->cr();

  // print interpreted methods
  tick_counter interpreted_ticks;
  bool has_interpreted_ticks = false;
  int print_count = 0;
  for (index = 0; index < array->length(); index++) {
    pnode* n = array->at(index);
    if (n->is_interpreted()) {
      interpreted_ticks.add(&n->ticks);
      if (!has_interpreted_ticks) {
        interpretedNode::print_title(std);
        has_interpreted_ticks = true;
      }
      if (print_count++ < ProfilerNumberOfInterpreterMethods) {
        n->print(std, total);
      }
    }
  }
  if (has_interpreted_ticks) {
    tick_counter others;
    for ( ; index < array->length(); index++) others.add(&array->at(index)->ticks);
    if (others.total() > 0) {
      interpretedNode::print_total(std, &interpreted_ticks, total, "(all above)");
      interpretedNode::print_total(std, &others, total, "(all others)");
    }
    interpreted_ticks.add(&others);
    interpretedNode::print_total(std, &interpreted_ticks, total, "Total interpreted");
    std->cr();
  }

  // print compiled methods
  print_count = 0;
  bool has_compiled_ticks = false;
  tick_counter compiled_ticks;
  for (index = 0; index < array->length(); index++) {
    pnode* n = array->at(index);
    if (n->is_compiled()) {
      compiled_ticks.add(&n->ticks);
      if (!has_compiled_ticks) {
        compiledNode::print_title(std);
        has_compiled_ticks = true;
      }
      if (print_count++ < ProfilerNumberOfCompiledMethods) {
        n->print(std, total);
      }
    }
  }

  if (has_compiled_ticks) {
    tick_counter others;
    for ( ; index < array->length(); index++) others.add(&array->at(index)->ticks);
    if (others.total() > 0) {
      compiledNode::print_total(std, &compiled_ticks, total, "(all above)");
      compiledNode::print_total(std, &others, total, "(all others)");
    }
    compiled_ticks.add(&others);
    compiledNode::print_total(std, &compiled_ticks, total, "Total compiled");
    std->cr();
  }

  std->cr();

  if (total_ticks() > 0) {
    std->print_cr(" Additional ticks:");
    print_ticks("Garbage collector", gc_ticks,        total);
    print_ticks("Process semaphore", semaphore_ticks, total);
    print_ticks("Unknown code",      unknown_ticks,   total);
    print_ticks("Stub routines",     stub_ticks,      total);
    print_ticks("Total compilation (already included above)", compiler_ticks, total);
  }
#endif
}

void fprofiler_init() {
  FlatProfiler::allocate_table();
}