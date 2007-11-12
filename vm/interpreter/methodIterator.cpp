/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.52 $ */
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
# include "incls/_methodIterator.cpp.incl"


// MethodInterval

MethodInterval::MethodInterval(methodOop method, MethodInterval* parent) {
  initialize(method, parent, 1, method->end_bci(), false);
}

MethodInterval::MethodInterval(methodOop method, MethodInterval* parent, int begin_bci, int end_bci, bool failBlock) {
  initialize(method, parent, begin_bci, end_bci, failBlock);
}

void MethodInterval::initialize(methodOop method, MethodInterval* parent, int begin_bci, int end_bci, bool failBlock) {
  _method    = method;
  _parent    = parent;
  _begin_bci = begin_bci;
  _end_bci   = end_bci;
  _in_prim_failure = failBlock;
#ifdef DELTA_COMPILER
  _info = NULL;
#endif
}


// InlineSendNode

InlineSendNode::InlineSendNode(methodOop method, MethodInterval* parent, int begin_bci, int end_bci)
 : MethodInterval(method, parent, begin_bci, end_bci) {}


// CondNode

CondNode::CondNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset)
 : InlineSendNode(method, parent, begin_bci, next_bci + dest_offset) {
  _expr_code = MethodIterator::factory->new_MethodInterval(method, this, next_bci, end_bci());
}


// AndNode

AndNode::AndNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset)
 : CondNode(method, parent, begin_bci, next_bci, dest_offset) {}

symbolOop AndNode::selector() const {
  return vmSymbols::and_();
}


// OrNode

OrNode::OrNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset)
 : CondNode(method, parent, begin_bci, next_bci, dest_offset) {}

symbolOop OrNode::selector() const {
  return vmSymbols::or_();
}


// WhileNode

WhileNode::WhileNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int cond_offset, 
		     int end_offset)
 : InlineSendNode(method, parent, begin_bci) {

  CodeIterator c(method, next_bci + cond_offset + end_offset);
  switch (c.code()) {
    case Bytecodes::whileTrue_byte: 
    case Bytecodes::whileTrue_word: 
    	_cond = true;
        break;
    case Bytecodes::whileFalse_byte:
    case Bytecodes::whileFalse_word:
    	_cond = false;
        break;
    default: fatal("expecting while jump");
  }
  int jump_end = c.next_bci();

  if (cond_offset == 0) {
    _body_code = NULL;
    _expr_code = MethodIterator::factory->new_MethodInterval(method, this, next_bci, jump_end);
  } else {
    int cond_dest = next_bci + cond_offset;
    _body_code = MethodIterator::factory->new_MethodInterval(method, this, next_bci, cond_dest);
    _expr_code = MethodIterator::factory->new_MethodInterval(method, this, cond_dest, jump_end);
  }
  set_end_bci(expr_code()->end_bci());
}

symbolOop WhileNode::selector() const {
  if (is_whileTrue()) return body_code() ? vmSymbols::while_true_()  : vmSymbols::while_true();
  else                return body_code() ? vmSymbols::while_false_() : vmSymbols::while_false();
}


// IfNode

IfNode::IfNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool cond, 
	       int else_offset, u_char structure)
 : InlineSendNode(method, parent, begin_bci) {
  bool has_else_branch;
  int  else_jump_size;
  _cond                       = cond;
  _produces_result            = isSet(structure, 0);
  has_else_branch             = isSet(structure, 1);
  _ignore_else_while_printing = isSet(structure, 2);
  else_jump_size              = structure >> 4;

  if (has_else_branch) {
    int else_jump = next_bci + else_offset - else_jump_size;
    _then_code = MethodIterator::factory->new_MethodInterval(method, this, next_bci, else_jump);
    CodeIterator c(method, else_jump);
    int end_offset;
    switch (c.code()) {
      case Bytecodes::jump_else_byte: end_offset = c.byte_at(1); break;
      case Bytecodes::jump_else_word: end_offset = c.word_at(1); break;
      default: fatal("expecting an else jump");
    }
    _else_code = MethodIterator::factory->new_MethodInterval(method, this, next_bci + else_offset, 
							     next_bci + else_offset + end_offset);
    set_end_bci(else_code()->end_bci());
  } else {
    _then_code = MethodIterator::factory->new_MethodInterval(method, this, next_bci, next_bci + else_offset);
    _else_code = NULL;
    set_end_bci(then_code()->end_bci());
  }
}

symbolOop IfNode::selector() const {
  if (is_ifTrue()) return else_code() ? vmSymbols::if_true_false() : vmSymbols::if_true();
  else             return else_code() ? vmSymbols::if_false_true() : vmSymbols::if_false();
}


// ExternalCallNode

ExternalCallNode::ExternalCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci) 
 : MethodInterval(method, parent, begin_bci, next_bci) {
  _failure_code = NULL;
}

ExternalCallNode::ExternalCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int end_offset) 
 : MethodInterval(method, parent, begin_bci, next_bci + end_offset) {
  assert(end_offset > 0, "wrong offset");
  _failure_code = MethodIterator::factory->new_MethodInterval(method, this, next_bci, end_bci(), true);
}


// PrimitiveCallNode

PrimitiveCallNode::PrimitiveCallNode(methodOop       method, 
				     MethodInterval* parent,
                                     int             begin_bci,
                                     int             next_bci,
                                     bool            has_receiver,
                                     symbolOop       name,
                                     primitive_desc* pdesc) 
 : ExternalCallNode(method, parent, begin_bci, next_bci) {
  assert( (name == NULL) != (pdesc == NULL), "we need one an only one kind");
  _has_receiver = has_receiver;
  _name  = (name  == NULL) ? pdesc->selector()        : name;
  _pdesc = (pdesc == NULL) ? primitives::lookup(name) : pdesc;
}

// DLLCallNode

PrimitiveCallNode::PrimitiveCallNode(methodOop       method, 
				     MethodInterval* parent,
                                     int             begin_bci,
                                     int             next_bci,
                                     bool            has_receiver,
                                     symbolOop       name,
                                     primitive_desc* pdesc,
                                     int             end_offset)
 : ExternalCallNode(method, parent, begin_bci, next_bci, end_offset) {
  assert( (name == NULL) != (pdesc == NULL), "we need one an only one kind");
  _has_receiver = has_receiver;
  _name  = (name  == NULL) ? pdesc->selector()        : name;
  _pdesc = (pdesc == NULL) ? primitives::lookup(name) : pdesc;
}

int PrimitiveCallNode::number_of_parameters() const {
  int result = name()->number_of_arguments()
             + (has_receiver() ? 1 : 0)
	     - (failure_code() ? 1 : 0);
  assert(_pdesc == NULL || pdesc()->number_of_parameters() == result, "checking result");
  return result;
}

void DLLCallNode::initialize(InterpretedDLL_Cache* cache) {
  _dll_name      = cache->dll_name();
  _function_name = cache->funct_name();
  _nofArgs       = cache->number_of_arguments();
  _function      = cache->entry_point();
  _async         = cache->async();
}

DLLCallNode::DLLCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, 
			 InterpretedDLL_Cache* cache)
 : ExternalCallNode(method, parent, begin_bci, next_bci) {
  initialize(cache);
}


// MethodClosure

MethodClosure::MethodClosure() {
  _method = NULL;
  _bci = _next_bci = PrologueBCI;
  _aborting = false;
  _in_prim_failure = false;
  _float0_index = 0;
}

void MethodClosure::set_method(methodOop method) {
  _method = method;
  assert(method->number_of_stack_temporaries() % 2 == 0 || !method->has_float_temporaries(), "inconsistency");
  _float0_index = 256 - method->number_of_stack_temporaries()/2;
}

int MethodClosure::float_at(int index) {
  int fno = _float0_index - index;
  assert(0 <= fno && fno < _method->total_number_of_floats(), "illegal float number");
  return fno;
}

// CustomizedMethodClosure

void CustomizedMethodClosure::push_instVar_name(symbolOop name) {
  fatal("instance variable not resolved");
}

void CustomizedMethodClosure::store_instVar_name(symbolOop name) {
  fatal("instance variable not resolved");
}

void CustomizedMethodClosure::push_classVar_name(symbolOop name) {
  fatal("class variable not resolved");
}


void CustomizedMethodClosure::store_classVar_name(symbolOop name) {
  fatal("class variable not resolved");
}

void CustomizedMethodClosure::push_classVar(associationOop assoc) {
  push_global(assoc);
}

void CustomizedMethodClosure::store_classVar(associationOop assoc) {
  store_global(assoc);
}

// SpecializedMethodClosure

void SpecializedMethodClosure::if_node(IfNode* node) {
  MethodIterator iter(node->then_code(), this);
  if (node->else_code() != NULL) {
    MethodIterator iter(node->else_code(), this);
  }
}

void SpecializedMethodClosure::cond_node(CondNode* node) {
  MethodIterator iter(node->expr_code(), this);
}

void SpecializedMethodClosure::while_node(WhileNode* node) {
  MethodIterator iter(node->expr_code(), this);
  if (node->body_code() != NULL) {
    MethodIterator iter(node->body_code(), this);
  }
}

void SpecializedMethodClosure::primitive_call_node(PrimitiveCallNode* node) {
  if (node->failure_code() != NULL) {
    MethodIterator iter(node->failure_code(), this);
  }
}

void SpecializedMethodClosure::dll_call_node(DLLCallNode* node) {
  if (node->failure_code() != NULL) {
    MethodIterator iter(node->failure_code(), this);
  }
}

// MethodIterator

void MethodIterator::unknown_code(u_char code) {
  std->print_cr("Unknown code found 0x%x", code);
  fatal("aborting");
}

void MethodIterator::should_never_encounter(u_char code) {
  std->print_cr("Should never iterate through code 0x%x", code);
  fatal("aborting");
}

static inline u_char map0to256(u_char ch) { return ch ? ch : 256; }

void MethodIterator::dispatch(MethodClosure* blk) {
  bool oldFailState = blk->in_prim_failure_block();
  blk->set_prim_failure(_interval->in_prim_failure_block());
  CodeIterator iter(_interval->method(), _interval->begin_bci());
  int lastArgNo = _interval->method()->number_of_arguments() - 1;
  blk->set_method(_interval->method());
  int next_bci = _interval->begin_bci();

  while (next_bci < _interval->end_bci() && !blk->aborting()) {
    iter.set_bci(next_bci);
    blk->set_bci(iter.bci());

    next_bci = iter.next_bci();
    blk->set_next_bci(next_bci);

    switch(iter.code()) {
      case Bytecodes::push_temp_0:
              blk->push_temporary(0);
              break;
      case Bytecodes::push_temp_1:
              blk->push_temporary(1);
              break;
      case Bytecodes::push_temp_2:
              blk->push_temporary(2);
              break;
      case Bytecodes::push_temp_3:
              blk->push_temporary(3);
              break;
      case Bytecodes::push_temp_4:
              blk->push_temporary(4);
              break;
      case Bytecodes::push_temp_5:
              blk->push_temporary(5);
              break;
      case Bytecodes::unimplemented_06:
              unknown_code(Bytecodes::unimplemented_06);
              break;
      case Bytecodes::push_temp_n:
              blk->push_temporary(255 - iter.byte_at(1));
              break;
      case Bytecodes::push_arg_1:
              blk->push_argument(lastArgNo);
              break;
      case Bytecodes::push_arg_2:
              blk->push_argument(lastArgNo - 1);
              break;
      case Bytecodes::push_arg_3:
              blk->push_argument(lastArgNo - 2);
              break;
      case Bytecodes::push_arg_n:
              blk->push_argument(lastArgNo - iter.byte_at(1));
              break;
      case Bytecodes::allocate_temp_1:
              blk->allocate_temporaries(1);
              break;
      case Bytecodes::allocate_temp_2:
              blk->allocate_temporaries(2);
              break;
      case Bytecodes::allocate_temp_3:
              blk->allocate_temporaries(3);
              break;
      case Bytecodes::allocate_temp_n:
              blk->allocate_temporaries(map0to256(iter.byte_at(1)));
              break;
      case Bytecodes::store_temp_0_pop:
              blk->store_temporary(0);
              blk->pop();
              break;
      case Bytecodes::store_temp_1_pop:
              blk->store_temporary(1);
              blk->pop();
              break;
      case Bytecodes::store_temp_2_pop:
              blk->store_temporary(2);
              blk->pop();
              break;
      case Bytecodes::store_temp_3_pop:
              blk->store_temporary(3);
              blk->pop();
              break;
      case Bytecodes::store_temp_4_pop:
              blk->store_temporary(4);
              blk->pop();
              break;
      case Bytecodes::store_temp_5_pop:
              blk->store_temporary(5);
              blk->pop();
              break;
      case Bytecodes::store_temp_n:
              blk->store_temporary(255 - iter.byte_at(1));
              break;
      case Bytecodes::store_temp_n_pop:
              blk->store_temporary(255 - iter.byte_at(1));
              blk->pop();
              break;
      case Bytecodes::push_neg_n:
              blk->push_literal(as_smiOop(-(int) iter.byte_at(1)));
              break;
      case Bytecodes::push_succ_n:
              blk->push_literal(as_smiOop(iter.byte_at(1)+1));
              break;
      case Bytecodes::push_literal:
              blk->push_literal(iter.oop_at(1));
              break;
      case Bytecodes::push_tos:
              blk->push_tos();
              break;
      case Bytecodes::push_self:
              blk->push_self();
              break;
      case Bytecodes::push_nil:
              blk->push_literal(nilObj);
              break;
      case Bytecodes::push_true:
              blk->push_literal(trueObj);
              break;
      case Bytecodes::push_false:
              blk->push_literal(falseObj);
              break;
      case Bytecodes::unimplemented_20:
              unknown_code(Bytecodes::unimplemented_20);
              break;
      case Bytecodes::unimplemented_21:
              unknown_code(Bytecodes::unimplemented_21);
              break;
      case Bytecodes::unimplemented_22:
              unknown_code(Bytecodes::unimplemented_22);
              break;
      case Bytecodes::unimplemented_23:
              unknown_code(Bytecodes::unimplemented_23);
              break;
      case Bytecodes::unimplemented_24:
              unknown_code(Bytecodes::unimplemented_24);
              break;
      case Bytecodes::unimplemented_25:
              unknown_code(Bytecodes::unimplemented_25);
              break;
      case Bytecodes::unimplemented_26:
              unknown_code(Bytecodes::unimplemented_26);
              break;
      case Bytecodes::unimplemented_27:
              unknown_code(Bytecodes::unimplemented_27);
              break;
      case Bytecodes::return_instVar_name: {
	        symbolOop name = symbolOop(iter.oop_at(1));
		assert(name->is_symbol(), "must be symbol");
                blk->push_instVar_name(name);
                blk->method_return(0); 
	      }
	      break;
      case Bytecodes::push_classVar: {
                associationOop assoc = associationOop(iter.oop_at(1));
	        assert(assoc->is_association(), "must be association");
    	        blk->push_classVar(assoc); 
	      }
	      break;
      case Bytecodes::store_classVar_pop: {
                associationOop assoc = associationOop(iter.oop_at(1));
	        assert(assoc->is_association(), "must be association");
    	        blk->store_classVar(assoc);
		blk->pop(); 
	      }
              break;
      case Bytecodes::store_classVar: {
                associationOop assoc = associationOop(iter.oop_at(1));
	        assert(assoc->is_association(), "must be association");
    	        blk->store_classVar(assoc);
	      }
    	      break;
      case Bytecodes::return_instVar: {
	        smiOop offset = smiOop(iter.oop_at(1));
		assert(offset->is_smi(), "must be smi");
                blk->push_instVar(offset->value());
                blk->method_return(0); 
	      }
	      break;
      case Bytecodes::push_instVar: {
	        smiOop offset = smiOop(iter.oop_at(1));
		assert(offset->is_smi(), "must be smi");
                blk->push_instVar(offset->value());
	      }
	      break;
      case Bytecodes::store_instVar_pop: {
	        smiOop offset = smiOop(iter.oop_at(1));
		assert(offset->is_smi(), "must be smi");
                blk->store_instVar(offset->value());
	        blk->pop();
	      }
	      break;
      case Bytecodes::store_instVar:{
	        smiOop offset = smiOop(iter.oop_at(1));
		assert(offset->is_smi(), "must be smi");
                blk->store_instVar(offset->value());
	      }
	      break;
      case Bytecodes::float_allocate:
              blk->allocate_temporaries(1 + iter.byte_at(1)*2);
              blk->float_allocate(iter.byte_at(2), iter.byte_at(3));
	      break;
      case Bytecodes::float_floatify_pop:
              blk->float_floatify(Floats::floatify, blk->float_at(iter.byte_at(1)));
	      break;
      case Bytecodes::float_move:
              blk->float_move(blk->float_at(iter.byte_at(1)), blk->float_at(iter.byte_at(2)));
	      break;
      case Bytecodes::float_set:
              blk->float_set(blk->float_at(iter.byte_at(1)), *(doubleOop*)iter.aligned_oop(2));
	      break;
      case Bytecodes::float_nullary_op:
              blk->float_nullary(Floats::Function(iter.byte_at(2)), blk->float_at(iter.byte_at(1)));
	      break;
      case Bytecodes::float_unary_op:
              blk->float_unary(Floats::Function(iter.byte_at(2)), blk->float_at(iter.byte_at(1)));
	      break;
      case Bytecodes::float_binary_op:
              blk->float_binary(Floats::Function(iter.byte_at(2)), blk->float_at(iter.byte_at(1)));
	      break;
      case Bytecodes::float_unary_op_to_oop:
              blk->float_unaryToOop(Floats::Function(iter.byte_at(2)), blk->float_at(iter.byte_at(1)));
	      break;
      case Bytecodes::float_binary_op_to_oop:
              blk->float_binaryToOop(Floats::Function(iter.byte_at(2)), blk->float_at(iter.byte_at(1)));
	      break;
      case Bytecodes::unimplemented_39:
              unknown_code(Bytecodes::unimplemented_39);
	      break;
      case Bytecodes::unimplemented_3a:
              unknown_code(Bytecodes::unimplemented_3a);
	      break;
      case Bytecodes::unimplemented_3b:
              unknown_code(Bytecodes::unimplemented_3b); 
	      break;
      case Bytecodes::unimplemented_3c:
              unknown_code(Bytecodes::unimplemented_3c); 
	      break;
      case Bytecodes::push_instVar_name: {
		symbolOop name = symbolOop(iter.oop_at(1));
		assert(name->is_symbol(), "must be symbol");
                blk->push_instVar_name(name);
	      }
	      break;
      case Bytecodes::store_instVar_pop_name:  {
		symbolOop name = symbolOop(iter.oop_at(1));
		assert(name->is_symbol(), "must be symbol");
                blk->store_instVar_name(name);
		blk->pop();
	      }
	      break;
      case Bytecodes::store_instVar_name:  {
		symbolOop name = symbolOop(iter.oop_at(1));
		assert(name->is_symbol(), "must be symbol");
                blk->store_instVar_name(name);
	      }
	      break;
      case Bytecodes::push_temp_0_context_0:
              blk->push_temporary(0, 0);
              break;
      case Bytecodes::push_temp_1_context_0:
              blk->push_temporary(1, 0);
              break;
      case Bytecodes::push_temp_2_context_0:
              blk->push_temporary(2, 0);
              break;
      case Bytecodes::push_temp_n_context_0:
              blk->push_temporary(iter.byte_at(1), 0);
              break;
      case Bytecodes::store_temp_0_context_0_pop:
	      blk->store_temporary(0, 0);
              blk->pop();
              break;
      case Bytecodes::store_temp_1_context_0_pop:
              blk->store_temporary(1, 0);
              blk->pop();
              break;
      case Bytecodes::store_temp_2_context_0_pop: 
              blk->store_temporary(2, 0);
              blk->pop();
              break;
      case Bytecodes::store_temp_n_context_0_pop:
              blk->store_temporary(iter.byte_at(1), 0);
              blk->pop();
              break;
      case Bytecodes::push_new_closure_context_0:
              blk->allocate_closure(context_as_scope, 0, methodOop(iter.oop_at(1)));
              break;
      case Bytecodes::push_new_closure_context_1:
              blk->allocate_closure(context_as_scope, 1, methodOop(iter.oop_at(1)));
              break;
      case Bytecodes::push_new_closure_context_2:
              blk->allocate_closure(context_as_scope, 2, methodOop(iter.oop_at(1)));
              break;
      case Bytecodes::push_new_closure_context_n:
              blk->allocate_closure(context_as_scope, iter.byte_at(1), methodOop(iter.oop_at(2)));
              break;
      case Bytecodes::install_new_context_method_0:
              blk->allocate_context(0, true);
              break;
      case Bytecodes::install_new_context_method_1:
              blk->allocate_context(1, true);
              break;
     case Bytecodes::install_new_context_method_2:
              blk->allocate_context(2, true);
              break;
      case Bytecodes::install_new_context_method_n:
              blk->allocate_context(iter.byte_at(1), true);
              break;
      case Bytecodes::push_temp_0_context_1:
              blk->push_temporary(0, 1);
              break;
      case Bytecodes::push_temp_1_context_1:
              blk->push_temporary(1, 1);
              break;
      case Bytecodes::push_temp_2_context_1:
              blk->push_temporary(2, 1);
              break;
      case Bytecodes::push_temp_n_context_1:
              blk->push_temporary(iter.byte_at(1), 1);
              break;
      case Bytecodes::store_temp_0_context_1_pop:
              blk->store_temporary(0, 1);
              blk->pop();
              break;
      case Bytecodes::store_temp_1_context_1_pop:
              blk->store_temporary(1, 1);
              blk->pop();
              break;
      case Bytecodes::store_temp_2_context_1_pop:
              blk->store_temporary(2, 1);
              blk->pop();
              break;
      case Bytecodes::store_temp_n_context_1_pop:
              blk->store_temporary(iter.byte_at(1), 1);
              blk->pop();
              break;
      case Bytecodes::push_new_closure_tos_0:
              blk->allocate_closure(tos_as_scope, 0, methodOop(iter.oop_at(1)));
              break;
      case Bytecodes::push_new_closure_tos_1:
              blk->allocate_closure(tos_as_scope, 1, methodOop(iter.oop_at(1)));
              break;
      case Bytecodes::push_new_closure_tos_2:
              blk->allocate_closure(tos_as_scope, 2, methodOop(iter.oop_at(1)));
              break;
      case Bytecodes::push_new_closure_tos_n:
              blk->allocate_closure(tos_as_scope, iter.byte_at(1), methodOop(iter.oop_at(2)));
              break;
      case Bytecodes::only_pop:
              blk->pop();
              break;
      case Bytecodes::install_new_context_block_1:
              blk->allocate_context(1, false);
              break;
      case Bytecodes::install_new_context_block_2:
              blk->allocate_context(2, false);
              break;
      case Bytecodes::install_new_context_block_n:
              blk->allocate_context(iter.byte_at(1), false);
              break;
      case Bytecodes::push_temp_0_context_n:
              blk->push_temporary(0, iter.byte_at(1));
              break;
      case Bytecodes::push_temp_1_context_n:
              blk->push_temporary(1, iter.byte_at(1));
              break;
      case Bytecodes::push_temp_2_context_n:
              blk->push_temporary(2, iter.byte_at(1));
              break;
      case Bytecodes::push_temp_n_context_n:
	      blk->push_temporary(iter.byte_at(1), map0to256(iter.byte_at(2)));
	      break;
      case Bytecodes::store_temp_0_context_n_pop:
              blk->store_temporary(0, iter.byte_at(1));
              blk->pop();
              break;
      case Bytecodes::store_temp_1_context_n_pop:
              blk->store_temporary(1, iter.byte_at(1));
              blk->pop();
              break;
      case Bytecodes::store_temp_2_context_n_pop:
              blk->store_temporary(2, iter.byte_at(1));
              blk->pop();
              break;
      case Bytecodes::store_temp_n_context_n_pop:
	      blk->store_temporary(iter.byte_at(1), map0to256(iter.byte_at(2)));
	      blk->pop();
	      break;
      case Bytecodes::set_self_via_context:
              blk->set_self_via_context();
              break;
      case Bytecodes::copy_1_into_context:
              blk->copy_argument_into_context(lastArgNo - iter.byte_at(1), 0);
              break;
      case Bytecodes::copy_2_into_context:
              blk->copy_argument_into_context(lastArgNo - iter.byte_at(1), 0); 
              blk->copy_argument_into_context(lastArgNo - iter.byte_at(2), 1);
              break;
      case Bytecodes::copy_n_into_context: {
              int len = map0to256(iter.byte_at(1));
              for (int i = 0; i < len; i++)
                blk->copy_argument_into_context(lastArgNo - iter.byte_at(i + 2), i);
              break;
           }
      case Bytecodes::copy_self_into_context:
              blk->copy_self_into_context();
              break;
      case Bytecodes::copy_self_1_into_context:
              blk->copy_self_into_context(); 
              blk->copy_argument_into_context(lastArgNo - iter.byte_at(1), 1);
              break;
      case Bytecodes::copy_self_2_into_context:
              blk->copy_self_into_context();
              blk->copy_argument_into_context(lastArgNo - iter.byte_at(1), 1); 
              blk->copy_argument_into_context(lastArgNo - iter.byte_at(2), 2);
              break;
      case Bytecodes::copy_self_n_into_context: {
              blk->copy_self_into_context();
              int len = map0to256(iter.byte_at(1));
              for (int i = 0; i < len; i++)
                blk->copy_argument_into_context(lastArgNo - iter.byte_at(i + 2), i+1);
              break;
           }
      case Bytecodes::ifTrue_byte: {
              IfNode* node = MethodIterator::factory->new_IfNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), true, iter.byte_at(2), iter.byte_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->if_node(node);
	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::ifFalse_byte: {
              IfNode* node = MethodIterator::factory->new_IfNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), false, iter.byte_at(2), iter.byte_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->if_node(node);
	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::and_byte: {
              AndNode* node = MethodIterator::factory->new_AndNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), iter.byte_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->cond_node(node);
      	      next_bci = node->end_bci();
      	      break;
	   }
      case Bytecodes::or_byte: {
              OrNode* node = MethodIterator::factory->new_OrNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), iter.byte_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->cond_node(node);
      	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::whileTrue_byte:
              // ignore since they are inside WhileNode expression body
              break;
      case Bytecodes::whileFalse_byte:
              // ignore since they are inside WhileNode expression body
              break;
      case Bytecodes::jump_else_byte:
              should_never_encounter(Bytecodes::jump_else_byte);
              break;
      case Bytecodes::jump_loop_byte: {
              WhileNode* node = MethodIterator::factory->new_WhileNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), iter.byte_at(2), iter.byte_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->while_node(node);
      	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::ifTrue_word: {
              IfNode* node = MethodIterator::factory->new_IfNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), true, 
		iter.word_at(2), iter.byte_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->if_node(node);
	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::ifFalse_word: {
              IfNode* node = MethodIterator::factory->new_IfNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), false, 
		iter.word_at(2), iter.byte_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->if_node(node);
 	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::and_word: {
              AndNode* node = MethodIterator::factory->new_AndNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), iter.word_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->cond_node(node);
	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::or_word: {
              OrNode* node = MethodIterator::factory->new_OrNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), iter.word_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->cond_node(node);
	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::whileTrue_word:
              // Ignore since they are inside WhileNode expression body
              break;
      case Bytecodes::whileFalse_word:
              // Ignore since they are inside WhileNode expression body
              break;
      case Bytecodes::jump_else_word:
              should_never_encounter(Bytecodes::jump_else_word);
              break;
      case Bytecodes::jump_loop_word: {
              WhileNode* node = MethodIterator::factory->new_WhileNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), 
		iter.word_at(1 + oopSize), iter.word_at(1));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
	      blk->while_node(node);
	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::interpreted_send_0:		// fall through
      case Bytecodes::interpreted_send_1:		// fall through
      case Bytecodes::interpreted_send_2:		// fall through
      case Bytecodes::interpreted_send_n:		// fall through
      case Bytecodes::compiled_send_0:			// fall through
      case Bytecodes::compiled_send_1:			// fall through
      case Bytecodes::compiled_send_2:			// fall through
      case Bytecodes::compiled_send_n:			// fall through
      case Bytecodes::primitive_send_0:			// fall through
      case Bytecodes::primitive_send_1:			// fall through
      case Bytecodes::primitive_send_2:			// fall through
      case Bytecodes::primitive_send_n:			// fall through
      case Bytecodes::polymorphic_send_0:		// fall through
      case Bytecodes::polymorphic_send_1:		// fall through
      case Bytecodes::polymorphic_send_2:		// fall through
      case Bytecodes::polymorphic_send_n:		// fall through
      case Bytecodes::megamorphic_send_0: 		// fall through
      case Bytecodes::megamorphic_send_1: 		// fall through
      case Bytecodes::megamorphic_send_2:		// fall through
      case Bytecodes::megamorphic_send_n:		// fall through
              blk->normal_send(iter.ic());
              break;
      case Bytecodes::interpreted_send_0_pop:		// fall through
      case Bytecodes::interpreted_send_1_pop:		// fall through
      case Bytecodes::interpreted_send_2_pop:		// fall through
      case Bytecodes::interpreted_send_n_pop:		// fall through
      case Bytecodes::compiled_send_0_pop: 		// fall through
      case Bytecodes::compiled_send_1_pop: 		// fall through
      case Bytecodes::compiled_send_2_pop:		// fall through
      case Bytecodes::compiled_send_n_pop:		// fall through
      case Bytecodes::primitive_send_0_pop:		// fall through
      case Bytecodes::primitive_send_1_pop:		// fall through
      case Bytecodes::primitive_send_2_pop:		// fall through
      case Bytecodes::primitive_send_n_pop:		// fall through
      case Bytecodes::polymorphic_send_0_pop:		// fall through
      case Bytecodes::polymorphic_send_1_pop:		// fall through
      case Bytecodes::polymorphic_send_2_pop:		// fall through
      case Bytecodes::polymorphic_send_n_pop:		// fall through
      case Bytecodes::megamorphic_send_0_pop: 		// fall through
      case Bytecodes::megamorphic_send_1_pop: 		// fall through
      case Bytecodes::megamorphic_send_2_pop:		// fall through
      case Bytecodes::megamorphic_send_n_pop:		// fall through
              blk->normal_send(iter.ic());
              blk->pop();
              break;
      case Bytecodes::interpreted_send_self:		// fall through
      case Bytecodes::compiled_send_self:		// fall through
      case Bytecodes::primitive_send_self:		// fall through
      case Bytecodes::polymorphic_send_self:		// fall through
      case Bytecodes::megamorphic_send_self:		// fall through
              blk->self_send(iter.ic());
              break;
      case Bytecodes::interpreted_send_self_pop:	// fall through
      case Bytecodes::compiled_send_self_pop:		// fall through
      case Bytecodes::primitive_send_self_pop:		// fall through
      case Bytecodes::polymorphic_send_self_pop:	// fall through
      case Bytecodes::megamorphic_send_self_pop:	// fall through
              blk->self_send(iter.ic());
              blk->pop();
              break;
      case Bytecodes::interpreted_send_super:		// fall through
      case Bytecodes::compiled_send_super:		// fall through
      case Bytecodes::primitive_send_super:		// fall through
      case Bytecodes::polymorphic_send_super:		// fall through
      case Bytecodes::megamorphic_send_super:		// fall through
              blk->super_send(iter.ic());
              break;
      case Bytecodes::interpreted_send_super_pop:	// fall through
      case Bytecodes::compiled_send_super_pop:		// fall through
      case Bytecodes::primitive_send_super_pop:		// fall through
      case Bytecodes::polymorphic_send_super_pop:	// fall through
      case Bytecodes::megamorphic_send_super_pop:	// fall through
              blk->super_send(iter.ic());
              blk->pop();
              break;
      case Bytecodes::return_tos_pop_0:
              blk->method_return(0);
              break;
      case Bytecodes::return_tos_pop_1:
              blk->method_return(1);
              break;
      case Bytecodes::return_tos_pop_2:
              blk->method_return(2);
              break;
      case Bytecodes::return_tos_pop_n:
              blk->method_return(iter.byte_at(1));
              break;
      case Bytecodes::return_self_pop_0:
	      blk->push_self();
              blk->method_return(0);
              break;
      case Bytecodes::return_self_pop_1:
	      blk->push_self();
              blk->method_return(1);
              break;
      case Bytecodes::return_self_pop_2:
              blk->push_self();
              blk->method_return(2);
              break;
      case Bytecodes::return_self_pop_n:
              blk->push_self();
              blk->method_return(iter.byte_at(1));
              break;
      case Bytecodes::return_tos_zap_pop_n:
              blk->zap_scope();
              blk->method_return(iter.byte_at(1));
              break;
      case Bytecodes::return_self_zap_pop_n:
              blk->zap_scope();
              blk->push_self();
              blk->method_return(iter.byte_at(1));
              break;
      case Bytecodes::non_local_return_tos_pop_n:
              blk->nonlocal_return(iter.byte_at(1));
              break;
      case Bytecodes::non_local_return_self_pop_n:
              blk->push_self();
              blk->nonlocal_return(iter.byte_at(1));
              break;
      case Bytecodes::prim_call: // fall through
      case Bytecodes::prim_call_self: {
	      primitive_desc* pdesc = primitives::lookup((fntype) iter.word_at(1));
              PrimitiveCallNode* node = 
                MethodIterator::factory->new_PrimitiveCallNode(_interval->method(),
				      _interval,
                                      iter.bci(),
                                      iter.next_bci(),
                                      pdesc->has_receiver(),
				      NULL,
                                      pdesc);
	      // %hack: this assertion fails
	     // assert(pdesc->has_receiver() == (iter.code() == Bytecodes::prim_call_self), "just checking");
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->primitive_call_node(node);
	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::predict_prim_call:
              blk->predict_prim_call(primitives::lookup((fntype) iter.word_at(1)), -1);
              break;
      case Bytecodes::prim_call_failure: // fall through
      case Bytecodes::prim_call_self_failure: {
	      primitive_desc* pdesc = primitives::lookup((fntype) iter.word_at(1));
              PrimitiveCallNode* node = MethodIterator::factory->new_PrimitiveCallNode(
                  _interval->method(),
                  _interval,
                  iter.bci(), 
		  iter.next_bci(),
		  pdesc->has_receiver(),
                  NULL,
                  pdesc,
                  iter.word_at(1 + oopSize));
	      assert(pdesc->has_receiver() == (iter.code() == Bytecodes::prim_call_self_failure), "just checking");
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->primitive_call_node(node);
              next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::predict_prim_call_failure:
              blk->predict_prim_call(
                primitives::lookup((fntype) iter.word_at(1)),
                iter.next_bci() + iter.word_at(1 + oopSize));
              break;
      case Bytecodes::dll_call_sync: {
              DLLCallNode* node = MethodIterator::factory->new_DLLCallNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), iter.dll_cache());
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->dll_call_node(node);
	      next_bci = node->end_bci();
	      break;
           }
      case Bytecodes::access_send_self:
              blk->self_send(iter.ic());
              break;
      case Bytecodes::unimplemented_bc:
              unknown_code(Bytecodes::unimplemented_bc);
	      break;
      case Bytecodes::prim_call_lookup: // fall through
      case Bytecodes::prim_call_self_lookup: {
              symbolOop name = symbolOop(iter.oop_at(1));
              assert(name->is_symbol(), "name must be symbolOop");
              PrimitiveCallNode* node = 
                MethodIterator::factory->new_PrimitiveCallNode(_interval->method(),
				      _interval, 
                                      iter.bci(),
                                      iter.next_bci(),
                                      iter.code() == Bytecodes::prim_call_self_lookup,
                                      name,
                                      NULL);
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->primitive_call_node(node);
	      next_bci = node->end_bci();
              break;
           }
      case Bytecodes::predict_prim_call_lookup:
              blk->predict_prim_call(primitives::lookup(symbolOop(iter.word_at(1))), -1);
	      break;
      case Bytecodes::prim_call_failure_lookup: // fall through
      case Bytecodes::prim_call_self_failure_lookup: {
              symbolOop name = symbolOop(iter.oop_at(1));
              assert(name->is_symbol(), "name must be symbolOop");
              PrimitiveCallNode* node = MethodIterator::factory->new_PrimitiveCallNode(
                  _interval->method(),
		  _interval, 
                  iter.bci(), 
                  iter.next_bci(),
                  iter.code() == Bytecodes::prim_call_self_failure_lookup, 
                  name,
                  NULL,
                  iter.word_at(1 + oopSize));
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->primitive_call_node(node);
	      next_bci = node->end_bci();
              break;
	   }
      case Bytecodes::predict_prim_call_failure_lookup:
              blk->predict_prim_call(
                primitives::lookup(symbolOop(iter.word_at(1))),
                iter.bci() + iter.word_at(1 + oopSize));
	      break;
      case Bytecodes::dll_call_async: {
              DLLCallNode* node = MethodIterator::factory->new_DLLCallNode(
		_interval->method(), _interval, iter.bci(), iter.next_bci(), iter.dll_cache());
	      assert(node->end_bci() <= _interval->end_bci(), "just checking");
              blk->dll_call_node(node);
	      next_bci = node->end_bci();
	      break;
	   }
      case Bytecodes::unimplemented_c7:
              unknown_code(Bytecodes::unimplemented_c7);
	      break;
      case Bytecodes::access_send_0:
              blk->normal_send(iter.ic());
              break;
      case Bytecodes::unimplemented_cc:
              unknown_code(Bytecodes::unimplemented_cc);
	      break;
      case Bytecodes::unimplemented_dc:
              unknown_code(Bytecodes::unimplemented_dc);
	      break;
      case Bytecodes::special_primitive_send_1_hint:
              // ignore - only meaningfull for the interpreter
	      break;
      case Bytecodes::unimplemented_de:
              unknown_code(Bytecodes::unimplemented_de);
	      break;
      case Bytecodes::unimplemented_df:
              unknown_code(Bytecodes::unimplemented_df);
	      break;
      case Bytecodes::smi_add		: // fall through
      case Bytecodes::smi_sub		: // fall through
      case Bytecodes::smi_mult		: // fall through
      case Bytecodes::smi_div		: // fall through
      case Bytecodes::smi_mod		: // fall through
      case Bytecodes::smi_create_point	: // fall through
      case Bytecodes::smi_equal		: // fall through
      case Bytecodes::smi_not_equal	: // fall through
      case Bytecodes::smi_less		: // fall through
      case Bytecodes::smi_less_equal	: // fall through
      case Bytecodes::smi_greater	: // fall through
      case Bytecodes::smi_greater_equal	: // fall through
      case Bytecodes::objArray_at	: // fall through
      case Bytecodes::objArray_at_put	: // fall through
      case Bytecodes::smi_and		: // fall through
      case Bytecodes::smi_or		: // fall through
      case Bytecodes::smi_xor		: // fall through
      case Bytecodes::smi_shift		:
              blk->normal_send(iter.ic());
              break;
      case Bytecodes::double_equal:
              blk->double_equal();
              break;
      case Bytecodes::double_tilde:
              blk->double_not_equal();
              break;
      case Bytecodes::push_global:
    	      assert(iter.oop_at(1)->is_association(), "must be an association");
    	      blk->push_global(associationOop(iter.oop_at(1))); 
    	      break;
      case Bytecodes::store_global_pop:
              assert(iter.oop_at(1)->is_association(), "must be an association");
    	      blk->store_global(associationOop(iter.oop_at(1)));
    	      blk->pop(); 
              break;
      case Bytecodes::store_global:
    	      assert(iter.oop_at(1)->is_association(), "must be an association");
    	      blk->store_global(associationOop(iter.oop_at(1)));  
    	      break;
      case Bytecodes::push_classVar_name: {
	        symbolOop name = symbolOop(iter.oop_at(1));
		assert(name->is_symbol(), "must be symbol");
		blk->push_classVar_name(name);
	      }
	      break;
      case Bytecodes::store_classVar_pop_name: {
	        symbolOop name = symbolOop(iter.oop_at(1));
		assert(name->is_symbol(), "must be symbol");
		blk->store_classVar_name(name);
		blk->pop();
	      }
              break;
      case Bytecodes::store_classVar_name: {
	        symbolOop name = symbolOop(iter.oop_at(1));
		assert(name->is_symbol(), "must be symbol");
		blk->store_classVar_name(name);
	      }
    	      break;
      case Bytecodes::unimplemented_fa:
              unknown_code(Bytecodes::unimplemented_fa);
	      break;
      case Bytecodes::unimplemented_fb:
              unknown_code(Bytecodes::unimplemented_fb);
	      break;
      case Bytecodes::unimplemented_fc:
              unknown_code(Bytecodes::unimplemented_fc);
	      break;
      case Bytecodes::unimplemented_fd:
              unknown_code(Bytecodes::unimplemented_fd);
	      break;
      case Bytecodes::unimplemented_fe:
              unknown_code(Bytecodes::unimplemented_fe);
	      break;
      case Bytecodes::halt:
              unknown_code(Bytecodes::halt);
	      break;
      default:
              ShouldNotReachHere();
    }
  }
  blk->set_prim_failure(oldFailState);
}

MethodIterator::MethodIterator(methodOop m, MethodClosure* blk, 
			       AbstractMethodIntervalFactory* f) {
  factory = f;
  _interval = factory->new_MethodInterval(m, NULL);
  dispatch(blk);
}

MethodIterator::MethodIterator(MethodInterval* interval, MethodClosure* blk, 
			       AbstractMethodIntervalFactory* f) {
  factory = f;
  _interval = interval;
  dispatch(blk);
}

MethodIntervalFactory MethodIterator::defaultFactory;
AbstractMethodIntervalFactory* MethodIterator::factory;

MethodInterval* MethodIntervalFactory::new_MethodInterval(methodOop method, MethodInterval* parent) {
  return new MethodInterval(method, parent);
}

MethodInterval* MethodIntervalFactory::new_MethodInterval(methodOop method, MethodInterval* parent, 
						          int begin_bci, int end_bci, bool failureBlock) {
  return new MethodInterval(method, parent, begin_bci, end_bci, failureBlock);
}

AndNode* MethodIntervalFactory::new_AndNode(methodOop method, MethodInterval* parent, int begin_bci, 
					    int next_bci, int dest_offset) {
  return new AndNode(method, parent, begin_bci, next_bci, dest_offset);
}

OrNode* MethodIntervalFactory::new_OrNode(methodOop method, MethodInterval* parent, int begin_bci, 
					  int next_bci, int dest_offset) {
  return new OrNode(method, parent, begin_bci, next_bci, dest_offset);
}


WhileNode* MethodIntervalFactory::new_WhileNode(methodOop method, MethodInterval* parent, int begin_bci, 
						int next_bci, int cond_offset, int end_offset) {
  return new WhileNode(method, parent, begin_bci, next_bci, cond_offset, end_offset);
}


IfNode*  MethodIntervalFactory::new_IfNode(methodOop method, MethodInterval* parent, int begin_bci, 
					   int next_bci, bool cond, int else_offset, u_char structure) {
  return new IfNode(method, parent, begin_bci, next_bci, cond, else_offset, structure);
}


PrimitiveCallNode* MethodIntervalFactory::new_PrimitiveCallNode(methodOop method, MethodInterval* parent, 
								int begin_bci, int next_bci, bool has_receiver, 
								symbolOop name, primitive_desc* pdesc) {
  return new PrimitiveCallNode(method, parent, begin_bci, next_bci, has_receiver, name, pdesc);
}


PrimitiveCallNode* MethodIntervalFactory::new_PrimitiveCallNode(methodOop method, MethodInterval* parent, 
								int begin_bci, int next_bci, bool has_receiver, 
								symbolOop name, primitive_desc* pdesc, int end_offset) {
  return new PrimitiveCallNode(method, parent, begin_bci, next_bci, has_receiver, name, pdesc, end_offset);
}


DLLCallNode* MethodIntervalFactory::new_DLLCallNode(methodOop method, MethodInterval* parent, int begin_bci, 
						    int next_bci, InterpretedDLL_Cache* cache) {
  return new DLLCallNode(method, parent, begin_bci, next_bci, cache);
}




