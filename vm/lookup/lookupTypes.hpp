/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.4 $ */
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

#ifdef junk

enum BaseLookupType {
  NormalLookupType,  SelfLookupType, SuperLookupType
};

enum CountType {
  NonCounting,	    	// no counting at all
  Counting, 	    	// incrementing a counter
  Comparing 	    	// increment & test for reaching limit (recompilation)
};


typedef int LookupType;

const int LookupTypeSize = 2;
const int LookupTypeMask = 3;

const int CountTypeMask = NonCounting | Counting | Comparing;
const int CountTypeSize = 2;
const int CountSendBit  = LookupTypeSize + 1;

// the dirty bit records whether the inline cache has ever made a transition 
// from non-empty to empty (e.g. through flushing)
const int DirtySendBit  = CountSendBit + CountTypeSize;
const int DirtySendMask = 1 << DirtySendBit;

// the optimized bit says that if no callee nmethod exists, an optimized
// method should be created immediately rather than going through an
// unoptimized version first
const int OptimizedSendBit  = DirtySendBit + 1;
const int OptimizedSendMask = 1 << OptimizedSendBit;

// the uninlinable bit says that the SIC has decided it's not worth
// inlining this send no matter how often it is executed.
const int UninlinableSendBit  = OptimizedSendBit + 1;
const int UninlinableSendMask = 1 << UninlinableSendBit;

inline LookupType withoutExtraBits(LookupType lookupType) {
  return lookupType & LookupTypeMask;
}
inline LookupType withCountBits(LookupType l, CountType t) {
  return LookupType((int(l) & ~(CountTypeMask << CountSendBit))
		    | (t << CountSendBit));
}

inline CountType countType(LookupType l) {
  return CountType((int(l) >> CountSendBit) & CountTypeMask);
}

extern "C" {
  void printLookupType(LookupType lookupType);
  char* lookupTypeName(LookupType lookupType);
}
#endif