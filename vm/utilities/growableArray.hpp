/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.15 $ */
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

// A growable array.

typedef void (*voidDoFn)(void* p);
typedef bool (*growableArrayFindFn)(void* token, void* elem);

class GenericGrowableArray : public PrintableResourceObj {
 protected:
  int    len;		// current length
  int    max;		// maximum length
  void** data;		// data array
  bool   on_C_heap;	// is data allocated on C heap?

  void grow(int j);	// grow data array (double length until j is a valid index)

  bool  raw_contains(const void* p) const;
  int   raw_find(const void* p) const; 
  int   raw_find(void* token, growableArrayFindFn f) const; 
  void  raw_remove(const void* p);
  void  raw_apply(voidDoFn f) const;
  void* raw_at_grow(int i, const void* fill);
  void  raw_at_put_grow(int i, const void* p, const void* fill);
  void  raw_appendAll(GenericGrowableArray* l);
  GenericGrowableArray* raw_copy() const;
  void  raw_sort(int f(const void *, const void *));

  GenericGrowableArray(int initial_size, bool on_C_heap = false);
  GenericGrowableArray(int initial_size, int initial_len, void* filler, bool on_C_heap = false);

 public:
  void  clear()    		{ len = 0; }
  int   length() const  	{ return len; }
  int   capacity() const 	{ return max; }
  bool  isEmpty() const  	{ return len == 0; }
  bool  nonEmpty() const 	{ return len != 0; }
  bool  isFull() const   	{ return len == max; }
  void** data_addr() const	{ return data; }	// for sorting

  void print_short();
  void print();
};

template<class E> class GrowableArray : public GenericGrowableArray {
 public:
  GrowableArray(int initial_size, bool on_C_heap = false) : GenericGrowableArray(initial_size, on_C_heap) {}
  GrowableArray(int initial_size, int initial_len, E filler, bool on_C_heap = false) : GenericGrowableArray(initial_size, initial_len, (void*)filler, on_C_heap) {}
  GrowableArray() : GenericGrowableArray(2) {}

  void append(const E elem) {
    if (len == max) grow(len);
    data[len++] = (void *) (elem);
  }

  E at(int i) const {
    assert(0 <= i && i < len, "illegal index");
    return reinterpret_cast<E> (data[i]);
  }

  E first() const {
    assert(len > 0, "empty list");
    return reinterpret_cast<E> (data[0]);
  }

  E last() const {                                                  
    assert(len > 0, "empty list");
    return reinterpret_cast<E> (data[len-1]);
  }

  void push(const E elem) { append(elem); }

  E pop() {
    assert(len > 0, "empty list");
    return reinterpret_cast<E> (data[--len]);
  }

  E top() const { return last(); }

  void at_put(int i, const E elem) {
    assert(0 <= i && i < len, "illegal index");
    data[i] = (void*) elem;
  }

  E at_grow(int i) {
    assert(0 <= i, "negative index");
    return reinterpret_cast<E> (raw_at_grow(i, NULL));
  }

  void at_put_grow(int i, const E elem) {
    assert(0 <= i, "negative index");
    raw_at_put_grow(i, (void*) elem, NULL);
  }

  void apply(Closure<E>* c) const {
    for (int i = 0; i < len; i++) c->do_it((E)data[i]);
  }

  bool contains(const E elem) const		{ return raw_contains((const void*) elem);           }
  int find(const E elem) const 			{ return raw_find((const void*) elem);               }
  int find(void* token, bool f(void*, E)) const	{ return raw_find(token, (growableArrayFindFn)f);    }
  void remove(const E elem)   			{ raw_remove((const void*) elem);                    }
  void apply(void f(E)) const 			{ raw_apply((voidDoFn)f);	                     }
  GrowableArray<E>* copy() const		{ return (GrowableArray<E>*) raw_copy();             }
  void appendAll(GrowableArray<E>* l) 		{ raw_appendAll((GenericGrowableArray*)l);           }
  void sort(int f(E*,E*))                       { raw_sort((int (*)(const void *, const void *)) f); }
};

