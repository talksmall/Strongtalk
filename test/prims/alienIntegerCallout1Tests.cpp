# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"
#include <time.h>
#include <stdlib.h>

using namespace easyunit;

typedef struct _size5 {
  int ignore;
  char byte;
} size5_t;
extern "C" int PRIM_API callLabs(int*ptr) {
  return labs(*ptr);
}

extern "C" int PRIM_API size5(size5_t arg) {
  return arg.byte == -1 ? 0 : -1;
}

extern "C" oop PRIM_API forceScavenge1(int ignore) {
  Universe::scavenge();
  return vmSymbols::completed();
}

extern "C" int PRIM_API argAlignment(int a) {
  return ((int) &a) & 0xF;
}

extern "C" char* PRIM_API argUnsafe1(char* a) {
  return a;
}

DECLARE(AlienIntegerCallout1Tests)
HeapResourceMark *rm;
GrowableArray<PersistentHandle**> *handles;
PersistentHandle *resultAlien, *addressAlien, *pointerAlien, *functionAlien; 
PersistentHandle *directAlien, *invalidFunctionAlien, *unsafeAlien, *unsafeContents;
smiOop smi0, smi1;
char address[16];

void allocateAlien(PersistentHandle* &alienHandle, int arraySize, int alienSize, void* ptr = NULL) {
  byteArrayOop alien = byteArrayOop(Universe::byteArrayKlassObj()->klass_part()->allocateObjectSize(arraySize));
  byteArrayPrimitives::alienSetSize(as_smiOop(alienSize), alien);
  if (ptr)
    byteArrayPrimitives::alienSetAddress(as_smiOop((int)ptr), alien);
  alienHandle = new PersistentHandle(alien);
  handles->append(&alienHandle);
}
void checkMarkedSymbol(char* message, oop result, symbolOop expected) {
  char text[200];
  ASSERT_TRUE_M(result->is_mark(), "Should be marked");
  sprintf(text,"Should be: %s, was: %s", message, unmarkSymbol(result)->as_string());
  ASSERT_TRUE_M(unmarkSymbol(result) == expected, text);
}
void checkIntResult(char* message, int expected, PersistentHandle* alien) {
  char text[200];
  bool ok;
  int actual = asInt(ok, byteArrayPrimitives::alienSignedLongAt(smi1, alien->as_oop()));
  ASSERT_TRUE_M(ok, "not an integer result");
  sprintf(text,"Should be: %d, was: %d", expected, actual);
  ASSERT_TRUE_M(actual == expected, text);
}
int asInt(bool &ok, oop intOop) {
  ok = true;
  if (intOop->is_smi())
    return smiOop(intOop)->value();
  if (!intOop->is_byteArray()) {
    ok = false;
    return 0;
  }
  return byteArrayOop(intOop)->number().as_int(ok);
}
oop asOop(int value) {
  int size = IntegerOps::int_to_Integer_result_size_in_bytes(value);
  byteArrayOop valueOop = byteArrayOop(Universe::byteArrayKlassObj()->klass_part()->allocateObjectSize(size));
  IntegerOps::int_to_Integer(value, valueOop->number());
  bool ok;
  oop result = valueOop->number().as_smi(ok);
  return ok ? result : valueOop;
}
void release(PersistentHandle** handle) {
  delete *handle;
  *handle = NULL;
}
void allocateUnsafe(PersistentHandle* &handle, PersistentHandle* &contents) {
  klassOop unsafeKlass = klassOop(Universe::find_global("UnsafeAlien"));
  unsafeAlien = new PersistentHandle(unsafeKlass->primitive_allocate());
  int offset = unsafeKlass->klass_part()->lookup_inst_var(oopFactory::new_symbol("nonPointerObject"));

  contents = new PersistentHandle(Universe::byteArrayKlassObj()->primitive_allocate_size(12));
  memOop(unsafeAlien->as_oop())->instVarAtPut(offset, contents->as_oop());
}
void setAddress(void* p, PersistentHandle* alien) {
  byteArrayPrimitives::alienSetAddress(asOop((int)p), alien->as_oop());
}
END_DECLARE

SETUP(AlienIntegerCallout1Tests) {
  rm = new HeapResourceMark();
  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);
  handles = new(true) GrowableArray<PersistentHandle**>(5);

  allocateAlien(functionAlien,        8,  0, &labs);
  allocateAlien(resultAlien,         12,  8);
  allocateAlien(directAlien,         12,  4);
  allocateAlien(addressAlien,         8, -4, &address);
  allocateAlien(pointerAlien,         8,  0, &address);
  allocateAlien(invalidFunctionAlien, 8,  0);
  allocateUnsafe(unsafeAlien, unsafeContents);

  memset(address, 0, 8);
}

TEARDOWN(AlienIntegerCallout1Tests){
  while(!handles->isEmpty())
    release(handles->pop());
  free(handles);
  handles = NULL;
  delete rm;
  rm = NULL;
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1ShouldCallFunction) {
  byteArrayPrimitives::alienCallResult1(as_smiOop(-1), resultAlien->as_oop(), functionAlien->as_oop());

  checkIntResult("wrong result", labs(-1), resultAlien);
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1WithUnsafeAlienShouldCallFunction) {
  setAddress(&argUnsafe1, functionAlien);
  byteArrayPrimitives::alienCallResult1(unsafeAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());

  checkIntResult("wrong result", (int)byteArrayOop(unsafeContents->as_oop())->bytes(), resultAlien);
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1ShouldCallFunctionAndIgnoreResultWhenResultAlienNil) {
  oop result = byteArrayPrimitives::alienCallResult1(as_smiOop(-1), nilObj, functionAlien->as_oop());
  ASSERT_TRUE_M(!result->is_mark(), "shoult not be marked");
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1WithIndirectArgumentShouldCallFunction) {
  byteArrayPrimitives::alienSignedLongAtPut(as_smiOop(-1), smi1, addressAlien->as_oop());
  byteArrayPrimitives::alienCallResult1(addressAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());

  checkIntResult("wrong result", labs(-1), resultAlien);
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1WithDirectArgumentShouldCallFunction) {
  byteArrayPrimitives::alienSignedLongAtPut(as_smiOop(-1), smi1, directAlien->as_oop());
  byteArrayPrimitives::alienCallResult1(directAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());

  checkIntResult("wrong result", labs(-1), resultAlien);
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1WithPointerArgumentShouldCallFunction) {
  oop address = asOop((int)&callLabs);
  byteArrayPrimitives::alienSetAddress(address, functionAlien->as_oop());
  byteArrayPrimitives::alienSignedLongAtPut(as_smiOop(-1), smi1, pointerAlien->as_oop());
  byteArrayPrimitives::alienCallResult1(pointerAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());

  checkIntResult("wrong result", labs(-1), resultAlien);
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1Should16ByteAlignArgs) {
  oop fnAddress = asOop((int)&argAlignment);
  byteArrayPrimitives::alienSetAddress(fnAddress, functionAlien->as_oop());

  oop result = byteArrayPrimitives::alienCallResult1(addressAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());
  ASSERT_TRUE_M(!result->is_mark(), "Should not be marked");
  checkIntResult("wrong result", 0, resultAlien);

  byteArrayPrimitives::alienSetSize(as_smiOop(-8), addressAlien->as_oop());
  byteArrayPrimitives::alienCallResult1(addressAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("wrong result", 0, resultAlien);

  byteArrayPrimitives::alienSetSize(as_smiOop(-12), addressAlien->as_oop());
  byteArrayPrimitives::alienCallResult1(addressAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("wrong result", 0, resultAlien);

  byteArrayPrimitives::alienSetSize(as_smiOop(-16), addressAlien->as_oop());
  byteArrayPrimitives::alienCallResult1(addressAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("wrong result", 0, resultAlien);
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1WithOddSizedArgumentShouldCallFunction) {
  oop address = asOop((int)&size5);
  byteArrayPrimitives::alienSetAddress(address, functionAlien->as_oop());
  byteArrayPrimitives::alienSetSize(as_smiOop(5), directAlien->as_oop());
  byteArrayPrimitives::alienUnsignedLongAtPut(smi0, smi1, directAlien->as_oop());
  byteArrayPrimitives::alienSignedByteAtPut(as_smiOop(-1), as_smiOop(5), directAlien->as_oop());

  byteArrayPrimitives::alienCallResult1(directAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());

  checkIntResult("wrong result", 0, resultAlien);
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1WithScavengeShouldReturnCorrectResult) {
  oop address = asOop((int)&forceScavenge1);
  byteArrayPrimitives::alienSetAddress(address, functionAlien->as_oop());
  byteArrayPrimitives::alienCallResult1(directAlien->as_oop(), resultAlien->as_oop(), functionAlien->as_oop());

  bool ok;
  oop result = (oop) asInt(ok, byteArrayPrimitives::alienUnsignedLongAt(smi1,
                                                                        resultAlien->as_oop()));

  ASSERT_TRUE_M(ok, "unsigned long at failed");
  ASSERT_TRUE_M(vmSymbols::completed() == result, "wrong result");
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1ShouldReturnMarkedResultForNonAlien) {
  oop result = byteArrayPrimitives::alienCallResult1(resultAlien->as_oop(),
                                                     smi0,
                                                     smi0);

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1ShouldReturnMarkedResultForDirectAlien) {
  oop result = byteArrayPrimitives::alienCallResult1(resultAlien->as_oop(), smi0,
    resultAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1ShouldReturnMarkedResultForNullFunctionPointer) {
  oop result = byteArrayPrimitives::alienCallResult1(resultAlien->as_oop(),
                                                     smi0,
                                                     invalidFunctionAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1ShouldReturnMarkedResultWhenResultNotAlien) {
  oop result = byteArrayPrimitives::alienCallResult1(smi0, smi0, functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(AlienIntegerCallout1Tests, alienCallResult1ShouldReturnMarkedResultWhenFunctionParameterNotAlienOrSMI) {
  oop result = byteArrayPrimitives::alienCallResult1(Universe::byteArrayKlassObj(),
                                                     resultAlien->as_oop(),
                                                     functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}
