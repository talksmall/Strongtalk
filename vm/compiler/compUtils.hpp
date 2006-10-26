/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.10 $ */
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

// helper functions / data structures for compiler support; e.g. compiler
// parameters (inlining limits etc.)

#ifdef DELTA_COMPILER

// kinds of inlining limits
enum InlineLimitType {
  NormalFnLimit,		    // "size" of normal method (see msgCost)
  BlockArgFnLimit,		    // size of method with block args
  BlockFnLimit, 	    	    // size of block method (value, value: etc)
  SplitCostLimit, 	    	    // max total cost of copied nodes
  NormalFnInstrLimit,		    // size (instructions) of normal method
  BlockArgFnInstrLimit,	    	    // ditto for method with block args
  BlockFnInstrLimit, 	    	    // ditto for block method
  NmInstrLimit,     	    	    // desired max. nmethod size
  LastLimit	    	    	    // sentinel
};

typedef bool (*checkLocalSendFn)(symbolOop sel);
enum InlineFnType {
  NormalFn,		    // normal method
  BlockArgFn,		    // method with one or more block args
  BlockFn 	    	    // block method (value, value: etc)
};

// parameters of cost function that computes space cost of inlining 
// a particular method
struct CostParam {	    
  int localCost;	    // local variable access / assign
  int cheapSendCost;	    // "cheap" send (see isCheapMessage)
  int sendCost;   	    // arbitrary send
  int blockArgPenalty;     // penalty if non-cheap send has block arg(s)
  int primCallCost;	    // primitive call
  
  CostParam(int l, int c, int s, int b, int p) {
    localCost = l; cheapSendCost = c; sendCost = s;
    blockArgPenalty = b; primCallCost = p;
  }
};

// The PerformanceDebugger reports info useful for finding performance bugs (e.g., 
// contexts and blocks that can't be eliminated and the resons why).

class PerformanceDebugger : public ResourceObj {
  Compiler* c;
  bool compileReported;	      // have we already reported something for this compile?
  GrowableArray<BlockPReg*>* blocks;
  GrowableArray<char*>* reports;  // list of reports already printed (to avoid duplicates)
  stringStream* str;
  GrowableArray<InlinedScope*>* notInlinedBecauseNmethodTooBig;
public:
  PerformanceDebugger(Compiler* c);

  void report_context(InlinedScope* s);			  // couldn't eliminate scope's context
  void report_block(Node* s, BlockPReg* blk, char* what); // couldn't eliminate block
  void report_toobig(InlinedScope* s);			  // nmethod getting too big
  void report_uncommon(bool reoptimizing);		  // uncommon recompile  
  void report_prim_failure(primitive_desc* pd);		  // failure not uncommon
  void finish_reporting();
private:
  void report_compile();
  void start_report();
  void stop_report();
  friend class Reporter;
};

#endif
