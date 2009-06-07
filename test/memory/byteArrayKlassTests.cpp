# include "incls/_precompiled.incl"
# include "incls/_byteArrayKlass.cpp.incl"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(ByteArrayKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(ByteArrayKlassTests) {
  theClass = klassOop(Universe::find_global("ByteArray"));
  oldEdenTop = eden_top;
}

TEARDOWN(ByteArrayKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(ByteArrayKlassTests, shouldBeDoubleByteArray) {
  eden_top = eden_end;
  ASSERT_TRUE(theClass->klass_part()->oop_is_byteArray());
}
TESTF(ByteArrayKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObjectSize(100, false)));
}

TESTF(ByteArrayKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObjectSize(100, false, true)));
}

TESTF(ByteArrayKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObjectSize(100, true)));
}
