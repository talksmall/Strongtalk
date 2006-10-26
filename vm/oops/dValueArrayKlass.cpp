/* Copyright 1996 LongView Technologies L.L.C. $Revision: 1.1 $ */
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
# include "incls/_dValueArrayKlass.cpp.incl"
# include <ctype.h>

oop doubleValueArrayKlass::allocateObject() {
  fatal("should never call allocateObject in doubleValueArrayKlass");
  return badOop;
}

oop doubleValueArrayKlass::allocateObjectSize(int size) {
  klassOop k        = as_klassOop();
  int      ni_size  = non_indexable_size();
  int      obj_size = ni_size + 1 + roundTo(size * sizeof(double), oopSize) / oopSize;
  // allocate
  doubleValueArrayOop obj = as_doubleValueArrayOop(Universe::allocate(obj_size, (memOop*)&k));
  // header
  memOop(obj)->initialize_header(true, k);
  // instance variables
  memOop(obj)->initialize_body(memOopDesc::header_size(), ni_size);
  // indexables
  obj->set_length(size);
  for (int index = 1; index <= size; index++) {
    obj->double_at_put(index, 0.0);
  }
  return obj;
}

klassOop doubleValueArrayKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == doubleValueArray_klass) {
    return doubleValueArrayKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop doubleValueArrayKlass::create_class(klassOop super_class, mixinOop mixin) {
  doubleValueArrayKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

void set_doubleValueArrayKlass_vtbl(Klass* k) {
  doubleValueArrayKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

bool doubleValueArrayKlass::oop_verify(oop obj) {
  assert_doubleValueArray(obj,"Argument must be doubleValueArray");
  return doubleValueArrayOop(obj)->verify();
}

void doubleValueArrayKlass::oop_print_value_on(oop obj, outputStream* st) {
  assert_doubleValueArray(obj,"Argument must be doubleValueArray");
  doubleValueArrayOop array = doubleValueArrayOop(obj);
  int len = array->length();
  int n   = min(MaxElementPrintSize, len);
  Unimplemented();
}

void doubleValueArrayKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  double* p       = doubleValueArrayOop(obj)->double_start();
  oop*        l   = doubleValueArrayOop(obj)->length_addr();
  int         len = doubleValueArrayOop(obj)->length();
  memOopKlass::oop_layout_iterate(obj, blk);
  blk->do_oop("length", l);
  blk->begin_indexables();
  Unimplemented();
  blk->end_indexables();
}

void doubleValueArrayKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  oop* l = doubleValueArrayOop(obj)->length_addr();
  memOopKlass::oop_oop_iterate(obj, blk);
  blk->do_oop(l);
}

int doubleValueArrayKlass::oop_scavenge_contents(oop obj) {
  memOopKlass::oop_scavenge_contents(obj);
  return object_size(doubleValueArrayOop(obj)->length());  
}

int doubleValueArrayKlass::oop_scavenge_tenured_contents(oop obj) {
  memOopKlass::oop_scavenge_tenured_contents(obj);
  return object_size(doubleValueArrayOop(obj)->length());  
}

