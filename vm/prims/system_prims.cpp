/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.61 $ */
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
# include "incls/_system_prims.cpp.incl"

TRACE_FUNC(TraceSystemPrims, "system")

int systemPrimitives::number_of_calls;

PRIM_DECL_5(systemPrimitives::createNamedInvocation, oop mixin, oop name, oop primary, oop superclass, oop format) {
  PROLOGUE_5("createNamedInvocation", mixin, primary, name, superclass, format)

  // Check argument types
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!name->is_symbol())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!(primary == trueObj || primary == falseObj))
    return markSymbol(vmSymbols::third_argument_has_wrong_type());
  if (!superclass->is_klass())
    return markSymbol(vmSymbols::fourth_argument_has_wrong_type());
  if (!format->is_symbol())
    return markSymbol(vmSymbols::fifth_argument_has_wrong_type());

  Klass::Format f = Klass::format_from_symbol(symbolOop(format));
  if (f == Klass::no_klass) {
    return markSymbol(vmSymbols::argument_is_invalid());
  }

  BlockScavenge bs;

  // Create the new klass
  klassOop new_klass = klassOop(superclass)->klass_part()->create_subclass(mixinOop(mixin), f);

  if (new_klass == NULL) {
    // Create more detailed error message
    return markSymbol(vmSymbols::argument_is_invalid());
  }

  // Set the primary invocation if needed.
  if (primary == trueObj) {
    mixinOop(mixin)->set_primary_invocation(new_klass);
    mixinOop(mixin)->class_mixin()->set_primary_invocation(new_klass->klass());
    mixinOop(mixin)->set_installed(trueObj);
    mixinOop(mixin)->class_mixin()->set_installed(trueObj);
  }

  // Make sure mixin->classMixin is present

  // Add the global
  Universe::add_global(oopFactory::new_association(symbolOop(name), new_klass, true));
  return new_klass;
}

PRIM_DECL_3(systemPrimitives::createInvocation, oop mixin, oop superclass, oop format) {
  PROLOGUE_3("createInvocation", mixin, superclass, format)

  // Check argument types
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!superclass->is_klass())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!format->is_symbol())
    return markSymbol(vmSymbols::third_argument_has_wrong_type());

  BlockScavenge bs;

  Klass::Format f = Klass::format_from_symbol(symbolOop(format));
  if (f == Klass::no_klass) {
    return markSymbol(vmSymbols::argument_is_invalid());
  }

  klassOop new_klass = klassOop(superclass)->klass_part()->create_subclass(mixinOop(mixin), f);

  if (new_klass == NULL) {
    // Create more detailed error message
    return markSymbol(vmSymbols::argument_is_invalid());
  }

  mixinOop(mixin)->set_installed(trueObj);
  mixinOop(mixin)->class_mixin()->set_installed(trueObj);

  return new_klass;
}

PRIM_DECL_1(systemPrimitives::applyChange, oop change) {
  PROLOGUE_1("applyChange", change)
  // Check argument types
  if (!change->is_objArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;

  return Reflection::apply_change(objArrayOop(change));
}

PRIM_DECL_1(systemPrimitives::scavenge, oop receiver) {
  PROLOGUE_1("scavenge", receiver)
  oop rec = receiver;
  VM_Scavenge op(&rec);
  // The operation takes place in the vmProcess
  VMProcess::execute(&op);
  return rec;
}

PRIM_DECL_0(systemPrimitives::oopSize) {
  PROLOGUE_0("oopSize")
  return as_smiOop(::oopSize);
}

PRIM_DECL_1(systemPrimitives::garbageGollect, oop receiver) {
  PROLOGUE_1("garbageGollect", receiver);
  oop rec = receiver;
  VM_GarbageCollect op(&rec);
  // The operation takes place in the vmProcess
  VMProcess::execute(&op);
  return rec;
}

PRIM_DECL_1(systemPrimitives::expandMemory, oop sizeOop) {
  PROLOGUE_1("expandMemory", sizeOop);
  if (!sizeOop->is_smi())
    return markSymbol(vmSymbols::argument_has_wrong_type());
  int size = smiOop(sizeOop)->value();
  if (size < 0)
    return markSymbol(vmSymbols::argument_is_invalid());
  Universe::old_gen.expand(size);
  return trueObj;
}

PRIM_DECL_1(systemPrimitives::shrinkMemory, oop sizeOop) {
  PROLOGUE_1("shrinkMemory", sizeOop);
  if (!sizeOop->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (smiOop(sizeOop)->value() < 0 || smiOop(sizeOop)->value() > Universe::old_gen.free())
    return markSymbol(vmSymbols::value_out_of_range());
  Universe::old_gen.shrink(smiOop(sizeOop)->value());
  return trueObj;
}

extern "C" int expansion_count;
extern "C" void single_step_handler();

PRIM_DECL_0(systemPrimitives::expansions) {
  PROLOGUE_0("expansions")
  return as_smiOop(expansion_count);
}

PRIM_DECL_0(systemPrimitives::breakpoint) {
  PROLOGUE_0("breakpoint")
  {
    ResourceMark rm;
    StubRoutines::setSingleStepHandler(&single_step_handler);
    dispatchTable::intercept_for_step(NULL);
  }
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::vmbreakpoint) {
  PROLOGUE_0("vmbreakpoint")
  os::breakpoint();
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::halt) {
  PROLOGUE_0("halt")

  // i think this is obsolete, hlt is a priviligied instruciton, and Object>>halt uses
  // Processor stopWithError: ProcessHaltError new.
  // removed because inline assembly isn't portable -Marc 04/07

  Unimplemented();
  PRIM_NOT_IMPLEMENTED

//  __asm hlt
//  return trueObj;
}

static oop fake_time() {
  static int time = 0;
  return oopFactory::new_double((double) time++);
}

PRIM_DECL_0(systemPrimitives::userTime) {
  PROLOGUE_0("userTime")
  if (UseTimers) {
    os::updateTimes();
    return oopFactory::new_double(os::userTime());
  } else {
    return fake_time();
  }
}

PRIM_DECL_0(systemPrimitives::systemTime) {
  PROLOGUE_0("systemTime")
  if (UseTimers) {
    os::updateTimes();
    return oopFactory::new_double(os::systemTime());
  } else {
    return fake_time();
  }
}

PRIM_DECL_0(systemPrimitives::elapsedTime) {
  PROLOGUE_0("elapsedTime")
  if (UseTimers) {
    return oopFactory::new_double(os::elapsedTime());
  } else {
    return fake_time();
  }
}

PRIM_DECL_1(systemPrimitives::writeSnapshot, oop fileName) {
  PROLOGUE_1("writeSnapshot", fileName);
  SnapshotDesc sd;
  char* name = "fisk.snap";
  sd.write_on(name);
  if (sd.has_error()) return markSymbol(sd.error_symbol());
  return fileName;
}

PRIM_DECL_1(systemPrimitives::globalAssociationKey, oop receiver) {
  PROLOGUE_1("globalAssociationKey", receiver);
  assert(receiver->is_association(), "receiver must be association");
  return associationOop(receiver)->key();
}

PRIM_DECL_2(systemPrimitives::globalAssociationSetKey, oop receiver, oop key) {
  PROLOGUE_2("globalAssociationSetKey", receiver,  key);
  assert(receiver->is_association(), "receiver must be association");
  if (!key->is_symbol())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  associationOop(receiver)->set_key(symbolOop(key));
  return receiver;
}

PRIM_DECL_1(systemPrimitives::globalAssociationValue, oop receiver) {
  PROLOGUE_1("globalAssociationValue", receiver);
  assert(receiver->is_association(), "receiver must be association");
  return associationOop(receiver)->value();
}

PRIM_DECL_2(systemPrimitives::globalAssociationSetValue, oop receiver, oop value) {
  PROLOGUE_2("globalAssociationSetValue", receiver, value);
  assert(receiver->is_association(), "receiver must be association");
  associationOop(receiver)->set_value(value);
  return receiver;
}

PRIM_DECL_1(systemPrimitives::globalAssociationIsConstant, oop receiver) {
  PROLOGUE_1("globalAssociationIsConstant", receiver);
  assert(receiver->is_association(), "receiver must be association");
  return associationOop(receiver)->is_constant() ? trueObj : falseObj;
}

PRIM_DECL_2(systemPrimitives::globalAssociationSetConstant, oop receiver, oop value) {
  PROLOGUE_2("globalAssociationSetConstant", receiver, value);
  assert(receiver->is_association(), "receiver must be association");
  oop old_value = associationOop(receiver)->is_constant() ? trueObj : falseObj;

       if (value == trueObj)  associationOop(receiver)->set_is_constant(true);
  else if (value == falseObj) associationOop(receiver)->set_is_constant(false);
  else return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return old_value;
}

PRIM_DECL_1(systemPrimitives::smalltalk_at, oop index) {
  PROLOGUE_1("smalltalk_at", index);
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (!Universe::systemDictionaryObj()->is_within_bounds(smiOop(index)->value()))
     return markSymbol(vmSymbols::out_of_bounds());

  return Universe::systemDictionaryObj()->obj_at(smiOop(index)->value());
}

PRIM_DECL_2(systemPrimitives::smalltalk_at_put, oop key, oop value) {
  PROLOGUE_2("smalltalk_at_put", key, value);

  BlockScavenge bs;
  associationOop assoc = oopFactory::new_association(symbolOop(key), value, false);
  Universe::add_global(assoc);
  return assoc;
}

PRIM_DECL_1(systemPrimitives::smalltalk_remove_at, oop index) {
  PROLOGUE_1("smalltalk_remove_at", index);

  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (!Universe::systemDictionaryObj()->is_within_bounds(smiOop(index)->value()))
     return markSymbol(vmSymbols::out_of_bounds());

  BlockScavenge bs;

  associationOop assoc = associationOop(Universe::systemDictionaryObj()->obj_at(smiOop(index)->value()));
  Universe::remove_global_at(smiOop(index)->value());
  return assoc;
}

PRIM_DECL_0(systemPrimitives::smalltalk_size) {
  PROLOGUE_0("smalltalk_size");
  return as_smiOop(Universe::systemDictionaryObj()->length());
}

PRIM_DECL_0(systemPrimitives::smalltalk_array) {
  PROLOGUE_0("smalltalk_array");
  return Universe::systemDictionaryObj();
}

PRIM_DECL_0(systemPrimitives::quit) {
  PROLOGUE_0("quit");
  exit(0);
  return badOop;
}

PRIM_DECL_0(systemPrimitives::printPrimitiveTable) {
  PROLOGUE_0("printPrimitiveTable");
  primitives::print_table();
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::print_memory) {
  PROLOGUE_0("print_memory");
  Universe::print();
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::print_zone) {
  PROLOGUE_0("print_zone");
  Universe::code->print();
  return trueObj;
}

PRIM_DECL_1(systemPrimitives::defWindowProc, oop resultProxy) {
  PROLOGUE_1("defWindowProc", resultProxy);
  if (!resultProxy->is_proxy())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  std->print_cr("Please use the new Platform DLLLookup system to retrieve DefWindowProcA");
  dll_func func = DLLs::lookup(oopFactory::new_symbol("user"), oopFactory::new_symbol("DefWindowProcA"));
  proxyOop(resultProxy)->set_pointer((void*)func);
  return resultProxy;
}

PRIM_DECL_1(systemPrimitives::windowsHInstance, oop resultProxy) {
  PROLOGUE_1("windowsHInstance", resultProxy);
  if (!resultProxy->is_proxy())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  proxyOop(resultProxy)->set_pointer(os::get_hInstance());
  return resultProxy;
}

PRIM_DECL_1(systemPrimitives::windowsHPrevInstance, oop resultProxy) {
  PROLOGUE_1("windowsHPrevInstance", resultProxy);
  if (!resultProxy->is_proxy())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  proxyOop(resultProxy)->set_pointer(os::get_prevInstance());
  return resultProxy;
}

PRIM_DECL_0(systemPrimitives::windowsNCmdShow) {
  PROLOGUE_0("windowsNCmdShow");
  return as_smiOop(os::get_nCmdShow());
}

PRIM_DECL_1(systemPrimitives::characterFor, oop value) {
  PROLOGUE_1("characterFor", value);

  // check value type
  if (!value->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if ((unsigned int) smiOop(value)->value() < 256)
    // return the n+1'th element in asciiCharacter
    return Universe::asciiCharacters()->obj_at(smiOop(value)->value() + 1);
  else return markSymbol(vmSymbols::out_of_bounds());
}

PRIM_DECL_0(systemPrimitives::traceStack) {
  PROLOGUE_0("traceStack");
  DeltaProcess::active()->trace_stack();
  return trueObj;
}

// Flat Profiler Primitives

PRIM_DECL_0(systemPrimitives::flat_profiler_reset) {
  PROLOGUE_0("flat_profiler_reset");
  FlatProfiler::reset();
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::flat_profiler_process) {
  PROLOGUE_0("flat_profiler_process");
  DeltaProcess* proc = FlatProfiler::process();
  return proc == NULL ? nilObj : proc->processObj();
}

PRIM_DECL_1(systemPrimitives::flat_profiler_engage, oop process) {
  PROLOGUE_1("flat_profiler_engage", process);

  // check value type
  if (!process->is_process())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  FlatProfiler::engage(processOop(process)->process());
  return process;
}

PRIM_DECL_0(systemPrimitives::flat_profiler_disengage) {
  PROLOGUE_0("flat_profiler_disengage");
  DeltaProcess* proc = FlatProfiler::disengage();
  return proc == NULL ? nilObj : proc->processObj();
}

PRIM_DECL_0(systemPrimitives::flat_profiler_print) {
  PROLOGUE_0("flat_profiler_print");
  FlatProfiler::print(0);
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::notificationQueueGet) {
  PROLOGUE_0("notificationQueueGet");
  if (NotificationQueue::is_empty())
    return markSymbol(vmSymbols::empty_queue());
  return NotificationQueue::get();
}


PRIM_DECL_1(systemPrimitives::notificationQueuePut, oop value) {
  PROLOGUE_1("notificationQueuePut", value);
  NotificationQueue::put(value);
  return value;
}

PRIM_DECL_1(systemPrimitives::hadNearDeathExperience, oop value) {
  PROLOGUE_1("hadNearDeathExperience", value);
  return (value->is_mem() && memOop(value)->mark()->is_near_death())
         ? trueObj : falseObj;
}


PRIM_DECL_2(systemPrimitives::dll_setup, oop receiver, oop selector) {
  PROLOGUE_2("dll_setup", receiver, selector);

  if (!selector->is_symbol())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  if (symbolOop(selector)->number_of_arguments() != 2)
    return markSymbol(vmSymbols::failed());

  Universe::set_dll_lookup(receiver, symbolOop(selector));
  return receiver;
}

PRIM_DECL_3(systemPrimitives::dll_lookup, oop name, oop library, oop result) {
  PROLOGUE_3("dll_lookup", name, library, result);

  if (!name->is_symbol())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (!library->is_proxy())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  if (!result->is_proxy())
    return markSymbol(vmSymbols::third_argument_has_wrong_type());

  dll_func res = DLLs::lookup(symbolOop(name), (DLL*) proxyOop(library)->get_pointer());
  if (res) {
    proxyOop(result)->set_pointer((void*)res);
    return result;
  } else {
    return markSymbol(vmSymbols::not_found());
  }
}

PRIM_DECL_2(systemPrimitives::dll_load, oop name, oop library) {
  PROLOGUE_2("dll_load", name, library);

  if (!name->is_symbol())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (!library->is_proxy())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  DLL* res = DLLs::load(symbolOop(name));
  if (res) {
    proxyOop(library)->set_pointer(res);
    return library;
  } else {
    return markSymbol(vmSymbols::not_found());
  }
}

PRIM_DECL_1(systemPrimitives::dll_unload, oop library) {
  PROLOGUE_1("dll_unload", library);

  if (!library->is_proxy())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  return DLLs::unload((DLL*) proxyOop(library)->get_pointer())
       ? library
       : markSymbol(vmSymbols::failed());
}

// Inlining Database

PRIM_DECL_0(systemPrimitives::inlining_database_directory) {
  PROLOGUE_0("inlining_database_directory");
  return oopFactory::new_symbol(InliningDatabase::directory());
}

PRIM_DECL_1(systemPrimitives::inlining_database_set_directory, oop name) {
  PROLOGUE_1("inlining_database_set_directory", name);

  // Check type on argument
  if (!name->is_byteArray() && !name->is_doubleByteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  ResourceMark rm;

  int len = name->is_byteArray() ? byteArrayOop(name)->length() : doubleByteArrayOop(name)->length();
  char* str = NEW_C_HEAP_ARRAY(char, len+1);
  name->is_byteArray()
  ?       byteArrayOop(name)->copy_null_terminated(str, len+1)
  : doubleByteArrayOop(name)->copy_null_terminated(str, len+1);
  // Potential memory leak, but this is temporary
  InliningDatabase::set_directory(str);
  return trueObj;
}

PRIM_DECL_1(systemPrimitives::inlining_database_file_out_class, oop receiver_class) {
  PROLOGUE_1("inlining_database_file_out_class", receiver_class);

  // Check type on argument
  if (!receiver_class->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // File out the class
  return as_smiOop(InliningDatabase::file_out(klassOop(receiver_class)));
}

PRIM_DECL_0(systemPrimitives::inlining_database_file_out_all) {
  PROLOGUE_0("inlining_database_file_out_all");

  // File out all nmethods
  return as_smiOop(InliningDatabase::file_out_all());
}

PRIM_DECL_1(systemPrimitives::inlining_database_compile, oop file_name) {
  PROLOGUE_1("inlining_database_compile", file_name);

  // Check type on argument
  if (!file_name->is_byteArray() && !file_name->is_doubleByteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  ResourceMark rm;

  int len = file_name->is_byteArray() ? byteArrayOop(file_name)->length() : doubleByteArrayOop(file_name)->length();
  char* str = NEW_RESOURCE_ARRAY(char, len+1);
  file_name->is_byteArray()
  ?       byteArrayOop(file_name)->copy_null_terminated(str, len+1)
  : doubleByteArrayOop(file_name)->copy_null_terminated(str, len+1);

  RScope* rs = InliningDatabase::file_in(str);
  if (rs) {
    // Remove old nmethod if present
    nmethod* old_nm = Universe::code->lookup(rs->key());
    if (old_nm) {
      old_nm->makeZombie(false);
    }

    VM_OptimizeRScope op(rs);
    VMProcess::execute(&op);

    if (TraceInliningDatabase) {
      std->print_cr("compiling {%s} completed", str);
      std->print_cr("[Database]");
      rs->print_inlining_database_on(std, NULL, -1, 0);
      std->print_cr("[Compiled method]");
      op.result()->print_inlining_database_on(std);
    }
  } else {
    if (TraceInliningDatabase) {
       std->print_cr("compiling {%s} failed", str);
    }
  }
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::inlining_database_compile_next) {
  PROLOGUE_0("inlining_database_compile_next");
  if (!UseInliningDatabase) {
    return falseObj;
  }

  bool end_of_table;
  RScope* rs = InliningDatabase::select_and_remove(&end_of_table);
  if (rs) {
    VM_OptimizeRScope op(rs);
    VMProcess::execute(&op);
    if (TraceInliningDatabase) {
      std->print("Compiling ");
      op.result()->key.print_on(std);
      std->print_cr(" in background = 0x%lx", op.result());
    }
  }
  return end_of_table ? falseObj : trueObj;
}

PRIM_DECL_1(systemPrimitives::inlining_database_mangle, oop name) {
  PROLOGUE_1("inlining_database_mangle", name);

  // Check type on argument
  if (!name->is_byteArray() && !name->is_doubleByteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  ResourceMark rm;

  int len = name->is_byteArray() ? byteArrayOop(name)->length() : doubleByteArrayOop(name)->length();
  char* str = NEW_RESOURCE_ARRAY(char, len+1);
  name->is_byteArray()
  ?       byteArrayOop(name)->copy_null_terminated(str, len+1)
  : doubleByteArrayOop(name)->copy_null_terminated(str, len+1);
  return oopFactory::new_byteArray(InliningDatabase::mangle_name(str));
}


PRIM_DECL_1(systemPrimitives::inlining_database_demangle, oop name) {
  PROLOGUE_1("inlining_database_demangle", name);
  // Check type on argument
  if (!name->is_byteArray() && !name->is_doubleByteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  ResourceMark rm;

  int len = name->is_byteArray() ? byteArrayOop(name)->length() : doubleByteArrayOop(name)->length();
  char* str = NEW_RESOURCE_ARRAY(char, len+1);
  name->is_byteArray()
  ?       byteArrayOop(name)->copy_null_terminated(str, len+1)
  : doubleByteArrayOop(name)->copy_null_terminated(str, len+1);
  return oopFactory::new_byteArray(InliningDatabase::unmangle_name(str));
}

PRIM_DECL_2(systemPrimitives::inlining_database_add_entry, oop receiver_class, oop method_selector) {
  PROLOGUE_2("inlining_database_add_entry", receiver_class, method_selector);

  // Check type of argument
  if (!receiver_class->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // Check type of argument
  if (!method_selector->is_symbol())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  LookupKey key(klassOop(receiver_class), method_selector);
  InliningDatabase::add_lookup_entry(&key);
  return receiver_class;
}

PRIM_DECL_0(systemPrimitives::sliding_system_average) {
  PROLOGUE_0("system_sliding_average");

  if (!UseSlidingSystemAverage)
    return markSymbol(vmSymbols::not_active());

  unsigned int* array = SlidingSystemAverage::update();

  objArrayOop result = oopFactory::new_objArray(SlidingSystemAverage::number_of_cases-1);

  for (int index = 1; index < SlidingSystemAverage::number_of_cases; index++) {
    result->obj_at_put(index, as_smiOop(array[index]));
  }
  return result;
}

// Enumeration primitives
// - it is important to exclude contextOops since they should be invisible to the Smalltalk level.

class InstancesOfClosure: public ObjectClosure {
 public:
  InstancesOfClosure(klassOop target, int limit) {
    this->result = new GrowableArray<oop>(100);
    this->target = target;
    this->limit  = limit;
  }

  int                 limit;
  klassOop            target;
  GrowableArray<oop>* result;

  void do_object(memOop obj) {
    if (obj->klass() == target) {
      if (result->length() < limit && !obj->is_context()) {
        result->append(obj);
      }
    }
  }
};

PRIM_DECL_2(systemPrimitives::instances_of, oop klass, oop limit) {
  PROLOGUE_2("instances_of", klass, limit);

  // Check type of argument
  if (!klass->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!limit->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  BlockScavenge bs;
  ResourceMark rm;

  InstancesOfClosure blk(klassOop(klass), smiOop(limit)->value());
  Universe::object_iterate(&blk);

  int length = blk.result->length();
  objArrayOop result = oopFactory::new_objArray(length);
  for (int index = 1; index <= length; index++) {
    result->obj_at_put(index, blk.result->at(index-1));
  }
  return result;
}

class ConvertClosure : public OopClosure {
  void do_oop(oop* o) {
    Reflection::convert(o);
  }
};

class HasReferenceClosure : public OopClosure {
  oop target;
 public:
  HasReferenceClosure(oop target) {
    this->target = target;
    this->result = false;
  }
  void do_oop(oop* o) {
    if (*o == target) result = true;
  }
  bool result;
};

class ReferencesToClosure: public ObjectClosure {
 public:
  ReferencesToClosure(oop target, int limit) {
    this->result = new GrowableArray<oop>(100);
    this->target = target;
    this->limit  = limit;
  }

  int                 limit;
  oop                 target;
  GrowableArray<oop>* result;

  bool has_reference(memOop obj) {
    HasReferenceClosure blk(target);
    obj->oop_iterate(&blk);
    return blk.result;
  }

  void do_object(memOop obj) {
    if (has_reference(obj)) {
      if (result->length() < limit && !obj->is_context()) {
        result->append(obj);
      }
    }
  }
};

PRIM_DECL_2(systemPrimitives::references_to, oop obj, oop limit) {
  PROLOGUE_2("references_to", obj, limit);

  // Check type of argument
  if (!limit->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  BlockScavenge bs;
  ResourceMark rm;

  ReferencesToClosure blk(obj, smiOop(limit)->value());
  Universe::object_iterate(&blk);

  int length = blk.result->length();
  objArrayOop result = oopFactory::new_objArray(length);
  for (int index = 1; index <= length; index++) {
    result->obj_at_put(index, blk.result->at(index-1));
  }
  return result;
}

class HasInstanceReferenceClosure : public OopClosure {
  klassOop target;
 public:
  HasInstanceReferenceClosure(klassOop target) {
    this->target = target;
    this->result = false;
  }
  void do_oop(oop* o) {
    if ((*o)->klass() == target) result = true;
  }
  bool result;
};

class ReferencesToInstancesOfClosure: public ObjectClosure {
 public:
  ReferencesToInstancesOfClosure(klassOop target, int limit) {
    this->result = new GrowableArray<oop>(100);
    this->target = target;
    this->limit  = limit;
  }

  int                 limit;
  klassOop            target;
  GrowableArray<oop>* result;

  bool has_reference(memOop obj) {
    HasInstanceReferenceClosure blk(target);
    obj->oop_iterate(&blk);
    return blk.result;
  }

  void do_object(memOop obj) {
    if (has_reference(obj)) {
      if (result->length() < limit && !obj->is_context()) {
        result->append(obj);
      }
    }
  }
};

PRIM_DECL_2(systemPrimitives::references_to_instances_of, oop klass, oop limit) {
  PROLOGUE_2("references_to_instances_of", klass, limit);

  // Check type of argument
  if (!klass->is_klass())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!limit->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  BlockScavenge bs;
  ResourceMark rm;

  ReferencesToInstancesOfClosure blk(klassOop(klass), smiOop(limit)->value());
  Universe::object_iterate(&blk);

  int length = blk.result->length();
  objArrayOop result = oopFactory::new_objArray(length);
  for (int index = 1; index <= length; index++) {
    result->obj_at_put(index, blk.result->at(index-1));
  }
  return result;
}

class AllObjectsClosure: public ObjectClosure {
 public:
  AllObjectsClosure(int limit) {
    this->result = new GrowableArray<oop>(20000);
    this->limit  = limit;
  }

  int                 limit;
  GrowableArray<oop>* result;

  void do_object(memOop obj) {
    if (result->length() < limit && !obj->is_context()) {
      result->append(obj);
    }
  }
};

PRIM_DECL_1(systemPrimitives::all_objects, oop limit) {
  PROLOGUE_1("all_objects", limit);

  // Check type of argument
  if (!limit->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  BlockScavenge bs;
  ResourceMark rm;

  AllObjectsClosure blk(smiOop(limit)->value());
  Universe::object_iterate(&blk);

  int length = blk.result->length();
  objArrayOop result = oopFactory::new_objArray(length);
  for (int index = 1; index <= length; index++) {
    result->obj_at_put(index, blk.result->at(index-1));
  }
  return result;
}

PRIM_DECL_0(systemPrimitives::flush_code_cache) {
  PROLOGUE_0("flush_code_cache");
  Universe::code->flush();
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::flush_dead_code) {
  PROLOGUE_0("flush_dead_code");
  Universe::code->flushZombies();
  return trueObj;
}

PRIM_DECL_0(systemPrimitives::command_line_args) {
  PROLOGUE_0("command_line_args");
  int argc = os::argc();
  char** argv = os::argv();
  objArrayOop result = oopFactory::new_objArray(argc);
  result->set_length(argc);
  for (int index = 0; index < argc; index++) {
    byteArrayOop arg = oopFactory::new_byteArray(argv[index]);
    result->obj_at_put(index + 1, arg);
  }
  return result;
}

PRIM_DECL_0(systemPrimitives::current_thread_id) {
  PROLOGUE_0("current_thread_id");
  return as_smiOop(os::current_thread_id());
}

PRIM_DECL_0(systemPrimitives::object_memory_size) {
  PROLOGUE_0("object_memory_size");
  return oopFactory::new_double(double(Universe::old_gen.used()) / Universe::old_gen.capacity());
}

PRIM_DECL_0(systemPrimitives::freeSpace) {
  PROLOGUE_0("freeSpace");
  return as_smiOop(Universe::old_gen.free());
}

PRIM_DECL_0(systemPrimitives::nurseryFreeSpace) {
  PROLOGUE_0("nurseryFreeSpace");
  return as_smiOop(Universe::new_gen.eden()->free());
}
