/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.30 $ */
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

// The MethodIterator iterates over the byte code structures of a methodOop
// Usage:
//    MethodIterator(method, &SomeMethodClosure);

// A MethodInterval represents an interval of byte codes
class MethodInterval: public ResourceObj {
 protected:
  MethodInterval* _parent;			// enclosing interval (or NULL if top-level)
  methodOop _method;
  int       _begin_bci;
  int       _end_bci;
  bool	    _in_prim_failure;			// currently in primitive failure block?
#ifdef DELTA_COMPILER
  IntervalInfo* _info;
#endif

  void initialize(methodOop method, MethodInterval* parent, int begin_bci, int end_bci, bool failBlock);
  void set_end_bci(int bci) { _end_bci = bci; }

  // Constructors
  MethodInterval(methodOop method, MethodInterval* parent);
  MethodInterval(methodOop method, MethodInterval* parent, 
		 int begin_bci, int end_bci = -1, bool failureBlock = false);
  friend class MethodIntervalFactory;

 public:
  // Test operations
  bool includes(int bci) const 		{ return begin_bci() <= bci && bci < end_bci(); }

  // Accessor operations
  methodOop method()     const 		{ return _method;     }
  int       begin_bci()  const 		{ return _begin_bci;  }
  int       end_bci()    const 		{ return _end_bci;    }
  MethodInterval* parent() const	{ return _parent; }

  // primitive failure block recognition (for inlining policy of compiler)
  bool in_prim_failure_block() const	{ return _in_prim_failure; }
  void set_prim_failure(bool f) 	{ _in_prim_failure = f; }

  // compiler support (not fully implemented/used yet)
#ifdef DELTA_COMPILER
  IntervalInfo* info() const		{ return _info; }
  void set_info(IntervalInfo* i)	{ _info = i; }
#endif
};


// The hierarchy of structures in a method is as follow:
//  - InlineSendNode
//    - CondNode           (expr_code)
//      - AndNode
//      - OrNode
//    - WhileNode          (expr_code, ?body_code)
//    - IfNode             (then_code, ?else_code)
//  - ExternalCallNode     (?failure_code)    
//    - PrimitiveCallNode
//    - DLLCallNode
// The names in the parenthesis show the inlined blocks. 
// ? denotes the inline block is optional.

class InlineSendNode: public MethodInterval {
 protected:
  // Constructor
  InlineSendNode(methodOop method, MethodInterval* parent, int begin_bci, int end_bci = -1);

 public:
  // Accessor operation
  virtual symbolOop selector() const = 0;
};


class CondNode: public InlineSendNode {
 protected:
  MethodInterval* _expr_code;

  // Constructor
  CondNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset);

 public:
  // Inlined block
  MethodInterval* expr_code() const { return _expr_code; }

  // Accessor operations
  virtual bool is_and() const = 0;
  virtual bool is_or() const  = 0;
};


class AndNode: public CondNode {
  // Constructor
  AndNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset);
  friend class MethodIntervalFactory;

 public:
  // Accessor operation
  symbolOop selector() const;
  bool is_and() const { return true; }
  bool is_or() const  { return false; }
};


class OrNode: public CondNode {
 protected:
  // Constructor
  OrNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset);
  friend class MethodIntervalFactory;

 public:
  // Accessor operation
  symbolOop selector() const;
  bool is_and() const { return false; }
  bool is_or() const  { return true; }
};


class WhileNode: public InlineSendNode {
 protected:
  bool _cond; 
  MethodInterval* _expr_code;
  MethodInterval* _body_code;
  
  // Constructor
  WhileNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int cond_offset, int end_offset);
  friend class MethodIntervalFactory;

 public:
  // Mandatory inlined block
  MethodInterval* expr_code() const { return _expr_code; }
  // Optional inlined block
  MethodInterval* body_code() const { return _body_code; }
  
  // Accessor operations
  symbolOop selector() const;
  bool is_whileTrue() const { return _cond; }
  bool is_whileFalse() const { return !_cond; }
};


class IfNode: public InlineSendNode {
 protected:
  bool _cond;
  bool _ignore_else_while_printing;
  bool _produces_result;
  MethodInterval* _then_code;
  MethodInterval* _else_code;

  // Constructor
  IfNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool cond, int else_offset, u_char structure);
  friend class MethodIntervalFactory;

 public:
  // Inlined block
  MethodInterval* then_code() const { return _then_code; }
  // Optional inlined block
  MethodInterval* else_code() const { return _else_code; }

  // Accessor operations
  symbolOop selector() const;
  bool is_ifTrue()		    const { return _cond; }
  bool is_ifFalse()		    const { return !_cond; }
  bool ignore_else_while_printing() const { return _ignore_else_while_printing; }
  bool produces_result()            const { return _produces_result; }
};


class ExternalCallNode: public MethodInterval {
 protected:
  MethodInterval* _failure_code;

  // Constructors
  ExternalCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci);
  ExternalCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int end_offset);

  public:
 // Optional inlined block
  MethodInterval* failure_code() const { return _failure_code; }
};


class PrimitiveCallNode: public ExternalCallNode {
 protected:
  primitive_desc* _pdesc;
  bool            _has_receiver;
  symbolOop       _name;

  // Constructors
  PrimitiveCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool has_receiver, symbolOop name, primitive_desc* pdesc);
  PrimitiveCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool has_receiver, symbolOop name, primitive_desc* pdesc, int end_offset);
  friend class MethodIntervalFactory;

 public:
  // Returns the primitive descriptor
  primitive_desc* pdesc() const        { return _pdesc; }
  bool            has_receiver() const { return _has_receiver; }
  symbolOop       name() const         { return _name; }
  int             number_of_parameters() const;
};


class DLLCallNode: public ExternalCallNode {
 protected:
  symbolOop       _dll_name;
  symbolOop       _function_name;
  int             _nofArgs;
  dll_func        _function;
  bool            _async;

  void initialize(InterpretedDLL_Cache* cache);

  // Constructors
  DLLCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, InterpretedDLL_Cache* cache);
  friend class MethodIntervalFactory;

 public:
  // DLL accessor operations
  symbolOop dll_name()      const { return _dll_name;      }
  symbolOop function_name() const { return _function_name; }
  int       nofArgs()       const { return _nofArgs;       }
  dll_func  function()      const { return _function;      }
  bool      async()         const { return _async;         }
};


// When creating a block closure, AllocationType specifies what is used
// in the context field of that block closure. When value is send to the
// block, the context field is copied into the activation frame of the block.

enum AllocationType {
  tos_as_scope,		// top of stack is used as context (usually nil or self)
  context_as_scope	// context of current stack frame (i.e. content of temp0)
  			// is used a context
};

class MethodIterator;

// A MethodClosure is the object handling the call backs when iterating through a MethodInterval.
// Virtuals are defined for each inline structure and each byte code not defining an inline structure.
class MethodClosure: ValueObj {
 private:
  methodOop _method;
  int       _bci;
  int       _next_bci;
  bool      _aborting;
  bool	    _in_prim_failure;			// currently in primitive failure block?
  int       _float0_index;

  void set_method(methodOop method);
  void set_bci(int bci)				{ _bci      = bci;      }
  void set_next_bci(int next_bci)		{ _next_bci = next_bci; }
  int  float_at(int index);			// converts the float byte code index into a float number

 protected:
  MethodClosure();
  // operations to terminate iteration
  virtual void abort()				{ _aborting = true; }
  virtual void un_abort()			{ _aborting = false; }

  friend class MethodIterator;

 public:
  int       bci() const				{ return _bci;      }
  int       next_bci() const			{ return _next_bci; }
  methodOop method() const			{ return _method;   }
  bool	    aborting() const			{ return _aborting; }// was iteration aborted?  (dead code)

  // The following function are called when an inlined structure
  // is recognized. It is the functions responsibility to iterate
  // over the structures byte codes.
  virtual void if_node(IfNode* node)				= 0; // inlined if message send
  virtual void cond_node(CondNode* node)			= 0; // inlined or/and message send
  virtual void while_node(WhileNode* node)			= 0; // inlined while message send
  virtual void primitive_call_node(PrimitiveCallNode* node)	= 0; // primitive call with inlined failure block
  virtual void dll_call_node(DLLCallNode* node)			= 0; // dll call 

  // primitive failure block recognition (for inlining policy of compiler)
  virtual bool in_prim_failure_block() const	{ return _in_prim_failure; }
  virtual void set_prim_failure(bool f) 	{ _in_prim_failure = f; }

 public:
  // Specifies the number of additional temporaries that are allocated on the stack
  // and initialized. Note: One temporary is always there and initialized (temp0).
  virtual void allocate_temporaries(int nofTemps)		= 0;

  // Push a value on the stack.
  virtual void push_self()					= 0;
  virtual void push_tos()					= 0;
  virtual void push_literal(oop obj)				= 0;

  // Argument numbers are 0, 1, ... starting with the first argument (0).
  // Temporary numbers are 0, 1, ... starting with the first temporary (0).
  // Contexts are numbered 0, 1, ... starting with the context held in the
  // current frame (i.e., in temp0). Context i+1 is the context reached by
  // dereferencing the home field of context i. Globals are held in the value
  // field of an association (obj).
  virtual void push_argument(int no)				= 0;
  virtual void push_temporary(int no)				= 0;
  virtual void push_temporary(int no, int context)		= 0;

  virtual void push_instVar(int offset)				= 0;
  virtual void push_instVar_name(symbolOop name)		= 0;

  virtual void push_classVar(associationOop assoc)		= 0;
  virtual void push_classVar_name(symbolOop name)		= 0;

  virtual void push_global(associationOop obj)	 		= 0;

  virtual void store_temporary(int no)				= 0;
  virtual void store_temporary(int no, int context)		= 0;

  virtual void store_instVar(int offset)			= 0;
  virtual void store_instVar_name(symbolOop name)		= 0;

  virtual void store_classVar(associationOop assoc)		= 0;
  virtual void store_classVar_name(symbolOop name)		= 0;

  virtual void store_global(associationOop obj)			= 0;

  virtual void pop() 						= 0;

  // The receiver and arguments are pushed prior to a normal send. Only the
  // arguments are pushed for self and super sends (the receiver is self).
  virtual void normal_send(InterpretedIC* ic) 			= 0;
  virtual void self_send  (InterpretedIC* ic) 			= 0;
  virtual void super_send (InterpretedIC* ic) 			= 0;

  // Hardwired sends
  virtual void double_equal()					= 0;
  virtual void double_not_equal()				= 0;

  // nofArgs is the number of arguments to be popped before returning (callee popped arguments).
  virtual void method_return(int nofArgs)			= 0;
  virtual void nonlocal_return(int nofArgs)			= 0;

  virtual void allocate_closure(AllocationType type, int nofArgs, methodOop meth) = 0;
  virtual void allocate_context(int nofTemps, bool forMethod)	= 0;

  // fp->recv = fp->context->bottom()->recv
  virtual void set_self_via_context()				= 0;
  virtual void copy_self_into_context()				= 0;
  virtual void copy_argument_into_context(int argNo, int no)	= 0;

  // fp->context->home
  virtual void zap_scope()					= 0;

  // indicates the method is a pure primitive call
  virtual void predict_prim_call(primitive_desc* pdesc, int failure_start) = 0;

  // floating-point operations
  virtual void float_allocate(int nofFloatTemps, int nofFloatExprs) = 0;
  virtual void float_floatify(Floats::Function f, int fno)	= 0;
  virtual void float_move(int fno, int from)			= 0;
  virtual void float_set(int fno, doubleOop value)		= 0;
  virtual void float_nullary(Floats::Function f, int fno)	= 0;
  virtual void float_unary(Floats::Function f, int fno)		= 0;
  virtual void float_binary(Floats::Function f, int fno)	= 0;
  virtual void float_unaryToOop(Floats::Function f, int fno)	= 0;
  virtual void float_binaryToOop(Floats::Function f, int fno)	= 0;
};

class CustomizedMethodClosure : public MethodClosure {
 public:
  // virtuals from MethodClosure
  void push_instVar_name(symbolOop name);
  void store_instVar_name(symbolOop name);

  void push_classVar_name(symbolOop name);
  void push_classVar(associationOop assoc);

  void store_classVar_name(symbolOop name);
  void store_classVar(associationOop assoc);
};

// A SpecializedMethodClosure is a MethodClosure that provides default implementations for
// all inlined structures as well as empty implementations for methods corresponding to
// individual bytecodes. A SpecializedMethodClosure should be used when only a few methods
// need to be specialized.
class SpecializedMethodClosure: public CustomizedMethodClosure {
 public:
  virtual void if_node(IfNode* node);
  virtual void cond_node(CondNode* node);
  virtual void while_node(WhileNode* node);
  virtual void primitive_call_node(PrimitiveCallNode* node);
  virtual void dll_call_node(DLLCallNode* node);

 public:
  // Customize this method to get uniform behavior for all instructions.
  virtual void instruction()								{}

 public:
  virtual void allocate_temporaries(int nofTemps)					{ instruction(); }
  virtual void push_self()								{ instruction(); }
  virtual void push_tos()								{ instruction(); }
  virtual void push_literal(oop obj)							{ instruction(); }
  virtual void push_argument(int no)							{ instruction(); }
  virtual void push_temporary(int no)							{ instruction(); }
  virtual void push_temporary(int no, int context)					{ instruction(); }
  virtual void push_instVar(int offset)							{ instruction(); }
  virtual void push_global(associationOop obj)						{ instruction(); }
  virtual void store_temporary(int no)							{ instruction(); }
  virtual void store_temporary(int no, int context)					{ instruction(); }
  virtual void store_instVar(int offset)						{ instruction(); }
  virtual void store_global(associationOop obj)						{ instruction(); }
  virtual void pop()									{ instruction(); }
  virtual void normal_send(InterpretedIC* ic) 						{ instruction(); }
  virtual void self_send  (InterpretedIC* ic) 						{ instruction(); }
  virtual void super_send (InterpretedIC* ic) 						{ instruction(); }
  virtual void double_equal()								{ instruction(); }
  virtual void double_not_equal()							{ instruction(); }
  virtual void method_return(int nofArgs)						{ instruction(); }
  virtual void nonlocal_return(int nofArgs)						{ instruction(); }
  virtual void allocate_closure(AllocationType type, int nofArgs, methodOop meth)	{ instruction(); }
  virtual void allocate_context(int nofTemps, bool forMethod)				{ instruction(); }
  virtual void set_self_via_context()							{ instruction(); }
  virtual void copy_self_into_context()							{ instruction(); }
  virtual void copy_argument_into_context(int argNo, int no)				{ instruction(); }
  virtual void zap_scope()								{ instruction(); }
  virtual void predict_prim_call(primitive_desc* pdesc, int failure_start)		{ instruction(); }
  virtual void float_allocate(int nofFloatTemps, int nofFloatExprs)			{ instruction(); }
  virtual void float_floatify(Floats::Function f, int fno)				{ instruction(); }
  virtual void float_move(int fno, int from)						{ instruction(); }
  virtual void float_set(int fno, doubleOop value)					{ instruction(); }
  virtual void float_nullary(Floats::Function f, int fno)				{ instruction(); }
  virtual void float_unary(Floats::Function f, int fno)					{ instruction(); }
  virtual void float_binary(Floats::Function f, int fno)				{ instruction(); }
  virtual void float_unaryToOop(Floats::Function f, int fno)				{ instruction(); }
  virtual void float_binaryToOop(Floats::Function f, int fno)				{ instruction(); }
};


// factory to parameterize construction of nodes
class AbstractMethodIntervalFactory : StackObj {
 public:
  virtual MethodInterval*    new_MethodInterval(methodOop method, MethodInterval* parent) = 0;
  virtual MethodInterval*    new_MethodInterval(methodOop method, MethodInterval* parent, 
					        int begin_bci, int end_bci = -1, bool failureBlock = false) = 0;
  virtual AndNode*	     new_AndNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset) = 0;
  virtual OrNode*	     new_OrNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset) = 0;
  virtual WhileNode*	     new_WhileNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int cond_offset, int end_offset) = 0;
  virtual IfNode*	     new_IfNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool cond, int else_offset, u_char structure) = 0;
  virtual PrimitiveCallNode* new_PrimitiveCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool has_receiver, symbolOop name, primitive_desc* pdesc) = 0;
  virtual PrimitiveCallNode* new_PrimitiveCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool has_receiver, symbolOop name, primitive_desc* pdesc, int end_offset) = 0;
  virtual DLLCallNode*	     new_DLLCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, InterpretedDLL_Cache* cache) = 0;
};

// default factory (used by everyone except the compiler)
class MethodIntervalFactory : public AbstractMethodIntervalFactory {
 public:
  MethodInterval*    new_MethodInterval(methodOop method, MethodInterval* parent);
  MethodInterval*    new_MethodInterval(methodOop method, MethodInterval* parent, 
					int begin_bci, int end_bci = -1, bool failureBlock = false);
  AndNode*	     new_AndNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset);
  OrNode*	     new_OrNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int dest_offset);
  WhileNode*	     new_WhileNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, int cond_offset, int end_offset);
  IfNode*	     new_IfNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool cond, int else_offset, u_char structure);
  PrimitiveCallNode* new_PrimitiveCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool has_receiver, symbolOop name, primitive_desc* pdesc);
  PrimitiveCallNode* new_PrimitiveCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, bool has_receiver, symbolOop name, primitive_desc* pdesc, int end_offset);
  DLLCallNode*	     new_DLLCallNode(methodOop method, MethodInterval* parent, int begin_bci, int next_bci, InterpretedDLL_Cache* cache);
};

// A MethodIterator iterates over a MethodInterval and dispatches calls to the provided MethodClosure
class MethodIterator: StackObj {
 private:
  void dispatch(MethodClosure* blk);
  void unknown_code(u_char code);
  void should_never_encounter(u_char code);
  MethodInterval* _interval;
  static MethodIntervalFactory defaultFactory;      // default factory
 public:
  static AbstractMethodIntervalFactory* factory;      // used to build nodes

  MethodIterator(methodOop m, MethodClosure* blk, AbstractMethodIntervalFactory* f = &defaultFactory);
  MethodIterator(MethodInterval* interval, MethodClosure* blk, AbstractMethodIntervalFactory* f = &defaultFactory);
  MethodInterval* interval() const { return _interval; }
};
