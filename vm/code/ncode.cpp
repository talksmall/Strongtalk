/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.14 $ */
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

# include "incls/_ncode.cpp.incl"

void OopNCode::remember() {
Unimplemented();//  if (rememberLink.isEmpty()) Memory->code->rememberLink.add(&rememberLink);
}


void OopNCode::relocate() {
  Unimplemented();
}

bool OopNCode::switch_pointers(oop from, oop to,
			       GrowableArray<nmethod*>* nmethods_to_invalidate) {
  Unused(nmethods_to_invalidate);
  bool needToInvalICache = false;
  Unimplemented();
  return needToInvalICache;
}

void NCodeBase::verify2(const char* name) {
  if ((int)this & (oopSize - 1))
    error("alignment error in %s at %#lx", name, this);
  if (instsLen() > 256 * K)
    error("instr length of %s at %#lx seems too big (%ld)", name, this, instsLen());
}
  
void OopNCode::verify() {
  const char* name = isNMethod() ? "nmethod" : (isPIC() ? " PIC" : "count stub");
  NCodeBase::verify2(name);
  // %fix: Verify via relocIterator
}

NCodeBase* findThing(void* addr) {
  if (Universe::code->contains(addr)) {
    nmethod* n = (nmethod*)nmethod_from_insts((char*)addr);
    return n;
  } else {
    return NULL;
  }
}  

#endif
