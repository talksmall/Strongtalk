/* Copyright 1994 - 1996, LongView Technologies, L.L.C. $Revision: 1.34 $ */
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
# include "incls/_interpretedIC.cpp.incl"


// Implementation of Interpreter_PICs
//
// A simple free list manager for interpreted PICs.

class Interpreter_PICs : AllStatic {
public:
  static objArrayOop free_list() { return Universe::pic_free_list(); }

  static objArrayOop allocate(int size) {
    oop first = free_list()->obj_at(size - 1);
    if (first == nilObj) {
      return objArrayKlass::allocate_tenured_pic(size*2);
    }
    free_list()->obj_at_put(size - 1, objArrayOop(first)->obj_at(1));

    objArrayOop result = objArrayOop(first);
    assert(result->is_objArray(),        "must be object array");
    assert(result->is_old(),             "must be tenured");
    assert(result->length() == size * 2, "checking size");
    return result;
  }

  static objArrayOop extend(objArrayOop old_pic) {
    int old_size = old_pic->length()/2;
    if (old_size >= size_of_largest_interpreterPIC) return NULL;
    objArrayOop result = allocate(old_size + 1);
    for (int index = 1; index <= old_size*2; index++) {
      result->obj_at_put(index, old_pic->obj_at(index));
    }
    return result;
  }

  static void deallocate(objArrayOop pic) {
    int entry = (pic->length()/2) - 1;
    oop first = free_list()->obj_at(entry);
    pic->obj_at_put(1, first);
    free_list()->obj_at_put(entry, pic);
  }

  static void set_first(objArrayOop pic, oop first, oop second) {
    pic->obj_at_put(1, first);
    pic->obj_at_put(2, second);
  }

  static void set_second(objArrayOop pic, oop first, oop second) {
    pic->obj_at_put(3, first);
    pic->obj_at_put(4, second);
  }

  static void set_last(objArrayOop pic, oop first, oop second) {
    int size = pic->length();
    pic->obj_at_put(size--, second);
    pic->obj_at_put(size,   first);
  }
};


// Implementation of InterpretedIC

void InterpretedIC::set(Bytecodes::Code send_code, oop first_word, oop second_word) {
  // Remember pics are allocated in new space and
  // require a store check when saved in the InlineCache
  // Note: If the second_word is a PIC, it has to be deallocated
  //       before using set!
  *send_code_addr() = send_code;
  Universe::store(first_word_addr(), first_word, first_word->is_new());
  Universe::store(second_word_addr(), second_word, second_word->is_new());
}


u_char* InterpretedIC::findStartOfSend(u_char* sel_addr) {
  u_char* p = sel_addr;			// start of inline cache
  while (*--p == Bytecodes::halt) ;	// skip alignment bytes if there - this works only if the nofArgs byte != halt ! FIX THIS!
  if (*p < 128) --p;			// skip nofArgs byte if there - this works only for sends with less than 128 args
  // and assumes that no send bytecodes is smaller than 128. FIX THIS!!!
  if (!Bytecodes::is_send_code(Bytecodes::Code(*p))) {
    return NULL;   // not a send
  }
  return p;
  // Clean solution:
  //
  // make all send bytecodes holding the no. of arguments,
  // use special filler value (e.g. restrict no of. args to 254).
  // not urgent (gri 1/29/96)
}


int InterpretedIC::findStartOfSend(methodOop m, int bci) {
  u_char* p = findStartOfSend(m->codes(bci));
  return (p == NULL) ? IllegalBCI : p - m->codes() + 1;
}


symbolOop InterpretedIC::selector() const {
  oop fw = first_word();
  if (fw->is_symbol()) {
    return symbolOop(fw);
  } else if (fw->is_method()) {
    return methodOop(fw)->selector();
  } else {
    jumpTableEntry* e = jump_table_entry();
    nmethod* nm = e->method();
    assert(nm != NULL, "must have an nmethod");
    return nm->key.selector();
  }
}


jumpTableEntry* InterpretedIC::jump_table_entry() const {
  assert(send_type() == Bytecodes::compiled_send ||
    send_type() == Bytecodes::megamorphic_send, "must be a compiled call");
  assert(first_word()->is_smi(), "must be smi");
  return (jumpTableEntry*) first_word();
}


int InterpretedIC::nof_arguments() const {
  u_char* p = send_code_addr();
  switch (Bytecodes::argument_spec(Bytecodes::Code(*p))) {
    case Bytecodes::recv_0_args: return 0;
    case Bytecodes::recv_1_args: return 1;
    case Bytecodes::recv_2_args: return 2;
    case Bytecodes::recv_n_args: {
      int n = selector()->number_of_arguments();
      assert(n = int(*(p+1)), "just checkin'..."); // send_code_addr()+1 must hold the number of arguments
      return n;
                                 }
    case Bytecodes::args_only: return selector()->number_of_arguments();
  }
  ShouldNotReachHere();
  return 0;
}


Bytecodes::SendType InterpretedIC::send_type() const {
  return Bytecodes::send_type(send_code());
}


Bytecodes::ArgumentSpec InterpretedIC::argument_spec() const {
  return Bytecodes::argument_spec(send_code());
}


void InterpretedIC::clear() {
  if (is_empty()) return;
  if (send_type() == Bytecodes::polymorphic_send) {
    // recycle PIC
    assert(second_word()->is_objArray(), "must be a pic");
    Interpreter_PICs::deallocate(objArrayOop(second_word()));
  }
  set(Bytecodes::original_send_code_for(send_code()), oop(selector()), smiOop_zero);
}


void InterpretedIC::replace(LookupResult result, klassOop receiver_klass) {
  // IC entries before modification - used for loging only
  Bytecodes::Code code_before  = send_code();
  oop             word1_before = first_word();
  oop             word2_before = second_word();
  int             transition   = 0;
  // modify IC
  guarantee(word2_before == receiver_klass, "klass should be the same");
  if (result.is_empty()) {
    clear();
    transition = 1;
  } else if (result.is_method()) {
    if (send_type() == Bytecodes::megamorphic_send) {
      set(send_code(), result.method(), receiver_klass);
      transition = 2;
    } else {
      // Please Fix this Robert
      // implement set_monomorphic(klass, method)
      clear();
      transition = 3;
    }
  } else {
    if (send_type() == Bytecodes::megamorphic_send) {
      set(send_code(), oop(result.entry()), receiver_klass);
      transition = 4;
    } else {
      assert(result.is_entry(), "must be jump table entry");
      // a jump table entry of a nmethod is found so let's update the current send
      set(Bytecodes::compiled_send_code_for(send_code()), oop(result.entry()), receiver_klass);
      transition = 5;
    }
  }
  // IC entries after modification - used for loging only
  Bytecodes::Code code_after  = send_code();
  oop             word1_after = first_word();
  oop             word2_after = second_word();
  // log modification
  LOG_EVENT3("InterpretedIC::replace: IC at 0x%x: entry for klass 0x%x replaced (transition %d)", this, receiver_klass, transition);
  LOG_EVENT3("  from (%s, 0x%x, 0x%x)", Bytecodes::name(code_before), word1_before, word2_before);
  LOG_EVENT3("  to   (%s, 0x%x, 0x%x)", Bytecodes::name(code_after ), word1_after , word2_after );
}


void InterpretedIC::cleanup() {
  if (is_empty()) return; // Nothing to cleanup

  switch (send_type()) {
    case Bytecodes::accessor_send:    // fall through
    case Bytecodes::primitive_send:   // fall through
    case Bytecodes::predicted_send:   // fall through
    case Bytecodes::interpreted_send:
      { // check if the interpreted send should be replaced by a compiled send
        klassOop receiver_klass = klassOop(second_word());
        assert(receiver_klass->is_klass(), "receiver klass must be a klass");
        methodOop method = methodOop(first_word());
        assert(method->is_method(), "first word in interpreter IC must be method");
        if (!Bytecodes::is_super_send(send_code())) {
          // super sends cannot be handled since the sending method holder is unknown at this point.
          LookupKey key(receiver_klass, selector());
          LookupResult result = lookupCache::lookup(&key);
          if (!result.matches(method)) {
            replace(result, receiver_klass);
          }
        }
      }
      break;
    case Bytecodes::compiled_send:
      { // check if the current compiled send is valid
        klassOop receiver_klass = klassOop(second_word());
        assert(receiver_klass->is_klass(), "receiver klass must be a klass");
        jumpTableEntry* entry = (jumpTableEntry*) first_word();
        nmethod* nm = entry->method();
        LookupResult result = lookupCache::lookup(&nm->key);
        if (!result.matches(nm)) {
          replace(result, receiver_klass);
        }
      }
      break;
    case Bytecodes::megamorphic_send:
      // Note that with the current definition of is_empty()
      // this will not be called for normal megamorphic sends
      // since they store only the selector.
      { klassOop receiver_klass = klassOop(second_word());
      if (first_word()->is_smi()) {
        jumpTableEntry* entry = (jumpTableEntry*) first_word();
        nmethod* nm = entry->method();
        LookupResult result = lookupCache::lookup(&nm->key);
        if (!result.matches(nm)) {
          replace(result, receiver_klass);
        }
      } else {
        methodOop method = methodOop(first_word());
        assert(method->is_method(), "first word in interpreter IC must be method");
        if (!Bytecodes::is_super_send(send_code())) {
          // super sends cannot be handled since the sending method holder is unknown at this point.
          LookupKey key(receiver_klass, selector());
          LookupResult result = lookupCache::lookup(&key);
          if (!result.matches(method)) {
            replace(result, receiver_klass);
          }
        }
      }
      }
      break;
    case Bytecodes::polymorphic_send:
      {
        // %implementation note:
        //   when cleaning up we can always preserve the old pic since the
        //   the only transitions are:
        //     (compiled    -> compiled)
        //     (compiled    -> interpreted)
        //     (interpreted -> compiled)
        //   in case of a super send we do not have to cleanup because
        //   no nmethods are compiled for super sends.
        if (!Bytecodes::is_super_send(send_code())) {
          objArrayOop pic = pic_array();
          for (int index = pic->length(); index > 0; index -= 2) {
            klassOop klass = klassOop(pic->obj_at(index));
            assert(klass->is_klass(), "receiver klass must be klass");
            oop first = pic->obj_at(index-1);
            if (first->is_smi()) {
              jumpTableEntry* entry = (jumpTableEntry*) first;
              nmethod* nm = entry->method();
              LookupResult result = lookupCache::lookup(&nm->key);
              if (!result.matches(nm)) {
                pic->obj_at_put(index-1, result.value());
              }
            } else {
              methodOop method = methodOop(first);
              assert(method->is_method(), "first word in interpreter IC must be method");
              LookupKey key(klass, selector());
              LookupResult result = lookupCache::lookup(&key);
              if (!result.matches(method)) {
                pic->obj_at_put(index-1, result.value());
              }
            }
          }
        }
      }
  }
}

void InterpretedIC::clear_without_deallocation_pic() {
  if (is_empty()) return;
  set(Bytecodes::original_send_code_for(send_code()), oop(selector()), smiOop_zero);
}


void InterpretedIC::replace(nmethod* nm) {
  // replace entry with nm's klass by nm (if entry exists)
  smiOop entry_point = smiOop(nm->jump_table_entry()->entry_point());
  assert(selector() == nm->key.selector(), "mismatched selector");
  if (is_empty()) return;

  switch (send_type()) {
    case Bytecodes::accessor_send:    // fall through
    case Bytecodes::primitive_send:   // fall through
    case Bytecodes::predicted_send:   // fall through
    case Bytecodes::interpreted_send:
      { // replace the monomorphic interpreted send with compiled send
        klassOop receiver_klass = klassOop(second_word());
        assert(receiver_klass->is_klass(), "receiver klass must be a klass");
        if (receiver_klass == nm->key.klass()) {
          set(Bytecodes::compiled_send_code_for(send_code()), entry_point, nm->key.klass());
        }
      }
      break;
    case Bytecodes::compiled_send:   // fall through
    case Bytecodes::megamorphic_send:
      // replace the monomorphic compiled send with compiled send
      set(send_code(), entry_point, nm->key.klass());
      break;
    case Bytecodes::polymorphic_send:
      { objArrayOop pic = pic_array();
      for (int index = pic->length(); index > 0; index -= 2) {
        klassOop receiver_klass = klassOop(pic->obj_at(index));
        assert(receiver_klass->is_klass(), "receiver klass must be klass");
        if (receiver_klass == nm->key.klass()) {
          pic->obj_at_put(index-1, entry_point);
          return;
        }
      }
      }
      // did not find klass
      break;
    default: fatal("unknown send type");
  }
  LOG_EVENT3("interpreted IC at 0x%x: new nmethod 0x%x for klass 0x%x replaces old entry", this, nm, nm->key.klass());
}


void InterpretedIC::print() {
  std->print("Inline cache (");
  if (is_empty()) {
    std->print("empty");
  } else {
    std->print(Bytecodes::send_type_as_string(send_type()));
  }
  std->print(") ");
  selector()->print_value();
  std->cr();
  InterpretedIC_Iterator it(this);
  while (!it.at_end()) {
    std->print("\t- klass: ");
    it.klass()->print_value();
    if (it.is_interpreted()) {
      std->print(";\tmethod  %#x\n", it.interpreted_method());
    } else {
      std->print(";\tnmethod %#x\n", it.compiled_method());
    }
    it.advance();
  }
}

objArrayOop InterpretedIC::pic_array() {
  assert(send_type() == Bytecodes::polymorphic_send, "Must be a polymorphic send site");
  objArrayOop result = objArrayOop(second_word());
  assert(result->is_objArray(), "interpreter pic must be object array");
  assert(result->length() >= 4, "pic should contain at least two entries");
  return result;
}

void InterpretedIC::update_inline_cache(InterpretedIC* ic, frame* f, Bytecodes::Code send_code, klassOop klass, LookupResult result) {
  // update inline cache
  if (ic->is_empty() && ic->send_type() != Bytecodes::megamorphic_send) {
    // fill ic for the first time
    Bytecodes::Code new_send_code = Bytecodes::halt;
    if (result.is_entry()) {

      methodOop method = result.method();
      if (UseAccessMethods && Bytecodes::has_access_send_code(send_code) && method->is_accessMethod()) {
        // access method found ==> switch to access send
        new_send_code = Bytecodes::access_send_code_for(send_code);
        ic->set(new_send_code, method, klass);

      } else if (UsePredictedMethods && Bytecodes::has_predicted_send_code(send_code) && method->is_special_primitiveMethod()) {
        // predictable method found ==> switch to predicted send
        // NB: ic of predicted send should be empty so that the compiler knows whether another type occurred or not
        // i.e., {predicted + empty} --> 1 class, {predicted + nonempty} --> 2 klasses (polymorphic)
        // but: this actually doesn't work (yet?) since the interpreter fills the ic on any failure (e.g. overflow)
        new_send_code = method->special_primitive_code();
        method = methodOop(ic->selector()); // ic must stay empty
        klass  = NULL;                     // ic must stay empty
        ic->set(new_send_code, method, klass);

      } else {
        // jump table entry found ==> switch to compiled send
        new_send_code = Bytecodes::compiled_send_code_for(send_code);
        ic->set(new_send_code, oop(result.entry()->entry_point()), klass);
      }
    } else {
      // methodOop found
      methodOop method = result.method();

      if (UseAccessMethods && Bytecodes::has_access_send_code(send_code) && method->is_accessMethod()) {
        // access method found ==> switch to access send
        new_send_code = Bytecodes::access_send_code_for(send_code);

      } else if (UsePredictedMethods && Bytecodes::has_predicted_send_code(send_code) && method->is_special_primitiveMethod()) {
        // predictable method found ==> switch to predicted send
        // NB: ic of predicted send should be empty so that the compiler knows whether another type occurred or not
        // i.e., {predicted + empty} --> 1 class, {predicted + nonempty} --> 2 klasses (polymorphic)
        // but: this actually doesn't work (yet?) since the interpreter fills the ic on any failure (e.g. overflow)
        new_send_code = method->special_primitive_code();
        method = methodOop(ic->selector()); // ic must stay empty
        klass  = NULL;                     // ic must stay empty

      } else if (UsePrimitiveMethods && method->is_primitiveMethod()) {
        // primitive method found ==> switch to primitive send
        new_send_code = Bytecodes::primitive_send_code_for(send_code);
        Unimplemented(); // take this out when all primitive send bytecodes implemented

      } else {
        // normal interpreted send ==> do not change
        new_send_code = send_code;
        assert(new_send_code == Bytecodes::original_send_code_for(send_code), "bytecode should not change");
      }
      assert(new_send_code != Bytecodes::halt, "new_send_code not set");
      ic->set(new_send_code, method, klass);
    }
  } else {
    // ic not empty
    switch (ic->send_type()) {
      // monomorphic send
  case Bytecodes::accessor_send   : // fall through
  case Bytecodes::predicted_send  : // fall through
  case Bytecodes::compiled_send   : // fall through
  case Bytecodes::interpreted_send: {
    // switch to polymorphic send with 2 entries
    objArrayOop pic = Interpreter_PICs::allocate(2);
    Interpreter_PICs::set_first(pic, ic->first_word(), ic->second_word());
    Interpreter_PICs::set_second(pic, result.value(), klass);
    ic->set(Bytecodes::polymorphic_send_code_for(send_code), ic->selector(), pic);
    break;
                                    }

                                    // polymorphic send
  case Bytecodes::polymorphic_send: {

    objArrayOop old_pic = ic->pic_array();
    objArrayOop new_pic = Interpreter_PICs::extend(old_pic); // add an entry to the PIC if appropriate
    if (new_pic == NULL) {
      // switch to megamorphic send
      if (Bytecodes::is_super_send(send_code)) {
        ic->set(Bytecodes::megamorphic_send_code_for(send_code), result.value(), klass);
      } else {
        ic->set(Bytecodes::megamorphic_send_code_for(send_code), ic->selector(), NULL);
      }
    } else {
      // still a polymorphic send, add entry and set ic to new_pic
      Interpreter_PICs::set_last(new_pic, result.value(), klass);
      ic->set(send_code, ic->selector(), new_pic);
    }
    // recycle old pic
    Interpreter_PICs::deallocate(old_pic);
    break;
                                    }

                                    // megamorphic send
  case Bytecodes::megamorphic_send: {
    if (Bytecodes::is_super_send(send_code)) {
      ic->set(send_code, result.value(), klass);
    }
    break;
                                    }

  default: ShouldNotReachHere();
    }
  }

  // redo send (reset instruction pointer)
  f->set_hp(ic->send_code_addr());
}


extern "C" bool have_nlr_through_C;

oop InterpretedIC::does_not_understand(oop receiver, InterpretedIC* ic, frame* f) {
  // message not understood...
  BlockScavenge bs; // make sure that no scavenge happens
  klassOop msgKlass = klassOop(Universe::find_global("Message"));
  oop obj = msgKlass->klass_part()->allocateObject();
  assert(obj->is_mem(), "just checkin'...");
  memOop msg = memOop(obj);
  int nofArgs = ic->selector()->number_of_arguments();
  objArrayOop args = oopFactory::new_objArray(nofArgs);
  for (int i = 1; i <= nofArgs; i++) {
    args->obj_at_put(i, f->expr(nofArgs - i));
  }
  // for now: assume instance variables are there...
  // later: should check this or use a VM interface:
  // msg->set_receiver(recv);
  // msg->set_selector(ic->selector());
  // msg->set_arguments(args);
  msg->raw_at_put(2, receiver);
  msg->raw_at_put(3, ic->selector());
  msg->raw_at_put(4, args);
  symbolOop sel = oopFactory::new_symbol("doesNotUnderstand:");
  if (interpreter_normal_lookup(receiver->klass(), sel).is_empty()) {
    // doesNotUnderstand: not found ==> process error
    { ResourceMark rm;
    std->print("LOOKUP ERROR\n");
    sel->print_value(); std->print(" not found\n");
    }
    if (DeltaProcess::active()->is_scheduler()) {
      DeltaProcess::active()->trace_stack();
      fatal("lookup error in scheduler");
    } else {
      DeltaProcess::active()->suspend(::lookup_error);
    }
    ShouldNotReachHere();
  }

  // return marked result of doesNotUnderstand: message
  return Delta::call(receiver, sel, msg);

  // Solution: use an nmethod-like stub-routine called from
  // within a (possibly one-element) PIC (in order to keep
  // the method selector in the IC. That stub does the
  // Delta:call and pops the right number of arguments
  // taken from the selector (the no. of arguments from
  // the selector can be optimized).
}


void InterpretedIC::trace_inline_cache_miss(InterpretedIC* ic, klassOop klass, LookupResult result) {
  std->print("InterpretedIC lookup (");
  klass->print_value();
  std->print(", ");
  ic->selector()->print_value();
  std->print(") --> ");
  result.print_short_on(std);
  std->cr();
}

objArrayOop cacheMissResult(oop result, int argCount) {
  BlockScavenge bs;
  objArrayOop resultHolder = oopFactory::new_objArray(2);
  resultHolder->obj_at_put(1, as_smiOop(argCount));
  resultHolder->obj_at_put(2, result);
  return resultHolder;
}

// The following routine handles all inline cache misses in the interpreter
// by looking at the ic state and the send byte code issuing the send. The
// inline cache is updated and the send bytecode might be backpatched such
// that it corresponds to the inline cache contents.

oop* InterpretedIC::inline_cache_miss() {
  NoGCVerifier noGC;

  // get ic info
  frame           f         = DeltaProcess::active()->last_frame();
  InterpretedIC*  ic        = f.current_interpretedIC();
  Bytecodes::Code send_code = ic->send_code();

  oop receiver = ic->argument_spec() == Bytecodes::args_only // Are we at a self or super send?
    ? f.receiver()                                //  yes: take receiver of frame
    : f.expr(ic->nof_arguments());                //  no:  take receiver pushed before the arguments

  // do the lookup
  klassOop klass = receiver->klass();
  LookupResult result = Bytecodes::is_super_send(send_code)
    ? interpreter_super_lookup(ic->selector())
    : interpreter_normal_lookup(klass, ic->selector());

  // tracing
  if (TraceInlineCacheMiss) {
    std->print("IC miss, ");  trace_inline_cache_miss(ic, klass, result);
  }

  // handle the lookup result
  if (!result.is_empty()) {
    update_inline_cache(ic, &f, ic->send_code(), klass, result);
    return NULL;
  } else {
    return cacheMissResult(does_not_understand(receiver, ic, &f), ic->nof_arguments())->objs(1);
  }
}

// Implementation of InterpretedIC_Iterator

InterpretedIC_Iterator::InterpretedIC_Iterator(InterpretedIC* ic) {
  _ic = ic;
  init_iteration();
}


void InterpretedIC_Iterator::set_klass(oop k) {
  assert(k->is_klass(), "not a klass");
  _klass = klassOop(k);
}


void InterpretedIC_Iterator::set_method(oop m) {
  if (m->is_mem()) {
    assert(m->is_method(), "must be a method");
    _method = (methodOop)m;
    _nm = NULL;
  } else {
    jumpTableEntry* e = (jumpTableEntry*)m;
    _nm = e->method();
    _method = _nm->method();
  }
}


void InterpretedIC_Iterator::init_iteration() {
  _pic = NULL;
  _index = 0;
  // determine initial state
  switch (_ic->send_type()) {
    case Bytecodes::interpreted_send:
      if (_ic->is_empty()) {
        // anamorphic call site (has never been executed => no type information)
        _number_of_targets = 0;
        _info = anamorphic;
      } else {
        // monomorphic call site
        _number_of_targets = 1;
        _info = monomorphic;
        set_klass(_ic->second_word());
        set_method(_ic->first_word());
      }
      break;
    case Bytecodes::compiled_send   :
      _number_of_targets = 1;
      _info = monomorphic;
      set_klass(_ic->second_word());
      assert(_ic->first_word()->is_smi(), "must have jumpTableEntry");
      set_method(_ic->first_word());
      assert(is_compiled(), "bad type");
      break;
    case Bytecodes::accessor_send   : // fall through
    case Bytecodes::primitive_send  :
      _number_of_targets = 1;
      _info = monomorphic;
      set_klass(_ic->second_word());
      set_method(_ic->first_word());
      assert(is_interpreted(), "bad type");
      break;
    case Bytecodes::megamorphic_send:
      // no type information stored
      _number_of_targets = 0;
      _info = megamorphic;
      break;
    case Bytecodes::polymorphic_send:
      // information on many types
      _pic = objArrayOop(_ic->second_word());
      _number_of_targets = _pic->length() / 2;
      _info = polymorphic;
      set_klass(_pic->obj_at(2));
      set_method(_pic->obj_at(1));
      break;
    case Bytecodes::predicted_send:
      if (_ic->is_empty() || _ic->second_word() == smiKlassObj) {
        _number_of_targets = 1;
        _info = monomorphic;
      } else {
        _number_of_targets = 2;
        _info = polymorphic;
      }
      set_klass(smiKlassObj);
      set_method(interpreter_normal_lookup(smiKlassObj, selector()).value());
      assert(_method != NULL && _method->is_mem(), "this method must be there");
      break;
    default: ShouldNotReachHere();
  }
  assert((number_of_targets() > 1) == (_info == polymorphic), "inconsistency");
}


void InterpretedIC_Iterator::advance() {
  assert(!at_end(), "iterated over the end");
  _index++;
  if (! at_end()) {
    if (_pic != NULL) {
      // polymorphic inline cache
      int index = _index + 1; 	// array is 1-origin
      set_klass (_pic->obj_at(2 * index));
      set_method(_pic->obj_at(2 * index - 1));
    } else {
      // predicted send with non_empty inline cache
      assert(_index < 2, "illegal index");
      set_klass(_ic->second_word());
      set_method(_ic->first_word());
    }
  }
}


methodOop InterpretedIC_Iterator::interpreted_method() const {
  if (is_interpreted()) {
    methodOop m = (methodOop)_method;
#ifdef ASSERT
    assert(m->is_old(), "methods must be old");
    m->verify();
#endif
    return m;
  } else {
    return compiled_method()->method();
  }
}


nmethod* InterpretedIC_Iterator::compiled_method() const {
  if (!is_compiled()) return NULL;
#ifdef ASSERT
  if (!_nm->isNMethod()) fatal("not an nmethod"); // cheap test
  // _nm->verify();	// very slow
#endif
  return _nm;
}


bool InterpretedIC_Iterator::is_super_send() const {
  return Bytecodes::is_super_send(_ic->send_code());
}


void InterpretedIC_Iterator::print() {
  std->print_cr("InterpretedIC_Iterator %#x for ic %#x (%s)", this, _ic, selector()->as_string());
}

