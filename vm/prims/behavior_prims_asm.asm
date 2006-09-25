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

; Imported 

nil_obj       EQU _nilObj

eden_top      EQU _eden_top
eden_end      EQU _eden_end

last_fp       EQU _last_Delta_fp
last_sp       EQU _last_Delta_sp

klass	      EQU dword ptr [esp+4]

scavenge_and_allocate EQU _scavenge_and_allocate

EXTRN   nil_obj:       PTR DWORD
EXTRN   eden_top:      PTR DWORD
EXTRN   eden_end:      PTR DWORD

EXTRN   last_fp:      PTR DWORD
EXTRN   last_sp:      PTR DWORD

EXTRN	scavenge_and_allocate: PROC

oopSize   EQU 4

scavenge MACRO size
	mov		DWORD PTR [last_fp], ebp
	add		esp, 4
	mov		DWORD PTR [last_sp], esp
	sub		esp, 4
	push		size
	call		scavenge_and_allocate
	add		esp, 4
	mov		DWORD PTR [last_fp], 0
	add		eax, size * oopSize
ENDM

primitive_new MACRO size, inst
LOCAL _A
LOCAL _B
LOCAL i
 	mov		eax, [eden_top]
	mov		ebx, klass
	add		eax, size * oopSize
	cmp		eax, [eden_end]
	jg		SHORT _A
	mov		[eden_top], eax

_B:	
	mov		DWORD PTR [eax-((size-1) * oopSize)], ebx       ; obj->set_klass(klass)
	mov		ebx, 80000003H
	mov		DWORD PTR [eax-(size * oopSize)], ebx		; obj->init_mark()

IF	inst NE 0
	mov		ebx, [nil_obj]
ENDIF
i	= 0
REPT	inst
	mov		DWORD PTR [eax-((inst-i) * oopSize)], ebx
i	= i+1
ENDM
	sub		eax, (size * oopSize) - 1
	ret
  
_A: scavenge size
	mov		ebx, klass
	jmp		SHORT _B
ENDM

PUBLIC _primitiveNew0
_primitiveNew0 proc near
	primitive_new 2, 0
endp

PUBLIC _primitiveNew1
_primitiveNew1 proc near
	primitive_new 3, 1
endp

PUBLIC _primitiveNew2
_primitiveNew2 proc near
	primitive_new 4, 2
endp

PUBLIC _primitiveNew3
_primitiveNew3 proc near
	primitive_new 5, 3
endp

PUBLIC _primitiveNew4
_primitiveNew4 proc near
	primitive_new 6, 4
endp

PUBLIC _primitiveNew5
_primitiveNew5 proc near
	primitive_new 7, 5
endp

PUBLIC _primitiveNew6
_primitiveNew6 proc near
	primitive_new 8, 6
endp

PUBLIC _primitiveNew7
_primitiveNew7 proc near
	primitive_new 9, 7
endp

PUBLIC _primitiveNew8
_primitiveNew8 proc near
	primitive_new 10, 8
endp

PUBLIC _primitiveNew9
_primitiveNew9 proc near
	primitive_new 11, 9
endp

ENDS

END




