/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.35 $ */
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
# include "incls/_pregMapping.cpp.incl"


// Implementation of PRegMapping
//
// The mapping is done via a simple dictionary implemented via the _pregs array holding
// the keys (the pregs), and the _reg_map & _stack_map arrays holding the values (the
// location indices). Note: A preg may be mapped to a register (via _reg_map & and a
// stack location at the same time). An unused dictionary entry is marked by a NULL
// _pregs entry. An unused map entry is marked by a number < 0.
//
// Invariant: A particular preg exists only once in the _pregs array; it has at least
//            one and at most two locations (a register and a stack location).
//
// Note: A future implementation might use little "mapping objects" instead if these
//       three arrays that are used right now. Likely to speed up the implementation
//       (at() and at_put() are not cheap compared to indirection ->. See class Entry).


int PRegMapping::index(PReg* preg) {
  assert(preg != NULL, "no preg specified");
  // try cashed index first
  int i = preg->_map_index_cash;
  assert(0 <= i, "_map_index_cash must always be > 0");
  if (i < size() && _pregs->at(i) == preg) return i;
  // otherwise search for it
  i = size();
  while (i-- > 0 && _pregs->at(i) != preg) ;
  // (-1 <= i < size() && (i >= 0 => _pregs->at(i) == preg)
  // if found, set cash for next time
  if (i >= 0) preg->_map_index_cash = i;
  return i;
}


void PRegMapping::set_entry(int i, PReg* preg, int rloc, int sloc) {
  assert(preg != NULL, "no preg specified");
  assert(!_locs->isLocation(rloc) || _locs->isRegister(rloc), "should be a register location if at all");
  assert(!_locs->isLocation(sloc) || _locs->isArgument(sloc) || _locs->isStackTmp(sloc), "should be a stack location if at all");
  assert(_locs->isLocation(rloc) || _locs->isLocation(sloc), "at least one location expected");
  _pregs->at_put(i, preg);
  _regLocs->at_put(i, rloc);
  _stkLocs->at_put(i, sloc);
  // set cash for next time
  preg->_map_index_cash = i;
}


int PRegMapping::freeSlot() {
  // search for an unused slot
  int i = size();
  while (i-- > 0 && used(i)) ;
  // (-1 <= i < size()) && (i >= 0 => !used(i))
  if (i < 0) {
    // no free slot => grow arrays
    _pregs->append(NULL);
    _regLocs->append(-1);
    _stkLocs->append(-1);
    i = size() - 1;
  }
  assert(!used(i), "just checkin'...");
  return i;
}


int PRegMapping::spillablePRegIndex() {
  // Finds a PReg that is mapped to a register location and that is not locked.
  // Returns a value < 0 if there's no such PReg.
  int uses = 999999999;
  int i0 = -1;
  int i = size();
  while (i-- > 0) {
    PReg* preg = _pregs->at(i);
    int rloc = regLoc(i);
    if (preg != NULL && _locs->isRegister(rloc) && !PRegLocker::locks(preg) &&
        _locs->nofUses(rloc) < uses) {
      uses = _locs->nofUses(rloc);
      i0 = i;
    }
  }
  return i0;
}


void PRegMapping::ensureOneFreeRegister() {
  if (!_locs->freeRegisters()) {
    // no free registers available => find a register to spill
    int i = spillablePRegIndex();
    if (i < 0) fatal("too many temporaries or locked pregs: out of spillable registers");
    // std->print("WARNING: Register spilling - check if this works\n");
    spillRegister(regLoc(i));
    assert(_locs->freeRegisters(), "at least one register should be available now");
    verify();
  }
}


void PRegMapping::spillRegister(int loc) {
  assert(_locs->isRegister(loc), "must be a register");
  int spillLoc = _locs->allocateStackTmp();
  // remap pregs
  int i = size();
  while (i-- > 0) {
    if (used(i) && regLoc(i) == loc) {
      _locs->release(loc);
      _regLocs->at_put(i, _locs->noLocation);
      if (!hasStkLoc(i)) {
        _locs->use(spillLoc);
	_stkLocs->at_put(i, spillLoc);
      }
    }
  }
  _locs->release(spillLoc);
  assert(_locs->nofUses(loc) == 0, "location should not be used anymore");
  // generate spill code
  Register reg = _locs->locationAsRegister(loc);
  _assm->movl(_locs->locationAsAddress(spillLoc), reg);
  verify();
}


int PRegMapping::allocateTemporary(Register hint) {
  ensureOneFreeRegister();
  int regLoc = _locs->noLocation;
  if (hint != noreg) {
    // try to use hint register
    int hintLoc = _locs->registerAsLocation(hint);
    if (_locs->nofUses(hintLoc) == 0) {
      // hintLoc is available
      _locs->allocate(hintLoc);
      regLoc = hintLoc;
    }
  }
  if (!_locs->isLocation(regLoc)) regLoc = _locs->allocateRegister();
  assert(_locs->isLocation(regLoc) && _locs->nofUses(regLoc) == 1, "should be allocated exactly once");
  _tmpLocs->push(regLoc);
  return regLoc;
}


void PRegMapping::releaseTemporary(int regLoc) {
  if (_tmpLocs->pop() == regLoc) {
    _locs->release(regLoc);
  } else {
    // temporary not freed in stack-fashioned manner
    // (can only happen if there's more than one temporary per function)
    Unimplemented();
  }
}


void PRegMapping::releaseAllTemporaries() {
  while (_tmpLocs->nonEmpty()) _locs->release(_tmpLocs->pop());
}


void PRegMapping::destroy() {
  _pregs   = NULL;
}


PRegMapping::PRegMapping(MacroAssembler* assm, int nofArgs, int nofRegs, int nofTemps) {
  const int initialSize = 8;
  _assm    = assm;
  _NLRinProgress = false;
  _locs    = new Locations(nofArgs, nofRegs, nofTemps);
  _pregs   = new GrowableArray<PReg*>(initialSize);
  _regLocs = new GrowableArray<int>(initialSize);
  _stkLocs = new GrowableArray<int>(initialSize);
  _tmpLocs = new GrowableArray<int>(2);
  verify();
}


PRegMapping::PRegMapping(PRegMapping* m) {
  _assm    = m->_assm;
  _NLRinProgress = m->_NLRinProgress;
  _locs    = new Locations(m->_locs);
  _pregs   = m->_pregs->copy();
  _regLocs = m->_regLocs->copy();
  _stkLocs = m->_stkLocs->copy();
  _tmpLocs = m->_tmpLocs->copy();
  releaseAllTemporaries();
  verify();
}


bool PRegMapping::isInjective() {
  int i = size();
  while (i-- > 0) {
    if (used(i)) {
      if (hasRegLoc(i) && _locs->nofUses(regLoc(i)) > 1) return false;
      if (hasStkLoc(i) && _locs->nofUses(stkLoc(i)) > 1) return false;
    }
  }
  return true;
}


bool PRegMapping::isConformant(PRegMapping* with) {
  // checks conformity on the intersection of this and with
  if (NLRinProgress() != with->NLRinProgress()) return false;
  int j = with->size();
  while (j-- > 0) {
    if (with->used(j)) {
      int i = index(with->_pregs->at(j));
      if (i >= 0) {
        if (regLoc(i) != with->regLoc(j) || stkLoc(i) != with->stkLoc(j)) return false;
      }
    }
  }
  return true;
}


void PRegMapping::mapToArgument(PReg* preg, int argNo) {
  assert(index(preg) < 0, "preg for argument defined twice");
  int loc = _locs->argumentAsLocation(argNo);
  _locs->use(loc);
  set_entry(freeSlot(), preg, _locs->noLocation, loc);
  verify();
}


void PRegMapping::mapToTemporary(PReg* preg, int tempNo) {
  assert(index(preg) < 0, "preg for argument defined twice");
  int loc = _locs->temporaryAsLocation(tempNo);
  _locs->allocate(loc);
  set_entry(freeSlot(), preg, _locs->noLocation, loc);
  verify();
}


void PRegMapping::mapToRegister(PReg* preg, Register reg) {
  assert(index(preg) < 0, "preg for register defined twice");
  int loc = _locs->registerAsLocation(reg);
  _locs->allocate(loc);
  set_entry(freeSlot(), preg, loc, _locs->noLocation);
  verify();
}


void PRegMapping::kill(PReg* preg) {
  int i = index(preg);
  if (i >= 0) {
    if (PrintPRegMapping) {
      std->print("kill ");
      print(i);
    }
    int rloc = regLoc(i);
    int sloc = stkLoc(i);
    if (_locs->isLocation(rloc)) _locs->release(rloc);
    if (_locs->isLocation(sloc)) _locs->release(sloc);
    _pregs->at_put(i, NULL);
    verify();
  }
}


void PRegMapping::killAll(PReg* exception) {
  int i = size();
  while (i-- > 0) {
    if (used(i) && _pregs->at(i) != exception) {
      assert(!PRegLocker::locks(_pregs->at(i)), "PReg is locked");
      if (PrintPRegMapping) {
        std->print("kill ");
        print(i);
      }
      int rloc = regLoc(i);
      int sloc = stkLoc(i);
      if (_locs->isLocation(rloc)) _locs->release(rloc);
      if (_locs->isLocation(sloc)) _locs->release(sloc);
      _pregs->at_put(i, NULL);
    }
  }
  verify();
}


void PRegMapping::killDeadsAt(Node* node, PReg* exception) {
  while (node->isTrivial() || node->isMergeNode()) node = node->next();
  // In case of a ReturnNode resultPR & scope context are needed
  // -> maybe use a better solution?
  //
  // if (node->isReturnNode()) {
  //   // end of method, only result needed
  //   killAll(node->scope()->resultPR);
  // }
  assert(node->id() >= 0, "should not be a comment");
  int i = size();
  while (i-- > 0) {
    PReg* preg = _pregs->at(i);
    if (preg != NULL && preg != exception && (!preg->isLiveAt(node) || preg->isConstPReg())) kill(preg);
  }
}


void PRegMapping::cleanupContextReferences() {
  int i = size();
  while (i-- > 0) {
    PReg* preg = _pregs->at(i);
    if (preg != NULL && preg->loc.isContextLocation()) {
      // refers to a context temporary -> kill it
      kill(preg);
    }
  }
}


// Note: Right now, if there's a hint register given, def() and use() make sure that the
// preg will be mapped to the hint register; and hint must be unallocated. This is not the
// same behaviour as in Temporary, where the hint register is only used if it is actually
// available. Furthermore, this behaviour seems inconsistent somehow, because if the preg
// is already in the hint register it will be used even though it is not actually available
// (kind of academic subtlety).

Register PRegMapping::def(PReg* preg, Register hint) {
//  assert(!preg->isSAPReg() || index(preg) < 0, "SAPRegs can be defined only once");
  int i = index(preg);
  assert(i < 0 || !hasStkLoc(i) || !_locs->isArgument(stkLoc(i)), "cannot assign to parameters");
  kill(preg);
  int loc;
  if (hint == noreg) {
    ensureOneFreeRegister();
    loc = _locs->allocateRegister();
    hint = _locs->locationAsRegister(loc);
  } else {
    loc = _locs->registerAsLocation(hint);
    _locs->allocate(loc);
  }
  set_entry(freeSlot(), preg, loc, _locs->noLocation);
  verify();
  return hint;
}


Register PRegMapping::use(PReg* preg, Register hint) {
  int i = index(preg);
  if (i < 0 && preg->loc.isContextLocation()) {
    // preg refers to context temporary
    // determine context temporary address
    PReg* context = theCompiler->contextList->at(preg->loc.contextNo())->context();
    PRegLocker lock(context);
    Address addr = Address(use(context), Mapping::contextOffset(preg->loc.tempNo()));
    // determine a target register
    int loc;
    if (hint == noreg) {
      ensureOneFreeRegister();
      loc = _locs->allocateRegister();
      hint = _locs->locationAsRegister(loc);
    } else {
      loc = _locs->registerAsLocation(hint);
      _locs->allocate(loc);
    }
    _assm->movl(hint, addr);
    set_entry(freeSlot(), preg, loc, _locs->noLocation);
  }
  i = index(preg);
  assert(i >= 0, "preg must have been defined")
  if (hasRegLoc(i)) {
    Register old = _locs->locationAsRegister(regLoc(i));
    if (hint == noreg || hint == old) {
      hint = old;
    } else {
      int loc = _locs->registerAsLocation(hint);
      _locs->release(regLoc(i));
      _locs->allocate(loc);
      _regLocs->at_put(i, loc);
      _assm->movl(hint, old);
    }
  } else {
    // copy into register
    assert(hasStkLoc(i), "must have at least one location");
    int loc;
    if (hint == noreg) {
      ensureOneFreeRegister();
      loc = _locs->allocateRegister();
      hint = _locs->locationAsRegister(loc);
    } else {
      loc = _locs->registerAsLocation(hint);
      _locs->allocate(loc);
    }
    _regLocs->at_put(i, loc);
    _assm->movl(hint, _locs->locationAsAddress(stkLoc(i)));
  }
  verify();
  return hint;
}


Register PRegMapping::use(PReg* preg) {
  Register reg;
  if (preg->isConstPReg() && !isDefined(preg)) {
    reg = def(preg);
    _assm->movl(reg, ((ConstPReg*)preg)->constant);
  } else {
    reg = use(preg, noreg);
  }
  return reg;
}


void PRegMapping::move(PReg* dst, PReg* src) {
  assert(dst->loc != topOfStack, "parameter passing cannot be handled here");
  kill(dst); // remove any previous definition
  int i = index(src);
  assert(i >= 0, "src must be defined");
  int rloc = regLoc(i);
  int sloc = stkLoc(i);
  if (_locs->isLocation(rloc)) _locs->use(rloc);
  if (_locs->isLocation(sloc)) _locs->use(sloc);
  set_entry(freeSlot(), dst, rloc, sloc);
  verify();
}


void PRegMapping::saveRegister(int loc) {
  assert(_locs->isRegister(loc), "must be a register");
  int spillLoc = _locs->allocateStackTmp();
  // remap pregs
  int i = size();
  while (i-- > 0) {
    if (used(i) && regLoc(i) == loc) {
      if (!hasStkLoc(i)) {
        _locs->use(spillLoc);
        _stkLocs->at_put(i, spillLoc);
      }
    }
  }
  _locs->release(spillLoc);
  // generate spill code
  _assm->movl(_locs->locationAsAddress(spillLoc), _locs->locationAsRegister(loc));
  verify();
}


void PRegMapping::saveRegisters(PReg* exception) {
  int i = size();
  while (i-- > 0) {
    if (used(i) && hasRegLoc(i) && !hasStkLoc(i) && _pregs->at(i) != exception) {
      saveRegister(regLoc(i));
    }
  }
  verify();
}


void PRegMapping::killRegisters(PReg* exception) {
  int i = size();
  while (i-- > 0) {
    if (used(i) && hasRegLoc(i) && _pregs->at(i) != exception) {
      // remove that register from mapping & release it
      _locs->release(regLoc(i));
      _regLocs->at_put(i, _locs->noLocation);
      if (!hasStkLoc(i)) {
        // remove entry for preg alltogether
	_pregs->at_put(i, NULL);
      }
    }
  }
  verify();
}


void PRegMapping::killRegister(PReg* preg) {
  int i = index(preg);
  assert(i >= 0, "preg must have been defined")
  if (hasRegLoc(i)) {
    // remove that register from mapping & release it
    _locs->release(regLoc(i));
    _regLocs->at_put(i, _locs->noLocation);
    if (!hasStkLoc(i)) {
      // remove entry for preg alltogether
      _pregs->at_put(i, NULL);
    }
  }
  verify();
}


void PRegMapping::acquireNLRRegisters() {
  guarantee(!NLRinProgress(), "no NLR must be in progress");
  _NLRinProgress = true;
  _locs->allocate(_locs->registerAsLocation(NLR_result_reg));
  _locs->allocate(_locs->registerAsLocation(NLR_home_reg  ));
  _locs->allocate(_locs->registerAsLocation(NLR_homeId_reg));
}


void PRegMapping::releaseNLRRegisters() {
  guarantee(NLRinProgress(), "NLR must be in progress");
  _NLRinProgress = false;
  _locs->release(_locs->registerAsLocation(NLR_result_reg));
  _locs->release(_locs->registerAsLocation(NLR_home_reg  ));
  _locs->release(_locs->registerAsLocation(NLR_homeId_reg));
}


void PRegMapping::makeInjective() {
  // Note: This routine must not generate any code that modifies CPU flags!
  int i = size();
  while (i-- > 0) {
    if (used(i)) {
      int rloc = regLoc(i);
      int sloc = stkLoc(i);
      assert(_locs->isLocation(rloc) || _locs->isLocation(sloc), "must have at least one location");
      if (_locs->isLocation(rloc) && _locs->isLocation(sloc)) {
        if (_locs->nofUses(rloc) > 1 && _locs->nofUses(sloc) > 1) {
	  // preg is mapped to both register and stack location that are shared with other pregs
	  // => map to a new stack location
          int newLoc = _locs->allocateStackTmp();
	  _assm->movl(_locs->locationAsAddress(newLoc), _locs->locationAsRegister(rloc));
	  _locs->release(rloc);
	  _locs->release(sloc);
	  _regLocs->at_put(i, _locs->noLocation);
	  _stkLocs->at_put(i, newLoc);
	} else if (_locs->nofUses(rloc) > 1) {
	  // preg mapped to a register that is shared with other pregs => use stack location only
	  _locs->release(rloc);
	  _regLocs->at_put(i, _locs->noLocation);
	} else if (_locs->nofUses(sloc) > 1) {
          // preg mapped to a stack location that is shared with other pregs => use register location only
	  _locs->release(sloc);
	  _stkLocs->at_put(i, _locs->noLocation);
	}
      } else if (_locs->isLocation(rloc)) {
        if (_locs->nofUses(rloc) > 1) {
	  // preg is mapped to a register that shared with other pregs => map to a new stack location
          int newLoc = _locs->allocateStackTmp();
	  _assm->movl(_locs->locationAsAddress(newLoc), _locs->locationAsRegister(rloc));
	  _locs->release(rloc);
	  _regLocs->at_put(i, _locs->noLocation);
	  _stkLocs->at_put(i, newLoc);
	}
      } else if (_locs->isLocation(sloc)) {
        if (_locs->nofUses(sloc) > 1) {
	  // preg is mapped to a stack location that is shared with other pregs => map to a new stack location
          ensureOneFreeRegister();
	  int tmpLoc = _locs->allocateRegister();
	  int newLoc = _locs->allocateStackTmp();
	  Register t = _locs->locationAsRegister(tmpLoc);
	  _assm->movl(t, _locs->locationAsAddress(sloc));
	  _assm->movl(_locs->locationAsAddress(newLoc), t);
	  _locs->release(tmpLoc);
	  _locs->release(sloc);
	  _regLocs->at_put(i, _locs->noLocation);
	  _stkLocs->at_put(i, newLoc);
        }
      } else {
        ShouldNotReachHere();
      }
    }
  }
  verify();
  assert(isInjective(), "mapping not injective");
}


void PRegMapping::old_makeConformant(PRegMapping* with) {
  // determine which entries have to be adjusted (save values on the stack)
  char* begin_of_code = _assm->pc();
  GrowableArray<int> src(4);
  GrowableArray<int> dst(4);
  int j = with->size();
  while (j-- > 0) {
    if (with->used(j)) {
      int i = index(with->_pregs->at(j));
      if (i >= 0) {
        assert(_pregs->at(i) == with->_pregs->at(j), "should be the same");
        if (regLoc(i) != with->regLoc(j) || stkLoc(i) != with->stkLoc(j)) {
          // push value if necessary
	  // (not necessary if one of the locations is conformant)
          if (hasRegLoc(i) && regLoc(i) == with->regLoc(j)) {
	    // register locations conform => not necessary to save a value
	    if (hasStkLoc(i)) {
	      assert(stkLoc(i) != with->stkLoc(j), "error in program logic");
              _locs->release(stkLoc(i));
	    }
	  } else if (hasStkLoc(i) && stkLoc(i) == with->stkLoc(j)) {
	    // stack locations conform => not necessary to save a value
	    if (hasRegLoc(i)) {
	      assert(regLoc(i) != with->regLoc(j), "error in program logic");
	      _locs->release(regLoc(i));
	    }
	  } else {
	    // none of the locations conform => push value
	    if (hasRegLoc(i)) {
	      _assm->pushl(_locs->locationAsRegister(regLoc(i)));
	    } else {
	      assert(hasStkLoc(i), "must have at least one location");
	      _assm->pushl(_locs->locationAsAddress(stkLoc(i)));
	    }
            // free allocated locations
            if (hasRegLoc(i)) _locs->release(regLoc(i));
            if (hasStkLoc(i)) _locs->release(stkLoc(i));
	  }
	  // mapping differs for this preg => remember entries
	  src.push(i);
	  dst.push(j);
	}
      }
    }
  }

  // pop values from stack and adjust entries
  while (src.nonEmpty()) {
    int i = src.pop();
    int j = dst.pop();
    if (hasRegLoc(i) && regLoc(i) == with->regLoc(j)) {
      // register locations conform => must have non-conformant stack location
      assert(stkLoc(i) != with->stkLoc(j), "error in program logic");
      if (with->hasStkLoc(j)) {
        _assm->movl(with->_locs->locationAsAddress(with->stkLoc(j)), _locs->locationAsRegister(regLoc(i)));
	_locs->allocate(with->stkLoc(j));
      }
    } else if (hasStkLoc(i) && stkLoc(i) == with->stkLoc(j)) {
      // stack locations conform => must have non-conformant register location
      assert(regLoc(i) != with->regLoc(j), "error in program logic");
      if (with->hasRegLoc(j)) {
        _assm->movl(with->_locs->locationAsRegister(with->regLoc(j)), _locs->locationAsAddress(stkLoc(i)));
	_locs->allocate(with->regLoc(j));
      }
    } else {
      // none of the locations conform => pop value
      if (with->hasRegLoc(j)) {
        Register reg = with->_locs->locationAsRegister(with->regLoc(j));
        _assm->popl(reg);
	if (with->hasStkLoc(j)) {
	  // copy register on stack
	  _assm->movl(with->_locs->locationAsAddress(with->stkLoc(j)), reg);
	}
      } else {
        assert(with->hasStkLoc(j), "must have at least one location");
	_assm->popl(with->_locs->locationAsAddress(with->stkLoc(j)));
      }
      // allocate locations
      if (with->hasRegLoc(j)) _locs->allocate(with->regLoc(j));
      if (with->hasStkLoc(j)) _locs->allocate(with->stkLoc(j));
    }
    // adjust mapping
    _regLocs->at_put(i, with->regLoc(j));
    _stkLocs->at_put(i, with->stkLoc(j));
  }
  char* end_of_code = _assm->pc();
  if (PrintMakeConformantCode && begin_of_code < end_of_code) {
    std->print_cr("MakeConformant:");
    Disassembler::decode(begin_of_code, end_of_code);
    std->cr();
  }

  verify();
  assert(isConformant(with), "mapping not conformant");
}


// Helper class to make mappings conformant

class ConformanceHelper: public MapConformance {
 private:
  MacroAssembler* _masm;

 public:
  void generate(MacroAssembler* masm, Variable temp1, Variable temp2);
  void move(Variable src, Variable dst);
  void push(Variable src);
  void pop (Variable dst);
};


void ConformanceHelper::generate(MacroAssembler* masm, Variable temp1, Variable temp2) {
  _masm = masm;
  MapConformance::generate(temp1, temp2);
  _masm = NULL;
};


void ConformanceHelper::move(Variable src, Variable dst) {
  Register src_reg = src.in_register() ? Register(src.register_number(), ' ') : noreg;
  Register dst_reg = dst.in_register() ? Register(dst.register_number(), ' ') : noreg;
  Address  src_adr = src.on_stack() ? Address(ebp, src.stack_offset()) : Address();
  Address  dst_adr = dst.on_stack() ? Address(ebp, dst.stack_offset()) : Address();

  if (src.in_register()) {
    if (dst.in_register()) {
      _masm->movl(dst_reg, src_reg);
    } else if (dst.on_stack()) {
      _masm->movl(dst_adr, src_reg);
    } else {
      ShouldNotReachHere();
    }
  } else if (src.on_stack()) {
    if (dst.in_register()) {
      _masm->movl(dst_reg, src_adr);
    } else {
      ShouldNotReachHere();
    }
  } else {
    ShouldNotReachHere();
  }
}


void ConformanceHelper::push(Variable src) {
  if (src.in_register()) {
    _masm->pushl(Register(src.register_number(), ' '));
  } else if (src.on_stack()) {
    _masm->pushl(Address(ebp, src.stack_offset()));
  } else {
    ShouldNotReachHere();
  }
}


void ConformanceHelper::pop(Variable dst) {
  if (dst.in_register()) {
    _masm->popl(Register(dst.register_number(), ' '));
  } else if (dst.on_stack()) {
    _masm->popl(Address(ebp, dst.stack_offset()));
  } else {
    ShouldNotReachHere();
  }
}


void PRegMapping::new_makeConformant(PRegMapping* with) {
  // set up ConformanceHelper
  bool makeConformant = false;
  Variable unused = Variable::unused();
  ConformanceHelper chelper;
  int j = with->size();
  while (j-- > 0) {
    if (with->used(j)) {
      int i = index(with->_pregs->at(j));
      if (i >= 0) {
        assert(_pregs->at(i) == with->_pregs->at(j), "should be the same");
        if (regLoc(i) != with->regLoc(j) || stkLoc(i) != with->stkLoc(j)) {
	  Variable src_reg =       hasRegLoc(i) ? Variable::new_register(      _locs->locationAsRegisterNo(      regLoc(i))) : unused;
	  Variable src_stk =       hasStkLoc(i) ? Variable::new_stack   (      _locs->locationAsByteOffset(      stkLoc(i))) : unused;
	  Variable dst_reg = with->hasRegLoc(j) ? Variable::new_register(with->_locs->locationAsRegisterNo(with->regLoc(j))) : unused;
	  Variable dst_stk = with->hasStkLoc(j) ? Variable::new_stack   (with->_locs->locationAsByteOffset(with->stkLoc(j))) : unused;
	  chelper.append_mapping(src_reg, src_stk, dst_reg, dst_stk);
	  makeConformant = true;
	}
      }
    }
  }

  if (makeConformant) {
    // mappings differ -> generate code to make them conformant
    // first: find free registers that can be used for "make conformance" code
    int this_mask =       _locs->freeRegisterMask();
    int with_mask = with->_locs->freeRegisterMask();
    int both_mask = this_mask & with_mask;
    Variable temp1 = unused;
    Variable temp2 = unused;
    if (both_mask != 0) {
      // free registers in common
      int i = 0;
      while ((both_mask & (1 << i)) == 0) i++;
      #ifdef ASSERT
        // make sure register can actually be used in both mappings
        { Register reg = Register(i, ' ');
          int this_rloc =       _locs->registerAsLocation(reg);
          int with_rloc = with->_locs->registerAsLocation(reg);
          // allocate & deallocate them - will fail if registers are already in use
	        _locs->allocate(this_rloc);       _locs->release(this_rloc);
          with->_locs->allocate(with_rloc); with->_locs->release(with_rloc);
        }
      #endif
      temp1 = Variable::new_register(i);
      clearNth(both_mask, i);
    }

    if (both_mask != 0) {
      // free registers in common
      int i = 0;
      while ((both_mask & (1 << i)) == 0) i++;
      #ifdef ASSERT
        // make sure register can actually be used in both mappings
        { Register reg = Register(i, ' ');
          int this_rloc =       _locs->registerAsLocation(reg);
          int with_rloc = with->_locs->registerAsLocation(reg);
	  // allocate & deallocate them - will fail if registers are already in use
	        _locs->allocate(this_rloc);       _locs->release(this_rloc);
	  with->_locs->allocate(with_rloc); with->_locs->release(with_rloc);
        }
      #endif
      temp2 = Variable::new_register(i);
      clearNth(both_mask, i);
    }
    guarantee(temp1 != temp2 || temp1 == unused, "should not be the same");

    // make conformant
    char* begin_of_code = _assm->pc();
    chelper.generate(_assm, temp1, temp2);
    char* end_of_code = _assm->pc();
    if (PrintMakeConformantCode) {
      chelper.print();
      std->print_cr("(using R%d & R%d as temporary registers)", temp1.register_number(), temp2.register_number());
      Disassembler::decode(begin_of_code, end_of_code);
      std->cr();
    }

    // Mapping should not be used anymore now, since it is not representing the
    // current situation (one should use the 'with' mapping instead). Destroy it
    // to make sure it is not accidentally used.
    destroy();
  }
}


void PRegMapping::makeConformant(PRegMapping* with) {
  //guarantee(NLRinProgress() == with->NLRinProgress(), "cannot be made conformant");

  if (PrintPRegMapping && WizardMode) {
    std->print("make conformant:\n");
    print();
    std->print("with ");
    with->print();
  }

  _locs->extendTo(with->_locs->nofStackTmps());
  assert(_locs->nofArguments() == with->_locs->nofArguments(), "must be the same");
  assert(_locs->nofRegisters() == with->_locs->nofRegisters(), "must be the same");
  assert(_locs->nofStackTmps() >= with->_locs->nofStackTmps(), "must be greater or equal");
  assert(with->isInjective(), "'with' mapping not injective");
  
  // strip mapping so that it contains the same pregs as 'with' mapping
  int i = size();
  while (i-- > 0) {
    if (used(i)) {
      int j = with->index(_pregs->at(i));
      if (j < 0) {
        // entry not found in 'with' mapping -> remove it from this mapping
	kill(_pregs->at(i));
      }
    }
  }

  if (UseNewMakeConformant) {
    new_makeConformant(with);
  } else {
    old_makeConformant(with);
  }
}


void PRegMapping::iterate(PRegClosure* closure) {
  for (int i = size(); i-- > 0; ) {
    PReg* preg = _pregs->at(i);
    if (preg != NULL) {
      preg->_map_index_cash = i;
      closure->preg_do(preg);
    }
  }
}


Location PRegMapping::locationFor(PReg* preg) {
  int i = index(preg);
  assert(i >= 0, "preg must be defined");
  Location loc = illegalLocation;
  if (hasStkLoc(i)) {
    loc = Location::stackLocation(_locs->locationAsWordOffset(stkLoc(i)));
  } else if (hasRegLoc(i)) {
    loc = Location::registerLocation(_locs->locationAsRegister(regLoc(i)).number());
  } else {
    ShouldNotReachHere();
  }
  return loc;
}


int PRegMapping::nofPRegs() {
  int i = size();
  int n = 0;
  while (i-- > 0) {
    if (used(i)) n++;
  };
  return n;
}


int PRegMapping::maxNofStackTmps() {
  return _locs->nofStackTmps();
}


void PRegMapping::my_print() {
  // This is only here for debugging because print() cannot
  // be called from within the debugger for some strange reason...
  print();
}


void PRegMapping::print(int i) {
  assert(used(i), "unused slot");
  int rloc = regLoc(i);
  int sloc = stkLoc(i);
  std->print("%s -> ", _pregs->at(i)->name());
  if (rloc >= 0) {
    std->print(_locs->locationAsRegister(rloc).name());
  }
  if (sloc >= 0) {
    if (rloc >= 0) std->print(", ");
    int offs = _locs->locationAsByteOffset(sloc);
    std->print("[ebp%s%d]", (offs < 0 ? "" : "+"), offs);
  }
  std->cr();
}


void PRegMapping::print() {
  if (WizardMode) _locs->print();
  if (nofPRegs() > 0) {
    std->print("PReg mapping:\n");
    for (int i = 0; i < size(); i++) {
      if (used(i)) print(i);
    }
  } else {
    std->print("PReg mapping is empty\n");
  }
  std->cr();
  if (_tmpLocs->length() > 0) {
    std->print("Temporaries in use:\n");
    for (int i = 0; i < _tmpLocs->length(); i++) {
      int loc = _tmpLocs->at(i);
      assert(_locs->isRegister(loc), "temporaries must be in registers");
      std->print("temp %d -> %d %s\n", i, loc, _locs->locationAsRegister(loc).name());
    }
    std->cr();
  }
  if (NLRinProgress()) {
    std->print_cr("NLR in progress");
    std->cr();
  }
}


void PRegMapping::verify() {
  if (!CompilerDebug) return;
  _locs->verify();
  int totalUses = 0;
  int i = size();
  while (i-- > 0) {
    if (used(i)) {
      // verify mapping for entry i
      int rloc = regLoc(i);
      int sloc = stkLoc(i);
      if (rloc < 0 && sloc < 0) fatal("no location associated with preg");
      int rlocUses = 0;
      int slocUses = 0;
      int j = size();
      while (j-- > 0) {
        if (used(j)) {
          if (i != j && _pregs->at(i) == _pregs->at(j)) fatal("preg found twice in mapping");
	    if (rloc >= 0 && regLoc(j) == rloc) rlocUses++;
	    if (sloc >= 0 && stkLoc(j) == sloc) slocUses++;
	  }
      }
      // check locations usage counter
      if (rloc >= 0 && _locs->nofUses(rloc) != rlocUses) fatal("inconsistent nofUses (register locations)");
      if (sloc >= 0 && _locs->nofUses(sloc) != slocUses) fatal("inconsistent nofUses (stack locations)");
      // compute total usage
      if (rloc >= 0) totalUses++;
      if (sloc >= 0) totalUses++;
    }
  }
  // check allocated temporaries
  i = _tmpLocs->length();
  while (i-- > 0) {
    int rloc = _tmpLocs->at(i);
    if (_locs->nofUses(rloc) != 1) fatal("inconsistent nofUses (temporaries)");
    totalUses++;
  }
  // check NLR registers if in use
  if (NLRinProgress()) {
    if (_locs->nofUses(_locs->registerAsLocation(NLR_result_reg)) != 1) fatal("inconsistent nofUses (NLR_result_reg)");
    if (_locs->nofUses(_locs->registerAsLocation(NLR_home_reg  )) != 1) fatal("inconsistent nofUses (NLR_home_reg  )");
    if (_locs->nofUses(_locs->registerAsLocation(NLR_homeId_reg)) != 1) fatal("inconsistent nofUses (NLR_homeId_reg)");
    totalUses += 3;
  }
  // check total uses
  if (_locs->nofTotalUses() != totalUses) fatal("inconsistent totalUses");
}


// Implementation of PRegLocker

PRegLocker* PRegLocker::_top;


PRegLocker::PRegLocker(PReg* r0) {
  assert(r0 != NULL, "PReg must be defined");
  lock(r0, NULL, NULL);
}


PRegLocker::PRegLocker(PReg* r0, PReg* r1) {
  assert(r0 != NULL && r1 != NULL, "PRegs must be defined");
  lock(r0, r1, NULL);
}


PRegLocker::PRegLocker(PReg* r0, PReg* r1, PReg* r2) {
  assert(r0 != NULL && r1 != NULL && r2 != NULL, "PRegs must be defined");
  lock(r0, r1, r2);
}


bool PRegLocker::holds(PReg* preg) const {
  assert(preg != NULL, "undefined preg");
  int i = sizeof(_pregs)/sizeof(PReg*);
  while (i-- > 0) {
    if (preg == _pregs[i]) return true;
  }
  return false;
}


bool PRegLocker::locks(PReg* preg) {
  assert(preg != NULL, "undefined preg");
  PRegLocker* p = _top;
  while (p != NULL && !p->holds(preg)) p = p->_prev;
  // p == NULL || p->holds(preg)
  return p != NULL;
}


// Implementation of Temporary

Temporary::Temporary(PRegMapping* mapping, Register hint) {
  _mapping = mapping;
  _regLoc  = mapping->allocateTemporary(hint);
}


Temporary::Temporary(PRegMapping* mapping, PReg* preg) {
  // old code - keep around for time comparison purposes
  const bool old_code = false;
  if (old_code) {
    _mapping = mapping;
    _regLoc  = mapping->allocateTemporary(noreg);
    mapping->assembler()->movl(Temporary::reg(), mapping->use(preg));
    return;
  }

  _mapping = mapping;
  Register reg  = mapping->use(preg);
  // preg is guaranteed to be in a register
  if (mapping->onStack(preg)) {
    // preg is also on stack -> release register location from mapping and use it as copy
    mapping->killRegister(preg);
    _regLoc = mapping->allocateTemporary(reg);
  } else {
    // preg is only in register -> need to allocate a new register & copy it explicitly
    _regLoc = mapping->allocateTemporary(noreg);
    mapping->assembler()->movl(Temporary::reg(), reg);
  }
}


Temporary::~Temporary() {
  _mapping->releaseTemporary(_regLoc);
}

# endif
