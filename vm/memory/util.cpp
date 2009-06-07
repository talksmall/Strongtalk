/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.21 $ */
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
# include "incls/_util.cpp.incl"
# include <string.h>

int Indent = 0;

void printIndent() {
  for (int i = 0; i < Indent; i++) lprintf("  ");
}


# define LOOP_UNROLL(count, body)                                             \
    {                                                                         \
    assert(count >= 0, "cannot have negative count in loop unroll");          \
    int __c1__ = count;                                                      \
    for (int __c__ = __c1__ >> 3; __c__; __c__ --) {                         \
    body;       body;                                                         \
    body;       body;                                                         \
    body;       body;                                                         \
    body;       body;                                                         \
  }                                                                           \
    switch (maskBits(__c1__, nthMask(3))) {                                   \
   case 7:      body;                                                         \
   case 6:      body;                                                         \
   case 5:      body;                                                         \
   case 4:      body;                                                         \
   case 3:      body;                                                         \
   case 2:      body;                                                         \
   case 1:      body;                                                         \
   case 0:      ;                                                             \
  } }

# define DO_UP(from) LOOP_UNROLL(count, *to++ = from)
# define DO_DOWN(from) LOOP_UNROLL(count, *--to = from)

void copy_oops_up(oop* from, oop* to, int count) {
  assert(maskBits(int(from), Tag_Size) == 0, "not word aligned");
  assert(maskBits(int(to), Tag_Size) == 0, "not word aligned");
  assert(count >= 0, "negative count");

  // block_step was determined by profiling the scavenger.
  //  11/14-95 (Robert and Lars)
  const int block_step = 4;
  
  while (count >= block_step) {
    *(to+0) = *(from+0);
    *(to+1) = *(from+1);
    *(to+2) = *(from+2);
    *(to+3) = *(from+3);
    to    += block_step;
    from  += block_step;
    count -= block_step;
  }

  if (count > 0) {
    *(to+0) = *(from+0);
    if (count > 1) {
      *(to+1) = *(from+1);
      if (count > 2) {
        *(to+2) = *(from+2);
      }
    }
  }
}

void copy_oops_down(oop* from, oop* to, int count) {
  assert(maskBits(int(from), Tag_Size) == 0, "not word aligned");
  assert(maskBits(int(to), Tag_Size) == 0, "not word aligned");
  assert(count >= 0, "negative count");
  DO_DOWN(*--from)
  }


void set_oops(oop* to, int count, oop value) {
  assert(maskBits(int(to), Tag_Size) == 0, "not word aligned");
  assert(count >= 0, "negative count");

  const int block_step = 4;
  
  while (count >= block_step) {
    *(to+0) = value;
    *(to+1) = value;
    *(to+2) = value;
    *(to+3) = value;
    to    += block_step;
    count -= block_step;
  }

  if (count > 0) {
    *(to+0) = value;
    if (count > 1) {
      *(to+1) = value;
      if (count > 2) {
        *(to+2) = value;
      }
    }
  }
}

char* copy_string(char* s) {
  int len = strlen(s) + 1;
  char* str = NEW_RESOURCE_ARRAY( char, len);
  strcpy(str, s);
  return str;
}

char* copy_c_heap_string(char* s) {
  int len = strlen(s) + 1;
  char* str = NEW_C_HEAP_ARRAY( char, len);
  strcpy(str, s);
  return str;
}

char* copy_string(char* s, smi len) {
  char* str = NEW_RESOURCE_ARRAY( char, len+1);
  memcpy(str, s, len+1);
  str[len] = '\0';
  return str;
}

oop catchThisOne;

void breakpoint() {
  flush_logFile();
  os::breakpoint();
}

void error_breakpoint() {
  breakpoint();
}
