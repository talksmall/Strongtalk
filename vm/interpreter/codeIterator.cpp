/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.42 $ */
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
# include "incls/_codeIterator.cpp.incl"


bool InterpretedDLL_Cache::async() const {
  u_char* p = (u_char*)this;				// p point to first oop in DLL call
  while (Bytecodes::Code(*--p) == Bytecodes::halt) ;	// search back for DLL call bytecode
  Bytecodes::Code code = Bytecodes::Code(*p);
  assert(code == Bytecodes::dll_call_sync || code == Bytecodes::dll_call_async, "not a dll call");
  return code == Bytecodes::dll_call_async;
}


inline void CodeIterator::align() {
  current = align(current);
}


CodeIterator::CodeIterator(methodOop method, int startBCI) {
  assert(PrologueBCI <= startBCI && startBCI <= method->size_of_codes() * oopSize, "startBCI out of range");
  this->method = method;
  set_bci(startBCI);
  end = method->codes_end();
}


CodeIterator::CodeIterator(u_char* hp) {
  method  = methodOopDesc::methodOop_from_hcode(hp);
  current = hp;
  end     = method->codes_end();
}


void CodeIterator::set_bci(int bci) {
  current = method->codes(bci);
}


int CodeIterator::bci() const {
  return (current - method->codes()) + 1;
}


int CodeIterator::next_bci() const {
  return (next_hp() - method->codes()) + 1;
}


u_char* CodeIterator::next_hp() const {
  if (current >= end) return 0;
  switch(format()) {
   case Bytecodes::B:     return current+1;
   case Bytecodes::BB:    return current+2;
   case Bytecodes::BBB:   return current+3;
   case Bytecodes::BBBB:  return current+4;
   case Bytecodes::BBO:   // fall through
   case Bytecodes::BBL:   return align(current+2) + oopSize;
   case Bytecodes::BO:    // fall through
   case Bytecodes::BL:    return align(current+1) + oopSize;
   case Bytecodes::BLB:   return align(current+1) + oopSize + 1;
   case Bytecodes::BOO:   // fall through
   case Bytecodes::BLO:   // fall through
   case Bytecodes::BOL:   // fall through
   case Bytecodes::BLL:   return align(current+1) + oopSize + oopSize;
   case Bytecodes::BBOO:  // fall through
   case Bytecodes::BBLO:  return align(current+2) + oopSize + oopSize;
   case Bytecodes::BOOLB: return align(current+1) + oopSize + oopSize + oopSize + 1;
   case Bytecodes::BBS:   return current + 2 + (current[1] == 0 ? 256 : current[1]);
  }
  ShouldNotReachHere();
  return 0;
}


InterpretedIC* CodeIterator::ic() { 
  switch (format()) {
   case Bytecodes::BOO:   // fall through
   case Bytecodes::BLO:   return (InterpretedIC*) align(current+1);
   case Bytecodes::BBOO:  // fall through
   case Bytecodes::BBLO:  return (InterpretedIC*) align(current+2);    
  }
  return NULL;
}


InterpretedDLL_Cache* CodeIterator::dll_cache() {
  return (InterpretedDLL_Cache*) align(current+1);
}

InterpretedPrim_Cache* CodeIterator::prim_cache() {
  return (InterpretedPrim_Cache*) current;
}

char* CodeIterator::interpreter_return_point(bool restore_value) const {
  // The return is only valid if we are in a send/primtive call/dll call.

  if (is_message_send()) {
    switch(argumentsType()) {
      case Bytecodes::recv_0_args:
      case Bytecodes::recv_1_args:
      case Bytecodes::recv_2_args:
      case Bytecodes::recv_n_args:
        return pop_result()
	     ? (restore_value
	        ? Interpreter::deoptimized_return_from_send_with_receiver_pop_restore()
		: Interpreter::deoptimized_return_from_send_with_receiver_pop())
             : (restore_value
                ? Interpreter::deoptimized_return_from_send_with_receiver_restore()
		: Interpreter::deoptimized_return_from_send_with_receiver());
      case Bytecodes::args_only:
        return pop_result()
	     ? (restore_value
	        ? Interpreter::deoptimized_return_from_send_without_receiver_pop_restore()
		: Interpreter::deoptimized_return_from_send_without_receiver_pop())
             : (restore_value
                ? Interpreter::deoptimized_return_from_send_without_receiver_restore()
		: Interpreter::deoptimized_return_from_send_without_receiver());
      default: ShouldNotReachHere();
    }
  }
  
  if (is_primitive_call()) {
    switch (code()) {
      case Bytecodes::prim_call:
      case Bytecodes::prim_call_lookup:
      case Bytecodes::prim_call_self:
      case Bytecodes::prim_call_self_lookup:
	return restore_value
             ? Interpreter::deoptimized_return_from_primitive_call_without_failure_block_restore()
             : Interpreter::deoptimized_return_from_primitive_call_without_failure_block();
      case Bytecodes::prim_call_failure:
      case Bytecodes::prim_call_failure_lookup:
      case Bytecodes::prim_call_self_failure:
      case Bytecodes::prim_call_self_failure_lookup:
	return  restore_value
             ? Interpreter::deoptimized_return_from_primitive_call_with_failure_block_restore()
             : Interpreter::deoptimized_return_from_primitive_call_with_failure_block();
      default: ShouldNotReachHere();
    }
  }

  if (is_dll_call()) {
    return restore_value
         ? Interpreter::deoptimized_return_from_dll_call_restore()
         : Interpreter::deoptimized_return_from_dll_call();
  }

  ShouldNotReachHere();
  return NULL;
}

oop* CodeIterator::block_method_addr() {
  switch(code()) {
    case Bytecodes::push_new_closure_tos_0:      // fall through
    case Bytecodes::push_new_closure_tos_1:      // fall through
    case Bytecodes::push_new_closure_tos_2:      // fall through
    case Bytecodes::push_new_closure_context_0:  // fall through
    case Bytecodes::push_new_closure_context_1:  // fall through
    case Bytecodes::push_new_closure_context_2:
      return aligned_oop(1);
    case Bytecodes::push_new_closure_tos_n:      // fall through
    case Bytecodes::push_new_closure_context_n:
      return aligned_oop(2);
  }
  return NULL;
}

methodOop CodeIterator::block_method() {
  switch(code()) {
    case Bytecodes::push_new_closure_tos_0:      // fall through
    case Bytecodes::push_new_closure_tos_1:      // fall through
    case Bytecodes::push_new_closure_tos_2:      // fall through
    case Bytecodes::push_new_closure_context_0:  // fall through
    case Bytecodes::push_new_closure_context_1:  // fall through
    case Bytecodes::push_new_closure_context_2:
        return methodOop(oop_at(1));
    case Bytecodes::push_new_closure_tos_n:      // fall through
    case Bytecodes::push_new_closure_context_n:
        return methodOop(oop_at(2));
  }
  return NULL;
}

void CodeIterator::customize_class_var_code(klassOop to_klass) {
  assert(   code() == Bytecodes::push_classVar_name
         || code() == Bytecodes::store_classVar_pop_name
         || code() == Bytecodes::store_classVar_name, "must be class variable byte code");

  oop* p = aligned_oop(1);
  symbolOop name = symbolOop(*p);
  assert(name->is_symbol(), "name must be symbol");
  associationOop assoc = to_klass->klass_part()->lookup_class_var(name);
  if (assoc) {
    assert(assoc->is_old(), "must be tenured");
         if (code() == Bytecodes::push_classVar_name)      set_code(Bytecodes::push_classVar);
    else if (code() == Bytecodes::store_classVar_pop_name) set_code(Bytecodes::store_classVar_pop);
    else                                                   set_code(Bytecodes::store_classVar);
    Universe::store(p, assoc, false);
  }
}

void CodeIterator::uncustomize_class_var_code(klassOop from_klass) {
  assert(   code() == Bytecodes::push_classVar
         || code() == Bytecodes::store_classVar_pop
         || code() == Bytecodes::store_classVar, "must be class variable byte code");

  oop* p = aligned_oop(1);
  associationOop old_assoc = associationOop(*p);
  assert(old_assoc->is_association(), "must be association");
       if (code() == Bytecodes::push_classVar)      set_code(Bytecodes::push_classVar_name);
  else if (code() == Bytecodes::store_classVar_pop) set_code(Bytecodes::store_classVar_pop_name);
  else                                              set_code(Bytecodes::store_classVar_name);
  Universe::store(p, old_assoc->key(), false);
}

void CodeIterator::recustomize_class_var_code(klassOop from_klass, klassOop to_klass) {
  assert(   code() == Bytecodes::push_classVar
         || code() == Bytecodes::store_classVar_pop
         || code() == Bytecodes::store_classVar, "must be class variable byte code");

  oop* p = aligned_oop(1);
  associationOop old_assoc = associationOop(*p);
  assert(old_assoc->is_association(), "must be association");
  associationOop new_assoc = to_klass->klass_part()->lookup_class_var(old_assoc->key());
  if (new_assoc) {
    Universe::store(p, new_assoc, false);
  } else {
         if (code() == Bytecodes::push_classVar)      set_code(Bytecodes::push_classVar_name);
    else if (code() == Bytecodes::store_classVar_pop) set_code(Bytecodes::store_classVar_pop_name);
    else                                              set_code(Bytecodes::store_classVar_name);
    Universe::store(p, old_assoc->key(), false);
  }
}

void CodeIterator::customize_inst_var_code(klassOop to_klass) {
  assert(   code() == Bytecodes::push_instVar_name
         || code() == Bytecodes::store_instVar_pop_name
         || code() == Bytecodes::store_instVar_name
         || code() == Bytecodes::return_instVar_name, "must be instance variable byte code");

  oop* p = aligned_oop(1);
  symbolOop name = symbolOop(*p);
  assert(name->is_symbol(), "name must be symbol");
  int offset = to_klass->klass_part()->lookup_inst_var(name);
  if (offset >= 0) {
         if (code() == Bytecodes::push_instVar_name)      set_code(Bytecodes::push_instVar);
    else if (code() == Bytecodes::store_instVar_pop_name) set_code(Bytecodes::store_instVar_pop);
    else if (code() == Bytecodes::store_instVar_name)     set_code(Bytecodes::store_instVar);
    else                                                  set_code(Bytecodes::return_instVar);
    Universe::store(p, as_smiOop(offset));
  }
}

void CodeIterator::uncustomize_inst_var_code(klassOop from_klass) {
  assert(   code() == Bytecodes::push_instVar
         || code() == Bytecodes::store_instVar_pop
         || code() == Bytecodes::store_instVar
         || code() == Bytecodes::return_instVar, "must be instance variable byte code");

  oop* p = aligned_oop(1);
  assert((*p)->is_smi(), "must be smi");
  int old_offset = smiOop(*p)->value();
  symbolOop name = from_klass->klass_part()->inst_var_name_at(old_offset);
  if (!name) {
    fatal("instance variable not found");
  }
       if (code() == Bytecodes::push_instVar)      set_code(Bytecodes::push_instVar_name);
  else if (code() == Bytecodes::store_instVar_pop) set_code(Bytecodes::store_instVar_pop_name);
  else if (code() == Bytecodes::store_instVar)     set_code(Bytecodes::store_instVar_name);
  else                                             set_code(Bytecodes::return_instVar_name);
  Universe::store(p, name, false);
}

void CodeIterator::recustomize_inst_var_code(klassOop from_klass, klassOop to_klass) {
  assert(   code() == Bytecodes::push_instVar
         || code() == Bytecodes::store_instVar_pop
         || code() == Bytecodes::store_instVar
         || code() == Bytecodes::return_instVar, "must be instance variable byte code");

  oop* p = aligned_oop(1);
  assert((*p)->is_smi(), "must be smi");
  int old_offset = smiOop(*p)->value();
  symbolOop name = from_klass->klass_part()->inst_var_name_at(old_offset);
  if (!name) {
    fatal("instance variable not found");
  }
  int new_offset = to_klass->klass_part()->lookup_inst_var(name);
  if (new_offset >= 0) {
    Universe::store(p, as_smiOop(new_offset));
  } else {
         if (code() == Bytecodes::push_instVar)      set_code(Bytecodes::push_instVar_name);
    else if (code() == Bytecodes::store_instVar_pop) set_code(Bytecodes::store_instVar_pop_name);
    else if (code() == Bytecodes::store_instVar)     set_code(Bytecodes::store_instVar_name);
    else                                             set_code(Bytecodes::return_instVar_name);
    Universe::store(p, name, false);
  }
}

