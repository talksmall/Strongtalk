# include "incls/_precompiled.incl"
# include "incls/_klassOop.cpp.incl"
//#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(MemOopAllocationTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(MemOopAllocationTests) {
  theClass = klassOop(Universe::find_global("Object"));
  oldEdenTop = eden_top;
}

TEARDOWN(MemOopAllocationTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(MemOopAllocationTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObject(false)));
}

TESTF(MemOopAllocationTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObject(false, true)));
}

TESTF(MemOopAllocationTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObject(true)));
}
