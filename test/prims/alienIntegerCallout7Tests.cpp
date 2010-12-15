# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"

using namespace easyunit;

extern "C" int PRIM_API sum7(int a, int b, int c, int d, int e, int f, int g) {
  return (a) + (b) + (c) + (d) + (e) + (f) + (g);
}

extern "C" int PRIM_API returnFirst7(int a, int b, int c, int d, int e, int f, int g) {
  return a;
}

extern "C" int PRIM_API returnFirstPointer7(int *a, int b, int c, int d, int e, int f, int g) {
  return *a;
}

extern "C" int PRIM_API returnSecond7(int a, int b, int c, int d, int e, int f, int g) {
  return b;
}

extern "C" int PRIM_API returnSecondPointer7(int a, int *b, int c, int d, int e, int f, int g) {
  return *b;
}

extern "C" int PRIM_API returnThird7(int a, int b, int c, int d, int e, int f, int g) {
  return c;
}

extern "C" int PRIM_API returnThirdPointer7(int a, int b, int *c, int d, int e, int f, int g) {
  return *c;
}

extern "C" int PRIM_API returnFourth7(int a, int b, int c, int d, int e, int f, int g) {
  return d;
}

extern "C" int PRIM_API returnFourthPointer7(int a, int b, int c, int *d, int e, int f, int g) {
  return *d;
}

extern "C" int PRIM_API returnFifth7(int a, int b, int c, int d, int e, int f, int g) {
  return e;
}

extern "C" int PRIM_API returnFifthPointer7(int a, int b, int c, int d, int *e, int f, int g) {
  return *e;
}

extern "C" int PRIM_API returnSixth7(int a, int b, int c, int d, int e, int f, int g) {
  return f;
}

extern "C" int PRIM_API returnSixthPointer7(int a, int b, int c, int d, int e, int *f) {
  return *f;
}

extern "C" int PRIM_API returnSeventh7(int a, int b, int c, int d, int e, int f, int g) {
  return g;
}

extern "C" int PRIM_API returnSeventhPointer7(int a, int b, int c, int d, int e, int f, int *g) {
  return *g;
}

extern "C" int PRIM_API forceScavenge7(int ignore1, int ignore2, int ignore3, int d, int e) {
  Universe::scavenge();
  return -1;
}

DECLARE(AlienIntegerCallout7Tests)
HeapResourceMark *rm;
GrowableArray<PersistentHandle**> *handles;
PersistentHandle *resultAlien, *addressAlien, *pointerAlien, *functionAlien; 
PersistentHandle *directAlien, *invalidFunctionAlien;
smiOop smi0, smi1, smim1;
static const int argCount = 7;
void* intCalloutFunctions[argCount];
void* intPointerCalloutFunctions[argCount];
oop zeroes[argCount];
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
oop callout(oop arg[], oop result, oop address) {
  return byteArrayPrimitives::alienCallResult7(arg[6], arg[5], arg[4], arg[3], arg[2], arg[1], arg[0], result, address);
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
    case 5:
      symbol = vmSymbols::seventh_argument_has_wrong_type();
      break;
    case 6:
      symbol = vmSymbols::eighth_argument_has_wrong_type();
      break;
    default:
      symbol = vmSymbols::argument_has_wrong_type();
  }
  checkMarkedSymbol("wrong type", result, symbol);
}
END_DECLARE

SETUP(AlienIntegerCallout7Tests) {
  rm = new HeapResourceMark();
  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);
  smim1 = as_smiOop(-1);
  handles = new(true) GrowableArray<PersistentHandle**>(7);

  allocateAlien(functionAlien,        8,  0, &returnFirst7);
  allocateAlien(resultAlien,         12,  8);
  allocateAlien(directAlien,         12,  4);
  allocateAlien(addressAlien,         8, -4, &address);
  allocateAlien(pointerAlien,         8,  0, &address);
  allocateAlien(invalidFunctionAlien, 8,  0);

  memset(address, 0, 8);

  intCalloutFunctions[0] = returnFirst7;
  intCalloutFunctions[1] = returnSecond7;
  intCalloutFunctions[2] = returnThird7;
  intCalloutFunctions[3] = returnFourth7;
  intCalloutFunctions[4] = returnFifth7;
  intCalloutFunctions[5] = returnSixth7;
  intCalloutFunctions[6] = returnSeventh7;

  intPointerCalloutFunctions[0] = returnFirstPointer7;
  intPointerCalloutFunctions[1] = returnSecondPointer7;
  intPointerCalloutFunctions[2] = returnThirdPointer7;
  intPointerCalloutFunctions[3] = returnFourthPointer7;
  intPointerCalloutFunctions[4] = returnFifthPointer7;
  intPointerCalloutFunctions[5] = returnSixthPointer7;
  intPointerCalloutFunctions[6] = returnSeventhPointer7;

  for (int index = 0; index < argCount; index++)
    zeroes[index] = smi0;
}

TEARDOWN(AlienIntegerCallout7Tests){
  while(!handles->isEmpty())
    release(handles->pop());
  free(handles);
  handles = NULL;
  delete rm;
  rm = NULL;
}
TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldCallIntArgFunction) {
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPassed(arg, -1, intCalloutFunctions);
}
TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldCallSumFunction) {
  oop arg[argCount];
  byteArrayPrimitives::alienSignedLongAtPut(asOop(-1), smi1, addressAlien->as_oop());
  for (int index = 0; index < argCount; index++)
    arg[index] = addressAlien->as_oop();
  byteArrayPrimitives::alienSetAddress(asOop((int)&sum7), functionAlien->as_oop());
  callout(arg);
  checkIntResult("wrong result", -1 * argCount, resultAlien);
}

TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldCallIntPointerArgFunction) {
  byteArrayPrimitives::alienSignedLongAtPut(asOop(-1), smi1, pointerAlien->as_oop());
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPtrPassed(arg, pointerAlien->as_oop(), intPointerCalloutFunctions);
}

TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldCallFunctionAndIgnoreResultWhenResultAlienNil) {
  oop result = callout(zeroes, nilObj, functionAlien->as_oop());
  ASSERT_TRUE_M(!result->is_mark(), "should not be marked");
}

TESTF(AlienIntegerCallout7Tests, alienCallResult7WithScavengeShouldReturnCorrectResult) {
  setAddress(functionAlien, &forceScavenge7);
  checkIntResult("incorrect initialization", 0, resultAlien);
  oop result = callout(zeroes, resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("result alien not updated", -1, resultAlien);
}

TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldReturnMarkedResultForNonAlien) {
  oop result = callout(zeroes, resultAlien->as_oop(), smi0);

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldReturnMarkedResultForDirectAlien) {
oop result = callout(zeroes,  resultAlien->as_oop(), resultAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldReturnMarkedResultForNullFunctionPointer) {
  oop result = callout(zeroes, resultAlien->as_oop(), invalidFunctionAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldReturnMarkedResultWhenResultNotAlienOrNil) {
  oop result = callout(zeroes, trueObj, functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(AlienIntegerCallout7Tests, alienCallResult7ShouldReturnMarkedResultWhenFunctionParameterNotAlienOrSMI) {
  for (int arg = 0; arg < argCount; arg++)
    checkIllegalArgnPassed(arg, trueObj);
}
