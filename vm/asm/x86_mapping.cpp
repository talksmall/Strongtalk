/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.30 $ */
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
# include "incls/_x86_mapping.cpp.incl"


// stack mapping

void Mapping::initialize() {
  _localRegisters[0] = asLocation(eax);
  _localRegisters[1] = asLocation(edi);
  _localRegisters[2] = asLocation(esi);
  int i;
  for (i = 0; i < nofRegisters     ; i++) _localRegisterIndex[i] = -1;
  for (i = 0; i < nofLocalRegisters; i++) _localRegisterIndex[_localRegisters[i].number()] = i;
  for (i = 0; i < nofLocalRegisters; i++) {
    Register r = asRegister(_localRegisters[i]);
    assert((r != temp1) && (r != temp2) && (r != temp3), "local registers must be disjoint from temporary registers");
  }
}


// local registers
Location Mapping::_localRegisters[nofLocalRegisters + 1]; // allow for 0 local registers
							  // C++ won't compile array with 0 elements
int Mapping::_localRegisterIndex[nofRegisters + 1];

Location Mapping::localRegister(int i) {
  assert(0 <= i && i < nofLocalRegisters, "illegal local register index");
  return _localRegisters[i];
}


int Mapping::localRegisterIndex(Location l) {
  assert(0 <= l.number() && l.number() < nofRegisters, "illegal local register");
  int res = _localRegisterIndex[l.number()];
  assert(res >= 0, "not a local register");
  assert(localRegister(res) == l, "incorrect mapping");
  return res;
}


// parameter passing
Location Mapping::incomingArg(int i, int nofArgs) {
  assert((0 <= i) && (i < nofArgs), "illegal arg number");
  return Location::stackLocation(nofArgs - i + 1);
}


Location Mapping::outgoingArg(int i, int nofArgs) {
  assert((0 <= i) && (i < nofArgs), "illegal arg number");
  return topOfStack;
}


// stack allocation (Note: offsets are always in oops!)
Location Mapping::localTemporary(int i) {
  assert(i >= 0, "illegal temporary number");
  int floats = theCompiler->totalNofFloatTemporaries();
  int offset = (floats > 0 ? first_float_offset - floats*(floatSize/oopSize) : first_temp_offset) - i;
  return Location::stackLocation(offset);
}


int Mapping::localTemporaryIndex(Location l) {
  int floats = theCompiler->totalNofFloatTemporaries();
  int i = (floats > 0 ? first_float_offset - floats*(floatSize/oopSize) : first_temp_offset) - l.offset();
  assert(localTemporary(i) == l, "incorrect mapping");
  return i;
}


Location Mapping::floatTemporary(int scope_id, int i) {
  InlinedScope* scope = theCompiler->scopes->at(scope_id);
  assert(scope->firstFloatIndex() >= 0, "firstFloatIndex not computed yet");
  // Floats must be 8byte aligned in order to a void massive time penalties.
  // They're accessed via a base register which holds the 8byte aligned value
  // of ebp. The byte-offset of the first float must be a multiple of floatSize
  // => need an extra filler word besides the Floats::magic value.
  //
  // base - 1: Floats::magic
  // base - 2: filler word - undefined
  // base - 3: (global) float 0 hi word
  // base - 4: (global) float 0 lo word
  assert(floatSize == 2*oopSize, "check this code");
  Location loc = Location::stackLocation(first_float_offset - (scope->firstFloatIndex() + i)*(floatSize/oopSize));
  assert((loc.offset() * oopSize) % floatSize == 0, "offset is not correctly aligned");
  return loc;
}


// context temporaries
Location Mapping::contextTemporary(int contextNo, int i, int scope_offset) {
  assert((0 <= contextNo) && (0 <= i), "illegal context or temporary no");
  return Location::compiledContextLocation(contextNo, i, scope_offset);
}


Location* Mapping::new_contextTemporary(int contextNo, int i, int scope_id) {
  assert((0 <= contextNo) && (0 <= i), "illegal context or temporary no");
  return new Location(contextLoc1, contextNo, i, scope_id);
}


int Mapping::contextOffset(int tempNo) {
  // computes the byte offset within the context object
  return tempNo*oopSize + contextOopDesc::temp0_byte_offset();
}


// predicates
bool Mapping::isNormalTemporary(Location loc) {
  assert(!loc.isFloatLocation(), "must have been converted into stackLoc by register allocation");
  return loc.isStackLocation() && !isFloatTemporary(loc);
}


bool Mapping::isFloatTemporary(Location loc) {
  assert(!loc.isFloatLocation(), "must have been converted into stackLoc by register allocation");
  if (!loc.isStackLocation()) return false;
  int floats = theCompiler->totalNofFloatTemporaries();
  int offset = loc.offset();
  return floats > 0 && first_float_offset + 2 >= offset && offset > first_float_offset - floats*(floatSize/oopSize);
}

  
// helper functions for code generation
void Mapping::load(Location src, Register dst) {
  switch (src.mode()) {
    case specialLoc: {
      if (src == resultOfNLR) {
        // treat as NLR_result_reg
	if (NLR_result_reg != dst) theMacroAssm->movl(dst, NLR_result_reg);
      } else {
        ShouldNotReachHere();
      }
      break;
    }
    case registerLoc: {
      Register s = asRegister(src);
      if (s != dst) theMacroAssm->movl(dst, s);
      break;
    }
    case stackLoc: {
      assert(isNormalTemporary(src), "must be a normal temporary location");
      theMacroAssm->Load(ebp, src.offset() * oopSize, dst);
      break;
    }
    case contextLoc1: {
      PReg* base = theCompiler->contextList->at(src.contextNo())->context();
      load(base->loc, dst);
      theMacroAssm->Load(dst, contextOffset(src.tempNo()), dst);
      break;
    }
    default: {
      ShouldNotReachHere();
      break;
    }
  }
}


void Mapping::store(Register src, Location dst, Register temp1, Register temp2, bool needsStoreCheck) {
  assert(src != temp1 && src != temp2 && temp1 != temp2, "registers must be different");
  switch (dst.mode()) {
    case specialLoc: {
      if (dst == topOfStack) {
	theMacroAssm->pushl(src);
      } else {
        ShouldNotReachHere();
      }
      break;
    }
    case registerLoc: {
      Register d = asRegister(dst);
      if (d != src) theMacroAssm->movl(d, src);
      break;
    }
    case stackLoc: {
      assert(isNormalTemporary(dst), "must be a normal temporary location");
      theMacroAssm->Store(src, ebp, dst.offset()*oopSize);
      break;
    }
    case contextLoc1: {
      PReg* base = theCompiler->contextList->at(dst.contextNo())->context();
      load(base->loc, temp1);
      theMacroAssm->Store(src, temp1, contextOffset(dst.tempNo()));
      if (needsStoreCheck) theMacroAssm->store_check(temp1, temp2);
      break;
    }
    default: {
      ShouldNotReachHere();
      break;
    }
  }
}


void Mapping::storeO(oop obj, Location dst, Register temp1, Register temp2, bool needsStoreCheck) {
  assert(temp1 != temp2, "registers must be different");
  switch (dst.mode()) {
    case specialLoc: {
      if (dst == topOfStack) {
	theMacroAssm->pushl(obj);
      } else {
        ShouldNotReachHere();
      }
      break;
    }
    case registerLoc: {
      theMacroAssm->movl(asRegister(dst), obj);
      break;
    }
    case stackLoc: {
      assert(isNormalTemporary(dst), "must be a normal temporary location");
      theMacroAssm->movl(Address(ebp, dst.offset()*oopSize), obj);
      break;
    }
    case contextLoc1: {
      PReg* base = theCompiler->contextList->at(dst.contextNo())->context();
      load(base->loc, temp1);
      theMacroAssm->movl(Address(temp1, contextOffset(dst.tempNo())), obj);
      if (needsStoreCheck) theMacroAssm->store_check(temp1, temp2);
      break;
    }
    default: {
      ShouldNotReachHere();
      break;
    }
  }
}


void Mapping::fload(Location src, Register base) {
  if (src == topOfFloatStack) {
    if (UseFPUStack) {
      // nothing to do, value is on stack already
    } else {
      ShouldNotReachHere();
    }
  } else {
    assert(isFloatTemporary(src), "must be a float location");
    assert((src.offset()*oopSize) % floatSize == 0, "float is not aligned");
    theMacroAssm->fld_d(Address(base, src.offset()*oopSize));
  }
}


void Mapping::fstore(Location dst, Register base) {
  if (dst == topOfFloatStack) {
    if (UseFPUStack) {
      // nothing to do, value is on stack already
    } else {
      ShouldNotReachHere();
    }
  } else {
    assert(isFloatTemporary(dst), "must be a float location");
    assert((dst.offset()*oopSize) % floatSize == 0, "float is not aligned");
    theMacroAssm->fstp_d(Address(base, dst.offset()*oopSize));
  }
}


void mapping_init() {
  Mapping::initialize();
}

# endif
