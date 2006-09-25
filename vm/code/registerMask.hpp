/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.5 $ */
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

// A RegisterMask tells the scavenger which registers / stack locs are
// live.  (One bit per location.)

# ifdef DELTA_COMPILER_unused

inline RegisterMask& allocate(RegisterMask& s1, RegisterMask s2) {
  setBits(s1, s2);
  return s1;
}

inline bool isAllocated(RegisterMask s, Location l) {
  Unimplemented();
  // return isSet(s, l);
  return false;
}

inline RegisterMask& deallocate(RegisterMask& s1, RegisterMask s2) {
  clearBits(s1, s2);
  return s1;
}

inline RegisterMask& allocateRegister(RegisterMask& s, Location r) {
  Unimplemented();
  // if (isRegister(r)) setNth(s, r);
  return s;
}

inline RegisterMask& deallocateRegister(RegisterMask& s1, Location r) {
  Unimplemented();
  // assert(isRegister(r), "not a register");
  // clearNth(s1, r);
  return s1;
}

Location pick(RegisterMask& alloc, RegisterMask mask = ~0);

void printRegister(Location r);
void printAllocated(RegisterMask r);

  // like a RegisterMask, but has arbitrary length (i.e., bits for
  // stack temps and regs)

  class LongRegisterMask : ResourceObj {
    BitVector* bv;
   public:
    LongRegisterMask();
    void allocate  (Location l);
    void deallocate(Location l);
    bool isAllocated(Location l);
    RegisterMask regs();		// returns mask for registers
    void print();
    
   private:
    void grow();
    friend int findFirstUnused(LongRegisterMask** masks, int len,
				int start);
  };

  Location findFirstUnusedTemp(LongRegisterMask** masks, int len);
# endif
    
