# include "incls/_precompiled.incl"
# include "incls/_mixinOop.cpp.incl"
#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" oop* eden_top;
extern "C" oop* eden_end;

DECLARE(MemOopKlassTests)
  klassOop theClass;
  oop* oldEdenTop;
  mixinOop createMixin() {
    PersistentHandle classMixinClass(Universe::find_global("ClassMixin"));
    PersistentHandle metaClassMixinClass(Universe::find_global("MetaClassMixin"));

    PersistentHandle methods(oopFactory::new_objArray(0));
    PersistentHandle ivars(oopFactory::new_objArray(0));
    PersistentHandle classMethods(oopFactory::new_objArray(0));
    PersistentHandle classIvars(oopFactory::new_objArray(0));
    PersistentHandle classVars(oopFactory::new_objArray(0));

    PersistentHandle classMixin(classMixinClass.as_klassOop()->klass_part()->allocateObject());
    PersistentHandle metaClassMixin(metaClassMixinClass.as_klassOop()->klass_part()->allocateObject());

    mixinOop klassMixin = mixinOop(classMixin.as_oop());
    klassMixin->set_methods(objArrayOop(methods.as_oop()));
    klassMixin->set_instVars(objArrayOop(ivars.as_oop()));
    klassMixin->set_classVars(objArrayOop(classVars.as_oop()));
    klassMixin->set_class_mixin(mixinOop(metaClassMixin.as_oop()));
    klassMixin->set_installed(falseObj);

    mixinOop metaKlassMixin = mixinOop(classMixin.as_oop());
    metaKlassMixin->set_methods(objArrayOop(classMethods.as_oop()));
    metaKlassMixin->set_instVars(objArrayOop(classIvars.as_oop()));
    metaKlassMixin->set_classVars(objArrayOop(classVars.as_oop()));
    metaKlassMixin->set_installed(falseObj);

    return klassMixin;
  }
END_DECLARE

SETUP(MemOopKlassTests) {
  //theClass = klassOop(Universe::find_global("Object"));
  //oldEdenTop = eden_top;
}

TEARDOWN(MemOopKlassTests) {
  //eden_top = oldEdenTop;
  //MarkSweep::collect();
}

TESTF(MemOopKlassTests, createSubclassShouldCreateClassWithCorrectSuperForClassAndMeta) {
  PersistentHandle kl(Universe::find_global("Class"));
  PersistentHandle mk(Universe::find_global("Metaclass"));
  PersistentHandle instSuper(Universe::find_global("Test"));
  ASSERT_TRUE(kl.as_oop());

  PersistentHandle classMixin(createMixin());

  klassOop newKlass = kl.as_klassOop()->klass_part()->create_subclass(mixinOop(classMixin.as_oop()), 
    instSuper.as_klassOop(), mk.as_klassOop(), Klass::mem_klass);

  ASSERT_TRUE(newKlass);
  ASSERT_TRUE(newKlass->klass_part()->superKlass() == instSuper.as_klassOop());
  ASSERT_TRUE(newKlass->klass()->klass_part()->superKlass() == kl.as_klassOop());
  ASSERT_TRUE(newKlass->klass()->klass() == mk.as_klassOop());
  ASSERT_TRUE(newKlass->klass_part()->mixin() == classMixin.as_oop());
  ASSERT_TRUE(newKlass->klass()->klass_part()->mixin() == mixinOop(classMixin.as_oop())->class_mixin());
}

TESTF(MemOopKlassTests, oldCreateSubclassShouldCreateClassWithCorrectSuperForClassAndMeta) {
  PersistentHandle kl(Universe::find_global("Class"));
  PersistentHandle mk(Universe::find_global("Metaclass"));
  PersistentHandle instSuper(Universe::find_global("Test"));
  ASSERT_TRUE(kl.as_oop());

  PersistentHandle classMixin(createMixin());

  klassOop newKlass = instSuper.as_klassOop()->klass_part()->create_subclass(mixinOop(classMixin.as_oop()), Klass::mem_klass);

  klassOop superClass = instSuper.as_klassOop();
  mixinOop mixin = mixinOop(classMixin.as_oop());

  ASSERT_TRUE(newKlass);
  ASSERT_TRUE(newKlass->klass_part()->superKlass() == superClass);
  ASSERT_TRUE(newKlass->klass()->klass_part()->superKlass() == superClass->klass());
  ASSERT_TRUE(newKlass->klass()->klass() == superClass->klass()->klass());
  ASSERT_TRUE(newKlass->klass_part()->mixin() == mixin);
  ASSERT_TRUE(newKlass->klass()->klass_part()->mixin() == mixin->class_mixin());
}