# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"
#include <time.h>
#include <stdlib.h>

using namespace easyunit;

extern "C" int PRIM_API returnFirst(int a, int b) {
  return a;
}

extern "C" int PRIM_API returnFirstPointer(int *a, int b) {
  return *a;
}

extern "C" int PRIM_API returnSecond(int a, int b) {
  return b;
}

extern "C" int PRIM_API returnSecondPointer(int a, int *b) {
  return *b;
}

extern "C" int PRIM_API forceScavenge2(int ignore1, int ignore2) {
  Universe::scavenge();
  return -1;
}

//extern "C" int PRIM_API size5(int ignore, char byte) {
//  return byte == -1 ? 0 : -1;
//}
//
//extern "C" oop PRIM_API forceScavenge(int ignore) {
//  Universe::scavenge();
//  return vmSymbols::completed();
//}

DECLARE(AlienIntegerCallout2Tests)
HeapResourceMark *rm;
GrowableArray<PersistentHandle**> *handles;
PersistentHandle *resultAlien, *addressAlien, *pointerAlien, *functionAlien; 
PersistentHandle *directAlien, *invalidFunctionAlien;
smiOop smi0, smi1, smim1;
static const int argCount = 2;
void* intCalloutFunctions[argCount];
void* intPointerCalloutFunctions[argCount];
char address[8];

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
void setAddress(PersistentHandle* handle, void* argument) {
  byteArrayPrimitives::alienSetAddress(asOop((int)argument), handle->as_oop());
}
void checkArgnPassed(int argIndex, int argValue, void**functionArray) {
  setAddress(functionAlien, functionArray[argIndex]);
  oop arg0 = argIndex == 0 ? asOop(argValue) : smi0;
  oop arg1 = argIndex == 1 ? asOop(argValue) : smi0;
  oop result = byteArrayPrimitives::alienCallResult2(arg1, arg0, resultAlien->as_oop(), functionAlien->as_oop());

  ASSERT_TRUE_M(result == resultAlien->as_oop(), "Should return result alien");
  checkIntResult("wrong result", argValue, resultAlien);
}
void checkArgnPtrPassed(int argIndex, int argValue, void**functionArray) {
  setAddress(functionAlien, functionArray[argIndex]);
  byteArrayPrimitives::alienSignedLongAtPut(asOop(argValue), smi1, pointerAlien->as_oop());
  oop arg0 = argIndex == 0 ? pointerAlien->as_oop() : smi0;
  oop arg1 = argIndex == 1 ? pointerAlien->as_oop() : smi0;
  oop result = byteArrayPrimitives::alienCallResult2(arg1, arg0, resultAlien->as_oop(), functionAlien->as_oop());

  ASSERT_TRUE_M(result == resultAlien->as_oop(), "Should return result alien");
  checkIntResult("wrong result", argValue, resultAlien);
}
END_DECLARE

SETUP(AlienIntegerCallout2Tests) {
  rm = new HeapResourceMark();
  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);
  smim1 = as_smiOop(-1);
  handles = new(true) GrowableArray<PersistentHandle**>(5);

  allocateAlien(functionAlien,        8,  0, &returnFirst);
  allocateAlien(resultAlien,         12,  8);
  allocateAlien(directAlien,         12,  4);
  allocateAlien(addressAlien,         8, -4, &address);
  allocateAlien(pointerAlien,         8,  0, &address);
  allocateAlien(invalidFunctionAlien, 8,  0);

  memset(address, 0, 8);

  intCalloutFunctions[0] = returnFirst;
  intCalloutFunctions[1] = returnSecond;
  intPointerCalloutFunctions[0] = returnFirstPointer;
  intPointerCalloutFunctions[1] = returnSecondPointer;
}

TEARDOWN(AlienIntegerCallout2Tests){
  while(!handles->isEmpty())
    release(handles->pop());
  free(handles);
  handles = NULL;
  delete rm;
  rm = NULL;
}
TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldCallIntArgFunction) {
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPassed(arg, -1, intCalloutFunctions);
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldCallIntPointerArgFunctionWithArg2) {
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPtrPassed(arg, -1, intPointerCalloutFunctions);
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldCallFunctionAndIgnoreResultWhenResultAlienNil) {
  oop result = byteArrayPrimitives::alienCallResult2(smi0, smim1, nilObj, functionAlien->as_oop());
  ASSERT_TRUE_M(!result->is_mark(), "should not be marked");
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2WithScavengeShouldReturnCorrectResult) {
  setAddress(functionAlien, &forceScavenge2);
  checkIntResult("incorrect initialization", 0, resultAlien);
  byteArrayPrimitives::alienCallResult2(smi0, smi0, resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("result alien not updated", -1, resultAlien);
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldReturnMarkedResultForNonAlien) {
  oop result = byteArrayPrimitives::alienCallResult2(smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     smi0);

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldReturnMarkedResultForDirectAlien) {
  oop result = byteArrayPrimitives::alienCallResult2(smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     resultAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldReturnMarkedResultForNullFunctionPointer) {
  oop result = byteArrayPrimitives::alienCallResult2(smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     invalidFunctionAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldReturnMarkedResultWhenResultNotAlienOrNil) {
  oop result = byteArrayPrimitives::alienCallResult2(smi0, smi0, trueObj, functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldReturnMarkedResultWhenFunctionParameter1NotAlienOrSMI) {
  oop result = byteArrayPrimitives::alienCallResult2(smi0,
                                                     trueObj,
                                                     resultAlien->as_oop(),
                                                     functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::second_argument_has_wrong_type());
}

TESTF(AlienIntegerCallout2Tests, alienCallResult2ShouldReturnMarkedResultWhenFunctionParameter2NotAlienOrSMI) {
  oop result = byteArrayPrimitives::alienCallResult2(trueObj,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::third_argument_has_wrong_type());
}
