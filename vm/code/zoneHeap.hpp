/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.8 $ */
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

// Basic heap management
// maintains a map of the heap + free lists to reduce fragmentation
// allocations are in multiples of block size (2**k)

class HeapChunk;
class ChunkKlass;
class FreeList;

class Heap: public CHeapObj {
 protected:
  int size;		// total size in bytes
 public:
  int blockSize;	// allocation unit in bytes (must be power of 2)
  int nfree;		// number of free lists
 protected:
  int log2BS;		// log2(blockSize)
  
  int bytesUsed;	// used bytes (rounded to block size)
  int total;		// total bytes allocated so far
  int ifrag;		// bytes wasted by internal fragmentation
  char* _base;	    	// for deallocation
  char* base;		// base addr of heap (aligned to block size)
  ChunkKlass* heapKlass;// map of heap (1 byte / block)
  FreeList* freeList;	// array of free lists for different chunk sizes
  FreeList* bigList;	// list of all big free blocks
  ChunkKlass* lastCombine;// result of last block combination
 public:
  Heap* newHeap;    	// only set when growing a heap (i.e. replacing it)
  bool combineMode;	// do eager block combination on deallocs?
  
 public:
  // Constructor
  Heap(int s, int bs);
  ~Heap();

  // Initializes the Heap
  void clear();
  
  // Allocation
  void* allocate(int wantedBytes);	// returns NULL if allocation failed
  void  deallocate(void* p, int bytes);

  // Compaction
  char* compact(void move(char* from, char* to, int nbytes));	// returns first free byte  

  // Sizes  
  int capacity()  const { return size; }
  int usedBytes() const { return bytesUsed; }
  int freeBytes() const { return size - bytesUsed; }

  // Fragmentation
  double intFrag() const { return usedBytes() ? (float)ifrag / usedBytes() : 0 ; }
  double extFrag() const { return usedBytes() ? 1.0 - (float)usedBytes() / capacity() : 0; }

  // Location
  char* startAddr() const { return base; }
  char* endAddr()   const { return startAddr() + capacity(); }

  // Queries
  bool  contains(void* p) const { return (void*)base <= p && p < (void*)(base + capacity()); }
  void* firstUsed() const; // Address of first used object
  void* nextUsed(void* prev) const;
  void* findStartOfBlock(void* start) const;
  int   sizeOfBlock(void* nm) const;
  
  // Misc.
  void verify() const;
  void print() const;

 protected:
  int mapSize() const { return size >> log2BS; }
  char*  blockAddr(ChunkKlass* m) const {
    u_char* fm = (u_char*)heapKlass;
    u_char* bm = (u_char*)m;
    assert(bm >= fm && bm < fm + mapSize(), "not a heapKlass entry");
    return base + ((bm - fm) << log2BS);
  }
  ChunkKlass* mapAddr(void* p) const {
    char* pp = (char*)p;
    assert(pp >= base && pp < base + size, "not in this heap");
    assert(int(pp) % blockSize == 0, "must be block-aligned");
    u_char* fm = (u_char*)heapKlass;
    return (ChunkKlass*)(fm + ((pp - base) >> log2BS));
  }

  ChunkKlass* heapEnd() const { return (ChunkKlass*)((u_char*)heapKlass + mapSize()); }

  // Free list management
  void* allocFromLists(int wantedBytes);
  bool  addToFreeList(ChunkKlass* m);
  void  removeFromFreeList(ChunkKlass* m);

  int combineAll();
  int combine(HeapChunk*& m);
};

#endif
