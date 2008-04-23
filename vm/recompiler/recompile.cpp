/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.57 $ */
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

#ifdef DELTA_COMPILER

#include "incls/_recompile.cpp.incl"

nmethod* recompilee = NULL;	    	// method being recompiled
Recompilation* theRecompilation;

char* Recompilation::methodOop_invocation_counter_overflow(oop rcvr, methodOop method) {
  // called by the interpreter whenever a method's invocation counter reaches the limit
  // returns continuation pc or NULL if continuing in interpreter
  // Note: For block methods the receiver is the context associated with the
  //       block closure from which the method is coming from.

  // It seems that sometimes the method is screwed up, which causes a crash in consequence.
  // The following tests are for debugging only and should be removed at some point - gri 7/11/96.
  // If the method is illegal, recompilation is simply aborted.
  bool ok = false;
  if (Universe::is_heap((oop*)method)) {
    memOop obj = as_memOop(Universe::object_start((oop*) method));
    if (obj->is_method()) {
      ok = true;
    }
  }
  if (!ok) {
    // Possibly caused by a method sweeper bug: inline cache has been modified during the send.
    // To check: method is a jumpTable entry to an nmethod instead of a methodOop.
    char* msg = oop(method)->is_smi() ? "(method might be jump table entry)" : "";
    LOG_EVENT3("invocation counter overflow with broken methodOop 0x%x (recv = 0x%x) %s", method, rcvr, msg);
    fatal("invocation counter overflow with illegal method - internal error");
    // fatal("invocation counter overflow with illegal method - tell Robert");
    // continuing here is probably catastrophal because the invocation counter
    // increment might have modified the jump table entries anyway.
    return NULL;
  }

  if (UseRecompilation) {
    Recompilation recomp(rcvr, method);
    VMProcess::execute(&recomp);
    if (recomp.recompiledTrigger()) {
      return recomp.result();
    } else {
      return NULL;
    }
  } else {
    method->set_invocation_count(0);
    return NULL;
  }
}


char* Recompilation::nmethod_invocation_counter_overflow(oop rcvr, char* retpc) {
  // called by an nmethod whenever the nmethod's invocation counter reaches its limit
  // retpc is the recompilee's return PC (i.e., the pc of the nmethod which triggered
  // the invocation counter overflow).
  ResourceMark rm;
  nmethod* trigger = findNMethod(retpc);
  LOG_EVENT3("nmethod_invocation_counter_overflow: rcvr = %#x, pc = %#x (nmethod %#x)", rcvr, retpc, trigger);
  char* continuationAddr = trigger->verifiedEntryPoint();   // where to continue
#ifdef ASSERT
  deltaVFrame* vf = DeltaProcess::active()->last_delta_vframe();
  assert(vf->is_compiled_frame() && ((compiledVFrame*)vf)->code() == trigger,
         "stack isn't set up right");
  //DeltaProcess::active()->trace_stack();
#endif
  if (UseRecompilation) {
    Recompilation recomp(rcvr, trigger);
    VMProcess::execute(&recomp);
    if (recomp.recompiledTrigger()) {
      continuationAddr = recomp.result();
    }
  } else {
    //compiler_warning("if UseRecompilation is off, why does nmethod %#x have a counter?", trigger);
    trigger->set_invocation_count(1);
  }
  return continuationAddr;
}


nmethod* compile_method(LookupKey* key, methodOop m) {
  if (UseInliningDatabase && !m->is_blockMethod()) {
    // Find entry in inlining database matching the key.
    // If entry found we use the stored RScope a basis for the compile.
    RScope* rscope = InliningDatabase::lookup_and_remove(key);
    if (rscope) {
      VM_OptimizeRScope op(rscope);
      VMProcess::execute(&op);
      if (TraceInliningDatabase) {
        std->print_cr("Inlining database compile ");
	key->print_on(std);
	std->cr();
      }
      return op.result();
    }
  }
  VM_OptimizeMethod op(key, m);
  VMProcess::execute(&op);
  return op.result();
}


void Recompilation::init() {
  _newNM = NULL; 
  _triggerVF = NULL;
  _recompiledTrigger = false;
  assert(!theRecompilation, "already set");
  theRecompilation = this;
  if (_method->is_blockMethod() && !isCompiled()) {
    // interpreter passes in parent context, not receiver
    // should compute receiver via context (although receiver isn't actually used
    // for block RFrames right now) -- fix this
    if (_rcvr->is_context()) {
      contextOop ctx = (contextOop)_rcvr;
    } else {
      assert(_rcvr == nilObj, "expected nil");
    }
  }
}


void Recompilation::doit() {
  ResourceMark rm;
  if (PrintRecompilation) {
    lprintf("recompilation trigger: %s (%#x)\n", 
	    _method->selector()->as_string(), 
	    isCompiled() ? (char*)_nm : (char*)_method);
  }

  _triggerVF = calling_process()->last_delta_vframe();
  // trigger is the frame that triggered its counter
  // caution: that frame may be in a bad state, so don't access data within it
  RFrame* first;
  if (isCompiled()) {
    first = new CompiledRFrame(_triggerVF->fr());
  } else {
    first = new InterpretedRFrame(_triggerVF->fr(), _method, _rcvr->klass());
  }

  RecompilationPolicy policy(first);
  if (handleStaleInlineCache(first)) {
    // called obsolete method/nmethod -- no need to recompile
  } else {
    Recompilee* r;
    if (_isUncommon) {
      assert(isCompiled(), "must be compiled");
      r = Recompilee::new_Recompilee(first);
    } else {
      r = policy.findRecompilee();
    }
    _recompiledTrigger = r != NULL && r->rframe() == first;
    if (r) {
      recompile(r);
    } 
    
    //slr debugging
  if (false && _nm) {
    std->cr();
    _nm->print_value_on(std);
    std->cr();
    _method->print_value_on(std);
    std->cr();
    std->print_cr("uncommon? %d", _nm->isUncommonRecompiled());
  }
    //slr debugging
  
    if (true || !_recompiledTrigger) {	  // fix this
      // reset the trigger's counter
      if (isCompiled() /*&& !_nm->isUncommonRecompiled()*/) {
	// don't 
	_nm->set_invocation_count(1);
      } else {
	_method->set_invocation_count(1);
      }
    }
  }
  policy.cleanupStaleInlineCaches();
  recompilee = NULL;
}

bool Recompilation::handleStaleInlineCache(RFrame* first) {
  // check if the trigger was an interpreted method that has already been compiled; return true if so
  LookupKey* key = first->key();
  nmethod* nm;
  if (key && (nm = Universe::code->lookup(key)) != NULL) {
    // yes, we already have a compiled method; see if the sending inline cache points to that nmethod
    IC_Iterator* it = first->caller()->fr().current_ic_iterator();
    if (!it) return false;	// no inline cache (perform)
    assert(it->selector() == key->selector(), "selector mismatch");
    while (!it->at_end() && it->klass() != key->klass()) it->advance();
    if (it->at_end()) {
      // NB: this is possible -- inline cache could have been modified after the call, so now the
      // called method is no longer in it
    } else {
      nmethod* target = it->compiled_method();
      assert(!target || target == nm || target->key.equal(&nm->key), "inconsistent target");
      if (!target || target != nm) {
	// yes, the inline cache still calls the interpreted method rather than the compiled one,
	// or calls an obsolete nmethod which has been recompiled
	// replace it with the compiled one; no need to recompile anything now
	if (PrintRecompilation) {
  	  if (it->is_interpreted_ic()) {
            lprintf("replacing nm %#x in InterpretedIC %#x\n", nm, it->interpreted_ic());
  	  } else {
            lprintf("replacing nm %#x in CompiledIC %#x\n", nm, it->compiled_ic());
  	  }
	}

	// Replace the element in the inline cache
 	if (it->is_interpreted_ic()) {
          it->interpreted_ic()->replace(nm);
  	} else {
          it->compiled_ic()->replace(nm);
  	}

	_newNM = nm;
	return true;
      }
    }
  }
  return false;
}


oop Recompilation::receiverOf(deltaVFrame* vf) const {
  return _triggerVF->equal(vf) ? _rcvr : vf->receiver();
}


#ifdef HEAVY_CLEANUP
class CleanupInlineCaches: public ObjectClosure {
  void do_object(memOop obj) {
    if (obj->is_method()) 
      methodOop(obj)->cleanup_inline_caches();
  }
};
#endif


void Recompilation::recompile(Recompilee* r) {
  // recompile r
  recompilee = r->is_compiled() ? r->code() : NULL;	// previous version (if any)
  if (r->rframe()->is_blockMethod()) {
    recompile_block(r);
  } else {
    recompile_method(r);
  }

  if (_newNM == NULL) return;		      // possible -- fix this later

  if (recompilee) {
    // discard old nmethod (*after* compiling newNM)
    recompilee->clear_inline_caches();
  }

  // because compilation uses oldNM's PICs)
  recompilee = NULL;

  // now install _newNM in calling inline cache
  vframe* vf = r->rframe()->top_vframe();
  IC_Iterator* it = vf->fr().sender_ic_iterator();
  if (it) {
    // Replace the element in the inline cache
    if (it->is_interpreted_ic()) {
      InterpretedIC* ic = it->interpreted_ic();
      if (!ic->is_empty()) ic->replace(_newNM);
    } else {
      CompiledIC* ic = it->compiled_ic();
      if (!ic->is_empty()) ic->replace(_newNM);
    }
  } else if (!_newNM->is_method()) {
    // recompiled a block: block call stub has already been updated
  } else {
    // called from C (incl. performs)
  }

  // update lookup caches
  lookupCache::flush(&_newNM->key);
  DeltaCallCache::clearAll();
}


void Recompilation::recompile_method(Recompilee* r) {
  // recompile method r
  LookupKey* key = r->key();
  methodOop m = r->method();
#ifdef ASSERT
  LookupResult res = lookupCache::lookup(key);
  assert(res.method() == m, "mismatched method");
#endif
  _newNM = Universe::code->lookup(key);	      // see if we've already compiled it

  if (_newNM == NULL || _newNM == recompilee) {
    if (recompilee && !recompilee->isZombie()) recompilee->unlink(); // remove it from the code table
    _newNM = compile_method(key, m);
    if (recompilee && !recompilee->isZombie()) recompilee->makeZombie(true);
#ifdef HEAVY_CLEANUP
    static int count;
    if (count++ > 10) {
      count = 0;
      TraceTime t("*cleaning inline caches...", PrintRecompilation2);
      CleanupInlineCaches blk;
      Universe::object_iterate(&blk);
      Universe::code->cleanup_inline_caches();
    }
#endif
  } else {
    recompilee = NULL;			      // no recompilation necessary
  }
}


void Recompilation::recompile_block(Recompilee* r) {
  assert(r->rframe()->is_blockMethod(), "must be block recompilation");
  if (recompilee == NULL) {
    // Urs please fix this.
    // Sometimes recompilee is NULL when this is called
    // Lars, 6-18-96
    compiler_warning("recompilee == NULL when recompile_block is called, possibly internal error");
    //compiler_warning("recompilee == NULL when recompile_block is called -- Urs please look at this");
    _newNM = NULL;
    return;
  }
  assert(recompilee != NULL, "must have block recompilee");

  deltaVFrame* vf = r->rframe()->top_vframe();
  oop block;
  if (recompilee && recompilee->is_block()) {
    deltaVFrame* sender = vf->sender_delta_frame();
    if (sender == NULL) return;		      // pathological case (not sure it can happen)
    GrowableArray<oop>* exprs = sender->expression_stack();
    // primitiveValue takes block as first argument
    int nargs = recompilee->method()->nofArgs();
    block = exprs->at(nargs);
  } else {
    block = receiverOf(vf);
  }
  assert(block->is_block(), "must be a block");
  _newNM = jumpTable::compile_block(blockClosureOop(block));
  if (recompilee != NULL) recompilee->makeZombie(true);	  // do this last (after recompilation) 
}


Recompilee* Recompilee::new_Recompilee(RFrame* rf) {
  if (rf->is_compiled()) {
    return new CompiledRecompilee(rf, ((CompiledRFrame*)rf)->nm());
  } else {
    InterpretedRFrame* irf = (InterpretedRFrame*)rf;
    // Urs, please check this!
    assert(irf->key() != NULL, "must have a key");
    return new InterpretedRecompilee(irf, irf->key(), irf->top_method());
  }
}

LookupKey* CompiledRecompilee::key() const  	{ return &_nm->key; }
methodOop  CompiledRecompilee::method() const	{ return _nm->method(); }

#else

extern "C" void invocation_counter_overflow(oop receiver, methodOop method) {
  fatal("performance bug: shouldn't count invocations");
}

#endif
