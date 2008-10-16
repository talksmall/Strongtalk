/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.31 $ */
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
# include "incls/_byteArrayKlass.cpp.incl"
# include <ctype.h>

oop byteArrayKlass::allocateObject(bool permit_scavenge) {
  assert(!can_inline_allocation(), "using nonstandard allocation");
  // This should not be fatal! This could be called erroneously from ST in which
  // case it should result in an ST level error. Instead return a marked symbol
  // to indicate the failure.
  //fatal("should never call allocateObject in byteArrayKlass");
  return markSymbol(vmSymbols::invalid_klass());
}

oop byteArrayKlass::allocateObjectSize(int size, bool permit_scavenge, bool permit_tenured) {
  klassOop k        = as_klassOop();
  int      ni_size  = non_indexable_size();
  int      obj_size = ni_size + 1 + roundTo(size, oopSize) / oopSize;
  // allocate
  void* chunk = Universe::allocate(obj_size, (memOop*)&k, permit_scavenge);
  if (chunk == NULL && permit_tenured) {
    chunk = Universe::allocate_tenured(obj_size, false);
  }
  if (chunk == NULL) return markSymbol(vmSymbols::failed_allocation());

  byteArrayOop obj = as_byteArrayOop(chunk);
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
  //               obj->byte_at_put(index, '\000')'
  base = &base[ni_size+1];
  while (base < end) *base++ = (oop) 0;
  return obj;
}

klassOop byteArrayKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == byteArray_klass) {
    return byteArrayKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop byteArrayKlass::create_class(klassOop super_class, mixinOop mixin) {
  byteArrayKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

void byteArrayKlass::initialize_object(byteArrayOop obj, char* value, int len){
  for (int index = 1; index <= len; index++) {
    obj->byte_at_put(index, value[index-1]);
  }
}

void set_byteArrayKlass_vtbl(Klass* k) {
  byteArrayKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

bool byteArrayKlass::oop_verify(oop obj) {
  assert_byteArray(obj,"Argument must be byteArray");
  return byteArrayOop(obj)->verify();
}

void byteArrayKlass::oop_print_value_on(oop obj, outputStream* st) {
  assert_byteArray(obj,"Argument must be byteArray");
  byteArrayOop array = byteArrayOop(obj);
  int len = array->length();
  int n   = min(MaxElementPrintSize, len);
  st->print("'");
  for(int index = 1; index <= n; index++) {
    char c = array->byte_at(index);
    if (isprint(c)) st->print("%c",   c);
    else            st->print("\\%o", c);
  }
  if (n < len) st->print("...");
  st->print("'");
}

void byteArrayKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  u_char* p   = byteArrayOop(obj)->bytes();
  oop*  l   = byteArrayOop(obj)->length_addr();
  int   len = byteArrayOop(obj)->length();
  // header + instance variables
  memOopKlass::oop_layout_iterate(obj, blk);
  // indexables
  blk->begin_indexables();
  blk->do_oop("length", l);
  for (int index = 1; index <= len; index++) {
    blk->do_indexable_byte(index, p++);
  }
  blk->end_indexables();
}

void byteArrayKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  oop* l = byteArrayOop(obj)->length_addr();
  // header + instance variables
  memOopKlass::oop_oop_iterate(obj, blk);
  blk->do_oop(l);
}

int byteArrayKlass::oop_scavenge_contents(oop obj) {
  // header + instance variables
  memOopKlass::oop_scavenge_contents(obj);
  return object_size(byteArrayOop(obj)->length());  
}

int byteArrayKlass::oop_scavenge_tenured_contents(oop obj) {
  // header + instance variables
  memOopKlass::oop_scavenge_tenured_contents(obj);
  return object_size(byteArrayOop(obj)->length());  
}
