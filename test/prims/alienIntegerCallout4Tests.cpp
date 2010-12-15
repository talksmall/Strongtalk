# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"
#include <time.h>
#include <stdlib.h>

using namespace easyunit;

extern "C" int PRIM_API returnFirst4(int a, int b, int c, int d) {
  return a;
}

extern "C" int PRIM_API returnFirstPointer4(int *a, int b, int c, int d) {
  return *a;
}

extern "C" int PRIM_API returnSecond4(int a, int b, int c, int d) {
  return b;
}

extern "C" int PRIM_API returnSecondPointer4(int a, int *b, int c, int d) {
  return *b;
}

extern "C" int PRIM_API returnThird4(int a, int b, int c, int d) {
  return c;
}

extern "C" int PRIM_API returnThirdPointer4(int a, int b, int *c, int d) {
  return *c;
}

extern "C" int PRIM_API returnFourth4(int a, int b, int c, int d) {
  return d;
}

extern "C" int PRIM_API returnFourthPointer4(int a, int b, int c, int *d) {
  return *d;
}

extern "C" int PRIM_API forceScavenge4(int ignore1, int ignore2, int ignore3, int d) {
  Universe::scavenge();
  return -1;
}

DECLARE(AlienIntegerCallout4Tests)
HeapResourceMark *rm;
GrowableArray<PersistentHandle**> *handles;
PersistentHandle *resultAlien, *addressAlien, *pointerAlien, *functionAlien; 
PersistentHandle *directAlien, *invalidFunctionAlien;
smiOop smi0, smi1, smim1;
static const int argCount = 4;
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
  sprintf(text,"%s. Should be: %s, was: %s", message, expected->as_string(), unmarkSymbol(result)->as_string());
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
  oop arg2 = argIndex == 2 ? asOop(argValue) : smi0;
  oop arg3 = argIndex == 3 ? asOop(argValue) : smi0;
  oop result = byteArrayPrimitives::alienCallResult4(arg3, arg2, arg1, arg0, resultAlien->as_oop(), functionAlien->as_oop());

  ASSERT_TRUE_M(result == resultAlien->as_oop(), "Should return result alien");
  checkIntResult("wrong result", argValue, resultAlien);
}
void checkArgnPtrPassed(int argIndex, oop pointer, void**functionArray) {
  setAddress(functionAlien, functionArray[argIndex]);
  oop arg0 = argIndex == 0 ? pointer : smi0;
  oop arg1 = argIndex == 1 ? pointer : smi0;
  oop arg2 = argIndex == 2 ? pointer : smi0;
  oop arg3 = argIndex == 3 ? pointer : smi0;
  oop result = byteArrayPrimitives::alienCallResult4(arg3, arg2, arg1, arg0, resultAlien->as_oop(), functionAlien->as_oop());

  ASSERT_TRUE_M(result == resultAlien->as_oop(), "Should return result alien");
  checkIntResult("wrong result", -1, resultAlien);
}
void checkIllegalArgnPassed(int argIndex, oop pointer) {
  oop arg0 = argIndex == 0 ? pointer : smi0;
  oop arg1 = argIndex == 1 ? pointer : smi0;
  oop arg2 = argIndex == 2 ? pointer : smi0;
  oop arg3 = argIndex == 3 ? pointer : smi0;
  oop result = byteArrayPrimitives::alienCallResult4(arg3, arg2, arg1, arg0, resultAlien->as_oop(), functionAlien->as_oop());

  symbolOop symbol;
  switch (argIndex) {
    case 0:
      symbol = vmSymbols::second_argument_has_wrong_type();
      break;
    case 1:
      symbol = vmSymbols::third_argument_has_wrong_type();
      break;
    case 2:
      symbol = vmSymbols::fourth_argument_has_wrong_type();
      break;
    case 3:
      symbol = vmSymbols::fifth_argument_has_wrong_type();
      break;
    default:
      symbol = vmSymbols::argument_has_wrong_type();
  }
  checkMarkedSymbol("wrong type", result, symbol);
}
END_DECLARE

SETUP(AlienIntegerCallout4Tests) {
  rm = new HeapResourceMark();
  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);
  smim1 = as_smiOop(-1);
  handles = new(true) GrowableArray<PersistentHandle**>(5);

  allocateAlien(functionAlien,        8,  0, &returnFirst4);
  allocateAlien(resultAlien,         12,  8);
  allocateAlien(directAlien,         12,  4);
  allocateAlien(addressAlien,         8, -4, &address);
  allocateAlien(pointerAlien,         8,  0, &address);
  allocateAlien(invalidFunctionAlien, 8,  0);

  memset(address, 0, 8);

  intCalloutFunctions[0] = returnFirst4;
  intCalloutFunctions[1] = returnSecond4;
  intCalloutFunctions[2] = returnThird4;
  intCalloutFunctions[3] = returnFourth4;
  intPointerCalloutFunctions[0] = returnFirstPointer4;
  intPointerCalloutFunctions[1] = returnSecondPointer4;
  intPointerCalloutFunctions[2] = returnThirdPointer4;
  intPointerCalloutFunctions[3] = returnFourthPointer4;
}

TEARDOWN(AlienIntegerCallout4Tests){
  while(!handles->isEmpty())
    release(handles->pop());
  free(handles);
  handles = NULL;
  delete rm;
  rm = NULL;
}
TESTF(AlienIntegerCallout4Tests, alienCallResult4ShouldCallIntArgFunction) {
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPassed(arg, -1, intCalloutFunctions);
}

TESTF(AlienIntegerCallout4Tests, alienCallResult4ShouldCallIntPointerArgFunction) {
  byteArrayPrimitives::alienSignedLongAtPut(asOop(-1), smi1, pointerAlien->as_oop());
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPtrPassed(arg, pointerAlien->as_oop(), intPointerCalloutFunctions);
}

TESTF(AlienIntegerCallout4Tests, alienCallResult4ShouldCallFunctionAndIgnoreResultWhenResultAlienNil) {
  oop result = byteArrayPrimitives::alienCallResult4(smi0, smi0, smi0, smim1, nilObj, functionAlien->as_oop());
  ASSERT_TRUE_M(!result->is_mark(), "should not be marked");
}

TESTF(AlienIntegerCallout4Tests, alienCallResult4WithScavengeShouldReturnCorrectResult) {
  setAddress(functionAlien, &forceScavenge4);
  checkIntResult("incorrect initialization", 0, resultAlien);
  byteArrayPrimitives::alienCallResult4(smi0, smi0, smi0, smi0, resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("result alien not updated", -1, resultAlien);
}

TESTF(AlienIntegerCallout4Tests, alienCallResult4ShouldReturnMarkedResultForNonAlien) {
  oop result = byteArrayPrimitives::alienCallResult4(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     smi0);

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(AlienIntegerCallout4Tests, alienCallResult4ShouldReturnMarkedResultForDirectAlien) {
  oop result = byteArrayPrimitives::alienCallResult4(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     resultAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout4Tests, alienCallResult4ShouldReturnMarkedResultForNullFunctionPointer) {
  oop result = byteArrayPrimitives::alienCallResult4(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     invalidFunctionAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout4Tests, alienCallResult4ShouldReturnMarkedResultWhenResultNotAlienOrNil) {
  oop result = byteArrayPrimitives::alienCallResult4(smi0, smi0, smi0, smi0, trueObj, functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(AlienIntegerCallout4Tests, alienCallResult4ShouldReturnMarkedResultWhenFunctionParameterNotAlienOrSMI) {
  for (int arg = 0; arg < argCount; arg++)
    checkIllegalArgnPassed(arg, trueObj);
}
