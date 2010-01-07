/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.18 $ */
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
# include "incls/_proxy_prims.cpp.incl"

TRACE_FUNC(TraceProxyPrims, "proxy")

int proxyOopPrimitives::number_of_calls;

#define ASSERT_RECEIVER assert(receiver->is_proxy(), "receiver must be proxy")

#define ASSERT_RECEIVER_ACCESS					\
  ASSERT_RECEIVER						\
 /* if (proxyOop(receiver)->is_null())				\
    return markSymbol(vmSymbols::null_proxy_access());	*/	\


PRIM_DECL_1(proxyOopPrimitives::getSmi, oop receiver) {
  PROLOGUE_1("getSmi", receiver);
  ASSERT_RECEIVER;
  unsigned int value   = (unsigned int) proxyOop(receiver)->get_pointer();
  unsigned int topBits = value >> (BitsPerWord - Tag_Size);
  if ((topBits != 0) && (topBits != 3))
    return markSymbol(vmSymbols::smi_conversion_failed());
  return as_smiOop((int) value);
}

PRIM_DECL_2(proxyOopPrimitives::set, oop receiver, oop value) {
  PROLOGUE_2("getSmi", receiver, value);
  ASSERT_RECEIVER;
  if (value->is_smi()) {
    proxyOop(receiver)->set_pointer((void*) smiOop(value)->value());
  } else if (value->is_proxy()) {
    proxyOop(receiver)->set_pointer(proxyOop(value)->get_pointer());
  } else 
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return receiver;
}

PRIM_DECL_3(proxyOopPrimitives::setHighLow, oop receiver, oop high, oop low) {
  PROLOGUE_3("setHighLow", receiver, high, low);
  ASSERT_RECEIVER;
  if (!high->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!low->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  unsigned int h     = (unsigned int) smiOop(high)->value();
  unsigned int l     = (unsigned int) smiOop(low)->value();
  unsigned int value = (h << 16) | l;
  proxyOop(receiver)->set_pointer((void*) value);
  return receiver;
}

PRIM_DECL_1(proxyOopPrimitives::getHigh, oop receiver) {
  PROLOGUE_1("getHigh", receiver);
  ASSERT_RECEIVER;
  unsigned int value = (int) proxyOop(receiver)->get_pointer();
  value = value >> 16;
  return as_smiOop(value);
}

PRIM_DECL_1(proxyOopPrimitives::getLow, oop receiver) {
  PROLOGUE_1("getLow", receiver);
  ASSERT_RECEIVER;
  unsigned int value = (int) proxyOop(receiver)->get_pointer();
  value &= 0x0000ffff;
  return as_smiOop(value);
}

PRIM_DECL_1(proxyOopPrimitives::isNull, oop receiver) {
  PROLOGUE_1("isNull", receiver);
  ASSERT_RECEIVER;
  return proxyOop(receiver)->is_null() ? trueObj : falseObj;
}

PRIM_DECL_1(proxyOopPrimitives::isAllOnes, oop receiver) {
  PROLOGUE_1("isAllOnes", receiver);
  ASSERT_RECEIVER;
  return proxyOop(receiver)->is_allOnes() ? trueObj : falseObj;
}

PRIM_DECL_2(proxyOopPrimitives::malloc, oop receiver, oop size) {
  PROLOGUE_2("malloc", receiver, size);
  ASSERT_RECEIVER;
  if (!size->is_smi()) return markSymbol(vmSymbols::first_argument_has_wrong_type());
  proxyOop(receiver)->set_pointer(os::malloc(smiOop(size)->value()));
  return receiver;
}

PRIM_DECL_2(proxyOopPrimitives::calloc, oop receiver, oop size) {
  PROLOGUE_2("calloc", receiver, size);
  ASSERT_RECEIVER;
  if (!size->is_smi()) return markSymbol(vmSymbols::first_argument_has_wrong_type());
  proxyOop(receiver)->set_pointer(os::calloc(smiOop(size)->value(), 1));
  return receiver;
}
PRIM_DECL_1(proxyOopPrimitives::free, oop receiver) {
  PROLOGUE_1("free", receiver);
  ASSERT_RECEIVER;
  os::free(proxyOop(receiver)->get_pointer());
  proxyOop(receiver)->null_pointer();
  return receiver;
}


PRIM_DECL_2(proxyOopPrimitives::byteAt, oop receiver, oop offset) {
  PROLOGUE_2("byteAt", receiver, offset);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  return as_smiOop(proxyOop(receiver)->byte_at(smiOop(offset)->value()));
}

PRIM_DECL_3(proxyOopPrimitives::byteAtPut, oop receiver, oop offset, oop value) {
  PROLOGUE_3("byteAtPut", receiver, offset, value);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!value->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  proxyOop(receiver)->byte_at_put(smiOop(offset)->value(), smiOop(value)->value());
  return receiver;
}

PRIM_DECL_2(proxyOopPrimitives::doubleByteAt, oop receiver, oop offset) {
  PROLOGUE_2("doubleByteAt", receiver, offset);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  return as_smiOop(proxyOop(receiver)->doubleByte_at(smiOop(offset)->value()));
}

PRIM_DECL_3(proxyOopPrimitives::doubleByteAtPut, oop receiver, oop offset, oop value) {
  PROLOGUE_3("doubleByteAtPut", receiver, offset, value);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!value->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  proxyOop(receiver)->doubleByte_at_put(smiOop(offset)->value(), smiOop(value)->value());
  return receiver;
}

PRIM_DECL_2(proxyOopPrimitives::smiAt, oop receiver, oop offset) {
  PROLOGUE_2("smiAt", receiver, offset);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  unsigned int value   = (unsigned int) proxyOop(receiver)->long_at(smiOop(offset)->value());
  unsigned int topBits = value >> (BitsPerWord - Tag_Size);
  if ((topBits != 0) && (topBits != 3))
    return markSymbol(vmSymbols::smi_conversion_failed());
  return as_smiOop((int) value);
}

PRIM_DECL_3(proxyOopPrimitives::smiAtPut, oop receiver, oop offset, oop value) {
  PROLOGUE_3("smiAtPut", receiver, offset, value);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!value->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  proxyOop(receiver)->long_at_put(smiOop(offset)->value(), smiOop(value)->value());
  return receiver;
}

PRIM_DECL_3(proxyOopPrimitives::subProxyAt, oop receiver, oop offset, oop result) {
  PROLOGUE_3("subProxyAt", receiver, offset, result);
  ASSERT_RECEIVER;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  proxyOop(result)->set_pointer((void*) ((char*) proxyOop(receiver)->get_pointer() + smiOop(offset)->value()));
  return result;
}

PRIM_DECL_3(proxyOopPrimitives::proxyAt, oop receiver, oop offset, oop result) {
  PROLOGUE_3("proxyAt", receiver, offset, result);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  proxyOop(result)->set_pointer((void*)proxyOop(receiver)->long_at(smiOop(offset)->value()));
  return result;
}

PRIM_DECL_3(proxyOopPrimitives::proxyAtPut, oop receiver, oop offset, oop value) {
  PROLOGUE_3("proxyAtPut", receiver, offset, value);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!value->is_proxy())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  proxyOop(receiver)->long_at_put(smiOop(offset)->value(), (long)proxyOop(value)->get_pointer());
  return receiver;
}

PRIM_DECL_2(proxyOopPrimitives::singlePrecisionFloatAt, oop receiver, oop offset) {
  PROLOGUE_2("singlePrecisionFloatAt", receiver, offset);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  return oopFactory::new_double((double) proxyOop(receiver)->float_at(smiOop(offset)->value()));
}

PRIM_DECL_3(proxyOopPrimitives::singlePrecisionFloatAtPut, oop receiver, oop offset, oop value) {
  PROLOGUE_3("singlePrecisionFloatAtPut", receiver, offset, value);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!value->is_double())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  proxyOop(receiver)->float_at_put(smiOop(offset)->value(), (float) doubleOop(value)->value());
  return receiver;
}

PRIM_DECL_2(proxyOopPrimitives::doublePrecisionFloatAt, oop receiver, oop offset) {
  PROLOGUE_2("doublePrecisionFloatAt", receiver, offset);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  return oopFactory::new_double(proxyOop(receiver)->double_at(smiOop(offset)->value()));
}

PRIM_DECL_3(proxyOopPrimitives::doublePrecisionFloatAtPut, oop receiver, oop offset, oop value) {
  PROLOGUE_3("doublePrecisionFloatAtPut", receiver, offset, value);
  ASSERT_RECEIVER_ACCESS;
  if (!offset->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!value->is_double())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());

  proxyOop(receiver)->double_at_put(smiOop(offset)->value(), doubleOop(value)->value());
  return receiver;
}

static bool convert_to_arg(oop arg, int* addr) {
  if (arg->is_smi()) {
    *addr = smiOop(arg)->value();
    return true;
  }
  if (arg->is_proxy()) {
    *addr = (int) proxyOop(arg)->get_pointer();
    return true;
  }
  return false;
}

typedef void* (__stdcall *call_out_func_0)();
PRIM_DECL_2(proxyOopPrimitives::callOut0, oop receiver, oop result) {
  PROLOGUE_2("callOut0", receiver, result);
  ASSERT_RECEIVER_ACCESS;

  if (!receiver->is_proxy())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  call_out_func_0 f = (call_out_func_0) proxyOop(receiver)->get_pointer();
  proxyOop(result)->set_pointer((*f)());
  return result;
}

typedef void* (__stdcall *call_out_func_1)(int a);
PRIM_DECL_3(proxyOopPrimitives::callOut1, oop receiver, oop arg1, oop result) {
  PROLOGUE_3("callOut1", receiver, arg1, result);
  ASSERT_RECEIVER_ACCESS;

  int a1;
  if (!receiver->is_proxy())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());
  if (!convert_to_arg(arg1, &a1)) 
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  call_out_func_1 f = (call_out_func_1) proxyOop(receiver)->get_pointer();
  proxyOop(result)->set_pointer((*f)(a1));
  return result;
}

typedef void* (__stdcall *call_out_func_2)(int a, int b);
PRIM_DECL_4(proxyOopPrimitives::callOut2, oop receiver, oop arg1, oop arg2, oop result) {
  PROLOGUE_4("callOut2", receiver, arg1, arg2, result);
  ASSERT_RECEIVER_ACCESS;

  int a1, a2;
  if (!receiver->is_proxy())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());
  if (!convert_to_arg(arg1, &a1)) 
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!convert_to_arg(arg2, &a2)) 
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::third_argument_has_wrong_type());

  call_out_func_2 f = (call_out_func_2) proxyOop(receiver)->get_pointer();
  proxyOop(result)->set_pointer((*f)(a1, a2));
  return result;
}

typedef void* (__stdcall *call_out_func_3)(int a, int b, int c);
PRIM_DECL_5(proxyOopPrimitives::callOut3, oop receiver, oop arg1, oop arg2, oop arg3, oop result) {
  PROLOGUE_5("callOut3", receiver, arg1, arg2, arg3, result);
  ASSERT_RECEIVER_ACCESS;

  int a1, a2, a3;
  if (!receiver->is_proxy())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());
  if (!convert_to_arg(arg1, &a1)) 
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!convert_to_arg(arg2, &a2)) 
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!convert_to_arg(arg3, &a3)) 
    return markSymbol(vmSymbols::third_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::fourth_argument_has_wrong_type());

  call_out_func_3 f = (call_out_func_3) proxyOop(receiver)->get_pointer();
  proxyOop(result)->set_pointer((*f)(a1, a2, a3));
  return result;
}

typedef void* (__stdcall *call_out_func_4)(int a, int b, int c, int d);
PRIM_DECL_6(proxyOopPrimitives::callOut4, oop receiver, oop arg1, oop arg2, oop arg3, oop arg4, oop result) {
  PROLOGUE_6("callOut4", receiver, arg1, arg2, arg3, arg4, result);
  ASSERT_RECEIVER_ACCESS;

  int a1, a2, a3, a4;
  if (!receiver->is_proxy())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());
  if (!convert_to_arg(arg1, &a1)) 
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!convert_to_arg(arg2, &a2)) 
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!convert_to_arg(arg3, &a3)) 
    return markSymbol(vmSymbols::third_argument_has_wrong_type());
  if (!convert_to_arg(arg4, &a4)) 
    return markSymbol(vmSymbols::fourth_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::fifth_argument_has_wrong_type());

  call_out_func_4 f = (call_out_func_4) proxyOop(receiver)->get_pointer();
  proxyOop(result)->set_pointer((*f)(a1, a2, a3, a4));
  return result;
}

typedef void* (__stdcall *call_out_func_5)(int a, int b, int c, int d, int e);

PRIM_DECL_7(proxyOopPrimitives::callOut5, oop receiver, oop arg1, oop arg2, oop arg3, oop arg4, oop arg5, oop result) {
  PROLOGUE_7("callOut5", receiver, arg1, arg2, arg3, arg4, arg5, result);
  ASSERT_RECEIVER_ACCESS;

  int a1, a2, a3, a4, a5;
  if (!receiver->is_proxy())
    return markSymbol(vmSymbols::receiver_has_wrong_type());
  if (proxyOop(receiver)->is_null())
    return markSymbol(vmSymbols::illegal_state());
  if (!convert_to_arg(arg1, &a1)) 
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!convert_to_arg(arg2, &a2)) 
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!convert_to_arg(arg3, &a3)) 
    return markSymbol(vmSymbols::third_argument_has_wrong_type());
  if (!convert_to_arg(arg4, &a4)) 
    return markSymbol(vmSymbols::fourth_argument_has_wrong_type());
  if (!convert_to_arg(arg5, &a5)) 
    return markSymbol(vmSymbols::fifth_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::sixth_argument_has_wrong_type());

  call_out_func_5 f = (call_out_func_5) proxyOop(receiver)->get_pointer();
  proxyOop(result)->set_pointer((*f)(a1, a2, a3, a4, a5));
  return result;
}
