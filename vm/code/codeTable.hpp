/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.13 $ */
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

#ifdef DELTA_COMPILER

// The code table is used to find nmethods in the zone.
// It is a hash table, where each bucket contains a list of nmethods.

//%note
// Should implement free list like symbolTable (Lars 2/10-95)

const int codeTableSize  = 2048;
const int debugTableSize = 256;

struct codeTableEntry;

struct codeTableLink : public CHeapObj {
  // instance variable
  nmethod*       nm;
  codeTableLink* next;

  // memory operations
  bool verify(int i);
};

struct codeTableEntry : ValueObj {
  // methods are tagged, links are not.
  void* nmethod_or_link;
  bool  is_empty()   { return nmethod_or_link == NULL; }
  bool  is_nmethod() { return (int) nmethod_or_link & 1; }
  void  clear()      { nmethod_or_link = NULL; }

  nmethod* get_nmethod() { 
    return (nmethod*) ((int) nmethod_or_link - 1);
  }
  void set_nmethod(nmethod* nm) {
    assert_oop_aligned(nm);
    nmethod_or_link = (void*) ((int) nm + 1); }

  codeTableLink* get_link() { return (codeTableLink*) nmethod_or_link; }
  void set_link(codeTableLink* l) {
    assert_oop_aligned(l);
    nmethod_or_link = (void*) l;
  }

  // memory operations
  void deallocate();
  
  int length();   // returns the number of nmethod in this bucket.

  bool verify(int i);
};

class codeTable : public PrintableCHeapObj{
 protected:
  int             tableSize;
  codeTableEntry* buckets;

  codeTableEntry* at(int index)       { return &buckets[index]; }
  codeTableEntry* bucketFor(int hash) { return at(hash & (tableSize - 1)); }
  codeTableLink*  new_link(nmethod* nm, codeTableLink* n = NULL);

 public:
  codeTable(int size);
  void clear();
  nmethod* lookup(LookupKey* L);
  bool verify();

  void print();

  void print_stats();

  // Tells whether a nmethod is present
  bool is_present(nmethod* nm);

  // Removes a nmethod from the table
  void remove(nmethod* nm);

 protected:
  // should always add through zone->addToCodeTable()
  void add(nmethod* nm);
  void addIfAbsent(nmethod* nm); // add only if not there yet

  friend class zone;
};

#endif
