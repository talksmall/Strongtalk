/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.155 $ */
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

# include "incls/_precompiled.incl"

# ifdef DELTA_COMPILER
# include "incls/_node.cpp.incl"

int BasicNode::currentID;
int BasicNode::currentCommentID;
int BasicNode::lastBCI;
ScopeInfo BasicNode::lastScopeInfo;
primitive_desc* InterruptCheckNode::_intrCheck;

int NodeFactory::cumulCost;


// Implementation of NodeFactory

PrologueNode* NodeFactory::new_PrologueNode(LookupKey* key, int nofArgs, int nofTemps) {
  PrologueNode* res = new PrologueNode(key, nofArgs, nofTemps);
  registerNode(res);
  return res;
}

LoadOffsetNode* NodeFactory::new_LoadOffsetNode(PReg* dst, PReg* base, int offs, bool isArray) {
  LoadOffsetNode* res = new LoadOffsetNode(dst, base, offs, isArray);
  registerNode(res);
  return res;
}

LoadUplevelNode* NodeFactory::new_LoadUplevelNode(PReg* dst, PReg* context0, int nofLevels, int offset, symbolOop name) {
  LoadUplevelNode* res = new LoadUplevelNode(dst, context0, nofLevels, offset, name);
  registerNode(res);
  return res;
}

LoadIntNode* NodeFactory::new_LoadIntNode(PReg* dst, int value) {
  LoadIntNode* res = new LoadIntNode(dst, value);
  registerNode(res);
  return res;
}
  
StoreOffsetNode* NodeFactory::new_StoreOffsetNode(PReg* src, PReg* base, int offs, bool needStoreCheck) {
  StoreOffsetNode* res = new StoreOffsetNode(src, base, offs, needStoreCheck);
  registerNode(res);
  return res;
}

StoreUplevelNode* NodeFactory::new_StoreUplevelNode(PReg* src, PReg* context0, int nofLevels, int offset, symbolOop name, bool needStoreCheck) {
  StoreUplevelNode* res = new StoreUplevelNode(src, context0, nofLevels, offset, name, needStoreCheck);
  registerNode(res);
  return res;
}

AssignNode* NodeFactory::new_AssignNode(PReg* src, PReg* dst) {
  AssignNode* res = new AssignNode(src, dst);
  registerNode(res);
  return res;
}

ReturnNode* NodeFactory::new_ReturnNode(PReg* result, int bci) {
  ReturnNode* res = new ReturnNode(result, bci);
  registerNode(res);
  return res;
}

InlinedReturnNode* NodeFactory::new_InlinedReturnNode(int bci, PReg* src, PReg* dst) {
  InlinedReturnNode* res = new InlinedReturnNode(bci, src, dst);
  registerNode(res);
  return res;
}

NLRSetupNode* NodeFactory::new_NLRSetupNode(PReg* result, int bci) {
  NLRSetupNode* res = new NLRSetupNode(result, bci);
  registerNode(res);
  return res;
}

NLRContinuationNode* NodeFactory::new_NLRContinuationNode(int bci) {
  InlinedScope* scope = theCompiler->currentScope();
  PReg* reg = new PReg(scope, NLRResultLoc, false, false);
  NLRContinuationNode* res = new NLRContinuationNode(bci, reg, reg);
  registerNode(res);
  return res;
}

NLRTestNode* NodeFactory::new_NLRTestNode(int bci) {
  NLRTestNode* res = new NLRTestNode(bci);
  registerNode(res);
  theCompiler->nlrTestPoints->append(res);
  return res;
}

ArithRRNode* NodeFactory::new_ArithRRNode(PReg* dst, PReg* src, ArithOpCode op, PReg* o2) {
  ArithRRNode* res = new ArithRRNode(op, src, o2, dst);
  registerNode(res);
  return res;
}

ArithRCNode* NodeFactory::new_ArithRCNode(PReg* dst, PReg* src, ArithOpCode op, int   o2) {
  ArithRCNode* res = new ArithRCNode(op, src, o2, dst);
  registerNode(res);
  return res;
}

TArithRRNode* NodeFactory::new_TArithRRNode(PReg* dst, PReg* src, ArithOpCode op, PReg* o2, bool a1, bool a2) {
  TArithRRNode* res = new TArithRRNode(op, src, o2, dst, a1, a2);
  registerNode(res);
  return res;
}

FloatArithRRNode* NodeFactory::new_FloatArithRRNode(PReg* dst, PReg* src, ArithOpCode op, PReg* o2) {
  FloatArithRRNode* res = new FloatArithRRNode(op, src, o2, dst);
  registerNode(res);
  return res;
}

FloatUnaryArithNode* NodeFactory::new_FloatUnaryArithNode(PReg* dst, PReg* src, ArithOpCode op) {
  FloatUnaryArithNode* res = new FloatUnaryArithNode(op, src, dst);
  registerNode(res);
  return res;
}

MergeNode* NodeFactory::new_MergeNode(Node* prev1, Node* prev2) {
  MergeNode* res = new MergeNode(prev1, prev2);
  registerNode(res);
  return res;
}

MergeNode* NodeFactory::new_MergeNode(int bci) {
  MergeNode* res = new MergeNode(bci);
  registerNode(res);
  return res;
}

SendNode* NodeFactory::new_SendNode(LookupKey* key, MergeNode* nlrTestPoint, GrowableArray<PReg*>* args,
				    GrowableArray<PReg*>* expr_stack, bool superSend, SendInfo* info) {
  SendNode* res = new SendNode(key, nlrTestPoint, args, expr_stack, superSend, info);
  assert(expr_stack, "must have expression stack");
  res->scope()->addSend(expr_stack, true);  // arguments to call are debug-visible
  registerNode(res);
  return res;
}

PrimNode* NodeFactory::new_PrimNode(primitive_desc* pdesc, MergeNode* nlrTestPoint, GrowableArray<PReg*>* args,
				    GrowableArray<PReg*>* expr_stack) {
  PrimNode* res = new PrimNode(pdesc, nlrTestPoint, args, expr_stack);
  if (pdesc->can_walk_stack()) {
    assert(expr_stack, "must have expression stack");
    if (expr_stack) res->scope()->addSend(expr_stack, true);  // arguments to some prim calls are debug-visible
  } else {
    assert(expr_stack == NULL, "should not have expression stack");
  }
  registerNode(res);
  return res;
}

DLLNode* NodeFactory::new_DLLNode(symbolOop dll_name, symbolOop function_name, dll_func function, bool async,
  				  MergeNode* nlrTestPoint, GrowableArray<PReg*>* args, GrowableArray<PReg*>* expr_stack) {
  DLLNode* res = new DLLNode(dll_name, function_name, function, async, nlrTestPoint, args, expr_stack);
  res->scope()->addSend(expr_stack, true);  // arguments to DLL call are debug-visible
  registerNode(res);
  return res;
}

InterruptCheckNode* NodeFactory::new_InterruptCheckNode(GrowableArray<PReg*>* exprStack) {
  InterruptCheckNode* res = new InterruptCheckNode(exprStack);
  registerNode(res);
  return res;
}

LoopHeaderNode* NodeFactory::new_LoopHeaderNode() {
  LoopHeaderNode* res = new LoopHeaderNode();
  registerNode(res);
  return res;
}
  
BlockCreateNode* NodeFactory::new_BlockCreateNode(BlockPReg* b, GrowableArray<PReg*>* expr_stack) {
  BlockCreateNode* res = new BlockCreateNode(b, expr_stack);
  registerNode(res);
  return res;
}

BlockMaterializeNode* NodeFactory::new_BlockMaterializeNode(BlockPReg* b, GrowableArray<PReg*>* expr_stack) {
  BlockMaterializeNode* res = new BlockMaterializeNode(b, expr_stack);
  registerNode(res);
  return res;
}

ContextCreateNode* NodeFactory::new_ContextCreateNode(PReg* parent, PReg* context, int nofTemps, GrowableArray<PReg*>* expr_stack) {
  ContextCreateNode* res = new ContextCreateNode(parent, context, nofTemps, expr_stack);
  registerNode(res);
  return res;
}

ContextCreateNode* NodeFactory::new_ContextCreateNode(PReg* b, const ContextCreateNode* n, GrowableArray<PReg*>* expr_stack) {
  ContextCreateNode* res = new ContextCreateNode(b, n, expr_stack);
  registerNode(res);
  return res;
}

ContextInitNode* NodeFactory::new_ContextInitNode(ContextCreateNode* creator) {
  ContextInitNode* res = new ContextInitNode(creator);
  registerNode(res);
  return res;
}

ContextInitNode* NodeFactory::new_ContextInitNode(PReg* b, const ContextInitNode* n) {
  ContextInitNode* res = new ContextInitNode(b, n);
  registerNode(res);
  return res;
}

ContextZapNode* NodeFactory::new_ContextZapNode(PReg* context) {
  ContextZapNode* res = new ContextZapNode(context);
  registerNode(res);
  return res;
}

BranchNode* NodeFactory::new_BranchNode(BranchOpCode op, bool taken_is_uncommon) {
  BranchNode* res = new BranchNode(op, taken_is_uncommon);
  registerNode(res);
  return res;
}

TypeTestNode* NodeFactory::new_TypeTestNode(PReg* recv, GrowableArray<klassOop>* classes, bool hasUnknown) {
  TypeTestNode* res = new TypeTestNode(recv, classes, hasUnknown);
  registerNode(res);
  res->scope()->addTypeTest(res);
  return res;
}

ArrayAtNode* NodeFactory::new_ArrayAtNode(ArrayAtNode::AccessType access_type, PReg* array, PReg* index, bool smiIndex,
				          PReg* result, PReg* error, int data_offset, int length_offset) {
  ArrayAtNode* res = new ArrayAtNode(access_type, array, index, smiIndex, result, error, data_offset, length_offset);
  registerNode(res);
  return res;
}
  
ArrayAtPutNode* NodeFactory::new_ArrayAtPutNode(ArrayAtPutNode::AccessType access_type, PReg* array, PReg* index, bool smi_index,
					        PReg* element, bool smi_element, PReg* result, PReg* error, int data_offset, int length_offset,
					        bool needs_store_check) {
  ArrayAtPutNode* res = new ArrayAtPutNode(access_type, array, index, smi_index, element, smi_element, result, error, data_offset, length_offset, needs_store_check);
  registerNode(res);
  return res;
}

InlinedPrimitiveNode* NodeFactory::new_InlinedPrimitiveNode(InlinedPrimitiveNode::Operation op, PReg* result, PReg* error,
                                                            PReg* recv, PReg* arg1, bool arg1_is_smi, PReg* arg2, bool arg2_is_smi) {
  InlinedPrimitiveNode* res = new InlinedPrimitiveNode(op, result, error, recv, arg1, arg1_is_smi, arg2, arg2_is_smi);
  registerNode(res);
  return res;
}
  
UncommonNode* NodeFactory::new_UncommonNode(GrowableArray<PReg*>* exprStack, int bci) {
  UncommonNode* res = new UncommonNode(exprStack, bci);
  registerNode(res);
  assert(exprStack, "must have expr. stack");
  res->scope()->addSend(exprStack, false);  // current expr stack is debug-visible
  return res;
}

FixedCodeNode* NodeFactory::new_FixedCodeNode(FixedCodeNode::FixedCodeKind k) {
  FixedCodeNode* res = new FixedCodeNode(k);
  registerNode(res);
  return res;
}

NopNode* NodeFactory::new_NopNode() {
  NopNode* res = new NopNode();
  registerNode(res);
  return res;
}

CommentNode* NodeFactory::new_CommentNode(char* comment) {
  CommentNode* res = new CommentNode(comment);
  registerNode(res);
  return res;
}



void initNodes() {
  Node::currentID = Node::currentCommentID = 0;
  Node::lastScopeInfo = (ScopeInfo)-1;
  Node::lastBCI = IllegalBCI;
  NodeFactory::cumulCost = 0;
}


void BasicNode::setScope(InlinedScope* s) {
  _scope = s;
  _bci = s ? s->bci() : IllegalBCI;
  assert(!s || !s->isInlinedScope() || s->bci() <= ((InlinedScope*)s)->nofBytes(), "illegal bci");
}


BasicNode::BasicNode() {
  _id = currentID++; _bb = NULL;
  setScope(theCompiler->currentScope());
  _num = -1; 
  dontEliminate = deleted = false;
  _mapping = NULL;
}


PRegMapping* BasicNode::mapping() const {
  return new PRegMapping(_mapping);
}


void BasicNode::setMapping(PRegMapping* mapping) {
  assert(!hasMapping(), "cannot be assigned twice");
  _mapping = new PRegMapping(mapping);
}


NonTrivialNode::NonTrivialNode() {
  _src = _dest = NULL;
  srcUse = NULL; destDef = NULL;
}


LoadUplevelNode::LoadUplevelNode(PReg* dst, PReg* context0, int nofLevels, int offset, symbolOop name) : LoadNode(dst) {
  assert(context0 != NULL, "context0 is NULL");
  assert(nofLevels >= 0  , "nofLevels must be >= 0");
  assert(offset >= 0     , "offset must be >= 0");
  _context0    = context0;  _context0Use = NULL;
  _nofLevels   = nofLevels;
  _offset      = offset;
  _name        = name;
}


StoreUplevelNode::StoreUplevelNode(PReg* src, PReg* context0, int nofLevels, int offset, symbolOop name, bool needsStoreCheck)
  : StoreNode(src) {
  assert(context0 != NULL, "context0 is NULL");
  assert(nofLevels >= 0  , "nofLevels must be >= 0");
  assert(offset >= 0     , "offset must be >= 0");
  _context0		= context0;
  _nofLevels		= nofLevels;
  _offset		= offset;
  _needsStoreCheck	= needsStoreCheck;
  _name			= name;
}


AssignNode::AssignNode(PReg* s, PReg* d) : StoreNode(s) {
  _dest = d; assert(d, "dest is NULL");
  // Fix this Lars assert(!s->isNoPReg(), "source must be a real PReg");
  assert(s != d, "creating dummy assignment"); 
}


CommentNode::CommentNode(char* s) {
  comment = s;
  // give all comments negative ids (don't disturb node numbers by turning
  // CompilerDebug off and on)
  _id = --currentCommentID; currentID--;
}


ArrayAtNode::ArrayAtNode(AccessType access_type, PReg* array, PReg* index, bool smiIndex,
                         PReg* result, PReg* error, int data_offset, int length_offset)
  : AbstractArrayAtNode(array, index, smiIndex, result, error, data_offset, length_offset) {
  _access_type = access_type;
}


ArrayAtPutNode::ArrayAtPutNode(AccessType access_type, PReg* array, PReg* index, bool smi_index,
			       PReg* element, bool smi_element, PReg* result, PReg* error, int data_offset, int length_offset,
			       bool needs_store_check)
  : AbstractArrayAtPutNode(array, index, smi_index, element, result, error, data_offset, length_offset) {
  _access_type			= access_type;
  _needs_store_check		= needs_store_check;
  _smi_element			= smi_element;
  _needs_element_range_check	= (access_type == byte_at_put || access_type == double_byte_at_put);
}


TypeTestNode::TypeTestNode(PReg* rr, GrowableArray<klassOop>* classes, bool hasUnknown) {
  _src = rr;
  _classes = classes;
  _hasUnknown = hasUnknown;
  int len = classes->length();
  assert(len > 0, "should have at least one class to test");
  // The assertion below has been replaced by a warning since
  // sometimes Inliner::inlineMerge(...) creates such a TypeTestNode.
  // FIX THIS
  // assert((len > 1) || hasUnknown, "TypeTestNode is not necessary");
  if ((len == 1) && !hasUnknown) {
    warning("TypeTestNode with only one klass & no uncommon case => performance bug");
  }
# ifdef ASSERT
    for (int i = 0; i < len; i++) {
      for (int j = i + 1; j < len; j++) {
	assert(classes->at(i) != classes->at(j), "duplicate class");
      }
    }
# endif
}


ArithRRNode::ArithRRNode(ArithOpCode op, PReg* arg1, PReg* arg2, PReg* dst) : ArithNode(op, arg1, dst) {
  _oper = arg2;
  if (_src->isConstPReg() && ArithOpIsCommutative[_op]) {
    // make sure that if there's a constant argument, it's the 2nd one
    PReg* t1 = _src; _src = _oper; _oper = t1;
  }
}


TArithRRNode::TArithRRNode(ArithOpCode op, PReg* arg1, PReg* arg2, PReg* dst, bool arg1IsInt, bool arg2IsInt) {
  if (arg1->isConstPReg() && ArithOpIsCommutative[op]) {
    // make sure that if there's a constant argument, it's the 2nd one
    PReg* t1 = arg1;      arg1      = arg2;      arg2      = t1;
    bool  t2 = arg1IsInt; arg1IsInt = arg2IsInt; arg2IsInt = t2;
  }
  _op = op;
  _src = arg1;
  _oper = arg2;
  _dest = dst;
  _arg1IsInt = arg1IsInt;
  _arg2IsInt = arg2IsInt;
  _constResult = NULL;
  dontEliminate = true; // don't eliminate even if result unused because primitive might fail
}
  

PReg* NonTrivialNode::dest() const {
  if (!hasDest()) fatal("has no dest");
  return _dest;
}


void NonTrivialNode::setDest(BB* bb, PReg* d) {
  // bb == NULL means don't update defs
  if (!hasDest()) fatal("has no dest");
  assert(bb || !destDef, "shouldn't have a def");
  if (destDef) _dest->removeDef(bb, destDef);
  _dest = d;
  if (bb) destDef = _dest->addDef(bb, (NonTrivialNode*)this);
}


PReg* NonTrivialNode::src() const {
  if (!hasSrc()) fatal("has no src");
  return _src;
}


bool AssignNode::isAccessingFloats() const {
  // After building the node data structure, float pregs have a float location but
  // later during compilation, this location is transformed into a stack location,
  // therefore the two tests. This should change at some point; it would be cleaner
  // to have a special FloatPRegs (or a flag in the PReg, respectively).
  return
    _src ->loc.isFloatLocation() || _src ->loc == topOfFloatStack || Mapping::isFloatTemporary(_src ->loc) ||
    _dest->loc.isFloatLocation() || _dest->loc == topOfFloatStack || Mapping::isFloatTemporary(_dest->loc) ;
}


oop AssignNode::constantSrc() const {
  assert(hasConstantSrc(), "no constant src");
  return ((ConstPReg*)_src)->constant;
}


bool AssignNode::canBeEliminated() const {
  return !(_src->loc.isTopOfStack() || _dest->loc.isTopOfStack());
}


bool Node::endsBB() const { 
  return next() == NULL || next()->startsBB(); 
}

void Node::removeMe() {
  assert(hasSingleSuccessor() && hasSinglePredecessor(), "subclass");
  if (_prev) _prev->moveNext(this, _next);
  if (_next) _next->movePrev(this, _prev);
  _prev = _next = NULL;
}

void Node::removePrev(Node* n) {
  /* cut the _prev link between this and n	*/
  assert(hasSinglePredecessor(), "subclass");
  assert(_prev == n, "should be same");
  _prev = NULL;
}

void Node::removeNext(Node* n) {
  /* cut the next link between this and n */
  assert(hasSingleSuccessor(), "subclass");
  assert(_next == n, "should be same");
  n->removePrev(this);
  _next = NULL;
}

Node* Node::endOfList() const {
  if (_next == NULL) return (Node*)this;
  for (Node* n = _next; n->_next; n = n->_next) {
      assert(n->hasSingleSuccessor(), ">1 successors");
  }
  return n;
}

void AbstractMergeNode::removeMe() {
  if (hasSinglePredecessor()) {
    _prev = firstPrev();
    TrivialNode::removeMe();
  } else {
    fatal("not implemented yet");
  }
}

void AbstractMergeNode::movePrev(Node* from, Node* to) {
  for (int i = _prevs->length() - 1; i >= 0; i--) {
    if (_prevs->at(i) == from) { _prevs->at_put(i, to); return; }
  }
  fatal("from not found");
}

bool AbstractMergeNode::isPredecessor(const Node* n) const {
  for (int i = _prevs->length() - 1; i >= 0; i--) {
    if (_prevs->at(i) == n) return true;
  }
  return false;
}

void AbstractBranchNode::removeMe() {
  if (hasSingleSuccessor()) {
    if (!_next && _nxt->nonEmpty()) { _next = next1(); _nxt->pop(); }
      NonTrivialNode::removeMe();
  } else {
      fatal("not implemented yet");
  }
}

void AbstractBranchNode::removeNext(Node* n) {
  /* cut the next link between this and n */
  if (n == _next) {
    n->removePrev(this);
    _next = NULL;
  } else {
    for (int i = 0; i < _nxt->length() && _nxt->at(i) != n; i++) ;
    assert(i < _nxt->length(), "not found");
    n->removePrev(this);
    for ( ; i < _nxt->length() - 1; i++) _nxt->at_put(i, _nxt->at(i+1));
    _nxt->pop();
  }
}

void AbstractBranchNode::setNext(int i, Node* n) {
  if (i == 0) {
    setNext(n);
  } else {
    assert(_nxt->length() == i - 1, "wrong index");
    _nxt->append(n);
  }
}

void AbstractBranchNode::moveNext(Node* from, Node* to) {
  if (_next == from) {
    _next = to;
  } else {
    for (int i = 0; i < _nxt->length() && _nxt->at(i) != from; i++) ;
    assert(i < _nxt->length(), "not found");
    _nxt->at_put(i, to);
  }
}

bool AbstractBranchNode::isSuccessor(const Node* n) const {
  if (_next == n) {
    return true;
  } else {
    for (int i = 0; i < _nxt->length() && _nxt->at(i) != n; i++) ;
    return i < _nxt->length();
  }
}

BB* BasicNode::newBB() {
  if (_bb == NULL) {
    int len = 0;
    _bb = new BB((Node*)this, (Node*)this, 1);     
    for (Node* n = (Node*)this; !n->endsBB() && n->next() != NULL; n = n->next()) {
      n->_num = len++; n->_bb = _bb;
    }
    n->_num = len++; n->_bb = _bb;
    _bb->last = n;
    _bb->nnodes = len;
  }
  return _bb;
}


MergeNode::MergeNode(Node* prev1, Node* prev2) : AbstractMergeNode(prev1, prev2) {
  _bci = max(prev1->bci(), prev2->bci());
  isLoopStart = isLoopEnd = didStartBB = false;
}


MergeNode::MergeNode(int bci) {
  _bci = bci;
  isLoopStart = isLoopEnd = didStartBB = false;
}


BB* MergeNode::newBB() {
  if (_bb == NULL) {
    // receiver starts a new BB
    didStartBB = true;
    _bb = Node::newBB();
  }
  return _bb;
}


ReturnNode::ReturnNode(PReg* res, int bci)
  : AbstractReturnNode(
      bci,
      res,
      new TempPReg(theCompiler->currentScope(), resultLoc, true, true)
    ) {
  assert(res->loc == resultLoc, "must be in special location");
}


NLRSetupNode::NLRSetupNode(PReg* result, int bci)
  : AbstractReturnNode(
      bci,
      result,
      new TempPReg(theCompiler->currentScope(), resultLoc, true, true)
    ) {
  contextUse = resultUse = NULL;
  assert(result->loc == NLRResultLoc, "must be in special location");
}


MergeNode* CallNode::nlrTestPoint() const {
  if (nSuccessors() > 1) {
    assert(next1()->isMergeNode(), "should be a merge");
    return (MergeNode*)next1();
  } else {
    return NULL;
  }
}


CallNode::CallNode(MergeNode* n, GrowableArray<PReg*>* a, GrowableArray<PReg*>* e) {
  if (n != NULL) append1(n);
  exprStack = e;
  args = a;
  _dest = new SAPReg(scope(), resultLoc, false, false, _bci, _bci);
  argUses = uplevelUses = NULL;
  uplevelDefs = NULL;
  uplevelUsed = uplevelDefd = NULL;
  nblocks = theCompiler->blockClosures->length();
}


SendNode::SendNode(
  LookupKey* key,
  MergeNode* nlrTestPoint,
  GrowableArray<PReg*>* args,
  GrowableArray<PReg*>* expr_stack,
  bool superSend,
  SendInfo* info
  )
  : CallNode(nlrTestPoint, args, expr_stack) {
  _key = key;
  _superSend = superSend; _info = info;
  assert(exprStack, "should have expr stack");
  // Fix this when compiler is more flexible
  // not a fatal because it could happen for super sends that fail (no super method found)
  if (_superSend && !UseNewBackend) warning("We cannot yet have super sends in nmethods");
}


ContextCreateNode::ContextCreateNode(PReg* parent, PReg* context, int nofTemps, GrowableArray<PReg*>* expr_stack)
: PrimNode(primitives::context_allocate(), NULL, NULL, expr_stack) {
  _src		= parent;
  _dest		= context;
  _nofTemps	= nofTemps;
  _contextSize  = 0;
  _parentContexts = NULL;
  _parentContextUses = NULL;
  Scope* p = _scope->parent();
  PReg* prevContext = parent;
  // collect all parent contexts
  while (p && p->isInlinedScope() && ((InlinedScope*)p)->context()) {
    PReg* c = ((InlinedScope*)p)->context();
    if (c != prevContext) {
      if (!_parentContexts) _parentContexts = new GrowableArray<PReg*>(5);
      _parentContexts->append(c);
      prevContext = c;
    }
    p = p->parent();
  }
}


ContextCreateNode::ContextCreateNode(PReg* b, const ContextCreateNode* n, GrowableArray<PReg*>* expr_stack)
: PrimNode(primitives::context_allocate(), NULL, NULL, expr_stack) {
  warning("check this implementation");
  Unimplemented();
  // Urs, don't we need a source here?
  // I've added hasSrc() (= true) to ContextCreateNode) - should double check this
  // What is this constructor good for? Cloning only? src should be taken care of as well, I guess.
  // This constructor would be called only for splitting (when copying the node) -- won't happen for now.
  _dest		= b;
  _nofTemps	= n->_nofTemps;
  _parentContextUses  = NULL;
}


ContextInitNode::ContextInitNode(ContextCreateNode* creator) {
  int nofTemps	= creator->nofTemps();
  _src		= creator->context();
  assert(_src, "must have context");
  _initializers	= new GrowableArray<Expr*>(nofTemps, nofTemps, NULL); 	// holds initializer for each element (or NULL)
  _contentDefs	= NULL;
  _initializerUses = NULL;
  _materializedBlocks = NULL;
}


ContextInitNode::ContextInitNode(PReg* b, const ContextInitNode* node) {
  _src		= b;
  assert(_src, "must have context");
  _initializers	= node->_initializers;
  assert((node->_contentDefs == NULL) && (node->_initializerUses == NULL), "shouldn't copy after uses have been built");
  _contentDefs	= NULL;
  _initializerUses = NULL;
  _materializedBlocks = NULL;
}


BlockCreateNode::BlockCreateNode(BlockPReg* b, GrowableArray<PReg*>* expr_stack) : 
PrimNode(primitives::block_allocate(), NULL, NULL, expr_stack) {
  _src = NULL;
  _dest = b;
  _contextUse = NULL;
  switch (b->method()->block_info()) {
   case methodOopDesc::expects_nil:		// no context needed
   	_context = NULL; break;
   case methodOopDesc::expects_self:
   	_context = b->scope()->self()->preg(); break;
   case methodOopDesc::expects_parameter:	// fix this -- should find which
	_context = NULL;
	break;
   case methodOopDesc::expects_context:
	_context = b->scope()->context(); break;
   default:
   	fatal("unexpected incoming info");
  };
}


int ContextInitNode::positionOfContextTemp(int n) const {
  // return position of ith context temp in compiled (physical) context
  int pos = 0;
  for (int i = 0; i < n; i++) {
    PReg* p = contents()->at(i)->preg();
    if (p->loc.isContextLocation()) pos++;
  }
  return pos;
}

void ContextInitNode::initialize(int no, Expr* expr) {
  assert((_initializers->at(no) == NULL) || (_initializers->at(no)->constant() == nilObj),
	 "already initialized this context element");
  _initializers->at_put(no, expr);
}


ContextCreateNode* ContextInitNode::creator() const {
  // returns the corresponding context creation node
  Node* n = _prev;
  assert(n->isContextCreateNode(), "must be creator node");
  return (ContextCreateNode*)n;
}


void ContextInitNode::addBlockMaterializer(BlockMaterializeNode* n) {
  if (!_materializedBlocks) _materializedBlocks = new GrowableArray<BlockMaterializeNode*>(5);
  _materializedBlocks->append(n);
}

void ContextInitNode::notifyNoContext() {
  // our context has been optimized away, i.e., all context contents
  // will be stack-allocated like normal PRegs
  // remove the context use (otherwise the contextPR has 1 use and no defs)
  _src->removeUse(bb(), srcUse);
  _src = NULL;
  if (_materializedBlocks) {
    for (int i = _materializedBlocks->length() - 1; i >= 0; i--) {
      // remove the block materialization node
      BlockMaterializeNode* n = _materializedBlocks->at(i);
      n->eliminate(n->bb(), NULL, true, false); 
      PReg* blk = n->src();
      assert(blk->isBlockPReg(), "must be a block");

      // remove use of block
      for (int j = _initializers->length() - 1; j >= 0; j--) {
	if (_initializers->at(j)->preg() == blk) {
	  blk->removeUse(_bb, _initializerUses->at(j));
	  break;
	}
      }

      // try to eliminate the block, too 
      if (blk->hasNoUses()) {
	// eliminate the block
	blk->eliminate(false);
      }
    }
  }
}


PrimNode::PrimNode(primitive_desc* pdesc, MergeNode* nlrTestPoint, GrowableArray<PReg*>* args, GrowableArray<PReg*>* expr_stack)
  : CallNode(nlrTestPoint, args, expr_stack) {
  _pdesc = pdesc;
  assert(_pdesc->can_perform_NLR() || (nlrTestPoint == NULL), "no NLR target needed"); 
  if (pdesc->can_invoke_delta()) {
    assert(expr_stack != NULL, "should have expr stack");
  } else {
    // the expression stack is only needed if the primitive can walk the
    // stack (then the elements will be debug-visible) or if the primitive
    // can scavenge (then the elems must be allocated to GCable regs)
    exprStack = NULL;        
  }
}


InlinedPrimitiveNode::InlinedPrimitiveNode(Operation op, PReg* result, PReg* error,
                                           PReg* recv, PReg* arg1, bool arg1_is_smi, PReg* arg2, bool arg2_is_smi) {
  _op		= op;
  _dest		= result;
  _error	= error;
  _src		= recv;
  _arg1		= arg1;
  _arg2		= arg2;
  _arg1_is_smi	= arg1_is_smi;
  _arg2_is_smi	= arg2_is_smi;
}


bool InlinedPrimitiveNode::canFail() const {
  switch (op()) {
    case obj_klass        : return false;
    case obj_hash         : return false;
    case proxy_byte_at    : return !arg1_is_smi();
    case proxy_byte_at_put: return !arg1_is_smi() || !arg2_is_smi();
  };
  ShouldNotReachHere();
  return false;
}


bool InlinedPrimitiveNode::canBeEliminated() const {
  switch (op()) {
    case obj_klass        : return true;
    case obj_hash         : return true;
    case proxy_byte_at    : return !canFail();
    case proxy_byte_at_put: return false;
  };
  ShouldNotReachHere();
  return false;
}


UncommonNode::UncommonNode(GrowableArray<PReg*>* e, int bci) { exprStack = e; _bci = bci;}


bool PrimNode::canBeEliminated() const {
  if (!Node::canBeEliminated()) return false;
  if (_pdesc->can_be_constant_folded() && !canFail()) return true;
  // temporary hack -- fix this
  // should test arg types to make sure prim won't fail
  // for now, treat cloning etc. special
  if (_pdesc->can_be_constant_folded()) return true;	// not safe!
  if (_pdesc->fn() == fntype(&behaviorPrimitives::allocate) ||
      _pdesc->fn() == fntype(&primitiveNew0) ||
      _pdesc->fn() == fntype(&primitiveNew1) ||
      _pdesc->fn() == fntype(&primitiveNew2) ||
      _pdesc->fn() == fntype(&primitiveNew3) ||
      _pdesc->fn() == fntype(&primitiveNew4) ||
      _pdesc->fn() == fntype(&primitiveNew5) ||
      _pdesc->fn() == fntype(&primitiveNew6) ||
      _pdesc->fn() == fntype(&primitiveNew7) ||
      _pdesc->fn() == fntype(&primitiveNew8) ||
      _pdesc->fn() == fntype(&primitiveNew9)
     ) {
    return true;
  }
  return false;
}

bool PrimNode::canInvokeDelta() const {
  return _pdesc->can_invoke_delta();
}

bool PrimNode::canFail() const {
  return _pdesc->can_fail();
}


DLLNode::DLLNode(symbolOop dll_name, symbolOop function_name, dll_func function, bool async,
  	         MergeNode* nlrTestPoint, GrowableArray<PReg*>* args, GrowableArray<PReg*>* expr_stack)
  : CallNode(nlrTestPoint, args, expr_stack) {
  _dll_name      = dll_name;
  _function_name = function_name;
  _function      = function;
  _async         = async;
}


bool DLLNode::canInvokeDelta() const {
  return true;		// user-defined DLL code can do anything
}


NLRTestNode::NLRTestNode(int bci) {}


void NLRTestNode::fixup() {
  // connect next() and next1()
  if (scope()->isTop()) {
    theCompiler->enterScope(scope()); // so that nodes get right scope
    // Non-inlined scope: continuation just continues NLR
    append(0, NodeFactory::new_NLRContinuationNode(bci()));
    // return point returns the NLR result
    PReg* nlr_result = new TempPReg(scope(), resultOfNLR, true, true);
    Node* n = NodeFactory::new_AssignNode(nlr_result, scope()->resultPR);
    append(1, n);
    n->append(scope()->returnPoint());
    theCompiler->exitScope(scope());
  } else {
    // Inlined scope: for continuation, follow caller chain until a nlrTestPoint is found
    // (the top scope is guaranteed to have a nlrTestPoint, so loop below will terminate correctly)
    // introduce an extra assignment to satisfy new backend, will be optimized away
    InlinedScope* s = scope()->sender();
    while (!s->has_nlrTestPoint()) s = s->sender();
    append(0, s->nlrTestPoint());
    // now connect to return point and return the NLR value
    // s may not have a return point, so search for one
    s = scope();
    while (s->returnPoint() == NULL) s = s->sender();
    theCompiler->enterScope(s); // so that node gets right scope
    PReg* nlr_result = new TempPReg(scope(), resultOfNLR, true, true);
    Node* n = NodeFactory::new_AssignNode(nlr_result, s->resultPR);
    theCompiler->exitScope(s);
    append(1, n);
    n->append(s->returnPoint());
  }
}


bool SendNode::isCounting() const	{ return _info->counting;	}
bool SendNode::isUninlinable() const	{ return _info->uninlinable;	}
bool SendNode::staticReceiver() const	{ return _info->receiverStatic;	}

PReg* SendNode::recv() const {
  int i = args->length() - 1;
  while (i >= 0 && args->at(i)->loc != receiverLoc) i--;
  assert(i >= 0, "must have a receiver");
  return args->at(i);
}


// ==================================================================================
// cloning: copy the node during splitting; returning NULL means node is a de facto
// no-op only need to copy the basic state; defs, uses etc haven't yet been computed
// ==================================================================================

// general splitting hasn't been implemented yet; only nodes that shouldCopyWhenSplitting()
// are currently copied  -Urs 5/96

Node* BasicNode::copy(PReg* from, PReg* to) const {
  Node* c = clone(from, to);
  if (c) { c->_scope = _scope; c->_bci = _bci; }
  return c;
}

# define SHOULD_NOT_CLONE  	{ Unused(from); Unused(to); ShouldNotCallThis(); return NULL; }
# define TRANSLATE(s) 		((s == from) ? to : s)

Node* PrologueNode::clone	(PReg* from, PReg* to) const { SHOULD_NOT_CLONE }
Node* NLRSetupNode::clone	(PReg* from, PReg* to) const { SHOULD_NOT_CLONE }
Node* NLRContinuationNode::clone(PReg* from, PReg* to) const { SHOULD_NOT_CLONE }
Node* ReturnNode::clone		(PReg* from, PReg* to) const { SHOULD_NOT_CLONE }
Node* BranchNode::clone		(PReg* from, PReg* to) const { SHOULD_NOT_CLONE }
Node* TypeTestNode::clone	(PReg* from, PReg* to) const { SHOULD_NOT_CLONE }
Node* FixedCodeNode::clone	(PReg* from, PReg* to) const { SHOULD_NOT_CLONE }

Node* LoadOffsetNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_LoadOffsetNode(TRANSLATE(_dest), _src, offset, isArraySize);
}
Node* LoadIntNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_LoadIntNode(TRANSLATE(_dest), _value);
}
Node* LoadUplevelNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_LoadUplevelNode(TRANSLATE(_dest), TRANSLATE(_context0), _nofLevels, _offset, _name);
}
Node* StoreOffsetNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_StoreOffsetNode(TRANSLATE(_src), _base, _offset, _needsStoreCheck);
}
Node* StoreUplevelNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_StoreUplevelNode(TRANSLATE(_src), TRANSLATE(_context0), _nofLevels, _offset, _name, _needsStoreCheck);
}
Node* AssignNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_AssignNode(TRANSLATE(_src), TRANSLATE(_dest));
}
Node* ArithRRNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_ArithRRNode(TRANSLATE(_dest), TRANSLATE(_src), _op, _oper);
}
Node* TArithRRNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_TArithRRNode(TRANSLATE(_dest), TRANSLATE(_src), _op, _oper, _arg1IsInt, _arg2IsInt);
}
Node* ArithRCNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_ArithRCNode(TRANSLATE(_dest), TRANSLATE(_src), _op, _oper);
}

Node* SendNode::clone(PReg* from, PReg* to) const {
  Unused(from); Unused(to);
  // NB: use current split signature, not the receiver's sig!
  SendNode* n = NodeFactory::new_SendNode(_key, nlrTestPoint(), args, exprStack, _superSend, _info);
  n->_dest = _dest;	    // don't give it a new dest!
  return n;
}

Node* PrimNode::clone(PReg* from, PReg* to) const {
  // NB: use scope's current sig, not the receiver's sig!
  PrimNode* n = NodeFactory::new_PrimNode(_pdesc, nlrTestPoint(), args, exprStack);
  assert(_dest != from, "shouldn't change dest");
  n->_dest = _dest;	    // don't give it a new dest!
  return n;
}

Node* DLLNode::clone(PReg* from, PReg* to) const {
  // NB: use scope's current sig, not the receiver's sig!
  DLLNode* n = NodeFactory::new_DLLNode(_dll_name, _function_name, _function, _async, nlrTestPoint(), args, exprStack);
  assert(_dest != from, "shouldn't change dest");
  n->_dest = _dest;	    // don't give it a new dest!
  return n;
}


Node* InterruptCheckNode::clone(PReg* from, PReg* to) const {
  Unused(from); Unused(to);
  // NB: use scope's current sig, not the receiver's sig!
  InterruptCheckNode* n = NodeFactory::new_InterruptCheckNode(exprStack);
  assert(_dest != from, "shouldn't change dest");
  n->_dest = _dest;	    // don't give it a new dest!
  return n;
}


Node* BlockCreateNode::clone(PReg* from, PReg* to) const  {
  // NB: use scope's current sig, not the receiver's sig!
  BlockCreateNode* n = NodeFactory::new_BlockCreateNode((BlockPReg*)TRANSLATE(block()), exprStack);    
  assert(_dest != from, "shouldn't change dest");
  n->_dest = _dest;	    // don't give it a new dest!
  return n;
}


Node* BlockMaterializeNode::clone(PReg* from, PReg* to) const {
  // NB: use scope's current sig, not the receiver's sig!
  BlockMaterializeNode* n = NodeFactory::new_BlockMaterializeNode((BlockPReg*)TRANSLATE(block()), exprStack);
  assert(_dest != from, "shouldn't change dest");
  n->_dest = _dest;	    // don't give it a new dest!
  return n;
}


Node* ContextCreateNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_ContextCreateNode(TRANSLATE(_dest), this, exprStack);
}


Node* ContextInitNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_ContextInitNode(TRANSLATE(_src), this);
}


Node* ContextZapNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_ContextZapNode(TRANSLATE(_src));
}


Node* NLRTestNode::clone(PReg* from, PReg* to) const {
  Unimplemented();
  return NULL;
}


Node* ArrayAtNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_ArrayAtNode(_access_type,
                                      TRANSLATE(_src), TRANSLATE(_arg), _intArg,
				      TRANSLATE(_dest), TRANSLATE(_error),
				      _dataOffset, _sizeOffset);
}


Node* ArrayAtPutNode::clone(PReg* from, PReg* to) const {
  return NodeFactory::new_ArrayAtPutNode(_access_type,
  					 TRANSLATE(_src), TRANSLATE(_arg), _intArg,
					 TRANSLATE(elem), _smi_element, TRANSLATE(_dest),
					 TRANSLATE(_error), _dataOffset, _sizeOffset, _needs_store_check);
}


Node* UncommonNode::clone(PReg* from, PReg* to) const {
  Unused(from); Unused(to);
  return NodeFactory::new_UncommonNode(exprStack, _bci);
}


Node* InlinedReturnNode::clone	(PReg* from, PReg* to) const { 
  return NodeFactory::new_InlinedReturnNode(bci(), TRANSLATE(src()), TRANSLATE(dest())); 
}


# define NO_NEED_TO_COPY	{ Unused(from); Unused(to); return NULL; }

Node* MergeNode::clone		(PReg* from, PReg* to) const { NO_NEED_TO_COPY }
Node* NopNode::clone		(PReg* from, PReg* to) const { NO_NEED_TO_COPY }
Node* CommentNode::clone	(PReg* from, PReg* to) const { NO_NEED_TO_COPY }


// ==================================================================================
// makeUses: construct all uses and defs
// ==================================================================================

void PrologueNode::makeUses(BB* bb) {
  InlinedScope* s = scope();
  // build initial def for self and context (for blocks only)
  bb->addDef(this, s->self()->preg());
  if (s->isBlockScope()) {
    bb->addDef(this, s->context());
  }
  // build initial defs for incoming args
  for (int i = 0; i < _nofArgs; i++) {
    Expr* a = s->argument(i);
    if (a) bb->addDef(this, a->preg());
  }
  // build initial defs for locals (initalization to nil)
  for (i = 0; i < _nofTemps; i++) {
    Expr* t = s->temporary(i);
    if (t) bb->addDef(this, t->preg());
  }
}

void NonTrivialNode::makeUses(BB* bb) {
  _bb = bb;
  assert(!hasSrc()  || srcUse,  "should have srcUse");
  assert(!hasDest() || destDef || _dest->isNoPReg(), "should have destDef");
}

void LoadNode::makeUses(BB* bb) {
  destDef = bb->addDef(this, _dest); NonTrivialNode::makeUses(bb); }
void LoadOffsetNode::makeUses(BB* bb) {
  srcUse = bb->addUse(this, _src); LoadNode::makeUses(bb); }
void LoadUplevelNode::makeUses(BB* bb) {
  _context0Use = bb->addUse(this, _context0); LoadNode::makeUses(bb);
}

void StoreNode::makeUses(BB* bb) {
  srcUse = bb->addUse(this, _src); NonTrivialNode::makeUses(bb); }
void StoreOffsetNode::makeUses(BB* bb) {
  _baseUse = bb->addUse(this, _base); StoreNode::makeUses(bb); }
void StoreUplevelNode::makeUses(BB* bb) {
  _context0Use = bb->addUse(this, _context0); StoreNode::makeUses(bb);
}

void AssignNode::makeUses(BB* bb) {
  destDef = bb->addDef(this, _dest); StoreNode::makeUses(bb); }

void AbstractReturnNode::makeUses(BB* bb) {
  srcUse = bb->addUse(this, _src);
  destDef = bb->addDef(this, _dest);
  NonTrivialNode::makeUses(bb);
}

void ReturnNode::makeUses(BB* bb) {
  // resultUse models the value's use in the caller
  resultUse = bb->addUse(this, _dest);
  AbstractReturnNode::makeUses(bb);
}

void NLRSetupNode::makeUses(BB* bb) {
  // has no src or dest -- uses hardwired NLR register
  // but track them anyway for consistency
  // resultUse models the value's use in the caller
  resultUse  = bb->addUse(this, _dest);
  contextUse = bb->addUse(this, _scope->context());
  AbstractReturnNode::makeUses(bb);
}

void NLRTestNode::makeUses(BB* bb) {
  AbstractBranchNode::makeUses(bb);
}

void NLRContinuationNode::makeUses(BB* bb) {
  // has no src or dest -- uses hardwired NLR register
  // but track them anyway for consistency
  AbstractReturnNode::makeUses(bb);
}

void ArithNode::makeUses(BB* bb) {
  srcUse = bb->addUse(this, _src);  destDef = bb->addDef(this, _dest);
  NonTrivialNode::makeUses(bb); 
}
void ArithRRNode::makeUses(BB* bb) { 
  _operUse = bb->addUse(this, _oper); ArithNode::makeUses(bb); }

void TArithRRNode::makeUses(BB* bb) { 
  _operUse = bb->addUse(this, _oper);
  srcUse = bb->addUse(this, _src);
  destDef = bb->addDef(this, _dest);
  NonTrivialNode::makeUses(bb);
}

void CallNode::makeUses(BB* bb) {
  destDef = bb->addDef(this, _dest); 
  if (args) {
    int len = args->length();
    argUses = new GrowableArray<Use*>(len);
    for (int i = 0; i < len; i++) {
      argUses->append(bb->addUse(this, args->at(i)));
    }
  }
  NonTrivialNode::makeUses(bb); 
  if (canInvokeDelta()) {
    // add defs/uses for all PRegs uplevel-accessed by live blocks
    const int InitialSize = 5;
    uplevelUses = new GrowableArray<Use*> (InitialSize);
    uplevelDefs = new GrowableArray<Def*> (InitialSize);
    uplevelUsed = new GrowableArray<PReg*>(InitialSize);
    uplevelDefd = new GrowableArray<PReg*>(InitialSize);
    GrowableArray<BlockPReg*>* blks = theCompiler->blockClosures;
    for (int i = 0; i < nblocks; i++) {
      BlockPReg* blk = blks->at(i);
      if (blk->escapes()) {
        // check if block's home is on current call stack; if not, we don't care about
	// the block's uplevel accesses since the block is non-LIFO and we won't access its
	// context anyway
	Scope* home = blk->scope()->home();
	if (home->isSenderOrSame(scope())) {
	  // ok, this block is live
	  GrowableArray<PReg*>* uplevelRead = blk->uplevelRead();
	  for (int j = uplevelRead->length() - 1; j >= 0; j--) {
	    PReg* r = uplevelRead->at(j);
	    uplevelUses->append(bb->addUse(this, r));
	    uplevelUsed->append(r);
	  }
	  GrowableArray<PReg*>* uplevelWritten = blk->uplevelWritten();
	  for (j = uplevelWritten->length() - 1; j >= 0; j--) {
	    PReg* r = uplevelWritten->at(j);
	    uplevelDefs->append(bb->addDef(this, r));
	    uplevelDefd->append(r);
	  }
	}
      }
    }
  }
}

void BlockCreateNode::makeUses(BB* bb) {
  if (_context && !isMemoized()) {
    _contextUse = bb->addUse(this, _context);
  }
  destDef = bb->addDef(this, _dest); 
  NonTrivialNode::makeUses(bb);
}

void BlockMaterializeNode::makeUses(BB* bb) {
  // without memoization, BlockMaterializeNode is a no-op
  if (isMemoized()) {
    srcUse = bb->addUse(this, _src);
    if (_context) _contextUse = bb->addUse(this, _context);
    BlockCreateNode::makeUses(bb);
  }
}

void ContextCreateNode::makeUses(BB* bb) {
  if (_src) srcUse = bb->addUse(this, _src); // no src if there's no incoming context
  destDef = bb->addDef(this, _dest); 
  if (_parentContexts) {
    int len = _parentContexts->length();
    _parentContextUses = new GrowableArray<Use*>(len, len, NULL);
    for (int i = _parentContexts->length() - 1; i>= 0; i--) {
      Use* u = bb->addUse(this, _parentContexts->at(i));
      _parentContextUses->at_put(i, u);
    }
  }
  NonTrivialNode::makeUses(bb);
}

void ContextInitNode::makeUses(BB* bb) {
  srcUse = bb->addUse(this, _src);
  int i = nofTemps();
  _contentDefs = new GrowableArray<Def*>(i, i, NULL);
  _initializerUses = new GrowableArray<Use*>(i, i, NULL);
  while (i-- > 0) {
    PReg* r = contents()->at(i)->preg();
    if (r->isBlockPReg()) {
      _contentDefs->at_put(i, NULL);	  // there is no assignment to the block
    } else {
      _contentDefs->at_put(i, bb->addDef(this, r));
    }
    _initializerUses->at_put(i, bb->addUse(this, _initializers->at(i)->preg()));
  } 
  NonTrivialNode::makeUses(bb);
}

void ContextZapNode::makeUses(BB* bb) {
  srcUse = bb->addUse(this, _src);
}

void TypeTestNode::makeUses(BB* bb) {
  srcUse = bb->addUse(this, _src); AbstractBranchNode::makeUses(bb);
}

void AbstractArrayAtNode::makeUses(BB* bb) {
  srcUse = bb->addUse(this, _src);
  destDef = _dest ? bb->addDef(this, _dest) : NULL;
  _argUse = bb->addUse(this, _arg);
  _errorDef = (_error && canFail()) ? bb->addDef(this, _error) : NULL;
  AbstractBranchNode::makeUses(bb);
}

void AbstractArrayAtPutNode::makeUses(BB* bb) {
  elemUse = bb->addUse(this, elem); 
  AbstractArrayAtNode::makeUses(bb);
}

void InlinedPrimitiveNode::makeUses(BB* bb) {
  srcUse     = _src   ? bb->addUse(this, _src  ) : NULL;
  _arg1_use  = _arg1  ? bb->addUse(this, _arg1 ) : NULL;
  _arg2_use  = _arg2  ? bb->addUse(this, _arg2 ) : NULL;
  destDef    = _dest  ? bb->addDef(this, _dest ) : NULL; 
  _error_def = (_error && canFail()) ? bb->addDef(this, _error) : NULL;
  AbstractBranchNode::makeUses(bb);
}

// ==================================================================================
// removeUses: remove all uses and defs (receiver node has been eliminated)
// ==================================================================================

// removeUses: remove all uses and defs
void NonTrivialNode::removeUses(BB* bb) { assert(_bb == bb, "wrong BB"); }

void LoadNode::removeUses(BB* bb) {
  _dest->removeDef(bb, destDef); NonTrivialNode::removeUses(bb);
}

void LoadOffsetNode::removeUses(BB* bb) {
  _src->removeUse(bb, srcUse); LoadNode::removeUses(bb);
}

void LoadUplevelNode::removeUses(BB* bb) {
  _context0->removeUse(bb, _context0Use); LoadNode::removeUses(bb);
}

void StoreNode::removeUses(BB* bb) {
  _src->removeUse(bb, srcUse); NonTrivialNode::removeUses(bb);
}

void StoreOffsetNode::removeUses(BB* bb) {
  _base->removeUse(bb, _baseUse); StoreNode::removeUses(bb);
}

void StoreUplevelNode::removeUses(BB* bb) {
  _context0->removeUse(bb, _context0Use); StoreNode::removeUses(bb);
}

void AssignNode::removeUses(BB* bb) {
  _dest->removeDef(bb, destDef); StoreNode::removeUses(bb);
}

void AbstractReturnNode::removeUses(BB* bb) {
  _src->removeUse(bb, srcUse); _dest->removeDef(bb, destDef);
  NonTrivialNode::removeUses(bb);
}

void ReturnNode::removeUses(BB* bb) {
  _dest->removeUse(bb, resultUse);
  AbstractReturnNode::removeUses(bb); 
}

void NLRSetupNode::removeUses(BB* bb) {
  _dest->removeUse(bb, resultUse);
  _scope->context()->removeUse(bb, contextUse);
  AbstractReturnNode::removeUses(bb); 
}

void NLRTestNode::removeUses(BB* bb) {
  AbstractBranchNode::removeUses(bb); 
}

void NLRContinuationNode::removeUses(BB* bb) {
  AbstractReturnNode::removeUses(bb); 
}

void ArithNode::removeUses(BB* bb) {
  _src->removeUse(bb, srcUse); _dest->removeDef(bb, destDef);
  NonTrivialNode::removeUses(bb);
}

void ArithRRNode::removeUses(BB* bb) { 
  _oper->removeUse(bb, _operUse); ArithNode::removeUses(bb);
}

void TArithRRNode::removeUses(BB* bb) { 
  _oper->removeUse(bb, _operUse);
  _src->removeUse(bb, srcUse);
  _dest->removeDef(bb, destDef);
  NonTrivialNode::removeUses(bb);
}

void CallNode::removeUses(BB* bb) {
  _dest->removeDef(bb, destDef);
  if (argUses) {
    int len = args->length();
    for (int i = 0; i < len; i++) {
      args->at(i)->removeUse(bb, argUses->at(i));
    }
  }
  if (uplevelUses) {
    for (int i = uplevelUses->length() - 1; i >= 0; i--) uplevelUsed->at(i)->removeUse(bb, uplevelUses->at(i));
    for (i = uplevelDefs->length() - 1; i >= 0; i--) uplevelDefd->at(i)->removeDef(bb, uplevelDefs->at(i));
  }
  NonTrivialNode::removeUses(bb);
}

void BlockCreateNode::removeUses(BB* bb) {
  if (_contextUse) _context->removeUse(bb, _contextUse);
  if (_src) _src->removeUse(bb, srcUse);
  _dest->removeDef(bb, destDef);
  NonTrivialNode::removeUses(bb);
}

void BlockMaterializeNode::removeUses(BB* bb) {
  // without memoization, BlockMaterializeNode is a no-op
  if (isMemoized()) BlockCreateNode::removeUses(bb);
}

void ContextCreateNode::removeUses(BB* bb) {
  if (_src) _src->removeUse(bb, srcUse); // no src if there's no incoming context
  _dest->removeDef(bb, destDef); 
  NonTrivialNode::removeUses(bb);
}

void ContextInitNode::removeUses(BB* bb) {
  int i = nofTemps();
  while (i-- > 0) {
    contents()->at(i)->preg()->removeDef(bb, _contentDefs->at(i));
    _initializers->at(i)->preg()->removeUse(bb, _initializerUses->at(i));
  } 
  _src->removeUse(bb, srcUse);
  NonTrivialNode::removeUses(bb);
}

void ContextZapNode::removeUses(BB* bb) {
  _src->removeUse(bb, srcUse);
  NonTrivialNode::removeUses(bb);
}

void TypeTestNode::removeUses(BB* bb) {
  _src->removeUse(bb, srcUse); 
  AbstractBranchNode::removeUses(bb); 
}

void AbstractArrayAtNode::removeUses(BB* bb) {
  _src->removeUse(bb, srcUse);
  if (_dest) _dest->removeDef(bb, destDef);
  _arg->removeUse(bb, _argUse); 
  if (_errorDef) _error->removeDef(bb, _errorDef);
  AbstractBranchNode::removeUses(bb);
}

void AbstractArrayAtPutNode::removeUses(BB* bb) {
  elem->removeUse(bb, elemUse);
  AbstractArrayAtNode::removeUses(bb);
}

void InlinedPrimitiveNode::removeUses(BB* bb) {
  if (srcUse  )	  _src ->removeUse (bb, srcUse    );
  if (_arg1_use ) _arg1->removeUse (bb, _arg1_use );
  if (_arg2_use ) _arg2->removeUse (bb, _arg2_use );
  if (destDef )	  _dest ->removeDef(bb, destDef   );
  if (_error_def) _error->removeDef(bb, _error_def);
  AbstractBranchNode::removeUses(bb);
}

// ==================================================================================
// eliminate: the receiver has just been removed because its
// result was not used.  Propagate this to all nodes whose use count
// has now become zero, too.  The register passed in is the one
// causing the elimination (passed in to catch recursive elimination
// of the same register).
// It is an error to call eliminate() if canBeEliminated() is false.
// If cp is true, the node is eliminated because of copy propagation.
// If removing is true, we're removing dead code (e.g. a branch of a type
// case that will never be executed)
// ==================================================================================

inline void Node::eliminate(BB* bb, PReg* r, bool removing, bool cp) {
  assert(!deleted, "already deleted this node");
  if (CompilerDebug) {
    char buf[1024];
    cout(PrintEliminateUnnededNodes)->print("*%s node N%ld: %s\n", removing ? "removing" : "eliminating",
					    id(), print_string(buf, PrintHexAddresses));
  }
  assert(!dontEliminate || removing, "shouldn't eliminate this node");
  bb->remove(this);
}

# define CHECK(preg, r)						      \
if (preg != r && preg->isOnlySoftUsed()) preg->eliminate(false);

inline void LoadNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  assert(canBeEliminated() || rem, "cannot be eliminated");
  NonTrivialNode::eliminate(bb, r, rem, cp); CHECK(_dest, r); }

inline void LoadOffsetNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  LoadNode::eliminate(bb, r, rem, cp); CHECK(_src, r); }

inline void LoadUplevelNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  LoadNode::eliminate(bb, r, rem, cp); CHECK(_context0, r);
}

inline void StoreNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  NonTrivialNode::eliminate(bb, r, rem, cp); CHECK(_src, r); }
inline void StoreOffsetNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  StoreNode::eliminate(bb, r, rem, cp); CHECK(_base, r); }
inline void StoreUplevelNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  StoreNode::eliminate(bb, r, rem, cp); CHECK(_context0, r);
}

inline void AssignNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  StoreNode::eliminate(bb, r, rem, cp); CHECK(_dest, r); }

inline void ReturnNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  assert(rem, "should not delete except during dead-code elimination");
  AbstractReturnNode::eliminate(bb, r, rem, cp); CHECK(_src, r); CHECK(_dest, r);
  // don't need to check resultPR
}

inline void ArithNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  NonTrivialNode::eliminate(bb, r, rem, cp); CHECK(_src, r); CHECK(_dest, r); }
inline void ArithRRNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) { 
  if (deleted) return;
  ArithNode::eliminate(bb, r, rem, cp); CHECK(_oper, r); }

inline void BranchNode::eliminate(BB* bb, PReg* r, bool removing, bool cp) {
  if (deleted) return;
  if (removing && nSuccessors() <= 1) {
    NonTrivialNode::eliminate(bb, r, removing, cp);
  } else {
    // caller has to handle this
    fatal("removing branch node with > 1 successor");
  }
}

inline void BlockMaterializeNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  BlockCreateNode::eliminate(bb, r, rem, cp);
}

void BasicNode::removeUpToMerge() {
  BB* thisBB = _bb;
  for (Node* n = (Node*)this; n && n->hasSinglePredecessor(); ) {
    while (n->nSuccessors() > 1) {
      int i = n->nSuccessors() - 1;
      Node* succ = n->next(i);
      n->removeNext(succ);
      succ->removeUpToMerge();
    }
    Node* nextn = n->next();
    if (!n->deleted) n->eliminate(thisBB, NULL, true, false);
    if (nextn) {
      BB* nextBB = nextn->bb();
      if (nextBB != thisBB) {
	if (nextn->nPredecessors() >= 2) {
	  // also remove n's successor so that we can delete past merges
	  // if all incoming branches of the merges are deleted
	  n->removeNext(nextn);
	  // nextn had at least 2 predecessors, so must stop deleting here
	  // NB: must break here -- if was 2 successors, will now be one
	  // and loop would continue (was a bug)  -Urs 8/94
	  break;
	}
	thisBB = nextBB;
      }
    }
    n = nextn;
  }
  BB* nextBB = n ? n->bb() : NULL;
}

void PrimNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  assert(rem || canBeEliminated(), "shouldn't call");
  if (nlrTestPoint()) {
    // remove all unused nodes along NLR branch
    // should be exactly 2 nodes (see IRGenerator::makeNLRPoint())
    MergeNode* n1 = nlrTestPoint();
    Node* n2 = n1->next();
    assert(n2->isNLRTestNode(), "unexpected node sequence");
    _nxt->pop();		// remove nlrTestPoint
    assert(nlrTestPoint() == NULL, "should be NULL now");
    n1->eliminate(n1->bb(), NULL, true, false);
    n2->eliminate(n2->bb(), NULL, true, false);
  }
  NonTrivialNode::eliminate(bb, r, rem, cp); 
  CHECK(_dest, r);
  if (args) {
    // check if any arg became unused
    int len = args->length();
    for (int i = 0; i < len; i++) {
      PReg* arg = args->at(i);
      if (arg->loc.isTopOfStack()) {
        // must delete this push, but node won't be eliminated with topOfStack because of side effect
	// so reset location first
	arg->loc = unAllocated;
      } else {
        fatal("internal compiler error");
        //fatal("Urs thinks all args should be topOfStack");
      }
      CHECK(arg, r);
    }
  }
}

void TypeTestNode::eliminate(BB* bb, PReg* rr, bool rem, bool cp) {
  // completely eliminate receiver and all successors
  Unused(rem); Unused(cp);
  if (deleted) return;

  eliminate(bb, rr, (ConstPReg*)NULL, (klassOop)badOop);
}

void TypeTestNode::eliminate(BB* bb, PReg* r, ConstPReg* c, klassOop theKlass) {
  // remove node and all successor branches (except for one if rcvr is known)
  if (deleted) return;
  GrowableArray<Node*>* successors = _nxt;
  _nxt = new GrowableArray<Node*>(1);
  oop constant = c ? c->constant : 0;
  Node* keep = NULL;
  if (CompilerDebug) {
    cout(PrintEliminateUnnededNodes)->print("*eliminating tt node %#lx const %#lx klass %#lx\n",
	    PrintHexAddresses ? this : 0, constant, theKlass);
  }
  Node* un = _next;		// save unknown branch
  // remove all successor nodes
  for (int i = 0; i < successors->length(); i++) {
    Node* succ = successors->at(i);
    succ->removePrev(this);
    klassOop m = _classes->at(i);
    if (constant == m || theKlass == m) {
      assert(keep == NULL, "shouldn't have more than one match");
      keep = succ;
    } else {
      _next = succ;
      _next->removeUpToMerge();
    }
  }
  
  if (keep || theKlass == klassOop(badOop)) {
    // found correct prediction, so can delete unknown branch, or
    // delete everything (theKlass == badOop)
    _next = un;
    un->removeUpToMerge();	// delete unknown branch
    _next = NULL;
  } else {
    // the type tests didn't predict for theKlass
    // (performance bug: should inline correct case since it's known now;
    // also, unknown branch may be uncommon)
    if (WizardMode) {
	warning("Compiler: typetest didn't predict klass %#lx", theKlass);
	lprintf("predicted klasses: ");
	_classes->print();
      }
    _next = un;		// keep unknown branch
  }
  assert(this == bb->last, "should end my BB");
    
  if (keep) {
    // append remaining case as fall-through
    append(keep);
  }
  NonTrivialNode::eliminate(bb, r, true, false);
  CHECK(_dest, r);
}

void TypeTestNode::eliminateUnnecessary(klassOop m) {
  // eliminate unnecessary type test: receiver is known to have map m
  eliminate(bb(), NULL, NULL, m);
}

void AbstractArrayAtNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  Unused(cp); Unused(rem);
  assert(rem, "shouldn't eliminate because of side effects (errors)");
  if (deleted) return;
  // remove fail branch nodes first
  Node* fail = next1();
  if (fail) {
    fail->removePrev(this);
    fail->removeUpToMerge();
  }
  AbstractBranchNode::eliminate(bb, r, rem, cp);
}

void InlinedPrimitiveNode::eliminate(BB* bb, PReg* r, bool rem, bool cp) {
  if (deleted) return;
  AbstractBranchNode::eliminate(bb, r, rem, cp);
  if (_arg1)  CHECK(_arg1, r);
  if (_arg2)  CHECK(_arg2, r);
  if (_dest)  CHECK(_dest, r);
  if (_error) CHECK(_error, r);
}

void ContextCreateNode::eliminate(BB* bb, PReg* r, bool removing, bool cp) {
  if (deleted) return;
  PrimNode::eliminate(bb, r, removing, cp);
}

void ContextInitNode::eliminate(BB* bb, PReg* r, bool removing, bool cp) {
  assert(removing, "should only remove when removing unreachable code");
  if (deleted) return;
  NonTrivialNode::eliminate(bb, r, removing, cp);
}

void BranchNode::eliminateBranch(int op1, int op2, int res) {
  // the receiver can be eliminated because the result it is testing
  // is a constant (res)
  bool ok;
  switch (_op) {
    case EQBranchOp:	ok = op1 == op2;	break;
    case NEBranchOp:	ok = op1 != op2;	break;
    case LTBranchOp:	ok = op1 <  op2;	break;
    case LEBranchOp:	ok = op1 <= op2;	break;
    case GTBranchOp:	ok = op1 >  op2;	break;
    case GEBranchOp:	ok = op1 >= op2;	break;
    case LTUBranchOp:	ok = (unsigned)op1 <  (unsigned)op2;	break;
    case LEUBranchOp:	ok = (unsigned)op1 <= (unsigned)op2;	break;
    case GTUBranchOp:	ok = (unsigned)op1 >  (unsigned)op2;	break;
    case GEUBranchOp:	ok = (unsigned)op1 >= (unsigned)op2;	break;
    case VSBranchOp:	return;		// can't handle yet
    case VCBranchOp:	return;		// can't handle yet
    default:		fatal("unexpected branch type");
  }
  int nodeToRemove;
  if (ok) {
    nodeToRemove = 0; // branch is taken
  } else {
    nodeToRemove = 1;
  }
  
  // discard one successor and make the remaining one the fall-thru case
  Node* discard = next(nodeToRemove);
  removeNext(discard);
  discard->removeUpToMerge();
  Node* succ = next(1 - nodeToRemove);
  removeNext(succ);
  append(succ);
  bb()->remove(this);	// delete the branch
}

// ==================================================================================
// likelySuccessor: answers the most likely successor node (or NULL). Used for better
// code positioning (determines traversal order for BBs).
// ==================================================================================

Node* Node::likelySuccessor() const { 
  assert(hasSingleSuccessor(), "should override likelySuccessor()");
  return next(); 
}

Node* TArithRRNode::likelySuccessor() const {
  return next();				// predict success 
}

Node* CallNode::likelySuccessor() const {
  return next();				// predict normal return, not NLR
}

Node* NLRTestNode::likelySuccessor() const {
  return next();				// predict home not found
}

Node* BranchNode::likelySuccessor() const {
  return next();				// predict untaken
}

Node* TypeTestNode::likelySuccessor() const {
  if (deleted) return next();
  assert(classes()->length() > 0, "no TypeTestNode needed");
  return next(hasUnknown() ? classes()->length() : classes()->length() - 1);
}

Node* AbstractArrayAtNode::likelySuccessor() const {
  return next();				// predict success 
}

Node* InlinedPrimitiveNode::likelySuccessor() const {
  return next();				// predict success 
}


// ==================================================================================
// uncommonSuccessor: answers the most uncommon successor node (or NULL). Used for
// better code positioning (determines traversal order for BBs).
// ==================================================================================

Node* Node::uncommonSuccessor() const { 
  assert(hasSingleSuccessor(), "should override uncommonSuccessor()");
  return NULL;					// no uncommon case
}

Node* TArithRRNode::uncommonSuccessor() const {
  return (deleted || !canFail()) ? NULL : next(1);		// failure case is uncommon
}

Node* CallNode::uncommonSuccessor() const {
  return NULL;					// no uncommon case (NLR is not uncommon)
}

Node* NLRTestNode::uncommonSuccessor() const {
  return NULL;					// no uncommon case (both exits are common)
}

Node* BranchNode::uncommonSuccessor() const {
  return (!deleted && _taken_is_uncommon) ? next(1) : NULL;
}

Node* TypeTestNode::uncommonSuccessor() const {
  if (!deleted && hasUnknown()) {
    // fall through case treated as uncommon case
    assert(next() != NULL, "just checking");
    return next();
  } else {
    // no unknown case, all cases are common
    return NULL;
  }
}

Node* AbstractArrayAtNode::uncommonSuccessor() const {
  return (deleted || !canFail()) ? NULL : next(1);	// failure case is uncommon
}

Node* InlinedPrimitiveNode::uncommonSuccessor() const {
  return nSuccessors() == 2 ? next(1) : NULL;	// failure case is uncommon if there
}


// ==================================================================================
// copy propagation: replace a use by another use; return false if unsuccessful
// ==================================================================================

# define CP_HELPER(_src, srcUse, return)				      \
    /* live range must be correct - otherwise reg. allocation breaks   */     \
    /* (even if doing just local CP - could fix this by checking for   */     \
    /* local conflicts when allocating PRegs, i.e. keep BB alloc info) */     \
    if (replace || (!d->loc.isTopOfStack() && d->extendLiveRange(this))) {    \
      _src->removeUse(bb, srcUse);					      \
      _src = d;								      \
      srcUse = _src->addUse(bb, this);					      \
      return true;							      \
    } else {								      \
      return false;							      \
    }

// if node can't fail anymore, remove failure branch (if not already removed)
void AbstractBranchNode::removeFailureIfPossible() {
  Node* taken = next1();
  if (!canFail() && taken != NULL && taken->bb()->isPredecessor(bb())) {
    taken->removeUpToMerge();
    removeNext(taken);
  }
}

bool BasicNode::canCopyPropagate(Node* fromNode) const {
  // current restriction: cannot copy-propagate into a loop
  // reason: copy-propagated PReg needs its live range extended to cover the entire loop, 
  // not just the stretch between fromNode and this node
  return canCopyPropagate() && fromNode->bb()->loopDepth() == _bb->loopDepth();
}
  
bool NonTrivialNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  assert(canCopyPropagate(), "can't copy-propagate");
  assert(hasSrc(), "has no src");
  if (srcUse == u) {
    CP_HELPER(_src, srcUse, return);
  } else {
    fatal("copyPropagate: not the source use");
  }
  return false;
}

bool LoadOffsetNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (u == srcUse) {
    // minor performance bug: prev. node should probably be deleted
    // (loads base reg) - eliminateUnneeded doesn't catch it - fix this
    // (happens esp. if d is a constant)
    CP_HELPER(_src, srcUse, return);
  } else {
    return LoadNode::copyPropagate(bb, u, d, replace);
  }
  return false;
  }

bool LoadUplevelNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (u == _context0Use) {
    CP_HELPER(_context0, _context0Use, return);
  } else {
    return LoadNode::copyPropagate(bb, u, d, replace);
  }
  return false;
}

bool StoreOffsetNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (u == _baseUse) {
    CP_HELPER(_base, _baseUse, return);
  } else {
    return StoreNode::copyPropagate(bb, u, d, replace);
  }
  return false;
}

bool StoreUplevelNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (u == _context0Use) {
    CP_HELPER(_context0, _context0Use, return);
  } else {
    return StoreNode::copyPropagate(bb, u, d, replace);
  }
  return false;
}

bool CallNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  //warning("fix this -- propagate args somewhere");
  return false;
}

bool  ArithRRNode::operIsConst() const { return _oper->isConstPReg(); }
int ArithRRNode::operConst() const   {
  assert(operIsConst(), "not a constant");
  return _oper->isConstPReg(); 
}
  
bool ArithNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  bool success = doCopyPropagate(bb, u, d, replace);
  if (_src->isConstPReg() && operIsConst()) {
    assert(success, "CP must have worked");
    // can constant-fold this operation
    int c1 = (int)((ConstPReg*)_src)->constant;
    int c2 = (int)operConst();
    int res;
    switch (_op) {
    case AddArithOp:
      res = c1 + c2; break;

    case SubArithOp:
      res = c1 - c2; break;
      
    case AndArithOp:		
      res = c1 & c2; break;
      
    case OrArithOp:		
      res = c1 | c2; break;
      
    case XOrArithOp:
      res = c1 ^ c2; break;
      
    default:	return success;		// can't constant-fold
    }
    
    _constResult = new_ConstPReg(scope(), (oop)res);
    // enable further constant propagation of the result
    dontEliminate = false;
    _src->removeUse(bb, srcUse);
    _src = _constResult;
    srcUse = bb->addUse(this, _src);
    
    // condition codes set -- see if there's a branch we can eliminate
    Node* branch = next();
    if (branch->isBranchNode()) {
      ((BranchNode*)branch)->eliminateBranch(c1, c2, res);
    }
  }
  return success;
}

bool ArithNode::doCopyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  return NonTrivialNode::copyPropagate(bb, u, d, replace);
}

bool ArithRRNode::doCopyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (u == _operUse) {
    CP_HELPER(_oper, _operUse, return);
  } else {
    return ArithNode::doCopyPropagate(bb, u, d, replace);
  }
  return false;
}

bool TArithRRNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  bool res = doCopyPropagate(bb, u, d, replace);
  if (_src->isConstPReg() && _oper->isConstPReg()) {
    assert(res, "CP must have worked");
    // can constant-fold this operation
    oop c1 = ((ConstPReg*)_src)->constant;
    oop c2 = ((ConstPReg*)_oper)->constant;
    oop result;
    switch (_op) {
      case tAddArithOp  : result = smiOopPrimitives_add(c1, c2);	break;
      case tSubArithOp  : result = smiOopPrimitives_subtract(c1, c2);	break;
      case tMulArithOp  : result = smiOopPrimitives_multiply(c1, c2);	break;
      case tDivArithOp  : result = smiOopPrimitives_div(c1, c2);	break;
      case tModArithOp  : result = smiOopPrimitives_mod(c1, c2);	break;
      case tAndArithOp  : result = smiOopPrimitives::bitAnd(c1, c2);	break;
      case tOrArithOp   : result = smiOopPrimitives::bitOr(c1, c2);	break;
      case tXOrArithOp  : result = smiOopPrimitives::bitXor(c1, c2);	break;
      case tShiftArithOp: warning("possible performance bug: constant folding of tShiftArithOp not implemented"); return false;
      case tCmpArithOp  : warning("possible performance bug: constant folding of tCmpArithOp not implemented"); return false;
      default           : fatal1("unknown tagged opcode %ld", _op);
    }
    bool ok = !result->is_mark();
    if (ok) {
      // constant-fold this operation
      if (CompilerDebug) cout(PrintCopyPropagation)->print("*constant-folding N%d --> %#x\n", _id, result);
      _constResult = new_ConstPReg(scope(), result);
      // first, discard the error branch (if there)
      Node* discard = next1();
      if (discard != NULL) {
	discard->bb()->remove(discard);
	discard->removeUpToMerge();
#     ifdef ASSERT
	bb->verify();
	((BB*)bb->next())->verify();
#     endif
      }
      // now, discard the overflow check
      discard = next();
      assert(discard->isBranchNode(), "must be a cond. branch");
      assert(((BranchNode*)discard)->op() == VSBranchOp, "expected an overflow check");
      discard->bb()->remove(discard);
      // and the "overflow taken" code
      discard = discard->next1();
      discard->bb()->remove(discard);
      discard->removeUpToMerge();
#     ifdef ASSERT
      bb->verify();
      ((BB*)bb->next())->verify();
#     endif
      // enable further constant propagation of the result
      dontEliminate = false;
      _src->removeUse(bb, srcUse);
      _src = _constResult;
      srcUse = bb->addUse(this, _src);
    } else {
      // for now, can't constant-fold failure - can't use marks in ConstPReg
      // (and GC can't handle markOops embedded in compiled code)
    }
  }
  removeFailureIfPossible();
  return res;
}

bool TArithRRNode::doCopyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  bool res;
  if (u == srcUse) {
    if (d->isConstPReg() && ((ConstPReg*)d)->constant->is_smi())
      _arg1IsInt = true;
    CP_HELPER(_src, srcUse, res = );
  } else if (u == _operUse) {
    if (d->isConstPReg() && ((ConstPReg*)d)->constant->is_smi())
      _arg2IsInt = true;
    CP_HELPER(_oper, _operUse, res =);
  } else {
    fatal("copyPropagate: not the source use");
  }
  removeFailureIfPossible();
  return res;
}

bool FloatArithRRNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (d->isConstPReg() && !((ConstPReg*)d)->constant->is_double()) {
    // can't handle non-float arguments (don't optimize guaranteed failure)
    return false;
  }
  bool res = ArithRRNode::copyPropagate(bb, u, d, replace);
  // should check for constant folding opportunity here -- fix this
  return res;
}

bool FloatUnaryArithNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (d->isConstPReg() && !((ConstPReg*)d)->constant->is_double()) {
    // can't handle non-float arguments (don't optimize guaranteed failure)
    return false;
  }
  bool res = ArithNode::copyPropagate(bb, u, d, replace);
  // should check for constant folding opportunity here -- fix this
  return res;
}

bool TypeTestNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (u == srcUse) {
    if (d->isConstPReg()) {
      // we know the receiver - the type test is unnecessary!
      ConstPReg* c = (ConstPReg*)d;
      // The code below has been disabled by Lars Bak 4-19-96 per request by Urs
      // Cause:
      //   Something may go wrong when deleting the non-integer branch messing up
      //   the node graph.
      if (false) {
        eliminate(bb, NULL, c, c->constant->klass());
        return true;
      }
      return false;
    } else {
      CP_HELPER(_src, srcUse, return);
    }
  } else {
    fatal("don't have this use");
  }
  return false;
}

bool AbstractArrayAtNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  if (u == _argUse) {
    bool res;
    CP_HELPER(_arg, _argUse, res =);
    removeFailureIfPossible();
    return res;
  } else {
    return AbstractBranchNode::copyPropagate(bb, u, d, replace);
  }
  return false;
}

bool AbstractArrayAtPutNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  bool res;
  if (u == _argUse) {
    CP_HELPER(_arg, _argUse, res = );
  } else if (u == elemUse) {
    CP_HELPER(elem, elemUse, res =);
  } else {
    return AbstractBranchNode::copyPropagate(bb, u, d, replace);
  } 
  removeFailureIfPossible();
  return res;
}

bool InlinedPrimitiveNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  // copyPropagate should be fairly easy to put in, right now it is doing nothing.
  return false;
}

bool ContextInitNode::copyPropagate(BB* bb, Use* u, PReg* d, bool replace) {
  for (int i = nofTemps() - 1; i >= 0; i--) {
    if (_initializerUses->at(i) == u) {
      Expr* initExpr = _initializers->at(i);
      PReg* initPR = initExpr->preg();
      Use* newUse = u;
      bool ok;
      CP_HELPER(initPR, newUse, ok = );
      if (ok) {
	assert(newUse != u, "must have new use");
	_initializers->at_put(i, initExpr->shallowCopy(d, initExpr->node()));
	_initializerUses->at_put(i, newUse);
      }
      return ok;
    }
  }
  return NonTrivialNode::copyPropagate(bb, u, d, replace);
}

// ==================================================================================
// markAllocated: for all PRegs used/defined by the node, increment the
// corresponding counters if the PReg is already allocated
// ==================================================================================

# define U_CHECK(r) if (r->loc.isRegisterLocation()) use_count[r->loc.number()]++
# define D_CHECK(r) if (r->loc.isRegisterLocation()) def_count[r->loc.number()]++

void LoadNode::markAllocated(int* use_count, int* def_count) {
  Unused(use_count);
  D_CHECK(_dest);
}
void LoadOffsetNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_src); LoadNode::markAllocated(use_count, def_count);
}
void LoadUplevelNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_context0); LoadNode::markAllocated(use_count, def_count);
}

void StoreNode::markAllocated(int* use_count, int* def_count) {
  Unused(def_count);
  U_CHECK(_src);
}
void StoreOffsetNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_base); StoreNode::markAllocated(use_count, def_count);
}
void StoreUplevelNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_context0); StoreNode::markAllocated(use_count, def_count);
}
void AssignNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_src); D_CHECK(_dest);
}

void ReturnNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_src); D_CHECK(_dest);
  AbstractReturnNode::markAllocated(use_count, def_count);
}

void ArithNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_src); D_CHECK(_dest); 
}

void ArithRRNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_oper); ArithNode::markAllocated(use_count, def_count);
}

void TArithRRNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_src); D_CHECK(_dest); 
  U_CHECK(_oper); 
}

void CallNode::markAllocated(int* use_count, int* def_count) {
  D_CHECK(_dest);
  // CallNode trashes all regs
  for (int i = 0; i < nofRegisters; i++) { use_count[i]++; def_count[i]++; }
}

void BlockCreateNode::markAllocated(int* use_count, int* def_count) {
  if (_src) U_CHECK(_src);
  if (_context) U_CHECK(_context);
  PrimNode::markAllocated(use_count, def_count);
}

void BlockMaterializeNode::markAllocated(int* use_count, int* def_count) {
  if (isMemoized()) BlockCreateNode::markAllocated(use_count, def_count);
}

void ContextCreateNode::markAllocated(int* use_count, int* def_count) {
  if (_src) U_CHECK(_src); // no src if there's no incoming context
  if (_parentContexts) {
    for (int i = _parentContexts->length() - 1; i>= 0; i--) {
      U_CHECK(_parentContexts->at(i));
    }
  }
  PrimNode::markAllocated(use_count, def_count);
};

void ContextInitNode::markAllocated(int* use_count, int* def_count) {
  if (_src) U_CHECK(_src);
  int i = nofTemps();
  while (i-- > 0) {
    D_CHECK(contents()->at(i)->preg());
  }
}

void ContextZapNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_src);
}

void TypeTestNode::markAllocated(int* use_count, int* def_count) {
  Unused(def_count);
  U_CHECK(_src);
}

void AbstractArrayAtNode::markAllocated(int* use_count, int* def_count) {
  U_CHECK(_src);
  if (_dest) D_CHECK(_dest);
  U_CHECK(_arg);
  if (_error) D_CHECK(_error);
}

void AbstractArrayAtPutNode::markAllocated(int* use_count, int* def_count) {
  if (elem) U_CHECK(elem);
  AbstractArrayAtNode::markAllocated(use_count, def_count);
}

void InlinedPrimitiveNode::markAllocated(int* use_count, int* def_count) {
  if (_src  ) U_CHECK(_src  );
  if (_arg1 ) U_CHECK(_arg1 );
  if (_arg2 ) U_CHECK(_arg2 );
  if (_dest ) D_CHECK(_dest );
  if (_error) D_CHECK(_error);
}

// trashedMask: return bit mask of trashed registers
SimpleBitVector BasicNode::trashedMask() 	{ return SimpleBitVector(0); }
SimpleBitVector CallNode::trashedMask() 	{ return SimpleBitVector(-1); }

// ==================================================================================
// computeEscapingBlocks: find escaping blocks
// ==================================================================================

inline void computeEscapingBlocks(Node* n, PReg* src, GrowableArray<BlockPReg*>* l, char* msg) {
  // helper function for computing exposed blocks
  if (src->isBlockPReg()) {
    BlockPReg* r = (BlockPReg*)src;
    r->markEscaped(n);
    if (!l->contains(r)) l->append(r);
    if (msg) theCompiler->reporter->report_block(n, r, msg);
  }
}

void StoreNode::computeEscapingBlocks(GrowableArray<BlockPReg*>* ll) {
  // Any store is considered to expose a block -- even if it's a store into a local.
  // That's pessimistic, but simple.  
  ::computeEscapingBlocks(this, _src, ll, action());
}

void AbstractReturnNode::computeEscapingBlocks(GrowableArray<BlockPReg*>* ll) {
  // a block returned by a nmethod escapes
  if (_src) ::computeEscapingBlocks(this, _src, ll, "returned");
}

void CallNode::computeEscapingBlocks(GrowableArray<BlockPReg*>* ll) {
  SubclassResponsibility();
}

void SendNode::computeEscapingBlocks(GrowableArray<BlockPReg*>* ll) {
  // all arguments to a non-inlined call escape
  if (exprStack && (args != NULL)) {
    // if the receiver is not pushed on the exprStack (self/super sends),
    // the exprStack is by 1 shorter than the args array
    // (exprStack may be longer than that, so just look at top elems)
    int len = exprStack->length();
    int i = min(args->length(), len);
    while (i-- > 0) {
      ::computeEscapingBlocks(this, exprStack->at(--len), ll, "exposed by a send");
    }
  }
}

void PrimNode::computeEscapingBlocks(GrowableArray<BlockPReg*>* ll) {
  // assume that all block arguments to a primitive call escape
  if (exprStack) {
    int len = exprStack->length();
    int i = min(len, _pdesc->number_of_parameters());
    while (i-- > 0) {
      ::computeEscapingBlocks(this, exprStack->at(--len), ll, "exposed by a primitive call");
    }
  }
}

void DLLNode::computeEscapingBlocks(GrowableArray<BlockPReg*>* ll) {
  // assume that all block arguments to a DLL call escape
  if (exprStack) {
    int len = exprStack->length();
    int i = min(len, nofArguments());
    while (i-- > 0) {
      ::computeEscapingBlocks(this, exprStack->at(--len), ll, "exposed by a DLL call");
    }
  }
}

void ContextInitNode::computeEscapingBlocks(GrowableArray<BlockPReg*>* ll) {
  // all blocks stored into a context escape
  // (later phase will recognize if context isn't created --> block doesn't really escape)
  int i = nofTemps();
  while (i-- > 0) {
    Expr* e = _initializers->at(i);
    if (e) ::computeEscapingBlocks(this, e->preg(), ll, NULL);
  }
}

void ArrayAtPutNode::computeEscapingBlocks(GrowableArray<BlockPReg*>* ll) {
  // all blocks stored into an array escape
  ::computeEscapingBlocks(this, elem, ll, "stored into an array");
}


// ==================================================================================
// machine-independent routines for code generation
// ==================================================================================

bool TypeTestNode::needsKlassLoad() const {
  // a test needs a klass load if it tests for any non-smi/bool/nil klass
  const int len = _hasUnknown ? _classes->length() : _classes->length() - 1;
  for (int i = 0; i < len; i++) {
    klassOop klass = _classes->at(i);
    if (klass != trueObj->klass() &&
        klass != falseObj->klass() &&
	klass != nilObj->klass() &&
	klass != smiKlassObj) {
      return true;
    }
  }
  return false;
}

static bool hasUnknownCode(Node* n) {
  while (n->isTrivial()) n = n->next();
  return !n->isUncommonNode();
}


bool TypeTestNode::hasUnknownCode() const {
  if (!_hasUnknown) return false;     // no unknown type
  return ::hasUnknownCode(next());
}

bool TArithRRNode::hasUnknownCode() const {
  return ::hasUnknownCode(next1());
}

bool AbstractArrayAtNode::hasUnknownCode() const {
  return ::hasUnknownCode(next1());
}

Node* TypeTestNode::smiCase() const {
  int i = _classes->length();
  while (i-- > 0) {
    if (_classes->at(i) == smiKlassObj) return next(i+1);
  }
  return NULL;
}

// ==================================================================================
// integer loop optimization
// ==================================================================================

LoopHeaderNode::LoopHeaderNode() { 
  _activated = false; _integerLoop = false; _tests = NULL; 
  _enclosingLoop = NULL; _nestedLoops = NULL; 
  _nofCalls = 0; _registerCandidates = NULL;
}


void LoopHeaderNode::activate(PReg* loopVar, PReg* lowerBound, PReg* upperBound, LoadOffsetNode* loopSizeLoad) {
  _activated     = true;
  _integerLoop   = true;
  _loopVar       = loopVar; 
  _lowerBound    = lowerBound; 
  _upperBound    = upperBound; 
  _upperLoad     = loopSizeLoad;
  _arrayAccesses = new GrowableArray<AbstractArrayAtNode*>(10);
}

void LoopHeaderNode::activate() {
  _activated = true;
  assert(_tests, "should have type tests");
  _loopVar = _lowerBound = _upperBound = NULL; 
  _upperLoad = NULL;
  _arrayAccesses = NULL;
}

void LoopHeaderNode::addArray(AbstractArrayAtNode* n) {
  assert(_activated, "shouldn't call");
  _arrayAccesses->append(n);
}

void LoopHeaderNode::set_enclosingLoop(LoopHeaderNode* l) {
  assert(_enclosingLoop == NULL, "already set");
  _enclosingLoop = l;
}

void LoopHeaderNode::addNestedLoop(LoopHeaderNode* l) {
  if (_nestedLoops == NULL) _nestedLoops = new GrowableArray<LoopHeaderNode*>(5);
  _nestedLoops->append(l);
}

void LoopHeaderNode::addRegisterCandidate(LoopRegCandidate* c) {
  if (_registerCandidates == NULL) _registerCandidates = new GrowableArray<LoopRegCandidate*>(2);
  _registerCandidates->append(c);
}



inline bool is_smi_type(GrowableArray<klassOop>* klasses) {
  return klasses->length() == 1 && klasses->at(0) == smiKlassObj;
}

inline GrowableArray<klassOop>* make_smi_type() {
  GrowableArray<klassOop>* t = new GrowableArray<klassOop>(1);
  t->append(smiKlassObj);
  return t;
}

void StoreNode::assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n) {
  if (is_smi_type(klasses) && r == src()) {
    if (CompilerDebug) cout(PrintLoopOpts)->print("*removing store check from N%d\n", id());
    setStoreCheck(false);
  } 
}

void AbstractArrayAtNode::assert_in_bounds(PReg* r, LoopHeaderNode* n) {
  if (r == _arg) {
    if (CompilerDebug && _needBoundsCheck) cout(PrintLoopOpts)->print("*removing bounds check from N%d\n", id());
    _needBoundsCheck = false;
    removeFailureIfPossible();
  }
}

void AbstractArrayAtNode::collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const {
  // ArrayAt node tests index for smi-ness
  regs.append(_arg);
  klasses.append(make_smi_type());
}

void AbstractArrayAtNode::assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n) {
  if (is_smi_type(klasses) && r == _arg) {
    if (CompilerDebug && !_intArg) cout(PrintLoopOpts)->print("*removing index tag check from N%d\n", id());
    _intArg = true;
    n->addArray(this);
    removeFailureIfPossible();
  } else if (r != _arg) {
    fatal("array can't be an integer");
  }
}

void ArrayAtPutNode::collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const {
  // atPut node tests element for smi-ness if character array
  AbstractArrayAtNode::collectTypeTests(regs, klasses);
  if (stores_smi_elements(_access_type)) {
    regs.append(elem);
    assert(klasses.first()->first() == smiKlassObj, "must be smi type for index");
    klasses.append(klasses.first());	// reuse smi type descriptor
  }
}

void ArrayAtPutNode::assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n) {
  if (is_smi_type(klasses) && r == elem) {
    if (CompilerDebug && _needs_store_check) 
      cout(PrintLoopOpts)->print("*removing array store check from N%d\n", id());
    _needs_store_check = false;
    removeFailureIfPossible();
  } else if (r == _arg) {
    AbstractArrayAtPutNode::assert_preg_type(r, klasses, n);
  }
}

void TArithRRNode::collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const {
  // tests receiver and/or arg for smi-ness
  if (canFail()) {
    GrowableArray<klassOop>* t = make_smi_type();
    if (!_arg1IsInt) {
      regs.append(_src);
      klasses.append(t);
    }
    if (!_arg2IsInt) {
      regs.append(_oper);
      klasses.append(t);
    }
  }
}

void TArithRRNode::assert_preg_type(PReg* r, GrowableArray<klassOop>* klasses, LoopHeaderNode* n) {
  if (is_smi_type(klasses) && r == _src) {
    if (CompilerDebug && !_arg1IsInt) cout(PrintLoopOpts)->print("*removing arith arg1 tag check from N%d\n", id());
    _arg1IsInt = true;
  } 
  if (is_smi_type(klasses) && r == _oper) {
    if (CompilerDebug && !_arg2IsInt) cout(PrintLoopOpts)->print("*removing arith arg2 tag check from N%d\n", id());
    _arg2IsInt = true;
  }
  removeFailureIfPossible();
}

void TypeTestNode::collectTypeTests(GrowableArray<PReg*>& regs, GrowableArray<GrowableArray<klassOop>*>& klasses) const {
  regs.append(_src);
  klasses.append(_classes);
}

void TypeTestNode::assert_preg_type(PReg* r, GrowableArray<klassOop>* k, LoopHeaderNode* n) {
  assert(r == src(), "must be source");
  if (k->length() == 1) {
    // common case: tests for one klass --> can be eliminated
    eliminateUnnecessary(k->at(0));
  } else {
    // can at least eliminate uncommon unknown case
    assert(k->length() <= _classes->length(), "type cannot widen");
    setUnknown(false);
  }
}

// ==================================================================================
// printing code (for debugging)
// ==================================================================================

#ifdef PRODUCT
  #define DISABLED_IN_PRODUCT return buf;
#else
  #define DISABLED_IN_PRODUCT
#endif


const int PrintStringLen = 40;	// width of output before printing address

void BasicNode::print_short()	{ char buf[1024]; lprintf(print_string(buf, PrintHexAddresses)); }
static int id_of(Node* node)	{ return node == NULL ? -1 : node->id(); }

char* PrologueNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "Prologue");
  if (printAddr) my_sprintf(buf, "((PrologueNode*)%#lx", this);
  return b;
}

char* InterruptCheckNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "InterruptCheckNode");
  if (printAddr) my_sprintf(buf, "((InterruptCheckNode*)%#lx)", this);
  return b;
}

char* LoadOffsetNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "LoadOffset %s := %s[%#lx]", _dest->safeName(), _src->safeName(), offset);
  if (printAddr) my_sprintf(buf, "((LoadOffsetNode*)%#lx)", this);
  return b;
}

char* LoadIntNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "LoadInt %s := %#lx", _dest->safeName(), _value);
  if (printAddr) my_sprintf(buf, "((LoadIntNode*)%#lx)", this);
  return b;
}

char* LoadUplevelNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "LoadUpLevel %s := %s^%d[%d]", _dest->safeName(), 
    		 _context0->safeName(), _nofLevels, _offset);
  if (printAddr) my_sprintf(buf, "((LoadUplevelNode*)%#lx)", this);
  return b;
}

char* StoreOffsetNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "StoreOffset %s[%#lx] := %s", _base->safeName(), _offset, _src->safeName());
  if (printAddr) my_sprintf(buf, "((StoreOffsetNode*)%#lx)", this);
  return b;
}

char* StoreUplevelNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "StoreUpLevel %s^%d[%d] := %s", 
		 _context0->safeName(), _nofLevels, _offset, _src->safeName());
  if (printAddr) my_sprintf(buf, "((StoreUplevelNode*)%#lx)", this);
  return b;
}
  
char* AssignNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "%s := %s", _dest->safeName(), _src->safeName());
  if (printAddr) my_sprintf(buf, "((AssignNode*)%#lx)", this);
  return b;
}

char* SendNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf,PrintStringLen, "Send %s NLR %ld ", _key->print_string(), id_of(nlrTestPoint()));
  if (printAddr) my_sprintf(buf, "((SendNode*)%#lx)", this);
  return b;
}


char* PrimNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "PrimCall _%s NLR %ld", _pdesc->name(), id_of(nlrTestPoint()));
  if (printAddr) my_sprintf(buf, "((PrimNode*)%#lx)", this);
  return b;
  }


char* DLLNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "DLLCall <%s, %s> NLR %ld", _dll_name->as_string(), _function_name->as_string(), id_of(nlrTestPoint()));
  if (printAddr) my_sprintf(buf, "((DLLNode*)%#lx)", this);
  return b;
}


char* BlockCreateNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "BlockCreate %s", _dest->safeName());
  if (printAddr) my_sprintf(buf, "((BlockCreateNode*)%#lx)", this);
  return b;
}

char* BlockMaterializeNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "BlockMaterialize %s", _dest->safeName());
    if (printAddr) my_sprintf(buf, "((BlockMaterializeNode*)%#lx)", this);
  return b;
}

char* InlinedReturnNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "InlinedReturn %s := %s", _dest->safeName(), _src->safeName());
  if (printAddr) my_sprintf(buf, "((InlinedReturnNode*)%#lx)", this);
  return b;
}

char* NLRSetupNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "NLReturn %s := %s", _dest->safeName(), _src->safeName());
  if (printAddr) my_sprintf(buf, "((NLRSetupNode*)%#lx)", this);
  return b;
}

char* NLRContinuationNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "NLR Continuation");
  if (printAddr) my_sprintf(buf, "((NLRContinuationNode*)%#lx)", this);
  return b;
}

char* ReturnNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "MethodReturn  %s", _src->safeName());
  if (printAddr) my_sprintf(buf, "((ReturnNode*)%#lx)", this);
  return b;
}

char* NLRTestNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "NLRTest  N%ld N%ld", id_of(next1()), id_of(next()));
  if (printAddr) my_sprintf(buf, "((NLRTestNode*)%#lx)", this);
  return b;
}

char* ArithNode::opName() const { return ArithOpName[_op]; }

char* ArithRRNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "%s := %s %s %s", 
    		 _dest->safeName(), _src->safeName(), opName(), _oper->safeName());
  if (printAddr) my_sprintf(buf, "((ArithRRNode*)%#lx)", this);
  return b;
}

char* FloatArithRRNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "%s := %s %s %s", 
    		 _dest->safeName(), _src->safeName(), opName(), _oper->safeName());
  if (printAddr) my_sprintf(buf, "((FloatArithRRNode*)%#lx)", this);
  return b;
}

char* FloatUnaryArithNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "%s := %s %s", 
    		 _dest->safeName(), opName(), _src->safeName());
  if (printAddr) my_sprintf(buf, "((FloatUnaryArithNode*)%#lx)", this);
  return b;
}

char* TArithRRNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "%s := %s %s %s   N%d, N%d",
		 _dest->safeName(), _src->safeName(), ArithOpName[_op], _oper->safeName(),
		 id_of(next1()), id_of(next()));
  if (printAddr) my_sprintf(buf, "((TArithRRNode*)%#lx)", this);
  return b;
}

char* ArithRCNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "%s := %s %s %#lx",
		 _dest->safeName(), _src->safeName(), opName(), _oper);
  if (printAddr) my_sprintf(buf, "((ArithRCNode*)%#lx)", this);
  return b;
}

char* BranchNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "%s  N%ld N%ld", BranchOpName[_op], id_of(next1()), id_of(next()));
  if (printAddr) my_sprintf(buf, "((BranchNode*)%#lx)", this);
  return b;
}

char* TypeTestNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf(buf, "TypeTest %s, ", _src->safeName());
  for (int i = 1; i <= _classes->length(); i++) {
    klassOop m = _classes->at(i-1);
    my_sprintf(buf, m->print_value_string());
    my_sprintf(buf, ": N%ld; ", (i < nSuccessors() && next(i) != NULL) ? next(i)->id() : -1);
  }
  my_sprintf_len(buf, b + PrintStringLen - buf,
		 "N%ld%s", id_of(next()),
		 _hasUnknown ? "" : "*");
  if (printAddr) my_sprintf(buf, "((TypeTestNode*)%#lx)", this);
  return b;
}

char* ArrayAtNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "ArrayAt %s := %s[%s]",
		 _dest->safeName(), _src->safeName(), _arg->safeName());
  if (printAddr) my_sprintf(buf, "((ArrayAtNode*)%#lx)", this);
  return b;
}

char* ArrayAtPutNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "ArrayAtPut %s[%s] := %s",
	   	   _src->safeName(), _arg->safeName(), elem->safeName());
  if (printAddr) my_sprintf(buf, "((ArrayAtPutNode*)%#lx)", this);
  return b;
}

char* FixedCodeNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "DeadEnd");
  if (printAddr) my_sprintf(buf, "((FixedCodeNode*)%#lx)", this);
  return b;
}

static int prevsLen;
static char* mergePrintBuf;
static void printPrevNodes(Node* n) {
  my_sprintf(mergePrintBuf, "N%ld%s", id_of(n), --prevsLen > 0 ? ", " : "");
}

char* MergeNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf(buf, "Merge ");
  prevsLen = _prevs->length();
  mergePrintBuf = buf;
  _prevs->apply(printPrevNodes);
  buf = mergePrintBuf;
  my_sprintf_len(buf, b + PrintStringLen - buf, " ");
  if (printAddr) my_sprintf(buf, "((MergeNode*)%#lx)", this);
  return b;
}

char* LoopHeaderNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf(buf, "LoopHeader ");
  if (_activated) {
    if (_integerLoop) {
      my_sprintf(buf, "int ");
      my_sprintf(buf, "%s=[%s..%s] ", _loopVar->safeName(), _lowerBound->safeName(), 
		 _upperBound ? _upperBound->safeName() : _upperLoad->base()->safeName());
    }
    if (_registerCandidates != NULL) {
      my_sprintf(buf, "reg vars = ");
      for (int i = 0; i < _tests->length(); i++) my_sprintf(buf, "%s ", _registerCandidates->at(i)->preg()->name());
    }
    if (_tests != NULL) {
      for (int i = 0; i < _tests->length(); i++) {
        HoistedTypeTest* t = _tests->at(i);
	if (t->testedPR->loc != unAllocated) {
	  stringStream s(50);
	  t->print_test_on(&s);
	  my_sprintf(buf, "%s ", s.as_string());
	}
      }
    }
    my_sprintf_len(buf, PrintStringLen - (buf - b), " ");
  } else {
    my_sprintf_len(buf, PrintStringLen - 11, "(inactive)");
  }
  if (printAddr) my_sprintf(buf, "((LoopHeaderNode*)%#lx)", this);
  return b;
}

char* ContextCreateNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "Create Context %s", _dest->safeName());
  if (printAddr) my_sprintf(buf, "((ContextCreateNode*)%#lx)", this);
  return b;
}

char* ContextInitNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf(buf, "Initialize context ");
  if (_src == NULL) {
    my_sprintf(buf, "(optimized away) ");
  } else {
    my_sprintf(buf, "%s { ", _src->safeName());
    for (int i = 0; i < contents()->length(); i++) {
      my_sprintf(buf, " %s := ", contents()->at(i)->preg()->safeName());
      Expr* e = _initializers->at(i);
      my_sprintf(buf, " %s; ", e->preg()->safeName());
    }
  }
  my_sprintf_len(buf, b + PrintStringLen - buf, "}");
  if (printAddr) my_sprintf(buf, "((ContextInitNode*)%#lx)", this);
  return b;
}

char* ContextZapNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "Zap Context %s", isActive() ? context()->safeName() : "- inactive");
  if (printAddr) my_sprintf(buf, "((ContextZapNode*)%#lx)", this);
  return b;
}

char* InlinedPrimitiveNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf(buf, "%s := ", _dest->safeName());
  char* op_name;
  switch (_op) {
    case obj_klass		: op_name = "obj_klass";		break;
    case obj_hash		: op_name = "obj_hash";			break;
    case proxy_byte_at		: op_name = "proxy_byte_at";		break;
    case proxy_byte_at_put	: op_name = "proxy_byte_at_put";	break;
    default			: op_name = "*** unknown primitive ***";break;
  }
  my_sprintf(buf, "%s(", op_name);
  my_sprintf(buf, " %s", _src ->safeName());
  my_sprintf(buf, " %s", _arg1->safeName());
  my_sprintf(buf, " %s", _arg2->safeName());
  my_sprintf_len(buf, b + PrintStringLen - buf, ")");
  if (printAddr) my_sprintf(buf, "((InlinedPrimitiveNode*)%#lx)", this);
  return b;
}

char* UncommonNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "UncommonBranch");
  if (printAddr) my_sprintf(buf, "((UncommonNode*)%#lx)", this);
  return b;
}

char* NopNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "Nop");
  if (printAddr) my_sprintf(buf, "((NopNode*)%#lx)", this);
  return b;
}

char* CommentNode::print_string(char* buf, bool printAddr) const {
  DISABLED_IN_PRODUCT
  char* b = buf;
  my_sprintf_len(buf, PrintStringLen, "'%s' ", comment);
  if (printAddr) my_sprintf(buf, "((CommentNode*)%#lx)", this);
  return b;
}

void BasicNode::printID() const {
  lprintf("%4ld:%1s %-4s", id(), deleted ? "D" : " ", " ");
    //c hasSplitSig() ? splitSig()->prefix(buf) : " ");
}

void Node::verify() const {
  if (deleted) return;
  if (!firstPrev() && !isPrologueNode())
    error("Node %#lx: no predecessor", this);
  if (firstPrev() && !firstPrev()->isSuccessor(this))
    error("prev->next != this for Node %#lx", this);
  if (_bb && !_bb->contains(this))
    error("BB doesn't contain Node %#lx", this);
  if (next() && ! next()->isPredecessor(this))
    error("next->prev != this for Node %#lx", this);
  if (bbIterator->blocksBuilt && _bb == NULL) error("Node %#lx: doesn't belong to any BB", this);
  if (next() == NULL && !isExitNode() &&
      !isCommentNode())   // for the "rest of method omitted (dead)" comment
    error("Node %#lx has no successor", this);
  if (next() != NULL && isExitNode()) {
    for (Node* n = next();
	 n && (n->isCommentNode() || n->isDeadEndNode());
	 n = n->next()) ;
      if (n) error("exit node %#lx has a successor (%#lx)", this, next());
  }
}

void NonTrivialNode::verify() const {
  if (deleted) return;
  if (hasSrc()) src()->verify();
  if (hasDest()) {
    dest()->verify();
    if (dest()->isConstPReg()) {
      error("Node %#lx: dest %#lx is ConstPR", this, dest());
    }
  }
  if (isAssignmentLike() && (!hasSrc() || !hasDest()))
    error("Node %#lx: isAssignmentLike() implies hasSrc/Dest", this);
  Node::verify();
}

void LoadOffsetNode::verify() const {
  if (deleted) return;
  NonTrivialNode::verify();
  base()->verify();
  if (offset < 0) error("Node %#lx: offset must be >= 0", this);
}
  
void LoadUplevelNode::verify() const {
  if (deleted) return;
  if (_context0 == NULL) error("Node %#lx: context0 is NULL", this);
  if (_nofLevels < 0)    error("Node %#lx: nofLevels must be >= 0", this);
  if (_offset < 0)       error("Node %#lx: offset must be >= 0", this);
  NonTrivialNode::verify();
  _context0->verify();
}

void StoreOffsetNode::verify() const {
  if (deleted) return;
  NonTrivialNode::verify();
  base()->verify();
  if (_offset < 0) error("Node %#lx: offset must be >= 0", this);
}

void StoreUplevelNode::verify() const {
  if (deleted) return;
  if (_context0 == NULL) error("Node %#lx: context0 is NULL", this);
  if (_nofLevels < 0)    error("Node %#lx: nofLevels must be > 0", this);
  if (_offset < 0)       error("Node %#lx: offset must be >= 0", this);
  NonTrivialNode::verify();
  _context0->verify();
}

void MergeNode::verify() const {
  if (deleted) return;
  if (isLoopStart && isLoopEnd) error("MergeNode %#x: cannot be both start and end of loop");
  TrivialNode::verify();
}

void BlockCreateNode::verify() const {
  if (deleted) return;
  PrimNode::verify();
}

void ReturnNode::verify() const {
  if (deleted) return;
  AbstractReturnNode::verify();
  if (next()) error("ReturnNode %#lx has a successor", this);
}

void NLRSetupNode::verify() const {
  if (deleted) return;
  AbstractReturnNode::verify();
  if (next()) error("NLRSetupNode %#lx has a successor", this);
}

void NLRContinuationNode::verify() const {
  if (deleted) return;
  AbstractReturnNode::verify();
  if (next()) error("NLRContinuationNode %#lx has a successor", this);
}

void NLRTestNode::verify() const {
  if (deleted) return;
  AbstractBranchNode::verify(false);
  if (next() == NULL)  error("NLRTestNode %#lx has no continue-NLR node", this);
  if (next1() == NULL) error("NLRTestNode %#lx has no end-of-NLR node", this);
}

void InlinedReturnNode::verify() const {
  if (deleted) return;
  AbstractReturnNode::verify();
  if (!next()) { 
    error("InlinedReturnNode %#lx has no successor", this);
  } else {
    Node* nextAfterMerge = next()->next();
    if (nextAfterMerge->scope() == scope())
      error("InlinedReturnNode %#lx: successor is in same scope", this);
  }
}

void ContextCreateNode::verify() const {
  PrimNode::verify();
}

void ContextInitNode::verify() const {
  if (deleted) return;
  int n = nofTemps();
  if ((n != contents()->length()) ||
      (n != _initializers->length()) ||
      (_contentDefs != NULL) && (n != _contentDefs->length()) ||
      (_initializerUses != NULL) && (n != _initializerUses->length())) {
    error("ContextInitNode %#lx: bad nofTemps %d", this, n);
  }
  int i = nofTemps();
  while (i-- > 0) {
    Expr* e = _initializers->at(i);
    if (e != NULL) e->verify();
    contents()->at(i)->verify();
    PReg* r = contents()->at(i)->preg();
    if (_src == NULL && r->loc.isContextLocation()) {
      ((ContextInitNode*)this)->print();
      scope()->print();
      error("ContextInitNode %#lx: context eliminated but temp %d is context location", this, i);
    }
    // isInContext is accessing _isInContext which is never set (and thus always 0 initially)
    // Should check if we're missing something here or if we can remove the code completely.
    // - gri 9/10/96
    /*
    if (r->isSAPReg() && !r->isBlockPReg() && !((SAPReg*)r)->isInContext()) {
      // I'm not sure what the isInContext() flag is supposed to do....but the error is triggered
      // in the test suite (when running the standard tests).  Could be because of copy propagation.
      // If the assertion does make sense, please also put it in ContextInitNode::initialize().
      // (But please turn the condition above into a method, don't duplicate it.)
      // Urs 9/8/96
      // error("ContextInitNode %#lx: temp %d is non-context SAPReg %s", this, i, r->safeName());
    }
    */
  }
}

void ContextZapNode::verify() const {
  if (deleted) return;
  if (_src != scope()->context()) {
    error("ContextZapNode %#lx: wrong context %#lx", this, _src);
  }
  NonTrivialNode::verify();
}

void CallNode::verify() const {
  if (deleted) return;
  if ((exprStack != NULL) && (args != NULL)) {
    if (exprStack->length() + 1 < args->length()) {
      error("CallNode %#lx: exprStack is too short", this);
    }
  }
}

void ArithRRNode::verify() const {
  if (deleted) return;
  ArithNode::verify();
  _oper->verify();
}

void TArithRRNode::verify() const {
  if (deleted) return;
  AbstractBranchNode::verify(true);
  if ((_op < tAddArithOp) || (tCmpArithOp < _op)) {
    error("TArithRRNode %#lx: wrong opcode %ld", this, _op);
  }
}

void FloatArithRRNode::verify() const {
  if (deleted) return;
  ArithRRNode::verify();
  // fix this -- check opcode
}

void FloatUnaryArithNode::verify() const {
  if (deleted) return;
  ArithNode::verify();
  // fix this -- check opcode
}

void AbstractBranchNode::verify(bool verifySuccessors) const {
  if (deleted) return;
  NonTrivialNode::verify();
  if (verifySuccessors && !canFail() && failureBranch() != NULL) {
    error("Node %#x: cannot fail, but failure branch is still there", this);
  }
}

void InlinedPrimitiveNode::verify() const {
  if (deleted) return;
  AbstractBranchNode::verify(true);
  // fix this - check node
}

void UncommonNode::verify() const {
  if (deleted) return;
  if ((Node*)this != bb()->last)
    error("UncommonNode %#lx: not last node in BB", this);
  NonTrivialNode::verify();
}

void TypeTestNode::verify() const {
  if (deleted) return;
  if ((Node*)this != bb()->last)
    error("TypeTestNode %#lx: not last node in BB", this);
  NonTrivialNode::verify();
}


// for debugging
void printNodes(Node* n) {
  for ( ; n; n = n->next()) {
    n->printID(); n->print_short(); lprintf("\n");
  }
}
  
# endif
