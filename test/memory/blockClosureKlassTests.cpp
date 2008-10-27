# include "incls/_precompiled.incl"
# include "incls/_blockKlass.cpp.incl"
#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(BlockClosureKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(BlockClosureKlassTests) {
  theClass = klassOop(Universe::find_global("BlockWithoutArguments"));
  oldEdenTop = eden_top;
}

TEARDOWN(BlockClosureKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(BlockClosureKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObject(false)));
}

TESTF(BlockClosureKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObject(false, true)));
}

TESTF(BlockClosureKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObject(true)));
}
