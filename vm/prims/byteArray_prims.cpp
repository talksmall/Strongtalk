/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.32 $ */
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
# include "incls/_byteArray_prims.cpp.incl"

TRACE_FUNC(TraceByteArrayPrims, "byteArray")

int byteArrayPrimitives::number_of_calls;

#define ASSERT_RECEIVER assert(receiver->is_byteArray(), "receiver must be byte array")

PRIM_DECL_2(byteArrayPrimitives::allocateSize, oop receiver, oop argument) {
  PROLOGUE_2("allocateSize", receiver, argument)
  assert(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_byteArray(),
        "receiver must byte array class");
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (smiOop(argument)->value() < 0)
    return markSymbol(vmSymbols::negative_size());

  klassOop k        = klassOop(receiver);
  int      ni_size  = k->klass_part()->non_indexable_size();
  int      obj_size = ni_size + 1 + roundTo(smiOop(argument)->value(), oopSize) / oopSize;
  // allocate
  byteArrayOop obj = as_byteArrayOop(Universe::allocate(obj_size, (memOop*)&k));
  // header
  memOop(obj)->initialize_header(true, k);
  // instance variables
  memOop(obj)->initialize_body(memOopDesc::header_size(), ni_size);
  // indexables
  oop* base = (oop*) obj->addr();
  oop* end  = base + obj_size;
  // %optimized 'obj->set_length(size)'
  base[ni_size] = argument;
  // %optimized 'for (int index = 1; index <= size; index++)
  //               obj->byte_at_put(index, '\000')'
  base = &base[ni_size+1];
  while (base < end) *base++ = (oop) 0;
  return obj;
}

PRIM_DECL_3(byteArrayPrimitives::allocateSize2, oop receiver, oop argument, oop tenured) {
  PROLOGUE_3("allocateSize2", receiver, argument, tenured)
  //These should be ordinary checks in case ST code erroneously passes an invalid value.
  //assert(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_byteArray(),
  //      "receiver must byte array class");
  if (!(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_byteArray()))
    return markSymbol(vmSymbols::invalid_klass());

  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (smiOop(argument)->value() < 0)
    return markSymbol(vmSymbols::negative_size());

  if (tenured != Universe::trueObj() && tenured != Universe::falseObj())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  memOopKlass* theKlass = (memOopKlass*)klassOop(receiver)->klass_part();
  oop result = theKlass->allocateObjectSize(smiOop(argument)->value(), false, tenured == trueObj);
  if (result == NULL) return markSymbol(vmSymbols::failed_allocation());
  return result;
}

PRIM_DECL_1(byteArrayPrimitives::size, oop receiver) {
  PROLOGUE_1("size", receiver);
  ASSERT_RECEIVER;
  // do the operation
  return as_smiOop(byteArrayOop(receiver)->length());
}

PRIM_DECL_2(byteArrayPrimitives::at, oop receiver, oop index) {
  PROLOGUE_2("at", receiver, index);
  ASSERT_RECEIVER;

  // check index type
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check index value
  if (!byteArrayOop(receiver)->is_within_bounds(smiOop(index)->value()))
     return markSymbol(vmSymbols::out_of_bounds());

  // do the operation
  return as_smiOop(byteArrayOop(receiver)->byte_at(smiOop(index)->value()));
}

PRIM_DECL_3(byteArrayPrimitives::atPut, oop receiver, oop index, oop value) {
  PROLOGUE_3("atPut", receiver, index, value);
  ASSERT_RECEIVER;

  // check index type
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check index type
  if (!value->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // check index value
  if (!byteArrayOop(receiver)->is_within_bounds(smiOop(index)->value()))
     return markSymbol(vmSymbols::out_of_bounds());

  // check value range (must be byte)
  unsigned int v = (unsigned int) smiOop(value)->value();
  if (v  >= (1<<8))
    return markSymbol(vmSymbols::value_out_of_range());

  // do the operation
  byteArrayOop(receiver)->byte_at_put(smiOop(index)->value(), v);
  return receiver;
}

PRIM_DECL_2(byteArrayPrimitives::compare, oop receiver, oop argument) {
  PROLOGUE_2("comare", receiver, argument);
  ASSERT_RECEIVER;

  if( receiver == argument)
    return as_smiOop(0);

  if (argument->is_byteArray())
    return as_smiOop(byteArrayOop(receiver)->compare(byteArrayOop(argument)));

  if (argument->is_doubleByteArray())
    return as_smiOop(byteArrayOop(receiver)->compare_doubleBytes(doubleByteArrayOop(argument)));

  return markSymbol(vmSymbols::first_argument_has_wrong_type());
}

PRIM_DECL_1(byteArrayPrimitives::intern, oop receiver) {
  PROLOGUE_1("intern", receiver);
  ASSERT_RECEIVER;

  return Universe::symbol_table->lookup(byteArrayOop(receiver)->chars(),
                                        byteArrayOop(receiver)->length());
}

PRIM_DECL_2(byteArrayPrimitives::characterAt, oop receiver, oop index) {
  PROLOGUE_2("characterAt", receiver, index);
  ASSERT_RECEIVER;

  // check index type
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // range check
  if (!byteArrayOop(receiver)->is_within_bounds(smiOop(index)->value()))
     return markSymbol(vmSymbols::out_of_bounds());

  // fetch byte
  int byte = byteArrayOop(receiver)->byte_at(smiOop(index)->value());
  
  // return the n+1'th element in asciiCharacter
  return Universe::asciiCharacters()->obj_at(byte+1);
}

PRIM_DECL_2(byteArrayPrimitives::at_all_put, oop receiver, oop value) {
  PROLOGUE_2("at_all_put", receiver, value);
  ASSERT_RECEIVER;

  // check index type
  if (!value->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check value range (must be byte)
  unsigned int v = (unsigned int) smiOop(value)->value();
  if (v  >= (1<<8))
    return markSymbol(vmSymbols::value_out_of_range());

  int length = byteArrayOop(receiver)->length();
  for (int index = 1; index <= length; index++) {
    byteArrayOop(receiver)->byte_at_put(index, v);
  }
  return receiver;
}

// LargeIntegers primitives

oop simplified(byteArrayOop result) {
  // Tries to simplify result, a large integer, into a small integer if possible.
  bool ok;
  oop smi_result = result->number().as_smi(ok);
  return ok ? smi_result : result;
}

PRIM_DECL_2(byteArrayPrimitives::largeIntegerFromSmallInteger, oop receiver, oop number) {
  PROLOGUE_2("largeIntegerFromSmallInteger", receiver, number);
  assert(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_byteArray(), "just checking");

  // Check arguments
  if (!number->is_smi()) return markSymbol(vmSymbols::second_argument_has_wrong_type());

  BlockScavenge bs;
  int           i = smiOop(number)->value();
  byteArrayOop  z;

  z = byteArrayOop(klassOop(receiver)->klass_part()->allocateObjectSize(IntegerOps::int_to_Integer_result_size_in_bytes(i)));
  IntegerOps::int_to_Integer(i, z->number());
  return z;
}

PRIM_DECL_2(byteArrayPrimitives::largeIntegerFromDouble, oop receiver, oop number) {
  PROLOGUE_2("largeIntegerFromDouble", receiver, number);
  assert(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_byteArray(), "just checking");

  if (!number->is_double()) return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;
  double x = doubleOop(number)->value();
  byteArrayOop z;

  z = byteArrayOop(klassOop(receiver)->klass_part()->allocateObjectSize(IntegerOps::double_to_Integer_result_size_in_bytes(x)));
  IntegerOps::double_to_Integer(x, z->number());
  return z;
}

PRIM_DECL_3(byteArrayPrimitives::largeIntegerFromString, oop receiver, oop argument, oop base) {
  PROLOGUE_3("largeIntegerFromString", receiver, argument, base);
  assert(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_byteArray(), "just checking");

  if (!argument->is_byteArray()) return markSymbol(vmSymbols::first_argument_has_wrong_type ());
  if (!base->is_smi()          ) return markSymbol(vmSymbols::second_argument_has_wrong_type());

  BlockScavenge bs;

  byteArrayOop x = byteArrayOop(argument);
  byteArrayOop z;

  z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::string_to_Integer_result_size_in_bytes(x->chars(), smiOop(base)->value())));
  IntegerOps::string_to_Integer(x->chars(), smiOop(base)->value(), z->number());
  return z;
}

PRIM_DECL_2(byteArrayPrimitives::largeIntegerAdd, oop receiver, oop argument) {
  PROLOGUE_2("largeIntegerAdd", receiver, argument);
  ASSERT_RECEIVER;

  if (!argument->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;

  byteArrayOop x = byteArrayOop(receiver);
  byteArrayOop y = byteArrayOop(argument);
  byteArrayOop z;

  if (!x->number().is_valid() || !y->number().is_valid()) return markSymbol(vmSymbols::argument_is_invalid());

  z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::add_result_size_in_bytes(x->number(), y->number())));
  x = byteArrayOop(receiver);
  y = byteArrayOop(argument);
  IntegerOps::add(x->number(), y->number(), z->number());
  return simplified(z);
}

PRIM_DECL_2(byteArrayPrimitives::largeIntegerSubtract, oop receiver, oop argument) {
  PROLOGUE_2("largeIntegerSubtract", receiver, argument);
  ASSERT_RECEIVER;

  if (!argument->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;

  byteArrayOop x = byteArrayOop(receiver);
  byteArrayOop y = byteArrayOop(argument);
  byteArrayOop z;

  if (!x->number().is_valid() || !y->number().is_valid()) return markSymbol(vmSymbols::argument_is_invalid());

  z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::sub_result_size_in_bytes(x->number(), y->number())));
  x = byteArrayOop(receiver);
  y = byteArrayOop(argument);
  IntegerOps::sub(x->number(), y->number(), z->number());
  return simplified(z);
}

PRIM_DECL_2(byteArrayPrimitives::largeIntegerMultiply, oop receiver, oop argument) {
  PROLOGUE_2("largeIntegerMultiply", receiver, argument);
  ASSERT_RECEIVER;

  if (!argument->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;

  byteArrayOop x = byteArrayOop(receiver);
  byteArrayOop y = byteArrayOop(argument);
  byteArrayOop z;

  if (!x->number().is_valid() || !y->number().is_valid()) return markSymbol(vmSymbols::argument_is_invalid());

  z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::mul_result_size_in_bytes(x->number(), y->number())));
  x = byteArrayOop(receiver);
  y = byteArrayOop(argument);
  IntegerOps::mul(x->number(), y->number(), z->number());
  return simplified(z);
}
#define BIT_OP(receiver, argument, sizeFn, opFn, label)\
  ARG_CHECK(receiver, argument, label);\
  byteArrayOop z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::sizeFn(x->number(), y->number())));\
\
  IntegerOps::opFn(x->number(), y->number(), z->number());\
  return simplified(z)
#define DIVISION(receiver, argument, sizeFn, divFn, label)\
  ARG_CHECK(receiver, argument, label);\
  if (y->number().is_zero()) return markSymbol(vmSymbols::division_by_zero   ());\
\
  byteArrayOop z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::sizeFn(x->number(), y->number())));\
\
  IntegerOps::divFn(x->number(), y->number(), z->number());\
  return simplified(z)
#define ARG_CHECK(receiver, argument, label)\
  PROLOGUE_2(label, receiver, argument);\
  ASSERT_RECEIVER;\
\
  if (!argument->is_byteArray())\
    return markSymbol(vmSymbols::first_argument_has_wrong_type());\
\
  byteArrayOop x = byteArrayOop(receiver);\
  byteArrayOop y = byteArrayOop(argument);\
\
  if (!x->number().is_valid() || !y->number().is_valid()) return markSymbol(vmSymbols::argument_is_invalid())

PRIM_DECL_2(byteArrayPrimitives::largeIntegerQuo, oop receiver, oop argument) {
  DIVISION(receiver, argument, quo_result_size_in_bytes, quo, "largeIntegerQuo");
}
PRIM_DECL_2(byteArrayPrimitives::largeIntegerDiv, oop receiver, oop argument) {
  DIVISION(receiver, argument, div_result_size_in_bytes, div, "largeIntegerDiv");
}
PRIM_DECL_2(byteArrayPrimitives::largeIntegerMod, oop receiver, oop argument) {
  DIVISION(receiver, argument, mod_result_size_in_bytes, mod, "largeIntegerMod");
}
PRIM_DECL_2(byteArrayPrimitives::largeIntegerRem, oop receiver, oop argument) {
  DIVISION(receiver, argument, rem_result_size_in_bytes, rem, "largeIntegerRem");
}
PRIM_DECL_2(byteArrayPrimitives::largeIntegerAnd, oop receiver, oop argument) {
  BIT_OP(receiver, argument, and_result_size_in_bytes, and, "largeIntegerAnd");
}
PRIM_DECL_2(byteArrayPrimitives::largeIntegerOr, oop receiver, oop argument) {
  BIT_OP(receiver, argument, or_result_size_in_bytes, or, "largeIntegerOr");
}
PRIM_DECL_2(byteArrayPrimitives::largeIntegerXor, oop receiver, oop argument) {
  BIT_OP(receiver, argument, xor_result_size_in_bytes, xor, "largeIntegerXor");
}
PRIM_DECL_2(byteArrayPrimitives::largeIntegerShift, oop receiver, oop argument) {
  PROLOGUE_2("largeIntegerShift", receiver, argument);
  ASSERT_RECEIVER;
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  byteArrayOop x = byteArrayOop(receiver);
  int shift = smiOop(argument)->value();

  if (!byteArrayOop(receiver)->number().is_valid())
    return markSymbol(vmSymbols::argument_is_invalid());

  byteArrayOop z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::ash_result_size_in_bytes(x->number(), shift)));

  IntegerOps::ash(x->number(), shift, z->number());

  return simplified(z);
}
PRIM_DECL_2(byteArrayPrimitives::largeIntegerCompare, oop receiver, oop argument) {
  PROLOGUE_2("largeIntegerCompare", receiver, argument);
  ASSERT_RECEIVER;

  // Check argument
  if (!argument->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;

  byteArrayOop x = byteArrayOop(receiver);
  byteArrayOop y = byteArrayOop(argument);

  if (!x->number().is_valid() || !y->number().is_valid()) return markSymbol(vmSymbols::argument_is_invalid());

  int res = IntegerOps::cmp(x->number(), y->number());
  if (res < 0) return as_smiOop(-1);
  if (res > 0) return as_smiOop(1);
  return as_smiOop(0);
}

PRIM_DECL_1(byteArrayPrimitives::largeIntegerToFloat, oop receiver) {
  PROLOGUE_1("largeIntegerToFloat", receiver);
  ASSERT_RECEIVER;

  bool ok;
  double result = byteArrayOop(receiver)->number().as_double(ok);

  if (!ok) return markSymbol(vmSymbols::conversion_failed());

  BlockScavenge bs;
  return oopFactory::new_double(result);
}

PRIM_DECL_2(byteArrayPrimitives::largeIntegerToString, oop receiver, oop base) {
  PROLOGUE_1("largeIntegerToString", receiver);
  ASSERT_RECEIVER;

  // Check argument
  if (!base->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;

  byteArrayOop x = byteArrayOop(receiver);
  int length = IntegerOps::Integer_to_string_result_size_in_bytes(x->number(), smiOop(base)->value());

  byteArrayOop result = oopFactory::new_byteArray(length);

  IntegerOps::Integer_to_string(x->number(), smiOop(base)->value(), result->chars());
  return result;
}

PRIM_DECL_1(byteArrayPrimitives::largeIntegerHash, oop receiver) {
  PROLOGUE_1("largeIntegerHash", receiver);
  ASSERT_RECEIVER;
  return as_smiOop(IntegerOps::hash(byteArrayOop(receiver)->number()));
}

PRIM_DECL_1(byteArrayPrimitives::hash, oop receiver) {
  PROLOGUE_1("hash", receiver);
  ASSERT_RECEIVER;
  return as_smiOop(byteArrayOop(receiver)->hash_value());
}

PersistentHandle* _unsafeAlienClass = NULL;
klassOop unsafeAlienClass() {
  if (_unsafeAlienClass)
    return _unsafeAlienClass->as_klassOop();
  oop uaKlass = Universe::find_global("UnsafeAlien");
  assert(uaKlass && uaKlass->is_klass(), "UnsafeAlien not found");
  if (!uaKlass) return NULL;
  _unsafeAlienClass = new PersistentHandle(uaKlass);
  return klassOop(uaKlass);
}
oop unsafeContents(oop unsafeAlien) {
  symbolOop ivarName = oopFactory::new_symbol("nonPointerObject");
  int offset = unsafeAlienClass()->klass_part()->lookup_inst_var(ivarName);
  return memOop(unsafeAlien)->instVarAt(offset);
}
#define checkAlienReceiver(receiver)\
  if (!receiver->is_byteArray())\
    return markSymbol(vmSymbols::receiver_has_wrong_type())

#define isUnsafe(argument)\
  (!oop(argument)->is_smi()\
  && memOop(argument)->klass_field() == unsafeAlienClass()\
  && unsafeContents(argument)->is_byteArray())

#define alienArg(argument)      (void*)argument

#define alienArray(receiver)    ((int*)byteArrayOop(receiver)->bytes())

#define alienSize(receiver)     (alienArray(receiver)[0])

#define alienAddress(receiver)  ((void**)alienArray(receiver))[1]

#define alienResult(handle)     (handle.as_oop() == nilObj ? NULL : (void*)&handle)

#define checkAlienCalloutReceiver(receiver) \
  checkAlienReceiver(receiver);\
  if (alienSize(receiver) > 0 || alienAddress(receiver) == NULL)\
    return markSymbol(vmSymbols::illegal_state())

#define checkAlienCalloutResult(argument) \
  if (!(argument->is_byteArray() || argument == nilObj))\
    return markSymbol(vmSymbols::argument_has_wrong_type())

#define checkAlienCalloutResultArgs(argument) \
  if (!(argument->is_byteArray() || argument == nilObj))\
    return markSymbol(vmSymbols::first_argument_has_wrong_type())

#define checkAlienCalloutArg(argument, symbol)\
  if (!(argument->is_byteArray() || argument->is_smi() || isUnsafe(argument)))\
    return markSymbol(symbol)

#define checkAlienCalloutArg1(argument)\
  checkAlienCalloutArg(argument, vmSymbols::second_argument_has_wrong_type())

#define checkAlienCalloutArg2(argument)\
  checkAlienCalloutArg(argument, vmSymbols::third_argument_has_wrong_type())

#define checkAlienCalloutArg3(argument)\
  checkAlienCalloutArg(argument, vmSymbols::fourth_argument_has_wrong_type())

#define checkAlienCalloutArg4(argument)\
  checkAlienCalloutArg(argument, vmSymbols::fifth_argument_has_wrong_type())

#define checkAlienCalloutArg5(argument)\
  checkAlienCalloutArg(argument, vmSymbols::sixth_argument_has_wrong_type())

#define checkAlienCalloutArg6(argument)\
  checkAlienCalloutArg(argument, vmSymbols::seventh_argument_has_wrong_type())

#define checkAlienCalloutArg7(argument)\
  checkAlienCalloutArg(argument, vmSymbols::eighth_argument_has_wrong_type())

#define checkAlienCalloutArg8(argument)\
  checkAlienCalloutArg(argument, vmSymbols::ninth_argument_has_wrong_type())

#define alienIndex(argument) (smiOop(argument)->value())

#define checkAlienAtIndex(receiver, argument, type)\
  if (!argument->is_smi())\
    return markSymbol(vmSymbols::argument_has_wrong_type());\
  if (alienIndex(argument) < 1 ||\
      (alienSize(receiver) != 0 && ((unsigned int)alienIndex(argument)) > abs(alienSize(receiver)) - sizeof(type) + 1))\
    return markSymbol(vmSymbols::index_not_valid())

#define checkAlienAtPutIndex(receiver, argument, type)\
  if (!argument->is_smi())\
    return markSymbol(vmSymbols::first_argument_has_wrong_type());\
  if (alienIndex(argument) < 1 ||\
      (alienSize(receiver) != 0 && ((unsigned int)alienIndex(argument)) > abs(alienSize(receiver)) - sizeof(type) + 1))\
    return markSymbol(vmSymbols::index_not_valid())

#define checkAlienAtPutValue(receiver, argument, type, min, max)\
  if (!argument->is_smi())\
    return markSymbol(vmSymbols::second_argument_has_wrong_type());\
  {\
    int value = smiOop(argument)->value();\
    if (value < min || value > max)\
      return markSymbol(vmSymbols::argument_is_invalid());\
  }

#define checkAlienAtReceiver(receiver)\
  checkAlienReceiver(receiver);\
  if (alienSize(receiver) <= 0 && alienAddress(receiver) == NULL)\
    return markSymbol(vmSymbols::illegal_state())

#define alienContents(receiver)\
  (alienSize(receiver) > 0\
    ? ((void*)(alienArray(receiver) + 1))\
    : (alienAddress(receiver)))

#define alienAt(receiver, argument, type)\
  *((type*)(((char*)alienContents(receiver)) + alienIndex(argument) - 1))

PRIM_DECL_2(byteArrayPrimitives::alienUnsignedByteAt, oop receiver, oop argument) {
  PROLOGUE_2("alienUnsignedByteAt", receiver, argument);
  checkAlienAtReceiver(receiver);
  checkAlienAtIndex(receiver, argument, u_char);
  
  return as_smiOop(alienAt(receiver, argument, u_char));
}

PRIM_DECL_3(byteArrayPrimitives::alienUnsignedByteAtPut, oop receiver, oop argument1, oop argument2){
  PROLOGUE_3("alienUnsignedByteAtPut", receiver, argument1, argument2);
  checkAlienAtReceiver(receiver);
  checkAlienAtPutIndex(receiver, argument1, u_char);
  checkAlienAtPutValue(receiver, argument2, u_char, 0, 255);
  
  alienAt(receiver, argument1, u_char) = smiOop(argument2)->value();

  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienSignedByteAt, oop receiver, oop argument) {
  PROLOGUE_2("alienSignedByteAt", receiver, argument);
  checkAlienAtReceiver(receiver);
  checkAlienAtIndex(receiver, argument, char);

  return as_smiOop(alienAt(receiver, argument, char));
}

PRIM_DECL_3(byteArrayPrimitives::alienSignedByteAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienSignedByteAtPut", receiver, argument1, argument2);
  checkAlienAtReceiver(receiver);
  checkAlienAtPutIndex(receiver, argument1, char);
  checkAlienAtPutValue(receiver, argument2, char, -128, 127);

  alienAt(receiver, argument1, char) = smiOop(argument2)->value();

  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienUnsignedShortAt, oop receiver, oop argument) {
  PROLOGUE_2("alienUnsignedShortAt", receiver, argument);
  checkAlienAtReceiver(receiver);
  checkAlienAtIndex(receiver, argument, unsigned short);

  return as_smiOop(alienAt(receiver, argument, unsigned short));
}

PRIM_DECL_3(byteArrayPrimitives::alienUnsignedShortAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienUnsignedShortAtPut", receiver, argument1, argument2);
  checkAlienAtReceiver(receiver);
  checkAlienAtPutIndex(receiver, argument1, unsigned short);
  checkAlienAtPutValue(receiver, argument2, unsigned short, 0, 65535);
  
  alienAt(receiver, argument1, unsigned short) = smiOop(argument2)->value();
  
  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienSignedShortAt, oop receiver, oop argument) {
  PROLOGUE_2("alienSignedShortAt", receiver, argument);
  checkAlienAtReceiver(receiver);
  checkAlienAtIndex(receiver, argument, short);

  return as_smiOop(alienAt(receiver, argument, short));
}

PRIM_DECL_3(byteArrayPrimitives::alienSignedShortAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienSignedShortAtPut", receiver, argument1, argument2);
  checkAlienAtReceiver(receiver);
  checkAlienAtPutIndex(receiver, argument1, short);
  checkAlienAtPutValue(receiver, argument2, short, -32768, 32767);

  alienAt(receiver, argument1, short) = smiOop(argument2)->value();

  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienUnsignedLongAt, oop receiver, oop argument) {
  PROLOGUE_2("alienUnsignedLongAt", receiver, argument);
  checkAlienAtReceiver(receiver);
  checkAlienAtIndex(receiver, argument, unsigned long);
  
  unsigned long value = alienAt(receiver, argument, unsigned long);

  int resultSize = IntegerOps::int_to_Integer_result_size_in_bytes(value);
  klassOop largeInteger = klassOop(Universe::find_global("LargeInteger"));
  byteArrayOop result = byteArrayOop(largeInteger->klass_part()->allocateObjectSize(resultSize));
  IntegerOps::unsigned_int_to_Integer(value, result->number());

  return simplified(result);
}

PRIM_DECL_3(byteArrayPrimitives::alienUnsignedLongAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienUnsignedLongAtPut", receiver, argument1, argument2);
  checkAlienAtReceiver(receiver);
  checkAlienAtPutIndex(receiver, argument1, unsigned long);
  if (!argument2->is_smi() && !argument2->is_byteArray())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  unsigned long value;
  if (argument2->is_smi())
    value = smiOop(argument2)->value();
  else {
    bool ok;
    value = byteArrayOop(argument2)->number().as_unsigned_int(ok);
    if (!ok)
      return markSymbol(vmSymbols::argument_is_invalid());
  }

  alienAt(receiver, argument1, unsigned long) = value;

  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienSignedLongAt, oop receiver, oop argument) {
  PROLOGUE_2("alienSignedLongAt", receiver, argument);
  checkAlienAtReceiver(receiver);
  checkAlienAtIndex(receiver, argument, long);

  int value = alienAt(receiver, argument, long);

  int resultSize = IntegerOps::int_to_Integer_result_size_in_bytes(value);
  klassOop largeInteger = klassOop(Universe::find_global("LargeInteger"));
  byteArrayOop result = byteArrayOop(largeInteger->klass_part()->allocateObjectSize(resultSize));
  IntegerOps::int_to_Integer(value, result->number());

  return simplified(result);
}

PRIM_DECL_3(byteArrayPrimitives::alienSignedLongAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienSignedLongAtPut", receiver, argument1, argument2);
  checkAlienAtReceiver(receiver);
  checkAlienAtPutIndex(receiver, argument1, long);
  if (!argument2->is_smi() && !argument2->is_byteArray())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  int value;

  if (argument2->is_smi())
    value = smiOop(argument2)->value();
  else {
    bool ok;
    value = byteArrayOop(argument2)->number().as_int(ok);
    if (!ok)
      return markSymbol(vmSymbols::argument_is_invalid());
  }

  alienAt(receiver, argument1, long) = value;

  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienDoubleAt, oop receiver, oop argument) {
  PROLOGUE_2("alienDoubleAt", receiver, argument);
  checkAlienAtReceiver(receiver);
  checkAlienAtIndex(receiver, argument, double);

  doubleOop result = doubleOop(Universe::doubleKlassObj()->klass_part()->allocateObject());
  result->set_value(alienAt(receiver, argument, double));
  
  return result;
}

PRIM_DECL_3(byteArrayPrimitives::alienDoubleAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienDoubleAtPut", receiver, argument1, argument2);
  checkAlienAtReceiver(receiver);
  checkAlienAtPutIndex(receiver, argument1, double);
  if (!argument2->is_double())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  alienAt(receiver, argument1, double) = doubleOop(argument2)->value();

  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienFloatAt, oop receiver, oop argument) {
  PROLOGUE_2("alienFloatAt", receiver, argument);
  checkAlienAtReceiver(receiver);
  checkAlienAtIndex(receiver, argument, float);

  doubleOop result = doubleOop(Universe::doubleKlassObj()->klass_part()->allocateObject());
  result->set_value(alienAt(receiver, argument, float));
  
  return result;
}

PRIM_DECL_3(byteArrayPrimitives::alienFloatAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienFloatAtPut", receiver, argument1, argument2);
  checkAlienAtReceiver(receiver);
  checkAlienAtPutIndex(receiver, argument1, float);
  if (!argument2->is_double())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  alienAt(receiver, argument1, float) = (float) doubleOop(argument2)->value();

  return argument2;
}

PRIM_DECL_1(byteArrayPrimitives::alienGetSize, oop receiver) {
  PROLOGUE_1("alienGetSize", receiver);
  checkAlienReceiver(receiver);

  return as_smiOop(alienSize(receiver));
}

PRIM_DECL_2(byteArrayPrimitives::alienSetSize, oop receiver, oop argument) {
  PROLOGUE_2("alienSetSize", receiver, argument);
  checkAlienReceiver(receiver);
  if (!argument->is_smi())
    return markSymbol(vmSymbols::argument_has_wrong_type());

  alienSize(receiver) = smiOop(argument)->value();
  return receiver;
}

PRIM_DECL_1(byteArrayPrimitives::alienGetAddress, oop receiver) {
  PROLOGUE_1("alienGetAddress", receiver);
  checkAlienReceiver(receiver);
  if (alienSize(receiver) > 0)
    return markSymbol(vmSymbols::illegal_state());

  int address = (int)alienAddress(receiver);
  int size = IntegerOps::int_to_Integer_result_size_in_bytes(address);

  oop largeInteger = Universe::find_global("LargeInteger");
  PersistentHandle z(klassOop(largeInteger)->klass_part()->allocateObjectSize(size));
  IntegerOps::int_to_Integer(address, byteArrayOop(z.as_oop())->number());
  return simplified(byteArrayOop(z.as_oop()));
}

PRIM_DECL_2(byteArrayPrimitives::alienSetAddress, oop receiver, oop argument) {
  PROLOGUE_2("alienSetAddress", receiver, argument);
  checkAlienReceiver(receiver);
  if (alienSize(receiver) > 0)
    return markSymbol(vmSymbols::illegal_state());
  if (!argument->is_smi() && 
     !(argument->is_byteArray() && byteArrayOop(argument)->number().signum() > 0))
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  int value;
  if (argument->is_smi())
    value = smiOop(argument)->value();
  else {
    bool ok;
    value = byteArrayOop(argument)->number().as_unsigned_int(ok);
    if (!ok)
      return markSymbol(vmSymbols::argument_is_invalid());
  }
  alienAddress(receiver) = (void*)value;

  return receiver;
}

typedef void (__stdcall *call_out_func_0)(void*, void*);
typedef void (__stdcall *call_out_func_1)(void*, void*, void*);
typedef void (__stdcall *call_out_func_2)(void*, void*, void*, void*);
typedef void (__stdcall *call_out_func_3)(void*, void*, void*, void*, void*);
typedef void (__stdcall *call_out_func_4)(void*, void*, void*, void*, void*, void*);
typedef void (__stdcall *call_out_func_5)(void*, void*, void*, void*, void*, void*, void*);
typedef void (__stdcall *call_out_func_6)(void*, void*, void*, void*, void*, void*, void*, void*);
typedef void (__stdcall *call_out_func_7)(void*, void*, void*, void*, void*, void*, void*, void*, void*);
typedef void (__stdcall *call_out_func_args)(void*, void*, oop, oop*);

PRIM_DECL_2(byteArrayPrimitives::alienCallResult0, oop receiver, oop argument) {
  PROLOGUE_2("alienCallResult0", receiver, argument);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResult(argument);

  PersistentHandle resultHandle(argument);
  call_out_func_0 entry = call_out_func_0(StubRoutines::alien_call_entry(0));

  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle));
  DLLs::exit_async_call(&process);

  return receiver;
}

PRIM_DECL_3(byteArrayPrimitives::alienCallResult1, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienCallResult1", receiver, argument1, argument2);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResultArgs(argument1);
  checkAlienCalloutArg1(argument2);

  PersistentHandle resultHandle(argument1);
  call_out_func_1 entry = call_out_func_1(StubRoutines::alien_call_entry(1));
  
  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle),
        alienArg(argument2));
  DLLs::exit_async_call(&process);

  return receiver;
}

PRIM_DECL_4(byteArrayPrimitives::alienCallResult2, oop receiver, oop argument1, oop argument2, oop argument3) {
  PROLOGUE_4("alienCallResult2", receiver, argument1, argument2, argument3);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResultArgs(argument1);
  checkAlienCalloutArg1(argument2);
  checkAlienCalloutArg2(argument3);

  PersistentHandle resultHandle(argument1);
  call_out_func_2 entry = call_out_func_2(StubRoutines::alien_call_entry(2));
  
  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle),
        alienArg(argument2),
        alienArg(argument3));
  DLLs::exit_async_call(&process);
  
  return receiver;
}

PRIM_DECL_5(byteArrayPrimitives::alienCallResult3, oop receiver, oop argument1, oop argument2, oop argument3, oop argument4) {
  PROLOGUE_5("alienCallResult3", receiver, argument1, argument2, argument3, argument4);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResultArgs(argument1);
  checkAlienCalloutArg1(argument2);
  checkAlienCalloutArg2(argument3);
  checkAlienCalloutArg3(argument4);

  PersistentHandle resultHandle(argument1);
  call_out_func_3 entry = call_out_func_3(StubRoutines::alien_call_entry(3));
  
  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle),
        alienArg(argument2),
        alienArg(argument3),
        alienArg(argument4));
  DLLs::exit_async_call(&process);
  
  return receiver;
}

PRIM_DECL_6(byteArrayPrimitives::alienCallResult4, oop receiver, oop argument1, oop argument2,
            oop argument3, oop argument4, oop argument5) {
  PROLOGUE_6("alienCallResult4", receiver, argument1, argument2, argument3, argument4, 
    argument5);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResultArgs(argument1);
  checkAlienCalloutArg1(argument2);
  checkAlienCalloutArg2(argument3);
  checkAlienCalloutArg3(argument4);
  checkAlienCalloutArg4(argument5);

  PersistentHandle resultHandle(argument1);
  call_out_func_4 entry = call_out_func_4(StubRoutines::alien_call_entry(4));
  
  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle),
        alienArg(argument2),
        alienArg(argument3),
        alienArg(argument4),
        alienArg(argument5));
  DLLs::exit_async_call(&process);
  
  return receiver;
}

PRIM_DECL_7(byteArrayPrimitives::alienCallResult5, oop receiver, oop argument1, oop argument2,
            oop argument3, oop argument4, oop argument5, oop argument6) {
  PROLOGUE_7("alienCallResult5", receiver, argument1, argument2, argument3, argument4, 
    argument5, argument6);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResultArgs(argument1);
  checkAlienCalloutArg1(argument2);
  checkAlienCalloutArg2(argument3);
  checkAlienCalloutArg3(argument4);
  checkAlienCalloutArg4(argument5);
  checkAlienCalloutArg5(argument6);

  PersistentHandle resultHandle(argument1);
  call_out_func_5 entry = call_out_func_5(StubRoutines::alien_call_entry(5));
  
  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle),
        alienArg(argument2),
        alienArg(argument3),
        alienArg(argument4),
        alienArg(argument5),
        alienArg(argument6));
  DLLs::exit_async_call(&process);
  
  return receiver;
}

PRIM_DECL_8(byteArrayPrimitives::alienCallResult6, oop receiver, oop argument1, oop argument2,
            oop argument3, oop argument4, oop argument5, oop argument6, oop argument7) {
  PROLOGUE_8("alienCallResult6", receiver, argument1, argument2, argument3, argument4, 
    argument5, argument6, argument7);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResultArgs(argument1);
  checkAlienCalloutArg1(argument2);
  checkAlienCalloutArg2(argument3);
  checkAlienCalloutArg3(argument4);
  checkAlienCalloutArg4(argument5);
  checkAlienCalloutArg5(argument6);
  checkAlienCalloutArg6(argument7);

  PersistentHandle resultHandle(argument1);
  call_out_func_6 entry = call_out_func_6(StubRoutines::alien_call_entry(6));
  
  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle),
        alienArg(argument2),
        alienArg(argument3),
        alienArg(argument4),
        alienArg(argument5),
        alienArg(argument6),
        alienArg(argument7));
  DLLs::exit_async_call(&process);
  
  return receiver;
}
PRIM_DECL_9(byteArrayPrimitives::alienCallResult7, oop receiver, oop argument1, oop argument2,
    oop argument3, oop argument4, oop argument5, oop argument6, oop argument7, oop argument8) {
  PROLOGUE_9("alienCallResult7", receiver, argument1, argument2, argument3, argument4, 
    argument5, argument6, argument7, argument8);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResultArgs(argument1);
  checkAlienCalloutArg1(argument2);
  checkAlienCalloutArg2(argument3);
  checkAlienCalloutArg3(argument4);
  checkAlienCalloutArg4(argument5);
  checkAlienCalloutArg5(argument6);
  checkAlienCalloutArg6(argument7);
  checkAlienCalloutArg7(argument8);

  PersistentHandle resultHandle(argument1);
  call_out_func_7 entry = call_out_func_7(StubRoutines::alien_call_entry(7));
  
  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle),
        alienArg(argument2),
        alienArg(argument3),
        alienArg(argument4),
        alienArg(argument5),
        alienArg(argument6),
        alienArg(argument7),
        alienArg(argument8));
  DLLs::exit_async_call(&process);
  
  return receiver;
}

PRIM_DECL_3(byteArrayPrimitives::alienCallResultWithArguments, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienCallResultWithArguments", receiver, argument1, argument2);
  DeltaProcess* process;
  checkAlienCalloutReceiver(receiver);
  checkAlienCalloutResultArgs(argument1);

  int length = objArrayOop(argument2)->length();
  for (int index = 1; index <= length; index++) {
    checkAlienCalloutArg(objArrayOop(argument2)->obj_at(index), vmSymbols::argument_has_wrong_type());
  }
  PersistentHandle resultHandle(argument1);
  call_out_func_args entry = call_out_func_args(StubRoutines::alien_call_with_args_entry());
  
  DLLs::enter_async_call(&process);
  entry(alienAddress(receiver),
        alienResult(resultHandle),
        as_smiOop(length),
        objArrayOop(argument2)->objs(1));
  DLLs::exit_async_call(&process);

  return receiver;
}
