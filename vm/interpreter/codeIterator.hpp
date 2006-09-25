/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.32 $ */
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

// CodeIterator is a simple but fast iterator for
// scanning byte code instructions in a methodOop.

class CodeIterator: public StackObj {
 private:
  methodOop method;
  u_char*   current;
  u_char*   end;

  inline void    align();
  inline u_char* align(u_char* p) const			{ return (u_char*) (((int) p + 3) & (~3)); }

 public:
  // Constructor
  CodeIterator(methodOop method, int startBCI = 1);
  CodeIterator(u_char* hp);

  // Advance to next instruction
  // Returns false if we passed the end.
  bool advance() {
    current = next_hp();
    return current < end;
  }

  // accessors
  u_char	byte_at(int offset_from_instruction)	{ return current[offset_from_instruction]; }
  oop		oop_at(int offset_from_instruction)	{ return *aligned_oop(offset_from_instruction); }
  int		word_at(int offset_from_instruction)	{ return (int) *aligned_oop(offset_from_instruction); }

  Bytecodes::Code		code() const		{ return Bytecodes::Code(*current); }
  Bytecodes::CodeType		code_type() const	{ return Bytecodes::code_type(code()); }
  Bytecodes::Format		format() const		{ return Bytecodes::format(code()); }
  Bytecodes::SendType		send() const		{ return Bytecodes::send_type(code()); }
  Bytecodes::ArgumentSpec	argumentsType() const	{ return Bytecodes::argument_spec(code()); }
  bool				pop_result() const	{ return Bytecodes::pop_tos(code()); }
  Bytecodes::LoopType		loopType() const	{ return Bytecodes::loop_type(code()); }
  int				bci() const;
  int				next_bci() const;
  u_char*			hp() const		{ return current; }
  u_char*			next_hp() const;

  // FOR DEOPTIMIZATION
  // Returns the interpreter return point for the current byte code.
  char*	interpreter_return_point(bool restore_value = false) const;

  void set_bci(int bci); 

  // returns the location of an aligned oop 
  oop* aligned_oop(int offset_from_instruction) {
    return (oop*) align(current + offset_from_instruction);
  }

  bool is_message_send()   const { return Bytecodes::code_type(code()) == Bytecodes::message_send;   }
  bool is_primitive_call() const { return Bytecodes::code_type(code()) == Bytecodes::primitive_call; }
  bool is_dll_call()       const { return Bytecodes::code_type(code()) == Bytecodes::dll_call;       }

  // Returns the address of the block method if the current butecode is a push closure, NULL otherwise.
  oop* block_method_addr();

  // Returns the block method if the current butecode is a push closure, NULL otherwise.
  methodOop block_method();

  // Customization
  void customize_class_var_code(klassOop to_klass);
  void recustomize_class_var_code(klassOop from_klass, klassOop to_klass);
  void uncustomize_class_var_code(klassOop from_klass);

  void customize_inst_var_code(klassOop to_klass);
  void recustomize_inst_var_code(klassOop from_klass, klassOop to_klass);
  void uncustomize_inst_var_code(klassOop from_klass);

  // Returns the inline cache iff the current instruction is a send
  InterpretedIC*         ic();
  InterpretedPrim_Cache* prim_cache(); 
  InterpretedDLL_Cache*  dll_cache();

  // For byte code manipulation
  void set_code(u_char code) { *current = code; }
};
