/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.25 $ */
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

#ifdef DELTA_COMPILER

# include "incls/_relocInfo.cpp.incl"

relocInfo::relocInfo(int t, int off) {
  assert(0 <= t && t < (1 << reloc_type_width), "wrong type");
  assert(off <= nthMask(reloc_offset_width), "offset out off bounds");
  _value =  (t << reloc_offset_width) | off;
}


int relocInfo::print(nmethod* m, int last_offset) {
  if (!isValid()) return 0;
  int current_offset = offset() + last_offset;
  int* addr = (int*) (m->insts() + current_offset);
  printIndent();
  if (isOop()) {
    std->print("embedded oop   @0x%lx = ", addr);
    oop(*addr)->print_value();
  } else {
    assert(isCall(), "must be a call");
    char* target = (char*) (*addr + (int) addr + oopSize);
    if (isIC()) {
      std->print("inline cache   @0x%lx", addr);
    } else if (isPrimitive()) {
      std->print("primitive call @0x%lx = ", addr);
      primitive_desc* pd = primitives::lookup((fntype) target);
      if (pd != NULL) {
        std->print("(%s)", pd->name());
      } else {
        std->print("runtime routine");
      }
    } else if (isDLL()) {
      std->print("DLL call @0x%lx = ", addr);
    } else if (isUncommonTrap()) {
      std->print("uncommon trap @0x%lx", addr);
    } else {
      std->print("run-time call @0x%lx", addr);
    }
  }
  return current_offset;
}

relocIterator::relocIterator(const nmethod* nm) {
  current = nm->locs()-1;
  end     = nm->locsEnd();
  addr    = nm->insts();
}

bool relocIterator::wasUncommonTrapExecuted() const {
  assert(current->isUncommonTrap(), "not an uncommon branch");
  return callDestination() == (char*)StubRoutines::used_uncommon_trap_entry();
}

bool relocIterator::is_position_dependent() const {
  switch (type()) {
    case relocInfo::ic_type:
    case relocInfo::prim_type:
    case relocInfo::uncommon_type:
    case relocInfo::runtime_call_type:
    case relocInfo::internal_word_type: 
    case relocInfo::dll_type: 
      return true;
  }
  return false;
}


#endif

