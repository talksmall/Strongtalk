# include "incls/_precompiled.incl"
# include "incls/_klass.cpp.incl"
//#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(ProcessKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(ProcessKlassTests) {
  theClass = klassOop(Universe::find_global("Process"));
  oldEdenTop = eden_top;
}

TEARDOWN(ProcessKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(ProcessKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObject(false)));
}

TESTF(ProcessKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObject(false, true)));
}

TESTF(ProcessKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObject(true)));
}
