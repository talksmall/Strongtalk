/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.19 $ */
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

// The Inliner controls and performs method inlining in the compiler.  It contains 
// all the code to set up the new scopes once a go-ahead decision has been made.
// (Most of) the actual decisions are made by InliningPolicy (further down).

enum SendKind { NormalSend, SelfSend, SuperSend };

class Inliner: public PrintableResourceObj {
 protected:
  InlinedScope* sender;		// scope containing the send
  SendKind kind;
  InlinedScope* callee;		// scope being inlined (or NULL)
  SendInfo* _info;		// send being inlined
  Expr* res;			// result expression
  SAPReg* resultPR;		// result PReg
  NodeBuilder* gen;		// current generator (sender's or callee's)
  MergeNode* merge;		// where multiple versions merge (NULL if only one)
  char* _msg;			// reason for not inlining the send
  bool lastLookupFailed;	// last tryLookup failed because no method found
 public:
  int depth;			// nesting depth (for debug output)
  
  Inliner(InlinedScope* s) 	{ this->sender = s; initialize(); }

  // The inlineXXX generate a non-inlined send if necessary, with the exception
  // of inlineBlockInvocation which returns NULL (and does nothing) if the block
  // shouldn't be inlined
  Expr* inlineNormalSend	(SendInfo* info);
  Expr* inlineSuperSend 	(SendInfo* info);
  Expr* inlineSelfSend  	(SendInfo* info);
  Expr* inlineBlockInvocation	(SendInfo* info);

  SendInfo* info() const	{ return _info; }
  char* msg() const		{ return _msg; }
  void print();

 protected:
  void initialize();
  void initialize(SendInfo* info, SendKind kind);
  Expr* inlineSend();
  void tryInlineSend();
  Expr* inlineMerge(SendInfo* info);
  Expr* picPredict ();
  Expr* picPredictUnlikely(SendInfo* info, RUntakenScope* uscope);
  Expr* typePredict();
  Expr* genRealSend();
  InlinedScope* tryLookup(Expr* rcvr);	  // try lookup and determine if should inline send
  Expr* doInline(Node* start);
  char* checkSendInPrimFailure();
  InlinedScope* notify(const char* msg);
  RScope* makeBlockRScope(const Expr* rcvr, LookupKey* key, const methodOop method);
  InlinedScope* makeScope(const Expr* rcvr, const klassOop klass, const LookupKey* key, const methodOop method);
  Expr* makeResult(Expr* res);
  bool checkSenderPath(Scope* here, ScopeDesc* there) const;

  friend class InliningPolicy;
};


class InliningPolicy : public ResourceObj {
  // the instance variables only serve as temporary storage during shouldInline()
 protected:
  methodOop method;		// target method
 public:
  int calleeCost;		// cost of inlining candidate

  InliningPolicy() { method = NULL; }
  char* basic_shouldInline(methodOop method);
   	// should send be inlined?  returns NULL (--> yes) or rejection msg 
 	// doesn't rely on compiler-internal information

  static bool isCriticalSmiSelector   (const symbolOop sel);
  static bool isCriticalArraySelector (const symbolOop sel);
  static bool isCriticalBoolSelector  (const symbolOop sel);

  // predicted by compiler?
  static bool isPredictedSmiSelector  (const symbolOop sel);
  static bool isPredictedArraySelector(const symbolOop sel);
  static bool isPredictedBoolSelector (const symbolOop sel);

  // predicted by interpreter?
  static bool isInterpreterPredictedSmiSelector  (const symbolOop sel);
  static bool isInterpreterPredictedArraySelector(const symbolOop sel);
  static bool isInterpreterPredictedBoolSelector (const symbolOop sel);

 protected:
  virtual klassOop receiverKlass() const = 0;		// return receiver klass (NULL if unknown)
  virtual klassOop nthArgKlass(int nth) const = 0;	// return nth argument of method (NULL if unknown)
  bool shouldNotInline() const;
  bool isBuiltinMethod() const;
};


// inlining policy of compiler
class CompilerInliningPolicy : public InliningPolicy {
 protected:
  InlinedScope* sender;		// sending scope
  Expr* rcvr;			// target receiver 

  klassOop receiverKlass() const;
  klassOop nthArgKlass(int n) const;
 public:
  char* shouldInline(InlinedScope* sender, InlinedScope* callee);
  	// should send be inlined?  returns NULL (--> yes) or rejection msg 
};


// "inlining policy" of recompiler (i.e., guesses whether method will be
// inlined or not
class RecompilerInliningPolicy : public InliningPolicy {
 protected:
  deltaVFrame* _vf;		// top vframe of this method/nmethod; may be NULL
  klassOop receiverKlass() const;
  klassOop nthArgKlass(int n) const;
  char* shouldInline(nmethod* nm);	// should nm be inlined?

 public:
  char* shouldInline(RFrame* rf);
  	// would send be inlined by compiler?  returns NULL (--> yes) or rejection msg 
};

# endif
