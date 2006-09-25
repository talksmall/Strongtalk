/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.23 $ */
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

# include "incls/_precompiled.incl"

# ifdef DELTA_COMPILER

# include "incls/_defUse.cpp.incl"


  void DUInfo::propagateTo(BB* useBB, Use* use, const NonTrivialNode* fromNode, 
  			   PReg* src, NonTrivialNode* toNode, const bool global) {
    // r1 := r2; ...; r3 := op(r1)  -->  r1 := r2; ...; r3 := op(r2)
    bool ok = toNode->copyPropagate(useBB, use, src);
    if (CompilerDebug) {
      cout(PrintCopyPropagation)->print("*%s cp:%s propagate %s from N%ld (%#lx) to N%ld (%#lx)\n",
	global ? "global" : "local", ok ? "" : " couldn't",
	src->name(), fromNode ? fromNode->id() : 0, PrintHexAddresses ? fromNode : 0,
	toNode->id(), PrintHexAddresses ? toNode : 0);
    }
  }

  void DUInfo::propagateTo(BB* useBB, const PReg* r, const Def* def, Use* use,
			   const bool global) {
    // def reaches use; try to eliminate r's use by using copy propagation
    NonTrivialNode* fromNode = def->node;
    const bool isAssignment = fromNode->isAssignmentLike();
    NonTrivialNode* toNode = use->node;
    const bool hasSrc = fromNode->hasSrc();
    PReg* fromPR = hasSrc ? fromNode->src() : NULL;
    const bool isConst = hasSrc && fromPR->isConstPReg();

    if (isAssignment && isConst && toNode->canCopyPropagateOop()) {
      // loadOop r1, oop; ...; r2 := op(r1)    --->
      // loadOop r1, oop; ...; r2 := op(oop)
      bool ok = toNode->copyPropagate(useBB, use, fromPR);
      if (CompilerDebug) {
	// the original code broke the MSC++ 3.0 optimizer, so now it looks a bit weird.  -Urs 7/96
	char* glob = global ? "global" : "local";
	char* prop = ok ? "" : " couldn't propagate";
	char* name = def->node->src()->name();	    // using fromNode or fromPR here will break
	void* from = PrintHexAddresses ? fromNode : 0;
	void* to   = PrintHexAddresses ? toNode : 0;
	cout(PrintCopyPropagation)->print("*%s cp:%s %s from N%ld (%#lx) to N%ld (%#lx)\n",
					  glob, prop, name, def->node->id(), from, toNode->id(), to);
      }
      return;
    }

    if (fromNode->loopDepth() != toNode->loopDepth()) {
      // currently can't propagate into a loop: PReg would have to be extended to
      // end of loop, but is only extended to use -- fix this later
      // NB: this is not completely safe since fromNode might be in different loop than
      // toNode (but at same loop nesting), so loopDepths match
      assert(global, "can't be local cp");
      if (CompilerDebug) {
	cout(PrintCopyPropagation)->print("*global cp: can't propagate %s from N%ld (%#lx) to N%ld (%#lx) -- loopDepth mismatch\n",
	  fromPR->name(), fromNode->id(), PrintHexAddresses ? fromNode : 0, toNode->id(), PrintHexAddresses ? toNode : 0);
      }
      return;
    }

    if (isAssignment && !isConst && hasSrc && toNode->canCopyPropagate(fromNode)) {
      // r1 := r2; ...; r3 := op(r1)  -->  r1 := r2; ...; r3 := op(r2)
      propagateTo(useBB, use, fromNode, fromPR, toNode, global);
      return;
    } 
    
    if (!global && r->isSinglyUsed() && (toNode->isAssignNode() || toNode->isInlinedReturnNode()) &&  // fix this -- should it be isAssignmentLike?
	toNode->canBeEliminated() && fromNode->hasDest() && fromNode->canChangeDest() &&
	r->canBeEliminated(true) && !toNode->dest()->loc.isTopOfStack()) {
      // fromNode: r := ...;  ... ; toNode: x := r    --->
      // fromNode: x := ...;
      // NB: should extend live range of toNode->dest() backwards to include fromNode
      // currently not done (extendLiveRange handles only forward extensions), but as
      // long as this optimization isn't performed globally this shouldn't hurt
      if (CompilerDebug) {
	cout(PrintCopyPropagation)->print("*%s cp: changing dest of N%ld (%#lx) to %s to match use at N%ld (%#lx)\n",
	       global ? "global" : "local", 
	       fromNode->id(), PrintHexAddresses ? fromNode : 0, toNode->dest()->name(), toNode->id(), PrintHexAddresses ? toNode : 0);
      }
      assert(!r->incorrectDU(), "shouldn't try CP on this");
      assert(!global || r->isSinglyAssigned(), "not safe with >1 defs");
      assert(fromNode->dest() == r, "unexpected dest");
      fromNode->setDest(fromNode->bb(), toNode->dest());
      toNode->eliminate(toNode->bb(), NULL, false, true);
      return;
    } 

    // nothing worked
    if (CompilerDebug) {
      cout(PrintCopyPropagation)->print("*%s cp: can't propagate N%ld (%#lx) to N%ld (%#lx)\n",
	      global ? "global" : "local", fromNode->id(), PrintHexAddresses ? fromNode : 0,
	      toNode->id(), PrintHexAddresses ? toNode : 0);
    }
  }
  
  void Use::print() 	 	  	{ lprintf("Use %#lx (N%ld)", PrintHexAddresses ? this : 0, node->id()); }
  void PSoftUse::print() 	  	{ lprintf("PSoftUse %#lx (N%ld)", PrintHexAddresses ? this : 0, node->id());}
  void Def::print() 	 	  	{ lprintf("Def %#lx (N%ld)", PrintHexAddresses ? this : 0, node->id()); }

  void BBDUTable::print() {
    if (!info) return;	    // not built yet
    print_short();
    for (int i = 0; i < info->length(); i++) {
      lprintf("%3ld: ", i); info->at(i)->print();
    }
  }
  
  void DUInfo::getLiveRange(int& firstNodeNum, int& lastNodeNum) {
    // compute live range of a local register
    // IN:  first & last node ID of BB
    // OUT: first & last node ID where this PReg is live
    // algorithm: first = first def if def precedes first use
    //			  0 if first use precedes def (live from start)
    // 		  last  = last use if it is later than last def
    //		  	  lastNodeNum if last def is later (live until end)
    SListElem<Def*>* d = defs.head();
    SListElem<Use*>* u = uses.head();

    // set firstNodeNum
    if (u && d) {
      int unode = u->data()->node->num();
      int dnode = d->data()->node->num();
      if (unode > dnode) {
        firstNodeNum = dnode;	// live range starts at definition
      } else {
        // use before def -- leave first at zero
	assert(firstNodeNum == 0, "should be zero");
      }
    } else if (u) {
      // only a use -- live range = [0..unode]
      assert(firstNodeNum == 0, "should be zero");
      while (u && u->next()) u = u->next();
      lastNodeNum = u->data()->node->num();
      return;
    } else if (d) {
      firstNodeNum = d->data()->node->num();	// live range starts at definition and ends at end of BB
      return;
    } else {
      // no defs or uses in this BB; must be a result register whose use has been
      // eliminated (the def is in the last node of the previous BB)
      assert(reg->loc == resultLoc, "must be result loc");
      firstNodeNum = lastNodeNum = 0;
      return;
    }

    // set lastNodeNum; first find last def and last use
    while (u && u->next()) u = u->next();
    while (d && d->next()) d = d->next();
    int unode = u->data()->node->num();
    int dnode = d->data()->node->num();
    if (unode > dnode) {
      lastNodeNum = unode;	// live range ends at last use
    } else {
      // defined last -- live until end of BB
    }
  }

  void DUInfo::print_short() { lprintf("DUInfo %#lx", this); }
  void DUInfo::print() {
    print_short(); lprintf(" for "); reg->print_short(); lprintf(": ");
    uses.print(); lprintf("; "); defs.print(); lprintf("\n");
  }

  void PRegBBIndex::print_short() {
    lprintf("PRegBBIndex [%ld] for", index); bb->print_short(); 
  }

  void PRegBBIndex::print() {
    print_short(); lprintf(": "); bb->duInfo.info->at(index)->print();
  }

  static bool cpCreateFailed = false;

  CPInfo* new_CPInfo(NonTrivialNode* n) {
    CPInfo* cpi = new CPInfo(n);
    if (cpCreateFailed) {
      cpCreateFailed = false;
      return NULL;
    }
    return cpi;
  }
  
  CPInfo::CPInfo(NonTrivialNode* n) {
    def = n;
    if (!n->hasSrc()) {
      cpCreateFailed = true;	    // can't eliminate register
      return;
    }
    r = n->src();
    if (r->isConstPReg()) return;   // can always eliminate if defined by constant  (was bug; fixed 7/26/96 -Urs)
				    // (as long as constants aren't register-allocated)  
    PReg* eliminatee = n->dest();
    if (eliminatee->debug) {
      if (r->extendLiveRange(eliminatee->scope(), eliminatee->endBCI())) {
	// ok, the replacement PReg covers the eliminated PReg's debug-visible live range
	// (begBCI must be covered if endBCI is covered)
      } else {
	cpCreateFailed = true;	  // need register for debug info (was bug; fixed 5/14/96 -Urs)
      }
    }
  }

  bool CPInfo::isConstant() const 	{ return r->isConstPReg(); }
  oop  CPInfo::constant() const 	{
    assert(isConstant(), "not constant");
    return ((ConstPReg*)r)->constant;
  }

  void CPInfo::print() {
    lprintf("*(CPInfo*)%#x : def %#x, %s\n", this, def, r->name());
  }
  
  static void printNodeFn(DefUse* du) { lprintf("N%d ", du->node->id()); }
  
  struct PrintNodeClosureD : public Closure<Def*> {
    void do_it(Def* d) { printNodeFn(d); }
  };

  struct PrintNodeClosureU : public Closure<Use*> {
    void do_it(Use* u) { printNodeFn(u); }
  };

  void printDefsAndUses(const GrowableArray<PRegBBIndex*>* l) {
    lprintf("defs: ");
    PrintNodeClosureD printNodeD;
    forAllDefsDo(l, &printNodeD);
    lprintf("; uses: ");
    PrintNodeClosureU printNodeU;
    forAllUsesDo(l, &printNodeU);
  }



  static Closure<Def*>* theDefIterator;
  static void doDefsFn(PRegBBIndex* p) {
    DUInfo* info = p->bb->duInfo.info->at(p->index);
    info->defs.apply(theDefIterator);
  }
  
  void forAllDefsDo(const GrowableArray<PRegBBIndex*>* l, Closure<Def*>* f) {
    theDefIterator = f;
    l->apply(doDefsFn);
  }

  static Closure<Use*>* theUseIterator;
  static void doUsesFn(PRegBBIndex* p) {
    DUInfo* info = p->bb->duInfo.info->at(p->index);
    info->uses.apply(theUseIterator);
  }
  
  void forAllUsesDo(const GrowableArray<PRegBBIndex*>* l, Closure<Use*>* f) {
    theUseIterator = f;
    l->apply(doUsesFn);
  }
  
# endif
