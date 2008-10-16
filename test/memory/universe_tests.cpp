# include "incls/_precompiled.incl"
#include "test.h"

using namespace easyunit;
DECLARE(UniverseTests)
oop* eden_old_top;
END_DECLARE

SETUP(UniverseTests) {
  eden_old_top = eden_top;
}

TEARDOWN(UniverseTests){
  //Can't collect because the allocated space is not an object.
  eden_top = eden_old_top;
}

TESTF(UniverseTests, allocateShouldAllocateInNewSpaceWhenSpaceAvailable) {
  ASSERT_TRUE(Universe::new_gen.eden()->free() > 10);
  oop* chunk = Universe::allocate(10, NULL, false);
  ASSERT_TRUE(chunk != NULL);
  ASSERT_TRUE(Universe::new_gen.eden()->contains(chunk));
}

TESTF(UniverseTests, allocateShouldFailWhenNoSpaceAndScavengeDisallowed) {
  int freeSpace = Universe::new_gen.eden()->free();
  oop* chunk = Universe::allocate(freeSpace/oopSize + 1, NULL, false);
  ASSERT_EQUALS(NULL, (char*)chunk);
}

TESTF(UniverseTests, allocateTenuredShouldFailWhenNoSpaceAndExpansionDisallowed) {
  int freeSpace = Universe::old_gen.free();
  oop* chunk = Universe::allocate_tenured(freeSpace/oopSize + 1, false);
  ASSERT_EQUALS(NULL, (char*)chunk);
}