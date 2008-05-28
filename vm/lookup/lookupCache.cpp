/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.75 $ */
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
# include "incls/_lookupCache.cpp.incl"

int lookupCache::number_of_primary_hits;
int lookupCache::number_of_secondary_hits;
int lookupCache::number_of_misses;

methodOop LookupResult::method() const {
  assert(!is_empty(), "connot be empty");
  if (is_method())
    return methodOop(_result);
  return get_nmethod()->method();
}

methodOop LookupResult::method_or_null() const {
  if (is_empty())  return NULL;
  if (is_method()) return methodOop(_result);
  return get_nmethod()->method();
}

jumpTableEntry* LookupResult::entry() const {
  assert(!is_empty(), "cannot be empty");
  return is_entry() ? (jumpTableEntry*) _result : NULL;
}

nmethod* LookupResult::get_nmethod() const {
  jumpTableEntry* e = entry();
  return e ? e->method() : NULL;
}

bool LookupResult::matches(methodOop m) const {
  if (is_empty()) return false;
  return is_method() ? method() == m : false;
}

bool LookupResult::matches(nmethod* nm) const {
  if (is_empty()) return false;
  return is_entry() ? entry()->method() == nm : false;
}

void LookupResult::print_on(outputStream* st) const {
  if (is_empty()) {
    st->print("empty");
  } else {
    method()->print_value_on(st);
    st->print(", 0x%lx", value());
  }
}

void LookupResult::print_short_on(outputStream* st) const {
  if (is_empty()) {
    st->print("empty");
  } else {
    st->print("0x%lx", value());
  }
}

class cacheElement { // : ValueObj {
 public:
  LookupKey    key;
  LookupResult result;
  int          filler;

  cacheElement() : key(), result() {}

  void verify() {
    assert(sizeof(cacheElement) == 16, 		"checking structure layout");
    assert((int)&this->key-(int)this == 0,  	"checking structure layout");
    assert((int)&this->result-(int)this == 8,  	"checking structure layout");

    if (key.klass() || key.selector_or_method()) {
      if (result.is_empty()) {
         std->print("Verify failed in lookupCache: ");
	 std->cr();
	 std->print("  element = (");
	 key.klass()->print_value_on(std);
         std->print("::");
         key.selector_or_method()->print_value_on(std);
         std->print(")");
         std->cr();
	 std->print("  result = (");
         result.print_on(std);
         std->print_cr(")");
	 fatal("lookupCache verify failed");
      }
      nmethod* nm = Universe::code->lookup(&key);
      if (result.is_method() && nm) {
	error("key %s has interpreted method in lookupTable but should have nmethod %#x", key.print_string(), nm);
      } else if (result.is_entry() && result.get_nmethod() != nm) {
	error("key %s: nmethod does not match codeTable nmethod", key.print_string());
      }
      if (UseInliningDatabaseEagerly && result.is_method() && InliningDatabase::lookup(&key) != NULL) {
	error("key %s: interpreted method in lookupTable despite inlining DB entry", key.print_string());
      }
    }
  }


  void clear() {
    key.initialize(NULL, NULL);
    result.clear();
  }

  void initialize(LookupKey* k, LookupResult r) {
    key    = *k;
    result = r;
  }
};

static cacheElement primary[primary_cache_size];
static cacheElement secondary[secondary_cache_size];

int lookupCache::primary_cache_address()	{ return int(&primary[0]); }
int lookupCache::secondary_cache_address()	{ return int(&secondary[0]); }

void lookupCache::flush() {
  int index;
  // Clear primary cache
  for (index = 0; index < primary_cache_size; index ++)
    primary[index].clear();
  // Clear secondary cache
  for (index = 0; index < secondary_cache_size; index ++)
    secondary[index].clear();
  // Clear counters
  number_of_primary_hits   = 0;
  number_of_secondary_hits = 0;
  number_of_misses         = 0;
}


void lookupCache::flush(LookupKey* key) {
  // Flush the entry associated the the lookup key k
  int primary_index   = hash_value(key) % primary_cache_size;
  int secondary_index = primary_index   % secondary_cache_size;

  if (primary[primary_index].key.equal(key)) {
    // If we have a hit in the primary cache
    // promoted the secondary entry
    primary[primary_index] = secondary[secondary_index];
    secondary[secondary_index].clear();
    return;
  }

  if (secondary[secondary_index].key.equal(key)) {
    // If we have a secondary hit clear the entry
    secondary[secondary_index].clear();
  }
}


void lookupCache::verify() {
  int index;
  for (index = 0; index < primary_cache_size; index ++)
    primary[index].verify();
  for (index = 0; index < secondary_cache_size; index ++)
    secondary[index].verify();
}

inline unsigned int lookupCache::hash_value(LookupKey* key) {
  return
    ((unsigned int) key->klass() ^ (unsigned int) key->selector_or_method())
    / sizeof(cacheElement);
}

inline LookupResult lookupCache::lookup_probe(LookupKey* key) {
  assert(key->verify(), "Lookupkey: verify failed");

  int primary_index = hash_value(key) % primary_cache_size;

  if (primary[primary_index].key.equal(key)) {
    return primary[primary_index].result;
  }

  int secondary_index = primary_index % secondary_cache_size;
  if (secondary[secondary_index].key.equal(key)) {
    cacheElement tmp;
    // swap primary <-> secondary.
    tmp                        = primary[primary_index];
    primary[primary_index]     = secondary[secondary_index];
    secondary[secondary_index] = tmp;
    return primary[primary_index].result;
  }
  LookupResult nothing;
  return nothing;
}

inline LookupResult lookupCache::lookup(LookupKey* key, bool compile) {
  // The cache is implemented as a 2-way associative cache.
  // Recipe for finding a lookup result.
  //  1. Check the primary cache. If hit return result.
  //  2. Check the secondary cache.
  //     If hit swap primary and secondary and return result.
  //  3. Move primary to secondary. Perform a manual lookup, place the result
  //     in primary and return the result.
  assert(key->verify(), "Lookupkey: verify failed");

  // 1. primary entry
  int primary_index = hash_value(key) % primary_cache_size;

  if (primary[primary_index].key.equal(key)) {
#ifdef DEBUG
    // this is a good place to put conditional breakpoints using number_of_primary_hits
    number_of_primary_hits++;
#endif
    return primary[primary_index].result;
  }

  // 2. secondary entry
  int secondary_index = primary_index % secondary_cache_size;
  if (secondary[secondary_index].key.equal(key)) {
#ifdef DEBUG
    number_of_secondary_hits++;
#endif
    cacheElement tmp;
    // swap primary <-> secondary.
    tmp                        = primary[primary_index];
    primary[primary_index]     = secondary[secondary_index];
    secondary[secondary_index] = tmp;
    return primary[primary_index].result;
  }

  // 3. lookup cache miss
  number_of_misses++;
  LookupResult result = cache_miss_lookup(key, compile);
  if (!result.is_empty()) {
    if (UseInliningDatabaseEagerly && result.is_method() && InliningDatabase::lookup(key) != NULL) {
      // don't update the cache during inliningDB compiles if the result is a methodOop
      // contained in the inlining DB -- otherwise method won't be compiled eagerly
      assert(theCompiler, "should only happen during compilation");   // otherwise ic lookup is broken
    } else {
      secondary[secondary_index] = primary[primary_index];
      primary[primary_index].initialize(key, result);
#ifdef ASSERT
      primary[primary_index].verify();
#endif
    }
  }
  return result;
}

LookupResult lookupCache::cache_miss_lookup(LookupKey* key, bool compile) {
  // Tracing
  if (TraceLookupAtMiss) {
    ResourceMark rm;
    key->print();
  }

  if (compile && Sweeper::is_running()) {
    // Do not compile if this is called from the sweeper.
    // The system will crash and burn since a compile does a process switch
    // Lars Bak, 6-19-96 (I don't like overwriting parameters either!)
    compile = false;
  }

  // Check Inlining database
  if (UseInliningDatabase && UseInliningDatabaseEagerly && compile) {
    ResourceMark rm;
    RScope* rs = InliningDatabase::lookup_and_remove(key);
    if (rs) {
      if (TraceInliningDatabase) {
        std->print("ID compile: ");
	key->print();
	std->cr();
      }

      // Remove old nmethod if present
      nmethod* old_nm = Universe::code->lookup(rs->key());
      VM_OptimizeRScope op(rs);
      VMProcess::execute(&op);
      if (old_nm) old_nm->makeZombie(true);

      if (op.result()) {
        LookupResult result(op.result());
        return result;
      }
    }
  }

  // Check the code table
  nmethod* nm = Universe::code->lookup(key);
  if (nm) {
    LookupResult result(nm);
    return nm;
  }

  // Last resort is searching class for the method
  methodOop method = key->is_normal_type()
                   ? key->klass()->klass_part()->lookup(key->selector())
                   : key->method();

  if (!method) {
    LookupResult result;
    return result;
  }

  if (CompiledCodeOnly && compile) {
    nm = compile_method(key, method);
    if (nm) {
      LookupResult result(nm);
      return nm;
    }
  }

  LookupResult result(method);
  return method;
}

methodOop lookupCache::compile_time_normal_lookup(klassOop receiver_klass, symbolOop selector) {
  LookupKey key(receiver_klass, selector);
  LookupResult res = lookup(&key, false);
  return res.method_or_null();
}

methodOop lookupCache::compile_time_super_lookup(klassOop receiver_klass, symbolOop selector) {
  methodOop method = method_lookup(receiver_klass->klass_part()->superKlass(), selector);
  if (method == NULL) return NULL;
  LookupKey key(receiver_klass, method);
  LookupResult res = lookup(&key, false);
  return res.method_or_null();
}

methodOop lookupCache::method_lookup(klassOop receiver_klass, symbolOop selector) {
  LookupKey key(receiver_klass, selector);
  methodOop method = key.klass()->klass_part()->lookup(key.selector());
  return method;
}

nmethod* lookupCache::compile_method(LookupKey *key, methodOop method) {
  if (!DeltaProcess::active()->in_vm_operation()) {
    VM_OptimizeMethod op(key, method);
    VMProcess::execute(&op);
    assert(op.result(),                 "nmethod must be there");
    assert(Universe::code->lookup(key), "nmethod must be there");
    return op.result();
  }
  return NULL;
}

// Lookup support for inline cache
inline LookupResult lookupCache::ic_lookup(klassOop receiver_klass, oop selector_or_method) {
  LookupKey key(receiver_klass, selector_or_method);
  return lookup(&key, true);
}

LookupResult lookupCache::ic_normal_lookup(klassOop receiver_klass, symbolOop selector) {
  return lookupCache::ic_lookup(receiver_klass, selector);
}

LookupResult lookupCache::ic_super_lookup(klassOop receiver_klass, klassOop superKlass, symbolOop selector) {
  methodOop method = method_lookup(superKlass, selector);
  return ic_lookup(receiver_klass, method);
}

LookupResult interpreter_normal_lookup(klassOop receiver_klass, symbolOop selector) {
  LookupKey key(receiver_klass, selector);
  if (!strncmp("foo", selector->chars(), 3)) {
    os::breakpoint();
  }
  return lookupCache::lookup(&key, true);
}

LookupResult interpreter_super_lookup(symbolOop selector) {
  // super lookup for interpreter: given the receiver and h-code pointer of the current
  // method (which is performing the super send), find the superclass and then the method
  // Note: the interpreter doesn't store the superclass in the InlineCache in order to
  // unify the format of inline caches for normal and super sends (the differences lead to
  // several bugs)

  // On request from Robert (6/19/95) this routine must return a methodOop since
  // the interpreter's super send cannot cope with compiled code.
  // This is a temporary restriction and should be removed soon (please bug Robert)
  //
  // This should not be a problem anymore (gri 1/24/96), clean up the lookup cache
  // interface.
  ResourceMark rm;
  frame f = DeltaProcess::active()->last_frame();
  assert(f.is_interpreted_frame(), "must be interpreter frame");

  // remember to take the home of the executing method.
  methodOop sendingMethod = f.method()->home();

  klassOop sendingMethodHolder = f.receiver()->blueprint()->lookup_method_holder_for(sendingMethod);
  // NB: lookup for current method can fail if the method was deleted while still on the stack

  if (sendingMethodHolder == NULL) {
    fatal("sending method holder cannot be found; this might be caused by a programming change -- fix this");
  }
  klassOop superKlass = sendingMethodHolder->klass_part()->superKlass();
  methodOop superMethod = lookupCache::method_lookup(superKlass, selector);

  LookupResult result(superMethod);
  return result;
}

LookupResult lookupCache::lookup(LookupKey* key) {
  return ic_lookup(key->klass(), key->selector_or_method());
}

oop lookupCache::normal_lookup(klassOop receiver_klass, symbolOop selector) {
  VerifyNoScavenge vna;
  LookupKey key(receiver_klass, selector);
  LookupResult result = ic_normal_lookup(receiver_klass, selector);
  assert(result.value() != NULL, "message not understood not implemented yet for compiled code");
  return result.value();
}

static void print_counter(char* title, int counter, int total) {
  lprintf("%20s: %3.1f%% (%d)\n",
	  title,
	  total == 0 ? 0.0 : 100.0 * (double) counter / (double) total,
	  counter);
}

void lookupCache::clear_statistics() {
  number_of_primary_hits    = 0;
  number_of_secondary_hits  = 0;
  number_of_misses          = 0;
}

void lookupCache::print_statistics() {
  int total = number_of_primary_hits
            + number_of_secondary_hits
            + number_of_misses;
  lprintf("Lookup Cache: size(%d, %d)\n",
	  primary_cache_size, secondary_cache_size);
  print_counter("Primary Hit Ratio",   number_of_primary_hits,   total);
  print_counter("Secondary Hit Ratio", number_of_secondary_hits, total);
  print_counter("Miss Ratio",          number_of_misses,         total);
}
