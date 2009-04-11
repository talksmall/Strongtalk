# include "incls/_precompiled.incl"
# include "incls/_blockOop.cpp.incl"
# include "incls/_vmOperations.cpp.incl"
# include "incls/_delta.cpp.incl"
# include "recompile.hpp"
#include "handle.hpp"
#include "test.h"
#include "testProcess.hpp"

using namespace easyunit;

class BlockMethodFinder : public SpecializedMethodClosure {
public:
  methodOop method;
  BlockMethodFinder() {
    method = NULL;
  }
  void allocate_closure(AllocationType type, int nofArgs, methodOop meth) {
    method = meth;
  }
};

DECLARE(CompilerTests)
  int count;
  void initializeSmalltalkEnvironment() {
    HandleMark mark;
    Handle _new(oopFactory::new_symbol("new"));
    Handle initialize(oopFactory::new_symbol("initialize"));
    Handle processorScheduler(Universe::find_global("ProcessorScheduler"));
    Handle run(oopFactory::new_symbol("run"));
    Handle systemInitializer(Universe::find_global("SystemInitializer"));
    
    Handle processor(Delta::call(processorScheduler.as_oop(), _new.as_oop()));
    associationOop processorAssoc = Universe::find_global_association("Processor");
    processorAssoc->set_value(processor.as_oop());

    Delta::call(processor.as_oop(), initialize.as_oop());
    Delta::call(systemInitializer.as_oop(), run.as_oop());
  }
END_DECLARE

SETUP(CompilerTests) {
  count = 0;
}

TEARDOWN(CompilerTests){
  count = 0;
}

TESTF(CompilerTests, compileContentsDo) {
  HandleMark mark;
  Handle setClassHandle(Universe::find_global("ContextNestingTest"));
  Handle selectorHandle(oopFactory::new_symbol("testWith:"));
  Handle newHandle(oopFactory::new_symbol("new"));
  Handle testOnce(oopFactory::new_symbol("testOnce"));
  Handle testTwice(oopFactory::new_symbol("testTwice"));

  Delta::call(Delta::call(setClassHandle.as_klass(), newHandle.as_oop()), testOnce.as_oop());

  LookupResult result = interpreter_normal_lookup(setClassHandle.as_klass(), symbolOop(selectorHandle.as_oop()));

  LookupKey key(setClassHandle.as_klass(), selectorHandle.as_oop());
  ASSERT_TRUE(!result.is_empty());
  VM_OptimizeMethod op(&key, result.method());
  VMProcess::execute(&op);

  DeltaCallCache::clearAll();
  lookupCache::flush();

  Delta::call(Delta::call(setClassHandle.as_klass(), newHandle.as_oop()), testTwice.as_oop());
}

TESTF(CompilerTests, uncommonTrap) {
  AddTestProcess addTest;
  {
    HandleMark mark;
    initializeSmalltalkEnvironment();

    Handle _new(oopFactory::new_symbol("new"));
    Handle setup(oopFactory::new_symbol("populatePIC"));
    Handle toCompile(oopFactory::new_symbol("type"));
    Handle triggerTrap(oopFactory::new_symbol("testTriggerUncommonTrap"));
    Handle testClass(Universe::find_global("DeltaParameterTest"));
    Handle varClass(Universe::find_global("DeltaParameter"));

    Handle newTest(Delta::call(testClass.as_klass(), _new.as_oop()));

    Delta::call(newTest.as_oop(), setup.as_oop());

    LookupResult result = interpreter_normal_lookup(varClass.as_klass(), symbolOop(toCompile.as_oop()));

    LookupKey key(varClass.as_klass(), toCompile.as_oop());
    ASSERT_TRUE(!result.is_empty());
    VM_OptimizeMethod op(&key, result.method());
    VMProcess::execute(&op);

    DeltaCallCache::clearAll();
    lookupCache::flush();

    int trapCount = op.result()->uncommon_trap_counter();
    Delta::call(newTest.as_oop(), triggerTrap.as_oop());
    ASSERT_EQUALS(trapCount + 1, op.result()->uncommon_trap_counter());
  }
}
TESTF(CompilerTests, invalidJumptableID) {
  AddTestProcess addTest;
  {
    HandleMark mark;
    initializeSmalltalkEnvironment();

    Handle _new(oopFactory::new_symbol("new"));
    Handle setup(oopFactory::new_symbol("testIgnoredBlock"));
    Handle toCompile(oopFactory::new_symbol("testIgnoredBlock"));
    Handle triggerTrap(oopFactory::new_symbol("testIgnoredBlock"));
    Handle testClass(Universe::find_global("BlockMaterializeTest"));
    Handle varClass(Universe::find_global("BlockMaterializeTest"));

    Handle newTest(Delta::call(testClass.as_klass(), _new.as_oop()));

    Delta::call(newTest.as_oop(), toCompile.as_oop());

    LookupResult result = interpreter_normal_lookup(testClass.as_klass(), symbolOop(toCompile.as_oop()));

    LookupKey key(testClass.as_klass(), toCompile.as_oop());
    ASSERT_TRUE(!result.is_empty());

    VM_OptimizeMethod op(&key, result.method());
    VMProcess::execute(&op);
      // was causing assertion failure in CompileTimeClosure::jump_table_entry()
      // due to no _id
  }
}

TESTF(CompilerTests, toplevelBlockScopeOuterContextFilledWithNils) {
  AddTestProcess addTest;
  {
    HandleMark mark;
    initializeSmalltalkEnvironment();

    Handle _new(oopFactory::new_symbol("new"));

    Handle setup(oopFactory::new_symbol("testSetup"));
    Handle toCompile(oopFactory::new_symbol("exercise:value:"));
    Handle trigger(oopFactory::new_symbol("testTrap"));
    Handle testClass(Universe::find_global("NonInlinedBlockTest"));
    Handle varClass(Universe::find_global("NonInlinedBlockTest"));

    Handle newTest(Delta::call(testClass.as_klass(), _new.as_oop()));

    Delta::call(newTest.as_oop(), setup.as_oop());

    LookupResult result = interpreter_normal_lookup(varClass.as_klass(), symbolOop(toCompile.as_oop()));

    LookupKey key(varClass.as_klass(), toCompile.as_oop());
    ASSERT_TRUE(!result.is_empty());

    VM_OptimizeMethod op(&key, result.method());
    VMProcess::execute(&op);

    DeltaCallCache::clearAll();
    lookupCache::flush();
    LookupResult setupResult = interpreter_normal_lookup(varClass.as_klass(), symbolOop(setup.as_oop()));
    setupResult.method()->cleanup_inline_caches();

    Delta::call(newTest.as_oop(), setup.as_oop());

    // forces deoptimization, resulting in construction of canonical_context where the
    // not all of the vframes are contained within a stack frame. Was causing MNU due to
    // nils in the outer contexts, since the code was not passing the outer vframe when
    // crossing stack boundaries.
    Delta::call(newTest.as_oop(), trigger.as_oop());
  }
}