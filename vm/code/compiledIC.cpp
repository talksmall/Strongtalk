/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.46 $ */
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

# ifdef DELTA_COMPILER
# include "incls/_compiledIC.cpp.incl"


extern "C" void UncommonTrap() {
  ResourceMark rm;
  DeltaProcess::active()->trace_stack();
  error("Uncommon trap has been executed");
  evaluator::read_eval_loop();
}


char* CompiledIC::normalLookupRoutine() {
  return StubRoutines::ic_normal_lookup_entry();
}


char* CompiledIC::superLookupRoutine() {
  return StubRoutines::ic_super_lookup_entry();
}


extern "C" char* icNormalLookup(oop recv, CompiledIC* ic) {
  // As soon as the lookup routine handles 'message not understood' correctly,
  // allocation may take place. Then we have to fix the lookup stub as well.
  // (receiver cannot be saved/restored within the C frame).
  VerifyNoScavenge vna;
  return ic->normalLookup(recv);
}


bool CompiledIC::is_empty() const { 
  char* d = destination(); 
  return d == normalLookupRoutine() || d == superLookupRoutine(); 
}


int CompiledIC::ntargets() const {
  if (is_empty()) return 0;
  PIC* p = pic();
  return p != NULL ? p->number_of_targets() : 1;
}


void CompiledIC::set_call_destination(char* entry_point) {
  // if the IC has a PIC we deallocate the PIC before setting the entry_point
  PIC* p = pic();
  assert(p == NULL || p->entry() != entry_point, "replacing with same address -- shouldn't dealloc");
  if (p != NULL) delete p;
  NativeCall::set_destination(entry_point);
}


extern "C" bool have_nlr_through_C;

oop nmethod_substitute() {
  return Universe::nilObj();
}


char* CompiledIC::normalLookup(oop recv) {
  ResourceMark rm;
  char* entry_point;

  // The assertion below is turned into an if so we can see
  // possible problems in the fast version as well - gri 6/21/96
  //
  // Last problem reported by Steffen (gri 7/24/96): This has been
  // called via a nmethod called from within a megamorphic self send
  // in the interpreter. I've added Interpreter::_last_native_called
  // for better debugging.
  //
  // assert(!Interpreter::contains(begin_addr()), "should be handled in the interpreter");
  if (Interpreter::contains(begin_addr())) {
    std->print_cr("nmethod called from interpreter reports ic miss:");
    std->print_cr("interpreter call at: 0x%x", begin_addr());
    std->print_cr("nmethod entry point: 0x%x", Interpreter::_last_native_called);
    InterpretedIC* ic = as_InterpretedIC(next_instruction_address());
    fatal("please notify VM people");
  }

  if (TraceLookup) {
    std->print("CompiledIC lookup (");
    recv->klass()->print_value();
    std->print(", ");
    selector()->print_value();
    std->print(")");
    std->cr();
  }
  klassOop klass = recv->klass();
  symbolOop sel = selector();
  LookupResult result = lookupCache::ic_normal_lookup(klass, sel);

  if (result.is_empty()) {
    // does not understand
    //
    // The following code is just a copy from the corresponding section
    // for interpreted code - especially the arguments for the message
    // object are not set up yet (0 arguments assumed). This code is
    // only here to prevent the system from crashing completely - in
    // general one should be able to recover using abort in the evaluator.
    //
    // FIX THIS (gri 6/10/96)
    BlockScavenge bs; // make sure that no scavenge happens
    klassOop msgKlass = klassOop(Universe::find_global("Message"));
    oop obj = msgKlass->klass_part()->allocateObject();
    assert(obj->is_mem(), "just checkin'...");
    memOop msg = memOop(obj);
    objArrayOop args = oopFactory::new_objArray(0);
    // for now: assume instance variables are there...
    // later: should check this or use a VM interface:
    // msg->set_receiver(recv);
    // msg->set_selector(ic->selector());
    // msg->set_arguments(args);
    msg->raw_at_put(2, recv);
    msg->raw_at_put(3, selector());
    msg->raw_at_put(4, args);
    symbolOop sel = oopFactory::new_symbol("doesNotUnderstand:");
    if (interpreter_normal_lookup(recv->klass(), sel).is_empty()) {
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
    oop result = Delta::call(recv, sel, msg);

    if (!have_nlr_through_C) {
      Unimplemented();
    }
    // return a substitute nmethod so that stub routine doesn't crash
    return (char*)nmethod_substitute;
  
    /* Old code - keep around till completely fixed

    LookupKey key(klass, sel);

    std->print("Compiled lookup failed for: ");
    key.print_on(std);
    std->cr();

    DeltaProcess::active()->trace_stack();
    warning("Lookup error: DoesNotUnderstand semantics not implemented for compiled code");
    evaluator::read_eval_loop();
    Unimplemented();
    */
  }

  if (isOptimized() && result.is_method()) {
    // If this call site is optimized only nmethods should be called.
    assert(!isMegamorphic(), "megamorphic ICs shouldn't be optimized");
    LookupKey key(klass, sel);
    VM_OptimizeMethod op(&key, result.method());
    VMProcess::execute(&op);
    result = lookupCache::ic_normal_lookup(klass, sel);
    assert(result.is_entry(), "result must contain a jump table entry");
  }

  bool empty = is_empty();
  if (empty) setDirty();
  if (!empty || result.is_method()) {
    PIC* pic = PIC::allocate(this, klass, result);
    if (pic == NULL) {
      // PIC too large & MICs (megamorphic PICs) turned off
      // => start with empty IC again
      assert(!UseMICs, "should return a MIC");
      clear();
      if (result.is_entry()) {
        entry_point =  isReceiverStatic()
                    ?  result.entry()->method()->verifiedEntryPoint()
                    :  result.entry()->destination();
      } else { 
        pic = PIC::allocate(this, klass, result);
        assert(pic != NULL, "pic must be present");
        entry_point = pic->entry();
      }
    } else {
      entry_point = pic->entry();
      if (pic->is_megamorphic()) {
        setMegamorphic(); // even when UseInlineCaching is turned off? (gri 5/24/96) - check this
      }
    }
  } else {
    assert(empty && result.is_entry(), "just checking");

    // result is a jump table entry for an nmethod
    if (TraceLookup2) lprintf("nmethod found, f = 0x%x\n", result.get_nmethod());
    // fetch the destination of the jump table entry to avoid the indirection

    // is the receiver is static we will use the verified entry point
    entry_point =  isReceiverStatic()
                ?  result.entry()->method()->verifiedEntryPoint()
                :  result.entry()->destination();
  }
  assert(isDirty(), "must be dirty now");	// important invariant for type feedback: non-empty IC must be dirty
  if (UseInlineCaching) set_call_destination(entry_point);
  if (TraceLookup2) print();
  LOG_EVENT3("CompiledICLookup (%#x, %#x) --> %#x", klass, sel, entry_point);
  return entry_point;
}


extern "C" char* icSuperLookup(oop recv, CompiledIC* ic) {
  // As soon as the lookup routine handles 'message not understood' correctly,
  // allocation may take place. Then we have to fix the lookup stub as well.
  // (receiver cannot be saved/restored within the C frame).
  VerifyNoScavenge vna;
  return ic->superLookup(recv);
}


extern "C" char* zombie_nmethod(char* return_addr) {
  // Called from zombie nmethods. Determines if called from interpreted
  // or compiled code, does cleanup of the corresponding inline caches
  // and restarts the send.
  //
  // Note: If the nmethod is called from within the interpreter, the
  //       send restart entry point is computed via a 2nd ic_info word
  //       that follows the ordinary ic_info associated with the call.
  //       (the same method as for NLRs used, the target is the send
  //       restart entry point).
  VerifyNoScavenge vna;
  if (Interpreter::contains(return_addr)) {
    // nmethod called from interpreted code
    frame          f  = DeltaProcess::active()->last_frame();
    InterpretedIC* ic = f.current_interpretedIC();
    LOG_EVENT1("zombie nmethod called => interpreted IC 0x%x cleared", ic);
    ic->cleanup();
    // reset instruction pointer => next instruction beeing executed is the same send
    f.set_hp(ic->send_code_addr());
    // restart send entry point for interpreted sends
    return Interpreter::redo_send_entry();
  } else {
    // nmethod called from compiled code
    CompiledIC* ic = CompiledIC_from_return_addr(return_addr);
    LOG_EVENT1("zombie nmethod called => compiled IC 0x%x cleaned up", ic);
    ic->cleanup();
    // restart send entry point is call address
    return ic->begin_addr();
  }
}


klassOop CompiledIC::targetKlass() const {
  nmethod* nm = target();
  if (nm) {
    return nm->key.klass();
  } else {
    Unimplemented(); return NULL;
  }
}


klassOop CompiledIC::sending_method_holder() {
  char* addr = begin_addr();
  nmethod* nm = findNMethod(addr);
  PcDesc* pcdesc = nm->containingPcDesc(addr);
  ScopeDesc* scope = pcdesc->containingDesc(nm);
  return scope->selfKlass()->klass_part()->lookup_method_holder_for(scope->method());
}


char* CompiledIC::superLookup(oop recv) {
  ResourceMark rm;
  char* entry_point;
  assert(!Interpreter::contains(begin_addr()), "should be handled in the interpreter");

  klassOop recv_klass = recv->klass();
  klassOop mhld_klass = sending_method_holder();
  symbolOop       sel = selector();

  if (TraceLookup) {
    std->print("CompiledIC super lookup (");
    recv_klass->print_value();
    std->print(", ");
    mhld_klass->print_value();
    std->print(", ");
    selector()->print_value();
    std->print(")");
    std->cr();
  }

  // The inline cache for super sends looks like the inline cache
  // for normal sends.

  LookupResult result = lookupCache::ic_super_lookup(recv_klass, mhld_klass->klass_part()->superKlass(), sel);
  assert(!result.is_empty(), "lookup cache error");
  if (result.is_method()) {
    // a methodOop
    if (TraceLookup2) lprintf("methodOop found, m = 0x%x\n", result.method());
    // result = (char*)&interpreter_call;
    // if (UseInlineCaching) set_call_destination(result);
    warning("CompiledIC::superLookup didn't find a nmethod - check this");
    Unimplemented();
  } else {
    // result is a jump table entry for an nmethod
    if (TraceLookup2) lprintf("nmethod %#x found\n", result.get_nmethod());
    // fetch the destination of the jump table entry to avoid the indirection
    entry_point = result.entry()->destination();
  }
  if (UseInlineCaching) set_call_destination(entry_point);
  if (TraceLookup2) print();
  LOG_EVENT3("SuperLookup (%#x, %#x) --> %#x", recv_klass, sel, entry_point);
  return entry_point;
}


bool CompiledIC::is_monomorphic() const  {
  if (target() != NULL) return true;
  PIC* p = pic();
  return p != NULL && p->is_monomorphic();
}


bool CompiledIC::is_polymorphic() const {
  PIC* p = pic();
  return p != NULL && p->is_polymorphic();
}


bool CompiledIC::is_megamorphic() const {
  PIC* p = pic();
  return p != NULL && p->is_megamorphic();
}


void CompiledIC::replace(nmethod* nm) {
  assert(selector() == nm->key.selector(), "mismatched selector");
  LOG_EVENT3("compiled IC at 0x%x: new nmethod 0x%x for klass 0x%x replaces old entry", this, nm, nm->key.klass());

  // MONO
  if (is_monomorphic()) {
    if (pic()) { 
      assert(pic()->klasses()->at(0) == nm->key.klass(), "mismatched klass");
    } else {
      // verify the key in the old nmethod matches the new
      assert(findNMethod(destination())->key.equal(&nm->key), "keys must match");
    }
    // set_call_destination deallocates the pic if necessary.
    set_call_destination(nm->entryPoint());
    return;
  }
  // POLY or MEGA
  PIC* p = pic();
  if (p != NULL) {
    PIC* new_pic = p->replace(nm);
    if (new_pic != p) {
      set_call_destination(new_pic->entry());
    }
    return;
  }
  // EMPTY
  ShouldNotReachHere();
}


void CompiledIC::clear() {
  // Fix this when compiler is more flexible
  assert(!isSuperSend() || UseNewBackend, "We cannot yet have super sends in nmethods");
  
  // Clear destination
  set_call_destination(isSuperSend() ? superLookupRoutine() : normalLookupRoutine());

  // Q: Are there any flags to be reset and if so, which ones?
  // A: No, they are "invariant" properties of the IC.  Even the dirty bit should not be
  //    reset, otherwise the compiler may think it was never executed.	  -Urs 7/96
}


void CompiledIC::cleanup() {
  // Convert all entries using the following rules:
  //
  //  nmethod   -> nmethod   (nothing changed) 
  //            or nmethod'  (new nmethod has been compiled)
  //            or methodOop (old nmethod is invalid)
  //
  //  methodOop -> methodOop (nothing changed)
  //            or nmethod   (new nmethod has been compiled)

  // EMPTY
  if (is_empty()) return;

  // MONOMORPHIC
  if (is_monomorphic()) {
    if (pic()) {
      // Must be interpreter send
      PIC_Iterator it(pic());
      assert(it.is_interpreted(), "must be interpreted send in monomorphic case");
      // Since it is impossible to retrieve the sending method for a methodOop
      // we leave the IC unchanged if we're in a super send.
      if (isSuperSend()) return;
      LookupKey key(it.get_klass(), selector());
      LookupResult result = lookupCache::lookup(&key);
      // Nothing to do if lookup result is the same
      if (result.matches(it.interpreted_method())) return;
      // Otherwise update IC depending on lookup result
      if (result.is_empty()) {
        clear();
      } else if (result.is_method()) {
        it.set_methodOop(result.method());
      } else {
        assert(result.is_entry(), "lookup result should be a jump table entry");
        set_call_destination(result.get_nmethod()->entryPoint());
      }
    } else {
      // compiled target
      nmethod* old_nm = findNMethod(destination());
      LookupResult result = lookupCache::lookup(&old_nm->key);
      // Nothing to do if lookup result is the same
      if (result.matches(old_nm)) return;
      // Otherwise update IC depending on lookup result
      if (result.is_empty()) {
	clear();
      } else if (result.is_method()) {
        // don't set to interpreted method -- may be "compiled only" send
	clear();
      } else {
        assert(result.is_entry(), "lookup result should be a jump table entry");
        set_call_destination(result.get_nmethod()->entryPoint());
      }
      /* Old code for compiled target - remove if new version works (gri 7/17/96)

      nmethod* nm = findNMethod(destination());
      LookupResult result = lookupCache::lookup(&nm->key);
      assert(nm->isZombie() || result.is_entry(), "lookup cache is wrong");
      // Nothing to do if lookup result is the same
      if (result.matches(nm)) return;
      assert(nm->isZombie(), "nmethod should be zombie");
      nmethod* newNM = result.get_nmethod();
      if (newNM != NULL) {
	set_call_destination(newNM->entryPoint());
      } else {
	clear();    // don't set to interpreted method -- may be "compiled only" send
      }
      */
    }
    return;
  } 

  // POLYMORPHIC
  PIC* p = pic();
  if (p) {
    nmethod* nm;
    PIC* result = p->cleanup(&nm);
    if (result != p) {
      if (p != NULL) {
        // still polymorphic
        set_call_destination(result->entry());
      } else {
        if (nm) {
          // monomorphic
          set_call_destination(nm->entryPoint());
	} else {
          // anamorphic
          clear();
	}
      }
    }
    return;
  }

  // IMPOSSIBLE STATE
  ShouldNotReachHere();
}


void CompiledIC::print() {
  ResourceMark rm;    // so we can print from debugger
  lprintf("\t((CompiledIC*)%#x) ", this);
  if (is_empty()) {
    lprintf("(empty) ");
  } else {
    lprintf("(filled: %d targets) ", ntargets());
  }
  if (isReceiverStatic()) lprintf("static ");
  if (isDirty()) lprintf("dirty ");
  if (isOptimized()) lprintf("optimized ");
  if (isUninlinable()) lprintf("uninlinable ");
  if (isSuperSend()) lprintf("super ");
  if (isMegamorphic()) lprintf("megamorphic ");
  lprintf("\n");

  lprintf("\t- selector    : ");
  selector()->print_symbol_on();
  lprintf("\n");

  CompiledIC_Iterator it(this);
  while (!it.at_end()) {
    lprintf("\t- klass       : ");
    it.klass()->print_value();
    if (it.is_compiled()) {
      lprintf(";\tnmethod %#x\n", it.compiled_method());
    } else {
      lprintf(";\tmethod %#x\n", it.interpreted_method());
    }
    it.advance();
  }
  
  lprintf("\t- call address: ");
  char* dest = destination();
  if (dest == normalLookupRoutine()) {
    lprintf("normalLookupRoutine\n");
  } else if (dest == superLookupRoutine()) {
    lprintf("superLookupRoutine\n");
  } else {
    // non-empty icache
    lprintf("0x%x\n", destination());
  }

  lprintf("\t- NLR testcode: 0x%x\n", NLR_testcode());
}


InterpretedIC* CompiledIC::inlineCache() const {
  // return interpreter inline cache in corresponding source method
  char* addr = begin_addr();
  nmethod* nm = findNMethod(addr);
  PcDesc* pcdesc = nm->containingPcDesc(addr);
  ScopeDesc* scope = pcdesc->containingDesc(nm);
  CodeIterator iter = CodeIterator(scope->method(), pcdesc->byteCode);
  return iter.ic();
}


symbolOop CompiledIC::selector() const {
  return inlineCache()->selector();
}


nmethod* CompiledIC::target() const {
  char* dest = destination();
  if (Universe::code->contains(dest)) {
    // linked to an nmethod
    nmethod* m = nmethod_from_insts(dest);
    assert(m == findNMethod(dest), "wrong nmethod start");
    return m;
  } else {
    return NULL;
  }
}


klassOop CompiledIC::get_klass(int i) const {
  PIC* p = pic();
  if (p) {
    PIC_Iterator it(p);
    for (int j = 0; j < i; j++) it.advance();
    return it.get_klass();
  } else {
    assert(i == 0, "have max. 1 target method");
    return target()->key.klass();
  }
}


PIC* CompiledIC::pic() const {
  char* dest = destination();
  return PIC::find(dest);
}


LookupKey* CompiledIC::key(int i, bool is_normal_send) const {
  if (is_normal_send) {
    return LookupKey::allocate(get_klass(i), selector());
  } else {
    CompiledIC_Iterator it((CompiledIC*)this);
    it.goto_elem(i);
    return LookupKey::allocate(it.klass(), it.interpreted_method());
  }
}


bool CompiledIC::wasNeverExecuted() const {
  return is_empty() && !isDirty();
}

primitive_desc* PrimitiveIC::primitive() {
  return primitives::lookup((fntype) destination());
}


char* PrimitiveIC::end_addr() {
  primitive_desc* pd = primitive();
  int offset = pd->can_perform_NLR() ? IC_Info::instruction_size : 0;
  return next_instruction_address() + offset;
}


void PrimitiveIC::print() {
  lprintf("\tPrimitive inline cache\n");
  primitive_desc* pd = primitive();
  lprintf("\t- name        : %s\n", pd->name());
  if (pd->can_perform_NLR()) {
    lprintf("\t- NLR testcode: 0x%x\n", NLR_testcode());
  }
}

#else

// Dummy for avoiding link error for system without the DELTA_COMPILER
extern "C" void icLookup(int a, int b) {}
extern "C" void icNormalLookup(int a, int b) {}
extern "C" void icSuperLookup(int a, int b) {}

#endif
