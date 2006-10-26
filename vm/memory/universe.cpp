/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.94 $ */
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
# include "incls/_universe.cpp.incl"

bool NeedScavenge  = false;
bool bootstrapping = true;

int BlockScavenge::counter = 0;


void Universe::genesis() {
  ResourceMark rm;

  /*
  std->print_cr("Delta version %d.%d%s (%s %s).",
                Universe::major_version(), Universe::minor_version(), Universe::beta_version(), 
		__DATE__, __TIME__);
  std->print_cr("Copyright 1994 - 1996, LongView Technologies L.L.C. All rights reserved.");
  std->print_cr("(use argument -? for a list of flags)");
  */
  
  std->cr();
  Disclaimer::print_disclaimer();
  std->print_cr("Version %d.%d (build %s %s)", Universe::major_version(), Universe::minor_version(), __DATE__, __TIME__);
  std->print_cr("(use argument -? for a list of flags)");
  std->cr();

  if (UseNewBackend | TryNewBackend)	std->print_cr("- VM using new backend (%s makeConformant)", UseNewMakeConformant ? "new" : "old");
  //if (Disclaimer::is_product())	Disclaimer::print_disclaimer();
  if (Disclaimer::is_debug())		std->print_cr("- VM is in debug mode");
  if (!Interpreter::is_optimized())	Interpreter::print_code_status();
  
  scavengeCount = 0;

  current_sizes.initialize();

  symbol_table = new symbolTable;

  age_table = new ageTable;

  // Reserve space for object heap 
  ReservedSpace rs(current_sizes.reserved_object_size);

  if (!rs.is_reserved()) {
    fatal("could not reserve enough space for object heap");
  }
 
  int new_size = ReservedSpace::page_align_size(current_sizes.eden_size + 2 * current_sizes.surv_size);

  ReservedSpace new_rs = rs.first_part(new_size);
  ReservedSpace old_rs = rs.last_part(new_size);

  new_gen.initialize(new_rs, current_sizes.eden_size, current_sizes.surv_size);
  old_gen.initialize(old_rs, current_sizes.old_size);

  assert(new_gen.high_boundary <= old_gen.low_boundary, "old space allocated lower than new space!");

  remembered_set = new rSet; // uses _boundary's

  tenuring_threshold = ageTable::table_size;	// don't tenure anything at first

  lookupCache::flush();

# ifdef DELTA_COMPILER
  code = new zone(current_sizes.code_size);
# endif
}


void Universe::cleanup_after_bootstrap() {
  objectIDTable::cleanup_after_bootstrap();

  Universe::_callBack_receiver = nilObj();
  Universe::_callBack_selector = symbolOop(nilObj());

  Universe::_dll_lookup_receiver = nilObj();
  Universe::_dll_lookup_selector = symbolOop(nilObj());  

  Universe::_pic_free_list = objArrayKlass::allocate_tenured_pic(number_of_interpreterPIC_sizes);

  Universe::_characterKlassObj = klassOop(find_global("Character"));
  // Check if all roots are valid.
  Universe::roots_do(Universe::check_root);
}


void Universe::check_root(oop* p) {
  if (*p == badOop) fatal("badOop found in roots");
}


void Universe::switch_pointers(oop from, oop to) {
  assert(from->is_mem() && to->is_mem(),
	 "unexpected kind of pointer switching");
  // FIX LATER  assert(!from->is_old() || to->is_old(),
  //	        "shouldn't be switching an old oop to a new oop");
  //APPLY_TO_VM_OOPS(SWITCH_POINTERS_TEMPLATE);
  new_gen.switch_pointers(from, to);
  old_gen.switch_pointers(from, to);
  // FIX LATER  processes->switch_pointers(from, to);
  symbol_table->switch_pointers(from, to);
}


memOop Universe::relocate(memOop p) {
  //APPLY_TO_SPACES(SPACE_OOP_RELOCATE_TEMPLATE);
  ShouldNotReachHere(); // oop not in any old space
  return NULL;
}


bool Universe::verify_oop(memOop p) {
  if (new_gen.eden()->contains(p)) return true;
  if (new_gen.from()->contains(p)) return true;
  if (old_gen.contains(p)) return true;
  if (new_gen.to()->contains(p)) {
    error("memOop %#lx is in to_space", p);
  } else {
    error("memOop %#lx not in any space", p);
  }
  return false;
}


void Universe::verify(bool postScavenge) {
  ResourceMark rm;
  lprintf("verifying ");
  lprintf("newgen: ");  new_gen.verify();
  lprintf("oldgen: ");  old_gen.verify();
  lprintf("r ");        remembered_set->verify(postScavenge);
  lprintf("S ");        symbol_table->verify();
  lprintf("P ");        Processes::verify();
  lprintf(" done\n");
}


void Universe::print() {
  std->print_cr("Memory:");
  new_gen.print();
  old_gen.print();
  if (WizardMode) {
    std->print_cr("  (threshold=%d)", tenuring_threshold);
  }
}


oop* Universe::object_start(oop* p) {
  if(new_gen.contains(p)) return new_gen.object_start(p);
  return old_gen.object_start(p);
}


class PrintClosure: public ObjectClosure {
  void do_object(memOop obj) { 
    PrintObjectClosure blk(std);
    blk.do_object(obj);
    obj->layout_iterate(&blk);
  }
};


void Universe::print_layout() {
  PrintClosure blk;
  object_iterate(&blk);
}


static void decode_method(methodOop method, klassOop klass) {
  if (WizardMode) {
    ResourceMark rm;
    method->print_codes();
  }
  { ResourceMark rm;
    method->print();
    prettyPrinter::print(method, klass);
  }
}


static void decode_klass(symbolOop name, klassOop klass) {
  // Klass methods
  { 
    objArrayOop f = klass->klass_part()->methods();
    for (int index = 1; index <= f->length(); index ++)
    decode_method(methodOop(f->obj_at(index)), klass);
  }
  // Mixin methods
  {
    objArrayOop f = klass->klass_part()->mixin()->methods();
    for (int index = 1; index <= f->length(); index ++)
    decode_method(methodOop(f->obj_at(index)), klass);
  }
}


void Universe::decode_methods() {
  int l = Universe::systemDictionaryObj()->length();
  for (int index = 1; index <= l; index++) {
     associationOop assoc = 
       (associationOop) Universe::systemDictionaryObj()->obj_at(index);
     if (assoc->value()->is_klass())
        decode_klass(assoc->key(), klassOop(assoc->value()));
  }
}


void Universe::object_iterate(ObjectClosure* blk) {
  new_gen.object_iterate(blk);
  old_gen.object_iterate(blk);
}


static OopClosure* the_blk;
static void the_func(oop* p) {
  the_blk->do_oop(p);
}


void Universe::root_iterate(OopClosure* blk) {
  the_blk = blk;
  Universe::oops_do(the_func);
}


// Traverses the system dictionary to find the 
// association referring the class or meta class
// and then prints the key.
void Universe::print_klass_name(klassOop k) {
  int l = systemDictionaryObj()->length();
  for (int index = 1; index <= l; index++) {
     associationOop assoc = 
       (associationOop) systemDictionaryObj()->obj_at(index);
     if (assoc->value() == k) {
     	assoc->key()->print_symbol_on();
	return;
     } else if (assoc->value()->klass() == k) {
        assoc->key()->print_symbol_on();
	lprintf(" class");
        return;
     }
  }
}


char* Universe::klass_name(klassOop k) {
  if (k == NULL) return "(NULL)";
  int l = systemDictionaryObj()->length();
  for (int index = 1; index <= l; index++) {
     associationOop assoc = 
       (associationOop) systemDictionaryObj()->obj_at(index);
     if (assoc->value() == k) {
     	return assoc->key()->as_string();
     } else if (assoc->value()->klass() == k) {
        symbolOop name = assoc->key();
	char* result = NEW_RESOURCE_ARRAY(char, name->length()+7);
	sprintf(result, "%s class", name->as_string());
        return result;
     }
  }
  // it's an unknown class (mixin invocation)
  klassOop super = k->klass_part()->superKlass();
  if (super != ::nilObj) {
    char* superName = klass_name(super);
    const char* txt = "unnamed class inheriting from ";
    char* result = NEW_RESOURCE_ARRAY(char, strlen(txt) + strlen(superName) + 2);
    sprintf(result, "%s%s", txt, superName);
    return result;
  } else {
    return "<top>";
  }
}


klassOop Universe::method_holder_of(methodOop m) {
  m = m->home();	// so block methods can be found, too
  int l = systemDictionaryObj()->length();
  for (int index = 1; index <= l; index++) {
     associationOop assoc = 
       (associationOop) systemDictionaryObj()->obj_at(index);
     if (assoc->value()->is_klass()) {
       klassOop k = klassOop(assoc->value());
       klassOop res;
       if ((res = k->klass_part()->lookup_method_holder_for(m)) != NULL) {
	 // note: do search this way because not all superclasses are in system dictionary
	 return res;
       } else if ((res = k->klass()->klass_part()->lookup_method_holder_for(m)) != NULL) {
         return res;	// in metaclass hierarchy
       }
     }
  }
  if (WizardMode) warning("could not find methodHolder of method %#x", m);
  return NULL;	
}


symbolOop Universe::find_global_key_for(oop value, bool* meta) {
  *meta = false;
  int l = systemDictionaryObj()->length();
  for (int index = 1; index <= l; index++) {
     associationOop assoc =  associationOop(systemDictionaryObj()->obj_at(index));
     if (assoc->is_constant() && assoc->value()->is_klass()) {
       if (assoc->value() == value) {
	 return assoc->key();
       } else {
         klassOop s = klassOop(assoc->value())->klass();
         if (s == value) {
           *meta = true;
           return assoc->key();
	 }
       }
     }
  }
  return NULL;
}


oop Universe::find_global(char* name, bool must_be_constant) {
  if (!must_be_constant) {
    if (strcmp(name, "true")  == 0 ) return trueObj();
    if (strcmp(name, "false") == 0 ) return falseObj();
    if (strcmp(name, "nil")   == 0 ) return nilObj();
  }

  symbolOop sym = oopFactory::new_symbol(name);

  int l = systemDictionaryObj()->length();
  for (int index = 1; index <= l; index++) {
     associationOop assoc = associationOop(systemDictionaryObj()->obj_at(index));
     if (assoc->key() == sym) {
       if (!must_be_constant || assoc->is_constant()) {
         return assoc->value();
       }
     }
  }
  return NULL;
}


associationOop Universe::find_global_association(char* name) {
  symbolOop sym = oopFactory::new_symbol(name);

  int l = systemDictionaryObj()->length();
  for (int index = 1; index <= l; index++) {
     associationOop assoc = 
       (associationOop) systemDictionaryObj()->obj_at(index);
     if (assoc->key() == sym) return assoc;
  }
  return NULL;
}


void Universe::methods_in_array_do(objArrayOop array, void f(methodOop method)) {
  int length = array->length();
  for (int index = 1; index <= length; index++) {
    methodOop method = methodOop(array->obj_at(index));
    assert(method->is_method(), "just checking");
    f(method);
  }
}

void Universe::methods_for_do(klassOop klass, void f(methodOop method)) {
  Klass* k = klass->klass_part();
  methods_in_array_do(k->methods(), f);
  methods_in_array_do(klass->klass()->klass_part()->methods(), f);
  if (k->is_named_class()) {
    // Fix the mixin parts
    methods_in_array_do(k->mixin()->methods(), f);
    methods_in_array_do(klass->klass()->klass_part()->mixin()->methods(), f);
  }
}

class MethodsClosure : public klassOopClosure {
  void (*f)(methodOop method);
public:  
  MethodsClosure(void f(methodOop method)) {
    this->f = f; 
  }
  void do_klass(klassOop klass) {
    Universe::methods_for_do(klass, f);
  }
};

void Universe::methods_do(void f(methodOop method)) {
  MethodsClosure it(f);
  classes_do(&it);
}

void Universe::classes_for_do(klassOop klass, klassOopClosure* iterator) {
  // call f with the class
  iterator->do_klass(klass);
  // recurse if the super class is a anonymous class
  if (!klass->klass_part()->has_superKlass()) return;
  if (!klass->klass_part()->superKlass()->klass_part()->is_named_class()) return;
  classes_for_do(klass->klass_part()->superKlass(), iterator);
}


void Universe::classes_do(klassOopClosure* iterator) {
  objArrayOop array  = Universe::systemDictionaryObj();
  int         length = array->length();
  for (int index = 1; index <= length; index++) {
    associationOop assoc = associationOop(array->obj_at(index));
    assert(assoc->is_association(), "just checking");
    if (assoc->is_constant() && assoc->value()->is_klass()) {
      classes_for_do(klassOop(assoc->value()), iterator);
    }
  }
}

void Universe::flush_inline_caches_in_method(methodOop method) {
  method->clear_inline_caches();
}

class FlushClosure: public ObjectClosure {
  void do_object(memOop obj) {
    if (obj->is_method()) 
      methodOop(obj)->clear_inline_caches();
  }
};

void Universe::flush_inline_caches_in_methods() {
  FlushClosure blk;
  object_iterate(&blk);
}

class AllMethodsClosure: public ObjectClosure {
  void (*f)(methodOop m);
 public:
  AllMethodsClosure(void f(methodOop m)) {
    this->f = f;
  }
   
  void do_object(memOop obj) {
    if (obj->is_method())
      f(methodOop(obj));
  }
};

void Universe::methodOops_do(void f(methodOop m)) {
  AllMethodsClosure blk(f);
  object_iterate(&blk);
}

static void cleanup_method(methodOop m) {
  m->cleanup_inline_caches();
}

void Universe::cleanup_all_inline_caches() {
  DeltaCallCache::clearAll();
  methodOops_do(cleanup_method);
  code->cleanup_inline_caches();
}

void universe_init() {
  Universe::genesis();
}

newGeneration Universe::new_gen;
oldGeneration Universe::old_gen;

symbolTable* Universe::symbol_table;
rSet*        Universe::remembered_set;
ageTable*    Universe::age_table;

#ifdef DELTA_COMPILER
zone* Universe::code;
#endif

spaceSizes Universe::current_sizes;

int Universe::tenuring_threshold;
int Universe::scavengeCount;

// Classes
klassOop smiKlassObj                           = klassOop(badOop);
klassOop contextKlassObj                       = klassOop(badOop);
klassOop doubleKlassObj                        = klassOop(badOop);
klassOop Universe::_memOopKlassObj             = klassOop(badOop);
klassOop Universe::_objArrayKlassObj           = klassOop(badOop);
klassOop Universe::_byteArrayKlassObj          = klassOop(badOop);
klassOop symbolKlassObj                        = klassOop(badOop);
klassOop Universe::_associationKlassObj        = klassOop(badOop);
klassOop zeroArgumentBlockKlassObj             = klassOop(badOop);
klassOop oneArgumentBlockKlassObj              = klassOop(badOop);
klassOop twoArgumentBlockKlassObj              = klassOop(badOop);
klassOop threeArgumentBlockKlassObj            = klassOop(badOop);
klassOop fourArgumentBlockKlassObj             = klassOop(badOop);
klassOop fiveArgumentBlockKlassObj             = klassOop(badOop);
klassOop sixArgumentBlockKlassObj              = klassOop(badOop);
klassOop sevenArgumentBlockKlassObj            = klassOop(badOop);
klassOop eightArgumentBlockKlassObj            = klassOop(badOop);
klassOop nineArgumentBlockKlassObj             = klassOop(badOop);
klassOop Universe::_methodKlassObj             = klassOop(badOop);
klassOop Universe::_characterKlassObj          = klassOop(badOop);
klassOop doubleValueArrayKlassObj              = klassOop(badOop);
klassOop Universe::_vframeKlassObj             = klassOop(badOop);

// Objects
oop         nilObj                             = oop(badOop);
oop         trueObj                            = oop(badOop);
oop         falseObj                           = oop(badOop);
objArrayOop Universe::_asciiCharacters         = objArrayOop(badOop);
objArrayOop Universe::_systemDictionaryObj     = objArrayOop(badOop);
objArrayOop Universe::_objectIDTable           = objArrayOop(badOop);
objArrayOop Universe::_pic_free_list           = objArrayOop(badOop); 
oop         Universe::_callBack_receiver       = oop(badOop);
symbolOop   Universe::_callBack_selector       = symbolOop(badOop);
oop         Universe::_dll_lookup_receiver     = oop(badOop);
symbolOop   Universe::_dll_lookup_selector     = symbolOop(badOop);
methodOop   Universe::_sweeper_method          = NULL;


void Universe::roots_do(void f(oop*)) {
  // External Objects
  f((oop*)&::nilObj);
  f((oop*)&::trueObj);
  f((oop*)&::falseObj);

  // External Classes
  f((oop*)&::smiKlassObj);
  f((oop*)&::contextKlassObj);
  f((oop*)&::doubleKlassObj);

  // Classes
  f((oop*)&_memOopKlassObj);
  f((oop*)&_objArrayKlassObj);
  f((oop*)&_byteArrayKlassObj);
  f((oop*)&::symbolKlassObj);
  f((oop*)&_associationKlassObj);
  f((oop*)&::zeroArgumentBlockKlassObj);
  f((oop*)&::oneArgumentBlockKlassObj);
  f((oop*)&::twoArgumentBlockKlassObj);
  f((oop*)&::threeArgumentBlockKlassObj);
  f((oop*)&::fourArgumentBlockKlassObj);
  f((oop*)&::fiveArgumentBlockKlassObj);
  f((oop*)&::sixArgumentBlockKlassObj);
  f((oop*)&::sevenArgumentBlockKlassObj);
  f((oop*)&::eightArgumentBlockKlassObj);
  f((oop*)&::nineArgumentBlockKlassObj);
  f((oop*)&_methodKlassObj);
  f((oop*)&_characterKlassObj);
  // fix f((oop*)&::doubleValueArrayKlassObj);

  // Objects
  f((oop*)&_asciiCharacters);
  f((oop*)&_systemDictionaryObj);
  f((oop*)&_objectIDTable);
  f((oop*)&_callBack_receiver);
  f((oop*)&_callBack_selector);
  f((oop*)&_dll_lookup_receiver);
  f((oop*)&_dll_lookup_selector);
  f((oop*)&_pic_free_list);

  f((oop*)&_sweeper_method);

  f((oop*)&_vframeKlassObj);
}


void Universe::oops_do(void f(oop*)) {
  // Iterate over the local roots
  roots_do(f);
  // Iterate over the zone
  code->oops_do(f);
  // Iterate over all handles
  Handles::oops_do(f);
  // Iterate over the oops in the inlining database
  InliningDatabase::oops_do(f);
}


void Universe::add_global(oop value) {
  _systemDictionaryObj = _systemDictionaryObj->copy_add(value);
}

void Universe::remove_global_at(int index) {
  _systemDictionaryObj = _systemDictionaryObj->copy_remove(index);
}

bool Universe::on_page_boundary(void* addr) {
  return ((int) addr) % page_size() == 0;
}

int Universe::page_size() {
  return os::vm_page_size();
}
