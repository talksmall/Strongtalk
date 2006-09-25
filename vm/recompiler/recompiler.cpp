/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.39 $ */
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
#ifdef DELTA_COMPILER

#include "incls/_recompiler.cpp.incl"

RecompilationPolicy::RecompilationPolicy(RFrame* first) {
  stack = new GrowableArray<RFrame*>(50);
  stack->push(first);
}


Recompilee* RecompilationPolicy::findRecompilee() {
  RFrame* rf = stack->at(0);
  if (PrintRecompilation2) {
    for (int i = 0; i < 10 && rf; i++, rf = senderOf(rf)) ;   // create 10 frames
    printStack();
  }
  RFrame* r = findTopInlinableFrame();
  if (r) {
    if (PrintRecompilation) r->print();
    return Recompilee::new_Recompilee(r);
  } else {
    return NULL;
  }
}

void RecompilationPolicy::cleanupStaleInlineCaches() {
  int len = min(20, stack->length());
  for (int i = 0; i < len; i++) stack->at(i)->cleanupStaleInlineCaches();
}

RFrame* RecompilationPolicy::findTopInlinableFrame() {
  // go up the stack until finding a frame that (probably) won't be inlined into its caller
  RecompilerInliningPolicy p;
  RFrame* current = stack->at(0);	// current choice for stopping
  RFrame* prev = NULL;			// prev. value of current
  RFrame* prevMethod = NULL;		// same as prev, except always holds method frames (not blocks)
  msg = NULL; 
  
  while (1) {
    if (current == NULL) {
      // reached end of stack without finding a candidate
      current = prev; 
      break;
    }

    if ((msg = p.shouldInline(current)) != NULL) {
      // current won't be inlined into caller, so stop here
      break;
    }

    // before going up the stack further, check if doing so would get us into compiled code
    RFrame* next = senderOrParentOf(current);

    if (next) {
      if (next->num() > MaxRecompilationSearchLength) {
	// don't go up too high when searching for recompilees
	msg = "(don't go up any further: next > MaxRecompilationSearchLength)";
	break;
      }
      if (next->distance() > MaxInterpretedSearchLength) {
	// don't go up too high when searching for recompilees
	msg = "(don't go up any further: next > MaxInterpretedSearchLength)";
	break;
      }
      if (current->is_interpreted()) {
	// Before recompiling any compiled code (next or its callers), first compile the interpreted
	// method; later, if that gets executed often, it will trigger another recompile that 
	// will lead to next (or its caller) to be reoptimized.  At that point, optimization can take
	// advantage of the better type information in the compiled version of current
	LookupKey* k = next->key();
	if (next->is_compiled()) {
	  msg = "(not going up into optimized code)";
	  break;
	} else if (k != NULL && Universe::code->lookup(k) != NULL) {
	  msg = "(already compiled this method)";
	  break;
	} 
      }
      if (next->is_compiled() && (msg = shouldNotRecompileNMethod(next->nm())) != NULL) {
	msg = "nmethod should not be recompiled; don't go up";
	break;
      }
      if (next->is_compiled() && next->sends() < MinSendsBeforeRecompile) {
	msg = "don't recompile -- hasn't sent MinSendsBeforeRecompile messages yet";
	if (PrintRecompilation && msg) current->print();
	break;
      }
    }
    prev = current;
    
    if (!current->is_blockMethod()) prevMethod = current;
    current = next;
  }

  if (current) checkCurrent(current, prev, prevMethod);

  if (PrintRecompilation && msg) lprintf("%s\n", msg);

  return current;
}

void RecompilationPolicy::checkCurrent(RFrame*& current, RFrame*& prev, RFrame*& prevMethod) {
  // current is the provisional recompilation candidate; perform a few sanity checks on it
  if (current->is_blockMethod() && current->is_interpreted()) {
    // can't recompile blocks in isolation, and this block is too big to inline
    // thus, optimize method called by block
    if (PrintRecompilation && msg) lprintf("%s\n", msg);
    fixBlockParent(current);
    if (prev && !prev->is_blockMethod()) {
      current = prev; prev = prevMethod = NULL;
      if (current) checkCurrent(current, prev, prevMethod);
      msg = "(can't recompile block in isolation)";
    } else {
      current = NULL;
      msg = "(can't recompile block in isolation, and no callee method found)";
    }
  } else if (current->is_super()) {
    current = prev; prev = prevMethod = NULL;
    if (current) checkCurrent(current, prev, prevMethod);
    msg = "(can't recompile super nmethods yet)";
  } else if (current->is_compiled()) {
    char* msg2;
    if ((msg2 = shouldNotRecompileNMethod(current->nm())) != NULL) {
      current = NULL;
      msg = msg2;
    } else {
      // should recompile nmethod
      msg = NULL;
    }
  }
  if (current == NULL) return;	    

  // check if we already recompiled this method (but old one is still on the stack)
  // Inserted after several hours of debugging by Lars
  if (current->is_compiled()) {
    if (EnableOptimizedCodeRecompilation) {
      LookupKey* k          = current->key();
      nmethod*   running_nm = current->nm();
      nmethod*   newest_nm  = k ? Universe::code->lookup(k) : NULL;
      // NB: newest_nm is always NULL for block nmethods
      if (k && running_nm && running_nm->is_method() && running_nm != newest_nm) {
        // ideally, should determine how many stack frames nm covers, then recompile
        // the next frame
        // for now, try previous method frame
        current = prevMethod; prev = prevMethod = NULL;
        if (current) checkCurrent(current, prev, prevMethod);
      }
    } else {
      current = NULL;
    }
  }
}

char* RecompilationPolicy::shouldNotRecompileNMethod(nmethod* nm) {
  // if nm should not be recompiled, return a string with the reason; otherwise, return NULL
  if (nm->isUncommonRecompiled()) {
    if (RecompilationPolicy::shouldRecompileUncommonNMethod(nm)) {
      nm->makeOld();
      return NULL;	  // ok
    } else {
      return "uncommon nmethod too young";
    }
  } else if (nm->version() >= MaxVersions) {
    return "max. version reached";
  } else if (nm->level() == MaxRecompilationLevels - 1) {
    return "maximally optimized";
  } else if (nm->isYoung()) {
    return "nmethod too young";
  }
  return NULL;
}

RFrame* RecompilationPolicy::senderOf(RFrame* rf) {
  RFrame* sender = rf->caller();
  if (sender && sender->num() == stack->length()) stack->push(sender);
  return sender;
}

void RecompilationPolicy::fixBlockParent(RFrame* rf) {
  // find the parent method and increase its counter so it will be recompiled next time
  methodOop blk = rf->top_method();
  assert(blk->is_blockMethod(), "must be a block");
  methodOop home = blk->home();
  int count = home->invocation_count();
  count += Interpreter::get_invocation_counter_limit();
  count = min(count, methodOopDesc::_invocation_count_max - 1);
  home->set_invocation_count(count);
  assert(home->invocation_count() >= Interpreter::get_invocation_counter_limit(),
         "counter increment didn't work");
}

RFrame* RecompilationPolicy::senderOrParentOf(RFrame* rf) {
  // "go up" on the stack to find a better recompilee;
  // for normal methods, that's the sender; for block methods, it's the home method
  // (*not* enclosing scope) unless that method is already optimized
  if (rf->is_blockMethod()) {
    RFrame* parent = parentOf(rf);
    if (parent == NULL) {
      // can't find the parent (e.g. because it's a non-LIFO block)
      fixBlockParent(rf);
      return senderOf(rf);	// is this a good idea???
      // It may be; this way, a parent-less block doesn't prevent its callers from being
      // optimized; on the other hand, it may lead to too much compilation since what we
      // really want to do is recompile the parent.  On the third hand, if a block is non-lifo
      // = (has no parent) it may not make sense to optimize its enclosing method anyway.
      // But on the fourth hand, to optimize any block its enclosing method must be optimized.
    } else {
      if (parent->is_compiled() == rf->is_compiled()) {
	// try to optimize the parent
        if (CountParentLinksAsOne) parent->set_distance(rf->distance() + 1);
	return parent;
      } else {
        return senderOf(rf);	// try sender
      }
    }
  } else if (rf->hasBlockArgs()) {
    // go up to highest home of any block arg
    // bug: should check how often block is created / invoked
    GrowableArray<blockClosureOop>* blockArgs = rf->blockArgs();
    RFrame* max = NULL;
    for (int i = 0; i < blockArgs->length(); i++) {
      blockClosureOop blk = blockArgs->at(i);
      //jumpTableEntry* e = blk->jump_table_entry();
      RFrame* home = parentOfBlock(blk);
      if (home == NULL) continue;
      if (max == NULL || home->num() > max->num()) max = home;
    }
    if (max) return max;
  }
  // default: return sender
  return senderOf(rf);
}

RFrame* RecompilationPolicy::parentOf(RFrame* rf) {
  assert(rf->is_blockMethod(), "shouldn't call");
  // use caller vframe's receiver instead of rf's receiver because vframe may not be set up correctly
  // for most recent frame [this may not be true anymore -- Urs 10/96]
  // caller vframe must have block as the receiver because it invokes primitiveValue

  // Urs - please put in more comments here (gri):
  // b) why do you look at the receiver of the sender instead of the receiver of the send
  //    (it only happens right now that they are the same, but what if we change what we
  //    can use as first argument for primitives (primitiveValue) ?). Seems to be a lot of
  //    implicit assumptions & no checks.
  // Yes this assumes that the caller invokes primitiveValue on self; if we change that the
  // code here breaks.

  deltaVFrame* sender = rf->top_vframe()->sender_delta_frame();  
  if (sender == NULL) return NULL;

  oop blk = sender->receiver();
  guarantee(blk->is_block(), "should be a block");
  return parentOfBlock(blockClosureOop(blk));
}

RFrame* RecompilationPolicy::parentOfBlock(blockClosureOop blk) {
  if (blk->is_pure()) return NULL;
  
  contextOop ctx = blk->lexical_scope();
  assert(ctx->is_context(), "make sure we have a context");

  int* fp = ctx->parent_fp();
  if (fp == NULL) {
    return NULL;	// non-LIFO block
  } 
  // try to find context's RFrame
  RFrame* parent = stack->first();
  for (int i = 0; i < MaxRecompilationSearchLength; i++ ) {
    parent = senderOf(parent);
    if (!parent) break;
    frame fr = parent->fr();
    if (fr.fp() == fp) return parent;	// found it!
  }
  return NULL;	  // parent not found
}


void RecompilationPolicy::printStack() {	// for debugging
  for (int i = 0; i < stack->length(); i++ ) stack->at(i)->print();
}

bool RecompilationPolicy::needRecompileCounter(Compiler* c) {
  if (!UseRecompilation) return false;
  if (c->version() == MaxVersions) return false;    // to prevent endless recompilation
  // also stop counting for "perfect" nmethods where nothing more can be optimized
  // NB: it is tempting to leave counters in very small methods (so that e.g. accessor functions 
  // still trigger counters), but that won't work if they're invoked from megamorphic
  // call sites --> put the counters in the caller, not the callee.
  return c->level() < MaxRecompilationLevels - 1;
}

bool RecompilationPolicy::shouldRecompileAfterUncommonTrap(nmethod* nm) {
  // called after nm encountered an uncommon trap; should it be recompiled into
  // less optimized form (without uncommon branches)?
  return nm->uncommon_trap_counter() >= UncommonRecompileLimit;
}

bool RecompilationPolicy::shouldRecompileUncommonNMethod(nmethod* nm) {
  assert(nm->isUncommonRecompiled(), "expected an uncommon nmethod");
  // nm looks like a recompilation candidate; can it really be optimized?
  // isUncommonRecompiled nmethods were created after the original nmethod encountered
  // too many "uncommon" cases.  Thus, the assumptions the compiler originally took
  // proved to be too aggressive.  In the uncommon-recompiled nmethod, there are
  // no uncommon branches, so it's slower.  But we don't want to reoptimize it too eagerly
  // because it needs to run long enough to accumulate type information that's truly
  // representative of its usage.  This method determines how to make that tradeoff.
  // The main idea is to back off exponentially each time we go through the cycle
  // of optimize -- uncommon recompile -- reoptimize.
  const int v = nm->version();
  const int c = nm->invocation_count();
  return  c >= uncommonNMethodInvocationLimit(v) ||
         (c >= UncommonInvocationLimit && nm->age() > uncommonNMethodAgeLimit(v));
}

int RecompilationPolicy::uncommonNMethodInvocationLimit(int version) {
  int n = UncommonInvocationLimit;
  for (int i = 0; i < version; i++) n *= UncommonAgeBackoffFactor;
  return n;
}

int RecompilationPolicy::uncommonNMethodAgeLimit(int version) {
  int n = NMethodAgeLimit;
  for (int i = 0; i < version; i++) n *= UncommonAgeBackoffFactor;
  return n;
}

#endif
