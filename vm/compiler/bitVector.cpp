/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.3 $ */
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

# ifdef DELTA_COMPILER

# include "incls/_bitVector.cpp.incl"

  bool BitVector::unionWith(BitVector* other) {
    while (length < other->length) bits[length++] = 0;
    assert(length <= maxLength, "grew too much");
    bool changed = false;
    for (int i = indexFromNumber(other->length-1); i >= 0; i--) {
      int old = bits[i];
      bits[i] |= other->bits[i];
      changed |= (old != bits[i]);
    }
    return changed;
  }

  bool BitVector::intersectWith(BitVector* other) {
    bool changed = false;
    for (int i = indexFromNumber(min(length, other->length)-1); i >= 0; i--) {
      int old = bits[i];
      bits[i] &= other->bits[i];
      changed |= (old != bits[i]);
    }
    return changed;
  }

  bool BitVector::isDisjointFrom(BitVector* other) {
    for (int i = indexFromNumber(min(length, other->length)-1); i >= 0; i--) {
      if ((bits[i] & other->bits[i]) != 0) return false;
    }
    return true;
  }

  void BitVector::addFromTo(int first, int last) {
    // mark bits [first..last]
    assert(first >= 0 && first < length, "wrong index");
    assert(last >= 0 && last < length, "wrong index");
    int startIndex = indexFromNumber(first);
    int   endIndex = indexFromNumber(last);
    if (startIndex == endIndex) {
      assert(last - first < BitsPerWord, "oops");
      int mask = nthMask(last - first + 1);
      bits[startIndex] |= mask << offsetFromNumber(first);
    } else {
      bits[startIndex] |= AllBits << offsetFromNumber(first);
      for (int i = startIndex + 1; i < endIndex; i++) bits[i] = AllBits;
      bits[endIndex] |= nthMask(offsetFromNumber(last) + 1);
    }
#   ifdef ASSERT
      for (int i = first; i <= last; i++)
	assert(includes(i), "bit should be set");
#   endif
  }

# ifdef UNUSED
  void BitVector::removeFromTo(int first, int last) {
    assert(first >= 0 && first < length, "wrong index");
    assert(last >= 0 && last < length, "wrong index");
    int startIndex = indexFromNumber(first);
    int   endIndex = indexFromNumber(last);
    if (startIndex == endIndex) {
      assert(last - first < BitsPerWord, "oops");
      int mask = ~nthMask(last - first + 1);
      bits[startIndex] &= mask << offsetFromNumber(first);
    } else {
      bits[startIndex] &= ~(AllBits << offsetFromNumber(first));
      for (int i = startIndex + 1; i < endIndex; i++) bits[i] = 0;
      bits[endIndex] &= ~nthMask(offsetFromNumber(last) + 1);
    }
#   ifdef ASSERT
      for (int i = first; i <= last; i++)
	assert(!includes(i), "bit shouldn't be set");
#   endif
  }
# endif

  void BitVector::print_short() { lprintf("BitVector %#lx", this); }
  
  void BitVector::doForAllOnes(intDoFn f) {
    for (int i = indexFromNumber(length-1); i >= 0; i--) {
      int b = bits[i];
      for (int j = 0; j < BitsPerWord; j++) {
	if (isSet(b, j)) {
	  f(i * BitsPerWord + j);
	  clearNth(b, j);
	  if (!b) break;
	}
      }
    }
  }
  
  void BitVector::print() {
    print_short();
    lprintf(": {");
    int last = -1;
    for (int i = 0; i < length; i ++) {
      if (includes(i)) {
	if (last < 0) {
	  lprintf(" %ld", i);	// first bit after string of 0s
	  last = i;
	}
      } else {
	if (last >= 0) lprintf("..%ld", i - 1);	// ended a group
	last = -1;
      }
    }
    if (last >= 0) lprintf("..%ld", i - 1);
    lprintf(" }");
  }
  
# endif
