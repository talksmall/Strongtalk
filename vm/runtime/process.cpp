/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.119 $ */
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
# include "incls/_process.cpp.incl"

// The tricky part is to restore the original return address of the primitive before
// the delta call. This is necessary for a consistent stack during the delta call.
extern "C" char*  C_frame_return_addr;
extern "C" bool have_nlr_through_C;
extern "C" int  nlr_home;
extern "C" int  nlr_home_id;
extern "C" oop  nlr_result;

unwindInfo::unwindInfo() {
   assert(have_nlr_through_C, "you must have have_nlr_through_C before using unwindInfo");

   // Save NLR state
   _nlr_home    = ::nlr_home;
   _nlr_home_id = ::nlr_home_id;
   _nlr_result  = ::nlr_result;

   // Save patch information
   assert(last_Delta_fp, "last_Delta_fp must be set");
   saved_C_frame_return_addr          = C_frame_return_addr;
   saved_C_frame_return_addr_location = (char**) (last_Delta_sp - 1);
   saved_patch_return_address         = *saved_C_frame_return_addr_location;

   // Restore original return address
   *saved_C_frame_return_addr_location = saved_C_frame_return_addr;

   _is_compiled = _nlr_home_id >= 0;
   DeltaProcess::active()->push_unwind(this);
}

unwindInfo::~unwindInfo() {
  // If we get an aborting NLR in the protect part we should continue the aborting NLR
  // and not the original NLR.
  if (::nlr_home != 0) {
    // Restore NLR state
    ::nlr_home    = _nlr_home;
    ::nlr_home_id = _nlr_home_id;
    ::nlr_result  = _nlr_result;
  }
  // Restore patch information
  *saved_C_frame_return_addr_location = saved_patch_return_address;
  C_frame_return_addr                 = saved_C_frame_return_addr;

  DeltaProcess::active()->pop_unwind();
}

void unwindInfo::update_nlr_targets(compiledVFrame* f, contextOop con) {
  // Convert the nlr information if:
  //    nlr_home     is the frame pointer of f
  // && nlr_home_id  is the offset of f's scope
  if (f->fr().fp() == (int*) nlr_home() && f->scope()->offset() == nlr_home_id()) {
    _nlr_home_context = con;
  }
}

bool processSemaphore = false;

// For current Delta process, the last FP/Sp is stored in these global vars, not
// the instance vars of the process
int* last_Delta_fp = NULL;
oop* last_Delta_sp = NULL;

// last_Delta_fp
int* DeltaProcess::last_Delta_fp() const {
  return this == _active_delta_process ? ::last_Delta_fp : _last_Delta_fp;
}

void DeltaProcess::set_last_Delta_fp(int* fp) {
  if (this == _active_delta_process) {
    ::last_Delta_fp = fp;
  } else {
    _last_Delta_fp = fp;
  }
}

// last_Delta_sp
oop* DeltaProcess::last_Delta_sp() const {
  return this == _active_delta_process ? ::last_Delta_sp : _last_Delta_sp;
}

void DeltaProcess::set_last_Delta_sp(oop* sp) {
  if (this == _active_delta_process) {
    ::last_Delta_sp = sp;
  } else {
    _last_Delta_sp = sp;
  }
}

// last_Delta_pc
char* DeltaProcess::last_Delta_pc() const {
  if (this == NULL) return NULL;
  return _last_Delta_pc;
}

void DeltaProcess::set_last_Delta_pc(char* pc) {
  _last_Delta_pc = pc;
}

int CurrentHash = 23;

bool Process::external_suspend_current() {
  if (current() == NULL) return false;
  os::suspend_thread(current()->_thread);
  return true;
}

void Process::external_resume_current() {
  os::resume_thread(current()->_thread);
}

void Process::basic_transfer(Process* target) {
  if (TraceProcessEvents) {
    std->print("Process: "); print();
    std->print(" -> "); target->print();
    std->cr();
  }
  os::transfer(_thread, _event, target->_thread, target->_event);
  applyStepping();
}

// ======= VMProcess ========

VMProcess::VMProcess() {
  assert(vm_process() == NULL, "we can only allocate one VMProcess");

  _vm_process   = this;
  _vm_operation = NULL;

  _thread    = os::starting_thread(&_thread_id);
  _event     = os::create_event(true);
}

void VMProcess::transfer_to(DeltaProcess* target) {
  {
    ThreadCritical tc;

    // restore state
    ::last_Delta_fp = target->_last_Delta_fp;	// *don't* use accessors!
    ::last_Delta_sp = target->_last_Delta_sp;
    DeltaProcess::set_active(target);
    DeltaProcess::set_current(target);
    resetStepping();
  }
  basic_transfer(target);
}


void VMProcess::terminate(DeltaProcess* proc) {
  assert(Process::current()->is_vmProcess(), "can only be called from vm process");
  assert(proc->is_deltaProcess(),            "must be deltaProcess");
  assert(proc->_thread,                      "thread must be present");
  assert(proc->_event,                       "event must be present");

  os::terminate_thread(proc->_thread);
  proc->_thread = NULL;
  os::delete_event(proc->_event);
  proc->_event  = NULL;

  DeltaProcess::set_terminating_process(proc->state());
}

void VMProcess::activate_system() {
  // Find the Delta level 'Processor'
  processOop proc = processOop(Universe::find_global("Processor"));
  if (!proc->is_process()) {
    klassOop scheduler_klass = klassOop(Universe::find_global("ProcessorScheduler"));
    proc = processOop(scheduler_klass->klass_part()->allocateObject());
    associationOop assoc = Universe::find_global_association("Processor");
    assoc->set_value(proc);
  }

  DeltaProcess::initialize_async_dll_event();

  // Create the initial process
  DeltaProcess::set_scheduler(new DeltaProcess(proc, oopFactory::new_symbol("start")));

  // Bind the scheduler to Processor
  proc->set_process(DeltaProcess::scheduler());
  DeltaProcess::scheduler()->set_processObj(proc);

  // Transfer control to the scheduler
  transfer_to(DeltaProcess::scheduler());

  // Call the ever running loop handling vm operations
  loop();
}

void VMProcess::loop() {
  while (true) {
    assert(vm_operation(), "A VM_Operation should be present");
    vm_operation()->evaluate();
    // if the process's thread is dead then the stack may already be released
    // in which case the vm_operation is no longer valid, so check for a
    // terminated process first. Can't use accessor as it resets the flag!
    DeltaProcess* p = DeltaProcess::_process_has_terminated
      ? DeltaProcess::scheduler() 
      : vm_operation()->calling_process();
    _vm_operation = NULL;
    transfer_to(p);
  }
}

void VMProcess::print() {
  std->print_cr("VMProcess");
}

void VMProcess::execute(VM_Operation* op) {

  if (Sweeper::is_running()) {
    // We cannot perform a vm operations when running the sweeper since
    // the sweeper run  outside the active process.
    fatal("VMProcess is called during sweeper run");
  }

  if (DeltaProcess::active()->in_vm_operation()) {
    // already running in VM process, no need to switch
    op->evaluate();
  } else {
    op->set_calling_process(DeltaProcess::active());
    _vm_operation = op;
    // Suspend currentProcess and resume vmProcess
    DeltaProcess::active()->transfer_to_vm();
  }
}

extern "C" void returnToDebugger() {
  if (!DeltaProcess::active()->is_scheduler()) {
    DeltaProcess::active()->returnToDebugger();
  }
}

void DeltaProcess::returnToDebugger() {
  resetStepping(); // reset dispatch table
  resetStep();     // disable stepping
  suspend(stopped);// stop!
}

void DebugInfo::interceptForStep() {
  interceptorEntryPoint = &dispatchTable::intercept_for_step;
  frameBreakpoint = NULL;
}

void DebugInfo::interceptForNext(int* fr) {
  interceptorEntryPoint = &dispatchTable::intercept_for_next;
  frameBreakpoint = fr;
}

void DebugInfo::interceptForReturn(int* fr) {
  interceptorEntryPoint = &dispatchTable::intercept_for_return;
  frameBreakpoint = fr;
}
void DebugInfo::apply() { 
  if (interceptorEntryPoint) {
    StubRoutines::setSingleStepHandler(&returnToDebugger);
    interceptorEntryPoint(frameBreakpoint);
    DeltaProcess::stepping = true;
  }
}

void DebugInfo::reset() {
  if (interceptorEntryPoint) {
    dispatchTable::reset();
    StubRoutines::setSingleStepHandler(NULL); // %TODO: replace with breakpoint() or similar?
    DeltaProcess::stepping = false;
  }
}

bool DeltaProcess::stepping = false;
VMProcess*    VMProcess::_vm_process   = NULL;
VM_Operation* VMProcess::_vm_operation = NULL;

// ======= DeltaProcess ========

Process*           Process::_current_process             = NULL;
DeltaProcess* DeltaProcess::_active_delta_process        = NULL;
DeltaProcess* DeltaProcess::_scheduler_process           = NULL;
bool          DeltaProcess::_is_idle                     = false;

volatile bool DeltaProcess::_process_has_terminated      = false;
ProcessState  DeltaProcess::_state_of_terminated_process = initialized;

Event*        DeltaProcess::_async_dll_completion_event  = NULL;

void DeltaProcess::transfer(ProcessState reason, DeltaProcess* target) {
  // change time_stamp for target
  target->inc_time_stamp();

  {
    ThreadCritical tc;

    assert(this == active(), "receiver must be the active process");

    // save state
    _last_Delta_fp = ::last_Delta_fp;	// *don't* use accessors! (check their implementation to see why)
    _last_Delta_sp = ::last_Delta_sp;
    set_state(reason);

    // restore state
    ::last_Delta_fp = target->_last_Delta_fp;	// *don't* use accessors!
    ::last_Delta_sp = target->_last_Delta_sp;
    set_current(target);
    set_active(target);
    resetStepping();
  }

  // transfer
  basic_transfer(target);
}

void DeltaProcess::suspend(ProcessState reason) {
  assert(!is_scheduler(), "active must be other than scheduler");
  assert(!in_vm_operation(), "must not be in VM operation");

  transfer(reason, scheduler());
  if (is_terminating())
    ErrorHandler::abort_current_process();
}

ProcessState DeltaProcess::transfer_to(DeltaProcess* destination) {
  assert(is_scheduler(), "active must be scheduler");
  assert(!in_vm_operation(), "must not be in VM operation");
  // Do not transfer if destination process is execution DLL.
  if (destination->state() == in_async_dll)
    return destination->state();

  transfer(yielded, destination);
  if (process_has_terminated()) {
    return state_of_terminated_process();
  }
  return destination->state();
}

void DeltaProcess::transfer_to_vm() {
  {
    ThreadCritical tc;

    assert(this == active(), "receiver must be the active process");

    // save state
    _last_Delta_fp = ::last_Delta_fp;	// *don't* use accessors! (check their implementation to see why)
    _last_Delta_sp = ::last_Delta_sp;
    set_current(VMProcess::vm_process());
    resetStepping();
  }
  basic_transfer(VMProcess::vm_process());
}

void DeltaProcess::suspend_at_creation() {
  // This is called as soon a DeltaProcess is created
  // Let's wait until we're given the torch.
  os::wait_for_event(_event);
}

void DeltaProcess::transfer_and_continue() {
  {
    ThreadCritical tc;

    assert(!is_scheduler(), "active must be other than scheduler");
    assert(!in_vm_operation(), "must not be in VM operation");
    assert(this == active(), "receiver must be the active process");


    // save state
    _last_Delta_fp = ::last_Delta_fp;	// *don't* use accessors! (check their implementation to see why)
    _last_Delta_sp = ::last_Delta_sp;
    set_state(in_async_dll);


    // restore state
    ::last_Delta_fp = scheduler()->_last_Delta_fp;	// *don't* use accessors!
    ::last_Delta_sp = scheduler()->_last_Delta_sp;
    set_current(scheduler());
    set_active(scheduler());

    if (TraceProcessEvents) {
      std->print("Async call: "); print();
      std->print("        to: "); scheduler()->print();
      std->cr();
    }
  }
  os::transfer_and_continue(_thread, _event, scheduler()->_thread, scheduler()->_event);
}

bool DeltaProcess::wait_for_async_dll(int timeout_in_ms) {
  os::reset_event(_async_dll_completion_event);
  if (Processes::has_completed_async_call()) return true;

  if (TraceProcessEvents) {
    std->print("Waiting for async %d ms", timeout_in_ms);
  }

  _is_idle = true;
  bool result = os::wait_for_event_or_timer(_async_dll_completion_event, timeout_in_ms);
  _is_idle = false;


  if (TraceProcessEvents) {
    std->print_cr(result ? " {timeout}" : " {async}");
  }

  return result;
}

void DeltaProcess::initialize_async_dll_event() {
  _async_dll_completion_event = os::create_event(false);
}

void DeltaProcess::async_dll_call_completed() {
  os::signal_event(_async_dll_completion_event);
}

void DeltaProcess::wait_for_control() {
  if (TraceProcessEvents) {
    std->print("*");
  }

  set_state(yielded_after_async_dll);
  async_dll_call_completed();
  os::wait_for_event(_event);
  if (is_terminating())
    ErrorHandler::abort_current_process();
}

extern "C" bool have_nlr_through_C;

// Code entry point for at Delta process
int DeltaProcess::launch_delta(DeltaProcess* process) {
  // Wait until we get the torch
  process->suspend_at_creation();

  // We have the torch
  assert(process == DeltaProcess::active(),  "process consistency check");
  assert(process->is_deltaProcess(), "this should be a deltaProcess");

  DeltaProcess* p = (DeltaProcess*) process;
  oop result = Delta::call(p->receiver(), p->selector());

  if (have_nlr_through_C) {
    if (nlr_home_id == ErrorHandler::aborting_nlr_home_id()) {
      p->set_state(aborted);
    } else {
      p->set_state(NLR_error);
    }
  } else {
    p->set_state(completed);
  }
  assert(process == DeltaProcess::active(),  "process consistency check");

  VM_TerminateProcess op(process);
  VMProcess::execute(&op);
  return 0;
}

DeltaProcess::DeltaProcess(oop receiver, symbolOop selector) {
  _receiver    = receiver;
  _selector    = selector;

  _state       = initialized;

  _is_terminating = false;

  _event       = os::create_event(false);
  _thread      = os::create_thread((int (*)(void*)) &launch_delta, (void*) this, &_thread_id);

  _unwind_head = NULL;

  _time_stamp  = 0;

  LOG_EVENT1("creating process %#lx", this);

  set_last_Delta_fp(NULL);
  set_last_Delta_sp(NULL);
  set_last_Delta_pc(NULL);
  Processes::add(this);
}

frame DeltaProcess::profile_top_frame() {
  int*  sp;
  int*  fp;
  char* pc;
  os::fetch_top_frame(_thread, &sp, &fp, &pc);
  frame result((oop*)sp, fp, pc);
  return result;
}


DeltaProcess::~DeltaProcess() {
  processObj()->set_process(NULL);
  if (Processes::includes(this)) {
    Processes::remove(this);
  }
}

void DeltaProcess::print() {
  processObj()->print_value();
  std->print(" ");
  switch (state()) {
    case initialized:             std->print_cr("initialized");            break;
    case running:                 std->print_cr("running");                break;
    case yielded:                 std->print_cr("yielded");                break;
    case in_async_dll:            std->print_cr("in asynchronous dll all");break;
    case yielded_after_async_dll: std->print_cr("yielded after asynchronous dll"); break;
    case preempted:               std->print_cr("preempted");              break;
    case completed:               std->print_cr("completed");              break;
    case boolean_error:           std->print_cr("boolean error");          break;
    case lookup_error:            std->print_cr("lookup error");           break;
    case primitive_lookup_error:  std->print_cr("primitive lookup error"); break;
    case DLL_lookup_error:        std->print_cr("DLL lookup error");       break;
    case NLR_error:               std->print_cr("NLR error");              break;
    case stack_overflow:          std->print_cr("stack overflow");         break;
  }
}

void DeltaProcess::frame_iterate(FrameClosure* blk) {
  blk->begin_process(this);

  if (has_stack()) {
    frame v = last_frame();
    do {
      blk->do_frame(&v);
      v = v.sender();
    } while (!v.is_first_frame());
  }

  blk->end_process(this);
}

void DeltaProcess::oop_iterate(OopClosure* blk) {
  blk->do_oop((oop*) &_receiver);
  blk->do_oop((oop*) &_selector);
  blk->do_oop((oop*) &_processObj);

  for (unwindInfo* p = _unwind_head; p; p = p->next())
    blk->do_oop((oop*) &p->_nlr_result);

  if (has_stack()) {
    frame v = last_frame();
    do {
      v.oop_iterate(blk);
      v = v.sender();
    } while (!v.is_first_frame());
  }
}

symbolOop DeltaProcess::symbol_from_state(ProcessState state) {
  switch(state) {
    case initialized:             return vmSymbols::initialized();
    case yielded:                 return vmSymbols::yielded();
    case running:                 return vmSymbols::running();
    case stopped:                 return vmSymbols::stopped();
    case preempted:               return vmSymbols::preempted();
    case aborted:                 return vmSymbols::aborted();
    case in_async_dll:            return vmSymbols::in_async_dll();
    case yielded_after_async_dll: return vmSymbols::yielded();
    case completed:               return vmSymbols::completed();
    case boolean_error:           return vmSymbols::boolean_error();
    case lookup_error:            return vmSymbols::lookup_error();
    case primitive_lookup_error:  return vmSymbols::primitive_lookup_error();
    case DLL_lookup_error:        return vmSymbols::DLL_lookup_error();
    case float_error:             return vmSymbols::float_error();
    case NLR_error:               return vmSymbols::NLR_error();
    case stack_overflow:          return vmSymbols::stack_overflow();
  }
  return vmSymbols::not_found();
}

bool DeltaProcess::has_stack() const {
  if (state() == initialized) return false;
  if (state() == completed)   return false;
  return true;
}


void DeltaProcess::follow_roots() {
  MarkSweep::follow_root((oop*) &_receiver);
  MarkSweep::follow_root((oop*) &_selector);
  MarkSweep::follow_root((oop*) &_processObj);

  if (has_stack()) {
    frame v = last_frame();
    do {
      v.follow_roots();
      v = v.sender();
    } while (!v.is_first_frame());
  }
}

void DeltaProcess::verify() {
  ResourceMark rm;
  BlockScavenge bs;

  vframe* f = last_delta_vframe();
  if (f == NULL) return;

  // Do not verify the first vframe
  // It will fail if scavenging in allocateContext (Lars)
  for (f = f->sender(); f; f = f->sender()) {
    f->verify();
  }
}


void DeltaProcess::enter_uncommon() {
  assert(_state == running, "must be running");
  _state = uncommon;
}

void DeltaProcess::exit_uncommon() {
  assert(_state == uncommon, "must be uncommon");
  _state = running;
}

//  [-1            ] <-- link to next frame
//  [-1            ] <-- return address
//
//
//
//  [              ] <-- old_sp
//  ...
//  [              ] <--   old_fp

static oop*        old_sp;
static oop*        new_sp;
static int*        old_fp;
static int*        cur_fp;
static objArrayOop frame_array;

extern "C" oop* setup_deoptimization_and_return_new_sp(oop* old_sp, int* old_fp, objArrayOop frame_array, int* current_frame) {
  ResourceMark rm;

  // Save all parameters for later use (check unpack_frame_array)
  ::old_sp      = old_sp;
  ::old_fp      = old_fp;
  ::frame_array = frame_array;
  ::cur_fp      = current_frame;

  smiOop number_of_vframes = smiOop(frame_array->obj_at(StackChunkBuilder::number_of_vframes_index));
  smiOop number_of_locals  = smiOop(frame_array->obj_at(StackChunkBuilder::number_of_locals_index));

  assert(number_of_vframes->is_smi(), "must be smi");
  assert(number_of_locals->is_smi(), "must be smi");

  new_sp = old_sp - frame::interpreter_stack_size(number_of_vframes->value(),
	                                          number_of_locals->value());
  return new_sp;
}


// Used to transfer information from deoptimize_stretch to unpack_frame_array.
static bool redo_the_send;

extern "C" int redo_send_offset = 0;

void DeltaProcess::deoptimize_redo_last_send() {
  redo_the_send = true;
}

// Interpreter entry point for redoing a send.
//extern "C" void redo_bytecode_after_deoptimization();
//extern "C" char redo_bytecode_after_deoptimization;

extern "C" bool       nlr_through_unpacking = false;
extern "C" oop        result_through_unpacking = NULL;
extern "C" int        number_of_arguments_through_unpacking = 0;
extern "C" char*      C_frame_return_addr = NULL;
extern "C" contextOop nlr_home_context;

inline void trace_deoptimization_start() {
  if (TraceDeoptimization) {
    std->print("[Unpacking]");
    if (nlr_through_unpacking) {
      std->print(" NLR %s", (nlr_home == (int) cur_fp) ? "inside" : "outside");
    }
    std->cr();
    std->print(" - array ");
    frame_array->print_value();
    std->print_cr(" @ 0x%lx", old_fp);
  }
}
inline void trace_deoptimization_frame(frame &current, oop* current_sp, char* current_pc) {
  if (TraceDeoptimization) {
    frame v(current_sp, current.fp(), current_pc);
    v.print_for_deoptimization(std);
  }
}

inline void unpack_first_frame(char* &current_pc, frame &current, CodeIterator &c) {
  // first vframe in the array
  if (nlr_through_unpacking) {
    // NLR is comming through unpacked vframes
    current_pc = c.interpreter_return_point();
    // current_pc points to a normal return point in the interpreter.
	// To find the nlr return point we first compute the nlr offset.
    current_pc = ic_info_at(current_pc)->NLR_target();
    current.set_hp(c.next_hp());
  } else if (redo_the_send) {
    // Deoptimizing uncommon trap
    current_pc = Interpreter::redo_bytecode_after_deoptimization();
    current.set_hp(c.next_hp());
	redo_send_offset = c.next_hp() - c.hp();
    redo_the_send = false;
  } else {
    // Normal case
    current_pc = c.interpreter_return_point(true);
    current.set_hp(c.next_hp());

    if (c.is_message_send()) {
	  number_of_arguments_through_unpacking = c.ic()->nof_arguments();
    } else if (c.is_primitive_call()) {
      number_of_arguments_through_unpacking = c.prim_cache()->number_of_parameters();
    } else if (c.is_dll_call()) {
      // The callee should not pop the argument since a DLL call is like a c function call.
      // The continuation code for the DLL call will pop the arguments!
      number_of_arguments_through_unpacking = 0;
    }
  }
}
// Called from assembler in unpack_unoptimized_frames.
// Based on the statics (old_sp, old_fp, and frame_array) this function unpacks
// the array into interpreter frames.
// Returning from this function should activate the most recent deoptimized frame.
extern "C" void unpack_frame_array() {
  BlockScavenge bs;
  ResourceMark rm;

  int* pc_addr = (int*) new_sp - 1;
  assert(*pc_addr = -1, "just checking");

  trace_deoptimization_start();

  bool must_find_nlr_target = nlr_through_unpacking && nlr_home == (int) cur_fp;
  bool nlr_target_found     = false; // For verification

  // link for the current frame
  int* link_addr = (int*) new_sp - 2;

  oop* current_sp = new_sp;
  int  pos        = 3;
  int  length     = frame_array->length();
  bool first = true;
  frame current;
  // unpack one frame at at time from most recent to least recent
  do {
    oop receiver     = frame_array->obj_at(pos++);
    methodOop method = methodOop(frame_array->obj_at(pos++));
    assert(method->is_method(), "expecting method");

    smiOop bci_obj = smiOop(frame_array->obj_at(pos++));
    assert(bci_obj->is_smi(), "expecting smi");
    int bci        = bci_obj->value();

    smiOop locals_obj = smiOop(frame_array->obj_at(pos++));
    assert(locals_obj->is_smi(), "expecting smi");
    int locals   = locals_obj->value();

    current = frame(current_sp, (int*) current_sp + locals + 2);

    // fill in the locals
    for (int index = 0; index < locals; index++) {
      current.set_temp(index, frame_array->obj_at(pos++));
    }

    CodeIterator c(method, bci);

    char* current_pc;

    if (first) {
      unpack_first_frame(current_pc, current, c);
    } else {
      current_pc = c.interpreter_return_point();
      current.set_hp(c.next_hp());
    }
    current.set_receiver(receiver);

    current.patch_pc(current_pc);
    current.patch_fp(current.fp());

    // Revive the contexts
    if (!method->is_blockMethod() && method->activation_has_context()) {
      contextOop con = contextOop(current.temp(0));
      assert(con->is_context(), "must be context");
      oop frame_oop = oop(current.fp());
      con->set_parent(frame_oop);

      if (nlr_through_unpacking && nlr_home == (int) cur_fp) {
        if (nlr_home_context == con) {
	  // This frame is the target of the NLR
	  // set nlr_home to frame pointer of current frame
	  nlr_home = (int) current.fp();
	  // compute number of arguments to pop
          nlr_home_id = ~method->number_of_arguments();
          nlr_target_found = true;
	  // std->print("target frame for NLR (%d, 0x%lx):",method->number_of_arguments(), nlr_home_id);
        }
      }
    }

    trace_deoptimization_frame(current, current_sp, current_pc);

    first = false;
    // Next pc
    current_sp += frame::interpreter_frame_size(locals);

  } while (pos <= length);

  if (must_find_nlr_target && !nlr_target_found) {
    fatal("Target for NLR not found when unpacking frame");
  }

  assert (current_sp == old_sp, "We have not reached the end");
  current.set_link(old_fp);
}

extern "C" void verify_at_end_of_deoptimization() {
  if (PrintStackAfterUnpacking) {
    BlockScavenge bs;
    ResourceMark rm;
    DeltaProcess::active()->verify();
    std->print_cr("[Stack after unpacking]");
    DeltaProcess::active()->trace_stack_for_deoptimization();
  }
}

void DeltaProcess::deoptimize_stretch(frame* first_frame, frame* last_frame) {
  if (TraceDeoptimization) {
    std->print_cr("[Deoptimizing]");
    frame c = *first_frame;
    c.print_for_deoptimization(std);
    while (c.fp() != last_frame->fp()) {
      c = c.sender();
      c.print_for_deoptimization(std);
    }
  }

  StackChunkBuilder packer(first_frame->fp());

  vframe* vf = vframe::new_vframe(first_frame);
  assert(vf->is_compiled_frame(), "must be Delta frame");

  for (deltaVFrame* current = (deltaVFrame*) vf;
       current && (current->fr().fp() <= last_frame->fp());
       current = (deltaVFrame*) current->sender()) {
    packer.append(current);
  }

  // Patch frame
  // - patch the pc first to convert the frame into a deoptimized_frame
  first_frame->patch_pc(StubRoutines::unpack_unoptimized_frames());
  first_frame->set_return_addr(last_frame->return_addr());
  first_frame->set_real_sender_sp(last_frame->sender_sp());
  first_frame->set_frame_array(packer.as_objArray());
  first_frame->set_link(last_frame->link());
}

void DeltaProcess::deoptimized_wrt_marked_nmethods() {
  // chop the stack into stretches of frames in need for deoptimization
  if (!has_stack()) return;

  frame v = last_frame();
  do {
    if (v.should_be_deoptimized())
      deoptimize_stretch(&v, &v);
    v = v.sender();
  } while (!v.is_first_frame());
}

frame DeltaProcess::last_frame() {
  assert(last_Delta_fp(), "must have last_Delta_fp() when suspended");
  if (last_Delta_fp() == NULL) {
	  trace_stack();
  }
  if (last_Delta_pc() == NULL) {
    frame c(last_Delta_sp(), last_Delta_fp());
    return c;
  } else {
    frame c(last_Delta_sp(), last_Delta_fp(), last_Delta_pc());
    return c;
  }
}

deltaVFrame* DeltaProcess::last_delta_vframe() {
  // If no stack is present return NULL
  if (!has_stack()) return NULL;

  frame f = last_frame();
  for (vframe* vf = vframe::new_vframe(&f); vf; vf = vf->sender() ) {
    if (vf->is_delta_frame()) return (deltaVFrame*) vf;
  }
  return NULL;
}


int DeltaProcess::depth() {
  int d = 0;
  for(frame v = last_frame(); v.link(); v = v.sender()) d++;
  return d;
}

int DeltaProcess::vdepth(frame* f) {
  Unimplemented();
  return 0;
}

void DeltaProcess::trace_stack() {
  trace_stack_from(last_delta_vframe());
}

void DeltaProcess::trace_stack_from(vframe* start_frame) {
  std->print_cr("- Stack trace");
  int  vframe_no   = 1;
  for (vframe* f = start_frame; f; f = f->sender() ) {
    if (f->is_delta_frame()) {
      ((deltaVFrame*) f)->print_activation(vframe_no++);
    } else {
      f->print();
    }
    if (vframe_no == StackPrintLimit) {
      std->print_cr("...<more frames>...");
      return;
    }
  }
}

void DeltaProcess::trace_stack_for_deoptimization(frame* f) {
  if (has_stack()) {
    int  vframe_no   = 1;
    frame v = f ? *f : last_frame();
    do {
      v.print_for_deoptimization(std);
      v = v.sender();
      if (vframe_no == StackPrintLimit) {
        std->print_cr("...<more frames>...");
        return;
      }
      vframe_no++;
    } while (!v.is_first_frame());
  }
}

void DeltaProcess::trace_top(int start_frame, int number_of_frames) {
  FlagSetting fs(ActivationShowCode, true);

  std->print_cr("- Stack trace (%d, %d)", start_frame, number_of_frames);
  int  vframe_no = 1;

  for (vframe* f = last_delta_vframe(); f; f = f->sender() ) {
    if (vframe_no >= start_frame) {
      if (f->is_delta_frame()) {
        ((deltaVFrame*) f)->print_activation(vframe_no);
      } else f->print();
      if (vframe_no - start_frame + 1 >= number_of_frames) return;
    }
    vframe_no++;
  }
}

void DeltaProcess::update_nlr_targets(compiledVFrame* f, contextOop con) {
  for (unwindInfo* p = _unwind_head; p; p = p->next()) {
    p->update_nlr_targets(f, con);
  }
}

double DeltaProcess::user_time() {
  return _thread
       ? os:: user_time_for(_thread)
       : 0.0;
}

double DeltaProcess::system_time() {
  return _thread
       ? os:: system_time_for(_thread)
       : 0.0;
}

// ======= Processes ========

DeltaProcess* Processes::processList = NULL;

void Processes::start(VMProcess* p) {
  processList = NULL;
  // activate the vm process
  p->activate_system();
}

void Processes::add(DeltaProcess* p) {
  p->set_next(processList);
  processList = p;
}

#define ALL_PROCESSES(X) for (DeltaProcess* X = processList; X; X = X->next())

DeltaProcess* Processes::find_from_thread_id(int id) {
  ALL_PROCESSES(p)
    if (p->thread_id() == id)
      return p;
  return NULL;
}

void Processes::frame_iterate(FrameClosure* blk) {
  ALL_PROCESSES(p) p->frame_iterate(blk);
}

void Processes::oop_iterate(OopClosure* blk) {
  ALL_PROCESSES(p) p->oop_iterate(blk);
}

void Processes::process_iterate(ProcessClosure* blk) {
  ALL_PROCESSES(p) blk->do_process(p);
}

void Processes::verify() {
  ALL_PROCESSES(p) p->verify();
}

bool Processes::has_completed_async_call() {
  ALL_PROCESSES(p) {
    if (p->state() == yielded_after_async_dll)
      return true;
  }
  return false;
}

void Processes::print() {
  std->print_cr("All processes:");
  ALL_PROCESSES(p) {
    ResourceMark rm;
    p->print();
    p->trace_stack();
  }
}

void Processes::remove(DeltaProcess* p) {
  assert(includes(p), "p must be present");
  DeltaProcess* current = processList;
  DeltaProcess* prev    = NULL;

  while (current != p) {
    prev    = current;
    current = current->next();
  }

  if (prev) {
    prev->set_next(current->next());
  } else {
    processList = p->next();
  }
}

bool Processes::includes(DeltaProcess* p) {
  ALL_PROCESSES(q)
    if (q == p ) return true;
  return false;
}


DeltaProcess* Processes::last() {
  DeltaProcess* last = NULL;
  ALL_PROCESSES(q) last = q;
  return last;
}

void Processes::kill_all() {
  DeltaProcess* current = processList;
  while (current) {
    DeltaProcess* next = current->next();
    VMProcess::terminate(current);
    current->set_next(NULL);
    delete current;
    current = next;
  }
  processList = NULL;
}

class ScavengeOopClosure : public OopClosure {
  void do_oop(oop* o) { SCAVENGE_TEMPLATE(o); }
};

void Processes::scavenge_contents() {
  ScavengeOopClosure blk;
  oop_iterate(&blk);
}

void Processes::follow_roots() {
  ALL_PROCESSES(p) p->follow_roots();
}

class ConvertHCodePointersClosure : public FrameClosure {
  void do_frame(frame* f) {
    if (f->is_interpreted_frame()) {
      f->convert_hcode_pointer();
    }
  }
};

void Processes::convert_hcode_pointers() {
  ConvertHCodePointersClosure blk;
  frame_iterate(&blk);
}

class RestoreHCodePointersClosure : public FrameClosure {
  void do_frame(frame* f) {
    if (f->is_interpreted_frame()) {
      f->restore_hcode_pointer();
    }
  }
};

void Processes::restore_hcode_pointers() {
  RestoreHCodePointersClosure blk;
  frame_iterate(&blk);
}

void Processes::deoptimized_wrt_marked_nmethods() {
  StackChunkBuilder::begin_deoptimization();
  ALL_PROCESSES(p) p->deoptimized_wrt_marked_nmethods();
  StackChunkBuilder::end_deoptimization();
}

void Processes::deoptimize_wrt(nmethod* nm) {
  GrowableArray<nmethod*>* nms = nm->invalidation_family();
  // mark family for deoptimization
  for (int index = 0; index < nms->length(); index++)
    nms->at(index)->mark_for_deoptimization();

  // deoptimize
  deoptimized_wrt_marked_nmethods();

  // unmark for deoptimization
  for (index = 0; index < nms->length(); index++)
    nms->at(index)->unmark_for_deoptimization();
}

void Processes::deoptimize_wrt(GrowableArray<nmethod*>* list) {
  // mark for deoptimization
  for (int i = 0; i < list->length(); i++) {
    nmethod* nm = list->at(i);
    GrowableArray<nmethod*>* nms = nm->invalidation_family();
    for (int index = 0; index < nms->length(); index++)
      nms->at(index)->mark_for_deoptimization();
  }

  // deoptimize
  deoptimized_wrt_marked_nmethods();

  // unmark for deoptimization
  for (i = 0; i < list->length(); i++) {
    nmethod* nm = list->at(i);
    GrowableArray<nmethod*>* nms = nm->invalidation_family();
    for (int index = 0; index < nms->length(); index++)
      nms->at(index)->unmark_for_deoptimization();
  }
}

void Processes::update_nlr_targets(compiledVFrame* f, contextOop con) {
 ALL_PROCESSES(p) p->update_nlr_targets(f, con);
}

void Processes::deoptimize_all() {
  Universe::code->mark_all_for_deoptimization();
  deoptimized_wrt_marked_nmethods();
  Universe::code->unmark_all_for_deoptimization();
}


void handle_error(ProcessState error) {
  DeltaProcess* proc = DeltaProcess::active();
  if (proc->is_scheduler()) {
    std->print_cr("Error happend in the scheduler");
    std->print("Status: ");
    proc->status_symbol()->print_symbol_on(std);
    std->cr();
    evaluator::read_eval_loop();
  } else {
    proc->suspend(error);
  }
  ErrorHandler::abort_current_process();
  ShouldNotReachHere();
}

void handle_interpreter_error(char* message) {
  warning("Interpreter error: %s", message);
  handle_error(stopped);
}


extern "C" void suspend_on_error(InterpreterErrorConstants error_code) {
  // Called from the the interpreter

  // Real errors
  switch (error_code) {
    case primitive_lookup_failed: handle_error(primitive_lookup_error);
    case boolean_expected	: handle_error(boolean_error);
    case nonlocal_return_error	: handle_error(NLR_error);
    case float_expected		: handle_error(float_error);
  }

  // Interpreter errors
  switch (error_code) {
    case halted			: handle_interpreter_error("executed halt bytecode");
    case illegal_code		: handle_interpreter_error("illegal code");
    case not_implemented	: handle_interpreter_error("not implemented");
    case stack_missaligned	: handle_interpreter_error("stack misaligned");
    case ebx_wrong		: handle_interpreter_error("ebx wrong");
    case obj_wrong		: handle_interpreter_error("obj wrong");
    case nlr_offset_wrong	: handle_interpreter_error("NLR offset wrong");
    case last_Delta_fp_wrong	: handle_interpreter_error("last Delta frame wrong");
    case primitive_result_wrong	: handle_interpreter_error("ilast C entry frame wrong");
  }
  ShouldNotReachHere();
}

extern "C" void suspend_on_NLR_error() {
  // Called from compiled code
  DeltaProcess::active()->suspend(NLR_error);
}


void trace_stack_at_exception(int* sp, int* fp, char* pc) {
  ResourceMark rm;

  std->print_cr("Trace at exception");

  vframe* vf;
  if (last_Delta_fp) {
    frame c(last_Delta_sp, last_Delta_fp);
    vf = vframe::new_vframe(&c);
  } else {
    frame c((oop*) sp, fp, pc);
    vf = vframe::new_vframe(&c);
  }
  DeltaProcess::trace_stack_from(vf);
}

void suspend_process_at_stack_overflow(int *sp, int* fp, char* pc) {
  DeltaProcess* proc = DeltaProcess::active();

  proc->set_last_Delta_pc(pc);
  last_Delta_fp = fp;
  last_Delta_sp = (oop*) sp;

  if (proc->is_scheduler()) {
    std->print_cr("Stack overflow happened in scheduler");
  } else {
    proc->suspend(stack_overflow);
    proc->set_terminating();
  }
}

void trace_stack(int thread_id) {
	ResourceMark rm;
	Process* process = Processes::find_from_thread_id(thread_id);
	if (process->is_deltaProcess())
		((DeltaProcess*)process)->trace_stack();
}
