/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.26 $ */
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
# include "incls/_byteArrayOop.cpp.incl"
# include <string.h>
# include <errno.h>

bool byteArrayOopDesc::verify() {
  bool flag = memOopDesc::verify();
  if (flag) {
    int l = length();
    if (l < 0) {
      error("byteArrayOop %#lx has negative length", this);
      flag = false;
    }
  }
  return flag;
}

char* byteArrayOopDesc::copy_null_terminated(int &Clength) {
  // Copy the bytes() part. Always add trailing '\0'. If byte array
  // contains '\0', these will be escaped in the copy, i.e. "....\0...".
  // Clength is set to length of the copy (may be longer due to escaping).
  // Presence of null chars can be detected by comparing Clength to length().

  assert_byteArray(this, "should be a byte array");
  Clength = length();
  char *res = copy_string((char*) bytes(), Clength);
  if (strlen(res) == (unsigned int) Clength) 
    return res;                   // Simple case, no '\0' in byte array.
  
  // Simple case failed ...
  smi t = length();               // Copy and 'escape' null chars.
  smi i;
  for (i = length()-1; i >= 0; i--) 
    if (byte_at(i) == '\0') t++; 
  // t is total length of result string.
  res = NEW_RESOURCE_ARRAY( char, t + 1);
  res[t--] = '\0';
  Clength  = t;
  for (i = length()-1; i >= 0; i--) {
    if (byte_at(i) != '\0') {
      res[t--] = byte_at(i);
    } else {
      res[t--] = '0';
      res[t--]   = '\\';
    }
  }
  assert(t == -1, "sanity check");
  return res;
}

char* byteArrayOopDesc::copy_c_heap_null_terminated() {
  // Copy the bytes() part. Always add trailing '\0'. If byte array
  // contains '\0', these will be escaped in the copy, i.e. "....\0...".
  // NOTE: The resulting string is allocated in Cheap
  
  assert_byteArray(this, "should be a byte array");
  smi t = length();               // Copy and 'escape' null chars.
  smi i;
  for (i = length()-1; i >= 0; i--) 
    if (byte_at(i) == '\0') t++; 
  // t is total length of result string.
  char* res = NEW_C_HEAP_ARRAY( char, t + 1);
  res[t--] = '\0';
  for (i = length()-1; i >= 0; i--) {
    if (byte_at(i) != '\0') {
      res[t--] = byte_at(i);
    } else {
      res[t--] = '0';
      res[t--]   = '\\';
    }
  }
  assert(t == -1, "sanity check");
  return res;
}

bool byteArrayOopDesc::copy_null_terminated(char* buffer, int max_length) {
  // %not optimized
  int len = length();
  bool is_truncated = false;
  if (len > max_length) {
    len = max_length - 1;
    is_truncated = true;
  }

  for (int i = 0; i < len; i++)
    buffer[i] = byte_at(i + 1);
  buffer[len] = '\0';

  return is_truncated;
}

void byteArrayOopDesc::bootstrap_object(bootstrap* st) {
  memOopDesc::bootstrap_object(st);

  st->read_oop(length_addr());
  for (int index = 1; index <= length(); index++) {
    byte_at_put(index, st->read_byte());
  }
}

inline int sub_sign(int a, int b) {
  if (a < b) return -1;
  if (a > b) return  1;
  return 0;
}

inline int compare_as_bytes(const unsigned char* a, const unsigned char* b) {
  // machine dependent code; little endian code
  if (a[0] - b[0]) return sub_sign(a[0], b[0]);
  if (a[1] - b[1]) return sub_sign(a[1], b[1]);
  if (a[2] - b[2]) return sub_sign(a[2], b[2]);
  return sub_sign(a[3], b[3]);
}

int byteArrayOopDesc::compare(byteArrayOop arg) {
  // Get the addresses of the length fields
  const unsigned int* a = (unsigned int*) length_addr();
  const unsigned int* b = (unsigned int*) arg->length_addr();

  // Get the word sizes of the arays
  int a_size = roundTo(smiOop(*a++)->value() * sizeof(char), sizeof(int)) / sizeof(int);
  int b_size = roundTo(smiOop(*b++)->value() * sizeof(char), sizeof(int)) / sizeof(int);

  const unsigned int* a_end = a + min(a_size, b_size);
  while(a < a_end) {
    if (*b++ != *a++) 
      return compare_as_bytes((const unsigned char*) (a-1), (const unsigned char*) (b-1));
  }
  return sub_sign(a_size, b_size);
}

int byteArrayOopDesc::compare_doubleBytes(doubleByteArrayOop arg) {
  // %not optimized
  int s1 = length();
  int s2 = arg->length();
  int n  = s1 < s2 ? s1 : s2;

  for (int i = 1; i <= n; i++) {
    int result = sub_sign(byte_at(i), arg->doubleByte_at(i));
    if (result != 0) return result;
  }
  return sub_sign(s1, s2);
}

int byteArrayOopDesc::hash_value() {
  int len = length();
  int result;

  if (len == 0) {
    result = 1;
  } else if (len == 1) {
    result = byte_at(1);
  } else {
    unsigned int val;
    val = byte_at(1);
    val = (val << 3) ^ (byte_at(2)         ^ val);
    val = (val << 3) ^ (byte_at(len)       ^ val);
    val = (val << 3) ^ (byte_at(len-1)     ^ val);
    val = (val << 3) ^ (byte_at(len/2 + 1) ^ val);
    val = (val << 3) ^ (len                ^ val);
    result = markOopDesc::masked_hash(val);
  }
  return result == 0 ? 1 : result;
}

char* byteArrayOopDesc::as_string() {
  int len = length();
  char* str = NEW_RESOURCE_ARRAY(char, len+1);
  for (int index = 0; index <len; index++) {
    str[index] = byte_at(index+1);
  }
  str[index] = '\0';
  return str;
}

# include <ctype.h>

int byteArrayOopDesc::number_of_arguments() const {
  int result = 0;
  assert(length() > 0, "selector should have a positive length");
  
  // Return 1 if binary selector
  if (ispunct(byte_at(1))) return 1;

  // Return number of colons
  for (int index = 1; index <= length(); index++)
    if (byte_at(index) == ':')
      result++;

  return result;
}

bool byteArrayOopDesc::is_unary() const {
  if (is_binary()) return false;
  for (int index = 1; index <= length(); index++)
    if (byte_at(index) == ':') return false;
  return true;
}

bool byteArrayOopDesc::is_binary() const {
  return ispunct(byte_at(1));
}

bool byteArrayOopDesc::is_keyword() const {
  if (is_binary()) return false;
  for (int index = 1; index <= length(); index++)
    if (byte_at(index) == ':') return true;
  return false;
}
