/* Copyright 1994-1996, LongView Technologies L.L.C. $Revision: 1.1 $ */
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
# include "incls/_intervalInfo.cpp.incl"

#ifdef DELTA_COMPILER

IntervalInfo::IntervalInfo(MethodInterval* interval, InlinedScope* scope) {
  _interval = interval; _scope = scope;
}

bool IntervalInfo::isParentOf(IntervalInfo* other) const {
  MethodInterval* mi = other->interval()->parent();
  while (mi && mi != _interval) mi = mi->parent();
  return mi != NULL;
}

bool IntervalInfo::dominates(int bci, IntervalInfo* other, int otherBCI) const {
  // "x dominates y" --> "if y is executed, x was executed earlier"
  // Bytecode bci1 in interval i1 dominates bytecode bci2 in interval i2 if 
  // - i1 == i2 && bci1 <= bci2  (i1 == i2 implies straight-line code)
  // - i1 is a parent of i2 && bci1 < bci2
  //   (e.g., i1 is a method and i2 a loop body or if condition)
  if (this == other && bci <= otherBCI) return true;
  if (isParentOf(other) && bci < otherBCI) return true;
  return false;
}

void IntervalInfo::print() {
  lprintf("((IntervalInfo*)%#x\n", this);
}

#endif