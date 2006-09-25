/* Copyright 1994, LongView Technologies. $Revision: 1.1 $ */
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
# ifdef COMPILER

  // various Compiler functionality associated with splitting

  // A split signature encodes the position of a SplitPReg or a node;
  // 0 means the node/preg isn't involved in any split, 1 means it is in
  // the first split path, etc.

  const int MaxSplitDepth = 7;

  class SplitSig {
    // "this" encodes 7 split 4-bit IDs plus nesting level in lowest bits
    // this == 0 means top level, not in any split
    static const uint32 LevelMask;
   public:
    SplitSig() { ShouldNotCallThis(); }
    friend SplitSig* new_SplitSig(SplitSig* current, int splitID);

    int level()	{ return uint32(this) & LevelMask; }
    bool contains(SplitSig* other) {
      // other sig is in same branch iff the receiver is a prefix of other
      // NB: this is not symmetric, i.e. it's like <=, not ==
      int shift = (MaxSplitDepth - level() + 1) << 2;
      if (shift == 32) return true;	// because x >> 32 is undefined
      return ((uint32(this) ^ uint32(other)) >> shift) == 0;
    }
    void print();
    char* prefix(char* buf);
  };

# endif
