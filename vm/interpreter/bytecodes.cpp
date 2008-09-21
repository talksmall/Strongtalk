/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.29 $ */
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
# include "incls/_bytecodes.cpp.incl"


char*			Bytecodes::_entry_point[number_of_codes];
char*			Bytecodes::_name[number_of_codes];
Bytecodes::Format	Bytecodes::_format[number_of_codes];
Bytecodes::CodeType     Bytecodes::_code_type[number_of_codes];
Bytecodes::ArgumentSpec	Bytecodes::_argument_spec[number_of_codes];
Bytecodes::SendType	Bytecodes::_send_type[number_of_codes];
bool			Bytecodes::_single_step[number_of_codes];
bool			Bytecodes::_pop_tos[number_of_codes];


void Bytecodes::def(Code code) {
  def(code, "undefined", UNDEF, miscellaneous, false, no_args, no_send, false);
}

  
void Bytecodes::def(Code code, char* name, Format format, CodeType code_type, bool single_step, bool pop_tos) {
  def(code, name, format, code_type, single_step, no_args, no_send, pop_tos);
}


void Bytecodes::def(Code code, char* name, Format format, ArgumentSpec argument_spec, SendType send_type, bool pop_tos) {
  assert(send_type != no_send, "must be a send");
  def(code, name, format, message_send, true, argument_spec, send_type, pop_tos);
}


void Bytecodes::def(Code code, char* name, Format format, CodeType code_type, bool single_step, ArgumentSpec argument_spec, SendType send_type, bool pop_tos) {
  assert(0 <= code && code < number_of_codes, "out of bounds");
  assert(_name[code] == NULL, "bytecode defined twice");

  // plausibility checks for arguments (compare with naming convention)
  #ifdef ASSERT
  if (format == UNDEF) {
    // bytecode name should be "undefined"
    for (int i = 0; i < 9; i++) {
      assert(name[i] == "undefined"[i], "inconsistency with naming convention");
    }
  }

  if (pop_tos) {
    // bytecode name unlikely to start with "push_"... and should end with ..."_pop"
    assert(name[0] != 'p' || name[1] != 'u' || name[2] != 's' || name[3] != 'h' || name[4] != '_', "unlikely naming");

    int i = 0;
    while (name[i] != '\00') i++;
    // Fix this lars
    // assert(name[i-4] == '_' && name[i-3] == 'p' && name[i-2] == 'o' && name[i-1] == 'p', "inconsistency with naming convention");
  }

  if (argument_spec != no_args) {
    // must be a send
    assert(send_type != no_send, "inconsistency between argument_spec and send_type");
  }

  if (code_type == control_struc) {
    // not intercepted on single step
    assert(!single_step, "control structures cannot be single stepped");
  }

  if (code_type == float_operation) {
    // bytecode name should start with "float_"
    for (int i = 0; i < 6; i++) {
      assert(name[i] == "float_"[i], "inconsistency with naming convention");
    }
  }
  #endif

  // set table entries
  _entry_point[code]   = NULL;
  _name[code]          = name;
  _format[code]        = format;
  _code_type[code]     = code_type;
  _argument_spec[code] = argument_spec;
  _send_type[code]     = send_type;
  _single_step[code]   = single_step;
  _pop_tos[code]       = pop_tos;
}


extern "C" doFn original_table[Bytecodes::number_of_codes];

void Bytecodes::set_entry_point(Code code, char* entry_point) {
  assert(is_defined(code), "undefined byte code");
  assert(entry_point != NULL, "not a valid entry_point");
  _entry_point[code] = entry_point;
  original_table[code] = (doFn)entry_point;
}


void Bytecodes::init() {
  // to allow check for complete initialization at end of init
  #ifdef ASSERT
  int i;
  for (i = 0; i < number_of_codes; i++) {
    _name[Code(i)] = NULL;
  }
  #endif

  // for better readability
  const bool do_sst = true;
  const bool no_sst = false;
  const bool pop    = true;

  def(push_temp_0,			"push_temp_0",				B,	local_access,	no_sst);
  def(push_temp_1,			"push_temp_1",				B,	local_access,	no_sst);
  def(push_temp_2,			"push_temp_2",				B,	local_access,	no_sst);
  def(push_temp_3,			"push_temp_3",				B,	local_access,	no_sst);
  def(push_temp_4,			"push_temp_4",				B,	local_access,	no_sst);
  def(push_temp_5,			"push_temp_5",				B,	local_access,	no_sst);
  def(unimplemented_06);
  def(push_temp_n,			"push_temp_n",				BB,	local_access,	no_sst);
  def(push_arg_1,			"push_arg_1",				B,	local_access,	no_sst);
  def(push_arg_2,			"push_arg_2",				B,	local_access,	no_sst);
  def(push_arg_3,			"push_arg_3",				B,	local_access,	no_sst);
  def(push_arg_n,			"push_arg_n",				BB,	local_access,	no_sst);
  def(allocate_temp_1,			"allocate_temp_1",			B,	miscellaneous,	no_sst);
  def(allocate_temp_2,			"allocate_temp_2",			B,	miscellaneous,	no_sst);
  def(allocate_temp_3,			"allocate_temp_3",			B,	miscellaneous,	no_sst);
  def(allocate_temp_n,			"allocate_temp_n",			BB,	miscellaneous,	no_sst);

  def(store_temp_0_pop,			"store_temp_0_pop",			B,	local_access,	do_sst,	pop);
  def(store_temp_1_pop,			"store_temp_1_pop",			B,	local_access,	do_sst,	pop);
  def(store_temp_2_pop,			"store_temp_2_pop",			B,	local_access,	do_sst,	pop);
  def(store_temp_3_pop,			"store_temp_3_pop",			B,	local_access,	do_sst,	pop);
  def(store_temp_4_pop,			"store_temp_4_pop",			B,	local_access,	do_sst,	pop);
  def(store_temp_5_pop,			"store_temp_5_pop",			B,	local_access,	do_sst,	pop);
  def(store_temp_n,			"store_temp_n",				BB,	local_access,	do_sst);
  def(store_temp_n_pop,			"store_temp_n_pop",			BB,	local_access,	do_sst,	pop);
  def(push_neg_n,			"push_neg_n",				BB,	miscellaneous,	no_sst);
  def(push_succ_n,			"push_succ_n",				BB,	miscellaneous,	no_sst);
  def(push_literal,			"push_literal",				BO,	miscellaneous,	no_sst);
  def(push_tos,				"push_tos",				B,	miscellaneous,	no_sst);
  def(push_self,			"push_self",				B,	miscellaneous,	no_sst);
  def(push_nil,				"push_nil",				B,	miscellaneous,	no_sst);
  def(push_true,			"push_true",				B,	miscellaneous,	no_sst);
  def(push_false,			"push_false",				B,	miscellaneous,	no_sst);

  def(unimplemented_20);
  def(unimplemented_21);
  def(unimplemented_22);
  def(unimplemented_23);
  def(unimplemented_24);
  def(unimplemented_25);
  def(unimplemented_26);
  def(unimplemented_27);
  def(return_instVar_name,		"return_instVar_name",			BO,	instVar_access,	no_sst);
  def(push_classVar,			"push_classVar",			BO,	classVar_access,no_sst);
  def(store_classVar_pop,		"store_classVar_pop",			BO,	classVar_access,do_sst,	pop);
  def(store_classVar,			"store_classVar",			BO,	classVar_access,do_sst);
  def(return_instVar,			"return_instVar",			BL,	instVar_access,	do_sst);
  def(push_instVar,			"push_instVar",				BL,	instVar_access,	no_sst);
  def(store_instVar_pop,		"store_instVar_pop",			BL,	instVar_access,	do_sst,	pop);
  def(store_instVar,			"store_instVar",			BL,	instVar_access,	do_sst);

  def(float_allocate,			"float_allocate",			BBBB,	float_operation,no_sst);
  def(float_floatify_pop,		"float_floatify_pop",			BB,	float_operation,no_sst, pop);
  def(float_move,			"float_move",				BBB,	float_operation,no_sst);
  def(float_set,			"float_set",				BBO,	float_operation,no_sst);
  def(float_nullary_op,			"float_nullary_op",			BBB,	float_operation,no_sst);
  def(float_unary_op,			"float_unary_op",			BBB,	float_operation,no_sst);
  def(float_binary_op,			"float_binary_op",			BBB,	float_operation,no_sst);
  def(float_unary_op_to_oop,		"float_unary_op_to_oop",		BBB,	float_operation,no_sst);
  def(float_binary_op_to_oop,		"float_binary_op_to_oop",		BBB,	float_operation,no_sst);
  def(unimplemented_39);
  def(unimplemented_3a);
  def(unimplemented_3b);
  def(unimplemented_3c);
  def(push_instVar_name,		"push_instVar_name",			BO,	instVar_access,	no_sst);
  def(store_instVar_pop_name,		"store_instVar_pop_name",		BO,	instVar_access,	no_sst,	pop);
  def(store_instVar_name,		"store_instVar_name",			BO,	instVar_access,	no_sst);

  def(push_temp_0_context_0,		"push_temp_0_context_0",		B,	context_access,	no_sst);
  def(push_temp_1_context_0,		"push_temp_1_context_0",		B,	context_access,	no_sst);
  def(push_temp_2_context_0,		"push_temp_2_context_0",		B,	context_access,	no_sst);
  def(push_temp_n_context_0,		"push_temp_n_context_0",		BB,	context_access,	no_sst);
  def(store_temp_0_context_0_pop,	"store_temp_0_context_0_pop",		B,	context_access,	do_sst,	pop);
  def(store_temp_1_context_0_pop,	"store_temp_1_context_0_pop",		B,	context_access,	do_sst,	pop);
  def(store_temp_2_context_0_pop,	"store_temp_2_context_0_pop",		B,	context_access,	do_sst,	pop);
  def(store_temp_n_context_0_pop,	"store_temp_n_context_0_pop",		BB,	context_access,	do_sst,	pop);
  def(push_new_closure_context_0,	"push_new_closure_context_0",		BO,	new_closure,	no_sst);
  def(push_new_closure_context_1,	"push_new_closure_context_1",		BO,	new_closure,	no_sst);
  def(push_new_closure_context_2,	"push_new_closure_context_2",		BO,	new_closure,	no_sst);
  def(push_new_closure_context_n,	"push_new_closure_context_n",		BBO,	new_closure,	no_sst);
  def(install_new_context_method_0,	"install_new_context_method_0",		B,	new_context,	no_sst);
  def(install_new_context_method_1,	"install_new_context_method_1",		B,	new_context,	no_sst);
  def(install_new_context_method_2,	"install_new_context_method_2",		B,	new_context,	no_sst);
  def(install_new_context_method_n,	"install_new_context_method_n",		BB,	new_context,	no_sst);

  def(push_temp_0_context_1,		"push_temp_0_context_1",		B,	context_access,	no_sst);
  def(push_temp_1_context_1,		"push_temp_1_context_1",		B,	context_access,	no_sst);
  def(push_temp_2_context_1,		"push_temp_2_context_1",		B,	context_access,	no_sst);
  def(push_temp_n_context_1,		"push_temp_n_context_1",		BB,	context_access,	no_sst);
  def(store_temp_0_context_1_pop,	"store_temp_0_context_1_pop",		B,	context_access,	do_sst,	pop);
  def(store_temp_1_context_1_pop,	"store_temp_1_context_1_pop",		B,	context_access,	do_sst,	pop);
  def(store_temp_2_context_1_pop,	"store_temp_2_context_1_pop",		B,	context_access,	do_sst,	pop);
  def(store_temp_n_context_1_pop,	"store_temp_n_context_1_pop",		BB,	context_access,	do_sst,	pop);
  def(push_new_closure_tos_0,		"push_new_closure_tos_0",		BO,	new_closure,	no_sst);
  def(push_new_closure_tos_1,		"push_new_closure_tos_1",		BO,	new_closure,	no_sst);
  def(push_new_closure_tos_2,		"push_new_closure_tos_2",		BO,	new_closure,	no_sst);
  def(push_new_closure_tos_n,		"push_new_closure_tos_n",		BBO,	new_closure,	no_sst);
  def(only_pop,				"only_pop",				B,	new_context,	no_sst,	pop);
  def(install_new_context_block_1,	"install_new_context_block_1",		B,	new_context,	no_sst);
  def(install_new_context_block_2,	"install_new_context_block_2",		B,	new_context,	no_sst);
  def(install_new_context_block_n,	"install_new_context_block_n",		BB,	new_context,	no_sst);

  def(push_temp_0_context_n,		"push_temp_0_context_n",		BB,	context_access,	no_sst);
  def(push_temp_1_context_n,		"push_temp_1_context_n",		BB,	context_access,	no_sst);
  def(push_temp_2_context_n,		"push_temp_2_context_n",		BB,	context_access,	no_sst);
  def(push_temp_n_context_n,		"push_temp_n_context_n",		BBB,	context_access,	no_sst);
  def(store_temp_0_context_n_pop,	"store_temp_0_context_n_pop",		BB,	context_access,	do_sst,	pop);
  def(store_temp_1_context_n_pop,	"store_temp_1_context_n_pop",		BB,	context_access,	do_sst,	pop);
  def(store_temp_2_context_n_pop,	"store_temp_2_context_n_pop",		BB,	context_access,	do_sst,	pop);
  def(store_temp_n_context_n_pop,	"store_temp_n_context_n_pop",		BBB,	context_access,	do_sst, pop);
  def(set_self_via_context,		"set_self_via_context",			B,	context_access,	no_sst);
  def(copy_1_into_context,		"copy_1_into_context",			BB,	context_access,	no_sst);
  def(copy_2_into_context,		"copy_2_into_context",			BBB,	context_access,	no_sst);
  def(copy_n_into_context,		"copy_n_into_context",			BBS,	context_access,	no_sst);
  def(copy_self_into_context,		"copy_self_into_context",		B,	context_access,	no_sst);
  def(copy_self_1_into_context,		"copy_self_1_into_context",		BB,	context_access,	no_sst);
  def(copy_self_2_into_context,		"copy_self_2_into_context",		BBB,	context_access,	no_sst);
  def(copy_self_n_into_context,		"copy_self_n_into_context",		BBS,	context_access,	no_sst);

  def(ifTrue_byte,			"ifTrue_byte",				BBB,	control_struc,	no_sst);
  def(ifFalse_byte,			"ifFalse_byte",				BBB,	control_struc,	no_sst);
  def(and_byte,				"and_byte",				BB,	control_struc,	no_sst);
  def(or_byte,				"or_byte",				BB,	control_struc,	no_sst);
  def(whileTrue_byte,			"whileTrue_byte",			BB,	control_struc,	no_sst);
  def(whileFalse_byte,			"whileFalse_byte",			BB,	control_struc,	no_sst);
  def(jump_else_byte,			"jump_else_byte",			BB,	control_struc,	no_sst);
  def(jump_loop_byte,			"jump_loop_byte",			BBB,	control_struc,	no_sst);
  def(ifTrue_word,			"ifTrue_word",				BBL,	control_struc,	no_sst);
  def(ifFalse_word,			"ifFalse_word",				BBL,	control_struc,	no_sst);
  def(and_word,				"and_word",				BL,	control_struc,	no_sst);
  def(or_word,				"or_word",				BL,	control_struc,	no_sst);
  def(whileTrue_word,			"whileTrue_word",			BL,	control_struc,	no_sst);
  def(whileFalse_word,			"whileFalse_word",			BL,	control_struc,	no_sst);
  def(jump_else_word,			"jump_else_word",			BL,	control_struc,	no_sst);
  def(jump_loop_word,			"jump_loop_word",			BLL,	control_struc,	no_sst);

  def(interpreted_send_0,		"interpreted_send_0",			BOO,	recv_0_args,	interpreted_send);
  def(interpreted_send_1,		"interpreted_send_1",			BOO,	recv_1_args,	interpreted_send);
  def(interpreted_send_2,		"interpreted_send_2",			BOO,	recv_2_args,	interpreted_send);
  def(interpreted_send_n,		"interpreted_send_n",			BBOO,	recv_n_args,	interpreted_send);
  def(interpreted_send_0_pop,		"interpreted_send_0_pop",		BOO,	recv_0_args,	interpreted_send,	pop);
  def(interpreted_send_1_pop,		"interpreted_send_1_pop",		BOO,	recv_1_args,	interpreted_send,	pop);
  def(interpreted_send_2_pop,		"interpreted_send_2_pop",		BOO,	recv_2_args,	interpreted_send,	pop);
  def(interpreted_send_n_pop,		"interpreted_send_n_pop",		BBOO,	recv_n_args,	interpreted_send,	pop);
  def(interpreted_send_self,		"interpreted_send_self",		BOO,	args_only,	interpreted_send);
  def(interpreted_send_self_pop,	"interpreted_send_self_pop",		BOO,	args_only,	interpreted_send,	pop);
  def(interpreted_send_super,		"interpreted_send_super",		BOO,	args_only,	interpreted_send);
  def(interpreted_send_super_pop,	"interpreted_send_super_pop",		BOO,	args_only,	interpreted_send,	pop);
  def(return_tos_pop_0,			"return_tos_pop_0",			B,	miscellaneous,	do_sst);
  def(return_tos_pop_1,			"return_tos_pop_1",			B,	miscellaneous,	do_sst);
  def(return_tos_pop_2,			"return_tos_pop_2",			B,	miscellaneous,	do_sst);
  def(return_tos_pop_n,			"return_tos_pop_n",			BB,	miscellaneous,	do_sst);

  def(polymorphic_send_0,		"polymorphic_send_0",			BOO,	recv_0_args,	polymorphic_send);
  def(polymorphic_send_1,		"polymorphic_send_1",			BOO,	recv_1_args,	polymorphic_send);
  def(polymorphic_send_2,		"polymorphic_send_2",			BOO,	recv_2_args,	polymorphic_send);
  def(polymorphic_send_n,		"polymorphic_send_n",			BBOO,	recv_n_args,	polymorphic_send);
  def(polymorphic_send_0_pop,		"polymorphic_send_0_pop",		BOO,	recv_0_args,	polymorphic_send,	pop);
  def(polymorphic_send_1_pop,		"polymorphic_send_1_pop",		BOO,	recv_1_args,	polymorphic_send,	pop);
  def(polymorphic_send_2_pop,		"polymorphic_send_2_pop",		BOO,	recv_2_args,	polymorphic_send,	pop);
  def(polymorphic_send_n_pop,		"polymorphic_send_n_pop",		BBOO,	recv_n_args,	polymorphic_send,	pop);
  def(polymorphic_send_self,		"polymorphic_send_self",		BOO,	args_only,	polymorphic_send);
  def(polymorphic_send_self_pop,	"polymorphic_send_self_pop",		BOO,	args_only,	polymorphic_send,	pop);
  def(polymorphic_send_super,		"polymorphic_send_super",		BOO,	args_only,	polymorphic_send);
  def(polymorphic_send_super_pop,	"polymorphic_send_super_pop",		BOO,	args_only,	polymorphic_send,	pop);
  def(return_self_pop_0,		"return_self_pop_0",			B,	miscellaneous,	do_sst);
  def(return_self_pop_1,		"return_self_pop_1",			B,	miscellaneous,	do_sst);
  def(return_self_pop_2,		"return_self_pop_2",			B,	miscellaneous,	do_sst);
  def(return_self_pop_n,		"return_self_pop_n",			BB,	miscellaneous,	do_sst);

  def(compiled_send_0,			"compiled_send_0",			BLO,	recv_0_args,	compiled_send);
  def(compiled_send_1,			"compiled_send_1",			BLO,	recv_1_args,	compiled_send);
  def(compiled_send_2,			"compiled_send_2",			BLO,	recv_2_args,	compiled_send);
  def(compiled_send_n,			"compiled_send_n",			BBLO,	recv_n_args,	compiled_send);
  def(compiled_send_0_pop,		"compiled_send_0_pop",			BLO,	recv_0_args,	compiled_send,		pop);
  def(compiled_send_1_pop,		"compiled_send_1_pop",			BLO,	recv_1_args,	compiled_send,		pop);
  def(compiled_send_2_pop,		"compiled_send_2_pop",			BLO,	recv_2_args,	compiled_send,		pop);
  def(compiled_send_n_pop,		"compiled_send_n_pop",			BBLO,	recv_n_args,	compiled_send,		pop);
  def(compiled_send_self,		"compiled_send_self",			BLO,	args_only,	compiled_send);
  def(compiled_send_self_pop,		"compiled_send_self_pop",		BLO,	args_only,	compiled_send,		pop);
  def(compiled_send_super,		"compiled_send_super",			BLO,	args_only,	compiled_send);
  def(compiled_send_super_pop,		"compiled_send_super_pop",		BLO,	args_only,	compiled_send,		pop);
  def(return_tos_zap_pop_n,		"return_tos_zap_pop_n",			BB,	miscellaneous,	do_sst);
  def(return_self_zap_pop_n,		"return_self_zap_pop_n",		BB,	miscellaneous,	do_sst);
  def(non_local_return_tos_pop_n,	"non_local_return_tos_pop_n",		BB,	nonlocal_return,do_sst);
  def(non_local_return_self_pop_n,	"non_local_return_self_pop_n",		BB,	nonlocal_return,do_sst);

  def(prim_call,			"prim_call",				BL,	primitive_call,	do_sst);
  def(predict_prim_call,		"predict_prim_call",			BL,	primitive_call,	do_sst);
  def(prim_call_failure,		"prim_call_failure",			BLL,	primitive_call,	do_sst);
  def(predict_prim_call_failure,	"predict_prim_call_failure",		BLL,	primitive_call,	do_sst);
  def(dll_call_sync,			"dll_call_sync",			BOOLB,	dll_call,	do_sst);
  def(prim_call_self,			"prim_call_self",			BL,	primitive_call,	do_sst);
  def(prim_call_self_failure,		"prim_call_self_failure",		BLL,	primitive_call,	do_sst);
  def(unimplemented_b7);
  def(access_send_self,			"access_send_self",			BOO,	args_only,	accessor_send);
  def(primitive_send_0,			"primitive_send_0",			BOO,	recv_0_args,	primitive_send);
  def(primitive_send_super,		"primitive_send_super",			BOO,	args_only,	primitive_send);
  def(primitive_send_super_pop,		"primitive_send_super_pop",		BOO,	args_only,	primitive_send,		pop);
  def(unimplemented_bc);
  def(primitive_send_1,			"primitive_send_1",			BOO,	recv_1_args,	primitive_send);
  def(primitive_send_2,			"primitive_send_2",			BOO,	recv_2_args,	primitive_send);
  def(primitive_send_n,			"primitive_send_n",			BBOO,	recv_n_args,	primitive_send);

  def(prim_call_lookup,			"prim_call_lookup",			BO,	primitive_call,	no_sst);
  def(predict_prim_call_lookup,		"predict_prim_call_lookup",		BO,	primitive_call,	no_sst);
  def(prim_call_failure_lookup,		"prim_call_failure_lookup",		BOL,	primitive_call,	no_sst);
  def(predict_prim_call_failure_lookup,	"predict_prim_call_failure_lookup",	BOL,	primitive_call,	no_sst);
  def(dll_call_async,			"dll_call_async",			BOOLB,	dll_call,	do_sst);
  def(prim_call_self_lookup,		"prim_call_self_lookup",		BO,	primitive_call,	no_sst);
  def(prim_call_self_failure_lookup,	"prim_call_self_failure_lookup",	BOL,	primitive_call,	no_sst);
  def(unimplemented_c7);
  def(access_send_0,			"access_send_0",			BOO,	recv_0_args,	accessor_send);
  def(primitive_send_0_pop,		"primitive_send_0_pop",			BOO,	recv_0_args,	primitive_send,		pop);
  def(primitive_send_self,		"primitive_send_self",			BOO,	args_only,	primitive_send);
  def(primitive_send_self_pop,		"primitive_send_self_pop",		BOO,	args_only,	primitive_send,		pop);
  def(unimplemented_cc);
  def(primitive_send_1_pop,		"primitive_send_1_pop",			BOO,	recv_1_args,	primitive_send,		pop);
  def(primitive_send_2_pop,		"primitive_send_2_pop",			BOO,	recv_2_args,	primitive_send,		pop);
  def(primitive_send_n_pop,		"primitive_send_n_pop",			BBOO,	recv_n_args,	primitive_send,		pop);

  def(megamorphic_send_0,		"megamorphic_send_0",			BOO,	recv_0_args,	megamorphic_send);
  def(megamorphic_send_1,		"megamorphic_send_1",			BOO,	recv_1_args,	megamorphic_send);
  def(megamorphic_send_2,		"megamorphic_send_2",			BOO,	recv_2_args,	megamorphic_send);
  def(megamorphic_send_n,		"megamorphic_send_n",			BBOO,	recv_n_args,	megamorphic_send);
  def(megamorphic_send_0_pop,		"megamorphic_send_0_pop",		BOO,	recv_0_args,	megamorphic_send,	pop);
  def(megamorphic_send_1_pop,		"megamorphic_send_1_pop",		BOO,	recv_1_args,	megamorphic_send,	pop);
  def(megamorphic_send_2_pop,		"megamorphic_send_2_pop",		BOO,	recv_2_args,	megamorphic_send,	pop);
  def(megamorphic_send_n_pop,		"megamorphic_send_n_pop",		BBOO,	recv_n_args,	megamorphic_send,	pop);
  def(megamorphic_send_self,		"megamorphic_send_self",		BOO,	args_only,	megamorphic_send);
  def(megamorphic_send_self_pop,	"megamorphic_send_self_pop",		BOO,	args_only,	megamorphic_send,	pop);
  def(megamorphic_send_super,		"megamorphic_send_super",		BOO,	args_only,	megamorphic_send);
  def(megamorphic_send_super_pop,	"megamorphic_send_super_pop",		BOO,	args_only,	megamorphic_send,	pop);
  def(unimplemented_dc);
  def(special_primitive_send_1_hint,	"special_primitive_send_1_hint",	BB,	miscellaneous,	no_sst);
  def(unimplemented_de);
  def(unimplemented_df);

  def(smi_add,				"smi_add",				BOO,	recv_1_args,	predicted_send);
  def(smi_sub,				"smi_sub",				BOO,	recv_1_args,	predicted_send);
  def(smi_mult,				"smi_mult",				BOO,	recv_1_args,	predicted_send);
  def(smi_div,				"smi_div",				BOO,	recv_1_args,	predicted_send);
  def(smi_mod,				"smi_mod",				BOO,	recv_1_args,	predicted_send);
  def(smi_create_point,			"smi_create_point",			BOO,	recv_1_args,	predicted_send);
  def(smi_equal,			"smi_equal",				BOO,	recv_1_args,	predicted_send);
  def(smi_not_equal,			"smi_not_equal",			BOO,	recv_1_args,	predicted_send);
  def(smi_less,				"smi_less",				BOO,	recv_1_args,	predicted_send);
  def(smi_less_equal,			"smi_less_equal",			BOO,	recv_1_args,	predicted_send);
  def(smi_greater,			"smi_greater",				BOO,	recv_1_args,	predicted_send);
  def(smi_greater_equal,		"smi_greater_equal",			BOO,	recv_1_args,	predicted_send);
  def(objArray_at,			"objArray_at",				BOO,	recv_1_args,	predicted_send);
  def(objArray_at_put,			"objArray_at_put",			BOO,	recv_1_args,	predicted_send);
  def(double_equal,			"double_equal",				B,	miscellaneous,	do_sst);
  def(double_tilde,			"double_tilde",				B,	miscellaneous,	do_sst);

  def(push_global,			"push_global",				BO,	global_access,	no_sst);
  def(store_global_pop,			"store_global_pop",			BO,	global_access,	do_sst,	pop);
  def(store_global,			"store_global",				BO,	global_access,	do_sst);
  def(push_classVar_name,		"push_classVar_name",			BO,	classVar_access,no_sst);
  def(store_classVar_pop_name,		"store_classVar_pop_name",		BO,	classVar_access,no_sst,	pop);
  def(store_classVar_name,		"store_classVar_name",			BO,	classVar_access,no_sst);
  def(smi_and,				"smi_and",				BOO,	recv_1_args,	predicted_send);
  def(smi_or,				"smi_or",				BOO,	recv_1_args,	predicted_send);
  def(smi_xor,				"smi_xor",				BOO,	recv_1_args,	predicted_send);
  def(smi_shift,			"smi_shift",				BOO,	recv_1_args,	predicted_send);
  def(unimplemented_fa);
  def(unimplemented_fb);
  def(unimplemented_fc);
  def(unimplemented_fd);
  def(unimplemented_fe);
  def(halt,				"halt",					B,	control_struc,	no_sst);

  // check if all bytecodes have been initialized
  #ifdef ASSERT
  for (i = 0; i < number_of_codes; i++) {
    assert(_name[Code(i)] != NULL, "bytecode table not fully initialized");
  }
  #endif
}


// The dispatchTable controls the dispatch of byte codes

Bytecodes::LoopType Bytecodes::loop_type(Code code) {
  switch(code) {
    case Bytecodes::jump_loop_byte      : // fall through
    case Bytecodes::jump_loop_word      : return loop_start;
    case Bytecodes::whileTrue_byte      : // fall through
    case Bytecodes::whileFalse_byte     : // fall through
    case Bytecodes::whileTrue_word      : // fall through
    case Bytecodes::whileFalse_word     : return loop_end;
    default                             : return no_loop;
  }
  ShouldNotReachHere();
}


char* Bytecodes::format_as_string(Format format) {
  switch (format) {
    case B    : return "B";
    case BB   : return "BB";
    case BBB  : return "BBB";
    case BBBB : return "BBBB";
    case BBO  : return "BBO";
    case BBL  : return "BBL";
    case BO   : return "BO";
    case BOO  : return "BOO";
    case BLO  : return "BLO";
    case BOL  : return "BOL";
    case BLL  : return "BLL";
    case BL   : return "BL";
    case BLB  : return "BLB";
    case BBOO : return "BBOO";
    case BBLO : return "BBLO";
    case BOOLB: return "BOOLB";
    case BBS  : return "BBS";
    case UNDEF: return "UNDEF";
  }
  ShouldNotReachHere();
  return NULL;
}


char* Bytecodes::send_type_as_string(SendType send_type) {
  switch (send_type) {
    case interpreted_send: return "interpreted_send";
    case compiled_send   : return "compiled_send";
    case polymorphic_send: return "polymorphic_send";
    case megamorphic_send: return "megamorphic_send";
    case predicted_send  : return "predicted_send";
    case accessor_send   : return "accessor send";
    case primitive_send  : return "primitive_send";
    case no_send         : return "no_send";
  }
  ShouldNotReachHere();
  return NULL;
}


char* Bytecodes::code_type_as_string(CodeType code_type) {
  switch (code_type) {
    case local_access   : return "local_access";
    case instVar_access : return "instVar_access";
    case context_access : return "context_access";
    case classVar_access: return "classVar_access";
    case global_access  : return "global_access";
    case new_closure    : return "new_closure";
    case new_context    : return "new_context";
    case control_struc  : return "control_struc";
    case message_send   : return "message_send";
    case nonlocal_return: return "nonlocal_return";
    case primitive_call : return "primitive_call";
    case dll_call       : return "dll_call";
    case float_operation: return "float_operation";
    case miscellaneous  : return "miscellaneous";
  }
  ShouldNotReachHere();
  return NULL;
}


char* Bytecodes::argument_spec_as_string(ArgumentSpec argument_spec) {
  switch (argument_spec) {
    case recv_0_args: return "recv_0_args";
    case recv_1_args: return "recv_1_args";
    case recv_2_args: return "recv_2_args";
    case recv_n_args: return "recv_n_args";
    case args_only  : return "args_only";
    case no_args    : return "no_args";
  }
  ShouldNotReachHere();
  return NULL;
}


char* Bytecodes::loop_type_as_string(LoopType loop_type) {
  switch (loop_type) {
    case loop_start: return "loop_start";
    case loop_end  : return "loop_end";
    case no_loop   : return "no_loop";
  }
  ShouldNotReachHere();
  return NULL;
}


bool Bytecodes::is_self_send(Bytecodes::Code code) {
  switch (code) {
    case interpreted_send_self		: // fall through
    case compiled_send_self		: // fall through
    case polymorphic_send_self		: // fall through
    case megamorphic_send_self		: // fall through
    case interpreted_send_self_pop	: // fall through
    case compiled_send_self_pop		: // fall through
    case polymorphic_send_self_pop	: // fall through
    case megamorphic_send_self_pop	: return true;
    default				: return false;
  }
  ShouldNotReachHere();
  return false;
}


bool Bytecodes::is_super_send(Bytecodes::Code code) {
  switch (code) {
    case interpreted_send_super		: // fall through
    case compiled_send_super		: // fall through
    case polymorphic_send_super		: // fall through
    case megamorphic_send_super		: // fall through
    case interpreted_send_super_pop	: // fall through
    case compiled_send_super_pop	: // fall through
    case polymorphic_send_super_pop	: // fall through
    case megamorphic_send_super_pop	: return true;
    default				: return false;
  }
  ShouldNotReachHere();
  return false;
}


bool Bytecodes::has_access_send_code(Bytecodes::Code code) {
  switch (code) {
    case interpreted_send_0		: // fall through
    case interpreted_send_self		: return true;
    default				: return false;
  }
  ShouldNotReachHere();
  return false;
}


bool Bytecodes::has_predicted_send_code(Bytecodes::Code code) {
  switch (code) {
    case interpreted_send_1		: return true;
    default				: return false;
  }
  ShouldNotReachHere();
  return false;
}


Bytecodes::Code Bytecodes::original_send_code_for(Bytecodes::Code code) {
  switch (code) {
    case smi_add			: // fall through
    case smi_sub			: // fall through
    case smi_mult			: // fall through
    case smi_div			: // fall through
    case smi_mod			: // fall through
    case smi_create_point		: // fall through
    case smi_equal			: // fall through
    case smi_not_equal			: // fall through
    case smi_less			: // fall through
    case smi_less_equal			: // fall through
    case smi_greater			: // fall through
    case smi_greater_equal		: // fall through
    case smi_and			: // fall through
    case smi_or				: // fall through
    case smi_xor			: // fall through
    case smi_shift			: // fall through
    case objArray_at			: // fall through
    case objArray_at_put		: // fall through
    case double_equal			: // fall through
    case double_tilde			: return code;
    default				: return interpreted_send_code_for(code);
  }
  ShouldNotReachHere();
  return halt;
}


Bytecodes::Code Bytecodes::interpreted_send_code_for(Bytecodes::Code code) {
  switch (code) {
    case interpreted_send_0		: // fall through
    case compiled_send_0		: // fall through
    case primitive_send_0		: // fall through
    case polymorphic_send_0		: // fall through
    case megamorphic_send_0		: return interpreted_send_0;

    case interpreted_send_0_pop		: // fall through
    case compiled_send_0_pop		: // fall through
    case primitive_send_0_pop		: // fall through
    case polymorphic_send_0_pop		: // fall through
    case megamorphic_send_0_pop		: return interpreted_send_0_pop;

    case interpreted_send_1		: // fall through
    case compiled_send_1		: // fall through
    case primitive_send_1		: // fall through
    case polymorphic_send_1		: // fall through
    case megamorphic_send_1		: return interpreted_send_1;

    case interpreted_send_1_pop		: // fall through
    case compiled_send_1_pop		: // fall through
    case primitive_send_1_pop		: // fall through
    case polymorphic_send_1_pop		: // fall through
    case megamorphic_send_1_pop		: return interpreted_send_1_pop;

    case interpreted_send_2		: // fall through
    case compiled_send_2		: // fall through
    case primitive_send_2		: // fall through
    case polymorphic_send_2		: // fall through
    case megamorphic_send_2		: return interpreted_send_2;

    case interpreted_send_2_pop		: // fall through
    case compiled_send_2_pop		: // fall through
    case primitive_send_2_pop		: // fall through
    case polymorphic_send_2_pop		: // fall through
    case megamorphic_send_2_pop		: return interpreted_send_2_pop;

    case interpreted_send_n		: // fall through
    case compiled_send_n		: // fall through
    case primitive_send_n		: // fall through
    case polymorphic_send_n		: // fall through
    case megamorphic_send_n		: return interpreted_send_n;

    case interpreted_send_n_pop		: // fall through
    case compiled_send_n_pop		: // fall through
    case primitive_send_n_pop		: // fall through
    case polymorphic_send_n_pop		: // fall through
    case megamorphic_send_n_pop		: return interpreted_send_n_pop;

    case interpreted_send_self		: // fall through
    case compiled_send_self		: // fall through
    case primitive_send_self		: // fall through
    case polymorphic_send_self		: // fall through
    case megamorphic_send_self		: return interpreted_send_self;

    case interpreted_send_self_pop	: // fall through
    case compiled_send_self_pop		: // fall through
    case primitive_send_self_pop	: // fall through
    case polymorphic_send_self_pop	: // fall through
    case megamorphic_send_self_pop	: return interpreted_send_self_pop;

    case interpreted_send_super		: // fall through
    case compiled_send_super		: // fall through
    case primitive_send_super		: // fall through
    case polymorphic_send_super		: // fall through
    case megamorphic_send_super		: return interpreted_send_super;

    case interpreted_send_super_pop	: // fall through
    case compiled_send_super_pop	: // fall through
    case primitive_send_super_pop	: // fall through
    case polymorphic_send_super_pop	: // fall through
    case megamorphic_send_super_pop	: return interpreted_send_super_pop;

    case access_send_0			: return interpreted_send_0;
    case access_send_self		: return interpreted_send_self;

    case smi_add			: // fall through
    case smi_sub			: // fall through
    case smi_mult			: // fall through
    case smi_div			: // fall through
    case smi_mod			: // fall through
    case smi_create_point		: // fall through
    case smi_equal			: // fall through
    case smi_not_equal			: // fall through
    case smi_less			: // fall through
    case smi_less_equal			: // fall through
    case smi_greater			: // fall through
    case smi_greater_equal		: // fall through
    case smi_and			: // fall through
    case smi_or				: // fall through
    case smi_xor			: // fall through
    case smi_shift			: // fall through
    case double_equal			: // fall through
    case double_tilde			: // fall through
    case objArray_at			: return interpreted_send_1;
    case objArray_at_put		: return interpreted_send_2;

    default				: fatal("not a send bytecode");
  }
  ShouldNotReachHere();
  return halt;
}


Bytecodes::Code Bytecodes::compiled_send_code_for(Bytecodes::Code code) {
  switch (interpreted_send_code_for(code)) {
    case interpreted_send_0		: return compiled_send_0;
    case interpreted_send_1		: return compiled_send_1;
    case interpreted_send_2		: return compiled_send_2;
    case interpreted_send_n		: return compiled_send_n;
    case interpreted_send_self		: return compiled_send_self;
    case interpreted_send_super		: return compiled_send_super;

    case interpreted_send_0_pop		: return compiled_send_0_pop;
    case interpreted_send_1_pop		: return compiled_send_1_pop;
    case interpreted_send_2_pop		: return compiled_send_2_pop;
    case interpreted_send_n_pop		: return compiled_send_n_pop;
    case interpreted_send_self_pop	: return compiled_send_self_pop;
    case interpreted_send_super_pop	: return compiled_send_super_pop;

    default				: fatal("no corresponding compiled send code");
  }
  ShouldNotReachHere();
  return halt;
}


Bytecodes::Code Bytecodes::access_send_code_for(Bytecodes::Code code) {
  switch (interpreted_send_code_for(code)) {
    case interpreted_send_0		: return access_send_0;
    case interpreted_send_self		: return access_send_self;
    default				: fatal("no corresponding access send code");
  }
  ShouldNotReachHere();
  return halt;
}


Bytecodes::Code Bytecodes::primitive_send_code_for(Bytecodes::Code code) {
  switch (interpreted_send_code_for(code)) {
    case interpreted_send_0		: return primitive_send_0;
    case interpreted_send_1		: return primitive_send_1;
    case interpreted_send_2		: return primitive_send_2;
    case interpreted_send_n		: return primitive_send_n;
    case interpreted_send_self		: return primitive_send_self;
    case interpreted_send_super		: return primitive_send_super;

    case interpreted_send_0_pop		: return primitive_send_0_pop;
    case interpreted_send_1_pop		: return primitive_send_1_pop;
    case interpreted_send_2_pop		: return primitive_send_2_pop;
    case interpreted_send_n_pop		: return primitive_send_n_pop;
    case interpreted_send_self_pop	: return primitive_send_self_pop;
    case interpreted_send_super_pop	: return primitive_send_super_pop;

    default				: fatal("no corresponding primitive send code");
  }
  ShouldNotReachHere();
  return halt;
}


Bytecodes::Code Bytecodes::polymorphic_send_code_for(Bytecodes::Code code) {
  switch (interpreted_send_code_for(code)) {
    case interpreted_send_0		: return polymorphic_send_0;
    case interpreted_send_1		: return polymorphic_send_1;
    case interpreted_send_2		: return polymorphic_send_2;
    case interpreted_send_n		: return polymorphic_send_n;
    case interpreted_send_self		: return polymorphic_send_self;
    case interpreted_send_super		: return polymorphic_send_super;

    case interpreted_send_0_pop		: return polymorphic_send_0_pop;
    case interpreted_send_1_pop		: return polymorphic_send_1_pop;
    case interpreted_send_2_pop		: return polymorphic_send_2_pop;
    case interpreted_send_n_pop		: return polymorphic_send_n_pop;
    case interpreted_send_self_pop	: return polymorphic_send_self_pop;
    case interpreted_send_super_pop	: return polymorphic_send_super_pop;

    default				: fatal("no corresponding polymorphic send code");
  }
  ShouldNotReachHere();
  return halt;
}


Bytecodes::Code Bytecodes::megamorphic_send_code_for(Bytecodes::Code code) {
  switch (interpreted_send_code_for(code)) {
    case interpreted_send_0		: return megamorphic_send_0;
    case interpreted_send_1		: return megamorphic_send_1;
    case interpreted_send_2		: return megamorphic_send_2;
    case interpreted_send_n		: return megamorphic_send_n;
    case interpreted_send_self		: return megamorphic_send_self;
    case interpreted_send_super		: return megamorphic_send_super;

    case interpreted_send_0_pop		: return megamorphic_send_0_pop;
    case interpreted_send_1_pop		: return megamorphic_send_1_pop;
    case interpreted_send_2_pop		: return megamorphic_send_2_pop;
    case interpreted_send_n_pop		: return megamorphic_send_n_pop;
    case interpreted_send_self_pop	: return megamorphic_send_self_pop;
    case interpreted_send_super_pop	: return megamorphic_send_super_pop;

    default				: fatal("no corresponding megamorphic send code");
  }
  ShouldNotReachHere();
  return halt;
}


Bytecodes::Code Bytecodes::original_primitive_call_code_for(Code code) {
  switch (code) {
    case prim_call:			
    case prim_call_lookup:              return prim_call_lookup;
    case prim_call_self:
    case prim_call_self_lookup:         return prim_call_self_lookup;
    case prim_call_failure:
    case prim_call_failure_lookup:      return prim_call_failure_lookup;
    case prim_call_self_failure:
    case prim_call_self_failure_lookup: return prim_call_self_failure_lookup;
    default: fatal("no corresponding primitive call code");
  }
  ShouldNotReachHere();
  return halt;
}


Bytecodes::Code Bytecodes::primitive_call_code_for(Code code) {
  switch (code) {
    case prim_call_lookup		: return prim_call;
    case prim_call_self_lookup		: return prim_call_self;
    case prim_call_failure_lookup	: return prim_call_failure;
    case prim_call_self_failure_lookup	: return prim_call_self_failure;
    default				: fatal("no corresponding primitive call code");
  }
  ShouldNotReachHere();
  return halt;
}


void Bytecodes::print() {
  for (int i = 0; i < number_of_codes; i++) {
    Code code = Code(i);
    if (is_defined(code)) {
      std->print("%s\n", name(code));
      std->print("  %s\n", code_type_as_string(code_type(code)));
      std->print("  %s\n", send_type_as_string(send_type(code)));
      std->cr();
    }
  }
}


// Smalltalk generation

#ifndef PRODUCT

static void generate_comment() {
  std->print("\t\"\n");
  std->print("\tGenerated method - do not modify manually\n");
  std->print("\t(use delta +GenerateSmalltalk to generate).\n");
  std->print("\t\"\n");
}


static bool actually_generated(Bytecodes::Code code) {
  return
    Bytecodes::is_defined(code) &&
    ( Bytecodes::send_type(code) == Bytecodes::no_send          ||
      Bytecodes::send_type(code) == Bytecodes::interpreted_send ||
      Bytecodes::send_type(code) == Bytecodes::predicted_send
    );
}


static void generate_instr_method() {
  std->print("instr: i\n");
  generate_comment();
  std->print("\t| h l |\n");
  std->print("\th := i // 16r10.\n");
  std->print("\tl := i \\\\ 16r10.\n\n");
  for (int h = 0; h < 0x10; h++) {
    std->print("\th = 16r%X ifTrue: [\n", h);
    for (int l = 0; l < 0x10; l++) {
      Bytecodes::Code code = Bytecodes::Code(h*0x10 + l);
      if (actually_generated(code)) {
        std->print("\t\tl = 16r%X\tifTrue:\t[ ^ '%s' ].\n", l, Bytecodes::name(code));
      }
    }
    std->print("\t\t^ ''\n");
    std->print("\t].\n\n");
  }
  std->print("\tself halt\n");
  std->print("!\n\n");
}


static void print_table_entry_for(char* selector, int code) {
  std->print("\tselector = #%s\t\tifTrue: [ ^ 16r%02X ].\n", selector, code);
}


static void generate_codeForPrimitive_method() {
  std->print("codeForPrimitive: selector\n");
  generate_comment();
  print_table_entry_for("primitiveAdd:ifFail:",			Bytecodes::smi_add		);
  print_table_entry_for("primitiveSubtract:ifFail:",		Bytecodes::smi_sub		);
  print_table_entry_for("primitiveMultiply:ifFail:",		Bytecodes::smi_mult		);

  print_table_entry_for("primitiveSmallIntegerEqual:ifFail:",	Bytecodes::smi_equal		);
  print_table_entry_for("primitiveSmallIntegerNotEqual:ifFail:",Bytecodes::smi_not_equal	);
  print_table_entry_for("primitiveLessThan:ifFail:",		Bytecodes::smi_less		);
  print_table_entry_for("primitiveLessThanOrEqual:ifFail:",	Bytecodes::smi_less_equal	);
  print_table_entry_for("primitiveGreaterThan:ifFail:",		Bytecodes::smi_greater		);
  print_table_entry_for("primitiveGreaterThanOrEqual:ifFail:",	Bytecodes::smi_greater_equal	);
  
  print_table_entry_for("primitiveBitAnd:ifFail:",		Bytecodes::smi_and		);
  print_table_entry_for("primitiveBitOr:ifFail:",		Bytecodes::smi_or		);
  print_table_entry_for("primitiveBitXor:ifFail:",		Bytecodes::smi_xor		);
  print_table_entry_for("primitiveRawBitShift:ifFail:",		Bytecodes::smi_shift		);
  std->print("\t^ nil\n");
  std->print("!\n\n");
}


static void generate_signature(char* sig, char separator) {
  int i = 1;
  int b_cnt = 1;
  int w_cnt = 1;
  int o_cnt = 1;
  int s_cnt = 1;
  while (sig[i] != '\0') {
    std->put(separator);
    switch (sig[i]) {
      case 'B': std->print("byte: b%d", b_cnt++); break;
      case 'L': std->print("word: w%d", w_cnt++); break;
      case 'O': std->print("oop: o%d", o_cnt++); break;
      case 'S': std->print("bytes: s%d", s_cnt++); break;
      default : ShouldNotReachHere();
    }
    separator = ' ';
    i++;
  }
}


static bool has_instVar_access(Bytecodes::Code code) {
  return Bytecodes::code_type(code) == Bytecodes::instVar_access;
}


static bool has_classVar_access(Bytecodes::Code code) {
  return Bytecodes::code_type(code) == Bytecodes::classVar_access;
}


static bool has_inline_cache(Bytecodes::Code code) {
  return Bytecodes::send_type(code) != Bytecodes::no_send;
}


static void generate_gen_method(Bytecodes::Code code) {
  char* sig = Bytecodes::format_as_string(Bytecodes::format(code));
  std->print(Bytecodes::name(code));
  generate_signature(sig, '_');
  std->cr();
  generate_comment();
  std->print("\tself byte: 16r%02X", code);
  generate_signature(sig, ' ');
  std->print(".\n");
  if (has_instVar_access (code)) std->print("\tself has_instVar_access.\n");
  if (has_classVar_access(code)) std->print("\tself has_classVar_access.\n");
  if (has_inline_cache   (code)) std->print("\tself has_inline_cache.\n");
  std->print("!\n\n");
}


static void generate_float_function_constant_method(Floats::Function f) {
  std->print("float_%s\n", Floats::function_name_for(f));
  generate_comment();
  std->print("\t^ %d\n", f);
  std->print("!\n\n");
}


static void generate_HCode_methods() {
  std->print("!DeltaHCode methods !\n\n");

  generate_instr_method();
  generate_codeForPrimitive_method();

  int i;
  for (i = 0; i < Bytecodes::number_of_codes; i++) {
    Bytecodes::Code code = Bytecodes::Code(i);
    if (actually_generated(code)) generate_gen_method(code);
  }

  // assert(Floats::is_initialized(), "Floats must be initialized");
  for (i = 0; i < Floats::number_of_functions; i++) {
    Floats::Function f = Floats::Function(i);
    generate_float_function_constant_method(f);
  }

  std->print("!\n\n");
}


// HTML generation

class Markup: StackObj {
 private:
  char* _tag;
 public:
  Markup(char* tag)	{ _tag = tag; std->print("<%s>\n", _tag); }
  ~Markup()		{ std->print("</%s>\n", _tag); }
};


static void markup(char* tag, char* text) {
  std->print("<%s>%s</%s>\n", tag, text, tag);
}


static void print_format(Bytecodes::Format format) {
  char* f = Bytecodes::format_as_string(format);
  while (*f) {
    switch (*f) {
      case 'B': std->print(" byte");	break;
      case 'L': std->print(" long");	break;
      case 'O': std->print(" oop");	break;
      case 'S': std->print(" {byte}");	break;
      default : ShouldNotReachHere();	break;
    }
    f++;
  }
}


static char* arguments_as_string(Bytecodes::ArgumentSpec spec) {
  switch (spec) {
    case Bytecodes::recv_0_args: return "receiver";
    case Bytecodes::recv_1_args: return "receiver argument_1";
    case Bytecodes::recv_2_args: return "receiver argument_1 argument_2";
    case Bytecodes::recv_n_args: return "receiver argument_1 argument_2 ... argument_n";
    case Bytecodes::args_only  : return "argument_1 argument_2 ... argument_n";
    default                    : ShouldNotReachHere();
  }
  return NULL;
}


static void generate_HTML_for(Bytecodes::Code code) {
  std->print("<TD>%02X<SUB>H</SUB><TD><B>%s</B><TD>", int(code), Bytecodes::name(code));
  print_format(Bytecodes::format(code));
  std->print("<TD>%s", Bytecodes::single_step(code) ? "intercepted" : "");
  if (Bytecodes::code_type(code) == Bytecodes::message_send) {
    std->print("<TD>%s", Bytecodes::send_type_as_string(Bytecodes::send_type(code)));
    std->print("<TD>%s", arguments_as_string(Bytecodes::argument_spec(code)));
  }
  std->print("<TR>\n");
}


static void generate_HTML_for(Bytecodes::CodeType type) {
  { Markup tag("H3");
    std->print("%s bytecodes\n", Bytecodes::code_type_as_string(type));
  }
  { Markup tag("TABLE");
    std->print("<TH>Code<TH>Name<TH>Format<TH>Single step");
    if (type == Bytecodes::message_send) std->print("<TH>Send type<TH>Arguments");
    std->print("<TR>\n");
    for (int i = 0; i < Bytecodes::number_of_codes; i++) {
      Bytecodes::Code code = Bytecodes::Code(i);
      if (Bytecodes::is_defined(code) && Bytecodes::code_type(code) == type) generate_HTML_for(code);
    }
  }
  std->print("<HR>\n");
}


static void generate_HTML_docu() {
  Markup tag("HTML");
  std->print("<!-- do not modify - use delta +GenerateHTML to generate -->\n");
  { Markup tag("HEAD");
    markup("TITLE", "Delta Bytecodes");
  }
  { Markup tag("BODY");
    { Markup tag("H2");
      std->print("Delta Bytecodes (Version %d)\n", Bytecodes::version());
    }
    for (int i = 0; i < Bytecodes::number_of_code_types; i++) generate_HTML_for(Bytecodes::CodeType(i));
  }
}

#endif


void bytecodes_init() {
  Bytecodes::init();
#ifndef PRODUCT
  if (GenerateSmalltalk) {
    generate_HCode_methods();
    exit(0);
  }
  if (GenerateHTML) {
    generate_HTML_docu();
    exit(0);
  }
#endif
}
