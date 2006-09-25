/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.14 $ */
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


// rframes ("recompiler frames") decorate stack frames with some extra information
// needed by the recompiler.  The recompiler views the stack (at the time of recompilation) 
// as a list of rframes.

#ifdef DELTA_COMPILER

class RFrame : public PrintableResourceObj {
 protected:
  frame _fr;			// my frame
  RFrame* _caller, *_callee;	// caller / callee rframes (or NULL)
  int _num;			// stack frame number (0 = most recent)
  int _distance;                // recompilation search "distance" (measured in # of interpreted frames)
  int _invocations;		// current invocation estimate (for this frame)
  				// (i.e., how often was thus frame called)
  int _ncallers;		// number of callers
  int _sends;			// sends caused by this frame
  int _cumulSends;		// sends including sends from nested blocks
  int _loopDepth;		// loop depth of callee

  RFrame(frame fr, const RFrame* callee);
  virtual void init() = 0;	// compute invocations, loopDepth, etc.
  void print(const char* name);

 public:

  static RFrame* new_RFrame(frame fr, const RFrame* callee);

  virtual bool is_interpreted() const	{ return false; }
  virtual bool is_compiled() const	{ return false; }
  bool is_super() const;		// invoked by super send?
  int invocations() const		{ return _invocations; }
  int sends() const			{ return _sends; }
  int cumulSends() const		{ return _cumulSends; }
  int loopDepth() const			{ return _loopDepth; }
  int num() const			{ return _num; }
  int distance() const			{ return _distance; }
  void set_distance(int d);
  int nCallers() const			{ return _ncallers; }
  bool is_blockMethod() const;
  frame fr() const			{ return _fr; }
  virtual LookupKey* key() const	= 0;	// lookup key or NULL (for block invoc.)
  virtual int cost() const		= 0;	// estimated inlining cost (size)
  virtual methodOop top_method() const	= 0;
  virtual deltaVFrame* top_vframe() const = 0;
  virtual void cleanupStaleInlineCaches() = 0;
  virtual nmethod* nm() const		{ ShouldNotCallThis(); return NULL; }
  bool hasBlockArgs() const;		// does top method receive block arguments?
  GrowableArray<blockClosureOop>* blockArgs() const;  // return list of all block args

  RFrame* caller();
  RFrame* callee() const		{ return _callee; }
  RFrame* parent() const;		// rframe containing lexical scope (if any)
  void print()				= 0;

  static int computeSends(methodOop m);
  static int computeSends(nmethod* nm);
  static int computeCumulSends(methodOop m);
  static int computeCumulSends(nmethod* nm);
};

class CompiledRFrame : public RFrame {		// frame containing a compiled method
 protected:
  nmethod* _nm;
  deltaVFrame* _vf;		// top vframe; may be NULL (for most recent frame)

  CompiledRFrame(frame fr, const RFrame* callee);
  void init();
  friend class RFrame;

 public:
  CompiledRFrame(frame fr);	// for nmethod triggering its counter (callee == NULL)
  bool is_compiled() const		{ return true; }
  methodOop top_method() const;
  deltaVFrame* top_vframe() const	{ return _vf; }
  nmethod* nm() const			{ return _nm; }
  LookupKey* key() const;
  int cost() const;
  void cleanupStaleInlineCaches();
  void print();
};

class InterpretedRFrame : public RFrame {	// interpreter frame
 protected:
  methodOop _method;
  int _bci;			// current bci
  klassOop _receiverKlass;
  deltaVFrame* _vf;		// may be NULL (for most recent frame)
  LookupKey* _key;		// cached value of key()
 
  InterpretedRFrame(frame fr, const RFrame* callee);
  void init();
  friend class RFrame;

 public:
  InterpretedRFrame(frame fr, methodOop m, klassOop rcvrKlass);	// for method triggering its invocation counter
  bool is_interpreted() const		{ return true; }
  methodOop top_method() const		{ return _method; }
  deltaVFrame* top_vframe() const	{ return _vf; }
  LookupKey* key() const;
  int cost() const;
  void cleanupStaleInlineCaches();
  void print();
};

#endif
