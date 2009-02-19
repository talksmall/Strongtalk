extern void addTestToProcesses();
extern void removeTestFromProcesses();

class AddTestProcess : public ValueObj {
public:
  AddTestProcess() {
    addTestToProcesses();
  }
  ~AddTestProcess() {
    removeTestFromProcesses();
  }
};