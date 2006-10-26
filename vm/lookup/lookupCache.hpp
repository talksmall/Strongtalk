/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.38 $ */
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

// The lookup cache is a 2-way associative cache mapping LookupKeys into LookupResult.
// Called when the inline cache fails or when a compile takes place.

// %gc-note
// When a garbage collect occurs the lookup cache has to be 
// recomputed since the has value is dependent on the location
// of symbols and klasses.

class LookupResult: ValueObj {
 protected:
  oop _result; // methodOop or jumpTableEntry

 public:
  // Constructors
  LookupResult()                 { clear();     }
  LookupResult(methodOop method) { set(method); }
  LookupResult(nmethod*  nm)     { set(nm);     }

  // Clears the result
  void clear() { _result = NULL; }

  // Test operations
  bool is_empty()  const { return _result == NULL;   }
  bool is_method() const { return _result->is_mem(); }
  bool is_entry()  const { return !is_method();      }


  // Maching operations
  bool matches(methodOop m) const; // Checks whether the result is methodOop m.
  bool matches(nmethod* nm) const; // Checks whether the result is nmethod nm.

  // Fetch operations
  oop             value()       const { return _result; }
  methodOop       method()      const;
  methodOop       method_or_null() const;
  jumpTableEntry* entry()       const;
  nmethod*        get_nmethod() const;
  
  // Set operations
  void set(methodOop method) {
    assert(method->is_method(), "must be method");
    _result = oop(method);
  }

  void set(nmethod* nm) {
    assert(oop(nm)->is_smi(), "nmethod must be aligned");
    _result = oop(nm->jump_table_entry()->entry_point());
  }

  void print_on(outputStream* st) const;
  void print_short_on(outputStream* st) const;
};


const unsigned int primary_cache_size   = 16 * K;
const unsigned int secondary_cache_size =  2 * K;

class lookupCache : AllStatic {
 private:
  static int primary_cache_address();
  static int secondary_cache_address();

  static unsigned int hash_value(LookupKey* key);
  static int number_of_primary_hits;
  static int number_of_secondary_hits;
  static int number_of_misses;

  static inline LookupResult ic_lookup(klassOop receiver_klass, oop selector_or_method);

  static LookupResult lookup(LookupKey* key, bool compile);
  static LookupResult cache_miss_lookup(LookupKey* key, bool compile);
  static nmethod* compile_method(LookupKey* key, methodOop method);

 public:
  // Lookup probe into the lookup cache
  static LookupResult lookup_probe(LookupKey* key);

  // Lookup support for inline cache (returns methodOop or jump_table_entry).
  static LookupResult ic_normal_lookup(klassOop receiver_klass, symbolOop selector);
  static LookupResult ic_super_lookup(klassOop receiver_klass, klassOop sending_method_holder, symbolOop selector);

  // Lookup support for interpreter  (returns methodOop or jump_table_entry).
  friend LookupResult interpreter_normal_lookup(klassOop receiver_klass, symbolOop selector);
  friend LookupResult interpreter_super_lookup(symbolOop selector);

  // Lookup support for compiler
  static methodOop method_lookup(klassOop receiver_klass, symbolOop selector);
  static methodOop compile_time_normal_lookup(klassOop receiver_klass, symbolOop selector);	// returns NULL if not found
  static methodOop compile_time_super_lookup (klassOop receiver_klass, symbolOop selector);	// returns NULL if not found

  // Lookup support for LookupKey
  static LookupResult lookup(LookupKey* key);

  // Lookup support for megamorphic sends (no super sends)
  static oop normal_lookup(klassOop receiver_klass, symbolOop selector);			// returns {methodOop or jump table entry}

  // Flushing
  static void flush(LookupKey* key);
  static void flush();

  // Clear all entries in the cache.
  static void verify();

  static void clear_statistics();
  static void print_statistics();

  friend class InterpreterGenerator;
  friend class StubRoutines;
  friend class debugPrimitives;
};
