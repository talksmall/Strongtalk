/* Copyright (c) 2007, Strongtalk Group
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
	  disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the Strongtalk Group nor the names of its contributors may be used to endorse or promote products derived 
	  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT 
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE


*/

# include "incls/_precompiled.incl"
# include "incls/_smi_prims_gen.cpp.incl"

char* PrimitivesGenerator::smiOopPrimitives_add() {
  Address argument = Address(esp, 4);
  Address receiver = Address(esp, 8);
  Label _overflow;

  char* entry_point = masm->pc();

  masm->movl(eax, receiver);
  masm->addl(eax, argument);
  masm->jcc(Assembler::overflow, _overflow);
  masm->testb(eax, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);
  masm->ret(8);

 masm->bind(_overflow);
  masm->movl(eax, argument);
  masm->testb(eax, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);
  masm->jmp(error_overflow);

  return entry_point;
}

char* PrimitivesGenerator::smiOopPrimitives_subtract() {
  Address argument = Address(esp, 4);
  Address receiver = Address(esp, 8);
  Label _overflow;

  char* entry_point = masm->pc();

  masm->movl(eax, receiver);
  masm->subl(eax, argument);
  masm->jcc(Assembler::overflow, _overflow);
  masm->testb(eax, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);
  masm->ret(8);
  
 masm->bind(_overflow);
  masm->movl(eax, argument);
  masm->testb(eax, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);
  masm->jmp(error_overflow);
  

  return entry_point;
}

char* PrimitivesGenerator::smiOopPrimitives_multiply() {
  Address argument = Address(esp, 4);
  Address receiver = Address(esp, 8);
  
  char* entry_point = masm->pc();

 // masm->int3();
  masm->movl(edx, argument);
  masm->movl(eax, receiver);
  masm->testb(edx, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);
  masm->sarl(edx, 2);
  masm->imull(edx);
  masm->jcc(Assembler::overflow, error_overflow);
  masm->ret(8);

  
  return entry_point;
}

char* PrimitivesGenerator::smiOopPrimitives_mod() {
  Address argument = Address(esp, 4);
  Address receiver = Address(esp, 8);
  Label _equal, _positive;

  char* entry_point = masm->pc();

  // PUBLIC _smiOopPrimitives_mod@8
  // 
  // ; Intel definition of mod delivers:
  // ;   0 <= |x%y| < |y|
  // ;
  // ; Standard definition requires:
  // ;   y>0: 
  // ;     0 <= x mod y < y
  // ;   y<0:
  // ;     y <  x mod y <= 0
  // ;
  // ; Conversion:
  // ;
  // ;   sgn(y)=sgn(x%y):
  // ;     x mod y = x%y
  // ;
  // ;   sgn(y)#sgn(x%y):
  // ;     x mod y = x%y + y
  // ;

//  masm->int3();
  masm->movl(eax, receiver);
  masm->movl(ecx, argument);
  masm->testl(ecx, ecx);
  masm->jcc(Assembler::equal, error_division_by_zero);
  
  masm->testb(ecx, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);

  masm->sarl(ecx, 2);
  masm->sarl(eax, 2);
  masm->cdq();
  masm->idivl(ecx);
  masm->jcc(Assembler::overflow, error_overflow);

  masm->movl(eax, edx);
  masm->testl(eax, eax);
  masm->jcc(Assembler::equal, _equal);
  
  masm->xorl(edx, ecx);
  masm->jcc(Assembler::negative, _positive);
  
  masm->bind(_equal);
  masm->shll(eax, 2);
  masm->ret(8);
  
  masm->bind(_positive);
  masm->addl(eax, ecx);
  masm->shll(eax, 2);
  masm->ret(8);

  return entry_point;
}

char* PrimitivesGenerator::smiOopPrimitives_div() {
  Address argument = Address(esp, 4);
  Address receiver = Address(esp, 8);
  Label _equal, _positive;

  char* entry_point = masm->pc();

  // ; Intel definition of div delivers:
  // ;   x = (x/y)*y + (x%y)
  // ;
  // ; Standard definition requires:
  // ;   x = (x div y)*y + (x mod y)
  // ;
  // ; Conversion:
  // ;
  // ;   sgn(y)=sgn(x%y):
  // ;     x div y = x/y
  // ;
  // ;   sgn(y)#sgn(x%y):
  // ;     x div y = x/y-1
  // ;
  // 
  
  masm->movl(ecx, argument);
  masm->movl(eax, receiver);
  masm->testl(ecx, ecx);
  masm->jcc(Assembler::equal, error_division_by_zero);

  masm->testb(ecx, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);

  masm->sarl(ecx, 2);
  masm->sarl(eax, 2);
  masm->cdq();
  masm->idivl(ecx);

  masm->jcc(Assembler::overflow, error_overflow);

  masm->testl(edx, edx);
  masm->jcc(Assembler::equal, _equal);

  masm->xorl(ecx, edx);
  masm->jcc(Assembler::negative, _positive);

 masm->bind(_equal);
  masm->shll(eax, 2);
  masm->ret(8);

 masm->bind(_positive);
  masm->decl(eax);
  masm->shll(eax, 2);
  masm->ret(8);

  return entry_point;
}

char* PrimitivesGenerator::smiOopPrimitives_quo() {
  Address argument = Address(esp, 4);
  Address receiver = Address(esp, 8);

  char* entry_point = masm->pc();

  masm->movl(ecx, argument);
  masm->movl(eax, receiver);

  masm->testb(eax, 0x03);
  masm->jcc(Assembler::notEqual, error_receiver_has_wrong_type);

  masm->testl(ecx, ecx);
  masm->jcc(Assembler::equal, error_division_by_zero);

  masm->testb(ecx, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);

  masm->sarl(ecx, 2);
  masm->sarl(eax, 2);
  masm->cdq();
  masm->idivl(ecx);

  masm->jcc(Assembler::overflow, error_overflow);
  masm->shll(eax, 2);
  masm->ret(8);

  return entry_point;
}

char* PrimitivesGenerator::smiOopPrimitives_remainder() {
  Address argument = Address(esp, 4);
  Address receiver = Address(esp, 8);

  char* entry_point = masm->pc();
  
  masm->movl(ecx, argument);
  masm->movl(eax, receiver);
  masm->testl(ecx, ecx);
  masm->jcc(Assembler::equal, error_division_by_zero);
  masm->testb(ecx, 0x03);
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);
  masm->sarl(ecx, 2);
  masm->sarl(eax, 2);
  masm->cdq();
  masm->idivl(ecx);
  masm->jcc(Assembler::overflow, error_overflow);
  masm->movl(eax, edx);
  masm->sarl(eax, 2);
  masm->ret(8);


  return entry_point;
}
