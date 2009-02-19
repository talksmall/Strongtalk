#define BUFLEN 2048

class TestNotifier: public Notifier, public ResourceObj {
  GrowableArray<char*>* errors;
  GrowableArray<char*>* warnings;
  GrowableArray<char*>* compilerWarnings;
public:
  TestNotifier() {
    errors = new GrowableArray<char*>;
    warnings = new GrowableArray<char*>;
    compilerWarnings = new GrowableArray<char*>;
  }
  int errorCount() {
    return errors->length();
  }
  int warningCount() {
    return warnings->length();
  }
  int compilerWarningCount() {
    return compilerWarnings->length();
  }
  char* errorAt(int index) {
    return errors->at(index);
  }
  char* warningAt(int index) {
    return warnings->at(index);
  }
  char* compilerWarningAt(int index) {
    return compilerWarnings->at(index);
  }
  void error(char* m, va_list ap) {
    char* buffer = NEW_RESOURCE_ARRAY(char, 2048);
    _vsnprintf(buffer, 2048, m, ap);
    errors->append(buffer);
  }
  void warning(char*m, va_list ap) {
    char* buffer = NEW_RESOURCE_ARRAY(char, BUFLEN);
    _vsnprintf(buffer, BUFLEN, m, ap);
    warnings->append(buffer);
  }
  void compiler_warning(char*m, va_list ap) {
    char* buffer = NEW_RESOURCE_ARRAY(char, BUFLEN);
    _vsnprintf(buffer, BUFLEN, m, ap);
    compilerWarnings->append(buffer);
  }
};
