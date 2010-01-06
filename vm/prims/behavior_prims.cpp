/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.30 $ */
/* Copyright (c) 2006, Sun Microsystems, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
	  disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Sun Microsystems nor the names of its contributors may be used to endorse or promote products derived 
	  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT 
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE


*/

# include "incls/_precompiled.incl"
# include "incls/_behavior_prims.cpp.incl"

TRACE_FUNC(TraceBehaviorPrims, "behavior")

int behaviorPrimitives::number_of_calls;

#define ASSERT_RECEIVER assert(receiver->is_klass(), "receiver must klass")

PRIM_DECL_2(behaviorPrimitives::allocate3, oop receiver, oop tenured) {
  PROLOGUE_2("allocate3", receiver, tenured)
  ASSERT_RECEIVER;
  if (tenured != Universe::trueObj() && tenured != Universe::falseObj())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  oop result = receiver->primitive_allocate(false, tenured == Universe::trueObj());
  if (NULL == result)
    return markSymbol(vmSymbols::failed_allocation());
  return result;
}

PRIM_DECL_1(behaviorPrimitives::allocate2, oop receiver) {
  PROLOGUE_1("allocate2", receiver)
  ASSERT_RECEIVER;
  return receiver->primitive_allocate();
}

PRIM_DECL_1(behaviorPrimitives::allocate, oop receiver) {
  PROLOGUE_1("allocate", receiver)
  ASSERT_RECEIVER;
  return receiver->primitive_allocate();
}

PRIM_DECL_1(behaviorPrimitives::superclass, oop receiver) {
  PROLOGUE_1("superclass", receiver);
  ASSERT_RECEIVER;
  return klassOop(receiver)->klass_part()->superKlass();
}

PRIM_DECL_1(behaviorPrimitives::superclass_of, oop klass) {
  PROLOGUE_1("superclass_of", klass);
  if (!klass->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return klassOop(klass)->klass_part()->superKlass();
}

PRIM_DECL_2(behaviorPrimitives::setSuperclass, oop receiver, oop newSuper) {
  PROLOGUE_2("setSuperclass", receiver, newSuper);
  if (!receiver->is_klass())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!(newSuper->is_klass() || newSuper == nilObj))
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  Klass* receiverClass = klassOop(receiver)->klass_part();
  klassOop newSuperclass;
  if (receiverClass->superKlass() == newSuper) return receiver; // no change
  if (receiverClass->superKlass() == nilObj) {
    newSuperclass = klassOop(newSuper);
    if (newSuperclass->klass_part()->number_of_instance_variables() > 0)
      return markSymbol(vmSymbols::argument_is_invalid());
  } else {
    Klass* oldSuperclass = receiverClass->superKlass()->klass_part();
    if (newSuper == nilObj) {
      newSuperclass = klassOop(nilObj);
      if (oldSuperclass->number_of_instance_variables() > 0)
        return markSymbol(vmSymbols::argument_is_invalid());
    } else {
      newSuperclass = klassOop(newSuper);

      if (!oldSuperclass->has_same_inst_vars_as(newSuperclass))
        return markSymbol(vmSymbols::invalid_klass());
    }
  }
  receiverClass->set_superKlass(newSuperclass);
  
  return receiver;
}

PRIM_DECL_1(behaviorPrimitives::mixinOf, oop behavior) {
  PROLOGUE_1("mixinOf", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return klassOop(behavior)->klass_part()->mixin();
}

PRIM_DECL_1(behaviorPrimitives::headerSize, oop behavior) {
  PROLOGUE_1("headerSize", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return as_smiOop(klassOop(behavior)->klass_part()->oop_header_size());
}

PRIM_DECL_1(behaviorPrimitives::nonIndexableSize, oop behavior) {
  PROLOGUE_1("nonIndexableSize", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return as_smiOop(klassOop(behavior)->klass_part()->non_indexable_size());
}

PRIM_DECL_1(behaviorPrimitives::is_specialized_class, oop behavior) {
  PROLOGUE_1("is_specialized_class", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return klassOop(behavior)->klass_part()->is_specialized_class() ? trueObj : falseObj;
}

PRIM_DECL_1(behaviorPrimitives::can_be_subclassed, oop behavior) {
  PROLOGUE_1("can_be_subclassed", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return klassOop(behavior)->klass_part()->can_be_subclassed() ? trueObj : falseObj;
}

PRIM_DECL_1(behaviorPrimitives::can_have_instance_variables, oop behavior) {
  PROLOGUE_1("can_have_instance_variables", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return klassOop(behavior)->klass_part()->can_have_instance_variables() ? trueObj : falseObj;
}

// OPERATIONS FOR CLASS VARIABLES

PRIM_DECL_2(behaviorPrimitives::classVariableAt, oop behavior, oop index) {
  PROLOGUE_2("classVariableAt", behavior, index);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!index->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  int i = smiOop(index)->value();
  if (i > 0 && i <= klassOop(behavior)->klass_part()->number_of_classVars())
    return klassOop(behavior)->klass_part()->classVar_at(i);
  return markSymbol(vmSymbols::out_of_bounds());
}

PRIM_DECL_1(behaviorPrimitives::classVariables, oop behavior) {
  PROLOGUE_1("classVariables", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return klassOop(behavior)->klass_part()->classVars();
}

// OPERATIONS FOR METHODS 

PRIM_DECL_2(behaviorPrimitives::printMethod, oop receiver, oop name){
  PROLOGUE_2("printMethod", receiver, name);
  ASSERT_RECEIVER;
  if (!name->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  methodOop m = klassOop(receiver)->klass_part()->lookup(symbolOop(name));
  if (!m) return markSymbol(vmSymbols::not_found());
  m->print_codes();
  return receiver;
}

/*

PRIM_DECL_1(behaviorPrimitives::new0, oop receiver){
  PROLOGUE_1("new0", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size(), &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new1, oop receiver){
  PROLOGUE_1("new1", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 1, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 1 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new2, oop receiver){
  PROLOGUE_1("new2", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 2, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 2 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 1), value, false);
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new3, oop receiver){
  PROLOGUE_1("new3", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 3, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 3 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 1), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 2), value, false);
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new4, oop receiver){
  PROLOGUE_1("new4", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 4, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 4 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 1), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 2), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 3), value, false);
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new5, oop receiver){
  PROLOGUE_1("new5", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 5, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 5 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 1), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 2), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 3), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 4), value, false);
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new6, oop receiver){
  PROLOGUE_1("new6", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 6, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 6 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 1), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 2), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 3), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 4), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 5), value, false);
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new7, oop receiver){
  PROLOGUE_1("new7", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 7, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 7 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 1), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 2), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 3), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 4), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 5), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 6), value, false);
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new8, oop receiver){
  PROLOGUE_1("new8", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 8, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 8 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 1), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 2), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 3), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 4), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 5), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 6), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 7), value, false);
  return obj;
}

PRIM_DECL_1(behaviorPrimitives::new9, oop receiver){
  PROLOGUE_1("new9", receiver);
  memOop klass = memOop(receiver);
  // allocate
  memOop obj = as_memOop(Universe::allocate(memOopDesc::header_size() + 9, &klass));
  // header
  obj->initialize_header(false, klassOop(klass));
  // initialize 9 instance variable
  oop value = nilObj;
  Universe::store(obj->oops(memOopDesc::header_size() + 0), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 1), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 2), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 3), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 4), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 5), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 6), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 7), value, false);
  Universe::store(obj->oops(memOopDesc::header_size() + 8), value, false);
   return obj;
}

*/

PRIM_DECL_2(behaviorPrimitives::methodFor, oop receiver, oop selector) {
  PROLOGUE_2("methodFor", receiver, selector);
  ASSERT_RECEIVER;

  if (!selector->is_symbol())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  methodOop m = klassOop(receiver)->klass_part()->lookup(symbolOop(selector));
  if (m) return m;
  return markSymbol(vmSymbols::not_found());
}

PRIM_DECL_1(behaviorPrimitives::format, oop behavior) {
  PROLOGUE_1("format", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  char* format_name =  Klass::name_from_format(klassOop(behavior)->klass_part()->format());
  return oopFactory::new_symbol(format_name);
}

PRIM_DECL_1(behaviorPrimitives::vm_type, oop behavior) {
  PROLOGUE_1("format", behavior);
  if (!behavior->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  Klass::Format f = klassOop(behavior)->klass_part()->format();

  switch (f) {
   case Klass::mem_klass:              return vmSymbols::mem_klass();
   case Klass::association_klass:      return vmSymbols::association_klass();
   case Klass::blockClosure_klass:     return vmSymbols::blockClosure_klass();
   case Klass::byteArray_klass:        return vmSymbols::byteArray_klass();
   case Klass::symbol_klass:           return vmSymbols::symbol_klass();
   case Klass::context_klass:          return vmSymbols::context_klass();
   case Klass::doubleByteArray_klass:  return vmSymbols::doubleByteArray_klass();
   case Klass::doubleValueArray_klass: return vmSymbols::doubleValueArray_klass();
   case Klass::double_klass:           return vmSymbols::double_klass();
   case Klass::klass_klass:            return vmSymbols::klass_klass();
   case Klass::method_klass:           return vmSymbols::method_klass();
   case Klass::mixin_klass:            return vmSymbols::mixin_klass();
   case Klass::objArray_klass:         return vmSymbols::objArray_klass();
   case Klass::weakArray_klass:        return vmSymbols::weakArray_klass();
   case Klass::process_klass:          return vmSymbols::process_klass();
   case Klass::vframe_klass:           return vmSymbols::vframe_klass();
   case Klass::proxy_klass:            return vmSymbols::proxy_klass();
   case Klass::smi_klass:              return vmSymbols::smi_klass();
   default: 
     fatal("wrong format for klass");
  }
  return markSymbol(vmSymbols::first_argument_has_wrong_type());
}


PRIM_DECL_2(behaviorPrimitives::is_class_of, oop receiver, oop obj) {
  PROLOGUE_2("is_class_of", receiver, obj);
  ASSERT_RECEIVER;
  return obj->klass() == receiver ? trueObj : falseObj;
}

// empty functions, we'll patch them later
static void trap() { assert(false, "This primitive should be patched"); };

extern "C" oop primitiveInlineAllocations(oop receiver, oop count) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew0(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew1(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew2(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew3(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew4(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew5(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew6(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew7(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew8(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop primitiveNew9(oop receiver, oop tenured) { trap(); return markSymbol(vmSymbols::primitive_trap()); };
