# include "incls/_precompiled.incl"
# include "incls/_dByteArray_prims.cpp.incl"
#include "test.h"
#include "handle.hpp"

using namespace easyunit;

extern "C" int expansion_count;
DECLARE(DByteArrayPrimsTests)
  klassOop dByteArrayClass;
END_DECLARE

SETUP(DByteArrayPrimsTests) {
  dByteArrayClass = klassOop(Universe::find_global("String"));
}

TEARDOWN(DByteArrayPrimsTests){
  MarkSweep::collect();
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldAllocateDByteArrayOfCorrectSize) {
  HandleMark handles;
  Handle dByteArrayClassHandle(dByteArrayClass);
  oop result = doubleByteArrayPrimitives::allocateSize2(falseObj, as_smiOop(10), dByteArrayClass);
  ASSERT_TRUE(result->is_doubleByteArray());
  ASSERT_TRUE(!Universe::old_gen.contains(result));
  ASSERT_TRUE(Universe::new_gen.contains(result));
  ASSERT_EQUALS(10, doubleByteArrayOop(result)->length());
  ASSERT_EQUALS((char*)dByteArrayClassHandle.as_klass(), (char*)result->klass());
  for (int index = 10; index > 0; index--)
    ASSERT_EQUALS(doubleByte(0), doubleByteArrayOop(result)->doubleByte_at(index));
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldAllocateTenuredWhenRequested) {
  HandleMark handles;
  Handle classHandle(dByteArrayClass);
  oop result = doubleByteArrayPrimitives::allocateSize2(trueObj,
                                                  as_smiOop(10),
                                                  dByteArrayClass);
  ASSERT_TRUE(result->is_doubleByteArray());
  ASSERT_TRUE(Universe::old_gen.contains(result));
  ASSERT_EQUALS(10, doubleByteArrayOop(result)->length());
  ASSERT_EQUALS((char*)classHandle.as_klass(), (char*)result->klass());
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldFailWithNonDoubleByteArray) {
  oop result = doubleByteArrayPrimitives::allocateSize2(falseObj, as_smiOop(10), Universe::find_global("Object"));
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((int)markSymbol(vmSymbols::invalid_klass()), (int)result);
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldFailWithNonKlass) {
  oop result = doubleByteArrayPrimitives::allocateSize2(falseObj, as_smiOop(10), Universe::trueObj());
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((int)markSymbol(vmSymbols::invalid_klass()), (int)result);
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldFailWithNonInteger) {
  oop result = doubleByteArrayPrimitives::allocateSize2(falseObj, dByteArrayClass, dByteArrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((char*)markSymbol(vmSymbols::first_argument_has_wrong_type()), (char*)result);
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldFailWithNegativeSize) {
  oop result = doubleByteArrayPrimitives::allocateSize2(falseObj, as_smiOop(-1), dByteArrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((char*)markSymbol(vmSymbols::negative_size()), (char*)result);
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldFailWhenTenuredNotBoolean) {
  oop result = doubleByteArrayPrimitives::allocateSize2(Universe::nilObj(), as_smiOop(10), dByteArrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((char*)markSymbol(vmSymbols::second_argument_has_wrong_type()), (char*)result);
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldFailWhenInsufficientSpace) {
  int size = Universe::new_gen.eden()->free();
  oop result = doubleByteArrayPrimitives::allocateSize2(falseObj, as_smiOop(size + 1), dByteArrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS_M(markSymbol(vmSymbols::failed_allocation()), result, unmarkSymbol(result)->as_string());
}

TESTF(DByteArrayPrimsTests, allocateSize2ShouldFailWhenTooBigForOldGen) {
  int size = Universe::old_gen.free();
  oop result = doubleByteArrayPrimitives::allocateSize2(trueObj, as_smiOop(size + 1), dByteArrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS_M(markSymbol(vmSymbols::failed_allocation()), result, unmarkSymbol(result)->as_string());
}
