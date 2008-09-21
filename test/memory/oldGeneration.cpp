# include "incls/_precompiled.incl"
# include "incls/_generation.cpp.incl"
#include "test.h"

using namespace easyunit;

TEST(OldGeneration, expansionShouldExpandOldGenerationCapacity)
{
  char msg[100];
  int oldSize = Universe::old_gen.capacity();
  Universe::old_gen.expand(1000 * K);
  int expectedSize = oldSize + 1000 * K;
  int actualSize = Universe::old_gen.capacity();
  sprintf(msg, "Generation has wrong capacity. Expected: %d, but was: %d", expectedSize, actualSize);
  ASSERT_EQUALS_M(expectedSize, actualSize, msg);
}

TEST(OldGeneration, expansionShouldExpandOldGenerationCapacityByMinimumAmount)
{
  char msg[100];
  int oldSize = Universe::old_gen.capacity();
  Universe::old_gen.expand(10);
  int expectedSize = oldSize + ReservedSpace::align_size(10, ObjectHeapExpandSize * K);
  int actualSize = Universe::old_gen.capacity();
  sprintf(msg, "Generation has wrong capacity. Expected: %d, but was: %d", expectedSize, actualSize);
  ASSERT_EQUALS_M(expectedSize, actualSize, msg);
}

TEST(OldGeneration, expansionShouldAlignExpansionWithExpansionSize)
{
  char msg[100];
  int oldSize = Universe::old_gen.capacity();
  Universe::old_gen.expand(ObjectHeapExpandSize * K * 3 / 2);
  int expectedSize = oldSize + ReservedSpace::align_size(ObjectHeapExpandSize * K * 3 / 2, ObjectHeapExpandSize * K);
  int actualSize = Universe::old_gen.capacity();
  sprintf(msg, "Generation has wrong capacity. Expected: %d, but was: %d", expectedSize, actualSize);
  ASSERT_EQUALS_M(expectedSize, actualSize, msg);
}
