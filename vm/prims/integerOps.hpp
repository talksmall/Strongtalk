/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.3 $ */
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


// Integer is the representation for arbitrary long integers. A non-
// zero Integer x is represented by n Digits to a base B such that:
//
// x = x[n-1]*B^(n-1) + x[n-2]*B^(n-2) + ... + x[1]*B + x[0]
//
// with 0 <= x[i] < B for 0 <= i < n and x[n-1] > 0. n is called the
// length of x. x = 0 is represented by the length n = 0 and no digits.

class IntegerOps;
typedef unsigned int Digit;
typedef unsigned long long DoubleDigit;

class Integer: ValueObj {
 private:
  int    _signed_length;
  Digit  _first_digit;
  Digit  _debug[9999];

  int    length() const				{ return _signed_length < 0 ? -_signed_length : _signed_length; }
  void   set_length(int l)			{ _signed_length = l; }
  Digit* digits() const				{ return (Digit*)&_first_digit; }
  Digit& operator[] (int i) const		{ return digits()[i]; }

  static int length_to_size_in_bytes(int l)	{ return sizeof(int) + l*sizeof(Digit); }
  
  int    length_in_bits() const;

 public:
  int    signum() const				{ return _signed_length; }	// returns < 0 for x < 0; 0 for x = 0; > 0 for x > 0
  bool   is_zero() const			{ return signum() == 0; }
  bool   is_not_zero() const			{ return signum() != 0; }
  bool   is_positive() const			{ return signum() >  0; }
  bool   is_negative() const			{ return signum() <  0; }

  bool   is_odd() const				{ return is_not_zero() && (_first_digit & 1) == 1; }
  bool   is_even() const			{ return !is_odd(); }

  bool   is_valid() const			{ return is_zero() || operator[](length() - 1) != 0; }
  int    size_in_bytes() const			{ return length_to_size_in_bytes(length()); }

  int             as_int            (bool& ok) const;
  unsigned int    as_unsigned_int   (bool& ok) const;
  double          as_double         (bool& ok) const;
  smiOop          as_smi            (bool& ok) const;

  void print();

  friend class IntegerOps;
};


// IntegerOps provides arithmethic & logic operations on Integers. Integer arguments are
// usually denoted by x & y, the result Integer is z. The space for the resulting integer
// must have been allocated before performing the operation, the amount of space needed
// can be computed via the corresponding functions.

class IntegerOps: AllStatic {
 public:
  static inline Digit as_Digit(char c);
  static inline char  as_char(int i);

  static inline Digit xpy (         Digit x, Digit y, Digit& carry);
  static inline Digit xmy (         Digit x, Digit y, Digit& carry);
  static inline Digit axpy(Digit a, Digit x, Digit y, Digit& carry);
  static inline Digit xdy (         Digit x, Digit y, Digit& carry);

  static Digit  power(Digit x, int n);					// returns x^n
  static Digit  max_power(Digit x);					// returns the largest y with x^y <= B
  
  static int    unsigned_add_result_length(Integer& x, Integer& y)	{ return max(x.length(), y.length()) + 1; }
  static int    unsigned_sub_result_length(Integer& x, Integer& y)	{ return x.length(); }
  static int    unsigned_mul_result_length(Integer& x, Integer& y)	{ return x.is_zero() || y.is_zero() ? 0 : x.length() + y.length(); }
  static int    unsigned_quo_result_length(Integer& x, Integer& y)	{ return max(x.length() - y.length() + 1, 0); }
  static int    unsigned_rem_result_length(Integer& x, Integer& y)	{ return y.length(); }
  
  static void   unsigned_add(Integer& x, Integer& y, Integer& z);
  static void   unsigned_sub(Integer& x, Integer& y, Integer& z);
  static void   unsigned_mul(Integer& x, Integer& y, Integer& z);
  static void   unsigned_quo(Integer& x, Integer& y, Integer& z);
  static void   unsigned_rem(Integer& x, Integer& y, Integer& z);
  static int    unsigned_cmp(Integer& x, Integer& y            );
  static void   signed_div  (Integer& x, Integer& y, Integer& z);
  static void   signed_mod  (Integer& x, Integer& y, Integer& z);

  static int    last_non_zero_index(Digit* z, int lastIndex);
  static Digit  scale(Digit* array, Digit factor, int length);
  static bool   sd_all_zero(Digit* digits, int start, int stop);
  static Digit* copyDigits(Digit* source, int length, int toCopy);
  static Digit* qr_decomposition(Integer& x, Integer& y);
  static Digit  qr_estimate_digit_quotient(Digit& xhi, Digit xlo, Digit y);
  static Digit* qr_decomposition_single_digit(Digit* qr, int length, Digit divisor);
  static Digit  qr_calculate_remainder(Digit* qr, Digit* divisor, Digit q, int qrStart, int stop);
  static Digit  qr_adjust_for_underflow(Digit* qr, Digit* divisor, Digit q, int qrStart, int stop);
  static Digit  qr_adjust_for_over_estimate(Digit y1, Digit y2, Digit q, Digit xi, Digit xi2);
  static Digit  qr_scaling_factor(Digit firstDivisorDigit);
  static void   qr_unscale_remainder(Digit* qr, Digit scalingFactor, int length);
  static Digit  last_digit (Integer& x, Digit b);			// divides x by b and returns x mod b
  static void   first_digit(Integer& x, Digit b, Digit c);		// multiplies x by b and adds c

  // the following functions return the maximum result size
  // in bytes for the operation specified in the function name
  static int  add_result_size_in_bytes(Integer& x, Integer& y);
  static int  sub_result_size_in_bytes(Integer& x, Integer& y);
  static int  mul_result_size_in_bytes(Integer& x, Integer& y);
  static int  quo_result_size_in_bytes(Integer& x, Integer& y);
  static int  rem_result_size_in_bytes(Integer& x, Integer& y);
  static int  div_result_size_in_bytes(Integer& x, Integer& y);
  static int  mod_result_size_in_bytes(Integer& x, Integer& y);

  static int  and_result_size_in_bytes(Integer& x, Integer& y);
  static int  or_result_size_in_bytes (Integer& x, Integer& y);
  static int  xor_result_size_in_bytes(Integer& x, Integer& y);
  static int  ash_result_size_in_bytes(Integer& x, int      n);

  static void and_both_negative(Integer& x, Integer& y, Integer& z);
  static void and_one_positive(Integer& x, Integer& y, Integer& z);
  static void xor_one_positive(Integer& positive, Integer& negative, Integer& z);
  static int  copy_result_size_in_bytes(Integer& x);
  static int  int_to_Integer_result_size_in_bytes(int i);
  static int  unsigned_int_to_Integer_result_size_in_bytes(unsigned int i);
  static int  double_to_Integer_result_size_in_bytes(double x);
  static int  string_to_Integer_result_size_in_bytes(char* s, int base);
  static int  Integer_to_string_result_size_in_bytes(Integer& x, int base);

  // arithmetic/binary operations & tests
  static void add(Integer& x, Integer& y, Integer& z);	// z := x + y
  static void sub(Integer& x, Integer& y, Integer& z);	// z := x - y
  static void mul(Integer& x, Integer& y, Integer& z);	// z := x * y
  static void quo(Integer& x, Integer& y, Integer& z);	// z := x quo y
  static void rem(Integer& x, Integer& y, Integer& z);	// z := x rem y
  static void div(Integer& x, Integer& y, Integer& z);	// z := x div y
  static void mod(Integer& x, Integer& y, Integer& z);	// z := x mod y

  static void and(Integer& x, Integer& y, Integer& z);	// z := x and y, bitwise, assuming 2's complement representation
  static void or (Integer& x, Integer& y, Integer& z);	// z := x or  y, bitwise, assuming 2's complement representation
  static void xor(Integer& x, Integer& y, Integer& z);	// z := x xor y, bitwise, assuming 2's complement representation
  static void ash(Integer& x, int      n, Integer& z);	// z := x * 2^n

  static int  cmp(Integer& x, Integer& y            );	// returns < 0 for x < y; 0 for x = y; > 0 for x > y

  static void abs(Integer& x);				// x := |x|
  static void neg(Integer& x);				// x := -x
  static int  hash(Integer& x);

  // copy & conversion operations
  static void copy(Integer& x, Integer& z);
  static void int_to_Integer(int i, Integer& z);
  static void unsigned_int_to_Integer(unsigned int i, Integer& z);
  static void double_to_Integer(double x, Integer& z);
  static void string_to_Integer(char* s, int base, Integer& z);
  static void Integer_to_string(Integer& x, int base, char* s);
  // testing/debugging
  static void self_test();
};
