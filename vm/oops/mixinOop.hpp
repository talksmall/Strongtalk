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

// mixin objects holds the description of a class
// All classes are results of mixin invocations.

// memory layout:
//    [header            ]
//    [klass_field       ]-  (Class)
//    [methods           ]-  (Array[Method])
//    [instVarDict       ]-  (Array[Symbol])
//    [class variables   ]-  (Array[Symbol])
//    [primary invocation]-  (Class)
//    [class mixin       ]-  (Mixin)
//    [instance variables]*

class mixinOopDesc: public memOopDesc {
 private:
   objArrayOop _methods;
   objArrayOop _inst_vars;          // Description of instance variables
   objArrayOop _class_vars;         // Description of class variables
   klassOop    _primary_invocation; // Points to the primary invocation
   mixinOop    _class_mixin;        // Mixin for the class part
   oop         _installed;          // Tells whether the mixin has been installed

 protected:
  mixinOopDesc* addr() const { return (mixinOopDesc*)memOopDesc::addr(); }

 public:
  friend mixinOop as_mixinOop(void* p) { return mixinOop(as_memOop(p)); }

  // sizing
  static int header_size() { return sizeof(mixinOopDesc)/oopSize; }

  // accessors
  objArrayOop methods() const             { return addr()->_methods; }
  void set_methods(objArrayOop m)         { STORE_OOP(&addr()->_methods, m); }

  objArrayOop instVars() const            { return addr()->_inst_vars; }
  void set_instVars(objArrayOop i)        { STORE_OOP(&addr()->_inst_vars, i); }

  objArrayOop classVars() const           { return addr()->_class_vars; }
  void set_classVars(objArrayOop c)       { STORE_OOP(&addr()->_class_vars, c); }

  klassOop primary_invocation() const     { return addr()->_primary_invocation; }
  void set_primary_invocation(klassOop k) { STORE_OOP(&addr()->_primary_invocation, k); }

  mixinOop class_mixin() const            { return addr()->_class_mixin; }
  void set_class_mixin(mixinOop m)        { STORE_OOP(&addr()->_class_mixin, m); }

  oop  installed() const                  { return addr()->_installed; }
  void set_installed(oop b)               { STORE_OOP(&addr()->_installed, b); }

  // primitive operations
  int       number_of_methods()   const;       // Return the number of methods.
  methodOop method_at(int index)  const;       // Return the method at index.
  void      add_method(methodOop method);      // Add/overwrite method.
  methodOop remove_method_at(int index);       // Remove and return the method at index.
  bool      includes_method(methodOop method); // Remove and return the class variable at index.

  int       number_of_instVars()  const;       // Return the number of instance variables.
  symbolOop instVar_at(int index) const;       // Return the instance variable at index.
  void      add_instVar(symbolOop name);       // Add instance variable.
  symbolOop remove_instVar_at(int index);      // Remove and return the instance variable at index.
  bool      includes_instVar(symbolOop name);  // Tells whether the name is present as an instance variable name.

  int       number_of_classVars()  const;      // Return the number of class variables.
  symbolOop classVar_at(int index) const;      // Return the class variable at index.
  void      add_classVar(symbolOop name);      // Add class variable.
  symbolOop remove_classVar_at(int index);     // Remove and return the class variable at index.
  bool      includes_classVar(symbolOop name); // Tells whether the name is present

  // Returns the offset of an instance variable.
  // -1 is returned if inst var is not present in mixin.
  int inst_var_offset(symbolOop name, int non_indexable_size) const;

  // Reflective operation
  void apply_mixin(mixinOop m);

  void customize_for(klassOop klass);
  void uncustomize_methods();

  // Tells whether the mixin has been installed
  bool is_installed() const;

  // Tells whether the mixin has been installed
  bool has_primary_invocation() const;

  // bootstrapping
  void bootstrap_object(bootstrap* st);

  friend mixinKlass;
};
