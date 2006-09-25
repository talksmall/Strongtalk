/* Copyright 1996 LongView Technologies L.L.C. $Revision: 1.1 $ */
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

class doubleValueArrayKlass: public memOopKlass {
 public:
  // allocation properties
  bool can_inline_allocation() const { return false; }

  // Return the oop size for a doubleValueArrayOop
  int object_size(int number_of_doubleValues) const {
    return   non_indexable_size() + 1
           + roundTo(number_of_doubleValues * sizeof(double), oopSize) / oopSize;
  }
 
  // creation operations
  oop allocateObject();
  oop allocateObjectSize(int size);

  // creates invocation
  klassOop create_subclass(mixinOop mixin, Format format);

  static klassOop create_class(klassOop super_class, mixinOop mixin);

  // Format
  Format format() { return doubleValueArray_klass; }

  friend void set_doubleValueArrayKlass_vtbl(Klass* k);

  char* name() const { return "doubleValueArray"; }

 // ALL FUNCTIONS BELOW THIS POINT ARE DISPATCHED FROM AN OOP 
 public:
  // accessors
  int oop_scavenge_contents(oop obj);
  int oop_scavenge_tenured_contents(oop obj); 

  bool oop_verify(oop obj);
  void oop_print_value_on(oop obj, outputStream* st);

  // iterators
  void oop_oop_iterate(oop obj, OopClosure* blk);
  void oop_layout_iterate(oop obj, ObjectLayoutClosure* blk);

  // Sizing
  int oop_header_size() const { return doubleValueArrayOopDesc::header_size(); }
  int oop_size(oop obj) const { return object_size(doubleValueArrayOop(obj)->length()); }

  // testers
  bool oop_is_doubleValueArray() const { return true; }
  bool oop_is_indexable() const        { return true; }
};
