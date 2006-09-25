/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.21 $ */
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
# include "incls/_method_prims.cpp.incl"

TRACE_FUNC(TraceMethodPrims, "method")

int methodOopPrimitives::number_of_calls;

#define ASSERT_RECEIVER assert(receiver->is_method(), "receiver must be method")

PRIM_DECL_1(methodOopPrimitives::selector, oop receiver) {
  PROLOGUE_1("selector", receiver);
  ASSERT_RECEIVER;
  return methodOop(receiver)->selector();
}

PRIM_DECL_1(methodOopPrimitives::debug_info, oop receiver) {
  PROLOGUE_1("debug_info", receiver);
  ASSERT_RECEIVER;
  return methodOop(receiver)->debugInfo();
}

PRIM_DECL_1(methodOopPrimitives::size_and_flags, oop receiver) {
  PROLOGUE_1("size_and_flags", receiver);
  ASSERT_RECEIVER;
  return methodOop(receiver)->size_and_flags();
}

PRIM_DECL_1(methodOopPrimitives::fileout_body, oop receiver) {
  PROLOGUE_1("fileout_body", receiver);
  ASSERT_RECEIVER;
  return methodOop(receiver)->fileout_body();
}

PRIM_DECL_2(methodOopPrimitives::setSelector, oop receiver, oop name) {
  PROLOGUE_2("setSelector", receiver, name);
  ASSERT_RECEIVER;
  if (!name->is_symbol())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  methodOop(receiver)->set_selector_or_method(oop(name));
  return receiver;
}

PRIM_DECL_1(methodOopPrimitives::numberOfArguments, oop receiver) {
  PROLOGUE_1("numberOfArguments", receiver);
  ASSERT_RECEIVER;
  return as_smiOop(methodOop(receiver)->number_of_arguments());
}

PRIM_DECL_1(methodOopPrimitives::outer, oop receiver) {
  PROLOGUE_1("outer", receiver);
  ASSERT_RECEIVER;
  return methodOop(receiver)->selector_or_method();
}

PRIM_DECL_2(methodOopPrimitives::setOuter, oop receiver, oop method) {
  PROLOGUE_2("setOuter", receiver, method);
  ASSERT_RECEIVER;
  methodOop(receiver)->set_selector_or_method(oop(method));
  return receiver;
}

PRIM_DECL_2(methodOopPrimitives::referenced_instance_variable_names, oop receiver, oop mixin) {
  PROLOGUE_2("referenced_instance_variable_names", receiver, mixin);
  ASSERT_RECEIVER;
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return methodOop(receiver)->referenced_instance_variable_names(mixinOop(mixin));
}

PRIM_DECL_1(methodOopPrimitives::referenced_class_variable_names, oop receiver) {
  PROLOGUE_1("referenced_class_variable_names", receiver);
  ASSERT_RECEIVER;
  return methodOop(receiver)->referenced_class_variable_names();
}

PRIM_DECL_1(methodOopPrimitives::referenced_global_names, oop receiver) {
  PROLOGUE_1("referenced_global_names", receiver);
  ASSERT_RECEIVER;
  return methodOop(receiver)->referenced_global_names();
}

PRIM_DECL_1(methodOopPrimitives::senders, oop receiver) {
  PROLOGUE_1("senders", receiver);
  ASSERT_RECEIVER;
  return methodOop(receiver)->senders();
}

PRIM_DECL_2(methodOopPrimitives::prettyPrint, oop receiver, oop klass) {
  PROLOGUE_2("prettyPrint", receiver, klass);
  ASSERT_RECEIVER;
  if (klass == nilObj) {
    prettyPrinter::print(methodOop(receiver));
  } else {
    if (!klass->is_klass())
      return markSymbol(vmSymbols::first_argument_has_wrong_type());
    prettyPrinter::print(methodOop(receiver), klassOop(klass));
  }
  return receiver;
}

PRIM_DECL_2(methodOopPrimitives::prettyPrintSource, oop receiver, oop klass) {
  PROLOGUE_2("prettyPrintSource", receiver, klass);
  ASSERT_RECEIVER;
  byteArrayOop a;
  if (klass == nilObj) {
    a = prettyPrinter::print_in_byteArray(methodOop(receiver));
  } else {
    if (!klass->is_klass())
      return markSymbol(vmSymbols::first_argument_has_wrong_type());
    a = prettyPrinter::print_in_byteArray(methodOop(receiver), klassOop(klass));
  }
  return a;
}

PRIM_DECL_1(methodOopPrimitives::printCodes, oop receiver) {
  PROLOGUE_1("printCodes", receiver);
  ASSERT_RECEIVER;
  #ifndef PRODUCT
  { ResourceMark rm;
    methodOop(receiver)->print_codes();
  }
  #endif
  return receiver;
}


PRIM_DECL_6(methodOopPrimitives::constructMethod, oop selector_or_method, oop flags, oop nofArgs, oop debugInfo, oop bytes, oop oops) {
  PROLOGUE_6("constructMethod", selector_or_method, flags, nofArgs, debugInfo, bytes, oops);
  if (!selector_or_method->is_symbol() && (selector_or_method != nilObj))
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!flags->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());
  if (!nofArgs->is_smi())
    return markSymbol(vmSymbols::third_argument_has_wrong_type());
  if (!debugInfo->is_objArray())
    return markSymbol(vmSymbols::fourth_argument_has_wrong_type());
  if (!bytes->is_byteArray())
    return markSymbol(vmSymbols::fifth_argument_has_wrong_type());
  if (!oops->is_objArray())
    return markSymbol(vmSymbols::sixth_argument_has_wrong_type());

  if (objArrayOop(oops)->length() * oopSize != byteArrayOop(bytes)->length()) {
	return markSymbol(vmSymbols::method_construction_failed());
  }

  methodKlass* k = (methodKlass*) Universe::methodKlassObj()->klass_part();
  methodOop result = k->constructMethod(selector_or_method,
                                        smiOop(flags)->value(),
                                        smiOop(nofArgs)->value(),
                                        objArrayOop(debugInfo),
					                    byteArrayOop(bytes),
					                    objArrayOop(oops));
  if (result) return result;
  return markSymbol(vmSymbols::method_construction_failed());
}

extern "C" blockClosureOop allocateBlock(smiOop nof);

static oop allocate_block_for(methodOop method, oop self) {
  BlockScavenge bs;

  if (!method->is_customized()) {
    method->customize_for(self->klass(), self->blueprint()->mixin());
  }

  // allocate the context for the block (make room for the self)
  contextKlass* ok = (contextKlass*) contextKlassObj->klass_part();
  contextOop con = contextOop(ok->allocateObjectSize(1));
  con->kill();
  con->obj_at_put(0, self);

  // allocate the resulting block
  blockClosureOop blk = allocateBlock(as_smiOop(method->number_of_arguments()));
  blk->set_method(method);
  blk->set_lexical_scope(con);

  return blk;
}

PRIM_DECL_1(methodOopPrimitives::allocate_block, oop receiver) {
  PROLOGUE_1("allocate_block", receiver);
  ASSERT_RECEIVER;

  if (!methodOop(receiver)->is_blockMethod())
    return markSymbol(vmSymbols::conversion_failed());

  return allocate_block_for(methodOop(receiver), nilObj);
}

PRIM_DECL_2(methodOopPrimitives::allocate_block_self, oop receiver, oop self) {
  PROLOGUE_2("allocate_block_self", receiver, self);
  ASSERT_RECEIVER;

  if (!methodOop(receiver)->is_blockMethod())
    return markSymbol(vmSymbols::conversion_failed());

  return allocate_block_for(methodOop(receiver), self);
}

static symbolOop symbol_from_method_inlining_info(methodOopDesc::Method_Inlining_Info info) { 
  if (info == methodOopDesc::normal_inline)  return oopFactory::new_symbol("Normal");
  if (info == methodOopDesc::never_inline)   return oopFactory::new_symbol("Never");
  if (info == methodOopDesc::always_inline)  return oopFactory::new_symbol("Always");
  ShouldNotReachHere();
  return NULL;
}

PRIM_DECL_2(methodOopPrimitives::set_inlining_info, oop receiver, oop info) {
  PROLOGUE_2("set_inlining_info", receiver, info);
  ASSERT_RECEIVER;
  if (!info->is_symbol())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  // Check argument value
  methodOopDesc::Method_Inlining_Info in;
  if (symbolOop(info)->equals("Never")) {
    in = methodOopDesc::never_inline;
  } else if(symbolOop(info)->equals("Always")) {
    in = methodOopDesc::always_inline;
  } else if(symbolOop(info)->equals("Normal")) {
    in = methodOopDesc::normal_inline;
  } else {
    return markSymbol(vmSymbols::argument_is_invalid());
  }
  methodOopDesc::Method_Inlining_Info old = methodOop(receiver)->method_inlining_info();
  methodOop(receiver)->set_method_inlining_info(in);
  return symbol_from_method_inlining_info(old);
}

PRIM_DECL_1(methodOopPrimitives::inlining_info, oop receiver) {
  PROLOGUE_1("inlining_info", receiver);
  ASSERT_RECEIVER;
  return symbol_from_method_inlining_info(methodOop(receiver)->method_inlining_info());
}
