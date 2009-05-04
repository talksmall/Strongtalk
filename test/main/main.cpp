#include "incls/_precompiled.incl"
#include "incls/_shell.cpp.incl"
#include "incls/_delta.cpp.incl"
#include "testharness.h"
#include <windows.h>

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
int TestDeltaProcess::launch_tests(DeltaProcess *process) {
  process->suspend_at_creation();
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
