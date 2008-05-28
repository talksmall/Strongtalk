/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.20 $ */
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

# include "incls/_location.cpp.incl"

static char
    * specialLocNames[nofSpecialLocations] = { "illegalLocation",
        "unAllocated", "noRegister", "topOfStack", "resultOfNLR",
            "topOfFloatStack" };

// Constructors

void Location::overflow(Mode mode, int f1, int f2, int f3) {
  // should handle field overflow somehow - for now: fatal error
  fatal("Location field overflow - please notify the compiler folks");
}

Location::Location(Mode mode, int f) {
  _loc = (int)mode + (f<<_fPos);
}

Location::Location(Mode mode, int f1, int f2, int f3) {
  if ((f1 & _f1Mask) != f1 || (f2 & _f2Mask) != f2 || (f3 & _f3Mask) != f3)
    overflow(mode, f1, f2, f3);
  _loc = int(mode) + (f1 << _f1Pos) + (f2 << _f2Pos) + (f3 << _f3Pos);
}

char* Location::name() const {
  char* s;
  switch (mode()) {
  case specialLoc: {
    const char* name = specialLocNames[id()];
    s = NEW_RESOURCE_ARRAY(char, strlen(name));
    sprintf(s, name);
    break;
  }
  case registerLoc: {
    const char* name = Mapping::asRegister(*this).name();
    s = NEW_RESOURCE_ARRAY(char, 8);
    sprintf(s, name);
    break;
  }
  case stackLoc: {
    s = NEW_RESOURCE_ARRAY(char, 8);
    sprintf(s, "S%d", offset());
    break;
  }
    break;
  case contextLoc1: {
    s = NEW_RESOURCE_ARRAY(char, 24);
    sprintf(s, "C%d,%d(%d)", contextNo(), tempNo(), scopeID());
  }
    break;
  case contextLoc2: {
    s = NEW_RESOURCE_ARRAY(char, 24);
    sprintf(s, "C%d,%d[%d]", contextNo(), tempNo(), scopeOffs());
  }
    break;
  case floatLoc: {
    s = NEW_RESOURCE_ARRAY(char, 16);
    sprintf(s, "F%d(%d)", floatNo(), scopeNo());
  }
    break;
  default:ShouldNotReachHere()
    ;
    break;
  }
  return s;
}

// predicates

bool Location::isTopOfStack() const {
  return *this == topOfStack || *this == topOfFloatStack;
}

bool Location::isTemporaryRegister() const {
  return isRegisterLocation() && Mapping::isTemporaryRegister(*this);
}

bool Location::isTrashedRegister() const {
  return isRegisterLocation() && Mapping::isTrashedRegister(*this);
}

bool Location::isLocalRegister() const {
  return isRegisterLocation() && Mapping::isLocalRegister(*this);
}

// Implementation of IntFreeList

void IntFreeList::grow() {
  _list->append(_first);
  _first = _list->length() - 1;
}

IntFreeList::IntFreeList(int size) {
  _first = -1;
  _list = new GrowableArray<int>(2);
  assert(_list->length() == 0, "should be zero");
}

int IntFreeList::allocate() {
  if (_first < 0)
    grow();
  int i = _first;
  _first = _list->at(i);
  _list->at_put(i, -1); // for debugging only
  return i;
}

int IntFreeList::allocated() {
  int n = length();
  int i = _first;
  while (i >= 0) {
    i = _list->at(i);
    n--;
  };
  assert(n >= 0, "should be >= 0");
  return n;
}

void IntFreeList::release(int i) {
  assert(_list->at(i) == -1, "should have been allocated before");
  _list->at_put(i, _first);
  _first = i;
}

int IntFreeList::length() {
  return _list->length();
}

void IntFreeList::print() {
  std->print("FreeList 0x%x:", this);
  _list->print();
}

# endif
