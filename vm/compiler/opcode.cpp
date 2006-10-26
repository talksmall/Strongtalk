/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.14 $ */
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

#ifdef DELTA_COMPILER

# include "incls/_opcode.cpp.incl"

char* BranchOpName[] = {
  "B=",    "B#",
  "B<",    "B<=",    "Bu<",    "Bu<=",
  "B>",    "B>=",    "Bu>",    "Bu>=",
  "Bovfl", "Bnofvl",

  "last (INVALID)"
};


char* ArithOpName[] = {
  "nil (INVALID)",
  "test",

  // untagged operations
  "+",    "-",    "*",    "div",    "mod",
  "and",  "or",   "xor",  "shift",  "cmp",

  // tagged operations
  "t+",   "t-",   "t*",   "tdiv",   "tmod",
  "tand", "tor",  "txor", "tshift", "tcmp",

  // untagged float operations
  "f+",   "f-",   "f*",   "fdiv",   "fmod",
  "fcmp",

  "fneg", "fabs", "f^2", "f2oop", 

  // tagged float operation
  "f2float",

  "last (INVALID)"
};


bool ArithOpIsCommutative[] = {
  false,
  true,

  // untagged operations
  true,  false, true,  false, false,
  true,  true,  true,  false, false,

  // tagged operations
  true,  false, true,  false, false,
  true,  true,  true,  false, false,

  // untagged float operations
  true,  false, true,  false, false,
  false,

  false, false, false, false,

  // tagged float operation
  false,

  false
};


void opcode_init() {
  if (sizeof(BranchOpName) / sizeof(char*) != LastBranchOp + 1) {
    fatal("forgot to change BranchOpName after changing BranchOpCode");
  }
  if (sizeof(ArithOpName) / sizeof(char*) != LastArithOp + 1) {
    fatal("forgot to change ArithOpName after changing ArithOpCode");
  }
  if (sizeof(ArithOpIsCommutative) / sizeof(bool) !=  LastArithOp + 1) {
    fatal("forgot to change ArithOpIsCommutative after changing ArithOpCode");
  }
}

#endif
