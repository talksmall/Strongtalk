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

// Primitives for processes

class processOopPrimitives : AllStatic {
 private: 
  static void inc_calls() { number_of_calls++; }
 public:
  static int number_of_calls;

  //%prim
  // <Process class> primitiveProcessCreate: block <BlockWithoutArguments>
  //                                 ifFail: failBlock <PrimFailBlock> ^<Process> =
  //   Internal { doc   = 'Creates a new process'
  //              error = #(ProcessAllocationFailed)
  //              name  = 'processOopPrimitives::create' }
  //%
  static PRIM_DECL_2(create, oop receiver, oop block);

  //%prim
  // <NoReceiver> primitiveProcessYield ^<Process> =
  //   Internal { doc   = 'Yields the control to the scheduler.'
  //              doc   = 'Does nothing if executed by the scheduler.'
  //              doc   = 'Returns current process when regaining control.' 
  //              name  = 'processOopPrimitives::yield' }
  //%
  static PRIM_DECL_0(yield);

  //%prim
  // <NoReceiver> primitiveProcessStop ^<Process> =
  //   Internal { doc   = 'Yields the control to the scheduler.'
  //              doc   = 'Does nothing if executed by the scheduler.'
  //              doc   = 'Returns current process when regaining control.' 
  //              name  = 'processOopPrimitives::stop' }
  //%
  static PRIM_DECL_0(stop);

  //%prim
  // <NoReceiver> primitiveProcessActiveProcess ^<Object> =
  //   Internal { doc   = 'Returns the active process'
  //              name  = 'processOopPrimitives::activeProcess' }
  //%
  static PRIM_DECL_0(activeProcess);

  //%prim
  // <NoReceiver> primitiveProcessTransferTo: process   <Process> 
  //                                  ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { doc   = 'Transfers the control from the scheduler to a process.'
  //              doc   = 'Returns the status of process (see primitiveProcessStatus).'
  //              error = #(NotInScheduler ProcessCannotContinue Dead)
  //              name  = 'processOopPrimitives::transferTo' }
  //%
  static PRIM_DECL_1(transferTo, oop process);

  //%prim
  // <Process> primitiveProcessSetMode: mode       <Symbol>
  //                        activation: activation <Activation>
  //                       returnValue: value      <Object>
  //                            ifFail: failBlock  <PrimFailBlock> ^<Symbol> =
  //   Internal { doc   = 'Change the mode of the process. mode: #Normal #Step #StepNext #StepEnd #Revert #Return'
  //              doc   = 'Returns the old mode of the process.'
  //              error = #(InScheduler Dead)
  //              name  = 'processOopPrimitives::set_mode' }
  //%
  static PRIM_DECL_3(set_mode, oop process, oop mode, oop value);

  //%prim
  // <NoReceiver> primitiveProcessStartEvaluator: process   <Process> 
  //                                      ifFail: failBlock <PrimFailBlock> ^<Object> =
  //   Internal { doc   = 'Transfers the control from the scheduler to a process,'
  //              doc   = 'and enters the evaluator.'
  //              doc   = 'Returns the status of process (see primitiveProcessStatus).'
  //              error = #(NotInScheduler ProcessCannotContinue Dead)
  //              name  = 'processOopPrimitives::start_evaluator' }
  //%
  static PRIM_DECL_1(start_evaluator, oop process);

  //%prim
  // <Process> primitiveProcessTerminateIfFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { doc   = 'Terminates the process'
  //              error = #(Dead)
  //              flags = #(NLR)
  //              name  = 'processOopPrimitives::terminate' }
  //%
  static PRIM_DECL_1(terminate, oop receiver);

  //%prim
  // <Process> primitiveProcessStatus ^<Symbol> =
  //   Internal { doc  = 'Returns the process status:'
  //              doc  = '  #Initialized            - State right after creation.'
  //              doc  = '  #Yielded                - Gave up control by calling yield.'
  //              doc  = '  #Running                - Is running.'
  //              doc  = '  #InAsyncDLL             - Gave up control but continues to execute asynchronous DLL.'
  //              doc  = '  #Stopped                - Gave up control by calling stop.'
  //              doc  = '  #Preempted              - Was preempted by system.'
  //              doc  = '  #Completed              - Ran to completion.'
  //              doc  = '  #Dead                   - The process is dead.'
  //              doc  = '  #BooleanError           - A boolean was expected at hardcoded control structure.'
  //              doc  = '  #FloatError             - A float was expected at hardcoded float operation.'
  //              doc  = '  #LookupError            - The receiver does not understand doesNotUnderstand:.'
  //              doc  = '  #PrimitiveLookupError   - Binding a primitive failed.'
  //              doc  = '  #DLLLookupError         - Binding a DLL function failed.'
  //              doc  = '  #NLRError               - Context for NLR did not exist.'
  //              doc  = '  #StackOverflow          - Stack exhausted.'
  //              name = 'processOopPrimitives::status' }
  //%
  static PRIM_DECL_1(status, oop process);
  
  //%prim
  // <Process> primitiveProcessSchedulerWait: milliseconds <SmallInteger>
  //                                  ifFail: failBlock <PrimFailBlock> ^<Boolean> =
  //   Internal { doc   = 'Waits until timer has expired or a asynchronous dll call has returned.'
  //              doc   = 'Returns whether the timer expired.'
  //              name  = 'processOopPrimitives::scheduler_wait' }
  //%
  static PRIM_DECL_1(scheduler_wait, oop milliseconds);

  //%prim
  // <Process> primitiveProcessTraceStack: size <SmallInteger>
  //                               ifFail: failBlock <PrimFailBlock> ^<Self> =
  //   Internal { doc    = 'Prints the stack trace'
  //              errors = #(Dead)
  //              name   = 'processOopPrimitives::trace_stack' }
  //%
  static PRIM_DECL_2(trace_stack, oop receiver, oop size);

  //%prim
  // <NoReceiver> primitiveProcessEnterCritical ^<Process> =
  //   Internal { doc    = 'Disables preemption in active process.'
  //              name   = 'processOopPrimitives::enter_critical' }
  //%
  static PRIM_DECL_0(enter_critical);

  //%prim
  // <NoReceiver> primitiveProcessLeaveCritical ^<Process> =
  //   Internal { doc    = 'Enables preemption in active process'
  //              name   = 'processOopPrimitives::leave_critical' }
  //%
  static PRIM_DECL_0(leave_critical);

  //%prim
  // <NoReceiver> primitiveProcessYieldInCritical ^<Process> =
  //   Internal { doc   = 'Yields the control to the scheduler.'
  //              doc   = 'Does nothing if executed by the scheduler.'
  //              doc   = 'Returns current process when regaining control.' 
  //              name  = 'processOopPrimitives::yield_in_critical' }
  //%
  static PRIM_DECL_0(yield_in_critical);

  //%prim
  // <Process> primitiveProcessUserTime ^<Float> =
  //   Internal { doc   = 'Returns time, in seconds, the process has spent in user code' 
  //              name  = 'processOopPrimitives::user_time' }
  //%
  static PRIM_DECL_1(user_time, oop receiver);

  //%prim
  // <Process> primitiveProcessSystemTime ^<Float> =
  //   Internal { doc   = 'Returns time, in seconds, the process has spent in system code' 
  //              name  = 'processOopPrimitives::user_time' }
  //%
  static PRIM_DECL_1(system_time, oop receiver);

  //%prim
  // <Process> primitiveProcessStackLimit: limit <SmallInteger>
  //                               ifFail: failBlock <PrimFailBlock> ^<IndexedInstanceVariables> =
  //   Internal { errors = #(Dead Running)
  //              doc    = 'Returns an array of the process activation.'
  //              doc    = 'The primitive fails if either the receiver is a dead process or'
  //              doc    = 'if it is the active process'
  //              name   = 'processOopPrimitives::stack' }
  //%
  static PRIM_DECL_2(stack, oop receiver, oop limit);

};
