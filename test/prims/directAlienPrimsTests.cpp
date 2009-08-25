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

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtShouldReturnCorrectValue) {
  byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(255), as_smiOop(1), alien);
  byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(255), as_smiOop(2), alien);

  oop result = byteArrayPrimitives::alienUnsignedShortAt(as_smiOop(1), alien);
  ASSERT_TRUE_M(result->is_smi(), "should be smi");
  ASSERT_EQUALS_M(65535, smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienUnsignedShortAt(as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienUnsignedShortAt(alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienUnsignedShortAt(as_smiOop(0), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtShouldReturnMarkedSymbolWhenIndexTooBig) {
  oop result = byteArrayPrimitives::alienUnsignedShortAt(as_smiOop(4), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldReturnCorrectValue) {
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(65535), as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "should be smi");
  ASSERT_EQUALS_M(65535, smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldSetCorrectValue) {
  byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(65535), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienUnsignedShortAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "should be smi");
  ASSERT_EQUALS_M(65535, smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(1), as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(1), alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldReturnMarkedSymbolWhenValueNotSMI) {
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(alien, as_smiOop(1), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(1), as_smiOop(0), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldReturnMarkedSymbolWhenIndexTooBig) {
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(1), as_smiOop(4), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldReturnMarkedSymbolWhenValueTooSmall) {
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(-1), as_smiOop(1), alien);

  checkMarkedSymbol("value too small", result, vmSymbols::argument_is_invalid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedShortAtPutShouldReturnMarkedSymbolWhenValueTooBig) {
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(65536), as_smiOop(1), alien);

  checkMarkedSymbol("value too small", result, vmSymbols::argument_is_invalid());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtShouldReturnCorrectValue) {
  byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(65535), as_smiOop(1), alien);

  oop result = byteArrayPrimitives::alienSignedShortAt(as_smiOop(1), alien);
  ASSERT_TRUE_M(result->is_smi(), "should be smi");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienSignedShortAt(as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienSignedShortAt(alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienSignedShortAt(as_smiOop(0), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtShouldReturnMarkedSymbolWhenIndexTooBig) {
  oop result = byteArrayPrimitives::alienSignedShortAt(as_smiOop(4), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldReturnCorrectValue) {
  oop result = byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(-1), as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "should be smi");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldSetCorrectValue) {
  byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(-1), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedShortAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "should be smi");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(1), as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(1), alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldReturnMarkedSymbolWhenValueNotSMI) {
  oop result = byteArrayPrimitives::alienSignedShortAtPut(alien, as_smiOop(1), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(1), as_smiOop(0), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldReturnMarkedSymbolWhenIndexTooBig) {
  oop result = byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(1), as_smiOop(4), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldReturnMarkedSymbolWhenValueTooSmall) {
  oop result = byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(-32769), as_smiOop(1), alien);

  checkMarkedSymbol("value too small", result, vmSymbols::argument_is_invalid());
}

TESTF(DirectAlienPrimsTests, alienSignedShortAtPutShouldReturnMarkedSymbolWhenValueTooBig) {
  oop result = byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(32768), as_smiOop(1), alien);

  checkMarkedSymbol("value too small", result, vmSymbols::argument_is_invalid());
}
