/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.2 $ */
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
# ifdef DELTA_COMPILER
# include "incls/_costModel.cpp.incl"


int CostModel::_cost[Bytecodes::number_of_codes];


void CostModel::set_default_costs() {
  // default cost for most codes
  set_cost_for_all(1);

  // specialize for individual code types
  set_cost_for_type(Bytecodes::new_closure,     3);
  set_cost_for_type(Bytecodes::new_context,     3);
  set_cost_for_type(Bytecodes::message_send,    5);
  set_cost_for_type(Bytecodes::nonlocal_return, 3);
  set_cost_for_type(Bytecodes::primitive_call,  5);
  set_cost_for_type(Bytecodes::dll_call,        5);

  // specialize for individual send types
  set_cost_for_send(Bytecodes::predicted_send,  2);
  set_cost_for_send(Bytecodes::accessor_send,   2);
}


void CostModel::set_cost_for_all(int cost) {
  for (int i = 0; i < Bytecodes::number_of_codes; i++) {
    _cost[i] = cost;
  }
}


void CostModel::set_cost_for_code(Bytecodes::Code code, int cost) {
  assert(Bytecodes::is_defined(code), "undefined bytecode");
  _cost[code] = cost;
}


void CostModel::set_cost_for_type(Bytecodes::CodeType type, int cost) {
  for (int i = 0; i < Bytecodes::number_of_codes; i++) {
    if (Bytecodes::code_type(Bytecodes::Code(i)) == type) {
      _cost[i] = cost;
    }
  }
}


void CostModel::set_cost_for_send(Bytecodes::SendType type, int cost) {
  for (int i = 0; i < Bytecodes::number_of_codes; i++) {
    if (Bytecodes::send_type(Bytecodes::Code(i)) == type) {
      _cost[i] = cost;
    }
  }
}

  
void CostModel::print() {
  std->print("%40s: %s\n", "Bytecode", "Cost");
  for (int i = 0; i < Bytecodes::number_of_codes; i++) {
    Bytecodes::Code code = Bytecodes::Code(i);
    if (Bytecodes::is_defined(code)) {
      std->print("%40s: %d\n", Bytecodes::name(code), cost_for(code));
    }
  }
}


void costModel_init() {
  CostModel::set_default_costs();
}


# endif