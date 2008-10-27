/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.47 $ */
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
# include "incls/_prim.cpp.incl"

// The primitive_table is generated from prims.src.
// The output has the following format:
//   static int size_of_primitive_table
//   static primitive_desc* primitive_table;
#include "prims.inc"

// the typedefs below are necessary to ensure that args are passed correctly when calling a primitive
// through a function pointer
// NB: there's no general n-argument primitive because some calling conventions can't handle vararg functions
typedef  oop (PRIM_API *prim_fntype0)();
typedef  oop (PRIM_API *prim_fntype1)(oop);
typedef  oop (PRIM_API *prim_fntype2)(oop, oop);
typedef  oop (PRIM_API *prim_fntype3)(oop, oop, oop);
typedef  oop (PRIM_API *prim_fntype4)(oop, oop, oop, oop);
typedef  oop (PRIM_API *prim_fntype5)(oop, oop, oop, oop, oop);
typedef  oop (PRIM_API *prim_fntype6)(oop, oop, oop, oop, oop, oop);
typedef  oop (PRIM_API *prim_fntype7)(oop, oop, oop, oop, oop, oop, oop);
typedef  oop (PRIM_API *prim_fntype8)(oop, oop, oop, oop, oop, oop, oop, oop);
typedef  oop (PRIM_API *prim_fntype9)(oop, oop, oop, oop, oop, oop, oop, oop, oop);

oop primitive_desc::eval(oop* a) {
  const bool reverseArgs = true;	// change this when changing primitive calling convention
  oop res;
  int ebx_on_stack;

  // %hack: see below
#ifndef __GNUC__
  __asm mov ebx_on_stack, ebx
#else
  asm("movl %%ebx, %0" : "=b"(ebx_on_stack));
#endif
  if (reverseArgs) {
    switch (number_of_parameters()) {
    case  0: res = ((prim_fntype0)_fn)(); break;
    case  1: res = ((prim_fntype1)_fn)(a[0]); break;
    case  2: res = ((prim_fntype2)_fn)(a[1], a[0]); break;
    case  3: res = ((prim_fntype3)_fn)(a[2], a[1], a[0]); break;
    case  4: res = ((prim_fntype4)_fn)(a[3], a[2], a[1], a[0]); break;
    case  5: res = ((prim_fntype5)_fn)(a[4], a[3], a[2], a[1], a[0]); break;
    case  6: res = ((prim_fntype6)_fn)(a[5], a[4], a[3], a[2], a[1], a[0]); break;
    case  7: res = ((prim_fntype7)_fn)(a[6], a[5], a[4], a[3], a[2], a[1], a[0]); break;
    case  8: res = ((prim_fntype8)_fn)(a[7], a[6], a[5], a[4], a[3], a[2], a[1], a[0]); break;
    case  9: res = ((prim_fntype9)_fn)(a[8], a[7], a[6], a[5], a[4], a[3], a[2], a[1], a[0]); break;
    default: ShouldNotReachHere();
    }
  } else {
    switch (number_of_parameters()) {
    case  0: res = ((prim_fntype0)_fn)(); break;
    case  1: res = ((prim_fntype1)_fn)(a[0]); break;
    case  2: res = ((prim_fntype2)_fn)(a[0], a[1]); break;
    case  3: res = ((prim_fntype3)_fn)(a[0], a[1], a[2]); break;
    case  4: res = ((prim_fntype4)_fn)(a[0], a[1], a[2], a[3]); break;
    case  5: res = ((prim_fntype5)_fn)(a[0], a[1], a[2], a[3], a[4]); break;
    case  6: res = ((prim_fntype6)_fn)(a[0], a[1], a[2], a[3], a[4], a[5]); break;
    case  7: res = ((prim_fntype7)_fn)(a[0], a[1], a[2], a[3], a[4], a[5], a[6]); break;
    case  8: res = ((prim_fntype8)_fn)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]); break;
    case  9: res = ((prim_fntype9)_fn)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]); break;
    default: ShouldNotReachHere();
    }
  }

  // %hack: some primitives alter EBX and crash the compiler's constant propagation
  int ebx_now;
#ifndef __GNUC__
  __asm mov ebx_now, ebx
  __asm mov ebx, ebx_on_stack
#else
  asm("movl %%ebx, %0; movl %1, %%ebx;" : "=b"(ebx_now) : "b"(ebx_on_stack));
#endif

  if (ebx_now != ebx_on_stack)
  {
    std->print_cr("ebx changed (%X -> %X) in :", ebx_on_stack, ebx_now);
    print();
  }

  return res;
}

void primitives::print_table() {
  std->print_cr("Primitive table:");
  for (int index = 0; index < size_of_primitive_table; index++) {
    primitive_desc* e = primitive_table[index];
    std->print("%3d ",index);
    e->print();
  }
  std->print_cr(" - format: <index> <name> <number_of_parameters> <flags> [category]");
  std->print_cr("    flags:  R = has receiver,           F = has failure block,");
  std->print_cr("            S = can scavenge,           N = can perform NLR,");
  std->print_cr("            C = can be constant folded, D = can invoke Delta code");
  std->print_cr("            I = internal,               P = needs Delta fp code");
  std->print_cr("            W = can walk stack (computed)");
}


bool primitive_desc::can_walk_stack() const {
  return can_scavenge() || can_invoke_delta() || can_perform_NLR();
}


symbolOop primitive_desc::selector() const {
  return oopFactory::new_symbol(name());
}


void primitive_desc::print() {
  std->print("%48s %d %s%s%s%s%s%s%s%s%s",
             name(),
             number_of_parameters(),
	     has_receiver()           ? "R" : "_",
             can_fail()               ? "F" : "_",
	     can_scavenge()           ? "S" : "_",
	     can_walk_stack()         ? "W" : "_",
	     can_perform_NLR()        ? "N" : "_",
	     can_be_constant_folded() ? "C" : "_",
	     can_invoke_delta()       ? "D" : "_",
	     is_internal()            ? "I" : "_",
	     needs_delta_fp_code()    ? "P" : "_");
  switch(group()) {
    case IntComparisonPrimitive:   std->print(", smi compare");           break;
    case IntArithmeticPrimitive:   std->print(", smi arith");             break;
    case FloatComparisonPrimitive: std->print(", double compare");        break;
    case FloatArithmeticPrimitive: std->print(", double arith");          break;
    case ByteArrayPrimitive:       std->print(", byte array op.");        break;
    case DoubleByteArrayPrimitive: std->print(", double-byte array op."); break;
    case ObjArrayPrimitive:        std->print(", array op.");             break;
    case BlockPrimitive: 	   std->print(", block/context");         break;
    case NormalPrimitive:                                                 break;
    default: fatal("Unknown primitive group");
  }
  std->cr();
}


char* primitive_desc::parameter_type(int index) const {
  assert((0 <= index) && (index < number_of_parameters()), "illegal parameter index");
  return _types[1 + index];
}


char* primitive_desc::return_type() const {
  return _types[0];
}


Expr* primitive_desc::convertToKlass(char* type, PReg* p, Node* n) const {
  if (0 == strcmp(type, "SmallInteger")) return new KlassExpr(Universe::smiKlassObj(), p, n);
  if (0 == strcmp(type, "Double"))       return new KlassExpr(Universe::doubleKlassObj(), p, n);
  if (0 == strcmp(type, "Float"))        return new KlassExpr(Universe::doubleKlassObj(), p, n);
  if (0 == strcmp(type, "Symbol"))       return new KlassExpr(Universe::symbolKlassObj(), p, n);
  if (0 == strcmp(type, "Boolean")) {
    // NB: set expression node to NULL, not n -- MergeExpr cannot be split
    Expr* t = new ConstantExpr(Universe::trueObj(), p, NULL);
    Expr* f = new ConstantExpr(Universe::falseObj(), p, NULL);
    return new MergeExpr(t, f, p, NULL);
  }

  // should extend:
  // - looking up klassName in global class dictionary would cover many other cases
  // - for these, need to agree what prim info means: "exact match" or "any subclass of"
  // fix this later  -Urs 11/95
  return NULL;
}


Expr* primitive_desc::parameter_klass(int index, PReg* p, Node* n) const {
  return convertToKlass(parameter_type(index), p, n);
}


Expr* primitive_desc::return_klass(PReg* p, Node* n) const {
  return convertToKlass(return_type(), p, n);
}


void primitive_desc::error(char* msg) {
  print();
  ::error(msg);
}

void primitive_desc::verify() {
  bool ok = true;
  if (can_invoke_delta()) {
    if (!can_scavenge()) error("canInvokeDelta implies canScavenge");
    if (!can_walk_stack()) error("canInvokeDelta implies can_walk_stack");
    if (can_be_constant_folded()) error("canInvokeDelta implies !canbeConstantFolded");
    if (!can_perform_NLR()) error("canInvokeDelta implies canPerformNLR");
  }
  if (can_be_constant_folded()) {
    if (can_perform_NLR()) error("canbeConstantFolded implies !canPerformNLR");
  }
  if (group() == BlockPrimitive) {
    if (!can_walk_stack()) error("blocks must have can_walk_stack");
  }
}

int primitive_desc::compare(char* str, int len) {
  int src_len = strlen(name());
  int sign = strncmp(name(), str, min(src_len, len));
  // if (sign != 0 || src_len == len) return sign;
  if (sign != 0) return sign < 0 ? -1 : 1;
  if (src_len == len) return 0;
  return src_len < len ? -1 : 1;
}

primitive_desc* primitives::lookup(char* s, int len) {
  int first = 0;
  int last  = size_of_primitive_table;

  primitive_desc* element;
  do {
    int middle = first + (last - first)/2;
    element    = primitive_table[middle];
    int sign   = element->compare(s, len);
         if (sign == -1) first = middle + 1;
    else if (sign ==  1) last  = middle - 1;
    else return element;
  } while (first < last);

  // This should not be an assertion as it is possible to compile a reference
  // to a non-existent primitive (for an example see 
  // ProcessPrimitiveLookupError>>provoke), in which case the lookup should
  // fail and signal a PrimitiveLookupError - slr 24/09/2008

  // assert(first == last, "check for one element");
  element = primitive_table[first];

  return element->compare(s, len) == 0 ? element : NULL;
}


primitive_desc* primitives::lookup(fntype fn) {
  for (int index = 0; index < size_of_primitive_table; index++) {
    primitive_desc* e = primitive_table[index];
    if (e->fn() == fn) return e;
  }
  return NULL;
}


void primitives::lookup_and_patch() {
  // get primitive call info
  frame f = DeltaProcess::active()->last_frame();
  CodeIterator it(f.hp());
  oop* selector_addr = it.aligned_oop(1);

  symbolOop sel = symbolOop(*selector_addr);
  assert(sel->is_symbol(), "symbol expected");
  // do lookup
  primitive_desc* pdesc = primitives::lookup(sel);
  if (pdesc != NULL && !pdesc->is_internal()) {
    // primitive found => patch bytecode & cache
    *f.hp() = u_char(Bytecodes::primitive_call_code_for(Bytecodes::Code(*f.hp())));
    *selector_addr = oop(pdesc->fn());
  } else {
    // advance hp so that it points to the next instruction
    it.advance();
    f.set_hp(it.hp());

    { ResourceMark rm;
      // primitive not found => process error
      std->print("primitive lookup error\n");
      sel->print_value(); std->print(" not found\n");
    }
    if (DeltaProcess::active()->is_scheduler()) {
      ResourceMark rm;
      DeltaProcess::active()->trace_stack();
      fatal("primitive lookup error in scheduler");
    } else {
      DeltaProcess::active()->suspend(primitive_lookup_error);
    }
    ShouldNotReachHere();
  }
}


void prim_init() {
  primitives::initialize();
  primitive_desc* prev = NULL;
  for (int index = 0; index < size_of_primitive_table; index++) {
    primitive_desc* e = primitive_table[index];
    e->verify();
    if (prev) {
      guarantee(strcmp(prev->name(), e->name()) == -1, "primitive table not sorted");
    }
  }
  primitives::clear_counters();
}

// For debugging/profiling
void primitives::clear_counters() {
  behaviorPrimitives::number_of_calls        = 0;
  byteArrayPrimitives::number_of_calls       = 0;
  callBackPrimitives::number_of_calls        = 0;
  doubleByteArrayPrimitives::number_of_calls = 0;
  debugPrimitives::number_of_calls           = 0;
  doubleOopPrimitives::number_of_calls       = 0;
  methodOopPrimitives::number_of_calls       = 0;
  mixinOopPrimitives::number_of_calls        = 0;
  objArrayPrimitives::number_of_calls        = 0;
  oopPrimitives::number_of_calls             = 0;
  processOopPrimitives::number_of_calls      = 0;
  proxyOopPrimitives::number_of_calls        = 0;
  smiOopPrimitives::number_of_calls          = 0;
  systemPrimitives::number_of_calls          = 0;
}


static void print_calls(char* name, int inc, int* total) {
  if (inc > 0) {
    lprintf(" %s:\t%6d\n", name, inc);
    *total = *total + inc;
  }
}


void primitives::print_counters() {
  int total = 0;
  lprintf("Primitive call counters:\n");
  print_calls("Behavoir",        behaviorPrimitives::number_of_calls,        &total);
  print_calls("byteArray",       byteArrayPrimitives::number_of_calls,       &total);
  print_calls("callBack",        callBackPrimitives::number_of_calls,        &total);
  print_calls("doubelByteArray", doubleByteArrayPrimitives::number_of_calls, &total);
  print_calls("debug",           debugPrimitives::number_of_calls,           &total);
  print_calls("double",          doubleOopPrimitives::number_of_calls,       &total);
  print_calls("method",          methodOopPrimitives::number_of_calls,       &total);
  print_calls("mixin",           mixinOopPrimitives::number_of_calls,        &total);
  print_calls("objArray",        objArrayPrimitives::number_of_calls,        &total);
  print_calls("oop",             oopPrimitives::number_of_calls,             &total);
  print_calls("process",         processOopPrimitives::number_of_calls,      &total);
  print_calls("proxy",           proxyOopPrimitives::number_of_calls,        &total);
  print_calls("smi",             smiOopPrimitives::number_of_calls,          &total);
  print_calls("system",          systemPrimitives::number_of_calls,          &total);
  lprintf("Total:\t%6d\n", total);

}

primitive_desc* InterpretedPrim_Cache::pdesc() const {
  CodeIterator c(hp());
  switch (c.code()) {
    case Bytecodes::prim_call:
    case Bytecodes::prim_call_failure:
    case Bytecodes::prim_call_self:
    case Bytecodes::prim_call_self_failure:
      return primitives::lookup((fntype) c.word_at(1));

    case Bytecodes::prim_call_lookup:
    case Bytecodes::prim_call_failure_lookup:
    case Bytecodes::prim_call_self_lookup:
    case Bytecodes::prim_call_self_failure_lookup:
      return primitives::lookup(symbolOop(c.oop_at(1)));

    default: fatal("Wrong bytecode");
  }
  return NULL;
}

bool InterpretedPrim_Cache::has_receiver() const {
  CodeIterator c(hp());
  switch (c.code()) {
    case Bytecodes::prim_call_self:
    case Bytecodes::prim_call_self_failure:
    case Bytecodes::prim_call_self_lookup:
    case Bytecodes::prim_call_self_failure_lookup:
      return true;

    case Bytecodes::prim_call:
    case Bytecodes::prim_call_failure:
    case Bytecodes::prim_call_lookup:
    case Bytecodes::prim_call_failure_lookup:
       return false;

    default: fatal("Wrong bytecode");
  }
  return false;
}

symbolOop InterpretedPrim_Cache::name() const {
  CodeIterator c(hp());
  switch (c.code()) {
    case Bytecodes::prim_call:
    case Bytecodes::prim_call_failure:
    case Bytecodes::prim_call_self:
    case Bytecodes::prim_call_self_failure:
      return primitives::lookup((fntype) c.word_at(1))->selector();

    case Bytecodes::prim_call_lookup:
    case Bytecodes::prim_call_failure_lookup:
    case Bytecodes::prim_call_self_lookup:
    case Bytecodes::prim_call_self_failure_lookup:
      return symbolOop(c.oop_at(1));

    default: fatal("Wrong bytecode");
  }
  return NULL;
}

int InterpretedPrim_Cache::number_of_parameters() const {
  int result = name()->number_of_arguments()
             + (has_receiver()     ? 1 : 0)
	     - (has_failure_code() ? 1 : 0);
  assert(pdesc() == NULL || pdesc()->number_of_parameters() == result, "checking result");
  return result;
}

bool InterpretedPrim_Cache::has_failure_code() const {
  CodeIterator c(hp());
  switch (c.code()) {
    case Bytecodes::prim_call_failure:
    case Bytecodes::prim_call_failure_lookup:
    case Bytecodes::prim_call_self_failure:
    case Bytecodes::prim_call_self_failure_lookup:
      return true;

    case Bytecodes::prim_call:
    case Bytecodes::prim_call_lookup:
    case Bytecodes::prim_call_self_lookup:
    case Bytecodes::prim_call_self:
      return false;

    default: fatal("Wrong bytecode");
  }
  return false;
}

primitive_desc* primitives::_new0;
primitive_desc* primitives::_new1;
primitive_desc* primitives::_new2;
primitive_desc* primitives::_new3;
primitive_desc* primitives::_new4;
primitive_desc* primitives::_new5;
primitive_desc* primitives::_new6;
primitive_desc* primitives::_new7;
primitive_desc* primitives::_new8;
primitive_desc* primitives::_new9;
primitive_desc* primitives::_equal;
primitive_desc* primitives::_not_equal;
primitive_desc* primitives::_block_allocate;
primitive_desc* primitives::_block_allocate0;
primitive_desc* primitives::_block_allocate1;
primitive_desc* primitives::_block_allocate2;
primitive_desc* primitives::_context_allocate;
primitive_desc* primitives::_context_allocate0;
primitive_desc* primitives::_context_allocate1;
primitive_desc* primitives::_context_allocate2;

primitive_desc* primitives::verified_lookup(char* selector) {
  primitive_desc* result = lookup(selector);
  if (result == NULL) {
    getErr()->print_cr("Verified primitive lookup failed");
    getErr()->print_cr(" selector = %s", selector);
    fatal("aborted");
  }
  return result;
}

void primitives::initialize() {
  _new0              = verified_lookup("primitiveNew0ifFail:");
  _new1              = verified_lookup("primitiveNew1ifFail:");
  _new2              = verified_lookup("primitiveNew2ifFail:");
  _new3              = verified_lookup("primitiveNew3ifFail:");
  _new4              = verified_lookup("primitiveNew4ifFail:");
  _new5              = verified_lookup("primitiveNew5ifFail:");
  _new6              = verified_lookup("primitiveNew6ifFail:");
  _new7              = verified_lookup("primitiveNew7ifFail:");
  _new8              = verified_lookup("primitiveNew8ifFail:");
  _new9              = verified_lookup("primitiveNew9ifFail:");

  _equal             = verified_lookup("primitiveEqual:");
  _not_equal         = verified_lookup("primitiveNotEqual:");

  _block_allocate    = verified_lookup("primitiveCompiledBlockAllocate:");
  _block_allocate0   = verified_lookup("primitiveCompiledBlockAllocate0");
  _block_allocate1   = verified_lookup("primitiveCompiledBlockAllocate1");
  _block_allocate2   = verified_lookup("primitiveCompiledBlockAllocate2");
  _context_allocate  = verified_lookup("primitiveCompiledContextAllocate:");
  _context_allocate0 = verified_lookup("primitiveCompiledContextAllocate0");
  _context_allocate1 = verified_lookup("primitiveCompiledContextAllocate1");
  _context_allocate2 = verified_lookup("primitiveCompiledContextAllocate2");

}

void primitives::patch(char* name, char* entry_point) {
  assert(entry_point, "just checking");
  primitive_desc* pdesc = verified_lookup(name);
  pdesc->_fn = (fntype)entry_point;
}
