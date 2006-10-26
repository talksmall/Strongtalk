/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.62 $ */
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
# include "incls/_methodPrinter.cpp.incl"

MethodPrinterClosure::MethodPrinterClosure(outputStream* stream) {
  st = stream ? stream : std;
}

void MethodPrinterClosure::indent() {
  if (WizardMode) {
    st->indent();
    st->print("      <");
    GrowableArray<int>* map = method()->expression_stack_mapping(bci());
    for (int i = 0; i < map->length(); i++)
      st->print(" %d", map->at(i));
    st->print_cr(" >");
  }
  st->indent();
  st->print("[%3d] ", bci());
}

void MethodPrinterClosure::show(char* str) {
  st->indent();
  st->print_cr("      %s", str);
}

void MethodPrinterClosure::print_sendtype(Bytecodes::SendType type) {
  st->print("(");
  switch (type) {
    case Bytecodes::interpreted_send: st->print("interpreted"); break;
    case Bytecodes::compiled_send   : st->print("compiled"   ); break;
    case Bytecodes::polymorphic_send: st->print("polymorphic"); break;
    case Bytecodes::megamorphic_send: st->print("megamorphic"); break;
    case Bytecodes::predicted_send  : st->print("predicted"  ); break;
    case Bytecodes::accessor_send   : st->print("access"     ); break;
    default                         : ShouldNotReachHere();     break;
  }
  st->print(")");
}

void MethodPrinterClosure::if_node(IfNode* node) {
  indent();
  node->selector()->print_symbol_on(st); 
  st->print_cr(" {%d} [", node->end_bci());
  MethodIterator mi(node->then_code(), this);
  if (node->else_code() && !node->ignore_else_while_printing()) {
    show("]  [");
    MethodIterator mi(node->else_code(), this);
  }
  show("]");
}

void MethodPrinterClosure::cond_node(CondNode* node) {
  indent();
  node->selector()->print_symbol_on(st);
  st->cr();
  show("[");
  MethodIterator mi(node->expr_code(), this);
  show("]");
}

void MethodPrinterClosure::while_node(WhileNode* node) {
  indent();
  node->selector()->print_symbol_on(st);
  st->print_cr(" {%d} [", node->end_bci());
  if (node->body_code()) {
    MethodIterator mi(node->body_code(), this);
    show("] [");
  }
  MethodIterator mi(node->expr_code(), this);
  show("]");
}

void MethodPrinterClosure::primitive_call_node(PrimitiveCallNode* node) {
  indent();
  st->print_cr("primitive call '%s'", node->pdesc()->name());
  if (node->failure_code()) {
    MethodIterator mi(node->failure_code(), this);
    show("]");
  }
}


void MethodPrinterClosure::dll_call_node(DLLCallNode* node) {
  indent();
  st->print("dll call <");
  node->dll_name()->print_symbol_on(st);
  st->print(",");
  node->function_name()->print_symbol_on(st);
  st->print_cr(",%d>", node->nofArgs());
  if (node->failure_code()) {
    show("[");
    MethodIterator mi(node->failure_code(), this);
    show("]");
  }
}


void MethodPrinterClosure::allocate_temporaries(int nofTemps) {
  indent();
  st->print_cr("allocate %d temporaries", nofTemps);
}

void MethodPrinterClosure::push_self() {
  indent();
  st->print_cr("push self"); 
}

void MethodPrinterClosure::push_tos() {
  indent();
  st->print_cr("push tos"); 
}

void MethodPrinterClosure::push_literal(oop obj) {
  indent();
  st->print("push literal "); 
  obj->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::push_argument(int no) {
  indent();
  st->print_cr("push arg %d", no); 
}

void MethodPrinterClosure::push_temporary(int no) {
  indent();
  st->print_cr("push temp %d", no);
}

void MethodPrinterClosure::push_temporary(int no, int context) {
  indent();
  st->print_cr("push temp %d [%d]", no, context);
}

void MethodPrinterClosure::push_instVar(int offset) {
  indent();
  st->print_cr("push instVar %d", offset);
}

void MethodPrinterClosure::push_instVar_name(symbolOop name) {
  indent();
  st->print("push instVar ");
  name->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::push_classVar(associationOop assoc) {
  indent();
  st->print("store classVar ");
  assoc->key()->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::push_classVar_name(symbolOop name) {
  indent();
  st->print("store classVar ");
  name->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::push_global(associationOop obj) {
  indent();
  st->print("push global ");
  obj->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::store_temporary(int no) {
  indent();
  st->print_cr("store temp %d", no);
}

void MethodPrinterClosure::store_temporary(int no, int context) {
  indent();
  st->print_cr("store temp %d [%d]", no, context);
}

void MethodPrinterClosure::store_instVar(int offset) {
  indent();
  st->print_cr("store instVar %d", offset);
}

void MethodPrinterClosure::store_instVar_name(symbolOop name) {
  indent();
  st->print("store instVar ");
  name->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::store_classVar(associationOop assoc) {
  indent();
  st->print("store classVar ");
  assoc->key()->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::store_classVar_name(symbolOop name) {
  indent();
  st->print("store classVar ");
  name->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::store_global(associationOop obj) {
  indent();
  st->print("store global "); 
  obj->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::pop() {
  indent();
  st->print_cr("pop");
}

void MethodPrinterClosure::normal_send(InterpretedIC* ic) {
  indent();
  st->print("normal send ");
  print_sendtype(ic->send_type());
  st->print(" ");
  oop s = oop(ic->selector());
  if (!s->is_smi() && Universe::is_heap((oop*) s)) {
    assert_symbol(ic->selector(), "selector in ic must be a symbol");
    ic->selector()->print_value_on(st);
  } else {
    st->print("INVALID SELECTOR, 0x%lx", s);
  }
  st->cr();
}

void MethodPrinterClosure::self_send(InterpretedIC* ic) {
  indent();
  st->print("self send ");
  print_sendtype(ic->send_type());
  ic->selector()->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::super_send(InterpretedIC* ic) {
  indent();
  st->print("super send ");
  print_sendtype(ic->send_type());
  ic->selector()->print_value_on(st);
  st->cr();
}

void MethodPrinterClosure::double_equal() { 
  indent();
  st->print_cr("hardwired ==");
}

void MethodPrinterClosure::double_not_equal() {
  indent();
  st->print_cr("hardwired ~~");
}

void MethodPrinterClosure::method_return(int nofArgs) {
  indent();
  st->print_cr("return (pop %d args)", nofArgs);
}

void MethodPrinterClosure::nonlocal_return(int nofArgs) {
  indent();
  st->print_cr("non-local return (pop %d args)", nofArgs);
}

void MethodPrinterClosure::allocate_closure(AllocationType type, int nofArgs, methodOop meth) {
  indent();
  st->print("allocate closure");
  switch (type) {
  case tos_as_scope:  
    st->print("{tos}"); break;
  case context_as_scope:
    st->print("{context}"); break;
  }
  st->print_cr(" %d args", nofArgs);

  {
    st->inc();
    st->inc();
    MethodIterator it(meth, &MethodPrinterClosure(st));
    st->dec();
    st->dec();
  }
}

void MethodPrinterClosure::allocate_context(int nofTemps, bool forMethod) {
  indent();
  st->print_cr("allocate %s context with %d temporaries", forMethod ? "method" : "block", nofTemps);
}

void MethodPrinterClosure::set_self_via_context() {
  indent();
  st->print_cr("set self via context");
}


void MethodPrinterClosure::copy_self_into_context() {
  indent();
  st->print_cr("copy self into context");
}

void MethodPrinterClosure::copy_argument_into_context(int argNo, int no) {
  indent();
  st->print_cr("copy argument %d into context at %d", argNo,  no);
}

void MethodPrinterClosure::zap_scope() {
  indent();
  st->print_cr("zap block");
}

void MethodPrinterClosure::predict_prim_call(primitive_desc* pdesc, int failure_start) {
  indent();
  st->print_cr("predicted prim method");  
}

void MethodPrinterClosure::float_allocate(int nofFloatTemps, int nofFloatExprs) {
  indent();
  st->print_cr("float allocate temps=%d, expr=%d", nofFloatTemps, nofFloatExprs);  
}

void MethodPrinterClosure::float_floatify(Floats::Function f, int tof) {
  indent();
  st->print("float floatify ");
  Floats::selector_for(f)->print_value_on(st);
  st->print_cr(" tof=%d", tof);  
}

void MethodPrinterClosure::float_move(int tof, int from) {
  indent();
  st->print_cr("float move tof=%d, from=%d", tof, from);  
}

void MethodPrinterClosure::float_set(int tof, doubleOop value) {
  indent();
  st->print_cr("float set tof=%d, value=%1.6g", tof, value->value());  
}

void MethodPrinterClosure::float_nullary(Floats::Function f, int tof) {
  indent();
  st->print("float nullary ");
  Floats::selector_for(f)->print_value_on(st);
  st->print_cr(" tof=%d", tof);  
}

void MethodPrinterClosure::float_unary(Floats::Function f, int tof) {
  indent();
  st->print("float unary ");
  Floats::selector_for(f)->print_value_on(st);
  st->print_cr(" tof=%d", tof);  
}

void MethodPrinterClosure::float_binary(Floats::Function f, int tof) {
  indent();
  st->print("float binary ");
  Floats::selector_for(f)->print_value_on(st);
  st->print_cr(" tof=%d", tof);  
}

void MethodPrinterClosure::float_unaryToOop(Floats::Function f, int tof) {
  indent();
  st->print("float unaryToOop ");
  Floats::selector_for(f)->print_value_on(st);
  st->print_cr(" tof=%d", tof);  
}

void MethodPrinterClosure::float_binaryToOop(Floats::Function f, int tof) {
  indent();
  st->print("float binaryToOop ");
  Floats::selector_for(f)->print_value_on(st);
  st->print_cr(" tof=%d", tof);  
}
