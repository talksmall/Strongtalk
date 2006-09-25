/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.17 $ */
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
# include "incls/_dByteArrayKlass.cpp.incl"
# include <ctype.h>

oop doubleByteArrayKlass::allocateObject() {
  fatal("should never call allocateObject in doubleByteArrayKlass");
  return badOop;
}

oop doubleByteArrayKlass::allocateObjectSize(int size) {
  klassOop k        = as_klassOop();
  int      ni_size  = non_indexable_size();
  int      obj_size = ni_size + 1 + roundTo(size * 2, oopSize) / oopSize;
  // allocate
  doubleByteArrayOop obj = as_doubleByteArrayOop(Universe::allocate(obj_size, (memOop*)&k));
  // header
  memOop(obj)->initialize_header(true, k);
  // instance variables
  memOop(obj)->initialize_body(memOopDesc::header_size(), ni_size);
  // indexables
  oop* base = (oop*) obj->addr();
  oop* end  = base + obj_size;
  // %optimized 'obj->set_length(size)'
  base[ni_size] = as_smiOop(size);
  // %optimized 'for (int index = 1; index <= size; index++)
  //               obj->doubleByte_at_put(index, 0)'
  base = &base[ni_size+1];
  while (base < end) *base++ = (oop) 0;
  return obj;
}

klassOop doubleByteArrayKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == doubleByteArray_klass) {
    return doubleByteArrayKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop doubleByteArrayKlass::create_class(klassOop super_class, mixinOop mixin) {
  doubleByteArrayKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

void set_doubleByteArrayKlass_vtbl(Klass* k) {
  doubleByteArrayKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

bool doubleByteArrayKlass::oop_verify(oop obj) {
  assert_doubleByteArray(obj,"Argument must be doubleByteArray");
  return doubleByteArrayOop(obj)->verify();
}

void doubleByteArrayKlass::oop_print_value_on(oop obj, outputStream* st) {
  assert_doubleByteArray(obj,"Argument must be doubleByteArray");
  doubleByteArrayOop array = doubleByteArrayOop(obj);
  int len = array->length();
  int n   = min(MaxElementPrintSize, len);
  st->print("'");
  for(int index = 1; index <= n; index++) {
    int c = array->doubleByte_at(index);
    if (isprint(c)) st->print("%c",   c);
    else            st->print("\\%o", c);
  }
  if (n < len) st->print("...");
  st->print("'");
}

void doubleByteArrayKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  doubleByte* p   = doubleByteArrayOop(obj)->doubleBytes();
  oop*        l   = doubleByteArrayOop(obj)->length_addr();
  int         len = doubleByteArrayOop(obj)->length();
  memOopKlass::oop_layout_iterate(obj, blk);
  blk->do_oop("length", l);
  blk->begin_indexables();
  for (int index = 1; index <= len; index++) {
    blk->do_indexable_doubleByte(index, p++);
  }
  blk->end_indexables();
}

void doubleByteArrayKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  oop* l = doubleByteArrayOop(obj)->length_addr();
  memOopKlass::oop_oop_iterate(obj, blk);
  blk->do_oop(l);
}

int doubleByteArrayKlass::oop_scavenge_contents(oop obj) {
  memOopKlass::oop_scavenge_contents(obj);
  return object_size(doubleByteArrayOop(obj)->length());  
}

int doubleByteArrayKlass::oop_scavenge_tenured_contents(oop obj) {
  memOopKlass::oop_scavenge_tenured_contents(obj);
  return object_size(doubleByteArrayOop(obj)->length());  
}

