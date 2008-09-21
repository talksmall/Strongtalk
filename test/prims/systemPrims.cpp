# include "incls/_precompiled.incl"
# include "incls/_system_prims.cpp.incl"
#include "test.h"

using namespace easyunit;

TEST(SystemPrims, expansionShouldExpandOldGenerationCapacity)
{
  char msg[100];
  int oldSize = Universe::old_gen.capacity();
  ASSERT_EQUALS_M(trueObj, systemPrimitives::expandMemory(as_smiOop(1000 * K)), "Wrong result");
  int expectedSize = oldSize + 1000 * K;
  int actualSize = Universe::old_gen.capacity();
  sprintf(msg, "Generation has wrong capacity. Expected: %d, but was: %d", expectedSize, actualSize);
  ASSERT_EQUALS_M(expectedSize, actualSize, msg);
}

TEST(SystemPrims, expansionShouldReturnMarkedResultWhenNotSMI)
{
  oop result = systemPrimitives::expandMemory(oop(vmSymbols::aborted()));
  ASSERT_TRUE_M(result->is_mark(), "Result should be marked");
  ASSERT_EQUALS_M(vmSymbols::argument_has_wrong_type(), unmarkSymbol(result), "Wrong symbol");
}

TEST(SystemPrims, expansionShouldReturnMarkedResultWhenNegative)
{
  oop result = systemPrimitives::expandMemory(as_smiOop(-1));
  ASSERT_TRUE_M(result->is_mark(), "Result should be marked");
  ASSERT_EQUALS_M(vmSymbols::argument_is_invalid(), unmarkSymbol(result), "Wrong symbol");
}

TEST(SystemPrims, oopSize)
{
  ASSERT_EQUALS_M(as_smiOop(oopSize), systemPrimitives::oopSize(), "oopSize is wrong");
}
