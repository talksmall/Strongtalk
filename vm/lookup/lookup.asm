; Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.3 $
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
CODESEG

;-----------------------------------------------------------------------------------------
; _lookup
;
;   parameters:
;     eax = class    (klassOop)
;     edx = selector (symbolOop)
;
;   returns:
;     eax = a methodOop or a jump table entry for a nmethod.
;
;   contract:
;     edi is not changed and
;     ebx will remain unchanged or contain 0x000000xx at return.
;
; TODO list
;  - inline the first part of _lookup in the interpreter making first hit 8 instructions.
;  - adjust the parameter registers to the insterpreter.

EXTRN	_lookup_in_klass:		PROC
EXTRN	_primary_cache:			PTR DWORD
EXTRN   _primary_cache_result:  	PTR DWORD
EXTRN	_secondary_cache:		PTR DWORD
EXTRN   _secondary_cache_result:	PTR DWORD

PUBLIC _lookup
_lookup PROC near
	mov	eax, ecx				; move class to accum
	xor	eax, edx				; xor class and selector
	and	eax, 03ffh				; compute offset in primary % 1024

	cmp	ecx, _primary_cache[eax*8]		; check class
	jne	primary_miss

	cmp	edx, _primary_cache[eax*8+4]		; check selector 
	jne	primary_miss

	mov	eax, _primary_cache_result[eax*4]	; get result
	ret

primary_miss:
	; edi is the primary offset
	mov	ebx, edi
	and	ebx, 07fh				; compute offset in secondary % 128

	mov	ecx, _secondary_cache[ebx*8]		; check class
	jne	secondary_miss

	cmp	edx, _secondary_cache[ebx*8+4]		; check selector 
	jne	secondary_miss

	; swap the entries
	mov	eax, _primary_cache[edi*8]		; class swap
	mov	ecx, _secondary_cache[ebx*8]
	mov	_secondary_cache[ebx*8], eax
	mov	_primary_cache[edi*8], ecx

	mov	eax, _primary_cache[edi*8+4]		; selector swap
	mov	ecx, _secondary_cache[ebx*8+4]
	mov	_secondary_cache[ebx*8+4], eax
	mov	_primary_cache[edi*8+4], ecx

	mov	eax, _primary_cache_result[edi*4]	; result swap
	mov	ecx, _secondary_cache_result[ebx*4]
	mov	_secondary_cache_result[ebx*4], eax
	mov	_primary_cache_result[edi*4], ecx

	xor	ebx, ebx				; clear ebx
	ret

secondary_miss:
	push	edi					; save edi for the interpreter
	CALL	_lookup_in_klass C, ecx, edx, edi, ebx
	pop	edi
	xor	ebx, ebx				; clear ebx for the interpreter
	ret
ENDP
ENDS
END
