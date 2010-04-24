# include "incls/_precompiled.incl"
# include "incls/_proxy_prims.cpp.incl"
#include "test.h"
//#include "handle.hpp"
#include "testUtils.hpp"

using namespace easyunit;

extern "C" int expansion_count;
DECLARE(ProxyPrimsTests)
  HeapResourceMark *rm;
  proxyOop proxy, subProxy, validProxy;
  doubleOop doubleValue;
  smiOop smi0, smi1;
  int address;

void checkMarkedSymbol(char* message, oop result, symbolOop expected) {
    char text[200];
    ASSERT_TRUE_M(result->is_mark(), "Should be marked");
    sprintf(text,"Should be: %s, was: %s", message, unmarkSymbol(result)->as_string());
    ASSERT_TRUE_M(unmarkSymbol(result) == expected, text);
  }
END_DECLARE

SETUP(ProxyPrimsTests) {
  rm = new HeapResourceMark();
  PersistentHandle proxyClass(Universe::find_global("ExternalProxy"));
  PersistentHandle proxyHandle(proxyClass.as_klassOop()->klass_part()->allocateObject());
  PersistentHandle subProxyHandle(proxyClass.as_klassOop()->klass_part()->allocateObject());
  PersistentHandle validProxyHandle(proxyClass.as_klassOop()->klass_part()->allocateObject());
  
  doubleValue = oopFactory::new_double(1.2345);

  smi0 = as_smiOop(0);
  smi1 = as_smiOop(1);
  address = 0;

  proxy = proxyOop(proxyHandle.as_oop());
  subProxy = proxyOop(subProxyHandle.as_oop());
  validProxy = proxyOop(validProxyHandle.as_oop());
  validProxy->set_pointer(&address);
}

TEARDOWN(ProxyPrimsTests){
  delete rm;
  rm = NULL;
}

TESTF(ProxyPrimsTests, smiAtPutShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::smiAtPut(smi1, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, smiAtShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::smiAt(smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, byteAtPutShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::byteAtPut(smi1, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, byteAtShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::byteAt(smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, doubleByteAtPutShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::doubleByteAtPut(smi1, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, doubleByteAtShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::doubleByteAt(smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, subProxyAtShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::subProxyAt(subProxy, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
  ASSERT_TRUE_M(subProxy->is_null(), "subproxy should have null pointer");
}

TESTF(ProxyPrimsTests, proxyAtShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::proxyAt(subProxy, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
  ASSERT_TRUE_M(subProxy->is_null(), "subproxy should have null pointer");
}

TESTF(ProxyPrimsTests, proxyAtPutShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::proxyAtPut(subProxy, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
  ASSERT_TRUE_M(subProxy->is_null(), "subproxy should have null pointer");
}

TESTF(ProxyPrimsTests, proxyAtPutShouldFailWhenValuePointerIsNULL) {
  int addr;
  proxy->set_pointer(&addr);
  addr = 1;
  oop result = proxyOopPrimitives::proxyAtPut(subProxy, smi0, proxy);
  ASSERT_EQUALS_M(0, addr, "Should overwrite value");
}

TESTF(ProxyPrimsTests, singlePrecisionFloatAtShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::singlePrecisionFloatAt(smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, singlePrecisionFloatAtPutShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::singlePrecisionFloatAtPut(doubleValue, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, doublePrecisionFloatAtShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::doublePrecisionFloatAt(smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, doublePrecisionFloatAtPutShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::doublePrecisionFloatAtPut(doubleValue, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, callout0ShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::callOut0(subProxy, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, callout1ShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::callOut1(subProxy, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, callout2ShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::callOut2(subProxy, smi0, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, callout3ShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::callOut3(subProxy, smi0, smi0, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, callout4ShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::callOut4(subProxy, smi0, smi0, smi0, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, callout5ShouldFailWhenPointerIsNULL) {
  oop result = proxyOopPrimitives::callOut5(subProxy, smi0, smi0, smi0, smi0, smi0, proxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::illegal_state());
}

TESTF(ProxyPrimsTests, callout5ShouldFailWhenReceiverNotProxy) {
  oop result = proxyOopPrimitives::callOut5(subProxy, smi0, smi0, smi0, smi0, smi0, smi0);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ProxyPrimsTests, callout4ShouldFailWhenReceiverNotProxy) {
  oop result = proxyOopPrimitives::callOut4(subProxy, smi0, smi0, smi0, smi0, smi0);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ProxyPrimsTests, callout3ShouldFailWhenReceiverNotProxy) {
  oop result = proxyOopPrimitives::callOut3(subProxy, smi0, smi0, smi0, smi0);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ProxyPrimsTests, callout2ShouldFailWhenReceiverNotProxy) {
  oop result = proxyOopPrimitives::callOut2(subProxy, smi0, smi0, smi0);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ProxyPrimsTests, callout1ShouldFailWhenReceiverNotProxy) {
  oop result = proxyOopPrimitives::callOut1(subProxy, smi0, smi0);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ProxyPrimsTests, callout0ShouldFailWhenReceiverNotProxy) {
  oop result = proxyOopPrimitives::callOut0(subProxy, smi0);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(ProxyPrimsTests, callout3ShouldFailWhenResultNotProxy) {
  oop result = proxyOopPrimitives::callOut3(smi0, smi0, smi0, smi0, validProxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::fourth_argument_has_wrong_type());
}

TESTF(ProxyPrimsTests, callout4ShouldFailWhenResultNotProxy) {
  oop result = proxyOopPrimitives::callOut4(smi0, smi0, smi0, smi0, smi0, validProxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::fifth_argument_has_wrong_type());
}

TESTF(ProxyPrimsTests, callout5ShouldFailWhenResultNotProxy) {
  oop result = proxyOopPrimitives::callOut5(smi0, smi0, smi0, smi0, smi0, smi0, validProxy);
  checkMarkedSymbol("receiver invalid", result, vmSymbols::sixth_argument_has_wrong_type());
}
