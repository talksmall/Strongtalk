/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.75 $ */
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

// Scopes represent the source-level scopes compiled by the compiler.
// Compilation starts with a method or a block. All subsequently in-
// lined methods/blocks are described via InlinedScopes. If and only
// if compilation starts with a block, there are parent scopes described
// via OutlinedScopes. OutlinedScopes represent scopes outside the current
// compilation; i.e., at runtime, OutlinedScopes will be in different frames
// than all InlinedScopes (which are part of the same frame).  Two OutlinedScopes
// may or may not be in different frames depending on previous inlining (a real stack
// frame may consist of several virtual frames / OutlinedScopes).  Thus the scopes
// up to the OutlinedScope representing the method enclosing the block method
// may be in one or several stack frames.

class Scope;
class   InlinedScope;
class     MethodScope;
class     BlockScope;
class   OutlinedScope;
class     OutlinedMethodScope;
class     OutlinedBlockScope;


// SendInfo holds various data about a send before/while it is being
// inlined

class SendInfo: public PrintableResourceObj {
 public:
  InlinedScope* senderScope;
  Expr* rcvr;
  LookupKey* key;
  symbolOop sel;
  PReg* resReg;			// register where result should end up
  bool needRealSend;		// need a real (non-inlined) send
  bool counting;		// count # sends? (for non-inlined send)
  int nsends;			// estimated # of invocations (< 0 == unknown)
  bool predicted;		// was receiver type-predicted?
  bool uninlinable;		// was send considered uninlinable?
  bool receiverStatic;		// receiver type is statically known
  bool inPrimFailure;		// sent from within prim. failure block

 protected:
  void init();

 public:
  SendInfo(InlinedScope* sen, Expr* r, symbolOop s) {
    senderScope = sen; rcvr = r; sel = s; key = NULL;
    init();
  }

  SendInfo(InlinedScope* sen, LookupKey* lkup, Expr* r);
  void computeNSends(RScope* rscope, int bci);
   
  void print();
};


// Scope definitions

class Scope : public PrintableResourceObj {
 private:
  static smi	_currentScopeID;			// for scope descs

 public:
  // scopes are numbered starting at 0
  static void	initialize()				{ _currentScopeID = 0; }
  smi		currentScopeID()			{ return _currentScopeID++; }
  virtual smi	scopeID() const				= 0;
    
  // test functions
  virtual bool	isInlinedScope() const			{ return false; }
  virtual bool	isMethodScope() const			{ return false; }
  virtual bool	isBlockScope() const			{ return false; }
  virtual bool	isOutlinedScope() const			{ return false; }
  virtual bool	isOutlinedMethodScope() const   	{ return false; }   
  virtual bool	isOutlinedBlockScope() const 		{ return false; }

  virtual klassOop	selfKlass() const		= 0;
  virtual symbolOop	selector() const		= 0;
  virtual methodOop	method() const			= 0;
  virtual Scope*	parent() const			= 0;	// lexically enclosing scope or NULL (if MethodScope)
  virtual InlinedScope* sender() const			= 0;	// caller scope
  virtual klassOop	methodHolder() const		= 0;	// for super sends

  virtual bool		allocatesInterpretedContext() const = 0;// true if the scope allocates its own context in the interpreter
  virtual bool		allocatesCompiledContext() const = 0;	// true if the scope allocates a context in the compiled code
  virtual bool		expectsContext() const		= 0;	// true if the scope has an incoming context in the interpreter
  bool			needsContextZapping() const	{ return (parent() == NULL) && allocatesCompiledContext(); }
  
  virtual Scope*	home() const			= 0;	// the home scope
  bool			isTop() const 			{ return sender() == NULL; }
  bool			isInlined() const 		{ return sender() != NULL; }
  virtual bool		isSenderOf(InlinedScope* s) const { return false; } // isSenderOf = this is a proper caller of s 
  bool			isSenderOrSame(InlinedScope* s)	{ return (Scope*)s == this || isSenderOf(s); }
    
  virtual bool isRecursiveCall(methodOop method, klassOop rcvrKlass, int n) = 0;
  virtual void		genCode()			{ ShouldNotCallThis(); }
};


class InlinedScope: public Scope {
 protected:
  int		_scopeID;
  InlinedScope* _sender;				// NULL for top scope
  int		_senderBCI;				// call position in sender (if inlined)
  ScopeInfo	_scopeInfo;				// for debugging information (see scopeDescRecoder.hpp)
  LookupKey*	_key;
  klassOop	_methodHolder;				// != receiver klass only for methods invoked by super sends
  methodOop	_method;
  int		_nofSends;				// no. of non-inlined sends, cumulative (incl. subScopes)
  int		_nofInterruptPoints;			// no. of interrupt points, cumulative (incl. subScopes)
							// (_nofInterruptPoints == 0 => needs no debug info)
  bool		_primFailure;				// true if in a primitive call failure branch
  bool		_endsDead;				// true if method ends with dead code
  Expr*		_self;					// the receiver
  NodeBuilder	_gen;					// the generator of the intermediate representation

  PReg* 			_context;		// context (either passed in or created here), if any
  GrowableArray<Expr*>*		_arguments;		// the arguments
  GrowableArray<Expr*>*		_temporaries;		// the (originally) stack-allocated temporaries
  GrowableArray<Expr*>*		_floatTemporaries;	// the (originally) stack-allocated float temporaries
  GrowableArray<Expr*>*		_contextTemporaries;	// the (originally) heap-allocated temporaries
  GrowableArray<Expr*>*		_exprStackElems;	// the expression stack elems for debugging (indexed by bci)
  GrowableArray<InlinedScope*>*	_subScopes;		// the inlined scopes
  GrowableArray<CompiledLoop*>*	_loops;			// loops contained in this scope
  GrowableArray<NonTrivialNode*>* _typeTests;		// type test-like nodes contained in this scope

  GrowableArray<PReg*>*		_pregsBegSorted;	// the scope's PRegs sorted with begBCI (used for regAlloc)
  GrowableArray<PReg*>*		_pregsEndSorted;	// the scope's PRegs sorted with endBCI (used for regAlloc)

  // float temporaries
  int				_firstFloatIndex;	// the (stack) float temporary index for the first float

  // for node builders
  MergeNode*			_returnPoint;		// starting point for shared return code
  MergeNode*			_NLReturnPoint;		// starting point for shared non-local return code
  MergeNode*			_nlrTestPoint;		// where NLRs coming from callees will jump to (or NULL)
  ContextInitNode*		_contextInitializer;	// node initializing context (if any)
  bool				_hasBeenGenerated;	// true iff genCode() was called
  
 public:
  // for node builders
  MergeNode*			returnPoint()		{ return _returnPoint; }
  MergeNode*			nlrPoint()		{ return _NLReturnPoint; }
  MergeNode*			nlrTestPoint();		// returns a lazily generated nlrTestPoint
  ContextInitNode*		contextInitializer()	{ return _contextInitializer; }
  bool				has_nlrTestPoint()	{ return _nlrTestPoint != NULL; }
  void				set_contextInitializer(ContextInitNode* n) { _contextInitializer = n; }

 public:
  RScope*	rscope; 	    			// equiv. scope in recompilee (if any) - used for type feedback
  bool		predicted;				// was receiver type-predicted?
  int		depth;   	    			// call nesting level (top = 0)
  int		loopDepth;  	    			// loop nesting level (top = 0)
  Expr*		result;  	    			// result of normal return (NULL if none)
  Expr*		nlrResult;	    			// NLR result (non-NULL only for blocks)
  PReg*		resultPR;				// pseudo register containing result

protected:
  InlinedScope();
  void initialize(methodOop method, klassOop methodHolder, InlinedScope* sender, RScope* rs, SendInfo* info);

public:
  int		scopeID() const 			{ return _scopeID; }
  InlinedScope*	sender() const 				{ return _sender; }
  int		senderBCI() const     			{ return _senderBCI; }
  ScopeInfo	scopeInfo() const			{ return _scopeInfo; }
  virtual int	bci() const 				{ return gen()->bci(); }
  bool		isInlinedScope() const			{ return true; }
  int		nofArguments() const			{ return _arguments->length(); }
  bool		hasTemporaries() const			{ return _temporaries != NULL; }
  int		nofTemporaries() const			{ return _temporaries->length(); }
  bool		hasFloatTemporaries() const		{ return _floatTemporaries != NULL; }
  int		nofFloatTemporaries() const		{ return _floatTemporaries->length(); }
  int		firstFloatIndex() const			{ assert(_firstFloatIndex >= 0, "not yet computed"); return _firstFloatIndex; }
  bool		allocatesInterpretedContext() const	{ return _method->allocatesInterpretedContext(); }
  bool		allocatesCompiledContext() const;
  bool		expectsContext() const			{ return _method->expectsContext(); }
  bool 		isSenderOf(InlinedScope* s) const;

  GrowableArray<Expr*>* contextTemporaries() const	{ return _contextTemporaries; }
  int		nofBytes() const			{ return _method->end_bci() - 1; }
  int		nofSends() const			{ return _nofSends; }
  bool		containsNLR()  const 			{ return _method->containsNLR(); }
  bool		primFailure() const			{ return _primFailure; }
  Expr*		self() const				{ return _self; }
  void 		set_self(Expr* e);
  NodeBuilder*	gen() const				{ return (NodeBuilder*const)&_gen; }

  GrowableArray<Expr*>*	exprStackElems() const		{ return _exprStackElems; }	
  void 		addSubScope(InlinedScope* s);

  klassOop	selfKlass() const			{ return _key->klass(); }
  LookupKey* 	key() const				{ return _key; }
  klassOop	methodHolder() const			{ return _methodHolder; }
  methodOop	method() const				{ return _method; }
  symbolOop	selector() const			{ return _key->selector(); }
  bool		isLite() const;
  Expr*		argument(int no) const			{ return _arguments->at(no); }
  Expr*		temporary(int no) const			{ return _temporaries->at(no); }
  Expr*		floatTemporary(int no) const		{ return _floatTemporaries->at(no); }
  void		set_temporary(int no, Expr* t) 		{ _temporaries->at_put(no, t); }
  Expr*		contextTemporary(int no) const		{ return _contextTemporaries->at(no); }
  PReg*		context() const				{ return _context; }
  void		setContext(PReg* ctx)			{ _context = ctx; }
  ExprStack* 	exprStack() const			{ return gen()->exprStack(); }
  Node* 	current() const				{ return gen()->current(); }
  virtual bool  is_self_initialized() const		{ return true; }
  virtual void  set_self_initialized() 			{ ShouldNotCallThis(); }
  bool		hasBeenGenerated() const		{ return _hasBeenGenerated; }

  void 		createTemporaries(int nofTemps);
  void		createFloatTemporaries(int nofFloats);
  void 		createContextTemporaries(int nofTemps);
  void 		contextTemporariesAtPut(int no, Expr* e);
  int  		homeContext() const;			// the home context level
  InlinedScope* find_scope(int context, int& nofIndirections, OutlinedScope*& out);	// find enclosing scope
  void		addResult(Expr* e);
    
  void 		genCode();
  void 		addSend(GrowableArray<PReg*>* exprStack, bool isSend);
  GrowableArray<NonTrivialNode*>* typeTests() const	{ return _typeTests; }
  GrowableArray<CompiledLoop*>*	loops() const		{ return _loops; }
  void		addTypeTest(NonTrivialNode* t);
  CompiledLoop*	addLoop();

  void 		setExprForBCI(int bci, Expr* expr);
  void		set2ndExprForBCI(int bci, Expr* expr);
   
  virtual void 	collectContextInfo(GrowableArray<InlinedScope*>* scopeList);
  virtual void 	generateDebugInfo();
          void 	generateDebugInfoForNonInlinedBlocks();
  void		optimizeLoops();
  void		subScopesDo(Closure<InlinedScope*>* c);		// apply f to receiver and all subscopes
    
 protected:
  void		initializeArguments();
  virtual void	initializeSelf() {}
  virtual void 	prologue();
  virtual void 	epilogue();

 public:
  int		descOffset();

 protected:
  // int calleeSize(RScope* rs);
  void 		markLocalsDebugVisible(GrowableArray<PReg*>* exprStack);

 public:
  // for global register allocation
  void		addToPRegsBegSorted(PReg* r);
  void		addToPRegsEndSorted(PReg* r);
  void		allocatePRegs(IntFreeList* f);
  int		allocateFloatTemporaries(int firstFloatIndex);	// returns the number of float temps allocated for
  								// this and all subscopes; sets _firstFloatIndex
 public:
  void print();
  void printTree();

  // see Compiler::number_of_noninlined_blocks
  int  number_of_noninlined_blocks();
  // see Compiler::copy_noninlined_block_info
  void copy_noninlined_block_info(nmethod* nm);
    
  friend class OutlinedScope;
};
  

class MethodScope: public InlinedScope {	 // ordinary methods
 protected:
  MethodScope();
  void initialize(methodOop method, klassOop methodHolder, InlinedScope* sen, RScope* rs, SendInfo* info);
 public:
  static MethodScope* new_MethodScope(methodOop method, klassOop methodHolder, InlinedScope* sen, RScope* rs, SendInfo* info);

  bool		isMethodScope() const 		{ return true; }
  Scope*	parent() const			{ return NULL; }
  Scope*	home() const			{ return (Scope*)this; }

  bool		isRecursiveCall(methodOop method, klassOop rcvrKlass, int n);

  void		generateDebugInfo();

// debugging
  void		print_short();
  void		print();
};


class BlockScope: public InlinedScope {		// block methods
 protected:
  Scope* _parent;				// lexically enclosing scope
  bool	_self_is_initialized;			// true if self has been loaded
  void initialize(methodOop method, klassOop methodHolder, Scope* p, InlinedScope* s, RScope* rs, SendInfo* info);
  void initializeSelf();
  BlockScope();

 public:
  static BlockScope* new_BlockScope(methodOop method, klassOop methodHolder, Scope* p, InlinedScope* s, RScope* rs, 
	     			    SendInfo* info);
    
  bool		isBlockScope() const		{ return true; }
  bool  is_self_initialized() const		{ return _self_is_initialized; }
  void  set_self_initialized() 			{ _self_is_initialized = true; }
    
  Scope*	parent() const			{ return _parent; }
  Scope*	home() const			{ return _parent->home(); }
  klassOop	selfKlass() const		{ return _parent->selfKlass(); }
  bool		isRecursiveCall(methodOop method, klassOop rcvrKlass, int n);

  void		generateDebugInfo();
    
// debugging
  void		print_short();
  void		print();
};
  

class OutlinedScope: public Scope {		// abstract; a scope outside of the current compilation
 protected:
  nmethod*	_nm;				// nmethod containing this scope
  ScopeDesc*	_scope;

 public:
  OutlinedScope(nmethod* nm, ScopeDesc* scope);
  bool		isOutlinedScope() const		{ return true; }
  symbolOop	selector() const		{ return _scope->selector(); }
  Expr*		receiverExpr(PReg* p) const;
  methodOop	method() const;
  nmethod*	nm() const			{ return _nm; }
  InlinedScope*	sender() const			{ return NULL; }
  ScopeDesc*	scope() const			{ return _scope; }
  bool		allocatesInterpretedContext() const { return method()->allocatesInterpretedContext(); }
  bool		allocatesCompiledContext() const { return _scope->allocates_compiled_context(); }
  bool		expectsContext() const		{ return method()->expectsContext(); }
  
  // debugging
  void		print_short(char* name);
  void		print(char* name);
};

OutlinedScope* new_OutlinedScope(nmethod* nm, ScopeDesc* sc);


class OutlinedMethodScope: public OutlinedScope {
 public:
  OutlinedMethodScope(nmethod* nm, ScopeDesc* s) : OutlinedScope(nm, s) {}
    
  bool isOutlinedMethodScope() const		{ return true; }
  bool isRecursiveCall(methodOop method, klassOop rcvrKlass, int n) { ShouldNotCallThis(); return false; }
  klassOop	selfKlass() const		{ return _scope->selfKlass(); }
  smi		scopeID() const			{ return _scope->scopeID(); }
  Scope*	parent() const			{ return NULL; }
  Scope*	home() const			{ return (Scope*)this; }
  klassOop	methodHolder() const;

// debugging
  void		print_short();
  void		print();
};
  
  
class OutlinedBlockScope: public OutlinedScope {
 protected:
  OutlinedScope* _parent;			// parent or NULL (if non-LIFO)

 public:
  OutlinedBlockScope(nmethod* nm, ScopeDesc* sen);
    
  bool isOutlinedBlockScope() const		{ return true; }   
  bool isRecursiveCall(methodOop method, klassOop rcvrKlass, int n) { ShouldNotCallThis(); return false; }
  Scope*	parent() const			{ return _parent; }
  Scope*	home() const			{ return _parent ? _parent->home() : NULL; }
  klassOop	selfKlass() const		{ return _parent->selfKlass(); }
  smi		scopeID() const			{ return _scope->scopeID(); }
  klassOop	methodHolder() const		{ return _parent->methodHolder(); }

// debugging
  void		print_short();
  void		print();
};


# endif
