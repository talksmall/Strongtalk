/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.9 $ */
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

// A klassOop is the C++ equivalent of a Delta class.
// Part of a klassOopDesc is a Klass which handle the 
// dispatching for the C++ method calls.

//  klassOop object layout:
//    [header     ]
//    [klass_field]
//      [vtbl                 ] <-- the Klass object starts here
//      [non_indexable_size   ]  
//      [has_untagged_contents]  can be avoided if prototype is stored.
//      [instvar              ]
//      [superKlass           ]
//      [methodDict           ]

class klassOopDesc : public memOopDesc {
 private:
  Klass _klass_part; 
 public:
  klassOop addr() const { return  (klassOop) memOopDesc::addr(); }
  Klass* klass_part() const { return &addr()->_klass_part; }

  bool is_invalid() const { return mark()->is_klass_invalid(); }

  void set_invalid(bool value) {
    set_mark(value ? mark()->set_klass_invalid() : mark()->clear_klass_invalid());
  }

  // sizing
  static int header_size() { return sizeof(klassOopDesc)/oopSize; }

  // debugging
  void print_superclasses();

  void bootstrap_object(bootstrap* st);
};

