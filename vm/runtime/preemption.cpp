# include "incls/_precompiled.incl"
# include "incls/_preemption.cpp.incl"

void ProcessPreemptionTask::task() {
  if (EnableProcessPreemption)
    DeltaProcess::preempt_active();
}

void preemption_init() {
  ProcessPreemptionTask* task = new ProcessPreemptionTask;
  task->enroll();
}