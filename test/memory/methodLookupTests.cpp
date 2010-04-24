# include "incls/_precompiled.incl"
# include "incls/_klassOop.cpp.incl"

#include "test.h"

using namespace easyunit;

DECLARE(MethodLookupTests)
  PersistentHandle* fixture;
END_DECLARE

SETUP(MethodLookupTests) {
  fixture = new PersistentHandle(Universe::find_global("DoesNotUnderstandFixture"));
}

TEARDOWN(MethodLookupTests) {
  delete fixture;
}

TESTF(MethodLookupTests, lookupShouldAddDNUInvokerWhenNoMatch) {
  BlockScavenge bs;
  symbolOop selector = oopFactory::new_symbol("unknownSelector");
  methodOop method = fixture->as_klassOop()->klass_part()->lookup(selector);
  ASSERT_TRUE_M(method != NULL, "Should find method");
  ASSERT_TRUE_M(selector == method->selector(), "Wrong selector");
  method->pretty_print();
}

TESTF(MethodLookupTests, lookupShouldAddDNUInvokerForOneArgSelector) {
  BlockScavenge bs;
  symbolOop selector = oopFactory::new_symbol("unknownSelector:");
  methodOop method = fixture->as_klassOop()->klass_part()->lookup(selector);
  ASSERT_TRUE_M(method != NULL, "Should find method");
  ASSERT_TRUE_M(selector == method->selector(), "Wrong selector");
  method->pretty_print();
}
