# include "incls/_precompiled.incl"
# include "incls/_dByteArrayKlass.cpp.incl"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(DByteArrayKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(DByteArrayKlassTests) {
  theClass = klassOop(Universe::find_global("String"));
  oldEdenTop = eden_top;
}

TEARDOWN(DByteArrayKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(DByteArrayKlassTests, shouldBeDoubleByteArray) {
  eden_top = eden_end;
  ASSERT_TRUE(theClass->klass_part()->oop_is_doubleByteArray());
}
TESTF(DByteArrayKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObjectSize(100, false)));
}

TESTF(DByteArrayKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObjectSize(100, false, true)));
}

TESTF(DByteArrayKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObjectSize(100, true)));
}
