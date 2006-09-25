; Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.103 $
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
_begin_of_interpreter_code:
ENDS

;-----------------------------------------------------------------------------------------
; Conventions
;
; Registers:
;
; eax   contains (logical) top of stack (tos)
; ebx	contains 000000xxh, used for zero extension of bytes
; ecx	general purpose
; edx	general purpose
;
; edi	general purpose (rarely used)
; esi	source index, points to the next hcode to be executed
; ebp	base pointer
; esp	stack pointer, pointing to (logical) tos-4 (in bytes)
;
;
; Naming:
;
; - labels and macros start with an underbar (_)
; - case labels end with an underbar (_)
;
; IMPORTANT: Within the interpreter, eax and ebx are used as top of stack and 000000xxh
; register and therefore must be set up accordingly whenever the interpreter is entered
; from the "ouside" (e.g., from C).


;-----------------------------------------------------------------------------------------
; Flags
;
; to enable, uncomment the particular line

	optimized	= 1	; enables optimized code generation, disables all other flags

IFNDEF	optimized
	tracing		= 1	; enables trace code generation (debugging)
				; (otherwise +TraceBytecodes doesn't work)
	trace_sends	= 1	; enables tracing of sends
	assertions	= 1	; enables code generation for general assertions (debugging)
	stack_checks	= 1	; enables stack check code generation (debugging)
ENDIF

;-----------------------------------------------------------------------------------------
; Store the status of the interpreter code by storing a byte for each define

DATASEG

IFDEF optimized
status_string    DB 1
ELSE
status_string    DB 0
ENDIF
IFDEF tracing
    DB 1
ELSE
    DB 0
ENDIF
IFDEF trace_sends
    DB 1
ELSE
    DB 0
ENDIF
IFDEF assertions
    DB 1
ELSE
    DB 0
ENDIF
IFDEF stack_checks
    DB 1
ELSE
    DB 0
ENDIF
IFDEF timer
    DB 1
ELSE
    DB 0
ENDIF
ENDS

CODESEG
PUBLIC		_InterpreterCodeStatus
_InterpreterCodeStatus PROC
    mov	eax, OFFSET status_string
    ret
ENDP
ENDS

;-----------------------------------------------------------------------------------------
; Error Messages, Output

EXTRN		_lprintf: PROC
EXTRN		_breakpoint: PROC

_print		MACRO string, arg1, arg2
LOCAL		stringa				; string address
ENDS						; close code segment
DATASEG						; add string definition to data segment
stringa		DB string, 10, 0		; terminate with LF, 0
ENDS						; close data segment
CODESEG						; re-open code segment
		pushad				; save state
		CALL _lprintf C, OFFSET stringa, arg1, arg2
		popad				; restore state
		ENDM

_break		MACRO
ERRIFDEF	optimized 'performance bug: _break called in optimized code'
		pushad
		CALL _breakpoint C
		popad
		ENDM


;-----------------------------------------------------------------------------------------
; Stack checker
;
; The stack checker pushes a magic number on the stack and
; checks for it when it is popped. The stack checker is helpfull
; to detect inconsistent numbers of pushes and pops within a
; structured construct (e.g., such as loops). Check code is only
; generated if stack_checks are enabled.

magic		= 0FCFCFCFCh			; must be a smi


_sc_push	MACRO
IFDEF		stack_checks
		push eax
		mov eax, magic
ENDIF
		ENDM


_sc_pop		MACRO
LOCAL		_L
IFDEF		stack_checks
		;_print "pop : esp = 0x%x", esp, 0
		;_print "      tos = 0x%x", eax, 0
		cmp eax, magic
		je _L
		jne _stack_missaligned
_L:
		pop eax
ENDIF
		ENDM


;-----------------------------------------------------------------------------------------
; Byte Code

EXTRN		_NumberOfBytecodesExecuted: DWORD
EXTRN		_TraceBytecodes: DWORD

_advance_aligned MACRO offs			; advance esi by offs and align it to dword
		add esi, offs+3
		and esi, NOT 3
		ENDM

_check_ebx	MACRO
IFDEF		assertions
		test ebx, 0FFFFFF00h
		jnz _ebx_wrong
ENDIF
		ENDM

_check_obj	MACRO reg
IFDEF		assertions
		test reg, 2
		jnz _obj_wrong
ENDIF
		ENDM

_load_ebx	MACRO
		_check_ebx
		mov bl, [esi]
		ENDM

_next_ebx	MACRO
		_check_ebx
		mov bl, [esi+1]
		inc esi
		ENDM

IFDEF		tracing
DATASEG
format		DB '%9d 0x%x: %02x', 10, 0
ENDS
ENDIF

_jump_ebx	MACRO
LOCAL		_no_trace
IFDEF		assertions
		cmp DWORD PTR [_last_Delta_fp], 0
		jne _last_Delta_fp_wrong
ENDIF
IFDEF		tracing
		inc [_NumberOfBytecodesExecuted]
		mov ecx, [_TraceBytecodes]
		test ecx, ecx
		jz SHORT _no_trace
		push eax			; save tos
		push ebx			; save hcode
		push esi			; save source index
		CALL _lprintf C, OFFSET format, [_NumberOfBytecodesExecuted], esi, ebx
		pop esi				; restore source index
		pop ebx				; restore hcode
		pop eax				; restore tos
_no_trace:
ENDIF
		_check_obj eax
		jmp [_dispatch_table + ebx*4]
		ENDM


;-----------------------------------------------------------------------------------------
; VM Objects

Mem_Tag		= 1
Mark_Tag	= 2
Tag_Size	= 2
Int_One		= 4

EXTRN		_nilObj
EXTRN		_trueObj
EXTRN		_falseObj
EXTRN		_smiKlassObj
EXTRN		_contextKlassObj

_nil		MACRO reg
		mov reg, [_nilObj]
		ENDM

_true		MACRO reg
		mov reg, [_trueObj]
		ENDM

_false		MACRO reg
		mov reg, [_falseObj]
		ENDM

_smiKlass	MACRO reg
		mov reg, [_smiKlassObj]
		ENDM

_contextKlass	MACRO reg
		mov reg, [_contextKlassObj]
		ENDM


Header		STRUC
		_mark		DB 4-Mem_Tag DUP (?)	; accounts for Mem_Tag
		_klass		DD ?			; klassOop
		ENDS

AbstractMethod	STRUC
Header		STRUC {}
		_source 	DD ?			; byteArrayOop
		_selector	DD ?			; symbolOop
		ENDS

Method		STRUC
AbstractMethod	STRUC {}
		_counter	DD ?			; smiOop
		_size_of_hcodes	DD ?			; smiOop
		_hcodes		DB ?			; start of hcodes
		ENDS

Proxy		STRUC
Header		STRUC {}
		_word32		DD ?			; a 32 bit word
		ENDS


;-----------------------------------------------------------------------------------------
; Stack frame
;
; An interpreter stack frame always provides space for at least one temporary (temp0).
; This speeds up method activation for methods with less than 2 temporaries. More
; temporaries are allocated via special bytecodes. Furthermore: Since there is always
; (at least) one temporary, which in case of context extensions holds the pointer to
; the heap-allocated context, non-local returns become simpler, because they can assume
; at least temp0 to be there (if there is a chance of having no temporaries and a self
; send without args, the temp0 location would contain the return address of the callee
; (a non-oop), if there were no temp0 always).

FOffs		= -12
Frame		STRUC
		_temp0		DD ?			; first temporary or context ptr
		_esi		DD ?			; saved esi
		_recv		DD ?			; receiver
		_bp		DD ?			; previous bp
		_ret		DD ?			; return address
		_params		DD ?			; last argument
		ENDS


_self		MACRO reg
		mov reg, [(ebp+FOffs)._recv]
		ENDM

_save_esi	MACRO
		mov [(ebp+FOffs)._esi], esi
		ENDM

_restore_esi	MACRO
		mov esi, [(ebp+FOffs)._esi]
		ENDM

_restore_ebx	MACRO
		xor ebx, ebx
		ENDM

_get_context	MACRO reg
		mov reg, [(ebp+FOffs)._temp0]
IFDEF		assertions
		; (*) check is_context
ENDIF
		ENDM


;-----------------------------------------------------------------------------------------
; Stores with store checks

EXTRN		_byte_map_base: DWORD
card_k		= 9				; card_size = 2^card_k

_store_check	MACRO obj, base
		mov base, [_byte_map_base]	; get base address
		shr obj, card_k			; divide by 2^card_k
		mov BYTE PTR [base + obj], 0
		ENDM

_store_in_at	MACRO obj, offs, val		; obj[offs] := val
		mov [obj(offs)], val		; assignment
		_store_check obj, val
		ENDM

; Optimization ideas:
; - keep _byte_map_base always in register (which one? currently none is available)
; - modify code if _byte_map_base changes
; - tradeoff between marking everything and more store check code
;   (e.g. no marking necessary if an integer is assigned)


;-----------------------------------------------------------------------------------------
; Primitive calls

_set_last_Delta_frame MACRO
		mov [_last_Delta_fp], ebp	; enter C -> store last Delta frame
		mov [_last_Delta_sp], esp
		ENDM


_set_last_Delta_frame_after_call MACRO
		add esp, 4
		_set_last_Delta_frame
		sub esp, 4
		ENDM


_reset_last_Delta_frame MACRO
		mov DWORD PTR [_last_Delta_fp], 0	; leaving C -> clear _last_Delta_fp
		ENDM


_call_C		MACRO dest
		_set_last_Delta_frame
		_save_esi
		call dest
		_ic_info _C_nlr_testpoint
		_restore_esi
		_restore_ebx			; ebx := 0
		_reset_last_Delta_frame
		ENDM


; _predict_prim & _predict_prim_ifFail are two bytecodes that are
; used during lookup, during execution they can be simply ignored.

_predict_prim	MACRO
		_advance_aligned 5
		_load_ebx
		_jump_ebx
		ENDM


_predict_prim_ifFail MACRO
		_advance_aligned 9
		_load_ebx
		_jump_ebx
		ENDM


;-----------------------------------------------------------------------------------------
; Calling conventions for sends
;
; For general sends the receiver and the arguments are pushed on the stack in the order
; in which they appear in the source code (i.e., receiver first, first argument, etc.).
; For self and super sends, the receiver is *not* pushed but taken directly from the
; callers stack frame, i.e. in these cases only the arguments are on the stack.
;
; The callee is responsible for removing the arguments from the stack, i.e., its return
; instructions have to know how many arguments there are.
;
; _load_recv is loading the receiver into eax and makes sure that the receiver
; (except for self and super sends) as well as the arguments are on the stack.

_load_recv	MACRO nofArgs
		push eax			; push last argument or receiver
IF		nofArgs EQ -999			; if n = -999 then
		_self eax			;    self send: load recv
ELSE						; else
    IF		nofArgs EQ 999			;    if n = 999 then
		mov bl, [esi + 1]		;       general case: b arguments
		mov eax, [esp + 4*ebx]		;       and load recv
    ELSE					;    else
	IF	nofArgs GT 0			;	if n > 0 then
		mov eax, [esp + 4*(nofArgs)]	;	   load recv
	ENDIF					;       else recv already in eax
    ENDIF					;    end
ENDIF						; end
		ENDM


;-----------------------------------------------------------------------------------------
; Normal sends, primitive methods
;
; HCode structure:
;
; 	[send		]	(1 byte)
; 	[method		]	(1 dword)
;	[icache (class)	]	(1 dword)
;	...	  	  <---	esi

EXTRN		_inline_cache_miss: PROC

_normal_send_prim MACRO noResult
LOCAL		_is_smi, _compare_class, _failed, _cache_miss
LOCAL		method, class

method		EQU esi-8
class		EQU esi-4

		;_print "normal send, primitive", 0, 0
		push eax			; save receiver
		_advance_aligned 9
		mov ecx, [class]		; get inline cache
		_smiKlass edx			; preload smi class
		test al, Mem_Tag		; check if smi
		jz SHORT _compare_class
		mov edx, [eax._klass]		; get recv class

_compare_class: ; eax: receiver
		; ebx: byte code
		; ecx: cached class
		; edx: receiver class
		; esi: next instruction
		cmp edx, ecx			; compare with inline cache
		jne _cache_miss

		;_print 'primitive called via normal_send_prim', 0, 0
		mov ecx, [method]		; get methodOop
		lea edx, [ecx._hcodes(4)]	; get primitive address
		mov edx, [edx]
		_call_C edx			; eax := primitive call
		test eax, Mark_Tag
		jnz _failed
		_load_ebx
IF		noResult NE 0			; if the result is not used
		pop eax				; discard it
ENDIF
		_jump_ebx

_cache_miss:	; eax: receiver
		; ebx: byte code
		; ecx: cached class
		; edx: receiver class
		; esi: next instruction
		_call_C _inline_cache_miss
		_load_ebx
		pop eax				; get receiver or last argument
		_jump_ebx

_failed:	_print 'predicted primitive failed - not yet implemented', 0, 0
		hlt

		ENDM


;-----------------------------------------------------------------------------------------
; Normal sends, access methods
;
; HCode structure:
;
; 	[send		]	(1 byte)
; 	[method		]	(1 dword)
;	[icache (class)	]	(1 dword)
;	...	  	  <---	esi
;
; Access methods cannot exist for the smi class which simplifies
; the inline cache test: if the receiver is a smi it is always
; a cache miss because the cache never caches the smi class.
;
; Note: Currently _load_recv is used to get the receiver. This
; is suboptimal since the receiver is also pushed on the stack
; and has to be popped again at the end (except for self sends).
; Should change this at some point (optimization).

_normal_send_access MACRO nofArgs
LOCAL		_cache_miss
LOCAL		method, class

method		EQU esi-8
class		EQU esi-4

		;_print "normal send, access, nofArgs = %d", nofArgs, 0
		_load_recv nofArgs
		_advance_aligned 9
		test al, Mem_Tag		; check if smi
		jz SHORT _cache_miss		; if smi then it's a cache miss
		mov ecx, [class]		; get inline cache
		mov edx, [eax._klass]		; get recv class

		; eax: receiver
		; ecx: cached class
		; edx: receiver class
		cmp edx, ecx			; compare with inline cache
		jne SHORT _cache_miss

		mov ecx, [method]		; get method
		lea edx, [ecx._hcodes(4)]	; start address of hcode + 4
		and edx, NOT 3			; align
		mov edx, [edx]			; get instVar offset
		mov eax, [eax + edx - Mem_Tag]	; load instVar at offset

IF		nofArgs GE 0			; if n >= 0 then
		pop ecx				;    receiver still on stack: remove it
ENDIF						; end

		_load_ebx
		_jump_ebx

_cache_miss:    _call_C _inline_cache_miss
		_load_ebx
		pop eax
		_jump_ebx
		ENDM


;-----------------------------------------------------------------------------------------
; Returns
;
; _return_tos pops the arguments and returns from a method or block. nofArgs stands
; for the number of arguments. If nofArgs = 999, the number of arguments is determined
; by the next byte in the byte code.

_return_tos	MACRO nofArgs
		mov esp, ebp
		pop ebp
IF		nofArgs EQ 999			; if nofArgs is the next byte then
		mov bl, [esi+1]			;    get nofArgs
		pop ecx				;    get return address
		lea esp, [esp + 4*ebx]		;    adjust esp
		jmp ecx				;    return
ELSE						; else nofArgs is constant
	IF	nofArgs EQ 0			;    make sure that only
		ret				;    ret is used if
	ELSE					;    nofArgs = 0
		ret 4*nofArgs
	ENDIF
ENDIF
		ENDM


;-----------------------------------------------------------------------------------------
; Zap context
;
; The context is zapped by storing 0 into the home field of the context.
; Only method contexts must be zapped.

_zap		MACRO
		push eax			; make sure temp0 (context) is in memory
		_get_context ecx
		pop eax
		mov [ecx._home], 0
		ENDM


;-----------------------------------------------------------------------------------------
; Blocks

Closure		STRUC
Header		STRUC {}
		_block		DD ?		; methodOop
		_context	DD ?		; context, receiver or single parameter
		ENDS

Context		STRUC
Header		STRUC {}
		_home		DD ?		; enclosing context/stack/method
		_vars		DD ?		; context variables
		ENDS

; _primitiveValue0..9 are the primitives called in block value messages.
; i is the number of arguments for the block.

invocation_counter_inc = 10000h

EXTRN		_block_entry_point

CODESEG
IRPC		i, 0123456789
PUBLIC		_primitiveValue&i
_primitiveValue&i:
		mov eax, [esp + 4*(i+1)]	; load recv (= block)
		jmp [_block_entry_point]
ENDM
ENDS


;-----------------------------------------------------------------------------------------
; Inline caches
;
; _ic_info puts inline cache information after a call
; as expected for compiled code. NLRs in interpreted
; and compiled code are treated the same way.
;
; Note: _ic_info must come *immediately* after a call!

ic_info_flags_size = 8					; the same as MacroAssembler::ic_info_flags_size

_ic_info	MACRO NLR_testpoint
LOCAL		nlr_offset, _ret_addr
nlr_offset	= NLR_testpoint - _ret_addr
_ret_addr:	DB 0A9h					; dummy instruction: test eax, imm32
		DD (nlr_offset SHL ic_info_flags_size)	; NLR offset & flags
		ENDM


;-----------------------------------------------------------------------------------------
; Copy parameters into context

_copy_params MACRO n, recv
LOCAL	i
	push eax				; make sure temp0 is in memory
	_get_context ecx
	mov eax, ecx
	_store_check eax, edx
IF	recv NE 0
	_self edx
	mov [ecx._vars], edx			; store recv
ENDIF
i	= 0
REPT	n
	mov bl, [esi+1+i]			; get i.th parameter index
	mov edx, [(ebp+FOffs)._params(4*ebx)]	; get parameter
IF	recv EQ 0
	mov [ecx._vars(4*i)], edx		; store in i.th context variable
ELSE
	mov [ecx._vars(4*(i+1))], edx		; store in (i+1).th context variable
ENDIF
i	= i+1
ENDM
	mov bl, [esi+1+n]			; get next byte code
	add esi, 1+n
	pop eax
	_jump_ebx
	ENDM


_copy_paramsb MACRO recv
LOCAL	_loop
	add esi, 2				; esi points to first parameter index
	push eax				; save tos (make sure temp0 is in memory)
	_get_context ecx
	mov eax, ecx
	_store_check eax, edx
IF	recv NE 0
	_self edx
	mov [ecx._vars], edx			; store recv
ENDIF
	mov al, [esi-1]				; get b (nof params)
_loop:
	mov bl, [esi]				; get parameter index
	mov edx, [(ebp+FOffs)._params(4*ebx)]	; get parameter
IF	recv EQ 0
	mov [ecx._vars], edx			; store in context variable
ELSE
	mov [ecx._vars(4)], edx			; store in context variable
ENDIF
	add ecx, 4
	inc esi
	dec al
	jnz _loop				; ebx = 0
	pop eax					; restore tos
	_load_ebx
	_jump_ebx
	ENDM


;-----------------------------------------------------------------------------------------
; Context variables
;
; Note that eax must be pushed first since it may contain
; the context (tos is always in eax).

_push_tempb_context MACRO c
	push eax
	_get_context ecx
	mov bl, [esi+1]					; get b
REPT	c						; get context -c
	mov ecx, [ecx._home]
ENDM
	mov eax, [ecx._vars(4*ebx)]
	mov bl, [esi+2]					; get next byte code
	add esi, 2
	_jump_ebx
	ENDM


_push_temp_contextb MACRO t
LOCAL	_loop
	push eax
	_get_context ecx
	mov dl, [esi+1]					; get b
_loop:	mov ecx, [ecx._home]
	dec dl
	jnz _loop
	mov eax, [ecx._vars(4*t)]			; ebx = 0
	mov bl, [esi+2]					; get next byte code
	add esi, 2
	_jump_ebx
	ENDM


_push_tempb_contextb MACRO
LOCAL	_loop
	push eax
	_get_context ecx
	mov bl, [esi+1]					; get b1
	mov dl, [esi+2]					; get b2
_loop:	mov ecx, [ecx._home]
	dec dl
	jnz _loop
	mov eax, [ecx._vars(4*ebx)]
	mov bl, [esi+3]					; get next byte code
	add esi, 3
	_jump_ebx
	ENDM


_store_pop_temp_context MACRO c, t
	_get_context ecx
REPT	c						; get context -c
	mov ecx, [ecx._home]
ENDM
	_store_in_at ecx, _vars(4*t), eax
	pop eax
	_next_ebx
	_jump_ebx
	ENDM


_store_pop_tempb_context MACRO c
	_get_context ecx
	mov bl, [esi+1]					; get b
REPT	c						; get context -c
	mov ecx, [ecx._home]
ENDM
	_store_in_at ecx, _vars(4*ebx), eax
	pop eax
	mov bl, [esi+2]					; get next byte code
	add esi, 2
	_jump_ebx
	ENDM


_store_pop_temp_contextb MACRO t
LOCAL	_loop
	_get_context ecx
	mov dl, [esi+1]					; get b
_loop:	mov ecx, [ecx._home]
	dec dl
	jnz _loop
	_store_in_at ecx, _vars(4*t), eax
	pop eax
	mov bl, [esi+2]					; get next byte code
	add esi, 2
	_jump_ebx
	ENDM


_store_pop_tempb_contextb MACRO
LOCAL	_loop
	_get_context ecx
	mov bl, [esi+1]					; get b1
	mov dl, [esi+2]					; get b2
_loop:	mov ecx, [ecx._home]
	dec dl
	jnz _loop
	_store_in_at ecx, _vars(4*ebx), eax
	pop eax
	mov bl, [esi+3]					; get next byte code
	add esi, 3
	_jump_ebx
	ENDM


;-----------------------------------------------------------------------------------------
; Control structures and jumps
;
; Jump offsets are unsigned bytes/words. For forward jumps, the jump destination
; is the address of the next instruction + offset, for backward jumps (whileTrue
; and whileFalse) the jump destination is the current instruction's address - offset.


EXTRN		_loop_counter_overflow: PROC	; this routine is called whenever the loop counter overflows

DATASEG
PUBLIC		_interpreter_loop_counter	; this variable is incremented in each loop iteration
PUBLIC		_interpreter_loop_counter_limit

ALIGN 4
_interpreter_loop_counter	DD 0
_interpreter_loop_counter_limit	DD 0
ENDS


_load_cond	MACRO reg, cond
IF		cond NE 0
		_true reg
ELSE
		_false reg
ENDIF
		ENDM


_load_not_cond	MACRO reg, cond
IF		cond EQ 0
		_true reg
ELSE
		_false reg
ENDIF
		ENDM


_if_b		MACRO cond
LOCAL		_else
		_load_cond ecx, cond
		cmp eax, ecx			; if tos # cond
		jne SHORT _else			; then jump to else part
		add esi, 3			; skip info & offset byte
		_load_ebx
		pop eax				; discard condition
		_jump_ebx

_else:		mov bl, [esi + 2]		; load jump offset
		_load_not_cond ecx, cond
		cmp eax, ecx			; if tos # ~cond
		jne _non_booleans		; then non-boolean arguments
		lea esi, [esi + ebx + 3]	; jump relative to next instr (must happen after the check for non-booleans)
		_load_ebx
		pop eax				; discard condition
		_jump_ebx
		ENDM


_if_w		MACRO cond
LOCAL		_else
		_load_cond ecx, cond
		_advance_aligned 6
		cmp eax, ecx			; if tos # cond
		jne SHORT _else			; then jump to else part
		_load_ebx
		pop eax				; else discard condition
		_jump_ebx

_else:		_load_not_cond ecx, cond
		cmp eax, ecx			; if tos # ~cond
		jne _non_booleans		; then non-boolean arguments
		mov ecx, [esi-4]		; load jump offset
		add esi, ecx			; jump relative to next instr (must happen after the check for non-booleans)
		_load_ebx
		pop eax				; else discard condition
		_jump_ebx
		ENDM


_cond_b		MACRO cond
LOCAL		_else
		_load_cond ecx, cond
		cmp eax, ecx			; if tos # cond
		jne SHORT _else			; then jump to else part
		add esi, 2			; skip info & offset byte
		_load_ebx
		pop eax				; discard condition
		_jump_ebx

_else:		mov bl, [esi + 1]		; load jump offset
		_load_not_cond ecx, cond
		cmp eax, ecx			; if tos # ~cond
		jne _non_booleans		; then non-boolean arguments
		lea esi, [esi + ebx + 2]	; jump relative to next instr (must happen after the check for non-booleans)
		_load_ebx
		_jump_ebx
		ENDM


_cond_w		MACRO cond
LOCAL		_else
		_load_cond ecx, cond
		_advance_aligned 5
		cmp eax, ecx			; if tos # cond
		jne SHORT _else			; then jump to else part
		_load_ebx
		pop eax				; else discard condition
		_jump_ebx

_else:		_load_not_cond ecx, cond
		cmp eax, ecx			; if tos # ~cond
		jne _non_booleans		; then non-boolean arguments
		mov ecx, [esi-4]		; load jump offset
		add esi, ecx			; jump relative to next instr (must happen after the check for non-booleans)
		_load_ebx
		_jump_ebx
		ENDM


_while_b	MACRO cond
LOCAL		_exit, _overflow, _call_overflow
		_load_cond ecx, cond
		mov bl, [esi + 1]		; get jump offset
		cmp eax, ecx
		jne SHORT _exit
		sub esi, ebx
		mov edx, [_interpreter_loop_counter]
		_load_ebx
		pop eax				; discard loop condition
		inc edx
		mov [_interpreter_loop_counter], edx
		cmp edx, [_interpreter_loop_counter_limit]
		jg _overflow
		_jump_ebx

_exit:		_load_not_cond ecx, cond
		cmp eax, ecx
		jne _non_booleans
		add esi, 2			; advance to next instruction (must happen after the check for non-booleans)
		_load_ebx
		pop eax				; discard loop condition
		_sc_pop
		_jump_ebx

_overflow:	_call_C _call_overflow
		_load_ebx
		_jump_ebx

_call_overflow: push eax			; save eax
		call _loop_counter_overflow
		pop eax				; restore eax
		ret
		ENDM


_while_w	MACRO cond
LOCAL		_exit, _overflow, _call_overflow
		_load_cond ecx, cond
		lea edx, [esi + 5 + 3]		; unaligned address of next instruction
		and edx, NOT 3			; aligned address of next instruction
		cmp eax, ecx
		jne SHORT _exit
		sub esi, [edx-4]
		mov edx, [_interpreter_loop_counter]
		_load_ebx
		pop eax				; discard loop condition
		inc edx
		mov [_interpreter_loop_counter], edx
		cmp edx, [_interpreter_loop_counter_limit]
		jg _overflow
		_jump_ebx

_exit:		_load_not_cond ecx, cond
		cmp eax, ecx
		jne _non_booleans
		mov esi, edx			; advance to next instruction (must happen after the check for non-booleans)
		_load_ebx
		pop eax				; discard loop condition
		_sc_pop
		_jump_ebx

_overflow:	_call_C _call_overflow
		_load_ebx
		_jump_ebx

_call_overflow: push eax			; save eax
		call _loop_counter_overflow
		pop eax				; restore eax
		ret
		ENDM


_jump_else_b	MACRO
		mov bl, [esi+1]			; get jump offset
		lea esi, [esi + ebx + 2]	; jump destination
		_load_ebx
		_jump_ebx
		ENDM


_jump_else_w	MACRO
		_advance_aligned 5
		add esi, [esi-4]		; jump destination
		_load_ebx
		_jump_ebx
		ENDM


_jump_loop_b	MACRO
		_sc_push
		mov bl, [esi+2]			; get jump offset
		lea esi, [esi + ebx + 3]	; jump destination
		_load_ebx
		_jump_ebx
		ENDM

_jump_loop_w	MACRO
		_sc_push
		_advance_aligned 9
		add esi, [esi-4]		; jump destination
		_load_ebx
		_jump_ebx
		ENDM


;-----------------------------------------------------------------------------------------
; Special hcodes

_compare MACRO cond				; cond = e : ==
LOCAL	_return_true				; cond = ne: ~~
	_next_ebx
	pop edx					; get receiver
	cmp eax, edx				; compare with argument
	j&cond SHORT _return_true
	_false eax
	_jump_ebx
_return_true:
	_true eax
	_jump_ebx
	ENDM


;-----------------------------------------------------------------------------------------
; Byte codes and dispatch table

BCodes	ENUM {
					store_pop_temp_context0_  =  44h,
					store_pop_temp_context1_  =  54h,
	push_temp_contextb_	=  60h,	store_pop_temp_contextb_  =  64h,

	push_tempb_context0_	=  43h,	store_pop_tempb_context0_ =  47h,
	push_tempb_context1_	=  53h,	store_pop_tempb_context1_ =  57h,
	push_tempb_contextb_	=  63h,	store_pop_tempb_contextb_ =  67h,

	copy_params_		=  68h,	copy_recv_params_	  =  6Ch,
	copy_paramsb_		=  6Bh,	copy_recv_paramsb_	  =  6Fh,

	ifTrue_b_		=  70h,	ifTrue_w_		  =  78h,
	ifFalse_b_		=  71h, ifFalse_w_		  =  79h,
	and_b_			=  72h, and_w_			  =  7Ah,
	or_b_			=  73h, or_w_			  =  7Bh,
	whileTrue_b_		=  74h, whileTrue_w_		  =  7Ch,
	whileFalse_b_		=  75h, whileFalse_w_		  =  7Dh,
	jump_else_b_		=  76h, jump_else_w_		  =  7Eh,
	jump_loop_b_		=  77h, jump_loop_w_		  =  7Fh,

	self_send_access_	= 0B8h,	normal_send_prim_,
	normal_send_access_     = 0C8h,	normal_send_pop_prim_,
	
	return_tos_		=  8Ch,	return_tos_b_		  =  8Fh,
	return_recv_		=  9Ch,	return_recv_b_		  =  9Fh,
	zap_return_tos_b_	= 0ACh,	zap_return_recv_b_,

	predict_prim_		= 0B1h,
	predict_prim_ifFail_	= 0B3h,

	predict_prim_lookup_		= 0C1h,
	predict_prim_ifFail_lookup_	= 0C3h,

	eql_eql_		= 0EEh, neq_neq_,
}


EXTRN	_dispatch_table: PTR DWORD

case	MACRO index
PUBLIC	_interpreter_case_&index
	ALIGN 4
_interpreter_case_&index:		; define the public dispatch entry
	ENDM


;-----------------------------------------------------------------------------------------
; HCode interpretation

CODESEG

IRPC	i, 012
case	%push_temp_contextb_+i
	_push_temp_contextb i
ENDM

IRPC	i, 012
case	%store_pop_temp_context0_+i
	_store_pop_temp_context 0, i
ENDM

IRPC	i, 012
case	%store_pop_temp_context1_+i
	_store_pop_temp_context 1, i
ENDM

IRPC	i, 012
case	%store_pop_temp_contextb_+i
	_store_pop_temp_contextb i
ENDM

case	%push_tempb_context0_
	_push_tempb_context 0

case	%push_tempb_context1_
	_push_tempb_context 1

case	%store_pop_tempb_context0_
	_store_pop_tempb_context 0

case	%store_pop_tempb_context1_
	_store_pop_tempb_context 1

case	%push_tempb_contextb_
	_push_tempb_contextb

case	%store_pop_tempb_contextb_
	_store_pop_tempb_contextb

IRPC	n, 12
case	%copy_params_+n
	_copy_params n, 0
ENDM

IRPC	n, 012
case	%copy_recv_params_+n
	_copy_params n, 1
ENDM

case	%copy_paramsb_
	_copy_paramsb 0

case	%copy_recv_paramsb_
	_copy_paramsb 1

case	%ifTrue_b_
	_if_b 1

case	%ifFalse_b_
	_if_b 0

case	%and_b_
	_cond_b 1

case	%or_b_
	_cond_b 0

case	%whileTrue_b_
	_while_b 1

case	%whileFalse_b_
	_while_b 0

case	%jump_else_b_
	_jump_else_b

case	%jump_loop_b_
	_jump_loop_b

case	%ifTrue_w_
	_if_w 1

case	%ifFalse_w_
	_if_w 0

case	%and_w_
	_cond_w 1

case	%or_w_
	_cond_w 0

case	%whileTrue_w_
	_while_w 1

case	%whileFalse_w_
	_while_w 0

case	%jump_else_w_
	_jump_else_w

case	%jump_loop_w_
	_jump_loop_w

case	%normal_send_access_
	_normal_send_access 0

case	%self_send_access_
	_normal_send_access -999

IRPC	i, 012
case	%return_tos_+i
	_return_tos i
ENDM

IRPC	i, 012
case	%return_recv_+i
	_self eax
	_return_tos i
ENDM

case	%return_tos_b_
	_return_tos 999

case	%return_recv_b_
	_self eax
	_return_tos 999

case	%zap_return_tos_b_
	_zap
	_return_tos 999

case	%zap_return_recv_b_
	_zap
	_self eax
	_return_tos 999

case	%normal_send_prim_
	_normal_send_prim 0

case	%normal_send_pop_prim_
	_normal_send_prim 1

case	%predict_prim_
case	%predict_prim_lookup_
	_predict_prim

case	%predict_prim_ifFail_
case	%predict_prim_ifFail_lookup_
	_predict_prim_ifFail

case	%eql_eql_
	_compare e

case	%neq_neq_
	_compare ne


;-----------------------------------------------------------------------------------------
; Return entry points for deoptimized interpreter frames
;
;  There is may ways of returning from an interpreter frame.
;    - from send  (with or without pop) X (with or without receiver) X (with or without restoring result value)
;    - from primitive call (with or without failure block)           X (with or without restoring result value)
;    - from DLL call                                                   (with or without restoring result value)


; SENDS
PUBLIC _deoptimized_return_from_send_without_receiver
PUBLIC _deoptimized_return_from_send_without_receiver_restore

PUBLIC _deoptimized_return_from_send_without_receiver_pop
PUBLIC _deoptimized_return_from_send_without_receiver_pop_restore

PUBLIC _deoptimized_return_from_send_with_receiver
PUBLIC _deoptimized_return_from_send_with_receiver_restore

PUBLIC _deoptimized_return_from_send_with_receiver_pop
PUBLIC _deoptimized_return_from_send_with_receiver_pop_restore

; PRIMITIVE CALLS
PUBLIC _deoptimized_return_from_primitive_call_without_failure_block
PUBLIC _deoptimized_return_from_primitive_call_without_failure_block_restore

PUBLIC _deoptimized_return_from_primitive_call_with_failure_block
PUBLIC _deoptimized_return_from_primitive_call_with_failure_block_restore

; DLL CALLS
PUBLIC _deoptimized_return_from_dll_call
PUBLIC _deoptimized_return_from_dll_call_restore

EXTRN	_result_through_unpacking: DWORD
EXTRN	_number_of_arguments_through_unpacking: DWORD

EXTRN	_nlr_testpoint_entry		; from generated interpreter


_deoptimized_C_nlr_continuation:
		_reset_last_Delta_frame
		; fall through		
_deoptimized_nlr_continuation:
		;	mov	eax, [_nlr_result]
		jmp	[_nlr_testpoint_entry]

_deoptimized_return_from_send_without_receiver_restore PROC
		mov	eax, [_number_of_arguments_through_unpacking]
		shl	eax, 2
		add	esp, eax
		mov	eax, [_result_through_unpacking]
		; fall through
ENDP

_deoptimized_return_from_send_without_receiver PROC
		_ic_info _deoptimized_nlr_continuation	; last part of the _call_method macro
		_restore_esi
		_restore_ebx
		_load_ebx
		_jump_ebx
ENDP

_deoptimized_return_from_send_without_receiver_pop_restore PROC
		mov	eax, [_number_of_arguments_through_unpacking]
		shl	eax, 2
		add	esp, eax
		mov	eax, [_result_through_unpacking]
		; fall through
ENDP

_deoptimized_return_from_send_without_receiver_pop PROC
		_ic_info _deoptimized_nlr_continuation	; last part of the _call_method macro
		_restore_esi
		_restore_ebx
		_load_ebx
		pop eax		; pop result
		_jump_ebx
ENDP

_deoptimized_return_from_send_with_receiver_restore PROC
		mov	eax, [_number_of_arguments_through_unpacking]
		shl	eax, 2
		add	esp, eax
		mov	eax, [_result_through_unpacking]
		; fall through
ENDP

_deoptimized_return_from_send_with_receiver PROC
		_ic_info _deoptimized_nlr_continuation	; last part of the _call_method macro
		_restore_esi
		_restore_ebx
		pop ecx							; pop receiver
		_load_ebx
		_jump_ebx
ENDP

_deoptimized_return_from_send_with_receiver_pop_restore PROC
		mov	eax, [_number_of_arguments_through_unpacking]
		shl	eax, 2
		add	esp, eax
		mov	eax, [_result_through_unpacking]
		; fall through
ENDP

_deoptimized_return_from_send_with_receiver_pop PROC	
		_ic_info _deoptimized_nlr_continuation	; last part of the _call_method macro
		_restore_esi
		_restore_ebx
		pop ecx							; pop receiver
		_load_ebx
		pop eax							; pop result
		_jump_ebx
ENDP

_deoptimized_return_from_primitive_call_without_failure_block_restore PROC
		mov	eax, [_number_of_arguments_through_unpacking]
		shl	eax, 2
		add	esp, eax
		mov	eax, [_result_through_unpacking]
		; fall through
ENDP

_deoptimized_return_from_primitive_call_without_failure_block PROC
		_ic_info _deoptimized_C_nlr_continuation
		_restore_esi
		_restore_ebx			; ebx := 0
		_reset_last_Delta_frame
IFDEF		assertions
		test eax, Mark_Tag
		jnz _primitive_result_wrong
ENDIF
		_load_ebx
		_jump_ebx
ENDP

_deoptimized_return_from_primitive_call_with_failure_block_restore PROC
		mov	eax, [_number_of_arguments_through_unpacking]
		shl	eax, 2
		add	esp, eax
		mov	eax, [_result_through_unpacking]
		; fall through
ENDP

_deoptimized_return_from_primitive_call_with_failure_block PROC
		_ic_info _deoptimized_C_nlr_continuation
		_restore_esi
		_restore_ebx			; ebx := 0
		_reset_last_Delta_frame
		test eax, Mark_Tag		; if not marked then
		jnz _deoptimized_return_from_primitive_call_with_failure_block_failed
		mov ecx, [esi-4]		; load jump offset
		add esi, ecx			; and jump over failure block
		_load_ebx
		_jump_ebx
ENDP

_deoptimized_return_from_primitive_call_with_failure_block_failed PROC
		and eax, NOT Mark_Tag		; else unmark result
		_load_ebx			; and execute failure block
		_jump_ebx			; the result will be stored
						; into a temp in the failure block
ENDP

_deoptimized_return_from_dll_call_restore PROC
		mov	eax, [_result_through_unpacking]
		; fall through
ENDP

_deoptimized_return_from_dll_call PROC
		_ic_info _deoptimized_C_nlr_continuation
		_reset_last_Delta_frame
		_restore_esi
		_restore_ebx			; ebx := 0
		; eax: DLL result
		mov bl, [esi-1]			; get no. of arguments
		lea esp, [esp + 4*ebx]		; adjust sp (pop arguments)
		pop ecx				; get proxy object
		mov [ecx._word32], eax		; box result
		_load_ebx
		mov eax, ecx			; return proxy
		_jump_ebx
ENDP

PUBLIC	_restart_primitiveValue

_restart_primitiveValue PROC
		push ebp			; save bp
		mov ebp, esp			; new bp
		mov ecx, [eax._context]
		mov edx, [eax._block]
		push ecx			; save recv (initialize with context)
		_restore_ebx			; if value... is called from compiled code
		add [edx._counter], invocation_counter_inc
		lea esi, [edx._hcodes]
		mov eax, ecx			; initialize temp 1 with context
		push esi			; initialize esi save
		_load_ebx
		_jump_ebx
ENDP


;-----------------------------------------------------------------------------------------
; Error handling
;
; Error codes:

start_of_runtime_system_errors	= 512

primitive_lookup_failed_errno	=  1 + start_of_runtime_system_errors
boolean_expected_errno		=  2 + start_of_runtime_system_errors
nonlocal_return_error_errno	=  3 + start_of_runtime_system_errors
halted_errno			=  4 + start_of_runtime_system_errors
illegal_code_errno         	=  5 + start_of_runtime_system_errors
not_implented_yet_errno    	=  6 + start_of_runtime_system_errors
stack_missaligned_errno   	=  7 + start_of_runtime_system_errors
ebx_wrong_errno			=  8 + start_of_runtime_system_errors
obj_wrong_errno			=  9 + start_of_runtime_system_errors
nlr_offset_wrong_errno		= 10 + start_of_runtime_system_errors
last_Delta_fp_wrong_errno	= 11 + start_of_runtime_system_errors
primitive_result_wrong_errno	= 12 + start_of_runtime_system_errors

PUBLIC	_illegal
EXTRN	_suspend_on_error: PROC

_error	MACRO errCode
LOCAL	_entry
	push eax
	_call_C _entry
	hlt
_entry:	CALL _suspend_on_error C, errCode
	ret
	ENDM

_illegal:			_print "illegal instruction - tell Robert (ebx = 0x%x, esi = 0x%x)", ebx, esi
				int 3
				_error illegal_code_errno

_non_booleans:			_error boolean_expected_errno

IFDEF	stack_checks
  _stack_missaligned:		_error stack_missaligned_errno
ENDIF

IFDEF	assertions
  _ebx_wrong:			_error ebx_wrong_errno
  _obj_wrong:			_error obj_wrong_errno
  _last_Delta_fp_wrong:		_error last_Delta_fp_wrong_errno
  _primitive_result_wrong:	_error primitive_result_wrong_errno
ENDIF


;-----------------------------------------------------------------------------------------
; NLR test point
;
; Jumped to when returning to interpreter code via NLRs. Since the NLR offsets
; in inline caches must be positive and the _nlr_testpoint_alias is 'called' from
; interpreter call sites only, it must come at the very end of interpreter code.
;
; Note: (gri 6/5/96) NLR offsets can now be negative & positive, there's no
;       such restriction anymore. This code is just here for historical reasons.
;
; For an detailed info see also ic.hpp and x86_node.cpp.

EXTRN		_nlr_testpoint_entry		; from generated interpreter

CODESEG
_C_nlr_testpoint:
		_reset_last_Delta_frame
		jmp [_nlr_testpoint_entry]
		

EXTRN	_redo_send_offset: 	PTR DWORD	; offset when redoing send
EXTRN   _verify_at_end_of_deoptimization: PROC
EXTRN	_result_through_unpacking: DWORD

PUBLIC	_redo_bytecode_after_deoptimization

_redo_bytecode_after_deoptimization PROC near
	; Call verify
	_set_last_Delta_frame
	call _verify_at_end_of_deoptimization
	_reset_last_Delta_frame

	; Redo the send
	_restore_esi
	_restore_ebx
	mov	eax, _redo_send_offset
	sub	esi, eax
	_load_ebx
	pop	eax				; get top of stack
	_jump_ebx
ENDP
	

EXTRN	_original_table: 	PTR DWORD	; dispatch table
EXTRN	_single_step_handler:	PROC

PUBLIC	_nlr_single_step_continuation

_nlr_single_step_continuation PROC near
	_reset_last_Delta_frame
	jmp [_nlr_testpoint_entry]
ENDP

_single_step_continuation PROC near
	_ic_info _nlr_single_step_continuation

	_reset_last_Delta_frame

	; pop tos
	pop	eax

    ; restore esi
	mov	esi, [ebp-8]

	; load ebx
	xor	ebx, ebx
	mov	bl, [esi]

	; jump to the clean entry
	jmp [_original_table + ebx*4]
	hlt
ENDP

ENDS
;-----------------------------------------------------------------------------------------
; char*		_interpreter_begin_addr
; char*		_interpreter_end_addr


CODESEG
_end_of_interpreter_code:
ENDS


DATASEG
PUBLIC		_interpreter_begin_addr
PUBLIC		_interpreter_end_addr

ALIGN 4
_interpreter_begin_addr	DD _begin_of_interpreter_code
_interpreter_end_addr	DD _end_of_interpreter_code
ENDS


;-----------------------------------------------------------------------------------------
; extern "C" oop call_delta(void* method, oop receiver, int nofArgs, oop* args)
;
; This is the general Delta entry point. All code that is calling the interpreter or
; compiled code is entering via this entry point. In case of an NLR leaving Delta code,
; the global NLR variables are set. Note: Needs to be *outside* the interpreters code.
;
; method must be a methodOop or a non-zombie nmethod (zombie nmethods must be filtered
; out before - calling call_delta with a zombie nmethod crashes the system).
;
; Note: Shouldn't we preserve some C registers before entering Delta and restore
;       them when coming back? At least that's what C calling convention would
;       require. Might not be a problem because the calling C funtions don't do
;       anything anymore calling call_delta. CHECK THIS! (gri 6/10/96)

EXTRN		_method_entry_point

EXTRN		_have_nlr_through_C
EXTRN		_nlr_result
EXTRN		_nlr_home
EXTRN		_nlr_home_id

EXTRN		_last_Delta_fp			; ebp of the last Delta frame before a C call
EXTRN		_last_Delta_sp			; esp of the last Delta frame before a C call
PUBLIC		_return_from_Delta		; address when returning from the Delta stack chunk

EXTRN		_C_frame_return_addr		; original return address of first C frame
						; in last_C_chunk on the stack


CODESEG
PUBLIC		_call_delta
_call_delta:
REPT 1						; only to make LOCAL work
LOCAL		method, receiver, nofArgs, args
LOCAL		_loop, _no_args, _is_compiled, _return, _nlr_test, _nlr_setup

		; incoming arguments
method		EQU ebp + 2*4
receiver	EQU ebp + 3*4
nofArgs		EQU ebp + 4*4
args		EQU ebp + 5*4

		; setup stack frame
		push ebp
		mov ebp, esp
		; _last_Delta_fp & _last_Delta_sp must be the first two words in
		; the stack frame; i.e. at ebp - 4 and ebp - 8. See also frame.hpp.
		push DWORD PTR [_last_Delta_fp]
		push DWORD PTR [_last_Delta_sp]

		; reset last Delta frame
		mov  DWORD PTR [_last_Delta_fp], 0

		; setup calling stack frame with arguments
		mov ebx, [nofArgs]		; get no. of arguments
		mov ecx, [args]			; pointer to first argument
		test ebx, ebx
		jz SHORT _no_args
_loop:		mov edx, [ecx]			; get argument
		add ecx, 4			; advance to next argument
		push edx			; push argument on stack
		dec ebx				; decrement argument counter
		jnz _loop			; until no arguments

		; call Delta method
_no_args:	mov eax, [receiver]
		_restore_ebx
		mov edx, [method]
		test edx, Mem_Tag		; test if methodOop or nmethod
		jz SHORT _is_compiled
		mov ecx, edx
		mov edx, [_method_entry_point]

_is_compiled:	; eax: receiver
		; ebx: 0
		; ecx: methodOop (if not compiled)
		; edx: calling address
		;
		; Note: no zombie nmethods possible -> no 2nd ic_info word required
		call edx
_return_from_Delta:
		_ic_info _nlr_test
		mov DWORD PTR [_have_nlr_through_C], 0

_return:	lea esp, [ebp - 8]
		pop DWORD PTR [_last_Delta_sp]	; reset _last_Delta_sp
		pop DWORD PTR [_last_Delta_fp]	; reset _last_Delta_fp
		pop ebp				; remove stack frame & return
		ret

		; When returning from Delta to C via a NLR, the following code
		; sets up the global NLR variables and patches the return address
		; of the first C frame in the last_C_chunk of the stack (see below).
_nlr_test:
		mov ecx, [ebp - 8]		; get pushed value of _last_Delta_sp
		test ecx, ecx
		jz SHORT @1996

		mov edx, [ecx - 4]		; get return address of the first C function called
		mov [_C_frame_return_addr], edx	; store return address for _nlr_setup
		mov DWORD PTR [ecx - 4], OFFSET _nlr_setup; patch return address

@1996:
		; setup global NLR variables
		mov DWORD PTR [_have_nlr_through_C], 1
		mov [_nlr_result], eax
		mov [_nlr_home], edi
		mov [_nlr_home_id], esi
		jmp _return

		; When returning from C to Delta via a NLR, the following code
		; continues an ongoing NLR. In case of a NLR, the return address of
		; the first C frame in the last_C_chunk of the stack is patched such
		; that C will return to _nlr_setup, which in turn returns to the
		; NLR testpoint of the primitive that called C.
PUBLIC _nlr_return_from_Delta
_nlr_return_from_Delta:
_nlr_setup:	_reset_last_Delta_frame
		mov eax, [_nlr_result]
		mov edi, [_nlr_home]
		mov esi, [_nlr_home_id]
		mov ebx, [_C_frame_return_addr]	; get return address
		mov ecx, [ebx + ic_info_offset]	; get nlr_offset
		sar ecx, ic_info_flags_size	; shift ic info flags out
		add ebx, ecx			; compute NLR test point address
		jmp ebx				; return to nlr test point
ENDM
ENDS


;-----------------------------------------------------------------------------------------
; _single_step_stub
;
;   parameters:
;     ebx = byte code
;     esi = byte code pointer
;

EXTRN	_frame_breakpoint: PTR DWORD		; dispatch table

CODESEG
PUBLIC _single_step_stub
_single_step_stub PROC near
	cmp	ebp, _frame_breakpoint
	jge	@99999
	jmp	[_original_table + ebx*4]
@99999:	
	; save esi
	mov		[ebp-8], esi

	; save tos
	push	eax

	_set_last_Delta_frame

	push	OFFSET _single_step_continuation
	jmp		_single_step_handler
    ; Should not reach here
	hlt
ENDP

;-----------------------------------------------------------------------------------------
; _unpack_unoptimized_frames
;
; Invoked when returning to an unoptimized frame. 
; Deoptimizes the frame_array into a stack stretch of interpreter frames
;
; _unpack_unoptimized_frames must look like a compiled inline cache
; so NLR works across unoptimized frames.
; Since a NLR may have its home inside the optimized frames we have to deoptimize
; and then continue the NLR.

PUBLIC	_unpack_unoptimized_frames

EXTRN	_setup_deoptimization_and_return_new_sp: PROC
EXTRN	_unpack_frame_array: PROC
EXTRN	_nlr_through_unpacking: DWORD

real_sender_sp	EQU ebp - 2*4
frame_array	EQU ebp - 1*4
real_fp		EQU ebp

nlr_result	EQU eax				; holds the result of the method
nlr_home	EQU edi				; the home frame ptr
nlr_home_id	EQU esi				; used to pass esi
ic_info_offset	= 1				; make sure to change this according to ic.hpp

_wrapper_for_unpack_frame_array PROC near
	push	ebp 
	mov 	ebp, esp
	call	_unpack_frame_array
	; Restore the nlr state
	cmp	[_nlr_through_unpacking], 0
	je	@8888
	mov	[_nlr_through_unpacking], 0
	mov	nlr_result,	[_nlr_result]
	mov	nlr_home,	[_nlr_home]
	mov	nlr_home_id,	[_nlr_home_id]
@8888:
	pop	ebp
	ret
ENDP

_common_unpack_unoptimized_frames PROC near
	push	ebp										; Push the old   frame pointer
	push	DWORD PTR [frame_array]					; Push the array with the packed frames
	push	DWORD PTR [real_fp]						; Push the frame pointer link
	push	DWORD PTR [real_sender_sp]				; Push the stack pointer of the calling activation
	call    _setup_deoptimization_and_return_new_sp	; Compute the new stack pointer
	mov		esp, eax 								; Set the new stack pointer
	mov		ebp, DWORD PTR [real_fp]				; Set the frame pointer to the link
	push	-1										; Push invalid return address
	jmp		_wrapper_for_unpack_frame_array			; Call the unpacking function
ENDP

_nlr_unpack_unoptimized_frames PROC near
	mov	[_nlr_through_unpacking],   1
	mov	[_nlr_result],		    nlr_result
	mov	[_nlr_home],		    nlr_home
	mov	[_nlr_home_id],		    nlr_home_id
	jmp	_common_unpack_unoptimized_frames
ENDP

_unpack_unoptimized_frames PROC near
	_ic_info _nlr_unpack_unoptimized_frames
	mov	_nlr_through_unpacking, 0
	mov [_result_through_unpacking], eax
	jmp	_common_unpack_unoptimized_frames
ENDP


; extern "C" void provoke_nlr_at(int* frame_pointer, oop* stack_pointer);

PUBLIC	_provoke_nlr_at

_provoke_nlr_at PROC near
	mov		ebp, [esp + 4]				; set new frame pointer
	mov		esp, [esp + 8]				; set new stack pointer
	mov		ebx, [esp - 4]				; find old return address

	mov		nlr_result, [_nlr_result]
	mov		nlr_home,	[_nlr_home]
	mov		nlr_home_id,[_nlr_home_id]

	mov		ecx, [ebx + ic_info_offset]	; get nlr_offset
	sar		ecx, ic_info_flags_size		; shift ic info flags out
	add		ebx, ecx					; compute NLR test point address
	jmp		ebx							; return to nlr test point
ENDP

PUBLIC	_continue_nlr_in_delta

_continue_nlr_in_delta PROC near
	mov		ebp, [esp + 4]				; set new frame pointer
	mov		esp, [esp + 8]				; set new stack pointer
	mov		ebx, [esp - 4]				; find old return address

	
	mov		nlr_result, [_nlr_result]
	mov		nlr_home,	[_nlr_home]
	mov		nlr_home_id,[_nlr_home_id]

	jmp		ebx							; continue
ENDP

ENDS

END
