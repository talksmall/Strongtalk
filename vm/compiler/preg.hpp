/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.47 $ */
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

// All intermediate language (IL) operations work on pseudo registers (PRegs).
// In contrast to real registers, the number of available pseudo registers is
// unlimited.

// PRegs are standard pseudo regs; the different kinds of PRegs differ
// mainly in their purpose and in their live ranges
// PReg: locals etc., multiply assigned, live range is the entire scope
// SAPReg: single source-level assignment, live range = arbitrary subrange
// SplitPReg: used for splitting
// BlockPRegs: for blocks, live range = creating bci .. end
// TempPRegs: local to one BB, used for certain hard-coded idioms (e.g.
// loading an uplevel-accessed value)

class PReg: public PrintableResourceObj {
 protected:
  int16 _id;				// unique id
  int16 _nuses, _ndefs;	        	// number of defs & uses (including soft uses)
    	    	    	    		// (negative means incorrect/unknown values,
    					// e.g. hardwired regs)
  int16 _nsoftUses;	    		// number of "soft" uses
  LogicalAddress* _logicalAddress;	// for new backend only: logical address if created or NULL
  static const int AvgBBIndexLen; 	// estimated # of BBs in which I appear
 public:
  static int currentNo;			// id of next PReg created
  GrowableArray<PRegBBIndex*> dus;	// defs and uses
  InlinedScope* _scope;			// scope to which I belong
  Location loc;				// real location assigned to this preg
  CPInfo* cpInfo;   	    		// to follow effects of copy propagation
  GrowableArray<PReg*>* cpRegs;		// regs cp-ed away by me
  int regClass;  	    		// register equivalence class number
  PReg* regClassLink;	    		// next element in class
  int weight;    	    		// weight (importance) for reg. allocation
    					// (-1 if targeted register)
  GrowableArray<BlockPReg*>* _uplevelR;	// list of blocks that uplevel-read me (or NULL)
  GrowableArray<BlockPReg*>* _uplevelW;	// list of blocks that uplevel-write me (or NULL)
  bool debug;	    	    		// value/loc needed for debugging info?
  int _map_index_cash;			// cashes old map index - used to improve PregMapping access speed - must be >= 0

 protected:
  void initialize() {
    _id = currentNo++; 
    _uplevelR = _uplevelW = NULL; debug = false;
    _nuses = _ndefs = _nsoftUses = weight = 0;
    _logicalAddress = NULL;
    cpInfo = NULL; regClass = 0;
    regClassLink = 0; cpRegs = NULL;
    _map_index_cash = 0;
  }
  static const int VeryNegative;
    
 public:
  PReg(InlinedScope* s) : dus(AvgBBIndexLen) {
    assert(s, "must have a scope");
    initialize(); _scope = s; loc = unAllocated;
  }

  PReg(InlinedScope* s, Location l, bool incU, bool incD) : dus(AvgBBIndexLen){
    assert(s, "must have a scope");
    assert(!l.equals(illegalLocation), "illegal location");
    initialize(); _scope = s; loc = l;
    if (incU) _nuses = VeryNegative;
    if (incD) _ndefs = VeryNegative;
  }

  int id() const			{ return _id; }
  InlinedScope* scope() const		{ return _scope; }
  virtual bool isSAPReg() const 	{ return false; }
  virtual bool isArgSAPReg() const 	{ return false; }
  virtual bool isSplitPReg() const 	{ return false; }
  virtual bool isTempPReg() const 	{ return false; }
  virtual bool isNoPReg() const 	{ return false; }
  virtual bool isBlockPReg() const 	{ return false; }
  virtual bool isConstPReg() const 	{ return false; }
  virtual bool isMemoized() const  	{ return false; }

  bool uplevelR() const			{ return _uplevelR != NULL; }  
  bool uplevelW() const			{ return _uplevelW != NULL; }
  void addUplevelAccessor(BlockPReg* blk, bool read, bool write);
  void removeUplevelAccessor(BlockPReg* blk);
  void removeAllUplevelAccessors();

  virtual int begBCI() const		{ return PrologueBCI; }
  virtual int endBCI() const		{ return EpilogueBCI; }

  virtual bool canCopyPropagate() const;
  bool incorrectDU() const 	    	{ return _nuses < 0 || _ndefs < 0; }
  bool incorrectD() const 	    	{ return               _ndefs < 0; } 
  bool incorrectU() const 	    	{ return _nuses < 0; }
  void makeIncorrectDU(bool incU, bool incD);

 protected:
  // don't use nuses() et al, use isUsed() instead
  // they are protected to avoid bugs; it's only safe to call them if uses are correct
  // (otherwise, nuses() et al can return negative values)
  int nuses() const    	    		{ return _nuses; }
  int hardUses() const 	    		{ return _nuses - _nsoftUses; }
  int ndefs() const    	    		{ return _ndefs; }
  friend class RegisterAllocator;
 public:
  int nsoftUses() const  	    	{ return _nsoftUses; }
  bool isUsed() const 	    		{ return _ndefs || _nuses; }
  bool isUnused() const 	    	{ return !isUsed(); }
  bool hasNoUses() const 	    	{ return _nuses == 0; }
  	// NB: be careful with isUnused() vs. hasNoUses() -- they're different if a PReg has
	// no (hard) uses but has definitions
  virtual bool isSinglyAssigned() const	{ return _ndefs == 1; }
  bool isSinglyUsed() const		{ return _nuses == 1; }
  bool isOnlySoftUsed() const		{ return hardUses() == 0; }

  void incUses(Use* use); 
  void decUses(Use* use); 
  void incDefs(Def* def); 
  void decDefs(Def* def); 

  // incremental update of def-use info
  Use* addUse(DUInfo* info, NonTrivialNode* n);
  Use* addUse(BB* bb, NonTrivialNode* n);
  void removeUse(DUInfo* info, Use* u);
  void removeUse(BB* bb, Use* u);
  Def* addDef(DUInfo* info, NonTrivialNode* n);
  Def* addDef(BB* bb, NonTrivialNode* n);
  void removeDef(DUInfo* info, Def* d);
  void removeDef(BB* bb, Def* d);
  virtual bool extendLiveRange(Node* n);
  virtual bool extendLiveRange(InlinedScope* s, int bci);
  void forAllDefsDo(Closure<Def*>* c);
  void forAllUsesDo(Closure<Use*>* c);
    
  bool isLocalTo(BB* bb) const;
  void makeSameRegClass(PReg* other, GrowableArray<RegisterEqClass*>* classes);
  virtual void allocateTo(Location r);
    
  virtual bool canBeEliminated(bool withUses = false) const;
  virtual void eliminate(bool withUses = false);
  bool isCPEquivalent(PReg* r) const;
  bool checkEquivalentDefs() const;
  bool slow_isLiveAt(Node* n) const;
  virtual bool isLiveAt(Node* n) const;
 protected:
  void addDUHelper(Node* n, SList<DefUse*>* l, DefUse* el);
  virtual NameNode* locNameNode(bool mustBeLegal) const;
  void eliminateUses(DUInfo* info, BB* bb);
  void eliminateDefs(DUInfo* info, BB* bb, bool removing);
  void updateCPInfo(NonTrivialNode* n);

 public:
  virtual void print();
  virtual void print_short()			{ lprintf("%s\n", name()); }
  virtual char* name() const;			// string representing the preg name
  char* safeName() const;			// same as name() but handles NULL receiver
  virtual char* prefix() const 			{ return "P"; }
  virtual bool verify() const;
  virtual NameNode* nameNode(bool mustBeLegal = true) const; // for debugging info
  LogicalAddress* createLogicalAddress();
  LogicalAddress* logicalAddress() const	{ return _logicalAddress; }
  PReg* cpReg() const;				// return "cp-equivalent" PReg

  friend InlinedScope* findAncestor(InlinedScope* s1, int& bci1,
			    	 InlinedScope* s2, int& bci2);
      // find closest common ancestor of s1 and s2, and the
      // respective sender bcis in that scope

  // Initialization (before every compile)
  static void initPRegs();
};

// A temp preg is exactly like a PReg except that it is live for only
// a very short (hard-wired) code sequence such as loading a frame ptr etc.
class TempPReg : public PReg {
 public:
  TempPReg(InlinedScope* s) : PReg(s) {}
  TempPReg(InlinedScope* s, Location l, bool incU, bool incD): PReg(s, l, incU, incD) {}
  bool isTempPReg() const 	    	{ return true; }
  bool isLiveAt(Node* n) const 		{ Unused(n);  return false; }
  bool canCopyPropagate() const 	{ return false; }
  char* prefix() const			{ return "TempP"; }
};
  
// singly-assigned PReg (in source-level terms, e.g. expr. stack entry, arg;
// may have several defs because of splitting etc.
// makes copy propagation simpler
class SAPReg : public PReg {
 protected:
  InlinedScope* _creationScope;		// source scope to which receiver belongs
  int creationStartBCI;  		// startBCI in creationScope 
  int _begBCI, _endBCI;  		// live range = [_begBCI, _endBCI] in scope
  	    	    	    		// (for reg. alloc. purposes)
  const bool _isInContext;		// is this SAPReg a context location?
 public:
  SAPReg(InlinedScope* s, int st = IllegalBCI, int en = IllegalBCI, bool inContext = false);
  SAPReg(InlinedScope* s, Location l, bool incU, bool incD,
  	 int st, int en) : PReg((InlinedScope*)s, l, incU, incD), _isInContext(false) {
    _begBCI = creationStartBCI = st; _endBCI = en; _creationScope = s;
  }

  int  begBCI() const			{ return _begBCI; }
  int  endBCI() const			{ return _endBCI; }
  bool isInContext() const		{ return _isInContext; }
  InlinedScope* creationScope() const   { return _creationScope; }

  bool isSinglyAssigned() const		{ return true; }
  bool extendLiveRange(Node* n);
  bool extendLiveRange(InlinedScope* s, int bci);
  bool isLiveAt(Node* n) const;
  bool isSAPReg() const 	    	{ return true; }
  char* prefix() const			{ return "SAP"; }
  bool verify() const;
 protected:
  bool basic_isLiveAt(InlinedScope* s, int bci) const;
  friend class ExprStack;
};


class BlockPReg : public SAPReg {
 protected:
  CompileTimeClosure*	_closure;			// the compile-time closure representation
  bool			_memoized;  	    		// is this a memoized block?
  bool			_escapes;			// does the block escape?
  GrowableArray<Node*>* _escapeNodes;			// list of all nodes where the block escapes (or NULL)
  GrowableArray<PReg*>* _uplevelRead;			// list of PRegs uplevel-read by block method (or NULL)
  GrowableArray<PReg*>* _uplevelWritten;		// list of PRegs uplevel-written by block method (or NULL)
  GrowableArray<Location*>* _contextCopies;		// list of context location containing a copy of the receiver (or NULL)
  static int		_numBlocks;

 public:
  BlockPReg(InlinedScope* scope, CompileTimeClosure* closure, int beg, int end);

  bool			isBlockPReg() const		{ return true; }
  virtual NameNode*	locNameNode(bool mustBeLegal) const;
  InlinedScope* 	scope() const			{ return (InlinedScope*)_scope; }
  InlinedScope*		parent() const;
  CompileTimeClosure*	closure() const			{ return _closure; }
  methodOop		method() const			{ return _closure->method(); }
  static int		numBlocks()			{ return _numBlocks; }
  void			memoize();	    		// memoize this block if possible/desirable
  void			markEscaped(Node* n);	    	// mark this block as escaping at node n
  void			markEscaped();	    		// ditto; receiver escapes because of uplevel access from escaping block
  bool			isMemoized() const	    	{ return _memoized; }
  bool			escapes() const			{ return _escapes; }
  bool			canBeEliminated(bool withUses = false) const;
  void			eliminate(bool withUses = false);
  void			computeUplevelAccesses();
  GrowableArray<PReg*>* uplevelRead() const		{ return _uplevelRead; }
  GrowableArray<PReg*>* uplevelWritten() const		{ return _uplevelWritten; }
  GrowableArray<Location*>* contextCopies() const	{ return _contextCopies; }
  void		        addContextCopy(Location* l);
  char*			prefix() const			{ return "BlkP"; }
  char*			name() const;
  bool			verify() const;
  void			print();

  friend InlinedScope*	scopeFromBlockKlass(klassOop blockKlass);
  friend class PReg;
};


class NoPReg : public PReg {	// "no result" register (should have no uses)
 public:
  NoPReg(InlinedScope* scope) : PReg(scope)	{ loc = noRegister; initialize(); }
  virtual bool isNoPReg() const 		{ return true; }
  bool canCopyPropagate() const 		{ return false; }
  NameNode* nameNode(bool mustBeLegal) const;
  char* name() const 				{ return "nil"; }
  bool verify() const;
};


class ConstPReg : public PReg {
  // ConstPRegs are used to CSE constants; at register allocation time,
  // they can either get a register (if one is available), or they're
  // treated as literals by code generation, i.e. loaded into a temp reg
  // before each use.
 public:
  oop constant;
  protected:
  ConstPReg(InlinedScope* s, oop c) : PReg(s) { 
    constant = c; 
    assert(!c->is_mem() || c->is_old(), "constant must be tenured");
  }
 public:
  friend ConstPReg* new_ConstPReg(InlinedScope* s, oop c);
  friend ConstPReg* findConstPReg(Node* n, oop c);
  bool isConstPReg() const 	    		{ return true; }
  void allocateTo(Location r);
  void extendLiveRange(InlinedScope* s);
  bool extendLiveRange(Node* n);
  bool covers(Node* n) const;
  bool needsRegister() const;
  NameNode* nameNode(bool mustBeLegal = true) const;
  char* prefix() const				{ return "ConstP"; }
  char* name() const;
  bool verify() const;
};



#ifdef not_yet_used
// SplitPRegs hold the receiver of a split message send; their main
// purpose is to make register allocation/live range computation simple
// and efficient without implementing general live range analysis
class SplitPReg : public SAPReg {
 public:
  SplitSig* sig;
  
  SplitPReg(InlinedScope* s, int st, int en, SplitSig* signature) : SAPReg(s, st, en) {
    sig = signature;
  }

  bool	isSinglyAssigned() const		{ return true; }
  bool	extendLiveRange(Node* n);
  bool	isLiveAt(Node* n) const;
  bool	isSplitPReg() const 	    		{ return true; }
  char*	prefix() const				{ return "SplitP"; }
  char*	name() const;
};
#endif


# endif
  
