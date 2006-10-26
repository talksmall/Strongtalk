/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.19 $ */
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
# include "incls/_dByteArrayOop.cpp.incl"

bool doubleByteArrayOopDesc::verify() {
  bool flag = memOopDesc::verify();
  if (flag) {
    int l = length();
    if (l < 0) {
      error("doubleByteArrayOop %#lx has negative length", this);
      flag = false;
    }
  }
  return flag;
}

void doubleByteArrayOopDesc::bootstrap_object(bootstrap* st) {
  memOopDesc::bootstrap_object(st);
  st->read_oop(length_addr());
  for (int index = 1; index <= length(); index++)
    doubleByte_at_put(index, st->read_doubleByte());
}

inline int sub_sign(int a, int b) {
  if (a < b) return -1;
  if (a > b) return  1;
  return 0;
}

inline int compare_as_doubleBytes(const doubleByte* a, const doubleByte* b) {
  // machine dependent code; little endian code
  if (a[0] - b[0]) return sub_sign(a[0], b[0]);
  return sub_sign(a[1], b[1]);
}

int doubleByteArrayOopDesc::compare(doubleByteArrayOop arg) {
  // Get the addresses of the length fields
  const unsigned int* a = (const unsigned int*) length_addr();
  const unsigned int* b = (const unsigned int*) arg->length_addr();

  // Get the word sizes of the arays
  int a_size = roundTo(smiOop(*a++)->value() * sizeof(doubleByte), sizeof(int)) / sizeof(int);
  int b_size = roundTo(smiOop(*b++)->value() * sizeof(doubleByte), sizeof(int)) / sizeof(int);

  const unsigned int* a_end = a + min(a_size, b_size);
  while(a < a_end) {
    if (*b++ != *a++) 
      return compare_as_doubleBytes((const doubleByte*) (a-1), (const doubleByte*) (b-1));
  }
  return sub_sign(a_size, b_size);
}

/*
int doubleByteArrayOopDesc::compare(doubleByteArrayOop arg) {
  // Get the addresses of the length fields
  int         len_a = length();
  doubleByte* a     = doubleBytes();

  int         len_b = arg->length();
  doubleByte* b     = arg->doubleBytes();

  doubleByte* end = len_a <= len_b ? a + len_a : b + len_b;

  while(a < end) {
    int result = *a++ - *b++;
    if (result != 0) {
      if (result < 0) return -1;
      if (result > 0) return  1;
    }
  }
  return sub_sign(len_a, len_b);
}
*/

int doubleByteArrayOopDesc::hash_value() {
  int len = length();
  int result;

  if (len == 0) {
    result = 1;
  } else if (len == 1) {
    result = doubleByte_at(1);
  } else {
    unsigned int val;
    val = doubleByte_at(1);
    val = (val << 3) ^ (doubleByte_at(2)         ^ val);
    val = (val << 3) ^ (doubleByte_at(len)       ^ val);
    val = (val << 3) ^ (doubleByte_at(len-1)     ^ val);
    val = (val << 3) ^ (doubleByte_at(len/2 + 1) ^ val);
    val = (val << 3) ^ (len                      ^ val);
    result = markOopDesc::masked_hash(val);
  }
  return result == 0 ? 1 : result;
}


bool doubleByteArrayOopDesc::copy_null_terminated(char* buffer, int max_length) {
  int len = length();
  bool is_truncated = false;
  if (len >= max_length) {
    len = max_length - 1;
    is_truncated = true;
  }
  for (int index = 0; index < len; index++)
    buffer[index] = (char) doubleByte_at(index+1);
  buffer[len]= '\0';
  return is_truncated;
}

char* doubleByteArrayOopDesc::as_string() {
  int len = length();
  char* str = NEW_RESOURCE_ARRAY(char, len+1);
  for (int index = 0; index <len; index++) {
    str[index] = (char) doubleByte_at(index+1);
  }
  str[index] = '\0';
  return str;
}

