/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.37 $ */
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

#include "incls/_precompiled.incl"
#include "incls/_assembler.cpp.incl"


// A Displacement describes the 32bit immediate field of an instruction which
// may be used together with a Label in order to refer to a yet unknown code
// position. Displacements stored in the instruction stream are used to describe
// the instruction and to chain a list of instructions using the same Label.
// A Displacement contains 3 different fields:
//
// next field: position of next displacement in the chain (0 = end of list)
// type field: instruction type
// info field: instruction specific information
//
// A next value of null (0) indicates the end of a chain (note that there can
// be no displacement at position zero, because there is always at least one
// instruction byte before the displacement).
//
// Displacement _data field layout
//
// |31....10|9......8|7......0|
// [  next  |  type  |  info  ]

class Displacement : public ValueObj {
 private:
  int _data;

  enum Layout {
    info_size	=  IC_Info::number_of_flags,
    type_size	=  2,
    next_size	= 32 - (type_size + info_size),

    info_pos	= 0,
    type_pos	= info_pos + info_size,
    next_pos	= type_pos + type_size,

    info_mask	= (1 << info_size) - 1,
    type_mask	= (1 << type_size) - 1,
    next_mask	= (1 << next_size) - 1,
  };

  enum Type {			// info field usage
    call,			// unused
    absolute_jump,		// unused
    conditional_jump,		// condition code
    ic_info,			// flags
  };

  void init(Label& L, Type type, int info) {
    assert(!L.is_bound(), "label is bound");
    int next = 0;
    if (L.is_unbound()) {
      next = L.pos();
      assert(next > 0, "Displacements must be at positions > 0");
    }
    assert((next & ~next_mask) == 0, "next field too small");
    assert((type & ~type_mask) == 0, "type field too small");
    assert((info & ~info_mask) == 0, "info field too small");
    _data = (next << next_pos) | (type << type_pos) | (info << info_pos);
  }

  int  data() const		{ return _data; }
  int  info() const		{ return     ((_data >> info_pos) & info_mask); }
  Type type() const		{ return Type((_data >> type_pos) & type_mask); }
  void next(Label & L) const	{ int n    = ((_data >> next_pos) & next_mask); n > 0 ? L.link_to(n) : L.unuse(); }
  void link_to(Label& L)	{ init(L, type(), info()); }

  Displacement(int data)	{ _data = data; }

  Displacement(Label& L, Type type, int info) {
    init(L, type, info);
  }

  void print() {
    char* s;
    switch (type()) {
      case call            : s = "call"; break;
      case absolute_jump   : s = "jmp "; break;
      case conditional_jump: s = "jcc "; break;
      case ic_info         : s = "nlr "; break;
      default              : s = "????"; break;
    }
    std->print("%s (info = 0x%x)", s, info());
  }

  friend class Assembler;
  friend class MacroAssembler;
};


// Use macros (otherwise must also declare Displacement class in .hpp file)
#define disp_at(L)		Displacement(long_at((L).pos()))
#define disp_at_put(L,disp)	long_at_put((L).pos(), (disp).data())
#define emit_disp(L,type,info)	{ Displacement disp((L), (type), (info));	\
				  L.link_to(offset());				\
				  emit_long(int(disp.data()));			\
				}

// Implementation of Register
char* registerNames[nofRegisters] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};

char* Register::name() const {
  return isValid() ? registerNames[_number] : "noreg";
}


// Implementation of Address

Address::Address() {
  _base  = noreg;
  _index = noreg;
  _scale = no_scale;
  _disp  = 0;
  _rtype = relocInfo::none;
}


Address::Address(int disp, relocInfo::relocType rtype) {
  _base  = noreg;
  _index = noreg;
  _scale = no_scale;
  _disp  = disp;
  _rtype = rtype;
}


Address::Address(Register base, int disp, relocInfo::relocType rtype) {
  _base  = base;
  _index = noreg;
  _scale = no_scale;
  _disp  = disp;
  _rtype = rtype;
}


Address::Address(Register base, Register index, ScaleFactor scale, int disp, relocInfo::relocType rtype) {
  assert((index == noreg) == (scale == Address::no_scale), "inconsistent address");
  _base  = base;
  _index = index;
  _scale = scale;
  _disp  = disp;
  _rtype = rtype;
}


// Implementation of Assembler

Assembler::Assembler(CodeBuffer* code) {
  _code        = code;
  _code_begin  = code->code_begin();
  _code_limit  = code->code_limit();
  _code_pos    = code->code_end();
}


void Assembler::finalize() {
  if (_unbound_label.is_unbound()) {
    bind_to(_unbound_label, _binding_pos);
  }
}


inline void Assembler::emit_byte(int x) {
  assert(isByte(x), "not a byte");
  *(unsigned char*)_code_pos = (unsigned char)x;
  _code_pos += sizeof(unsigned char);
  code()->set_code_end(_code_pos);
}


inline void Assembler::emit_long(int x) {
  *(int*)_code_pos = x;
  _code_pos += sizeof(int);
  code()->set_code_end(_code_pos);
}


inline void Assembler::emit_data(int data, relocInfo::relocType rtype) {
  if (rtype != relocInfo::none) code()->relocate(_code_pos, rtype);
  emit_long(data);
}


void Assembler::emit_arith_b(int op1, int op2, Register dst, int imm8) {
  guarantee(dst.hasByteRegister(), "must have byte register");
  assert(isByte(op1) && isByte(op2), "wrong opcode");
  assert(isByte(imm8), "not a byte");
  assert((op1 & 0x01) == 0, "should be 8bit operation");
  emit_byte(op1);
  emit_byte(op2 | dst.number());
  emit_byte(imm8);
}


void Assembler::emit_arith(int op1, int op2, Register dst, int imm32) {
  assert(isByte(op1) && isByte(op2), "wrong opcode");
  assert((op1 & 0x01) == 1, "should be 32bit operation");
  assert((op1 & 0x02) == 0, "sign-extension bit should not be set");
  if (is8bit(imm32)) {
    emit_byte(op1 | 0x02); // set sign bit
    emit_byte(op2 | dst.number());
    emit_byte(imm32 & 0xFF);
  } else {
    emit_byte(op1);
    emit_byte(op2 | dst.number());
    emit_long(imm32);
  }
}


void Assembler::emit_arith(int op1, int op2, Register dst, oop obj) {
  assert(isByte(op1) && isByte(op2), "wrong opcode");
  assert((op1 & 0x01) == 1, "should be 32bit operation");
  assert((op1 & 0x02) == 0, "sign-extension bit should not be set");
  emit_byte(op1);
  emit_byte(op2 | dst.number());
  emit_data((int)obj, relocInfo::oop_type);
}


void Assembler::emit_arith(int op1, int op2, Register dst, Register src) {
  assert(isByte(op1) && isByte(op2), "wrong opcode");
  emit_byte(op1);
  emit_byte(op2 | dst.number() << 3 | src.number());
}


void Assembler::emit_operand(Register reg, Register base, Register index, Address::ScaleFactor scale, int disp, relocInfo::relocType rtype) {
  if (base.isValid()) {
    if (index.isValid()) {
      assert(scale != Address::no_scale, "inconsistent address");
      // [base + index*scale + disp]
      if (disp == 0 && rtype == relocInfo::none) {
        // [base + index*scale]
        // [00 reg 100][ss index base]
        assert(index != esp && base != ebp, "illegal addressing mode");
        emit_byte(0x04 | reg.number() << 3);
        emit_byte(scale << 6 | index.number() << 3 | base.number());
      } else if (is8bit(disp) && rtype == relocInfo::none) {
        // [base + index*scale + imm8]
        // [01 reg 100][ss index base] imm8
	assert(index != esp, "illegal addressing mode");
        emit_byte(0x44 | reg.number() << 3);
        emit_byte(scale << 6 | index.number() << 3 | base.number());
        emit_byte(disp & 0xFF);
      } else {
        // [base + index*scale + imm32]
        // [10 reg 100][ss index base] imm32
	assert(index != esp, "illegal addressing mode");
	emit_byte(0x84 | reg.number() << 3);
        emit_byte(scale << 6 | index.number() << 3 | base.number());
	emit_data(disp, rtype);
      }
    } else if (base == esp) {
      // [esp + disp]
      if (disp == 0 && rtype == relocInfo::none) {
        // [esp]
        // [00 reg 100][00 100 100]
        emit_byte(0x04 | reg.number() << 3);
        emit_byte(0x24);
      } else if (is8bit(disp) && rtype == relocInfo::none) {
        // [esp + imm8]
        // [01 reg 100][00 100 100] imm8
        emit_byte(0x44 | reg.number() << 3);
        emit_byte(0x24);
        emit_byte(disp & 0xFF);
      } else {
        // [esp + imm32]
        // [10 reg 100][00 100 100] imm32
	emit_byte(0x84 | reg.number() << 3);
        emit_byte(0x24);
	emit_data(disp, rtype);
      }
    } else {
      // [base + disp]
      assert(base != esp, "illegal addressing mode");
      if (disp == 0 && rtype == relocInfo::none && base != ebp) {
        // [base]
        // [00 reg base]
	assert(base != ebp, "illegal addressing mode");
        emit_byte(0x00 | reg.number() << 3 | base.number());
      } else if (is8bit(disp) && rtype == relocInfo::none) {
        // [base + imm8]
        // [01 reg base] imm8
        emit_byte(0x40 | reg.number() << 3 | base.number());
        emit_byte(disp & 0xFF);
      } else {
        // [base + imm32]
        // [10 reg base] imm32
        emit_byte(0x80 | reg.number() << 3 | base.number());
        emit_data(disp, rtype);
      }
    }
  } else {
    if (index.isValid()) {
      assert(scale != Address::no_scale, "inconsistent address");
      // [index*scale + disp]
      // [00 reg 100][ss index 101] imm32
      assert(index != esp, "illegal addressing mode");
      emit_byte(0x04 | reg.number() << 3);
      emit_byte(scale << 6 | index.number() << 3 | 0x05);
      emit_data(disp, rtype);
    } else {
      // [disp]
      // [00 reg 101] imm32
      emit_byte(0x05 | reg.number() << 3);
      emit_data(disp, rtype);
    }
  }
}


void Assembler::emit_operand(Register reg, Address adr) {
  emit_operand(reg, adr._base, adr._index, adr._scale, adr._disp, adr._rtype);
}


void Assembler::emit_farith(int b1, int b2, int i) {
  assert(isByte(b1) && isByte(b2), "wrong opcode");
  assert(0 <= i &&  i < 8, "illegal stack offset");
  emit_byte(b1);
  emit_byte(b2 + i);
}


void Assembler::pushad() {
  emit_byte(0x60);
}


void Assembler::popad() {
  emit_byte(0x61);
}


void Assembler::pushl(int imm32) {
  emit_byte(0x68);
  emit_long(imm32);
}


void Assembler::pushl(oop obj) {
  emit_byte(0x68);
  emit_data((int)obj, relocInfo::oop_type);
}


void Assembler::pushl(Register src) {
  emit_byte(0x50 | src.number());
}


void Assembler::pushl(Address src) {
  emit_byte(0xFF);
  emit_operand(esi, src);
}


void Assembler::popl(Register dst) {
  emit_byte(0x58 | dst.number());
}


void Assembler::popl(Address dst) {
  emit_byte(0x8F);
  emit_operand(eax, dst);
}


void Assembler::movb(Register dst, Address src) {
  guarantee(dst.hasByteRegister(), "must have byte register");
  emit_byte(0x8A);
  emit_operand(dst, src);
}


void Assembler::movb(Address dst, int imm8) {
  emit_byte(0xC6);
  emit_operand(eax, dst);
  emit_byte(imm8);
}


void Assembler::movb(Address dst, Register src) {
  guarantee(src.hasByteRegister(), "must have byte register");
  emit_byte(0x88);
  emit_operand(src, dst);
}


void Assembler::movw(Register dst, Address src) {
  emit_byte(0x66);
  emit_byte(0x8B);
  emit_operand(dst, src);
}


void Assembler::movw(Address dst, Register src) {
  emit_byte(0x66);
  emit_byte(0x89);
  emit_operand(src, dst);
}


void Assembler::movl(Register dst, int imm32) {
  emit_byte(0xB8 | dst.number());
  emit_long(imm32);
}


void Assembler::movl(Register dst, oop obj) {
  emit_byte(0xB8 | dst.number());
  emit_data((int)obj, relocInfo::oop_type);
}


void Assembler::movl(Register dst, Register src) {
  emit_byte(0x8B);
  emit_byte(0xC0 | (dst.number() << 3) | src.number());
}


void Assembler::movl(Register dst, Address src) {
  emit_byte(0x8B);
  emit_operand(dst, src);
}


void Assembler::movl(Address dst, int imm32) {
  emit_byte(0xC7);
  emit_operand(eax, dst);
  emit_long(imm32);
}


void Assembler::movl(Address dst, oop obj) {
  emit_byte(0xC7);
  emit_operand(eax, dst);
  emit_data((int)obj, relocInfo::oop_type);
}


void Assembler::movl(Address dst, Register src) {
  emit_byte(0x89);
  emit_operand(src, dst);
}


void Assembler::movsxb(Register dst, Address src) {
  emit_byte(0x0F);
  emit_byte(0xBE);
  emit_operand(dst, src);
}


void Assembler::movsxb(Register dst, Register src) {
  guarantee(src.hasByteRegister(), "must have byte register");
  emit_byte(0x0F);
  emit_byte(0xBE);
  emit_byte(0xC0 | (dst.number() << 3) | src.number());
}


void Assembler::movsxw(Register dst, Address src) {
  emit_byte(0x0F);
  emit_byte(0xBF);
  emit_operand(dst, src);
}


void Assembler::movsxw(Register dst, Register src) {
  emit_byte(0x0F);
  emit_byte(0xBF);
  emit_byte(0xC0 | (dst.number() << 3) | src.number());
}


void Assembler::cmovccl(Condition cc, Register dst, int imm32) {
  Unimplemented();
}


void Assembler::cmovccl(Condition cc, Register dst, oop obj) {
  Unimplemented();
}


void Assembler::cmovccl(Condition cc, Register dst, Register src) {
  Unimplemented();
}


void Assembler::cmovccl(Condition cc, Register dst, Address src) {
  Unimplemented();
}


void Assembler::adcl(Register dst, int imm32) {
  emit_arith(0x81, 0xD0, dst, imm32);
}


void Assembler::adcl(Register dst, Register src) {
  emit_arith(0x13, 0xC0, dst, src);
}


void Assembler::addl(Address dst, int imm32) {
  emit_byte(0x81);
  emit_operand(eax, dst);
  emit_long(imm32);
}


void Assembler::addl(Register dst, int imm32) {
  emit_arith(0x81, 0xC0, dst, imm32);
}


void Assembler::addl(Register dst, Register src) {
  emit_arith(0x03, 0xC0, dst, src);
}

void Assembler::addl(Register dst, Address  src) {
  emit_byte(0x03);
  emit_operand(dst, src);
}


void Assembler::andl(Register dst, int imm32) {
  emit_arith(0x81, 0xE0, dst, imm32);
}


void Assembler::andl(Register dst, Register src) {
  emit_arith(0x23, 0xC0, dst, src);
}


void Assembler::cmpl(Address dst, int imm32) {
  emit_byte(0x81);
  emit_operand(edi, dst);
  emit_long(imm32);
}


void Assembler::cmpl(Address dst, oop obj) {
  emit_byte(0x81);
  emit_operand(edi, dst);
  emit_data((int)obj, relocInfo::oop_type);
}


void Assembler::cmpl(Register dst, int imm32) {
  emit_arith(0x81, 0xF8, dst, imm32);
}


void Assembler::cmpl(Register dst, oop obj) {
  emit_arith(0x81, 0xF8, dst, obj);
}


void Assembler::cmpl(Register dst, Register src) {
  emit_arith(0x3B, 0xC0, dst, src);
}


void Assembler::cmpl(Register dst, Address  src) {
  emit_byte(0x3B);
  emit_operand(dst, src);
}


void Assembler::decb(Register dst) {
  guarantee(dst.hasByteRegister(), "must have byte register");
  emit_byte(0xFE);
  emit_byte(0xC8 | dst.number());
}


void Assembler::decl(Register dst) {
  emit_byte(0x48 | dst.number());
}

void Assembler::decl(Address dst) {
  emit_byte(0xFF);
  emit_operand(ecx, dst);
}

void Assembler::idivl(Register src) {
  emit_byte(0xF7);
  emit_byte(0xF8 | src.number());
}

void Assembler::imull(Register src) {
  emit_byte(0xF7);
  emit_byte(0xE8 | src.number());
}

void Assembler::imull(Register dst, Register src) {
  emit_byte(0x0F);
  emit_byte(0xAF);
  emit_byte(0xC0 | dst.number() << 3 | src.number());
}


void Assembler::imull(Register dst, Register src, int value) {
  if (is8bit(value)) {
    emit_byte(0x6B);
    emit_byte(0xC0 | dst.number() << 3 | src.number());
    emit_byte(value);
  } else {
    emit_byte(0x69);
    emit_byte(0xC0 | dst.number() << 3 | src.number());
    emit_long(value);
  }
}


void Assembler::incl(Register dst) {
  emit_byte(0x40 | dst.number());
}


void Assembler::incl(Address dst) {
  emit_byte(0xFF);
  emit_operand(eax, dst);
}


void Assembler::leal(Register dst, Address src) {
  emit_byte(0x8D);
  emit_operand(dst, src);
}


void Assembler::mull(Register src) {
  emit_byte(0xF7);
  emit_byte(0xE0 | src.number());
}


void Assembler::negl(Register dst) {
  emit_byte(0xF7);
  emit_byte(0xD8 | dst.number());
}


void Assembler::notl(Register dst) {
  emit_byte(0xF7);
  emit_byte(0xD0 | dst.number());
}


void Assembler::orl(Register dst, int imm32) {
  emit_arith(0x81, 0xC8, dst, imm32);
}


void Assembler::orl(Register dst, Register src) {
  emit_arith(0x0B, 0xC0, dst, src);
}



void Assembler::orl(Register dst, Address src) {
  emit_byte(0x0B);
  emit_operand(dst, src);
}


void Assembler::rcll(Register dst, int imm8) {
  assert(isShiftCount(imm8), "illegal shift count");
  if (imm8 == 1) {
    emit_byte(0xD1);
    emit_byte(0xD0 | dst.number());
  } else {
    emit_byte(0xC1);
    emit_byte(0xD0 | dst.number());
    emit_byte(imm8);
  }
}


void Assembler::sarl(Register dst, int imm8) {
  assert(isShiftCount(imm8), "illegal shift count");
  if (imm8 == 1) {
    emit_byte(0xD1);
    emit_byte(0xF8 | dst.number());
  } else {
    emit_byte(0xC1);
    emit_byte(0xF8 | dst.number());
    emit_byte(imm8);
  }
}


void Assembler::sarl(Register dst) {
  emit_byte(0xD3);
  emit_byte(0xF8 | dst.number());
}


void Assembler::sbbl(Register dst, int imm32) {
  Unimplemented();
}


void Assembler::sbbl(Register dst, Register src) {
  emit_arith(0x1B, 0xC0, dst, src);
}


void Assembler::shldl(Register dst, Register src) {
  emit_byte(0x0F);
  emit_byte(0xA5);
  emit_byte(0xC0 | src.number() << 3 | dst.number());
}


void Assembler::shll(Register dst, int imm8) {
  assert(isShiftCount(imm8), "illegal shift count");
  if (imm8 == 1 ) {
    emit_byte(0xD1);
    emit_byte(0xE0 | dst.number());
  } else {
    emit_byte(0xC1);
    emit_byte(0xE0 | dst.number());
    emit_byte(imm8);
  }
}


void Assembler::shll(Register dst) {
  emit_byte(0xD3);
  emit_byte(0xE0 | dst.number());
}


void Assembler::shrdl(Register dst, Register src) {
  emit_byte(0x0F);
  emit_byte(0xAD);
  emit_byte(0xC0 | src.number() << 3 | dst.number());
}


void Assembler::shrl(Register dst, int imm8) {
  assert(isShiftCount(imm8), "illegal shift count");
  emit_byte(0xC1);
  emit_byte(0xE8 | dst.number());
  emit_byte(imm8);
}


void Assembler::shrl(Register dst) {
  emit_byte(0xD3);
  emit_byte(0xE8 | dst.number());
}


void Assembler::subl(Register dst, int imm32) {
  emit_arith(0x81, 0xE8, dst, imm32);
}


void Assembler::subl(Register dst, Register src) {
  emit_arith(0x2B, 0xC0, dst, src);
}

void Assembler::subl(Register dst, Address src) {
  emit_byte(0x2B);
  emit_operand(dst, src);
}


void Assembler::testb(Register dst, int imm8) {
  guarantee(dst.hasByteRegister(), "must have byte register");
  emit_arith_b(0xF6, 0xC0, dst, imm8);
}


void Assembler::testl(Register dst, int imm32) {
  // not using emit_arith because test
  // doesn't support sign-extension of
  // 8bit operands
  if (dst.number() == 0) {
    emit_byte(0xA9);
  } else {
    emit_byte(0xF7);
    emit_byte(0xC0 | dst.number());
  }
  emit_long(imm32);
}


void Assembler::testl(Register dst, Register src) {
  emit_arith(0x85, 0xC0, dst, src);
}


void Assembler::xorl(Register dst, int imm32) {
  emit_arith(0x81, 0xF0, dst, imm32);
}


void Assembler::xorl(Register dst, Register src) {
  emit_arith(0x33, 0xC0, dst, src);
}

void Assembler::cdq() {
  emit_byte(0x99);
}

void Assembler::hlt() {
  emit_byte(0xF4);
}


void Assembler::int3() {
  if (EnableInt3) emit_byte(0xCC);
}


void Assembler::nop() {
  emit_byte(0x90);
}


void Assembler::ret(int imm16) {
  if (imm16 == 0) {
    emit_byte(0xC3);
  } else {
    emit_byte(0xC2);
    emit_byte(imm16 & 0xFF);
    emit_byte((imm16 >> 8) & 0xFF);
  }
}


// Labels refer to positions in the (to be) generated code.
// There are bound, unbound and undefined labels.
//
// Bound labels refer to known positions in the already
// generated code. pos() is the position the label refers to.
//
// Unbound labels refer to unknown positions in the code
// to be generated; pos() is the position of the 32bit
// Displacement of the last instruction using the label.
//
// Undefined labels are labels that haven't been used yet.
// They refer to no position at all.


void Assembler::print(Label& L) {
  if (L.is_unused()) {
    std->print_cr("undefined label");
  } else if (L.is_bound()) {
    std->print_cr("bound label to %d", L.pos());
  } else if (L.is_unbound()) {
    Label l = L;
    std->print_cr("unbound label");
    while (l.is_unbound()) {
      Displacement disp = disp_at(l);
      std->print("@ %d ", l.pos());
      disp.print();
      std->cr();
      disp.next(l);
    }
  } else {
    std->print_cr("label in inconsistent state (pos = %d)", L._pos);
  }
}


void Assembler::bind_to(Label& L, int pos) {
  bool tellRobert = false;

  assert(0 <= pos && pos <= offset(), "must have a valid binding position");
  while (L.is_unbound()) {
    Displacement disp = disp_at(L);
    int fixup_pos = L.pos();
    int imm32 = 0;
    switch (disp.type()) {
      case Displacement::call:
        { assert(byte_at(fixup_pos - 1) == 0xE8, "call expected");
          imm32 = pos - (fixup_pos + sizeof(int));
        }
	break;
      case Displacement::absolute_jump:
        { assert(byte_at(fixup_pos - 1) == 0xE9, "jmp expected");
          imm32 = pos - (fixup_pos + sizeof(int));
	  if (imm32 == 0 && EliminateJumpsToJumps) tellRobert = true;
        }
	break;
      case Displacement::conditional_jump:
        { assert(byte_at(fixup_pos - 2) == 0x0F, "jcc expected");
          assert(byte_at(fixup_pos - 1) == (0x80 | disp.info()), "jcc expected");
          imm32 = pos - (fixup_pos + sizeof(int));
	}
	break;
      case Displacement::ic_info:
        { assert(byte_at(fixup_pos - 1) == 0xA9, "test eax expected");
	  int offs = pos - (fixup_pos - IC_Info::info_offset);
	  assert(((offs << IC_Info::number_of_flags) >> IC_Info::number_of_flags) == offs, "NLR offset out of bounds");
	  imm32 = (offs << IC_Info::number_of_flags) | disp.info();
	}
        break;
      default:
        ShouldNotReachHere();
    }
    long_at_put(fixup_pos, imm32);
    disp.next(L);
  }
  L.bind_to(pos);

  if (tellRobert) {
    //warning("jmp to next has not been eliminated - tell Robert, please");
    code()->decode();
  }
}


void Assembler::link_to(Label& L, Label& appendix) {
  if (appendix.is_unbound()) {
    if (L.is_unbound()) {
      // append appendix to L's list
      Label p, q = L;
      do { p = q; disp_at(q).next(q); } while (q.is_unbound());
      Displacement disp = disp_at(p);
      disp.link_to(appendix);
      disp_at_put(p, disp);
      p.unuse(); // to avoid assertion failure in ~Label
    } else {
      // L is empty, simply use appendix
      L = appendix;
    }
  }
  appendix.unuse(); // appendix should not be used anymore
}


void Assembler::bind(Label& L) {
  assert(!L.is_bound(), "label can only be bound once");
  if (EliminateJumpsToJumps) {
    // resolve unbound label
    if (_unbound_label.is_unbound()) {
      // unbound label exists => link it with L if same binding position, otherwise fix it
      if (_binding_pos == offset()) {
        // link it to L's list
        link_to(L, _unbound_label);
      } else {
        // otherwise bind unbound label
	assert(_binding_pos < offset(), "assembler error");
        bind_to(_unbound_label, _binding_pos);
      }
    }
    assert(!_unbound_label.is_unbound(), "assembler error");
    // try to eliminate jumps to next instruction
    while (L.is_unbound() && (L.pos() + int(sizeof(int)) == offset()) && (disp_at(L).type() == Displacement::absolute_jump)) {
      // previous instruction is jump jumping immediately after it => eliminate it
      const int long_size = 5;
      assert(byte_at(offset() - long_size) == 0xE9, "jmp expected");
      if (PrintJumpElimination) std->print_cr("@ %d jump to next eliminated", L.pos());
      // remove first entry from label list
      disp_at(L).next(L);
      // eliminate instruction (set code pointers back)
      _code_pos -= long_size;
      code()->set_code_end(_code_pos);
    }
    // delay fixup of L => store it as unbound label
    _unbound_label = L;
    _binding_pos = offset();
    L.unuse();
  }
  bind_to(L, offset());
}


void Assembler::merge(Label& L, Label& with) {
  Unimplemented();
}


void Assembler::call(Label& L) {
  if (L.is_bound()) {
    const int long_size = 5;
    int offs = L.pos() - offset();
    assert(offs <= 0, "assembler error");
    // 1110 1000 #32-bit disp
    emit_byte(0xE8);
    emit_long(offs - long_size);
  } else {
    // 1110 1000 #32-bit disp
    emit_byte(0xE8);
    emit_disp(L, Displacement::call, 0);
  }
}


void Assembler::call(char* entry, relocInfo::relocType rtype) {
  emit_byte(0xE8);
  emit_data((int)entry - ((int)_code_pos + sizeof(long)), rtype);
}


void Assembler::call(Register dst) {
  emit_byte(0xFF);
  emit_byte(0xD0 | dst.number());
}


void Assembler::call(Address adr) {
  emit_byte(0xFF);
  emit_operand(edx, adr);
}


void Assembler::jmp(char* entry, relocInfo::relocType rtype) {
  emit_byte(0xE9);
  emit_data((int)entry - ((int)_code_pos + sizeof(long)), rtype);
}


void Assembler::jmp(Register reg) {
  emit_byte(0xFF);
  emit_byte(0xE0 | reg.number());
}


void Assembler::jmp(Address adr) {
  emit_byte(0xFF);
  emit_operand(esp, adr);
}


void Assembler::jmp(Label& L) {
  if (L.is_bound()) {
    const int short_size = 2;
    const int long_size  = 5;
    int offs = L.pos() - offset();
    assert(offs <= 0, "assembler error");
    if (isByte(offs - short_size)) {
      // 1110 1011 #8-bit disp
      emit_byte(0xEB);
      emit_byte((offs - short_size) & 0xFF);
    } else {
      // 1110 1001 #32-bit disp
      emit_byte(0xE9);
      emit_long(offs - long_size);
    }
  } else {
    if (EliminateJumpsToJumps && _unbound_label.is_unbound() && _binding_pos == offset()) {
      // current position is target of jumps
      if (PrintJumpElimination) {
        std->print_cr("eliminated jumps/calls to %d", _binding_pos);
        std->print("from ");
        print(_unbound_label);
      }
      link_to(L, _unbound_label);
    }
    // 1110 1001 #32-bit disp
    emit_byte(0xE9);
    emit_disp(L, Displacement::absolute_jump, 0);
  }
}


void Assembler::jcc(Condition cc, Label& L) {
  assert((0 <= cc) && (cc < 16), "illegal cc");
  if (L.is_bound()) {
    const int short_size = 2;
    const int long_size  = 6;
    int offs = L.pos() - offset();
    assert(offs <= 0, "assembler error");
    if (isByte(offs - short_size)) {
      // 0111 tttn #8-bit disp
      emit_byte(0x70 | cc);
      emit_byte((offs - short_size) & 0xFF);
    } else {
      // 0000 1111 1000 tttn #32-bit disp
      emit_byte(0x0F);
      emit_byte(0x80 | cc);
      emit_long(offs - long_size);
    }
  } else {
    // 0000 1111 1000 tttn #32-bit disp
    // Note: could eliminate cond. jumps to this jump if condition
    //       is the same however, seems to be rather unlikely case.
    emit_byte(0x0F);
    emit_byte(0x80 | cc);
    emit_disp(L, Displacement::conditional_jump, cc);
  }
}


void Assembler::jcc(Condition cc, char* dst, relocInfo::relocType rtype) {
  assert((0 <= cc) && (cc < 16), "illegal cc");
  // 0000 1111 1000 tttn #32-bit disp
  emit_byte(0x0F);
  emit_byte(0x80 | cc);
  emit_data((int)dst - ((int)_code_pos + sizeof(long)), rtype);
}


void Assembler::ic_info(Label& L, int flags) {
  assert((unsigned int)flags >> IC_Info::number_of_flags == 0, "too many flags set");
  if (L.is_bound()) {
    int offs = L.pos() - offset();
    assert(offs <= 0, "assembler error");
    assert(((offs << IC_Info::number_of_flags) >> IC_Info::number_of_flags) == offs, "NLR offset out of bounds");
    emit_byte(0xA9);
    emit_long((offs << IC_Info::number_of_flags) | flags);
  } else {
    emit_byte(0xA9);
    emit_disp(L, Displacement::ic_info, flags);
  }
}


// FPU instructions

void Assembler::fld1() {
  emit_byte(0xD9);
  emit_byte(0xE8);
}


void Assembler::fldz() {
  emit_byte(0xD9);
  emit_byte(0xEE);
}


void Assembler::fld_s(Address adr) {
  emit_byte(0xD9);
  emit_operand(eax, adr);
}


void Assembler::fld_d(Address adr) {
  emit_byte(0xDD);
  emit_operand(eax, adr);
}


void Assembler::fstp_s(Address adr) {
  emit_byte(0xD9);
  emit_operand(ebx, adr);
}


void Assembler::fstp_d(Address adr) {
  emit_byte(0xDD);
  emit_operand(ebx, adr);
}


void Assembler::fild_s(Address adr) {
  emit_byte(0xDB);
  emit_operand(eax, adr);
}


void Assembler::fild_d(Address adr) {
  emit_byte(0xDF);
  emit_operand(ebp, adr);
}


void Assembler::fistp_s(Address adr) {
  emit_byte(0xDB);
  emit_operand(ebx, adr);
}


void Assembler::fistp_d(Address adr) {
  emit_byte(0xDF);
  emit_operand(edi, adr);
}


void Assembler::fabs() {
  emit_byte(0xD9);
  emit_byte(0xE1);
}


void Assembler::fchs() {
  emit_byte(0xD9);
  emit_byte(0xE0);
}

void Assembler::fadd_d(Address adr) {
  emit_byte(0xDC);
  emit_operand(eax, adr);
}

void Assembler::fsub_d(Address adr) {
  emit_byte(0xDC);
  emit_operand(esp, adr);
}

void Assembler::fmul_d(Address adr) {
  emit_byte(0xDC);
  emit_operand(ecx, adr);
}

void Assembler::fdiv_d(Address adr) {
  emit_byte(0xDC);
  emit_operand(esi, adr);
}

void Assembler::fadd(int i) {
  emit_farith(0xDC, 0xC0, i);
}


void Assembler::fsub(int i) {
  emit_farith(0xDC, 0xE8, i);
}


void Assembler::fmul(int i) {
  emit_farith(0xDC, 0xC8, i);
}


void Assembler::fdiv(int i) {
  emit_farith(0xDC, 0xF8, i);
}


void Assembler::faddp(int i) {
  emit_farith(0xDE, 0xC0, i);
}


void Assembler::fsubp(int i) {
  emit_farith(0xDE, 0xE8, i);
}


void Assembler::fsubrp(int i) {
  emit_farith(0xDE, 0xE0, i);
}


void Assembler::fmulp(int i) {
  emit_farith(0xDE, 0xC8, i);
}


void Assembler::fdivp(int i) {
  emit_farith(0xDE, 0xF8, i);
}



void Assembler::fprem() {
  emit_byte(0xD9);
  emit_byte(0xF8);
}


void Assembler::fprem1() {
  emit_byte(0xD9);
  emit_byte(0xF5);
}


void Assembler::fxch(int i) {
  emit_farith(0xD9, 0xC8, i);
}


void Assembler::fincstp() {
  emit_byte(0xD9);
  emit_byte(0xF7);
}


void Assembler::ffree(int i) {
  emit_farith(0xDD, 0xC0, i);
}


void Assembler::ftst() {
  emit_byte(0xD9);
  emit_byte(0xE4);
}


void Assembler::fcompp() {
  emit_byte(0xDE);
  emit_byte(0xD9);
}


void Assembler::fnstsw_ax() {
  emit_byte(0xdF);
  emit_byte(0xE0);
}


void Assembler::fwait() {
  emit_byte(0x9B);
}


// Implementation of MacroAssembler

void MacroAssembler::align(int modulus) {
  while (offset() % modulus != 0) nop();
}


void MacroAssembler::test(Register dst, int imm8) {
  if (!CodeForP6 && dst.hasByteRegister()) {
    testb(dst, imm8);
  } else {
    testl(dst, imm8);
  }
}


void MacroAssembler::enter() {
  pushl(ebp);
  movl(ebp, esp);
}


void MacroAssembler::leave() {
  movl(esp, ebp);
  popl(ebp);
}


// Support for inlined data

void MacroAssembler::inline_oop(oop o) {
  emit_byte(0xA9);
  emit_data((int)o, relocInfo::oop_type);
}


// Calls to C land
//
// When entering C land, the ebp & esp of the last Delta frame have to be recorded.
// When leaving C land, last_Delta_fp has to be reset to 0. This is required to
// allow proper stack traversal.

void MacroAssembler::set_last_Delta_frame_before_call() {
  movl(Address((int)&last_Delta_fp, relocInfo::external_word_type), ebp);
  movl(Address((int)&last_Delta_sp, relocInfo::external_word_type), esp);
}


void MacroAssembler::set_last_Delta_frame_after_call() {
  addl(esp, oopSize);	// sets esp to value before call (i.e., before pushing the return address)
  set_last_Delta_frame_before_call();
  subl(esp, oopSize);	// resets esp to original value
}


void MacroAssembler::reset_last_Delta_frame() {
  movl(Address((int)&last_Delta_fp, relocInfo::external_word_type), 0);
}


void MacroAssembler::call_C(Label& L) {
  set_last_Delta_frame_before_call();
  call(L);
  reset_last_Delta_frame();
}


void MacroAssembler::call_C(Label& L, Label& nlrTestPoint) {
  set_last_Delta_frame_before_call();
  call(L);
  Assembler::ic_info(nlrTestPoint, 0);
  reset_last_Delta_frame();
}


void MacroAssembler::call_C(char* entry, relocInfo::relocType rtype) {
  set_last_Delta_frame_before_call();
  call(entry, rtype);
  reset_last_Delta_frame();
}


void MacroAssembler::call_C(char* entry, relocInfo::relocType rtype, Label& nlrTestPoint) {
  set_last_Delta_frame_before_call();
  call(entry, rtype);
  Assembler::ic_info(nlrTestPoint, 0);
  reset_last_Delta_frame();
}


void MacroAssembler::call_C(Register entry) {
  set_last_Delta_frame_before_call();
  call(entry);
  reset_last_Delta_frame();
}


void MacroAssembler::call_C(Register entry, Label& nlrTestPoint) {
  set_last_Delta_frame_before_call();
  call(entry);
  Assembler::ic_info(nlrTestPoint, 0);
  reset_last_Delta_frame();
}


// The following 3 macros implement C run-time calls with arguments. When setting up the
// last Delta frame, the value pushed after last_Delta_sp MUST be a valid return address,
// therefore an additional call to a little stub is required which does the parameter
// passing.
//
// [return addr] \
// [argument 1 ]  |   extra stub in C land
//  ...           |
// [argument n ] /
// [return addr] <=== must be valid return address  \
// [...        ] <--- last_Delta_sp                  |
//  ...                                              | last Delta frame in Delta land
// [...        ]                                     |
// [previous fp] <--- last_Delta_fp                 /
//
// Note: The routines could be implemented slightly more efficient and shorter by
// explicitly pushing/popping a valid return address instead of calling the extra
// stub. However, currently the assembler doesn't support label pushes.


void MacroAssembler::call_C(char* entry, Register arg1) {
  Label L1, L2;
  jmp(L1);

  bind(L2);
  pushl(arg1);
  call(entry, relocInfo::runtime_call_type);
  addl(esp, 1*oopSize);
  ret(0);

  bind(L1);
  call_C(L2);
}


void MacroAssembler::call_C(char* entry, Register arg1, Register arg2) {
  Label L1, L2;
  jmp(L1);

  bind(L2);
  pushl(arg2);
  pushl(arg1);
  call(entry, relocInfo::runtime_call_type);
  addl(esp, 2*oopSize);
  ret(0);

  bind(L1);
  call_C(L2);
}


void MacroAssembler::call_C(char* entry, Register arg1, Register arg2, Register arg3) {
  Label L1, L2;
  jmp(L1);

  bind(L2);
  pushl(arg3);
  pushl(arg2);
  pushl(arg1);
  call(entry, relocInfo::runtime_call_type);
  addl(esp, 3*oopSize);
  ret(0);

  bind(L1);
  call_C(L2);
}


void MacroAssembler::call_C(char* entry, Register arg1, Register arg2, Register arg3, Register arg4) {
  Label L1, L2;
  jmp(L1);

  bind(L2);
  pushl(arg4);
  pushl(arg3);
  pushl(arg2);
  pushl(arg1);
  call(entry, relocInfo::runtime_call_type);
  addl(esp, 4*oopSize);
  ret(0);

  bind(L1);
  call_C(L2);
}


void MacroAssembler::store_check(Register obj, Register tmp) {
  // Does a store check for the oop in register obj. The content of
  // register obj is destroyed afterwards.
  // Note: Could be optimized by hardwiring the byte map base address
  // in the code - however relocation would be necessary whenever the
  // base changes. Advantage: only one instead of two instructions.
  assert(obj != tmp, "registers must be different");
  movl(tmp, Address((int)&byte_map_base, relocInfo::external_word_type));
  shrl(obj, card_shift);
  movb(Address(tmp, obj, Address::times_1), 0);
}


void MacroAssembler::fpu_mask_and_cond_for(Condition cc, int& mask, Condition& cond) {
  switch (cc) {
    case equal		: mask = 0x4000; cond = notZero;	break;
    case notEqual	: mask = 0x4000; cond = zero;		break;
    case less		: mask = 0x0100; cond = notZero;	break;
    case lessEqual	: mask = 0x4500; cond = notZero;	break;
    case greater	: mask = 0x4500; cond = zero;		break;
    case greaterEqual	: mask = 0x0100; cond = zero;		break;
    default		: Unimplemented();
  };
}


void MacroAssembler::fpop() {
  ffree();
  fincstp();
}


// debugging

void MacroAssembler::print_reg(char* name, oop obj) {
  std->print("%s = ", name);
  if (obj == NULL) {
    std->print_cr("NULL");
  } else if (obj->is_smi()) {
    std->print_cr("smi (%d)", smiOop(obj)->value());
  } else if (obj->is_mem() && Universe::is_heap((oop*)obj)) {
    // use explicit checks to avoid crashes even in a broken system
    if (obj == Universe::nilObj()) {
      std->print_cr("nil (0x%08x)", obj);
    } else if (obj == Universe::trueObj()) {
      std->print_cr("true (0x%08x)", obj);
    } else if (obj == Universe::falseObj()) {
      std->print_cr("false (0x%08x)", obj);
    } else {
      std->print_cr("memOop (0x%08x)", obj);
    }
  } else {
    std->print_cr("0x%08x", obj);
  }
}


void MacroAssembler::inspector(oop edi, oop esi, oop ebp, oop esp, oop ebx, oop edx, oop ecx, oop eax, char* eip) {
  char* title = (char*)(nativeTest_at(eip)->data());
  if (title != NULL) std->print_cr("%s", title);
  print_reg("eax", eax);
  print_reg("ebx", ebx);
  print_reg("ecx", ecx);
  print_reg("edx", edx);
  print_reg("edi", edi);
  print_reg("esi", esi);
  std->print_cr("ebp = 0x%08x", ebp);
  std->print_cr("esp = 0x%08x", esp);
  std->cr();
}


void MacroAssembler::inspect(char* title) {
  char* entry = StubRoutines::call_inspector_entry();
  if (entry != NULL) {
    call(entry, relocInfo::runtime_call_type);			// call stub invoking the inspector
    testl(eax, int(title));					// additional info for inspector
  } else {
    const char* s = (title == NULL) ? "" : title;
    std->print_cr("cannot call inspector for \"%s\" - no entry point yet", s);
  }
}
