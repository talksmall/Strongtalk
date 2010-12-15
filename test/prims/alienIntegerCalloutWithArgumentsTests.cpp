# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"
#include <time.h>
#include <stdlib.h>

using namespace easyunit;

extern "C" int PRIM_API returnFirst2(int a, int b) {
  return a;
}

extern "C" int PRIM_API returnFirstPointer2(int *a, int b) {
  return *a;
}

extern "C" int PRIM_API returnSecond2(int a, int b) {
  return b;
}

extern "C" int PRIM_API returnSecondPointer2(int a, int *b) {
  return *b;
}

extern "C" int PRIM_API forceScavengeWA(int a, int b) {
  Universe::scavenge();
  return -1;
}

extern "C" int PRIM_API argAlignment2(int a, int b) {
  return ((int) &a) & 0xF;
}

DECLARE(AlienIntegerCalloutWithArgumentsTests)
HeapResourceMark *rm;
GrowableArray<PersistentHandle**> *handles;
PersistentHandle *resultAlien, *addressAlien, *pointerAlien, *functionAlien; 
PersistentHandle *directAlien, *invalidFunctionAlien;
smiOop smi0, smi1, smim1;
static const int argCount = 2;
void* intCalloutFunctions[argCount];
void* intPointerCalloutFunctions[argCount];
oop zeroes[argCount];
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
void setAddress(PersistentHandle* handle, void* argument) {
  byteArrayPrimitives::alienSetAddress(asOop((int)argument), handle->as_oop());
}
oop callout(oop arg[], oop result, oop address) {
  objArrayOop argOops = oopFactory::new_objArray(2);
  for (int index = 0; index < 2; index++)
    argOops->obj_at_put(index + 1, arg[index]);
  return byteArrayPrimitives::alienCallResultWithArguments(argOops, result, address);
}
oop callout(oop arg[]) {
  return callout(arg, resultAlien->as_oop(), functionAlien->as_oop());
}
void checkArgnPassed(int argIndex, int argValue, void**functionArray) {
  setAddress(functionAlien, functionArray[argIndex]);
  oop arg[argCount];
  for (int index = 0; index < argCount; index++)
    arg[index] = argIndex == index ? asOop(argValue) : smi0;
  oop result = callout(arg);

  ASSERT_TRUE_M(result == resultAlien->as_oop(), "Should return result alien");
  checkIntResult("wrong result", argValue, resultAlien);
}
void checkArgnPtrPassed(int argIndex, oop pointer, void**functionArray) {
  setAddress(functionAlien, functionArray[argIndex]);
  oop arg[argCount];
  for (int index = 0; index < argCount; index++)
    arg[index] = argIndex == index ? pointer : smi0;
  oop result = callout(arg);

  ASSERT_TRUE_M(result == resultAlien->as_oop(), "Should return result alien");
  checkIntResult("wrong result", -1, resultAlien);
}
void checkIllegalArgnPassed(int argIndex, oop pointer) {
  oop arg[argCount];
  for (int index = 0; index < argCount; index++)
    arg[index] = argIndex == index ? pointer : smi0;
  oop result = callout(arg);

  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}
END_DECLARE

SETUP(AlienIntegerCalloutWithArgumentsTests) {
  rm = new HeapResourceMark();
  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);
  smim1 = as_smiOop(-1);
  handles = new(true) GrowableArray<PersistentHandle**>(5);

  allocateAlien(functionAlien,        8,  0, &returnFirst2);
  allocateAlien(resultAlien,         12,  8);
  allocateAlien(directAlien,         12,  4);
  allocateAlien(addressAlien,         8, -4, &address);
  allocateAlien(pointerAlien,         8,  0, &address);
  allocateAlien(invalidFunctionAlien, 8,  0);

  memset(address, 0, 8);

  intCalloutFunctions[0] = returnFirst2;
  intCalloutFunctions[1] = returnSecond2;
  intPointerCalloutFunctions[0] = returnFirstPointer2;
  intPointerCalloutFunctions[1] = returnSecondPointer2;
}

TEARDOWN(AlienIntegerCalloutWithArgumentsTests){
  while(!handles->isEmpty())
    release(handles->pop());
  free(handles);
  handles = NULL;
  delete rm;
  rm = NULL;
}
TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsShouldCallIntArgFunction) {
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPassed(arg, -1, intCalloutFunctions);
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResult1Should16ByteAlignArgs) {
  oop fnAddress = asOop((int)&argAlignment2);
  byteArrayPrimitives::alienSetAddress(fnAddress, functionAlien->as_oop());

  oop arg[argCount];
  arg[1] = smi0;
  for (int size = -4; size > -20; size -= 4) {
    arg[0] = addressAlien->as_oop();
    byteArrayPrimitives::alienSetSize(as_smiOop(size), addressAlien->as_oop());
    callout(arg);
    checkIntResult("wrong result", 0, resultAlien);
  }
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsShouldCallIntPointerArgFunction) {
  byteArrayPrimitives::alienSignedLongAtPut(asOop(-1), smi1, pointerAlien->as_oop());
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPtrPassed(arg, pointerAlien->as_oop(), intPointerCalloutFunctions);
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsShouldCallFunctionAndIgnoreResultWhenResultAlienNil) {
  oop result = callout(zeroes, nilObj, functionAlien->as_oop());
  ASSERT_TRUE_M(!result->is_mark(), "should not be marked");
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsWithScavengeShouldReturnCorrectResult) {
  setAddress(functionAlien, &forceScavengeWA);
  checkIntResult("incorrect initialization", 0, resultAlien);
  oop result = callout(zeroes, resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("result alien not updated", -1, resultAlien);
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsShouldReturnMarkedResultForNonAlien) {
  oop result = callout(zeroes, resultAlien->as_oop(), smi0);

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsShouldReturnMarkedResultForDirectAlien) {
oop result = callout(zeroes,  resultAlien->as_oop(), resultAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsShouldReturnMarkedResultForNullFunctionPointer) {
  oop result = callout(zeroes, resultAlien->as_oop(), invalidFunctionAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsShouldReturnMarkedResultWhenResultNotAlienOrNil) {
  oop result = callout(zeroes, trueObj, functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(AlienIntegerCalloutWithArgumentsTests, alienCallResultWithArgumentsShouldReturnMarkedResultWhenFunctionParameterNotAlienOrSMI) {
  for (int arg = 0; arg < argCount; arg++)
    checkIllegalArgnPassed(arg, trueObj);
}
