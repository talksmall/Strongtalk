; Copyright 1994, 1995, 1996 LongView Technologies L.L.C. $Revision: 1.2 $
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


argument EQU dword ptr [esp+4]
receiver EQU dword ptr [esp+8]

first_argument_has_wrong_type:
	return_error _first_argument_has_wrong_type

overflow:
	return_error _smi_overflow

division_by_zero:
	return_error _division_by_zero


PUBLIC  _smiOopPrimitives_multiply@8
_smiOopPrimitives_multiply@8 proc	near
 	mov	EDX, argument
 	mov	EAX, receiver
 	test	DL, 03H
	jne	SHORT first_argument_has_wrong_type
 	sar	EDX,2
 	imul	EDX
	jo	SHORT overflow
        ret	8
endp

PUBLIC _smiOopPrimitives_add@8
_smiOopPrimitives_add@8 proc near
	mov	EAX, receiver
	add	EAX, argument
	jo	SHORT @98
	test	AL, 03H
	jne	SHORT first_argument_has_wrong_type
        ret	8
@98:	mov	EAX, argument
	test	EAX, 03H
	jne	SHORT first_argument_has_wrong_type
	jmp	SHORT overflow
endp

PUBLIC _smiOopPrimitives_subtract@8
_smiOopPrimitives_subtract@8 proc	near
	mov	EAX, receiver
	sub	EAX, argument
	jo	SHORT @99
	test	AL, 03H
	jne	SHORT first_argument_has_wrong_type
        ret	8
@99:	mov	EAX, argument
	test	AL, 03H
	jne	SHORT first_argument_has_wrong_type
	jmp	SHORT overflow
endp


PUBLIC _smiOopPrimitives_mod@8

; Intel definition of mod delivers:
;   0 <= |x%y| < |y|
;
; Standard definition requires:
;   y>0: 
;     0 <= x mod y < y
;   y<0:
;     y <  x mod y <= 0
;
; Conversion:
;
;   sgn(y)=sgn(x%y):
;     x mod y = x%y
;
;   sgn(y)#sgn(x%y):
;     x mod y = x%y + y
;

_smiOopPrimitives_mod@8 proc near
 	mov	EAX, receiver	    ; x
 	mov	ECX, argument	    ; y
 	test	ECX, ECX	    ; if (y == 0)
	je	division_by_zero    ;   goto division_by_zero
 	test	CL, 03H		    ; if (!y->is_smi())
				    ;   goto first_argument_has_wrong_type
	jne	first_argument_has_wrong_type
 	sar	ECX,2		    ;  y = smiOop(y)->value()
 	sar	EAX,2		    ;  x = smiOop(x)->value()
 	cdq	
 	idiv	ECX		    ; eax = x/y, edx = x%y
	jo	overflow
	mov	EAX, EDX
	test	EAX, EAX
	je	@1
	xor	EDX, ECX 	    ; if (sgn(y) != sgn(x%y))
	js	SHORT @2	    ;   goto @4
@1: 	sal	EAX, 2		    ; assert(sgn(y) == sgn(x%y))
 	ret	8		    ; return as_smiOop(x%y)

@2:	add	EAX, ECX	    ; assert(sgn(y) != sgn(x%y))
 	sal	EAX, 2
 	ret	8		    ; return as_smiOop(x%y+y)
endp

first_argument_has_wrong_type_1:
	return_error _first_argument_has_wrong_type

overflow_1:
	return_error _smi_overflow

division_by_zero_1:
	return_error _division_by_zero

PUBLIC _smiOopPrimitives_div@8

; Intel definition of div delivers:
;   x = (x/y)*y + (x%y)
;
; Standard definition requires:
;   x = (x div y)*y + (x mod y)
;
; Conversion:
;
;   sgn(y)=sgn(x%y):
;     x div y = x/y
;
;   sgn(y)#sgn(x%y):
;     x div y = x/y-1
;

_smiOopPrimitives_div@8 proc	near
 	mov	ECX, argument	    ; y
 	mov	EAX, receiver	    ; x
 	test	ECX, ECX	    ; if (y == 0)
				    ;   goto division_by_zero
	je	SHORT division_by_zero_1
 	test	CL, 03H		    ; if (!y->is_smi())
				    ;   goto first_argument_has_wrong_type
	jne	SHORT first_argument_has_wrong_type_1
 	sar	ECX,2		    ;  y = smiOop(y)->value()
 	sar	EAX,2		    ;  x = smiOop(x)->value()
 	cdq	
 	idiv	ECX		    ; eax = x/y, edx = x%y
	jo	SHORT overflow_1
	test	EDX, EDX
	je	@3
	xor	ECX, EDX	    ; if (sgn(y) != sgn(x%y))
	js	SHORT @4	    ;   goto @4
@3: 	sal	EAX, 2		    ; assert(sgn(y) == sgn(x%y))
 	ret	8		    ; return as_smiOop(x/y)

@4:	dec	EAX		    ; assert(sgn(y) != sgn(x%y))
 	sal	EAX, 2
 	ret	8		    ; return as_smiOop(x/y-1)
endp


first_argument_has_wrong_type_2:
	return_error _first_argument_has_wrong_type

overflow_2:
	return_error _smi_overflow

division_by_zero_2:
	return_error _division_by_zero



PUBLIC _smiOopPrimitives_remainder@8
_smiOopPrimitives_remainder@8 proc	near
 	mov	ECX, argument
 	mov	EAX, receiver
 	test	ECX, ECX
	je	SHORT division_by_zero_2
 	test	CL, 03H
	jne	SHORT first_argument_has_wrong_type_2
 	sar	ECX, 2
 	sar	EAX, 2
 	cdq	
 	idiv	ECX
	jo	SHORT overflow_2
	mov	EAX, EDX
 	sal	EAX, 2
	ret	8
endp

PUBLIC _smiOopPrimitives_quo@8
_smiOopPrimitives_quo@8 proc	near
 	mov	ECX, argument
 	mov	EAX, receiver
 	test	ECX, ECX
	je	SHORT division_by_zero_2
 	test	CL, 03H
	jne	SHORT first_argument_has_wrong_type_2
 	sar	ECX, 2
 	sar	EAX, 2
 	cdq	
 	idiv	ECX
	jo	SHORT overflow_2
 	sal	EAX, 2
	ret	8
endp

ENDS

END




