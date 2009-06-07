/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.26 $ */
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


# ifdef DELTA_COMPILER

// The NodeBuilder generates the intermediate representation for a method or block.

class AssignNode;
class TypeTestNode;
class ContextInitNode;
class Inliner;

class NodeBuilder: public CustomizedMethodClosure {
 private:
  ExprStack*	_exprStack;		// current expression stack
  Inliner*	_inliner;		// my inliner
  InlinedScope*	_scope;			// scope for which this NodeBuilder is generating code
  Node*		_current;		// where new nodes are appended

  void append_exit(Node* exitNode);	// append an exit node (UncommonNode, Return, etc.)
  void append1(Node* node);
  void branch (MergeNode* target);
  void comment(char* s);
  GrowableArray<PReg*>* copyCurrentExprStack();
  void access_temporary(int no, int context, bool push);
  GrowableArray<PReg*>* pass_arguments(PReg* self, int nofArgs);
  void splitMergeExpr(Expr* expr, TypeTestNode* test);
  GrowableArray<Expr*>* splittablePaths(const Expr* expr, const TypeTestNode* test) const;
  GrowableArray<NonTrivialNode*>* nodesBetween(Node* from, Node* to);
  MergeNode* insertMergeBefore(Node* n);
  Expr* copy_into_context(Expr* e, int no);
  void materialize(PReg* r, GrowableArray<BlockPReg*>* materialized);	// materialize block (always call before storing/assigning PReg)

  bool abortIfDead(Expr* e);						// helper function for dead code handling
  void generate_subinterval(MethodInterval* m, bool produces_result);	// generate subinterval (e.g., code in then branch)
  void constant_if_node(IfNode* node, ConstantExpr* cond);		// code for if with const condition
  TypeTestNode* makeTestNode(bool cond, PReg* r);			// make boolean type test node

  // for Inliner
  void gen_normal_send(SendInfo* info, int nofArgs, SAPReg* result);
  void gen_self_send  (SendInfo* info, int nofArgs, SAPReg* result);
  void gen_super_send (SendInfo* info, int nofArgs, SAPReg* result);

  friend class Inliner;
  friend class CompilerInliningPolicy;
  friend class PrimInliner;
 protected:
  void abort();

 public:
  static Node*	EndOfCode;		// "at end of code" marker
  
  NodeBuilder();
  void initialize(InlinedScope* scope);

  InlinedScope* scope() const		{ return _scope; }
  ExprStack* exprStack() const		{ return _exprStack; }
  Node* current() const			{ return _current; }
  void setCurrent(Node* n)		{ assert(n != EndOfCode, "bad node"); _current = n; }
  void append (Node* node);		// append a node
  bool is_in_dead_code() const		{ return _current == EndOfCode; }
  Inliner* inliner() const		{ return _inliner; }
  void removeContextCreation();		// remove context creation node 
  PReg* float_at(int fno);

 public:
  void if_node(IfNode* node);
  void cond_node(CondNode* node);
  void while_node(WhileNode* node);
  void primitive_call_node(PrimitiveCallNode* node);
  void dll_call_node(DLLCallNode* node);

 public:
  void allocate_temporaries(int nofTemps);

  void push_self();
  void push_tos();
  void push_literal(oop obj);

  void push_argument(int no);
  void push_temporary(int no);
  void push_temporary(int no, int context);
  void push_instVar(int offset);
  void push_global(associationOop obj);

  void store_temporary(int no);
  void store_temporary(int no, int context);
  void store_instVar(int offset);
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
  void float_floatify(Floats::Function f, int fno);
  void float_move(int fno, int from);
  void float_set(int fno, doubleOop value);
  void float_nullary(Floats::Function f, int fno);
  void float_unary(Floats::Function f, int fno);
  void float_binary(Floats::Function f, int fno);
  void float_unaryToOop(Floats::Function f, int fno);
  void float_binaryToOop(Floats::Function f, int fno);
};


# endif
