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
  *(int64_t*)&low  = value;
}

long_int long_int::operator +(long_int arg) {
  return long_int(*(int64_t*)&low + *(int64_t*)&arg.low);
}

long_int long_int::operator -(long_int arg) {
  long_int result = *(int64_t*)&low - *(int64_t*)&arg.low;
  return result;
}

bool long_int::operator ==(long_int arg) {
  return *(int64_t*)&low == *(int64_t*)&arg.low;
}

bool long_int::operator !=(long_int arg) {
  return *(int64_t*)&low != *(int64_t*)&arg.low;
}

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4244 ) // conversion between __int64/double, possible loss of data
#endif

double long_int::as_double() {
  return double(*(int64_t*)&low);
}

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif
