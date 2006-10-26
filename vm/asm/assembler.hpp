/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.34 $ */
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

const int nofRegisters = 8;	// total number of registers

class Register: public ValueObj {
 private:
  int _number;
  
 public:
  // creation
  Register(void): _number(-1)			{}
  Register(int number, char f): _number(number)	{}	// f is only to make sure that
  							// an int is not accidentially
							// converted into a Register...

  // attributes
  int number() const		{ assert(isValid(), "not a register"); return _number; }
  bool isValid() const		{ return (0 <= _number) && (_number < nofRegisters); }
  bool hasByteRegister() const	{ return 0 <= _number && _number <= 3; }

  // comparison
  friend bool operator == (Register x, Register y) { return x._number == y._number; }
  friend bool operator != (Register x, Register y) { return x._number != y._number; }

  // debugging
  char* name() const;
};


// Available registers
const Register eax = Register(0, ' ');
const Register ecx = Register(1, ' ');
const Register edx = Register(2, ' ');
const Register ebx = Register(3, ' ');
const Register esp = Register(4, ' ');
const Register ebp = Register(5, ' ');
const Register esi = Register(6, ' ');
const Register edi = Register(7, ' ');
const Register noreg; // Dummy register used in Load, LoadAddr, and Store.


// Label represent a target destination for jumps, calls and non-local returns.
// After declaration they can be freely used to denote known or (yet) unknown
// target destinations. Assembler::bind is used to bind a label to the current
// code position. A label can be bound only once.

class Label : public ValueObj {
 private:
  // _pos encodes both the binding state (via its sign)
  // and the binding position (via its value) of a label.
  //
  // _pos <  0	bound label, pos() returns the target (jump) position
  // _pos == 0	unused label
  // _pos >  0	unbound label, pos() returns the last displacement (see .cpp file) in the chain
  int _pos;

  int pos() const {
    if (_pos < 0) return -_pos - 1;
    if (_pos > 0) return  _pos - 1;
    ShouldNotReachHere();
    return 0;
  }

  void bind_to(int pos)		{ assert(pos >= 0, "illegal position"); _pos = -pos - 1; }
  void link_to(int pos)		{ assert(pos >= 0, "illegal position"); _pos =  pos + 1; }
  void unuse()			{ _pos = 0; }

 public:
  bool is_bound() const		{ return _pos <  0; }
  bool is_unbound() const	{ return _pos >  0; }
  bool is_unused() const	{ return _pos == 0; }

  Label() : _pos(0)		{}
  ~Label()			{ assert(!is_unbound(), "unbound label"); }

  friend class Assembler;
  friend class MacroAssembler;
  friend class Displacement;
};


// Address operands for assembler

class Address: public ValueObj {
 public:
  enum ScaleFactor {
    no_scale	= -1,
    times_1	=  0,
    times_2	=  1,
    times_4	=  2,
    times_8	=  3
  };

 private:
  Register		_base;
  Register		_index;
  ScaleFactor		_scale;
  int			_disp;
  relocInfo::relocType	_rtype;

 public:
  Address();
  Address(int disp, relocInfo::relocType rtype);
  Address(Register base, int disp = 0, relocInfo::relocType rtype = relocInfo::none);
  Address(Register base, Register index, ScaleFactor scale, int disp = 0, relocInfo::relocType rtype = relocInfo::none);

  friend class Assembler;
};


// The assembler for the new backend

class Assembler: public ResourceObj {
 protected:
  CodeBuffer* _code;

  char* _code_begin;			// first byte of code buffer
  char* _code_limit;			// first byte after code buffer
  char* _code_pos;			// current code generation position

  Label _unbound_label;			// the last label to be bound to _binding_pos, if unbound
  int	_binding_pos;			// the position to which _unbound_label has to be bound, if there

  char* addr_at(int pos) 		{ return _code_begin + pos; }

  int  byte_at(int pos)			{ return *(unsigned char*)addr_at(pos); }
  void byte_at_put(int pos, int x)	{ *(unsigned char*)addr_at(pos) = (unsigned char)x; }

  int  long_at(int pos)			{ return *(int*)addr_at(pos); }
  void long_at_put(int pos, int x)	{ *(int*)addr_at(pos) = x; }

  bool is8bit(int x)			{ return -0x80 <= x && x < 0x80; }
  bool isByte(int x)			{ return 0 <= x && x < 0x100; }
  bool isShiftCount(int x)		{ return 0 <= x && x < 32; }

  inline void emit_byte(int x);
  inline void emit_long(int x);
  inline void emit_data(int data, relocInfo::relocType rtype);

  void emit_arith_b(int op1, int op2, Register dst, int imm8);

  void emit_arith(int op1, int op2, Register dst, int imm32);
  void emit_arith(int op1, int op2, Register dst, oop obj);
  void emit_arith(int op1, int op2, Register dst, Register src);

  void emit_operand(Register reg, Register base, Register index, Address::ScaleFactor scale, int disp, relocInfo::relocType rtype);
  void emit_operand(Register reg, Address adr);

  void emit_farith(int b1, int b2, int i);

  void print  (Label& L);
  void bind_to(Label& L, int pos);
  void link_to(Label& L, Label& appendix);

 public:
  enum Condition {
    zero	= 0x4,
    notZero	= 0x5,
    equal	= 0x4,
    notEqual	= 0x5,
    less	= 0xc,
    lessEqual	= 0xe,
    greater	= 0xf,
    greaterEqual= 0xd,
    below	= 0x2,
    belowEqual	= 0x6,
    above	= 0x7,
    aboveEqual	= 0x3,
    overflow	= 0x0,
    noOverflow	= 0x1,
    carrySet	= 0x2,
    carryClear	= 0x3,
    negative	= 0x8,
    positive	= 0x9,
  };

  enum Constants {
    sizeOfCall = 5			// length of call instruction in bytes
  };

  Assembler(CodeBuffer* code);

  void		finalize();		// call this before using/copying the code
  CodeBuffer*	code() const		{ return _code; }
  char*		pc() const		{ return _code_pos; }
  int		offset() const		{ return _code_pos - _code_begin; }

  // Stack
  void pushad();
  void popad();

  void pushl(int imm32);
  void pushl(oop obj);
  void pushl(Register src);
  void pushl(Address src);

  void popl(Register dst);
  void popl(Address dst);

  // Moves
  void movb(Register dst, Address src);
  void movb(Address dst, int imm8);
  void movb(Address dst, Register src);

  void movw(Register dst, Address src);
  void movw(Address dst, Register src);

  void movl(Register dst, int imm32);
  void movl(Register dst, oop obj);
  void movl(Register dst, Register src);
  void movl(Register dst, Address src);

  void movl(Address dst, int imm32);
  void movl(Address dst, oop obj);
  void movl(Address dst, Register src);

  void movsxb(Register dst, Address src);
  void movsxb(Register dst, Register src);

  void movsxw(Register dst, Address src);
  void movsxw(Register dst, Register src);

  // Conditional moves (P6 only)
  void cmovccl(Condition cc, Register dst, int imm32);
  void cmovccl(Condition cc, Register dst, oop obj);
  void cmovccl(Condition cc, Register dst, Register src);
  void cmovccl(Condition cc, Register dst, Address src);

  // Arithmetics
  void adcl(Register dst, int imm32);
  void adcl(Register dst, Register src);

  void addl(Address dst, int imm32);
  void addl(Register dst, int imm32);
  void addl(Register dst, Register src);

  void andl(Register dst, int imm32);
  void andl(Register dst, Register src);

  void cmpl(Address dst, int imm32);
  void cmpl(Address dst, oop obj);
  void cmpl(Register dst, int imm32);
  void cmpl(Register dst, oop obj);
  void cmpl(Register dst, Register src);
  void cmpl(Register dst, Address src);

  void decb(Register dst);
  void decl(Register dst);

  void idivl(Register src);

  void imull(Register dst, Register src);
  void imull(Register dst, Register src, int value);

  void incl(Register dst);
  void incl(Address dst);

  void leal(Register dst, Address src);

  void mull(Register src);

  void negl(Register dst);

  void notl(Register dst);

  void orl(Register dst, int imm32);
  void orl(Register dst, Register src);
  void orl(Register dst, Address src);

  void rcll(Register dst, int imm8);

  void sarl(Register dst, int imm8);
  void sarl(Register dst);

  void sbbl(Register dst, int imm32);
  void sbbl(Register dst, Register src);

  void shldl(Register dst, Register src);

  void shll(Register dst, int imm8);
  void shll(Register dst);

  void shrdl(Register dst, Register src);

  void shrl(Register dst, int imm8);
  void shrl(Register dst);

  void subl(Register dst, int imm32);
  void subl(Register dst, Register src);

  void testb(Register dst, int imm8);
  void testl(Register dst, int imm32);
  void testl(Register dst, Register src);

  void xorl(Register dst, int imm32);
  void xorl(Register dst, Register src);

  // Miscellaneous
  void hlt();
  void int3();
  void nop();
  void ret(int imm16);

  // Labels

  void bind(Label& L);			// binds an unbound label L to the current code position
  void merge(Label& L, Label& with);	// merges L and with, L is the merged label

  // Calls
  void call(Label& L);
  void call(char* entry, relocInfo::relocType rtype);
  void call(Register reg);
  void call(Address adr);
  
  // Jumps
  void jmp(char* entry, relocInfo::relocType rtype);
  void jmp(Register reg);
  void jmp(Address adr);

  // Label operations & relative jumps (PPUM Appendix D)
  void jmp(Label& L);		// unconditional jump to L

  // jccI is the generic conditional branch generator to run-
  // time routines, jcc is used for branches to labels. jcc
  // takes a branch opcode (cc) and a label (L) and generates
  // either a backward branch or a forward branch and links it
  // to the label fixup chain. Usage:
  //
  // Label L;		// unbound label
  // jcc(cc, L);	// forward branch to unbound label
  // bind(L);		// bind label to the current pc
  // jcc(cc, L);	// backward branch to bound label
  // bind(L);		// illegal: a label may be bound only once
  //
  // Note: The same Label can be used for forward and backward branches
  // but it may be bound only once.

  void jcc(Condition cc, char* dst, relocInfo::relocType rtype = relocInfo::runtime_call_type);
  void jcc(Condition cc, Label& L);

  // Support for inline cache information (see also IC_Info)
  void ic_info(Label& L, int flags);
  
  // Floating-point operations
  void fld1();
  void fldz();

  void fld_s(Address adr);
  void fld_d(Address adr);

  void fstp_s(Address adr);
  void fstp_d(Address adr);

  void fild_s(Address adr);
  void fild_d(Address adr);

  void fistp_s(Address adr);
  void fistp_d(Address adr);

  void fabs();
  void fchs();

  void fadd(int i);
  void fsub(int i);
  void fmul(int i);
  void fdiv(int i);

  void faddp(int i = 1);
  void fsubp(int i = 1);
  void fsubrp(int i = 1);
  void fmulp(int i = 1);
  void fdivp(int i = 1);
  void fprem();
  void fprem1();

  void fxch(int i = 1);
  void fincstp();
  void ffree(int i = 0);

  void ftst();
  void fcompp();
  void fnstsw_ax();
  void fwait();

  // For compatibility with old assembler only - should be removed at some point
  void Load (Register base, int disp, Register dst) { movl(dst, Address(base, disp)); }
  void Store(Register src, Register base, int disp) { movl(Address(base, disp), src); }
};


// MacroAssembler extends Assembler by a few macros used for
// generating the interpreter and for compiled code.

class MacroAssembler: public Assembler {
 public:
  MacroAssembler(CodeBuffer* code) : Assembler(code) {}

  // Alignment
  void align(int modulus);

  // Test-Instructions optimized for length
  void test (Register dst, int imm8);		// use testb if possible, testl otherwise
  
  // Stack frame operations
  void enter();
  void leave();

  // Support for inlined data

  void inline_oop(oop o);

  // C calls
  void set_last_Delta_frame_before_call();	// assumes that the return address has not been pushed yet
  void set_last_Delta_frame_after_call();	// assumes that the return address has been pushed already
  void reset_last_Delta_frame();

  void call_C(Label& L);
  void call_C(Label& L, Label& nlrTestPoint);

  void call_C(char* entry, relocInfo::relocType rtype);
  void call_C(char* entry, relocInfo::relocType rtype, Label& nlrTestPoint);

  void call_C(Register entry);
  void call_C(Register entry, Label& nlrTestPoint);

  // C calls to run-time routines with arguments (args are not preserved)
  void call_C(char* entry, Register arg1);
  void call_C(char* entry, Register arg1, Register arg2);
  void call_C(char* entry, Register arg1, Register arg2, Register arg3);
  void call_C(char* entry, Register arg1, Register arg2, Register arg3, Register arg4);

  // Stores
  void store_check(Register obj, Register tmp);

  // Floating-point comparisons
  // To jump conditionally on cc, test FPU status word with mask and
  // jump conditionally using cond.
  static void fpu_mask_and_cond_for(Condition cc, int& mask, Condition& cond);

  // Pop ST (ffree & fincstp combined)
  void fpop();

  // debugging
  static void print_reg(char* name, oop obj);
  static void inspector(oop edi, oop esi, oop ebp, oop esp, oop ebx, oop edx, oop ecx, oop eax, char* eip);
  void inspect(char* title = NULL);
};
