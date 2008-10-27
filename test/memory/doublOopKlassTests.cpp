# include "incls/_precompiled.incl"
# include "incls/_doubleKlass.cpp.incl"
#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(DoubleKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(DoubleKlassTests) {
  theClass = klassOop(Universe::find_global("Float"));
  oldEdenTop = eden_top;
}

TEARDOWN(DoubleKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(DoubleKlassTests, floatShouldBeDoubleC) {
  ASSERT_TRUE(theClass->klass_part()->oop_is_double());
}

TESTF(DoubleKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObject(false)));
}

TESTF(DoubleKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObject(false, true)));
}

TESTF(DoubleKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObject(true)));
}
