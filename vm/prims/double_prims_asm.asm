; Copyright 1996 LongView Technologies L.L.C. $Revision: 1.3 $
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

double_klass EQU _doubleKlassObj

eden_top      EQU _eden_top
eden_end      EQU _eden_end

last_fp       EQU _last_Delta_fp
last_sp       EQU _last_Delta_sp

scavenge_and_allocate EQU _scavenge_and_allocate

EXTRN	double_klass:  PTR DWORD
EXTRN   eden_top:      PTR DWORD
EXTRN   eden_end:      PTR DWORD

EXTRN   last_fp:      PTR DWORD
EXTRN   last_sp:      PTR DWORD


vmSymbols EQU _vm_symbols

VMSYMBOLS STRUC
	_smi_overflow			DD ?
        _division_by_zero		DD ?
	_receiver_has_wrong_type	DD ?
	_division_not_excact		DD ?
	_first_argument_has_wrong_type  DD ?
ENDS

extrn	vmSymbols: PTR VMSYMBOLS

return_error MACRO  error
	mov	eax, [vmSymbols.&error]
	add	eax,2
	ret 8
ENDM

EXTRN	scavenge_and_allocate: PROC

oopSize   EQU 4

doubleOperation MACRO op
  LOCAL a, c, b
	; Tag test for argument
	mov	ebx, [esp+04]
	mov	edx, [double_klass]
	test	bl, 01H
	jz	SHORT c

	; klass test for argument

	cmp	edx, [ebx+3]
	mov	eax, [eden_top]
	jne	SHORT c

	; allocate_double

	add	eax, 4 * oopSize
	cmp	eax, [eden_end]
	jg	SHORT a
	mov	[eden_top], eax

b:
	mov ecx, [esp+08]


	mov	DWORD PTR [eax-(4 * oopSize)], 0A0000003H ; obj->init_mark()
	fld  QWORD PTR [ecx+07]
	mov	DWORD PTR [eax-(3 * oopSize)], edx        ; obj->set_klass(klass)

	op   QWORD PTR [ebx+07]

	sub	eax, 15
 
	; eax result   doubleOop
	; ecx receiver doubleOop
	; ebx argument doubleOop

	fstp QWORD PTR [eax+07]
	ret 8

a:
	mov		DWORD PTR [last_fp], ebp
	add		esp, 4
	mov		DWORD PTR [last_sp], esp
	sub		esp, 4
	push	4
	call	scavenge_and_allocate
	add		esp, 4
	mov		DWORD PTR [last_fp], 0
	add		eax, 4 * oopSize
	mov		edx, [double_klass]
	mov		ebx, [esp+04]
	jmp		SHORT b

c:
	return_error	_first_argument_has_wrong_type
ENDM

PUBLIC _double_subtract@8
_double_subtract@8 proc near
	doubleOperation fsub
endp

PUBLIC _double_add@8
_double_add@8 proc near
	doubleOperation fadd
endp

PUBLIC _double_multiply@8
_double_multiply@8 proc near
	doubleOperation fmul
endp

PUBLIC _double_divide@8
_double_divide@8 proc near
	doubleOperation fdiv
endp

PUBLIC _double_from_smi@4
_double_from_smi@4 proc near
	; allocate_double
	mov		eax, [eden_top]
	add		eax, 4 * oopSize
	cmp		eax, [eden_end]
	jg		SHORT @1
	mov		[eden_top], eax

@2:
	mov		ecx, [esp+04]
	mov		edx, [double_klass]
	sar		ecx, 2
	mov		DWORD PTR [eax-(4 * oopSize)], 0A0000003H ; obj->init_mark()
	mov		[esp-04], ecx
	mov		DWORD PTR [eax-(3 * oopSize)], edx        ; obj->set_klass(klass)

	fild	DWORD PTR [esp-04]
	sub		eax, 15
 
	; eax result   doubleOop
	fstp	QWORD PTR [eax+07]
	ret		4

@1:
	mov		DWORD PTR [last_fp], ebp
	add		esp, 4
	mov		DWORD PTR [last_sp], esp
	sub		esp, 4
	push	4
	call	scavenge_and_allocate
	add		esp, 4
	mov		DWORD PTR [last_fp], 0
	add		eax, 4 * oopSize
	jmp		SHORT @2
endp

ENDS

END




