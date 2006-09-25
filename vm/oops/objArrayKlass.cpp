/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.35 $ */
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
# include "incls/_objArrayKlass.cpp.incl"

oop objArrayKlass::allocateObjectSize(int size) {
  klassOop k        = as_klassOop();
  int      ni_size  = non_indexable_size();
  int      obj_size = ni_size + 1 + size;
  // allocate
  objArrayOop obj = as_objArrayOop(Universe::allocate(obj_size, (memOop*)&k));
  // header
  memOop(obj)->initialize_header(has_untagged_contents(), k);
  // instance variables
  memOop(obj)->initialize_body(memOopDesc::header_size(), ni_size);
  // indexables
  oop* base = (oop*) obj->addr();
  oop* end  = base + obj_size;
  // %optimized 'obj->set_length(size)'
  base[ni_size] = as_smiOop(size);
  memOop(obj)->initialize_body(ni_size+1, obj_size);
  return obj;
}

klassOop objArrayKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == weakArray_klass) return weakArrayKlass::create_class(as_klassOop(), mixin);
  if (format == mem_klass || format == objArray_klass) {
    return objArrayKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop objArrayKlass::create_class(klassOop super_class, mixinOop mixin) {
  objArrayKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

objArrayOop objArrayKlass::allocate_tenured_pic(int size) {
  klassOop k        = Universe::objArrayKlassObj();
  int      ni_size  = k->klass_part()->non_indexable_size();
  int      obj_size = ni_size + 1 + size;
  // allocate
  objArrayOop obj = as_objArrayOop(Universe::allocate_tenured(obj_size));
  // header
  memOop(obj)->initialize_header(k->klass_part()->has_untagged_contents(), k);
  // instance variables
  memOop(obj)->initialize_body(memOopDesc::header_size(), ni_size);
    // indexables
  oop* base = (oop*) obj->addr();
  oop* end  = base + obj_size;
  // %optimized 'obj->set_length(size)'
  base[ni_size] = as_smiOop(size);
  memOop(obj)->initialize_body(ni_size+1, obj_size);
  return obj;
}

void set_objArrayKlass_vtbl(Klass* k) {
  objArrayKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

void objArrayKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // Retrieve length information in case the iterator mutates the object
  oop* p   = objArrayOop(obj)->objs(0);
  int  len = objArrayOop(obj)->length();
  // header + instance variables
  memOopKlass::oop_layout_iterate(obj, blk);
  // indexables
  blk->do_oop("length", p++);
  blk->begin_indexables();
  for (int index = 1; index <= len; index++) {
    blk->do_indexable_oop(index, p++);
  }
  blk->end_indexables();
}

void objArrayKlass::oop_short_print_on(oop obj, outputStream* st) {
  const int MaxPrintLen = 255;	// to prevent excessive output -Urs
  assert_objArray(obj,"Argument must be objArray");
  objArrayOop array = objArrayOop(obj);
  int len = array->length();
  int n   = min(MaxElementPrintSize, len);
  st->print("'");
  for(int index = 1; index <= n && st->position() < MaxPrintLen; index++) {
    array->obj_at(index)->print_value_on(st);
    st->print(", ");
  }
  if (n < len) st->print("... ");
  else         st->print("' ");
  oop_print_value_on(obj, st);
}

void objArrayKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // Retrieve length information in case the iterator mutates the object
  oop* p   = objArrayOop(obj)->objs(0);
  int  len = objArrayOop(obj)->length();
  // header + instance variables
  memOopKlass::oop_oop_iterate(obj, blk);
  // indexables
  blk->do_oop(p++);
  for (int index = 1; index <= len; index++) {
    blk->do_oop(p++);
  }
}

int objArrayKlass::oop_scavenge_contents(oop obj) {
  // header + instance variables
  memOopKlass::oop_scavenge_contents(obj);
  // indexables
  objArrayOop o = objArrayOop(obj);
  oop* base = o->objs(1);
  oop* end  = base + o->length();
  while (base < end) { scavenge_oop(base++); }
  return object_size(o->length());  
}

int objArrayKlass::oop_scavenge_tenured_contents(oop obj) {
  // header + instance variables
  memOopKlass::oop_scavenge_tenured_contents(obj);
  // indexables
  objArrayOop o = objArrayOop(obj);
  oop* base = o->objs(1);
  oop* end  = base + o->length();
  while (base < end) scavenge_tenured_oop(base++);
  return object_size(o->length());  
}

void objArrayKlass::oop_follow_contents(oop obj) {
  // Retrieve length information since header information  mutates the object
  oop* base = objArrayOop(obj)->objs(1);
  oop* end  = base + objArrayOop(obj)->length();

  // header + instance variables
  memOopKlass::oop_follow_contents(obj);
  // indexables
  while (base < end) MarkSweep::reverse_and_push(base++);
}
