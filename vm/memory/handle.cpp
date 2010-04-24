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

BaseHandle::BaseHandle(oop toSave, bool log, const char* label) 
  : saved(toSave), log(log), label(label) {}
void BaseHandle::push() {
  next = first();
  prev = NULL;
  if (next) {
    if (log) {
      char msg[200];
      sprintf(msg, "unpopped StackHandle '%s->%s' : 0x%x->0x%x", label, 
        next->label, this, next);
      assert((char*) this < (char*) next, msg);
    }
    next->prev = this;
  }
  if (log)
    std->print_cr("Pushing handle '%s': 0x%x", label, this);
  setFirst(this);
}
void BaseHandle::pop() {
  if (log)
    std->print_cr("Popping handle '%s': 0x%x", label, this);
  if (prev) {
    prev->next = next;
  } else {
    setFirst(next);
  }
  if (next)
    next->prev = prev;
}
void BaseHandle::oops_do(void f(oop*)){
  for (BaseHandle* current = this;
       current;
       current = current->next)
    f(&current->saved);
}
class FunctionProcessClosure: public ProcessClosure {
  void (*function)(oop*);
public:
  FunctionProcessClosure(void f(oop*)) {
    function = f;
  }
  void do_process(DeltaProcess* p) {
    if (p->firstHandle())
      p->firstHandle()->oops_do(function);
  }
};
void StackHandle::all_oops_do(void f(oop*)) {
  FunctionProcessClosure closure(f);
  Processes::process_iterate(&closure);
}
BaseHandle* StackHandle::first() {
  return DeltaProcess::active()->firstHandle();
}
void StackHandle::setFirst(BaseHandle* handle) {
  DeltaProcess::active()->setFirstHandle(handle);
}
StackHandle::StackHandle(oop toSave, bool log, const char* label) 
  : BaseHandle(toSave, log, label) {
  push();
}
StackHandle::~StackHandle() {
  pop();
}

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
  StackHandle::all_oops_do(f);
}
