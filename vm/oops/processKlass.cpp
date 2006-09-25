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
# include "incls/_processKlass.cpp.incl"

void set_processKlass_vtbl(Klass* k) {
  processKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop processKlass::allocateProcess(int stack_size) {
  return allocateObject();
}

oop processKlass::allocateObject() {
  klassOop k    = as_klassOop();
  int      size = non_indexable_size();
  // allocate
  processOop obj = as_processOop(Universe::allocate(size, (memOop*)&k));
  // header
  memOop(obj)->initialize_header(true, k);
  obj->set_process(NULL);
  // instance variables
  memOop(obj)->initialize_body(processOopDesc::header_size(), size);
  return obj;
}

klassOop processKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == process_klass) {
    return processKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop processKlass::create_class(klassOop super_class, mixinOop mixin) {
  processKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

int processKlass::oop_scavenge_contents(oop obj) {
  int size = non_indexable_size();
  // header
  memOop(obj)->scavenge_header();
  // instance variables
  memOop(obj)->scavenge_body(processOopDesc::header_size(), size);
  return size;  
}

int processKlass::oop_scavenge_tenured_contents(oop obj) {
  int size = non_indexable_size();
  // header
  memOop(obj)->scavenge_tenured_header();
  // instance variables
  memOop(obj)->scavenge_tenured_body(processOopDesc::header_size(), size);
  return size;
}

void processKlass::oop_follow_contents(oop obj) {
  // header
  memOop(obj)->follow_header();
  // instance variables
  memOop(obj)->follow_body(processOopDesc::header_size(), non_indexable_size());
}

void processKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  blk->do_long("process", (void**) &processOop(obj)->addr()->_process);
  // instance variables
  memOop(obj)->layout_iterate_body(blk, processOopDesc::header_size(), non_indexable_size());
}

void processKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  // instance variables
  memOop(obj)->oop_iterate_body(blk, processOopDesc::header_size(), non_indexable_size());
}
