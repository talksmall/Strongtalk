/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.9 $ */
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

# ifdef DELTA_COMPILER_unused

# include "incls/_registerMask.cpp.incl"

Location pick(RegisterMask& alloc, RegisterMask mask) {
  Unimplemented();
  unsigned r = mask & ~alloc;
  if (r == 0) return unAllocated;
  for (int reg = 0; ! isSet(r, 0); reg ++, r >>= 1) ;
  setNth(alloc, reg);
  // return Location(ireg, reg); /// fix this
  return Location();
}
 
void printAllocated(RegisterMask rs) {
  Unimplemented();
  /*
  printf("{");
  bool first = true;
  unsigned r = rs;		// safer for >>
  for (int d = 0; r; d++, r >>= 1) {
    if (isSet(r, 0)) {
      if (first) {
	first = false;
      } else {
	printf(",");
      }
      printf("%s", RegisterNames[d]);
      Unimplemented();
      // Location d1 = Location(d); <<< fix this
      Location d1;
      for (char c = RegisterNames[d][0];
	   isSet(r, 1) && c == RegisterNames[d + 1][0];
	   d ++, r >>= 1)
	;
      if (d > d1.no()) printf("-%s", RegisterNames[d]);
    }
  }
  printf("}");
  fflush(stdout);
  */
}


inline int tempToIndex(Location temp) { Unimplemented(); return 0;
  // return temp - FirstStackLocation + 32;
}

inline Location indexToTemp(int temp) { Unimplemented(); return Location(); 
  // return Location(temp + FirstStackLocation - 32);
}

LongRegisterMask::LongRegisterMask() {
  bv = new BitVector(128);
}

void LongRegisterMask::allocate(Location l) {
  if (l.isRegister()) {
    bv->add(l.number());
  } else {
    assert(l.isStackLocation(), "should be stack reg");
    int i = tempToIndex(l);
    if (i >= bv->length) grow();
    bv->add(i);
  }
}

void LongRegisterMask::deallocate(Location l) {
  if (l.isRegister()) {
    bv->remove(l.number());
  } else {
    assert(l.isStackLocation(), "should be stack reg");
    int i = tempToIndex(l);
    bv->remove(i);
  }
}

bool LongRegisterMask::isAllocated(Location l) {
  if (l.isRegister()) {
    return bv->includes(l.number());
  } else {
    assert(l.isStackLocation(), "should be stack reg");
    int i = tempToIndex(l);
    if (l.number() < bv->length) {
      return bv->includes(i);
    } else {
      return false;
    }
  }
}

RegisterMask LongRegisterMask::regs() {
  return bv->bits[0];
}

void LongRegisterMask::grow() {
  bv = bv->copy(bv->length * 2);
}

void LongRegisterMask::print() {
}


// find the first bit >= start that is unused in all strings[0..len-1]
int findFirstUnused(LongRegisterMask** masks, int len,
		    int start) {
  // currently quite unoptimized
  BitVector* b = masks[0]->bv->copy(masks[0]->bv->maxLength);
  for (int i = 1; i < len; i++) {
    b->unionWith(masks[i]->bv);
  }
    for (i = start; i < b->length; i++) {
      if (!b->includes(i)) break;
    }
  return i;
}

Location findFirstUnusedTemp(LongRegisterMask** masks, int len) {
  Unimplemented();
  // int i = findFirstUnused(masks, len, tempToIndex(FirstStackLocation));
  // return indexToTemp(i);
  return Location();
}

# endif
