/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.36 $ */
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
# include "incls/_blockKlass.cpp.incl"

klassOop blockClosureKlass::blockKlassFor(int numberOfArguments) {
  switch (numberOfArguments) {
    case 0: return Universe::zeroArgumentBlockKlassObj();  break;
    case 1: return Universe::oneArgumentBlockKlassObj();   break;
    case 2: return Universe::twoArgumentBlockKlassObj();   break;
    case 3: return Universe::threeArgumentBlockKlassObj(); break;
    case 4: return Universe::fourArgumentBlockKlassObj();  break;
    case 5: return Universe::fiveArgumentBlockKlassObj();  break;
    case 6: return Universe::sixArgumentBlockKlassObj();   break;
    case 7: return Universe::sevenArgumentBlockKlassObj(); break;
    case 8: return Universe::eightArgumentBlockKlassObj(); break;
    case 9: return Universe::nineArgumentBlockKlassObj();  break;
  }
 fatal("cannot handle block with more than 9 arguments");
 return NULL;
}

bool blockClosureKlass::oop_verify(oop obj) {
  bool flag = Klass::oop_verify(obj);
  // FIX LATER
  return flag;
}

void set_blockClosureKlass_vtbl(Klass* k) {
  blockClosureKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop blockClosureKlass::allocateObject(bool permit_scavenge) {
  klassOop k    = as_klassOop();
  // allocate
  blockClosureOop obj =
    as_blockClosureOop(Universe::allocate(blockClosureOopDesc::object_size(), (memOop*)&k, permit_scavenge));
  // header
  obj->initialize_header(false, k);
  // %not initialized by the interpreter
  // obj->addr()->_method         = (methodOop)      smiOop_zero;
  // obj->addr()->_lexical_scope  = (heapContextOop) smiOop_zero;
  return obj;
}

klassOop blockClosureKlass::create_subclass(mixinOop mixin, Format format) {
  return NULL;
}

int blockClosureKlass::oop_scavenge_contents(oop obj) {
  // header
  memOop(obj)->scavenge_header();
  // %note _method can be ignored since methods are tenured 
  scavenge_oop((oop*) &blockClosureOop(obj)->addr()->_lexical_scope);
  return blockClosureOopDesc::object_size();
}

int blockClosureKlass::oop_scavenge_tenured_contents(oop obj) {
  // header
  memOop(obj)->scavenge_tenured_header();
  // %note _method can be ignored since methods are tenured 
  scavenge_tenured_oop((oop*) &blockClosureOop(obj)->addr()->_lexical_scope);
  return blockClosureOopDesc::object_size();
}

void blockClosureKlass::oop_follow_contents(oop obj) {
  // header
  memOop(obj)->follow_header();
  MarkSweep::reverse_and_push((oop*) &blockClosureOop(obj)->addr()->_methodOrJumpAddr);
  MarkSweep::reverse_and_push((oop*) &blockClosureOop(obj)->addr()->_lexical_scope);
}

void blockClosureKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  blk->do_oop("method", (oop*) &blockClosureOop(obj)->addr()->_methodOrJumpAddr);
  blk->do_oop("scope",  (oop*) &blockClosureOop(obj)->addr()->_lexical_scope);
}

void blockClosureKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  blk->do_oop((oop*) &blockClosureOop(obj)->addr()->_methodOrJumpAddr);
  blk->do_oop((oop*) &blockClosureOop(obj)->addr()->_lexical_scope);
}

int blockClosureKlass::number_of_arguments() const {
  klassOop k = klassOop(this);		// C++ bogosity alert
  if (k == Universe::zeroArgumentBlockKlassObj())  return 0;
  if (k == Universe::oneArgumentBlockKlassObj())   return 1;
  if (k == Universe::twoArgumentBlockKlassObj())   return 2;
  if (k == Universe::threeArgumentBlockKlassObj()) return 3;
  if (k == Universe::fourArgumentBlockKlassObj())  return 4;
  if (k == Universe::fiveArgumentBlockKlassObj())  return 5;
  if (k == Universe::sixArgumentBlockKlassObj())   return 6;
  if (k == Universe::sevenArgumentBlockKlassObj()) return 7;
  if (k == Universe::eightArgumentBlockKlassObj()) return 8;
  if (k == Universe::nineArgumentBlockKlassObj())  return 9;
  fatal("unknown block closure class");
  return 0;
}

void blockClosureKlass::oop_print_value_on(oop obj, outputStream* st) {
  if (PrintObjectID) {
    memOop(obj)->print_id_on(st);
    st->print("-");
  }
  st->print("[] in ");
  methodOop method = blockClosureOop(obj)->method();
  method->home()->selector()->print_symbol_on(st);
  st->print("(scope = ");
  blockClosureOop(obj)->lexical_scope()->print_value_on(st);
  st->print(")");
}

void set_contextKlass_vtbl(Klass* k) {
  contextKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

oop contextKlass::allocateObjectSize(int num_of_temps) {
  klassOop k        = as_klassOop();
  int      obj_size = contextOopDesc::header_size() + num_of_temps;
  // allocate
  contextOop obj = as_contextOop(Universe::allocate(obj_size, (memOop*)&k));
  // header
  obj->set_klass_field(k);
  //%clean the up later
  //  hash value must by convention be different from 0 (check markOop.hpp)
  obj->set_mark(markOopDesc::tagged_prototype()->set_hash(num_of_temps + 1));
  // indexables
  memOop(obj)->initialize_body(contextOopDesc::header_size(), obj_size);
  return obj;
}

klassOop contextKlass::create_subclass(mixinOop mixin, Format format) {
  return NULL;
}

contextOop contextKlass::allocate_context(int num_of_temps) {
  contextKlass* ck = (contextKlass*) contextKlassObj->klass_part();
  return contextOop(ck->allocateObjectSize(num_of_temps));
}

int contextKlass::oop_scavenge_contents(oop obj) {
  int size = contextOop(obj)->object_size();
  // header
  memOop(obj)->scavenge_header();
  scavenge_oop((oop*) &contextOop(obj)->addr()->_parent);
  // temporaries
  memOop(obj)->scavenge_body(contextOopDesc::header_size(), size);
  return size;
}

int contextKlass::oop_scavenge_tenured_contents(oop obj) {
  int size = contextOop(obj)->object_size();
  // header
  memOop(obj)->scavenge_tenured_header();
  scavenge_tenured_oop((oop*) &contextOop(obj)->addr()->_parent);
  // temporaries
  memOop(obj)->scavenge_tenured_body(contextOopDesc::header_size(), size);
  return size;
}

void contextKlass::oop_follow_contents(oop obj) {
  // header
  memOop(obj)->follow_header();
  MarkSweep::reverse_and_push((oop*) &contextOop(obj)->addr()->_parent);
  // temporaries

  // we have to find the header word in order to compute object size.
  // %implementation note:
  //   implement this another way if possible
  oop* root_or_mark = (oop*) memOop(obj)->mark();
  while (!oop(root_or_mark)->is_mark()) {
    root_or_mark = (oop*) *root_or_mark;
  }
  int len = markOop(root_or_mark)->hash() - 1;
  memOop(obj)->follow_body(contextOopDesc::header_size(), contextOopDesc::header_size() + len);
}

void contextKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  blk->do_oop((oop*) &contextOop(obj)->addr()->_parent);
  // temporaries
  memOop(obj)->oop_iterate_body(blk, contextOopDesc::header_size(), oop_size(obj));
}

void contextKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  blk->do_oop("home", (oop*) &contextOop(obj)->addr()->_parent);
  // temporaries
  memOop(obj)->layout_iterate_body(blk, contextOopDesc::header_size(), oop_size(obj));
}

int contextKlass::oop_size(oop obj) const {
  return contextOop(obj)->object_size();
}

void contextKlass::oop_print_value_on(oop obj, outputStream* st) {
  memOopKlass::oop_print_value_on(obj, st);
  assert(obj->is_context(), "must be context");
  contextOop con = contextOop(obj);
  st->print("{");
  con->print_home_on(st);
  st->print(" |%d}", con->length());
}

void contextKlass::oop_print_on(oop obj, outputStream* st) {
  memOopKlass::oop_print_value_on(obj, st);
  st->cr();
  assert(obj->is_context(), "must be context");
  contextOop con = contextOop(obj);
  st->print(" home: ");
  con->print_home_on(st);
  st->cr();
  for (int index = 0; index < con->length(); index++) {
    st->print(" - %d: ", index);
    con->obj_at(index)->print_value_on(st);
    st->cr();
  }
}

