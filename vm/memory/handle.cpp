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
# include "incls/_handle.cpp.incl"

PersistentHandle* PersistentHandle::first = NULL;
int Handles::_top   = 0;
int Handles::_size  = 20;
oop Handles::_array[20];

PersistentHandle::PersistentHandle(oop toSave) : saved(toSave) {
  next = first;
  prev = NULL;
  if (first) first->prev = this;
  first = this;
}
PersistentHandle::~PersistentHandle() {
  if (prev) {
    prev->next = next;
  } else {
    first = next;
  }
  if (next)
    next->prev = prev;
}
void PersistentHandle::oops_do(void f(oop*)){
  for (PersistentHandle* current = first;
       current;
       current = current->next)
    f(&current->saved);
}

oop Handles::oop_at(int index) {
  assert(index >= 0 && index < top(), "index check");
  return _array[index];
}

int Handles::push_oop(oop value) {
  assert(_top < _size, "bounds check");
  _array[_top] = value;
  return _top++;
}

void Handles::set_top(int t) {
  assert(t >= 0 && t < top(), "index check");
  _top = t;
}

void Handles::oops_do(void f(oop*)) {
  for(int index = 0; index < top(); index++) {
    f(&_array[index]);
  }
  PersistentHandle::oops_do(f);
}
