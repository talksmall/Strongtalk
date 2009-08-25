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

PRIM_DECL_2(byteArrayPrimitives::largeIntegerDiv, oop receiver, oop argument) {
  PROLOGUE_2("largeIntegerDiv", receiver, argument);
  ASSERT_RECEIVER;

  if (!argument->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;

  byteArrayOop x = byteArrayOop(receiver);
  byteArrayOop y = byteArrayOop(argument);
  byteArrayOop z;

  if (!x->number().is_valid() || !y->number().is_valid()) return markSymbol(vmSymbols::argument_is_invalid());
  if (y->number().is_zero()                             ) return markSymbol(vmSymbols::division_by_zero   ());

  z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::div_result_size_in_bytes(x->number(), y->number())));
  x = byteArrayOop(receiver);
  y = byteArrayOop(argument);
  IntegerOps::div(x->number(), y->number(), z->number());
  return simplified(z);
}

PRIM_DECL_2(byteArrayPrimitives::largeIntegerMod, oop receiver, oop argument) {
  PROLOGUE_2("largeIntegerMod", receiver, argument);
  ASSERT_RECEIVER;

  if (!argument->is_byteArray())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  BlockScavenge bs;

  byteArrayOop x = byteArrayOop(receiver);
  byteArrayOop y = byteArrayOop(argument);
  byteArrayOop z;

  if (!x->number().is_valid() || !y->number().is_valid()) return markSymbol(vmSymbols::argument_is_invalid());
  if (y->number().is_zero()                             ) return markSymbol(vmSymbols::division_by_zero   ());

  z = byteArrayOop(x->klass()->klass_part()->allocateObjectSize(IntegerOps::mod_result_size_in_bytes(x->number(), y->number())));
  x = byteArrayOop(receiver);
  y = byteArrayOop(argument);
  IntegerOps::mod(x->number(), y->number(), z->number());
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

PRIM_DECL_1(byteArrayPrimitives::hash, oop receiver) {
  PROLOGUE_1("hash", receiver);
  ASSERT_RECEIVER;
  return as_smiOop(byteArrayOop(receiver)->hash_value());
}

#define ALIEN_SIZE(alien) ((int*)byteArrayOop(alien)->bytes())[0]
#define ALIEN_ADDRESS(alien) ((int*)byteArrayOop(alien)->bytes())[1]
#define ALIEN_BYTE_AT(alien, index) byteArrayOop(alien)->byte_at(index + 4)
#define ALIEN_BYTE_AT_PUT(alien, index, byte) byteArrayOop(alien)->byte_at_put(smiOop(index)->value() + 4, byte)
#define ALIEN_PTR_AT(alien, index) ((u_char*)(ALIEN_ADDRESS(alien)))[index - 1]
#define ALIEN_SHORT_AT(alien, index) (short*)(((u_char*)byteArrayOop(alien)->bytes()) + index + 3)
#define ALIEN_SHORT_PTR_AT(alien, index) ((short*)&ALIEN_PTR_AT(receiver, index))

PRIM_DECL_2(byteArrayPrimitives::alienUnsignedByteAt, oop receiver, oop argument) {
  PROLOGUE_2("alienUnsignedByteAt", receiver, argument);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument->is_smi())
    return markSymbol(vmSymbols::argument_has_wrong_type());
  
  int size = ALIEN_SIZE(receiver);
  int absSize = abs(size);
  int index = smiOop(argument)->value();

  if (index < 1 || (size != 0 && index > absSize))
    return markSymbol(vmSymbols::argument_is_invalid());

  u_char byte;
  if (size > 0)
    byte = ALIEN_BYTE_AT(receiver, index);
  else
    byte = ALIEN_PTR_AT(receiver, index);
  
  return as_smiOop(byte);
}

PRIM_DECL_3(byteArrayPrimitives::alienUnsignedByteAtPut, oop receiver, oop argument1, oop argument2){
  PROLOGUE_3("alienUnsignedByteAtPut", receiver, argument1, argument2);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument1->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!argument2->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  
  int index = smiOop(argument1)->value();
  int size = ALIEN_SIZE(receiver);
  int absSize = abs(size);
  int byte = smiOop(argument2)->value();
  
  if (index < 1 || (size != 0 && index > absSize))
    return markSymbol(vmSymbols::index_not_valid());
  if (byte < 0 || byte > 255)
    return markSymbol(vmSymbols::argument_is_invalid());

  if (size > 0)
    byteArrayOop(receiver)->byte_at_put(index + 4, smiOop(argument2)->value());
  else {
    ALIEN_PTR_AT(receiver, index) = (u_char)byte;
  }
  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienSignedByteAt, oop receiver, oop argument) {
  PROLOGUE_2("alienSignedByteAt", receiver, argument);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument->is_smi())
    return markSymbol(vmSymbols::argument_has_wrong_type());

  int index = smiOop(argument)->value();
  int size = ALIEN_SIZE(receiver);

  if (index < 1 || (size != 0 && index > abs(size)))
    return markSymbol(vmSymbols::index_not_valid());

  int byte;
  if (size > 0)
    byte = ALIEN_BYTE_AT(receiver, index);
  else
    byte = ALIEN_PTR_AT(receiver, index);
  return as_smiOop((char)byte);
}

PRIM_DECL_3(byteArrayPrimitives::alienSignedByteAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienSignedByteAtPut", receiver, argument1, argument2);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument1->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!argument2->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  int index = smiOop(argument1)->value();
  int size = ALIEN_SIZE(receiver);
  int value = smiOop(argument2)->value();

  if (index < 1 || (size != 0 && index > abs(size)))
    return markSymbol(vmSymbols::index_not_valid());
  if (value < -128 || value > 127)
    return markSymbol(vmSymbols::argument_is_invalid());

  u_char byte = value;
  if (size > 0)
    ALIEN_BYTE_AT_PUT(receiver, argument1, byte);
  else
    ALIEN_PTR_AT(receiver, index) = byte;
  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienUnsignedShortAt, oop receiver, oop argument) {
  PROLOGUE_2("alienUnsignedShortAt", receiver, argument);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument->is_smi())
    return markSymbol(vmSymbols::argument_has_wrong_type());

  int size = ALIEN_SIZE(receiver);
  int index = smiOop(argument)->value();

  if (index < 1 || (size != 0 && index > abs(size) - 1))
    return markSymbol(vmSymbols::index_not_valid());

  unsigned short value;
  if (size > 0)
    value = *ALIEN_SHORT_AT(receiver, index);
  else
    value = *ALIEN_SHORT_PTR_AT(receiver, index);
  
  return as_smiOop(value);
}

PRIM_DECL_3(byteArrayPrimitives::alienUnsignedShortAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienUnsignedShortAtPut", receiver, argument1, argument2);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument1->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!argument2->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  
  int size = ALIEN_SIZE(receiver);
  int index = smiOop(argument1)->value();
  int value = smiOop(argument2)->value();
  
  if (index < 1 || (size != 0 && index > abs(size) - 1))
    return markSymbol(vmSymbols::index_not_valid());
  if (value < 0 || value > 65535)
    return markSymbol(vmSymbols::argument_is_invalid());

  if (size > 0)
    *ALIEN_SHORT_AT(receiver, index) = value;
  else
    *ALIEN_SHORT_PTR_AT(receiver, index) = value;
  
  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienSignedShortAt, oop receiver, oop argument) {
  PROLOGUE_2("alienSignedShortAt", receiver, argument);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument->is_smi())
    return markSymbol(vmSymbols::argument_has_wrong_type());

  int size = ALIEN_SIZE(receiver);
  int index = smiOop(argument)->value();

  if (index < 1 || (size != 0 && index > abs(size) - 1))
    return markSymbol(vmSymbols::index_not_valid());

  short value;
  if (size > 0)
    value = *ALIEN_SHORT_AT(receiver, index);
  else
    value = *ALIEN_SHORT_PTR_AT(receiver, index);
  
  return as_smiOop(value);
}

PRIM_DECL_3(byteArrayPrimitives::alienSignedShortAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienSignedShortAtPut", receiver, argument1, argument2);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument1->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!argument2->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  int size = ALIEN_SIZE(receiver);
  int index = smiOop(argument1)->value();
  int value = smiOop(argument2)->value();

  if (index < 1 || (size != 0 && index > abs(size) - 1))
    return markSymbol(vmSymbols::index_not_valid());
  if (value < -32768 || value > 32767)
    return markSymbol(vmSymbols::argument_is_invalid());

  if (size > 0)
    *ALIEN_SHORT_AT(receiver, index) = value;
  else
    *ALIEN_SHORT_PTR_AT(receiver, index) = value;

  return argument2;
}

PRIM_DECL_2(byteArrayPrimitives::alienUnsignedLongAt, oop receiver, oop argument) {
  PROLOGUE_2("alienUnsignedLongAt", receiver, argument);
  return nilObj;
}

PRIM_DECL_3(byteArrayPrimitives::alienUnsignedLongAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienUnsignedLongAtPut", receiver, argument1, argument2);
  return nilObj;
}

PRIM_DECL_2(byteArrayPrimitives::alienSignedLongAt, oop receiver, oop argument) {
  PROLOGUE_2("alienSignedLongAt", receiver, argument);
  return nilObj;
}

PRIM_DECL_3(byteArrayPrimitives::alienSignedLongAtPut, oop receiver, oop argument1, oop argument2) {
  PROLOGUE_3("alienSignedLongAtPut", receiver, argument1, argument2);
  return nilObj;
}

PRIM_DECL_1(byteArrayPrimitives::alienSize, oop receiver) {
  PROLOGUE_1("alienSize", receiver);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());

  return as_smiOop(ALIEN_SIZE(receiver));
}

PRIM_DECL_2(byteArrayPrimitives::alienSetSize, oop receiver, oop argument) {
  PROLOGUE_2("alienSetSize", receiver, argument);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (!argument->is_smi())
    return markSymbol(vmSymbols::argument_has_wrong_type());

  ALIEN_SIZE(receiver) = smiOop(argument)->value();
  return receiver;
}

PRIM_DECL_1(byteArrayPrimitives::alienAddress, oop receiver) {
  PROLOGUE_1("alienAddress", receiver);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (ALIEN_SIZE(receiver) > 0)
    return markSymbol(vmSymbols::illegal_state());

  int address = ALIEN_ADDRESS(receiver);
  int size = IntegerOps::int_to_Integer_result_size_in_bytes(address);

  oop largeInteger = Universe::find_global("LargeInteger");
  PersistentHandle z(klassOop(largeInteger)->klass_part()->allocateObjectSize(size));
  IntegerOps::int_to_Integer(address, byteArrayOop(z.as_oop())->number());
  return simplified(byteArrayOop(z.as_oop()));
}

PRIM_DECL_2(byteArrayPrimitives::alienSetAddress, oop receiver, oop argument) {
  PROLOGUE_2("alienSetAddress", receiver, argument);
  if (!receiver->is_byteArray())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (ALIEN_SIZE(receiver) > 0)
    return markSymbol(vmSymbols::illegal_state());
  if (!argument->is_smi() && 
     !(argument->is_byteArray() && byteArrayOop(argument)->number().signum() > 0))
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (argument->is_smi())
    ALIEN_ADDRESS(receiver) = smiOop(argument)->value();
  else {
    bool ok;
    int address = byteArrayOop(argument)->number().as_int(ok);
    if (!ok) return markSymbol(vmSymbols::argument_is_invalid());
    ALIEN_ADDRESS(receiver) = address;
  }

  return receiver;
}
