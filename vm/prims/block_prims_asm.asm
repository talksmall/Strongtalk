; Copyright 1996 LongView Technologies L.L.C. $Revision: 1.6 $
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

context_klass EQU _contextKlassObj
nil_obj       EQU _nilObj

eden_top      EQU _eden_top
eden_end      EQU _eden_end

last_fp       EQU _last_Delta_fp
last_sp       EQU _last_Delta_sp


block_klass_0       EQU _zeroArgumentBlockKlassObj
block_klass_1       EQU _oneArgumentBlockKlassObj
block_klass_2       EQU _twoArgumentBlockKlassObj
block_klass_3       EQU _threeArgumentBlockKlassObj
block_klass_4       EQU _fourArgumentBlockKlassObj
block_klass_5       EQU _fiveArgumentBlockKlassObj
block_klass_6       EQU _sixArgumentBlockKlassObj
block_klass_7       EQU _sevenArgumentBlockKlassObj
block_klass_8       EQU _eightArgumentBlockKlassObj
block_klass_9       EQU _nineArgumentBlockKlassObj

context_klass       EQU _contextKlassObj

scavenge_and_allocate EQU _scavenge_and_allocate

EXTRN	context_klass: PTR DWORD
EXTRN   nil_obj:       PTR DWORD
EXTRN   eden_top:      PTR DWORD
EXTRN   eden_end:      PTR DWORD

EXTRN   last_fp:      PTR DWORD
EXTRN   last_sp:      PTR DWORD

EXTRN   block_klass_0: PTR DWORD
EXTRN   block_klass_1: PTR DWORD
EXTRN   block_klass_2: PTR DWORD
EXTRN   block_klass_3: PTR DWORD
EXTRN   block_klass_4: PTR DWORD
EXTRN   block_klass_5: PTR DWORD
EXTRN   block_klass_6: PTR DWORD
EXTRN   block_klass_7: PTR DWORD
EXTRN   block_klass_8: PTR DWORD
EXTRN   block_klass_9: PTR DWORD

EXTRN   context_klass: PTR DWORD

EXTRN	scavenge_and_allocate: PROC

oopSize   EQU 4

scavenge MACRO size
	mov		DWORD PTR [last_fp], ebp
	add		esp, 4
	mov		DWORD PTR [last_sp], esp
	sub		esp, 4
	push	size
	call	scavenge_and_allocate
	add		esp, 4
	mov		DWORD PTR [last_fp], 0
	add		eax, size * oopSize
ENDM

allocate_block MACRO klass
LOCAL _A
LOCAL _B
 	mov		eax, [eden_top]
	mov		ebx, [klass]
	add		eax, 4 * oopSize
	cmp		eax, [eden_end]
	jg		SHORT _A
	mov		[eden_top], eax

_B:	
	mov		DWORD PTR [eax-(4 * oopSize)], 80000003H ; obj->init_mark()
	mov		DWORD PTR [eax-(3 * oopSize)], ebx       ; obj->set_klass(klass)
	; mov		DWORD PTR [eax-(2 * oopSize)], 0     ; obj->set_method(NULL)
	; mov		DWORD PTR [eax-(1 * oopSize)], 0     ; obj->set_lexical_scope(NULL)
	sub		eax, (4 * oopSize) - 1
	ret
  
_A: scavenge 4
	mov		ebx, [klass]
	jmp		SHORT _B
ENDM


PUBLIC _allocateBlock0
_allocateBlock0 proc near
	allocate_block block_klass_0
endp

PUBLIC _allocateBlock1
_allocateBlock1 proc near
	allocate_block block_klass_1
endp

PUBLIC _allocateBlock2
_allocateBlock2 proc near
	allocate_block block_klass_2
endp

PUBLIC _allocateBlock3
_allocateBlock3 proc near
	allocate_block block_klass_3
endp

PUBLIC _allocateBlock4
_allocateBlock4 proc near
	allocate_block block_klass_4
endp

PUBLIC _allocateBlock5
_allocateBlock5 proc near
	allocate_block block_klass_5
endp

PUBLIC _allocateBlock6
_allocateBlock6 proc near
	allocate_block block_klass_6
endp

PUBLIC _allocateBlock7
_allocateBlock7 proc near
	allocate_block block_klass_7
endp

PUBLIC _allocateBlock8
_allocateBlock8 proc near
	allocate_block block_klass_8
endp

PUBLIC _allocateBlock9
_allocateBlock9 proc near
	allocate_block block_klass_9
endp

PUBLIC _allocateContext
_allocateContext proc near
	mov		ecx, [esp+4]		; load length  (remember this is a smiOop)
 	mov		eax, [eden_top]
	mov		edx, ecx
	add		edx, (3*oopSize)
	add		edx, eax
	cmp		edx, [eden_end]
	jg		SHORT @10
	mov		[eden_top], edx

@11:
	mov		ebx, [context_klass]

	add		ecx, 4
	add		ecx, 80000003H
	mov		DWORD PTR [eax], ecx ; obj->init_mark()
	mov		ecx, [nil_obj]

	mov		DWORD PTR [eax+(1*oopSize)], ebx	; obj->set_klass(klass)
	mov		DWORD PTR [eax+(2*oopSize)], 0		; obj->set_home(NULL)
    lea		ebx, [eax+(3*oopSize)]
	jmp		@12    
@13:
	mov		DWORD PTR [ebx], ecx
	add		ebx, oopSize
@12:
	cmp		ebx, edx
	jl		@13
	inc		eax
	ret
  
@10:
	mov		DWORD PTR [last_fp], ebp
	add		esp, 4
	mov		DWORD PTR [last_sp], esp
	sub		esp, 4
	shr		ecx, 2
	add		ecx, 3
	push	ecx
	call	scavenge_and_allocate
	add		esp, 4
	mov		DWORD PTR [last_fp], 0
	mov		ecx, [esp+4]
	mov		edx, ecx			; length
	add		edx, (3*oopSize)	; header size
	add		edx, eax
	jmp		SHORT @11
endp

PUBLIC _allocateContext0
_allocateContext0 proc near
 	mov		eax, [eden_top]
	add		eax, 3 * oopSize
	cmp		eax, [eden_end]
	jg		SHORT @1
	mov		[eden_top], eax

@2:	mov		ebx, [context_klass]
	mov		DWORD PTR [eax-(3 * oopSize)], 80000003H + (1 * 4) ; obj->init_mark()
	mov		DWORD PTR [eax-(2 * oopSize)], ebx                 ; obj->set_klass(klass)
	mov		DWORD PTR [eax-(1 * oopSize)], 0                   ; obj->set_home(NULL)
	sub		eax, (3 * oopSize) - 1
	ret
  
@1: scavenge 3
	jmp		SHORT @2
endp

PUBLIC _allocateContext1
_allocateContext1 proc near
 	mov		eax, [eden_top]
	add		eax, 4 * oopSize
	cmp		eax, [eden_end]
	jg		SHORT @3
	mov		[eden_top], eax

@4:	mov		ebx, [context_klass]
	mov		ecx, [nil_obj]
	mov		DWORD PTR [eax-(4 * oopSize)], 80000003H + (2 * 4) ; obj->init_mark()
	mov		DWORD PTR [eax-(3 * oopSize)], ebx                 ; obj->set_klass(klass)
	mov		DWORD PTR [eax-(2 * oopSize)], 0                   ; obj->set_home(NULL)
	mov		DWORD PTR [eax-(1 * oopSize)], ecx                 ; obj->obj_at_put(0,nilObj)
	sub		eax, (4 * oopSize) - 1
	ret
  
@3: scavenge 4
	jmp		SHORT @4
endp

PUBLIC _allocateContext2
_allocateContext2 proc near
 	mov		eax, [eden_top]
	add		eax, 5 * oopSize
	cmp		eax, [eden_end]
	jg		SHORT @5
	mov		[eden_top], eax

@6:	mov		ebx, [context_klass]
	mov		ecx, [nil_obj]
	mov		DWORD PTR [eax-(5 * oopSize)], 80000003H + (3 * 4) ; obj->init_mark()
	mov		DWORD PTR [eax-(4 * oopSize)], ebx                 ; obj->set_klass(klass)
	mov		DWORD PTR [eax-(3 * oopSize)], 0                   ; obj->set_home(NULL)
	mov		DWORD PTR [eax-(2 * oopSize)], ecx                 ; obj->obj_at_put(0,nilObj)
	mov		DWORD PTR [eax-(1 * oopSize)], ecx                 ; obj->obj_at_put(0,nilObj)
	sub		eax, (5 * oopSize) - 1
	ret
  
@5: scavenge 5
	jmp		SHORT @6
endp

ENDS

END




