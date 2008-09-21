/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.14 $ */
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
# include "incls/_proxyKlass.cpp.incl"

void set_proxyKlass_vtbl(Klass* k) {
  proxyKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop proxyKlass::allocateObject(bool permit_scavenge) {
  klassOop k    = as_klassOop();
  int      size = non_indexable_size();
  // allocate
  proxyOop obj = as_proxyOop(Universe::allocate(size, (memOop*)&k));
  // header
  memOop(obj)->initialize_header(true, k);
  obj->set_pointer(NULL);
  // instance variables
  memOop(obj)->initialize_body(proxyOopDesc::header_size(), size);
  return obj;
}

klassOop proxyKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == proxy_klass) {
    return proxyKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop proxyKlass::create_class(klassOop super_class, mixinOop mixin) {
  proxyKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

int proxyKlass::oop_scavenge_contents(oop obj) {
  int size = non_indexable_size();
  // header
  memOop(obj)->scavenge_header();
  // instance variables
  memOop(obj)->scavenge_body(proxyOopDesc::header_size(), size);
  return size;  
}

int proxyKlass::oop_scavenge_tenured_contents(oop obj) {
  int size = non_indexable_size();
  // header
  memOop(obj)->scavenge_tenured_header();
  // instance variables
  memOop(obj)->scavenge_tenured_body(proxyOopDesc::header_size(), size);
  return size;
}

void proxyKlass::oop_follow_contents(oop obj) {
  // header
  memOop(obj)->follow_header();
  // instance variables
  memOop(obj)->follow_body(proxyOopDesc::header_size(), non_indexable_size());
}

void proxyKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  blk->do_long("pointer", &proxyOop(obj)->addr()->_pointer);
  // instance variables
  memOop(obj)->layout_iterate_body(blk, proxyOopDesc::header_size(), non_indexable_size());
}

void proxyKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  // instance variables
  memOop(obj)->oop_iterate_body(blk, proxyOopDesc::header_size(), non_indexable_size());
}
