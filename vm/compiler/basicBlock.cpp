/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.66 $ */
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
# include "incls/_basicBlock.cpp.incl"

int BB::genCounter = 0;

void BB::init(Node* f, Node* l, int n) {
  first = f; last = l; nnodes = n;
  _id = 0; _genCount = 0; BB::genCounter = 0;
  _loopDepth = 0;
}

BB*  BB::next()		const { Node* n = last ->next();      return n ? n->bb() : NULL; }
BB*  BB::firstPrev()	const { Node* n = first->firstPrev(); return n ? n->bb() : NULL; }
BB*  BB::next(int i)	const { Node* n = last ->next(i);     return n ? n->bb() : NULL; }
BB*  BB::prev(int i)	const { Node* n = first->prev(i);     return n ? n->bb() : NULL; }
int  BB::nSuccessors()	const { return last ->nSuccessors(); }
int  BB::nPredecessors()const { return first->nPredecessors(); }
bool BB::isSuccessor  (const BB* bb) const { return last->isSuccessor(bb->first); }
bool BB::isPredecessor(const BB* bb) const { return first->isPredecessor(bb->last); }

// note: the functions below are defined here rather than in PReg to localize
// this temporary hack
const int MaxSearch = 50;	// max. # of nodes to search backwards
NonTrivialNode* findDefinitionOf(Node* endNode, const PReg* r, int max = MaxSearch) {
  // search backwards for a definition of r
  Node* current = endNode;
  for (int i = 0; i < max && current != NULL; i++) {
    if (current->deleted) continue;
    if (current->hasSinglePredecessor()) {
      if ((current->isAssignNode() || current->isInlinedReturnNode()) && 
          ((NonTrivialNode*)current)->dest() == r) {
        return (NonTrivialNode*)current;
      }
    } else {
      // merge node with several predecessors: search each one and make sure all find the same def
      assert(current->isMergeNode(), "must be a merge");
      MergeNode* merge = (MergeNode*)current;
      NonTrivialNode* candidate = NULL;	// possible def of r
      for (int j = merge->nPredecessors() - 1; j >= 0; j--) {
	Node* prev = merge->prev(j);
	NonTrivialNode* cand = findDefinitionOf(prev, r, max - i);
	if (cand == NULL || candidate && cand != candidate) return NULL;
	candidate = cand;
      }
      return candidate;	// all paths lead to the same candidate
    }
    current = current->firstPrev();
  }
  return NULL;	// search limit exceeded
}


void propagateTo(BB* useBB, Use* use, NonTrivialNode* fromNode, PReg* src,
		 NonTrivialNode* toNode) {
  // r1 := r2; ...; r3 := op(r1)  -->  r1 := r2; ...; r3 := op(r2)
  if (toNode->canCopyPropagate(fromNode)) {
    if (!src->extendLiveRange(toNode)) {
      if (CompilerDebug) {
	cout(PrintCopyPropagation)->print("*brute-force cp: cannot propagate %s from N%ld to N%ld because of extendLiveRange\n",
	        src->name(), fromNode->id(), toNode->id());
      }
      return;
    }
    PReg* replaced = fromNode->dest();
    bool ok = toNode->copyPropagate(useBB, use, src, true);

    if (!ok) {
      // This if statement has been added by Lars Bak 29-4-96 to work around the type check node
      // elimination problem. (Ask Urs for details).
      return;
    }
    assert(ok, "should have worked");
    if (CompilerDebug) {
      cout(PrintCopyPropagation)->print("*brute-force cp: propagate %s from N%ld to N%ld\n",
	      src->name(), fromNode->id(), toNode->id());
    }
    // if this was the last use, make sure its value can be recovered for debugging
    if (replaced->debug && replaced->hasNoUses()) {
      assert(!replaced->cpInfo, "already has cpInfo");
      replaced->cpInfo = new_CPInfo(fromNode);
      assert(replaced->cpInfo, "should have cpInfo now");
    }
  } else {
    if (CompilerDebug) cout(PrintCopyPropagation)->print("*Node N%d cannot copy-propagate\n", toNode->id());
  }
}


bool regAssignedBetween(const PReg* r, const Node* startNode, Node* endNode) {
  // check if r is assigned somewhere between start and end node
  // quite inefficient
  // search backwards from end to start
  BB* bbWithoutDefs = NULL;	// BB w/o defs of r
  for (Node* n = endNode; n != startNode; n = n->firstPrev()) {
    // see if n's bb has a def or r
    BB* bb = n->bb();
    if (bb == bbWithoutDefs) continue; // no defs here
    bool hasDefs = false;
    for (int i = 0; i < bb->duInfo.info->length(); i++) {// forall def/use info lists
      DUInfo* dui = bb->duInfo.info->at(i);
      if (dui->reg == r && !dui->defs.isEmpty()) {
	// yes, it has a def
	hasDefs = true;
        for (SListElem<Def*>* d = dui->defs.head(); d; d = d->next()) {
	  if (d->data()->node == n) return true; 
	}
	break;	// no need to search the other duInfo entries
      }
    }
    if (!hasDefs) {
      // r has no def in this BB, avoid search next time
      bbWithoutDefs = bb;
    }
  }
  return false;		// no def found
}


void BB::bruteForceCopyPropagate() {
  const int len = duInfo.info->length();
  
  for (int i = 0; i < len; i++) {		// forall def/use info lists
    DUInfo* dui = duInfo.info->at(i);
    const PReg* r = dui->reg;
    if (!r->isSAPReg() || !r->loc.equals(unAllocated)) {
      // optimize only SAPRegs for now
      // preallocated PReg may have aliases - don't do CP
      continue;
    }

    // try to find a def them with other PRegs
    if (dui->uses.isEmpty()) continue;	// has only defs	
    const Use* use = dui->uses.head()->data();
    Node* firstUseNode = use->node;
    NonTrivialNode* defNode = findDefinitionOf(firstUseNode, r);
    if (defNode == NULL) continue;	// no def found

    PReg* candidate = ((NonTrivialNode*)defNode)->src();
    if (!candidate->loc.equals(unAllocated)) continue;
    assert(candidate->isUsed(), "should not be unused");
    if (!regAssignedBetween(candidate, defNode, firstUseNode)) {
      // ok: the candidate reaches the use at firstUseNode
      // try to propagate it to all uses of r in this BB
      SListElem<Use*>* nextu;
      for (SListElem<Use*>* u = dui->uses.head(); u; u = nextu) {
	nextu = u->next();	// because we're mutating the list
	Use* thisUse = u->data();
        NonTrivialNode* thisNode = thisUse->node;
	if (!regAssignedBetween(candidate, firstUseNode, thisNode)) {
 	  propagateTo(this, thisUse, defNode, candidate, thisNode);
	} else {
	  // candidate can't reach anything downstream of this use
	  break;
	}
      }
    }
  }	
}


void BB::localCopyPropagate() {
  // perform local copy propagation using the local def/use information
  // should be rewritten to make just one pass over the nodes, keeping track of
  // aliases created by assignments -- fix this 
  const int len = duInfo.info->length();
  SimpleBitVector used = 0;		// hardwired registers used
  SimpleBitVector usedTwice = 0;
  for (int i = 0; i < len; i++) {
    PReg* r = duInfo.info->at(i)->reg;
    if (!r->loc.equals(unAllocated) && r->loc.isRegisterLocation()) {
      if (used.isAllocated(r->loc.number())) {
	// two PRegs have same preallocated reg - algorithm below can't handle this
	usedTwice = usedTwice.allocate(r->loc.number());
      } else {
      	used = used.allocate(r->loc.number());
      }
    }
  }
  
  for (i = 0; i < len; i++) {
    const int BIG = 9999999;
    DUInfo* dui = duInfo.info->at(i);
    PReg* r = dui->reg;
    if (!r->loc.equals(unAllocated) && r->loc.isRegisterLocation() && usedTwice.isAllocated(r->loc.number())) {
      // this preallocated PReg has aliases - don't do CP
      continue;
    }
    SListElem<Use*>* u = dui->uses.head();
    SListElem<Def*>* nextd;
    for (SListElem<Def*>* d = dui->defs.head(); d && u; d = nextd) {
      // try to find a use of the def at d
      nextd = d->next();
      const Def* def = d->data();
      SList<Def*>* srcDefs = NULL;	// redefinition of src that defines this def (if any) 
      if (def->node->hasSrc()) {
	PReg* src = def->node->src();
	if (src->loc.isRegisterLocation() && usedTwice.isAllocated(src->loc.number())) {
	  // r := f(r2), and r2 is aliased preallocated - can't handle
	  continue;
	}
	if (!src->isSinglyAssigned()) {
	  // careful: r is only equivalent to src as long as src isn't reassigned
	  // within this basic block
	  for (int j = 0; j < len; j++) {
	    PReg* r = duInfo.info->at(j)->reg;
	    if (r == src) break;
	  }
	  assert(j < len, "should have found duInfo for src");
	  srcDefs = &duInfo.info->at(j)->defs;
	  if (srcDefs->length() == 0) {
	    srcDefs = NULL;   // ok, src is constant during this BB
	  }
	}
      }
      const int d_id = def->node->num();
      int u_id;
      // find a use in a node following the current def
      for ( ; u && (u_id = u->data()->node->num()) <= d_id; u = u->next()) ;
      if (!u) break;	  // no such use in this BB

      // visit all uses with a node ID between here and the next def of either the
      // current PReg or the source PReg that defines it
      int stop_id = nextd ? nextd->data()->node->num() : BIG;
      if (srcDefs) {
	for (SListElem<Def*>* d = srcDefs->head(); 
	     d && d->data()->node->num() < u_id; 
	     d = d->next()) ;
	if (d) stop_id = min(stop_id, d->data()->node->num());
      }

      while (u_id <= stop_id) {
	// the def at d_id reaches the use at u_id
	assert(d_id < u_id, "just checking");
	dui->propagateTo(this, r, def, u->data(), false);
	u = u->next();
	u_id = u ? u->data()->node->num() : BIG + 1;
      }
    }
  }	
}


void BB::makeUses() {
  // collect defs and uses for all pregs (and fill pregTable in the process)
  assert(duInfo.info == NULL, "shouldn't be set");
  duInfo.info = new GrowableArray<DUInfo*>(nnodes + 10);
  for (Node* n = first; n != last->next(); n = n->next()) {
    if (n->deleted) continue;
    n->makeUses(this);
  }
}


void BB::renumber() {
  int count = 0;
  for (Node* n = first; n != last->next(); n = n->next()) n->setNum(count++);
  nnodes = count;
}


void BB::remove(Node* n) {
  // remove this node and its defs & uses
  // NB: nodes aren't actually removed from the graph but just marked as
  // deleted.  This is much simpler because the topology of the flow graph
  // doesn't change this way
  assert(contains(n), "node isn't in this BB");
  n->removeUses(this);
  n->deleted = true;
}


void BB::addAfter(Node* prev, Node* newNode) {
  // prev == NULL means add as first node
  assert(nnodes, "shouldn't add anything to this BB");
  assert(prev == NULL || contains(prev), "node isn't in this BB");
  if (prev) {
    prev->insertNext(newNode);
    if (prev == last) last = newNode;
  } else {
    first->insertPrev(newNode);
    first = newNode;
  }
  if (bbIterator->usesBuilt) {
    newNode->makeUses(this);
  } else {
    newNode->setBB(this);
  }
  renumber();
  assert(newNode->bb() == this, "should point to me now");
}


static BB* thisBB;
static void duChecker(PRegBBIndex* p) {
  if (p->bb == thisBB) fatal("should not be in middle of list");
}


static bool findMyBB(void* bb, PRegBBIndex* p) {
  return p->bb == (BB*)bb;
}


int BB::addUDHelper(PReg* r) {
  // we're computing the uses block by block, and the current BB's
  // PRegBBIndex is always the last entry in the preg's list.
  assert(nnodes, "shouldn't add anything to this BB");
  bbIterator->pregTable->at_put_grow(r->id(), r);
  PRegBBIndex* p;
  if (bbIterator->usesBuilt) {
    // find entry for the PReg
    int i = r->dus.find(this, findMyBB);
    if (i >= 0) {
      p = r->dus.at(i);
    } else {
      // create new entry
      duInfo.info->append(new DUInfo(r));
      r->dus.append(p = new PRegBBIndex(this, duInfo.info->length() - 1, r));
    }
  } else {
    // while building the defs & uses, the PReg's entry must be the last
    // one in the list (if there is an entry for this BB)
    if (r->dus.isEmpty() || r->dus.last()->bb != this) {
      // PReg doesn't yet have an entry for this block
#	ifdef ASSERT
	thisBB = this;
	r->dus.apply(duChecker);
#	endif
      duInfo.info->append(new DUInfo(r));
      r->dus.append(new PRegBBIndex(this, duInfo.info->length() - 1, r));
    }
    p = r->dus.last();
  }
  assert(p->bb == this, "wrong BB");
  assert(duInfo.info->at(p->index)->reg == r, "wrong PReg");
  return p->index;
}


Use* BB::addUse(NonTrivialNode* n, PReg* r, bool soft) {
  assert(contains(n), "node isn't in this BB");
  if (r->isNoPReg()) return NULL;
  Use* u = soft ? new PSoftUse(n) : new Use(n);
  r->incUses(u);
  int index = addUDHelper(r);
  duInfo.info->at(index)->uses.append(u);
  return u;
}


Def* BB::addDef(NonTrivialNode* n, PReg* r) {
  assert(contains(n), "node isn't in this BB");
  if (r->isNoPReg()) return NULL;
  Def* d = new Def(n);
  r->incDefs(d);
  int index = addUDHelper(r);
  duInfo.info->at(index)->defs.append(d);
  return d;
}


// allocate PRegs that are used & defined solely within this BB
void BB::localAlloc(GrowableArray<BitVector*>* hardwired, 
  		    GrowableArray<PReg*>* localRegs,
		    GrowableArray<BitVector*>* lives) {
  // try fast allocation first -- use only local regs that aren't touched
  // by any pre-allocated (hardwired) registers

  // hardwired, localRegs, lives: just passed on to slowLocalAlloc

  // Note: Fix problem with registers that are used after a call in PrimNodes
  // such as ContextCreateNode, etc. Problem is that values like self might
  // be allocated in registers but the registers are trashed after a call.
  // Right now: PrologueNode terminates BB to fix the problem.

  if (!nnodes) return;    	    // empty BB

  GrowableArray<RegCandidate*> cands(nnodes);
  GrowableArray<RegisterEqClass*> regClasses(nnodes + 1);
  regClasses.append(NULL);   	    // first reg class has index 1

  int  use_count[nofRegisters], def_count[nofRegisters];
  for (int i = 0; i < nofRegisters; i++) use_count[i] = def_count[i] = 0;

  for (Node* nn = first; nn != last->next(); nn = nn->next()) {
    if (nn->deleted) continue;
    nn->markAllocated(use_count, def_count);
    if (nn->isAssignNode()) {
      NonTrivialNode* n = (NonTrivialNode*)nn;
      PReg* src = n->src();
      PReg* dest = n->dest();
      bool localSrc  = src ->isLocalTo(this);
      bool localDest = dest->isLocalTo(this);
      if (src->loc.isRegisterLocation()) {
	if (dest->loc.equals(unAllocated) && localDest) {
	  // PR = PR2(reg)
	  // allocate dest->loc to src->loc, but only if src->loc
	  // isn't defined again
	  cands.append(new RegCandidate(dest, src->loc, def_count[src->loc.number()]));
	}
      } else if (dest->loc.isRegisterLocation()) {
	if (src->loc.equals(unAllocated) && localSrc) {
	  // PR2(reg) = PR
	  // should allocate src->loc to dest->loc, but only if dest->loc
	  // has single definition (this one) and isn't used before
	  // this point   [simplification]
	  if (def_count[dest->loc.number()] != 1 || use_count[dest->loc.number()]) {
	    // not eligible for special treatment
	  } else {
	    cands.append(new RegCandidate(src, dest->loc, 1));
	  }
	}
      } else if (localSrc && localDest) {
	// both regs are local and unallocated - put them in same
	// equivalence class
	// fix this - must check for overlapping live ranges first
      } else {
	// non-local registers - skip
      }
    }
  }

  // now examine all candidates and allocate them to preferred register
  // if possible
  while (cands.nonEmpty()) {
    RegCandidate* c = cands.pop();
    if (def_count[c->loc.number()] == c->ndefs) {
      doAlloc(c->r, c->loc);
    }
  }

  // allocate other local regs (using the untouched temp regs of this BB)
  int temp = 0;
  for (i = 0; i < duInfo.info->length(); i++) {
    // collect local regs 
    PReg* r = duInfo.info->at(i)->reg;
    if (r->loc.equals(unAllocated) && !r->isUnused() && r->isLocalTo(this)) {
      assert(r->dus.first()->index == i, "should be the same");
      for ( ; temp < nofLocalRegisters &&
	   use_count[Mapping::localRegister(temp).number()] + def_count[Mapping::localRegister(temp).number()] > 0;
	   temp++) ;
      if (temp == nofLocalRegisters) break;	    // ran out of regs
      // ok, allocate Mapping::localRegisters[temp] to the preg and equivalent pregs
      Location t = Mapping::localRegister(temp++);
      PReg* frst = r->regClass ? regClasses.at(r->regClass)->first : r;
      for (PReg* pr = frst; pr;
	   pr = pr->regClassLink) {
	doAlloc(pr, t);
	pr->regClass = 0;
      }
    }
    r->regClass = 0;
  }

  if (temp == nofLocalRegisters) {
    // ran out of local regs with the simple strategy - try using slow
    // allocation algorithm
    slowLocalAlloc(hardwired, localRegs, lives);
  }
}

	
// slower but more capable version of local allocation; keeps track of live
// ranges via a bit map
// note: temporary data structs are passed in so they can be reused for all
// BBs (otherwise would allocate too much junk in resource area)
void BB::slowLocalAlloc(GrowableArray<BitVector*>* hardwired, 
  			GrowableArray<PReg*>* localRegs,
			GrowableArray<BitVector*>* lives) {
  // clear temporary data structures
  localRegs->clear();
  lives->clear();
  for (int i = 0; i < nofLocalRegisters; i++) {
    hardwired->at(i)->setLength(nnodes);
    hardwired->at(i)->clear();
  }
  // hardwired->at(i): indexed by reg no, gives nodes in which register is busy
  // localRegs: collects all PRegs that could be allocated locally
  // lives: for each reg in localRegs, holds live range (bit vector with one bit per node)

  for (i = 0; i < duInfo.info->length(); i++) {
    // collect local regs
    PReg* r = duInfo.info->at(i)->reg;
    if (r->isLocalTo(this)) {
      assert(r->dus.first()->index == i, "should be the same");
      if (r->isUnused()) {
	// unused register - ignore
      } else {
	DUInfo* info = duInfo.info->at(r->dus.first()->index);
	localRegs->append(r);
	BitVector* bv = new BitVector(nnodes);
	lives->append(bv);
	int firstUse = 0, lastUse = nnodes - 1;
	duInfo.info->at(i)->getLiveRange(firstUse, lastUse);
	bv->addFromTo(firstUse, lastUse);
      }
    } else if (r->loc.isLocalRegister()) {
      // already allocated (i.e., hardwired register)
      assert(!r->loc.equals(unAllocated), "unAllocated should not count as isRegister()");
      int firstUse = 0, lastUse = nnodes - 1;
      if (!r->incorrectDU()) {
	duInfo.info->at(i)->getLiveRange(firstUse, lastUse);
      } else {
	// can't really compute live range since the temp might be non-local
	// so assume it's live from first node til the end
      }
      hardwired->at(Mapping::localRegisterIndex(r->loc))->addFromTo(firstUse, lastUse);
    }
  }

  // now, localRegs holds all local regs, and lives contains each register's
  // live range (one bit per node, 1 = reg is live); hardwired contains
  // the ranges where temp regs are already taken (e.g. for NLR, calls, etc)

  // just add trashed registers now
  for (Node* n = first; n != last->next(); n = n->next()) {
    if (n->deleted) continue;
    SimpleBitVector v = n->trashedMask();
    if (v.isEmpty()) continue;	// nothing trashed (normal case)
    for (int i = 0; i < nofLocalRegisters; i++) {
      if (v.isAllocated(i)) hardwired->at(i)->add(n->num());
    }
  }


  // cycle through the temp registers to (hopefully) allow more optimizations
  // later (e.g. scheduling)
  int lastTemp = 0;
#   define nextTemp(n) (n == nofLocalRegisters - 1) ? 0 : n + 1

  for (i = 0; i < localRegs->length(); i++) {
    // try to allocate localRegs[i] to a local (temp) register
    PReg* r = localRegs->at(i);
    if (!r->loc.equals(unAllocated)) {
      assert(r->regClass == 0, "should have been cleared");
      continue;
    }
    BitVector* liveRange = lives->at(i);
    for (int tempNo = lastTemp, ntries = 0; ntries < nofLocalRegisters;
	 tempNo = nextTemp(tempNo), ntries++) {
      if (liveRange->isDisjointFrom(hardwired->at(tempNo))) {
	Location temp = Mapping::localRegister(tempNo);
	doAlloc(r, temp);
	hardwired->at(tempNo)->unionWith(liveRange);
	lastTemp = nextTemp(tempNo);
	break;
      }
    }
    if (CompilerDebug) {
      if (r->loc.equals(unAllocated)) {
	cout(PrintLocalAllocation)->print("*could NOT find local assignment for local %s in BB%ld\n", r->name(), id());
      }
    }
    r->regClass = 0;
  }
}


void BB::doAlloc(PReg* r, Location l) {
  if (CompilerDebug) cout(PrintLocalAllocation)->print("*assigning %s to local %s in BB%ld\n", l.name(), r->name(), id());
  assert(!r->debug, "should not allocate to temp reg");
  r->loc = l;
}


void BB::computeEscapingBlocks(GrowableArray<BlockPReg*>* l) {
  // add all escaping blocks to l
  for (Node* n = first; n != last->next(); n = n->next()) {
    if (n->deleted) continue;
    n->computeEscapingBlocks(l);
  }
}


void BB::apply(NodeVisitor* v) {
  if (nnodes > 0) {
    Node* end = last->next();
    for (Node* n = first; n != end; n = n->next()) {
      if (!n->deleted) {
	v->beginOfNode(n);
	n->apply(v);
	v->endOfNode(n);
      }
    }
  } else {
  #ifdef ASSERT
    // just checking...
    assert(nnodes == 0, "nnodes should be 0");
    Node* end = last->next();
    for (Node* n = first; n != end; n = n->next()) {
      assert(n->deleted, "basic block is not empty even though nnodes == 0!");
    }
  #endif
  }
}


bool BB::verifyLabels() {
  bool ok = true;
  if (nnodes > 0) {
    for (Node* n = first; n != last->next(); n = n->next()) {
      if (n->deleted) continue;
      if (n->label.is_unbound()) {
	ok = false;
	lprintf("unbound label at N%d\n", n->id());
      }
    }
  }
  return ok;
}


static void printPrevBBs(BB* b, const char* str) {
  lprintf("BB%ld%s", b->id(), str);
}


void BB::print_short() {
  lprintf("BB%-3ld[%d] %#lx (%ld, %ld); prevs ", id(), _loopDepth, this, first->id(), last->id());
  for (int i = 0; i < nPredecessors(); i++) printPrevBBs(prev(i),  (i == nPredecessors() - 1) ? " : " : ", ");
  lprintf("; ");
  if (next ()) lprintf("next BB%ld", next ()->id());
  for (i = 1; i < nSuccessors(); i++) printPrevBBs(next(i), (i == nSuccessors() - 1) ? " : " : ", ");
}


void BB::print() {
  print_short(); lprintf("(%ld nodes):\n", nnodes);
  print_code(false);
  lprintf("duInfo: "); duInfo.print();
}


void BB::print_code(bool suppressTrivial) {
  for (Node* n = first; n != last->next(); n = n->next()) {
    if (n->deleted && !(n == first || n == last)) continue;
    if (suppressTrivial && n->isTrivial()) {
      // don't print
    } else {
      n->printID(); n->print_short(); lprintf("\n");
    }
  }
}


bool BB::contains(const Node* which) const {
  for (Node* n = first; n != last->next(); n = n->next()) {
    if (n == which) return true;
  }
  return false;
}


void BB::verify() {
  int count = 0;
  for (Node* n = first; n != last->next(); n = n->next()) {
    count++;
    if (n->deleted) continue;
    n->verify();
    if (n->bb() != this)
      error("BB %#lx: Node %#lx doesn't point back to me", this, n);
    if (n == last && !n->endsBB() &&
	!(n->next() && n->next()->isMergeNode() &&
	  ((MergeNode*)(n->next()))->didStartBB)) {
      error("BB %#lx: last Node %#lx isn't endsBB()", this, n);
    }
    if (n->endsBB() && n != last)
      error("BB %#lx: Node %#lx ends BB but isn't last node", this, n);
  }
  if (count != nnodes) error("incorrect nnodes in BB %#lx", this);
  if (_loopDepth < 0) error  ("BB %#lx: negative loopDepth %d", this, _loopDepth);
  
  // Fix this Urs, 3/11/96
  if (_loopDepth > 9) warning("BB %#lx: suspiciously high loopDepth %d", this, _loopDepth);
}


BBIterator* bbIterator;

void BBIterator::build(Node* first) {
  assert(bbTable == NULL, "already built");
  _first = first;
  bbCount = 0;
  pregTable = globals = NULL;
  buildBBs();
  blocksBuilt = true;
}


void BBIterator::buildBBs() {
  // build basic blocks
  // first, sort them topologically (ignoring backward arcs)
  // some things (e.g. SplitPReg::isLiveAt) depend on correct ordering
  GrowableArray<BB*>* list = new GrowableArray<BB*>(max(BasicNode::currentID >> 3, 10));
  _first->newBB()->dfs(list, 0);
  // now, the list contains the BBs in reverse order
  bbCount = list->length();
  bbTable = new GrowableArray<BB*>(bbCount);
  for (int i = bbCount-1; i >= 0; i--) {
    BB* bb = list->at(i);
    bb->_id = bbCount - i - 1;
    bbTable->append(bb);
  }
}


void BB::dfs(GrowableArray<BB*>* list, int loopDepth) {
  // build BB graph with depth-first traversal
  if (_id == 1) return;
  _id = 1;		// mark as visited
#ifdef fix_this
  setting of isLoopStart/End is broken -- fix this (currently not used)
  if (first->isMergeNode()) {
    MergeNode* m = (MergeNode*)first;
    if (m->isLoopStart) {
      loopDepth++;
    } else if (m->isLoopEnd) {
      assert(loopDepth > 0, "bad loop end marker");
      loopDepth--;
    }
  }
#endif
_loopDepth = loopDepth;
  // Note: originally this code simply skipped missing (NULL) successors;
  //       however it doesn't work correctly because if a node (or a BB)
  //       has n successors they're all assumed to be non-NULL. Code with
  //       missing successors (e.g. TypeTestNode with no next(0) = NULL)
  //       cause the BB graph to screw up after this node. (gri 7/22/96)
  int n = last->nSuccessors();
  for (int i = 0; i < n; i++) {
    Node* next = last->next(i);
    BB* nextBB = next->newBB();
  }	
  for (i = nSuccessors() - 1; i >= 0; i--) {
    BB* nextBB = next(i);
    // only follow the link if next->bb hasn't been visited yet
    if (nextBB->id() == 0) nextBB->dfs(list, loopDepth);
  }
  list->append(this);
}


static void clearNodes(BB* bb) {
  for (Node* n = bb->first; n != bb->last->next(); n = n->next()) {
    n->setBB(NULL);
  }
}


void BBIterator::clear() {
  apply(clearNodes);
  bbTable = NULL; pregTable = globals = NULL;
}


void BBIterator::makeUses() {
  // a few PRegs may be created during makeUses (e.g. for deadBlockObj,
  // true/false etc), so leave a bit of room
  const int ExtraPRegs = 10;
  int n = PReg::currentNo + ExtraPRegs;
  pregTable = new GrowableArray<PReg*>(n);
  for (int i = 0; i < n; i++) pregTable->append(NULL);
  for (i = 0; i < bbCount; i++) { bbTable->at(i)->makeUses(); }
  usesBuilt = true;
}


void BBIterator::localAlloc() {
  // speed/space optimization: allocate hardwired et al only once, not for
  // every BB
  { ResourceMark rm;
    GrowableArray<BitVector*> hardwired(nofLocalRegisters, nofLocalRegisters, NULL); 
    GrowableArray<PReg*> localRegs(BasicNode::currentID);
    GrowableArray<BitVector*> lives(BasicNode::currentID);    
    for (int i = 0; i < nofLocalRegisters; i++) {
      hardwired.at_put(i, new BitVector(roundTo(BasicNode::currentID, BitsPerWord)));
    }
  
    for (i = 0; i < bbCount; i++) {
      bbTable->at(i)->localAlloc(&hardwired, &localRegs, &lives);
    }
  }

  int done = 0;
  globals = new GrowableArray<PReg*>(pregTable->length());
  for (int i = 0; i < pregTable->length(); i++) {
    PReg* r = pregTable->at(i);
    if (r) {
      if (r->isUnused()) {
	pregTable->at_put(i, NULL);		// no longer needed
      } else if (r->loc.equals(unAllocated)) {
	globals->append(r);
      } else {
	done++;				// locally allocated
      }
    }
  }
  if (PrintLocalAllocation) {
    int total = globals->length() + done;
    lprintf("*local reg. allocations done; %ld out of %ld = (%3.1f%%).\n",
	   done, total, 100.0 * done / total);
  }
}


void BBIterator::print() {
  for (int i = 0; i < bbCount; i++) {
    lprintf("  "); bbTable->at(i)->print_short(); lprintf("\n");
  }
}


void BBIterator::localCopyPropagate() {
  for (int i = 0; i < bbCount; i++) { bbTable->at(i)->localCopyPropagate(); }
}


void BBIterator::bruteForceCopyPropagate() {
  for (int i = 0; i < bbCount; i++) { bbTable->at(i)->bruteForceCopyPropagate(); }
}


void BBIterator::computeEscapingBlocks() {
  // Escape analysis for blocks: compute initial approximation (lower bound)
  // by collecting all blocks that are stored into the heap or returned from
  // nmethods.
  // Terminology: an "escaping" block is a block closure that
  // could potentially be created at runtime and passed to some code which
  // could store it in the heap (thus "escape").  An escaping block could
  // thus be invoked during any non-inlined send (since the callee could 
  // read the block from the heap and send value to it) or after the method
  // has returned.  Consequently, the variables uplevel-accessed by an escaping
  // block cannot be stack-allocated, and any uplevel-written variables cannot
  // be cached across calls.
  exposedBlks = new GrowableArray<BlockPReg*>(BlockPReg::numBlocks());
  for (int i = 0; i < bbCount; i++) {
    bbTable->at(i)->computeEscapingBlocks(exposedBlks);
  }
}


void BBIterator::computeUplevelAccesses() {
  // Compute the set of uplevel-accessed variables for each exposed block.
  // Terminology: variables are considered "uplevel-accessed" only if they
  // are accessed by an exposed block.  I.e., if a block is completely inlined,
  // it's uplevel accesses aren't really "uplevel" anymore and thus are ignored.
  int len = exposedBlks->length();
  for (int i = 0; i < len; i++) {
    BlockPReg* r = exposedBlks->at(i);
    assert (r->parent() == r->scope(), "block preg was copied/changed scope");
    assert(r->scope()->isInlinedScope(), "must be code scope");
    r->computeUplevelAccesses();
  }
}


bool BBIterator::isSequentialCode(BB* curr, BB* next) const {
  return curr == next || 
         (curr->genCount() + 1 == next->genCount() &&
	  curr->next() == next);
}


bool BBIterator::isSequential(int curr, int next) const {
  // is control flow between current and next BB sequential?
  if (curr == next) return true;
  if (next < bbCount && bbTable->at(curr)->next() != NULL &&
      bbTable->at(curr)->next() != bbTable->at(next)) {
    return false; // non-sequential control flow
  }

  if (next == bbCount && bbTable->at(curr)->next() != NULL) {
    // we're "running off the end", so must be non-seq
    return false;
  }

  return true;	    // none of the above, so must be sequential
}


BB* BBIterator::bb_for(Node* n) {
  return n != NULL ? n->bb() : NULL;
}


void BBIterator::add_BBs_to_list(GrowableArray<BB*>& list, GrowableArray<BB*>& work) {
  if (!work.isEmpty()) {
    GrowableArray<BB*> uncommon;	// uncommon BBs are handled at the very end
    do {
      BB* bb = work.pop();
      assert(bb->visited(), "must have been visited");
      list.append(bb);			// do this even if empty (label may be referenced)
      bb->setGenCount();		// can probably be removed at some point
      // determine likely & uncommon successors
      BB* likely_next   = bb_for(bb->last->likelySuccessor());
      BB* uncommon_next = bb_for(bb->last->uncommonSuccessor());
      assert(likely_next != uncommon_next || likely_next == NULL, "likely BB cannot be uncommon BB at the same time");
      // first, push all other successors
      for (int i = bb->nSuccessors(); i-- > 0; ) {
        BB* next = (BB*)bb->next(i);
        if (next != NULL && !next->visited() && next != likely_next && next != uncommon_next) {
	  work.push(next->after_visit());
	}
      }
      // then, push likely successor (will be handled next)
      if (likely_next != NULL && !likely_next->visited()) {
        work.push(likely_next->after_visit());
      }
      // remember uncommon successor for the very end
      if (uncommon_next != NULL && !uncommon_next->visited()) uncommon.push(uncommon_next->after_visit());
    } while(!work.isEmpty());
    // add all the uncommon cases
    add_BBs_to_list(list, uncommon);
  }
}


GrowableArray<BB*>* BBIterator::code_generation_order() {
  if (!ReorderBBs) return bbTable;
  // initialize visited field for all nodes
  for (int i = 0; i < bbCount; i++) bbTable->at(i)->before_visit();
  // working sets
  GrowableArray<BB*>* list = new GrowableArray<BB*>(bbCount);	// eventually holds all reachable BBs again
  GrowableArray<BB*>  work(bbCount);				// may hold only a part of all BBs at any given time
  // setup starting point
  BB* bb = _first->bb();
  work.push(bb->after_visit());
  // compute order
  add_BBs_to_list(*list, work);
  // NB: "loosing" BBs is ok since some may become unreachable during optimization  -Robert 7/96
  assert(list->length() <= bbCount, "gained BBs?");
  return list;
}


void BBIterator::print_code(bool suppressTrivial) {
  GrowableArray<BB*>* list = code_generation_order();
  for (int i = 0; i < list->length(); i++) {
    BB* bb = list->at(i);
    if (bb->nnodes > 0) bb->print_code(suppressTrivial);
    if (bb->next() != NULL) std->print_cr("\tgoto N%d", bb->next()->first->id());
    if (!suppressTrivial) lprintf("\n");
  }
}


void BBIterator::apply(NodeVisitor* v) {
  GrowableArray<BB*>* list = code_generation_order();
  int length = list->length();
  for (int i = 0; i < length; i++) {
    BB*   bb    = list->at(i);
    Node* first = bb->first;
    Node* last  = bb->last;
    v->beginOfBasicBlock(first);
    bb->apply(v);
    v->endOfBasicBlock(last);
  }
}


bool BBIterator::verifyLabels() {
  bool ok = true;
  for (int i = 0; i < bbCount; i++) ok &= bbTable->at(i)->verifyLabels();
  return ok;
}


void BBIterator::globalCopyPropagate() {
  // do global copy propagation of singly-assigned PRegs

  for (int i = 0; i < pregTable->length(); i++) {
    PReg* r = pregTable->at(i);
    if (!r || r->isConstPReg() || !r->canCopyPropagate()) continue;
    Def* def = NULL;
    // get definition
    int dulength = r->dus.length();
    for (int e = 0; e < dulength; e++) {
      PRegBBIndex* index = r->dus.at(e);
      DUInfo* info = index->bb->duInfo.info->at(index->index);
      if (info->defs.length()) {
	assert(info->defs.length() == 1, "should be one definition only");
	assert(def == NULL, "already have def!?!");
	def = info->defs.first();
#	ifndef ASSERT
	  break;
#	endif
      }
    }
    assert(def, "should have found the definition");
    NonTrivialNode* defNode = def->node;
      
    // don't propagate if the defNode isn't assignment
    if (!defNode->isAssignmentLike()) continue;
    assert(defNode->dest() == r, "not assignment-like");

    PReg* defSrc = defNode->src();

    // don't propagate if source does not survive calls (i.e. is local to BB), 
    // or if it isn't singly-assigned as well (if it's multiply assigned,
    // we'd have to make sure it's not assigned between defNode and the use nodes
    if (!defSrc->isSinglyAssigned() || defSrc->loc.isTrashedRegister())
	continue;

    // ok, everything is fine - propagate the def to the uses
    for (e = 0; e < dulength; e++) {
      PRegBBIndex* index = r->dus.at(e);
      BB* bb = index->bb;
      DUInfo* info = bb->duInfo.info->at(index->index);
      // caution: propagateTo may eliminate nodes and thus shorten
      // info->uses
      int j = 0;
      while (j < info->uses.length()) {
	int oldLen = info->uses.length();
	info->propagateTo(bb, info->reg, def, info->uses.at(j), true);
	if (info->uses.length() == oldLen) {
	  // propagate didn't eliminate this use; try next one
	  j++;
	}
      }
    }
  }
}


void BBIterator::eliminateUnneededResults() {
  // eliminate nodes producing results that are never used
  for (int i = 0; i < pregTable->length(); i++) {
    PReg* r = pregTable->at(i);
    if (r && r->isOnlySoftUsed()) {
      r->eliminate(false);
    }
  }
}


void BBIterator::apply(BBDoFn f) {
  for (int i = 0; i < bbCount; i++) { f(bbTable->at(i)); }
}


void BBIterator::verify() {
  if (bbTable) {
    for (int i = 0; i < bbCount; i++) { bbTable->at(i)->verify(); }
    if (pregTable) {
      for (i = 0; i < pregTable->length(); i++) {
        if (pregTable->at(i)) pregTable->at(i)->verify();
      }
    }
  }
}


# endif
