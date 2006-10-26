/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.23 $ */
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
# include "incls/_allocation.cpp.incl"

ResourceArea resource_area;

void PrintableResourceObj::print_short() { print(); }
void PrintableCHeapObj::print_short()    { print(); }
void PrintableStackObj::print_short()    { print(); }

void* CHeapObj::operator new(size_t size){
  return (void *) AllocateHeap(size, "operator-new");
}

void CHeapObj::operator delete(void* p){
 assert( !resources.contains((char*)p),
        "CHeapObj should not be in resource area");
 FreeHeap(p);
}

void* StackObj::operator new(size_t size) {
  Unused(size);
  ShouldNotCallThis();
  return 0;
};

void StackObj::operator delete(void* p) {
  Unused(p);
  ShouldNotCallThis();
};

void* ValueObj::operator new(size_t size) {
  Unused(size);
  ShouldNotCallThis();
  return 0;
};

void ValueObj::operator delete(void* p) {
  Unused(p);
  ShouldNotCallThis();
};

ResourceAreaChunk::ResourceAreaChunk(int min_capacity,
                                     ResourceAreaChunk* previous) {
  // lprintf("Resources cap=%d used=%d\n", resources.capacity(), resources.used());

  int size = max(min_capacity + min_resource_free_size,
                  min_resource_chunk_size);
  bottom = (char*) AllocateHeap(size,"resourceAreaChunk");
  top    = bottom + size;
  initialize(previous);
}

void ResourceAreaChunk::initialize(ResourceAreaChunk* previous) {
  first_free  = bottom;
  prev        = previous;

  _allocated     = capacity() + (prev ? prev->_allocated : 0);
  _previous_used = prev ? (prev->_previous_used + used()) : 0;
}

ResourceAreaChunk::~ResourceAreaChunk() {
  FreeHeap(bottom);
}

void ResourceAreaChunk::print() {
  if (prev) prev->print();
  print_short();
  lprintf(": [%#lx, %#lx), prev = %#lx\n", bottom, top, prev);
}

void ResourceAreaChunk::print_short() { std->print("ResourceAreaChunk %#lx", this); }
void ResourceAreaChunk::print_alloc(char* addr, int size) {
  std->print_cr("allocating %ld bytes at %#lx", size, addr);
}

ResourceArea::ResourceArea() {
  chunk = NULL;
# ifdef ASSERT
  nesting = 0;
# endif
}

ResourceArea::~ResourceArea() {
  // deallocate all chunks
  ResourceAreaChunk* prevc;
  for (ResourceAreaChunk* c = chunk; c != NULL; c = prevc) {
    prevc = c->prev;
    resources.addToFreeList(c);
  }
}

char* ResourceArea::allocate_more_bytes(int size) {
  chunk = resources.new_chunk(size, chunk);
  char* p = chunk->allocate_bytes(size);
  assert(p, "Nothing returned");
  return p;
}

int ResourceArea::used() {
  if (chunk == NULL) return 0;
  return chunk->used() + (chunk->prev ? chunk->prev->_previous_used : 0);
}

Resources resources;

int Resources::capacity() { return _allocated; }

int Resources::used() {
  return resource_area.used();
}

static bool  in_rsrc;
static char* p_rsrc;

bool Resources::contains(char* p) {
  in_rsrc = false;
  p_rsrc = p;
  // FIX LATER  processes->processesDo(rsrcf2);
  return in_rsrc;
}

void Resources::addToFreeList(ResourceAreaChunk* c) {
  if (ZapResourceArea)
    c->clear();
  c->prev = freeChunks;
  freeChunks = c;
}

ResourceAreaChunk* Resources::getFromFreeList(int min_capacity) {
  if (!freeChunks) return NULL;

  // Handle the first element special
  if (freeChunks->capacity() >= min_capacity) {
    ResourceAreaChunk* res = freeChunks;
    freeChunks = freeChunks->prev;
    return res;
  }

  ResourceAreaChunk* cursor = freeChunks;
  while (cursor->prev) {
    if (cursor->prev->capacity() >= min_capacity) {
      ResourceAreaChunk* res = cursor->prev;
      cursor->prev = cursor->prev->prev;
      return res;
    }
    cursor = cursor->prev;
  }

  // No suitable chunk found
  return NULL;
}

    
ResourceAreaChunk* Resources::new_chunk(int min_capacity,
                                        ResourceAreaChunk* previous) {
  _in_consistent_state = false;
  ResourceAreaChunk* res = getFromFreeList(min_capacity);
  if (res) {
    res->initialize(previous);
  } else {
    res = new ResourceAreaChunk(min_capacity, previous);
    _allocated += res->capacity();
    if (PrintResourceChunkAllocation) {
      std->print("*allocating new resource area chunk of >=%d bytes, new total = %d bytes\n", min_capacity, _allocated);
    }
  }
  _in_consistent_state = true;

  assert(res, "just checking");

  return res;
}

void ResourceAreaChunk::freeTo(char *new_first_free) {
  assert(new_first_free <= first_free, "unfreeing in resource area");
  if (ZapResourceArea) clear(new_first_free, first_free);
  first_free= new_first_free;
}

Resources::Resources() {
 _allocated           = 0;
 _in_consistent_state = true;
}

bool ResourceMark::enabled = true;

ResourceMark::ResourceMark() {
  if (!enabled) return;
  area  = &resource_area;
  chunk = area->chunk;
  top   = chunk ? chunk->first_free : NULL;
# ifdef ASSERT
  area->nesting++;
  assert(area->nesting > 0, "nesting must be positive");
# endif
}


ResourceMark::~ResourceMark() {
  if (!enabled) return;
# ifdef ASSERT
  assert(area->nesting > 0, "nesting must be positive");
  area->nesting--;
# endif
  if (PrintResourceAllocation) {
    lprintf("deallocating to mark %#lx\n", top);
  }
  ResourceAreaChunk* prevc;
  for (ResourceAreaChunk* c = area->chunk; c != chunk; c = prevc) {
    // deallocate all chunks behind marked chunk
    prevc = c->prev;
    resources.addToFreeList(c);
  }
  area->chunk = c;
  if (c == NULL) {
    top = NULL;
    return;
  }
  c->freeTo(top); 
  if (top == c->bottom) {
    // this chunk is completely unused - deallocate it
    area->chunk = c->prev;
    resources.addToFreeList(c);
  }
}

FinalResourceMark::FinalResourceMark() {
  enabled = false;
}

FinalResourceMark::~FinalResourceMark() {
  enabled = true;
}


NoGCVerifier::NoGCVerifier() {
  old_scavenge_count = Universe::scavengeCount;
}


NoGCVerifier::~NoGCVerifier() {
  if (old_scavenge_count != Universe::scavengeCount) {
    warning("scavenge in a NoGCVerifier secured function");
  }
}

char *align(void* p, int alignment) {
  int number = (int) p;
  int adjust =  alignment - (number%alignment) % alignment;
  return (char*) number + adjust;
}

char* AllocatePageAligned(int size, char* name) {
  int page_size = Universe::page_size();
  char* block = align(malloc(size + page_size), page_size);
  if (PrintHeapAllocation)
    lprintf("Malloc (page-aligned) %s: %d = %#lx\n", name, size, block);
  return block;
}

char* AllocateHeap(int size, char* name) {
  if (PrintHeapAllocation) lprintf("Heap %7d %s\n", size, name);
  return (char*) malloc(size);
}

void FreeHeap(void* p) {
  free(p);
}

// The global operator new should never be called since it will usually indicate
// a memory leak.  Use CHeapObj as the base class of such objects to make it explicit
// that they're allocated on the C heap.
void* operator new(size_t size){
  fatal("should not call global (default) operator new");
  return (void *) AllocateHeap(size, "global operator new");
}
