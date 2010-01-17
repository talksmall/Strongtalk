/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.11 $ */
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

// Associations are cons cells (key, value) used in the Delta system dictionary.
//  - associationOops      are mutable.
//  - constAssociationOops are immutable.

// memory layout:
//  [header      ]
//  [klass_field ]
//  [key         ]
//  [value       ]
//  [is_constant ]

class associationOopDesc: public memOopDesc {
 protected:
  symbolOop _key;
  oop       _value;
  oop       _is_constant;
 public:
  associationOopDesc* addr() const {
    return (associationOopDesc*) memOopDesc::addr(); }

	friend associationOop as_associationOop(void* p);

  // sizing
  static int header_size()	{ return sizeof(associationOopDesc)/oopSize; }

  void bootstrap_object(bootstrap* st);

  symbolOop key() const		{ return addr()->_key; }
  void set_key(symbolOop k)	{ STORE_OOP(&addr()->_key,k);}

  oop value() const		{ return addr()->_value; }
  void set_value(oop v)		{ STORE_OOP(&addr()->_value, v); }

  bool is_constant() const	{ return addr()->_is_constant == trueObj; }
  void set_is_constant(bool v);

  static int key_offset()	  { return 2; } // offset of the key field in words
  static int value_offset()	  { return 3; } // offset of the value field in words
  static int is_constant_offset() { return 4; } // offset of the is_constant field in words
  friend class associationKlass;
};
inline associationOop as_associationOop(void* p) {
    return associationOop(as_memOop(p));
}
