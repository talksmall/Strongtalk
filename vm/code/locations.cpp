/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.12 $ */
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
# include "incls/_locations.cpp.incl"


// Implementation of Locations
//
// Each entry of _freeList corresponds to a register or stack location. If the location
// is used, the _freeList entry holds the negative reference count for that location. If
// the location is not used, the _freeList entry holds a link (an index) to the next un-
// used location or a sentinel value which indicates the end of the list. There are two
// free lists, one for registers and one for stack locations; the end of a list is indi-
// cated by a sentinel entry.
//
// _freeList->at(i) >= 0: unused, entry is index to next unused entry or sentinel
// _freeList->at(i) <  0: used, entry is negative reference count

Locations::Locations(int nofArgs, int nofRegs, int nofInitialStackTmps) {
  assert(0 <= nofArgs, "illegal number of arguments");
  assert(0 <= nofRegs && nofRegs <= maxNofUsableRegisters, "too many registers required");
  _nofArguments = nofArgs;
  _nofRegisters = nofRegs;
  _freeList     = new GrowableArray<int>(nofArgs + nofRegs + nofInitialStackTmps);
  int i = 0;
  // initialize argument reference counts
  while (i < nofArgs) {
    _freeList->at_put_grow(i, 0);
    i++;
  }
  // initialize register free list
  while (i < nofArgs + nofRegs) {
    _freeList->at_put_grow(i, i-1);
    i++;
  }
  _freeList->at_put(nofArgs, sentinel); // end of list
  _firstFreeRegister = i-1;
  // initialize stackTmps free list
  while (i < nofArgs + nofRegs + nofInitialStackTmps) {
    _freeList->at_put_grow(i, i-1);
    i++;
  }
  _freeList->at_put(nofArgs + nofRegs, sentinel); // end of list
  _firstFreeStackTmp = i-1;
  verify();
}


Locations::Locations(Locations* l) {
  l->verify();
  _nofArguments      = l->_nofArguments;
  _nofRegisters      = l->_nofRegisters;
  _freeList          = l->_freeList->copy();
  _firstFreeRegister = l->_firstFreeRegister;
  _firstFreeStackTmp = l->_firstFreeStackTmp;
  verify();
}


void Locations::extendTo(int nofStackTmps) {
  while (this->nofStackTmps() < nofStackTmps) {
    // grow _freeList
    _freeList->append(_firstFreeStackTmp);
    _firstFreeStackTmp = _freeList->length() - 1;
  }
  verify();
}


int Locations::allocateRegister() {
  int i = _firstFreeRegister;
  if (!isRegister(i)) fatal("out of registers");
  _firstFreeRegister = _freeList->at(i);
  _freeList->at_put(i, -1); // initialize reference count
  verify();
  return i;
}


int Locations::allocateStackTmp() {
  int i = _firstFreeStackTmp;
  if (!isStackTmp(i)) {
    // grow _freeList
    _freeList->append(sentinel);
    assert(_freeList->length() <= sentinel, "sentinel too small");
    i = _freeList->length() - 1;
  }
  _firstFreeStackTmp = _freeList->at(i);
  _freeList->at_put(i, -1); // initialize reference count
  verify();
  return i;
}


void Locations::allocate(int i) {
  assert(isLocation(i), "illegal location");
  assert(nofUses(i) == 0, "already allocated");
  if (isRegister(i)) {
    int j = _firstFreeRegister;
    if (j == i) {
      // remove first entry from free list
      _firstFreeRegister = _freeList->at(i);
    } else {
      // find i in the free list
      while (_freeList->at(j) != i) j = _freeList->at(j);
      _freeList->at_put(j, _freeList->at(i));
    }
    _freeList->at_put(i, -1); // initialize reference count
  } else if (isStackTmp(i)) {
    int j = _firstFreeStackTmp;
    if (j == i) {
      // remove first entry from free list
      _firstFreeStackTmp = _freeList->at(i);
    } else {
      // find i in the free list
      while (_freeList->at(j) != i) j = _freeList->at(j);
      _freeList->at_put(j, _freeList->at(i));
    }
    _freeList->at_put(i, -1); // initialize reference count
  } else {
    ShouldNotReachHere();
  }
  verify();
}


void Locations::use(int i) {
  assert(isLocation(i), "illegal location");
  assert(isArgument(i) || nofUses(i) > 0, "not yet allocated");
  _freeList->at_put(i, _freeList->at(i) - 1); // adjust reference counter
  verify();
}


void Locations::release(int i) {
  assert(isLocation(i), "illegal location");
  assert(nofUses(i) > 0, "not yet allocated");
  _freeList->at_put(i, _freeList->at(i) + 1); // adjust reference counter
  if (_freeList->at(i) == 0) {
    // not used anymore => recycle
    if (isRegister(i)) {
      // register location
      _freeList->at_put(i, _firstFreeRegister);
      _firstFreeRegister = i;
    } else if (isStackTmp(i)) {
      // stack location
      _freeList->at_put(i, _firstFreeStackTmp);
      _firstFreeStackTmp = i;
    }
  }
  verify();
}


int Locations::nofUses(int i) const {
  assert(isLocation(i), "illegal location");
  int n = _freeList->at(i);
  return n < 0 ? -n : 0;
}


int Locations::nofTotalUses() const {
  int totalUses = 0;
  for (int i = locationsBeg(); i < locationsEnd(); i++) totalUses += nofUses(i);
  return totalUses;
}


int Locations::nofFreeRegisters() const {
  int i = _firstFreeRegister;
  int n = 0;
  while (isRegister(i)) {
    i = _freeList->at(i);
    n++;
  }
  return n;
}


int Locations::freeRegisterMask() const {
  int mask = 0;
  for (int i = registersBeg(); i < registersEnd(); i++) {
    if (nofUses(i) == 0) mask |= 1 << locationAsRegister(i).number();
  }
  return mask;
}


int Locations::usedRegisterMask() const {
  int mask = 0;
  for (int i = registersBeg(); i < registersEnd(); i++) {
    if (nofUses(i) > 0) mask |= 1 << locationAsRegister(i).number();
  }
  return mask;
}


int Locations::argumentAsLocation(int argNo) const {
  assert(0 <= argNo && argNo < nofArguments(), "illegal argument no.");
  return argumentsBeg() + argNo;
}


int Locations::registerAsLocation(Register reg) const {
  assert(maxNofUsableRegisters == 6, "inconsistency - adjust this code");
  if (reg == eax) return registersBeg() + 0;
  if (reg == ebx) return registersBeg() + 1;
  if (reg == ecx) return registersBeg() + 2;
  if (reg == edx) return registersBeg() + 3;
  if (reg == edi) return registersBeg() + 4;
  if (reg == esi) return registersBeg() + 5;
  ShouldNotReachHere();
  return 0;
}


int Locations::temporaryAsLocation(int tempNo) const {
  assert(0 <= tempNo, "illegal temporary no.");
  return stackTmpsBeg() + tempNo;
}


Register Locations::locationAsRegister(int loc) const {
  assert(isRegister(loc), "location is not a register");
  switch (loc - registersBeg()) {
    case  0: return eax;
    case  1: return ebx;
    case  2: return ecx;
    case  3: return edx;
    case  4: return edi;
    case  5: return esi;
    default: fatal("inconsistency - adjust this code");
  }
  ShouldNotReachHere();
  return eax;
}


// Stack frame
//
// |    ^   |
// |1st temp| <-- ebp - 1*oopSize (firstTemporaryOffset)
// |ebp save| <-- ebp
// |ret addr|
// |last arg| <-- ebp + 2*oopSize (lastParameterOffset)
// |        |

int Locations::locationAsWordOffset(int loc) const {
  assert(isLocation(loc), "illegal location");
  if (isArgument(loc)) {
    int lastParameterOffset = 2;
    return lastParameterOffset + (nofArguments() - 1 - (loc - argumentsBeg()));
  } else if (isStackTmp(loc)) {
    const int firstTemporaryOffset = -1;
    return firstTemporaryOffset - (loc - stackTmpsBeg());
  }
  ShouldNotReachHere();
  return 0;
}


void Locations::print() {
  int len = _freeList->length();
  int i;
  // print used locations
  std->print("Locations:\n");
  for (i = 0; i < len; i++) {
    if (nofUses(i) > 0) {
      std->print("%d: %d uses\n", i, nofUses(i));
    }
  }
  std->cr();
  // print whole free list
  std->print("Free List:\n");
  std->print("no. of arguments    : %d\n", _nofArguments);
  std->print("no. of registers    : %d\n", _nofRegisters);
  std->print("first free register : %d\n", _firstFreeRegister);
  std->print("first free stack loc: %d\n", _firstFreeStackTmp);
  for (i = 0; i < len; i++) {
    std->print("%d: %d\n", i, _freeList->at(i));
  }
  std->cr();
}


void Locations::verify() {
  if (!CompilerDebug) return;
  int nofFreeRegisters = 0;
  int nofFreeStackTmps = 0;
  int nofUsedLocations = 0;
  int i;
  // verify arguments reference counts
  i = 0;
  while (i < _nofArguments) {
    if (_freeList->at(i) > 0) fatal("bug in argument reference counts");
    i++;
  }
  // verify register free list
  i = _firstFreeRegister;
  while (i != sentinel) {
    if (!isRegister(i)) fatal("bug in registers free list");
    nofFreeRegisters++;
    i = _freeList->at(i);
  }
  if (nofFreeRegisters > _nofRegisters) fatal("too many free registers");
  // verify stack locs free list
  i = _firstFreeStackTmp;
  while (i != sentinel) {
    if (!isStackTmp(i)) fatal("bug in stack locs free list");
    nofFreeStackTmps++;
    i = _freeList->at(i);
  }
  if (nofFreeStackTmps > _freeList->length() - _nofRegisters - _nofArguments) fatal("too many free stack locs");
  // verify used locations
  i = _freeList->length();
  while (i-- > _nofArguments) {
    if (_freeList->at(i) < 0) nofUsedLocations++;
  }
  // verify total number
  if (_nofArguments + nofFreeRegisters + nofFreeStackTmps + nofUsedLocations != _freeList->length()) fatal("locations data structure is leaking");
}


# endif