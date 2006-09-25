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
# include "incls/_interpreterStatistics.cpp.incl"


bool InterpreterStatistics::_is_initialized = false;
unsigned int InterpreterStatistics::_bytecode_counters[Bytecodes::number_of_codes];
int InterpreterStatistics::_bytecode_generation_order[Bytecodes::number_of_codes];


void InterpreterStatistics::reset_bytecode_counters() {
  for (int i = 0; i < Bytecodes::number_of_codes; i++) _bytecode_counters[i] = 0;
}


void InterpreterStatistics::reset_bytecode_generation_order() {
  for (int i = 0; i < Bytecodes::number_of_codes; i++) _bytecode_generation_order[i] = i;
}


Bytecodes::Code InterpreterStatistics::ith_bytecode_to_generate(int i) {
  assert(0 <= i && i < Bytecodes::number_of_codes, "illegal index");
  return Bytecodes::Code(_bytecode_generation_order[i]);
}


void InterpreterStatistics::initialize() {
  if (is_initialized()) return;
  reset_bytecode_counters();
  reset_bytecode_generation_order();
  _is_initialized = true;
}