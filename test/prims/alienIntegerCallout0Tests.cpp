# include "incls/_precompiled.incl"
# include "incls/_byteArray_prims.cpp.incl"
#include "test.h"
//#include "delta.hpp"
#include "testUtils.hpp"
#include "testProcess.hpp"
#include <time.h>

using namespace easyunit;

DECLARE(AlienIntegerCallout0Tests)
  HeapResourceMark *rm;
  byteArrayOop fnAlien;
  byteArrayOop invalidFunctionAlien;
  byteArrayOop resultAlien, addressAlien, pointerAlien, argumentAlien;
  smiOop smi0, smi1;
  char address[8];

  byteArrayOop allocateAlien(int arraySize, int alienSize) {
    byteArrayOop alien = byteArrayOop(Universe::byteArrayKlassObj()->klass_part()->allocateObjectSize(arraySize));
    byteArrayPrimitives::alienSetSize(as_smiOop(alienSize), alien);
    return alien;
  }
  void checkMarkedSymbol(char* message, oop result, symbolOop expected) {
    char text[200];
    ASSERT_TRUE_M(result->is_mark(), "Should be marked");
    sprintf(text,"Should be: %s, was: %s", message, unmarkSymbol(result)->as_string());
    ASSERT_TRUE_M(unmarkSymbol(result) == expected, text);
  }
  void checkIntResult(char* message, int expected, int actual) {
    char text[200];
    sprintf(text,"Should be: %d, was: %d", expected, actual);
    ASSERT_TRUE_M(actual == expected, text);
  }
  int asInt(bool &ok, oop intOop) {
    if (intOop->is_smi())
      return smiOop(intOop)->value();
    if (!intOop->is_byteArray()) {
      ok = false;
      return 0;
    }
    return byteArrayOop(intOop)->number().as_int(ok);
  }
END_DECLARE

SETUP(AlienIntegerCallout0Tests) {
  rm = new HeapResourceMark();
  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);

  PersistentHandle ca(allocateAlien(8, 0));
  PersistentHandle ifa(allocateAlien(8, 0));
  PersistentHandle ra(allocateAlien(12, 8));
  PersistentHandle aa(allocateAlien(8, -8));
  PersistentHandle pa(allocateAlien(8, 0));

  resultAlien = byteArrayOop(ra.as_oop());
  invalidFunctionAlien = byteArrayOop(ifa.as_oop());
  byteArrayPrimitives::alienSetAddress(smi0, invalidFunctionAlien);

  fnAlien = byteArrayOop(ca.as_oop());
  byteArrayPrimitives::alienSetAddress(as_smiOop((int)&clock), fnAlien);

  addressAlien = byteArrayOop(aa.as_oop());
  byteArrayPrimitives::alienSetAddress(as_smiOop((int)&address), addressAlien);

  pointerAlien = byteArrayOop(pa.as_oop());
  byteArrayPrimitives::alienSetAddress(as_smiOop((int)&address), pointerAlien);
}

TEARDOWN(AlienIntegerCallout0Tests){
  delete rm;
  rm = NULL;
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldReturnResultAlien) {
  clock_t clockResult = clock();

  oop result = byteArrayPrimitives::alienCallResult0(resultAlien, fnAlien);
  ASSERT_TRUE_M(result == resultAlien, "should return result alien");
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldReturnMarkedResultForNonAlien) {
  oop result = byteArrayPrimitives::alienCallResult0(resultAlien, smi0);
 
  checkMarkedSymbol("wrong type", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldReturnMarkedResultForDirectAlien) {
  oop result = byteArrayPrimitives::alienCallResult0(resultAlien, resultAlien);
 
  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldReturnMarkedResultForNullFunctionPointer) {
  oop result = byteArrayPrimitives::alienCallResult0(resultAlien, invalidFunctionAlien);
 
  checkMarkedSymbol("illegal state", result, vmSymbols::illegal_state());
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldReturnMarkedResultWhenResultNotAlien) {
  oop result = byteArrayPrimitives::alienCallResult0(smi0, fnAlien);
 
  checkMarkedSymbol("wrong type", result, vmSymbols::argument_has_wrong_type());
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldCallClock) {
  clock_t clockResult = clock();
  byteArrayPrimitives::alienCallResult0(resultAlien, fnAlien);
 
  ASSERT_TRUE_M(sizeof(clock_t) == 4, "wrong size");
  oop alienClockResult = byteArrayPrimitives::alienUnsignedLongAt(smi1, resultAlien);
  ASSERT_TRUE_M(clockResult == smiOop(alienClockResult)->value(), "wrong result");
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldSetResultInPointerAlien) {
  clock_t clockResult = clock();
  byteArrayPrimitives::alienCallResult0(pointerAlien, fnAlien);
 
  oop alienClockResult = byteArrayPrimitives::alienUnsignedLongAt(smi1, pointerAlien);
  ASSERT_TRUE_M(clockResult == smiOop(alienClockResult)->value(), "wrong result");
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldSetResultInAddressAlien) {
  clock_t clockResult = clock();
  byteArrayPrimitives::alienCallResult0(addressAlien, fnAlien);
 
  oop alienClockResult = byteArrayPrimitives::alienUnsignedLongAt(smi1, addressAlien);
  ASSERT_TRUE_M(clockResult == smiOop(alienClockResult)->value(), "wrong result");
}

TESTF(AlienIntegerCallout0Tests, alienCallResult0ShouldIgnoreResultWhenResultArgZero) {
  oop result = byteArrayPrimitives::alienCallResult0(nilObj, fnAlien);
  ASSERT_TRUE_M(!result->is_mark(), "Should not be marked");
}
