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

char* PrimitivesGenerator::double_op(arith_op op) {
  Label need_scavenge, fill_object;
  
  char* entry_point = masm->pc();

  // 	Tag test for argument
  masm->movl(ebx, Address(esp, +oopSize));
  masm->movl(edx, doubleKlass_addr());
  masm->testb(ebx, 0x01);
  masm->jcc(Assembler::zero, error_first_argument_has_wrong_type);
  
  // 	klass test for argument
  masm->cmpl(edx, Address(ebx, + 3));
  masm->jcc(Assembler::notEqual, error_first_argument_has_wrong_type);

  // 	allocate_double
  test_for_scavenge(eax, 4 * oopSize, need_scavenge);
  
  // 	mov	DWORD PTR [eax-(4 * oopSize)], 0A0000003H ; obj->init_mark()
  // 	fld  QWORD PTR [ecx+07]
  // 	mov	DWORD PTR [eax-(3 * oopSize)], edx        ; obj->set_klass(klass)
  // 
  // 	op   QWORD PTR [ebx+07]

  masm->bind(fill_object);
  masm->movl(ecx, Address(esp, +2*oopSize));

  masm->movl(edx, doubleKlass_addr());
  masm->movl(Address(eax, -4*oopSize), 0xA0000003);		// obj->init_mark()
  masm->movl(Address(eax, -3*oopSize), edx);			// obj->set_klass(klass)
  
  masm->fld_d(Address(ecx,  (2 * oopSize) - 1));
  
  switch(op) {
    case op_add: masm->fadd_d(Address(ebx,  (2 * oopSize) - 1)); break;
    case op_sub: masm->fsub_d(Address(ebx,  (2 * oopSize) - 1)); break;
    case op_mul: masm->fmul_d(Address(ebx,  (2 * oopSize) - 1)); break;
    case op_div: masm->fdiv_d(Address(ebx,  (2 * oopSize) - 1));
#if 0
    masm->movl(edx, eax);                 // save result oop in temporary register
    masm->fnstsw_ax();                    // store x87 FPU status word in eax
    masm->testb(eax, 0x4);                // check Z-flag for Division-By-Zero
    masm->movl(eax, edx);                 // restore result oop in eax (no effect on EFLAGS)
    masm->movl(edx, doubleKlass_addr());  // restore klass oop (no effect on EFLAGS)
    masm->jcc(Assembler::notZero, error_division_by_zero); // if Division-By-Zero, jump to error Label
#endif
		break;
  }

  masm->subl(eax, (4 * oopSize) - 1);
  
  // 	eax result   doubleOop
  // 	ecx receiver doubleOop
  // 	ebx argument doubleOop
  masm->fstp_d(Address(eax, (2 * oopSize) - 1));
  masm->ret(8);
  
  masm->bind(need_scavenge);
  scavenge(4);
  masm->movl(ebx, Address(esp, +oopSize));
  masm->movl(edx, doubleKlass_addr());
  masm->jmp(fill_object);
  

  return entry_point;
}

char* PrimitivesGenerator::double_from_smi() {
  Label need_scavenge, fill_object;

  char* entry_point = masm->pc();
  
  test_for_scavenge(eax, 4 * oopSize, need_scavenge);
  
 masm->bind(fill_object);
  masm->movl(ecx, Address(esp, +oopSize));
  masm->movl(edx, doubleKlass_addr());
  masm->sarl(ecx, 2);
  masm->movl(Address(eax, -4*oopSize), 0xA0000003);		// obj->init_mark()
  masm->movl(Address(esp, -oopSize), ecx);
  masm->movl(Address(eax, -3*oopSize), edx);			// obj->set_klass(klass)
  masm->fild_s(Address(esp, -oopSize));
  masm->subl(eax, (4 * oopSize) - 1);

  //	eax result   doubleOop
  masm->fstp_d(Address(eax, (2 * oopSize) - 1));
  masm->ret(4);
    
 masm->bind(need_scavenge);
  scavenge(4);
  masm->jmp(fill_object);


  return entry_point;
}

