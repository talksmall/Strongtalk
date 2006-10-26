/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.7 $ */
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
# include "incls/_callBack_prims.cpp.incl"

TRACE_FUNC(TraceCallBackPrims, "callBack")

int callBackPrimitives::number_of_calls;

PRIM_DECL_2(callBackPrimitives::initialize, oop receiver, oop selector) {
  PROLOGUE_2("initialize", receiver, selector);
  if (!selector->is_symbol())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  callBack::initialize(receiver, symbolOop(selector));
  return receiver;
}

PRIM_DECL_3(callBackPrimitives::registerPascalCall, oop index, oop nofArgs, oop result) {
  PROLOGUE_3("registerPascalCall", index, nofArgs, result);
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!nofArgs->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::third_argument_has_wrong_type());
  proxyOop(result)->set_pointer(callBack::registerPascalCall(smiOop(index)->value(), smiOop(nofArgs)->value()));
  return result;
}

PRIM_DECL_2(callBackPrimitives::registerCCall, oop index, oop result) {
  PROLOGUE_2("registerCCall", index, result);
  if (!index->is_smi())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!result->is_proxy())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  proxyOop(result)->set_pointer(callBack::registerCCall(smiOop(index)->value()));
  return result;
}

PRIM_DECL_1(callBackPrimitives::unregister, oop proxy) {
  PROLOGUE_1("unregister", proxy);
  if (!proxy->is_proxy())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  callBack::unregister(proxyOop(proxy)->get_pointer());
  proxyOop(proxy)->set_pointer(NULL);
  return proxy;
}

typedef int (__stdcall *mytype)(int a, int b);

PRIM_DECL_1(callBackPrimitives::invokePascal, oop proxy) {
  PROLOGUE_1("invokePascal", proxy);
  if (!proxy->is_proxy())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  mytype f = (mytype) proxyOop(proxy)->get_pointer();
  return as_smiOop((*f) (10, 5));
}

PRIM_DECL_1(callBackPrimitives::invokeC, oop proxy) {
  PROLOGUE_1("invokeC", proxy);
  if (!proxy->is_proxy())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  fntype f = (fntype) proxyOop(proxy)->get_pointer();
  return as_smiOop( (int) (*f) (10, 5));
}

