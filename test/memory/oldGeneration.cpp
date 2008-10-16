# include "incls/_precompiled.incl"
# include "incls/_generation.cpp.incl"
#include "test.h"

using namespace easyunit;
DECLARE(OldGeneration)
  void expandAndCheckCapacity(int expansionSize) {
    char msg[100];
    int oldSize = Universe::old_gen.capacity();

    Universe::old_gen.expand(expansionSize);

    int expectedIncrement = ReservedSpace::align_size(expansionSize, ObjectHeapExpandSize * K);
    int actual = Universe::old_gen.capacity();
    int expectedSize = oldSize + expectedIncrement;
    sprintf(msg, "Generation has wrong capacity. Expected: %d, but was: %d", expectedSize, actual);
    ASSERT_EQUALS_M(expectedSize, actual, msg);
  }
END_DECLARE

SETUP(OldGeneration) {
}

TEARDOWN(OldGeneration){
}
TESTF(OldGeneration, expansionShouldExpandOldGenerationCapacity) {
  expandAndCheckCapacity(1000 * K);
}

TESTF(OldGeneration, expansionShouldExpandOldGenerationCapacityByMinimumAmount) {
  expandAndCheckCapacity(10);
}

TESTF(OldGeneration, expansionShouldAlignExpansionWithExpansionSize) {
  expandAndCheckCapacity(ObjectHeapExpandSize * K * 3 / 2);
}

TESTF(OldGeneration, allocateWithoutExpansionWhenEmptyShouldFail) {
  int free = Universe::old_gen.free();
  oop* result = Universe::old_gen.allocate(free + 1, false);
  ASSERT_EQUALS_M(NULL, result, "Result should be NULL");
}

TESTF(OldGeneration, shrinkShouldReduceOldSpaceCapacity) {
  int freeSpace = Universe::old_gen.free();
  Universe::old_gen.expand(ObjectHeapExpandSize *K);
  Universe::old_gen.shrink(ObjectHeapExpandSize *K);
  ASSERT_EQUALS(freeSpace, Universe::old_gen.free());
}

TESTF(OldGeneration, shrinkShouldReturnZeroWhenInsufficientFreeSpace) {
  int freeSpace = Universe::old_gen.free();
  ASSERT_EQUALS(0, Universe::old_gen.shrink(freeSpace + 1));
  ASSERT_EQUALS(freeSpace, Universe::old_gen.free());
}