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

# ifdef DELTA_COMPILER

// Exprs denote the results of expressions in the compiler.  Their main
// purpose is to annotate a PReg with type information.  
// Another way of looking at Exprs is that they represent values in the compiler,
// whereas PRegs denote locations (names).  Thus, many Exprs may point to the
// same PReg.

# define BASIC_FLAG_DEF(name, prot)					      \
 protected:								      \
    static const int CONC(name,Bit);					      \
 prot	    								      \
  bool CONC(is,name)() const { return flags & CONC(name,Bit); }		      \
  void CONC(set,name)(bool b) {						      \
    if (b) flags |= CONC(name,Bit); else flags &= ~CONC(name,Bit); }	      \

# define FLAG_DEF(name)	      BASIC_FLAG_DEF(name, public:)
# define PFLAG_DEF(name)      BASIC_FLAG_DEF(name, protected:)

class Expr : public PrintableResourceObj {	// abstract base class
 protected:
  PReg* _preg;				// PReg holding it
  Node* _node;				// defining node or NULL if unknown
  InlinedScope* unlikelyScope;		// scope/bci making unknown unlikely
  int unlikelyBCI;			// (only set if isUnknownUnlikely())

 public:
  Expr* next;    	    		// used for splittable MergeExprs
  int flags;
  
  Expr(PReg* p, Node* n);
  
  virtual bool isUnknownExpr() const 		{ return false; }
  virtual bool isNoResultExpr() const 		{ return false; }
  virtual bool isKlassExpr() const 		{ return false; }
  virtual bool isBlockExpr() const		{ return false; }
  virtual bool isConstantExpr() const 		{ return false; }
  virtual bool isMergeExpr() const		{ return false; }
  virtual bool isContextExpr() const		{ return false; }
  
  virtual bool hasKlass() const			{ return false; }
  virtual int  nklasses() const			= 0;	// number of klasses contained in expr
  virtual bool really_hasKlass(InlinedScope* s) const { Unused(s);  return hasKlass(); }
  virtual klassOop klass() const		{ ShouldNotCallThis(); return 0; }
  virtual bool hasConstant() const		{ return false; }
  virtual oop constant() const			{ ShouldNotCallThis(); return 0; }
  virtual bool containsUnknown()		= 0;
  virtual Expr* makeUnknownUnlikely(InlinedScope* s) {
    Unused(s); ShouldNotCallThis(); return 0;}
  virtual bool isUnknownUnlikely() const	{ return false; }
  virtual bool needsStoreCheck() const		{ return true; }
  
  virtual UnknownExpr* findUnknown() const 	{ return NULL; }
  virtual Expr* findKlass(klassOop map) const	{ Unused(map);  return NULL; }
  
  virtual Expr* asReceiver() const;
  virtual KlassExpr* asKlassExpr() const 	{ ShouldNotCallThis(); return NULL; }
  virtual ConstantExpr* asConstantExpr() const	{ ShouldNotCallThis(); return NULL; }
  virtual MergeExpr* asMergeExpr() const 	{ ShouldNotCallThis(); return NULL; }
  
  virtual Expr* shallowCopy(PReg* p, Node* n) const 	= 0;  // return a shallow copy
  virtual Expr* copyWithout(Expr* e) const 		= 0;  // return receiver w/o expr case
  virtual Expr* mergeWith(Expr* other, Node* n) 	= 0;  // return receiver merged with other
  virtual Expr* convertToKlass(PReg* p, Node* n) const 	= 0;  // convert constants to klasses
  virtual bool equals(Expr* other) const 		= 0;
  
  Node*   node() const    	    	    	{ return _node; }
  PReg*   preg() const     	            	{ return _preg; }
  void    setNode(Node* n, PReg* p) 		{ _node = n; _preg = p; }
  bool    is_smi() const			{ return hasKlass() && klass() == smiKlassObj; }

  InlinedScope* scope() const;
  virtual NameNode* nameNode(bool mustBeLegal = true) const;
  virtual void verify() const;

 protected:
  void print_helper(char* type);
};

// an expression whose type is unknown
class UnknownExpr : public Expr {
 public:
  UnknownExpr(PReg* p, Node* n = NULL, bool u = false) : Expr(p, n) {
    setUnlikely(u); }
  bool isUnknownExpr() const 		{ return true; }
  bool containsUnknown()		{ return true; }
  FLAG_DEF(Unlikely);    		// true e.g. if this is the "unknown" branch of a
    	    	    			// type-predicted receiver, or result of prim. failure
  UnknownExpr* findUnknown() const 	{ return (UnknownExpr*)this; }
  bool isUnknownUnlikely() const	{ return isUnlikely(); }
  int  nklasses() const			{ return 0; }  
  Expr* shallowCopy(PReg* p, Node* n) const;
  Expr* copyWithout(Expr* e) const	{ return (Expr*)this; }
  Expr* mergeWith(Expr* other, Node* n);
  Expr* convertToKlass(PReg* p, Node* n) const { return shallowCopy(p, n); };
  Expr* makeUnknownUnlikely(InlinedScope* s);
  bool equals(Expr* other) const;
  void print();
};

// an expression that has no value, i.e., will never exist at runtime
// example: the return value of a block method that ends with a non-local return
// used mainly for compiler debugging and to avoid generating unreachable code
class NoResultExpr : public Expr {
 public:
  NoResultExpr(Node* n = NULL);
  bool isNoResultExpr() const 		{ return true; }
  int  nklasses() const			{ return 0; }  
  bool containsUnknown() 		{ return false; }
  Expr* shallowCopy(PReg* p, Node* n) const;
  Expr* copyWithout(Expr* e) const	{ return (Expr*)this; }
  Expr* mergeWith(Expr* other, Node* n);
  Expr* convertToKlass(PReg* p, Node* n) const 	{ return shallowCopy(p, n); };
  bool equals(Expr* other) const;
  void print();
};


// an expression whose klass is known
class KlassExpr : public Expr {
 protected:
  klassOop _klass;
 public:
  KlassExpr(klassOop m, PReg* p, Node* n);
  
  bool isKlassExpr() const 		{ return true; }
  bool containsUnknown()		{ return false; }
  
  KlassExpr* asKlassExpr() const	{ return (KlassExpr*)this; }
  bool hasKlass() const			{ return true; }
  int  nklasses() const			{ return 1; }  
  klassOop klass() const		{ return _klass; }
  virtual bool needsStoreCheck() const;
  Expr* shallowCopy(PReg* p, Node* n) const;
  Expr* copyWithout(Expr* e) const;
  Expr* mergeWith(Expr* other, Node* n);
  Expr* convertToKlass(PReg* p, Node* n) const 	{ return shallowCopy(p, n); };
  Expr* findKlass(klassOop map) const	{ return _klass == map ? (Expr*)this : NULL; }
  bool equals(Expr* other) const;
  void print();
  virtual void verify() const;
};

// a cloned block literal (result of BlockClone node)
class BlockExpr : public KlassExpr {
 protected:
  InlinedScope* _blockScope;   		// block's parent scope
 public:
  BlockExpr(BlockPReg* p, Node* n);
  bool isBlockExpr() const		{ return true; }
  InlinedScope* blockScope() const 	{ return _blockScope; }
  BlockPReg* preg() const		{ return (BlockPReg*)_preg; }
  int  nklasses() const			{ return 1; }  
  Expr* shallowCopy(PReg* p, Node* n) const;
  Expr* mergeWith(Expr* other, Node* n);
  bool equals(Expr* other) const;
  void print();
  virtual void verify() const;
};

// an expression whose exact runtime value is known
class ConstantExpr : public Expr {
  oop _c;
 public:
  ConstantExpr(oop c, PReg* p, Node* n) : Expr(p, n) { _c = c; }
  
  bool isConstantExpr() const 			{ return true; }
  bool containsUnknown()			{ return false; }
  bool hasKlass() const				{ return true; }
  KlassExpr* asKlassExpr() const;
  klassOop klass() const			{ return _c->klass(); }
  int  nklasses() const				{ return 1; }  
  ConstantExpr* asConstantExpr() const		{ return (ConstantExpr*)this; }
  bool hasConstant() const			{ return true; }
  oop constant() const				{ return _c; }
  virtual bool needsStoreCheck() const;
  NameNode* nameNode(bool mustBeLegal = true) const;
  Expr* shallowCopy(PReg* p, Node* n) const;
  Expr* copyWithout(Expr* e) const;
  Expr* mergeWith(Expr* other, Node* n);
  Expr* convertToKlass(PReg* p, Node* n) const;
  Expr* findKlass(klassOop map) const;
  bool equals(Expr* other) const;
  void print();
  virtual void verify() const;
};


// an expression that is the result of merging two or more other expressions
// when control flow merges
// example: the result of foo ifTrue: [ 1 ] ifFalse: [ i + j ] could be
// MergeExpr(ConstantExpr(1), KlassExpr(IntegerKlass))
// merge expressions have the following properties:
// - they never have more than MaxPICSize+1 elements
// - they're flat (i.e don't contain other MergeExprs)

class MergeExpr : public Expr {
 public:
  GrowableArray<Expr*>* exprs;
  
  MergeExpr(Expr* e1, Expr* e2, PReg* p, Node* n);
  MergeExpr(PReg* p, Node* n);
  	// A MergeExpr's PReg says where the merged result is (i.e., it may be different from
	// the PRegs of the individual expressions); typically, there's an assigmnent from the
	// subexpression's PReg to the MergeExpr's PReg just before the control flow merge.
	// The node n (if non-NULL) is considered the defining node of the MergeExpr; it is usually
	// one of the first nodes after the control flow merge.  It is always legal to pass
	// NULL for n, but doing so prevents splitting.
  
  bool isMergeExpr() const 		{ return true; }
  FLAG_DEF(Splittable);
  PFLAG_DEF(UnknownSet);
  PFLAG_DEF(ContainingUnknown);
 public:
  bool containsUnknown();
  MergeExpr* asMergeExpr() const	{ return (MergeExpr*)this; }
  bool hasKlass() const;
  int  nklasses() const;  
  KlassExpr* asKlassExpr() const;
  bool really_hasKlass(InlinedScope* s) const;
  klassOop klass() const;
  Expr* asReceiver() const;
  bool hasConstant() const;
  oop constant() const;
  Expr* shallowCopy(PReg* p, Node* n) const;
  Expr* copyWithout(Expr* e) const;
  Expr* mergeWith(Expr* other, Node* n);
  Expr* convertToKlass(PReg* p, Node* n) const;
  Expr* makeUnknownUnlikely(InlinedScope* s);
  bool equals(Expr* other) const;
  void print();
  virtual void verify() const;
  
  UnknownExpr* findUnknown() const;    // returns NULL if no expression found
  Expr* findKlass(klassOop map) const;
  
protected:
  void initialize();
  void mergeInto(Expr* other, Node* n);
  void add(Expr* s);
};


// an expression for a context pointer
// used only for compiler debugging; it should never appear where a normal expression is expected
class ContextExpr : public Expr {
 public:
  	ContextExpr(PReg* r);
  bool	isContextExpr() const			{ return true; }
  bool	containsUnknown()			{ ShouldNotCallThis(); return false; }
  int   nklasses() const			{ ShouldNotCallThis(); return 1; }  
  Expr*	shallowCopy(PReg* p, Node* n) const	{ ShouldNotCallThis(); return (Expr*)this; }
  Expr* copyWithout(Expr* e) const		{ ShouldNotCallThis(); return (Expr*)this; }
  Expr*	mergeWith(Expr* other, Node* n) 	{ ShouldNotCallThis(); return (Expr*)this; }
  Expr*	convertToKlass(PReg* p, Node* n) const	{ return shallowCopy(p, n); };
  bool	equals(Expr* other) const		{ ShouldNotCallThis(); return false; }
  void	print();
  virtual void verify() const;
};


class ExprStack : public GrowableArray<Expr*> {
  // an ExprStack simulates the run-time expression stack during compilation
  // it keeps track of the live ranges by recording the bci on push/pop
 private:
  InlinedScope* _scope;				// scope that generates the pushes and pops
 public:
  ExprStack(InlinedScope* scope, int size);
  void	push(Expr* expr, InlinedScope* currentScope, int bci);
  void  push2nd(Expr* expr, InlinedScope* currentScope, int bci); // allows a 2nd expr to be pushed for the same bci
  void	assign_top(Expr* expr);
  Expr*	pop();
  void	pop(int nofExprsToPop);
  void  print();
};

# undef BASIC_FLAG_DEF
# undef FLAG_DEF
# undef PFLAG_DEF

# endif
