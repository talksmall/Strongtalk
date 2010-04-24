# include "incls/_precompiled.incl"
# include "incls/_behavior_prims.cpp.incl"
//#include "handle.hpp"
#include "test.h"
#include "edenMark.hpp"

using namespace easyunit;

DECLARE(BehaviorPrimitives)
  EdenMark edenMark;
  klassOop objectClass;
END_DECLARE

SETUP(BehaviorPrimitives) {
  edenMark.setToEnd();
  objectClass = klassOop(Universe::find_global("Object"));
}

TEARDOWN(BehaviorPrimitives){
}

TESTF(BehaviorPrimitives, allocateForMemOopShouldReportFailureWhenNoSpace) {
  ASSERT_TRUE_M(Universe::new_gen.eden()->free() < (2 * oopSize), "Too much free space");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::failed_allocation()), behaviorPrimitives::allocate3(Universe::falseObj(), objectClass), "Allocation should fail");
}

TESTF(BehaviorPrimitives, allocateForMemOopShouldAllocateTenureWhenRequired) {
  ASSERT_TRUE(behaviorPrimitives::allocate3(Universe::trueObj(), objectClass)->is_old());
}

TESTF(BehaviorPrimitives, allocateForMemOopShouldCheckTenuredIsBoolean) {
  ASSERT_TRUE(markSymbol(vmSymbols::second_argument_has_wrong_type()) == behaviorPrimitives::allocate3(Universe::nilObj(), objectClass));
}

TESTF(BehaviorPrimitives, allocateForMemOopShouldScavengeAndAllocateWhenAllowed) {
  HandleMark mark;
  Handle objectClassHandle(objectClass);
  ASSERT_TRUE_M(Universe::new_gen.eden()->free() < (2 * oopSize), "Too much free space");
  oop object = behaviorPrimitives::allocate(objectClass);
  ASSERT_TRUE_M(!object->is_mark(), "result should not be marked");
  ASSERT_TRUE_M(object->is_mem(), "result should be mem");
  ASSERT_EQUALS_M(memOop(object)->klass(), objectClassHandle.as_memOop(), "wrong class");
}