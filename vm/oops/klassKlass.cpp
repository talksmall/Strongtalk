/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.34 $ */
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
# include "incls/_klassKlass.cpp.incl"

void set_klassKlass_vtbl(Klass* k) {
  klassKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop klassKlass::allocateObject() {
  // allocate
  memOop obj = as_memOop(Universe::allocate_tenured(non_indexable_size()));
  // header + instance variables
  obj->initialize_header(has_untagged_contents(), as_klassOop());
  obj->initialize_body(klassOopDesc::header_size(), non_indexable_size());
  return obj;
}

klassOop klassKlass::create_subclass(mixinOop mixin, Format format) {
  return NULL;
}

int klassKlass::oop_scavenge_contents(oop obj) {
  int size = non_indexable_size();
  // header
  memOop(obj)->scavenge_header();
  Klass* k = klassOop(obj)->klass_part();
  scavenge_oop((oop*) &k->_non_indexable_size);
  scavenge_oop((oop*) &k->_has_untagged_contents);
  scavenge_oop((oop*) &k->_classVars);
  scavenge_oop((oop*) &k->_methods);
  scavenge_oop((oop*) &k->_superKlass);
  scavenge_oop((oop*) &k->_mixin);
  // instance variables
  memOop(obj)->scavenge_body(klassOopDesc::header_size(), size);
  return size;
}

int klassKlass::oop_scavenge_tenured_contents(oop obj) {
  int size = non_indexable_size();
  // header
  memOop(obj)->scavenge_tenured_header();
  Klass* k = klassOop(obj)->klass_part();
  scavenge_tenured_oop((oop*) &k->_non_indexable_size);
  scavenge_tenured_oop((oop*) &k->_has_untagged_contents);
  scavenge_tenured_oop((oop*) &k->_classVars);
  scavenge_tenured_oop((oop*) &k->_methods);
  scavenge_tenured_oop((oop*) &k->_superKlass);
  scavenge_tenured_oop((oop*) &k->_mixin);
  // instance variables
  memOop(obj)->scavenge_tenured_body(klassOopDesc::header_size(), size);
  return size;
}

void klassKlass::oop_follow_contents(oop obj) {
  // header
  memOop(obj)->follow_header();
  Klass* k = klassOop(obj)->klass_part();
  MarkSweep::reverse_and_push((oop*) &k->_non_indexable_size);
  MarkSweep::reverse_and_push((oop*) &k->_has_untagged_contents);
  MarkSweep::reverse_and_push((oop*) &k->_classVars);
  MarkSweep::reverse_and_push((oop*) &k->_methods);
  MarkSweep::reverse_and_push((oop*) &k->_superKlass);
  MarkSweep::reverse_and_push((oop*) &k->_mixin);
  // instance variables
  memOop(obj)->follow_body(klassOopDesc::header_size(), non_indexable_size());
}

bool klassKlass::oop_verify(oop obj) {
  assert(obj->is_klass(), "must be class");
  Klass* k = klassOop(obj)->klass_part();
  /*
  if (!k->oop_is_smi()) {
    int a = k->non_indexable_size();
    int b = k->oop_header_size();
    int c = k->number_of_instance_variables();
    if (a != b + c) {
      warning("inconsistent non indexable size");
    }
  }
  */
  bool flag = memOop(obj)->verify();
  return flag;
}

void klassKlass::oop_print_on(oop obj, outputStream* st) {
  memOopKlass::oop_print_on(obj, st);
}

void klassKlass::oop_print_value_on(oop obj, outputStream* st) {
  if (PrintObjectID) {
    memOop(obj)->print_id_on(st);
    st->print("-");
  }
  klassOop(obj)->klass_part()->print_name_on(st);
  st->print(" class");
}

void klassKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  Klass* k = klassOop(obj)->klass_part();
  blk->do_oop("size",      (oop*) &k->_non_indexable_size);
  blk->do_oop("untag",     (oop*) &k->_has_untagged_contents);
  blk->do_oop("classVars", (oop*) &k->_classVars);
  blk->do_oop("methods",   (oop*) &k->_methods);
  blk->do_oop("super",     (oop*) &k->_superKlass);
  blk->do_oop("mixin",     (oop*) &k->_mixin);
  // instance variables
  memOop(obj)->layout_iterate_body(blk, klassOopDesc::header_size(), non_indexable_size());
}

void klassKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  Klass* k = klassOop(obj)->klass_part();
  blk->do_oop((oop*) &k->_non_indexable_size);
  blk->do_oop((oop*) &k->_has_untagged_contents);
  blk->do_oop((oop*) &k->_classVars);
  blk->do_oop((oop*) &k->_methods);
  blk->do_oop((oop*) &k->_superKlass);
  blk->do_oop((oop*) &k->_mixin);
  // instance variables
  memOop(obj)->oop_iterate_body(blk, klassOopDesc::header_size(), non_indexable_size());
}

oop klassKlass::oop_primitive_allocate(oop obj) {
  return klassOop(obj)->klass_part()->allocateObject();
}

oop klassKlass::oop_primitive_allocate_size(oop obj, int size) {
  return klassOop(obj)->klass_part()->allocateObjectSize(size);
}

oop klassKlass::oop_shallow_copy(oop obj, bool tenured) {
  return markSymbol(vmSymbols::not_clonable());
}
