/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.50 $ */
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
#ifndef __OS_LINUX__
#define __OS_LINUX__
#ifdef __LINUX__
# include <pthread.h>
# include "incls/_os.cpp.incl"
# include <unistd.h>
# include <semaphore.h>
# include <sys/times.h>
# include <sys/mman.h>
# include <time.h>
# include <stdio.h>
# include <dlfcn.h>
# include <signal.h>
# include <ucontext.h>
# include <errno.h>

void os_dump_context2(ucontext_t *context) {
    mcontext_t mcontext = context->uc_mcontext;
    printf("\nEAX: %x", mcontext.gregs[REG_EAX]);
    printf("\nEBX: %x", mcontext.gregs[REG_EBX]);
    printf("\nECX: %x", mcontext.gregs[REG_ECX]);
    printf("\nEDX: %x", mcontext.gregs[REG_EDX]);
    printf("\nEIP: %x", mcontext.gregs[REG_EIP]);
    printf("\nESP: %x", mcontext.gregs[REG_ESP]);
    printf("\nEBP: %x", mcontext.gregs[REG_EBP]);
    printf("\nEDI: %x", mcontext.gregs[REG_EDI]);
    printf("\nESI: %x", mcontext.gregs[REG_ESI]);
}
void os_dump_context() {
	ucontext_t context;
	getcontext(&context);
	os_dump_context2(&context);
}

static int    main_thread_id;
class Lock {
    private:
        pthread_mutex_t* mutex;

    public:
        Lock(pthread_mutex_t* mutex) : mutex(mutex) {
            pthread_mutex_lock(mutex);
        }
        ~Lock() {
            pthread_mutex_unlock(mutex);
        }
};

static int _argc;
static char** _argv;

int os::argc() {
  return _argc;
}

char** os::argv() {
  return _argv;
}

void os::set_args(int argc, char* argv[]) {
    _argc = argc;
    _argv = argv;
}

class Event: public CHeapObj {
    private:
        bool _signalled;
        pthread_mutex_t mutex;
        pthread_cond_t notifier;
    public:
        void inline signal() {
            Lock mark(&mutex);
            _signalled = true;
            pthread_cond_signal(&notifier);
        }
        void inline reset() {
            Lock mark(&mutex);
            _signalled = false;
            pthread_cond_signal(&notifier);
        }
        bool inline waitFor() {
            Lock mark(&mutex);
            while (!_signalled)
              pthread_cond_wait(&notifier, &mutex);
        }
        Event(bool state) {
            _signalled = state;
            pthread_mutex_init(&mutex, NULL);
            pthread_cond_init(&notifier, NULL);
        }
        ~Event() {
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&notifier);
        }
};

class Thread : CHeapObj {
	public:
        static Thread* find(pthread_t threadId) {
          for (int index = 0; index < _threads->length(); index++) {
            Thread* candidate = _threads->at(index);
            if (candidate == NULL) continue;
            if (pthread_equal(threadId, candidate->_threadId))
              return candidate;
          }
          return NULL;
        }
        void suspend() {
        	suspendEvent.waitFor();
        }
        void resume() {
        	suspendEvent.signal();
        }
    private:
        Event suspendEvent;
        static GrowableArray<Thread*>* _threads;
        pthread_t _threadId;
        clockid_t _clockId;
        int _thread_index;
        void* stackLimit;
        
        static void init() {
          ThreadCritical lock;
          _threads = new(true) GrowableArray<Thread*>(10, true);
          
        }
	    Thread(pthread_t threadId, void* stackLimit) : _threadId(threadId), suspendEvent(false), stackLimit(stackLimit) {
          ThreadCritical lock;
          pthread_getcpuclockid(_threadId, &_clockId);
          _thread_index = _threads->length();
          _threads->push(this);
        };
        ~Thread() {
          ThreadCritical lock;
          _threads->at_put(_thread_index, NULL);
        }
        double get_cpu_time() {
          struct timespec cpu;
          clock_gettime(_clockId, &cpu);
          return ((double)cpu.tv_sec) + ((double)cpu.tv_nsec)/1000000000.0; 
        }
        friend class os;
};

GrowableArray<Thread*>* Thread::_threads = NULL;
static Thread* main_thread;

extern void intercept_for_single_step();

// No references in VM
int os::getenv(char* name,char* buffer,int len) {
 return 0;
}

// 1 reference (lprintf.cpp)
bool os::move_file(char* from, char* to) {
	return false;
}

// 1 reference (inliningdb.cpp)
bool os::check_directory(char* dir_name) {
  return false;
}

// 1 reference (memory/util.cpp)
void os::breakpoint() {
  asm("int3");
}

// 1 reference process.cpp
Thread* os::starting_thread(int* id_addr) {
  *id_addr = main_thread->_thread_index;
  return main_thread;
}

typedef struct {
	int (*main)(void* parameter);
	void* parameter;
  char* stackLimit;
} thread_args_t;

static Event* threadCreated = NULL;

#define STACK_SIZE ThreadStackSize * K

char* calcStackLimit() {
  char* stackptr;
  asm("movl %%esp, %0;" : "=a"(stackptr));
  stackptr = (char*) align(stackptr, os::vm_page_size());
  
  int stackHeadroom = 2 * os::vm_page_size();
  return stackptr - STACK_SIZE + stackHeadroom;
}
void* mainWrapper(void* args) {
  thread_args_t * targs = (thread_args_t*) args;
  targs->stackLimit = calcStackLimit();
	
  int (*threadMain)(void*) = targs->main;
  void* parameter = targs->parameter;
  int* result = (int*) malloc(sizeof(int));
  threadCreated->signal();
  *result = threadMain(parameter);
  return (void *) result; 
}

Thread* os::create_thread(int threadStart(void* parameter), void* parameter, int* id_addr) {
	pthread_t threadId;
  thread_args_t threadArgs;
  {
    ThreadCritical tc;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, STACK_SIZE);
    
    threadCreated->reset();
    threadArgs.main = threadStart;
    threadArgs.parameter = parameter;
    
    int status = pthread_create(&threadId, &attr, &mainWrapper, &threadArgs);
    if (status != 0) {
      fatal1("Unable to create thread. status = %d", status);
    }
  }
  threadCreated->waitFor();
	Thread* thread = new Thread(threadId, threadArgs.stackLimit);
	*id_addr = thread->_thread_index;
	return thread;
}

void* os::stack_limit(Thread* thread) {
  return thread->stackLimit;
}

// 1 reference process.cpp
void os::terminate_thread(Thread* thread) {
}

// 1 reference process.cpp
void os::delete_event(Event* event) {
    delete event;
}

// 1 reference process.cpp
Event* os::create_event(bool initial_state) {
  return new Event(initial_state);
}

tms processTimes;

// 2 references - prims/system_prims.cpp, timer.cpp
int os::updateTimes() {
  return times(&processTimes) != (clock_t) -1;
}

// 2 references - prims/system_prims.cpp, timer.cpp
double os::userTime() {
  return ((double) processTimes.tms_utime)/ CLOCKS_PER_SEC;
}

// 2 references - prims/system_prims.cpp, timer.cpp
double os::systemTime() {
  return ((double) processTimes.tms_stime)/ CLOCKS_PER_SEC;
}

// 1 reference - process.cpp
double os::user_time_for(Thread* thread) {
  //Hack warning - assume half time is spent in kernel, half in user code
  return thread->get_cpu_time()/2;
}

// 1 reference - process.cpp
double os::system_time_for(Thread* thread) {
  //Hack warning - assume half time is spent in kernel, half in user code
  return thread->get_cpu_time()/2;
}

static int      has_performance_count = 0;
static long_int initial_performance_count(0,0);
static long_int performance_frequency(0,0);

// 2 references - memory/error.cpp, evaluator.cpp
void os::fatalExit(int num) {
    exit(num);
}

class DLLLoadError {
};
  
class DLL : CHeapObj {
  private:
    char* _name;
    void* _handle;

    DLL(char* name) {
      _handle = dlopen(name, RTLD_LAZY);
      checkHandle(_handle, "could not find library: %s");
      _name = (char*)malloc(strlen(name) + 1);
      strcpy(_name, name);
    }
    void checkHandle(void* handle, const char* format) {
      if (handle == NULL) {
        char* message = (char*) malloc(200);
        sprintf(message, format, dlerror());
        assert(handle != NULL, message);
        free(message);
      }
    }
    ~DLL() {
      if (_handle) dlclose(_handle);
      if (_name) free(_name);
    }
    bool isValid() {
      return (_handle != NULL) && (_name != NULL);
    }
    dll_func lookup(char* funcname) {
      dll_func function = dll_func(dlsym(_handle, funcname));
      checkHandle((void*) function, "could not find function: %s"); 
      return function; 
    }
  friend class os;
};

// 1 reference - prims/dll.cpp
dll_func os::dll_lookup(char* name, DLL* library) {
  return library->lookup(name);
}

// 1 reference - prims/dll.cpp
DLL* os::dll_load(char* name) {
  DLL* library = new DLL(name);
  if (library->isValid()) return library;
  delete library;
  return NULL;
}

// 1 reference - prims/dll.cpp
bool os::dll_unload(DLL* library) {
  delete library;
  return true;
}

char* os::dll_extension() {
  return ".so";
}

int       nCmdShow      = 0;

// 1 reference - prims/system_prims.cpp
void* os::get_hInstance()    { return (void*) NULL;     }
// 1 reference - prims/system_prims.cpp
void* os::get_prevInstance() { return (void*) NULL; }
// 1 reference - prims/system_prims.cpp
int   os::get_nCmdShow()     { return 0;            }

extern int bootstrapping;

// 1 reference - prims/debug_prims.cpp
void os::timerStart() {}

// 1 reference - prims/debug_prims.cpp
void os::timerStop() {}

// 1 reference - prims/debug_prims.cpp
void os::timerPrintBuffer() {}

char* os::reserve_memory(int size) {
  return (char*) mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
}
  
bool os::commit_memory(char* addr, int size) {
  return !mprotect(addr, size, PROT_READ | PROT_WRITE);
}

bool os::uncommit_memory(char* addr, int size) {
  return !mprotect(addr, size, PROT_NONE);
}

bool os::release_memory(char* addr, int size) {
  return !munmap(addr, size);
}

// No references
bool os::guard_memory(char* addr, int size) {
  return false;
}

void* os::malloc(int size) {
  return ::malloc(size);
}

char* os::exec_memory(int size) {
  return (char*) mmap(0, size, PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
}

void* os::calloc(int size, char filler) {
  return ::calloc(size, filler);
}

void os::free(void* p) {
  ::free(p);
}

// 1 reference - process.cpp
void os::transfer(Thread* from_thread, Event* from_event, Thread* to_thread, Event* to_event) {
  from_event->reset();
  to_event->signal();
  from_event->waitFor();  
}

// 1 reference - process.cpp
void os::transfer_and_continue(Thread* from_thread, Event* from_event, Thread* to_thread, Event* to_event) {
  from_event->reset();
  to_event->signal();
}

// 1 reference - process.cpp
void os::suspend_thread(Thread* thread) {
	os_dump_context();
	pthread_kill(thread->_threadId, SIGUSR1);
}

void suspendHandler(int signum) {
	Thread* current = Thread::find(pthread_self());
	assert(current, "Suspended thread not found!");
	current->suspend();
}
// 1 reference - process.cpp
void os::resume_thread(Thread* thread) {
	thread->resume();
}

// No references
void os::sleep(int ms) {
}

// 1 reference - process.cpp
void os::fetch_top_frame(Thread* thread, int** sp, int** fp, char** pc) {
}
  
// 1 reference - callBack.cpp
int os::current_thread_id() {
  Thread* currentThread = Thread::find(pthread_self());
  if (currentThread == NULL) return -1;
  return currentThread->_thread_index;
}

// 1 reference - process.cpp
void os::wait_for_event(Event* event) {
    event->waitFor();
}

// 1 reference - process.cpp
void os::reset_event(Event* event) {
    event->reset();
}

// 1 reference - process.cpp
void os::signal_event(Event* event) {
    event->signal();
}

// 1 reference - process.cpp
bool os::wait_for_event_or_timer(Event* event, int timeout_in_ms) {
  return false;
}

extern "C" bool WizardMode;

void process_settings_file(char* file_name, bool quiet);

static int number_of_ctrl_c = 0;

// 2 references - memory/universe, runtime/virtualspace
int os::_vm_page_size = getpagesize();

// 1 reference - timer.cpp
long_int os::elapsed_counter() {
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  int64_t current64 = ((int64_t)current_time.tv_sec) * 1000000000 + current_time.tv_nsec;
  uint high = current64 >> 32;
  uint low  = current64 & 0xffffffff;
  long_int current(low, high);
  return current;
}

// 1 reference - timer.cpp
long_int os::elapsed_frequency() {
  return long_int(1000000000, 0);
}

static struct timespec initial_time;

// 1 reference - prims/system_prims.cpp
double os::elapsedTime() {
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  long int secs = current_time.tv_sec - initial_time.tv_sec;
  long int nsecs = current_time.tv_nsec - initial_time.tv_nsec;
  if (nsecs < 0) {
    secs--;
    nsecs += 1000000000;
  } 
  return secs + (nsecs / 1000000000.0);
}

// No references
double os::currentTime() {
  return 0;
}

static void initialize_performance_counter() {
  clock_gettime(CLOCK_REALTIME, &initial_time);
}

// No references
void os::initialize_system_info() {
    Thread::init();
    main_thread = new Thread(pthread_self(), calcStackLimit());
    initialize_performance_counter();
}

// 1 reference - memory/error.cpp
int os::message_box(char* title, char* message) {
  return 0;
}

char* os::platform_class_name() { return "UnixPlatform"; }

int os::error_code() {
  return errno;
}
extern "C" bool EnableTasks;

pthread_mutex_t ThreadSection; 

bool ThreadCritical::_initialized = false;
void ThreadCritical::intialize() {
  pthread_mutex_init(&ThreadSection, NULL);
  _initialized = true;
}
void ThreadCritical::release()   { pthread_mutex_destroy(&ThreadSection);     }

ThreadCritical::ThreadCritical() {
  pthread_mutex_lock(&ThreadSection);
}

ThreadCritical::~ThreadCritical() {
  pthread_mutex_unlock(&ThreadSection);
}

void real_time_tick(int delay_time);

void* watcherMain(void* ignored) {
  const struct timespec delay = { 0, 1 * 1000 * 1000 };
  const int delay_interval = 1; // Delay 1 ms
  while(1) {
    int status = nanosleep(&delay, NULL);
    if (!status) return 0;
    real_time_tick(delay_interval);
  }
  return 0;
}

void segv_repeated(int signum, siginfo_t* info, void* context) {
	printf("SEGV during signal handling. Aborting.");
	exit(-2);
}

void install_dummy_handler() {
	  struct sigaction sa;

	  sigemptyset(&sa.sa_mask);
	  sa.sa_flags = SA_RESTART |SA_SIGINFO;
	  sa.sa_sigaction = segv_repeated;
	  if (sigaction(SIGSEGV, &sa, NULL) == -1)
	      /* Handle error */;
}

void trace_stack(int thread_id);
void (*userHandler)(void* fp, void* sp, void* pc) = NULL;

static void handler(int signum, siginfo_t* info, void* context) {
//	install_dummy_handler();
//	trace_stack(os::current_thread_id());
    if (!userHandler) {
        printf("\nsignal: %d\ninfo: %x\ncontext: %x", signum, (int) info, (int) context);
	    os_dump_context2((ucontext_t*) context);
    } else {
        mcontext_t mcontext = ((ucontext_t*) context)->uc_mcontext;
        userHandler((void*) mcontext.gregs[REG_EBP], 
                    (void*) mcontext.gregs[REG_ESP], 
                    (void*) mcontext.gregs[REG_EIP]);
    }
    exit(-1);
}

void os::add_exception_handler(void newHandler(void* fp, void* sp, void* pc)) {
    userHandler = newHandler;
}

void install_signal_handlers() {
	  struct sigaction sa;

	  sigemptyset(&sa.sa_mask);
	  sa.sa_flags = SA_RESTART; /* Restart functions if
	                               interrupted by handler */
	  sa.sa_handler = suspendHandler;
	  if (sigaction(SIGUSR1, &sa, NULL) == -1)
	      /* Handle error */;
	  sa.sa_flags |= SA_SIGINFO;
	  sa.sa_sigaction = handler;
	  if (sigaction(SIGSEGV, &sa, NULL) == -1)
	      /* Handle error */;
}

void os_init() {
  ThreadCritical::intialize();

  install_signal_handlers();
  os::initialize_system_info();
	
  pthread_setconcurrency(1);

  threadCreated = new Event(false);
  
  if (EnableTasks) {
    pthread_t watcherThread;
    int status = pthread_create(&watcherThread, NULL, &watcherMain, NULL);
    if (status != 0) {
      fatal("Unable to create thread");
    }
  }
}

void os_exit() {
  ThreadCritical::release();
}
#endif /* __GNUC__ */
#endif
