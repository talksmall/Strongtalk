/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.3 $ */
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
# include "incls/_nativeInstruction.cpp.incl"


// Implementation of NativeCall

void NativeCall::verify() {
  // make sure code pattern is actually a call imm32 instruction
  if (*(u_char*)instruction_address() != instruction_code) fatal("not a call imm32");
}


void NativeCall::print() {
  std->print_cr("0x%x: call 0x%x", instruction_address(), destination());
}

// Implementation of NativeMov
void NativeMov::verify() {
  // make sure code pattern is actually a mov reg, imm32 instruction
  if ((*(u_char*)instruction_address() & ~register_mask) != instruction_code)
    fatal("not a mov reg, imm32");
}


void NativeMov::print() {
  std->print_cr("0x%x: mov reg, 0x%x", instruction_address(), data());
}

// Implementation of NativeTest
void NativeTest::verify() {
  // make sure code pattern is actually a test eax, imm32 instruction
  if (*(u_char*)instruction_address() != instruction_code) fatal("not a test eax, imm32");
}


void NativeTest::print() {
  std->print_cr("0x%x: test eax, 0x%x", instruction_address(), data());
}

