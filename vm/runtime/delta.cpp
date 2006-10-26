/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.6 $ */
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
# include "incls/_delta.cpp.incl"

// Implementation of DeltaCallCache

DeltaCallCache* DeltaCallCache::_root = NULL;	// anchor of all DeltaCallCaches


DeltaCallCache::DeltaCallCache() {
  _link = _root;
  _root = this;
  clear();
}


void DeltaCallCache::clear() {
  _key.clear();
  _result.clear();
}


void DeltaCallCache::clearAll() {
  DeltaCallCache* p = _root;
  while (p != NULL) {
    p->clear();
    p = p->_link;
  }
}


// Implementation of Delta

extern "C" oop call_delta(void* method, oop receiver, int nofArgs, oop* args);	// see interpreter_asm.asm

oop Delta::call_generic(DeltaCallCache* ic, oop receiver, oop selector, int nofArgs, oop* args) {
  if (ic->match(receiver->klass(), symbolOop(selector))) {
    // use ic entry - but first make sure it's not a zombie nmethod
    jumpTableEntry* entry = ic->result().entry();
    if (entry != NULL && entry->method()->isZombie()) {
      // is a zombie nmethod => do a new lookup
      LookupResult result = ic->lookup(receiver->klass(), symbolOop(selector));
      if (result.is_empty()) fatal("lookup failure - not treated");
      return call_delta(result.value(), receiver, nofArgs, args);
    }
    return call_delta(ic->result().value(), receiver, nofArgs, args);
  }

  if (!selector->is_symbol())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  LookupResult result = ic->lookup(receiver->klass(), symbolOop(selector));
  if (result.is_empty()) fatal("lookup failure - not treated");
  return call_delta(result.value(), receiver, nofArgs, args);
}

oop Delta::call(oop receiver, oop selector) {
  static DeltaCallCache cache;
  return call_generic(&cache, receiver, selector, 0, NULL);
}

oop Delta::call(oop receiver, oop selector, oop arg1) {
  static DeltaCallCache cache;
  return call_generic(&cache, receiver, selector, 1, &arg1);
}

oop Delta::call(oop receiver, oop selector, oop arg1, oop arg2) {
  static DeltaCallCache cache;
  return call_generic(&cache, receiver, selector, 2, &arg1);
}

oop Delta::call(oop receiver, oop selector, oop arg1, oop arg2, oop arg3) {
  static DeltaCallCache cache;
  return call_generic(&cache, receiver, selector, 3, &arg1);
}

oop Delta::call(oop receiver, oop selector, objArrayOop args) {
  static DeltaCallCache cache;
  return call_generic(&cache, receiver, selector, args->length(), args->objs(1));
}
