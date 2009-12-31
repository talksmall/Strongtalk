/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.65 $ */
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

// unwindInfo is a wrapper calls allowing a primitive like unwindprotect
// to call delta even though a non local return is in progress.

class unwindInfo : public StackObj {
 private:
  // NLR state
  int        _nlr_home;
  int        _nlr_home_id;
  contextOop _nlr_home_context; 
 public:
  oop        _nlr_result;
 private:

  bool _is_compiled;

  // Link to next unwindinfo
  unwindInfo* _next;

  // Return address patch state
  char*  saved_C_frame_return_addr;
  char** saved_C_frame_return_addr_location;
  char*  saved_patch_return_address;
 public:
  unwindInfo();
  ~unwindInfo();

  unwindInfo* next() const        { return _next; }   
  void set_next(unwindInfo* next) { _next = next; }

  int        nlr_home()         const { return _nlr_home;         }
  int        nlr_home_id()      const { return _nlr_home_id;      }
  contextOop nlr_home_context() const { return _nlr_home_context; }
  void update_nlr_targets(compiledVFrame* f, contextOop con);
};

// Class hierarchy
// - Process
//   - VMProcess
//   - DeltaProcess

class Process: public PrintableCHeapObj {
 public:
  // testers
  virtual bool is_vmProcess()    const { return false; }
  virtual bool is_deltaProcess() const { return false; }
  virtual void resetStepping() {};
  virtual void applyStepping() {};
  // operations
  void abort();

  int thread_id() const { return _thread_id; }

  static bool external_suspend_current();
  static void external_resume_current();

  // sets the current process
  static void set_current(Process* p) { _current_process = p; }
  // returns the running process DeltaProcess or VMProcess
  static Process* current()        { return _current_process; } 

 protected:
  // transfer to control
  void basic_transfer(Process* target);

  // OS data associated with the process
  Thread* _thread;     // Native thread
  int     _thread_id;  // Native thread id (set by OS when created)
  Event*  _event;      // Thread lock
  char*   _stack_limit;// lower limit of stack

  static Process* _current_process;       //  active Delta process or vm process
};


// A single VMProcess (the default thread) is used for heavy 
// vm operations like scavenge, garbage_collect etc.
class VMProcess: public Process {
 public:
  // constructor
  VMProcess();

  // tester
  bool is_vmProcess() const { return true; }
  
  // activates the virtual machine
  void activate_system();

  // the ever running loop for the VMProcess
  void loop();

  // transfer 
  void transfer_to(DeltaProcess* target_process);

  // misc.
  void print();

  // Terminate  
  static void terminate(DeltaProcess* proc);

  // execution of vm operation
  static void execute(VM_Operation* op);

  // returns the current vm operation if any.
  static VM_Operation* vm_operation() { return _vm_operation; }

  // returns the single instance of VMProcess.
  static VMProcess* vm_process() { return _vm_process; }

 private:
  static VM_Operation* _vm_operation;
  static VMProcess*    _vm_process;
};


enum ProcessState {
  initialized, 			// State right after creation.
  running,                      // The process is running.
  yielded,			// Gave up control by calling yield.
  stopped,                      // Gave up control by calling stop.
  preempted,			// Was preempted by system.
  uncommon,			// currently handling an uncommon branch
  in_async_dll,                 // currently executing an asynchronous dll call
  yielded_after_async_dll,      // completed executution of asynchronous dll call
  completed,			// Ran to completion.
  boolean_error,		// A boolean was expected at hardcoded control structure.
  lookup_error,			// The receiver does not understand doesNotUnderstand:.
  primitive_lookup_error,	// Binding a primitive failed.
  DLL_lookup_error,		// Binding a DLL function failed.
  float_error,			// A float was expected at hardcoded float operation.
  NLR_error,			// Context for NLR did not exist.
  stack_overflow,		// Stack exhausted.
  aborted	                // Process has been aborted
};

class DebugInfo: public ValueObj {
 private:
  void (*interceptorEntryPoint)(int*); // entry point of the dispatch table entry point
  int* frameBreakpoint;                // pointer to the target frame for stepping, if any
 public:
  DebugInfo() : interceptorEntryPoint(NULL), frameBreakpoint(NULL) {};
  void interceptForStep();
  void interceptForNext(int*fr);
  void interceptForReturn(int*fr);
  void apply();
  void reset();
  void resetInterceptor() {
    interceptorEntryPoint = NULL; // %TODO: should we use breakpoint() or similar?
    frameBreakpoint = NULL;
  }
};

extern "C" char* active_stack_limit();
extern "C" void check_stack_overflow();

class DeltaProcess: public Process {
 private:
  oop           _receiver;           // receiver of the initial message.
  symbolOop     _selector;           // selector of the initial message.
  DeltaProcess* _next;               // the next process in the list (see Processes).
  processOop    _processObj;         // the Delta level process object.
  ProcessState  _state;              // process state.

  int*          _last_Delta_fp;
  oop*          _last_Delta_sp;
  char*         _last_Delta_pc;      // For now only used for stack overflow

  volatile bool _is_terminating;

  int           _time_stamp;
  DebugInfo     debug;               // debug info used while stepping
  friend class VMProcess;
 public:
  static bool stepping;
  // constructor
  DeltaProcess(oop receiver, symbolOop selector);
  ~DeltaProcess();

  virtual void applyStepping() {
    debug.apply();
  }
  virtual void resetStepping() {
    debug.reset();
  }
  void setupSingleStep() {
    debug.interceptForStep();
  }
  void setupStepNext(int* fr) {
    debug.interceptForNext(fr);
  }
  void setupStepReturn(int* fr) {
    debug.interceptForReturn(fr);
  }
  void resetStep() {
    debug.resetInterceptor();
  }
  void returnToDebugger();
  // testers
  bool is_deltaProcess() const { return true;  }
  bool isUncommon()      const { return _state == uncommon; }

  // Accessors
  oop       receiver() const { return _receiver; }
  symbolOop selector() const { return _selector; }

  // process chain operations
  DeltaProcess* next() const	 { return _next; }
  void set_next(DeltaProcess* p) { _next = p; }

  // process oop
  processOop processObj() const { return _processObj; }
  void set_processObj(processOop p) { _processObj = p; }

  bool is_terminating()  { return _is_terminating; }
  void set_terminating() { _is_terminating = true; }

  int time_stamp() const { return _time_stamp; }

  void inc_time_stamp()  { _time_stamp++; }

  // last_Delta_fp
  int* last_Delta_fp() const;
  void set_last_Delta_fp(int* fp);

  // last_Delta_sp
  oop* last_Delta_sp() const;
  void set_last_Delta_sp(oop* sp);

  // last_Delta_pc
  char* last_Delta_pc() const;
  void set_last_Delta_pc(char* pc);

  ProcessState state() const { return _state; }

  symbolOop status_symbol() const { return symbol_from_state(state()); }

  static symbolOop symbol_from_state(ProcessState state);

 // List of active unwind protect activations for this process.
 // We need this list for deoptimization. If a compiled frame
 // is replaced with a deoptimized frame and the unwind protect
 // has nlr_target in the compiled frame, we have to convert
 // the nlr information to match the deoptimized frame.
 public:
  void push_unwind(unwindInfo *info) {
    info->set_next(_unwind_head);
    _unwind_head = info;
  }

  void pop_unwind() {
    _unwind_head = _unwind_head->next();
  }

  void unwinds_do(void f(unwindInfo* info)) {
    for (unwindInfo* p = _unwind_head; p; p = p->next())
      f(p);
  };

  void update_nlr_targets(compiledVFrame* f, contextOop con);

 private:
  unwindInfo* _unwind_head; // points to the most recent unwind protect activation.

 private:
  void set_state(ProcessState s) { _state = s; }

 public:
  // State operations
  
  // returns whether this process has a stack.
  bool has_stack() const;
  // returns whether this process is ready for execution.
  bool is_ready() const { return state() == initialized || state() == yielded; }

  // Memory operations
  void follow_roots();

  // Iterator
  void frame_iterate(FrameClosure* blk);
  void oop_iterate(OopClosure* blk);

  // Deoptimization  
  virtual void deoptimized_wrt_marked_nmethods();
  void deoptimize_stretch(frame* first_frame, frame* last_frame);
  static void deoptimize_redo_last_send();

  // Uncommon branches
  void enter_uncommon();
  void exit_uncommon();

  // Misc. operations
  void print();
  void verify();

  // Stack operations
  frame        last_frame();
  deltaVFrame* last_delta_vframe();

  // Print the stack trace
  void trace_stack();
  void trace_top(int start_frame, int number_of_frames);
  void trace_stack_for_deoptimization(frame* f = NULL);

  // Print the stack starting at top_frame 
  static void trace_stack_from(vframe* top_frame);

  // Timing 
  double user_time();
  double system_time();

  int depth();
  int vdepth(frame* f = NULL);

#ifdef unused
  // Debugging state
  bool stepping;	// are we in single-step mode?
  bool stopping;	// just returned from "finish" operation; stop ASAP
#endif

  // Profiling operation (see fprofile.cpp)
  frame profile_top_frame();

 private:
  void transfer(ProcessState reason, DeltaProcess* destination);

 public:

  // returns whether this process is the active delta process.
  bool is_active()       const { return this == active(); }

  // returns whether this process is the scheduler.
  bool is_scheduler()    const { return this == scheduler(); }

  // returns whether this process currently is executing a vm_operation.
  bool in_vm_operation() const { return is_active() && VMProcess::vm_operation() != NULL; }

  // transfer control to the scheduler.
  void suspend(ProcessState reason);
  void suspend_at_creation();

  // asynchronous dll support
  void transfer_and_continue();
  void wait_for_control();

  // transfers control from the scheduler.
  ProcessState transfer_to(DeltaProcess* target);

  // transfers control to vm process.
  void transfer_to_vm();

 // Static operations
 private:
  static DeltaProcess*  _active_delta_process; 
  static DeltaProcess*  _scheduler_process;
  static bool           _is_idle;
  static volatile char* _active_stack_limit;
  static volatile bool  _interrupt;

  // The launch function for a new thread
  static int launch_delta(DeltaProcess* process);

 public:
  // sets the active process (note: public only to support testing)
  static void set_active(DeltaProcess* p) {
    _active_delta_process = p;
    _active_stack_limit = p->_stack_limit;

    if (_active_delta_process->state() != uncommon) {
      _active_delta_process->set_state(running);
    }
  }

  // sets the scheduler process
  static void set_scheduler(DeltaProcess* p) { _scheduler_process = p; }
    
  // returns the active delta process
  static DeltaProcess* active()    { return _active_delta_process; }

  // tells whether the system is idle (waiting in wait_for_async_dll).
  static bool is_idle() { return _is_idle; }

  // returns the scheduler process
  static DeltaProcess* scheduler() { return _scheduler_process; }

  static void set_terminating_process(ProcessState state) {
    _state_of_terminated_process = state;
    _process_has_terminated      = true;
  }

  static bool process_has_terminated() {
    bool result = _process_has_terminated;
    _process_has_terminated = false;
    return result;
  }
  static ProcessState state_of_terminated_process() {
    return _state_of_terminated_process;
  }

 private: 
  static volatile bool _process_has_terminated;
  static ProcessState  _state_of_terminated_process;
  static void check_stack_overflow();
 public:
  // Called whenever a async dll call completes
  static void async_dll_call_completed();
  static void initialize_async_dll_event();
  // Waits for a completed async call or timeout.
  // Returns whether the timer expired. 
  static bool wait_for_async_dll(int timeout_in_ms);
  static void preempt_active();
 private:
  // Event for waking up the process scheduler when a
  // async dll completes
  static Event* _async_dll_completion_event;
  friend class VMProcess; // to allow access to _process_has_terminated
  friend class InterpreterGenerator;
  friend char* active_stack_limit();
  friend void check_stack_overflow();
};


class Processes: AllStatic {
 private:
  static DeltaProcess* processList;
 public:
  // Process management
  static void add(DeltaProcess* p);
  static void remove(DeltaProcess* p);
  static bool includes(DeltaProcess* p);
  static DeltaProcess* last();

  static DeltaProcess* find_from_thread_id(int id);

  // Start the vm process
  static void start(VMProcess* p);

  // State
  static bool has_completed_async_call();

  // Killing
  static void kill_all();
   
  // Iterating
  static void frame_iterate(FrameClosure* blk);
  static void oop_iterate(OopClosure* blk);
  static void process_iterate(ProcessClosure* blk);
  
  // Scavenge
  static void scavenge_contents();

  // Garbage collection
  static void follow_roots();
  static void convert_hcode_pointers();
  static void restore_hcode_pointers();

  // Verifycation
  static void verify();
  static void print();

  // Deoptimization
 public:
  // deoptimizes frames dependent on a nmethod
  static void deoptimize_wrt(nmethod* nm); 
  // deoptimizes frames dependent on at least one nmethod in the list                 
  static void deoptimize_wrt(GrowableArray<nmethod*>* list);
  // deoptimizes all frames
  static void deoptimize_all();
  // deoptimizes all frames tied to marked nmethods
  static void deoptimized_wrt_marked_nmethods();

  // deoptimization support for NLR
  static void update_nlr_targets(compiledVFrame* f, contextOop con);
};


// "semaphore" to protect some vm critical sections (process transfer etc.)
extern "C" bool processSemaphore; 

extern "C" int* last_Delta_fp;
extern "C" oop* last_Delta_sp;

extern int CurrentHash;

extern "C" void set_stack_overflow_for(DeltaProcess* currentProcess);

// error handling routines called from compiled code

extern "C" void suspend_on_NLR_error();


enum InterpreterErrorConstants {
  start_of_runtime_system_errors =      512,
  primitive_lookup_failed	 =  1 + start_of_runtime_system_errors,
  boolean_expected		 =  2 + start_of_runtime_system_errors,
  nonlocal_return_error		 =  3 + start_of_runtime_system_errors,
  halted			 =  4 + start_of_runtime_system_errors,
  illegal_code         		 =  5 + start_of_runtime_system_errors,
  not_implemented    		 =  6 + start_of_runtime_system_errors,
  stack_missaligned   		 =  7 + start_of_runtime_system_errors,
  ebx_wrong			 =  8 + start_of_runtime_system_errors,
  obj_wrong			 =  9 + start_of_runtime_system_errors,
  nlr_offset_wrong		 = 10 + start_of_runtime_system_errors,
  last_Delta_fp_wrong	         = 11 + start_of_runtime_system_errors,
  primitive_result_wrong	 = 12 + start_of_runtime_system_errors,
  float_expected		 = 13 + start_of_runtime_system_errors,
};

void trace_stack(int thread_id);
