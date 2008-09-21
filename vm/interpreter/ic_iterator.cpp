/* Copyright 1994, 1995, LongView Technologies, L.L.C. $Revision: 1.8 $ */
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
# include "incls/_ic_iterator.cpp.incl"

// Implementation of IC

IC::IC(CompiledIC* ic)    : _iter(new CompiledIC_Iterator(ic)) {}
IC::IC(InterpretedIC* ic) : _iter(new InterpretedIC_Iterator(ic)) {}

GrowableArray<klassOop>* IC::receiver_klasses() const {
  GrowableArray<klassOop>* result = new GrowableArray<klassOop>();
  IC_Iterator* it = iterator();
  it->init_iteration();
  while (!it->at_end()) {
    result->append(it->klass());
    it->advance();
  }
  return result;
}


void IC::replace(nmethod* nm) {
  Unimplemented();
  IC_Iterator* it = iterator();
  it->init_iteration();
  while (!it->at_end()) {
    // replace if found
    it->advance();
  }
}


void IC::print() {
  char* s;
  switch (shape()) {
    case anamorphic : s = "Anamorphic";  break;
    case monomorphic: s = "Monomorphic"; break;
    case polymorphic: s = "Polymorphic"; break;
    case megamorphic: s = "Megamorphic"; break;
    default         : ShouldNotReachHere();
  }
  std->print("%s IC: %d entries\n", s, number_of_targets());

  IC_Iterator* it = iterator();
  it->init_iteration();
  while (!it->at_end()) {
    lprintf("\t- klass: ");
    it->klass()->print_value();
    if (it->is_interpreted()) {
      lprintf(";\tmethod  %#x\n", it->interpreted_method());
    } else {
      lprintf(";\tnmethod %#x\n", it->compiled_method());
    }
    it->advance();
  }
}

void IC_Iterator::goto_elem(int n) {
  init_iteration();
  for (int i = 0; i < n; i++) advance();
}

methodOop IC_Iterator::interpreted_method(int i) {
  goto_elem(i);
  return interpreted_method();
}

nmethod*  IC_Iterator::compiled_method(int i) {
  goto_elem(i);
  return compiled_method();
}

klassOop  IC_Iterator::klass(int i) {
  goto_elem(i);
  return klass();
}

