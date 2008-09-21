/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.7 $ */
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
# include "incls/_weakArrayKlass.cpp.incl"

klassOop weakArrayKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == weakArray_klass) {
    return weakArrayKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop weakArrayKlass::create_class(klassOop super_class, mixinOop mixin) {
  weakArrayKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

void set_weakArrayKlass_vtbl(Klass* k) {
  weakArrayKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

void weakArrayKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
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

void weakArrayKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // Retrieve length information in case the iterator mutates the object
  oop* p   = weakArrayOop(obj)->objs(0);
  int  len = weakArrayOop(obj)->length();
  // header + instance variables
  memOopKlass::oop_oop_iterate(obj, blk);
  // indexables
  blk->do_oop(p++);
  for (int index = 1; index <= len; index++) {
    blk->do_oop(p++);
  }
}

int weakArrayKlass::oop_scavenge_contents(oop obj) {
  // header + instance variables
  memOopKlass::oop_scavenge_contents(obj);
  // indexables
  weakArrayOop o = weakArrayOop(obj);
  if (!WeakArrayRegister::scavenge_register(o)) {
    oop* base = o->objs(1);
    oop* end  = base + o->length();
    while (base <= end) { scavenge_oop(base++); }
  }
  return object_size(o->length());  
}

int weakArrayKlass::oop_scavenge_tenured_contents(oop obj) {
  // header + instance variables
  memOopKlass::oop_scavenge_tenured_contents(obj);
  // indexables
  weakArrayOop o = weakArrayOop(obj);
  if (!WeakArrayRegister::scavenge_register(o)){
    oop* base = o->objs(1);
    oop* end  = base + o->length();
    while (base <= end) scavenge_tenured_oop(base++);
  }
  return object_size(o->length());  
}

void weakArrayKlass::oop_follow_contents(oop obj) {
  // indexables
  if (!WeakArrayRegister::mark_sweep_register(weakArrayOop(obj), non_indexable_size())){
    oop* base = weakArrayOop(obj)->objs(1);
    oop* end  = base + weakArrayOop(obj)->length();
    while (base <= end) MarkSweep::reverse_and_follow(base++);
  } 

  // header + instance variables
  memOopKlass::oop_follow_contents(obj);
}

// WeakArrayRegister 
// - static variables
bool WeakArrayRegister::during_registration = false;
GrowableArray<weakArrayOop>* WeakArrayRegister::weakArrays = NULL;
GrowableArray<int>* WeakArrayRegister::nis                 = NULL;

// - Scavenge operations
void WeakArrayRegister::begin_scavenge() {
  during_registration = true;
  weakArrays          = new GrowableArray<weakArrayOop>(10); 
}

bool WeakArrayRegister::scavenge_register(weakArrayOop obj) {
  if (during_registration) weakArrays->push(obj);
  return during_registration;
}

void WeakArrayRegister::check_and_scavenge_contents() {
  scavenge_check_for_dying_objects();
  scavenge_contents();
  during_registration = false;
  weakArrays = NULL;
}

void WeakArrayRegister::scavenge_contents() {
  for(int index = 0; index < weakArrays->length(); index++)
    weakArrays->at(index)->scavenge_contents_after_registration(); 
}

inline bool WeakArrayRegister::scavenge_is_near_death(oop obj) {
  // must be memOop and unmarked (no forward pointer)
  return obj->is_new() && !memOop(obj)->is_forwarded();
}

void WeakArrayRegister::scavenge_check_for_dying_objects() {
  NotificationQueue::mark_elements();
  for(int index = 0; index < weakArrays->length(); index++) {
    weakArrayOop w = weakArrays->at(index);
    bool encounted_near_death_objects = false;
    for(int i = 1; i <= w->length(); i++) {
      oop obj = w->obj_at(i);
      if (scavenge_is_near_death(obj)) {
        encounted_near_death_objects = true;
        memOop(obj)->mark_as_dying();
      }
    }
    if (encounted_near_death_objects && !w->is_queued())
      NotificationQueue::put(w);
  }
  NotificationQueue::clear_elements();
}

// - Mark sweep operations

void WeakArrayRegister::begin_mark_sweep() {
  during_registration = true;
  weakArrays          = new GrowableArray<weakArrayOop>(100);
  nis                 = new GrowableArray<int>(100); 
}

bool WeakArrayRegister::mark_sweep_register(weakArrayOop obj, int non_indexable_size) {
  if (during_registration) {
    weakArrays->push(obj);
    nis->push(non_indexable_size);
  }
  return during_registration;
}

void WeakArrayRegister::check_and_follow_contents() {
  mark_sweep_check_for_dying_objects();
  follow_contents();
  during_registration = false;
  weakArrays = NULL;
  nis        = NULL;
}

void WeakArrayRegister::follow_contents() {
  for(int index = 0; index < weakArrays->length(); index++) {
    weakArrayOop w = weakArrays->at(index);
    int non_indexable_size = nis->at(index);
    bool encounted_near_death_objects = false;
    int length = smiOop(w->raw_at(non_indexable_size))->value();
    for(int i = 1; i <= length; i++) {
      MarkSweep::reverse_and_follow(w->oops(non_indexable_size+i));
    }
  }
}

inline bool WeakArrayRegister::mark_sweep_is_near_death(oop obj) {
  return obj->is_mem() && !memOop(obj)->is_gc_marked();
}

void WeakArrayRegister::mark_sweep_check_for_dying_objects() {
  for(int index = 0; index < weakArrays->length(); index++) {
    weakArrayOop w = weakArrays->at(index);
    int non_indexable_size = nis->at(index);
    bool encounted_near_death_objects = false;
    int length = smiOop(w->raw_at(non_indexable_size))->value();
    for(int i = 1; i <= length; i++) {
      oop obj = w->raw_at(non_indexable_size+i);
      if (mark_sweep_is_near_death(obj)) {
        encounted_near_death_objects = true;
        memOop(obj)->mark_as_dying();
      }
    }
    if (encounted_near_death_objects)
      NotificationQueue::put_if_absent(w);
  }
}

// NotificationQueue

oop* NotificationQueue::array = NULL;
int  NotificationQueue::size  = 100;
int  NotificationQueue::first = 0;
int  NotificationQueue::last  = 0;

bool NotificationQueue::is_empty() {
  return first == last;
}

int NotificationQueue::succ(int index) {
  return (index+1) % size;
}

oop NotificationQueue::get() {
  assert(!is_empty(), "must contain elements");
  oop result = array[first];
  first = succ(first);
  return result;
}

void NotificationQueue::put(oop obj) {
  if (array == NULL) array = NEW_C_HEAP_ARRAY(oop, size);
  if (succ(last) == first) {
    int new_size = size * 2;
    int new_last = 0;
    // allocate new_array
    oop* new_array = NEW_C_HEAP_ARRAY(oop, new_size);
    // copy from array to new_array
    for (int index = first; index != last; index = succ(index))
      new_array[new_last++] = array[index];
    free(array);
    // replace array
    array = new_array;
    size  = new_size;
    first = 0;
    last  = new_last;
  } 
  array[last] = obj;
  last = succ(last);
}

void NotificationQueue::put_if_absent(oop obj) {
  for (int index = first; index != last; index = succ(index)) {
    if (array[index] == obj) return;
  }
  put(obj);
}

void NotificationQueue::oops_do(void f(oop*)) {
  for (int index = first; index != last; index = succ(index))
    f(&array[index]);
}

void NotificationQueue::mark_elements() {
  for (int index = first; index != last; index = succ(index)) {
    oop obj = array[index];
    if (obj->is_mem()) memOop(obj)->set_queued();
  }
}

void NotificationQueue::clear_elements() {
  for (int index = first; index != last; index = succ(index)) {
    oop obj = array[index];
    if (obj->is_mem()) memOop(obj)->clear_queued();
  }
}

