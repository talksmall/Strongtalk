# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"

using namespace easyunit;

extern "C" int PRIM_API returnFirst6(int a, int b, int c, int d, int e, int f) {
  return a;
}

extern "C" int PRIM_API returnFirstPointer6(int *a, int b, int c, int d, int e, int f) {
  return *a;
}

extern "C" int PRIM_API returnSecond6(int a, int b, int c, int d, int e, int f) {
  return b;
}

extern "C" int PRIM_API returnSecondPointer6(int a, int *b, int c, int d, int e, int f) {
  return *b;
}

extern "C" int PRIM_API returnThird6(int a, int b, int c, int d, int e, int f) {
  return c;
}

extern "C" int PRIM_API returnThirdPointer6(int a, int b, int *c, int d, int e, int f) {
  return *c;
}

extern "C" int PRIM_API returnFourth6(int a, int b, int c, int d, int e, int f) {
  return d;
}

extern "C" int PRIM_API returnFourthPointer6(int a, int b, int c, int *d, int e, int f) {
  return *d;
}

extern "C" int PRIM_API returnFifth6(int a, int b, int c, int d, int e, int f) {
  return e;
}

extern "C" int PRIM_API returnFifthPointer6(int a, int b, int c, int d, int *e, int f) {
  return *e;
}

extern "C" int PRIM_API returnSixth6(int a, int b, int c, int d, int e, int f) {
  return f;
}

extern "C" int PRIM_API returnSixthPointer6(int a, int b, int c, int d, int e, int *f) {
  return *f;
}

extern "C" int PRIM_API forceScavenge6(int ignore1, int ignore2, int ignore3, int d, int e) {
  Universe::scavenge();
  return -1;
}

DECLARE(AlienIntegerCallout6Tests)
HeapResourceMark *rm;
GrowableArray<PersistentHandle**> *handles;
PersistentHandle *resultAlien, *addressAlien, *pointerAlien, *functionAlien; 
PersistentHandle *directAlien, *invalidFunctionAlien;
smiOop smi0, smi1, smim1;
static const int argCount = 6;
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
oop callout(oop arg[]) {
  return byteArrayPrimitives::alienCallResult6(arg[5], arg[4], arg[3], arg[2], arg[1], arg[0], resultAlien->as_oop(), functionAlien->as_oop());
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
    default:
      symbol = vmSymbols::argument_has_wrong_type();
  }
  checkMarkedSymbol("wrong type", result, symbol);
}
END_DECLARE

SETUP(AlienIntegerCallout6Tests) {
  rm = new HeapResourceMark();
  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);
  smim1 = as_smiOop(-1);
  handles = new(true) GrowableArray<PersistentHandle**>(6);

  allocateAlien(functionAlien,        8,  0, &returnFirst6);
  allocateAlien(resultAlien,         12,  8);
  allocateAlien(directAlien,         12,  4);
  allocateAlien(addressAlien,         8, -4, &address);
  allocateAlien(pointerAlien,         8,  0, &address);
  allocateAlien(invalidFunctionAlien, 8,  0);

  memset(address, 0, 8);

  intCalloutFunctions[0] = returnFirst6;
  intCalloutFunctions[1] = returnSecond6;
  intCalloutFunctions[2] = returnThird6;
  intCalloutFunctions[3] = returnFourth6;
  intCalloutFunctions[4] = returnFifth6;
  intCalloutFunctions[5] = returnSixth6;
  intPointerCalloutFunctions[0] = returnFirstPointer6;
  intPointerCalloutFunctions[1] = returnSecondPointer6;
  intPointerCalloutFunctions[2] = returnThirdPointer6;
  intPointerCalloutFunctions[3] = returnFourthPointer6;
  intPointerCalloutFunctions[4] = returnFifthPointer6;
  intPointerCalloutFunctions[5] = returnSixthPointer6;
}

TEARDOWN(AlienIntegerCallout6Tests){
  while(!handles->isEmpty())
    release(handles->pop());
  free(handles);
  handles = NULL;
  delete rm;
  rm = NULL;
}
TESTF(AlienIntegerCallout6Tests, alienCallResult6ShouldCallIntArgFunction) {
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPassed(arg, -1, intCalloutFunctions);
}

TESTF(AlienIntegerCallout6Tests, alienCallResult6ShouldCallIntPointerArgFunction) {
  byteArrayPrimitives::alienSignedLongAtPut(asOop(-1), smi1, pointerAlien->as_oop());
  for (int arg = 0; arg < argCount; arg++)
    checkArgnPtrPassed(arg, pointerAlien->as_oop(), intPointerCalloutFunctions);
}

TESTF(AlienIntegerCallout6Tests, alienCallResult6ShouldCallFunctionAndIgnoreResultWhenResultAlienNil) {
  oop result = byteArrayPrimitives::alienCallResult6(smi0, smi0, smi0, smi0, smi0, smim1, nilObj, functionAlien->as_oop());
  ASSERT_TRUE_M(!result->is_mark(), "should not be marked");
}

TESTF(AlienIntegerCallout6Tests, alienCallResult6WithScavengeShouldReturnCorrectResult) {
  setAddress(functionAlien, &forceScavenge6);
  checkIntResult("incorrect initialization", 0, resultAlien);
  byteArrayPrimitives::alienCallResult6(smi0, smi0, smi0, smi0, smi0, smi0, resultAlien->as_oop(), functionAlien->as_oop());
  checkIntResult("result alien not updated", -1, resultAlien);
}

TESTF(AlienIntegerCallout6Tests, alienCallResult6ShouldReturnMarkedResultForNonAlien) {
  oop result = byteArrayPrimitives::alienCallResult6(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     smi0);

  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(AlienIntegerCallout6Tests, alienCallResult6ShouldReturnMarkedResultForDirectAlien) {
  oop result = byteArrayPrimitives::alienCallResult6(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     resultAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout6Tests, alienCallResult6ShouldReturnMarkedResultForNullFunctionPointer) {
  oop result = byteArrayPrimitives::alienCallResult6(smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     smi0,
                                                     resultAlien->as_oop(),
                                                     invalidFunctionAlien->as_oop());

  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout6Tests, alienCallResult6ShouldReturnMarkedResultWhenResultNotAlienOrNil) {
  oop result = byteArrayPrimitives::alienCallResult6(smi0, smi0, smi0, smi0, smi0, smi0, trueObj, functionAlien->as_oop());

  checkMarkedSymbol("wrong type", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(AlienIntegerCallout6Tests, alienCallResult6ShouldReturnMarkedResultWhenFunctionParameterNotAlienOrSMI) {
  for (int arg = 0; arg < argCount; arg++)
    checkIllegalArgnPassed(arg, trueObj);
}
