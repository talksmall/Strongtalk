/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.19 $ */
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

// LookupKeys are the keys into the code table.  
// There should be at most one compiled method for a given LookupKey.
// A LookupKey can take two forms:
// 1) normal send lookup key (is_normal_type() == true).
//    {receiver klass, selector}
// 2) super  send lookup key (is_super_type()  == true).
//    {receiver klass, method}
// 2) fake lookup key for block methods
//    {receiver klass, block method}

// LookupKeys are allocated in different ways:
//   as embedded objects: the lookupCache has LookupKeys as part of an array element.
//   as dynamically allocated resource objects: used by the optimizing compiler.
//      Use "LookupKey::allocate" for allocating a LookupKey in the resource area.

class LookupKey : ValueObj {
 protected:
  klassOop  _klass;
  oop       _selector_or_method;
 public:
  LookupKey() {
    clear();
  }
  LookupKey(klassOop klass, oop selector_or_method) {
    _klass              = klass;
    _selector_or_method = selector_or_method;
  }
  LookupKey(LookupKey* key) {
    _klass              = key->klass();
    _selector_or_method = key->selector_or_method();
  }

  klassOop klass() const              { return _klass; }
  oop      selector_or_method() const { return _selector_or_method; }

  symbolOop selector() const;

  methodOop method() const {
     assert(selector_or_method()->is_method(), "Wrong lookup type");
     return methodOop(selector_or_method());
  }

  // Lookup type
  bool is_super_type()  const { return selector_or_method()->is_method() && !methodOop(selector_or_method())->is_blockMethod();}
  bool is_normal_type() const { return !selector_or_method()->is_method(); }
  bool is_block_type()  const { return selector_or_method()->is_method() && methodOop(selector_or_method())->is_blockMethod(); }

  bool equal(LookupKey* p) const {
    return klass()              == p->klass() &&
           selector_or_method() == p->selector_or_method();
  }

  void initialize(klassOop klass, oop selector_or_method) {
    _klass              = klass;
    _selector_or_method = selector_or_method;
  }

  void clear() {
    _klass              = NULL;
    _selector_or_method = NULL;
  }

  int hash() const;
  
  void switch_pointers(oop from, oop to);
  void relocate();
  bool verify() const;
  void oops_do(void f(oop*));
  
  // Printing support output format is:
  //   "class::selector" for normal sends and
  //   "class^^selector" for super sends.
  //   "class->selector {bci}+" for block keys
  void  print() const;
  char* print_string() const;
  void  print_on(outputStream* st) const;
  void  print_inlining_database_on(outputStream* st) const;

  // For resource allocation.
  static LookupKey* allocate(klassOop klass, oop selector_or_method);
};

