/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.55 $ */
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
# include "incls/_klass.cpp.incl" 

void Klass::initialize() {
  set_untagged_contents(false);
  set_classVars(objArrayOop(oopFactory::new_objArray(0)));
  set_methods(objArrayOop(oopFactory::new_objArray(0)));
  set_superKlass(klassOop(nilObj));
  set_mixin(mixinOop(nilObj));
}

oop Klass::allocateObject(bool permit_scavenge, bool tenured) {
  return markSymbol(vmSymbols::not_oops());
}
oop Klass::allocateObjectSize(int size, bool permit_scavenge, bool permit_tenured) {
  return markSymbol(vmSymbols::not_oops());
}

void Klass::mark_for_schema_change() {
  set_mixin(NULL);
}

bool Klass::is_marked_for_schema_change() {
  return mixin() == NULL;
}

Klass::Format Klass::format_from_symbol(symbolOop format) {
  if (format->equals("Oops"))                               return mem_klass;
  if (format->equals("ExternalProxy"))                      return proxy_klass;
  // if (format->equals("Process"))                            return process_klass;
  if (format->equals("IndexedInstanceVariables"))           return objArray_klass;
  if (format->equals("IndexedByteInstanceVariables"))       return byteArray_klass;
  if (format->equals("IndexedDoubleByteInstanceVariables")) return doubleByteArray_klass;
  if (format->equals("IndexedNextOfKinInstanceVariables"))  return weakArray_klass;
  if (format->equals("Special"))                            return special_klass;
  return Klass::no_klass;
}

char* Klass::name_from_format(Format format) {
  switch (format) {
  case mem_klass:             return "Oops";
  case proxy_klass:           return "ExternalProxy";
  // case process_klass:         return "Process";
  case objArray_klass:        return "IndexedInstanceVariables";
  case byteArray_klass:       return "IndexedByteInstanceVariables";
  case doubleByteArray_klass: return "IndexedDoubleByteInstanceVariables";
  case weakArray_klass:       return "IndexedNextOfKinInstanceVariables";
  }
  return "Special";
}

bool Klass::has_same_layout_as(klassOop klass) {
  assert(klass->is_klass(), "argument must be klass");
  // Check equality
  if (as_klassOop() == klass) return true;

  // Check format
  if (format() != klass->klass_part()->format()) return false; 

  // Check instance size
  if (non_indexable_size() != klass->klass_part()->non_indexable_size()) return false;

  // Check instance variables
  for (int index = oop_header_size(); index < non_indexable_size(); index++) {
    if (inst_var_name_at(index) !=  klass->klass_part()->inst_var_name_at(index)) return false;
  }
  return true;
}

bool Klass::has_same_inst_vars_as(klassOop klass) {
  assert(klass->is_klass(), "argument must be klass");
  // Check equality
  if (as_klassOop() == klass) return true;

  Klass* classPart = klass->klass_part();
  // Check instance size
  int ivars      = number_of_instance_variables();
  int classIvars = classPart->number_of_instance_variables();
  if (ivars != classIvars)
    return false;

  // Check instance variables
  for (int offset = 1; offset <= number_of_instance_variables(); offset++) {
    if (inst_var_name_at(non_indexable_size() - offset) != 
      classPart->inst_var_name_at(classPart->non_indexable_size() - offset)) return false;
  }
  return true;
}

klassOop Klass::create_subclass(mixinOop mixin, Format format) {
  ShouldNotCallThis();
  return NULL;
}

klassOop Klass::create_subclass(mixinOop mixin, klassOop instSuper, klassOop metaClass, Format format) {
  ShouldNotCallThis();
  return NULL;
}

klassOop Klass::create_generic_class(klassOop superMetaClass, klassOop superClass, klassOop metaMetaClass, mixinOop mixin, int vtbl) {
  BlockScavenge bs;

  assert(mixin->classVars()->is_objArray(),                "checking instance side class var names");
  assert(mixin->class_mixin()->classVars()->is_objArray(), "checking class side class var names");
  assert(mixin->class_mixin()->classVars()->length() == 0, "checking class side class var names");

  int length = mixin->number_of_classVars();

  objArrayOop class_vars = oopFactory::new_objArray(mixin->number_of_classVars());
  for (int index = 1; index <= length; index++) {
    associationOop assoc = oopFactory::new_association(mixin->classVar_at(index), nilObj, false);
    class_vars->obj_at_put(index, assoc);
  }

  // Meta class
  klassOop meta_klass = klassOop(metaMetaClass->klass_part()->allocateObject());
  Klass* mk = meta_klass->klass_part();
  mk->set_untagged_contents(false);
  mk->set_classVars(class_vars);
  mk->set_methods(oopFactory::new_objArray(0));
  mk->set_superKlass(superMetaClass);
  mk->set_mixin(mixin->class_mixin());
  mk->set_non_indexable_size(klassOopDesc::header_size() + mk->number_of_instance_variables());
  set_klassKlass_vtbl(mk);

  klassOop klass = klassOop(mk->allocateObject());
  Klass*   k     = klass->klass_part();

  k->set_untagged_contents(false);
  k->set_classVars(class_vars);
  k->set_methods(oopFactory::new_objArray(0));
  k->set_superKlass(superClass);
  k->set_mixin(mixin);
  k->set_vtbl_value(vtbl);
  k->set_non_indexable_size(k->oop_header_size() + k->number_of_instance_variables());

  return klass;
}
klassOop Klass::create_generic_class(klassOop super_class, mixinOop mixin, int vtbl) {
  return create_generic_class(super_class->klass(), super_class, super_class->klass()->klass(), mixin, vtbl);
}

symbolOop Klass::inst_var_name_at(int offset) const {
  Klass* current_klass  = (Klass*) this;
  int   current_offset = non_indexable_size();
  do {
    mixinOop m = current_klass->mixin();
    for (int index = m->number_of_instVars(); index > 0; index--) {
      current_offset--;
      if (offset == current_offset) return m->instVar_at(index);
    } 
  } while(current_klass = (current_klass->superKlass() == nilObj ? NULL : current_klass->superKlass()->klass_part()));
  return NULL;
}

int Klass::lookup_inst_var(symbolOop name) const {
  int offset = mixin()->inst_var_offset(name, non_indexable_size());
  if (offset >= 0) return offset;
  return has_superKlass() ? superKlass()->klass_part()->lookup_inst_var(name) : -1;
}

int Klass::number_of_instance_variables() const {
  return mixin()->number_of_instVars() 
       + (has_superKlass() ? superKlass()->klass_part()->number_of_instance_variables() : 0);
}

// OPERATION FOR METHODS

int Klass::number_of_methods() const {
  return methods()->length();
}

methodOop Klass::method_at(int index)  const {
  return methodOop(methods()->obj_at(index));
}

void Klass::add_method(methodOop method) {
  objArrayOop old_array = methods();
  symbolOop selector = method->selector();
  // Find out if a method with the same selector exists.
  for (int index = 1; index <= old_array->length(); index++) {
    assert(old_array->obj_at(index)->is_method(), "must be method");
    methodOop m = methodOop(old_array->obj_at(index));
    if (m->selector() == selector) {
      old_array->obj_at_put(index, method);
      return;
    }
  }
  // Extend the array
  set_methods(methods()->copy_add(method));
}

methodOop Klass::remove_method_at(int index) {
  methodOop method = method_at(index);
  set_methods(methods()->copy_remove(index));
  return method;
}

int Klass::number_of_classVars() const {
  return classVars()->length();
}

associationOop Klass::classVar_at(int index)  const {
  return associationOop(classVars()->obj_at(index));
}

void Klass::add_classVar(associationOop assoc) {
  objArrayOop array = classVars();
  // Find out if it already exists.
  for (int index = 1; index <= array->length(); index++) {
    assert(array->obj_at(index)->is_association(), "must be symbol");
    associationOop elem = associationOop(array->obj_at(index));
    if (elem->key() == assoc->key()) return;
  }
  // Extend the array
  set_classVars(array->copy_add(assoc));
}

associationOop Klass::remove_classVar_at(int index) {
  associationOop assoc = classVar_at(index);
  set_classVars(classVars()->copy_remove(index));
  return assoc;
}

bool Klass::includes_classVar(symbolOop name) {
  objArrayOop array = classVars();
  for (int index = 1; index <= array->length(); index++) {
    associationOop elem = associationOop(array->obj_at(index));
    if (elem->key() == name) return true;
  }
  return false;
}

associationOop Klass::local_lookup_class_var(symbolOop name) {
  objArrayOop array = classVars();
  for (int index = 1; index <= array->length(); index++) {
    assert(array->obj_at(index)->is_association(), "must be symbol");
    associationOop elem = associationOop(array->obj_at(index));
    if (elem->key() == name) return elem;
  }
  return NULL;
}

associationOop Klass::lookup_class_var(symbolOop name) {
  associationOop result = local_lookup_class_var(name);
  if (result) return result;
  result = (superKlass() == nilObj)
           ? NULL
           : superKlass()->klass_part()->lookup_class_var(name);
  return result;
}


inline methodOop Klass::local_lookup(symbolOop selector) {
  objArrayOop array;
  int         length;
  oop*        current;

  // Find out if there is a customized method matching the selector.
  array  = methods();
  length = array->length();
  if (length > 0) {
    current = array->objs(1);
    do {
      methodOop method = methodOop(*current++);
      assert(method->is_method(), "must be method");   
      if (method->selector() == selector)
	return method;
    } while (--length > 0);
  }

  assert(mixin()->is_mixin(), "mixin must exist");

  array   = mixin()->methods();
  length  = array->length();
  current = array->objs(1);

  while (length-- > 0) {
    methodOop method = methodOop(*current++);
    assert(method->is_method(), "must be method");   
    if (method->selector() == selector) {
      if (method->should_be_customized()) {
        if (as_klassOop() == mixin()->primary_invocation()){
          if (!method->is_customized()) method->customize_for(as_klassOop(), mixin());
          return method;
	} else {
	  BlockScavenge bs;
          // Make customized version for klass
          methodOop new_method = method->copy_for_customization();
	  new_method->customize_for(as_klassOop(), mixin());
          add_method(new_method);
	  return new_method;
	}
      }
      return method;
    }
  }
  return NULL;
}

methodOop Klass::lookup(symbolOop selector) {
  Klass* current = this;
  while (true) {
    methodOop result = current->local_lookup(selector);
    if (result)                          return result;
    if (current->superKlass() == nilObj) {
      ResourceMark rm;
      MissingMethodBuilder builder(selector);
      builder.build();
      methodOop method = builder.method();
      current->add_method(method);
      return method;
    }
    current = current->superKlass()->klass_part();
  }
  return NULL;
}

bool Klass::is_method_holder_for(methodOop method) {
  // Always use the home  of the method in case of a blockMethod  
  methodOop m = method->home();

  objArrayOop array = methods();
  // Find out if a method with the same selector exists.
  for (int index = 1; index <= array->length(); index++) {
    assert(array->obj_at(index)->is_method(), "must be method");
    if (methodOop(array->obj_at(index)) == m) return true;
  }
  // Try the mixin
  if (oop(mixin()) != nilObj) {
    assert(mixin()->is_mixin(), "must be mixin");
    array = mixin()->methods();
    for (int index = 1; index <= array->length(); index++) {
      assert(array->obj_at(index)->is_method(), "must be method");
      if (methodOop(array->obj_at(index)) == m) return true;
    }
  }
  return false;
}

klassOop Klass::lookup_method_holder_for(methodOop method) {
  if (is_method_holder_for(method)) return as_klassOop();
  return (superKlass() == nilObj)
         ? NULL
         : superKlass()->klass_part()->lookup_method_holder_for(method);
}

void Klass::flush_methods() {
  set_methods(oopFactory::new_objArray(0));
}

bool Klass::is_specialized_class() const {
  memOopKlass m;
  return name() == m.name();
}

bool Klass::is_named_class() const {
  return mixin()->primary_invocation() == as_klassOop();
}

void Klass::print_klass() {
  lprintf("%sKlass (%s)", name(), name_from_format(format()));
}

char* Klass::delta_name() {
  bool meta = false;
  int offset = as_klassOop()->blueprint()->lookup_inst_var(oopFactory::new_symbol("name"));
  symbolOop name = NULL;
  if (offset >= 0) {
    name = symbolOop(as_klassOop()->raw_at(offset));
    if (!name->is_symbol()) name = NULL;
  }
  if (name == NULL) {
    name = Universe::find_global_key_for(as_klassOop(), &meta);
    if (!name) return NULL;
  }

  int length = name->length() + (meta ? 7 : 1);
  char* toReturn = NEW_RESOURCE_ARRAY(char, length);
  strncpy(toReturn, name->chars(), name->length());

  if (meta)
    strcpy(toReturn + name->length(), " class");
  toReturn[length-1] = '\0';

  return toReturn;
}

void Klass::print_name_on(outputStream* st) {
  int offset = as_klassOop()->blueprint()->lookup_inst_var(oopFactory::new_symbol("name"));
  symbolOop name = NULL;
  if (offset >= 0) {
    name = symbolOop(as_klassOop()->raw_at(offset));
    if (!name->is_symbol()) name = NULL;
  }
  if (name != NULL) name->print_symbol_on(st);
  else {
    bool meta;
    name = Universe::find_global_key_for(as_klassOop(), &meta);
    if (name) {
      name->print_symbol_on(st);
      if (meta) st->print(" class");
    } else {
      st->print("??");
    }
  }
}

int Klass::oop_scavenge_contents(oop obj) {
  fatal("should not call Klass::oop_scavenge_contents");
  return 0;
}

int Klass::oop_scavenge_tenured_contents(oop obj) {
  fatal("should not call Klass::oop_scavenge_promotion");
  return 0;
}

void Klass::oop_follow_contents(oop obj) {
  fatal("should not call Klass::oop_follow_contents");
}

void Klass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  fatal("should not call layout_iterate on Klass");
}

void Klass::oop_oop_iterate(oop obj, OopClosure *blk) {
  fatal("should not call oop_iterate on Klass");
}

void Klass::oop_print_on      (oop obj, outputStream* st) { fatal("should not call Klass::oop_print_on"); }
void Klass::oop_print_value_on(oop obj, outputStream* st) { oop_print_on(obj, st); }
void Klass::oop_short_print_on(oop obj, outputStream* st) { 
  if (obj == trueObj) {
    st->print("true");
  } else if (obj == falseObj) {
    st->print("false");
  } else if (obj == nilObj) {
     st->print("nil");
  } else {
    oop_print_value_on(obj, st); 
  }
}

bool Klass::oop_verify(oop obj) {
  // FIX LATER
  return true;
}

void Klass::bootstrap_klass_part_one(bootstrap* st) {
  st->read_oop((oop*)&_non_indexable_size);
  st->read_oop((oop*)&_has_untagged_contents);
}

void Klass::bootstrap_klass_part_two(bootstrap* st) {
  st->read_oop((oop*)&_classVars);
  st->read_oop((oop*)&_methods);
  st->read_oop((oop*)&_superKlass);
  st->read_oop((oop*)&_mixin);
}  

oop Klass::oop_primitive_allocate(oop obj, bool allow_scavenge, bool tenured) {
  return markSymbol(vmSymbols::not_klass()); }

oop Klass::oop_primitive_allocate_size(oop obj, int size) {
  return markSymbol(vmSymbols::not_klass()); }

oop Klass::oop_shallow_copy(oop obj, bool tenured) {
  return markSymbol(vmSymbols::not_oops()); }
