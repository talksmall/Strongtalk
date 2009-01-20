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
# include "incls/_behavior_prims_gen.cpp.incl"
static bool stop = false;
char* PrimitivesGenerator::primitiveNew(int n) {
  Address klass_addr =  Address(esp, +2 * oopSize);
  Label need_scavenge, fill_object;
  int size = n+2;

  // %note: it looks like the compiler assumes we spill only eax/ebx here -Marc 04/07

  char* entry_point = masm->pc();
  
  test_for_scavenge(eax, size * oopSize, allocation_failure);
  Address _stop = Address((int)&stop, relocInfo::external_word_type);
  Label _break, no_break;
 masm->bind(fill_object);
  masm->movl(ebx, _stop);
  masm->testl(ebx, ebx);
  masm->jcc(Assembler::notEqual, _break);
 masm->bind(no_break);
  masm->movl(ebx, klass_addr);
  masm->movl(Address(eax, (-size+0)*oopSize), 0x80000003);	// obj->init_mark()
  masm->movl(Address(eax, (-size+1)*oopSize), ebx);		// obj->init_mark()

  if (n>0) {
    masm->movl(ebx, nil_addr());
    for (int i = 0; i < n; i++) {
      masm->movl(Address(eax, (-size+2+i)*oopSize), ebx);	// obj->obj_at_put(i,nilObj)
    }
  }

  masm->subl(eax, (size * oopSize) - 1);
  masm->ret(2 * oopSize);
  
 masm->bind(_break);
  masm->int3();
  masm->jmp(no_break);

 masm->bind(need_scavenge);
  scavenge(size);
  masm->jmp(fill_object);
  
  return entry_point;
}

char* PrimitivesGenerator::inline_allocation() {
  Address klass_addr =  Address(esp, +2 * oopSize);
  Address count_addr =  Address(esp, +1 * oopSize);

  Label need_scavenge1, fill_object1, need_scavenge2, fill_object2, loop, loop_test, exit;
  int size = 2;

  char* entry_point = masm->pc();

  masm->movl(ebx, klass_addr);
  masm->movl(edx, count_addr);
  masm->testl(edx, 1);
  masm->jcc(Assembler::notEqual, exit);
  masm->sarl(edx, 3);
 masm->bind(loop);

  test_for_scavenge(eax, size * oopSize, need_scavenge1);
 masm->bind(fill_object1);
  masm->movl(Address(eax, (-size+0)*oopSize), 0x80000003);	// obj->init_mark()
  masm->movl(Address(eax, (-size+1)*oopSize), ebx);		// obj->init_mark()

  masm->subl(eax, (size * oopSize) - 1);

  test_for_scavenge(ecx, size * oopSize, need_scavenge2);
 masm->bind(fill_object2);
  masm->movl(Address(ecx, (-size+0)*oopSize), 0x80000003);	// obj->init_mark()
  masm->movl(Address(ecx, (-size+1)*oopSize), ebx);		// obj->init_mark()

  masm->subl(ecx, (size * oopSize) - 1);
  //masm->jmp(loop);
 masm->bind(loop_test);
  masm->decl(edx);
  masm->jcc(Assembler::notEqual, loop);
 masm->bind(exit);
  masm->ret(2 * oopSize);
  
 masm->bind(need_scavenge1);
  masm->pushl(ebx);
  masm->pushl(edx);
  scavenge(size);
  masm->popl(edx);
  masm->popl(ebx);
  masm->jmp(fill_object1);
  
 masm->bind(need_scavenge2);
  masm->pushl(ebx);
  masm->pushl(edx);
  scavenge(size);
  masm->movl(ecx, eax);
  masm->popl(edx);
  masm->popl(ebx);
  masm->jmp(fill_object2);
  
  return entry_point;
}
