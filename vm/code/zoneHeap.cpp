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

# include "incls/_zoneHeap.cpp.incl"

class HeapChunk : public ValueObj {	// a heap chunk is a consecutive sequence of blocks
 protected:
  HeapChunk* _next, *_prev;  // doubly-linked ring
  void initialize() { _next = _prev = this; size = 0; }
 public:
  int size;	// size in blocks (only for heterogenuous list)
  
  HeapChunk() { initialize(); }

  HeapChunk* next() const { return _next; }
  HeapChunk* prev() const { return _prev; }

  void insert(HeapChunk* other) { 
    other->_next = _next;
    _next->_prev = other;
    _next = other;
    other->_prev = this; 
  }

  void remove() {
    _next->_prev = _prev;
    _prev->_next = _next;
    initialize(); 
  }
};

class FreeList: private HeapChunk {
 public:
  void clear() { initialize(); }

  HeapChunk* anchor() const { return (HeapChunk*)this; }
  bool isEmpty() const { return next() == anchor(); }
  void append(HeapChunk* h);
  void remove(HeapChunk* h);
  HeapChunk* get();
  int length() const;
};


enum chunkState {
  ZeroDistance = 0, MaxDistance = 128,
  unused = 128, unusedOvfl = 190,
  used = 192, usedOvfl = 254,
  invalid = 255};

# define MaxDistLog    7  /* log2(MaxDistance) */
# define maxOneByteLen (usedOvfl - used)

// format of chunks in free map: first & last byte hold chunk size
// unused..unusedOvfl-1:unused, length n+1
// used..usedOvfl-1	used, length n - used + 1
// unusedOvfl:          unused, encoded length in next (prev) 3 bytes
// usedOvfl:		used, encoded length in next (prev) 3 bytes

// The other bytes hold the distance to the chunk header (or an approximation
// thereof); headers are found by following the distance pointers downwards

const int minHeaderSize = 1;
const int maxHeaderSize = 4;

class ChunkKlass;
inline ChunkKlass* asChunkKlass(u_char* c) 	{ return (ChunkKlass*)c; }

class ChunkKlass {
  u_char c(int which) 		{ return ((u_char*)this)[which]; }
  u_char n(int which) 		{ return c(which) - unused; }
 public:
  ChunkKlass() 				{ fatal("shouldn't create"); }
  u_char* asByte()			{ return (u_char*)this; }
  void markSize(int nChunks, chunkState s);
  void markUsed(int nChunks)		{ markSize(nChunks, used); }
  void markUnused(int nChunks)	{ markSize(nChunks, unused); }
  ChunkKlass* findStart(ChunkKlass* mapStart, ChunkKlass* mapEnd);
  
  bool verify();
  void print();
  bool isValid();
  void invalidate() 			{ asByte()[0] = invalid; }
  
  chunkState state() 			{ return chunkState(c(0)); }
  bool isUsed()				{ return state() >= used; }
  bool isUnused()			{ return ! isUsed(); }
  int headerSize() {		// size of header in bytes
    int ovfl = isUsed() ? usedOvfl: unusedOvfl;
    return c(0) == ovfl ? maxHeaderSize : minHeaderSize;
  }
  int size() {		// size of this block
    int ovfl = isUsed() ? usedOvfl: unusedOvfl;
    int len;
    assert(c(0) != invalid && c(0) >= MaxDistance, "invalid chunk");
    if (c(0) != ovfl) {
      len = c(0) + 1 - (isUsed() ? used : unused);
    } else {
      len = (((n(1) << MaxDistLog) + n(2)) << MaxDistLog) + n(3);
    }
    assert(len > 0, "negative/zero chunk length");
    return len;
  }
  bool contains(u_char* p) { return asByte() <= p && p < asByte() + size(); }
  ChunkKlass*  next() { return asChunkKlass(asByte()+size()); }
  ChunkKlass*  prev() {
    ChunkKlass* p = asChunkKlass(asByte() - 1);
    int ovfl = p->isUsed() ? usedOvfl: unusedOvfl;
    int len;
    if (c(-1) != ovfl) {
      len = p->size();
    } else {
      len = (((n(-4) << MaxDistLog) + n(-3)) << MaxDistLog) + n(-2);
    }
    assert(len > 0, "negative/zero chunk length");
    return asChunkKlass(asByte() - len);
  }
};

void ChunkKlass::markSize(int nChunks, chunkState s) {
  // write header
  u_char* p = asByte();
  u_char* e = p + nChunks - 1;
  if (nChunks < maxOneByteLen) {
    p[0] = e[0] = s + nChunks - 1;
  } else {
    assert(nChunks < (1<<(3*MaxDistLog)), "chunk too large");
    unsigned mask = nthMask(MaxDistLog);
    p[0] = e[0] = (s == used) ? usedOvfl : unusedOvfl;
    p[1] = e[-3] = unused +  (nChunks >> (2*MaxDistLog));
    p[2] = e[-2] = unused + ((nChunks >>    MaxDistLog) & mask);
    p[3] = e[-1] = unused + ( nChunks 		        & mask);
  }
  assert(size() == nChunks, "incorrect size encoding");
  // mark distance for used blocks
  if (s == unused) {
    // don't mark unused blocks - not necessary, and would be a performance 
    // bug (start: *huge* free block, shrinks with every alloc -> quadratic)
    // however, the first distance byte must be correct (for findStart)
    if (nChunks > 2 * minHeaderSize) p[headerSize()] = headerSize();
  } else {
    if (nChunks < maxOneByteLen) {
      assert(maxOneByteLen <= MaxDistance, "oops!");
      for (int i = minHeaderSize; i < nChunks - minHeaderSize; i++) p[i] = i;
    } else {
      int max = min(nChunks - 4, MaxDistance);
      for (int i = maxHeaderSize; i < max; i++) p[i] = i;
      // fill rest with large distance values (don't use MaxDistance - 1 because
      // the elems MaxDistance..MaxDistance+maxHeaderSize-1 would point *into*
      // the header)
      for ( ; i < nChunks - maxHeaderSize; i++) p[i] = MaxDistance - maxHeaderSize;
    }
  }
}

ChunkKlass* ChunkKlass::findStart(ChunkKlass* mapStart, ChunkKlass* mapEnd) {
  // this points into the middle of a chunk; return start of chunk
  u_char* p = asByte();
  u_char* start = mapStart->asByte();
  u_char* end   = mapEnd  ->asByte();
  ChunkKlass* m;
  if (*p < MaxDistance) {
    // we're outside the header, so just walk down the trail
    while (*p < MaxDistance) p -= *p;
    assert(p >= start, "not found");
    m = asChunkKlass(p);
  } else {
    // pointing to a header, but we don't know whether long/short etc
    // first walk up to first non-header byte
    // (note that first distance byte of unused blocks is correct, but
    // the others aren't)
    while (*p >= MaxDistance && p < end) p++;
    if (p < end) {
      // find start of this block
      while (*p < MaxDistance) p -= *p;
      assert(p >= start, "not found");
    }
    m = asChunkKlass(p);
    while (! m->contains(this->asByte())) m = m->prev();
  }
  assert(m->verify(), "invalid chunk map");
  assert(m->contains(this->asByte()), "wrong chunk");
  return m;
}

bool ChunkKlass::isValid() {
  u_char* p = asByte();
  bool ok;
  if (p[0] == invalid || p[0] < MaxDistance) {
    ok = false;
  } else {
    u_char* e = next()->asByte() - 1;
    int ovfl = isUsed() ? usedOvfl: unusedOvfl;
    ok = p[0] == e[0] &&
        (p[0] != ovfl || p[1] == e[-3] && p[2] == e[-2] && p[3] == e[-1]);
  }
  return ok;
}

void ChunkKlass::print() {
  lprintf("chunk [%#lx..%#lx[\n", this, next());
}

bool ChunkKlass::verify() {
  if (! isValid()) {
    error("inconsistent chunk map %#lx", this);
    return false;
  }
  return true;
}

void FreeList::append(HeapChunk* h) {
  insert(h);
}

void FreeList::remove(HeapChunk* h) {
  h->remove();
}
  
HeapChunk* FreeList::get() {
  if (isEmpty()) {
    return NULL;
  } else {
    HeapChunk* res = anchor()->next();
    remove(res);
    return res;
  } 
}

int FreeList::length() const {
  int i = 0;
  HeapChunk* f = anchor();
  for (HeapChunk* p = f->next(); p != f; p = p->next()) i++;
  return i;
}

Heap::Heap(int s, int bs) {
  assert(s % bs == 0, "size not a multiple of blockSize");
  size = s;
  if (bs & (bs - 1)) fatal1("heap block size (%ld) isn't power of 2", bs);
  blockSize = bs;
  log2BS = 0;
  while (bs > 1) { bs >>= 1; log2BS++; }
  nfree = 30;
//  _base = AllocateHeap(size + blockSize, "zone");
  _base = os::exec_memory(size + blockSize); //, "zone");
  base = (char*)((int(_base) + blockSize - 1) / blockSize * blockSize);
  assert(int(base) % blockSize == 0, "base not aligned to blockSize");
  heapKlass = (ChunkKlass*)(AllocateHeap(mapSize() + 2, "zone free map") + 1);
  // + 2 for sentinels
  freeList = NEW_C_HEAP_ARRAY( FreeList, nfree);
  bigList  = NEW_C_HEAP_OBJ(FreeList);
  newHeap = NULL;
  clear();
}

void Heap::clear() {
  // initialize the statistics
  bytesUsed = 0;
  total     = 0;
  ifrag     = 0;

  // initialize the free lists
  for(int i = 0; i < nfree; i++) {
    freeList[i].clear();
  }
  bigList->clear();

  // initialize the map
  heapKlass->markUnused(mapSize());		      // mark everything as unused
  heapEnd()->markUsed(1);			      // start sentinel
  asChunkKlass(heapKlass->asByte() - 1)->markUsed(1); // stop sentinel

  // add whole chunk to free list
  addToFreeList(heapKlass);

  // set the combine variables
  combineMode = false;
  lastCombine = heapKlass;
}

bool  Heap::contains(void* p) const {
  return between(p, base,     base + capacity()) || 
         between(p, freeList, freeList + nfree); 
}

Heap::~Heap() {
# ifdef ASSERT
    set_oops((oop*)base, capacity() / oopSize, NULL);
# endif
  free(_base);
  free(heapKlass - 1);	    // -1 to get rid of sentinel
  free(freeList);
  free(bigList);
}

void Heap::removeFromFreeList(ChunkKlass* m) {
# ifdef ASSERT
    m->verify();
# endif
  HeapChunk* p = (HeapChunk*)blockAddr(m);
  p->remove();
}

bool Heap::addToFreeList(ChunkKlass* m) {
# ifdef ASSERT
    m->verify();
# endif
  HeapChunk* p = (HeapChunk*)blockAddr(m);
  int sz = m->size();
  if (sz <= nfree) {
    freeList[sz-1].append(p);
    return false;
  } else {
    bigList->append(p);
    p->size = sz;
    return true;
  }
}

void* Heap::allocFromLists(int wantedBytes) {
  assert(wantedBytes % blockSize == 0, "not a multiple of blockSize");
  int wantedBlocks = wantedBytes >> log2BS;
  assert(wantedBlocks > 0, "negative alloc size");
  int blocks = wantedBlocks - 1;
  void* p = NULL;
  while (!p && ++blocks <= nfree) {
    p = freeList[blocks-1].get();
  }
  if (! p) {
    HeapChunk* f = bigList->anchor();
    for (HeapChunk* c = f->next();
	 c != f && c->size < wantedBlocks;
	 c = c->next());
    if (c == f) {
      if (! combineMode && combineAll() >= wantedBlocks)
	return allocFromLists(wantedBytes);
    } else {
      p = (void*)c;
      blocks = c->size;
      bigList->remove(c);
    }
  } 
  if (p) {
    ChunkKlass* m = mapAddr(p);
    assert(m->size() == blocks, "inconsistent sizes");
    m->markUsed(wantedBlocks);
    if (blocks > wantedBlocks) {
#ifdef LOG_LOTSA_STUFF
      if (!bootstrapping) LOG_EVENT("zoneHeap: splitting allocated block");
#endif
      int freeChunkSize = blocks - wantedBlocks;
      ChunkKlass* freeChunk = m->next();
      freeChunk->markUnused(freeChunkSize);
      addToFreeList(freeChunk);
    }
  }
  return p;
}

void* Heap::allocate(int wantedBytes) {
  assert(wantedBytes > 0, "Heap::allocate: size <= 0");
  int rounded = ((wantedBytes + blockSize - 1) >> log2BS) << log2BS;
  
  void* p = allocFromLists(rounded);
  if (p) {
    bytesUsed += rounded;
    total += rounded;
    ifrag += rounded - wantedBytes;
  }
  if (VerifyZoneOften) {
    verify();
  }
  return p;
}


void Heap::deallocate(void* p, int bytes) {
  ChunkKlass* m = mapAddr(p);
  int myChunkSize = m->size();
  int blockedBytes = myChunkSize << log2BS;
  bytesUsed -= blockedBytes;
  ifrag -= blockedBytes - bytes;
  m->markUnused(myChunkSize);
  bool big = addToFreeList(m);
  HeapChunk* c = (HeapChunk*)p;
  if (combineMode || big) combine(c);	// always keep bigList combined

  if (VerifyZoneOften) {
    verify();
  }
}

# define INC(p, n)   p = asChunkKlass(p->asByte() + n)

char* Heap::compact(void move(char* from, char* to, int nbytes)) {
  if (usedBytes() == capacity()) return NULL;
  
  ChunkKlass* m = heapKlass;
  ChunkKlass* end = heapEnd();
  
  ChunkKlass* freeChunk = m;
  while (freeChunk->isUsed()) {				// find 1st unused blk
    freeChunk = freeChunk->next();
  }
  ChunkKlass* usedChunk = freeChunk;
  
  for(;;) {
    while (usedChunk->isUnused()) usedChunk = usedChunk->next();
    if (usedChunk == end) break;
    int uSize = usedChunk->size();
    assert(freeChunk < usedChunk, "compaction bug");
    move(blockAddr(usedChunk), blockAddr(freeChunk), uSize << log2BS);
    freeChunk->markUsed(uSize);	  // must come *after* move
    INC(freeChunk, uSize);
    INC(usedChunk, uSize);
  }
  for(int i = 0; i < nfree; i++) freeList[i].clear();
  bigList->clear();
  int freeBlocks = end->asByte() - freeChunk->asByte();
  freeChunk->markUnused(freeBlocks);
  addToFreeList(freeChunk);
  assert(freeBlocks * blockSize == capacity() - usedBytes(),
	 "usage info inconsistent");
  lastCombine = heapKlass;
  return blockAddr(freeChunk);
}  

int Heap::combine(HeapChunk*& c) {
  // Try to combine c with its neighbors; on return, c will point to
  // the next element in the freeList, and the return value will indicate
  // the size of the combined block.
  ChunkKlass* cm = mapAddr((char*)c);
  assert(cm < heapEnd(), "beyond heap");
  ChunkKlass* cmnext = cm->next();
  ChunkKlass* cm1;
  if (cm == heapKlass) {
    cm1 = cm;
  } else {
    cm1 = cm->prev();			// try to combine with prev
    while (cm1->isUnused()) {		// will terminate because of sentinel
      ChunkKlass* free = cm1;
      cm1 = free->prev();
      removeFromFreeList(free);
      free->invalidate();		// make sure it doesn't look valid
    }
    cm1 = cm1->next();
  }
  ChunkKlass* cm2 = cmnext;		// try to combine with next
  while (cm2->isUnused()) {		// will terminate because of sentinel
    ChunkKlass* free = cm2;
    cm2 = cm2->next();
    removeFromFreeList(free);
    free->invalidate();			// make sure it doesn't look valid
  }

  // The combined block will move to a new free list; make sure that c
  // returns an element in the current list so that iterators work.
  c = c->next();

  if (cm1 != cm || cm2 != cmnext) {
    removeFromFreeList(cm);
    cm->invalidate();	
    cm1->markUnused(cm2->asByte() - cm1->asByte());
    addToFreeList(cm1);
    lastCombine = cm1;
  }
  assert(cm1 >= heapKlass && cm2 <= heapEnd() && cm1 < cm2, "just checkin'");
  return cm1->size();
}

// Try to combine adjacent free chunks; return size of biggest chunk (in blks).
int Heap::combineAll() {
  int biggest = 0;
  for (int i = 0; i < nfree; i++) {
    HeapChunk* f = freeList[i].anchor();
    for (HeapChunk* c = f->next(); c != f; ) {
      HeapChunk* c1 = c;
      int sz = combine(c);
      if (c1 == c) fatal("infinite loop detected while combining blocks");
      if (sz > biggest) biggest = sz;
    }
  }
  combineMode = true;
  if (VerifyZoneOften) {
    verify();
  }
  return biggest;
}

void* Heap::firstUsed() const {
  if (usedBytes() == 0) return NULL;
  if (heapKlass->isUsed()) {
    return base;
  } else {
    return nextUsed(base);
  }
}

// return next used chunk with address > p
void* Heap::nextUsed(void* p) const {
  ChunkKlass* m = mapAddr(p);
  if (m->isValid() && !lastCombine->contains(m->asByte())) {
    if (VerifyZoneOften) {
      for (ChunkKlass* m1 = heapKlass; m1 < m; m1 = m1->next()) ;
      assert(m1 == m, "m isn't a valid chunk");
    }
    assert(m->verify(), "valid chunk doesn't verify");
    m = m->next();
  } else {
    // m is pointing into the middle of a block (because of block
    // combination)
#   ifdef ASSERT
      for (ChunkKlass* m1 = heapKlass; m1 < lastCombine; m1 = m1->next()) ;
      assert(m1 == lastCombine, "lastCombine not found");
      assert(lastCombine->verify(), "invalid lastCombine");
#   endif
    for (ChunkKlass* n = lastCombine; n <= m; n = n->next()) ;
    m = n;
    assert(m->isValid(), "something's wrong");
  }
  
  while (m->isUnused()) {
    m = m->next();
  } 
  
  assert(m->isValid(), "something's wrong");
  assert(m <= heapEnd(), "past end of heap");
  if (m == heapEnd()) {
    return NULL;					// was last one
  } else {
    void* next = blockAddr(m);
    assert(next > p, "must be monotonic");
    assert(!(int(next) & 1), "must be even");
    return next;
  }
}

void* Heap::findStartOfBlock(void* start) const {
  // used a lot -- help the optimizer a bit
  if (newHeap && newHeap->contains(start))
    return newHeap->findStartOfBlock(start);
  
  const int  blockSz = blockSize;	
  start = (void*)(int(start) & ~(blockSz-1));
  assert(contains(start), "start not in this zone");
  ChunkKlass* m = mapAddr(start)->findStart(heapKlass, heapEnd());
  return blockAddr(m);
}

int Heap::sizeOfBlock(void* p) const {
  return mapAddr(p)->size() << log2BS;
}

void Heap::verify() const {
  ChunkKlass* m = heapKlass;
  ChunkKlass* end = heapEnd();
  if (end->isUnused() || end->size() != 1)
    error("wrong end-sentinel %d in heap %#lx", *(u_char*)end, this);
  ChunkKlass* begin = asChunkKlass(heapKlass->asByte() - 1);
  if (begin->isUnused() || begin->size() != 1)
    error("wrong begin-sentinel %d in heap %#lx", *(u_char*)begin, this);
  
  // verify map structure
  while (m < end) {
    if (!m->verify()) lprintf(" in heap %#lx", this);
    m = m->next();
  }
  // verify free lists
  for (int i = 0; i < nfree; i++) {
    int j = 0;
    int lastSize = 0;
    HeapChunk* f = freeList[i].anchor();
    for(HeapChunk* h = f->next(); h != f; h = h->next(), j++) {
      ChunkKlass* p = mapAddr(h);
      if (!p->verify())
	lprintf(" in free list %ld (elem %ld) of heap %#lx", i, j, this);
      if (p->isUsed()) {
	error("inconsistent freeList %ld elem %#lx in heap %#lx (map %#lx)",
	       i, h, this, p);
      }
      if (p->size() != lastSize && j != 0) {
	error("freeList %ld elem %#lx in heap %#lx (map %#lx) has wrong size",
	       i, h, this, p);
      }
      lastSize = p->size();
      if (h->next() == h) {
	error("loop in freeList %ld elem %#lx in heap %#lx", i, h, this);
	break;
      }
    }
  }
  int j = 0;
  HeapChunk* f = bigList->anchor();
  for(HeapChunk* h = f->next(); h != f; h = h->next(), j++) {
    ChunkKlass* p = mapAddr(h);
    if (!p->verify())
      lprintf(" in bigList (elem %ld) of heap %#lx", j, this);
    if (p->isUsed()) {
      error("inconsistent freeList %ld elem %#lx in heap %#lx (map 0xlx)",
	     i, h, this, p);
    }
  }
  if (! lastCombine->verify())
    error("invalid lastCombine in heap %#lx", this);
  
}

void Heap::print() const {
  lprintf("%#lx: [%#lx..%#lx)\n", this, base, base + capacity());
  printIndent();
  lprintf("  size %ld (blk %ld), used %ld (%1.1f%%), ifrag %1.1f%%;\n",
	 capacity(), blockSize, usedBytes(),
	 100.0 * usedBytes() / capacity(), 100.0 * intFrag());
  printIndent();
  lprintf("  grand total allocs = %ld bytes\n", total);
  printIndent();
  lprintf("  free lists: ");
  for (int i = 0; i < nfree; i++) lprintf("%ld ", freeList[i].length());
  lprintf("; %ld\n", bigList->length());
}

#endif
