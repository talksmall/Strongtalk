/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.5 $ */
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
# include "incls/_integerOps.cpp.incl"


static const int   maxD = 36;
static const int   logB = sizeof(Digit)*8;
static const Digit hlfB = 0x80000000;
static const Digit oneB = 0xFFFFFFFF;


// double layout
//
// double fields: [s|exponent|mantissa]
// field lengths: |1|<--11-->|<--52-->|
const int sign_length     =    1;
const int exponent_length =   11;
const int mantissa_length =   52;
const int double_length   =   64;
const int exponent_bias   = 1023;
const int max_exponent    = 2046;


// Treating of unimplemented code

static void missing_code_for(char* s) {
  fatal1("code missing for %s", s);
}


// Helper functions

static inline int exponent(double x) {
// Extracts the un-biased (binary) exponent of x.
  const int n = double_length/logB;
  Digit d[n];
  *((double*)d) = x;
  return int((d[n-1] << sign_length) >> (logB - exponent_length)) - exponent_bias;
}


static int length_in_bits(Digit x) {
// Computes the index of the most significant bit + 1
// (length is 0 for x = 0).
  int i = 0;
  while (x != 0) { x >>= 1; i++; }
  return i;
}


static void shift_left(Digit d[], int length, int shift_count) {
// Implements d[length] << shift_count (logical bit-wise left shift).
  if (shift_count <= 0) return;
  // shift_count > 0
  if (shift_count % logB == 0) {
    // no bit-shifting needed
    int i = length - 1;
    int k = shift_count / logB;
    while (i > k) {
      d[i] = d[i-k];
      i--;
    }
    while (i > 0) {
      d[i] = 0;
      i--;
    }
  } else {
    // bit-shifting needed
    int i = length - 1;
    int k = shift_count / logB;
    int h = shift_count % logB;
    int l = logB - h;
    while (i > k) {
      d[i] = (d[i-k] << h) | (d[i-k-1] >> l);
      i--;
    }
    d[i] = d[i-k] << h;
    i--;
    while (i > 0) {
      d[i] = 0;
      i--;
    }
  }
}


static void shift_right(Digit d[], int length, int shift_count) {
// Implements d[length] >> shift_count (logical bit-wise right shift).
  if (shift_count <= 0) return;
  // shift_count > 0
  if (shift_count % logB == 0) {
    // no bit shifting needed
    int i = 0;
    int k = shift_count / logB;
    while (i < length - k) {
      d[i] = d[i+k];
      i++;
    }
    while (i < length) {
      d[i] = 0;
      i++;
    }
  } else {
    // bit-shifting needed
    int i = 0;
    int k = shift_count / logB;
    int l = shift_count % logB;
    int h = logB - l;
    while (i < length - k - 1) {
      d[i] = (d[i+k+1] << h) | (d[i+k] >> l);
      i++;
    }
    d[i] = d[i+k] >> l;
    i++;
    while (i < length) {
      d[i] = 0;
      i++;
    }
  }
}


// Implementation of Integer

int Integer::length_in_bits() const {
  if (is_zero()) {
    return 0;
  } else {
    int i = length() - 1;
    return i*logB + ::length_in_bits(operator[](i));
  }
}


int Integer::as_int(bool& ok) const {
  ok = true;
  switch (_signed_length) {
    case -1:
      if (-int(_first_digit) < 0) return -int(_first_digit);
      break;
    case  0:
      return 0;
    case  1:
      if ( int(_first_digit) > 0) return  int(_first_digit);
      break;
  }
  ok = false;
  return 0;
}


double Integer::as_double(bool& ok) const {
  // filter out trivial result 0.0
  ok = true;
  if (is_zero()) return 0.0;
  
  // get an n-Digit integer d[n] built from the n most significant digits of self
  // n needs to be big enough so that we have enough bits for the mantissa (note
  // that the mantissa consists of one (implicit) extra bit which is always 1).
  const int n = (mantissa_length + 1)/logB + 2;
  Digit d[n];
  int l = length();
  int i = 1;
  while (i <= n) {
    d[n-i] = l-i >= 0 ? operator[](l-i) : 0;
    i++;
  }

  // shift d[n] to the left so that the most significant bit of d is shifted out
  int left_shift_count = logB - ::length_in_bits(d[n-1]) + 1;
  shift_left(d, n, left_shift_count);

  // shift d[n] to the right so it builds the mantissa of a double
  const int right_shift_count = sign_length + exponent_length;
  shift_right(d, n, right_shift_count);

  // add exponent to d
  int exponent = exponent_bias + length_in_bits() - 1;
  if (exponent > max_exponent) {
    // integer too large => doesn't fit into double representation
    ok = false;
    return 0.0;
  }
  assert(logB - right_shift_count > 0, "check this code");
  d[n-1] = d[n-1] | (Digit(exponent) << (logB - right_shift_count));

  // cast d into double & set sign
  double result = *((double*)&d[n - (double_length/logB)]);
  if (is_negative()) result = -result;
  return result;
}


smiOop Integer::as_smi(bool& ok) const {
  ok = true;
  switch (_signed_length) {
    case -1:
      if (_first_digit <= -smi_min) return as_smiOop(-int(_first_digit));
      break;
    case  0:
      return as_smiOop(0);
    case  1:
      if (_first_digit <=  smi_max) return as_smiOop( int(_first_digit));
      break;
  }
  ok = false;
  return as_smiOop(0);
}


void Integer::print() {
  char s[100000]; // for the time being - FIX THIS
  IntegerOps::Integer_to_string(*this, 10, s);
  int i = 0;
  while (s[i] != '\x0') {
    std->print("%c", s[i]);
    i++;
  }
}


// Basic 32/64bit unsigned operations

inline Digit IntegerOps::as_Digit(char c) {
  if ('0' <= c && c <= '9') return Digit(c - '0');
  if ('A' <= c && c <= 'Z') return Digit(c - 'A') + 10;
  if ('a' <= c && c <= 'z') return Digit(c - 'a') + 10;
  fatal("illegal digit");
  return 0;
}


inline char IntegerOps::as_char(int i) {
  assert(0 <= i && i < maxD, "illegal digit");
  return "0123456789abcdefghijklmnopqrstuvwxyz"[i];
}


inline Digit IntegerOps::xpy(Digit x, Digit y, Digit& carry) {
  // returns (x + y + c) mod B; sets carry = (x + y + c) div B
  Digit c = carry;	// make sure that carry is used below and not &carry
  Digit r;
  assert(c <= 1, "wrong carry");
  __asm {
    //push eax		// save eax
    mov eax, x		// eax = x
    add eax, y		// eax = (x + y    ) mod B,	0 <= carry = (x + y    ) div B <= 1
    adc eax, c		// eax = (x + y + c) mod B,	0 <= carry = (x + y + c) div B <= 1
    mov r, eax		// eax is result
    mov eax, 0		// cannot use xor because that clears carry bit!
    adc eax, 0		// eax = carry
    mov c, eax		// save carry
    //pop eax		// restore eax
  }
  carry = c;
  return r;
}


inline Digit IntegerOps::xmy(Digit x, Digit y, Digit& carry) {
  // returns (x - y - c) mod B; sets carry = -((x - y - c) div B)
  Digit c = carry;	// make sure that carry is used below and not &carry
  Digit r;
  assert(c <= 1, "wrong carry");
  __asm {
    //push eax		// save eax
    mov eax, x		// eax = x
    sub eax, y		// eax = (x - y    ) mod B,	0 <= carry = (x - y    ) div B <= 1
    sbb eax, c		// eax = (x - y - c) mod B,	0 <= carry = (x - y - c) div B <= 1
    mov r, eax		// eax is result
    mov eax, 0		// cannot use xor because that clears carry bit!
    adc eax, 0		// eax = carry
    mov c, eax		// save carry
    //pop eax		// restore eax
  }
  carry = c;
  return r;
}


inline Digit IntegerOps::axpy(Digit a, Digit x, Digit y, Digit& carry) {
  // returns (a*x + y + c) mod B; sets carry = (a*x + y + c) div B
  Digit c = carry;	// make sure that carry is used below and not &carry
  Digit r;
  __asm {
    //push eax		// save eax
    //push edx		// save edx
    mov eax, a		// eax = a
    mul x		// edx:eax = a*x
    add eax, y		//
    adc edx, 0		// edx:eax = a*x + y
    add eax, c		//
    adc edx, 0		// edx:eax = a*x + y + c
    mov r, eax		// eax is result
    mov c, edx		// save carry
    //pop edx		// restore edx
    //pop eax		// restore eax
  }
  carry = c;
  return r;
}


inline Digit IntegerOps::xdy(Digit x, Digit y, Digit& carry) {
  // returns (carry*B + x) div y; sets carry = (carry*B + x) mod y
  Digit c = carry;	// make sure that carry is used below and not &carry
  Digit r;
  __asm {
    //push eax		// save eax
    //push edx		// save edx
    mov eax, x		// eax = x
    mov edx, c		// edx:eax =  carry*B + x
    div y		// edx:eax = (carry*B + x) mod y : (carry*B + x) div y
    mov r, eax		// eax is result
    mov c, edx		// save carry
    //pop edx		// restore edx
    //pop eax		// restore eax
  }
  carry = c;
  return r;
}


Digit IntegerOps::power(Digit x, int n) {
  Digit f = x;
  Digit p = 1;
  int   i = n;
  while (i > 0) {
    // p * f^i = x^n
    if ((i & 1) == 1) {
      // i is odd
      p *= f;
    }
    f *= f;
    i >>= 1;
  }
  return p;
}


Digit IntegerOps::max_power(Digit x) {
  Digit n = 1;
  Digit p = x;
  Digit c = 0;
  while (c == 0) {
    // x^n = c*B + p
    p = axpy(x, p, 0, c);
    n++;
  }
  return p == 0 ? n : n-1;
}


// Unsigned operations

void IntegerOps::unsigned_add(Integer& x, Integer& y, Integer& z) {
  int   xl = x.length();
  int   yl = y.length();
  int   l  = min(xl, yl);
  int   i  = 0;
  Digit c  = 0;
  while (i <  l) { z[i] = xpy(x[i], y[i], c); i++; }
  while (i < xl) { z[i] = xpy(x[i], 0,    c); i++; }
  while (i < yl) { z[i] = xpy(0,    y[i], c); i++; }
  if    (c != 0) { z[i] = c;                  i++; }
  z.set_length(i);
}


void IntegerOps::unsigned_sub(Integer& x, Integer& y, Integer& z) {
  int   xl = x.length();
  int   yl = y.length();
  int   i  = 0;
  Digit c  = 0;
  while (i < yl) { z[i] = xmy(x[i], y[i], c); i++; }
  while (i < xl) { z[i] = xmy(x[i], 0,    c); i++; }
  if (c != 0) fatal("negative result");
  while (i > 0 && z[i-1] == 0) i--;
  z.set_length(i);
}


void IntegerOps::unsigned_mul(Integer& x, Integer& y, Integer& z) {
  int xl = x.length();
  int yl = y.length();
  // initialize z
  int i  = xl + yl;
  while (i > 0) { i--; z[i] = 0; }
  // i == 0
  int k = 0;
  while (i < xl) {
    Digit d = x[i];
    if (d != 0) {
      int j = 0;
      k = i;
      Digit c = 0;
      while (j < yl) { z[k] = axpy(d, y[j], z[k], c); j++; k++; }
      if    (c != 0) { z[k] = c;                           k++; }
      i++;
    }
  }
  z.set_length(k);
}


Digit IntegerOps::scale(Digit* array, Digit factor, int length) {
// scale multiplies an integer representation stored
// in array by factor and returns the last carry.

  int   i = 0;
  Digit c = 0;
  while (i < length) { array[i] = axpy(factor, array[i], 0, c); i++; }
  return c;
}


Digit* IntegerOps::qr_decomposition(Integer& x0, Integer& y0) {
// qr_decomposition divides x by y (unsigned) and returns its decomposition
// into quotient (q) and remainder (r) packed into the array qr with length
// x.length() + 1. The layout of the result qr is as follows:
//
// qr      [<--- quotient (q) ---->|<-- remainder (r) -->]
// index   |xl  xl-1  ...  yl+1  yl|yl-1  yl-2  ...  1  0|
//
// length of quotient : ql = xl - yl + 1 (xl >= yl => ql >= 1)
// length of remainder: rl = yl          (yl >   0 => rl >= 1)

  int xl = x0.length();
  int yl = y0.length();
  if (xl < yl) fatal("division not needed");
  if (yl == 0) fatal("division by zero");
  // initialize qr
  Digit* x = NEW_RESOURCE_ARRAY(Digit, xl+1);
  int i = xl;
  while (i > 0) { i--; x[i] = x0[i]; }
  // decompose
  if (yl == 1) {
    // single-digit y => use simple division
    int i = xl;
    Digit c = 0;
    Digit d = y0._first_digit;
    while (i > 0) { i--; x[i+1] = xdy(x[i], d, c); }
    x[0] = c;
  } else if (xl >= yl) {
    missing_code_for("division by large integers");
    // full division
    x[xl] = 0;
    Digit* y = y0.digits();
    Digit d = (hlfB - 1) / y[yl-1] + 1;

    if (d != 1) {
      // scale x (already copied)
      x[xl] = scale(x, d, xl);
      // make a copy of y
      y = NEW_RESOURCE_ARRAY(Digit, yl);
      int i = yl;
      while (i > 0) { i--; y[i] = y0[i]; }
      // scale y
      Digit c = scale(y, d, yl);
      if (c != 0) fatal("qr_decomposition broken");
    }

    Digit y1 = y[yl-1];
    Digit y2 = y[yl-2];
    int   i  = xl;
    while (i >= yl) {
      Digit q = (x[i] == y1) ? oneB : xdy(x[i-1], y1, x[i]);

      while (true) {}

      int   j = i-yl;
      int   k = 0;
      Digit c = 0;
      while (k < yl) { j++; k++; }
      if (true) {
        
      } else {
        x[i] = q;
      }
      i--;
    }
    if (d != 1) {
      int   i = yl;
      Digit c = 0;
      while (i > 0) { i--; x[i] = xdy(x[i], d, c); }
    }
  } else {
    // xl < yl
    //
    // add some more comments here
  }
  return x;
}


void IntegerOps::unsigned_quo(Integer& x, Integer& y, Integer& z) {
  int xl = x.length();
  int yl = y.length();
  if (xl < yl) {
    // unsigned x < unsigned y => z = 0
    z.set_length(0);
  } else {
    // xl >= yl
    ResourceMark rm;
    Digit* qr = qr_decomposition(x, y);
    int i  = xl;
    while (i >= yl && qr[i] == 0) i--;
    // i < yl || qr[i] != 0
    z.set_length(i-yl+1);
    while (i >= yl) { z[i-yl] = qr[i]; i--; }
  }
}


void IntegerOps::unsigned_rem(Integer& x, Integer& y, Integer& z) {
  int xl = x.length();
  int yl = y.length();
  if (xl < yl) {
    // unsigned x < unsigned y => z = y
    copy(y, z);
  } else {
    // xl >= yl
    ResourceMark rm;
    Digit* qr = qr_decomposition(x, y);
    int i = yl-1;
    while (i >= 0 && qr[i] == 0) i--;
    // i < 0 || qr[i] != 0
    z.set_length(i+1);
    while (i >= 0) { z[i] = qr[i]; i--; }
  }
}


int IntegerOps::unsigned_cmp(Integer& x, Integer& y) {
  int xl = x.length();
  int yl = y.length();
  if (xl == yl && xl > 0) {
    int i = xl - 1;
    while (i > 0 && x[i] == y[i]) i--;
    return int(x[i] - y[i]);
  } else {
    return xl - yl;
  }
}


Digit IntegerOps::last_digit(Integer& x, Digit b) {
  int   xl = x.length();
  int   i  = xl;
  Digit c  = 0;
  while (i > 0) { i--; x[i] = xdy(x[i], b, c); }
  if (xl > 0 && x[xl-1] == 0) x.set_length(xl-1);
  return c;
}


void IntegerOps::first_digit(Integer& x, Digit b, Digit c) {
  int   xl = x.length();
  int   i  = 0;
  while (i < xl) { x[i] = axpy(b, x[i], 0, c); i++; }
  if    (c != 0) { x[i] = c;                   i++; }
  x.set_length(i);
}


// Implementation of IntegerOps

int IntegerOps::add_result_size_in_bytes(Integer& x, Integer& y) {
  int l;
  if (x.is_negative() == y.is_negative()) {
    l = unsigned_add_result_length(x, y);
  } else if (unsigned_cmp(x, y) < 0) {
    l = unsigned_sub_result_length(y, x);
  } else {
    l = unsigned_sub_result_length(x, y);
  }
  return Integer::length_to_size_in_bytes(l);
}


int IntegerOps::sub_result_size_in_bytes(Integer& x, Integer& y) {
  int l;
  if (x.is_negative() != y.is_negative()) {
    l = unsigned_add_result_length(x, y);
  } else if (unsigned_cmp(x, y) < 0) {
    l = unsigned_sub_result_length(y, x);
  } else {
    l = unsigned_sub_result_length(x, y);
  }
  return Integer::length_to_size_in_bytes(l);
}


int IntegerOps::mul_result_size_in_bytes(Integer& x, Integer& y) {
  return Integer::length_to_size_in_bytes(unsigned_mul_result_length(x, y));
}


int IntegerOps::quo_result_size_in_bytes(Integer& x, Integer& y) {
  return Integer::length_to_size_in_bytes(unsigned_quo_result_length(x, y));
}


int IntegerOps::rem_result_size_in_bytes(Integer& x, Integer& y) {
  return Integer::length_to_size_in_bytes(unsigned_rem_result_length(x, y));
}


int IntegerOps::div_result_size_in_bytes(Integer& x, Integer& y) {
  int l;
  if (!x.is_negative() && y.is_positive()) {
    l = unsigned_quo_result_length(x, y);
  } else {
    missing_code_for("division with negative numbers");
  }
  return Integer::length_to_size_in_bytes(l);
}


int IntegerOps::mod_result_size_in_bytes(Integer& x, Integer& y) {
  int l;
  if (!x.is_negative() && y.is_positive()) {
    l = unsigned_rem_result_length(x, y);
  } else {
    missing_code_for("division with negative numbers");
  }
  return Integer::length_to_size_in_bytes(l);
}


int IntegerOps::and_result_size_in_bytes(Integer& x, Integer& y) {
  int l;
  if (x.is_positive() && y.is_positive()) {
    l = min(x.length(), y.length());
  } else {
    missing_code_for("bitwise and with negative numbers");
  }
  return Integer::length_to_size_in_bytes(l);
}


int IntegerOps::or_result_size_in_bytes (Integer& x, Integer& y) {
  int l;
  if (x.is_positive() && y.is_positive()) {
    l = max(x.length(), y.length());
  } else {
    missing_code_for("bitwise or with negative numbers");
  }
  return Integer::length_to_size_in_bytes(l);
}


int IntegerOps::xor_result_size_in_bytes(Integer& x, Integer& y) {
  int l;
  if (x.is_positive() && y.is_positive()) {
    l = max(x.length(), y.length());
  } else {
    missing_code_for("bitwise xor with negative numbers");
  }
  return Integer::length_to_size_in_bytes(l);
}


int IntegerOps::ash_result_size_in_bytes(Integer& x, int n) {
  missing_code_for("arithmetic shift");
  return 0;
}


int IntegerOps::copy_result_size_in_bytes(Integer& x) {
  return x.size_in_bytes();
}


int IntegerOps::int_to_Integer_result_size_in_bytes(int i) {
  return Integer::length_to_size_in_bytes(i != 0);
}


int IntegerOps::double_to_Integer_result_size_in_bytes(double x) {
  if (x < 0.0) x = -x;
  return Integer::length_to_size_in_bytes(x < 1.0 ? 0 : (exponent(x) + logB)/logB);
}


int IntegerOps::string_to_Integer_result_size_in_bytes(char* s, int base) {
  // for now: implement for base 10 only, use simple heuristics
  if (base == 10) {
    int i = 0;
    while (s[i] != '\x0') i++;
    return Integer::length_to_size_in_bytes(i/9 + 1);
  }
  missing_code_for("string conversion with base not equal to 10");
  return -1;
}


int IntegerOps::Integer_to_string_result_size_in_bytes(Integer& x, int base) {
  // for now: implement for base 10 only, use simple heuristics
  if (base == 10) {
    return (x.length() + 1) * 10; // add one for sign & zero
  }
  missing_code_for("string conversion with base not equal to 10");
  return -1;
}


void IntegerOps::add(Integer& x, Integer& y, Integer& z) {
  if (x.is_negative() == y.is_negative()) {
    unsigned_add(x, y, z);
  } else if (unsigned_cmp(x, y) < 0) {
    unsigned_sub(y, x, z); neg(z);
  } else {
    unsigned_sub(x, y, z);
  }
  if (x.is_negative()) neg(z);
}


void IntegerOps::sub(Integer& x, Integer& y, Integer& z) {
  if (x.is_negative() != y.is_negative()) {
    unsigned_add(x, y, z);
  } else if (unsigned_cmp(x, y) < 0) {
    unsigned_sub(y, x, z); neg(z);
  } else {
    unsigned_sub(x, y, z);
  }
  if (x.is_negative()) neg(z);
}


void IntegerOps::mul(Integer& x, Integer& y, Integer& z) {
  if (x.is_zero() || y.is_zero()) {
    z.set_length(0);
  } else if (x.length() < y.length()) {
    unsigned_mul(x, y, z);
  } else {
    unsigned_mul(y, x, z);
  }
  if (x.is_negative() != y.is_negative()) neg(z);
}


void IntegerOps::quo(Integer& x, Integer& y, Integer& z) {
  unsigned_quo(x, y, z);
  if (x.is_negative() != y.is_negative()) neg(z);
}


void IntegerOps::rem(Integer& x, Integer& y, Integer& z) {
  unsigned_rem(x, y, z);
  if (x.is_negative()) neg(z);
}


void IntegerOps::div(Integer& x, Integer& y, Integer& z) {
  if (!x.is_negative() && y.is_positive()) {
    unsigned_quo(x, y, z);
  } else {
    missing_code_for("division with negative numbers");
  }
}


void IntegerOps::mod(Integer& x, Integer& y, Integer& z) {
  if (!x.is_negative() && y.is_positive()) {
    unsigned_rem(x, y, z);
  } else {
    missing_code_for("division with negative numbers");
  }
}


void IntegerOps::and(Integer& x, Integer& y, Integer& z) {
  if (x.is_positive() && y.is_positive()) {
    int l  = min(x.length(), y.length());
    int i  = 0;
    while (i <  l) { z[i] = x[i] & y[i]; i++; }
    z.set_length(i);
  } else {
    missing_code_for("bitwise and with negative numbers");
  }
}


void IntegerOps::or(Integer& x, Integer& y, Integer& z) {
  if (x.is_positive() && y.is_positive()) {
    int xl = x.length();
    int yl = y.length();
    int l  = min(xl, yl);
    int i  = 0;
    while (i <  l) { z[i] = x[i] | y[i]; i++; }
    while (i < xl) { z[i] = x[i]       ; i++; }
    while (i < yl) { z[i] =        y[i]; i++; }
    z.set_length(i);
  } else {
    missing_code_for("bitwise or with negative numbers");
  }
}


void IntegerOps::xor(Integer& x, Integer& y, Integer& z) {
  if (x.is_positive() && y.is_positive()) {
    int xl = x.length();
    int yl = y.length();
    int l  = min(xl, yl);
    int i  = 0;
    while (i <  l) { z[i] = x[i] ^ y[i]; i++; }
    while (i < xl) { z[i] = x[i]       ; i++; }
    while (i < yl) { z[i] =        y[i]; i++; }
    z.set_length(i);
  } else {
    missing_code_for("bitwise xor with negative numbers");
  }
}


void IntegerOps::ash(Integer& x, int n, Integer& z) {
  missing_code_for("arithmetic shift");
}


int IntegerOps::cmp(Integer& x, Integer& y) {
  if (x.is_negative() == y.is_negative()) {
    return x.is_negative() ? unsigned_cmp(y, x) : unsigned_cmp(x, y);
  } else {
    return x.signum();
  }
}


void IntegerOps::abs(Integer& x) {
  if (x.is_negative()) neg(x);
}


void IntegerOps::neg(Integer& x) {
  x.set_length(-x._signed_length);
}


void IntegerOps::copy(Integer& x, Integer& z) {
  z._signed_length = x._signed_length;
  int i = x.length();
  while (i > 0) { i--; z[i] = x[i]; }
}


void IntegerOps::int_to_Integer(int i, Integer& z) {
  if (i < 0) {
    z.set_length(-1);
    z._first_digit = Digit(-i);
  } else if (i == 0) {
    z.set_length( 0);
    // no digits in this case
  } else {
    z.set_length( 1);
    z._first_digit = Digit( i);
  }
}


void IntegerOps::double_to_Integer(double x, Integer& z) {
  // filter out sign
  bool negative = false;
  if (x < 0.0) { negative = true; x = -x; }
  
  // filter out trivial cases
  if (x < 1.0) { z.set_length(0); return; }

  // get an n-Digit integer d[n] built from x. n needs to be big enough
  // so that we don't loose bits after shifting (note that the mantissa
  // consists of one (implicit) extra bit which is always 1).
  const int n = (mantissa_length + 1)/logB + 2;
  Digit d[n];
  int i = 0;
  while (i < n) { d[i] = 0; i++; }
  *((double*)&d[n - (double_length/logB)]) = x;

  // compute length l of integer
  int length_in_bits = exponent(x) + 1;
  int l = (length_in_bits + logB - 1)/logB;

  // shift sign & exponent out but keep space for implicit 1 bit
  const int left_shift_count = sign_length + exponent_length - 1;
  shift_left(d, n, left_shift_count);

  // add implicit 1 bit
  const Digit mask = Digit(-1) << (logB - 1);
  d[n-1] |= mask;

  // shift right to the right
  const int right_shift_count = logB - length_in_bits%logB;
  shift_right(d, n, right_shift_count);
  
  // copy most significant digits into z & fill with zeros
  i = 1;
  while (l-i >= 0 && n-i >= 0) { z[l-i] = d[n-i]; i++; }
  while (l-i >= 0            ) { z[l-i] = 0;      i++; }

  // set length & adjust sign
  z.set_length(l);
  if (negative) neg(z);
}


void IntegerOps::string_to_Integer(char* s, int base, Integer& z) {
  int_to_Integer(0, z);
  int i = s[0] == '-';
  while (s[i] != '\x0') {
    first_digit(z, base, as_Digit(s[i]));
    i++;
  }
  if (s[0] == '-') neg(z);
}


void IntegerOps::Integer_to_string(Integer& x, int base, char* s) {
  assert(2 <= base && base <= maxD, "illegal base");
  Integer t;
  assert(x.size_in_bytes() <= 10001*sizeof(Digit), "temporary array too small");
  copy(x, t);
  // convert t into s (destructive)
  int i = 0;
  do {
    s[i] = as_char(last_digit(t, base));
    i++;
  } while (t.is_not_zero());
  if (x.is_negative()) { s[i] = '-'; i++; }
  s[i] = '\x0';
  // reverse string
  int j = i-1;
  i = 0;
  while (i < j) {
    char c = s[i]; s[i] = s[j]; s[j] = c;
    i++; j--;
  }
}

  
// testing

static inline void check(bool p, char* s) {
  if (!p) fatal(s);
}


static void factorial(int n) {
  Integer x, y, z;
  IntegerOps::int_to_Integer(1, z);
  for (int i = 2; i <= n; i++) {
    IntegerOps::int_to_Integer(i, x);
    IntegerOps::copy(z, y);
    IntegerOps::mul(x, y, z);
    assert(z.size_in_bytes() <= sizeof(z), "result too big");
  };
  std->print("%d! = ", n);
  z.print();
  std->cr();
}


static void unfactorial(int n) {
  Integer x, y, z;
  IntegerOps::int_to_Integer(1, z);
  for (int i = 2; i <= n; i++) {
    IntegerOps::int_to_Integer(i, x);
    IntegerOps::copy(z, y);
    IntegerOps::mul(x, y, z);
    assert(z.size_in_bytes() <= sizeof(z), "result too big");
  };

  for (int j = 2; j <= n; j++) {
    IntegerOps::int_to_Integer(j, y);
    IntegerOps::copy(z, x);
    IntegerOps::div(x, y, z);
    assert(z.size_in_bytes() <= sizeof(z), "result too big");
  };

  std->print("%dun! = ", n);
  z.print();
  std->cr();
}


static void convert_to_double(int n) {
  Integer z;
  IntegerOps::int_to_Integer(n, z);
  bool ok;
  std->print_cr("%d = %f", n, z.as_double(ok));
}


static void convert_to_integer(double x) {
  Integer z;
  IntegerOps::double_to_Integer(x, z);
  std->print("%f = ", x);
  z.print();
  std->cr();
}


static void convert_from_string(char* s) {
  Integer z;
  IntegerOps::string_to_Integer(s, 10, z);
  std->print("%s = ", s);
  z.print();
  std->cr();
}


void IntegerOps::self_test() {
  bool ok;
  const int n = 10000;
  const int l = n*sizeof(int);
  Integer x, y, z;
  int i, j;
  // test integer conversion
  for (i = -10; i <= 10; i++) {
    check(int((i == 0 ? 1 : 2)*sizeof(int)) == int_to_Integer_result_size_in_bytes(i), "int_to_Integer_result_size failed");
    int_to_Integer(i, z);
    check(i == z.as_int(ok), "int_to_Integer/Integer_to_int failed");
  }
  // test string conversion

  // test addition
  for (i = -12345; i <= 12345; i += 1234) {
    for (j = -12345; j <= 12345; j += 1234) {
      int_to_Integer(i, x);
      int_to_Integer(j, y);
      add(x, y, z);
      check(z.as_int(ok) == i + j, "add failed");
    }
  }

  // test subtraction
  for (i = -12345; i <= 12345; i += 1234) {
    for (j = -12345; j <= 12345; j += 1234) {
      int_to_Integer(i, x);
      int_to_Integer(j, y);
      sub(x, y, z);
      check(z.as_int(ok) == i - j, "sub failed");
    }
  }

  // test multiplication
  for (i = -12345; i <= 12345; i += 1234) {
    for (j = -12345; j <= 12345; j += 1234) {
      int_to_Integer(i, x);
      int_to_Integer(j, y);
      mul(x, y, z);
      check(z.as_int(ok) == i * j, "mul failed");
    }
  }

  // factorial
  i = 0;
  while (i <=  10) { factorial(i); i++; }
  i = 20;
  while (i <= 100) { factorial(i); i += 10; }
  factorial(1000);

  // unfactorial
  i = 0;
  while (i <=  10) { unfactorial(i); i++; }
  i = 20;
  while (i <= 100) { unfactorial(i); i += 10; }
  unfactorial(1000);

  // double
  i = -10;
  while (i <=  10) { convert_to_double(i); i++; }
  i = 20;
  while (i <= 100) { convert_to_double(i); i += 10; }

  // double to Integer
  i = -10;
  while (i <=  10) { convert_to_integer(i); i++; }
  i = 20;
  while (i <= 100) { convert_to_integer(i); i += 10; }
  convert_to_integer(0.49);
  convert_to_integer(123.0e10);
  convert_to_integer(1.2e12);
  convert_to_integer(314.159265358979323e-2);
  convert_to_integer(-1.5);

  // string conversion
  convert_from_string("0");
  convert_from_string("1");
  convert_from_string("1234");
  convert_from_string("-1");
  convert_from_string("-1234");
  convert_from_string("1234567890123456789");
  convert_from_string("12345674401234567891234567890123456789");
  convert_from_string("12345678901255678912345676601234567891234567890123456789");

  // end
  std->print_cr("IntegerOps::self_test() done");
  exit(0);
}


void integerOps_init() {
  // uncomment line below if testing is desired
  // IntegerOps::self_test();
}