# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"

using namespace easyunit;

extern "C" int expansion_count;
DECLARE(DirectAlienPrimsTests)
  HeapResourceMark *rm;
  byteArrayOop alien;
  byteArrayOop largeUnsignedInteger;
  byteArrayOop largeUnsignedInteger2;
  byteArrayOop veryLargeUnsignedInteger;
  byteArrayOop largeSignedInteger;
  doubleOop    doubleValue;

  void checkMarkedSymbol(char* message, oop result, symbolOop expected) {
    char text[200];
    ASSERT_TRUE_M(result->is_mark(), "Should be marked");
    sprintf(text,"Should be: %s, was: %s", message, unmarkSymbol(result)->as_string());
    ASSERT_TRUE_M(unmarkSymbol(result) == expected, text);
  }
  void checkLargeInteger(oop result, int expected) {
    char message[200];
    ASSERT_TRUE_M(result->is_byteArray(), "Should be integer");
    bool ok;
    int actual = byteArrayOop(result)->number().as_int(ok);
    ASSERT_TRUE_M(ok, "should be integer");
    sprintf(message, "wrong value. expected: %d, was: %d", expected, actual);
    ASSERT_EQUALS_M(expected, actual, message);
  }
  void checkLargeUnsigned(oop result, unsigned int expected) {
    char message[200];
    ASSERT_TRUE_M(result->is_byteArray(), "Should be integer");
    bool ok;
    unsigned int actual = byteArrayOop(result)->number().as_unsigned_int(ok);
    ASSERT_TRUE_M(ok, "should be integer");
    sprintf(message, "wrong value. expected: %d, was: %d", expected, actual);
    ASSERT_EQUALS_M(expected, actual, message);
  }
  void checkSmallInteger(oop result, int expected) {
    char message[200];
    ASSERT_TRUE_M(result->is_smi(), "Should be small integer");
    int actual = smiOop(result)->value();
    sprintf(message, "wrong value. expected: %d, was: %d", expected, actual);
    ASSERT_EQUALS_M(expected, actual, message);
  }
END_DECLARE

SETUP(DirectAlienPrimsTests) {
  rm = new HeapResourceMark();
  PersistentHandle bac(Universe::byteArrayKlassObj());
  PersistentHandle ah(klassOop(bac.as_oop())->klass_part()->allocateObjectSize(12));
  PersistentHandle lu(klassOop(bac.as_oop())->klass_part()->allocateObjectSize(8));
  PersistentHandle lu2(klassOop(bac.as_oop())->klass_part()->allocateObjectSize(8));
  PersistentHandle vlu(klassOop(bac.as_oop())->klass_part()->allocateObjectSize(12));
  PersistentHandle ls(klassOop(bac.as_oop())->klass_part()->allocateObjectSize(8));

  alien = byteArrayOop(ah.as_oop());
  largeUnsignedInteger = byteArrayOop(lu.as_oop());
  largeUnsignedInteger2 = byteArrayOop(lu2.as_oop());
  veryLargeUnsignedInteger = byteArrayOop(vlu.as_oop());
  largeSignedInteger = byteArrayOop(ls.as_oop());
  doubleValue = doubleOop(Universe::doubleKlassObj()->klass_part()->allocateObject());
  doubleValue->set_value(1.625);

  IntegerOps::unsigned_int_to_Integer((unsigned int)0xFFFFFFFF, byteArrayOop(largeUnsignedInteger)->number());
  IntegerOps::int_to_Integer(1 << 30, byteArrayOop(largeUnsignedInteger2)->number());
  IntegerOps::int_to_Integer(-1 << 31, byteArrayOop(largeSignedInteger)->number());
  IntegerOps::mul(largeUnsignedInteger->number(), largeUnsignedInteger->number(), veryLargeUnsignedInteger->number());
  byteArrayPrimitives::alienSetSize(as_smiOop(8), alien);
}

TEARDOWN(DirectAlienPrimsTests){
  MarkSweep::collect();
  delete rm;
  rm = NULL;
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
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(9), alien);

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
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(0), as_smiOop(9), alien);

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
  oop result = byteArrayPrimitives::alienUnsignedShortAt(as_smiOop(8), alien);

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
  oop result = byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(1), as_smiOop(8), alien);

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
  oop result = byteArrayPrimitives::alienSignedShortAt(as_smiOop(8), alien);

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
  oop result = byteArrayPrimitives::alienSignedShortAtPut(as_smiOop(1), as_smiOop(8), alien);

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

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtShouldReturnCorrectValue) {
  byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(65535), as_smiOop(1), alien);
  byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(65535), as_smiOop(3), alien);

  oop result = byteArrayPrimitives::alienUnsignedLongAt(as_smiOop(1), alien);

  checkLargeUnsigned(result, 0xFFFFFFFF);
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtShouldReturnSmallIntegerWhenResultSmall) {
  byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(1), as_smiOop(1), alien);
  byteArrayPrimitives::alienUnsignedShortAtPut(as_smiOop(0), as_smiOop(3), alien);

  oop result = byteArrayPrimitives::alienUnsignedLongAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "Should be small integer");
  ASSERT_EQUALS_M(1, smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienUnsignedLongAt(as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienUnsignedLongAt(alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienUnsignedLongAt(as_smiOop(0), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtShouldReturnMarkedSymbolWhenIndexTooBig) {
  oop result = byteArrayPrimitives::alienUnsignedLongAt(as_smiOop(6), alien);

  checkMarkedSymbol("index invalid", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldReturnCorrectValue) {
  oop result = byteArrayPrimitives::alienUnsignedLongAtPut(largeUnsignedInteger, as_smiOop(1), alien);

  checkLargeUnsigned(result, 0xFFFFFFFF);
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldSetCorrectValue) {
  byteArrayPrimitives::alienUnsignedLongAtPut(largeUnsignedInteger, as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienUnsignedLongAt(as_smiOop(1), alien);

  checkLargeUnsigned(result, 0xFFFFFFFF);
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldReturnMarkedSymbolWhenValueTooLarge) {
  oop result = byteArrayPrimitives::alienUnsignedLongAtPut(veryLargeUnsignedInteger, as_smiOop(1), alien);

  checkMarkedSymbol("invalid value", result, vmSymbols::argument_is_invalid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldSetCorrectValueWithSMI) {
  byteArrayPrimitives::alienUnsignedLongAtPut(as_smiOop(1), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienUnsignedLongAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "Should be small integer");
  ASSERT_TRUE_M(1 == smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienUnsignedLongAtPut(as_smiOop(1), as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienUnsignedLongAtPut(as_smiOop(1), alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldReturnMarkedSymbolWhenValueNotInteger) {
  oop result = byteArrayPrimitives::alienUnsignedLongAtPut(Universe::byteArrayKlassObj(), as_smiOop(1), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienUnsignedLongAtPut(as_smiOop(1), as_smiOop(0), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienUnsignedLongAtPutShouldReturnMarkedSymbolWhenIndexTooLarge) {
  oop result = byteArrayPrimitives::alienUnsignedLongAtPut(as_smiOop(1), as_smiOop(6), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtShouldReturnCorrectValue) {
  byteArrayPrimitives::alienUnsignedLongAtPut(largeUnsignedInteger, as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedLongAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "Should be integer");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtShouldReturnLargeInteger) {
  byteArrayPrimitives::alienUnsignedLongAtPut(largeUnsignedInteger2, as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedLongAt(as_smiOop(1), alien);

  checkLargeInteger(result, 1 << 30);
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienSignedLongAt(as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienSignedLongAt(alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienSignedLongAt(as_smiOop(0), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtShouldReturnMarkedSymbolWhenIndexTooLarge) {
  oop result = byteArrayPrimitives::alienSignedLongAt(as_smiOop(6), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutShouldReturnCorrectValue) {
  oop result = byteArrayPrimitives::alienSignedLongAtPut(largeSignedInteger, as_smiOop(1), alien);

  checkLargeInteger(result, -1 << 31);
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutShouldSetCorrectValue) {
  byteArrayPrimitives::alienSignedLongAtPut(largeSignedInteger, as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedLongAt(as_smiOop(1), alien);

  checkLargeInteger(result, -1 << 31);
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutWithSMIShouldSetCorrectValue) {
  byteArrayPrimitives::alienSignedLongAtPut(as_smiOop(1), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedLongAt(as_smiOop(1), alien);

  checkSmallInteger(result, 1);
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienSignedLongAtPut(as_smiOop(1), as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienSignedLongAtPut(as_smiOop(1), alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutShouldReturnMarkedSymbolWhenValueNotInteger) {
  oop result = byteArrayPrimitives::alienSignedLongAtPut(Universe::byteArrayKlassObj(), as_smiOop(1), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienSignedLongAtPut(as_smiOop(1), as_smiOop(0), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutShouldReturnMarkedSymbolWhenIndexTooLarge) {
  oop result = byteArrayPrimitives::alienSignedLongAtPut(as_smiOop(1), as_smiOop(6), alien);

  checkMarkedSymbol("invalid index", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienSignedLongAtPutShouldReturnMarkedSymbolWhenValueTooLarge) {
  oop result = byteArrayPrimitives::alienSignedLongAtPut(veryLargeUnsignedInteger, as_smiOop(1), alien);

  checkMarkedSymbol("invalid value", result, vmSymbols::argument_is_invalid());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtPutShouldReturnValue) {
  oop result = byteArrayPrimitives::alienDoubleAtPut(doubleValue, as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_double(), "should be double");
  ASSERT_EQUALS_M(1.625, doubleOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienDoubleAtPutShouldSetCorrectValue) {
  byteArrayPrimitives::alienDoubleAtPut(doubleValue, as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienDoubleAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_double(), "should be double");
  ASSERT_EQUALS_M(1.625, doubleOop(result)->value(), "wrong value");
  ASSERT_EQUALS_M(1.625, ((double*)(alien->bytes() + 4))[0], "value not set");
}

TESTF(DirectAlienPrimsTests, alienDoubleAtPutShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienDoubleAtPut(doubleValue, as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienDoubleAt(as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtPutShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienDoubleAtPut(doubleValue, alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienDoubleAt(alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtPutShouldReturnMarkedSymbolWhenValueNotDouble) {
  oop result = byteArrayPrimitives::alienDoubleAtPut(as_smiOop(1), as_smiOop(1), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtPutShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienDoubleAtPut(doubleValue, as_smiOop(0), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienDoubleAt(as_smiOop(0), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtPutShouldReturnMarkedSymbolWhenIndexTooLarge) {
  oop result = byteArrayPrimitives::alienDoubleAtPut(doubleValue, as_smiOop(2), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienDoubleAtShouldReturnMarkedSymbolWhenIndexTooLarge) {
  oop result = byteArrayPrimitives::alienDoubleAt(as_smiOop(2), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienFloatAtPutShouldSetCorrectValue) {
  byteArrayPrimitives::alienFloatAtPut(doubleValue, as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienFloatAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_double(), "should be double");
  ASSERT_EQUALS_M(1.625, doubleOop(result)->value(), "wrong value");
  ASSERT_EQUALS_M(1.625F, ((float*)(alien->bytes() + 4))[0], "value not set");
}

TESTF(DirectAlienPrimsTests, alienFloatAtPutShouldSetCorrectValueAt2) {
  byteArrayPrimitives::alienFloatAtPut(doubleValue, as_smiOop(2), alien);
  oop result = byteArrayPrimitives::alienFloatAt(as_smiOop(2), alien);

  ASSERT_TRUE_M(result->is_double(), "should be double");
  ASSERT_EQUALS_M(1.625, doubleOop(result)->value(), "wrong value");
  ASSERT_EQUALS_M(1.625F, ((float*)(alien->bytes() + 5))[0], "value not set");
}

TESTF(DirectAlienPrimsTests, alienFloatAtPutShouldReturnCorrectValue) {
  oop result = byteArrayPrimitives::alienFloatAtPut(doubleValue, as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_double(), "should be double");
  ASSERT_EQUALS_M(1.625, doubleOop(result)->value(), "wrong value");
}

TESTF(DirectAlienPrimsTests, alienFloatAtPutShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienFloatAtPut(doubleValue, as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienFloatAtShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienFloatAt(as_smiOop(1), as_smiOop(0));

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienFloatAtPutShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienFloatAtPut(doubleValue, alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienFloatAtShouldReturnMarkedSymbolWhenIndexNotSMI) {
  oop result = byteArrayPrimitives::alienFloatAt(alien, alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienFloatAtPutShouldReturnMarkedSymbolWhenValueNotDouble) {
  oop result = byteArrayPrimitives::alienFloatAtPut(as_smiOop(1), as_smiOop(1), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(DirectAlienPrimsTests, alienFloatAtShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienFloatAt(as_smiOop(0), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienFloatAtPutShouldReturnMarkedSymbolWhenIndexTooSmall) {
  oop result = byteArrayPrimitives::alienFloatAtPut(doubleValue, as_smiOop(0), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienFloatAtPutShouldReturnMarkedSymbolWhenIndexTooLarge) {
  oop result = byteArrayPrimitives::alienFloatAtPut(doubleValue, as_smiOop(6), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(DirectAlienPrimsTests, alienFloatAtShouldReturnMarkedSymbolWhenIndexTooLarge) {
  oop result = byteArrayPrimitives::alienFloatAt(as_smiOop(6), alien);

  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}
