/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.12 $ */
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

# include "incls/_codeTable.cpp.incl"

codeTable::codeTable(int size) {
  tableSize = size;
  buckets = NEW_C_HEAP_ARRAY(codeTableEntry, size);
  clear();
}

codeTableLink* codeTable::new_link(nmethod* nm, codeTableLink* n) {
  codeTableLink* res = NEW_C_HEAP_OBJ(codeTableLink);
  res->nm   = nm;
  res->next = n;
  return res;
}

void codeTable::clear() {
  for (int index = 0; index < tableSize; index++) at(index)->clear();
}

nmethod* codeTable::lookup(LookupKey* L) {
  codeTableEntry* bucket = bucketFor(L->hash());

  // Empty
  if (bucket->is_empty()) return NULL;

  // Singleton
  if (bucket->is_nmethod()) {
      if (bucket->get_nmethod()->key.equal(L))
        return bucket->get_nmethod();
      return NULL;
  }

  // Bucket
  for (codeTableLink* l = bucket->get_link(); l; l = l->next) {
    if (l->nm->key.equal(L)) return l->nm;
  }

  return NULL;
}

void codeTable::add(nmethod* nm) {
# ifdef ASSERT
    if (lookup(&nm->key)) {
      fatal2("adding duplicate key to code table: %#lx and new %#lx",
	     lookup(&nm->key), nm);
    }
# endif
  codeTableEntry* bucket = bucketFor(nm->key.hash());

  if (bucket->is_empty()) {
    bucket->set_nmethod(nm);
  } else {
    codeTableLink* old_link;
    if (bucket->is_nmethod()) {
      old_link = new_link(bucket->get_nmethod());
    } else {
      old_link = bucket->get_link();
    }
    bucket->set_link(new_link(nm, old_link));
  }
}

void codeTable::addIfAbsent(nmethod* nm) {
  if (!lookup(&nm->key)) add(nm);
}

bool codeTable::is_present(nmethod* nm) {
  return lookup(&nm->key) == nm;
}

void codeTable::remove(nmethod* nm) {
  codeTableEntry* bucket = bucketFor(nm->key.hash());
  if (bucket->is_empty()) {
    fatal("trying to remove nmethod that is not present");
  } else {
    if (bucket->is_nmethod()) {
      bucket->clear();
    } else {
      if (bucket->get_link()->nm == nm) {
        // is it the first link
        codeTableLink* disposable_link = bucket->get_link();
	bucket->set_link(disposable_link->next);
	delete disposable_link;
      } else {
        // the the method must be further down the chain
        codeTableLink* current = bucket->get_link();
        while (current->next) {
          if (current->next->nm == nm) {
             codeTableLink* disposable_link = current->next;
	     current->next = disposable_link->next;
	     delete disposable_link;
             return;
	  }
          current = current->next;
	}
        fatal("trying to remove nmethod that is not present");
      }
    }
  }
}

bool codeTable::verify() {
  bool flag = true;
  return flag;
}

void codeTable::print() {
  lprintf("CodeTable\n");
}

void codeTable::print_stats() {
# ifdef NOT_IMPLEMENTED
  int nmin = 9999999, nmax = 0, total = 0, nonzero = 0;
  const int N = 10;
  int histo[N];
  for (int i = 0; i < N; i++) histo[i] = 0;
  for (nmln* p = buckets;  p < &buckets[tableSize];  ++p) {
    int len = 0;
    for (nmln* q = p->next;  q != p;  q = q->next) len++;
    if (len < nmin) nmin = len;
    if (len > nmax) nmax = len;
    if (len) nonzero++;
    total += len;
    histo[min(len, N-1)]++;
  }
  lprintf("\ncodeTable statistics: %d nmethods; min chain = %d, max = %d, avg = %4.1f\n",
	  total, nmin, nmax, (float)total / nonzero);
  lprintf("histogram:\n");
  for (i = 0; i < N - 1; i++) lprintf("%4d:\t%d", i, histo[i]);
  lprintf(">=%d:\t%d\n", N-1, histo[N-1]);
# endif
}

#endif
