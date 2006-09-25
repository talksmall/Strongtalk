/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.22 $ */
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
# include "incls/_symbolKlass.cpp.incl"
# include <ctype.h>

symbolOop symbolKlass::allocateSymbol(char* value, int len) {
  symbolOop sym = as_symbolOop(Universe::allocate_tenured(object_size(len)));
  sym->init_untagged_contents_mark();
  sym->set_klass_field(Universe::symbolKlassObj());
  sym->set_length(len);
  initialize_object(sym, value, len);
  return sym;
}

klassOop symbolKlass::create_subclass(mixinOop mixin, Format format) {
  if (format == mem_klass || format == symbol_klass) {
    return symbolKlass::create_class(as_klassOop(), mixin);
  }
  return NULL;
}

klassOop symbolKlass::create_class(klassOop super_class, mixinOop mixin) {
  symbolKlass o;
  return create_generic_class(super_class, mixin, o.vtbl_value());
}

void set_symbolKlass_vtbl(Klass* k) {
  symbolKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop symbolKlass::scavenge(oop obj) {
  Unused(obj);
  ShouldNotCallThis(); // shouldn't need to scavenge canonical symbols
                       // (should be tenured)
  return NULL;
}

bool symbolKlass::verify(oop obj) {
  return symbolOop(obj)->verify();
}

void symbolKlass::oop_print_value_on(oop obj, outputStream* st) {
  assert_symbol(obj, "dispatch check");
  symbolOop array = symbolOop(obj);
  int len = array->length();
  int n   = min(MaxElementPrintSize, len);
  st->print("#");
  for(int index = 1; index <= n; index++) {
    char c = array->byte_at(index);
    if (isprint(c)) st->print("%c",   c);
    else            st->print("\\%o", c);
  }
 if (n < len) st->print("...");
}

void symbolKlass::print(oop obj) {
  assert_symbol(obj, "dispatch check");
  std->print("'");
  symbolOop(obj)->print_symbol_on();
  std->print("' ");
}

oop symbolKlass::oop_shallow_copy(oop obj, bool tenured) {
  assert_symbol(obj, "dispatch check");
  return obj;
}
