/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.24 $ */
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
# include "incls/_associationKlass.cpp.incl"

void set_associationKlass_vtbl(Klass* k) {
  associationKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop associationKlass::allocateObject() {
  klassOop k    = as_klassOop();
  int      size = non_indexable_size();
  // allocate
  associationOop obj = as_associationOop(Universe::allocate_tenured(size));
  // header
  memOop(obj)->initialize_header(has_untagged_contents(), k);
  obj->set_key(symbolOop(nilObj));
  obj->set_value(nilObj);
  obj->set_is_constant(false);
  // instance variables
  obj->initialize_body(associationOopDesc::header_size(), size);
  return obj;
}

klassOop associationKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == association_klass) {
    return associationKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop associationKlass::create_class(klassOop super_class, mixinOop mixin) {
  associationKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

bool associationKlass::oop_verify(oop obj) {
  bool flag = memOop(obj)->verify();
  return flag;
}

void associationKlass::oop_short_print_on(oop obj, outputStream* st) {
  associationOop assoc = associationOop(obj);
  st->print("{"); 
  assoc->key()->print_symbol_on(st);
  st->print(", "); 
  assoc->value()->print_value_on(st);
  if (assoc->is_constant()) {
    st->print(" (constant)");
  }
  st->print("} "); 
}

void associationKlass::oop_print_value_on(oop obj, outputStream* st) {
  associationOop assoc = associationOop(obj);
  if (PrintObjectID) {
    memOop(obj)->print_id_on(st);
    st->print("-");
  }
  print_name_on(st);
  st->print(" {"); 
  assoc->key()->print_symbol_on(st);
  st->print(", "); 
  assoc->value()->print_value_on(st);
  if (assoc->is_constant()) {
    st->print(" (constant)");
  }
  st->print("}");
  if (PrintOopAddress) st->print(" (%#x)", this);
}

int associationKlass::oop_scavenge_contents(oop obj) {
  int size = non_indexable_size();
  // header + instance variables
  memOop(obj)->scavenge_header();
  memOop(obj)->scavenge_body(memOopDesc::header_size(), size);
  return size;  
}

int associationKlass::oop_scavenge_tenured_contents(oop obj) {
  int size = non_indexable_size();
  // header + instance variables
  memOop(obj)->scavenge_tenured_header();
  memOop(obj)->scavenge_tenured_body(memOopDesc::header_size(), size);
  return size;
}

void associationKlass::oop_follow_contents(oop obj) {
  // header + instance variables
  memOop(obj)->follow_header();
  memOop(obj)->follow_body(memOopDesc::header_size(), non_indexable_size());
}

void associationKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  associationOop assoc = associationOop(obj);
  blk->do_oop("key",         (oop*) &assoc->addr()->_key);
  blk->do_oop("value",       (oop*) &assoc->addr()->_value);
  blk->do_oop("is_constant", (oop*) &assoc->addr()->_is_constant);
  // instance variables
  memOop(obj)->layout_iterate_body(blk, associationOopDesc::header_size(), non_indexable_size());
}

void associationKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  associationOop assoc = associationOop(obj);
  blk->do_oop((oop*) &assoc->addr()->_key);
  blk->do_oop((oop*) &assoc->addr()->_value);
  blk->do_oop((oop*) &assoc->addr()->_is_constant);
  // instance variables
  memOop(obj)->oop_iterate_body(blk, associationOopDesc::header_size(), non_indexable_size());
}
