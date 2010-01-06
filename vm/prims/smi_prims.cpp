/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.15 $ */
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


# include "incls/_precompiled.incl"
# include "incls/_smi_prims.cpp.incl"

TRACE_FUNC(TraceSmiPrims, "smi")

int smiOopPrimitives::number_of_calls;

# define ASSERT_RECEIVER assert(receiver->is_smi(), "receiver must be smi")

# define SMI_RELATIONAL_OP(op)                                     \
  if (!argument->is_smi())                                         \
    return markSymbol(vmSymbols::first_argument_has_wrong_type()); \
  int a = (int) receiver;                                          \
  int b = (int) argument;                                          \
  return a op b ? trueObj : falseObj

PRIM_DECL_2(smiOopPrimitives::lessThan, oop receiver, oop argument) {
  PROLOGUE_2("lessThan", receiver, argument);
  ASSERT_RECEIVER;
  SMI_RELATIONAL_OP(<);
}

PRIM_DECL_2(smiOopPrimitives::greaterThan, oop receiver, oop argument){
  PROLOGUE_2("greaterThan", receiver, argument);
  ASSERT_RECEIVER;
  SMI_RELATIONAL_OP(>);
}

PRIM_DECL_2(smiOopPrimitives::lessThanOrEqual, oop receiver, oop argument) {
  PROLOGUE_2("lessThanOrEqual", receiver, argument);
  ASSERT_RECEIVER;
  SMI_RELATIONAL_OP(<=);
}

PRIM_DECL_2(smiOopPrimitives::greaterThanOrEqual, oop receiver, oop argument) {
  PROLOGUE_2("greaterThanOrEqual", receiver, argument);
  ASSERT_RECEIVER;
  SMI_RELATIONAL_OP(>=);
}

PRIM_DECL_2(smiOopPrimitives::equal, oop receiver, oop argument) {
  PROLOGUE_2("equal", receiver, argument);
  ASSERT_RECEIVER;
  SMI_RELATIONAL_OP(==);
}

PRIM_DECL_2(smiOopPrimitives::notEqual, oop receiver, oop argument) {
  PROLOGUE_2("notEqual", receiver, argument);
  ASSERT_RECEIVER;
  SMI_RELATIONAL_OP(!=);
}

PRIM_DECL_2(smiOopPrimitives::bitAnd, oop receiver, oop argument) {
  PROLOGUE_2("bitAnd", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return smiOop(int(receiver) & int(argument));  
}

PRIM_DECL_2(smiOopPrimitives::bitOr, oop receiver, oop argument) {
  PROLOGUE_2("bitOr", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return smiOop(int(receiver) | int(argument));  
}

PRIM_DECL_2(smiOopPrimitives::bitXor, oop receiver, oop argument) {
  PROLOGUE_2("bitXor", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return smiOop(int(receiver) ^ int(argument));  
}

PRIM_DECL_2(smiOopPrimitives::bitShift, oop receiver, oop argument) {
  PROLOGUE_2("bitShift", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  assert(Int_Tag == 0, "check this code");
  const int bitsPerWord = oopSize*8;
  const int maxShiftCnt = bitsPerWord - Tag_Size - 1;
  int n = smiOop(argument)->value();
  if (n > 0) {
    // arithmetic shift left
    if (n < maxShiftCnt) {
      // 0 < n < maxShiftCnt < bitsPerWord	// |<- n ->|<- 1 ->|<- 32-(n+1) ->|
      int mask1 =  1 << (bitsPerWord - (n+1));	// |00...00|   1   |00..........00|
      int mask2 = -1 << (bitsPerWord - n);	// |11...11|   0   |00..........00|
      if (((int(receiver) + mask1) & mask2) == 0) {
        // i.e., the bit at position (32-(n+1)) is the same as the upper n bits, thus
	// after shifting out the upper n bits the sign hasn't changed -> no overflow
        return smiOop(int(receiver) << n);
      }
    }
    return vmSymbols::smi_overflow();
  } else {
    // arithmetic shift right
    if (n < -maxShiftCnt) n = -maxShiftCnt;
    return smiOop((int(receiver) >> -n) & (-1 << Tag_Size));
  }
}

PRIM_DECL_2(smiOopPrimitives::rawBitShift, oop receiver, oop argument) {
  PROLOGUE_2("rawBitShift", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  assert(Int_Tag == 0, "check this code");
  const int bitsPerWord = oopSize*8;
  int n = smiOop(argument)->value();
  if (n >= 0) {
    // logical shift right
    return smiOop((unsigned int)receiver << (n % bitsPerWord));
  } else {
    // logical shift left
    return smiOop(((unsigned int)receiver >> ((-n) % bitsPerWord)) & (-1 << Tag_Size));
  }
}

PRIM_DECL_1(smiOopPrimitives::asObject, oop receiver) {
  PROLOGUE_1("asObject", receiver);
  ASSERT_RECEIVER;
  int id = smiOop(receiver)->value();
  if (objectIDTable::is_index_ok(id))
     return objectIDTable::at(id);
  return markSymbol(vmSymbols::index_not_valid());
}

PRIM_DECL_1(smiOopPrimitives::printCharacter, oop receiver) {
  PROLOGUE_1("printCharacter", receiver);
  ASSERT_RECEIVER;
  lprintf("%c", smiOop(receiver)->value());
  return receiver;
}

static void trap() { assert(false, "This primitive should be patched"); };

extern "C" oop PRIM_API smiOopPrimitives_add(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API smiOopPrimitives_subtract(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API smiOopPrimitives_multiply(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API smiOopPrimitives_mod(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API smiOopPrimitives_div(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API smiOopPrimitives_quo(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
extern "C" oop PRIM_API smiOopPrimitives_remainder(oop receiver, oop argument) 	{ trap(); return markSymbol(vmSymbols::primitive_trap()); };
