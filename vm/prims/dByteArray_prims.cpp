/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.18 $ */
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
# include "incls/_dByteArray_prims.cpp.incl"

TRACE_FUNC(TraceDoubleByteArrayPrims, "doubleByteArray")

int doubleByteArrayPrimitives::number_of_calls;

#define ASSERT_RECEIVER assert(receiver->is_doubleByteArray(), "receiver must be double byte array")


PRIM_DECL_2(doubleByteArrayPrimitives::allocateSize, oop receiver, oop argument) {
  PROLOGUE_2("allocateSize", receiver, argument)
  assert(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_doubleByteArray(),
         "receiver must double byte array class");
  if (!argument->is_smi())
    markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (smiOop(argument)->value() < 0)
    return markSymbol(vmSymbols::negative_size());

  klassOop k        = klassOop(receiver);
  int      ni_size  = k->klass_part()->non_indexable_size();
  int      obj_size = ni_size + 1 + roundTo(smiOop(argument)->value() * 2, oopSize) / oopSize;
  // allocate
  doubleByteArrayOop obj = as_doubleByteArrayOop(Universe::allocate(obj_size, (memOop*)&k));
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
  //               obj->doubleByte_at_put(index, 0)'
  base = &base[ni_size+1];
  while (base < end) *base++ = (oop) 0;
  return obj;
}

PRIM_DECL_1(doubleByteArrayPrimitives::size, oop receiver) {
  PROLOGUE_1("size", receiver);
  ASSERT_RECEIVER;

  // do the operation
  return as_smiOop(doubleByteArrayOop(receiver)->length());
}

PRIM_DECL_2(doubleByteArrayPrimitives::at, oop receiver, oop index) {
  PROLOGUE_2("at", receiver, index);
  ASSERT_RECEIVER;

  // check index type
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check index value
  if (!doubleByteArrayOop(receiver)->is_within_bounds(smiOop(index)->value()))
     return markSymbol(vmSymbols::out_of_bounds());

  return as_smiOop(doubleByteArrayOop(receiver)->doubleByte_at(smiOop(index)->value()));
}

PRIM_DECL_3(doubleByteArrayPrimitives::atPut, oop receiver, oop index, oop value) {
  PROLOGUE_3("atPut", receiver, index, value);
  ASSERT_RECEIVER;

  // check index type
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check value type
  if (!value->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // check index value
  if (!doubleByteArrayOop(receiver)->is_within_bounds(smiOop(index)->value()))
     return markSymbol(vmSymbols::out_of_bounds());

  // check value as double byte
  unsigned int v = (unsigned int) smiOop(value)->value();
  if (v  >= (1<<16))
      return markSymbol(vmSymbols::value_out_of_range());

  // do the operation
  doubleByteArrayOop(receiver)->doubleByte_at_put(smiOop(index)->value(), v);
  return receiver;
}


PRIM_DECL_2(doubleByteArrayPrimitives::compare, oop receiver, oop argument) {
  PROLOGUE_2("compare", receiver, argument);
  ASSERT_RECEIVER;

  if(receiver == argument)
    return as_smiOop(0);

  if (argument->is_doubleByteArray())
    return as_smiOop(doubleByteArrayOop(receiver)->compare(doubleByteArrayOop(argument)));

  if (argument->is_byteArray())
    return as_smiOop(- byteArrayOop(argument)->compare_doubleBytes(doubleByteArrayOop(receiver)));

  return markSymbol(vmSymbols::first_argument_has_wrong_type());
}

PRIM_DECL_1(doubleByteArrayPrimitives::intern, oop receiver) {
  PROLOGUE_1("intern", receiver);
  ASSERT_RECEIVER;

  ResourceMark rm;
  int   len    = doubleByteArrayOop(receiver)->length();
  char* buffer = NEW_RESOURCE_ARRAY(char, len);

  for (int i = 0; i < len; i++) {
    int c = doubleByteArrayOop(receiver)->doubleByte_at(i + 1);
    if (c >= (1<<8)) {
      return markSymbol(vmSymbols::value_out_of_range());
    }
    buffer[i] = c;
  }
  symbolOop sym = Universe::symbol_table->lookup(buffer, len);
  return sym;
}

PRIM_DECL_2(doubleByteArrayPrimitives::characterAt, oop receiver, oop index) {
  PROLOGUE_2("characterAt", receiver, index);
  ASSERT_RECEIVER;

  // check index type
  if (!index->is_smi()) 
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // range check
  if (!doubleByteArrayOop(receiver)->is_within_bounds(smiOop(index)->value()))
    return markSymbol(vmSymbols::out_of_bounds());

  // fetch double byte
  doubleByte byte = doubleByteArrayOop(receiver)->doubleByte_at(smiOop(index)->value());
 
  if (byte < 256) {
    // return the byte+1'th element in asciiCharacter
    return Universe::asciiCharacters()->obj_at(byte+1);
  } else return markSymbol(vmSymbols::out_of_bounds());
}


PRIM_DECL_1(doubleByteArrayPrimitives::hash, oop receiver) {
  PROLOGUE_1("intern", receiver);
  ASSERT_RECEIVER;
  return as_smiOop(doubleByteArrayOop(receiver)->hash_value());
}
