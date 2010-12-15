# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"

using namespace easyunit;

extern "C" int expansion_count;
DECLARE(ByteArrayPrimsTests)
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
    ResourceMark rm;
    char text[200];
    ASSERT_TRUE_M(result->is_mark(), "Should be marked");
    sprintf(text,
            "%s. Should be: %s, was: %s",
            message,
            expected->as_string(),
            unmarkSymbol(result)->as_string());
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

SETUP(ByteArrayPrimsTests) {
  byteArrayClass = Universe::byteArrayKlassObj();
  alien = byteArrayOop(Universe::byteArrayKlassObj()->klass_part()->allocateObjectSize(8));
  byteArrayPrimitives::alienSetSize(as_smiOop(4), alien);
  memset(alien_byte_region, 0, 16);
}

TEARDOWN(ByteArrayPrimsTests){
}

TESTF(ByteArrayPrimsTests, allocateSize2ShouldAllocateByteArrayOfCorrectSize) {
  HandleMark handles;
  Handle byteArrayClassHandle(byteArrayClass);
  oop result = byteArrayPrimitives::allocateSize2(falseObj, as_smiOop(10), byteArrayClass);
  ASSERT_TRUE(result->is_byteArray());
  ASSERT_EQUALS(10, byteArrayOop(result)->length());
  ASSERT_EQUALS((char*)byteArrayClassHandle.as_klass(), (char*)result->klass());
  for (int index = 10; index > 0; index--)
    ASSERT_EQUALS(u_char(0), byteArrayOop(result)->byte_at(index));
}

TESTF(ByteArrayPrimsTests, allocateSize2ShouldAllocateTenuredWhenRequested) {
  HandleMark handles;
  Handle byteArrayClassHandle(byteArrayClass);
  int size = Universe::new_gen.eden()->free() + 1;
  oop result = byteArrayPrimitives::allocateSize2(trueObj,
                                                  as_smiOop(size),
                                                  byteArrayClass);
  ASSERT_TRUE(result->is_byteArray());
  ASSERT_TRUE(Universe::old_gen.contains(result));
  ASSERT_EQUALS(size, byteArrayOop(result)->length());
  ASSERT_EQUALS((char*)byteArrayClassHandle.as_klass(), (char*)result->klass());
}

TESTF(ByteArrayPrimsTests, allocateSize2ShouldFailWithNonByteArray) {
  oop result = byteArrayPrimitives::allocateSize2(falseObj, as_smiOop(10), Universe::find_global("Object"));
  checkMarkedSymbol("wrong class", result, vmSymbols::invalid_klass());
}

TESTF(ByteArrayPrimsTests, allocateSize2ShouldFailWithNonInteger) {
  oop result = byteArrayPrimitives::allocateSize2(falseObj, byteArrayClass, byteArrayClass);
  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, allocateSize2ShouldFailWithNegativeSize) {
  oop result = byteArrayPrimitives::allocateSize2(falseObj, as_smiOop(-1), byteArrayClass);
  checkMarkedSymbol("negative size", result, vmSymbols::negative_size());
}

TESTF(ByteArrayPrimsTests, allocateSize2ShouldFailWhenTenuredNotBoolean) {
  oop result = byteArrayPrimitives::allocateSize2(Universe::nilObj(), as_smiOop(10), byteArrayClass);
  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, allocateSize2ShouldFailWhenInsufficientSpace) {
  int size = Universe::new_gen.eden()->free();
  oop result = byteArrayPrimitives::allocateSize2(falseObj, as_smiOop(size + 1), byteArrayClass);
  checkMarkedSymbol("failed allocation", result, vmSymbols::failed_allocation());
}

TESTF(ByteArrayPrimsTests, allocateSize2ShouldFailWhenTooBigForOldGen) {
  int size = Universe::old_gen.free();
  oop result = byteArrayPrimitives::allocateSize2(trueObj, as_smiOop(size + 1), byteArrayClass);
  checkMarkedSymbol("failed allocation", result, vmSymbols::failed_allocation());
}

TESTF(ByteArrayPrimsTests, alienSizeShouldReturnCorrectSize) {
  u_char* bytes = alien->bytes();

  ((int*)bytes)[0] = 4;

  ASSERT_EQUALS_M(4, smiOop(byteArrayPrimitives::alienGetSize(alien))->value(), "wrong size");
}

TESTF(ByteArrayPrimsTests, alienAddressShouldReturnCorrectAddress) {
  u_char* bytes = alien->bytes();

  ((int*)bytes)[0] = -16;
  ((u_char**)bytes)[1] = alien_byte_region;
  ASSERT_EQUALS_M((int)alien_byte_region, smiOop(byteArrayPrimitives::alienGetAddress(alien))->value(), "wrong address");
}

TESTF(ByteArrayPrimsTests, alienSetAddressShouldAssignCorrectAddress) {
  u_char* bytes = alien->bytes();

  int address = (int)alien_byte_region;
  ((int*)bytes)[0] = -16;
  ASSERT_TRUE_M(alien == byteArrayPrimitives::alienSetAddress(as_smiOop(address), alien), "Should return alien");
  ASSERT_EQUALS_M(address, smiOop(byteArrayPrimitives::alienGetAddress(alien))->value(), "Address should match");
}

TESTF(ByteArrayPrimsTests, alienSetAddressShouldAssignCorrectAddressFromLargeInteger) {
  PersistentHandle address(as_large_integer((int)alien_byte_region));
  byteArrayPrimitives::alienSetSize(as_smiOop(-16), alien);
  
  ASSERT_TRUE_M(alien == byteArrayPrimitives::alienSetAddress(address.as_oop(), alien), "Should return alien");
  ASSERT_EQUALS_M((int)alien_byte_region, smiOop(byteArrayPrimitives::alienGetAddress(alien))->value(), "Address should match");
}

TESTF(ByteArrayPrimsTests, alienSetAddressShouldReturnMarkedSymbolForAddressOutOfRange) {
  BlockScavenge bs;
  byteArrayPrimitives::alienSetSize(as_smiOop(0), alien);
  oop largeInteger = as_large_integer(256*256*256);
  oop tooBig = byteArrayPrimitives::largeIntegerMultiply(largeInteger, largeInteger);
  oop result = byteArrayPrimitives::alienSetAddress(tooBig, alien);
  
  checkMarkedSymbol("invalid argument", result, vmSymbols::argument_is_invalid());
}

TESTF(ByteArrayPrimsTests, alienSetAddressShouldReturnMarkedSymbolWhenNotAlien) {
  oop result = byteArrayPrimitives::alienSetAddress(smiOop(0), smiOop(0));
  checkMarkedSymbol("wrong receiver", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienSetAddressShouldReturnMarkedSymbolWhenAlienIsDirect) {
  u_char* bytes = alien->bytes();

  byteArrayPrimitives::alienSetSize(smiOop(4), alien);

  oop result = byteArrayPrimitives::alienSetAddress(smiOop(0), alien);
  checkMarkedSymbol("iillegal state", result, vmSymbols::illegal_state());
}

TESTF(ByteArrayPrimsTests, alienSetAddressShouldReturnMarkedSymbolWhenArgumentNotInteger) {
  byteArrayPrimitives::alienSetSize(smiOop(-16), alien);

  oop result = byteArrayPrimitives::alienSetAddress(trueObj, alien);
  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}
TESTF(ByteArrayPrimsTests, alienAddressShouldReturnLargeIntegerAddress) {
  u_char* bytes = alien->bytes();

  int address = -128 * 256 * 256 * 256;
  ((int*)bytes)[0] = 0;
  ((u_char**)bytes)[1] = (u_char*) address;
  oop result = byteArrayPrimitives::alienGetAddress(alien);
  ASSERT_TRUE_M(result->is_byteArray(), "should be large integer");

  Integer* number = &byteArrayOop(result)->number();
  bool ok;
  int resultAddress = number->as_int(ok);
  ASSERT_TRUE_M(ok, "too large for int");
  ASSERT_EQUALS_M(address, resultAddress, "wrong address");
}

TESTF(ByteArrayPrimsTests, alienAddressShouldReturnMarkedSymbolWhenNotByteArray) {
  oop result = byteArrayPrimitives::alienGetAddress(as_smiOop(0));
  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienSizeShouldReturnMarkedSymbolWhenNotByteArray) {
  oop result = byteArrayPrimitives::alienGetSize(as_smiOop(0));
  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienAddressShouldReturnMarkedSymbolWhenDirect) {
  u_char* bytes = alien->bytes();

  ((int*)bytes)[0] = 4;

  oop result = byteArrayPrimitives::alienGetAddress(alien);
  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(ByteArrayPrimsTests, alienSetSizeShouldReturnMarkedResultWhenReceiverNotByteArray) {
  oop result = byteArrayPrimitives::alienSetSize(as_smiOop(4), as_smiOop(0));
  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienSetSizeShouldReturnMarkedResultWhenSizeNotSMI) {
  oop result = byteArrayPrimitives::alienSetSize(vmSymbols::abs(), alien);
  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienSetSizeShouldSetCorrectSize) {
  u_char* bytes = alien->bytes();

  ASSERT_TRUE_M(alien == byteArrayPrimitives::alienSetSize(as_smiOop(4), alien), "should return receiver");
  ASSERT_EQUALS_M(4, ((int*)bytes)[0], "wrong size");
}

TESTF(ByteArrayPrimsTests, unsignedByteAtWithDirectAlienShouldReturnCorrectByte) {
  u_char* bytes = alien->bytes();

  ((int*)bytes)[0] = 4;
  setUnsignedContents(&bytes[4]);

  checkAlienContents(alien);
}

TESTF(ByteArrayPrimsTests, unsignedByteAtWithIndirectAlienShouldReturnCorrectByteFromIndirectAlien) {
  u_char* bytes = alien->bytes();
  u_char* contents = bytes + 4;

  ((int*)bytes)[0] = -16;
  ((u_char**)bytes)[1] = alien_byte_region;
  setUnsignedContents(alien_byte_region);

  checkAlienContents(alien);
}

TESTF(ByteArrayPrimsTests, unsignedByteAtWithPointerAlienShouldReturnCorrectByteFromPointerAlien) {
  u_char* bytes = alien->bytes();

  ((int*)bytes)[0] = 0;
  ((u_char**)bytes)[1] = alien_byte_region;
  setUnsignedContents(alien_byte_region);

  checkAlienContents(alien);
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtShouldReturnMarkedSymbolWhenNotByteArray) {
  oop result = byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(1), as_smiOop(0));
  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtPutShouldReturnMarkedSymbolWhenNotByteArray) {
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(1), as_smiOop(1), as_smiOop(0));
  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtPutShouldReturnMarkedSymbolWhenIndexNotSmallInteger) {
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(1), alien, alien);
  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtPutShouldReturnMarkedSymbolWhenValueNotSmallInteger) {
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(alien, as_smiOop(1), alien);
  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtPutShouldReturnMarkedSymbolWhenIndexTooSmall) {
  byteArrayPrimitives::alienSetAddress(as_smiOop((int)&alien_byte_region), alien);
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(0), as_smiOop(0), alien);
  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtPutShouldReturnMarkedSymbolWhenValueTooSmall) {
  byteArrayPrimitives::alienSetSize(as_smiOop(4), alien);
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(-1), as_smiOop(1), alien);
  checkMarkedSymbol("argument invalid", result, vmSymbols::argument_is_invalid());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtPutShouldReturnMarkedSymbolWhenValueTooLarge) {
  byteArrayPrimitives::alienSetSize(as_smiOop(4), alien);
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(256), as_smiOop(1), alien);
  checkMarkedSymbol("argument invalid", result, vmSymbols::argument_is_invalid());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtPutShouldReturnMarkedSymbolWhenIndexTooLarge) {
  byteArrayPrimitives::alienSetSize(as_smiOop(4), alien);
  oop result = byteArrayPrimitives::alienUnsignedByteAtPut(as_smiOop(0), as_smiOop(5), alien);
  checkMarkedSymbol("wrong type", result, vmSymbols::index_not_valid());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtShouldReturnMarkedSymbolWhenIndexNotSMI) {
  u_char* bytes = alien->bytes();

  ((int*)bytes)[0] = 4;
  setUnsignedContents(&bytes[4]);

  oop result = byteArrayPrimitives::alienUnsignedByteAt(vmSymbols::abs(), alien);
  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(ByteArrayPrimsTests, alienUnsignedByteAtShouldReturnMarkedSymbolWhenIndexNotInRange) {
  u_char* bytes = alien->bytes();

  ((int*)bytes)[0] = 4;
  setUnsignedContents(&bytes[4]);

  oop result = byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(5), alien);
  checkMarkedSymbol("invalid argument", result, vmSymbols::index_not_valid());
  
  result = byteArrayPrimitives::alienUnsignedByteAt(as_smiOop(0), alien);
  checkMarkedSymbol("invalid argument", result, vmSymbols::index_not_valid());
}
