#include "incls/_precompiled.incl"
#include "incls/_shell.cpp.incl"
#include "incls/_delta.cpp.incl"
#include "handle.hpp"
#include "testharness.h"
#include <windows.h>
#include "testProcess.hpp"
void ostream_init();

using namespace easyunit;
extern "C" void load_image();

static Event* done;

// This is a fake DeltaProcess used to run the tests.
// It is there to allow VM operations to be executed on the VMProcess thread.
class TestDeltaProcess : public DeltaProcess {
private:
  static int launch_tests(DeltaProcess *process);
  oop newProcess();
public:
  TestDeltaProcess();
  ~TestDeltaProcess();
  void addToProcesses();
  void removeFromProcesses();
  void deoptimized_wrt_marked_nmethods() {}
  bool has_stack() const { return false; }
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
oop TestDeltaProcess::newProcess() {
  return Delta::call(Universe::find_global("Process"), oopFactory::new_symbol("new"));
}

void TestDeltaProcess::addToProcesses() {
  oop process = newProcess();
  assert(process->is_process(), "Should be process");
  set_processObj(processOop(process));
  Processes::add(this);
}

TestDeltaProcess::TestDeltaProcess(): DeltaProcess(NULL, NULL) {
  int ignore;
  Processes::remove(this);
  os::terminate_thread(_thread); // don't want to launch delta!
  os::create_thread((int (*)(void*)) &launch_tests, this, &ignore);
}

TestDeltaProcess::~TestDeltaProcess() {
  set_processObj(processOop(newProcess()));
}
void initializeSmalltalkEnvironment() {
  AddTestProcess ap;
  PersistentHandle _new(oopFactory::new_symbol("new"));
  PersistentHandle initialize(oopFactory::new_symbol("initialize"));
  PersistentHandle runBase(oopFactory::new_symbol("runBaseClassInitializers"));
  PersistentHandle processorScheduler(Universe::find_global("ProcessorScheduler"));
  PersistentHandle smalltalk(Universe::find_global("Smalltalk"));
  PersistentHandle systemInitializer(Universe::find_global("SystemInitializer"));
  
  PersistentHandle processor(Delta::call(processorScheduler.as_oop(), _new.as_oop()));
  associationOop processorAssoc = Universe::find_global_association("Processor");
  processorAssoc->set_value(processor.as_oop());

  Delta::call(processor.as_oop(), initialize.as_oop());
  Delta::call(systemInitializer.as_oop(), runBase.as_oop());
  Delta::call(smalltalk.as_oop(), initialize.as_oop());
}
int TestDeltaProcess::launch_tests(DeltaProcess *process) {
  process->suspend_at_creation();
  initializeSmalltalkEnvironment();
  TestRegistry::runAndPrint();
  os::signal_event(done);
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
  vmThread = os::create_thread((int(*)(void*))&vmLoopLauncher, testProcess, &threadId);
  ::testProcess = testProcess;
}
void stop_vm_process() {
  os::terminate_thread(vmThread);
}

static __declspec( thread ) int val1;

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
