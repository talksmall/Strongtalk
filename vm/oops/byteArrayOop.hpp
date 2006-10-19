/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.24 $ */
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

class Integer;

// byteArrays are arrays containing bytes
//
// memory layout:
//    [header      ]
//    [klass_field ]
//    [instVars    ]*
//    [length      ]
//    [bytes       ]* = bytes(1) .. bytes(length) + padding

class byteArrayOopDesc: public memOopDesc {
  public:
  // constructor
  friend byteArrayOop as_byteArrayOop(void* p);

  void bootstrap_object(bootstrap* st);


  // accessors
  byteArrayOopDesc* addr() const {
    return (byteArrayOopDesc*) memOopDesc::addr(); }

  bool is_within_bounds(int index) const { return 1 <= index && index <= length(); }

  oop* addr_as_oops() const { return (oop*)addr(); }

  oop* length_addr() const  {
    return &addr_as_oops()[blueprint()->non_indexable_size()];
  }

  smi length() const {
    oop len = *length_addr();
    assert(len->is_smi(), "length of indexable should be smi");
    return smiOop(len)->value();}

  void set_length(smi len) {
    *length_addr() = (oop) as_smiOop(len); }
 
  u_char* bytes() const {
    return (u_char*) &length_addr()[1];  
  }

  char*   chars() const { return (char*) bytes(); }

  u_char* byte_at_addr(int which) const {
    assert(which > 0 && which <= length(), "index out of bounds");
    return &bytes()[which - 1];
  }

  u_char byte_at(int which) const {
    return *byte_at_addr(which); }
  void byte_at_put(int which, u_char contents) {
    *byte_at_addr(which) = contents; }


  // support for large integers

  Integer& number() { return *((Integer*)bytes()); }


  // memory operations
  bool verify();


  // C-string operations

  char *copy_null_terminated(int &Clength);
    // Copy the bytes() part. Always add trailing '\0'. If byte array
    // contains '\0', these will be escaped in the copy, i.e. "....\0...".
    // Clength is set to length of the copy (may be longer due to escaping).
    // Presence of null chars can be detected by comparing Clength to length().

  bool copy_null_terminated(char* buffer, int max_length);

  char *copy_null_terminated() {
    int ignore;
    return copy_null_terminated(ignore);
  }

  char *copy_c_heap_null_terminated();
    // Identical to copy_null_terminated but allocates the resulting string
    // in the C heap instead of in the resource area.

  bool equals(char* name) {
    return equals(name, strlen(name));
  }

  bool equals(char* name, int len) {
    return len == length() && strncmp(chars(), name, len) == 0; }
  bool equals(byteArrayOop s) {
    return equals(s->chars(), s->length()); }

  // three way compare
  int compare(byteArrayOop arg);
  int compare_doubleBytes(doubleByteArrayOop arg);

  // Returns the hash value for the string.
  int hash_value();

  // resource allocated print string
  char* as_string();

  // Selector specific operations.
  int number_of_arguments() const;
  bool is_unary()           const;
  bool is_binary()          const;
  bool is_keyword()         const;

  friend byteArrayKlass;
};

inline byteArrayOop as_byteArrayOop(void* p)
{
    return byteArrayOop(as_memOop(p));
}
