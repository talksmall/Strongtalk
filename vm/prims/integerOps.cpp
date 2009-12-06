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
static const int   digitBitLength = sizeof(Digit) * 8;

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

unsigned int Integer::as_unsigned_int(bool& ok) const {
  ok = true;
  switch (_signed_length) {
    case  0:
      return 0;
    case  1:
      return  (unsigned int)_first_digit;
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

  DoubleDigit lx = x;
  DoubleDigit r = lx + y + carry;
  carry = r >> digitBitLength;
  return (Digit)(r  & oneB);
}

inline Digit IntegerOps::xmy(Digit x, Digit y, Digit& carry) {
  // returns (x - y - c) mod B; sets carry = -((x - y - c) div B)
  DoubleDigit lx = x;
  DoubleDigit r = lx - y - carry;
  carry = r >> digitBitLength & 1;
  return Digit(r  & oneB);
}

inline Digit IntegerOps::axpy(Digit a, Digit x, Digit y, Digit& carry) {
  // returns (a*x + y + c) mod B; sets carry = (a*x + y + c) div B
  DoubleDigit lx = x;
  DoubleDigit r = (lx * a) + y + carry;
  carry = r >> digitBitLength;
  return Digit(r  & oneB);
}

inline Digit IntegerOps::xdy(Digit x, Digit y, Digit& carry) {
  // returns (carry*B + x) div y; sets carry = (carry*B + x) mod y
  DoubleDigit c = carry;	// make sure that carry is used below and not &carry
  DoubleDigit total = ((c << digitBitLength) + x);
  carry = total % y;
  return Digit((total / y) & oneB);
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

Digit* IntegerOps::copyDigits(Digit* source, int length, int toCopy) {
  Digit* x = NEW_RESOURCE_ARRAY(Digit, length);
  for (int i = toCopy - 1; i >= 0; i--)
    x[i] = source[i];
  return x;
}

Digit* IntegerOps::qr_decomposition_single_digit(Digit* x, int length, Digit divisor) {
  Digit c = 0;
  for (int i = length - 1; i >= 0; i--)
    x[i+1] = xdy(x[i], divisor, c);
  x[0] = c;
  return x;
}

Digit IntegerOps::qr_estimate_digit_quotient(Digit& xhi, Digit xlo, Digit y) {
  if (xhi == y) return oneB;
  return xdy(xlo, y, xhi);
}

Digit IntegerOps::qr_calculate_remainder(Digit* qr, Digit* divisor, Digit q, int qrStart, int stop) {
  Digit c = 0;
  int   j = qrStart;
  int   k = 0;
  Digit b = 0;
  while (k < stop) {
    qr[j] = xmy(qr[j], 0, b);
    Digit qyk = axpy(q, divisor[k], 0, c);
    Digit c2 = 0;
    qr[j] = xmy(qr[j], qyk, c2);
    b = xpy(b, c2, c);
    assert(c == 0, "Overflow");
    
    j++;
    k++;
  }
  return b;
}

Digit IntegerOps::qr_adjust_for_underflow(Digit* qr, Digit* divisor, Digit q, int qrStart, int stop) {
  int j = qrStart;
  int k = 0;
  Digit c = 0;
  while(k < stop) {
    qr[j] = xpy(qr[j], divisor[k], c);
    j++;
    k++;
  }
  assert(c == 1, "Should have carry to balance borrow"); // see Knuth p258 D6
  return q-1; // correct estimate
}

Digit IntegerOps::qr_adjust_for_over_estimate(Digit y1, Digit y2, Digit q, Digit xi, Digit xi2) {
  //check if estimate is too large
  Digit c = 0;
  Digit y2q = axpy(y2, q, 0, c);
  if (c > xi || (c = xi && y2q > xi2)) {
    q--; // too large by 1
    Digit c2 = 0;
    xpy(xi, y1, c2); // add back divisor

    if (!c2) {
      c = 0;
      y2q = axpy(y2, q, 0, c);
      if (c > xi || (c = xi && y2q > xi2))
        q--; // too large by 2
    }
  }
  return q;
}

Digit IntegerOps::qr_scaling_factor(Digit firstDivisorDigit) {
  Digit c = 1;
  return (firstDivisorDigit == oneB) ? 1 : xdy(0, firstDivisorDigit + 1, c); // d = B/(yn + 1)
}

void IntegerOps::qr_unscale_remainder(Digit* qr, Digit scalingFactor, int length) {
  Digit c = 0;
  for (int i = length - 1; i >= 0; i--)
    qr[i] = xdy(qr[i], scalingFactor, c); // undo scaling to get remainder
  assert(c == 0, "qr scaling broken");
}

Digit* IntegerOps::qr_decomposition(Integer& dividend, Integer& y0) {
// qr_decomposition divides x by y (unsigned) and returns its decomposition
// into quotient (q) and remainder (r) packed into the array qr with length
// x.length() + 1. The layout of the result qr is as follows:
//
// qr      [<--- quotient (q) ---->|<-- remainder (r) -->]
// index   |xl  xl-1  ...  yl+1  yl|yl-1  yl-2  ...  1  0|
//
// length of quotient : ql = xl - yl + 1 (xl >= yl => ql >= 1)
// length of remainder: rl = yl          (yl >   0 => rl >= 1)

  int dividendLength = dividend.length();
  int divisorLength = y0.length();
  if (dividendLength < divisorLength) fatal("division not needed");
  if (divisorLength == 0) fatal("division by zero");
  // initialize qr
  Digit* x = copyDigits(dividend.digits(), dividendLength+1, dividendLength);

  if (divisorLength == 1)
    return qr_decomposition_single_digit(x, dividendLength, y0._first_digit);

  // full division
  x[dividendLength] = 0;
  Digit* y = y0.digits();

  Digit d = qr_scaling_factor(y[divisorLength-1]); // d = B/(yn + 1)

  if (d > 1) {
    x[dividendLength] = scale(x, d, dividendLength);
    y = copyDigits(y, divisorLength, divisorLength);
    Digit c = scale(y, d, divisorLength);

    if (c != 0) fatal("qr_decomposition broken");
  }

  Digit y1 = y[divisorLength-1];
  Digit y2 = y[divisorLength-2];
  int   i  = dividendLength;
  while (i >= divisorLength) {
    Digit xi = x[i]; //x[i] gets overwritten by remainder so save it for later
    Digit q = qr_estimate_digit_quotient(x[i], x[i-1], y1); // estimate q = rem/y
    //x[i] now contains remainder - used in test below
    q = qr_adjust_for_over_estimate(y1, y2, q, x[i], x[i - 2]);

    Digit b = qr_calculate_remainder(x, y, q, i - divisorLength, divisorLength);
    xmy(xi, 0, b);
    if (b) { // underflow 
      x[i] = qr_adjust_for_underflow(x, y, q, i - divisorLength, divisorLength);
    } else {
      x[i] = q;
    }
    i--;
  }
  if (d != 1) {
    qr_unscale_remainder(x, d, divisorLength);
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

bool IntegerOps::sd_all_zero(Digit* digits, int start, int stop) {
  for (int i = start; i < stop; i++)
    if (digits[i]) return false;
  return true;
}

void IntegerOps::signed_div(Integer& x, Integer& y, Integer& z) {
  int xl = x.length();
  bool xneg = x.is_negative();
  int yl = y.length();
  bool yneg = y.is_negative();

  if (xl < yl) {
    // unsigned x < unsigned y => z = 0
    if (xneg == yneg)
      z.set_length(0);
    else {
      z.set_length(1);
      z[0] = 1;
      neg(z);
    }
  } else {
    // xl >= yl
    ResourceMark rm;
    Digit* qr = qr_decomposition(x, y);

    int i  = xl;
    while (i >= yl && qr[i] == 0) i--;
    // i < yl || qr[i] != 0
    Digit carry = !sd_all_zero(qr, 0, yl) && xneg != yneg;
    int digits = i - yl + 1;
    for (int j = 0; j < digits; j++)
      z[j] = xpy(qr[yl + j], 0, carry);

    if (carry) {
      z.set_length(i-yl+2);
      z[i - yl + 1] = carry;
    } else
      z.set_length(i-yl+1);
    if (xneg != yneg) neg(z);
  }
}
int IntegerOps::last_non_zero_index(Digit* z, int lastIndex) {
  int i = lastIndex;
  while (i >= 0&& z[i] == 0) i--;
  return i;
}
void IntegerOps::signed_mod(Integer& x, Integer& y, Integer& z) {
  int xl = x.length();
  int yl = y.length();
  if (xl < yl) {
    // unsigned x < unsigned y => z = (y - x)
    Digit carry = 0;
    int i;
    for (i = 0; i < xl; i++)
      z[i] = xmy(y[i], x[i], carry);
    for (i = xl; i < yl; i++)
      z[i] = xmy(y[i], 0, carry);
    assert(carry == 0, "Remainder too large");
    
    z.set_length(last_non_zero_index(z.digits(), yl -1) + 1);
  } else {
    // xl >= yl
    ResourceMark rm;
    Digit* qr = qr_decomposition(x, y);

    if (!sd_all_zero(qr, 0, yl)) {
      Digit carry = 0;
      for (int j = 0; j < yl; j++)
        qr[j] = xmy(y[j], qr[j], carry);
      assert(carry == 0, "Remainder too large");
    }

    int i = last_non_zero_index(qr, yl -1);
    z.set_length(i+1);
    while (i >= 0) { z[i] = qr[i]; i--; }
  }
}

void IntegerOps::unsigned_rem(Integer& x, Integer& y, Integer& z) {
  int xl = x.length();
  int yl = y.length();
  if (xl < yl) {
    // unsigned x < unsigned y => z = x
    copy(x, z);
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
  if (x.is_negative() == y.is_negative()) {
    l = unsigned_quo_result_length(x, y);
  } else {
    l = unsigned_quo_result_length(x, y) + 1;
  }
  return Integer::length_to_size_in_bytes(l);
}


int IntegerOps::mod_result_size_in_bytes(Integer& x, Integer& y) {
  int digitLength = unsigned_rem_result_length(x, y);
  return Integer::length_to_size_in_bytes(digitLength);
}


int IntegerOps::and_result_size_in_bytes(Integer& x, Integer& y) {
  int digitLength;
  if (x.is_zero()||y.is_zero()) {
    digitLength = 0;
  } else if (x.is_positive() && y.is_positive()) {
    digitLength = min(x.length(), y.length());
  } else if (x.is_positive()) {
    digitLength = x.length();
  } else if (y.is_positive()) {
    digitLength = y.length();
  } else {
    digitLength = max(x.length(), y.length());
  }
  return Integer::length_to_size_in_bytes(digitLength);
}


int IntegerOps::or_result_size_in_bytes (Integer& x, Integer& y) {
  int digitLength;
  if (x.is_positive() && y.is_positive())
    digitLength = max(x.length(), y.length());
  else if (y.is_positive())
    digitLength = x.length();
  else if (x.is_positive())
    digitLength = y.length();
  else
    digitLength = min(x.length(), y.length());

  return Integer::length_to_size_in_bytes(digitLength);
}


int IntegerOps::xor_result_size_in_bytes(Integer& x, Integer& y) {
  int digitLength;
  if (x.is_negative() != y.is_negative())
    digitLength = max(x.length(), y.length()) + 1;
  else
    digitLength = max(x.length(), y.length());
  return Integer::length_to_size_in_bytes(digitLength);
}


int IntegerOps::ash_result_size_in_bytes(Integer& x, int n) {
  int digitLength;
  if (x.is_zero() || n == 0) {
    digitLength = x.length();
  } else if (n > 0) {
    int rem = n % logB;
    Digit mask = nthMask(logB - rem) ^ oneB;
    bool overflow = (mask & x[x.length() - 1]) != 0;
    digitLength = x.length() + (n / logB) + overflow;
  } else {
    int rem = (-n) % logB;
    Digit mask = nthMask(rem) ^ oneB;
    digitLength = max(x.length() + (n / logB), 1);
  }
  return Integer::length_to_size_in_bytes(digitLength);
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

#define copyInteger(x)\
  (Integer*)memcpy((void*)NEW_RESOURCE_ARRAY(Digit, x.length() + 1), \
                 (void*)&x._signed_length,\
                 (x.length() + 1) * sizeof(Digit))

void IntegerOps::div(Integer& x, Integer& y, Integer& z) {
  ResourceMark rm;

  if (!x.is_negative() && y.is_positive()) {
    unsigned_quo(x, y, z);
  } else {
    signed_div(x, y, z);
  }
}


void IntegerOps::mod(Integer& x, Integer& y, Integer& z) {
  if (x.is_negative() == y.is_negative()) {
    unsigned_rem(x, y, z);
  } else {
    signed_mod(x, y, z);
  }
  if (y.is_negative() && z.is_not_zero()) neg(z);
}


void IntegerOps::and(Integer& x, Integer& y, Integer& z) {
  if (x.is_zero() || y.is_zero()) {
    z.set_length(0);
  } else if (x.is_positive() && y.is_positive()) {
    int l  = min(x.length(), y.length());
    int i  = 0;
    while (i <  l) { z[i] = x[i] & y[i]; i++; }
    z.set_length(i);
  } else if (x.is_positive()) {
    and_one_positive(x, y, z);
  } else if (y.is_positive()) {
    and_one_positive(y, x, z);
  } else {
    and_both_negative(x, y, z);
  }
}
void IntegerOps::and_both_negative(Integer& x, Integer& y, Integer& z) {
  int digitLength = max(x.length(), y.length());

  Digit xcarry = 1;
  Digit ycarry = 1;
  Digit zcarry = 1;
  int i = 0;
  while(i < min(x.length(), y.length())) {
    z[i] = xpy((xpy(x[i] ^ 0xffffffff, 0, xcarry)
              & xpy(y[i] ^ 0xffffffff, 0, ycarry)) ^ 0xffffffff, 0, zcarry);
    i++;
  }
  while(i < x.length()) {
    z[i] = x[i];
    i++;
  }
  while(i < y.length()) {
    z[i] = y[i];
    i++;
  }
  z.set_length(-digitLength);
}

void IntegerOps::and_one_positive(Integer& positive, Integer& negative, Integer& z) {
  int digitLength = positive.length();

  Digit carry = 1;
  int i = 0;
  while(i < min(positive.length(), negative.length())) { // digits in both
    z[i] = positive[i] & xpy((negative[i] ^oneB), 0, carry);
    i++;
  }
  while(i < digitLength) { // remaining digits in positive
    z[i] = positive[i];
    i++;
  }
  z.set_length(digitLength);
}

void IntegerOps::or(Integer& x, Integer& y, Integer& z) {
  int xl = x.length();
  int yl = y.length();
  int l  = min(xl, yl);
  Digit xcarry = 1;
  Digit ycarry = 1;
  Digit zcarry = 1;
  int i  = 0;
  if (!x.is_negative() && !y.is_negative()) {
    while (i <  l) { z[i] = x[i] | y[i]; i++; }
    while (i < xl) { z[i] = x[i]       ; i++; }
    while (i < yl) { z[i] =        y[i]; i++; }
    z.set_length(i);
    return;
  } else if (!x.is_negative()) {
    while (i < l) {
      z[i] = xpy((x[i] | xpy(y[i] ^ oneB, 0, ycarry)) ^ oneB, 0, zcarry);
      i++;
    }
    while (i < yl) {
      z[i] = y[i];
      i++;
    }
  } else if (!y.is_negative()) {
    while (i < l) {
      z[i] = xpy((y[i] | xpy(x[i] ^ oneB, 0, xcarry)) ^ oneB, 0, zcarry);
      i++;
    }
    while (i < xl) {
      z[i] = x[i];
      i++;
    }
  } else {
    assert(x.is_negative() && y.is_negative(), "Error invalid integers?");
    while (i < l) {
      z[i] = xpy((xpy(y[i] ^ oneB, 0, ycarry) | xpy(x[i] ^ oneB, 0, xcarry)) ^ oneB, 0, zcarry);
      i++;
    }
  }
  while (i > 0 && !z[i - 1]) i--;
  z.set_length(-i);
}


void IntegerOps::xor(Integer& x, Integer& y, Integer& z) {
  int xl = x.length();
  int yl = y.length();
  int l  = min(xl, yl);
  int i  = 0;
  if (!x.is_negative() && !y.is_negative()) {
    while (i <  l) { z[i] = x[i] ^ y[i]; i++; }
    while (i < xl) { z[i] = x[i]       ; i++; }
    while (i < yl) { z[i] =        y[i]; i++; }
    while (i > 0 && !z[i - 1]) i--;
    z.set_length(i);
  } else if (!y.is_negative()) {
    xor_one_positive(y, x, z);
  } else if (!x.is_negative()) {
    xor_one_positive(x, y, z);
  } else {
    assert(x.is_negative() && y.is_negative(), "Error invalid integers?");
    Digit xcarry = 1;
    Digit ycarry = 1;
    while(i < l) {
      z[i] = xmy(x[i], 0, xcarry) ^ xmy(y[i], 0, ycarry);
      i++;
    }
    while (i < xl) {
      z[i] = xmy(x[i], 0, xcarry);
      i++;
    }
    while (i < yl) {
      z[i] = xmy(y[i], 0, ycarry);
      i++;
    }
    while (i > 0 && z[i - 1] == 0) i--;
    z.set_length(i);
  }
}

void IntegerOps::xor_one_positive(Integer& positive, Integer& negative, Integer& z) {
  int pl = positive.length();
  int nl = negative.length();
  int l  = min(pl, nl);
  int i  = 0;
  Digit ncarry = 1;
  Digit zcarry = 1;
  while (i < l) {
    z[i] = xpy(positive[i] ^ xmy(negative[i], 0, ncarry), 0, zcarry);
    i++;
  }
  while (i < nl) {
    z[i] = xpy(xmy(negative[i], 0, ncarry), 0, zcarry);
    i++;
  }
  while (i < pl) {
    z[i] = xpy(positive[i], 0, zcarry);
    i++;
  }
  if (zcarry) {
    z[i] = 1;
    i++;
  }
  z.set_length(-i);
}
void IntegerOps::ash(Integer& x, int n, Integer& z) {
  if (n > 0) {
    int i = 0;
    int bitShift = n % logB;
    int digitShift = n / logB;
    while (i < digitShift) {
      z[i] = 0;
      i++;
    }
    Digit carry = 0;
    while (i < x.length() + digitShift) {
      z[i] = (x[i - digitShift] << bitShift) + carry;
      carry = bitShift ? x[i - digitShift] >> (logB - bitShift) : 0;
      i++;
    }
    if (carry) {
      z[i] = carry;
      i++;
    }
    z.set_length(i);
    if (x.is_negative()) neg(z);
  } else {
    int digitShift = -n / logB;
    int bitShift = -n % logB;
    int i = x.length() - digitShift;
    Digit carry = 0;
    while (i > 0) {
      i--;
      z[i]  = (x[i + digitShift] >> bitShift) + carry;
      carry = bitShift ? (x[i + digitShift] & nthMask(bitShift)) << (logB - bitShift) : 0;
    }
    i = x.length() - digitShift;
    while(i > 0 && z[i - 1] == 0) i--;
    z.set_length(max(i, 0));
    if (x.is_negative()) neg(z);
    if (x.is_negative() && z.is_zero()) {
      z.set_length(-1);
      z[0] = 1;
    }
  }
}


int IntegerOps::cmp(Integer& x, Integer& y) {
  if (x.is_negative() == y.is_negative()) {
    return x.is_negative() ? unsigned_cmp(y, x) : unsigned_cmp(x, y);
  } else {
    return x.is_zero() ? -y.signum() : x.signum();
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

void IntegerOps::unsigned_int_to_Integer(unsigned int i, Integer& z) {
  if (i == 0)
    z.set_length(0);
  else {
    z.set_length(1);
    z._first_digit = Digit(i);
  }
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

int IntegerOps::hash(Integer& x) {
  int hash = 0;
  for (int i = x.length() - 1; i >= 0; i--)
    hash ^= x[i];
  hash ^= x.signum();
  return hash>>2;
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
