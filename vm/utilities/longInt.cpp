/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.1 $ */
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
# include "incls/_longInt.cpp.incl"

inline long_int::long_int() {}

long_int::long_int(unsigned int low, unsigned int high) {
  this->low  = low;
  this->high = high;
}

long_int::long_int(double value) {
  long_int result = double_conversion(value);
  low  = result.low;
  high = result.high;
}

long_int long_int::operator +(long_int arg) {
  long_int receiver = *this;
  long_int result;
  /*
  __asm {
    mov eax, receiver.low
    mov edx, receiver.high
    add eax, arg.low
    adc edx, arg.high
    mov result.low,  eax
    mov result.high, edx
  }
  */
  *(int64_t*)&result = *(int64_t*)&receiver + *(int64_t*)&arg;
  return result;
}

long_int long_int::operator -(long_int arg) {
  long_int receiver = *this;
  long_int result;
/*  __asm {
    mov eax, receiver.low
    mov edx, receiver.high
    sub eax, arg.low
    sbb edx, arg.high
    mov result.low,  eax
    mov result.high, edx
  }*/
  *(int64_t*)&result = *(int64_t*)&receiver - *(int64_t*)&arg;
  return result;
}

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4244 ) // conversion between __int64/double, possible loss of data
#endif

double long_int::as_double() {
  long_int receiver = *this;  
  double result;
/*  __asm {
    fild receiver
    fstp result
  }
*/
  result = *(int64_t*)&receiver;
  return result;
}

long_int long_int::double_conversion(double value) {
  register long_int result;
/*  __asm {
    fld   value
    fistp result
  }*/
  *(int64_t*)&result = value; // %note: will use C rounding, not equivalent to the assembly
  return result;
}

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif
