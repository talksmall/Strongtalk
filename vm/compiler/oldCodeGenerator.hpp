/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.3 $ */
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

#ifdef DELTA_COMPILER

// The OldCodeGenerator is the new interface to the gen() routines in x86_node.cpp.
// It allows to call the old code generation routines via the new apply method.

class OldCodeGenerator: public NodeVisitor {
 public:
  // Basic blocks
  void beginOfBasicBlock(Node* node);
  void endOfBasicBlock(Node* node);
  
 public:
  // For all nodes
  void beginOfNode(Node* node);
  void endOfNode(Node* node);

 public:
  // Individual nodes
  void aPrologueNode(PrologueNode* node);

  void aLoadIntNode(LoadIntNode* node);
  void aLoadOffsetNode(LoadOffsetNode* node);
  void aLoadUplevelNode(LoadUplevelNode* node);

  void anAssignNode(AssignNode* node);
  void aStoreOffsetNode(StoreOffsetNode* node);
  void aStoreUplevelNode(StoreUplevelNode* node);
  
  void anArithRRNode(ArithRRNode* node);
  void aFloatArithRRNode(FloatArithRRNode* node);
  void aFloatUnaryArithNode(FloatUnaryArithNode* node);
  void anArithRCNode(ArithRCNode* node);
  void aTArithRRNode(TArithRRNode* node);
  
  void aContextCreateNode(ContextCreateNode* node);
  void aContextInitNode(ContextInitNode* node);
  void aContextZapNode(ContextZapNode* node);

  void aBlockCreateNode(BlockCreateNode* node);
  void aBlockMaterializeNode(BlockMaterializeNode* node);
  
  void aSendNode(SendNode* node);
  void aPrimNode(PrimNode* node);
  void aDLLNode(DLLNode* node);

  void aLoopHeaderNode(LoopHeaderNode* node);

  void aReturnNode(ReturnNode* node);
  void aNLRSetupNode(NLRSetupNode* node);
  void anInlinedReturnNode(InlinedReturnNode* node);
  void aNLRContinuationNode(NLRContinuationNode* node);
  
  void aBranchNode(BranchNode* node);
  void aTypeTestNode(TypeTestNode* node);
  void aNLRTestNode(NLRTestNode* node);
  void aMergeNode(MergeNode* node);
  
  void anArrayAtNode(ArrayAtNode* node);
  void anArrayAtPutNode(ArrayAtPutNode* node);

  void anInlinedPrimitiveNode(InlinedPrimitiveNode* node);
  void anUncommonNode(UncommonNode* node);
  void aFixedCodeNode(FixedCodeNode* node);

  void aNopNode(NopNode* node);
  void aCommentNode(CommentNode* node);
};

#endif
