; Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.4 $
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


;-----------------------------------------------------------------------
; Stub routines called from a "Pascal" callBack chunk (see callBack.cpp)
; Incomming arguments:
;  ecx, ecx = index           (passed on to Delta)
;  edx, edx = number of bytes (to be deallocated before returning)

EXTRN		_handleCallBack: PROC
PUBLIC		_handlePascalCallBackStub

_handlePascalCallBackStub PROC
		; create link
		push	ebp   
		mov	ebp, esp

		; save registers for Pascal calling convention
		push	ebx
		push	edi
		push	esi

		; save number of bytes in parameter list
		push	edx

                ; compute parameter start
		mov	edx, esp
		add	edx, 24 ; (esi, edi, ebx, edx, fp, return address)

        	; eax = handleCallBack(index, &params)
		CALL	_handleCallBack C, ecx, edx

        	; restore number of bytes in parameter list
		pop	edx

		; restore registers for Pascal calling convention
		pop	esi
		pop	edi
		pop	ebx
	
		; destroy link
		mov	esp, ebp
		pop	ebp

        	; get return address
		pop	ecx

                ; deallocate the callers parameters
		add	esp, edx

		; jump to caller
		jmp	ecx
		ENDP


;------------------------------------------------------------------
; Stub routines called from a "C" callBack chunk (see callBack.cpp)
; Incomming arguments:
;  eax = index               (passed on to Delta)

PUBLIC		_handleCCallBackStub

_handleCCallBackStub PROC
		; create link
		push ebp   
		mov  ebp, esp

		; save registers for Pascal calling convention
		push	ebx
		push	edi
		push	esi

		; compute parameter start
		mov edx, esp
		add edx, 8 ; (fp, return address)

        	; eax = handleCallBack(index, &params)
		CALL _handleCallBack C, ecx, edx
	
		; restore registers for Pascal calling convention
		pop	esi
		pop	edi
		pop	ebx

		; destroy link
		mov  esp, ebp
		pop  ebp

                ; return (the caller handles the parameters)
        	ret
		ENDP

ENDS
END
