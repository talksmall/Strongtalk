/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.8 $ */
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


// The symbol table (Memory->symbol_table) holds all canonical symbols. 
// It is implemented as an open hash table with a fixed number of buckets.
// A bucket (symbolTableEntry) is a union containing either:
//   NULL             => bucket is empty.
//   symbolOop        => bucket has one element.
//   symbolTableLink* => bucket has more than one element.

// SPACE HACK:
//  - symbolTableLinks are allocated in blocks to reduce the malloc overhead.

const int symbol_table_size = 20011;

int hash(char* name, int len);

struct symbolTableLink {
  // instance variable
  symbolOop symbol;
  symbolTableLink* next;
  
  // memory operations
  bool verify(int i);
};

struct symbolTableEntry {
  void* symbol_or_link;
  bool is_empty()  { return symbol_or_link == NULL; }
  bool is_symbol() { return oop(symbol_or_link)->is_mem(); }
  void clear()     { symbol_or_link = NULL; }

  symbolOop get_symbol() {
    return symbolOop(symbol_or_link); }
  void set_symbol(symbolOop s) {
    symbol_or_link = (void*) s; }

  symbolTableLink* get_link() {
    return (symbolTableLink*) symbol_or_link; }
  void set_link(symbolTableLink* l) {
    symbol_or_link = (void*) l; }

  // memory operations
  bool verify(int i);
  void deallocate();
  
  int length();
};

class symbolTable: public CHeapObj {
 private:
  // instance variables
  symbolTableEntry buckets[symbol_table_size];
  symbolTableLink* free_list;
  symbolTableLink* first_free_link;
  symbolTableLink* end_block;
 public:
  // constructor
  symbolTable();

  // operations
  symbolOop lookup(char* name, int len);

  // Used in bootstrap for checking
  bool is_present(symbolOop sym);
 protected:
  void add_symbol(symbolOop s); // Only used by bootstrap

  symbolOop basic_add(char *name, int len, int hashValue);
  symbolOop basic_add(symbolOop s, int hashValue);
  symbolTableEntry* bucketFor(int hashValue) {
    assert(hashValue % symbol_table_size >= 0, "must be positive");
    return &buckets[hashValue % symbol_table_size]; }
  symbolTableEntry* firstBucket() { return &buckets[0]; }
  symbolTableEntry* lastBucket()  { return &buckets[symbol_table_size-1]; }
 public:
  void add(symbolOop s);
  
  // memory operations
  void follow_used_symbols(); // Used during phase1 of garbage collection

  void switch_pointers(oop from, oop to);
  void relocate();
  void verify();

  // memory management for symbolTableLinks
  symbolTableLink* new_link(symbolOop s, symbolTableLink* n = NULL);
  void             delete_link(symbolTableLink* l);

  // histogram
  void print_histogram();

  friend bootstrap;
};
