/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.129 $ */
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

// This file defines the intermediate language used by the Compiler.
// For each node a code pattern is generated during code generation.
// Note: offsets in nodes are always in oops!

class Node;				// abstract

class   TrivialNode;			// abstract; has no defs/uses; most generate no code
class     MergeNode;			// nop; can have >1 predecessors
class     NopNode;			// generates no code (place holders etc)
class     CommentNode;			// for compiler debugging
class     FixedCodeNode;		// fixed code pattern for compiler debugging, instrumentation, etc.
class     LoopHeaderNode;		// loop header (contains type tests moved out of loop)

class   NonTrivialNode;			// non-trivial node, may have defs/uses and generate code

class     PrologueNode;			// entire method prologue

class     LoadNode;			// abstract
class       LoadIntNode;		// load vm constant
class       LoadOffsetNode;		// load slot of some object
class       LoadUplevelNode;		// load up-level accessed variable

class     StoreNode;
class       AssignNode;			// general assignment (e.g. into temp)
class       StoreOffsetNode;		// store into slot of object
class       StoreUplevelNode;		// store up-level accessed variable

class     AbstractReturnNode;
class       ReturnNode;			// method return
class       NLRSetupNode;		// setup NLR leaving this compiled method
class       InlinedReturnNode;		// inlined method return (old backend only)
class       NLRContinuationNode;	// continue NLR going through this method

class     ArithNode;			// all computations (incl comparisons)
class     AbstractBranchNode;		// nodes with (potentially) >1 sucessors
class       TArithRRNode;		// tagged arithmetic nodes
class       CallNode;			// abstract
class         SendNode;			// Delta send
class         PrimNode;			// primitive call
class           BlockCreateNode;	// inital block clone (possibly memoized)
class             BlockMaterializeNode;	// create block (testing for memoization)
class           InterruptCheckNode;	// interrupt check / stack overflow test
class         DLLNode;			// DLL call
class	      ContextCreateNode;	// creates a context and copies arguments

class       AbstractArrayAtNode;
class         ArrayAtNode;   		// _At: primitive
class         AbstractArrayAtPutNode;
class           ArrayAtPutNode;		// _At:Put: primitive

class       InlinedPrimitiveNode;       // specialized inlined primitives

class       BranchNode;			// machine-level conditional branches
class       TypeTestNode;		// type tests (compare klasses, incl int/float)
class       NLRTestNode;		// tests whether NLR has found home method
class	  ContextInitNode;		// initialize context fields
class     ContextZapNode;		// zap context (new backend only)

class     UncommonNode;			// uncommon branch

void initNodes();			// to be called before each compilation


// Node definitions

// Node: abstract superclass, holds common behavior (except prev/next links)

class PRegMapping;

class BasicNode: public PrintableResourceObj {
 protected:
  BB*		_bb;			// basic block to which I belong
  int16		_id;			// unique node id for debugging
  int16		_num;			// node number within basic block
  InlinedScope*	_scope;			// scope to which we belong
  int16		_bci;			// bci within that scope
  PRegMapping*	_mapping;		// the mapping at that node, if any (will be modified during code generation)

 public:
  Label		label;	    	    	// for jumps to this node -- SHOULD BE MOVED TO BB -- fix this
  bool		dontEliminate;	    	// for special cases: must not elim. this node
  bool		deleted;		// node has been deleted

  int		id() const		{ return this == NULL ? -1 : _id; }
  BB*		bb() const		{ return _bb; }
  int		num() const		{ return _num; }
  InlinedScope*	scope() const		{ return _scope; }
  int		bci() const		{ return _bci; }

  void		setBB(BB* b)		{ _bb = b; }
  void		setNum(int n)		{ _num = n; }
  void		setScope(InlinedScope* s);

  static int currentID;			// current node ID
  static int currentCommentID;		// current ID for comment nodes
  static ScopeInfo lastScopeInfo; 	// for pcDesc generation
  static int lastBCI;

  BasicNode();

  virtual bool isPrologueNode() const		{ return false; }
  virtual bool isAssignNode() const		{ return false; }
  virtual bool isTArithNode() const		{ return false; }
  virtual bool isArithNode() const		{ return false; }
  virtual bool isNLRSetupNode() const		{ return false; }
  virtual bool isNLRTestNode() const		{ return false; }
  virtual bool isNLRContinuationNode() const	{ return false; }
  virtual bool isReturnNode() const		{ return false; }
  virtual bool isInlinedReturnNode() const	{ return false; }
  virtual bool isLoopHeaderNode() const		{ return false; }
  virtual bool isExitNode() const		{ return false; }
  virtual bool isMergeNode() const		{ return false; }
  virtual bool isBranchNode() const		{ return false; }
  virtual bool isBlockCreateNode() const	{ return false; }
  virtual bool isContextCreateNode() const	{ return false; }
  virtual bool isContextInitNode() const	{ return false; }
  virtual bool isContextZapNode() const		{ return false; }
  virtual bool isCommentNode() const		{ return false; }
  virtual bool isSendNode() const    		{ return false; }
  virtual bool isCallNode() const    		{ return false; }
  virtual bool isStoreNode() const    		{ return false; }
  virtual bool isDeadEndNode() const   		{ return false; }
  virtual bool isTypeTestNode() const  		{ return false; }
  virtual bool isUncommonNode() const  		{ return false; }
  virtual bool isNopNode() const		{ return false; }
  virtual bool isCmpNode() const		{ return false; }
  virtual bool isArraySizeLoad() const		{ return false; }
  virtual bool isAccessingFloats() const	{ return false; }
  virtual bool isTrivial() const		= 0;

  protected:
  virtual Node* clone(PReg* from, PReg* to) const { SubclassResponsibility(); return NULL; }
  void genPcDesc();
 public:
  // for splitting: rough estimate of space cost of node (in bytes)
  virtual int  cost() const 			{ return oopSize; }
  virtual bool hasDest() const 			{ return false; }
  virtual bool canCopyPropagate() const 	{ return false; }
  	// canCopyPropagate: can node replace a use with copy-propagated PReg?
    	// if true, must implement copyPropagate below
          bool canCopyPropagate(Node* fromNode) const; // can copy-propagate from fromNode to receiver?
  virtual bool copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false) = 0;
  virtual bool canCopyPropagateOop() const 	{ return false; }
  	// canCopyPropagateOop: can node replace a use with a copy-propagated
    	// oop?  if true, must handle ConstPRegs; implies canCopyPropagate
  virtual bool isAssignmentLike() const 	{ return false; }
    	// isAssignmentLike: node copies src to dest (implies hasSrc/Dest)
  virtual bool shouldCopyWhenSplitting() const 	{ return false; }
  virtual bool hasSrc() const 			{ return false; }
  virtual bool hasConstantSrc() const 		{ return false; }
  virtual oop  constantSrc() const	    	{ ShouldNotCallThis(); return 0; }
  virtual bool canChangeDest() const 		{ assert(hasDest(), "shouldn't call"); return true; }

  virtual bool endsBB() const			= 0;
  virtual bool startsBB() const 	    	{ return false; }

  int loopDepth() const				{ return _bb->loopDepth(); }

  virtual BB* newBB();
  virtual void makeUses(BB* bb) { Unused(bb); }
  virtual void removeUses(BB* bb) { Unused(bb); }
  virtual void eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false) = 0;
  virtual bool canBeEliminated() const { return !dontEliminate; }
  virtual void computeEscapingBlocks(GrowableArray<BlockPReg*>* lst) { Unused(lst); }
  virtual void markAllocated(int* use_count, int* def_count) = 0;
  virtual SimpleBitVector trashedMask();
  virtual void gen();
  virtual void apply(NodeVisitor* v)		{ ShouldNotCallThis(); }
  virtual Node* likelySuccessor() const = 0;	// most likely code path
  virtual Node* uncommonSuccessor() const = 0;	// code path taken only in uncommon situations
  void removeUpToMerge();			// remove all nodes from this to next merge
  Node* copy(PReg* from, PReg* to) const;	// make a copy, substituting 'to' wherever 'from' is used

  // for type test / loop optimization
  // If a node includes one or more type tests of its argument(s), it should return true for doesTypeTests
  // and implement the other four methods in this group.  It can then benefit from type test optimizations
  // (e.g., moving a test out of a loop).
  virtual bool doesTypeTests()  const { return false; }	      // does node perform any type test?
  virtual bool hasUnknownCode() const { return false; }	      // does handle unknown cases? (with real code, not uncommon branch)
  virtual void collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const { ShouldNotCallThis(); }
	  // return a list of pregs tested and, for each preg, a list of its types
  virtual void assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n) {} // assert that the klass of r (used by the reciver) is oneof(klasses)
  virtual void assert_in_bounds(PReg* r, LoopHeaderNode* n) {}// assert that r (used by the reciver) is within array bounds

  virtual void print_short();
  virtual void print() 			{ print_short(); }
  virtual char* print_string(char* buf, bool printAddr = true) const = 0;
  void printID() const;
  virtual void verify() const {}

  // Mappings
  //
  // Note: make sure, mapping at node is always unchanged, once it is set.

  PRegMapping* mapping() const;
  bool hasMapping() const			{ return _mapping != NULL; }
  void setMapping(PRegMapping* mapping);
};


// Linking code for Nodes
// originally: NodeClassTemplate(Node, BasicNode)

class Node: public BasicNode {
 protected:
  Node *_prev, *_next;
  Node() { _prev = _next = NULL; }

 public:
  virtual bool endsBB() const;
  virtual Node* likelySuccessor() const;
  virtual Node* uncommonSuccessor() const;
  void eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  virtual void verify() const;

  friend class NodeFactory;

  // ----------- node linking behavior -----------
  virtual bool hasSingleSuccessor() const		{ return true; }
  virtual bool hasSinglePredecessor() const		{ return true; }
  virtual int nPredecessors() const			{ return _prev ? 1 : 0; }
  virtual int nSuccessors() const			{ return _next ? 1 : 0; }
  virtual bool isPredecessor(const Node* n) const	{ return _prev == n; }
  virtual bool isSuccessor(const Node* n) const		{ return _next == n; }
  Node* next() const					{ return _next; }
  virtual Node* next1() const				{ return NULL; }
  virtual Node* next(int i) const {
    if (i == 0) return _next; else { fatal("single next"); return NULL;} }
  virtual Node* firstPrev() const 			{ return _prev; }
  virtual Node* prev(int n) const			{ if (n == 0) return _prev; else fatal("single prev"); return NULL; }

  virtual void setPrev(Node* p)				{ assert(_prev == NULL, "already set"); _prev = p; }
  virtual void movePrev(Node* from, Node* to)		{ assert(_prev == from, "mismatched prev link"); _prev = to; }

  void setNext(Node* n)					{ assert(_next == NULL, "already set"); _next = n; }
  virtual void setNext1(Node* n)			{ Unused(n); ShouldNotCallThis(); }
  virtual void setNext(int i, Node* n)			{ if (i == 0) setNext(n); else fatal("subclass"); }
  virtual void moveNext(Node* from, Node* to)		{ assert(_next == from, "mismatched next link"); _next = to; }

 protected:
  virtual void removeMe();
  Node* endOfList() const;

 public:  // really should be private, but C++ has instance- rather than class-based privacy -- don't you love it?
  virtual void removePrev(Node* n);

 public:
  virtual void removeNext(Node* n);			// cut the next link between this and n
  Node* append(Node* p)					{ setNext(p);  p->setPrev(this); return p; }
  Node* append1(Node* p)				{ setNext1(p); p->setPrev(this); return p; }
  Node* append(int i, Node* p)				{ setNext(i, p);  p->setPrev(this); return p; }
  Node* appendEnd(Node* p)				{ return endOfList()->append(p); }

  void insertNext(Node* n) {
    assert(hasSingleSuccessor(), ">1 successors");
    n->setNext(_next); n->setPrev(this);
    _next->movePrev(this, n); _next = n; }
  void insertPrev(Node* n) {
    assert(n->hasSinglePredecessor(), "where to insert?");
    n->setPrev(_prev); n->setNext(this);
    _prev->moveNext(this, n); _prev = n; }

  friend class NodeBuilder;
};


class TrivialNode: public Node {
 public:
  bool isTrivial() const 		{ return true; }
  int  cost() const			{ return 0; }
  bool copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false) { return false; }
  void markAllocated(int* use_count, int* def_count) {}

  // may need to implement several inherited dummy methods here -- most
  // will do nothing

  friend class NodeFactory;
};


class NonTrivialNode: public Node {
 protected:
  PReg*	_dest;				// not all nodes actually have src & dest,
  PReg*	_src;				// but they're declared here for convenience
  Use*	srcUse;				// and easier code sharing
  Def*	destDef;

  NonTrivialNode();

 public:
  bool	isTrivial() const 		{ return false; }
  PReg*	src() const;
  PReg*	dest() const;
  PReg*	dst() const			{ return dest(); }
  void	setDest(BB* bb, PReg* d);
  virtual bool copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	verify() const;

  friend class NodeFactory;
};


class PrologueNode : public NonTrivialNode {
 protected:
  LookupKey*  _key;
  const int   _nofArgs;
  const int   _nofTemps;

  PrologueNode(LookupKey* key, int nofArgs, int nofTemps) : _nofArgs(nofArgs), _nofTemps(nofTemps) {
    _key	= key;
  }

 public:
  bool isPrologueNode() const		{ return true; }
  virtual bool canChangeDest() const 	{ return false; }	// has no dest
  Node* clone(PReg* from, PReg* to) const;
  void makeUses(BB* bb);
  void removeUses(BB* bb) { ShouldNotCallThis(); }
  void gen();
  void apply(NodeVisitor* v)		{ v->aPrologueNode(this); }
  bool canBeEliminated() const 		{ return false; }
  void markAllocated(int* use_count, int* def_count) {
    Unused(use_count); Unused(def_count); }
  char* print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class LoadNode: public NonTrivialNode {
 protected:
  LoadNode(PReg* d)			{ _dest = d; assert(d, "dest is NULL"); }

 public:
  bool hasDest() const			{ return true; }
  bool canCopyPropagate() const		{ return true; }
  bool canCopyPropagateOop() const	{ return true; }
  void makeUses(BB* bb);
  void removeUses(BB* bb);
  void markAllocated(int* use_count, int* def_count);
  void eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);

  friend class NodeFactory;
};


class LoadIntNode: public LoadNode {
 protected:
  int _value;		// constant (vm-level, not an oop) to be loaded

  LoadIntNode(PReg* dst, int value) : LoadNode(dst) { _value = value; }

 public:
  Node*	clone(PReg* from, PReg* to) const;
  int	value()				{ return _value; }
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aLoadIntNode(this); }
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class LoadOffsetNode: public LoadNode {
 public:
  // _src is base address (e.g. object containing a slot)
  int	offset;		// offset in words
  bool  isArraySize;	// is this load implementing an array size primitive?

 protected:
  LoadOffsetNode(PReg* dst, PReg* b, int offs, bool arr) : LoadNode(dst) {
    _src = b; offset = offs; isArraySize = arr; }

 public:
  Node*	clone(PReg* from, PReg* to) const;
  PReg* base() const			{ return _src; }
  bool  hasSrc() const			{ return true; }
  bool  isArraySizeLoad() const		{ return isArraySize; }
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  bool	copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aLoadOffsetNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class LoadUplevelNode: public LoadNode {
 private:
  Use*		_context0Use;
  PReg*		_context0;		// starting context
  int		_nofLevels;		// no. of indirections to follow via context home field
  int		_offset;		// offset of temporary in final context
  symbolOop	_name;			// temporary name (for printing)

 protected:
  LoadUplevelNode(PReg* dst, PReg* context0, int nofLevels, int offset, symbolOop name);

 public:
  PReg*	context0() const		{ return _context0; }
  int	nofLevels() const		{ return _nofLevels; }
  int	offset() const			{ return _offset; }
  Node* clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  bool	copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aLoadUplevelNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class StoreNode: public NonTrivialNode {
 protected:
  StoreNode(PReg* s) 			{ _src = s; assert(_src, "src is NULL"); }

 public:
  bool isStoreNode() const 		{ return true; }
  bool canCopyPropagate() const 	{ return true; }
  bool canCopyPropagateOop() const 	{ return true; }
  bool hasSrc() const			{ return true; }
  virtual bool needsStoreCheck() const	{ return false; }
  virtual const char* action() const		= 0;		// for debugging messages
  virtual void setStoreCheck(bool ncs)	{}
  void assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n);
  void makeUses(BB* bb);
  void removeUses(BB* bb);
  void markAllocated(int* use_count, int* def_count);
  void eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void computeEscapingBlocks(GrowableArray<BlockPReg*>* l);

  friend class NodeFactory;
};


class StoreOffsetNode: public StoreNode {
  // store into data slot, do check-store if necessary
  // _src is value being stored, may be a ConstPReg*
 private:
  PReg* _base;				// base address (object containing the slot)
  Use*  _baseUse;
  int	_offset;			// offset in words
  bool	_needsStoreCheck;		// does store need a GC store check?

 protected:
  StoreOffsetNode(PReg* s, PReg* b, int o, bool nsc) : StoreNode(s) {
    _base = b; assert(b, "base is NULL"); _offset = o; _needsStoreCheck = nsc; }

 public:
  PReg* base() const			{ return _base; }
  int	offset() const			{ return _offset; }
  bool	needsStoreCheck() const		{ return _needsStoreCheck; }
  bool	hasSrc() const			{ return true; }
  void  setStoreCheck(bool ncs)		{ _needsStoreCheck = ncs; }
  char* action() const			{ return "stored into an object"; }
  Node*	clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  bool	copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  bool	canBeEliminated() const 	{ return false; }
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aStoreOffsetNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class StoreUplevelNode: public StoreNode {
  // when node is created, it is not known whether store will be to stack/reg or context
  // _src may be a ConstPReg*
 private:
  Use*		_context0Use;
  PReg*		_context0;		// starting context
  int		_nofLevels; 		// no. of indirections to follow via context home field
  int		_offset;		// offset of temporary in final context
  bool		_needsStoreCheck;	// generate a store check if true
  symbolOop	_name;			// temporary name (for printing)

 protected:
  StoreUplevelNode(PReg* src, PReg* context0, int nofLevels, int offset, symbolOop name, bool needsStoreCheck);

 public:
  PReg*	context0() const		{ return _context0; }
  int	nofLevels() const		{ return _nofLevels; }
  int	offset() const			{ return _offset; }
  bool	needsStoreCheck() const		{ return _needsStoreCheck; }
  void  setStoreCheck(bool ncs) 	{ _needsStoreCheck = ncs; }
  char* action() const			{ return "stored into a context temporary"; }
  Node*	clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  bool	copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aStoreUplevelNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class AssignNode : public StoreNode {
  // _src may be a ConstPReg*
 protected:
  AssignNode(PReg* s, PReg* d);

 public:
  int	cost() const	    	    	{ return oopSize/2; }  // assume 50% eliminated
  bool	isAccessingFloats() const;
  bool	isAssignNode() const		{ return true; }
  bool	hasDest() const 		{ return true; }
  bool	hasSrc() const 			{ return true; }
  bool	hasConstantSrc() const  	{ return _src->isConstPReg(); }
  bool	isAssignmentLike() const	{ return true; }
  bool  shouldCopyWhenSplitting() const { return true; }
  bool	canBeEliminated() const;
  oop	constantSrc() const;
  const char* action() const			{ return _dest->isSAPReg() ?
						  "passed as an argument" : "assigned to a local"; }
  Node*	clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void	gen();
  void	apply(NodeVisitor* v)		{ v->anAssignNode(this); }
  char*	print_string(char* buf, bool printAddr = true) const;

 protected:
  void	genOop();

  friend class NodeFactory;
};


class AbstractReturnNode: public NonTrivialNode {
 protected:
  AbstractReturnNode(int bci, PReg* src, PReg* dest)	{ _bci = bci; _src = src; _dest = dest; }

 public:
  bool	canBeEliminated() const 	{ return false; }
  bool	isReturnNode() const		{ return true; }
  bool	endsBB() const 			{ return true; }
  bool	canCopyPropagate() const 	{ return true; }
  bool	canCopyPropagateOop() const 	{ return true; }
  bool	hasSrc() const 			{ return true; }
  bool	isAssignmentLike() const	{ return true; }
  bool	hasDest() const 		{ return true; }
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	computeEscapingBlocks(GrowableArray<BlockPReg*>* l);
  void	markAllocated(int* use_count, int* def_count) { Unused(use_count); Unused(def_count); }

  friend class NodeFactory;
};


class InlinedReturnNode: public AbstractReturnNode {
  // should replace with AssignNode + ContextZap (if needed)
 protected:
  InlinedReturnNode(int bci, PReg* src, PReg* dest) : AbstractReturnNode(bci, src, dest) {}

 public:
  bool	isInlinedReturnNode() const	{ return true; }
  bool	endsBB() const 			{ return NonTrivialNode::endsBB(); }
  bool	isTrivial() const 	    	{ return true; }
  bool	canBeEliminated() const 	{ return true; }
  bool  shouldCopyWhenSplitting() const { return true; }
  int	cost() const 			{ return 0; }
  Node*	clone(PReg* from, PReg* to) const;
  void	gen();
  void	apply(NodeVisitor* v)		{ v->anInlinedReturnNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


// The NLRSetupNode starts an NLR by setting up two of the three special NLR regs
// (home fp and home scope ID); it assumes the NLR result already is in NLRResultReg
// (so that the setup code can be shared).
// next() is the NLRContinuationNode that actually does the NLR.  (Functionality is split
// up because the NLRContinuationNode may be shared with NLRTestNodes.)

class NLRSetupNode: public AbstractReturnNode {
  Use*	resultUse;
  Use*	contextUse;			// needs context to load home FP
 protected:
  NLRSetupNode(PReg* result, int bci);

 public:
  bool	isExitNode() const		{ return true; }
  bool	isNLRSetupNode() const		{ return true; }
  // uses hardwired regs, has no src or dest
  bool	canCopyPropagate() const 	{ return false; }
  bool	canCopyPropagateOop() const 	{ return false; }
  bool	hasSrc() const 			{ return false; }
  bool	isAssignmentLike() const	{ return false; }
  bool	hasDest() const 		{ return false; }
  bool	canChangeDest() const 		{ return false; }	// has no dest
  Node*	clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aNLRSetupNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class NLRContinuationNode: public AbstractReturnNode {
 protected:
  NLRContinuationNode(int bci, PReg* src, PReg* dest) : AbstractReturnNode(bci, src, dest) { }

 public:
  bool	isExitNode() const		{ return true; }
  bool	isNLRContinuationNode() const	{ return true; }
  // uses hardwired regs, has no src or dest
  bool	canCopyPropagate() const 	{ return false; }
  bool	canCopyPropagateOop() const 	{ return false; }
  bool	hasSrc() const 			{ return false; }
  bool	isAssignmentLike() const	{ return false; }
  bool	hasDest() const 		{ return false; }
  bool	canChangeDest() const 		{ return false; }	// has no dest
  Node*	clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aNLRContinuationNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


// normal method return
class ReturnNode: public AbstractReturnNode {
 private:
  Use*	resultUse;

 protected:
  ReturnNode(PReg* res, int bci);

 public:
  Node*	clone(PReg* from, PReg* to) const;
  bool	isExitNode() const		{ return true; }
  bool	hasSrc() const 			{ return false; }
  bool	isAssignmentLike() const	{ return false; }
  bool	hasDest() const 		{ return false; }
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aReturnNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


// Linking code for MergeNodes
// originally: MergeNodeClassTemplate(AbstractMergeNode, Node, GrowableArray<Node*>, TrivialNode, 3)

class AbstractMergeNode: public TrivialNode {
 private:
  enum { N = 3 };
  /* n-way merge */
 protected:
  GrowableArray<Node*>* _prevs;

 public:
  AbstractMergeNode()			{ _prevs = new GrowableArray<Node*>(N); }
  AbstractMergeNode(Node* prev1, Node* prev2) {
    _prevs = new GrowableArray<Node*>(N);
    _prevs->append(prev1); prev1->setNext(this);
    _prevs->append(prev2); prev2->setNext(this);
  }

  bool hasSinglePredecessor() const	{ return _prevs->length() <= 1; }
  int nPredecessors() const		{ return _prevs->length(); }
  Node* firstPrev() const		{ return _prevs->isEmpty() ? NULL : _prevs->at(0); }

  void setPrev(Node* p) {
    assert(p, "should be something");
    assert(!_prevs->contains(p), "shouldn't already be there");
    _prevs->append(p);
  }

  Node* prev(int n) const		{ return _prevs->at(n); }

 protected:
  void removeMe();
  virtual void removePrev(Node* n) {
    /* cut the _prev link between this and n	*/
    _prevs->remove(n);
  }

 public:
  void movePrev(Node* from, Node* to);
  bool isPredecessor(const Node* n) const;
};


class MergeNode : public AbstractMergeNode {
 protected:
  MergeNode(int bci);
  MergeNode(Node* prev1, Node* prev2);

 public:
  bool	isLoopStart;			// does this node start a loop?
  bool	isLoopEnd;			// does this node end a loop? (i.e., first node after loop)
  bool	didStartBB;			// used for debugging / assertion checks
  int	cost() const			{ return 0; }
  bool	isTrivial() const		{ return _prevs->length() <= 1; }
  bool	startsBB() const 		{ return _prevs->length() > 1 || isLoopStart; }
  bool	isMergeNode() const		{ return true; }
  BB*	newBB();
  Node*	clone(PReg* from, PReg* to) const;
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aMergeNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class ArithNode : public NonTrivialNode {	// abstract
  // NB: ArithNodes are not used for tagged int arithmetic -- see TArithNode
 protected:
  ArithOpCode	_op;
  ConstPReg*	_constResult; 	// non-NULL if constant-folded

  ArithNode(ArithOpCode op, PReg* src, PReg* dst) {
    _op = op; _src = src; _dest = dst; _constResult = NULL;
  }

 public:
  bool		canCopyPropagate() const 	{ return true; }
  bool		canCopyPropagateOop() const 	{ return true; }
  bool		hasSrc() const 			{ return true; }
  bool		hasDest() const 		{ return true; }
  bool		isAssignmentLike() const 	{ return _constResult != NULL; }
  bool		isArithNode() const		{ return true; }
  bool		isCmpNode() const		{ return _op == tCmpArithOp; }
  void		makeUses(BB* bb);
  void		removeUses(BB* bb);
  void		markAllocated(int* use_count, int* def_count);
  void		eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  bool		copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  ArithOpCode	op() const			{ return _op; }
  virtual bool	operIsConst() const = 0;
  virtual int	operConst() const = 0;
  virtual bool	doCopyPropagate(BB* bb, Use* u, PReg* d, bool repl);
  char*		opName() const;

  friend class NodeFactory;
};


class ArithRRNode : public ArithNode {  // reg op reg => reg
 protected:
  PReg*	_oper;
  Use*	_operUse;

  ArithRRNode(ArithOpCode o, PReg* s, PReg* o2, PReg* d);

 public:
  PReg*	operand() const				{ return _oper; }
  Node*	clone(PReg* from, PReg* to) const;
  bool	operIsConst() const;
  int	operConst() const;
  bool	doCopyPropagate(BB* bb, Use* u, PReg* d, bool replace);
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void	gen();
  void	apply(NodeVisitor* v)			{ v->anArithRRNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class FloatArithRRNode : public ArithRRNode {  // for untagged float operations
  FloatArithRRNode(ArithOpCode o, PReg* s, PReg* o2, PReg* d) : ArithRRNode(o, s, o2, d) { }
 public:
  bool	isAccessingFloats() const		{ return true; }
  bool	copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void	gen();
  void	apply(NodeVisitor* v)			{ v->aFloatArithRRNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class FloatUnaryArithNode : public ArithNode {
  // unary untagged float operation; src is an untagged float, dest is either another
  // untagged float or a floatOop
  FloatUnaryArithNode(ArithOpCode op, PReg* src, PReg* dst) : ArithNode(op, src, dst) {}
 public:
  bool	isAccessingFloats() const		{ return true; }
  bool	isCmpNode() const			{ return false; }
  bool	copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  bool	operIsConst() const			{ return false; }
  int	operConst() const			{ ShouldNotCallThis(); return 0; }
  void	gen();
  void	apply(NodeVisitor* v)			{ v->aFloatUnaryArithNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;
  friend class NodeFactory;
};


class ArithRCNode : public ArithNode {  // reg op const => reg
  // used to compare against non-oop constants (e.g. for markOop test)
  // DO NOT USE to add a reg and an oop constant -- use ArithRR + ConstPRegs for that
 protected:
  int	_oper;

  ArithRCNode(ArithOpCode o, PReg* s, int o2, PReg* d)
    : ArithNode(o, s, d) { _oper = o2; }

 public:
  int   operand() const				{ return _oper; }
  Node*	clone(PReg* from, PReg* to) const;
  void	gen();
  void	apply(NodeVisitor* v)			{ v->anArithRCNode(this); }
  bool	operIsConst() const			{ return true; }
  int	operConst() const			{ return _oper; }
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};

// node with >1 successor; supplies linking code (next(i) et al.) and some default behavior
class AbstractBranchNode : public NonTrivialNode {
  // next() is the fall-through case, next1() the taken branch
 public:
  virtual bool	canFail() const = 0;		// does node have a failure branch?
  virtual Node*	failureBranch() const		{ return next1(); }
  bool		endsBB() const 			{ return true; }
 protected:
  AbstractBranchNode()				{ _nxt = new GrowableArray<Node*>(EstimatedSuccessors); }
  void		removeFailureIfPossible();
  void		verify(bool verifySuccessors) const;

  // ---------- node linking code --------------
 private:
  enum { EstimatedSuccessors = 2 };		// most nodes have only 2 successors
 protected:
  GrowableArray<Node*>* _nxt;			/* elem 0 is next1 */
 public:

  Node* next1() const				{ return _nxt->length() ? _nxt->at(0) : NULL; }
  bool hasSingleSuccessor() const		{ return next1() == NULL; }
  int nSuccessors() const			{ return _nxt->length() + (_next ? 1 : 0); }
  Node* next() const				{ return _next; }
  Node* next(int i) const			{ return i == 0 ? _next : _nxt->at(i-1); }

  void removeMe();
  void removeNext(Node* n);

  void setNext1(Node* n) {
    assert(_nxt->length() == 0, "already set");
    _nxt->append(n);
  }

  void setNext(Node* n)				{ NonTrivialNode::setNext(n); }
  void setNext(int i, Node* n);
  void moveNext(Node* from, Node* to);
  bool isSuccessor(const Node* n) const;
};

class TArithRRNode : public AbstractBranchNode {
  // tagged arithmetic operation; next() is sucess case, next1()
  // is failure (leaving ORed operands in Temp1 for tag test)
 protected:
  ArithOpCode	_op;
  PReg*		_oper;
  Use*		_operUse;
  bool		_arg1IsInt;			// is _src a smi?
  bool		_arg2IsInt;			// is _oper a smi?
  ConstPReg*	_constResult;			// non-NULL if constant-folded

  TArithRRNode(ArithOpCode o, PReg* s, PReg* o2, PReg* d, bool a1, bool a2);

 public:
  ArithOpCode	op() const			{ return _op; }
  PReg*		operand() const			{ return _oper; }
  bool		arg1IsInt() const		{ return _arg1IsInt; }
  bool		arg2IsInt() const		{ return _arg2IsInt; }
  bool		canFail() const			{ return !(_arg1IsInt && _arg2IsInt); }
  bool		isTArithNode() const		{ return true; }
  bool		isAssignmentLike() const	{ return _constResult != NULL; }
  bool		canCopyPropagate() const	{ return true; }
  bool		canCopyPropagateOop() const	{ return true; }
  bool		hasSrc() const			{ return true; }
  bool		hasDest() const			{ return true; }

  bool		doesTypeTests()  const		{ return true; }
  bool		hasUnknownCode() const;
  void		collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const;
  void		assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n);

  void		makeUses(BB* bb);
  void		removeUses(BB* bb);
  void		markAllocated(int* use_count, int* def_count);
  Node*		clone(PReg* from, PReg* to) const;
  bool		copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  Node* 	likelySuccessor() const;
  Node*		uncommonSuccessor() const;
  void		gen();
  void		apply(NodeVisitor* v)		{ v->aTArithRRNode(this); }
  void		verify() const;
  char*		print_string(char* buf, bool printAddr = true) const;

 protected:
  bool		doCopyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);

  friend class NodeFactory;
};


class CallNode : public AbstractBranchNode {
  // next1 is the NLR branch (if there is one)
  // dest() is the return value
 protected:
  CallNode(MergeNode* n, GrowableArray<PReg*>* args, GrowableArray<PReg*>* exprs);

 public:
  GrowableArray<PReg*>*	exprStack;   	// current expr. stack for debugging info (NULL if not needed)
  GrowableArray<Use*>*	argUses;	// uses for args and receiver
  GrowableArray<PReg*>*	uplevelUsed;	// PRegs potentially uplevel-read during this call (NULL if not needed)
  GrowableArray<PReg*>*	uplevelDefd;	// PRegs potentially uplevel-written during this call (NULL if not needed)
  GrowableArray<Use*>*	uplevelUses;	// uses for uplevel-read names
  GrowableArray<Def*>*	uplevelDefs;	// defs for uplevel-written names
  GrowableArray<PReg*>*	args;		// args including receiver (at index 0, followed by first arg), or NULL
  int			nblocks;	// number of possibly live blocks at this point (for uplevel access computation)

  bool hasDest() const  	    	{ return true; }
  bool isCallNode() const  	    	{ return true; }
  bool canChangeDest() const		{ return false; }
  bool canBeEliminated() const 		{ return false; }
  virtual bool canInvokeDelta() const = 0;	// can call invoke Delta code?
  MergeNode* nlrTestPoint() const;
  Node* likelySuccessor() const;
  Node* uncommonSuccessor() const;
  bool copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void computeEscapingBlocks(GrowableArray<BlockPReg*>* ll);
  void makeUses(BB* bb);
  void removeUses(BB* bb);
  void markAllocated(int* use_count, int* def_count);
  SimpleBitVector trashedMask();
  void nlrCode();			// generate NLR code sequence
  void verify() const;

  friend class NodeFactory;
};


class SendNode : public CallNode {
 protected:
  LookupKey* _key;	  // lookup key (for selector)
  bool	     _superSend;  // is it a super send?
  SendInfo*  _info;	  // to set CompiledIC flags (counting, uninlinable, etc.)

  SendNode(
    LookupKey* key,
    MergeNode* nlrTestPoint,
    GrowableArray<PReg*>* args,
    GrowableArray<PReg*>* exprStk,
    bool superSend,
    SendInfo* info
  );

 public:
  bool	isSendNode() const		{ return true; }
  bool	isSuperSend() const		{ return _superSend; }
  bool	isCounting() const;
  bool  isUninlinable() const;
  bool	staticReceiver() const;
  bool	canInvokeDelta() const		{ return true; }
  bool  canFail() const			{ return false; }
  int	cost() const			{ return oopSize * 5; }	  // include IC + some param pushing
  PReg* recv() const;
  Node*	clone(PReg* from, PReg* to) const;
  void  computeEscapingBlocks(GrowableArray<BlockPReg*>* ll);
  void	gen();
  void  apply(NodeVisitor* v)		{ v->aSendNode(this); }
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class PrimNode: public CallNode {
 protected:
  primitive_desc*	_pdesc;

  PrimNode(primitive_desc* pdesc, MergeNode* nlrTestPoint, GrowableArray<PReg*>* args, GrowableArray<PReg*>* expr_stack);

 public:
  bool			canBeEliminated() const;
  bool 			canInvokeDelta() const;
  bool			canFail() const;
  primitive_desc*	pdesc() const		{ return _pdesc; }
  Node*			clone(PReg* from, PReg* to) const;
  void			computeEscapingBlocks(GrowableArray<BlockPReg*>* ll);
  void			eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void			gen();
  void			apply(NodeVisitor* v)	{ v->aPrimNode(this); }
  char*			print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class DLLNode: public CallNode {
 protected:
  symbolOop	_dll_name;
  symbolOop	_function_name;
  dll_func	_function;
  bool		_async;

  DLLNode(symbolOop dll_name, symbolOop function_name, dll_func function, bool async,
  	  MergeNode* nlrTestPoint, GrowableArray<PReg*>* args, GrowableArray<PReg*>* expr_stack);

 public:
  bool		canInvokeDelta() const;
  bool		canFail() const			{ return true; }
  int		nofArguments() const		{ return args == NULL ? 0 : args->length(); }
  symbolOop	dll_name() const		{ return _dll_name; }
  symbolOop	function_name() const		{ return _function_name; }
  dll_func	function() const		{ return _function; }
  bool		async() const			{ return _async; }
  void		computeEscapingBlocks(GrowableArray<BlockPReg*>* ll);
  Node*		clone(PReg* from, PReg* to) const;
  void		gen();
  void		apply(NodeVisitor* v)		{ v->aDLLNode(this); }
  char*		print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class InterruptCheckNode : public PrimNode {
 protected:
  static primitive_desc* _intrCheck;

  InterruptCheckNode(GrowableArray<PReg*>* exprs)
    : PrimNode(_intrCheck, NULL, NULL, exprs) {}

 public:
  Node*	clone(PReg* from, PReg* to) const;
  void	gen();
  void	apply(NodeVisitor* v)		{ Unimplemented(); }
  char*	print_string(char* buf, bool printAddr = true) const;

  friend void node_init();
  friend class NodeFactory;
};


// a LoopHeaderNode is inserted before every loop; usually it is a no-op
// for optimized integer loops, it does the pre-loop type tests
class LoopHeaderNode : public TrivialNode {
 protected:
  // info for integer loops
  bool	_integerLoop;					// integer loop? (if no: inst. vars below are not set)
  PReg* _loopVar;					// loop variable
  PReg* _lowerBound;					// lower bound
  PReg* _upperBound;					// upper bound (or NULL; mutually exclusive with boundArray)
  LoadOffsetNode* _upperLoad;				// loads array size that is the upper bound
  GrowableArray<AbstractArrayAtNode*>* _arrayAccesses;	// arrays indexed by loopVar
  // info for generic loops; all instance variables below this line are valid only after the loop optimization pass!
  GrowableArray<HoistedTypeTest*>* _tests;		// type tests hoisted out of loop
  bool _activated;					// gen() does nothing until activated
  LoopHeaderNode* _enclosingLoop;			// enclosing loop or NULL
  GrowableArray<LoopHeaderNode*>* _nestedLoops;		// nested loops (NULL if none)
  int  _nofCalls;					// number of non-inlined calls in loop (excluding unlikely code)
  GrowableArray<LoopRegCandidate*>* _registerCandidates;// candidates for reg. allocation within loop (best comes first); NULL if none

  LoopHeaderNode();

 public:
  bool	isLoopHeaderNode() const				{ return true; }
  bool  isActivated() const					{ return _activated; }
  bool  isInnerLoop() const					{ return _nestedLoops == NULL; }
  bool  isIntegerLoop() const					{ return _integerLoop; }
  PReg* loopVar() const						{ return _loopVar; }
  PReg* lowerBound() const					{ return _lowerBound; }
  PReg* upperBound() const					{ return _upperBound; }
  LoadOffsetNode*			upperLoad() const	{ return _upperLoad; }
  GrowableArray<AbstractArrayAtNode*>*	arrayAccesses() const	{ return _arrayAccesses; }
  GrowableArray<HoistedTypeTest*>*	tests() const		{ return _tests; }
  Node*	clone(PReg* from, PReg* to) const			{ ShouldNotCallThis(); return NULL; }
  int   nofCallsInLoop() const					{ return _nofCalls; }
  void  set_nofCallsInLoop(int n)				{ _nofCalls = n; }
  void  activate(PReg* loopVar, PReg* lowerBound, PReg* upperBound, LoadOffsetNode* upperLoad);
  void  activate();						// for non-integer loops
  void  addArray(AbstractArrayAtNode* n);
  LoopHeaderNode* enclosingLoop() const				{ return _enclosingLoop; }
  void  set_enclosingLoop(LoopHeaderNode* l);
  GrowableArray<LoopHeaderNode*>* nestedLoops() const   	{ return _nestedLoops; }
  void  addNestedLoop(LoopHeaderNode* l);
  GrowableArray<LoopRegCandidate*>* registerCandidates() const	{ return _registerCandidates; }
  void  addRegisterCandidate(LoopRegCandidate* c);
  void	gen();
  void	apply(NodeVisitor* v)					{ v->aLoopHeaderNode(this); }
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
  friend class CompiledLoop;
 protected:
   void generateTypeTests(Label& cont, Label& failure);
   void generateIntegerLoopTests(Label& cont, Label& failure);
   void generateArrayLoopTests(Label& cont, Label& failure);
   void generateIntegerLoopTest(PReg* p, Label& cont, Label& failure);
   void handleConstantTypeTest(ConstPReg* r, GrowableArray<klassOop>* klasses);
};


class BlockCreateNode : public PrimNode {
  // Initializes block (closure) location with closure if it's
  // not a memoized block; and with 0 otherwise
  // src is NULL (but non-NULL for subclass instances)
 protected:
  PReg*				_context;	// context or parameter/self that's copied into the block (or NULL)
  Use*				_contextUse;	// use of _context

  void materialize();				// generates code to materialize the block
  void copyIntoContexts(Register val, Register t1, Register t2);    // helper for above

  BlockCreateNode(BlockPReg* b, GrowableArray<PReg*>* expr_stack);

 public:
  bool	isBlockCreateNode() const	{ return true; }
  // block creation nodes are like assignments if memoized, so don't end BBs here
  bool	endsBB() const 			{ return !isMemoized() || NonTrivialNode::endsBB(); }
  BlockPReg* block() const 		{ assert(_dest->isBlockPReg(), "must be BlockPReg"); return (BlockPReg*)_dest; }
  bool	isMemoized() const		{ return block()->isMemoized(); }
  bool	hasConstantSrc() const 		{ return false; }
  bool	hasSrc() const 			{ return false; }
  int	cost() const 			{ return 2*oopSize; }    // hope it's memoized
  Node*	clone(PReg* from, PReg* to) const;
  bool	canBeEliminated() const 	{ return !dontEliminate; }
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	gen();
  void  apply(NodeVisitor* v)		{ v->aBlockCreateNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend void node_init();

  friend class NodeFactory;
};


class BlockMaterializeNode : public BlockCreateNode {
  // Materializes (creates) a block if it has not been materialized yet (no-op if not a memoized block)
  // src and dest are the BlockPReg
 protected:
  BlockMaterializeNode(BlockPReg* b, GrowableArray<PReg*>* expr_stack)
    : BlockCreateNode(b, expr_stack) { _src = _dest; }

 public:
  bool	endsBB() const 			{ return true; }
  bool	hasSrc() const 			{ return true; }
  int	cost() const 			{ return 5*oopSize; } // assume blk is memoized
  Node*	clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aBlockMaterializeNode(this); }
  char* print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class ContextCreateNode: public PrimNode {
  // src is parent context, dest is register holding created context
 protected:
  int				_nofTemps;			// no. of temps in context
  int				_contextSize;			// size of compiled context
  int				_contextNo;			// context number (index into compiler's contextList)
  GrowableArray<PReg*>*		_parentContexts;		// context chain above parent context (if any)
  GrowableArray<Use*>*		_parentContextUses;		// for _parentContexts

  ContextCreateNode(PReg* parent, PReg* context, int nofTemps, GrowableArray<PReg*>* expr_stack);
  ContextCreateNode(PReg* b, const ContextCreateNode* n, GrowableArray<PReg*>* expr_stack); // for cloning

 public:
  bool		hasSrc() const					{ return _src != NULL; }
  bool		canChangeDest() const 				{ return false; }
  bool 		canBeEliminated() const 			{ return false; }
  bool		isContextCreateNode() const			{ return true; }
  bool		canFail() const					{ return false; }
  PReg*		context() const					{ return _dest; }
  int		nofTemps() const				{ return _nofTemps; }
  int		sizeOfContext() const				{ return _contextSize; }
  void		set_sizeOfContext(int s )			{ _contextSize = s; }
  int		contextNo() const				{ return _contextNo; }
  void		set_contextNo(int s )				{ _contextNo = s; }

  Node*		clone(PReg* from, PReg* to) const;
  void		makeUses(BB* bb);
  void		removeUses(BB* bb);
  void		eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void		markAllocated(int* use_count, int* def_count);
  void		gen();
  void		apply(NodeVisitor* v)				{ v->aContextCreateNode(this); }
  void		verify() const;
  char*		print_string(char* buf, bool printAddr = true) const;

  friend void	node_init();

  friend class NodeFactory;
};


class ContextInitNode: public NonTrivialNode {
  // initializes contents of context; src is context (if _src == NULL context was eliminated)
 protected:
  GrowableArray<Expr*>*		_initializers;		// arguments/nil to be copied into context
  GrowableArray<Def*>*		_contentDefs;
  GrowableArray<Use*>*		_initializerUses;
  GrowableArray<BlockMaterializeNode*>*	_materializedBlocks;	// blocks that must be materialized
								// if this context is created

  ContextInitNode(ContextCreateNode* creator);
  ContextInitNode(PReg* b, const ContextInitNode* node);	// for cloning

 public:
  bool		hasSrc() const				{ return _src != NULL; }
  bool		hasDest() const	    			{ return false; }
  bool		isContextInitNode() const		{ return true; }
  bool 		canCopyPropagate() const 		{ return true; }
  bool 		canBeEliminated() const 		{ return false; }
  bool		wasEliminated() const			{ return _src == NULL; }
  GrowableArray<Expr*>* contents() const 		{ return scope()->contextTemporaries(); }
  int		nofTemps() const			{ return _initializers->length(); }
  Expr*		contextTemp(int i) const		{ return contents()->at(i); }
  Expr*		initialValue(int i) const		{ return _initializers->at(i); }
  void		addBlockMaterializer(BlockMaterializeNode* n);
  ContextCreateNode* creator() const;
  void		notifyNoContext();
  bool		hasNoContext() const			{ return _src == NULL; }

  void		initialize(int no, Expr* expr);		// to copy something into the context right after creation
  int		positionOfContextTemp(int i) const;	// position of ith context temp in compiled context
  Node*		clone(PReg* from, PReg* to) const;
  void		makeUses(BB* bb);
  void		removeUses(BB* bb);
  void		eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  bool 		copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void		markAllocated(int* use_count, int* def_count);
  void		computeEscapingBlocks(GrowableArray<BlockPReg*>* l);
  void		gen();
  void		apply(NodeVisitor* v)			{ v->aContextInitNode(this); }
  void		verify() const;
  char*		print_string(char* buf, bool printAddr = true) const;

  friend void	node_init();

  friend class NodeFactory;
};


class ContextZapNode: public NonTrivialNode {
// placeholder for context zap code; zapping is only needed if the node isActive().
 private:
  ContextZapNode(PReg* context)			{ _src = context; assert(_src, "src is NULL"); }

 public:
  bool	isActive() const			{ return scope()->needsContextZapping(); }
  bool	isContextZapNode() const		{ return true; }
  bool	hasSrc() const				{ return true; }
  bool  shouldCopyWhenSplitting() const		{ return true; }
  PReg*	context() const				{ return _src; }

  Node*	clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	gen();
  void	apply(NodeVisitor* v)			{ v->aContextZapNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class NLRTestNode: public AbstractBranchNode {
  // placeholder for NLR test code: tests if NLR reached home scope, zaps context (if needed), then
  // returns from method (if home scope found) or continues the NLR
  // next() is the "continue NLR" branch, next1() the "found home" branch
 protected:
  NLRTestNode(int bci);

 public:
  bool	isNLRTestNode() const			{ return true; }
  bool	canChangeDest() const 			{ return false; }
  bool	canBeEliminated() const 		{ return false; }
  bool	canFail() const				{ return false; }
  void	fixup();				// connect next() and next1() to sender scopes

  Node*	clone(PReg* from, PReg* to) const;
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  // void eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void	markAllocated(int* use_count, int* def_count) {};
  Node*	likelySuccessor() const;
  Node* uncommonSuccessor() const;
  void	gen();
  void	apply(NodeVisitor* v)			{ v->aNLRTestNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class BranchNode : public AbstractBranchNode {
  // next() is the target of an untaken branch, next1() the taken
  // conditional branches expect CCs to be set by previous node (backend specific?)
  // usually after a node that sets CCs
 protected:
  BranchOpCode	_op;				// branch untaken is likely
  bool		_taken_is_uncommon;		// true if branch taken is uncommon

  BranchNode(BranchOpCode op, bool taken_is_uncommon) {
    _op = op;
    _taken_is_uncommon = taken_is_uncommon;
  }

 public:
  BranchOpCode	op() const			{ return _op; }
  bool		isBranchNode() const		{ return true; }
  bool		canFail() const			{ return false; }
  void		eliminateBranch(int op1, int op2, int res);
  void		eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void		markAllocated(int* use_count, int* def_count) { Unused(use_count); Unused(def_count); }
  Node*		clone(PReg* from, PReg* to) const;
  Node*		likelySuccessor() const;
  Node*		uncommonSuccessor() const;
  void		gen();
  void		apply(NodeVisitor* v)		{ v->aBranchNode(this); }
  char*		print_string(char* buf, bool printAddr = true) const;
  void		verify()			{ AbstractBranchNode::verify(false); }

  friend class NodeFactory;
};


class TypeTestNode : public AbstractBranchNode {
  // n-way map test; next(i) is the ith class [i=1..n], next() is the
  // "otherwise" branch
  // _src is the register containing the receiver
 protected:
  GrowableArray<klassOop>* _classes; 	// classes to test for
  bool _hasUnknown;	    		// can recv be anything? (if false, recv class
  	    	    	    		// guaranteed to be in classes list)

  bool needsKlassLoad() const;		// does test need object's klass?
  TypeTestNode(PReg* r, GrowableArray<klassOop>* classes, bool hasUnknown);

 public:
  GrowableArray<klassOop>* classes() const	{ return _classes; }
  bool	hasUnknown() const			{ return _hasUnknown; }
  bool	canFail() const				{ return _hasUnknown; }
  Node*	failureBranch() const			{ return next(); }
  void	setUnknown(bool u) 			{ _hasUnknown = u; }
  bool	isTypeTestNode() const    		{ return true; }
  bool	hasSrc() const 				{ return true; }
  bool 	canCopyPropagate() const 		{ return true; }
  bool	canCopyPropagateOop() const 		{ return true; }
  int	cost() const	    	      		{ return 2 * oopSize * (_classes->length() + needsKlassLoad() ? 1 : 0); }
  Node*	smiCase() const;			// the continuation for the smi case, NULL if there is none

  bool	doesTypeTests()  const			{ return true; }
  bool	hasUnknownCode() const;			// is there code (send) for unknown case?
  void	collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const;
  void  assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n);

  Node*	clone(PReg* from, PReg* to) const;
  bool	copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  void	eliminate(BB* bb, PReg* r, ConstPReg* c, klassOop m);
  void	eliminateUnnecessary(klassOop m);
  Node*	likelySuccessor() const;
  Node* uncommonSuccessor() const;
  void	gen();
  void	apply(NodeVisitor* v)			{ v->aTypeTestNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class AbstractArrayAtNode : public AbstractBranchNode {
  // array access: test index type & range, load element
  // next() is the success case, next1() the failure
 protected:
  	    	    	    	// _src is the array, _dest the result
  PReg*	_arg;	    	    	// index value
  Use*	_argUse;
  bool	_intArg;    	    	// need not test for int if true
  PReg*	_error;  	    	// where to move the error string
  Def*	_errorDef;
  bool  _needBoundsCheck;	// need array bounds check?
  int	_dataOffset;	    	// where start of array is (oop offset)
  int	_sizeOffset;	    	// where size of array is (oop offset)

   AbstractArrayAtNode(PReg* r, PReg* idx, bool ia, PReg* res, PReg* _err, int dataOffset, int sizeOffset) {
     _src = r; _arg = idx; _intArg = ia; _dest = res; _error = _err;
     _dataOffset = dataOffset; _sizeOffset = sizeOffset;
     dontEliminate = true; _needBoundsCheck = true;
   }

 public:
  bool	hasSrc() const 			{ return true; }
  bool	hasDest() const 		{ return _dest != NULL; }
  bool  canFail() const = 0;
  int	cost() const	     		{ return 20 + (_intArg ? 0 : 12); } // fix this
  bool	canCopyPropagate() const	{ return true; }
  int   sizeOffset() const		{ return _sizeOffset; }
  bool	copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);

  bool	doesTypeTests() const		{ return true; }
  bool	needsBoundsCheck() const	{ return _needBoundsCheck; }
  bool	hasUnknownCode() const;
  void	collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const;
  void  assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n);
  void  assert_in_bounds(PReg* r, LoopHeaderNode* n);

  void	makeUses(BB* bb);
  void	removeUses(BB* bb);
  void	markAllocated(int* use_count, int* def_count);
  void	eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  Node* likelySuccessor() const;
  Node* uncommonSuccessor() const;

  friend class NodeFactory;
};


class ArrayAtNode : public AbstractArrayAtNode {
 public:
  enum AccessType {
    byte_at,				// corresponds to primitiveIndexedByteAt:ifFail:
    double_byte_at,			// corresponds to primitiveIndexedDoubleByteAt:ifFail:
    character_at,			// corresponds to primitiveIndexedDoubleByteCharacterAt:ifFail:
    object_at				// corresponds to primitiveIndexedObjectAt:ifFail:
  };

 protected:
  AccessType	_access_type;

  ArrayAtNode(
    AccessType	access_type,		// specifies the operation
    PReg*	array,			// holds the array
    PReg*	index,			// holds the index
    bool	smiIndex,		// true if index is known to be a smi, false otherwise
    PReg*	result,			// where the result is stored
    PReg*	error,			// where the error symbol is stored if the operation fails
    int		data_offset,		// data offset in oops relative to array
    int		length_offset		// array length offset in oops relative to array
  );

 public:
  AccessType	access_type() const	{ return _access_type; }
  PReg*		array() const		{ return _src; }
  PReg*		index() const		{ return _arg; }
  PReg*		error() const		{ return _error; }
  bool		index_is_smi() const	{ return _intArg; }
  bool		index_needs_bounds_check() const { return _needBoundsCheck; }
  bool		canFail() const		{ return !index_is_smi() || index_needs_bounds_check(); }
  int		data_word_offset() const{ return _dataOffset; }
  int		size_word_offset() const{ return _sizeOffset; }
  Node*		clone(PReg* from, PReg* to) const;
  char*		print_string(char* buf, bool printAddr = true) const;
  void		gen();
  void		apply(NodeVisitor* v)	{ v->anArrayAtNode(this); }

  friend class	NodeFactory;
};


class AbstractArrayAtPutNode : public AbstractArrayAtNode {
  // array store: test index type & range, store element
  // next() is the success case, next1() the failure
 protected:
  PReg* elem;
  Use* elemUse;

   AbstractArrayAtPutNode(PReg* arr, PReg* idx, bool ia, PReg* el, PReg* res, PReg* _err, int doff, int soff)
    : AbstractArrayAtNode(arr, idx, ia, res, _err, doff, soff) { elem = el; }

 public:
  bool copyPropagate(BB* bb, Use* u, PReg* d, bool replace = false);
  bool canCopyPropagateOop() const 	{ return true; }
  void makeUses(BB* bb);
  void removeUses(BB* bb);
  void markAllocated(int* use_count, int* def_count);

  friend class NodeFactory;
};


class ArrayAtPutNode : public AbstractArrayAtPutNode {
 public:
  enum AccessType {
    byte_at_put,			// corresponds to primitiveIndexedByteAt:put:ifFail:
    double_byte_at_put,			// corresponds to primitiveIndexedDoubleByteAt:put:ifFail:
    object_at_put			// corresponds to primitiveIndexedObjectAt:put:ifFail:
  };
  static bool stores_smi_elements(AccessType t) { return t != object_at_put; }

 protected:
  AccessType	_access_type;
  bool		_needs_store_check;
  bool		_smi_element;
  bool		_needs_element_range_check;

  ArrayAtPutNode(
    AccessType	access_type,		// specifies the operation
    PReg*	array,			// holds the array
    PReg*	index,			// holds the index
    bool	smi_index,		// true if index is known to be a smi, false otherwise
    PReg*	element,		// holds the element
    bool	smi_element,		// true if element is known to be a smi, false otherwise
    PReg*	result,			// where the result is stored
    PReg*	error,			// where the error symbol is stored if the operation fails
    int		data_offset,		// data offset in oops relative to array
    int		length_offset,		// array length offset in oops relative to array
    bool	needs_store_check	// indicates whether a store check is necessary or not
  );

 public:
  AccessType	access_type() const	{ return _access_type; }
  PReg*		array() const		{ return _src; }
  PReg*		index() const		{ return _arg; }
  PReg*		element() const		{ return elem; }
  PReg*		error() const		{ return _error; }
  bool		index_is_smi() const	{ return _intArg; }
  bool		element_is_smi() const	{ return _smi_element; }
  bool		index_needs_bounds_check() const { return _needBoundsCheck; }
  bool		element_needs_range_check() const { return _needs_element_range_check; }
  bool		canFail() const		{ return !index_is_smi() || index_needs_bounds_check() ||
						  (access_type() != object_at_put && (!element_is_smi() || element_needs_range_check())); }
  bool		needs_store_check() const { return _needs_store_check; }
  int		data_word_offset() const{ return _dataOffset; }
  int		size_word_offset() const{ return _sizeOffset; }
  Node*		clone(PReg* from, PReg* to) const;

  void		collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const;
  void		assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n);

  void		computeEscapingBlocks(GrowableArray<BlockPReg*>* l);
  char*		print_string(char* buf, bool printAddr = true) const;
  void		gen();
  void		apply(NodeVisitor* v)	{ v->anArrayAtPutNode(this); }

  friend class NodeFactory;
};


class InlinedPrimitiveNode : public AbstractBranchNode {
 public:
  enum Operation {
    obj_klass,				// corresponds to primitiveClass
    obj_hash,				// corresponds to primitiveHash
    proxy_byte_at,			// corresponds to primitiveProxyByteAt:ifFail:
    proxy_byte_at_put,			// corresponds to primitiveProxyByteAt:put:ifFail:
  };

 private:
  Operation	_op;
  // _src is	_recv;			// receiver or NULL
  PReg*		_arg1;			// 1st argument or NULL
  PReg*		_arg2;			// 2nd argument or NULL
  PReg*		_error;			// primitive error or NULL if primitive can't fail
  Use*		_arg1_use;
  Use*		_arg2_use;
  Def*		_error_def;
  bool		_arg1_is_smi;		// true if 1st argument is known to be a smi
  bool		_arg2_is_smi;		// true if 2nd argument is known to be a smi

  InlinedPrimitiveNode(
    Operation op,
    PReg* result,
    PReg* error,
    PReg* recv,
    PReg* arg1, bool arg1_is_smi,
    PReg* arg2, bool arg2_is_smi
  );

 public:
  Operation	op() const		{ return _op; }
  PReg*         arg1() const		{ return _arg1; }
  PReg*		arg2() const		{ return _arg2; }
  PReg*		error() const		{ return _error; }
  bool		arg1_is_smi() const	{ return _arg1_is_smi; }
  bool		arg2_is_smi() const	{ return _arg2_is_smi; }
  bool		hasSrc() const		{ return _src != NULL; }
  bool		hasDest() const		{ return _dest != NULL; }
  bool		canFail() const;
  void		makeUses(BB* bb);
  void		removeUses(BB* bb);
  void		markAllocated(int* use_count, int* def_count);
  bool		canBeEliminated() const;
  void		eliminate(BB* bb, PReg* r, bool removing = false, bool cp = false);
  bool		canCopyPropagate() const	{ return false; }
  bool		canCopyPropagateOop() const	{ return false; }
  bool		copyPropagate(BB* bb, Use* u, PReg* d, bool replace);
  Node*		likelySuccessor() const;
  Node*		uncommonSuccessor() const;
  void		gen();
  void		apply(NodeVisitor* v)		{ v->anInlinedPrimitiveNode(this); }
  void		verify() const;
  char*		print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class UncommonNode : public NonTrivialNode {
  GrowableArray<PReg*>* exprStack;

 protected:
  UncommonNode(GrowableArray<PReg*>* e, int bci);

 public:
  bool	isUncommonNode() const    	{ return true; }
  int	cost() const 			{ return 4; } // fix this
  bool	isExitNode() const		{ return true; }
  bool	isEndsBB() const		{ return true; }
  Node*	clone(PReg* from, PReg* to) const;
  void	markAllocated(int* use_count, int* def_count) { Unused(use_count); Unused(def_count); }
  void	gen();
  void	apply(NodeVisitor* v)		{ v->anUncommonNode(this); }
  void	verify() const;
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class FixedCodeNode : public TrivialNode {
 public:
  enum FixedCodeKind {
    dead_end,				// dead-end marker (for compiler debugging)
    inc_counter				// increment invocation counter
  };
 protected:
  const FixedCodeKind _kind;
  FixedCodeNode(FixedCodeKind k) : _kind(k) {}

 public:
  bool	isExitNode() const		{ return _kind == dead_end; }
  bool	isDeadEndNode() const    	{ return _kind == dead_end; }
  FixedCodeKind kind() const		{ return _kind; }
  Node*	clone(PReg* from, PReg* to) const;
  void	gen();
  void	apply(NodeVisitor* v)		{ v->aFixedCodeNode(this); }
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class NopNode : public TrivialNode {
 protected:
  NopNode() {}

 public:
  bool	isNopNode() const		{ return true; }
  Node*	clone(PReg* from, PReg* to) const;
  void  apply(NodeVisitor* v)		{ v->aNopNode(this); }
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


class CommentNode : public TrivialNode {
 protected:
  CommentNode(char* s);

 public:
  char*	comment;
  bool	isCommentNode() const		{ return true; }
  Node*	clone(PReg* from, PReg* to) const;
  void  apply(NodeVisitor* v)		{ v->aCommentNode(this); }
  char*	print_string(char* buf, bool printAddr = true) const;

  friend class NodeFactory;
};


// NodeFactory is used to create new nodes.

class NodeFactory : AllStatic {
 public:
  static int cumulCost;					// cumulative cost of all nodes generated so far

  static void			registerNode(Node* n)	{ cumulCost += n->cost(); }

  static PrologueNode*		new_PrologueNode	(LookupKey* key, int nofArgs, int nofTemps);

  static LoadOffsetNode*	new_LoadOffsetNode	(PReg* dst, PReg* base, int offs, bool isArray);
  static LoadUplevelNode*	new_LoadUplevelNode	(PReg* dst, PReg* context0, int nofLevels, int offset, symbolOop name);
  static LoadIntNode*		new_LoadIntNode		(PReg* dst, int value);

  static StoreOffsetNode*	new_StoreOffsetNode	(PReg* src, PReg* base, int offs, bool needStoreCheck);
  static StoreUplevelNode*	new_StoreUplevelNode	(PReg* src, PReg* context0, int nofLevels, int offset, symbolOop name, bool needStoreCheck);
  static AssignNode*		new_AssignNode		(PReg* src, PReg* dst);

  static ReturnNode*		new_ReturnNode		(PReg* res, int bci);
  static InlinedReturnNode*	new_InlinedReturnNode	(int bci, PReg* src, PReg* dst);
  static NLRSetupNode*		new_NLRSetupNode	(PReg* result, int bci);
  static NLRContinuationNode*	new_NLRContinuationNode	(int bci);
  static NLRTestNode*		new_NLRTestNode		(int bci);

  static ArithRRNode*		new_ArithRRNode		(PReg* dst, PReg* src, ArithOpCode op, PReg* o2);
  static ArithRCNode*		new_ArithRCNode		(PReg* dst, PReg* src, ArithOpCode op, int   o2);
  static TArithRRNode*		new_TArithRRNode	(PReg* dst, PReg* src, ArithOpCode op, PReg* o2, bool a1, bool a2);
  static FloatArithRRNode*	new_FloatArithRRNode	(PReg* dst, PReg* src, ArithOpCode op, PReg* o2);
  static FloatUnaryArithNode*	new_FloatUnaryArithNode	(PReg* dst, PReg* src, ArithOpCode op);

  static MergeNode*		new_MergeNode		(Node* prev1, Node* prev2);
  static MergeNode*		new_MergeNode		(int bci);

  static SendNode*		new_SendNode		(LookupKey* key, MergeNode* nlrTestPoint,
  							 GrowableArray<PReg*>* args, GrowableArray<PReg*>* expr_stack,
							 bool superSend, SendInfo* info);
  static PrimNode*		new_PrimNode		(primitive_desc* pdesc, MergeNode* nlrTestPoint,
  							 GrowableArray<PReg*>* args, GrowableArray<PReg*>* expr_stack);
  static DLLNode*		new_DLLNode		(symbolOop dll_name, symbolOop function_name, dll_func function, bool async,
  							 MergeNode* nlrTestPoint, GrowableArray<PReg*>* args, GrowableArray<PReg*>* expr_stack);

  static InterruptCheckNode*	new_InterruptCheckNode	(GrowableArray<PReg*>* expr_stack);
  static LoopHeaderNode*	new_LoopHeaderNode	();

  static BlockCreateNode*	new_BlockCreateNode	(BlockPReg* b, GrowableArray<PReg*>* expr_stack);
  static BlockMaterializeNode*	new_BlockMaterializeNode(BlockPReg* b, GrowableArray<PReg*>* expr_stack);

  static ContextCreateNode*	new_ContextCreateNode	(PReg* parent, PReg* context, int nofTemps, GrowableArray<PReg*>* expr_stack);
  static ContextCreateNode*	new_ContextCreateNode	(PReg* b, const ContextCreateNode* n, GrowableArray<PReg*>* expr_stack);
  static ContextInitNode*	new_ContextInitNode	(ContextCreateNode* creator);
  static ContextInitNode*	new_ContextInitNode	(PReg* b, const ContextInitNode* n);
  static ContextZapNode*	new_ContextZapNode	(PReg* context);

  static BranchNode*		new_BranchNode		(BranchOpCode op, bool taken_is_uncommon = false);

  static TypeTestNode*		new_TypeTestNode	(PReg* recv, GrowableArray<klassOop>* classes, bool hasUnknown);

  static ArrayAtNode*		new_ArrayAtNode		(ArrayAtNode::AccessType access_type, PReg* array, PReg* index, bool smiIndex,
							 PReg* result, PReg* error, int data_offset, int length_offset);

  static ArrayAtPutNode*	new_ArrayAtPutNode	(ArrayAtPutNode::AccessType access_type, PReg* array, PReg* index, bool smi_index,
							 PReg* element, bool smi_element, PReg* result, PReg* error, int data_offset, int length_offset,
							 bool needs_store_check);

  static InlinedPrimitiveNode*  new_InlinedPrimitiveNode(InlinedPrimitiveNode::Operation op, PReg* result, PReg* error = NULL,
                                                         PReg* recv = NULL,
                                                         PReg* arg1 = NULL, bool arg1_is_smi = false,
							 PReg* arg2 = NULL, bool arg2_is_smi = false);

  static UncommonNode*		new_UncommonNode	(GrowableArray<PReg*>* exprStack, int bci);
  static FixedCodeNode*		new_FixedCodeNode	(FixedCodeNode::FixedCodeKind k);
  static NopNode*		new_NopNode		();
  static CommentNode*		new_CommentNode		(char* comment);
};

#ifdef DEBUG
void printNodes(Node* n);   // print n and its successors
#endif

# endif
