/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.20 $ */
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

// The zone implements the code cache for optimized methods and contains:
//   1) a lookup table:      "Lookup key -> nmethod"
//   2) the optimized methods (nmethods).

// Implementation:
//   - Each compiled method occupies one chunk of memory.
//   - Like the offset table in oldspace the zone has at table for
//     locating a method given a addess of an instruction.

class ScopeDesc;
class PcDesc;
class relocInfo;

class ZombieLink;

class zone: public CHeapObj {
 public:
  Heap*       methodHeap;     // Contains all nmethods
  Heap*       picHeap;        // Contains all compiled CompiledPICs
  codeTable*  methodTable;    // Hash table: LookupKey --> nmethod
  jumpTable   _jump_table;    // Contains all jump entries

 public: 
  // returns the optimized method matching the lookup key, otherwise NULL
  nmethod* lookup(LookupKey* key) { return methodTable->lookup(key); }

 protected:
  nmethod* LRUhand;	// for LRU algorithm; sweeps through iZone

  bool _needsCompaction;
  bool _needsLRUSweep;
  bool _needsSweep;

  int compactTime;	// time of last compaction
  int compactDuration;  // duration of last compaction

  double minFreeFrac; 	// fraction of free space needed at compaction time

 public:  
  zone(int& size);

  void* operator new(size_t size) 	{ return AllocateHeap(size, "nmethod zone header"); }

  void clear();
  
  int capacity() const			{ return methodHeap->capacity(); }

  jumpTable* jump_table() const		{ return (jumpTable*)&_jump_table; }

  void verify_if_often();

  int used();
  int numberOfNMethods() const 		{ return jump_table()->usedIDs; }
  
  nmethod* allocate(int size);

  void free(nmethod* m);
  void addToCodeTable(nmethod* nm);

  void compact(bool forced = false);
  bool needsCompaction() const 		{ return _needsCompaction; }
  bool needsWork() const 		{ return needsCompaction() || _needsSweep; }
  bool needsSweep() const 		{ return _needsSweep; }
  void doWork();
  void doSweep();
  void flush();
  void flushZombies(bool deoptimize = true);
  void flushUnused();

  // Clear all inline caches of all nmethods
  void clear_inline_caches();

  void cleanup_inline_caches();

  int findReplCandidates(int needed);
  
  bool isDeltaPC(void* p) const;
  bool contains(void *p) const 		{ return methodHeap->contains(p); }

  nmethod* findNMethod(void* start) const;
  nmethod* findNMethod_maybe(void* start) const;  	// slow!
  
  void nmethods_do(void f(nmethod* nm));
  void PICs_do(void f(PIC* pic));

  // Iterates over all oops is the zone
  void oops_do(void f(oop*));

  void switch_pointers(oop from, oop to);
  void verify();

  void print();
  
  void print_nmethod_histogram(int size);

  nmethod* first_nm() const 		{ return (nmethod*)(methodHeap->firstUsed()); }
  nmethod* next_nm(nmethod* p) const  	{ return (nmethod*)(methodHeap->nextUsed(p)); }

  PIC* first_pic() const 	{ return (PIC*)(picHeap->firstUsed()); }
  PIC* next_pic(PIC* p) const  	{ return (PIC*)(picHeap->nextUsed(p)); }

  char*    instsStart();
  int	   instsSize();
  int    LRU_time();
  int	   sweeper(int maxVisit, int maxReclaim,
		   int* nvisited = NULL, int* nbytesReclaimed = NULL);
  int    nextNMethodID();

 public:
   void mark_dependents_for_deoptimization();
   void mark_all_for_deoptimization();
   void unmark_all_for_deoptimization();
   void make_marked_nmethods_zombies();

 protected:
  void     print_helper(bool stats);
  void 	   adjustPolicy();
  int    flushNextMethod(int needed);
  inline nmethod* next_circular_nm(nmethod* nm);
 
  friend void moveInsts(char* from, char* to, int size);
  friend void printAllNMethods();
  friend void sweepTrigger();
};

// holds usage information for nmethods (or index of next free nmethod ID
// if not assigned to any nmethod)
class LRUcount : ValueObj {
 public:
  uint16 unused;	// nmethod prologue clears BOTH fields to 0
  uint16 lastUsed;	// time of last use
  
  LRUcount() 		{ ShouldNotCallThis(); } // shouldn't create
  void  set(int i) 	{ *(int*)this = i; }
};

extern LRUcount* LRUtable;	// for optimized methods
extern int* LRUflag;	// == LRUtable, just different type for convenience
#endif
