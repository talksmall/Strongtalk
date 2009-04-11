/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.42 $ */
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

class NameDescClosure {
 public:
  virtual void arg          (int no, NameDesc* a, char* pc) {}
  virtual void temp         (int no, NameDesc* t, char* pc) {}
  virtual void context_temp (int no, NameDesc* c, char* pc) {}
  virtual void stack_expr   (int no, NameDesc* e, char* pc) {}
};


const int16 IllegalBCI  =    -1;
const int16 PrologueBCI =     0;
const int16 EpilogueBCI = 32766;

// use the following functions to compare bcis; they handle PrologueBCI et al.
// negative if bci1 is before bci2, 0 if same, positive if after
int compareBCI(int bci1, int bci2);

inline bool bciLT(int bci1, int bci2) { return compareBCI(bci1, bci2) <  0; }
inline bool bciLE(int bci1, int bci2) { return compareBCI(bci1, bci2) <= 0; }
inline bool bciGT(int bci1, int bci2) { return compareBCI(bci1, bci2) >  0; }
inline bool bciGE(int bci1, int bci2) { return compareBCI(bci1, bci2) >= 0; }

// Blocks belonging to scopes that aren't described (because they
// can't possibly be visible to the user) get IllegalDescOffset as
// their desc offset
const int IllegalDescOffset = -2;


// scopeDescs contain the information that makes source-level debugging of
// nmethods possible; each scopeDesc describes a method activation
// Class Hierarchy:
//  - ScopeDesc          (abstract)
//    - MethodScopeDesc
//    - BlockScopeDesc
//    - TopLevelScopeDesc
//  - NonInlinedBlockScopeDesc

class ScopeDesc : public PrintableResourceObj {		// abstract
 protected:
  // Creation information
  const nmethodScopes* _scopes;
  int                  _offset;
  char*                _pc;

 protected:
  // Cached information
  bool      _has_temps;
  bool      _has_context_temps;
  bool      _has_expr_stack;
  methodOop _method;
  int       _scopeID;
  bool      _lite;
  int       _senderScopeOffset;
  int       _senderByteCodeIndex;
  bool      _allocates_compiled_context;
  int       _name_desc_offset;
  int       _next;

  // If the pc of a scopeDesc is equal to invalid_pc, the scopeDesc is pc independent.
  // A pc independent scopeDesc prints out all the locations for its nameDescs.
  // NB: Only relevant for new backend.
  static char* invalid_pc;

 public:
  void iterate    (NameDescClosure* blk);	// info for given pc()
  void iterate_all(NameDescClosure* blk);	// info for all pc's

  NameDesc* nameDescAt(int& offset) const;
  int       valueAt(int& offset) const;

  ScopeDesc(const nmethodScopes* scopes, int offset, char* pc);

  int   offset() const 				{ return int(_offset); }
  char* pc()     const				{ return _pc; }

  bool is_equal(ScopeDesc* s) const {
    return    _scopes == s->_scopes
           && _offset == s->_offset;
  }

  // A lite scopeDesc has no information saved on temporaries or expression stack.
  bool		is_lite() const			{ return _lite; }
  bool		allocates_interpreted_context() const;
  methodOop	method() const			{ return _method; }

  // Tells whether a compiled context is allocated for this scope.
  bool          allocates_compiled_context() const { return _allocates_compiled_context; }

  // Returns the name desc for the allocated compiled context.
  NameDesc*	compiled_context();

  int		scopeID() const			{ return _scopeID; }
  const nmethodScopes* scopes() const           { return _scopes; }
  symbolOop	selector() const		{ return method()->selector(); }
  ScopeDesc* 	sender() const;
  virtual klassOop selfKlass() const		= 0;

  // Returns the parent scope
  // If cross_nmethod_boundary is false parent will return NULL if
  // the parent is located in another compilation unit (nmethod).
  virtual ScopeDesc* parent(bool cross_nmethod_boundary = false) const = 0;
  ScopeDesc* home(bool cross_nmethod_boundary = false) const;

  // Returns the bci of the calling method if this scopeDesc is inlined.
  // For a root scopeDesc IllegalBCI is returned.
  int senderBCI() const {
    assert(_senderByteCodeIndex != IllegalBCI,
	   "need to ask calling byte code");
    return _senderByteCodeIndex; }

  // Root scope?
  bool isTop() const 				{ return _senderScopeOffset == 0; }

  // Lookup key
  virtual LookupKey* key() const		= 0;

  // scope equivalence -- for compiler
  virtual bool s_equivalent(ScopeDesc* s) const;
  virtual bool l_equivalent(LookupKey* s) const;

  // types test operations
  virtual bool isMethodScope() const 		{ return false; }
  virtual bool isBlockScope() const		{ return false; }
  virtual bool isTopLevelBlockScope() const	{ return false; }
  virtual bool isNonInlinedBlockScope() const	{ return false; }

  virtual NameDesc* self() const		= 0;
  NameDesc* temporary	    (int index, bool canFail = false);
  NameDesc* contextTemporary(int index, bool canFail = false);
  NameDesc* exprStackElem   (int bci);

 public:
  int next_offset() const 			{ return _next; }
  int sender_scope_offset() const		{ return _offset - _senderScopeOffset; }

  bool verify();
  void verify_expression_stack(int bci);

  // printing support
  void print(int indent, bool all_pcs);		// print info for current/all pc's
  void print()					{ print(0, false); }
  virtual void print_value_on(outputStream* st) const;

 protected:
  virtual bool shallow_verify() 		{ return true; }
  virtual void printName() = 0;
  virtual void printSelf() {}

 public:
  Expr* selfExpr(PReg* p) const;		// Type information for the optimizing compiler

  friend class nmethodScopes;
  friend class NonInlinedBlockScopeDesc;
};


class MethodScopeDesc : public ScopeDesc {
 protected:
  // Cached information
  LookupKey _key;
  NameDesc* _self_name;

 public:
  MethodScopeDesc(const nmethodScopes* scopes, int offset, char* pc);
  bool s_equivalent(ScopeDesc* s) const;
  bool l_equivalent(LookupKey* s) const;

  bool isMethodScope() const 			{ return true; }

  NameDesc* self() const			{ return _self_name; }
  LookupKey* key() const			{ return (LookupKey*)&_key; }
  klassOop selfKlass() const			{ return _key.klass(); }

  ScopeDesc* parent(bool cross_nmethod_boundary = false) const { return NULL; }

  // printing support
  void printName();
  void printSelf();
  void print_value_on(outputStream* st) const;
};

// an inlined block whose parent scope is in the same nmethod
class BlockScopeDesc : public ScopeDesc {
 protected:
  // Cached information
  int _parentScopeOffset;

 public:
  BlockScopeDesc(const nmethodScopes* scopes, int offset, char* pc);

  // NB: the next three operations may return NULL (no context)
  klassOop   selfKlass() const;
  NameDesc*  self() const;
  ScopeDesc* parent(bool cross_nmethod_boundary = false) const;

  LookupKey* key() const;

  bool s_equivalent(ScopeDesc* s) const;

  bool isBlockScope() const 			{ return true; }

  // print operations
  void printSelf();
  void printName();
  void print_value_on(outputStream* st) const;
};

// A block method whose enclosing scope isn't in the same nmethod.
// Must be a root scope.
class TopLevelBlockScopeDesc : public ScopeDesc {
 protected:
  // Cached information
  NameDesc* _self_name;
  klassOop  _self_klass;

 public:
  TopLevelBlockScopeDesc(const nmethodScopes* scopes, int offset, char* pc);

  // type test operations
  bool isBlockScope() const 	      		{ return true; }
  bool isTopLevelBlockScope() const 		{ return true; }

  klassOop selfKlass() const 			{ return _self_klass; }
  NameDesc* self() const 			{ return _self_name; }
  // NB: parent() may return NULL for clean blocks
  ScopeDesc* parent(bool cross_nmethod_boundary = false) const;

  LookupKey* key() const;

  bool s_equivalent(ScopeDesc* s) const;

  // print operations
  void printSelf();
  void printName();
  void print_value_on(outputStream* st) const;
};

class NonInlinedBlockScopeDesc : public PrintableResourceObj {
 protected:
  // Creation information
  const nmethodScopes* _scopes;
  int                  _offset;

 protected:
  // Cached information
  methodOop _method;
  int       _parentScopeOffset;

 public:
  NonInlinedBlockScopeDesc(const nmethodScopes* scopes, int offset);

  methodOop  method() const { return _method; }
  ScopeDesc* parent() const;
  void       print();
};

#endif
