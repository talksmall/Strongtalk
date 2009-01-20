# include "incls/_precompiled.incl"
# include "incls/_blockOop.cpp.incl"
# include "incls/_vmOperations.cpp.incl"
# include "incls/_delta.cpp.incl"
#include "handle.hpp"
#include "test.h"

using namespace easyunit;

DECLARE(ContextOptimizationTests)
  int count;
END_DECLARE

SETUP(ContextOptimizationTests) {
  count = 0;
}

TEARDOWN(ContextOptimizationTests){
  count = 0;
}

TESTF(ContextOptimizationTests, compileContentsDo) {
  HandleMark mark;
  Handle setClassHandle(Universe::find_global("ContextNestingTest"));
  Handle selectorHandle(oopFactory::new_symbol("testWith:"));
  Handle newHandle(oopFactory::new_symbol("new"));
  Handle testOnce(oopFactory::new_symbol("testOnce"));
  Handle testTwice(oopFactory::new_symbol("testTwice"));

  Delta::call(Delta::call(setClassHandle.as_klass(), newHandle.as_oop()), testOnce.as_oop());

  LookupKey key(setClassHandle.as_klass(), selectorHandle.as_oop());
  LookupResult result = interpreter_normal_lookup(setClassHandle.as_klass(), symbolOop(selectorHandle.as_oop()));

  ASSERT_TRUE(!result.is_empty());

  VM_OptimizeMethod op(&key, result.method());
  
  VMProcess::execute(&op);

  lookupCache::flush();
  Delta::call(Delta::call(setClassHandle.as_klass(), newHandle.as_oop()), testTwice.as_oop());
}