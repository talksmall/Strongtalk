/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.39 $ */
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

// BBs (Basic Blocks) are used by the Compiler to perform local optimizations 
// and code generation

# ifdef DELTA_COMPILER

class NodeVisitor;

class BB: public PrintableResourceObj {
 protected:
  bool _visited;

 public:			// was protected: originally
  Node	*first, *last;
  int16	nnodes;			// number of nodes in this BB
 protected:
  int16	_id;			// unique BB id
  int16	_loopDepth;		// the loop nesting level
  int16	_genCount;		// code already generated?

 public:	
  BBDUTable  duInfo;		// defs/uses of PRegs
  static int genCounter;	// to enumerate BBs in code-generation order

 public:
  BB(Node* f, Node* l, int n) { init(f, l, n); _visited = false; }

  BB*  after_visit()			{ _visited = true;  return this; }
  BB*  before_visit()			{ _visited = false; return this; }
  bool visited() const			{ return _visited; }

  // successor/predecessor functionality
  bool hasSingleSuccessor() const;
  bool hasSinglePredecessor() const;
  int nPredecessors() const;
  int nSuccessors() const;
  bool isPredecessor(const BB* n) const;
  bool isSuccessor(const BB* n) const;
  BB* next() const;
  BB* next1() const;
  BB* next(int i) const;
  BB* firstPrev() const;
  BB* prev(int i) const;
   
  int  id() const 		{ return this == NULL ? -1 : _id; }
  int  loopDepth() const 	{ return _loopDepth; }
  void setGenCount()		{ _genCount = genCounter++; }
  int  genCount() const 	{ return _genCount; }

  void localCopyPropagate();
  void bruteForceCopyPropagate();

  void makeUses();
  Use* addUse(NonTrivialNode* n, PReg* r, bool soft = false);
  Def* addDef(NonTrivialNode* n, PReg* r);
  void remove(Node* n);				// remove node
  void addAfter(Node* prev, Node* newNode);	// add node after prev
  void localAlloc(GrowableArray<BitVector*>* hardwired, 
    		  GrowableArray<PReg*>* localRegs,
		  GrowableArray<BitVector*>* lives);
 protected:
  void init(Node* f, Node* l, int n);
  void slowLocalAlloc(GrowableArray<BitVector*>* hardwired, 
    		      GrowableArray<PReg*>* localRegs,
		      GrowableArray<BitVector*>* lives);
  void doAlloc(PReg* r, Location l);
 public:
  void computeEscapingBlocks(GrowableArray<BlockPReg*>* l);

  void apply(NodeVisitor* v);
  bool verifyLabels();

  bool contains(const Node* n) const;
  void verify();
  void dfs(GrowableArray<BB*>* list, int depth);
  void print_short();
  void print_code(bool suppressTrivial);
  void print();

 protected:
  int addUDHelper(PReg* r);
  void renumber();
  friend class BBIterator;
};


typedef void (*BBDoFn)(BB* b);

  
class BBIterator: public PrintableResourceObj {
  Node* _first;						// first node 
 public:						// was protected: originally
  GrowableArray<BB*>* bbTable;				// BBs sorted in topological order
  int bbCount;						// number of BBs
  GrowableArray<PReg*>* pregTable;			// holds all PRegs; indexed by their id
  GrowableArray<PReg*>* globals;			// holds globally allocated PRegs; indexed by
    							// their num()
  bool usesBuilt; 	    				// true after uses have been built
  bool blocksBuilt; 	    				// true after basic blocks have been built
  GrowableArray<BlockPReg*>* exposedBlks;		// list of escaping blocks

 public:
  BBIterator() {
    bbTable = NULL; bbCount = 0; usesBuilt = blocksBuilt = false;
  }

  void build(Node* first);				// build bbTable

 protected:
  void buildBBs();
  void buildTable();

  static BB* bb_for(Node* n);
  void add_BBs_to_list(GrowableArray<BB*>& list, GrowableArray<BB*>& work);

 public:
  bool isSequential(int curr, int next) const;		// are the two BB indices sequential in bbTable order?
  bool isSequentialCode(BB* curr, BB* next) const;	// are the two BBs sequential in codeGen order?
  GrowableArray<BB*>* code_generation_order();		// list of BBs in code generation order

  void clear();   	    	    			// clear bbTable
  void apply(BBDoFn f);
    
  void makeUses();
  void eliminateUnneededResults();
  void computeEscapingBlocks();
  void computeUplevelAccesses();
  void localAlloc();
  void localCopyPropagate();
  void globalCopyPropagate();
  void bruteForceCopyPropagate();
  void gen(Node* first);
  void apply(NodeVisitor* v);
  bool verifyLabels();

  void print();
  void print_code(bool suppressTrivial);
  void verify();
};

extern BBIterator* bbIterator;


// NodeVisitor

class Node;
class PrologueNode;
class LoadIntNode;
class LoadOffsetNode;
class LoadUplevelNode;
class AssignNode;
class StoreOffsetNode;
class StoreUplevelNode;
class ArithRRNode;
class ArithRCNode;
class FloatArithRRNode;
class FloatUnaryArithNode;
class TArithRRNode;
class ContextCreateNode;
class ContextInitNode;
class ContextZapNode;
class BlockCreateNode;
class BlockMaterializeNode;
class SendNode;
class PrimNode;
class DLLNode;
class LoopHeaderNode;
class ReturnNode;
class NLRSetupNode;
class InlinedReturnNode;
class NLRContinuationNode;
class BranchNode;
class TypeTestNode;
class NLRTestNode;
class MergeNode;
class ArrayAtNode;
class ArrayAtPutNode;
class InlinedPrimitiveNode;
class UncommonNode;
class FixedCodeNode;
class NopNode;
class CommentNode;


class NodeVisitor: public ResourceObj {
 public:
  // Basic blocks
  virtual void beginOfBasicBlock(Node* node) = 0;	// called for the first node in a BB, before the node's method is called
  virtual void endOfBasicBlock(Node* node) = 0;		// called for the last node in a BB, after the node's method was called
  
 public:
  // For all nodes
  virtual void beginOfNode(Node* node) = 0;		// called for each node, before the node's method is called
  virtual void endOfNode(Node* node) = 0;		// called for each node, after the node's method was called

 public:
  // Individual nodes
  virtual void aPrologueNode(PrologueNode* node) = 0;

  virtual void aLoadIntNode(LoadIntNode* node) = 0;
  virtual void aLoadOffsetNode(LoadOffsetNode* node) = 0;
  virtual void aLoadUplevelNode(LoadUplevelNode* node) = 0;

  virtual void anAssignNode(AssignNode* node) = 0;
  virtual void aStoreOffsetNode(StoreOffsetNode* node) = 0;
  virtual void aStoreUplevelNode(StoreUplevelNode* node) = 0;
  
  virtual void anArithRRNode(ArithRRNode* node) = 0;
  virtual void aFloatArithRRNode(FloatArithRRNode* node) = 0;
  virtual void aFloatUnaryArithNode(FloatUnaryArithNode* node) = 0;
  virtual void anArithRCNode(ArithRCNode* node) = 0;
  virtual void aTArithRRNode(TArithRRNode* node) = 0;
  
  virtual void aContextCreateNode(ContextCreateNode* node) = 0;
  virtual void aContextInitNode(ContextInitNode* node) = 0;
  virtual void aContextZapNode(ContextZapNode* node) = 0;

  virtual void aBlockCreateNode(BlockCreateNode* node) = 0;
  virtual void aBlockMaterializeNode(BlockMaterializeNode* node) = 0;
  
  virtual void aSendNode(SendNode* node) = 0;
  virtual void aPrimNode(PrimNode* node) = 0;
  virtual void aDLLNode(DLLNode* node) = 0;

  virtual void aLoopHeaderNode(LoopHeaderNode* node) = 0;

  virtual void aReturnNode(ReturnNode* node) = 0;
  virtual void aNLRSetupNode(NLRSetupNode* node) = 0;
  virtual void anInlinedReturnNode(InlinedReturnNode* node) = 0;
  virtual void aNLRContinuationNode(NLRContinuationNode* node) = 0;
  
  virtual void aBranchNode(BranchNode* node) = 0;
  virtual void aTypeTestNode(TypeTestNode* node) = 0;
  virtual void aNLRTestNode(NLRTestNode* node) = 0;
  virtual void aMergeNode(MergeNode* node) = 0;
  
  virtual void anArrayAtNode(ArrayAtNode* node) = 0;
  virtual void anArrayAtPutNode(ArrayAtPutNode* node) = 0;

  virtual void anInlinedPrimitiveNode(InlinedPrimitiveNode* node) = 0;
  virtual void anUncommonNode(UncommonNode* node) = 0;
  virtual void aFixedCodeNode(FixedCodeNode* node) = 0;

  virtual void aNopNode(NopNode* node) = 0;
  virtual void aCommentNode(CommentNode* node) = 0;
};


# endif
