/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.18 $ */
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
# include "incls/_objArrayOop.cpp.incl"

bool objArrayOopDesc::verify() {
  bool flag = memOopDesc::verify();
  if (flag) {
    int l = length();
    if (l < 0) {
      error("objArrayOop %#lx has negative length", this);
      flag = false;
    }
  }
  return flag;
}

void objArrayOopDesc::bootstrap_object(bootstrap* st) {
  memOopDesc::bootstrap_object(st);
  st->read_oop(length_addr());
  for (int index = 1; index <= length(); index++)
    st->read_oop(objs(index));
}

objArrayOop objArrayOopDesc::copy_remove(int index, int number) {
  objArrayOop new_array = oopFactory::new_objArray(length() - number);
  // copy [1..index-1]
  for (int i = 1; i < index ; i++)
    new_array->obj_at_put(i, obj_at(i));

  // copy  [index+number..length]
  for (i = index; i <= length() - number; i++)
    new_array->obj_at_put(i, obj_at(i+number));

  return new_array;
}

objArrayOop objArrayOopDesc::copy() {
  objArrayOop new_array = oopFactory::new_objArray(length());
  for (int i = 1; i <= length(); i++)
    new_array->obj_at_put(i, obj_at(i));
  return new_array;
}

objArrayOop objArrayOopDesc::copy_add(oop a) {
  objArrayOop new_array = oopFactory::new_objArray(length() + 1);
  for (int i = 1; i <= length(); i++)
    new_array->obj_at_put(i, obj_at(i));
  new_array->obj_at_put(i, a);
  return new_array;
}

objArrayOop objArrayOopDesc::copy_add_two(oop a, oop b) {
  objArrayOop new_array = oopFactory::new_objArray(length() + 2);
  for (int i = 1; i < length(); i++)
    new_array->obj_at_put(i, obj_at(i));
  new_array->obj_at_put(i++, a);
  new_array->obj_at_put(i,   b);
  return new_array;
}

// Old Smalltalk code:
//  replaceFrom: start <Int> to: stop <Int> with: other <SeqCltn[E]> startingAt: repStart <Int>
//	"replace the elements of the receiver from start to stop with elements from other,
//	  starting with the element of other with index repStart."
//
//	| otheri <Int> |
//	repStart < start
//		ifFalse: [ otheri := repStart.
//				  start to: stop do:
//					[ :i <Int> |
//						self at: i put: (other at: otheri).
//						otheri := otheri + 1.	]]
//		ifTrue: [ otheri := repStart + (stop - start).
//				stop to: start by: -1 do:
//					[ :i <Int> |
//						self at: i put: (other at: otheri).
//						otheri := otheri - 1.	]]

void objArrayOopDesc::replace_from_to(int from, int to, objArrayOop source, int start) {
  int other_index = start;
  if (start < to) {
    // copy up
    for (int index = from; index <= to; index++) {
      source->obj_at_put(other_index++, obj_at(index));
    }
  } else {
    // copy down
    for (int index = to; index >= from; index--) {
      source->obj_at_put(other_index--, obj_at(index));
    }
  }
}

void objArrayOopDesc::replace_and_fill(int from, int start, objArrayOop source) {
  // Fill the first part
  for (int index = 1; index < from; index++) {
    obj_at_put(index, nilObj);
  }

  // Fill the middle part
  int to = min(source->length() - start + 1, length());

  for (index = from; index <= to; index++) {
    obj_at_put(index, source->obj_at(start + index - from));
  }

  // Fill the last part
  for (index = to + 1; index <= length(); index++) {
    obj_at_put(index, nilObj);
  }
}

void weakArrayOopDesc::scavenge_contents_after_registration() {
  oop* p   = objs(1);
  int  len = length();
  for (int index = 1; index <= len; index++)
    scavenge_tenured_oop(p++);
}

void weakArrayOopDesc::follow_contents_after_registration() {
  oop* p   = objs(1);
  int  len = length();
  for (int index = 1; index <= len; index++)
    scavenge_tenured_oop(p++);
}
