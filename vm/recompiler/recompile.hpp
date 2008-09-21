/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.20 $ */
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

// The recompilation system determines which interpreted methods should be compiled
// by the native-code compiler, or which compiled methods need to be recompiled for
// further optimization. This file contains the interface to the run-time system.   
//
// A Recompilation performs a recompilation from interpreted or compiled code to 
// (hopefully better) compiled code. The global theRecompilation is set only during
// a recompilation.
 
class Recompilation;
extern Recompilation* theRecompilation;
extern nmethod* recompilee;		// method currently being recompiled

class Recompilation : public VM_Operation {
 private:
  oop		_rcvr;			// receiver of trigger method/nmethod
  methodOop	_method;		// trigger method
  nmethod*	_nm;			// trigger nmethod (if compiled, NULL otherwise)
  nmethod*	_newNM;			// new nmethod replacing trigger (if any)
  deltaVFrame*	_triggerVF;		// vframe of trigger method/nmethod (NOT COMPLETELY INITIALIZED)
  bool		_isUncommon;		// recompiling because of uncommon branch?
  bool		_recompiledTrigger;	// is newNM the new version of _nm?

 public:
  Recompilation(oop rcvr, methodOop method) {		    // used if interpreted method triggers counter
    _method = method; _rcvr = rcvr; _nm = NULL; _isUncommon = false; init();
  }
  Recompilation(oop rcvr, nmethod* nm, bool unc = false) {  // used if compiled method triggers counter
    _method = nm->method(); _rcvr = rcvr; _nm = nm; _isUncommon = unc; init();
  }
  ~Recompilation()			{ theRecompilation = NULL; }
  void doit();
  bool isCompiled() const		{ return _nm != NULL; }
  bool recompiledTrigger() const	{ return _recompiledTrigger; }
  char* result() const			{ return _newNM ? _newNM->verifiedEntryPoint() : NULL; }
  oop  receiverOf(deltaVFrame* vf) const;     // same as vf->receiver() but also works for trigger
  char* name() 				{ return "recompile"; }

  // entry points dealing with invocation counter overflow
  static char* methodOop_invocation_counter_overflow(oop rcvr, methodOop method); // called by interpreter
  static char* nmethod_invocation_counter_overflow  (oop rcvr, char*     pc    ); // called by nmethods

 protected:
  void init();
  void recompile(Recompilee* r);
  void recompile_method(Recompilee* r);
  void recompile_block(Recompilee* r);
  bool handleStaleInlineCache(RFrame* first);
};


// A Recompilee represents something to be recompiled (either an interpreted method or
// a compiled method).

class Recompilee : public ResourceObj {
 protected:
  RFrame* _rf;
  Recompilee(RFrame* rf) 		{ _rf = rf; }
 public:
  virtual bool is_interpreted() const	{ return false; }
  virtual bool is_compiled() const	{ return false; }
  virtual LookupKey* key() const 	= 0;
  virtual methodOop  method() const	= 0;
  virtual nmethod*   code() const 	{ ShouldNotCallThis(); return NULL; }	// only for compiled recompileed
  	  RFrame*    rframe() const	{ return _rf; }
  static Recompilee* new_Recompilee(RFrame* rf);
};

class InterpretedRecompilee : public Recompilee {
  LookupKey* _key;
  methodOop _method;
 public:
  InterpretedRecompilee(RFrame* rf, LookupKey* k, methodOop m) : Recompilee(rf) { _key = k; _method = m; }
  bool       is_interpreted() const	{ return true; }
  LookupKey* key() const 		{ return _key; }
  methodOop  method() const		{ return _method; }
};

class CompiledRecompilee : public Recompilee {
  nmethod* _nm;
 public:
  CompiledRecompilee(RFrame* rf, nmethod* nm) : Recompilee(rf) { _nm = nm; }
  bool       is_compiled() const	{ return true; }
  LookupKey* key() const;
  methodOop  method() const;
  nmethod*   code() const		{ return _nm; }
};


#ifdef junk
extern int nstages;			// # of recompilation stages
extern smi* compileCounts;		// # of compilations indexed by stage
extern int* recompileLimits;		// recompilation limits indexed by stage
#endif
const int MaxRecompilationLevels = 4;	// max. # recompilation levels
const int MaxVersions = 4 - 1;		// desired max. # nmethod recompilations


#endif
