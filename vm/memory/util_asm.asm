; Copyright 1996 LongView Technologies L.L.C. $Revision: 1.1 $
;Copyright (c) 2006, Sun Microsystems, Inc.
;All rights reserved.

;Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
;following conditions are met:

;    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
;    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
;	  disclaimer in the documentation and/or other materials provided with the distribution.
;   * Neither the name of Sun Microsystems nor the names of its contributors may be used to endorse or promote products derived 
;	  from this software without specific prior written permission.

;THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT 
;NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
;THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
;(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
;INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
;OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE


P386
MODEL	OS_NT FLAT
CODESEG						; open code segment

oopSize   EQU 4

;
; void copy_oops_up(oop* from, oop* to, int count)
;

PUBLIC _copy_oops_up
_copy_oops_up proc near
	mov eax, [esp+04]	; from
	mov ecx, [esp+08]	; to
	mov edx, [esp+12]	; count
	push ebx
	push esi
	sub ecx, eax		; to - from
	lea edx, [eax+edx*4]	; end


	; while (count >= 4)

	jmp	SHORT $2
	
$1:	mov	ebx, [eax-16]
	mov	esi, [eax-12]
	mov	[eax+ecx-16], ebx
	mov	[eax+ecx-12], esi

	mov	ebx, [eax-08]
	mov	esi, [eax-04]
	mov	[eax+ecx-08], ebx
	mov	[eax+ecx-04], esi
$2:	lea	eax, [eax+oopSize*4]
	cmp	eax, edx
	jle	SHORT $1

	lea	eax, [eax-oopSize*4]
$3:	mov	ebx, [eax]
	cmp	eax, edx
	mov	[eax+ecx], ebx
	lea	eax, [eax+oopSize]
	jb	SHORT $3

	pop esi
	pop ebx
	ret
endp

;
; void set_oops(oop* to, int count, oop value)
;
PUBLIC _set_oops
_set_oops proc near
	mov eax, [esp+04]	; from
	mov edx, [esp+08]	; count
	mov ecx, [esp+12]	; value
	dec edx

$4:	mov [eax+edx*4], ecx
	dec edx
	jge SHORT $4

	ret
endp

ENDS

END




