/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.7 $ */
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

class weakArrayKlass: public objArrayKlass {
 public:   
  friend void set_weakArrayKlass_vtbl(Klass* k);

  char* name() const { return "weakArray"; }

  // creates invocation
  klassOop create_subclass(mixinOop mixin, Format format);

  static klassOop create_class(klassOop super_class, mixinOop mixin);

  // Format
  Format format() { return weakArray_klass; }

  // ALL FUNCTIONS BELOW THIS POINT ARE DISPATCHED FROM AN OOP 

  // memory operations
  int  oop_scavenge_contents(oop obj);
  int  oop_scavenge_tenured_contents(oop obj);
  void oop_follow_contents(oop obj);

  // testers
  bool oop_is_weakArray() const { return true; }

  // iterators
  void oop_oop_iterate(oop obj, OopClosure* blk);
  void oop_layout_iterate(oop obj, ObjectLayoutClosure* blk);

  // Sizing
  int oop_header_size() const { return weakArrayOopDesc::header_size(); }
};
void set_weakArrayKlass_vtbl(Klass* k);
// The weak array register is used during memory management to
// split the object scanning into two parts:
//   1. Transively traverse all object except the indexable part
//      of weakArrays. Then a weakArray is encountered it is registered
//   2. Using the registered weakArrays continue the transitive traverse.
// Inbetween we can easily compute the set of object with a
// near death experience.
//
// Scavenge and Mark Sweep use to disjunct parts of the interface.

// Implementation note:
//  During phase1 of Mark Sweep pointers are reversed and a objects 
//  structure cannot be used (the klass pointer is gone). This makes
//  it necessary to register weakArrays along with their non indexable sizes.
//  'nis' contains the non indexable sizes.

// Interface for weak array support
class WeakArrayRegister : AllStatic {
 public:
  // Scavenge interface
  static void begin_scavenge();
  static bool scavenge_register(weakArrayOop obj);
  static void check_and_scavenge_contents();

  // Mark sweep interface
  static void begin_mark_sweep();
  static bool mark_sweep_register(weakArrayOop obj, int non_indexable_size);
  static void check_and_follow_contents();

 private:
  // Variables
  static bool                         during_registration;
  static GrowableArray<weakArrayOop>* weakArrays;
  static GrowableArray<int>*          nis;

  // Scavenge operations
  static void scavenge_contents();
  static inline bool scavenge_is_near_death(oop obj);
  static void scavenge_check_for_dying_objects();

  // Mark sweep operations
  static void follow_contents();
  static inline bool mark_sweep_is_near_death(oop obj);
  static void mark_sweep_check_for_dying_objects();
};

// The NotificationQueue holds references to weakArrays
// containing object with a near death experience.
class NotificationQueue : AllStatic {
 public:
  // Queue operations
  static void mark_elements();   // Marks all elements as queued (by using the sentinel bit)
  static void clear_elements();  // Reset the sentinel bit

  static bool is_empty();
  static oop  get();
  static void put(oop obj);
  static void put_if_absent(oop obj);

  // Memory management
  static void oops_do(void f(oop*));
 private:
  static oop* array;
  static int size;  
  static int first;
  static int last;
  static int succ(int index);
};
