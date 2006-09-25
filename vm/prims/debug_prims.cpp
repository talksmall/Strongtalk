/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.49 $ */
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
# include "incls/_debug_prims.cpp.incl"

TRACE_FUNC(TraceDebugPrims, "debug")

int debugPrimitives::number_of_calls;

PRIM_DECL_1(debugPrimitives::boolAt, oop name) {
  PROLOGUE_1("boolAt", name)
  if (!name->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  bool result;
  if (debugFlags::boolAt(byteArrayOop(name)->chars(),
			 byteArrayOop(name)->length(),
			 &result))
    return result ? trueObj : falseObj;
  return markSymbol(vmSymbols::not_found());
}

PRIM_DECL_2(debugPrimitives::boolAtPut , oop name, oop value) {
  PROLOGUE_2("boolAtPut", name, value)
  if (!name->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  bool b;
  if      (value == trueObj)  b = true;
  else if (value == falseObj) b = false;
  else return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (debugFlags::boolAtPut(byteArrayOop(name)->chars(),
			    byteArrayOop(name)->length(),
			    &b))
    return b ? trueObj : falseObj;
  return markSymbol(vmSymbols::not_found());
}

PRIM_DECL_1(debugPrimitives::smiAt, oop name) {
  PROLOGUE_1("smiAt", name)
  if (!name->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  int result;
  if (debugFlags::intAt(byteArrayOop(name)->chars(),
			byteArrayOop(name)->length(),
			&result))
    return as_smiOop(result);
  return markSymbol(vmSymbols::not_found());
}

PRIM_DECL_2(debugPrimitives::smiAtPut , oop name, oop value) {
  PROLOGUE_2("smiAtPut", name, value)
  if (!name->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!value->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  int v = smiOop(value)->value();
  if (debugFlags::intAtPut(byteArrayOop(name)->chars(),
			   byteArrayOop(name)->length(),
			   &v))
    return as_smiOop(v);
  return markSymbol(vmSymbols::not_found());
}

PRIM_DECL_0(debugPrimitives::clearLookupCache) {
  PROLOGUE_0("clearLookupCache")
  lookupCache::flush();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::clearLookupCacheStatistics) {
  PROLOGUE_0("clearLookupCacheStatistics")
  lookupCache::clear_statistics();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::printLookupCacheStatistics) {
  PRIMITIVE_RETURNS_TRUE_IN_PRODUCT
  PROLOGUE_0("printLookupCacheStatistics")
  lookupCache::print_statistics();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::printMemoryLayout) {
  PRIMITIVE_RETURNS_TRUE_IN_PRODUCT
  PROLOGUE_0("printMemoryLayout")
  Universe::print_layout();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::decodeAllMethods) {
  PRIMITIVE_RETURNS_TRUE_IN_PRODUCT
  PROLOGUE_0("decodeAllMethods")
  Universe::decode_methods();
  return trueObj;
}

PRIM_DECL_2(debugPrimitives::printMethodCodes, oop receiver, oop sel) {
  PRIMITIVE_FAILS_IN_PRODUCT
  PROLOGUE_2("printMethodCodes", receiver, sel)
  if (!sel->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  symbolOop s = oopFactory::new_symbol(byteArrayOop(sel));
  methodOop m = receiver->blueprint()->lookup(s);
  if (!m) return markSymbol(vmSymbols::not_found());
  { ResourceMark rm;
     if (WizardMode) m->print_codes();
     prettyPrinter::print(methodOop(m), receiver->klass());
  }
  return receiver;
}

PRIM_DECL_2(debugPrimitives::generateIR, oop receiver, oop sel) {
  PRIMITIVE_FAILS_IN_PRODUCT
  std->print_cr("primitiveGenerateIR called...");
  ResourceMark rm;	// needed to avoid memory leaks!
  PROLOGUE_2("generateIR", receiver, sel)
  if (!sel->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  symbolOop s = oopFactory::new_symbol(byteArrayOop(sel));
  methodOop m = receiver->blueprint()->lookup(s);
  if (!m) return markSymbol(vmSymbols::not_found());
  LookupKey key(receiver->klass(), s);
# ifdef DELTA_COMPILER
  VMProcess::execute(&VM_OptimizeMethod(&key, m));
# endif
  return receiver;
}

PRIM_DECL_2(debugPrimitives::optimizeMethod, oop receiver, oop sel) {
  PRIMITIVE_FAILS_IN_PRODUCT
  PROLOGUE_2("optimizeMethod", receiver, sel);

  if (!sel->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  symbolOop s = oopFactory::new_symbol(byteArrayOop(sel));
  methodOop m = receiver->blueprint()->lookup(s);
  if (!m) return markSymbol(vmSymbols::not_found());

# ifdef DELTA_COMPILER
  LookupKey key(receiver->klass(), s);
  VM_OptimizeMethod op(&key, m);
  // The operation takes place in the vmProcess
  VMProcess::execute(&op);
# endif
  return receiver;
}

PRIM_DECL_2(debugPrimitives::decodeMethod, oop receiver, oop sel) {
  PRIMITIVE_FAILS_IN_PRODUCT
  PROLOGUE_2("decodeMethod", receiver, sel);
  if (!sel->is_symbol())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  LookupResult result = lookupCache::ic_normal_lookup(receiver->klass(), symbolOop(sel));
  if (result.is_empty())
    return markSymbol(vmSymbols::not_found());
  if (result.is_method()) {
    // methodOop found => print byte codes
   ResourceMark rm;
   result.method()->print_codes();
  } else {
    // nmethod found => print assembly code
   result.get_nmethod()->printCode();
  }
  return receiver;
}

PRIM_DECL_0(debugPrimitives::timerStart) {
  PROLOGUE_0("timerStart");
  os::timerStart();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::timerStop) {
  PROLOGUE_0("timerStop");
  os::timerStop();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::timerPrintBuffer) {
  PROLOGUE_0("timerPrintBuffer");
  os::timerPrintBuffer();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::interpreterInvocationCounterLimit) {
  PROLOGUE_0("interpreterInvocationCounterLimit");
  int limit = Interpreter::get_invocation_counter_limit();
  if (limit < smi_min) limit = smi_min;
  else if (limit > smi_max) limit = smi_max;
  return as_smiOop(limit);
}

PRIM_DECL_1(debugPrimitives::setInterpreterInvocationCounterLimit, oop limit) {
  PROLOGUE_1("setInterpreterInvocationCounterLimit", limit);
  if (!limit->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  int value = smiOop(limit)->value();
  if (value < 0 || value > methodOopDesc::_invocation_count_max)
    return markSymbol(vmSymbols::out_of_bounds());
  Interpreter::set_invocation_counter_limit(value);
  Interpreter::set_loop_counter_limit(value);	// for now - probably should have its own primitive
  return limit;
}

class ClearInvocationCounterClosure: public ObjectClosure {
  void do_object(memOop obj) {
    if (obj->is_method())
      methodOop(obj)->set_invocation_count(0);
  }
};

PRIM_DECL_0(debugPrimitives::clearInvocationCounters) {
  PROLOGUE_0("clearInvocationCounters");
  ClearInvocationCounterClosure blk;
  Universe::object_iterate(&blk);
  return trueObj;
}

// Collects all methods with invocation counter >= cutoff
class CollectMethodClosure: public ObjectClosure {
  GrowableArray <methodOop>* col;
  int                        cutoff;
 public:
  CollectMethodClosure(GrowableArray <methodOop>* col, int cutoff) {
    this->col    = col;
    this->cutoff = cutoff;
  }
  void do_object(memOop obj) {
    if (obj->is_method())
      if (methodOop(obj)->invocation_count() >= cutoff)
        col->push(methodOop(obj));
  }
};

static int compare_method_counters(methodOop* a,  methodOop* b) {
  return (*b)->invocation_count() - (*a)->invocation_count();
}

PRIM_DECL_1(debugPrimitives::printInvocationCounterHistogram, oop size) {
  PRIMITIVE_FAILS_IN_PRODUCT
  PROLOGUE_1("printInvocationCounterHistogram", size);
  if (!size->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  ResourceMark rm;
  GrowableArray <methodOop>* col = new GrowableArray<methodOop>(1024);
  // Collect the methods
  CollectMethodClosure blk(col, smiOop(size)->value());
  Universe::object_iterate(&blk);
  std->print_cr("Collected %d methods", col->length());
  // Sort the methods based on the invocation counters.
  col->sort(&compare_method_counters);
  // Print out the result
  for (int index = 0; index < col->length(); index++) {
    methodOop m = col->at(index);
    std->print("[%d] ", m->invocation_count());
    m->pretty_print();
  }
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::clearInlineCaches) {
  PROLOGUE_0("clearInlineCaches");
  Universe::flush_inline_caches_in_methods();
  Universe::code->clear_inline_caches();
  Universe::code->print();
  return trueObj;
}

#define FOR_ALL_NMETHOD(var) \
  for (nmethod *var = Universe::code->first_nm(); var; var = Universe::code->next_nm(var))

PRIM_DECL_0(debugPrimitives::clearNMethodCounters) {
  PROLOGUE_0("clearNMethodCounters");
  FOR_ALL_NMETHOD(nm) nm->set_invocation_count(0);
  return trueObj;
}

static int compare_nmethod_counters(nmethod** a, nmethod** b) {
  return (*b)->invocation_count() - (*a)->invocation_count();
}

PRIM_DECL_1(debugPrimitives::printNMethodCounterHistogram, oop size) {
  PRIMITIVE_FAILS_IN_PRODUCT
  PROLOGUE_1("printNMethodCounterHistogram", size);
  if (!size->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  ResourceMark rm;
  GrowableArray <nmethod*>* col = new GrowableArray<nmethod*>(1024);
  // Collect the nmethods
  FOR_ALL_NMETHOD(nm) col->push(nm);

  std->print_cr("Collected %d nmethods", col->length());
  // Sort the methods based on the invocation counters.
  col->sort(&compare_nmethod_counters);
  // Print out the result
  int end = (col->length() > smiOop(size)->value()) 
            ? smiOop(size)->value() : col->length();
  for (int index = 0; index < end; index++) {
    nmethod* m = col->at(index);
    std->print("[%d] ", m->invocation_count());
    m->scopes()->print_partition();
    m->method()->pretty_print();
  }
  return trueObj;
}

class SumMethodInvocationClosure: public ObjectClosure {
  int sum;
 public:
  SumMethodInvocationClosure() {
    sum = 0;
  }
  void do_object(memOop obj) {
    if (obj->is_method())
      sum += methodOop(obj)->invocation_count();
  }
  int result() { return sum; }
};

PRIM_DECL_0(debugPrimitives::numberOfMethodInvocations) {
  PRIMITIVE_RETURNS_ZERO_IN_PRODUCT
  PROLOGUE_0("numberOfMethodInvocations");
  SumMethodInvocationClosure blk;
  Universe::object_iterate(&blk);
  return as_smiOop(blk.result());
}

PRIM_DECL_0(debugPrimitives::numberOfNMethodInvocations) {
  PRIMITIVE_RETURNS_ZERO_IN_PRODUCT
  PROLOGUE_0("numberOfNMethodInvocations");
  int sum = 0;
  FOR_ALL_NMETHOD(nm) sum += nm->invocation_count();
  return as_smiOop(sum);
}

PRIM_DECL_0(debugPrimitives::numberOfPrimaryLookupCacheHits) {
  PRIMITIVE_RETURNS_ZERO_IN_PRODUCT
  PROLOGUE_0("numberOfPrimaryLookupCacheHits");
  return as_smiOop(lookupCache::number_of_primary_hits);
}

PRIM_DECL_0(debugPrimitives::numberOfSecondaryLookupCacheHits) {
  PRIMITIVE_RETURNS_ZERO_IN_PRODUCT
  PROLOGUE_0("numberOfSecondaryLookupCacheHits");
  return as_smiOop(lookupCache::number_of_secondary_hits);
}

PRIM_DECL_0(debugPrimitives::numberOfLookupCacheMisses) {
  PRIMITIVE_RETURNS_ZERO_IN_PRODUCT
  PROLOGUE_0("numberOfLookupCacheMisses");
  return as_smiOop(lookupCache::number_of_misses);
}


PRIM_DECL_0(debugPrimitives::clearPrimitiveCounters){
  PRIMITIVE_RETURNS_TRUE_IN_PRODUCT
  PROLOGUE_0("clearPrimitiveCounters");
  primitives::clear_counters();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::printPrimitiveCounters){
  PRIMITIVE_RETURNS_TRUE_IN_PRODUCT
  PROLOGUE_0("printPrimitiveCounters");
  primitives::print_counters();
  return trueObj;
}

class Counter : public ResourceObj {
 public:
   char* title;
   int total_size;
   int number;
   Counter(char* t) {
     title = t;
     total_size = 0;
     number = 0;
   }
   void update(memOop obj) { 
     total_size += obj->size();
     number++;
   }
   void print(char* prefix) {
     std->print("%s%s", prefix, title);
     std->fill_to(22);
     std->print_cr("%6d %8d", number, total_size * oopSize);
   }
   void add(Counter* i) {
     total_size += i->total_size;
     number     += i->number;
   }

   static int compare(Counter** a,  Counter** b) { 
     return (*b)->total_size - (*a)->total_size;
   }
};

class ObjectHistogram : public ObjectClosure {
 private:
  Counter* doubles;
  Counter* blocks;
  Counter* objArrays;
  Counter* symbols;
  Counter* byteArrays;
  Counter* doubleByteArrays;
  Counter* klasses;
  Counter* processes;
  Counter* vframes;
  Counter* methods;
  Counter* proxies;
  Counter* mixins;
  Counter* associations;
  Counter* contexts;
  Counter* memOops;
  GrowableArray<Counter*>* counters;
 public:
  ObjectHistogram();

  Counter* counter(memOop obj);
  void do_object(memOop obj) { counter(obj)->update(obj); }
  void print();
};

ObjectHistogram::ObjectHistogram() {
  counters = new GrowableArray<Counter*>(20);
  counters->push(doubles           = new Counter("doubles"));
  counters->push(blocks            = new Counter("blocks"));
  counters->push(objArrays         = new Counter("arrays"));
  counters->push(symbols           = new Counter("symbols"));
  counters->push(byteArrays        = new Counter("byte arrays"));
  counters->push(doubleByteArrays  = new Counter("double byte arrays"));
  counters->push(klasses           = new Counter("class"));
  counters->push(processes         = new Counter("processes"));
  counters->push(vframes           = new Counter("vframes"));
  counters->push(methods           = new Counter("methods"));
  counters->push(proxies           = new Counter("proxies"));
  counters->push(mixins            = new Counter("mixins"));
  counters->push(associations      = new Counter("associations"));
  counters->push(contexts          = new Counter("contexts"));
  counters->push(memOops           = new Counter("oops"));
}

Counter* ObjectHistogram::counter(memOop obj) {
  if (obj->is_double())           return doubles;
  if (obj->is_block())            return blocks;
  if (obj->is_objArray())         return objArrays;
  if (obj->is_symbol())           return symbols;		// Must be before byteArray
  if (obj->is_byteArray())        return byteArrays;
  if (obj->is_doubleByteArray())  return doubleByteArrays;
  if (obj->is_klass())            return klasses;
  if (obj->is_process())          return processes;
  if (obj->is_vframe())           return vframes;
  if (obj->is_method())           return methods;
  if (obj->is_proxy())            return proxies;
  if (obj->is_mixin())            return mixins;
  if (obj->is_association())      return associations;
  if (obj->is_context())          return contexts;
  return memOops;
}

void ObjectHistogram::print() {
  std->print("Object Histogram");
  std->fill_to(22);
  std->print_cr("number    bytes");
  Counter* total = new Counter("Total");
  counters->sort(&Counter::compare);
  for(int index = 0; index < counters->length(); index++) {
    Counter* c = counters->at(index);
    if (c->number > 0) {
      c->print(" - ");
      total->add(c);
    }
  }
  total->print(" ");
}

PRIM_DECL_0(debugPrimitives::printObjectHistogram) {
  PROLOGUE_0("printObjectHistogram");
  ResourceMark rm;
  ObjectHistogram blk;
  Universe::object_iterate(&blk);
  blk.print();
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::deoptimizeStacks) {
  PROLOGUE_0("deoptimizeStacks");
  VM_DeoptimizeStacks op;
  // The operation takes place in the vmProcess
  VMProcess::execute(&op);
  return trueObj;
}

PRIM_DECL_0(debugPrimitives::verify) {
  PROLOGUE_0("verify");
  Universe::verify();
  return trueObj;
}
