# include "incls/_precompiled.incl"
# include "incls/_blockOop.cpp.incl"
# include "incls/_vmOperations.cpp.incl"
# include "incls/_delta.cpp.incl"
//# include "recompile.hpp"
//#include "handle.hpp"
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
  HeapResourceMark *rm;
  int count;
  nmethod* seed;
  nmethod* alloc_nmethod(LookupKey* key, int size) {
    Heap* heap = Universe::code->methodHeap;
    nmethod* nm = NULL;
    nm = (nmethod*) heap->allocate(size);
    if (!nm) return NULL;
    *((void**)nm) = *((void**)seed); // ugly hack to copy the vftable
    nm->initForTesting(size - sizeof nmethod, key);
    nm->makeZombie(false);
    return nm;
  }
  void initializeSmalltalkEnvironment() {
    //HandleMark mark;
    //Handle _new(oopFactory::new_symbol("new"));
    //Handle initialize(oopFactory::new_symbol("initialize"));
    //Handle processorScheduler(Universe::find_global("ProcessorScheduler"));
    //Handle run(oopFactory::new_symbol("run"));
    //Handle systemInitializer(Universe::find_global("SystemInitializer"));
    //
    //Handle processor(Delta::call(processorScheduler.as_oop(), _new.as_oop()));
    //associationOop processorAssoc = Universe::find_global_association("Processor");
    //processorAssoc->set_value(processor.as_oop());

    //Delta::call(processor.as_oop(), initialize.as_oop());
    //Delta::call(systemInitializer.as_oop(), run.as_oop());
  }
  void exhaustMethodHeap(LookupKey& key, int requiredSize) {
    GrowableArray<nmethod*>* nmethods = new GrowableArray<nmethod*>;
    int blockSize = Universe::code->methodHeap->blockSize;
    int size = Universe::code->methodHeap->freeBytes();

    bool hasFailed = false;
    while(!hasFailed) {
      nmethod* newnm = alloc_nmethod(&key, size);
      if (newnm) {
        nmethods->append(newnm);
      } else {
        if (size == requiredSize) hasFailed = true;
        size /= 2;
        if (size < requiredSize) size = requiredSize;
      }
    }
  }
  nmethod* compile(char* className, char* selectorName) {
    HandleMark mark;
    Handle toCompile(oopFactory::new_symbol(selectorName));
    Handle varClass(Universe::find_global(className));
    return compile(varClass,  toCompile);
  }
  nmethod* compile(Handle& klassHandle, Handle& selectorHandle) {
    klassOop  klass    = klassHandle.as_klass();
    symbolOop selector = symbolOop(selectorHandle.as_oop());

    LookupResult result = interpreter_normal_lookup(klass, selector);
    LookupKey key(klass, selector);

    VM_OptimizeMethod op(&key, result.method());
    VMProcess::execute(&op);
    DeltaCallCache::clearAll();
    lookupCache::flush();

    return op.result();
  }
  void clearICs(char* className, char* selectorName) {
    HandleMark mark;
    Handle varClass(Universe::find_global(className));
    Handle setup(oopFactory::new_symbol(selectorName));
    clearICs(varClass, setup);
  }
  void clearICs(Handle& klassHandle, Handle& selectorHandle) {
    klassOop  klass    = klassHandle.as_klass();
    symbolOop selector = symbolOop(selectorHandle.as_oop());
    LookupResult result = interpreter_normal_lookup(klass, selector);
    
    result.method()->cleanup_inline_caches();
  }
  nmethod* lookup(char* className, char* selectorName) {
    HandleMark mark;
    Handle classHandle(Universe::find_global(className));
    Handle selectorHandle(oopFactory::new_symbol(selectorName));

    klassOop  klass    = classHandle.as_klass();
    symbolOop selector = symbolOop(selectorHandle.as_oop());

    LookupKey key(klass, selector);
    return Universe::code->lookup(&key);
  }
  void call(char* className, char* selectorName) {
    HandleMark mark;
    Handle _new(oopFactory::new_symbol("new"));
    Handle setup(oopFactory::new_symbol(selectorName));
    Handle testClass(Universe::find_global(className));

    Handle newTest(Delta::call(testClass.as_klass(), _new.as_oop()));
    
    Delta::call(newTest.as_oop(), setup.as_oop());
  }
  static void resetInvocationCounter(methodOop method) {
    method->set_invocation_count(0);
  }
END_DECLARE

SETUP(CompilerTests) {
  rm = new HeapResourceMark();
  count = 0;
}

TEARDOWN(CompilerTests){
  count = 0;
  Universe::code->methodHeap->combineMode = true;
  Universe::code->flush(); // free all nmethods
  Universe::code->compact();
  Universe::methodOops_do(&CompilerTestsDeclareTest::resetInvocationCounter);
  delete rm;
  rm = NULL;
}

TESTF(CompilerTests, compileContentsDo) {
  call("ContextNestingTest", "testOnce");
  compile("ContextNestingTest", "testWith:");
  call("ContextNestingTest", "testTwice");
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

    call("DeltaParameterTest", "populatePIC");

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
    initializeSmalltalkEnvironment();
    call("BlockMaterializeTest", "testIgnoredBlock");
    compile("BlockMaterializeTest", "testIgnoredBlock");
      // was causing assertion failure in CompileTimeClosure::jump_table_entry()
      // due to no _id
  }
}

TESTF(CompilerTests, toplevelBlockScopeOuterContextFilledWithNils) {
  AddTestProcess addTest;
  {
    initializeSmalltalkEnvironment();
    call("NonInlinedBlockTest", "testSetup");
    compile("NonInlinedBlockTest", "exercise:value:");
    clearICs("NonInlinedBlockTest", "testSetup");
    call("NonInlinedBlockTest", "testSetup");

    // forces deoptimization, resulting in construction of canonical_context where the
    // not all of the vframes are contained within a stack frame. Was causing MNU due to
    // nils in the outer contexts, since the code was not passing the outer vframe when
    // crossing stack boundaries.
    call("NonInlinedBlockTest", "testTrap");
  }
}

TESTF(CompilerTests, toplevelBlockScopeWithContextContainingBlockReferencingContext) {
  AddTestProcess addTest;
  {
    initializeSmalltalkEnvironment();
    call("NonInlinedBlockTest", "testSetup2");
    compile("NonInlinedBlockTest",  "exercise2:value:");
    clearICs("NonInlinedBlockTest", "testSetup2");
    call("NonInlinedBlockTest", "testSetup2");

    // forces deoptimization, resulting in construction of canonical_context where the
    // not all of the vframes are contained within a stack frame. Was causing MNU due to
    // nils in the outer contexts, since the code was not passing the outer vframe when
    // crossing stack boundaries.
    call("NonInlinedBlockTest", "testTrap2");
  }
}

TESTF(CompilerTests, recompileZombieForcingFlush) {
  AddTestProcess addTest;
  {
    HandleMark mark;
    initializeSmalltalkEnvironment();

    Handle setup(oopFactory::new_symbol("testSetup2"));
    Handle varClass(Universe::find_global("NonInlinedBlockTest"));

    Universe::code->flush();
    Universe::code->compact();
    lookupCache::flush();

    ASSERT_TRUE(lookup("NonInlinedBlockTest", "exercise2:value:") == NULL);
    clearICs("NonInlinedBlockTest", "testSetup2");
    call("NonInlinedBlockTest", "testSetup2");
    ASSERT_TRUE(lookup("NonInlinedBlockTest", "exercise2:value:") == NULL);

    seed = compile("NonInlinedBlockTest", "exercise2:value:");
    clearICs("NonInlinedBlockTest", "testSetup2");
    call("NonInlinedBlockTest", "testSetup2");

    jumpTableEntry* entry = seed->noninlined_block_jumpEntry_at(1);
    nmethod* blocknm = entry->block_nmethod();

    LookupKey bogus(varClass.as_klass(), setup.as_oop());
    exhaustMethodHeap(bogus, blocknm->size());

    blocknm->inc_uncommon_trap_counter();
    blocknm->inc_uncommon_trap_counter();
    blocknm->inc_uncommon_trap_counter();
    blocknm->inc_uncommon_trap_counter();
    blocknm->inc_uncommon_trap_counter();

    call("NonInlinedBlockTest", "testTrap2");
  }
}

TESTF(CompilerTests, recompileZombieWhenMethodHeapExhausted) {
  AddTestProcess addTest;
  {
    initializeSmalltalkEnvironment();
    call("CompilerTest", "testOnce");
    compile("CompilerTest",  "with:");
    clearICs("CompilerTest", "testOnce");
    call("CompilerTest", "testOnce");
    seed = lookup("CompilerTest",  "with:");
    seed->inc_uncommon_trap_counter();
    seed->inc_uncommon_trap_counter();
    seed->inc_uncommon_trap_counter();
    seed->inc_uncommon_trap_counter();
    seed->inc_uncommon_trap_counter();

    exhaustMethodHeap(seed->key, seed->size());

    ASSERT_FALSE(seed->isZombie());
    // forces deoptimization and recompilation.
    call("CompilerTest", "testTwice");
    ASSERT_TRUE(seed->isZombie());
    nmethod* nm = lookup("CompilerTest",  "with:");
    ASSERT_FALSE((nm == seed));
  }
}
