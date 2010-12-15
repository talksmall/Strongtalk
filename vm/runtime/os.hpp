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

// os defines the interface to operating system

typedef void (*dll_func)(...);

class Thread;
class Event;
class DLL;

class os {
 private:
  static int _vm_page_size;
  static void initialize_system_info();
  friend void os_init();
 public:
  static int    argc();
  static char** argv();
  static void   set_args(int argc, char* argv[]);
  static int    getenv(char* name,char* buffer,int len);
  static void   add_exception_handler(void handler(void* fp, void* sp, void* pc));

  // We must call updateTimes before calling userTime or currentTime.
  static int    updateTimes();
  static double userTime();
  static double systemTime();

  static double currentTime();

  // Timing opertaions for threads
  static double user_time_for(Thread* thread);
  static double system_time_for(Thread* thread);

  // Returns the elapsed time in seconds since the vm started.
  static double elapsedTime();

  // Interface to the win32 performance counter
  static long_int elapsed_counter();
  static long_int elapsed_frequency();

  static void timerStart();
  static void timerStop();
  static void timerPrintBuffer();

  static void* get_hInstance();
  static void* get_prevInstance();
  static int   get_nCmdShow();

  // OS interface to Virtual Memory - used for object memory allocations
  static int   vm_page_size() { return _vm_page_size; }
  static char* reserve_memory(int size);
  static bool  commit_memory(char* addr, int size);
  static bool  uncommit_memory(char* addr, int size);
  static bool  release_memory(char* addr, int size);
  static bool  guard_memory(char* addr, int size);
  static char* exec_memory(int size);
  
  // OS interface to C memory routines - used for small allocations
  static void* malloc(int size);
  static void* calloc(int size, char filler);
  static void  free(void* p);

  // threads
  static Thread* starting_thread(int* id_addr);
  static Thread* create_thread(int main(void* parameter), void* parameter, int* id_addr);
  static Event*  create_event(bool initial_state);
  static void*   stack_limit(Thread* thread);

  static int  current_thread_id();
  static void wait_for_event(Event* event);
  static void transfer(Thread* from_thread, Event* from_event, Thread* to_thread, Event* to_event);
  static void transfer_and_continue(Thread* from_thread, Event* from_event, Thread* to_thread, Event* to_event);
  static void terminate_thread(Thread* thread);
  static void delete_event(Event* event);

  static void reset_event(Event* event);
  static void signal_event(Event* event);
  static bool wait_for_event_or_timer(Event* event, int timeout_in_ms);

  static void sleep(int ms);

  // thread support for profiling
  static void suspend_thread(Thread* thread);
  static void resume_thread(Thread* thread);
  static void fetch_top_frame(Thread* thread, int** sp, int** fp, char** pc);

  static void breakpoint();

  static int message_box(char* title, char* message);

  static void     fatalExit(int num);

  static bool move_file(char* from, char* to);
  static bool check_directory(char* dir_name);

  // DLL support
  static dll_func dll_lookup(char* name, DLL* library);
  static DLL*     dll_load(char* name);
  static bool     dll_unload(DLL* library);
  static char*    dll_extension();

  // Platform
  static char*	  platform_class_name();
  static int      error_code();
};

// A critial region for controling thread transfer at
// interrupts
class ThreadCritical {
 private:
  static bool _initialized;
  friend void os_init();
  friend void os_exit();
  static void intialize();
  static void release();
 public:
  static bool initialized() { return _initialized; }
  ThreadCritical();
  ~ThreadCritical();
};
