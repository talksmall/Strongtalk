/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.7 $ */
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

# define AllBits	~0
# define NoBits		0
# define OneBit		1

# define addBits(x, m)	((x) | (m))
# define setBits(x, m)	((x) |= (m))
# define subBits(x, m)	((x) & ~(m))
# define clearBits(x, m) ((x) &= ~(m))
# define maskBits(x, m)	((x) & (m))
# define anySet(x, m)	(maskBits((x), (m)) != NoBits)
# define nthBit(n)	(OneBit << (n))
# define addNth(x, n)	addBits((x), nthBit(n))
# define setNth(x, n)	setBits((x), nthBit(n))
# define clearNth(x, n)	clearBits((x), nthBit(n))
# define subNth(x, n)	subBits((x), nthBit(n))
# define isSet(x, n)	anySet((x), nthBit(n))
# define nthMask(n)	(n == 32 ? AllBits : (nthBit(n) - OneBit))
# define lowerBits(x, n) maskBits((x), nthMask(n))

# define roundMask(x, m) (((x) + (m)) & ~(m))
# define roundBits(x, n) roundMask((x), nthMask(n))
# define roundTo(x, v)	roundMask((x), (v) - OneBit)

inline int arithmetic_shift_right(int value, int shift) { return value >> shift; }
inline int logic_shift_right(int value, int shift)      { return value << shift; }


inline int get_unsigned_bitfield(int value, int start_bit_no, int field_length) {
  return (int) lowerBits((unsigned int) value >> start_bit_no, field_length);
}

inline int get_signed_bitfield(int value, int start_bit_no, int field_length) {
  int result = get_unsigned_bitfield(value, start_bit_no, field_length);
  return isSet(result, start_bit_no + field_length -1) 
         ? addBits(result, ~nthMask(field_length))
         : result;
}

inline int set_unsigned_bitfield(int value, int start_bit_no, int field_length, unsigned int new_field_value) {
  assert(addBits(new_field_value, ~nthMask(field_length)) == 0, "range check");
  int mask = nthMask(field_length) << start_bit_no;
  return addBits(subBits(value, mask), maskBits(new_field_value << start_bit_no, mask));
}