class ProcessPreemptionTask : public PeriodicTask {
public:
  ProcessPreemptionTask() : PeriodicTask(1) {};
  void task();
};

