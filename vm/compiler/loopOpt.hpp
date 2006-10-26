/* Copyright 1996, LongView Technologies L.L.C. $Revision: 1.6 $ */
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

// Implementation of loop optimizations: moving type tests out of loops, finding candidates for register
// allocation within a loop, plus integer-specific optimizations (removing tag checks and bound checks).

// a candidate for register allocation within a loop
class LoopRegCandidate : public PrintableResourceObj {
  PReg* _preg;
  int _nuses, _ndefs;
public:
  LoopRegCandidate(PReg* r)	{ _preg = r; _nuses = _ndefs = 0; }
  PReg* preg() const		{ return _preg; }
  int   nuses() const		{ return _nuses; }
  int   ndefs() const		{ return _ndefs; }
  int   weight() const		{ return _ndefs + _nuses; }
  void  incDUs(int u, int d)    { _nuses += u; _ndefs += d; }
  void  print();
};


// A CompiledLoop contains annotations for the compiler for integer loop optimizations.

class CompiledLoop : public PrintableResourceObj {
  InlinedScope* _scope;
  Node* _beforeLoop;			  // last node before loop (init. of loop var)
  LoopHeaderNode* _loopHeader;
  Node* _startOfLoop;
  Node* _endOfLoop;
  Node* _startOfBody;
  Node* _endOfBody;
  Node* _startOfCond;
  Node* _endOfCond;
  BranchNode* _loopBranch;		  // branch ending loop condition  
  int _firstNodeID, _lastNodeID;	  // all node IDs in loop are between these two

  // the instance variables below are set as a result of recognize()
  // and are valid only if isIntegerLoop()
  bool  _isIntegerLoop;			  // is this loop a recognized integer loop?
  PReg* _loopVar;			  // suspected loop variable
  PReg* _lowerBound;			  // lower bound of for-like loop (see comment in findLowerBound)
  PReg* _upperBound;			  // upper bound 
  PReg* _increment;			  // increment of loopVar
  bool  _isCountingUp;			  // true if loop is counting up, false if counting down
  NonTrivialNode* _incNode;		  // node incrementing loop var
  PReg* _loopArray;			  // array whose size is upper bound (or NULL)
  LoadOffsetNode* _loopSizeLoad;	  // node loading loopArray's size

  // instance variables for general loop optimizations
  GrowableArray<HoistedTypeTest*>* _hoistableTests;	// tests that can be hoisted out of the loop
  static GrowableArray<BB*>* _bbs;	  // BBs in code generation order
 public:
  CompiledLoop();	  
  
  // the routines below should be called with the appropriate nodes
  void set_startOfLoop(LoopHeaderNode* current);
  void set_startOfBody(Node* current);
  void set_endOfBody(Node* current);
  void set_startOfCond(Node* current);
  void set_endOfCond(Node* current);
  void set_endOfLoop(Node* end);

  bool isInLoop(Node* n) const;
  bool isInLoopCond(Node* n) const;
  bool isInLoopBody(Node* n) const;
  LoopHeaderNode* loopHeader() const	  { return _loopHeader; }


  char* recognize();			  // does integer loop recognition; called after defs/uses built
					  // returns NULL if successful, reason otherwise
  bool isIntegerLoop() const		  { return _isIntegerLoop; }
  void optimize();			  // perform general loop optimization
  void optimizeIntegerLoop();		  // perform integer loop optimization

  void print();

 protected:
   void  discoverLoopNesting();
   int   findStartOfSend(int bci);
   char* findLowerBound();
   char* findUpperBound();
   char* checkLoopVar();
   char* checkUpperBound();
   NonTrivialNode* findDefInLoop(PReg* r);	  // find single definition of r in loop
   bool  isIncrement(PReg* r, ArithOpCode op);	  // is r a suitable loop variable increment?
   void  removeTagChecks();
   void  removeLoopVarOverflow();
   void  checkForArraysDefinedInLoop();
   void  hoistTypeTests();
   void  removeBoundsChecks(PReg* array, PReg* var);
   void  findRegCandidates();
   bool  isEquivalentType(GrowableArray<klassOop>* klasses1, GrowableArray<klassOop>* klasses2);
 public:  // for iterators
   int   defsInLoop(PReg* r, NonTrivialNode** defNode = NULL);   // return number of definitions of r in loop and sets defNode if non-NULL
};

// holds the info associated with a single type test hoisted out of a loop
class HoistedTypeTest: public PrintableResourceObj {
public:
  NonTrivialNode* node;			    // node (within loop) doing the test
  PReg* testedPR;			    // PReg whose type is tested
  GrowableArray<klassOop>* klasses;	    // klasses for which the PReg is tested
  bool  invalid;			    

  HoistedTypeTest(NonTrivialNode* node, PReg* testedPR, GrowableArray<klassOop>* klasses);
  void print();
  void print_test_on(outputStream* s);
};

#endif

