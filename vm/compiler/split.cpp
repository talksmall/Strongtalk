/* Copyright 1994, LongView Technologies. $Revision: 1.6 $ */
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


/************* This file is not currently used and should not be in the build */

# include "incls/_precompiled.incl"

# ifdef COMPILER

# include "incls/_split.cpp.incl"

  const uint32 SplitSig::LevelMask = 0xf;

  struct SplitSetting : StackObj {
    SplitSig*& sig;
    SplitSig* saved;
    SplitSetting(SplitSig*& oldsig, SplitSig* newsig) : sig(oldsig) {
      saved = oldsig; oldsig = newsig;
    }
    ~SplitSetting() { sig = saved; }
  };

  SplitSig* new_SplitSig(SplitSig* current, int splitID) {
    int level = current->level() + 1;
    assert(level <= MaxSplitDepth, "max. split level exceeded");
    uint32 newID = splitID << ((MaxSplitDepth - level + 1) << 2);
    SplitSig* sig =
      (SplitSig*)((uint32(current) & ~SplitSig::LevelMask) | newID | level);
    assert(current->contains(sig), "should be in same branch");
    return sig;
  }

  void SplitSig::print() {
    char buf[MaxSplitDepth + 1];
    lprintf("SplitSig %#lx: %s", this, prefix(buf));
  }

  char* SplitSig::prefix(char* buf) {
    // fill buf with an ASCII representation of the receiver and return buf
    // e.g. a level-2 sig with first branch = 1 and 2nd branch = 3 --> "AB"
    int l = level();
    buf[l--] = 0;
    uint32 sig = uint32(this) >> 4;
    while (l >= 0) {
      buf[l--] = 'A' + (sig & 0xf);
      sig = sig >> 4;
    }
    return buf;
  }

  // compiler code for splitting

  bool CodeScope::shouldSplit(SendInfo* info) {
    assert(info->rcvr->isMergeExpr(), "should be merge expr");
    MergeExpr* r = (MergeExpr*) info->rcvr;
    assert(r->isSplittable(), "should be splittable");
    if (!CompilerSplitting) return false;
    if (sig->level() == MaxSplitDepth) return false;
    Node* current = theNodeGen->current;
    if (!current->isSplittable()) return false;

    int cost = theCompiler->inlineLimit[SplitCostLimit];
    Node* n = NULL;
    // compute the cost of all nodes that would be copied (i.e. all exprs
    // with a map type)
    int i;
    for (i = 0; i < r->exprs->length(); i++) {
      Expr* expr = r->exprs->at(i);
      if (!expr->hasKlass()) continue;    // won't copy these
      InlinedScope* theScope = expr->node()->scope();
      int theBCI = expr->node()->bci();
      for (Expr* e = expr; e; e = e->next) {
	if (e->node()->scope() != theScope || e->node()->bci() != theBCI) {
	  // make sure all subexpressions have the same scope
	  // (otherwise can't describe live range of that value when split)
	  // could fix this with better splitting (introduce temps to
	  // "synchronize" the value's scopes)
	  if (PrintInlining) {
	    lprintf("%*s*not splitting %s: too complicated (scopes)\n",
		    depth, "", info->sel->copy_null_terminated());
	  }
	  r->setSplittable(false);  	// no sense trying again
	  return false;
	}
	Node* prev;
	for (n = e->node(); cost > 0 && n && n != current; n = n->next()) {
	  cost -= n->cost();
	  if (!n->isSplittable()) {
	    if (PrintInlining) {
	      lprintf("%*s*not splitting %s: unsplittable node\n",
		     depth, "", info->sel->copy_null_terminated());
	    }
	    return false;
	  }
#	  ifdef ASSERT
	    prev = n;	    // for easier debugging
#	  endif
	}
	assert(n, "why didn't we find current?");
	if (n == NULL || cost < 0) goto done;
      }
    }
    
   done:
    if (n != current || cost < 0) {
      if (PrintInlining) {
	lprintf("%*s*not splitting %s: cost too high (>%ld)\n", depth, "",
	       info->sel->copy_null_terminated(),
	       theCompiler->inlineLimit[SplitCostLimit] - cost);
      }
      if (n == current) theCompiler->registerUninlinable(info, SplitCostLimit,cost);
      return false;
    }
    
    return true;
  }

  Expr* CodeScope::splitMerge(SendInfo* info, MergeNode*& merge) {
    // Split this send: copy nodes between sources of r's parts and the current
    // node, then inline the send along all paths; merge paths back at merge,
    // result in info->resReg.
    MergeExpr* r = (MergeExpr*)info->rcvr;
    assert(r->isSplittable(), "should be splittable");

    // performance bug - fix this: can't split on MergeExpr more than once
    // because after changing the CFG the old expr points to the wrong nodes.
    // For now, just prohibit future splitting on this expr.
    r->setSplittable(false);
    
    Expr* res = NULL;
    int ncases = r->exprs->length();
    memoizeBlocks(info->sel);
    if (PrintInlining) {
      lprintf("%*s*splitting %s\n", depth, "", selector_string(info->sel));
    }
    Node* current = theNodeGen->current;
    bool first = true;
    GrowableArray<oop>*  splitRcvrKlasss = new GrowableArray<oop>(10);// receiver map of each branch
    GrowableArray<PReg*>* splitRcvrs = new GrowableArray<PReg*>(10);	// receiver reg of each branch
    GrowableArray<Node*>* splitHeads = new GrowableArray<Node*>(10);	// first node of each branch
    bool needKlassLoad = false;
    
    for (int i = 0; i < ncases; i++) {
      Expr* nth = r->exprs->at(i);
      assert(!nth->isConstantExpr() || nth->next == NULL ||
	     nth->constant() == nth->next->constant(),
	     "shouldn't happen: merged consts - convert to map");
      InlinedScope* s;
      if (nth->hasKlass() &&
	  (s = tryLookup(info, nth)) != NULL) {
	// Create a new PReg&Expr for the receiver so that it has the right
	// scope (the nodes will replace the old result reg with the new
	// one while they are copied).  
	// (Since we're splitting starting at the original producer, the
	// original result may be in an arbitrary subscope of the receiver
	// or even in a sibling.  For reg. allocation et al., the result
	// must have a PReg that's live from the producing point to here.)
	SplitSetting setting(theCompiler->splitSig, new_SplitSig(sig, i + 1));
	if (s->isCodeScope()) ((CodeScope*)s)->sig = theCompiler->splitSig;
	SplitPReg* newPR = coveringRegFor(nth, theCompiler->splitSig);
	Expr* newRcvr = nth->shallowCopy(newPR, nth->node());
	
	Node* mapMerge = new MergeNode;	    // where all copied paths merge
	splitHeads->append(mapMerge);
	splitRcvrs->append(newPR);
	if (nth->isConstantExpr()) {
	  splitRcvrKlasss->append(nth->constant());
	} else {
	  splitRcvrKlasss->append(nth->klass());
	  Klass* m = nth->klass()->addr();
	  needKlassLoad |= m != smiKlassObj && m != doubleKlassObj;
	}

	// split off paths of all Exprs with this map up to merge point
	Node* rmerge = r->node();
	assert(rmerge, "should have a node");
	for (Expr* expr = nth; expr; expr = expr->next) {
	  Node* n = expr->node();
	  PReg* oldPR = expr->preg();
	  assert(n->isSplittable(), "can't handle branches etc. yet");
	  Node* frst = n->next();
	  n->removeNext(frst);
	  // replace n's destination or insert an assignment
	  if (n->hasDest() && n->dest() == oldPR) {
	    n->setDest(NULL, newPR);
	  } else if (newPR) {
	    n = n->append(new AssignNode(oldPR, newPR));
	  }
	  n = copyPath(n, frst, rmerge, oldPR, newPR, r, newRcvr);
	  n = n->append(mapMerge);
	}

	// copy everything between mapMerge and current
	theNodeGen->current = copyPath(mapMerge, rmerge, current,
				       NULL, NULL, r, newRcvr);
	
	// now inline the send
	Expr* e = doInline(s, newRcvr, theNodeGen->current, NULL);
	if (!e->isNoResultExpr()) {
	  theNodeGen->append(new NopNode);
	  e = e->shallowCopy(info->resReg, theNodeGen->current);
	  res = res ? res->mergeWith(e, merge) : e;
	}
	theNodeGen->branch(merge);
      } else {
	// can't inline - need to append a real send after current
	if (!nth->isUnknownUnlikely()) info->needRealSend = true;
      }
    }

    UnknownExpr* u = r->findUnknown();
    if (u && splitRcvrKlasss->length() > 0) {
      // insert a type test after oldMerge (all unknown paths should meet
      // at that node)
      // Performance bug: the known-but-uninlinable sends will also go
      // through the type test; they should be redirected until after the
      // test.  The problem is that oldMerge may not be the actual merge
      // point but slightly later (i.e. a few InlinedReturns later).
      int diff;
      if (WizardMode && PrintInlining &&
	  (diff = r->exprs->length() - splitRcvrKlasss->length()) > 1) {
	lprintf("*unnecessary %d-way type test for %d cases\n",
	       splitRcvrKlasss->length(), diff);
      }
      Node* oldMerge = r->node();
      Node* oldNext = oldMerge->next();
      if (oldNext) oldMerge->removeNext(oldNext);
      PReg* pr = r->preg();
      Node* typeCase = new TypeTestNode(pr, splitRcvrKlasss, needKlassLoad, true);
      oldMerge->append(typeCase);
      if (info->needRealSend || !theCompiler->useUncommonTraps) {
	// connect fall-through (unknown) case to old merge point's successor
	info->needRealSend = true;
	if (oldNext) {
	  typeCase->append(oldNext);
	} else {
	  assert(current == oldMerge, "oops");
	  current = typeCase->append(new NopNode);
	}
      } else {
	// make unknown case uncommon
	if (oldNext) {
	  // must copy nodes between old merge point (i.e. end of the send
	  // generating the receiver value) and current node; this code
	  // computes the args of the current send
	  theNodeGen->current = copyPath(typeCase, oldNext, current,
					 NULL, NULL, r, u);
	} else {
	  assert(current == oldMerge, "oops");
	  theNodeGen->current = typeCase;
	}
	theNodeGen->uncommonBranch(currentExprStack(0), info->restartPrim);
	if (PrintInlining) {
	  lprintf("%*s*making %s uncommon (3)\n",
		 depth, "", selector_string(info->sel));
	}
      }
      for (int j = 0; j < splitRcvrKlasss->length(); j++) {
	Node* n = new AssignNode(pr, splitRcvrs->at(j));
	typeCase->append(j + 1, n);
	n->append(splitHeads->at(j));
      }
    }
    if (info->needRealSend) {
      // The non-inlined send will be generated along the original (merged)
      // path, which will then branch to "merge".
      theNodeGen->current = current;
    } else {
      // discard the original path - can no longer reach it
      theNodeGen->current = merge;
    }

    if (res && res->isMergeExpr()) res->setNode(merge, info->resReg);
    return res;
  }

  Node* CodeScope::copyPath(Node* n, Node* start, Node* end,
			     PReg* oldPR, PReg* newPR,
			     MergeExpr* rcvr, Expr* newRcvr) {
    // copy the path from start to end, replacing occurrences of oldPR
    // with newPR; append copies to n, return last node
    if (CompilerDebug) {
      char* s = NEW_RESOURCE_ARRAY(char, 100);
      sprintf(s, "start of copied code: %#lx(N%d) --> %#lx(N%d) @ %#lx(N%d)",
	      start, start->id(), end, end->id(), n, n->id());
      n = n->append(new CommentNode(s));
    } 
    assert(!oldPR || !oldPR->isBlockPReg(), "cannot handle BlockPRegs");
    for (Node* c = start; true; c = c->next()) {
      assert(c->isSplittable(), "can't handle branches yet");
      Node* copy = c->copy(oldPR, newPR);
      if (copy) n = n->append(copy);
      if (c == end) break;
    }
    if (CompilerDebug) n = n->append(new CommentNode("end of copied code"));
    return n;
  }

  SplitPReg* CodeScope::coveringRegFor(Expr* expr, SplitSig* sg) {
    // create a PReg with a live range covering all nodes between the
    // producer and the receiver scope/bci
    // see also SAPReg::isLiveAt
    InlinedScope* s = expr->node()->scope();
    int bci = expr->node()->bci();
    assert(s->isCodeScope(), "oops");
    SplitPReg* r = regCovering(this, _bci, (CodeScope*)s, bci, sg);
#   ifdef ASSERT
      for (Expr* e = expr; e; e = e->next) {
	InlinedScope* s2 = e->node()->scope();
	int bci2 = e->node()->bci();
	assert(s2 == s, "oops");
	assert(bci2 == bci, "oops");
      }
#   endif
    return r;
  }

# endif
