/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.71 $ */
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
# include "incls/_methodKlass.cpp.incl"

void set_methodKlass_vtbl(Klass *k) {
  methodKlass o;
  k->set_vtbl_value(o.vtbl_value());
}

klassOop methodKlass::create_subclass(mixinOop mixin, Format format) {
  return NULL;
}

void methodKlass::oop_layout_iterate(oop obj, ObjectLayoutClosure* blk) {
  // header
  memOop(obj)->layout_iterate_header(blk);
  methodOop m = methodOop(obj);
  blk->do_oop("debugInfo", (oop*) &m->addr()->_debugInfo);
  blk->do_oop("selector",  (oop*) &m->addr()->_selector_or_method);
  blk->do_oop("sizeCodes", (oop*) &m->addr()->_size_and_flags);
  // indexables
  lprintf("methodKlass::oop_layout_iterate not implemented yet\n");
  CodeIterator c(m);
  do {
    // Put in the meat here.
  } while (c.advance());
}

void methodKlass::oop_oop_iterate(oop obj, OopClosure* blk) {
  // header
  memOop(obj)->oop_iterate_header(blk);
  methodOop m = methodOop(obj);
  blk->do_oop((oop*) &m->addr()->_debugInfo);
  blk->do_oop((oop*) &m->addr()->_selector_or_method);
  blk->do_oop((oop*) &m->addr()->_size_and_flags);
  // codes
  CodeIterator c(m);
  do {
    switch(c.format()) {
     case Bytecodes::BBO  : blk->do_oop(c.aligned_oop(2));   break; // BBO
     case Bytecodes::BBOO : blk->do_oop(c.aligned_oop(2));
     case Bytecodes::BBLO : blk->do_oop(c.aligned_oop(2)+1); break; // BBOO, BBLO
     case Bytecodes::BOL  :
     case Bytecodes::BO   : blk->do_oop(c.aligned_oop(1));   break; // BOL, BO
     case Bytecodes::BOO  :
     case Bytecodes::BOOLB: blk->do_oop(c.aligned_oop(1));
     case Bytecodes::BLO  : blk->do_oop(c.aligned_oop(1)+1); break; // BOO, BOOLB, BLO
    }
  } while (c.advance());
}

void methodKlass::oop_print_layout(oop obj) {
  methodOop(obj)->print_codes();
}

void methodKlass::oop_print_on(oop obj, outputStream* st) {
  assert(obj->is_method(), "must be method");
  methodOop method = methodOop(obj);

  int indent_col =  3;
  int value_col  = 16;

  // header
  memOopKlass::oop_print_value_on(obj, st);
  st->cr();

  // selector/outer method
  st->fill_to(indent_col);
  st->print("%s:", method->is_blockMethod() ? "outer" : "selector");
  st->fill_to(value_col);
  method->selector_or_method()->print_value_on(st);
  st->cr();

  // holder
  klassOop k = Universe::method_holder_of(method->home());
  if (k) {
    st->fill_to(indent_col);
    st->print("holder:");
    st->fill_to(value_col);
    k->print_value_on(st);
    st->cr();
  }

  // incovation counter
  st->fill_to(indent_col);
  st->print("invocation:");
  st->fill_to(value_col);
  st->print_cr("%d", method->invocation_count());

  // sharing counter
  st->fill_to(indent_col);
  st->print("sharing:");
  st->fill_to(value_col);
  st->print_cr("%d", method->sharing_count());

  // code size
  st->fill_to(indent_col);
  st->print("code size:");
  st->fill_to(value_col);
  st->print_cr("%d", method->size_of_codes());

  // arguments
  st->fill_to(indent_col);
  st->print("arguments:");
  st->fill_to(value_col);
  st->print_cr("%d", method->nofArgs());

  // debug array
  st->fill_to(indent_col);
  st->print("debug info:");
  st->fill_to(value_col);
  method->debugInfo()->print_value_on(st);
  st->cr();

  // flags
  st->fill_to(indent_col);
  st->print("flags:");
  st->fill_to(value_col);
  st->print("%s", method->is_customized() ? "customized" : "not_customized");
  if (method->allocatesInterpretedContext()) {
    st->print(" allocates_context"); 
  }
  if (method->mustBeCustomizedToClass()) {
    st->print(" class_specific"); 
  }
  if (method->containsNLR()) {
    st->print(" NLR"); 
  }

  // flags for blocks
  if (method->is_blockMethod()) {
    switch(method->block_info()) {
      case methodOopDesc::expects_nil:       st->print(" pure_block");              break;
      case methodOopDesc::expects_self:      st->print(" self_copying_block");      break;
      case methodOopDesc::expects_parameter: st->print(" parameter_copying_block"); break;
      case methodOopDesc::expects_context:   st->print(" full_block");              break;
    }
  } else { 
    switch(method->method_inlining_info()) {
      case methodOopDesc::normal_inline:     st->print(" normal inline");           break;
      case methodOopDesc::never_inline:      st->print(" never inline");            break;
      case methodOopDesc::always_inline:     st->print(" aways inline");            break;
    }
  }
  st->cr();
}

void methodKlass::oop_print_value_on(oop obj, outputStream* st) {
  assert(obj->is_method(), "must be method");
  methodOop method = methodOop(obj);
  if (PrintObjectID) {
    memOop(obj)->print_id_on(st);
    st->print("-");
  }
  if (method->is_blockMethod()) {
    st->print("BlockMethod in ");
    method->enclosing_method_selector()->print_symbol_on(st);
  } else {
    st->print("Method ");
    method->selector()->print_symbol_on(st);
  }
  if (ProfilerShowMethodHolder) {
    klassOop k = Universe::method_holder_of(method);
    st->print(" in ");
    if (k) {
      k->print_value_on(st);
    } else {
      st->print("(unknown)");
    }
  }
}

int methodKlass::oop_scavenge_contents(oop obj) {
  // Methods must reside in old space
  ShouldNotCallThis();
  return -1;  
}

int methodKlass::oop_scavenge_tenured_contents(oop obj) {
  // There should be no new objects referred insinde a methodOop
  return object_size(methodOop(obj)->size_of_codes());  
}

void methodKlass::oop_follow_contents(oop obj) {
  methodOop m = methodOop(obj);
  CodeIterator c(m);
  do {
    switch(c.format()) {
     case Bytecodes::BBO  : MarkSweep::reverse_and_push(c.aligned_oop(2));   break; // BBO
     case Bytecodes::BBOO : MarkSweep::reverse_and_push(c.aligned_oop(2));
     case Bytecodes::BBLO : MarkSweep::reverse_and_push(c.aligned_oop(2)+1); break; // BBOO, BBLO
     case Bytecodes::BOL  :
     case Bytecodes::BO   : MarkSweep::reverse_and_push(c.aligned_oop(1));   break; // BOL, BO
     case Bytecodes::BOO  :
     case Bytecodes::BOOLB: MarkSweep::reverse_and_push(c.aligned_oop(1));
     case Bytecodes::BLO  : MarkSweep::reverse_and_push(c.aligned_oop(1)+1); break; // BOO, BOOLB, BLO
    }
  } while (c.advance());
  MarkSweep::reverse_and_push((oop*) &m->addr()->_debugInfo);
  MarkSweep::reverse_and_push((oop*) &m->addr()->_selector_or_method);
  m->follow_header();
}

static oop tenured(oop obj) {
  return obj->is_old() ? obj : obj->shallow_copy(true);
}

methodOop methodKlass::constructMethod(oop selector_or_method, int flags, int nofArgs, 
                                       objArrayOop debugInfo, byteArrayOop bytes, objArrayOop oops) {
  klassOop k        = as_klassOop();
  int      obj_size = methodOopDesc::header_size() + oops->length();

  assert(oops->length() * oopSize == bytes->length(), "Invalid array sizes");

  // allocate
  methodOop method = as_methodOop(Universe::allocate_tenured(obj_size));
  memOop(method)->initialize_header(has_untagged_contents(), k);

  // initialize the header
  method->set_debugInfo(objArrayOop(tenured(debugInfo)));
  method->set_selector_or_method(tenured(selector_or_method));

  method->set_invocation_count(0);
  method->set_sharing_count(0);
  method->set_size_and_flags(oops->length(), nofArgs, flags);

  // merge the bytes and the oops

  // first copy the byte array into the method
  for (int index = 1; index <= bytes->length(); index++) {
    method->byte_at_put(index, bytes->byte_at(index));
  }

  // then merge in the oops
  for (index = 1; index <= oops->length(); index++) {
    bool copyOop = true;
    int bc_index = index*oopSize-(oopSize-1);
    for (int i = 0; i < oopSize; i++) {
      // copy oop if bytearray holds 4 consecutive aligned zeroes
      if (bytes->byte_at(bc_index+i) != 0) {
	copyOop = false;
      }
    }
    if (copyOop) {
      oop value = tenured(oops->obj_at(index));
      assert(value->is_smi() || value->is_old(), "literal must be tenured");
      method->oop_at_put(bc_index, value);
    }
  }

  assert(method->is_method(), "must be method");
  return method;
}
