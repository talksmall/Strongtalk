/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.4 $ */
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

/********** This file is not used right now; I'm leaving it in in case
 ********** the code is ever needed (It looks like fast process switching
 ********** code, which might be handy someday)
 */

# include "incls/_process_asm.cpp.incl"

void Process::transfer() {
  processSemaphore = true;

  previousProcess = currentProcess;

  asm mov edi, large dword ptr this;

  // save sp of currentProcess
  asm mov EAX, large dword ptr currentProcess;
  asm mov [EAX+4], ESP;
  asm mov [EAX+8], EBP;

  asm mov large dword ptr currentProcess, edi;

  // save ret addr of currentProcess
  asm mov EBX, [EBP+4];
  asm mov [EAX+12], EBX;

  // get callee pc
  asm mov EAX, large dword ptr currentProcess;
  asm mov EBX, [EAX+12];  

  // restore callee sp
  asm mov EBP, [EAX+8];  
  asm mov ESP, [EAX+4];  

  // Clear the semaphore and jump
  asm mov large dword ptr processSemaphore, 0x0;
  asm jmp EBX;
}

