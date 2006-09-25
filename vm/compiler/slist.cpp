/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.7 $ */
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

# include "incls/_slist.cpp.incl"
  
GenericSListElem* GenericSList::findL(void* p) const {
  for (GenericSListElem* e = headL(); e; e = e->nextL()) {
    if (e->dataL() == p) return e;
  }
  return NULL;
}

GenericSListElem* GenericSList::findL(void* token, slistFindFn f) const {
  for (GenericSListElem* e = headL(); e; e = e->nextL()) {
    if (f(token, e->dataL())) return e;
  }
  return NULL;
}

void* GenericSList::nthL(int n) const {
  assert(n < length(), "non-existing element");
  GenericSListElem* e = headL();
  for (int i = 0; i < n; i++, e = e->nextL()) ;
  return e->dataL();
}

void GenericSList::insertAfterL(GenericSListElem* e, void* d) {
  if (e == tailL()) {
    appendL(d);
  } else if (e == NULL) {
    prependL(d);
  } else {
    GenericSListElem* newe = new GenericSListElem(d, e->nextL());
    e->setNextL(newe);
    _len++;
  }
}

void GenericSList::removeAfterL(GenericSListElem* e) {
  if (e == NULL) {
    removeHeadL();
  } else {
    GenericSListElem* deletee = e->nextL();
    e->_next = deletee->nextL();
    if (deletee == tailL()) _tail = e;
    _len--;
  }
}

void GenericSList::removeL(void* p) {
  GenericSListElem* prev = NULL;
  for (GenericSListElem* e = headL(); e && e->dataL() != p;
       prev = e, e = e->nextL()) ;
  if (e == NULL) fatal("not in list");
  removeAfterL(prev);
  assert(!includesL(p), "remove doesn't work");
}

void GenericSList::applyL(void f(void*)) {
  GenericSListElem* nexte;	// to permit removing during iteration
  for (GenericSListElem* e = headL(); e; e = nexte) {
    nexte = e->nextL();
    f(e->dataL());
  }
}

void GenericSList::print_short() { lprintf("GenericSList %#lx", this); }

static void print_them(void* p) {
  ((PrintableResourceObj*)p)->print_short(); lprintf(" ");
}

void GenericSList::print() {
  print_short(); lprintf(": ");
  ((GenericSList*)this)->applyL(print_them);
}

# endif
