# include "incls/_precompiled.incl"
# include "incls/_behavior_prims.cpp.incl"
#include "system_prims.hpp"
#include "handle.hpp"
#include "delta.hpp"
#include "test.h"

using namespace easyunit;

DECLARE(BehaviorPrimitivesSuperclassTests)
  PersistentHandle* delta;
  PersistentHandle* objectClassHandle;
  PersistentHandle* classClassHandle;
  PersistentHandle* metaclassClassHandle;
  PersistentHandle* classMetaclass;
  PersistentHandle* subclassHandle;
  PersistentHandle* superclassHandle;
  PersistentHandle* subclassName;
  PersistentHandle* superclassName;

  mixinOop createEmptyMixin() {
    PersistentHandle metaclassMixinHandle = createMixinSide("MetaClassMixin");
    mixinOop classMixin = createMixinSide("ClassMixin");

    classMixin->set_class_mixin(mixinOop(metaclassMixinHandle.as_oop()));

    return classMixin;
  }
  mixinOop createMixinSide(char* mixinClassName) {
    PersistentHandle classHandle(Universe::find_global(mixinClassName));
    PersistentHandle methods(oopFactory::new_objArray(0));
    PersistentHandle ivars(oopFactory::new_objArray(0));
    PersistentHandle classMethods(oopFactory::new_objArray(0));
    PersistentHandle classVars(oopFactory::new_objArray(0));

    mixinOop mixin = mixinOop(classHandle.as_klassOop()->klass_part()->allocateObject());
    mixin->set_methods(objArrayOop(methods.as_oop()));
    mixin->set_instVars(objArrayOop(ivars.as_oop()));
    mixin->set_classVars(objArrayOop(classVars.as_oop()));
    mixin->set_installed(falseObj);

    return mixin;
  }
  klassOop createClass(symbolOop className) {
    char name[100];
    {
      ResourceMark rm;
      strcpy(name, className->as_string());
    }

    PersistentHandle classNameHandle(className);
    symbolOop format = oopFactory::new_symbol("Oops");
    mixinOop mixin = createEmptyMixin();
    return klassOop(systemPrimitives::createNamedInvocation(format,
                                                            objectClassHandle->as_oop(),
                                                            trueObj, 
                                                            className,
                                                            mixin));
  }
  symbolOop as_symbol(PersistentHandle* handle) {
    return symbolOop(handle->as_oop());
  }
#define checkDoesntExist(className) {\
    ResourceMark rm;\
    char message[100];\
    char* name = as_symbol(className)->as_string();\
\
    sprintf(message, "Class '%s' already exists", name);\
    ASSERT_TRUE_M(!Universe::find_global(name), message);\
  }
#define checkClass(classHandle) {\
    ASSERT_TRUE_M(classHandle->as_oop()->is_mem(), "Should be memOop");\
    ASSERT_TRUE_M(Universe::really_contains(classHandle->as_oop()), "Should be in universe");\
  }
  void safeDelete(PersistentHandle* handle) {
    if (handle)
      delete(handle);
  }
  void remove(PersistentHandle* classNameHandle) {
    ResourceMark rm;
    symbolOop className = as_symbol(classNameHandle);
    char* name = className->as_string();
    if (Universe::find_global(name))
      Delta::call(delta->as_oop(), oopFactory::new_symbol("removeKey:"), className);
  }
  klassOop superclassOf(klassOop aClass) {
    return aClass->klass_part()->superKlass();
  }
  klassOop subclass() {
    return subclassHandle->as_klassOop();
  }
  klassOop superclass() {
    return superclassHandle->as_klassOop();
  }
  klassOop objectClass() {
    return objectClassHandle->as_klassOop();
  }
  klassOop classClass() {
    return classClassHandle->as_klassOop();
  }
  klassOop metaclassClass() {
    return metaclassClassHandle->as_klassOop();
  }
  void checkMarkedSymbol(char* message, oop result, symbolOop expected) {
    ResourceMark rm;
    char text[200];
    ASSERT_TRUE_M(result->is_mark(), "Should be marked");
    sprintf(text,"%s. Should be: %s, was: %s", message, expected->as_string(), unmarkSymbol(result)->as_string());
    ASSERT_TRUE_M(unmarkSymbol(result) == expected, text);
  }
END_DECLARE

SETUP(BehaviorPrimitivesSuperclassTests) {
   delta          = new PersistentHandle(Universe::find_global("Smalltalk"));
   objectClassHandle    = new PersistentHandle(Universe::find_global("Object"));
   classClassHandle     = new PersistentHandle(Universe::find_global("Class"));
   metaclassClassHandle     = new PersistentHandle(Universe::find_global("Metaclass"));
   classMetaclass = new PersistentHandle(classClassHandle->as_klassOop()->klass_part()->superKlass());
   subclassName   = new PersistentHandle(oopFactory::new_symbol("BehaviorPrimsSubclassFixture"));
   superclassName = new PersistentHandle(oopFactory::new_symbol("BehaviorPrimsSuperclassFixture"));

   checkDoesntExist(superclassName);
   checkDoesntExist(subclassName);

   subclassHandle       = new PersistentHandle(createClass(as_symbol(subclassName)));
   checkClass(subclassHandle);
   superclassHandle     = new PersistentHandle(createClass(as_symbol(superclassName)));
   checkClass(superclassHandle);
}

TEARDOWN(BehaviorPrimitivesSuperclassTests){
  if (subclassHandle)
    remove(subclassName);
  if (superclassHandle)
    remove(superclassName);

  safeDelete(delta);
  safeDelete(classMetaclass);
  safeDelete(classClassHandle);
  safeDelete(metaclassClassHandle);
  safeDelete(objectClassHandle);
  safeDelete(subclassName);
  safeDelete(superclassName);
  safeDelete(subclassHandle);
  safeDelete(superclassHandle);
}

TESTF(BehaviorPrimitivesSuperclassTests, setSuperclassShouldChangeSuperclassToNewClass) {
  ASSERT_TRUE_M(superclassOf(subclass()) == objectClass(), "Original superclassHandle");

  oop result = behaviorPrimitives::setSuperclass(superclass(), subclass());

  ASSERT_TRUE_M(subclass() == result, "Should return receiver");
  ASSERT_TRUE_M(superclassOf(subclass()) == superclass(), "Superclass should have changed");
  ASSERT_TRUE_M(superclassOf(subclass()->klass()) == objectClass()->klass(), 
                "Metasuperclass should be unchanged");
}

TESTF(BehaviorPrimitivesSuperclassTests, setSuperclassShouldReportErrorWhenReceiverNotAClass) {
  oop result = behaviorPrimitives::setSuperclass(superclass(), oopFactory::new_objArray(0));

  checkMarkedSymbol("Should report error", result, vmSymbols::receiver_has_wrong_type());
}

TESTF(BehaviorPrimitivesSuperclassTests, setSuperclassShouldReportErrorWhenNewSuperclassNotAClass) {
  oop result = behaviorPrimitives::setSuperclass(oopFactory::new_objArray(0), subclass());

  checkMarkedSymbol("Should report error", result, vmSymbols::first_argument_has_wrong_type());
}

TESTF(BehaviorPrimitivesSuperclassTests, setSuperclassShouldReportErrorWhenNewSuperclassHasDifferentSize) {
  oop result = behaviorPrimitives::setSuperclass(classClass(), subclass());

  checkMarkedSymbol("Should report error", result, vmSymbols::invalid_klass());
}

TESTF(BehaviorPrimitivesSuperclassTests, setSuperclassShouldReportErrorWhenInstanceVariableNamesAreDifferent) {
  subclass()->klass_part()->set_superKlass(classClass());
  oop result = behaviorPrimitives::setSuperclass(metaclassClass(), subclass());

  checkMarkedSymbol("Should report error", result, vmSymbols::invalid_klass());
}
