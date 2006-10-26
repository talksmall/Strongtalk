/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.33 $ */
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

#include "incls/_rframe.cpp.incl"

const RFrame* noCaller    = (RFrame*) 0x1;		// no caller (i.e., initial frame)
const RFrame* noCallerYet = (RFrame*) 0x0;		// caller not yet computed

RFrame::RFrame(frame fr, const RFrame* callee) : _fr(fr) {
  _caller = (RFrame*)noCallerYet;
  _callee = (RFrame*)callee;
  _invocations = _sends = _cumulSends = _loopDepth = 0;
  _num = callee ? callee->num() + 1 : 0;
  _distance = -1;
}

void RFrame::set_distance(int d) { 
  _distance = d; 
}

InterpretedRFrame::InterpretedRFrame(frame fr, const RFrame* callee) : RFrame(fr, callee) {
  vframe* vf1 = vframe::new_vframe(&_fr);
  assert(vf1->is_interpreted_frame(), "must be interpreted");
  interpretedVFrame* vf = (interpretedVFrame*)vf1;
  _method = vf->method();
  assert(_method->codes() <= _fr.hp() && _fr.hp() < _method->codes_end(),
  	 "frame doesn't match method");
  _bci = vf->bci();
  _receiverKlass = theRecompilation->receiverOf(vf)->klass();
  _vf = vf;
}

InterpretedRFrame::InterpretedRFrame(frame fr, methodOop m, klassOop rcvrKlass) : RFrame(fr, NULL) {
  _method = m;
  assert(_method->codes() <= _fr.hp() && _fr.hp() < _method->codes_end(),
  	 "frame doesn't match method");
  _bci = PrologueBCI;
  _receiverKlass = rcvrKlass;
  vframe* vf1 = vframe::new_vframe(&_fr);
  assert(vf1->is_interpreted_frame(), "must be interpreted");
  interpretedVFrame* vf = (interpretedVFrame*)vf1;
  _vf = vf;
  init();
}

CompiledRFrame::CompiledRFrame(frame fr, const RFrame* callee) : RFrame(fr, callee) {
}

CompiledRFrame::CompiledRFrame(frame fr) : RFrame(fr, NULL) { init(); }


RFrame* RFrame::new_RFrame(frame fr, const RFrame* callee) {
  RFrame* rf;
  int dist = callee ? callee->distance() : -1;
  if (fr.is_interpreted_frame()) {
    rf = new InterpretedRFrame(fr, callee);
    dist++;
  } else if (fr.is_compiled_frame()) { 
    rf = new CompiledRFrame(fr, callee);
  } else {
    fatal("not a Delta frame");
  }
  rf->init();
  rf->set_distance(dist);
  return rf;
}

bool RFrame::is_blockMethod() const { return top_method()->is_blockMethod(); }

RFrame* RFrame::caller() {
  if (_caller != noCallerYet) return (_caller == noCaller) ? NULL : _caller;	// already computed caller
  
  // caller not yet computed; do it now
  if (_fr.is_first_delta_frame()) {
    _caller = (RFrame*)noCaller;
    return NULL;
  } else {
    _caller = new_RFrame(_fr.delta_sender(), this);
    return _caller;
  }
}

methodOop CompiledRFrame::top_method() const {
  return _nm->method();
}

bool RFrame::is_super() const {
  if (is_blockMethod()) return false;
  IC_Iterator* it = _fr.sender_ic_iterator();
  return it ? it->is_super_send() : false;
}

bool RFrame::hasBlockArgs() const {
  deltaVFrame* vf = top_vframe();
  if (!vf) return false;
  int nargs = vf->method()->number_of_arguments();
  for (int i = 0; i < nargs; i++) {
    oop b = vf->argument_at(i);
    if (b->is_block()) return true;
  }
  return false;
}

GrowableArray<blockClosureOop>* RFrame::blockArgs() const {
  deltaVFrame* vf = top_vframe();
  int nargs = top_method()->number_of_arguments();
  GrowableArray<blockClosureOop>* blocks = new GrowableArray<blockClosureOop>(nargs);
  if (!vf) return blocks;
  for (int i = 0; i < nargs; i++) {
    oop b = vf->argument_at(i);
    if (b->is_block()) blocks->append(blockClosureOop(b));
  }
  return blocks;
}

LookupKey* CompiledRFrame::key() const {
  return &_nm->key;
}

LookupKey* InterpretedRFrame::key() const {
  if (_key) return _key;			// cached result
  if (_method->is_blockMethod()) return NULL;	// has no lookup key
  symbolOop sel = _method->selector();
  ((InterpretedRFrame*)this)->_key = LookupKey::allocate(_receiverKlass, sel);
  // Note: this code should really be factored out somewhere; it's duplicated (at least) in lookupCache
  if (is_super()) {
    deltaVFrame* senderVF = _vf ? _vf->sender_delta_frame() : DeltaProcess::active()->last_delta_vframe();
    methodOop sendingMethod = senderVF->method()->home();
    klassOop sendingMethodHolder =_receiverKlass->klass_part()->lookup_method_holder_for(sendingMethod);
    if (sendingMethodHolder) {
      klassOop superKlass = sendingMethodHolder->klass_part()->superKlass();
      assert(_method == lookupCache::method_lookup(superKlass, sel),
	     "inconsistent lookup result");
      _key->initialize(superKlass, sel);
    } else {
      if (WizardMode) warning("sending method holder not found??");
      ((InterpretedRFrame*)this)->_key = NULL;
    }
  } else {
    assert(_method == lookupCache::compile_time_normal_lookup(_receiverKlass, sel),
	   "inconsistent lookup result");
  }
  return _key;
}

int InterpretedRFrame::cost() const {
  return _method->estimated_inline_cost(_receiverKlass);
}

int CompiledRFrame::cost() const {
  return _nm->instsLen();
}

void CompiledRFrame::cleanupStaleInlineCaches() { _nm->cleanup_inline_caches(); }
void InterpretedRFrame::cleanupStaleInlineCaches() { _method->cleanup_inline_caches(); }

int RFrame::computeSends(methodOop m) {
  // how many sends did m cause?  (rough approximation)
  // add up invocation counts of all methods called by m
  int sends = 0;
  CodeIterator iter(m);
  do {
    switch (iter.send()) {
      case Bytecodes::interpreted_send:
      case Bytecodes::compiled_send   :
      case Bytecodes::polymorphic_send:
      case Bytecodes::predicted_send  :
      case Bytecodes::accessor_send   :
        { InterpretedIC* ic = iter.ic();
	  InterpretedIC_Iterator it(ic);
	  while (!it.at_end()) {
	    int count;
	    if (it.is_compiled()) {
              sends += (count = it.compiled_method()->invocation_count());
	    } else {
              sends += (count = it.interpreted_method()->invocation_count());
	    }
	    assert(count >= 0 && count <= 100 * 1000 * 1000, "bad invocation count");
	    it.advance();
	  }
	}
	break;
      case Bytecodes::megamorphic_send:
        // don't know how to count megamorphic sends; for now, just ignore them
	// because compiler can't eliminate them anyway
	break;
      case Bytecodes::primitive_send  : // send to method containing predicted primitive
      case Bytecodes::no_send         :
        break;
      default:
        fatal1("unexpected send type %d", iter.send());
    }
  } while (iter.advance());
  return sends;
}

static CompiledRFrame* this_rframe = NULL;
static int sum_ics_result = 0;
static void sum_ics(CompiledIC* ic) {
  // estimate ic's # of sends and add to sum_ics_result
  // This code is here and not in CompiledIC_Iterator because it contains policy decisions,
  // e.g. how to attribute sends for nmethods w/multiple callers or w/o counters.
  if (ic->is_empty()) return;	  // no sends
  CompiledIC_Iterator it(ic);
  while (!it.at_end()) {
    if (it.is_compiled()) {
      sum_ics_result += it.compiled_method()->invocation_count();
    } else {
      sum_ics_result += it.interpreted_method()->invocation_count();
    }
    it.advance();
  }
}

void CompiledRFrame::init() {
  vframe* vf = vframe::new_vframe(&_fr);
  assert(vf->is_compiled_frame(), "must be compiled");
  _nm = ((compiledVFrame*)vf)->code();
  vf = vf->top();
  assert(vf->is_compiled_frame(), "must be compiled");
  _vf = (deltaVFrame*)vf;  
  _invocations = _nm->invocation_count();
  _ncallers = _nm->number_of_links();
  this_rframe = this;
  sum_ics_result = 0;
  _nm->CompiledICs_do(sum_ics);
  _sends += sum_ics_result;
}

void InterpretedRFrame::init() {
  // find the InlineCache
  if (_bci != PrologueBCI) {
    CodeIterator iter(_method);
    while (bciLT(iter.bci(), _bci)) {
      switch (iter.loopType()) {
        case Bytecodes::loop_start:	_loopDepth++; break;
        case Bytecodes::loop_end  :	_loopDepth--; break;
        case Bytecodes::no_loop   :	break;
        default:			fatal1("unexpected loop type %d", iter.loopType());
      }
      if (!iter.advance()) break;
    }
    assert(iter.bci() == _bci, "should have found exact bci");
  }
  _invocations = _method->invocation_count();
  _ncallers = _method->sharing_count();
  _sends = computeSends(_method);
  _cumulSends = computeCumulSends(_method) + _sends;
  _key = NULL;
}


// a helper class for computing cumulCost
class CumulCounter: public SpecializedMethodClosure {
 public:
  methodOop method;			// the method currently being scanned for uplevel-accesses
  int cumulSends;
  bool top;

  CumulCounter(methodOop m) { cumulSends = 0; method = m; top = true;}

  void count() {
    if (!top) cumulSends += RFrame::computeSends(method);
    top = false;
    MethodIterator iter(method, this);
  }

  void allocate_closure(AllocationType type, int nofArgs, methodOop meth) { // recursively search nested blocks
    methodOop savedMethod = method;
    method = meth;
    count();
    method = savedMethod;
  }
};


int RFrame::computeCumulSends(methodOop m) {
  CumulCounter c(m);
  c.count();
  return c.cumulSends;
}

void RFrame::print(const char* kind) {
  lprintf("%3d %s %-15.15s: inv=%5d/%3d snd=%6d cum=%6d loop=%2d cst=%4d\n", 
  	  _num, is_interpreted() ? "I" : "C", top_method()->selector()->as_string(),  
	  _invocations, _ncallers, _sends, _cumulSends, _loopDepth, cost());
}

void CompiledRFrame::print() {
  RFrame::print("comp");
}

void InterpretedRFrame::print() {
  RFrame::print("int.");
}

#endif
