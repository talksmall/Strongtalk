# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
#include "delta.hpp"
#include "testUtils.hpp"

using namespace easyunit;

extern "C" int expansion_count;
DECLARE(IndirectAlienPrimsTests)
  klassOop byteArrayClass;
  byteArrayOop alien;
  u_char alien_byte_region[16];

  void checkAlienContents(byteArrayOop alien) {
    ASSERT_TRUE_M(255 == smiOop(byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(1), alien))->value(), "Wrong byte at index 1");
    ASSERT_TRUE_M(2 == smiOop(byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(2), alien))->value(), "Wrong byte at index 2");
    ASSERT_TRUE_M(3 == smiOop(byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(3), alien))->value(), "Wrong byte at index 3");
    ASSERT_TRUE_M(4 == smiOop(byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(4), alien))->value(), "Wrong byte at index 4");
  }

  void checkMarkedSymbol(char* message, oop result, symbolOop expected) {
    char text[200];
    ASSERT_TRUE_M(result->is_mark(), "Should be marked");
    sprintf(text,"Should be: %s, was: %s", message, unmarkSymbol(result)->as_string());
    ASSERT_TRUE_M(unmarkSymbol(result) == expected, text);
  }

  void setUnsignedContents(u_char* contents) {
    contents[0] = 255;
    contents[1] = 2;
    contents[2] = 3;
    contents[3] = 4;
  }

  int asInteger(oop largeInteger, bool& ok) {
    Integer* number = &byteArrayOop(largeInteger)->number();
    return number->as_int(ok);
  }
END_DECLARE

SETUP(IndirectAlienPrimsTests) {
  byteArrayClass = klassOop(Universe::find_global("ByteArray"));
  alien = byteArrayOop(byteArrayPrimitives::allocateSize2(falseObj, as_smiOop(8), byteArrayClass));
  byteArrayPrimitives::alienSetSize(as_smiOop(-16), alien);
  byteArrayPrimitives::alienSetAddress(as_smiOop((int)alien_byte_region), alien);
  memset(alien_byte_region, 0, 16);
}

TEARDOWN(IndirectAlienPrimsTests){
  MarkSweep::collect();
}

TESTF(IndirectAlienPrimsTests, alienUnsignedByteAtPutShouldSetUnsignedByte) {
  byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(255), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(255, smiOop(result)->value(), "Should set value");
}

TESTF(IndirectAlienPrimsTests, alienUnsignedByteAtPutShouldReturnAssignedByte) {
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(255), as_smiOop(1), alien);
  
  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(255, smiOop(result)->value(), "Should return value");
}

TESTF(IndirectAlienPrimsTests, alienSignedByteAtShouldReturnCorrectByte) {
  byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(255), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "Should return value");
}

TESTF(IndirectAlienPrimsTests, alienSignedByteAtPutShouldReturnCorrectByte) {
  oop result = byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(-1), as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "Should return value");
}

TESTF(IndirectAlienPrimsTests, alienSignedByteAtPutShouldSetCorrectByte) {
  byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(-1), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(-1, smiOop(result)->value(), "Should return value");
}

TESTF(IndirectAlienPrimsTests, alienSignedByteAtPutShouldSetMaxValue) {
  byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(127), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(127, smiOop(result)->value(), "Should return value");
}

TESTF(IndirectAlienPrimsTests, alienSignedByteAtPutShouldSetMinValue) {
  byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(-128), as_smiOop(1), alien);
  oop result = byteArrayPrimitives::alienSignedByteAt(as_smiOop(1), alien);

  ASSERT_TRUE_M(result->is_smi(), "result should be SmallInteger");
  ASSERT_EQUALS_M(-128, smiOop(result)->value(), "Should return value");
}
