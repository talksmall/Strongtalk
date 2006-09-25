/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.13 $ */
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

// register allocator for the Compiler
// currently a simple usage count thing

# ifdef DELTA_COMPILER

class RegisterAllocator;
extern RegisterAllocator* theAllocator;

class RegisterAllocator : public ResourceObj {
 private:
  IntFreeList*		_stackLocs;

 public:
  RegisterAllocator();

  void preAllocate(PReg* r);
  void allocate(GrowableArray<PReg*>* globals);
  bool allocateConst(ConstPReg* r, Location preferred = unAllocated);
  int  nofStackTemps()	{ return _stackLocs->length(); }
};

  
// helper structure for local register allocation
class RegCandidate : public ResourceObj{
 public:
  PReg* r;	    	// preg to be allocated
  Location loc;   	// possible location for it
  int ndefs;	    	// required # defs of loc

  RegCandidate(PReg* reg, Location l, int n) { r = reg; loc = l; ndefs = n; }
};

class RegisterEqClass : public ResourceObj {
 public:
  // holds pregs belonging to equivalence class
  // PRegs are linked through regClassLink
  PReg* first;
  PReg* last;
   
  RegisterEqClass(PReg* f);
  void append(PReg* other);
};

# endif
