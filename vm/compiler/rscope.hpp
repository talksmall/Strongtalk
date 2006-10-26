/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.32 $ */
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


// RScopes represent the inlined scopes of a nmethod during recompilation.
// Essentially, they are another view of the ScopeDesc information, in a
// format more convenient for the (re)compiler and extended with additional
// info from PICs and uncommon branches.

# ifdef DELTA_COMPILER

class RNonDummyScope;
class RInterpretedScope;

class RScope : public PrintableResourceObj {
 protected:
  RNonDummyScope* _sender;
  const int _senderBCI;
 public:
  int nsends;    	// estimated # of invocations (-1 == unknown)
    
  RScope(RNonDummyScope* s, int bci);

  // Type tests
  virtual bool isInterpretedScope() const			{ return false; }
  virtual bool isInlinedScope() const				{ return false; }
  virtual bool isNullScope() const				{ return false; }
  virtual bool isUninlinableScope() const			{ return false; }
  virtual bool isPICScope() const				{ return false; }
  virtual bool isUntakenScope() const				{ return false; }
  virtual bool isDatabaseScope() const				{ return false; }

  virtual bool isCompiled() const				{ return false; }
  RNonDummyScope* sender() const 				{ return (RNonDummyScope*)_sender; }
  int senderBCI() const	    					{ return _senderBCI; }
  virtual bool equivalent(InlinedScope* s) const = 0;
  virtual bool equivalent(LookupKey* l) const = 0;
  virtual RScope* subScope(int bci, LookupKey* l) const 	{ ShouldNotCallThis(); return NULL;}
  virtual GrowableArray<RScope*>* subScopes(int bci) const 	{ ShouldNotCallThis(); return NULL;}
  virtual bool  hasSubScopes(int bci) const 			{ ShouldNotCallThis(); return false;}
  virtual bool isUncommonAt(int bci) const 			{ ShouldNotCallThis(); return false; }
	  // was send at this bci uncommon (never taken) in recompilee?  (false means "don't know")
  virtual bool isNotUncommonAt(int bci) const 			{ ShouldNotCallThis(); return false; }
	  // return true iff the send at this bci *must not* be made uncommon
  virtual bool isLite() const 					{ return false; }
  virtual nmethod* get_nmethod() const 				{ return NULL; }
  virtual Expr* receiverExpr(PReg* p) const;
  virtual klassOop receiverKlass() const			= 0;
  virtual methodOop method() const				= 0;
  virtual LookupKey* key() const				= 0;
  bool wasNeverExecuted() const;				// was method never executed?
  virtual void print();
  virtual void print_short() = 0;
  virtual void printTree(int bci, int level) const;

  // Support for inlining database:
  // - returns the size of the inlining database tree.
  virtual int  inlining_database_size() { return 0; }
  // - prints the inlining database tree on a stream.
  virtual void print_inlining_database_on(outputStream* st, GrowableArray<PcDesc*>* uncommon, int bci = -1, int level = 0) { }

  virtual void extend() {}
  
  friend class RNonDummyScope;
};
  
class RNullScope : public RScope {
  // dummy scope for convenience; e.g. can call subScope() w/o checking for
  // this==NULL
 public:
  RNullScope() : RScope(NULL, 0) 			{}
  RNullScope(RNonDummyScope* sender, int bci) : RScope(sender, bci) {}
  bool isNullScope() const	    			{ return true; }
  bool equivalent(InlinedScope* s) const  		{ Unused(s);  return false; }
  bool equivalent(LookupKey* l) const  			{ Unused(l);  return false; }
  RScope* subScope(int bci, LookupKey* l) const  	{ return (RScope*)this; }
  GrowableArray<RScope*>* subScopes(int bci) const;
  bool hasSubScopes(int bci) const			{ Unused(bci);  return false; }
  bool isUncommonAt(int bci) const 			{ return false; }
  bool isNotUncommonAt(int bci) const 			{ return false; }
  klassOop receiverKlass() const			{ ShouldNotCallThis(); return 0; }
  methodOop method() const				{ ShouldNotCallThis(); return 0; }
  LookupKey* key() const				{ ShouldNotCallThis(); return 0; }
  void print()    	    				{ print_short(); }
  void printTree(int bci, int level) const;
  void print_short();
};

class RUninlinableScope : public RNullScope {
  // scope marking "callee" of an uninlinable or megamorphic send
 public:
  RUninlinableScope(RNonDummyScope* sender, int bci);		// for interpreted senders

  bool isUninlinableScope() const    			{ return true; }
  bool isNullScope() const	    			{ return false; }
  void print_inlining_database_on(outputStream* st, GrowableArray<PcDesc*>* uncommon, int bci, int level);
  void print_short();
};

class RNonDummyScope : public RScope {
  // abstract -- a non-dummy scope with subscopes
 protected:
  const int _level;				// distance from root
  const int ncodes;    	    			// # byte codes in method
  GrowableArray<RScope*>** _subScopes; 		// indexed by bci
 public:
  GrowableArray<RUncommonBranch*> uncommon;	// list of uncommon branches

  RNonDummyScope(RNonDummyScope* s, int bci, methodOop m, int level);
  RScope* subScope(int bci, LookupKey* l) const;  
 	// return the subscope matching the lookup
  	// return NullScope if no scope
  GrowableArray<RScope*>* subScopes(int bci) const;
  	// return all subscopes at bci
  bool hasSubScopes(int bci) const;
  int  level() const					{ return _level; }
  bool isUncommonAt(int bci) const;
  bool isNotUncommonAt(int bci) const;
  void addScope(int bci, RScope* s);
  void printTree(int bci, int level) const;

  virtual void unify(RNonDummyScope* s);
  static int compare(RNonDummyScope** a, RNonDummyScope** b); // for sorting

 protected:
  virtual void constructSubScopes(bool trusted);
  virtual int scopeID() const				{ ShouldNotCallThis(); return 0; }
  static bool trustPICs(methodOop m);
  void printSubScopes() const;
  static RNonDummyScope* constructRScopes(const nmethod* nm, bool trusted = true, int level = 0);
  friend class Compiler;
  friend class nmethod;
  friend class InliningDatabase;
};

class RInterpretedScope : public RNonDummyScope {
  // a scope corresponding to an interpreted method
  LookupKey* _key;
  methodOop _method;
  bool trusted;			// is PIC info trusted?
  bool extended;		// subScopes computed?
 public:
  RInterpretedScope(RNonDummyScope* sender, int bci, LookupKey* key, methodOop m,
  		    int level, bool trusted);
  bool isInterpretedScope() const	    		{ return true; }
  methodOop method() const				{ return _method; }
  LookupKey* key() const				{ return _key; }
  klassOop receiverKlass() const;
  bool equivalent(InlinedScope* s) const;
  bool equivalent(LookupKey* l) const;
  bool isUncommonAt(int bci) const;
  bool isNotUncommonAt(int bci) const 			{ return false; }
  void extend();
  void print()    	    				{ print_short(); }
  void print_short();
};
  
class RInlinedScope : public RNonDummyScope {
  // an inlined scope in the recompilee
 public:
  const nmethod* nm;    	// containing nmethod
  const ScopeDesc* desc;	// scope

  RInlinedScope(RNonDummyScope* s, int bci, const nmethod* nm, ScopeDesc* d, int level);
  bool isInlinedScope()	const		{ return true; }
  bool isCompiled() const		{ return true; }
  methodOop method() const;
  nmethod* get_nmethod() const		{ return (nmethod*)nm; }
  LookupKey* key() const;
  klassOop receiverKlass() const;
  bool isLite() const;
  bool equivalent(LookupKey* l) const;
  bool equivalent(InlinedScope* s) const;
  void print();
  void print_short();
  int  inlining_database_size();
  void print_inlining_database_on(outputStream* st, GrowableArray<PcDesc*>* uncommon, int bci, int level);
 protected:
  void constructSubScopes()		{ ShouldNotCallThis(); }
  int scopeID() const			{ return desc->offset(); }
};


// this class should be removed -- it's just a top-level inlined or interpreted scope -- fix this   -Urs
class RPICScope : public RNonDummyScope {
  // a scope called by the recompilee via a compiled PIC
 protected:
  const nmethod* caller;	// calling nmethod
  const CompiledIC* _sd;   	// calling IC
  const PcDesc* pcDesc; 	// calling pcDesc
  const klassOop klass;	    	// receiver klass
  const nmethod* nm;    	// called nmethod (or NULL if interpreted)
  const methodOop _method;	// called method
  const bool trusted;		// is PIC info trusted?
  bool extended;		// subScopes computed?
  const ScopeDesc* desc;	// scope (or NULL if interpreted)

 public:    
  RPICScope(const nmethod* caller, PcDesc* pc, CompiledIC* s, klassOop k,
	    ScopeDesc* d, nmethod* n, methodOop m, int nsends, int level, bool trusted);
  bool isPICScope() const	    	{ return true; }
  bool isCompiled() const		{ return nm != NULL; }
  methodOop method() const		{ return _method; }
  nmethod* get_nmethod() const		{ return (nmethod*)nm; }
  klassOop receiverKlass() const	{ return klass; }
  CompiledIC* sd() const		{ return (CompiledIC*)_sd; }
  LookupKey* key() const;
  bool isLite() const;
  bool equivalent(InlinedScope* s) const;
  bool equivalent(LookupKey* l) const;
  void unify(RNonDummyScope* s);
  void extend();
  void print();
  void print_short();

 protected:
  int scopeID() const			{ return pcDesc->scope; }
  static bool trustPICs(const nmethod* nm);
  friend RNonDummyScope* RNonDummyScope::constructRScopes(const nmethod* nm, bool trusted, int level);
};

class RDatabaseScope : public RNonDummyScope {
 // a scope created from the inlining database
 private:
  klassOop             _receiver_klass;
  methodOop            _method;
  LookupKey*           _key;
  GrowableArray<bool>* _uncommon;  // list of uncommon branch
 public:
  RDatabaseScope(RNonDummyScope* sender, int bci, klassOop receiver_klass, methodOop method, int level);
  bool isDatabaseScope()   const { return true; }
  methodOop method()       const { return _method; }
  LookupKey* key()         const { return _key; }
  klassOop receiverKlass() const { return _receiver_klass; }
  bool isUncommonAt(int bci) const;
  bool isNotUncommonAt(int bci) const;

  // Mark a bci as uncommon (used when constructing database scopes)
  void mark_as_uncommon(int bci) { _uncommon->at_put(bci, true); }

  bool equivalent(InlinedScope* s) const;
  bool equivalent(LookupKey* l)    const;
  void print();
  void print_short();
};

class RUntakenScope : public RNonDummyScope {
  //  send/inline cache that was never executed (either an empty ic or an untaken uncommon branch)
  const bool isUncommon;			// true iff untaken uncommon branch
  const PcDesc* pc;
 public:
  RUntakenScope(RNonDummyScope* sender, PcDesc* pc, bool isUnlikely);
  bool isUntakenScope() const    		{ return true; }
  bool isUnlikely() const			{ return isUncommon; }
  bool equivalent(InlinedScope* s) const	{ Unused(s);  return false; }
  bool equivalent(LookupKey* l) const  		{ Unused(l);  return false; }
  Expr* receiverExpr(PReg* p) const;
  methodOop method()       const		{ ShouldNotCallThis(); return NULL; }
  LookupKey* key()         const		{ ShouldNotCallThis(); return NULL; }
  klassOop receiverKlass() const		{ ShouldNotCallThis(); return NULL; }
  void extend() {}
  void print();
  void print_short();
 protected:
  int scopeID() const				{ return pc->scope; }
};


class RUncommonBranch : public PrintableResourceObj {
  // represents a taken uncommon branch
 public:
  RScope* scope;
  PcDesc* pcDesc; 	    // where the trap instruction is

  RUncommonBranch(RScope* r, PcDesc* pc) { scope = r; pcDesc = pc; }

  int bci() const	    		{ return pcDesc->byteCode; }
  void print();
};

# endif
