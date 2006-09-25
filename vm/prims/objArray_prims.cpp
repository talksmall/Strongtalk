/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.15 $ */
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
# include "incls/_objArray_prims.cpp.incl"

TRACE_FUNC(TraceObjArrayPrims, "objArray")

int objArrayPrimitives::number_of_calls;

#define ASSERT_RECEIVER assert(receiver->is_objArray(), "receiver must be object array")

PRIM_DECL_2(objArrayPrimitives::allocateSize, oop receiver, oop argument) {
  PROLOGUE_2("allocateSize", receiver, argument);
  assert(receiver->is_klass() && klassOop(receiver)->klass_part()->oop_is_objArray(),
         "receiver must object array class");
  if (!argument->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  if (smiOop(argument)->value() < 0)
    return markSymbol(vmSymbols::negative_size());

  klassOop k        = klassOop(receiver);
  int      ni_size  = k->klass_part()->non_indexable_size();
  int      obj_size = ni_size + 1 + smiOop(argument)->value();
  // allocate
  objArrayOop obj = as_objArrayOop(Universe::allocate(obj_size, (memOop*)&k));
  // header
  memOop(obj)->initialize_header(k->klass_part()->has_untagged_contents(), k);
  // instance variables
  memOop(obj)->initialize_body(memOopDesc::header_size(), ni_size);
  // %optimized 'obj->set_length(size)'
  oop* base = (oop*) obj->addr();
  base[ni_size] = argument;
  memOop(obj)->initialize_body(ni_size+1, obj_size);
  return obj;
}

PRIM_DECL_1(objArrayPrimitives::size, oop receiver) {
  PROLOGUE_1("size", receiver);
  ASSERT_RECEIVER;
  // do the operation
  return as_smiOop(objArrayOop(receiver)->length());
}

PRIM_DECL_2(objArrayPrimitives::at, oop receiver, oop index) {
  PROLOGUE_2("at", receiver, index);
  ASSERT_RECEIVER;

  // check index type
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check index value
  if (!objArrayOop(receiver)->is_within_bounds(smiOop(index)->value()))
     return markSymbol(vmSymbols::out_of_bounds());

  // do the operation
  return objArrayOop(receiver)->obj_at(smiOop(index)->value());
}

PRIM_DECL_3(objArrayPrimitives::atPut, oop receiver, oop index, oop value) {
  PROLOGUE_3("atPut", receiver, index, value);
  ASSERT_RECEIVER;

  // check index type
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check index value
  if (!objArrayOop(receiver)->is_within_bounds(smiOop(index)->value()))
    return markSymbol(vmSymbols::out_of_bounds());

  // do the operation
  objArrayOop(receiver)->obj_at_put(smiOop(index)->value(), value);
  return receiver;
}


PRIM_DECL_2(objArrayPrimitives::at_all_put, oop receiver, oop obj) {
  PROLOGUE_2("at_all_put", receiver, obj);
  ASSERT_RECEIVER;

  int  length = objArrayOop(receiver)->length();
  if (obj->is_new() && receiver->is_old()) {
    // Do store checks
    for (int index = 1; index <= length; index++) {
      objArrayOop(receiver)->obj_at_put(index, obj); 
    }
  } else {
    // Ignore store check for speed
    set_oops(objArrayOop(receiver)->objs(1), length, obj);
  }
  return receiver;
}

PRIM_DECL_5(objArrayPrimitives::replace_from_to, oop receiver, oop from, oop to, oop source, oop start) {
  PROLOGUE_5("replace_from_to", receiver, from, to, source, start);
  ASSERT_RECEIVER;

  // check from type
  if (!from->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check to type
  if (!to->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // check source type
  if (!source->is_objArray())
    return markSymbol(vmSymbols::third_argument_has_wrong_type());

  // check start type
  if (!start->is_smi())
    return markSymbol(vmSymbols::third_argument_has_wrong_type());

  // check from > 0
  if (smiOop(from) <= 0)
    return markSymbol(vmSymbols::out_of_bounds());

  // check to < self size
  if (smiOop(to)->value() > objArrayOop(receiver)->length())
    return markSymbol(vmSymbols::out_of_bounds());

  // check from <= to
  if (smiOop(from)->value() > smiOop(to)->value())
    return markSymbol(vmSymbols::out_of_bounds());

  // check if source is big enough
  if (smiOop(start)->value() + (smiOop(to)->value() - smiOop(from)->value() + 1) > objArrayOop(source)->length())
    return markSymbol(vmSymbols::out_of_bounds());

  // Dispatch the operation to the array
  objArrayOop(receiver)->replace_from_to(smiOop(from)->value(), smiOop(to)->value(), objArrayOop(source), smiOop(start)->value());

  return receiver;
}

PRIM_DECL_4(objArrayPrimitives::copy_size, oop receiver, oop from, oop start, oop size) {
  PROLOGUE_4("copy_size", receiver, from, start, size);
  ASSERT_RECEIVER;

  // check from type
  if (!from->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // check start type
  if (!start->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // check size type
  if (!size->is_smi())
    return markSymbol(vmSymbols::third_argument_has_wrong_type());

  // check from > 0
  if (smiOop(from) <= 0)
    return markSymbol(vmSymbols::out_of_bounds());

  // check start > 0
  if (smiOop(start) <= 0)
    return markSymbol(vmSymbols::out_of_bounds());
  
  // Check size is positive
  if (smiOop(size)->value() < 0)
    return markSymbol(vmSymbols::negative_size());

  HandleMark hm;
  Handle saved_receiver(receiver); 

  // allocation of object array
  klassOop k        = receiver->klass();
  int      ni_size  = k->klass_part()->non_indexable_size();
  int      obj_size = ni_size + 1 + smiOop(size)->value();
  // allocate
  objArrayOop obj = as_objArrayOop(Universe::allocate(obj_size, (memOop*)&k));

  objArrayOop src = saved_receiver.as_objArray();

  // header
  memOop(obj)->initialize_header(k->klass_part()->has_untagged_contents(), k);
  // copy instance variables
  for (int index = memOopDesc::header_size(); index < ni_size; index++ ) {
     obj->raw_at_put(index, src->raw_at(index));
  }
  // length
  obj->set_length(smiOop(size)->value());
  // fill the array
  obj->replace_and_fill(smiOop(from)->value(), smiOop(start)->value(), src);

  return obj;
}