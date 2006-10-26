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

#ifdef DELTA_COMPILER

class Stub;
class DebugInfoWriter;

class CodeGenerator: public NodeVisitor {
 private:
  MacroAssembler*	_masm;			// the low-level assembler
  PRegMapping*		_currentMapping;	// currently used mapping of PRegs
  GrowableArray<Stub*>	_mergeStubs;		// a stack of yet to generate merge stubs
  DebugInfoWriter*	_debugInfoWriter;	// keeps track of PReg location changes and updates debug info
  int			_maxNofStackTmps;	// the maximum number of stack allocated variables so far
  Node*			_previousNode;		// the previous node in the same basic block or NULL
  // info used to patch temporary initialization
  Register		_nilReg;		// the register holding nilObj used to initialize the stack frame
  CodeBuffer*		_pushCode;		// the code area that can be patched with push instructions

 private:
  // Helper routines for mapping
  Register	def(PReg* preg) const;
  Register	use(PReg* preg) const		{ return _currentMapping->use(preg); }
 
  void	setMapping(PRegMapping* mapping);
  int	maxNofStackTmps();
  bool	isLiveRangeBoundary(Node* a, Node* b) const;
  void	jmp(Node* from, Node* to, bool to_maybe_nontrivial = false);
  void	jcc(Assembler::Condition cc, Node* from, Node* to, bool to_maybe_nontrivial = false);
  void	bindLabel(Node* node);
  void	inlineCache(Node* call, MergeNode* nlrTestPoint, int flags = 0);
  void	updateDebuggingInfo(Node* node);

  // Helper routines for code generation
  char* nmethodAddress() const;
  void  incrementInvocationCounter();
  int   byteOffset(int offset);
  void  zapContext(PReg* context);
  void  storeCheck(Register obj);
  void  assign(PReg* dst, PReg* src, bool needsStoreCheck = true);
  void  uplevelBase(PReg* startContext, int nofLevels, Register base);
  void  moveConstant(ArithOpCode op, PReg*& x, PReg*& y, bool& x_attr, bool& y_attr);
  void  arithRROp(ArithOpCode op, Register x, Register y);
  void  arithRCOp(ArithOpCode op, Register x, int y);
  void  arithROOp(ArithOpCode op, Register x, oop y);
  void  arithRXOp(ArithOpCode op, Register x, oop y);
  bool  producesResult(ArithOpCode op);
  Register targetRegister(ArithOpCode op, PReg* z, PReg* x);
  Assembler::Condition mapToCC(BranchOpCode op);
  void  generateMergeStubs();
  void  copyIntoContexts(BlockCreateNode* node);
  void  materializeBlock(BlockCreateNode* node);
  void  jcc_error(Assembler::Condition cc, AbstractBranchNode* node, Label& label);
  void  testForSingleKlass(Register obj, klassOop klass, Register klassReg, Label& success, Label& failure);
  void  generateTypeTests(LoopHeaderNode* node, Label& failure);
  void  generateIntegerLoopTest(PReg* preg, LoopHeaderNode* node, Label& failure);
  void  generateIntegerLoopTests(LoopHeaderNode* node, Label& failure);
  void  generateArrayLoopTests(LoopHeaderNode* node, Label& failure);

  // Debugging
  static void  indent();
  static char* nmethodName();

  static void  verifyObj(oop obj);
  static void  verifyContext(oop obj);
  static void  verifyArguments(oop recv, oop* ebp, int nofArgs);
  static void  verifyReturn(oop result);
  static void  verifyNLR(char* fp, char* nlrFrame, int nlrScopeID, oop result);

  void callVerifyObj(Register obj);
  void callVerifyContext(Register context);
  void callVerifyArguments(Register recv, int nofArgs);
  void callVerifyReturn();
  void callVerifyNLR();

 public:
  CodeGenerator(MacroAssembler* masm, PRegMapping* mapping);
  Assembler* assembler() const			{ return _masm; }
  void initialize(InlinedScope* scope);		// call this before code generation
  void finalize(InlinedScope* scope);		// call this at end of code generation

 public:
  void beginOfBasicBlock(Node* node);
  void endOfBasicBlock(Node* node);
  
 public:
  void beginOfNode(Node* node);
  void endOfNode(Node* node);
  
 public:
  void aPrologueNode(PrologueNode* node);

  void aLoadIntNode(LoadIntNode* node);
  void aLoadOffsetNode(LoadOffsetNode* node);
  void aLoadUplevelNode(LoadUplevelNode* node);

  void anAssignNode(AssignNode* node);
  void aStoreOffsetNode(StoreOffsetNode* node);
  void aStoreUplevelNode(StoreUplevelNode* node);
  
  void anArithRRNode(ArithRRNode* node);
  void anArithRCNode(ArithRCNode* node);
  void aTArithRRNode(TArithRRNode* node);
  void aFloatArithRRNode(FloatArithRRNode* node);
  void aFloatUnaryArithNode(FloatUnaryArithNode* node);

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

  void aNopNode(NopNode* node) {}
  void aCommentNode(CommentNode* node) {}
};

#endif
