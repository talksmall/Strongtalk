# include "incls/_precompiled.incl"
# include "incls/_lprintf.cpp.incl"

#include "test.h"
#include "testNotifier.hpp"

using namespace easyunit;

DECLARE(errorTests)
  ResourceMark mark;
  Notifier* saved;
  TestNotifier* notifier;
END_DECLARE

SETUP(errorTests) {
  saved = Notifier::current;
  notifier = new TestNotifier;
  Notifier::current = notifier;
}

TEARDOWN(errorTests){
  Notifier::current = saved;
}
TESTF(errorTests, strcmp) {
  ASSERT_EQUALS(0, strcmp("format arg1", "format arg1"));
}

TESTF(errorTests, errorShouldReportErrorWithOneArgToNotifier) {
  ::error("format %s", "arg1");
  
  ASSERT_EQUALS(1, notifier->errorCount());
  ASSERT_EQUALS(0, strcmp("format arg1", notifier->errorAt(0)));
}

TESTF(errorTests, errorShouldReportErrorWithTwoArgToNotifier) {
  ::error("format %s %s", "arg1", "arg2");

  ASSERT_EQUALS(1, notifier->errorCount());
  ASSERT_EQUALS(0, strcmp("format arg1 arg2", notifier->errorAt(0)));
}

TESTF(errorTests, warningShouldReportWarningWithOneArgToNotifier) {
  ::warning("format %s", "arg1");

  ASSERT_EQUALS(1, notifier->warningCount());
  ASSERT_EQUALS(0, strcmp("format arg1", notifier->warningAt(0)));
}

TESTF(errorTests, compiler_warningShouldReportWarningWithOneArgToNotifier) {
  ::compiler_warning("format %s", "arg1");

  ASSERT_EQUALS(1, notifier->compilerWarningCount());
  ASSERT_EQUALS(0, strcmp("format arg1", notifier->compilerWarningAt(0)));
}

TESTF(errorTests, compiler_warningShouldNotReportWarningWhenPrintCompilerWarningsIsFalse) {
  FlagSetting fl(PrintCompilerWarnings, false);

  ASSERT_EQUALS(0, notifier->compilerWarningCount());
  ::compiler_warning("format %s", "arg1");
  ASSERT_EQUALS(0, notifier->compilerWarningCount());
}
