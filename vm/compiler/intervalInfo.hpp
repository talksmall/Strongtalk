/* Copyright 1994-1996, LongView Technologies L.L.C. $Revision: 1.2 $ */
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

#ifdef DELTA_COMPILER

// A IntervalInfo contains compiler-related data/functionality that is associated
// with a MethodInterval.  (It's not in MethodInterval itself to avoid cluttering
// it up.)
// The main purpose of IntervalInfo is to exploit the method structure for optimization
// (esp. to get a cheap "dominates" relationship).   
// This code is only halfway finished -- not used yet.  -Urs 9/96

class IntervalInfo: public PrintableResourceObj {
  MethodInterval* _interval;	  // my interval
  InlinedScope*   _scope;	  // my scope

 public:
  IntervalInfo(MethodInterval* interval, InlinedScope* scope);

  MethodInterval* interval() const		{ return _interval; }
  InlinedScope*   scope() const			{ return _scope; }

  bool dominates(int bci, IntervalInfo* other, int otherBCI) const;	
      // does bytecode (receiver, bci) dominate (other, otherBCI)?
  bool isParentOf(IntervalInfo* other) const;	
      // is receiver a parent of other?

  void print();
};

#endif
