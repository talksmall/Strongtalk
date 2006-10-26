/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.14 $ */
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
# include "incls/_mixin_prims.cpp.incl"

TRACE_FUNC(TraceMixinPrims, "mixin")

int mixinOopPrimitives::number_of_calls;

PRIM_DECL_1(mixinOopPrimitives::number_of_methods, oop mixin) {
  PROLOGUE_1("number_of_methods", mixin);
  // Check parameter
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return as_smiOop(mixinOop(mixin)->number_of_methods());
}

PRIM_DECL_2(mixinOopPrimitives::method_at, oop mixin, oop index) {
  PROLOGUE_2("method_at", mixin, index);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!index->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  int i = smiOop(index)->value();  
  if (i > 0 && i <= mixinOop(mixin)->number_of_methods()) 
    return mixinOop(mixin)->method_at(i);
  return markSymbol(vmSymbols::out_of_bounds());
}

PRIM_DECL_2(mixinOopPrimitives::add_method, oop mixin, oop method) {
  PROLOGUE_2("add_method", mixin, method);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!method->is_method())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // Check the mixin is not installed
  if (mixinOop(mixin)->is_installed())
    return markSymbol(vmSymbols::is_installed());


  BlockScavenge bs;
  mixinOop(mixin)->add_method(methodOop(method));
  return mixin;
}

PRIM_DECL_2(mixinOopPrimitives::remove_method_at, oop mixin, oop index) {
  PROLOGUE_2("remove_method_at", mixin, index);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!index->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // Check the mixin is not installed
  if (mixinOop(mixin)->is_installed())
    return markSymbol(vmSymbols::is_installed());

  BlockScavenge bs;
  int i = smiOop(index)->value(); 
  if (i > 0 && i <= mixinOop(mixin)->number_of_methods()) 
    return mixinOop(mixin)->remove_method_at(i);
  return markSymbol(vmSymbols::out_of_bounds());
}

PRIM_DECL_1(mixinOopPrimitives::methods, oop mixin) {
  PROLOGUE_1("methods", mixin);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return mixinOop(mixin)->methods();
}

PRIM_DECL_1(mixinOopPrimitives::number_of_instance_variables, oop mixin) {
  PROLOGUE_1("number_of_instance_variables", mixin);
  // Check parameter
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return as_smiOop(mixinOop(mixin)->number_of_instVars());
}

PRIM_DECL_2(mixinOopPrimitives::instance_variable_at, oop mixin, oop index) {
  PROLOGUE_2("instance_variable_at", mixin, index);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!index->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  int i = smiOop(index)->value();
  if (i > 0 && i <= mixinOop(mixin)->number_of_instVars()) 
    return mixinOop(mixin)->instVar_at(i);
  return markSymbol(vmSymbols::out_of_bounds());
}

PRIM_DECL_2(mixinOopPrimitives::add_instance_variable, oop mixin, oop name) {
  PROLOGUE_2("add_instance_variable", mixin, name);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!name->is_symbol())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // Check the mixin is not installed
  if (mixinOop(mixin)->is_installed())
    return markSymbol(vmSymbols::is_installed());

  BlockScavenge bs;
  mixinOop(mixin)->add_instVar(symbolOop(name));
  return mixin;
}

PRIM_DECL_2(mixinOopPrimitives::remove_instance_variable_at, oop mixin, oop index) {
  PROLOGUE_2("remove_instance_variable_at", mixin, index);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!index->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // Check the mixin is not installed
  if (mixinOop(mixin)->is_installed())
    return markSymbol(vmSymbols::is_installed());


  BlockScavenge bs;
  int i = smiOop(index)->value();
  if (i > 0 && i <= mixinOop(mixin)->number_of_instVars()) 
    return mixinOop(mixin)->remove_instVar_at(i);
  return markSymbol(vmSymbols::out_of_bounds());
}

PRIM_DECL_1(mixinOopPrimitives::instance_variables, oop mixin) {
  PROLOGUE_1("instance_variables", mixin);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return mixinOop(mixin)->instVars();
}

PRIM_DECL_1(mixinOopPrimitives::number_of_class_variables, oop mixin) {
  PROLOGUE_1("number_of_class_variables", mixin);
  // Check parameter
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return as_smiOop(mixinOop(mixin)->number_of_classVars());
}

PRIM_DECL_2(mixinOopPrimitives::class_variable_at, oop mixin, oop index) {
  PROLOGUE_2("class_variable_at", mixin, index);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!index->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  int i = smiOop(index)->value();
  if (i > 0 && i <= mixinOop(mixin)->number_of_classVars()) 
    return mixinOop(mixin)->classVar_at(i);
  return markSymbol(vmSymbols::out_of_bounds());
}

PRIM_DECL_2(mixinOopPrimitives::add_class_variable, oop mixin, oop name) {
  PROLOGUE_2("add_class_variable", mixin, name);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!name->is_symbol())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // Check the mixin is not installed
  if (mixinOop(mixin)->is_installed())
    return markSymbol(vmSymbols::is_installed());

  BlockScavenge bs;
  mixinOop(mixin)->add_classVar(symbolOop(name));
  return mixin;
}

PRIM_DECL_2(mixinOopPrimitives::remove_class_variable_at, oop mixin, oop index) {
  PROLOGUE_2("remove_class_variable_at", mixin, index);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!index->is_smi())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // Check the mixin is not installed
  if (mixinOop(mixin)->is_installed())
    return markSymbol(vmSymbols::is_installed());

  BlockScavenge bs;
  int i = smiOop(index)->value();
  if (i > 0 && i <= mixinOop(mixin)->number_of_classVars()) 
    return mixinOop(mixin)->remove_classVar_at(i);
  return markSymbol(vmSymbols::out_of_bounds());
}

PRIM_DECL_1(mixinOopPrimitives::class_variables, oop mixin) {
  PROLOGUE_1("class_variables", mixin);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  return mixinOop(mixin)->classVars();
}

PRIM_DECL_1(mixinOopPrimitives::primary_invocation, oop mixin) {
  PROLOGUE_1("primary_invocation", mixin);
  // Check parameter
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return mixinOop(mixin)->primary_invocation();
}

PRIM_DECL_2(mixinOopPrimitives::set_primary_invocation, oop mixin, oop klass) {
  PROLOGUE_2("class_mixin", mixin, klass);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!klass->is_klass())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // Check the mixin is not installed
  if (mixinOop(mixin)->is_installed())
    return markSymbol(vmSymbols::is_installed());

  mixinOop(mixin)->set_primary_invocation(klassOop(klass));
  return mixin;
}

PRIM_DECL_1(mixinOopPrimitives::class_mixin, oop mixin) {
  PROLOGUE_1("class_mixin", mixin);
  // Check parameter
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return mixinOop(mixin)->class_mixin();
}

PRIM_DECL_2(mixinOopPrimitives::set_class_mixin, oop mixin, oop class_mixin) {
  PROLOGUE_2("class_mixin", mixin, class_mixin);
  // Check parameters
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());
  if (!class_mixin->is_mixin())
    return markSymbol(vmSymbols::second_argument_has_wrong_type());

  // Check the mixin is not installed
  if (mixinOop(mixin)->is_installed())
    return markSymbol(vmSymbols::is_installed());

  mixinOop(mixin)->set_class_mixin(mixinOop(class_mixin));
  return mixin;
}

PRIM_DECL_1(mixinOopPrimitives::is_installed, oop mixin) {
  PROLOGUE_1("is_installed", mixin);
  // Check parameter
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  return mixinOop(mixin)->is_installed() ? trueObj : falseObj;
}

PRIM_DECL_1(mixinOopPrimitives::set_installed, oop mixin) {
  PROLOGUE_1("set_installed", mixin);
  // Check parameter
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  mixinOop instance_mixin = mixinOop(mixin);
  mixinOop class_mixin    = instance_mixin->class_mixin();

  instance_mixin->set_installed(trueObj);
  if (class_mixin->is_mixin()) {
    class_mixin->set_installed(trueObj);
  }
  return mixin;
}

PRIM_DECL_1(mixinOopPrimitives::set_uninstalled, oop mixin) {
  PROLOGUE_1("set_uninstalled", mixin);
  // Check parameter
  if (!mixin->is_mixin())
    return markSymbol(vmSymbols::first_argument_has_wrong_type());

  mixinOop(mixin)->set_installed(falseObj);
  return mixin;
}
