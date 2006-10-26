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
# include "incls/_mixinKlass.cpp.incl"

void set_mixinKlass_vtbl(Klass* k) {
  mixinKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop mixinKlass::allocateObject() {
  klassOop k    = as_klassOop();
  int      size = non_indexable_size();
  // allocate
  mixinOop obj = as_mixinOop(Universe::allocate(size, (memOop*)&k));
  // header + instance variables
  memOop(obj)->initialize_header(true, k);
  memOop(obj)->initialize_body(memOopDesc::header_size(), size);
  objArrayOop filler = oopFactory::new_objArray(0);
  obj->set_methods(filler);
  obj->set_instVars(filler);
  obj->set_classVars(filler);
  obj->set_installed(falseObj);
  return obj;
}

klassOop mixinKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == mixin_klass) {
    return mixinKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop mixinKlass::create_class(klassOop super_class, mixinOop mixin) {
  mixinKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

oop mixinKlass::oop_shallow_copy(oop obj, bool tenured) {
  mixinOop copy = mixinOop(memOopKlass::oop_shallow_copy(obj, tenured));
  copy->set_installed(falseObj);
  return copy;
}

int mixinKlass::oop_scavenge_contents(oop obj) {
  int size = non_indexable_size();
  // header + instance variables
  memOop(obj)->scavenge_header();
  memOop(obj)->scavenge_body(memOopDesc::header_size(), size);
  return size;  
}

int mixinKlass::oop_scavenge_tenured_contents(oop obj) {
  int size = non_indexable_size();
  // header + instance variables
  memOop(obj)->scavenge_tenured_header();
  memOop(obj)->scavenge_tenured_body(memOopDesc::header_size(), size);
  return size;
}

void mixinKlass::oop_follow_contents(oop obj) {
  // header + instance variables
  memOop(obj)->follow_header();
  memOop(obj)->follow_body(memOopDesc::header_size(), non_indexable_size());
}

void mixinKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  blk->do_oop("methods",            (oop*)&mixinOop(obj)->addr()->_methods);
  blk->do_oop("instVars",           (oop*)&mixinOop(obj)->addr()->_inst_vars);
  blk->do_oop("classVars",          (oop*)&mixinOop(obj)->addr()->_class_vars);
  blk->do_oop("primary invocation", (oop*)&mixinOop(obj)->addr()->_primary_invocation);
  blk->do_oop("class mixin",        (oop*)&mixinOop(obj)->addr()->_class_mixin);
  // instance variables
  memOop(obj)->layout_iterate_body(blk, mixinOopDesc::header_size(), non_indexable_size());
}

void mixinKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header + instance variables
  memOop(obj)->oop_iterate_header(blk);
  memOop(obj)->oop_iterate_body(blk, memOopDesc::header_size(), non_indexable_size());
}
