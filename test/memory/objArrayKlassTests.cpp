# include "incls/_precompiled.incl"
# include "incls/_objArrayKlass.cpp.incl"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(ObjArrayKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
END_DECLARE

SETUP(ObjArrayKlassTests) {
  theClass = klassOop(Universe::find_global("Array"));
  oldEdenTop = eden_top;
}

TEARDOWN(ObjArrayKlassTests){
  eden_top = oldEdenTop;
  MarkSweep::collect();
}

TESTF(ObjArrayKlassTests, shouldBeObjArray) {
  eden_top = eden_end;
  ASSERT_TRUE(theClass->klass_part()->oop_is_objArray());
}
TESTF(ObjArrayKlassTests, allocateShouldFailWhenAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_EQUALS((int)NULL, (int)(theClass->klass_part()->allocateObjectSize(100, false)));
}

TESTF(ObjArrayKlassTests, allocateShouldAllocateTenuredWhenRequired) {
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObjectSize(100, false, true)));
}

TESTF(ObjArrayKlassTests, allocateShouldNotFailWhenNotAllowedAndNoSpace) {
  eden_top = eden_end;
  ASSERT_TRUE(Universe::new_gen.eden()->free() < 4 * oopSize);
  ASSERT_TRUE(Universe::new_gen.contains(theClass->klass_part()->allocateObjectSize(100, true)));
}

TESTF(ObjArrayKlassTests, allocateShouldExpandOldSpaceDuringTenuredAllocWhenAllowed) {
  OldSpaceMark mark = Universe::old_gen.memo();
  oldSpace* space = mark.theSpace;
  int free = Universe::old_gen.free()/oopSize;
  oop* temp = Universe::allocate_tenured(free - 1, false);
  ASSERT_TRUE(temp != NULL);
  ASSERT_TRUE(Universe::old_gen.contains(theClass->klass_part()->allocateObjectSize(100, true, true)));
}

TESTF(ObjArrayKlassTests, allocateShouldFailDuringTenuredAllocWhenOldSpaceExpansionNotAllowed) {
  OldSpaceMark mark = Universe::old_gen.memo();
  oldSpace* space = mark.theSpace;
  int free = Universe::old_gen.free()/oopSize;
  oop* temp = Universe::allocate_tenured(free - 1, false);
  ASSERT_TRUE(temp != NULL);
  ASSERT_TRUE(NULL == theClass->klass_part()->allocateObjectSize(100, false, true));
}
