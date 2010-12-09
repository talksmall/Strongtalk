/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.35 $ */
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

#include "incls/_precompiled.incl"
#ifdef DELTA_COMPILER
#include "incls/_disassembler.cpp.incl"

#include <inttypes.h>

#define DISASM_LIBRARY  "libnasm.dylib"
#define DISASM_FUNCTION "disasm"

typedef int32_t (*disasm_f) (uint8_t *, char *, int, int, int32_t, int, uint32_t);
static disasm_f disassemble;

static bool library_loaded;

/* parameters for the NASM disassembler */
static int32_t   offset    = 0;
static int       autosync  = 0;
static uint32_t  prefer    = 0;    // select instruction set; 0 = Intel (default)  

static char outbuf[256];

static void initialize(void) {
  DLL* library_handle;
  library_handle = os::dll_load(DISASM_LIBRARY);
  if (library_handle == NULL) {
    return;
  }
  disassemble = (disasm_f) os::dll_lookup(DISASM_FUNCTION, library_handle);
  if (disassemble == NULL) {
    return;
  }
  library_loaded = true;
}

static char tohex(unsigned char c) {
  char* digits="0123456789ABCDEF";
  if (c > 0xf) return '?';
  return digits[c];
}

static void disasm(char* begin, char* end, outputStream* st) {
  char* pos;
  char ch, hexcode[256];
  int lendis, i;      
  
  if (!library_loaded) {
    initialize();
  }
  if (disassemble) {
    pos = begin;
    while (pos < end) {
      lendis = disassemble((uint8_t*) pos, outbuf, sizeof(outbuf), 32, offset, autosync, prefer);
      if (lendis) {
        for (i = 0; i < lendis; i++) {
          ch = *(pos+i);
          hexcode[2*i]   = tohex((ch & 0xF0) >> 4);
          hexcode[2*i+1] = tohex(ch & 0x0F);
        }
        hexcode[2*i] = '\0';
        st->print_cr("    %p %-20s    %s", pos, hexcode, outbuf);
      }
      pos += lendis;
    }
  } else {
    st->print_cr("INFO: no disassemble() function available!");
  }  
}


void Disassembler::decode(nmethod* nm, outputStream* st) {
  disasm(nm->insts(), nm->instsEnd(), st);
}

void Disassembler::decode(char* begin, char* end, outputStream* st) {
  disasm(begin, end, st);
}

#endif
