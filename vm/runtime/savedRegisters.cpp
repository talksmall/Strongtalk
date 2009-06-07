/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.5 $ */
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
#include "incls/_savedRegisters.cpp.incl"


// Need to be static so they can be accessed in assembly code
// of SavedRegisters::save_registers() (compiler doesn't accept
// static class variables).
static int* stored_frame_pointer = NULL;
static oop  saved_eax;
static oop  saved_ecx;
static oop  saved_edx;
static oop  saved_ebx;
static oop  saved_esi;
static oop  saved_edi;


oop SavedRegisters::fetch(int register_number, int* frame_pointer) {
  if (frame_pointer != stored_frame_pointer) {
    std->print_cr("Cannot fetch register from non-bottom frame:");
    std->print_cr(" register number = %d, fp = 0x%lx", register_number, frame_pointer);
    fatal("vm aborted");
  }
  if (register_number == eax.number()) return saved_eax;
  if (register_number == ecx.number()) return saved_ecx;
  if (register_number == edx.number()) return saved_edx;
  if (register_number == ebx.number()) return saved_ebx;
  if (register_number == esi.number()) return saved_esi;
  if (register_number == edi.number()) return saved_edi;
  fatal("cannot fetch esp or ebp from saved registers");
  return NULL;
}


void SavedRegisters::clear() {
  stored_frame_pointer = NULL;
}

/*
#define Naked __declspec( naked )
Naked void SavedRegisters::save_registers() {
  __asm {
    // save the registers
    mov	saved_eax, eax
    mov	saved_ecx, ecx
    mov	saved_edx, edx
    mov	saved_ebx, ebx
    mov	saved_esi, esi
    mov	saved_edi, edi
    // save frame pointer w/o destroying any register contents
    mov eax, last_Delta_fp
    mov stored_frame_pointer, eax
    mov eax, saved_eax
    // return
    ret
  }
}
#undef Naked
*/

void SavedRegisters::generate_save_registers(MacroAssembler* masm)
{
  // save the registers
  masm->movl( Address((int)&saved_eax, relocInfo::external_word_type), eax );
  masm->movl( Address((int)&saved_ecx, relocInfo::external_word_type), ecx );
  masm->movl( Address((int)&saved_edx, relocInfo::external_word_type), edx );
  masm->movl( Address((int)&saved_ebx, relocInfo::external_word_type), ebx );
  masm->movl( Address((int)&saved_esi, relocInfo::external_word_type), esi );
  masm->movl( Address((int)&saved_edi, relocInfo::external_word_type), edi );
  // save frame pointer w/o destroying any register contents
  masm->movl(eax, Address((int)&last_Delta_fp, relocInfo::external_word_type));
  masm->movl(Address((int)&stored_frame_pointer, relocInfo::external_word_type), eax);
  masm->movl(eax, Address((int)&saved_eax, relocInfo::external_word_type));
  // return
  // %note: we don't return because the code is inlined in stubs -Marc 04/07
//  masm->ret();
}
