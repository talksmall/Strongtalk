# include "incls/_precompiled.incl"
# include "incls/_BlockKlass.cpp.incl"
#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(ContextKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(ContextKlassTests) {
  theClass = klassOop(Universe::find_global("Context"));
  oldEdenTop = eden_top;
}

TEARDOWN(ContextKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(ContextKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObject(false)));
}

TESTF(ContextKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObject(false, true)));
}

TESTF(ContextKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObject(true)));
}
