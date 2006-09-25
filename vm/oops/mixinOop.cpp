/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.13 $ */
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
# include "incls/_mixinOop.cpp.incl"

int mixinOopDesc::inst_var_offset(symbolOop name, int non_indexable_size) const {
  objArrayOop array  = instVars();
  int         length = array->length(); 
  for (int index = 1; index <= length; index++) {
    if (array->obj_at(index) == name) {
      return non_indexable_size - (length - index + 1);
    }
  }
  return -1;
}

void mixinOopDesc::bootstrap_object(bootstrap* st) {
  memOopDesc::bootstrap_header(st);
  st->read_oop((oop*)&addr()->_methods);
  st->read_oop((oop*)&addr()->_inst_vars);
  st->read_oop((oop*)&addr()->_class_vars);
  st->read_oop((oop*)&addr()->_primary_invocation);
  st->read_oop((oop*)&addr()->_class_mixin);
  st->read_oop((oop*)&addr()->_installed);
  memOopDesc::bootstrap_body(st, header_size());
}

int mixinOopDesc::number_of_methods() const {
  return methods()->length();
}

methodOop mixinOopDesc::method_at(int index) const {
  return methodOop(methods()->obj_at(index));
}

void mixinOopDesc::add_method(methodOop method) {
  objArrayOop old_array = methods();
  symbolOop selector = method->selector();
  // Find out if a method with the same selector exists.
  for (int index = 1; index <= old_array->length(); index++) {
    assert(old_array->obj_at(index)->is_method(), "must be method");
    methodOop m = methodOop(old_array->obj_at(index));
    if (m->selector() == selector) {
      objArrayOop new_array = old_array->copy();
      new_array->obj_at_put(index, method);
      set_methods(new_array);
      return;
    }
  }
  // Extend the array
  set_methods(old_array->copy_add(method));
}

methodOop mixinOopDesc::remove_method_at(int index) {
  methodOop method = method_at(index);
  set_methods(methods()->copy_remove(index));
  return method;
}

bool mixinOopDesc::includes_method(methodOop method) {
  objArrayOop array = methods();
  for (int index = 1; index <= array->length(); index++) {
    methodOop m = methodOop(array->obj_at(index));
    if (m == method) return true;
  }
  return false;
}

int mixinOopDesc::number_of_instVars() const {
  return instVars()->length();
}

symbolOop mixinOopDesc::instVar_at(int index) const {
  return symbolOop(instVars()->obj_at(index));
}

void mixinOopDesc::add_instVar(symbolOop name) {
  objArrayOop old_array = instVars();
  // Find out if it already exists.
  for (int index = 1; index <= old_array->length(); index ++) {
    assert(old_array->obj_at(index)->is_symbol(), "must be symbol");
    if (old_array->obj_at(index) == name) return;
  }
  // Extend the array
  set_instVars(old_array->copy_add(name));
}

symbolOop mixinOopDesc::remove_instVar_at(int index) {
  symbolOop name = instVar_at(index);
  set_instVars(instVars()->copy_remove(index, 1));
  return name;
}

bool mixinOopDesc::includes_instVar(symbolOop name) {
  objArrayOop array = instVars();
  for (int index = 1; index <= array->length(); index++) {
    symbolOop elem = symbolOop(array->obj_at(index));
    if (elem == name) return true;
  }
  return false;
}

int mixinOopDesc::number_of_classVars() const {
  return classVars()->length();
}

symbolOop mixinOopDesc::classVar_at(int index) const {
  return symbolOop(classVars()->obj_at(index));
}

void mixinOopDesc::add_classVar(symbolOop name) {
  objArrayOop old_array = classVars();
  // Find out if it already exists.
  for (int index = 1; index <= old_array->length(); index++) {
    symbolOop elem = symbolOop(old_array->obj_at(index));
    if (elem == name) return;
  }
  // Extend the array
  set_classVars(old_array->copy_add(name));
}

symbolOop mixinOopDesc::remove_classVar_at(int index) {
  symbolOop name = classVar_at(index);
  set_classVars(classVars()->copy_remove(index));
  return name;
}


bool mixinOopDesc::includes_classVar(symbolOop name) {
  objArrayOop array = classVars();
  for (int index = 1; index <= array->length(); index++) {
    symbolOop elem = symbolOop(array->obj_at(index));
    if (elem == name) return true;
  }
  return false;
}

bool mixinOopDesc::is_installed() const {
  if (installed() == trueObj) return true;
  assert(installed() == falseObj, "verify installed");
  return false;
}

bool mixinOopDesc::has_primary_invocation() const {
  return primary_invocation()->is_klass();
}

void mixinOopDesc::apply_mixin(mixinOop m) {
  set_methods(m->methods());
}

void mixinOopDesc::customize_for(klassOop klass) {
  objArrayOop array = methods();
  for (int index = 1; index <= array->length(); index++) {
    methodOop m = methodOop(array->obj_at(index));
    m->customize_for(klass, this);
  }
}

void mixinOopDesc::uncustomize_methods() {
  objArrayOop array = methods();
  for (int index = 1; index <= array->length(); index++) {
    methodOop m = methodOop(array->obj_at(index));
    m->uncustomize_for(this);
  }
}
