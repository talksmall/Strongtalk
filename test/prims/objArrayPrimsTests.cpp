# include "incls/_precompiled.incl"
# include "incls/_objArray_prims.cpp.incl"
#include "test.h"

using namespace easyunit;

extern "C" int expansion_count;
DECLARE(ObjectArrayPrimsTests)
  klassOop arrayClass;
END_DECLARE

SETUP(ObjectArrayPrimsTests) {
  arrayClass = klassOop(Universe::find_global("Array"));
}

TEARDOWN(ObjectArrayPrimsTests){
  MarkSweep::collect();
}

TESTF(ObjectArrayPrimsTests, allocateSize2ShouldAllocateArrayOfCorrectSize) {
  HandleMark handles;
  Handle arrayClassHandle(arrayClass);

  oop result = objArrayPrimitives::allocateSize2(falseObj, as_smiOop(10), arrayClass);
  ASSERT_TRUE(result->is_objArray());
  ASSERT_EQUALS(10, objArrayOop(result)->length());
  ASSERT_EQUALS((char*)arrayClassHandle.as_klass(), (char*)result->klass());
  for (int index = 10; index > 0; index--)
    ASSERT_TRUE(Universe::nilObj() == (objArrayOop(result)->obj_at(index)));
}

TESTF(ObjectArrayPrimsTests, allocateSize2ShouldAllocateTenuredArrayOfCorrectSize) {
  HandleMark handles;
  Handle arrayClassHandle(arrayClass);

  oop result = objArrayPrimitives::allocateSize2(trueObj, as_smiOop(10), arrayClass);
  ASSERT_TRUE(result->is_objArray());
  ASSERT_EQUALS(10, objArrayOop(result)->length());
  ASSERT_TRUE(result->is_old());
  ASSERT_EQUALS((char*)arrayClassHandle.as_klass(), (char*)result->klass());
  for (int index = 10; index > 0; index--)
    ASSERT_TRUE(Universe::nilObj() == (objArrayOop(result)->obj_at(index)));
}

TESTF(ObjectArrayPrimsTests, allocateSize2ShouldFailWithNonObjectArray) {
  oop result = objArrayPrimitives::allocateSize2(falseObj, as_smiOop(10), Universe::find_global("Object"));
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((char*)markSymbol(vmSymbols::invalid_klass()), (char*)result);
}

TESTF(ObjectArrayPrimsTests, allocateSize2ShouldFailWithNonInteger) {
  oop result = objArrayPrimitives::allocateSize2(falseObj, arrayClass, arrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((char*)markSymbol(vmSymbols::first_argument_has_wrong_type()), (char*)result);
}

TESTF(ObjectArrayPrimsTests, allocateSize2ShouldFailWithNegativeSize) {
  oop result = objArrayPrimitives::allocateSize2(falseObj, as_smiOop(-1), arrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((char*)markSymbol(vmSymbols::negative_size()), (char*)result);
}

TESTF(ObjectArrayPrimsTests, allocateSize2ShouldFailWhenTenuredNotBoolean) {
  oop result = objArrayPrimitives::allocateSize2(Universe::nilObj(), as_smiOop(10), arrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS((char*)markSymbol(vmSymbols::second_argument_has_wrong_type()), (char*)result);
}

TESTF(ObjectArrayPrimsTests, allocateSize2ShouldFailWhenInsufficientSpace) {
  int size = Universe::new_gen.eden()->free() / oopSize;
  oop result = objArrayPrimitives::allocateSize2(falseObj, as_smiOop(size + 1), arrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS_M(markSymbol(vmSymbols::failed_allocation()), result, unmarkSymbol(result)->as_string());
}

TESTF(ObjectArrayPrimsTests, allocateSize2ShouldFailWhenTooBigForOldGen) {
  int size = Universe::old_gen.free() / oopSize;
  oop result = objArrayPrimitives::allocateSize2(trueObj, as_smiOop(size + 1), arrayClass);
  ASSERT_TRUE(result->is_mark());
  ASSERT_EQUALS_M(markSymbol(vmSymbols::failed_allocation()), result, unmarkSymbol(result)->as_string());
}
