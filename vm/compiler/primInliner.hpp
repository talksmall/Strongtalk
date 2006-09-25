/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.36 $ */
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

// the PrimInliner inlines primitives (if possible) or generates a non-inlined call
// there's one PrimInliner for each primitive call encountered
class PrimInliner: public PrintableResourceObj {
 private:
  NodeBuilder*		_gen;				// the active node generator
  int			_bci;				// bci of primitive call
  primitive_desc*	_pdesc;				// the primitive
  MethodInterval*	_failure_block;			// code in primitive failure block

  InlinedScope*		_scope;				// the current scope
  ExprStack*		_exprStack;			// the current expression stack
  GrowableArray<Expr*>*	_params;			// the copy of the top number_of_parameters() elements of _exprStack
							// NB: don't use _params->at(...) -- use parameter() below
  bool  _usingUncommonTrap;				// using uncommon trap for prim. failure?
  bool  _cannotFail;		    			// true if primitive can't fail
  int	number_of_parameters()	const			{ return _pdesc->number_of_parameters(); }
  Expr*	parameter(int index)	const 			{ return _params->at(index); }		// parameter of primitive call
  bool	is_power_of_2(int x) const			{ return x > 0 && (x & (x-1)) == 0; }	// true if there's an n with 2^n = x
  int	log2(int x) const;				// if is_power_of_2(x) then 2^(log2(x)) = x

  inline void assert_failure_block();			// debugging: asserts that there's a failure block
  inline void assert_no_failure_block();		// debugging: asserts that there's no failure block
  inline void assert_receiver();			// debugging: asserts that the first parameter is self

  Expr*	tryConstantFold();				// try constant-folding the primitive
  Expr*	tryTypeCheck();					// try constant-folding primitive failures
  Expr*	tryInline();					// try inlining or special-casing the primitive
  Expr* genCall(bool canFail);				// generate non-inlined primitive call
  Expr* primitiveFailure(symbolOop failureCode);	// handle primitive that always fail
  Expr* merge_failure_block(Node* ok_exit, Expr* ok_result, Node* failure_exit, Expr* failure_code, bool ok_result_is_read_only = true);
  symbolOop failureSymbolForArg(int i);			// error string for "n.th arg has wrong type"
  bool  shouldUseUncommonTrap();			// use uncommon trap for primitive failure?
  inline bool basic_shouldUseUncommonTrap() const;
  
  Expr* smi_ArithmeticOp(ArithOpCode  op  , Expr* x, Expr* y);
  Expr* smi_Comparison  (BranchOpCode cond, Expr* x, Expr* y);
  Expr* smi_BitOp       (ArithOpCode  op  , Expr* x, Expr* y);
  Expr* smi_Div         (Expr* x, Expr* y);
  Expr* smi_Mod         (Expr* x, Expr* y);
  Expr* smi_Shift       (Expr* x, Expr* y);

  Expr* array_size();
  Expr* array_at_ifFail(ArrayAtNode::AccessType access_type);
  Expr* array_at_put_ifFail(ArrayAtPutNode::AccessType access_type);

  Expr* obj_new();
  Expr* obj_shallowCopy();
  Expr* obj_equal();
  Expr* obj_class(bool has_receiver);
  Expr* obj_hash(bool has_receiver);
  
  Expr* proxy_byte_at();
  Expr* proxy_byte_at_put();

  Expr* block_primitiveValue();

 public:
  PrimInliner(NodeBuilder* gen, primitive_desc* pdesc, MethodInterval* failure_block);
  void generate();

  static Expr* generate_cond(BranchOpCode cond, NodeBuilder* gen, PReg* resPReg); 
      // generates cond. branch and code to assign true/false to resPReg

  void print();
};

# endif
