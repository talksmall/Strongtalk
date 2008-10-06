/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.8 $ */
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
# include "incls/_vframeKlass.cpp.incl"

void set_vframeKlass_vtbl(Klass* k) {
  vframeKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop vframeKlass::allocateObject(bool permit_scavenge) {
  klassOop k    = as_klassOop();
  int      size = non_indexable_size();
  // allocate
  vframeOop obj = as_vframeOop(Universe::allocate(size, (memOop*)&k, permit_scavenge));
  // header
  memOop(obj)->initialize_header(true, k);
 
  obj->set_process(processOop(nilObj));
  obj->set_index(0);
  obj->set_time_stamp(1);

  // instance variables
  memOop(obj)->initialize_body(vframeOopDesc::header_size(), size);
  return obj;
}

klassOop vframeKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == vframe_klass) {
    return vframeKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop vframeKlass::create_class(klassOop super_class, mixinOop mixin) {
  vframeKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

int vframeKlass::oop_scavenge_contents(oop obj) {
  int size = non_indexable_size();
  // header + instance variables
  memOop(obj)->scavenge_header();
  memOop(obj)->scavenge_body(memOopDesc::header_size(), size);
  return size;  
}

int vframeKlass::oop_scavenge_tenured_contents(oop obj) {
  int size = non_indexable_size();
  // header + instance variables
  memOop(obj)->scavenge_tenured_header();
  memOop(obj)->scavenge_tenured_body(memOopDesc::header_size(), size);
  return size;
}

void vframeKlass::oop_follow_contents(oop obj) {
  // header + instance variables
  memOop(obj)->follow_header();
  memOop(obj)->follow_body(memOopDesc::header_size(), non_indexable_size());
}

void vframeKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  vframeOop vf = vframeOop(obj);
  blk->do_oop("process",    (oop*) &vf->addr()->_process);
  blk->do_oop("index",      (oop*) &vf->addr()->_index);
  blk->do_oop("time stamp", (oop*) &vf->addr()->_time_stamp);
  memOop(obj)->layout_iterate_body(blk, vframeOopDesc::header_size(), non_indexable_size());
}

void vframeKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  vframeOop vf = vframeOop(obj);
  blk->do_oop((oop*) &vf->addr()->_process);
  blk->do_oop((oop*) &vf->addr()->_index);
  blk->do_oop((oop*) &vf->addr()->_time_stamp);
  // instance variables
  memOop(obj)->oop_iterate_body(blk, vframeOopDesc::header_size(), non_indexable_size());
}
