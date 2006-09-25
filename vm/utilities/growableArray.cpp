/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.16 $ */
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
# include "incls/_growableArray.cpp.incl"

GenericGrowableArray::GenericGrowableArray(int initial_size, bool c_heap) {
  len = 0;
  max = initial_size;
  assert(len <= max, "initial_size too small");
  on_C_heap = c_heap;
  if (c_heap) {
    data = (void**)AllocateHeap(max * sizeof(void*), "bounded list");
  } else {
    data = NEW_RESOURCE_ARRAY( void*, max);
  }
}

GenericGrowableArray::GenericGrowableArray(int initial_size, int initial_len, void* filler, bool c_heap) {
  len = initial_len;
  max = initial_size;
  assert(len <= max, "initial_len too big");
  on_C_heap = c_heap;
  if (c_heap) {
    data = (void**)AllocateHeap(max * sizeof(void*), "bounded list");
  } else {
    data = NEW_RESOURCE_ARRAY( void*, max);
  }
  for (int i = 0; i < len; i++) data[i] = filler;
}

void GenericGrowableArray::grow(int j) {
  void** newData;
  if (max == 0) fatal("cannot grow array with max = 0"); // for debugging - should create such arrays with max > 0
  while (j >= max) max = max*2;
  // j < max
  if (on_C_heap) {
    newData = (void**)AllocateHeap(max * sizeof(void*), "bounded list");
  } else {
    newData = NEW_RESOURCE_ARRAY( void*, max);
  }
  for (int i = 0; i < len; i++) newData[i] = data[i];
  data = newData;
}

bool GenericGrowableArray::raw_contains(const void* elem) const {
  for (int i = 0; i < len; i++) {
    if (data[i] == elem) return true;
  }
  return false;
}

GenericGrowableArray* GenericGrowableArray::raw_copy() const {
  GenericGrowableArray* copy = new GenericGrowableArray(max, len, NULL);
  for (int i = 0; i < len; i++) {
    copy->data[i] = data[i];
  }
  return copy;
}

void GenericGrowableArray::raw_appendAll(GenericGrowableArray* l) {
  for (int i = 0; i < l->len; i++) {
    raw_at_put_grow(len, l->data[i], NULL);
  }
}

int GenericGrowableArray::raw_find(const void* elem) const {
  for (int i = 0; i < len; i++) {
    if (data[i] == elem) return i;
  }
  return -1;
}

int GenericGrowableArray::raw_find(void* token, growableArrayFindFn f) const  {
  for (int i = 0; i < len; i++) {
    if (f(token, data[i])) return i;
  }
  return -1;
}

void GenericGrowableArray::raw_remove(const void* elem) {
  for (int i = 0; i < len; i++) {
    if (data[i] == elem) {
      for (int j = i + 1; j < len; j++) data[j-1] = data[j];
      len--;
      return;
    }
  }
  ShouldNotReachHere();
}

void GenericGrowableArray::raw_apply(voidDoFn f) const {
  for (int i = 0; i < len; i++) f(data[i]);
}

void* GenericGrowableArray::raw_at_grow(int i, const void* fill) {
  if (i >= len) {
    if (i >= max) grow(i);
    for (int j = len; j <= i; j++) data[j] = (void*)fill;
    len = i+1;
  }
  return data[i];
}

void GenericGrowableArray::raw_at_put_grow(int i, const void* p, const void* fill) {
  if (i >= len) {
    if (i >= max) grow(i);
    for (int j = len; j < i; j++) data[j] = (void*)fill;
    len = i+1;
  }
  data[i] = (void*)p;
}

void GenericGrowableArray::print() {
  print_short();
  lprintf(": length %ld (max %ld) { ", len, max);
  for (int i = 0; i < len; i++) lprintf("%#lx ", (long)data[i]);
  lprintf("}\n");
}


void GenericGrowableArray::raw_sort(int f(const void *, const void *)) {
  qsort(data, length(), sizeof(void*), f);
}

void GenericGrowableArray::print_short() { std->print("Growable Array %#lx", this); }
