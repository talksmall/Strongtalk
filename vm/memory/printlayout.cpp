/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.16 $ */
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
# include "incls/_printlayout.cpp.incl"
# include <ctype.h>

const int indent_col =  3;
const int value_col  = 16;

PrintObjectClosure::PrintObjectClosure(outputStream* st = NULL) {
  this->st = st ? st : std;
}

void PrintObjectClosure::do_object(memOop obj) {
  this->obj = obj;
  obj->print_value();
  if (WizardMode) {
    st->print(" (size = %d)", obj->size());
  }
  st->cr();
}

void PrintObjectClosure::do_mark(markOop *m) {
  st->fill_to(indent_col);
  st->print("mark");
  st->sp();
  st->fill_to(value_col);
  (*m)->print_value();
  st->cr(); 
}

void PrintObjectClosure::do_oop(char* title, oop* o) {
  symbolOop name = obj->blueprint()->inst_var_name_at(o-(oop*)obj->addr());
  st->fill_to(indent_col);
  if (name) {
    name->print_symbol_on(st);
  } else {
    st->print("%s", title); 
  }
  st->sp();
  st->fill_to(value_col);
  (*o)->print_value();
  st->cr();
}

void PrintObjectClosure::do_byte(char* title, u_char* b) {
  st->fill_to(indent_col);
  st->print("%s", title);
  st->sp();
  st->fill_to(value_col);
  char c = (char) *b;
  if (isprint(c)) st->print_cr("%c",   c);
  else            st->print_cr("\\%o", c);
}

void PrintObjectClosure::do_long(char* title, void** p) {
  st->fill_to(indent_col);
  st->print("%s", title);
  st->sp();
  st->fill_to(value_col);
  st->print_cr("%#lx", *p);
}

void PrintObjectClosure::do_double(char* title, double* d) {
  st->fill_to(indent_col);
  st->print("%s", title);
  st->sp();
  st->fill_to(value_col);
  st->print_cr("%.15f", *d);
}

void PrintObjectClosure::begin_indexables() {
}

void PrintObjectClosure::end_indexables() {
}
 
void PrintObjectClosure::do_indexable_oop(int index, oop* o) {
  if (index > MaxElementPrintSize) return;
  st->fill_to(indent_col);
  st->print("%d", index);
  st->sp();
  st->fill_to(value_col);
  (*o)->print_value();
  st->cr();
}

void PrintObjectClosure::do_indexable_byte(int index, u_char* b) {
  if (index > MaxElementPrintSize) return;
  st->fill_to(indent_col);
  st->print("%d", index);
  st->sp();
  st->fill_to(value_col);
  int c = (int) *b;
  if (isprint(c)) st->print_cr("%c",   c);
  else            st->print_cr("\\%o", c);
}

void PrintObjectClosure::do_indexable_doubleByte(int index, doubleByte* b) {
  if (index > MaxElementPrintSize) return;
  st->fill_to(indent_col);
  st->print("%d", index);
  st->sp();
  st->fill_to(value_col);
  int c = (int) *b;
  if (isprint(c)) st->print_cr("%c",   c);
  else            st->print_cr("\\%o", c);
}

void PrintObjectClosure::do_indexable_long(int index, long* l) {
  if (index > MaxElementPrintSize) return;
  st->fill_to(indent_col);
  st->print("%d", index);
  st->sp();
  st->fill_to(value_col);
  st->print_cr("0x%lx", *l);
}

