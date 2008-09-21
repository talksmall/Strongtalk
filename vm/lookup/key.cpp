/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.30 $ */
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
# include "incls/_key.cpp.incl"

int LookupKey::hash() const {
  return klass()->identity_hash() ^ selector_or_method()->identity_hash();
}

void LookupKey::relocate() {
  RELOCATE_TEMPLATE(&_klass);
  RELOCATE_TEMPLATE(&_selector_or_method);
}

symbolOop LookupKey::selector() const {
  if (is_normal_type()) {
    return symbolOop(selector_or_method());
  } else {
    return method()->selector();
  }
}

bool LookupKey::verify() const {
  bool flag = true;
  if (!klass()->is_klass()) {
    error("klass %#lx isn't a klass", klass());
    flag = false;
  }
  if (!selector_or_method()->is_symbol() && !selector_or_method()->is_method()) {
    lprintf("\tin selector_or_method of LookupKey %#lx\n", this);
    flag = false;
  }
  return flag;
}


void LookupKey::switch_pointers(oop from, oop to) {
  SWITCH_POINTERS_TEMPLATE(&_klass);
  SWITCH_POINTERS_TEMPLATE(&_selector_or_method);
}

void LookupKey::oops_do(void f(oop*)) {
  f((oop*) &_klass);
  f((oop*) &_selector_or_method);
}

void LookupKey::print_on(outputStream* st) const {
  print_inlining_database_on(st);
}

void LookupKey::print_inlining_database_on(outputStream* st) const {
  if (klass()) {
    klass()->klass_part()->print_name_on(st);
  } else {
    st->print("NULL");
  }
  if (is_normal_type()) {
    st->print("::");
    selector()->print_symbol_on(st);
  } else if (is_super_type()) {
    st->print("^^");
    method()->selector()->print_symbol_on(st);
  } else if (is_block_type()) {
    st->print("->");
    method()->print_inlining_database_on(st);
  } else {
    ShouldNotReachHere();
  }
}

void LookupKey::print() const {
  print_on(std);
}

char* LookupKey::print_string() const {
  stringStream* st = new stringStream(50);
  print_on(st);
  return st->as_string();
}

LookupKey* LookupKey::allocate(klassOop klass, oop selector_or_method) {
  LookupKey* result = NEW_RESOURCE_OBJ(LookupKey);
  result->initialize(klass, selector_or_method);
  return result;
}

