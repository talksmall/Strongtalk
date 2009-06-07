# include "incls/_precompiled.incl"
# include "incls/_mixinKlass.cpp.incl"
#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(MixinKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(MixinKlassTests) {
  theClass = klassOop(Universe::find_global("Object"))->klass_part()->mixin()->klass();
  oldEdenTop = eden_top;
}

TEARDOWN(MixinKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(MixinKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObject(false)));
}

TESTF(MixinKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObject(false, true)));
}

TESTF(MixinKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObject(true)));
}
