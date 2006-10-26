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


// proxy objects are handles to external data.

// memory layout:
//    [header      ]
//    [klass_field ]
//    [pointer     ]

const int pointer_no     = sizeof(memOopDesc)/oopSize;
const int pointer_offset = sizeof(memOopDesc) - Mem_Tag;

class proxyOopDesc: public memOopDesc {
 private:
  void* _pointer;
 protected:
  proxyOopDesc* addr() const { return (proxyOopDesc*)memOopDesc::addr(); }

 public:
  friend proxyOop as_proxyOop(void* p) { return proxyOop(as_memOop(p)); }

  // sizing
  static int header_size() { return sizeof(proxyOopDesc)/oopSize; }

  void* get_pointer() const    { return addr()->_pointer; }
  void  set_pointer(void *ptr) { addr()->_pointer = ptr;  }

  void  null_pointer() { set_pointer(NULL); }

  bool is_null() const    { return get_pointer() ==       NULL; }
  bool is_allOnes() const { return get_pointer() == (void*) -1; }

  smi foreign_hash() const { return smi(get_pointer()) >> Tag_Size; }

  bool same_pointer_as(proxyOop x) const {
    return get_pointer() == x->get_pointer();
  }

  void bootstrap_object(bootstrap* st);

 private:
   unsigned char* addr_at(int offset) const {
     return ((unsigned char*) get_pointer()) + offset;
   }
 public:
  unsigned char byte_at(int offset) const {
    return *addr_at(offset);
  }
  void byte_at_put(int offset, unsigned char c) {
    *addr_at(offset) = c;
  }
  doubleByte doubleByte_at(int offset) const { 
    return *((doubleByte*) addr_at(offset));
  }
  void doubleByte_at_put(int offset, doubleByte db) {
     *((doubleByte*) addr_at(offset)) = db;
  }
  long long_at(int offset) const {
    return *((long*) addr_at(offset));
  }
  void long_at_put(int offset, long l) {
    *((long*) addr_at(offset)) = l;
  }

  float float_at(int offset) const {
    return *((float*) addr_at(offset));
  }
  void float_at_put(int offset, float f) {
    *((float*) addr_at(offset)) = f;
  }

  double double_at(int offset) const {
    return *((double*) addr_at(offset));
  }
  void double_at_put(int offset, double d) {
    *((double*) addr_at(offset)) = d;
  }

  friend proxyKlass;
};
