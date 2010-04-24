/* Copyright (c) 2010, Stephen Rees 
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
# include "incls/_missingMethodBuilder.cpp.incl"

void MissingMethodBuilder::build() {
  BlockScavenge bs;

  int argCount = selector->number_of_arguments();
  if (argCount > 0)
    buffer.pushByte(Bytecodes::allocate_temp_1);
  buffer.pushByte(Bytecodes::push_global);
  buffer.pushOop(Universe::find_global_association("Message"));
  buffer.pushByte(Bytecodes::push_self);
  buffer.pushByte(Bytecodes::push_literal);
  buffer.pushOop(selector);
  if (argCount == 0) {
    buffer.pushByte(Bytecodes::push_literal);
    buffer.pushOop(oopFactory::new_objArray(0));
  } else {
    buffer.pushByte(Bytecodes::push_global);
    buffer.pushOop(Universe::find_global_association("Array"));
    buffer.pushByte(Bytecodes::push_succ_n);
    buffer.pushByte(argCount - 1);
    buffer.pushByte(Bytecodes::interpreted_send_1);
    buffer.pushOop(oopFactory::new_symbol("new:"));
    buffer.pushOop(as_smiOop(0));
    buffer.pushByte(Bytecodes::store_temp_n);
    buffer.pushByte(0xFF);
    for (int index = 0; index < argCount; index++) {
      buffer.pushByte(Bytecodes::push_succ_n);
      buffer.pushByte(index);
      buffer.pushByte(Bytecodes::push_arg_n);
      buffer.pushByte(argCount - index - 1);
      buffer.pushByte(Bytecodes::interpreted_send_2_pop);
      buffer.pushOop(oopFactory::new_symbol("at:put:"));
      buffer.pushOop(as_smiOop(0));
      buffer.pushByte(Bytecodes::push_temp_0);
    }
  }
  buffer.pushByte(Bytecodes::interpreted_send_n);
  buffer.pushByte(3);
  buffer.pushOop(oopFactory::new_symbol("receiver:selector:arguments:"));
  buffer.pushOop(as_smiOop(0));
  buffer.pushByte(Bytecodes::interpreted_send_self);
  buffer.pushOop(oopFactory::new_symbol("doesNotUnderstand:"));
  buffer.pushOop(as_smiOop(0));
  switch (argCount) {
    case 0:
      buffer.pushByte(Bytecodes::return_tos_pop_0);
      break;
    case 1:
      buffer.pushByte(Bytecodes::return_tos_pop_1);
      break;
    case 2:
      buffer.pushByte(Bytecodes::return_tos_pop_2);
      break;
    default:
      buffer.pushByte(Bytecodes::return_tos_pop_n);
      buffer.pushByte(argCount);
  }
  methodKlass* k = (methodKlass*) Universe::methodKlassObj()->klass_part();
  _method = k->constructMethod(selector,
                               0,         // flags
                               argCount,  // number of arguments
                               oopFactory::new_objArray(0), // debug info
                               bytes(),
                               oops());
}

byteArrayOop MissingMethodBuilder::bytes() {
  return buffer.bytes();
}

objArrayOop MissingMethodBuilder::oops() {
  return buffer.oops();
}
