/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.24 $ */
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
# include "incls/_process_prims.cpp.incl"

TRACE_FUNC(TraceProcessPrims, "process")

int processOopPrimitives::number_of_calls;

#define ASSERT_RECEIVER assert(receiver->is_process(), "receiver must be process")

PRIM_DECL_2(processOopPrimitives::create, oop receiver, oop block) {
  PROLOGUE_2("create", receiver, block)
  assert(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_process(), "must be process class");
  if (block->klass() != Universe::zeroArgumentBlockKlassObj())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  processOop process = processOop(receiver->primitive_allocate());
  assert(process->is_process(), "must be process");
 
  DeltaProcess* p = new DeltaProcess(block, oopFactory::new_symbol("value"));
  process->set_process(p);
  p->set_processObj(process);
  return process;
}

PRIM_DECL_0(processOopPrimitives::yield) {
  PROLOGUE_0("yield");
  if (!DeltaProcess::active()->is_scheduler()) {
    DeltaProcess::active()->suspend(yielded);
  }
  return DeltaProcess::active()->processObj();
}

class PrintFrameClosure : public FrameClosure {
  void do_frame(frame* f) {
    f->print();
    if (f->is_compiled_frame()) {
      f->code()->print();
    } else if (f->is_interpreted_frame()) {
      f->method()->print();
    }
  }
};

void print_nmethod(nmethod* nm) {
  nm->print();
}

PRIM_DECL_0(processOopPrimitives::stop) {
  PROLOGUE_0("stop");
  //PrintFrameClosure pfc;
  //DeltaProcess::active()->frame_iterate(&pfc);
  //Universe::code->nmethods_do(print_nmethod);
  if (!DeltaProcess::active()->is_scheduler()) {
    DeltaProcess::active()->suspend(stopped);
  }
  return DeltaProcess::active()->processObj();
}
PRIM_DECL_1(processOopPrimitives::setMainProcess, oop receiver) {
  PROLOGUE_1("setMainProcess", receiver);

  if (!receiver->is_process())
    return markSymbol(vmSymbols::receiver_has_wrong_type());

  processOop process = processOop(receiver);
  DeltaProcess::main()->set_processObj(process);
  process->set_process(DeltaProcess::main());

  return receiver;
}
PRIM_DECL_1(processOopPrimitives::transferTo, oop process) {
  PROLOGUE_1("transferTo", process);

  // Check if argument is a processOop
  if (!process->is_process())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // Check if we are in the scheduler
  if (!DeltaProcess::active()->is_scheduler())
    return markSymbol(vmSymbols::not_in_scheduler());

  // Make sure process is not dead
  if (!processOop(process)->is_live())
    return markSymbol(vmSymbols::dead());

  DeltaProcess* proc = processOop(process)->process();

  // Do the transfer (remember transfer_to is a no-op if process is in async DLL)
  ProcessState state = DeltaProcess::scheduler()->transfer_to(proc);

  // Please be careful here since a scavenge/garbage collect can happen!

  return DeltaProcess::symbol_from_state(state);
}

PRIM_DECL_3(processOopPrimitives::set_mode, oop process, oop mode, oop value) {
  PROLOGUE_3("set_mode", process, mode, value);

  // Check if argument is a processOop
  if (!process->is_process())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // Check if we are in the scheduler
  if (!DeltaProcess::active()->is_scheduler())
    return markSymbol(vmSymbols::not_in_scheduler());

  // Make sure process is not dead
  if (!processOop(process)->is_live())
    return markSymbol(vmSymbols::dead());

  DeltaProcess* proc = processOop(process)->process();

  // FIX THIS
  //  proc->set_single_step_mode();

  // Do the transfer (remember transfer_to is a no-op if process is in async DLL)
  ProcessState state = DeltaProcess::scheduler()->transfer_to(proc);

  // Please be careful here since a scavenge/garbage collect can happen!

  // FIX THIS
  //  proc->set_normal_mode();

  return DeltaProcess::symbol_from_state(state);
}
extern "C" void single_step_handler();

PRIM_DECL_1(processOopPrimitives::start_evaluator, oop process) {
  PROLOGUE_1("start_evaluator", process);

  // Check if argument is a processOop
  if (!process->is_process())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // Check if we are in the scheduler
  if (!DeltaProcess::active()->is_scheduler())
    return markSymbol(vmSymbols::not_in_scheduler());

  // Make sure process is not dead
  if (!processOop(process)->is_live())
    return markSymbol(vmSymbols::dead());

  DeltaProcess* proc = processOop(process)->process();
  {
    ResourceMark rm;
    StubRoutines::setSingleStepHandler(&single_step_handler);
    dispatchTable::intercept_for_step(NULL);
  }
  ProcessState state = DeltaProcess::scheduler()->transfer_to(proc);

  return DeltaProcess::symbol_from_state(state);
}

PRIM_DECL_1(processOopPrimitives::terminate, oop receiver) {
  PROLOGUE_1("terminate", receiver);
  ASSERT_RECEIVER;

  // Make sure process is not dead
  if (!processOop(receiver)->is_live())
    return markSymbol(vmSymbols::dead());

  DeltaProcess* proc = processOop(receiver)->process();
  if (proc == DeltaProcess::active()) {
    // Start the abort now
    ErrorHandler::abort_current_process();
  } else {
    proc->set_terminating();
  }
  return receiver;
}

PRIM_DECL_0(processOopPrimitives::activeProcess) {
  PROLOGUE_0("activeProcess");

  processOop p = DeltaProcess::active()->processObj();
  assert(p->is_process(), "must be process");

  return p;
}

PRIM_DECL_1(processOopPrimitives::status, oop process) {
  PROLOGUE_1("status", process);

  // Check if argument is a processOop
  if (!process->is_process())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return processOop(process)->status_symbol();
}

PRIM_DECL_1(processOopPrimitives::scheduler_wait, oop milliseconds) {
  PROLOGUE_1("scheduler_wait", milliseconds);

  // Check if argument is a smi
  if (!milliseconds->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return DeltaProcess::wait_for_async_dll(smiOop(milliseconds)->value()) ? trueObj : falseObj;
}

PRIM_DECL_2(processOopPrimitives::trace_stack, oop receiver, oop size) {
  PROLOGUE_2("trace_stack", receiver, size);
  ASSERT_RECEIVER;

  // Check argument
  if (!size->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // Make sure process is not dead
  if (!processOop(receiver)->is_live())
    return markSymbol(vmSymbols::dead());

  // Print the stack
  { ResourceMark rm; 
    processOop(receiver)->process()->trace_top(1, smiOop(size)->value());
  }

  return receiver;
}

PRIM_DECL_0(processOopPrimitives::enter_critical) {
  PROLOGUE_0("enter_critical");
  // %fix this when implementing preemption
  return DeltaProcess::active()->processObj();
}

PRIM_DECL_0(processOopPrimitives::leave_critical) {
  PROLOGUE_0("leave_critical");
  // %fix this when implementing preemption
  return DeltaProcess::active()->processObj();
}

PRIM_DECL_0(processOopPrimitives::yield_in_critical) {
  PROLOGUE_0("yield_in_critical");
  // %fix this when implementing preemption
  if (!DeltaProcess::active()->is_scheduler()) {
    DeltaProcess::active()->suspend(yielded);
  }
  return DeltaProcess::active()->processObj();
}

PRIM_DECL_1(processOopPrimitives::user_time, oop receiver) {
  PROLOGUE_1("enter_critical", receiver);
  ASSERT_RECEIVER;
  return oopFactory::new_double(processOop(receiver)->user_time());
}

PRIM_DECL_1(processOopPrimitives::system_time, oop receiver) {
  PROLOGUE_1("enter_critical", receiver);
  ASSERT_RECEIVER;
  return oopFactory::new_double(processOop(receiver)->system_time());
}

PRIM_DECL_2(processOopPrimitives::stack, oop receiver, oop limit) {
  PROLOGUE_2("stack", receiver, limit);
  ASSERT_RECEIVER;

  // Check type of limit
  if (!limit->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // Make sure process is not dead
  if (!processOop(receiver)->is_live())
    return markSymbol(vmSymbols::dead());

  // Make sure we are not retrieving activation for the current process
  if (processOop(receiver)->process() == DeltaProcess::active())
    return markSymbol(vmSymbols::running());


  ResourceMark rm;
  BlockScavenge bs;

  int                     l = smiOop(limit)->value();
  processOop        process = processOop(receiver);
  GrowableArray<oop>* stack = new GrowableArray<oop> (100);
  
  vframe* vf = processOop(receiver)->process()->last_delta_vframe();

  for (int index = 1; index <= l && vf; index++) {
    stack->push(oopFactory::new_vframe(process, index));
    vf = vf->sender();
  }
  return oopFactory::new_objArray(stack);
}
