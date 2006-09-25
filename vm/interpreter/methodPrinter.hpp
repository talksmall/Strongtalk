/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.42 $ */
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

// MethodPrinter dumps, on the console, all byte-codes of a method.
// In addition blocks contained in the method are dumped at the
// byte code pushing the block context.
// Usage:
//   MethodPrinterClosure blk(output);
//   MethodIterator mi(method, &blk);

class MethodPrinterClosure: public MethodClosure {
 private:
   outputStream*  st;
   void MethodPrinterClosure::print_sendtype(Bytecodes::SendType type);
   void show(char* str);
   void indent();
 public:
  MethodPrinterClosure(outputStream* stream = NULL);
  void if_node(IfNode* node);
  void cond_node(CondNode* node);
  void while_node(WhileNode* node);
  void primitive_call_node(PrimitiveCallNode* node);
  void dll_call_node(DLLCallNode* node);

  void allocate_temporaries(int nofTemps);
  void push_self();
  void push_tos();
  void push_literal(oop obj);
  void push_argument(int no);
  void push_temporary(int no);
  void push_temporary(int no, int context);
  void push_instVar(int offset);
  void push_instVar_name(symbolOop name);
  void push_classVar(associationOop assoc);
  void push_classVar_name(symbolOop name);

  void push_global(associationOop obj);
  void store_temporary(int no);
  void store_temporary(int no, int context);
  void store_instVar(int offset);
  void store_instVar_name(symbolOop name);
  void store_classVar(associationOop assoc);
  void store_classVar_name(symbolOop name);

  void store_global(associationOop obj);
  void pop();
  void normal_send(InterpretedIC* ic);
  void self_send  (InterpretedIC* ic);
  void super_send (InterpretedIC* ic);
  void double_equal();
  void double_not_equal();
  void method_return(int nofArgs);
  void nonlocal_return(int nofArgs);
  void allocate_closure(AllocationType type, int nofArgs, methodOop meth);
  void allocate_context(int nofTemps, bool forMethod);
  void set_self_via_context();
  void copy_self_into_context();
  void copy_argument_into_context(int argNo, int no);
  void zap_scope();
  void predict_prim_call(primitive_desc* pdesc, int failure_start);
  void float_allocate(int nofFloatTemps, int nofFloatExprs);
  void float_floatify(Floats::Function f, int tof);
  void float_move(int tof, int from);
  void float_set(int tof, doubleOop value);
  void float_nullary(Floats::Function f, int tof);
  void float_unary(Floats::Function f, int tof);
  void float_binary(Floats::Function f, int tof);
  void float_unaryToOop(Floats::Function f, int tof);
  void float_binaryToOop(Floats::Function f, int tof);
};
