/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.21 $ */
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

# include <string.h>

/*
  All classes in the virtual machine must be subclassed
  by one of the following allocation classes:

   "For objects allocated in the resource area."
   - ResourceObj
     - PrintableResourceObj

   "For objects allocated in the C-heap (managed by: free & malloc)."
   - CHeapObj
     - PrintableCHeapObj

   "For objects allocated on the stack."
   - StackObj
     - PrintableStackObj

   "For embedded objects."
   - ValueObj

   "For classes used as name spaces."
   - AllStatic

   "The printable subclasses are used for debugging and define virtual
    member functions for printing. Classes that avoid allocating the
    vtbl entries in the objects should therefore not the printable subclasses"
*/

// base class ResourceObj follows at the end of the file because it uses ResourceArea

// Base class for objects allocated in the c-heap.
class CHeapObj {
 public:
  void* operator new(size_t size);
  void  operator delete(void* p);
  void* new_array(size_t size);
};

// Base class for objects allocated in the c-heap
// with printing behavior
class PrintableCHeapObj : public CHeapObj {
 public:
  virtual void print() = 0;
  virtual void print_short();
};

// Base class for objects allocated on the stack only.
// Calling new or delete will result in fatal error.
class StackObj {
 public:
  void* operator new(size_t size);
  void  operator delete(void* p);
};

// Base class for objects allocated on the stack only but
// with printing behavior
class PrintableStackObj : StackObj {
 public:
  virtual void print() = 0;
  virtual void print_short();
};

// Base class for objects used as value objects.
// Calling new or delete will result in fatal error.
class ValueObj {
 public:
  void* operator new(size_t size);
  void operator delete(void* p);
};

// Base class for classes that constitute name spaces.
class AllStatic {
 public:
  void* operator new(size_t size);
  void operator delete(void* p);
};


// One of the following macros must be used when allocating an array to
// determine which area the array should reside in.
# define NEW_RESOURCE_ARRAY( type, size )\
    (type*) allocateResource( (size) * sizeof(type))

# define NEW_C_HEAP_ARRAY( type, size )\
    (type*) malloc( (size) * sizeof(type)); //XSTR(type) " in " __FILE__)

# define NEW_RESOURCE_OBJ( type ) NEW_RESOURCE_ARRAY( type, 1 )
# define NEW_C_HEAP_OBJ( type )   NEW_C_HEAP_ARRAY( type, 1 )

// The resource area holds temporary data structures of the VM.  Things
// in the resource area can be deallocated very efficiently using
// ResourceMarks.  (The destructor of a ResourceMark will deallocate
// everything that was created since the ResourceMark was created.)

const int min_resource_chunk_size = 256 * K;
const int min_resource_free_size  =  32 * K;

char* AllocatePageAligned(int size, char* name);
char* AllocateHeap(int size, char* name);
void  FreeHeap(void* p);

extern "C" bool PrintResourceAllocation; // to break cycle in includeDB

class ResourceAreaChunk: public PrintableCHeapObj {
  friend class ResourceMark;
  friend class ResourceArea;
  friend class Resources;
  char* bottom;
  char* top;
  char* first_free;
  ResourceAreaChunk* prev;

  int _allocated;     // Allocated bytes in this and previous chunks.
  int _previous_used; // Used bytes in previous chunks.

  void clear(char *start, char *end) { memset(start, 33, end - start); }
  void clear() { clear(bottom, first_free); }
  void freeTo(char *new_first_free);

public:
  char* allocate_bytes(int size) {
    char* p = first_free;
    if (first_free + size <= top) {
#ifndef PRODUCT
      if (PrintResourceAllocation) print_alloc(p, size);
#endif
      first_free += size;
      return p;
    } else return NULL;
  }

  ResourceAreaChunk(int min_capacity, ResourceAreaChunk* previous);
  ~ResourceAreaChunk();

  void initialize(ResourceAreaChunk* previous);

  int capacity() { return top        - bottom; }
  int used()     { return first_free - bottom; }
  
  bool contains(void* p) {
    if (p >= (void*) bottom && p < (void*) top) return true;
    else if (prev) return prev->contains(p);
    else return false; }

  void print();
  void print_short();
 protected:
  void print_alloc(char* addr, int size);
};

class ResourceArea {
 public:
  ResourceAreaChunk* chunk;       // current chunk
# ifdef ASSERT
  int nesting;            // current # of nested ResourceMarks
                          // (will warn if alloc with nesting == 0)
# endif
  
  ResourceArea();
  ~ResourceArea();

  char* allocate_more_bytes(int size);
  char* allocate_bytes(int size) {
    assert(size    >= 0, "negative size in allocate_bytes");
#ifdef ASSERT
    // NB: don't make it a fatal error -- otherwise, if you call certain functions
    // from the debugger, it might report a leak since there might not be a
    // ResourceMark.
    // However, in all other situations, calling allocate_bytes with nesting == 0
    // is a definitive memory leak.  -Urs 10/95
    static int warned = 0;	// to suppress multiple warnings (e.g. when allocating from the debugger)
    if (nesting < 1 && !warned++) error("memory leak: allocating w/o ResourceMark!");
#endif
    if (size == 0) {
      // want to return an invalid pointer for a zero-sized allocation,
      // but not NULL, because routines may want to use NULL for failure.
      return (char*) 1;
    }
    size = roundTo(size, oopSize);
    char* p;
    if (chunk && (p = chunk->allocate_bytes(size))) return p;
    return allocate_more_bytes(size);
  }

  int capacity() { return chunk ? chunk->_allocated : 0; }
  
  int used();
  bool contains(void* p) { return chunk != NULL && chunk->contains(p); }
};


// A resource mark releases all resources allocated after it was created
// when the mark is deleted.  Typically used as a local variable.
class ResourceMark: StackObj {
 protected:
  static bool enabled;
  ResourceArea* area;
  ResourceAreaChunk* chunk;
  char* top;
 public:
  ResourceMark();
  ~ResourceMark();
};

class FinalResourceMark: public ResourceMark {
 public:
  FinalResourceMark();
  ~FinalResourceMark();
};

// A NoGCVerifier makes sure that inbetween its creation and deletion
// there are no scavanges. Typically used as a local variable.

class NoGCVerifier: StackObj {
 private:
  int old_scavenge_count;
 public:
  NoGCVerifier();
  ~NoGCVerifier();
};


class Resources {
 private:
  ResourceAreaChunk* freeChunks;        // list of unused chunks
  int               _allocated;    // total number of bytes allocated
  bool               _in_consistent_state;
  ResourceAreaChunk* getFromFreeList(int min_capacity);
 public:
  Resources();
  ResourceAreaChunk* new_chunk(int min_capacity, ResourceAreaChunk* area);
  
  void addToFreeList(ResourceAreaChunk* c);
  bool in_consistent_state() { return _in_consistent_state; }

  bool  contains(char* p);
  int capacity();
  int used();
};

extern Resources resources;
extern ResourceArea resource_area;

inline char* allocateResource(int size) {
  return resource_area.allocate_bytes(size);
}


// Base class for objects allocated in the resource area per default.
// Optionally, objects may be allocated on the C heap with new(true) Foo(...)
class ResourceObj {
 public:
  void* operator new(size_t size, bool on_C_heap = false) {
    return on_C_heap ? (char*) malloc(size) : allocateResource(size);
  }

  void  operator delete(void* p, int) {} // use explicit free() to deallocate heap-allocated objects
};

// Base class for objects allocated in the resource area
// with printing behavior.
class PrintableResourceObj : public ResourceObj {
 public:
  virtual void print() = 0;
  virtual void print_short();
};

