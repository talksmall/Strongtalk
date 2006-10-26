/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.37 $ */
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
# include "incls/_memOopKlass.cpp.incl"

void set_memOopKlass_vtbl(Klass* k) {
  memOopKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

int memOopKlass::oop_scavenge_contents(oop obj) {
  int size = non_indexable_size();
  // header
  memOop(obj)->scavenge_header();
  // instance variables
  memOop(obj)->scavenge_body(memOopDesc::header_size(), size);
  return size;
}

int memOopKlass::oop_scavenge_tenured_contents(oop obj) {
  int size = non_indexable_size();
  // header
  memOop(obj)->scavenge_tenured_header();
  // instance variables
  memOop(obj)->scavenge_tenured_body(memOopDesc::header_size(), size);
  return size;
}

void memOopKlass::oop_follow_contents(oop obj) {
  // header
  memOop(obj)->follow_header();
  // instance variables
  memOop(obj)->follow_body(memOopDesc::header_size(), non_indexable_size());
}

bool memOopKlass::oop_verify(oop obj) {
  return Universe::verify_oop(memOop(obj));
}

void memOopKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  // instance variables
  memOop(obj)->layout_iterate_body(blk, memOopDesc::header_size(), non_indexable_size());
}

void memOopKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  // instance variables
  memOop(obj)->oop_iterate_body(blk, memOopDesc::header_size(), non_indexable_size());
}

void memOopKlass::oop_print_on(oop obj, outputStream* st) {
  PrintObjectClosure blk(st);
  blk.do_object(memOop(obj));
  memOop(obj)->layout_iterate(&blk);
}

void memOopKlass::oop_print_value_on(oop obj, outputStream* st) {
       if (obj == nilObj)   st->print("nil");
  else if (obj == trueObj)  st->print("true");
  else if (obj == falseObj) st->print("false");
  else {
    if (PrintObjectID) {
      memOop(obj)->print_id_on(st);
      st->print("-");
    }
    print_name_on(st);
  }
  if (PrintOopAddress) st->print(" (%#x)", this);
}

oop memOopKlass::allocateObject() {
  klassOop k    = as_klassOop();
  int      size = non_indexable_size();
  // allocate
  memOop obj = as_memOop(Universe::allocate(size, (memOop*)&k));
  // header
  obj->initialize_header(has_untagged_contents(), k);
  // instance variables
  obj->initialize_body(memOopDesc::header_size(), size);
  return obj;
}

oop memOopKlass::allocateObjectSize(int size) {
  return markSymbol(vmSymbols::not_indexable());
}

klassOop memOopKlass::create_subclass(mixinOop mixin, Format format) {
  assert(can_be_subclassed(), "must be able to subclass this");
  if (format == mem_klass)             return memOopKlass::create_class(as_klassOop(), mixin);

  if (format == objArray_klass)        return objArrayKlass::create_class(as_klassOop(), mixin);
  if (format == byteArray_klass)       return byteArrayKlass::create_class(as_klassOop(), mixin);
  if (format == doubleByteArray_klass) return doubleByteArrayKlass::create_class(as_klassOop(), mixin);
  if (format == weakArray_klass)       return weakArrayKlass::create_class(as_klassOop(), mixin);

  if (number_of_instance_variables() > 0) {
    warning("super class has instance variables when mixing in special mixin");
    return NULL;
  }

  if (format == mixin_klass)           return mixinKlass::create_class(as_klassOop(), mixin);
  if (format == proxy_klass)           return proxyKlass::create_class(as_klassOop(), mixin);
  if (format == process_klass)         return processKlass::create_class(as_klassOop(), mixin);
  return NULL;
}

klassOop memOopKlass::create_class(klassOop super_class, mixinOop mixin) {
  memOopKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

oop memOopKlass::oop_shallow_copy(oop obj, bool tenured) {
  // Do not copy oddballs (nil, true, false)
  if (obj == nilObj)   return obj;
  if (obj == trueObj)  return obj;
  if (obj == falseObj) return obj;

  int  len   = memOop(obj)->size();
  // Important to preserve obj (in case of scavenge).
  oop* clone = tenured ? Universe::allocate_tenured(len) : Universe::allocate(len, (memOop*)&obj);
  oop* to    = clone;
  oop* from  = (oop*) memOop(obj)->addr();
  oop* end   = to + len;
  while (to < end) *to++ = *from++;

  if (!as_memOop(clone)->is_new()) {
    // Remember to update the remembered set if the clone is in old space.
    // Note:
    //   should we do something special for arrays.
    Universe::remembered_set->record_store(clone);
  }
  return as_memOop(clone);
}
