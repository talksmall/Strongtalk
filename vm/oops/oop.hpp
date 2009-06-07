/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.37 $ */
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

// oopDesc is the top of the oop hierarchy.  The xxxDesc classes describe
// the format of ST objects so the fields can be accessed from C++.
// Caution: "oop" pointers to xxxDesc structures (e.g., oop, proxyOop) are
// TAGGED and thus should not be used to access the fields.  Rather,
// convert the xxxOop to a xxxDesc* with the ->addr() function, then
// work with the xxxDesc* pointer.
// So, xxxOop pointers are tagged, but xxxDesc* pointers aren't.
// NB: the above is true only for memOops

extern "C" oop nilObj;

class oopDesc {
// protected:
 public:
  // The _mark instance variable is here rather than in memOop (where
  // it belongs) because many C++ compilers have trouble with empty
  // objects (size 0), i.e., give them nonzero length which messes up
  // all the subclasses.
  // So, to be perfectly clear, not all oopDescs truly have a _mark word;
  // only the ones below memOop do.
  markOop _mark;

 public:
  // Called during bootstrapping for computing vtbl values see (create_*Klass)
  oopDesc();

  // tag checks
  int tag() const 		{ return maskBits(int(this), Tag_Mask); }
  bool is_smi() const   { return tag() == Int_Tag; }
  bool is_mem() const   { return tag() == Mem_Tag; }
  bool is_mark() const  { return tag() == Mark_Tag; }

  // tag dispatchers (inlined in oop.inline.h)
  inline Klass*   blueprint() const;
  inline klassOop klass() const;

  inline smi identity_hash();

  // memory management
  inline oop scavenge();
  inline oop relocate();

  // generation testers (inlined in oop.inline.h)
  inline bool is_old() const;
  inline bool is_new() const;
  inline generation* my_generation();

  // type test operations (inlined in oop.inline.h)
  inline bool is_double()           const;
  inline bool is_block()            const;
  inline bool is_byteArray()        const;
  inline bool is_doubleByteArray()  const;
  inline bool is_doubleValueArray() const;
  inline bool is_symbol()           const;
  inline bool is_objArray()         const;
  inline bool is_weakArray()        const;
  inline bool is_klass()            const;
  inline bool is_process()          const;
  inline bool is_vframe()           const;
  inline bool is_method()           const;
  inline bool is_proxy()            const;
  inline bool is_mixin()            const;
  inline bool is_association()      const;
  inline bool is_context()          const;
  inline bool is_indexable()        const;

  // Returns is the oop is the nil object
  inline bool is_nil() const { return this == nilObj; }

  // Primitives
  inline oop primitive_allocate(bool allow_scavenge=true, bool tenured=false);
  inline oop primitive_allocate_size(int size);

  inline oop shallow_copy(bool tenured);

  inline bool verify();

  // FIX LATER
  oop gc_mark()   { return this; }
  oop gc_unmark() { return this; }

  // printing functions for VM debugging
  void print_on(outputStream* st);        // First level print
  void print_value_on(outputStream* st);  // Prints oop as <ClassName>(<objectID>).

  // printing on default output stream
  void print();
  void print_value();

  // return the print strings
  char* print_string();
  char* print_value_string();

  friend class memOopKlass;
};
