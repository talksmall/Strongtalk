# include "incls/_precompiled.incl"
# include "incls/_system_prims.cpp.incl"
//#include "handle.hpp"
#include "behavior_prims.hpp"
#include "byteArray_prims.hpp"
#include "test.h"
#include "testUtils.hpp"

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

TEST(SystemPrimTests, alienMallocShouldReturnAlignedValue) {
  oop pointer = systemPrimitives::alienMalloc(as_smiOop(4));
  ASSERT_TRUE_M(pointer->is_smi(), "result should be SmallInteger");
  int address = smiOop(pointer)->value();
  ASSERT_TRUE_M(address % 4 == 0, "not aligned");
  ASSERT_TRUE_M(address != 0, "not allocated");
  free((void*)address);
}

TEST(SystemPrimTests, alienMallocShouldReturnMarkedSymbolWhenNotSmi) {
  oop pointer = systemPrimitives::alienMalloc(vmSymbols::abs());
  ASSERT_TRUE_M(pointer->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_has_wrong_type()), pointer, "wrong symbol returned");
}

TEST(SystemPrimTests, alienMallocShouldReturnMarkedSymbolWhenSizeNegative) {
  oop pointer = systemPrimitives::alienMalloc(as_smiOop(-1));
  ASSERT_TRUE_M(pointer->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_is_invalid()), pointer, "wrong symbol returned");
}

TEST(SystemPrimTests, alienMallocShouldReturnMarkedSymbolWhenSizeZero) {
  oop pointer = systemPrimitives::alienMalloc(as_smiOop(0));
  ASSERT_TRUE_M(pointer->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_is_invalid()), pointer, "wrong symbol returned");
}

TEST(SystemPrimTests, alienCallocShouldReturnAlignedValue) {
  oop pointer = systemPrimitives::alienCalloc(as_smiOop(4));
  ASSERT_TRUE_M(pointer->is_smi(), "result should be SmallInteger");
  int address = smiOop(pointer)->value();
  ASSERT_TRUE_M(address % 4 == 0, "not aligned");
  ASSERT_TRUE_M(address != 0, "not allocated");
  free((void*)address);
}

TEST(SystemPrimTests, alienCallocContentsShouldBeZero) {
  char message[100];
  oop pointer = systemPrimitives::alienCalloc(as_smiOop(4));
  char* address = (char*) smiOop(pointer)->value();
  for (int index = 0; index < 4; index++) {
    sprintf(message, "char %d should be zero", index);
    ASSERT_EQUALS_M((int)address[index], 0, message);
  }
  free((void*)address);
}

TEST(SystemPrimTests, alienCallocShouldReturnMarkedSymbolWhenNotSmi) {
  oop pointer = systemPrimitives::alienCalloc(vmSymbols::abs());
  ASSERT_TRUE_M(pointer->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_has_wrong_type()), pointer, "wrong symbol returned");
}

TEST(SystemPrimTests, alienCallocShouldReturnMarkedSymbolWhenSizeNegative) {
  oop pointer = systemPrimitives::alienCalloc(as_smiOop(-1));
  ASSERT_TRUE_M(pointer->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_is_invalid()), pointer, "wrong symbol returned");
}

TEST(SystemPrimTests, alienFreeShouldReturnMarkedSymbolWhenNotSmi) {
  oop result = systemPrimitives::alienFree(vmSymbols::abs());
  ASSERT_TRUE_M(result->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_has_wrong_type()), result, "wrong symbol returned");
}

TEST(SystemPrimTests, alienCallocShouldReturnMarkedSymbolWhenAddressZero) {
  oop result = systemPrimitives::alienCalloc(as_smiOop(0));
  
  ASSERT_TRUE_M(result->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_is_invalid()), result, "wrong symbol returned");
}

TEST(SystemPrimTests, alienFreeShouldReturnMarkedSymbolWhenAddressZero) {
  oop result = systemPrimitives::alienFree(as_smiOop(0));
  
  ASSERT_TRUE_M(result->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_is_invalid()), result, "wrong symbol returned");
}

TEST(SystemPrimTests, alienFreeShouldReturnMarkedSymbolWhenAddressLargeIntegerZero) {
  oop result = systemPrimitives::alienFree(as_large_integer(0));
  
  ASSERT_TRUE_M(result->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_is_invalid()), result, "wrong symbol returned");
}

TEST(SystemPrimTests, alienFreeShouldReturnMarkedSymbolWhenLargeIntegerAddressTooBig) {
  oop largeInteger = as_large_integer(256*256*256);
  oop tooBig = byteArrayPrimitives::largeIntegerMultiply(largeInteger, largeInteger);
  oop result = systemPrimitives::alienFree(tooBig);

  ASSERT_TRUE_M(result->is_mark(), "return should be marked");
  ASSERT_EQUALS_M(markSymbol(vmSymbols::argument_is_invalid()), result, "wrong symbol returned");
}

TEST(SystemPrimTests, alienFreeShouldFreeLargeIntegerAddress) {
  int address = (int) malloc(4);
  oop result = systemPrimitives::alienFree(as_large_integer(address));
  
  ASSERT_TRUE_M(!result->is_mark(), "should not be marked");
  ASSERT_TRUE_M(result == trueObj, "result should be true");
}
