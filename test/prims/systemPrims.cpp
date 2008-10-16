# include "incls/_precompiled.incl"
# include "incls/_system_prims.cpp.incl"
#include "handle.hpp"
#include "behavior_prims.hpp"
#include "test.h"

using namespace easyunit;

TEST(SystemPrimTests, expansionShouldExpandOldGenerationCapacity) {
  char msg[100];
  int oldSize = Universe::old_gen.capacity();
  ASSERT_EQUALS_M(trueObj, systemPrimitives::expandMemory(as_smiOop(1000 * K)), "Wrong result");
  int expectedSize = oldSize + ReservedSpace::align_size(1000 * K, ObjectHeapExpandSize * K);
  int actualSize = Universe::old_gen.capacity();
  sprintf(msg, "Generation has wrong capacity. Expected: %d, but was: %d", expectedSize, actualSize);
  ASSERT_EQUALS_M(expectedSize, actualSize, msg);
}

TEST(SystemPrimTests, expansionShouldReturnMarkedResultWhenNotSMI) {
  oop result = systemPrimitives::expandMemory(oop(vmSymbols::aborted()));
  ASSERT_TRUE_M(result->is_mark(), "Result should be marked");
  ASSERT_EQUALS_M(vmSymbols::argument_has_wrong_type(), unmarkSymbol(result), "Wrong symbol");
}

TEST(SystemPrimTests, expansionShouldReturnMarkedResultWhenNegative) {
  oop result = systemPrimitives::expandMemory(as_smiOop(-1));
  ASSERT_TRUE_M(result->is_mark(), "Result should be marked");
  ASSERT_EQUALS_M(vmSymbols::argument_is_invalid(), unmarkSymbol(result), "Wrong symbol");
}

TEST(SystemPrimTests, oopSize) {
  ASSERT_EQUALS_M(as_smiOop(oopSize), systemPrimitives::oopSize(), "oopSize is wrong");
}

extern "C" int expansion_count;
TEST(SystemPrimTests, expansionsShouldReturnExpansionCountAsSMI) {
  int expansions = expansion_count;
  systemPrimitives::expandMemory(as_smiOop(ObjectHeapExpandSize * K));
  ASSERT_EQUALS_M(as_smiOop(expansions + 1), systemPrimitives::expansions(), "Wrong expansion count");
}

TEST(SystemPrimTests, nurseryFreeSpaceShouldReturnEdenFreeSpaceAsSMI) {
  ASSERT_EQUALS(Universe::new_gen.eden()->free(), smiOop(systemPrimitives::nurseryFreeSpace())->value());
}

TEST(SystemPrimTests, shrinkMemoryShouldReduceOldSpaceCapacity) {
  int freeSpace = smiOop(systemPrimitives::freeSpace())->value();
  systemPrimitives::expandMemory(as_smiOop(ObjectHeapExpandSize *K));
  systemPrimitives::shrinkMemory(as_smiOop(ObjectHeapExpandSize *K));
  ASSERT_EQUALS(freeSpace, smiOop(systemPrimitives::freeSpace())->value());
}

TEST(SystemPrimTests, shrinkMemoryShouldReturnValueOutOfRangeWhenInsufficientFreeSpace) {
  int freeSpace = smiOop(systemPrimitives::freeSpace())->value();
  ASSERT_EQUALS((int)markSymbol(vmSymbols::value_out_of_range()), (int)systemPrimitives::shrinkMemory(as_smiOop(freeSpace + 1)));
  ASSERT_EQUALS(freeSpace, smiOop(systemPrimitives::freeSpace())->value());
}

TEST(SystemPrimTests, shrinkMemoryShouldReturnValueOutOfRangeWhenNegative) {
  int freeSpace = smiOop(systemPrimitives::freeSpace())->value();
  ASSERT_EQUALS((int)markSymbol(vmSymbols::value_out_of_range()), (int)systemPrimitives::shrinkMemory(as_smiOop(-1)));
  ASSERT_EQUALS(freeSpace, smiOop(systemPrimitives::freeSpace())->value());
}

TEST(SystemPrimTests, shrinkMemoryShouldReturnArgumentIsOfWrongType) {
  int freeSpace = smiOop(systemPrimitives::freeSpace())->value();
  ASSERT_EQUALS((int)markSymbol(vmSymbols::first_argument_has_wrong_type()), (int)systemPrimitives::shrinkMemory(vmSymbols::and1()));
  ASSERT_EQUALS(freeSpace, smiOop(systemPrimitives::freeSpace())->value());
}