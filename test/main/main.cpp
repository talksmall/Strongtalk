#include "incls/_precompiled.incl"
#include "incls/_delta.cpp.incl"
#include "incls/_shell.cpp.incl"
#ifdef WIN32
//#include "processOop.hpp"
#include <windows.h>
#endif
//#include "handle.hpp"
#include "testharness.h"
#include "testProcess.hpp"
void ostream_init();

using namespace easyunit;
extern "C" void load_image();

static Event* done;

typedef int (*osfn)(void*);
typedef int (*fn)(DeltaProcess*);

// This is a fake DeltaProcess used to run the tests.
// It is there to allow VM operations to be executed on the VMProcess thread.
class TestDeltaProcess : public DeltaProcess {
private:
  static int launch_tests(DeltaProcess *process);
public:
  TestDeltaProcess();
  TestDeltaProcess(fn launchfn);
  ~TestDeltaProcess();
  void addToProcesses();
  void removeFromProcesses();
  void deoptimized_wrt_marked_nmethods() {}
  bool has_stack() const { return false; }

  static int launch_scheduler(DeltaProcess *process);
};

TestDeltaProcess*    testProcess = NULL;
static VMProcess*    vmProcess;
static Thread*       vmThread;

void addTestToProcesses() {
  testProcess->addToProcesses();
}

void removeTestFromProcesses() {
  testProcess->removeFromProcesses();
}

void TestDeltaProcess::removeFromProcesses() {
  Processes::remove(this);
}
oop newProcess() {
  return Delta::call(Universe::find_global("Process"), oopFactory::new_symbol("new"));
}

void TestDeltaProcess::addToProcesses() {
  oop process = newProcess();
  assert(process->is_process(), "Should be process");
  set_processObj(processOop(process));
  processOop(process)->set_process(this);
  Processes::add(this);
}

TestDeltaProcess::TestDeltaProcess(): DeltaProcess(NULL, NULL) {
  int ignore;
  Processes::remove(this);
  os::terminate_thread(_thread); // don't want to launch delta!
  _thread = os::create_thread((int (*)(void*)) &launch_tests, this, &ignore);
  _stack_limit = (char*)os::stack_limit(_thread);

  oop process = newProcess();
  assert(process->is_process(), "Should be process");
  set_processObj(processOop(process));
  processOop(process)->set_process(this);
}
TestDeltaProcess::TestDeltaProcess(fn launchfn): DeltaProcess(NULL, NULL) {
  int ignore;
  Processes::remove(this);
  os::terminate_thread(_thread); // don't want to launch delta!
  _thread = os::create_thread((osfn) launchfn, this, &ignore);
  _stack_limit = (char*)os::stack_limit(_thread);
  
  oop process = newProcess();
  assert(process->is_process(), "Should be process");
  set_processObj(processOop(process));
  processOop(process)->set_process(this);
}

TestDeltaProcess::~TestDeltaProcess() {
  set_processObj(processOop(newProcess()));
}
void setProcessRefs(DeltaProcess* process, processOop processObj) {
  processObj->set_process(process);
  process->set_processObj(processObj);
}
void initializeSmalltalkEnvironment() {
  AddTestProcess ap;
  PersistentHandle _new(oopFactory::new_symbol("new"));
  PersistentHandle initialize(oopFactory::new_symbol("initialize"));
  PersistentHandle runBase(oopFactory::new_symbol("runBaseClassInitializers"));
  PersistentHandle processorScheduler(Universe::find_global("ProcessorScheduler"));
  PersistentHandle smalltalk(Universe::find_global("Smalltalk"));
  PersistentHandle systemInitializer(Universe::find_global("SystemInitializer"));
  PersistentHandle forSeconds(oopFactory::new_symbol("forSeconds:"));
  
  PersistentHandle processor(Delta::call(processorScheduler.as_oop(), _new.as_oop()));

  associationOop processorAssoc = Universe::find_global_association("Processor");
  processorAssoc->set_value(processor.as_oop());

  DeltaProcess* scheduler = new TestDeltaProcess(&TestDeltaProcess::launch_scheduler);
  DeltaProcess::set_scheduler(scheduler);

  Delta::call(processor.as_oop(), initialize.as_oop());
  Delta::call(systemInitializer.as_oop(), runBase.as_oop());
  Delta::call(smalltalk.as_oop(), initialize.as_oop());
}
int TestDeltaProcess::launch_tests(DeltaProcess *process) {
  process->suspend_at_creation();
  DeltaProcess::set_active(process);
  initializeSmalltalkEnvironment();
  TestRegistry::runAndPrint();
  os::signal_event(done);
  return 0;
}
// mock scheduler loop to allow test process->scheduler transfers and returns
int TestDeltaProcess::launch_scheduler(DeltaProcess *process) {
  process->suspend_at_creation();
  DeltaProcess::set_active(process);
  while(true) {
    if (DeltaProcess::wait_for_async_dll(10))
      process->transfer_to(testProcess);
  }
  return 0;
}
static int vmLoopLauncher(DeltaProcess* testProcess) {
  vmProcess->transfer_to(testProcess);
  vmProcess->loop();
  return 0;
}
void start_vm_process(TestDeltaProcess* testProcess) {
  int threadId;
  vmProcess = new VMProcess();
  DeltaProcess::initialize_async_dll_event();
  vmThread = os::create_thread((int(*)(void*))&vmLoopLauncher, testProcess, &threadId);
  ::testProcess = testProcess;
}
void stop_vm_process() {
  os::terminate_thread(vmThread);
}

int main(int argc, char* argv[]) {
  parse_arguments(argc, argv);
  init_globals();
  load_image();

  done = os::create_event(false);

  TestDeltaProcess testProcess;
  start_vm_process(&testProcess);
  os::wait_for_event(done);
  stop_vm_process();
}
