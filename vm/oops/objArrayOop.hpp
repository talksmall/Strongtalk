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

// objArrays are arrays containing oops

// memory layout:
//    [header      ]
//    [klass_field ]
//    [instVars    ]*
//    [length      ]
//    [elements    ]* = objs(1) .. objs(length)

class objArrayOopDesc: public memOopDesc {
 public:
  // constructor
  friend objArrayOop as_objArrayOop(void* p) {
    return objArrayOop(as_memOop(p)); }

  void bootstrap_object(bootstrap* st);

  // accessors
  objArrayOopDesc* addr() const {
    return (objArrayOopDesc*) memOopDesc::addr(); }

  bool is_within_bounds(int index) const { return 1 <= index && index <= length(); }

  oop* addr_as_oops() const { return (oop*) addr(); }

  oop* objs(int which) const {
    return &length_addr()[which];
  }

  oop* length_addr() const  {
    return &addr_as_oops()[blueprint()->non_indexable_size()];
  }

  smi length() const {
    oop len = *length_addr();
    assert(len->is_smi(), "length of indexable should be smi");
    return smiOop(len)->value();
  }

  void set_length(smi len) {
    *length_addr() = as_smiOop(len);
  }

  oop obj_at(int which) const {
    assert(which > 0 && which <= length(), "index out of bounds");
    return *objs(which);
  }

  void obj_at_put(int which, oop contents, bool cs = true) {
    assert(which > 0 && which <= length(), "index out of bounds");
    assert(!is_symbol(), "shouldn't be modifying a canonical string");
    assert(contents->verify(), "check contents");
    if (cs) {
      STORE_OOP(objs(which), contents);
    } else {
      *objs(which) = contents;
    }
  }

  objArrayOop growBy(int increment);

  // memory operations
  bool verify();

  objArrayOop copy_remove(int from, int number = 1);

  objArrayOop copy();
  objArrayOop copy_add(oop a);
  objArrayOop copy_add_two(oop a, oop b);

  // primtiive operations
  void replace_from_to(int from, int to, objArrayOop source, int start);
  void replace_and_fill(int from, int start, objArrayOop source);

  friend class objArrayKlass;
 private:
  // define the interval [begin .. end[ where the indexables are.
  oop* begin_indexables() const { return objs(1); }
  oop* end_indexables()   const { return begin_indexables() + length(); }
};


class weakArrayOopDesc: public objArrayOopDesc {
 public:
  friend weakArrayOop as_weakArrayOop(void* p) {
    return weakArrayOop(as_memOop(p)); }

  // accessors
  weakArrayOopDesc* addr() const {
    return (weakArrayOopDesc*) memOopDesc::addr(); }

  void scavenge_contents_after_registration();
  void follow_contents_after_registration();
};
