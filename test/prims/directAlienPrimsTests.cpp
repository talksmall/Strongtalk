# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
#include "delta.hpp"
#include "testUtils.hpp"

using namespace easyunit;

extern "C" int expansion_count;
DECLARE(DirectAlienPrimsTests)
  klassOop byteArrayClass;
  byteArrayOop alien;

  void checkMarkedSymbol(char* message, oop result, symbolOop expected) {
    char text[200];
    ASSERT_TRUE_M(result->is_mark(), "Should be marked");
    sprintf(text,"Should be: %s, was: %s", message, unmarkSymbol(result)->as_string());
    ASSERT_TRUE_M(unmarkSymbol(result) == expected, text);
  }
END_DECLARE

SETUP(DirectAlienPrimsTests) {
  byteArrayClass = klassOop(Universe::find_global("ByteArray"));
  alien = byteArrayOop(byteArrayPrimitives::allocateSize2(falseObj, as_smiOop(8), byteArrayClass));
  byteArrayPrimitives::alienSetSize(as_smiOop(4), alien);
}

TEARDOWN(DirectAlienPrimsTests){
  MarkSweep::collect();
}

TESTF(DirectAlienPrimsTests, alienUnsignedByteAtPutShouldSetUnsignedByte) {
  byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(255), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(255, smiOop(result)->value(), "Should set value");
}

TESTF(DirectAlienPrimsTests, alienUnsignedByteAtPutShouldReturnAssignedByte) {
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(255), as_smiOop(1), alien);
  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(255, smiOop(result)->value(), "Should return value");
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtShouldReturnSignedValue) {
  byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(255), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "Should return correct value");
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtShouldReturnMarkedSymbolWhenIndexNotSmallInteger) {
  oop result = byteArrayPrimitives::alienSignedByteAt(alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(0), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtShouldReturnMarkedSymbolWhenIndexTooLarge) {
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(5), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldReturnSignedValue) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(-1), as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "Should return correct value");
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldSetCorrectValue) {
  byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(-1), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "Should return correct value");
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldSetCorrectValueWhenPositive) {
  byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(1), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(1, smiOop(result)->value(), "Should return correct value");
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(1), as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldReturnMarkedSymbolWhenIndexNotSmallInteger) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(1), alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldReturnMarkedSymbolWhenValieNotSmallInteger) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(alien, as_smiOop(1), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(0), as_smiOop(0), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldReturnMarkedSymbolWhenIndexTooBig) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(0), as_smiOop(5), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldReturnMarkedSymbolWhenValueTooSmall) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(-129), as_smiOop(1), alien);

  checkMarkedSymbol("invalid value", result, vmSymbols::argument_is_invalid());
}

TESTF(DirectAlienPrimsTests, alienSignedByteAtPutShouldReturnMarkedSymbolWhenValueTooBig) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(128), as_smiOop(1), alien);

  checkMarkedSymbol("invalid value", result, vmSymbols::argument_is_invalid());
}
