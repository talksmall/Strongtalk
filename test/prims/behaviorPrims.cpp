# include "incls/_precompiled.incl"
# include "incls/_behavior_prims.cpp.incl"
#include "handle.hpp"
#include "test.h"

using namespace easyunit;

DECLARE(BehaviorPrimitives)
END_DECLARE

SETUP(BehaviorPrimitives) {
}

TEARDOWN(BehaviorPrimitives){
  MarkSweep::collect();
}

TESTF(BehaviorPrimitives, allocateForMemOopShouldReportFailureWhenNoSpace) {
  Handle objectClass(Universe::find_global("Object"));
  int freeSpace = Universe::new_gen.eden()->free();
  oopFactory::new_byteArray(max(1, freeSpace - (4 * oopSize)));
  ASSERT_TRUE_M(Universe::new_gen.eden()->free() < (2 * oopSize), "Too much free space");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::failed_allocation()), behaviorPrimitives::allocate3(objectClass.as_oop()), "Allocation should fail");
}

TESTF(BehaviorPrimitives, allocateForMemOopShouldScavengeAndAllocateWhenAllowed) {
  Handle objectClass(Universe::find_global("Object"));
  int freeSpace = Universe::new_gen.eden()->free();
  oopFactory::new_byteArray(max(1, freeSpace - (4 * oopSize)));
  ASSERT_TRUE_M(Universe::new_gen.eden()->free() < (2 * oopSize), "Too much free space");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::failed_allocation()), behaviorPrimitives::allocate(objectClass.as_oop()), "Allocation should fail");
}