# include "incls/_precompiled.incl"
# include "incls/_klass.cpp.incl"
//#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(VframeKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(VframeKlassTests) {
  theClass = klassOop(Universe::find_global("Activation"));
  oldEdenTop = eden_top;
}

TEARDOWN(VframeKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(VframeKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObject(false)));
}

TESTF(VframeKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObject(false, true)));
}

TESTF(VframeKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObject(true)));
}
