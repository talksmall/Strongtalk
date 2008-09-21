/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.57 $ */
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

// A Klass is the the part of the klassOop that provides:
//  1: language level class object (method dictionary etc.)
//  2: provide vm dispatch behavior for the object
// Both functions are combined into one C++ class. The toplevel class "Klass"
// implements purpose 1 whereas all subclasses provide extra virtual functions
// for purpose 2.

// One reason for the oop/klass dichotomy in the implementation is
// that we don't want a C++ vtbl pointer in every object.  Thus,
// normal oops don't have any virtual functions.  Instead, they
// forward all "virtual" functions to their klass, which does have
// a vtbl and does the C++ dispatch depending on the object's
// actual type.  (See oop.inline.h for some of the forwarding code.)
// ALL FUNCTIONS IMPLEMENTING THIS DISPATCH ARE PREFIXED WITH "oop_"!

// Microsoft C++ 2.0 always forces the vtbl at offset 0.
const int vtbl_position = 0;

//  Klass layout:
//    [vtbl                 ]
//    [non_indexable_size   ]
//    [has_untagged_contents]  can be avoided if prototype is stored.
//    [classVars            ]  class variables copied from mixin
//    [methods              ]  customized methods from the mixin
//    [superKlass           ]  super
//    [mixin                ]  the mixin for the class

class Klass : ValueObj {
 protected:
  smiOop        _non_indexable_size;
  smiOop        _has_untagged_contents;
  objArrayOop   _classVars;
  objArrayOop   _methods;
  klassOop      _superKlass;
  mixinOop      _mixin;

 public:
  friend klassOop as_klassOop(void* p);

  // Returns the enclosing klassOop
  klassOop as_klassOop() const {
   // see klassOop.hpp for layout.
   return (klassOop) (((char*) this) - sizeof(memOopDesc) + Mem_Tag);
  }

  smi non_indexable_size() const {
     return _non_indexable_size->value();  }
  void set_non_indexable_size(smi size) {
     _non_indexable_size = as_smiOop(size); }

  bool has_untagged_contents() const  { return _has_untagged_contents == smiOop_one; }
  void set_untagged_contents(bool v)  { _has_untagged_contents = v ? smiOop_one : smiOop_zero; }

  objArrayOop classVars()       const { return _classVars;  }
  void  set_classVars(objArrayOop c)  { STORE_OOP(&_classVars, c); }

  objArrayOop methods()         const { return _methods;  }
  void set_methods(objArrayOop m)     { STORE_OOP(&_methods, m); }

  klassOop superKlass() const         { return _superKlass;  }
  void set_superKlass(klassOop super) { STORE_OOP(&_superKlass, super); }

  mixinOop mixin() const              { return _mixin;  }
  void set_mixin(mixinOop m)          { STORE_OOP(&_mixin, m); }

  // Tells whether here is a super class
  bool has_superKlass() const { return oop(superKlass()) != nilObj; }

 public:
  int            number_of_methods()   const;        // Returns the number of methods in this class.
  methodOop      method_at(int index)  const;        // Returns the method at index.
  void           add_method(methodOop method);       // Adds or overwrites with method.
  methodOop      remove_method_at(int index);        // Removes method at index and returns the removed method.

  int            number_of_classVars()   const;      // Returns the number of class variables.
  associationOop classVar_at(int index)  const;      // Returns the class variable at index.
  void           add_classVar(associationOop assoc); // Adds or overwrites class variable.
  associationOop remove_classVar_at(int index);      // Removes class variable at index and returns the removed association.
  bool           includes_classVar(symbolOop name);  // Tells whether the name is present

  // virtual pointer value
  int    vtbl_value() const           { return ((int*) this)[vtbl_position]; }
  void set_vtbl_value(int vtbl) {
    assert(vtbl % 4 == 0, "VTBL should be aligned");
    ((int*) this)[vtbl_position] = vtbl; }

  void bootstrap_klass_part_one(bootstrap* bs);
  void bootstrap_klass_part_two(bootstrap* bs);

 public:
   // After reading the snapshot the klass has to be fixed e.g. vtbl initialized!
  void fixup_after_snapshot_read();  // must not be virtual; vtbl not fixed yet

  // allocation operations
  virtual bool can_inline_allocation() const { return false; }
  // If this returns true, the compiler may inline the allocation code.
  // This means that the actual definition of allocate() is ignored!!
  // Fix this compare member function pointers (9/9-1994)

  // Reflective properties
  virtual bool can_have_instance_variables() const { return false; }
  virtual bool can_be_subclassed()           const { return false; }
          bool is_specialized_class()        const;

  // Tells whether this is a named class
  bool is_named_class() const;

  // allocation operations
  int size() const {  return sizeof(Klass)/sizeof(oop); }

  virtual oop allocateObject(bool permit_scavenge = true);
  virtual oop allocateObjectSize(int size);

  enum Format {	// Format of a vm klass
    no_klass,
    mem_klass,
      association_klass,
      blockClosure_klass,
      byteArray_klass,
        symbol_klass,
      context_klass,
      doubleByteArray_klass,
      doubleValueArray_klass,
      double_klass,
      klass_klass,
      method_klass,
      mixin_klass,
      objArray_klass,
        weakArray_klass,
    process_klass,
    vframe_klass,
    proxy_klass,
    smi_klass,
    special_klass
  };

  // format
  virtual Format format() { return no_klass; }

  static Format format_from_symbol(symbolOop format);
  static char*  name_from_format(Format format);

  // Tells whether the two klass have same layout (format and instance variables)
  bool has_same_layout_as(klassOop klass);

  // creates invocation
  virtual klassOop create_subclass(mixinOop mixin, Format format);

 protected:
  static klassOop create_generic_class(klassOop super_class, mixinOop mixin, int vtbl);

 public:

  virtual char* name() const { return ""; }
  void  print_klass();
  void print_name_on(outputStream* st);

  // Methods
  inline methodOop local_lookup(symbolOop selector);
  methodOop lookup(symbolOop selector);
  bool is_method_holder_for(methodOop method);
  klassOop lookup_method_holder_for(methodOop method);

  // Reflective operation
  void flush_methods();

  // Class variables
  associationOop local_lookup_class_var(symbolOop name);
  associationOop lookup_class_var(symbolOop name);

  // Instance variables

  // Returns the word offset for an instance variable.
  // -1 is returned if the search failed.
  int lookup_inst_var(symbolOop name) const;

  // Returns the name of the instance variable at offset.
  // NULL is returned if the search failed.
  symbolOop inst_var_name_at(int offset) const;

  // Compute the number of instance variables based on the mixin,
  int number_of_instance_variables() const;

  // Schema change support
  void mark_for_schema_change();
  bool is_marked_for_schema_change();

  void initialize();

 // ALL FUNCTIONS BELOW THIS POINT ARE DISPATCHED FROM AN OOP
 // These functions describe behavior for the oop not the KLASS.
 public:
  // actual oop size of obj in memory
  virtual int oop_size(oop obj) const { return non_indexable_size(); }

  // Returns the header size for an instance of this klass
  virtual int oop_header_size() const { return 0; }

  // memory operations
  virtual bool oop_verify(oop obj);

  virtual int  oop_scavenge_contents(oop obj);
  virtual int  oop_scavenge_tenured_contents(oop obj);
  virtual void oop_follow_contents(oop obj);

  // type testing operations
  virtual bool oop_is_smi()              const { return false; }
  virtual bool oop_is_double()           const { return false; }
  virtual bool oop_is_block()            const { return false; }
  virtual bool oop_is_byteArray()        const { return false; }
  virtual bool oop_is_doubleByteArray()  const { return false; }
  virtual bool oop_is_doubleValueArray() const { return false; }
  virtual bool oop_is_symbol()           const { return false; }
  virtual bool oop_is_objArray()         const { return false; }
  virtual bool oop_is_weakArray()        const { return false; }
  virtual bool oop_is_klass()            const { return false; }
  virtual bool oop_is_process()          const { return false; }
  virtual bool oop_is_vframe()           const { return false; }
  virtual bool oop_is_method()           const { return false; }
  virtual bool oop_is_proxy()            const { return false; }
  virtual bool oop_is_mixin()            const { return false; }
  virtual bool oop_is_association()      const { return false; }
  virtual bool oop_is_context()          const { return false; }
  virtual bool oop_is_message()          const { return false; }
  virtual bool oop_is_indexable()        const { return false; }

  // Dispatched primitives
  virtual oop oop_primitive_allocate(oop obj);
  virtual oop oop_primitive_allocate_size(oop obj, int size);
  virtual oop oop_shallow_copy(oop obj, bool tenured);

  // printing operations
  virtual void oop_print_on      (oop obj, outputStream* st);
  virtual void oop_short_print_on(oop obj, outputStream* st);
  virtual void oop_print_value_on(oop obj, outputStream* st);

  // iterators
  virtual void oop_oop_iterate(oop obj, OopClosure* blk);
  virtual void oop_layout_iterate(oop obj, ObjectLayoutClosure* blk);

  friend class klassKlass;
};

inline klassOop as_klassOop(void* p)
{
	return klassOop(as_memOop(p));
}
