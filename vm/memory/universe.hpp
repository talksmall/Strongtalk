/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.76 $ */
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


extern bool NeedScavenge;       // set when eden overflows

extern  bool GCInProgress;       // GC/scavenge in progress
extern  bool bootstrapping;      // true only at the very beginning

// When you add a new root is added to Universe remember to:
//   1. add a private static variable prefixed with _.
//   2. add a public static accessor function.
//   3. define the static variable in universe.cpp.
//   4. update Universe::do_oops to iterate over the new root.

// Used classes by the interpreter
extern "C" klassOop smiKlassObj;
extern "C" klassOop contextKlassObj;
extern "C" klassOop doubleKlassObj;
extern "C" klassOop symbolKlassObj;

// Used objects by the interpreter
extern "C" oop nilObj;
extern "C" oop trueObj;
extern "C" oop falseObj;

// Used objects by the block primitives
extern "C" klassOop zeroArgumentBlockKlassObj;
extern "C" klassOop oneArgumentBlockKlassObj;
extern "C" klassOop twoArgumentBlockKlassObj;
extern "C" klassOop threeArgumentBlockKlassObj;
extern "C" klassOop fourArgumentBlockKlassObj;
extern "C" klassOop fiveArgumentBlockKlassObj;
extern "C" klassOop sixArgumentBlockKlassObj;
extern "C" klassOop sevenArgumentBlockKlassObj;
extern "C" klassOop eightArgumentBlockKlassObj;
extern "C" klassOop nineArgumentBlockKlassObj;

extern "C" klassOop doubleValueArrayKlassObj;

class Universe: AllStatic {
 private:
  // Known classes in the VM
  static klassOop _memOopKlassObj;
  static klassOop _objArrayKlassObj;
  static klassOop _byteArrayKlassObj;
  static klassOop _associationKlassObj;
  static klassOop _doubleKlassObj;
  static klassOop _methodKlassObj;
  static klassOop _characterKlassObj;
  static klassOop _vframeKlassObj;

  // Known objects in tbe VM
  static objArrayOop _asciiCharacters;
  static objArrayOop _systemDictionaryObj;
  static objArrayOop _objectIDTable;
  static objArrayOop _pic_free_list; 

  static oop         _callBack_receiver;
  static symbolOop   _callBack_selector;

  static oop         _dll_lookup_receiver;
  static symbolOop   _dll_lookup_selector;

  static methodOop   _sweeper_method; // Variable used by Sweeper only
  static bool        _scavenge_blocked;

  friend class bootstrap;
 public:
  // Known classes in tbe VM
  static klassOop smiKlassObj()                { return ::smiKlassObj;               }
  static klassOop contextKlassObj()            { return ::contextKlassObj;           }
  static klassOop doubleKlassObj()             { return ::doubleKlassObj;            }
  static klassOop memOopKlassObj()             { return _memOopKlassObj;             }
  static klassOop objArrayKlassObj()           { return _objArrayKlassObj;           }
  static klassOop byteArrayKlassObj()          { return _byteArrayKlassObj;          }
  static klassOop symbolKlassObj()             { return ::symbolKlassObj;            }
  static klassOop associationKlassObj()        { return _associationKlassObj;        }
  static klassOop zeroArgumentBlockKlassObj()  { return ::zeroArgumentBlockKlassObj; }
  static klassOop oneArgumentBlockKlassObj()   { return ::oneArgumentBlockKlassObj;  }
  static klassOop twoArgumentBlockKlassObj()   { return ::twoArgumentBlockKlassObj;  }
  static klassOop threeArgumentBlockKlassObj() { return ::threeArgumentBlockKlassObj;}
  static klassOop fourArgumentBlockKlassObj()  { return ::fourArgumentBlockKlassObj; }
  static klassOop fiveArgumentBlockKlassObj()  { return ::fiveArgumentBlockKlassObj; }
  static klassOop sixArgumentBlockKlassObj()   { return ::sixArgumentBlockKlassObj;  }
  static klassOop sevenArgumentBlockKlassObj() { return ::sevenArgumentBlockKlassObj;}
  static klassOop eightArgumentBlockKlassObj() { return ::eightArgumentBlockKlassObj;}
  static klassOop nineArgumentBlockKlassObj()  { return ::nineArgumentBlockKlassObj; }
  static klassOop methodKlassObj()             { return _methodKlassObj;             }
  static klassOop characterKlassObj()          { return _characterKlassObj;          }
  static klassOop doubleValueArrayKlassObj()   { return ::doubleValueArrayKlassObj;  }
  static klassOop vframeKlassObj()             { return _vframeKlassObj;             }

  // Known objects in tbe VM
  static oop         nilObj()                  { return ::nilObj;                    }
  static oop         trueObj()                 { return ::trueObj;                   }
  static oop         falseObj()                { return ::falseObj;                  }
  static objArrayOop asciiCharacters()         { return _asciiCharacters;            }
  static objArrayOop systemDictionaryObj()     { return _systemDictionaryObj;        }
  static objArrayOop pic_free_list()           { return _pic_free_list;              }

  
  static oop         callBack_receiver()       { return _callBack_receiver;          }
  static symbolOop   callBack_selector()       { return _callBack_selector;          }
  static void set_callBack(oop receiver, symbolOop selector) {
    _callBack_receiver = receiver;
    _callBack_selector = selector;
  }

  static oop         dll_lookup_receiver()     { return _dll_lookup_receiver;        }
  static symbolOop   dll_lookup_selector()     { return _dll_lookup_selector;        }
  static void set_dll_lookup(oop receiver, symbolOop selector) {
    _dll_lookup_receiver = receiver;
    _dll_lookup_selector = selector;
  }

  static methodOop         sweeper_method()        { return _sweeper_method;         }
  static void set_sweeper_method(methodOop method) { _sweeper_method = method;       }

  static objArrayOop objectIDTable()           { return _objectIDTable;              }
  static void set_objectIDTable(objArrayOop array) {
    _objectIDTable = array;
  }

  // Version numbers
  //   increment snapshot_version whenever old snapshots will break; reset
  //   it to zero when changing the minor or major version
  static int major_version()    { return 1; }
  static int minor_version()    { return 1; }
  //static int minor_version()    { return 0; }
  static char* beta_version()   { return "alpha5"; }
  static int snapshot_version() { return 3; }

  // Check root is not badOop
  static void check_root(oop* p);

  // Iterates over roots defined in Universe
  static void roots_do(void f(oop*));

  // Iterates through all oops of Universe/Zone
  static void oops_do(void f(oop*));

  // Iterates over all active classes and mixins in the system.
  // Active means reachable from the clases in the system dictionary.
  static void classes_do(klassOopClosure* iterator);

  static void methods_do(void f(methodOop method));

 private:
  static void classes_for_do(klassOop klass, klassOopClosure* iterator);
  static void methods_in_array_do(objArrayOop array, void f(methodOop method));
  static void methods_for_do(klassOop klass, void f(methodOop method));
  friend class MethodsClosure;

 public:

  static newGeneration new_gen;
  static oldGeneration old_gen;

  static symbolTable* symbol_table;
  static rSet*        remembered_set;
  static ageTable*    age_table;

#ifdef DELTA_COMPILER
  static zone* code;
#endif

  // additional variables
  static int tenuring_threshold;
  static int scavengeCount;

    // space operations
  static bool is_heap(oop* p) {
    return new_gen.contains(p) || old_gen.contains(p); }

  static oop* object_start(oop* p);
  
  // relocate is used for moving objects around after reading in a snapshot
  static memOop relocate(memOop p);

  static bool verify_oop(memOop p);
  static bool really_contains(void *p);
  static space* spaceFor(void* p);

  static generation* generation_containing(oop p) {
    return new_gen.contains(p) ? (generation*)&new_gen : (generation*)&old_gen; }
  
  // allocators
  static oop* allocate(int size, memOop* p = NULL, bool permit_scavenge = true) {
    if (_scavenge_blocked && can_scavenge())
      return scavenge_and_allocate(size, (oop*) p);
    oop* obj = new_gen.allocate(size);
    if (!permit_scavenge) return obj;
    return obj ? obj : scavenge_and_allocate(size, (oop*) p);
  }

  static oop* allocate_without_scavenge(int size) {
    oop* obj = new_gen.allocate(size);
    return obj ? obj : allocate_tenured(size);
  }

  static oop* allocate_tenured(int size) {
    return old_gen.allocate(size); 
  }

  // Tells whether we should force a garbage collection
  static bool needs_garbage_collection();

  // tells if the vm is in a state where we can scavenge.
  static bool can_scavenge();

  // scavenging operations.
  static oop* scavenge_and_allocate(int size, oop* p);

  static void scavenge(oop* p = NULL);
  static oop tenure(oop p = NULL);
  static void default_low_space_handler(oop p= NULL);
  
  static void need_scavenge() {
    if (! NeedScavenge) {
      NeedScavenge = true;
      // setupPreemption();
    }
  }
  static bool needs_scavenge() { return NeedScavenge; }
  
  static bool should_scavenge(memOop p) {
    return !(((char*) p > Universe::old_gen.low_boundary) || Universe::new_gen.to()->contains(p)); }

  static inline oop* allocate_in_survivor_space(memOop p, int size, bool &is_new);
  
  static int free()      { return old_gen.free();      }
  
private:
  static void  get_space_sizes();

  static char* check_eden_size(spaceSizes &snap_sizes);
  static char* check_surv_size(spaceSizes &snap_sizes);
  static char* check_old_size(spaceSizes &snap_sizes);

public:
  static void  genesis();
  static spaceSizes current_sizes;

public:  
  // operations: we need extras because of include file orderings
  static void store(oop* p, smiOop contents) { *(smiOop*)p = contents;}
  static inline void store(oop* p, oop contents, bool cs= true);
  
  static void cleanup_after_bootstrap();
  
  static void switch_pointers(oop from, oop to);

  static void verify(bool postScavenge = false);

  // printing operations
  static void print();
  static void print_layout();
  static void decode_methods();
  static void objectSizeHistogram(int maxSize);

  // Iterator
  static void object_iterate(ObjectClosure* blk);
  static void root_iterate(OopClosure* blk);

  // System dictionary manipulation
  static void add_global(oop value);
  static void remove_global_at(int index);

public:
  static char* printAddr; // used for debug printing

  static void printRegion(char *&caddr, int count= 16);

  // for debugging
  static void print_klass_name(klassOop k);
  static char* klass_name(klassOop k);
  static klassOop method_holder_of(methodOop m);
  static symbolOop find_global_key_for(oop value, bool* meta);
  static oop find_global(char* name, bool must_be_constant = false);
  static associationOop find_global_association(char* name);

 public:
  static void scavenge_oop(oop* p);

  // flushes inline caches in all methodOops
  static void flush_inline_caches_in_methods();

  // clean all inline caches in methodOops and nmethods
  static void cleanup_all_inline_caches();

 private:
  static void methodOops_do(void f(methodOop));
  static void flush_inline_caches_in_method(methodOop method);

 public:
  static bool on_page_boundary(void* addr);
  static int  page_size();
};

# define STORE_OOP(ADDR, VALUE) Universe::store((oop*) ADDR, (oop) VALUE)
inline void scavenge_oop(oop* p) { *p = (*p)->scavenge(); }

inline void scavenge_tenured_oop(oop* p) {
  scavenge_oop(p);
  if ((*p)->is_new()) {
    Universe::remembered_set->record_store(p);
  }
}

inline oop* min(oop* a, oop* b) { return a > b ? b : a; }
inline oop* max(oop* a, oop* b) { return a > b ? a : b; }
  

class BlockScavenge {
 private:
  static int counter;
 public:
  static int* counter_addr() { return &counter; }
  static bool is_blocked() { return counter > 0; }

  BlockScavenge()  { counter++; }
  ~BlockScavenge() { counter--; }
};


// Lars, please complete this at some point
class VerifyNoScavenge : public StackObj {
 private:
  int  _scavengeCount;

 public:
  VerifyNoScavenge() {
    _scavengeCount   = Universe::scavengeCount;
  }

  virtual ~VerifyNoScavenge() {
    if (_scavengeCount != Universe::scavengeCount ) {
      fatal("scavenge should not have happened");
    }
  }
};

class VerifyNoAllocation : public VerifyNoScavenge {
 private:
  oop* _top_of_eden;

 public:
  VerifyNoAllocation()	{
    _top_of_eden     = Universe::new_gen.eden()->top();
  }

  virtual ~VerifyNoAllocation()	{
    if (_top_of_eden != Universe::new_gen.eden()->top()) {
      fatal("allocation should not have happened");
    }
  }
};


# define OOPS_DO_TEMPLATE(p, f)                                               \
    (*f)((oop*)p);

# define SCAVENGE_TEMPLATE(p)                                                 \
    *((oop*) p) = oop(*p)->scavenge();

# define VERIFY_TEMPLATE(p)                                                   \
    if (!oop(*p)->verify()) lprintf("\tof object at %#lx\n", p);

# define SWITCH_POINTERS_TEMPLATE(p)                                          \
    if ((oop) *p == (oop) from) *((oop*) p) = (oop) to;

# define RELOCATE_TEMPLATE(p)                                                 \
    *((oop*) p) = oop(*p)->relocate();
    
# define APPLY_TO_YOUNG_SPACE_NAMES(template)                                 \
    template(eden())                                                            \
    template(from())                                                            \
    template(to())                                                      

# define APPLY_TO_YOUNG_SPACES(template)                                      \
    template(new_gen.eden())                                             \
    template(new_gen.from())                                             \
    template(new_gen.to())                                               

#define APPLY_TO_OLD_SPACES(template)                                         \
    {FOR_EACH_OLD_SPACE(s) {template(s);}}

# define APPLY_TO_SPACES(template)                                            \
    APPLY_TO_YOUNG_SPACES(template)                                           \
    APPLY_TO_OLD_SPACES(template)

# define YOUNG_SPACE_COMPACT_TEMPLATE(s)                                      \
    c2= s; s->compact(c2, d);

# define OLD_SPACE_COMPACT_TEMPLATE(s)                                        \
    s->compact(c2, d);

# define SPACE_VERIFY_TEMPLATE(s)                                             \
    s->verify();

# define SPACE_RELOCATE_TEMPLATE(s)                                           \
    s->relocate();

# define SPACE_NEED_TO_RELOCATE_TEMPLATE(s)                                   \
    need_to_relocate |= s->need_to_relocate();

# define SPACE_FIXUP_KILLABLES_TEMPLATE(s)                                    \
    s->fixup_killables(okZone);

# define SPACE_OOP_RELOCATE_TEMPLATE(s)                                       \
    if (s->old_contains(p)) return s->relocate_objs(p);

# define SPACE_VERIFY_OOP_TEMPLATE(s)                                         \
    if (s->contains(p)) return true;
