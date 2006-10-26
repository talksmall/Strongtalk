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

/************* This file is not currently used and should not be in the build */

# include "incls/_precompiled.incl"

# ifdef COMPILER

# include "incls/_inline.cpp.incl"

  static int msgCost = 0;  	// estimated cost of last inlining candidate
  

  void CodeScope::memoizeBlocks(symbolOop sel) {
    // memoize block args so they aren't created for inlined cases
    int top = exprStack->length();
    int argc = sel->arg_count();
    for (int i = 1; i <= argc; i++) {
      PReg* r = exprStack->nth(top - i)->preg();
      if (r->isBlockPReg()) ((BlockPReg*)r)->memoize();
    }
  }

  int InlinedScope::calleeSize(RScope* rs) {
    // try to get the callee's size (in bytes)
    if (!rs->isPICScope()) return 0;	// no info
    RPICScope* ps = (RPICScope*)rs;
    nmethod* inlinee = ps->nm;
    if (inlinee->compiler() == NIC) {
      return 0; 	// can't say much about the real code size
    }
    int size = inlinee->instsLen() - oopSize * PrologueSize;
    if (inlinee->isUncommonRecompiled()) {
      // uncommon nmethods are bigger because the contain many more non-
      // inlined sends for all the uncomon cases
      size /= 2;
    }
    return size;
  }
  
  bool CodeScope::calleeTooBig(SendInfo* info, RScope* rs,
				InlineLimitType limitType) {
    // try to see if the potential inlinee is too big
    int size = calleeSize(rs);
    // NB: continue even if size == 0 to bring current estimated size into play
    assert(limitType >= NormalFnLimit && limitType <= BlockFnLimit,
	   "bad limit");
    limitType = InlineLimitType(limitType + NormalFnInstrLimit-NormalFnLimit);
    int cutoff = theCompiler->inlineLimit[limitType];
    int estimated = theCompiler->estimatedSize();
    int limit = theCompiler->inlineLimit[NmInstrLimit];
    // reject if inlinee too large, but correct for well-known cheap messages
    bool bad = size > cutoff &&
      !(info->rcvr->hasKlass() &&
	isCheapMessage(info->sel, info->rcvr->klass()));
    if (bad && estimated + size < limit / 2) {
      // allow inlining if recompilee itself is small (i.e., a forwarder)
      if (recompilee &&
	  recompilee->instsLen() - oopSize * PrologueSize < cutoff) {
	bad = false;
      }
    }
    // also reject if est. total size too large (except for ifTrue et al)
    // but don't trust the isCheap thing if we're way over the limit
    bad = bad || estimated + size >= limit;
    if (bad &&
	estimated + size < 2 * limit &&
	(size == 0 || size < cutoff / 8) &&
	info->rcvr->hasKlass() &&
	isReallyCheapMessage(info->sel, info->rcvr->klass())) {
      bad = false;
    }
    if (bad && PrintInlining) {
      lprintf("%*s*not inlining %s: callee too big (%d/%d/%d/%d)\n", depth, "",
	      selector_string(info->sel), size, cutoff, estimated, limit);
    }
    if (CompilerDebug && bad && estimated > limit)
      warning("Compiler: (while compiling %s/%s) estimated nmethod size > limit (%ld > %ld)",
	       selector_string(theCompiler->key->L.selector),
	       selector_string(info->sel), estimated, limit);
    return bad;
  }
  
  bool CodeScope::calleeIsSmall(SendInfo* info, RScope* rs,
				 InlineLimitType limitType) {
    // try to see if the potential inlinee is small
    int size = calleeSize(rs);
    if (!size) return false;	// no size info
    assert(limitType >= NormalFnLimit && limitType <= BlockFnLimit,
	   "bad limit");
    limitType = InlineLimitType(limitType + NormalFnInstrLimit-NormalFnLimit);
    int cutoff = theCompiler->inlineLimit[limitType];
    int estimated = theCompiler->estimatedSize();
    int limit = theCompiler->inlineLimit[NmInstrLimit];
    bool ok = size <= cutoff && estimated + size < limit;
    if (ok && PrintInlining) {
      lprintf("%*s*inlining %s anyway: callee is small (%d/%d/%d/%d)\n",
	      depth, "", selector_string(info->sel),
	      size, cutoff, estimated, limit);
    }
    return ok;
  }
  
  bool CodeScope::shouldInlineSend(SendInfo* info, RScope* rs, Expr* rcvr,
				    oop m, InlineLimitType limitType) {
    LookupKey* L = info->L;
    if (!L->selector->is_string()) return false;

    if (isRecursiveCall(m, L->receiverKlass, MaxRecursionUnroll)) {
      info->uninlinable = true;
      return false;
    }
    
    if (limitType == NormalFnLimit) {
      // check args to see if any of them is a block; if so, increase limit
      int top = exprStack->length();
      int argc = symbolOop(L->selector)->arg_count();
      for (int i = argc; i > 0; i--) {
	if (exprStack->nth(top - i)->preg()->isBlockPReg()) {
	  limitType = BlockArgFnLimit;
	  goto done;
	}
      }
      // check receiver
      if (lookupReceiverIsDelta(L->lookupType)) {
	if (self->preg()->isBlockPReg()) limitType = BlockArgFnLimit;
      } else if (exprStack->nth(top - argc - 1)->preg()->isBlockPReg()) {
	limitType = BlockArgFnLimit;
      }
    }
    
   done:
    if (calleeTooBig(info, rs, limitType)) {
      // register this send as uninlinable
      theCompiler->registerUninlinable(info, limitType, 9999);
      return false;
    }

    // NB: this test comes after calleeTooBig to prevent forced inlining of
    // e.g. a really complicated user-defined '+' for matrices
    if (isCheapMessage(symbolOop(L->selector))) {
      msgCost = costP->cheapSendCost;
      return true;
    }

    int cutoff = theCompiler->inlineLimit[limitType];
    msgCost = sicCost((methodKlass*)m->klass(), this, costP);
    if (info->primFailure &&
	info->nsends < MinPrimFailureInvocations) {
      if (rs->isPICScope() && ((RPICScope*)rs)->sd->isOptimized()) {
	// the fail block send is optimized, it's probably executed frequently
      } else if (rs->isInlinedScope()) {
	// was inlined in previous version, so do it again
      } else {
	// don't inline error block unless trivial or taken often
	if (msgCost > MaxTrivialPrimFailureCost) return false;
	// should also look at block method, not default value:With:
	Klass* map = rcvr->klass()->addr();
	if (map->is_block()) {
	  memOop method = ((blockKlass*)map)->value();
	  msgCost = sicCost((methodKlass*)method->klass(), this, failCostP);
	  // bug: should estimate real length of prim failure; e.g. could
	  // have single send (cost 1) but that method sends more and more
	  // msgs...i.e. need concept of "being in fail branch" so that
	  // no further inlining takes place -- fix this
	  if (msgCost > MaxTrivialPrimFailureCost) return false;
	}
      }
    }
    if (msgCost > cutoff) {
      if (calleeIsSmall(info, rs, limitType)) return true;
      theCompiler->registerUninlinable(info, limitType, msgCost);
      return false;
    }
    return true;
  }

  bool CodeScope::shouldInlineBlock(SendInfo* info, RScope* rs,
				     Expr* rcvr, oop method) {
    return shouldInlineSend(info, rs, rcvr, method, BlockFnLimit);
  }

  bool CodeScope::shouldInlineMethod(SendInfo* info, RScope* rs,
				      Expr* rcvr, oop meth) {
    return shouldInlineSend(info, rs, rcvr, meth, NormalFnLimit);
  }
  
  Expr* CodeScope::picPredictUnlikely(SendInfo* info,
					RUntakenScope* uscope) {
    if (theCompiler->useUncommonTraps &&
	info->primFailure && uscope->isUnlikely()) {
      // this send was never executed in the recompilee
      // only make the send unlikely if it had a chance to execute
      // (If the send isn't a prim failure, don't trust the info --
      // it's unlikely that the method just stops executing in the middle.
      // What probably happened is that recompilation happens before the
      // rest of the method got a chance to execute (e.g. recursion), or it
      // always quit via NLR.  In any case, the compiler can't handle this
      // yet - need to treat it specially similar to endsDead.)
      info->nsends = 0;
      const int MinSends = MinPrimFailureInvocations;
      int count = uscope->caller->invocationCount();
      if (count <= 0) {
	// hack: optimized method has no count, so assume it's > MinSends
	count = MinSends + 1;
      }
      bool makeUncommon = count >= MinSends && uscope->sd->wasNeverExecuted();
      if (PrintInlining) {
	lprintf("%*s*%sPIC-type-predicting %s as never executed\n",
		depth, "", makeUncommon ? "" : "NOT ",
		info->sel->copy_null_terminated());
      }
      if (makeUncommon) {
	return new UnknownExpr(info->rcvr->preg(), NULL, true);
      }
    }
    return info->rcvr;
  }

  Expr* CodeScope::picPredict(SendInfo* info) {
    // check PICs for information
    if (!UsePICRecompilation) return info->rcvr;
    bool canBeUnlikely = theCompiler->useUncommonTraps;
    if (rscope->hasSubScopes(_bci)) {
      RScopeBList* l = rscope->subScopes(_bci);
      if (l->first()->isUntakenScope() && l->length() == 1) {
	return picPredictUnlikely(info, (RUntakenScope*)l->first());
      } else if (info->rcvr->containsUnknown()) {
	if (PrintInlining) {
	  lprintf("%*s*PIC-type-predicting %s (%ld maps)\n", depth, "",
		  info->sel->copy_null_terminated(), l->length());
	}
	for (int i = 0; i < l->length(); i++) {
	  RScope* r = l->nth(i);
	  Expr* expr = r->receiverExpr();
	  if (expr->isUnknownExpr()) {
	    // untaken real send (from PIC)
	  } else if (expr->klass()->addr()->is_block()) {
	    // for now, doesn't make sense to predict block maps because of
	    // map cloning
	    if (PrintInlining) {
	      lprintf("%*s*not predicting block map\n", depth, ""); 
	    }
	    canBeUnlikely = false;
	  } else {
	    Expr* alreadyThere = info->rcvr->findKlass(expr->klass());
	    if (alreadyThere) {
	      // generalize to map if only have constant
	      if (alreadyThere->isConstantExpr())
		info->rcvr = info->rcvr->mergeWith(expr, NULL);
	    } else {
	      // add map only if type isn't already present (for splitting)
	      info->predicted = true;
	      info->rcvr = info->rcvr->mergeWith(expr, NULL);
	      if (expr->hasConstant() && l->length() == 1) {
		// check to see if single predicted receiver is true or false;
		// if so, add other boolean to prediction.  Reduces the number
		// of uncommon branches; not doing so appears to be overly
		// aggressive (as observed experimentally)
		oop c = expr->constant();
		if (c == Memory->trueObj &&
		    !info->rcvr->findKlass(as_klassOop(Memory->falseObj->klass()))) {
		  Expr* f = new ConstantExpr(Memory->falseObj, NULL, NULL);
		  info->rcvr = info->rcvr->mergeWith(f, NULL);
		} else if (c == Memory->falseObj &&
			   !info->rcvr->findKlass(as_klassOop(Memory->trueObj->klass()))) {
		  Expr* t = new ConstantExpr(Memory->trueObj, NULL, NULL);
		  info->rcvr = info->rcvr->mergeWith(t, NULL);
		}
	      }
	    }
	  }
	}
      } else {
	// know receiver type precisely
	return info->rcvr;
      }
      // mark unknown branch as unlikely
      UnknownExpr* u = info->rcvr->findUnknown();
      if (u && canBeUnlikely && theCompiler->useUncommonTraps && 
	  rscope->isUncommonAt(_bci, false)) {
	info->rcvr = info->rcvr->makeUnknownUnlikely(this);
      }
    } else if (theCompiler->useUncommonTraps &&
	       info->primFailure &&
	       rscope->isUncommonAt(_bci, true)) {
      // this is the failure send of a primitive, and the failure was made
      // uncommon in the recompilee, and it was never taken, so keep it
      // uncommon
      if (PrintInlining) {
	lprintf("%*s*PIC-type-predicting %s as never executed (2)\n",
		depth, "", info->sel->copy_null_terminated());
      }
      info->rcvr = new UnknownExpr(info->rcvr->preg(), NULL, true);
    }
     
    assert(info->rcvr->preg(), "should have a preg");
    return info->rcvr;
  }

  Expr* CodeScope::typePredict(SendInfo* info) {
    // try static type prediction
    PReg* r = info->rcvr->preg();
    symbolOop sel = info->sel;
    if (sel == VMString[IF_TRUE_] ||
	sel == VMString[IF_FALSE_] ||
	sel == VMString[IF_TRUE_FALSE_] ||
	sel == VMString[IF_FALSE_TRUE_] ||
	sel == VMString[OR]  || sel == VMString[AND] || sel == VMString[NOT]) {
      // boolean message
      if (PrintInlining) {
	lprintf("%*s*type-predicting %s\n", depth, "",
	       sel->copy_null_terminated());
      }
      info->predicted = true;
      bool allowUnlikely = theCompiler->useUncommonTraps;
      if (CompilerDeferUncommonBranches &&
	  (sel == VMString[IF_TRUE_] ||
	   sel == VMString[IF_FALSE_] ||
	   sel == VMString[IF_TRUE_FALSE_] ||
	   sel == VMString[IF_FALSE_TRUE_])) {
	// these bets are really safe - make uncommon even when recompiling
	// due to uncommon trap (if the ifTrue itself caused the trap,
	// rscope->isUncommonAt will be false, so this is safe)
	allowUnlikely = true;
      }
      if (allowUnlikely) {
	if (rscope->isUncommonAt(_bci, false)) {
	  // ok, no uncommon trap here
	} else if (rscope->hasSubScopes(_bci)) {
	  // has real send for ifTrue et al. -- must be NIC-compiled
	  // make uncommon unlikely if no non-true/false receiver present
	  RScopeBList* subs = rscope->subScopes(_bci);
	  for (int i = subs->length() - 1; i >= 0; i--) {
	    RScope* s = subs->nth(i);
	    Expr* rcvr = s->receiverExpr();
	    if (rcvr->hasKlass()) {
	      Klass* m = rcvr->klass()->addr();
	      if (m != Memory->trueObj->klass() &&
		  m != Memory->falseObj->klass()) {
		allowUnlikely = false;
		break;
	      }
	    }
	  }
	  if (WizardMode && !allowUnlikely)
	    warning("Compiler: non-bool receiver for ifTrue: et al. detected");
	}
	if (allowUnlikely) {
	  // make unknown case unlikely despite 
	  info->rcvr = info->rcvr->makeUnknownUnlikely(this);
	}
      }
      Expr* rcvr = info->rcvr;
      Expr* t = new ConstantExpr(Memory->trueObj , r, NULL);
      Expr* f = new ConstantExpr(Memory->falseObj, r, NULL);
      // make sure we don't destroy splitting info; only add types if not
      // already present
      if (rcvr->findKlass(as_klassOop(Memory->trueObj->klass())) == NULL)
d135 1
a135 1
      if (rcvr->findKlass(as_klassOop(Memory->falseObj->klass())) == NULL)
d141 7
# endif
