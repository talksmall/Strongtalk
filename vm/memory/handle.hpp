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

// Interface for preserving memOops across scavenge/gc in primitives.
// ex.
//    HandleMark hm;
//    Handle saved_oop(obj);
//    // call that might cause scavenge/gc
//    saved_oop.as_klass()->foo();

class HandleMark;
class Handle;
class FunctionProcessClosure;
extern "C" volatile void* handleCallBack(int index, int params);

class BaseHandle {
  const char* label;
  bool log;
  oop saved;
  BaseHandle* next;
  BaseHandle* prev;
protected:
  void push();
  virtual BaseHandle* first() = 0;
  virtual void setFirst(BaseHandle*) = 0;
  BaseHandle(oop toSave, bool log, const char* label);
  void oops_do(void f(oop*));
public:
  void pop();
  oop* asPointer() {
    return &saved;
  }
  oop as_oop() {
    return saved;
  }
  klassOop as_klassOop() {
    return klassOop(saved);
  }
  friend class FunctionProcessClosure;
  friend volatile void* handleCallBack(int index, int params);
};

class StackHandle : public BaseHandle, StackObj {
protected:
  BaseHandle* first();
  void setFirst(BaseHandle* handle);
public:
  StackHandle(oop toSave, bool log = false, const char* label = "");
  ~StackHandle();
  static void all_oops_do(void f(oop*));
};
// PersistentHandles can preserve a memOop without occupying space in the Handles
// array and do not require a HandleMark. This means they can be used in contexts
// where a thread switch may occur (eg. surrounding a delta call). 
class PersistentHandle : public CHeapObj {
  oop saved;
  PersistentHandle* next;
  PersistentHandle* prev;
  static PersistentHandle* first;

public:
  static int savedOffset() {
    return (int)&((PersistentHandle*)NULL)->saved;
  }
  PersistentHandle(oop toSave);
  ~PersistentHandle();
  oop as_oop() {
    return saved;
  }
  klassOop as_klassOop() {
    return klassOop(saved);
  }
  oop* asPointer() {
    return &saved;
  }
  static void oops_do(void f(oop*));
};

class Handles : AllStatic {
  static int  _top;
  static int  _size;
  static oop  _array[];
  static oop  oop_at(int index);
  static int  push_oop(oop value);
  static int  top() { return _top; }
  static void set_top(int t);

  friend class HandleMark;
  friend class Handle;
 public:
  // Memory managment
  static void oops_do(void f(oop*));
};

class HandleMark {
  int top;
 public:
  HandleMark()  { top = Handles::top(); }
  ~HandleMark() { Handles::set_top(top);}
};

class Handle : StackObj {
  int index;
 public:
  Handle(oop value) {
    index = Handles::push_oop(value);
  }
  
  oop as_oop() {
    return Handles::oop_at(index);
  }

  objArrayOop as_objArray() {
    assert(Handles::oop_at(index)->is_objArray(), "type check");
    return objArrayOop(Handles::oop_at(index));
  }

  memOop as_memOop() {
    assert(Handles::oop_at(index)->is_mem(), "type check");
    return memOop(Handles::oop_at(index));
  }

  klassOop as_klass() {
    assert(Handles::oop_at(index)->is_mem(), "type check");
    return klassOop(Handles::oop_at(index));
  }
};
