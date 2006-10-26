/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.11 $ */
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

// Defs represent PReg definitions, Uses the uses.  Each basic block
// has a list of the defs and uses it contains, and a PReg has a list
// of the BBs it is used/defined in.

# ifdef DELTA_COMPILER

  class DefUse : public PrintableResourceObj {		// abstract
   public:
    NonTrivialNode* node;
    DefUse(NonTrivialNode* n) { node = n; }

  };

  class Def : public DefUse {
   public:
    Def(NonTrivialNode* n) : DefUse(n) { }

    void print();
  };

  class Use : public DefUse {
   public:
    Use(NonTrivialNode* n) : DefUse(n) { }
    virtual bool isSoft() const   	{ return false; }
    void print();
  };

  class PSoftUse : public Use {
    // a debugger-related use; doesn't prevent block elimination
   public:
    PSoftUse(NonTrivialNode* n) : Use(n) { }
    bool isSoft() const   	{ return true; }
    void print();
  };

  class DUInfo : public PrintableResourceObj {// represents PReg's defs/uses within BB
   public:
    PReg* reg;
    SList<Use*> uses;  	// uses (in order of nodes within BB)
    SList<Def*> defs;
    DUInfo(PReg* r) { reg = r; }

    void getLiveRange(int& firstNodeID, int& lastNodeId);
    void propagateTo(BB* bb, const PReg* r, const Def* def, Use* use, const bool global);
    void propagateTo(BB* useBB, Use* use, const NonTrivialNode* fromNode, PReg* src,
		     NonTrivialNode* toNode, const bool global);
    void print_short();
    void print();
  };

  class DUInfoList;

  // a BBDUTable contains all defs and uses of a BB
  class BBDUTable : public PrintableResourceObj {	
   public:
    GrowableArray<DUInfo*>* info;		// one element per PReg used/defd
    BBDUTable() { info = NULL; }

    void print_short() { lprintf("BBDUTable %#lx", this); }
    void print();    
  };

  // a PRegBBIndex is an index into a particular element of a BBDUTable
  class PRegBBIndex : public PrintableResourceObj {
   public:
    BB* bb;		// BB containing some of PReg's defs/uses
    int index;		// index into BB's BBDUTable

    PRegBBIndex(BB* b, int i, PReg* pr) { Unused(pr);  bb = b; index = i; }
    void print_short();
    void print();
  };

  class CPInfo : public PrintableResourceObj {
    // keeps track of effects of copy propagation
   public:    	    	    	// (for debugging info)
    NonTrivialNode* def;	// eliminated definition
    PReg* r;	    	    	// equivalent PReg

    CPInfo(NonTrivialNode* d, PReg* r1) { def = d; r = r1; }
    bool isConstant() const;
    oop  constant() const;
    void print();
   protected:
    CPInfo(NonTrivialNode* def);
    friend CPInfo* new_CPInfo(NonTrivialNode* def);
  };

  CPInfo* new_CPInfo(NonTrivialNode* def); // may return NULL if def isn't suitable
  void forAllDefsDo(const GrowableArray<PRegBBIndex*>* l, Closure<Def*>* f);
  void forAllUsesDo(const GrowableArray<PRegBBIndex*>* l, Closure<Use*>* f);
  void printDefsAndUses(const GrowableArray<PRegBBIndex*>* l);	// for debugging
# endif
