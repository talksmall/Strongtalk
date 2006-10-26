/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.9 $ */
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

// doubleByteArrays are arrays containing double bytes

// memory layout:
//    [header      ]
//    [klass_field ]
//    [instVars    ]*
//    [length      ]      offset

class doubleByteArrayOopDesc: public memOopDesc {
  public:
  // constructor
  friend doubleByteArrayOop as_doubleByteArrayOop(void* p) {
    return doubleByteArrayOop(as_memOop(p)); }

  void bootstrap_object(bootstrap* st);

  // accessors
  doubleByteArrayOopDesc* addr() const {
    return (doubleByteArrayOopDesc*) memOopDesc::addr(); }

  bool is_within_bounds(int index) const { return 1 <= index && index <= length(); }

  oop* addr_as_oops() const { return (oop*)addr(); }

  // returns the location of the length field
  oop* length_addr() const {
    return &addr_as_oops()[blueprint()->non_indexable_size()];
  }

  smi length() const {
    oop len = *length_addr();
    assert(len->is_smi(), "length of indexable should be smi");
    return smiOop(len)->value();}

  void set_length(smi len) {
    *length_addr() = (oop) as_smiOop(len); }
 
  // returns the location where the double bytes start
  doubleByte* doubleBytes() const {
    return (doubleByte*) &length_addr()[1];  
  }

  doubleByte* doubleByte_at_addr(int which) const {
    assert(which > 0 && which <= length(), "index out of bounds");
    return &doubleBytes()[which - 1];
  }

  doubleByte doubleByte_at(int which) const {
    return *doubleByte_at_addr(which); }

  void doubleByte_at_put(int which, doubleByte contents) {
    *doubleByte_at_addr(which) = contents; }

  // three way compare
  int compare(doubleByteArrayOop arg);

  // Returns the hash value for the string
  int hash_value();

  // copy string to buffer as null terminated ascii string. 
  bool copy_null_terminated(char* buffer, int max_length);
  char* as_string();

  // memory operations
  bool verify();

  friend doubleByteArrayKlass;
};

