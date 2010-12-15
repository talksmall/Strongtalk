# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"
#include <time.h>
#include <stdlib.h>

using namespace easyunit;

extern "C" int PRIM_API returnFirst5(int a, int b, int c, int d, int e) {
  return a;
}

extern "C" int PRIM_API returnFirstPointer5(int *a, int b, int c, int d, int e) {
  return *a;
}

extern "C" int PRIM_API returnSecond5(int a, int b, int c, int d, int e) {
  return b;
}

extern "C" int PRIM_API returnSecondPointer5(int a, int *b, int c, int d, int e) {
  return *b;
}

extern "C" int PRIM_API returnThird5(int a, int b, int c, int d, int e) {
  return c;
}

extern "C" int PRIM_API returnThirdPointer5(int a, int b, int *c, int d, int e) {
  return *c;
}

extern "C" int PRIM_API returnFourth5(int a, int b, int c, int d, int e) {
  return d;
}

extern "C" int PRIM_API returnFourthPointer5(int a, int b, int c, int *d, int e) {
  return *d;
}

extern "C" int PRIM_API returnFifth5(int a, int b, int c, int d, int e) {
  return e;
}

extern "C" int PRIM_API returnFifthPointer5(int a, int b, int c, int d, int *e) {
  return *e;
}

extern "C" int PRIM_API forceScavenge5(int ignore1, int ignore2, int ignore3, int d, int e) {
  Universe::scavenge();
  return -1;
}

DECLARE(AlienIntegerCallout5Tests)
HeapResourceMark *rm;
GrowableArray<PersistentHandle**> *handles;
PersistentHandle *resultAlien, *addressAlien, *pointerAlien, *functionAlien; 
PersistentHandle *directAlien, *invalidFunctionAlien;
smiOop smi0, smi1, smim1;
static const int argCount = 5;
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
  oop arg[argCount];
  for (int index = 0; index < argCount; index++)
    arg[index] = argIndex == index ? asOop(argValue) : smi0;
  oop result = byteArrayPrimitives::alienCallResult5(arg[4], arg[3], arg[2], arg[1], arg[0], resultAlien->as_oop(), functionAlien->as_oop());

  ASSERT_TRUE_M(result == resultAlien->as_oop(), "Should return result alien");
  checkIntResult("wrong result", argValue, resultAlien);
}
void checkArgnPtrPassed(int argIndex, oop pointer, void**functionArray) {
  setAddress(functionAlien, functionArray[argIndex]);
  oop arg[argCount];
  for (int index = 0; index < argCount; index++)
    arg[index] = argIndex == index ? pointer : smi0;
  oop result = byteArrayPrimitives::alienCallResult5(arg[4], arg[3], arg[2], arg[1], arg[0], resultAlien->as_oop(), functionAlien->as_oop());

  ASSERT_TRUE_M(result == resultAlien->as_oop(), "Should return result alien");
  checkIntResult("wrong result", -1, resultAlien);
}
void checkIllegalArgnPassed(int argIndex, oop pointer) {
  oop arg[argCount];
  for (int index = 0; index < argCount; index++)
    arg[index] = argIndex == index ? pointer : smi0;
  oop result = byteArrayPrimitives::alienCallResult5(arg[4], arg[3], arg[2], arg[1], arg[0], resultAlien->as_oop(), functionAlien->as_oop());

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
    case 4:
      symbol = vmSymbols::sixth_argument_has_wrong_type();
      break;
    default:
      symbol = vmSymbols::argument_has_wrong_type();
  }
  checkMarkedSymbol("wrong type", result, symbol);
}
END_DECLARE

SETUP(AlienIntegerCallout5Tests) {
  rm = new HeapResourceMark();
  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);
  smim1 = as_smiOop(-1);
  handles = new(true) GrowableArray<PersistentHandle**>(5);

  allocateAlien(functionAlien,        8,  0, &returnFirst5);
  allocateAlien(resultAlien,         12,  8);
  allocateAlien(directAlien,         12,  4);
  allocateAlien(addressAlien,         8, -4, &address);
  allocateAlien(pointerAlien,         8,  0, &address);
  allocateAlien(invalidFunctionAlien, 8,  0);

  memset(address, 0, 8);

  intCalloutFunctions[0] = returnFirst5;
  intCalloutFunctions[1] = returnSecond5;
  intCalloutFunctions[2] = returnThird5;
  intCalloutFunctions[3] = returnFourth5;
  intCalloutFunctions[4] = returnFifth5;
  intPointerCalloutFunctions[0] = returnFirstPointer5;
  intPointerCalloutFunctions[1] = returnSecondPointer5;
  intPointerCalloutFunctions[2] = returnThirdPointer5;
  intPointerCalloutFunctions[3] = returnFourthPointer5;
  intPointerCalloutFunctions[4] = returnFifthPointer5;
}

TEARDOWN(AlienIntegerCallout5Tests){
  while(!handles->isEmpty())
    release(handles->pop());
  free(handles);
  handles = NULL;
  delete rm;
  rm = NULL;
}
TESTF(AlienIntegerCallout5Tests, alienCallResult5ShouldCallIntArgFunction) {
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPassed(arg, -1, intCalloutFunctions);
}

TESTF(AlienIntegerCallout5Tests, alienCallResult5ShouldCallIntPointerArgFunction) {
  byteArrayPrimitives::alienSignedLongAtPut(asOop(-1), smi1, pointerAlien->as_oop());
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPtrPassed(arg, pointerAlien->as_oop(), intPointerCalloutFunctions);
}

TESTF(AlienIntegerCallout5Tests, alienCallResult5ShouldCallFunctionAndIgnoreResultWhenResultAlienNil) {
  oop result = byteArrayPrimitives::alienCallResult5(smi0, smi0, smi0, smi0, smim1, nilObj, functionAlien->as_oop());
  ASSERT_TRUE_M(!result->is_mark(), "should not be marked");
}

TESTF(AlienIntegerCallout5Tests, alienCallResult5WithScavengeShouldReturnCorrectResult) {
  setAddress(functionAlien, &forceScavenge5);
  checkIntResult("incorrect initialization", 0, resultAlien);
  byteArrayPrimitives::alienCallResult5(smi0, smi0, smi0, smi0, smi0, resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("result alien not updated", -1, resultAlien);
}

TESTF(AlienIntegerCallout5Tests, alienCallResult5ShouldReturnMarkedResultForNonAlien) {
  oop result = byteArrayPrimitives::alienCallResult5(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     smi0);

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(AlienIntegerCallout5Tests, alienCallResult5ShouldReturnMarkedResultForDirectAlien) {
  oop result = byteArrayPrimitives::alienCallResult5(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     resultAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout5Tests, alienCallResult5ShouldReturnMarkedResultForNullFunctionPointer) {
  oop result = byteArrayPrimitives::alienCallResult5(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     invalidFunctionAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout5Tests, alienCallResult5ShouldReturnMarkedResultWhenResultNotAlienOrNil) {
  oop result = byteArrayPrimitives::alienCallResult5(smi0, smi0, smi0, smi0, smi0, trueObj, functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(AlienIntegerCallout5Tests, alienCallResult5ShouldReturnMarkedResultWhenFunctionParameterNotAlienOrSMI) {
  for (int arg = 0; arg < argCount; arg++)
    checkIllegalArgnPassed(arg, trueObj);
}
