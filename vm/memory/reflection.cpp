/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.21 $ */
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
# include "incls/_reflection.cpp.incl"

// Converter hierarchy:
// - memConverter
//   - byteArrayConverter
//   - doubleByteArrayConverter
//   - doubleValueArrayConverter
//   - klassConverter
//   - mixinConverter
//   - objArrayConverter
//   - processConverter
//   - proxyConverter

class memConverter: public ResourceObj {
 protected:
  klassOop old_klass;
  klassOop new_klass;
  GrowableArray<int>* mapping;

  void compute_mapping() {
    mapping = new GrowableArray<int>(20);
    int old_header_size = old_klass->klass_part()->oop_header_size();
    int new_header_size = new_klass->klass_part()->oop_header_size();
    int n               = old_klass->klass_part()->number_of_instance_variables();
    for (int old_index = 0; old_index < n; old_index++) {
      symbolOop name = old_klass->klass_part()->inst_var_name_at(old_index + old_header_size);
      assert(name->is_symbol(), "instance variable name must be symbol");
      int new_index = new_klass->klass_part()->lookup_inst_var(name);
      if (new_index > 0) {
	if (TraceApplyChange) {
          std->print("map ");
	  name->print_symbol_on(std);
	  std->print_cr(" %d -> %d", old_index + old_header_size, new_index);
	}
        // We found a match between a old and new class
        mapping->push(old_index + old_header_size);
        mapping->push(new_index);
      }
    }
  }

 public:
  memConverter(klassOop old_klass, klassOop new_klass) {
    this->old_klass = old_klass;
    this->new_klass = new_klass;
    compute_mapping();
  }

  memOop convert(memOop src) {
    memOop dst = allocate(src);
    // Transfer contents from src to dst
    transfer(src, dst);
    Reflection::forward(src, dst);
    return dst;
  }

  virtual void transfer(memOop src, memOop dst) {
    // header information
    if (src->mark()->is_near_death())
      dst->mark_as_dying();
    dst->set_identity_hash(src->identity_hash());
    // Instance variables
    for (int index = 0; index < mapping->length(); index += 2) {
      int from = mapping->at(index);
      int to   = mapping->at(index+1);
      dst->raw_at_put(to, src->raw_at(from));
    }
  }

  virtual memOop allocate(memOop src) {
    return memOop(new_klass->klass_part()->allocateObject());
  }
};

class proxyConverter: public memConverter {
 private:
  bool source_is_proxy;
 public:
  proxyConverter(klassOop old_klass, klassOop new_klass)
  : memConverter(old_klass, new_klass){
    assert(new_klass->klass_part()->oop_is_proxy(), "new_klass must be a proxy klass");
    source_is_proxy = old_klass->klass_part()->oop_is_proxy();
  }

  void transfer(memOop src, memOop dst) {
    if (source_is_proxy) 
      proxyOop(dst)->set_pointer(proxyOop(src)->get_pointer());
    memConverter::transfer(src, dst);
  }
};

class processConverter: public memConverter {
 private:
  bool source_is_process;
 public:
  processConverter(klassOop old_klass, klassOop new_klass)
  : memConverter(old_klass, new_klass){
    assert(new_klass->klass_part()->oop_is_process(), "new_klass must be a process klass");
    source_is_process = old_klass->klass_part()->oop_is_process();
  }

  void transfer(memOop src, memOop dst) {
    if (source_is_process) 
      processOop(dst)->set_process(processOop(src)->process());
    memConverter::transfer(src, dst);
  }
};

class byteArrayConverter: public memConverter {
 private:
  bool source_is_byte_array;
 public:
  byteArrayConverter(klassOop old_klass, klassOop new_klass)
  : memConverter(old_klass, new_klass){
    assert(new_klass->klass_part()->oop_is_byteArray(), "new_klass must be a byteArray klass");
    source_is_byte_array = old_klass->klass_part()->oop_is_byteArray();
  }

  void transfer(memOop src, memOop dst) {
    memConverter::transfer(src, dst);
    if (source_is_byte_array) {
      int length = byteArrayOop(src)->length();
      for (int index = 1; index <= length; index++)
	byteArrayOop(dst)->byte_at_put(index, byteArrayOop(src)->byte_at(index));
    }
  }

  memOop allocate(memOop src) {
    int len = source_is_byte_array ? byteArrayOop(src)->length() : 0;
    return memOop(new_klass->klass_part()->allocateObjectSize(len));
  }
};

class doubleByteArrayConverter: public memConverter {
 private:
  bool source_is_doubleByte_array;
 public:
  doubleByteArrayConverter(klassOop old_klass, klassOop new_klass)
  : memConverter(old_klass, new_klass){
    assert(new_klass->klass_part()->oop_is_doubleByteArray(), "new_klass must be a byteArray klass");
    source_is_doubleByte_array = old_klass->klass_part()->oop_is_doubleByteArray();
  }

  void transfer(memOop src, memOop dst) {
    memConverter::transfer(src, dst);
    if (source_is_doubleByte_array) {
      int length = doubleByteArrayOop(src)->length();
      for (int index = 1; index <= length; index++)
	doubleByteArrayOop(dst)->doubleByte_at_put(index, doubleByteArrayOop(src)->doubleByte_at(index));
    }
  }

  memOop allocate(memOop src) {
    int len = source_is_doubleByte_array ? doubleByteArrayOop(src)->length() : 0;
    return memOop(new_klass->klass_part()->allocateObjectSize(len));
  }
};

class objArrayConverter: public memConverter {
 private:
  bool source_is_obj_array;
 public:
  objArrayConverter(klassOop old_klass, klassOop new_klass)
  : memConverter(old_klass, new_klass){
    assert(new_klass->klass_part()->oop_is_objArray(), "new_klass must be a objArray klass");
    source_is_obj_array = old_klass->klass_part()->oop_is_objArray();
  }

  void transfer(memOop src, memOop dst) {
    memConverter::transfer(src, dst);
    if (source_is_obj_array) {
      int length = objArrayOop(src)->length();
      for (int index = 1; index <= length; index++)
	objArrayOop(dst)->obj_at_put(index, objArrayOop(src)->obj_at(index));
    }
  }

  memOop allocate(memOop src) {
    int len = source_is_obj_array ? objArrayOop(src)->length() : 0;
    return memOop(new_klass->klass_part()->allocateObjectSize(len));
  }
};

class doubleValueArrayConverter: public memConverter {
 private:
  bool source_is_obj_array;
 public:
  doubleValueArrayConverter(klassOop old_klass, klassOop new_klass)
  : memConverter(old_klass, new_klass){
    assert(new_klass->klass_part()->oop_is_doubleValueArray(), "new_klass must be a doubleValueArray klass");
    source_is_obj_array = old_klass->klass_part()->oop_is_doubleValueArray();
  }

  void transfer(memOop src, memOop dst) {
    memConverter::transfer(src, dst);
    if (source_is_obj_array) {
      int length = doubleValueArrayOop(src)->length();
      for (int index = 1; index <= length; index++)
	doubleValueArrayOop(dst)->double_at_put(index, doubleValueArrayOop(src)->double_at(index));
    }
  }

  memOop allocate(memOop src) {
    int len = source_is_obj_array ? doubleValueArrayOop(src)->length() : 0;
    return memOop(new_klass->klass_part()->allocateObjectSize(len));
  }
};

class klassConverter: public memConverter {
 public:
  klassConverter(klassOop old_klass, klassOop new_klass)
  : memConverter(old_klass, new_klass){
    assert(old_klass->klass_part()->oop_is_klass(), "new_klass must be a klass klass");
    assert(new_klass->klass_part()->oop_is_klass(), "new_klass must be a klass klass");
  }

  void transfer(memOop src, memOop dst) {
    memConverter::transfer(src, dst);
  }

  memOop allocate(memOop src) {
    Unimplemented();
    return NULL;
  }
};

class mixinConverter: public memConverter {
 public:
  mixinConverter(klassOop old_klass, klassOop new_klass)
  : memConverter(old_klass, new_klass){
    assert(old_klass->klass_part()->oop_is_mixin(), "new_klass must be a mixin klass");
    assert(new_klass->klass_part()->oop_is_mixin(), "new_klass must be a mixin klass");
  }

  void transfer(memOop src, memOop dst) {
    Unimplemented();
    memConverter::transfer(src, dst);
  }
};

// Static variables in Reflection
GrowableArray<ClassChange*>*  Reflection::class_changes = NULL;
GrowableArray<memOop>*        Reflection::converted   = NULL;

class ConvertClosure : public OopClosure {
  void do_oop(oop* o) {
    Reflection::convert(o);
  }
};

class ConvertOopClosure : public ObjectClosure {
 public:
  void do_object(memOop obj) {
    if (obj->klass()->klass_part()->is_marked_for_schema_change()) return;
    ConvertClosure blk;
    obj->oop_iterate(&blk);
  }
};

class ClassChange: public ResourceObj {
 private:
  klassOop      _old_klass;
  mixinOop      _new_mixin;
  Klass::Format _new_format;
  klassOop      _new_klass;
  klassOop      _new_super;
  memConverter* _converter;
  ClassChange*  _super_change;
  bool          _is_schema_change_computed;
  bool          _needs_schema_change;
  char*         _reason_for_schema_change;

 public:
  ClassChange(klassOop old_klass, mixinOop new_mixin, Klass::Format new_format, klassOop new_super) {
    _old_klass    = old_klass;
    _new_mixin    = new_mixin;
    _new_format   = new_format;
    _new_super    = new_super;
    _new_klass    = NULL;
    _converter    = NULL;
    _super_change = NULL;
    _is_schema_change_computed = false;
    _reason_for_schema_change = "";
  }

  ClassChange(klassOop old_klass, Klass::Format new_format) {
    _old_klass    = old_klass;
    _new_mixin    = old_klass->klass_part()->mixin();
    _new_format   = new_format;
    _new_super    = NULL;
    _new_klass    = NULL;
    _converter    = NULL;
    _super_change = NULL;
    _is_schema_change_computed = false;
    _reason_for_schema_change = "";
  }
  klassOop      old_klass()    const { return _old_klass;    }
  mixinOop      new_mixin()    const { return _new_mixin;    }
  Klass::Format new_format()   const { return _new_format;   }
  klassOop      new_klass()    const { return _new_klass;    }
  klassOop      new_super()    const { return _new_super;    }
  memConverter* converter()    const { return _converter;    }
  ClassChange*  super_change() const { return _super_change; }
  mixinOop      old_mixin()    const { return _old_klass->klass_part()->mixin(); }


  char* reason_for_schema_change()             { return _reason_for_schema_change; }
  void set_reason_for_schema_change(char* msg) { _reason_for_schema_change = msg; }

  void set_super_change(ClassChange* change) { 
    _super_change = change;
  }

  void setup_schema_change();
  void recustomize_methods();

  bool compute_needed_schema_change();
  bool needs_schema_change() {
    if (!_is_schema_change_computed)
      _needs_schema_change = compute_needed_schema_change(); 
    return _needs_schema_change;
  }

  klassOop new_class_from(klassOop      old_klass,
                                 klassOop      new_super_klass,
                                 mixinOop      new_mixin,
                                 Klass::Format new_format,
                                 mixinOop      old_mixin);

  memConverter* create_converter_for(klassOop old_class, klassOop new_class);

  void transfer_misc(memOop src, memOop dst);

  // Updating if no schema change is needed
  void update_class(bool class_vars_changed,
                    bool instance_methods_changed,
		    bool class_methods_changed);

  void update_class_vars();
  void update_methods(bool instance_side);
};


void ClassChange::recustomize_methods() {
  new_mixin()->customize_for(new_klass());
  new_mixin()->class_mixin()->customize_for(new_klass()->klass());
}

klassOop ClassChange::new_class_from(klassOop      old_klass,
                                     klassOop      new_super_klass,
                                     mixinOop      new_mixin,
                                     Klass::Format new_format,
                                     mixinOop      old_mixin) {

  Klass::Format format = (new_format != Klass::special_klass)
                       ?  new_format
		       :  old_klass->klass_part()->format();

  klassOop result = new_super_klass->klass_part()->create_subclass(new_mixin, format);
  if (result == NULL) {
    fatal("class creation failed - internal error");
  }

  // %cleanup code
  _new_klass = result;

  if (old_klass->klass_part()->mixin()->primary_invocation() == old_klass) {
    recustomize_methods();
    new_mixin->set_primary_invocation(result);
    new_mixin->class_mixin()->set_primary_invocation(result->klass());
  }

  new_mixin->set_installed(trueObj);
  new_mixin->class_mixin()->set_installed(trueObj);

  transfer_misc(old_klass,          result);
  transfer_misc(old_klass->klass(), result->klass());

  // Copy the class variables
  for (int index = old_klass->klass_part()->number_of_classVars(); index > 0; index--) {
    associationOop old_assoc = old_klass->klass_part()->classVar_at(index);
    associationOop new_assoc = result->klass_part()->local_lookup_class_var(old_assoc->key());
    if (new_assoc) {
      new_assoc->set_value(old_assoc->value());
    }
  }
  return result;
}


void ClassChange::transfer_misc(memOop  src, memOop dst) {
  memConverter* c = new memConverter(src->klass(), dst->klass());
  c->transfer(src, dst);
}

memConverter* ClassChange::create_converter_for(klassOop old_class, klassOop new_class) {
  switch (new_class->klass_part()->format()) {
  case Klass::mem_klass:              return new              memConverter(old_class, new_class);
  case Klass::byteArray_klass:        return new        byteArrayConverter(old_class, new_class);
  case Klass::doubleByteArray_klass:  return new  doubleByteArrayConverter(old_class, new_class);
  case Klass::doubleValueArray_klass: return new doubleValueArrayConverter(old_class, new_class);
  case Klass::klass_klass:            return new            klassConverter(old_class, new_class);
  case Klass::mixin_klass:            return new            mixinConverter(old_class, new_class);
  case Klass::objArray_klass:         return new         objArrayConverter(old_class, new_class);
  case Klass::weakArray_klass:        return new         objArrayConverter(old_class, new_class);
  case Klass::process_klass:          return new          processConverter(old_class, new_class);
  case Klass::proxy_klass:            return new            proxyConverter(old_class, new_class);
  }
  fatal("cannot create converter for type");
  return NULL;
}

void ClassChange::update_class_vars() {
  if (TraceApplyChange) {
    std->print(" - updating class variables for: ");
    old_klass()->print_value();
    std->cr();
  }

  Klass* k = old_klass()->klass_part();

  // Remove the dead entries
  for (int index = k->number_of_classVars(); index > 0; index--) {
    associationOop assoc = k->classVar_at(index);
    if (!new_mixin()->includes_classVar(assoc->key())) {
      k->remove_classVar_at(index);
    }
  }
  // Add the new ones
  for (index = new_mixin()->number_of_classVars(); index > 0; index--) {
    symbolOop name = new_mixin()->classVar_at(index);
    if (!k->includes_classVar(name)) {
      k->add_classVar(oopFactory::new_association(name, nilObj, false));
    }
  }

  // Make fhe Class side enherit the Instance side class variables
  old_klass()->klass()->klass_part()->set_classVars(k->classVars());

  old_mixin()->set_classVars(new_mixin()->classVars());
  old_mixin()->class_mixin()->set_classVars(new_mixin()->classVars());
}

void ClassChange::update_methods(bool instance_side) {
  if (TraceApplyChange) {
    std->print(" updating %s-side methods for: ", instance_side ? "instance" : "class");
    old_klass()->print_value();
    std->cr();
  }

  if (instance_side) {
    old_klass()->klass_part()->flush_methods();
    old_mixin()->set_methods(new_mixin()->methods());
  } else {
    old_klass()->klass()->klass_part()->flush_methods();
    old_mixin()->class_mixin()->set_methods(new_mixin()->class_mixin()->methods());
  }
}

void ClassChange::update_class(bool class_vars_changed,
			       bool instance_methods_changed,
		               bool class_methods_changed) {
  // The format has not changed which means we can patch the existing classes and mixins
  // We only have to change classes using the old_mixin
  if (old_mixin() == new_mixin()) return;

  // This is an invocation 
  if (class_vars_changed)
    update_class_vars();
  if (instance_methods_changed)
    update_methods(true);
  if (class_methods_changed)
    update_methods(false);

  if (old_klass()->klass_part()->superKlass() != new_super()) {
    old_klass()->klass_part()->set_superKlass(new_super());
  }
}

void ClassChange::setup_schema_change() {

  // Setup the new super class if we have super change
  if (_super_change) {
    _new_super = super_change()->new_klass();
    assert(_new_super != NULL, "super class must exist");
  }

  // Create new class
  _new_klass = new_class_from(old_klass(), new_super(), new_mixin(), new_format(), old_mixin());

  // Create the converter
  _converter = create_converter_for(old_klass(), new_klass());

  // Set forward pointer in old_class
  Reflection::forward(old_klass(), new_klass());

  // Set forward pointer in old_class class
  Reflection::forward(old_klass()->klass(), new_klass()->klass());

  // Set forward pointer in mixin
  Reflection::forward(old_klass()->klass_part()->mixin(), new_klass()->klass_part()->mixin());

  // Set forward pointer in mixinClass
  Reflection::forward(old_klass()->klass()->klass_part()->mixin(), new_klass()->klass()->klass_part()->mixin());
}

bool ClassChange::compute_needed_schema_change() {
  // Be dependent on the super change 
  if (   new_super()->is_klass()
      && !new_super()->klass_part()->has_same_layout_as(old_klass()->klass_part()->superKlass())) {
    set_reason_for_schema_change("super class has changed");
    return true;
  }

  // Has the format changed
  if (   new_format() != Klass::special_klass
      && new_format() != old_klass()->klass_part()->format()) {
    set_reason_for_schema_change("format of class has changed");
    return true;
  }

  // Check if we've changed the of instance variables
  if (new_mixin()->number_of_instVars() != old_mixin()->number_of_instVars()) {
    set_reason_for_schema_change("number of instance variables have changed");
    return true;
  }

  for (int index = new_mixin()->number_of_instVars(); index > 0; index--) {
    if (new_mixin()->instVar_at(index) != old_mixin()->instVar_at(index)) {
      set_reason_for_schema_change("instance variables have changed");
      return true;
    }
  }
  return false;
}

bool Reflection::needs_schema_change() {
  bool result = false;
  for (int index = 0; index < class_changes->length(); index++) {
    bool sub_result = class_changes->at(index)->needs_schema_change();
    if (TraceApplyChange && sub_result) {
      class_changes->at(index)->old_klass()->print_value();
      std->cr();
      std->print_cr("  needs schema change because: %s.", class_changes->at(index)->reason_for_schema_change());
    }
    result = result || sub_result;
  }
  return result;
}

void Reflection::forward(memOop old_obj, memOop new_obj) {
  if (old_obj == new_obj) return;
  if (old_obj->is_forwarded()) {
    if (old_obj->forwardee() != new_obj) {
      fatal("inconsistent forwarding");
    }
    return;
  }
  old_obj->forward_to(new_obj);
  converted->append(old_obj);
}

bool Reflection::has_methods_changed(mixinOop new_mixin, mixinOop old_mixin) {
  if (new_mixin->number_of_methods() != old_mixin->number_of_methods())
    return true;
  
  for (int index = 1; index <= new_mixin->number_of_methods(); index++) {
    if (!old_mixin->includes_method(new_mixin->method_at(index))) 
      return true;
  }
  return false;
}

bool Reflection::has_class_vars_changed(mixinOop new_mixin, mixinOop old_mixin) {
  if (new_mixin->number_of_classVars() != old_mixin->number_of_classVars())
    return true;
  
  for (int index = 1; index <= new_mixin->number_of_classVars(); index++) {
    if (!old_mixin->includes_classVar(new_mixin->classVar_at(index))) 
      return true;
  }
  return false;
}


ClassChange* Reflection::find_change_for(klassOop klass) {
  for (int index = 0; index < class_changes->length(); index++) {
    ClassChange* e = class_changes->at(index);
    if (e->old_klass() == klass) return e;
  }
  return NULL;
}

void Reflection::register_class_changes(mixinOop    new_mixin,
			                objArrayOop invocations) {

  class_changes = new GrowableArray<ClassChange*>(100);
  int length = invocations->length();
  for (int index = invocations_offset(); index <= length; index++) {
    objArrayOop invocation = objArrayOop(invocations->obj_at(index));
    assert(invocation->is_objArray(), "type check");


    ClassChange* change = new ClassChange(klassOop(invocation->obj_at(1)),
                                          new_mixin,
                                          Klass::format_from_symbol(symbolOop(invocation->obj_at(2))),
                                          klassOop(invocation->obj_at(3)));
    change->set_super_change(find_change_for(change->old_klass()->klass_part()->superKlass()));
    class_changes->append(change);

    for (int j = 4; j <= invocation->length() - 1; j += 2) {
      klassOop old_klass = klassOop(invocation->obj_at(j));
      change = new ClassChange(old_klass,
	                Klass::format_from_symbol(symbolOop(invocation->obj_at(j+1))));
      change->set_super_change(find_change_for(old_klass->klass_part()->superKlass()));
      class_changes->append(change);
    }
  }
}

void Reflection::invalidate_classes(bool value) {
  for (int index = 0; index < class_changes->length(); index++) {
    klassOop old_klass = class_changes->at(index)->old_klass();
    old_klass->set_invalid(value);
    old_klass->klass()->set_invalid(value);
  }
}

void Reflection::update_classes(bool class_vars_changed, bool instance_methods_changed, bool class_methods_changed) {
  for (int index = 0; index < class_changes->length(); index++) {
    class_changes->at(index)->update_class(class_vars_changed, instance_methods_changed, class_methods_changed);
  }
}

void Reflection::setup_schema_change() {
  for (int index = 0; index < class_changes->length(); index++) {
    class_changes->at(index)->setup_schema_change();
  }
  for (index = 0; index < class_changes->length(); index++) {
    // Mark old class for schema change
    class_changes->at(index)->old_klass()->klass_part()->mark_for_schema_change();
  }
}

void Reflection::apply_change(mixinOop    new_mixin,
			      mixinOop    old_mixin,
			      objArrayOop invocations) {
  ResourceMark rm;
  if (TraceApplyChange) {
    std->print("Reflective change");
    std->print_cr("[new]"); 
    new_mixin->print();
    std->print_cr("[old]"); 
    old_mixin->print();
    std->print_cr("[invocations]"); 
    invocations->print();
    Universe::verify();
  }

  register_class_changes(new_mixin, invocations);

  invalidate_classes(true);

  // Invalidate compiled code
  Universe::code->mark_dependents_for_deoptimization();
  Processes::deoptimized_wrt_marked_nmethods();
  Universe::code->make_marked_nmethods_zombies();

  // check for change mixin format too
  bool format_changed = needs_schema_change();

  bool class_vars_changed       = has_class_vars_changed(new_mixin, old_mixin);
  bool instance_methods_changed = has_methods_changed(new_mixin,    old_mixin);
  bool class_methods_changed    = has_methods_changed(new_mixin->class_mixin(), old_mixin->class_mixin());

  if (format_changed) {
    if (TraceApplyChange) {
      std->print_cr(" - schema change is needed");
    }

    converted = new GrowableArray<memOop> (100);

    setup_schema_change();
    
    // Do the transformation
    ConvertOopClosure blk;
    ConvertClosure    bl;
    Universe::roots_do(&convert);
    Processes::oop_iterate(&bl);

    // Save top of to_space and old_gen
    NewWaterMark eden_mark = Universe::new_gen.eden()->top_mark();
    OldWaterMark old_mark  = Universe::old_gen.top_mark();

    Universe::new_gen.object_iterate(&blk);
    Universe::old_gen.object_iterate(&blk);

    while (   eden_mark != Universe::new_gen.eden()->top_mark()
           || old_mark  != Universe::old_gen.top_mark()) {
      Universe::new_gen.eden()->object_iterate_from(&eden_mark, &blk);
      Universe::old_gen.object_iterate_from(&old_mark, &blk);
    }

    // NotificationQueue::oops_do(&follow_root);

    // Reset the marks for the converted objects
    for (int j = 0; j < converted->length(); j++) {
      memOop obj = converted->at(j);
      if (TraceApplyChange) {
        std->print_cr("Old: 0x%lx, 0x%lx", obj,              obj->mark());
        std->print_cr("New: 0x%lx, 0x%lx", obj->forwardee(), obj->forwardee()->mark());
      }
      obj->set_mark(obj->forwardee()->mark());
    }

    // Clear the static variables
    converted = NULL;

  } else {
    if (TraceApplyChange) {
      std->print_cr(" - no schema change (%s%s%s)",
	class_vars_changed        ? "class variables "  : "",
	instance_methods_changed  ? "instance methods " : "",
	class_methods_changed     ? "class methods "    : "");
    }
    update_classes(class_vars_changed, instance_methods_changed, class_methods_changed);
  }

  invalidate_classes(false);
  class_changes = NULL;

  // Clear inline caches
  Universe::flush_inline_caches_in_methods();
  Universe::code->clear_inline_caches();

  lookupCache::flush();
  DeltaCallCache::clearAll();

  if (TraceApplyChange) 
    Universe::verify();
}

oop Reflection::apply_change(objArrayOop change) {
   TraceTime t("ApplyChange", TraceApplyChange);
  
  // [1]     = new-mixin   <Mixin>
  // [2]     = old-mixin   <Mixin>
  // [3 - n] = invocations <Array>

  // Check array format
  int length = change->length();

  if (length < 3)
    return markSymbol(vmSymbols::argument_is_invalid());

  mixinOop new_mixin = mixinOop(change->obj_at(1));
  if (!new_mixin->is_mixin()) 
    return markSymbol(vmSymbols::argument_is_invalid());

  mixinOop old_mixin = mixinOop(change->obj_at(2));
  if (!old_mixin->is_mixin()) 
    return markSymbol(vmSymbols::argument_is_invalid());

  for (int index = 3; index <= length; index++) {
    objArrayOop array = objArrayOop(change->obj_at(index));

    if (!array->is_objArray())
      return markSymbol(vmSymbols::argument_is_invalid());

    if (array->length() < 3)      
      return markSymbol(vmSymbols::argument_is_invalid());

    if (!array->obj_at(1)->is_klass()) 
      return markSymbol(vmSymbols::argument_is_invalid());

    if (!array->obj_at(2)->is_symbol()) 
      return markSymbol(vmSymbols::argument_is_invalid());

    if (!array->obj_at(3)->is_klass() && array->obj_at(3) != nilObj) 
      return markSymbol(vmSymbols::argument_is_invalid());

    for (int j = 4; j <= array->length() - 1; j += 2) {
     if (!array->obj_at(j)->is_klass()) 
       return markSymbol(vmSymbols::argument_is_invalid());
     if (!array->obj_at(j+1)->is_symbol()) 
      return markSymbol(vmSymbols::argument_is_invalid());
    }
  }

  apply_change(new_mixin, old_mixin, change);
  return trueObj;
}

void Reflection::convert(oop* p) {
  oop obj = *p;

  if (!obj->is_mem())
    return;

  if (memOop(obj)->is_forwarded()) {
    Universe::store(p, memOop(obj)->forwardee());
    return;
  }

  if (memOop(obj)->klass()->klass_part()->is_marked_for_schema_change()) {
    Universe::store(p, convert_object(memOop(obj)));
  }
}

memOop Reflection::convert_object(memOop obj) {
  assert(obj->klass()->klass_part()->is_marked_for_schema_change(), "just checking");
  ClassChange* change = find_change_for(obj->klass());
  assert(change, "change must be present");
  return change->converter()->convert(obj);
}
