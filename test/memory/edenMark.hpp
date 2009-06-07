extern "C" oop* eden_top;
extern "C" oop* eden_end;

class EdenMark: ValueObj {
private:
  oop* old_eden_top;
public:
  EdenMark() {
    old_eden_top = eden_top;
  }
  ~EdenMark() {
    eden_top = old_eden_top;
  }
  void setToEnd() {
    eden_top = eden_end;
  }
};