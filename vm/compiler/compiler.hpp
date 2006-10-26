/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.56 $ */
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

extern int nofCompilations;
extern Compiler* theCompiler;

class CodeGenerator;
class NLRTestNode;

class Compiler : public PrintableResourceObj {
  // A Compiler holds all the global state required for a single compilation.
 private:
  GrowableArray<InlinedScope*>	_scopeStack;			// to keep track of current scope
  int				_totalNofBytes;			// total no. of bytes compiled (for statistics)
  int				_special_handler_call_offset;	// call to recompilation stub or zombie handler (offset in bytes)
  int				_entry_point_offset;		// nmethod entry point if receiver is unknown (offset in bytes)
  int				_verified_entry_point_offset;	// nmethod entry point if receiver is known (offset in bytes)
  int				_totalNofFloatTemporaries;	// the number of floatTemporaries for this nmethod
  int				_float_section_size;		// the size of the float section on the stack in oops
  int				_float_section_start_offset;	// the offset of the float section on the stack relative to ebp in oops
  CodeBuffer*			_code;				// the buffer used for code generation

  int	_nextLevel; 	    	    			// optimization level for nmethod being created
  bool	_noInlinableSends;				// no inlinable sends remaining?
  bool  _uses_inlining_database;			// tells whether the compilation is base on inlinine database information.

public:
  LookupKey*			key;
  CompiledIC*			ic;			// sending inline cache
  nmethod*			parentNMethod;		// nmethod of lexical parent (or NULL if not compiling a block method)
  methodOop			method;			// top-level method being compiled
  NonInlinedBlockScopeDesc*	blockScope;		// or NULL if not compiling a block method

  RScope*			recompileeRScope;	// recompilee's rscope (or NULL)
  int				countID;		// recompile counter ID

  jumpTableID                   main_jumpTable_id;      // jump table id
  jumpTableID                   promoted_jumpTable_id;  // promoted jump table entry for block method only

  bool				useUncommonTraps;	// ok to use uncommon traps?

  ScopeDescRecorder*		rec;
  InlinedScope*			topScope;		// top scope 
  BB*				firstBB;		// first basic block
  GrowableArray<NLRTestNode*>*	nlrTestPoints;		// needed to fix up NLR points after node generation
  GrowableArray<InlinedScope*>*	scopes;			// list of all scopes (indexed by scope ID)
  GrowableArray<InlinedScope*>*	contextList;		// list of all scopes with run-time contexts
  GrowableArray<BlockPReg*>*	blockClosures;		// list of all block literals created so far
  Node*				firstNode;		// the very first node of the intermediate representation
  PerformanceDebugger*		reporter;		// for reporting performance info
  
#ifdef DEBUG
  stringStream* messages;				// debug messages    
#endif 

#ifdef LATER
  int	inlineLimit[LastLimit];    			// limits for current compilation
#endif


 private:
  void initialize(RScope* remote_scope = NULL);
  void initTopScope();
  void initLimits();
  void buildBBs();
  void fixupNLRTestPoints();
  void computeBlockInfo();

public:
  Compiler(LookupKey* k, methodOop m, CompiledIC* ic = NULL);		// normal entry point (method lookups)
  Compiler(blockClosureOop blk, NonInlinedBlockScopeDesc* scope);	// for block methods
  Compiler(RScope* scope);						// for inlining database
  ~Compiler() { finalize(); }

  CodeBuffer*		code() const;
  ScopeDescRecorder*	scopeDescRecorder();
  nmethod*		compile();
  void			finalize();

  int	level() const;					// optimization level of new nmethod
  int	version() const;				// version ("nth recompilation") of new nmethod
  int	special_handler_call_offset() const		{ return _special_handler_call_offset ; }
  int	entry_point_offset() const			{ return _entry_point_offset; }
  int	verified_entry_point_offset() const		{ return _verified_entry_point_offset; }
  int	get_invocation_counter_limit() const;		// invocation limit for nmethod being created

  void	set_special_handler_call_offset(int offset);
  void  set_entry_point_offset(int offset);
  void  set_verified_entry_point_offset(int offset);

  int	totalNofFloatTemporaries() const		{ assert(_totalNofFloatTemporaries >= 0, "not yet determined"); return _totalNofFloatTemporaries; }
  bool	has_float_temporaries() const			{ return totalNofFloatTemporaries() > 0; }
  int	float_section_size() const			{ return has_float_temporaries() ? _float_section_size : 0; }
  int	float_section_start_offset() const		{ return has_float_temporaries() ? _float_section_start_offset : 0; }

  void	set_float_section_size(int size);
  void	set_float_section_start_offset(int offset);

  bool	is_block_compile() const			{ return parentNMethod != NULL; }
  bool	is_method_compile() const			{ return !is_block_compile(); }
  bool	is_uncommon_compile() const;			// recompiling because of uncommon trap?
  bool	is_database_compile() const			{ return _uses_inlining_database; }

  int	number_of_noninlined_blocks() const;		// no. of noninlined blocks in nmethod (used for jump entry alloc.)
  void	copy_noninlined_block_info(nmethod* nm);	// copy the noninlined block info to the nmethod.
  void	nofBytesCompiled(int n) 			{ _totalNofBytes += n; }
  int	totalNofBytes() const				{ return _totalNofBytes; }

  int		estimatedSize() const;
  InlinedScope*	currentScope() const;			// scope currently being compiled
  void		enterScope(InlinedScope* s);
  void		exitScope (InlinedScope* s);

  void		allocateArgs(int nargs, bool isPrimCall);
  bool		registerUninlinable(Inliner* inliner);
  
  void print();
  void print_short();
  void print_key(outputStream* s);
  void print_code(bool suppressTrivial);
};

#ifdef DEBUG
// cout: output stream for compiler messages
// Intended usage pattern: if (CompilerDebug) cout(PrintXXX)->print("....").
// This will print the message to std if the PrintXXX flag is set or to the compiler's string buffer otherwise.
outputStream* cout(bool flag);	// for compiler debugging; returns stdout or compiler-internal string stream depending on flag
void print_cout();		// prints hidden messages of current compilation
#else
#define cout(flag)  if (flag) std
#endif

#endif
