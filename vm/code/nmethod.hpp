/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.76 $ */
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

// nmethods (native methods) are the compiled code versions of Delta
// methods.

struct nmFlags {
  unsigned int version:8;        	// version number (0 = first version)
  unsigned int level:4;	    		// optimization level
  unsigned int age:4;	    		// age (in # of sweep steps)

  unsigned int state:2;			// {alive, zombie, dead)

  unsigned int isUncommonRecompiled:1;	// recompiled because of uncommon trap?
  unsigned int isYoung:1;		// "young"? (recently recompiled)
  unsigned int isToBeRecompiled:1;	// to be recompiled as soon as it matures
  unsigned int is_block:1;		// tells whether is is a block nmethod;

  unsigned int markedForDeoptimization:1;// Used for stack deoptimization

  void clear();
};

// A nmethod has five parts:    
//  1) header
//  2) machine instructions		(mostly in NCodeBase)
//  3) oop location information		(in OopNCode)
//  4) mapping from block_closure_index to NonInlinedBlockScope offset
//  5) debugging information
//  6) dependency information
class nmethod : public OopNCode {
 protected:
  uint16 _special_handler_call_offset;	// offset (in bytes) of call to special handler (*) (see comment below)
  uint16 _entry_point_offset;		// offset (in bytes) of entry point with class check
  uint16 _verified_entry_point_offset;	// offset (in bytes) of entry point without class check
  uint16 _scopeLen;
  uint16 _number_of_noninlined_blocks;
  uint16 _number_of_links;		// # of inline caches (including PICs) calling this nmethod
  uint16 _number_of_float_temporaries;	// # of floats in activation frame of this nmethod
  uint16 _float_section_size;		// size of float section in words
  uint16 _float_section_start_offset;	// offset of float section relative to frame pointer (in oops)

  int	 _invocation_count;		// incremented for each nmethod invocation if CountExecution == true
  int	 _uncommon_trap_counter;	// # of times uncommon traps have been executed
  static int	 all_uncommon_trap_counter;	// # of times uncommon traps have been executed across all nmethods
  nmFlags flags;			// various flags to keep track of nmethod state

  // (*) At this address there's 5 bytes of extra space reserved to accomodate for a call to the zombie handler
  // if the nmethod is a zombie. If the nmethod is not a zombie, there's a call to StubRoutines::recompile_stub_entry()
  // instead (even if the nmethod doesn't count invocations). If the method turns zombie, this call is overwritten
  // by the beforementioned call, thereby making sure that the relocation information for the modified nmethod stays
  // valid. In both cases this address is jumped to via a short jump from one of the entry points.

  // Note: If it becomes important to save some space per nmethod, note that both _zombie_handler_jump_offset
  //       and _verified_entry_point_offset are less than 128 bytes away from _entry_point_offset, thus these
  //       offsets could be stored in one byte relative to _entry_point_offset.

   enum { alive = 0, zombie = 1, dead = 2, resurrected = 3 };
 
 public:
  char* insts() const			{ return (char*)(this + 1); }				// machine instructions
  char* specialHandlerCall() const	{ return insts() + _special_handler_call_offset; }	// call to special handler
  char* entryPoint() const		{ return insts() + _entry_point_offset; }		// normal entry point
  char* verifiedEntryPoint() const	{ return insts() + _verified_entry_point_offset; }	// e.p. if klass is correct
  bool  isFree()                        { return Universe::code->contains((void*) _instsLen); } // has this nmethod been freed

  // debugging information
  nmethodScopes* scopes() const		{ return (nmethodScopes*) locsEnd(); }
  PcDesc*	pcs() const		{ return (PcDesc*) scopes()->pcs(); }
  PcDesc*	pcsEnd() const		{ return (PcDesc*) scopes()->pcsEnd(); }
  methodOop	method() const;
  klassOop      receiver_klass() const;

  uint16*	noninlined_block_offsets() const { return (uint16*) pcsEnd(); }
  char*         end() const             { return (char*)(noninlined_block_offsets() + 
                                          (_number_of_noninlined_blocks * sizeof uint16)); }

  
 public:
  LookupKey	key;			// key for code table searching

  jumpTableID   main_id;     
  jumpTableID   promoted_id; 

  int  number_of_float_temporaries() const	{ return _number_of_float_temporaries; }
  int  float_section_size() const		{ return _float_section_size; }
  int  float_section_start_offset() const	{ return _float_section_start_offset; }
  
  // Interface for number_of_callers
  int  number_of_links() const		{ return _number_of_links; }
  void inc_number_of_links()		{ _number_of_links++; }
  void dec_number_of_links()		{ _number_of_links--; }

  // Interface for uncommon_trap_invocation
  int  uncommon_trap_counter() const	{ return _uncommon_trap_counter; }
  void inc_uncommon_trap_counter()	{ _uncommon_trap_counter++; all_uncommon_trap_counter++;}

  // Returns the parent nmethod if present, NULL otherwise
  nmethod* parent();
  // Returns the outermost nmethod, returns this if no parent is present.
  nmethod* outermost(); 

 protected:
  nmethod(Compiler* c);
  void* operator new(size_t size);

 public:
  friend nmethod* new_nmethod(Compiler* c);
  void   initForTesting(int size, LookupKey* key); // to support testing
  int    size() const 			{ return end() - ((char*) this); }	// total size of nmethod
  int    codeSize() const               { return instsEnd() - insts(); }	// size of code in bytes

  // Shift the pc relative information by delta.
  // Call this whenever the code is moved.
  void fix_relocation_at_move(int delta);

  void	moveTo(void* to, int size);

 public:
  bool  isNMethod() const		{ return true; }

  // Flag accessing and manipulation.
  bool  isAlive() const			{ return flags.state == alive;  }
  bool  isZombie() const		{ return flags.state == zombie; }
  bool  isDead()  const                 { return flags.state == dead;   } // unused
  bool  isResurrected()                 { return flags.state == resurrected; }

  void  makeZombie(bool clearInlineCaches);
  // allow resurrection of zombies - for use during recompilation
  void  resurrect()                     { assert(isZombie(), "cannot resurrect non-zombies"); flags.state = resurrected; }

  bool	isUncommonRecompiled() const	{ return flags.isUncommonRecompiled; }

  bool	isYoung();
  void	makeYoung()			{ flags.isYoung = 1; }
  void	makeOld();

  int	age() const			{ return flags.age; }
  void	incrementAge()			{ const int MaxAge = 15; flags.age = min(flags.age + 1, MaxAge); }

  bool	is_marked_for_deoptimization() const{ return flags.markedForDeoptimization; }
  void	mark_for_deoptimization()	{ flags.markedForDeoptimization = 1; }
  void	unmark_for_deoptimization()	{ flags.markedForDeoptimization = 0; }

  bool	isToBeRecompiled() const	{ return flags.isToBeRecompiled; }
  void	makeToBeRecompiled()		{ flags.isToBeRecompiled = 1; }

  bool	is_block() const		{ return flags.is_block == 1; }
  bool	is_method() const		{ return flags.is_block == 0; }

  int   level() const;
  void  setLevel(int newLevel)		{ flags.level = newLevel; }

  int   version() const 		{ return flags.version; }
  void  setVersion(int v);

 public:
  int estimatedInvocationCount() const;	// approximation (not all calls have counters)
  int ncallers() const;			// # of callers (# nmethods, *not* # of inline caches)

  bool	encompasses(void* p) const;
  
  // for zone LRU management
  int lastUsed() const			{ return LRUtable[0].lastUsed; }
  
  void  clear_inline_caches();

  void  cleanup_inline_caches();

  void	forwardLinkedSends(nmethod* to);
  
  void	unlink(); // unlink from codeTable, deps etc.

  // Removes the nmethod from the zone 
  void flush();

  // Returns the set of nmethods to invalidate if THIS nmethod is the root of invalidation.
  // The set will be this nmethod plus all offspring (seperately compiled block code).
  GrowableArray<nmethod*>* invalidation_family();

  // Tells whether this nmethod dependes on invalid classes (classes flagged invalid) 
  bool depends_on_invalid_klass();

 private:
  // Recursive helper function for invalidation_family()
  void add_family(GrowableArray<nmethod*>* result);
  
 protected:
  void	check_store();
 public:
  // Iterate over all oops in the nmethod
  void	oops_do(void f(oop*));

  bool	switch_pointers(oop from, oop to,
			GrowableArray<nmethod*>* nmethods_to_invalidate);
  void relocate();

  // Verify operations
  void verify();
  void verify_expression_stacks();            // verify the expression stacks at all interrupt points
  void verify_expression_stacks_at(char* pc); // verify the expression stacks at pc

  // Iterates over all inline caches in the nmethod
  void CompiledICs_do(void f(CompiledIC*));
  
  // Iterates over all primitive inline caches in the nmethod
  void PrimitiveICs_do(void f(PrimitiveIC*));

  // programming and debugging
  PcDesc*    containingPcDesc(char* pc, PcDesc* start = NULL) const;
 protected:
  PcDesc*    containingPcDescOrNULL(char* pc, PcDesc* start = NULL) const;
 public:

  ScopeDesc* containingScopeDesc(char* pc) const;
#ifdef DEBUG
  PcDesc*    correspondingPC(ScopeDesc* sd, int bci) const;
#endif

  // For debugging
  CompiledIC*    IC_at(char* p) const;
  PrimitiveIC*   primitiveIC_at(char* p) const;
  oop*           embeddedOop_at(char* p) const;

  jumpTableEntry* jump_table_entry() const;

  // noninlined block mapping
  bool has_noninlined_blocks()       const;
  int  number_of_noninlined_blocks() const;

 protected:
  void validate_noninlined_block_scope_index(int index) const;

 public:
  NonInlinedBlockScopeDesc* noninlined_block_scope_at(int noninlined_block_index) const;
  methodOop                 noninlined_block_method_at(int noninlined_block_index) const;
  void                      noninlined_block_at_put(int noninlined_block_index, int offset) const;
  jumpTableEntry*	    noninlined_block_jumpEntry_at(int noninlined_block_index) const;


  // Returns true if activation frame has been established. 
  bool has_frame_at(char* pc) const;

  // Returns true if pc is not in prologue or epilogue code.
  bool in_delta_code_at(char* pc) const;
  
  // Printing support
  void print();
  void printCode();
  void printLocs();
  void printPcs();

  void print_value_on(outputStream* st);

  // Inlining Database
  void print_inlining_database();
  void print_inlining_database_on(outputStream* st);
  GrowableArray<PcDesc*>* uncommonBranchList();

  // prints the inlining structure (one line per scope with indentation if there's no debug info)
  void print_inlining(outputStream* st = NULL, bool with_debug_info = false);

  friend nmethod* nmethodContaining(char* pc, char* likelyEntryPoint);
  friend nmethod* findNMethod(void* start);
  friend nmethod* findNMethod_maybe(void* start);
  
  friend nmethod* nmethod_from_insts(char* insts);

 public:
  // Counting & Timing
  int invocation_count() const		{ return _invocation_count; }
  void set_invocation_count(int c)	{ _invocation_count = c; }

  // Perform a sweeper task
  void sweeper_step(double decay_factor);
 private:
  inline void decay_invocation_count(double decay_factor);

 public: 
  static int invocationCountOffset()	{ return (int)&((nmethod*)0)->_invocation_count; }

   // Support for preemption:

   // Overwrites the nmethod allowing trap at interrupt point.
   // After the overwrite data contains the information for restoring the nmethod.
   void overwrite_for_trapping(nmethod_patch* data);

   // Restores the nmethod 
   void restore_from_patch(nmethod_patch* data);
};

inline nmethod* nmethod_from_insts(char* insts)
{	// for efficiency
    nmethod* nm = (nmethod*) insts - 1;
    return findNMethod(nm);
}

#endif
