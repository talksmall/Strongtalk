# include "incls/_precompiled.incl"
# include "incls/_smi_prims.cpp.incl"
#include "test.h"

using namespace easyunit;

typedef oop (__stdcall * smifntype)(smiOop, smiOop);

extern "C" int expansion_count;
DECLARE(SmiPrimsTests)
  smifntype smiQuo;
  symbolOop quoSymbol;
END_DECLARE

SETUP(SmiPrimsTests) {
  quoSymbol = oopFactory::new_symbol("primitiveQuo:ifFail:");
  primitive_desc* prim = primitives::lookup(quoSymbol);
  smiQuo = smifntype(prim->fn());
}

TEARDOWN(SmiPrimsTests){
}

TESTF(SmiPrimsTests, quoShouldReturnDivideReceiverByArgument) {
  ASSERT_EQUALS(5, smiOop(smiQuo(as_smiOop(2), as_smiOop(10)))->value());
}

TESTF(SmiPrimsTests, quoShouldReturnReceiverHasWrongTypeWhenNotSMI) {
  oop result = smiQuo(as_smiOop(2), smiOop(quoSymbol));
  ASSERT_EQUALS((int)markSymbol(vmSymbols::receiver_has_wrong_type()), (int)result);
}
