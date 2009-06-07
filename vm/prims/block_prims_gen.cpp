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
# include "incls/_block_prims_gen.cpp.incl"

char* PrimitivesGenerator::allocateBlock(int n) {

  klassOopDesc** block_klass;
  
  switch (n) {
    case 0	: block_klass = &::zeroArgumentBlockKlassObj;  break;
    case 1	: block_klass = &::oneArgumentBlockKlassObj;   break;
    case 2	: block_klass = &::twoArgumentBlockKlassObj;   break;
    case 3	: block_klass = &::threeArgumentBlockKlassObj; break;
    case 4	: block_klass = &::fourArgumentBlockKlassObj;  break;
    case 5	: block_klass = &::fiveArgumentBlockKlassObj;  break;
    case 6	: block_klass = &::sixArgumentBlockKlassObj;   break;
    case 7	: block_klass = &::sevenArgumentBlockKlassObj; break;
    case 8	: block_klass = &::eightArgumentBlockKlassObj; break;
    case 9	: block_klass = &::nineArgumentBlockKlassObj;  break;
  }

  Address block_klass_addr =  Address((int)block_klass, relocInfo::external_word_type);
  Label need_scavenge, fill_object;

  char* entry_point = masm->pc();
  
  test_for_scavenge(eax, 4 * oopSize, need_scavenge);

 masm->bind(fill_object);
  masm->movl(ebx, block_klass_addr);
  masm->movl(Address(eax, -4*oopSize), 0x80000003);		// obj->init_mark()
  masm->movl(Address(eax, -3*oopSize), ebx);			// obj->set_klass(klass)
//  masm->movl(Address(eax, -2*oopSize), 0);			// obj->set_method(NULL)
//  masm->movl(Address(eax, -1*oopSize), 0);			// obj->set_lexical_scope(NULL)
  masm->subl(eax, (4 * oopSize) - 1);
  masm->ret(0);

 masm->bind(need_scavenge);
  scavenge(4);
  masm->jmp(fill_object);

  return entry_point;
}

extern "C" void scavenge_and_allocate(int size);

char* PrimitivesGenerator::allocateContext_var() {
  
  Label need_scavenge, fill_object;
  Label _loop, _loop_end;
    
  char* entry_point = masm->pc();
  
  masm->movl(ecx, Address(esp, +oopSize));	// load length  (remember this is a smiOop)
  masm->movl(eax, Address((int)&eden_top, relocInfo::external_word_type));
  masm->movl(edx, ecx);
  masm->addl(edx, 3*oopSize);
  masm->addl(edx, eax);
// Equals? ==>  masm->leal(edx, Address(ecx, eax, Address::times_1, 3*oopSize));
  masm->cmpl(edx, Address((int)&eden_end, relocInfo::external_word_type));
  masm->jcc(Assembler::greater, need_scavenge);
  masm->movl(Address((int)&eden_top, relocInfo::external_word_type), edx);

 masm->bind(fill_object);
  masm->movl(ebx, contextKlass_addr());
  masm->addl(ecx, 4);
  masm->addl(ecx, 0x80000003);				// obj->init_mark()
  masm->movl(Address(eax), ecx);
  masm->movl(ecx, nil_addr());

  masm->movl(Address(eax, 1*oopSize), ebx);		// obj->set_klass(klass)
  masm->movl(Address(eax, 2*oopSize), 0);		// obj->set_home(NULL)
  masm->leal(ebx, Address(eax, +3*oopSize));
  masm->jmp(_loop_end);

 masm->bind(_loop);
  masm->movl(Address(ebx), ecx);
  masm->addl(ebx, oopSize);

 masm->bind(_loop_end);
  masm->cmpl(ebx, edx);
  masm->jcc(Assembler::below, _loop);

  masm->incl(eax);
  masm->ret(0);
  
 masm->bind(need_scavenge);
  masm->set_last_Delta_frame_after_call();
  masm->shrl(ecx, Tag_Size); 			// smiOop->value()
  masm->addl(ecx, 3);
  masm->pushl(ecx);
  masm->call((char*)&scavenge_and_allocate, relocInfo::runtime_call_type);
  masm->addl(esp, 4);
  masm->reset_last_Delta_frame();
  masm->movl(ecx, Address(esp, +oopSize));	// reload length  (remember this is a smiOop)
  masm->movl(edx, ecx);
  masm->addl(edx, 3 * oopSize);
  masm->addl(edx, eax);
  masm->jmp(fill_object);
  
  return entry_point;
}

char* PrimitivesGenerator::allocateContext(int n) {
  
  Label need_scavenge, fill_object;
  int size = n + 3;

  char* entry_point = masm->pc();

  test_for_scavenge(eax, size * oopSize, need_scavenge);

 masm->bind(fill_object);
  masm->movl(ebx, contextKlass_addr());
  masm->movl(ecx, nil_addr());
  masm->movl(Address(eax, (-size+0)*oopSize), 0x80000003 + ((n+1) * 4));// obj->init_mark()
  masm->movl(Address(eax, (-size+1)*oopSize), ebx);			// obj->set_klass(klass)
  masm->movl(Address(eax, (-size+2)*oopSize), 0);			// obj->set_home(NULL)
  for (int i = 0; i < n; i++) {
    masm->movl(Address(eax, (-size+3+i)*oopSize), ecx);			// obj->obj_at_put(i,nilObj)
  }
  masm->subl(eax, size * oopSize - 1);
  masm->ret(0);

 masm->bind(need_scavenge);
  scavenge(size);
  masm->jmp(fill_object);

  return entry_point;
}
