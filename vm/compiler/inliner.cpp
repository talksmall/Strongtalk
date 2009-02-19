/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.81 $ */
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

# include "incls/_inliner.cpp.incl"

// ----------- inlining policy ---------------

#ifdef later
static const int DefaultCompilerMaxSplitCost        =   50;
static const int DefaultCompilerMaxBlockInstrSize   =  400;
static const int DefaultCompilerMaxFnInstrSize      =  250;
static const int DefaultCompilerMaxBlockFnInstrSize =  600;
static const int DefaultCompilerMaxNmethodInstrSize = 5000;
#endif

static CompilerInliningPolicy inliningPolicy;

bool InliningPolicy::shouldNotInline() const {
  if (method->method_inlining_info() == methodOopDesc::never_inline) return true;
  const symbolOop sel = method->selector();
  return (sel == vmSymbols::error() ||
    sel == vmSymbols::error_() ||
    sel == vmSymbols::subclassResponsibility());
}

bool InliningPolicy::isCriticalSmiSelector(const symbolOop sel) {
  // true if performance-critical smi method in standard library
  // could also handle these by putting a bit in the methodOops
  return  sel == vmSymbols::plus() ||
    sel == vmSymbols::minus() ||
    sel == vmSymbols::multiply() ||
    sel == vmSymbols::divide() ||
    sel == vmSymbols::mod() ||
    sel == vmSymbols::equal() ||
    sel == vmSymbols::not_equal() ||
    sel == vmSymbols::less_than() ||
    sel == vmSymbols::less_than() ||
    sel == vmSymbols::less_than_or_equal() ||
    sel == vmSymbols::greater_than() ||
    sel == vmSymbols::greater_than_or_equal() ||
    sel == vmSymbols::double_equal() ||
    sel == vmSymbols::bitAnd_() ||
    sel == vmSymbols::bitOr_() ||
    sel == vmSymbols::bitXor_() ||
    sel == vmSymbols::bitShift_() ||
    sel == vmSymbols::bitInvert();
}

bool InliningPolicy::isCriticalArraySelector(const symbolOop sel) {
  return  sel == vmSymbols::at() ||
    sel == vmSymbols::at_put() ||
    sel == vmSymbols::size();
}

bool InliningPolicy::isCriticalBoolSelector(const symbolOop sel) {
  return  sel == vmSymbols::and_() ||
    sel == vmSymbols::or_() ||
    sel == vmSymbols::_and() ||
    sel == vmSymbols::_or() ||
    sel == vmSymbols::and1() ||
    sel == vmSymbols::or1() ||
    sel == vmSymbols::_and() ||
    sel == vmSymbols::_not() ||
    sel == vmSymbols::xor_() ||
    sel == vmSymbols::eqv_();
}

bool InliningPolicy::isPredictedSmiSelector(const symbolOop sel) {
  return sel != vmSymbols::equal()     &&
    sel != vmSymbols::not_equal() &&
    isCriticalSmiSelector(sel);
}

bool InliningPolicy::isPredictedArraySelector(const symbolOop sel) {
  return isCriticalArraySelector(sel);
}
bool InliningPolicy::isPredictedBoolSelector(const symbolOop sel) {
  return isCriticalBoolSelector(sel);
}

bool InliningPolicy::isInterpreterPredictedSmiSelector(const symbolOop sel) {
  // true if performance-critical smi method in standard library
  // could also handle these by putting a bit in the methodOops
  return  sel == vmSymbols::plus() ||
    sel == vmSymbols::minus() ||
    sel == vmSymbols::multiply() ||
    sel == vmSymbols::divide() ||
    sel == vmSymbols::mod() ||
    sel == vmSymbols::equal() ||
    sel == vmSymbols::not_equal() ||
    sel == vmSymbols::less_than() ||
    sel == vmSymbols::less_than() ||
    sel == vmSymbols::less_than_or_equal() ||
    sel == vmSymbols::greater_than() ||
    sel == vmSymbols::greater_than_or_equal();
}

bool InliningPolicy::isInterpreterPredictedArraySelector(const symbolOop sel) {
  return false;
}

bool InliningPolicy::isInterpreterPredictedBoolSelector(const symbolOop sel) {
  return false;
}



bool InliningPolicy::isBuiltinMethod() const {
  // true if performance-critical method in standard library
  // could also handle these by putting a bit in the methodOops
  if (method->method_inlining_info() == methodOopDesc::always_inline) return true;
  const symbolOop sel = method->selector();
  const klassOop klass = receiverKlass();
  const bool isNum = klass == Universe::smiKlassObj() || klass == Universe::doubleKlassObj();
  if  (isNum && isCriticalSmiSelector(sel)) return true;

  const bool isArr = klass == Universe::objArrayKlassObj()  ||
    klass == Universe::byteArrayKlassObj() ||
    klass == Universe::symbolKlassObj()    ||
    false;	// probably should add doubleByteArray et al
  if (isArr && isCriticalArraySelector(sel)) return true;

  const bool isBool = klass == Universe::trueObj()->klass() || klass == Universe::falseObj()->klass();
  if (isBool && isCriticalBoolSelector(sel)) return true;
  return false;
}

char* CompilerInliningPolicy::shouldInline(InlinedScope* s, InlinedScope* callee) {
  if (callee == NULL) {
    return "cannot handle super sends";
  }

  if (callee->rscope->isDatabaseScope()) {
    // This scope is provided by the inlining database and should
    // always be inlined.
    return NULL;
  }

  if (s->rscope->isDatabaseScope()) {
    // The caller scope is provided by the inlining database but the callee scope is not.
    // ignore the callee when inlining.
    return "do not inline (Inlining Database)";
  }

  // should check for existing compiled version here -- fix this
  sender = s;
  this->method = callee->method();
  this->rcvr   = callee->self();
  if (NodeFactory::cumulCost > MaxNmInstrSize) {
    theCompiler->reporter->report_toobig(callee);
    return "method getting too big";
  }
  if (shouldNotInline()) return "should not inline (special)";
  // performance bug: should check how many recursive calls the method has -- unrolling factorial
  // to depth N gives N copies, but unrolling fibonacci gives 2**N
  // also, should look at call chain to estimate how big inlined recursion will get
  if (sender->isRecursiveCall(method, callee->selfKlass(), MaxRecursionUnroll)) return "recursive";
  return basic_shouldInline(method);
}

char* InliningPolicy::basic_shouldInline(methodOop method) {
  // should the interpreted method be inlined?
  if (method->method_inlining_info() == methodOopDesc::always_inline) return NULL;
  calleeCost = method->estimated_inline_cost(receiverKlass());

  if (method->is_blockMethod()) {
    // even large blocks should be inlined if they make up most of their home's code
    int parentCost = method->parent()->estimated_inline_cost(receiverKlass());
    assert(parentCost > calleeCost, "must be higher");
    if (float(parentCost - calleeCost) / parentCost * 100.0 < MinBlockCostFraction) return NULL;
  }

  // compute the cost limit based on the provided arguments
  int cost_limit = method->is_blockMethod() ? MaxBlockInlineCost : MaxFnInlineCost;
  for (int i = method->number_of_arguments() - 1; i >= 0; i--) {
    klassOop k = nthArgKlass(i);
    if (k && k->klass_part()->oop_is_block()) cost_limit += BlockArgAdditionalAllowedInlineCost;
  }
  if (calleeCost < cost_limit) return NULL;
  if (isBuiltinMethod()) return NULL;
  return "too big";
}

klassOop CompilerInliningPolicy::nthArgKlass(int i) const {
  int first = sender->exprStack()->length() - method->number_of_arguments();
  Expr* e = sender->exprStack()->at(first + i);
  return e->hasKlass() ? e->klass() : NULL;
}

klassOop CompilerInliningPolicy::receiverKlass() const 		{ return rcvr->klass(); }

klassOop RecompilerInliningPolicy::nthArgKlass(int i) const 	{ return _vf ? _vf->argument_at(i)->klass() : NULL; }
klassOop RecompilerInliningPolicy::receiverKlass() const {
  return _vf ? theRecompilation->receiverOf(_vf)->klass() : NULL;
}

char* RecompilerInliningPolicy::shouldInline(RFrame* rf) {
  // determine if rf's method or nmethod should be inlined into its caller
  // use compiled-code size if available, even for interpreted methods
  // (gives better info on how big method will become since it includes inlined methods)
  // return NULL if ok, reason for not inlining otherwise (for performance debugging)

  // for now, always inline super sends
  extern bool SuperSendsAreAlwaysInlined;
  assert(SuperSendsAreAlwaysInlined, "fix this");
  if (rf->is_super()) return NULL;

  _vf = rf->top_vframe();
  this->method = rf->top_method();
  nmethod* nm = NULL;
  if (rf->is_interpreted()) {
    // check to see if we have a compiled version of the method
    LookupKey* key = rf->key();
    if (key) {
      nm = Universe::code->lookup(key);
    } else {
      // interpreted block; should check for compiled block
      // fix this later
    }
  } else {
    assert(rf->is_compiled(), "oops");
    nm = ((CompiledRFrame*)rf)->nm();
  }
  if (nm) {
    return shouldInline(nm);
  } else {
    return basic_shouldInline(method);
  }
}

char* RecompilerInliningPolicy::shouldInline(nmethod* nm) {
  if (!CodeSizeImpactsInlining) return NULL;
  if (method->method_inlining_info() == methodOopDesc::always_inline) return NULL;
  // compute the allowable cost based on the method type and the provided arguments
  int cost_limit = method->is_blockMethod() ? MaxBlockInstrSize : MaxFnInstrSize;
  int i = method->number_of_arguments();
  while (i-- > 0) {
    klassOop k = nthArgKlass(i);
    if (k && k->klass_part()->oop_is_block()) cost_limit += BlockArgAdditionalInstrSize;
  }
  if (nm->size() < cost_limit) return NULL;	// ok
  if (isBuiltinMethod()) return NULL;		// ok, special case (?)
  return "too big (compiled)";
}

// --------------- Inliner ------------------

void Inliner::initialize() {
  _info = NULL;
  res = NULL;
  callee = NULL;
  gen = NULL;
  merge = NULL;
  resultPR = NULL;
  depth = 0;
  _msg = NULL;
  lastLookupFailed = false;
}

void Inliner::initialize(SendInfo* info, SendKind kind) {
  initialize();
  _info = info;
  this->kind = kind;
  sender = _info->senderScope;
  gen = sender->gen();
  _info->resReg = resultPR = new SAPReg(sender);
  depth = sender->depth;
}

Expr* Inliner::inlineNormalSend(SendInfo* info) {
  initialize(info, NormalSend);
  return inlineSend();
}

Expr* Inliner::inlineSuperSend(SendInfo* info) {
  initialize(info, SuperSend);
  return inlineSend();
}

Expr* Inliner::inlineSelfSend(SendInfo* info) {
  initialize(info, SelfSend);
  return inlineSend();
}

Expr* Inliner::inlineSend() {
  if (!Inline) {
    // don't do any inlining
    _info->needRealSend = true;
  } else if (gen->is_in_dead_code()) {
    // don't waste time inlining dead code
    res = new NoResultExpr;
    gen->abort();   // the rest of this method is dead code, too
    if (CompilerDebug) cout(PrintInlining)->print("%*s*skipping %s (dead code)\n", depth, "", _info->sel->as_string());
  } else {
    tryInlineSend();
  }

  // generate a real send if necessary
  if (_info->needRealSend) {
    Expr* r = genRealSend();
    res = res ? res->mergeWith(r, merge) : r;
    assert(res, "must have result");
  }
  // merge end of inlined version with end of noninlined version
  if (merge && res && !res->isNoResultExpr()) gen->branch(merge);

  // update caller's current node
  if (gen != sender->gen()) sender->gen()->setCurrent(gen->current());

  // ...and return result (sender is responsible for popping expr stack)
  if (!res) res = new NoResultExpr;
  return res;
}

Expr* Inliner::genRealSend() {
  const int nofArgs = _info->sel->number_of_arguments();
  bool uninlinable = theCompiler->registerUninlinable(this);
  if (CompilerDebug) {
    cout(PrintInlining)->print("%*s*sending %s %s%s\n", depth, "", _info->sel->as_string(),
      uninlinable ? "(unlinlinable) " : "",
      _info->counting ? "(counting) " : "");
  }
  switch(kind) {
    case NormalSend: 	gen->gen_normal_send(_info, nofArgs, resultPR); break;
    case SelfSend: 	gen->gen_self_send  (_info, nofArgs, resultPR); break;
    case SuperSend: 	gen->gen_super_send (_info, nofArgs, resultPR); break;
    default:		fatal1("illegal SendKind %d", kind);
  }
  return new UnknownExpr(resultPR, gen->current());
}

void Inliner::tryInlineSend() {
  const symbolOop sel = _info->sel;

  UnknownExpr* u = _info->rcvr->findUnknown();
  bool usingInliningDB = sender->rscope->isDatabaseScope();
  // first, use type feedback
  if (TypeFeedback && u) {
    assert(kind != SuperSend, "shouldn't PIC-predict super sends");
    // note: when compiling using the inlining database, picPredict won't actually look at any PICs,
    // just at the inlining DB
    _info->rcvr = picPredict();
  }

  // now try static type prediction (but only if type isn't good enough yet)
  if (TypePredict && !usingInliningDB && u && !u->isUnlikely() && !_info->uninlinable) {
    _info->rcvr = typePredict();
  }

  if (_info->rcvr->really_hasKlass(sender)) {
    // single klass - try to inline this send
    callee = tryLookup(_info->rcvr);
    if (callee) {
      Expr* r = doInline(sender->current());
      res = makeResult(r);
    } else {
      // must distinguish between lookup failures and rejected successes
      if (lastLookupFailed) {
        // receiver type is constant (but e.g. method was too big to inline)
        _info->receiverStatic = true;
      }
      _info->needRealSend = true;
    }
  } else if (_info->rcvr->isMergeExpr()) {
    res = inlineMerge(_info);		// inline some cases
    if (res) {
      // inlined some cases; inlineMerge decided whether needRealSend should be set or not
      if (!theCompiler->is_uncommon_compile()) {
        _info->uninlinable = true;	  // remaining sends are here by choice
        _info->counting = false;
      }
    } else {
      // didn't inline anything -- just generate a non-inlined send
      _info->needRealSend = true;
    }
  } else {
    // unknown receiver
    // NB: *must* use uncommon branch if marked unlikely because
    // future type tests won't test for unknown
    if (CompilerDebug) cout(PrintInlining)->print("%*s*cannot inline %s (unknown receiver)\n", depth, "", sel->as_string());
    if (_info->rcvr->findUnknown()->isUnlikely()) {
      // generate an uncommon branch for the unknown case, not a send
      gen->append_exit(NodeFactory::new_UncommonNode(sender->gen()->copyCurrentExprStack(), sender->bci()));
      _info->needRealSend = false;
      if (CompilerDebug) cout(PrintInlining)->print("%*s*making %s uncommon\n", depth, "", sel->as_string());
      res = new NoResultExpr();
      // rest of method's code is unreachable
      assert(gen->current() == NULL, "expected no current node");
      gen->abort();
    } else {
      _info->needRealSend = true;
    }
  }
}


Expr* Inliner::inlineMerge(SendInfo* info) {
  // try to inline the send by type-casing
  merge = NodeFactory::new_MergeNode(sender->bci());		// where all cases merge again
  Expr* res = NULL;							// final (merged) result
  assert(info->rcvr->isMergeExpr(), "must be a merge");
  MergeExpr* r = (MergeExpr*)info->rcvr;				// receiver type
  symbolOop sel = info->sel;

  int nexprs = r->exprs->length();
  int ncases = nexprs - (r->containsUnknown() ? 1 : 0);
  if (ncases > MaxTypeCaseSize) {
    info->needRealSend = true;
    info->uninlinable = true;
    info->counting = false;
    if (CompilerDebug) cout(PrintInlining)->print("%*s*not type-casing %s (%ld > MaxTypeCaseSize)\n",
      depth, "", sel->as_string(), ncases);
    return res;
  }

  // build list of cases to inline
  // (add only immediate klasses (currently only smis) at first, collect others in ...2 lists
  GrowableArray<InlinedScope*>* scopes  = new GrowableArray<InlinedScope*>(nexprs);
  GrowableArray<InlinedScope*>* scopes2 = new GrowableArray<InlinedScope*>(nexprs);
  GrowableArray<Expr*>* 	exprs  	= new GrowableArray<Expr*>(nexprs);
  GrowableArray<Expr*>* 	exprs2 	= new GrowableArray<Expr*>(nexprs);
  GrowableArray<Expr*>* 	others 	= new GrowableArray<Expr*>(nexprs);
  GrowableArray<klassOop>* 	klasses = new GrowableArray<klassOop>(nexprs);
  GrowableArray<klassOop>* 	klasses2= new GrowableArray<klassOop>(nexprs);
  const bool containsUnknown = r->containsUnknown();

  for (int i = 0; i < nexprs; i++) {
    Expr* nth = r->exprs->at(i)->shallowCopy(r->preg(), NULL);
    assert(!nth->isConstantExpr() || nth->next == NULL ||
      nth->constant() == nth->next->constant(),
      "shouldn't happen: merged consts - convert to klass");
    // NB: be sure to generalize constants to klasses before inlining, so that values
    // from an unknown source are dispatched to the optimized code
    // also, right now the TypeTestNode only tests for klasses, not constants
    if (containsUnknown && nth->isConstantExpr()) {
      nth = nth->convertToKlass(nth->preg(), nth->node());
    }

    InlinedScope* s;
    if (nth->hasKlass() &&
      (s = tryLookup(nth)) != NULL) {
        // can inline this case
        klassOop klass = nth->klass();
        if (klass == smiKlassObj) {
          scopes  ->append(s);	    // smis go first
          exprs   ->append(nth);
          klasses ->append(klass);
        } else {
          scopes2 ->append(s);	    // append later
          exprs2  ->append(nth);
          klasses2->append(klass);
        }
    } else {
      if (lastLookupFailed) {
        // ignore this case -- most probably it will never happen
        // (typical case: the class is returned by something like
        // self error: 'should not happen'. ^ self)
        if (others->isEmpty()) {
          others->append(new UnknownExpr(nth->preg(), NULL, theCompiler->useUncommonTraps));
        }
      } else {
        others->append(nth);
      }
      info->needRealSend = true;
    }
  }

  // combine all lists into one (with immediate case first)
  klasses->appendAll(klasses2);
  exprs  ->appendAll(exprs2);
  scopes ->appendAll(scopes2);

  // decide whether to use uncommon branch for unknown case (if any)
  // NB: *must* use uncommon branch if marked unlikely because
  // future type tests won't test for unknown
  bool useUncommonBranchForUnknown = false;
  if (others->length() == 1 && others->first()->isUnknownExpr() &&
    ((UnknownExpr*)others->first())->isUnlikely()) {
      // generate an uncommon branch for the unknown case, not a send
      useUncommonBranchForUnknown = true;
      if (CompilerDebug) cout(PrintInlining)->print("%*s*making %s uncommon (2)\n", depth,"",sel->as_string());
  }

  // now do the type test and inline the individual cases
  Node* typeCase    = NULL;
  Node* fallThrough = NULL;
  if (scopes->length() > 0) {
    //memoizeBlocks(sel);

    if (CompilerDebug) {
      char* s = NEW_RESOURCE_ARRAY(char, 200);
      sprintf(s, "begin type-case of %s (ends at node N%ld)",
        sel->copy_null_terminated(), merge->id());
      gen->comment(s);
    }
    if (CompilerDebug) cout(PrintInlining)->print("%*s*type-casing %s (%d cases)\n", depth, "", sel->as_string(), scopes->length());

    typeCase = NodeFactory::new_TypeTestNode(r->preg(), klasses, info->needRealSend || containsUnknown);
    gen->append(typeCase);
    fallThrough = typeCase->append(NodeFactory::new_NopNode());	// non-predicted case
    for (i = 0; i < scopes->length(); i++) {
      // inline one case
      Inliner* inliner = new Inliner(sender);
      inliner->initialize(new SendInfo(*info), kind);
      inliner->callee = scopes->at(i);			// scope to inline
      inliner->gen = callee->gen();			// node builder to use
      inliner->gen->setCurrent(typeCase->append(i + 1, NodeFactory::new_NopNode()));
      Expr* rcvr = exprs->at(i);
      inliner->info()->rcvr = rcvr;
      assert(r->scope()->isSenderOf(inliner->callee), "r must be from caller scope");
      Expr* e = inliner->doInline(inliner->gen->current());
      if (e->isNoResultExpr()) {
        if (!res) res = e;	// must return non-NULL result (otherwise sender thinks no inlining happened)
      } else {
        assert(e->preg()->scope()->isSenderOf(inliner->callee),
          "result register must be from caller scope");
        gen->append(NodeFactory::new_NopNode());
        e = e->shallowCopy(info->resReg, gen->current());
        res = res ? res->mergeWith(e, merge) : e;
      }
      gen->branch(merge);
    }
    assert(gen == sender->gen(), "oops");
    gen->setCurrent(fallThrough);
  } else {
    // no case was deemed inlinable
    if (!info->counting && !theCompiler->is_uncommon_compile()) info->uninlinable = true;
    useUncommonBranchForUnknown = false;
  }

  if (res && res->isMergeExpr()) res->setNode(merge, info->resReg);

  assert( info->needRealSend &&  others->length() ||
    !info->needRealSend && !others->length(), "inconsistent");

  if (useUncommonBranchForUnknown) {
    // generate an uncommon branch for the unknown case, not a send
    // use an uncommon send rather than an uncommon node to capture
    // the argument usage in case none of the type tests uses the arguments.
    // - was a bug slr 12/02/2009.
    gen->append_exit(NodeFactory::new_UncommonSendNode(gen->copyCurrentExprStack(), 
                                                       sender->bci(),
                                                       info->sel->number_of_arguments()));
    info->needRealSend = false;
  } else if (others->isEmpty()) {
    // typecase cannot fail
    gen->append_exit(NodeFactory::new_FixedCodeNode(FixedCodeNode::dead_end));
  }

  return res;
}


Expr* Inliner::makeResult(Expr* r) {
  Expr* res;
  // massage the result expression so it fulfills all constraints
  if (r->isNoResultExpr()) {
    res = r;
  } else {
    // result's scope must be in sender, not in callee (for correct reg. alloc.)
    if (r->scope() == sender) {
      res = r;
    } else {
      assert(r->scope() == callee, "what scope?");
      //fatal("Urs thinks this shouldn't happen");
      ShouldNotReachHere(); // added instead of the fatal (gri 11/27/01)
      // bind it to new NopNode to fix scope
      Node* n = NodeFactory::new_NopNode();
      assert(n->scope() == sender, "wrong scope");
      gen->append(n);
      res = r->shallowCopy(r->preg(), n);
    }
  }
  return res;
}

Expr* Inliner::doInline(Node* start) {
  // callee scope should be inlined; do it
  // HACK -- fix
#define calleeSize(n) 0

  if (CompilerDebug) {
    cout(PrintInlining)->print("%*s*inlining %s, cost %ld/size %ld (%#lx)%s\n", depth, "",
      callee->selector()->as_string(), inliningPolicy.calleeCost, calleeSize(callee->rscope),
      PrintHexAddresses ? callee : 0, callee->rscope->isNullScope() ? "" : "*");
    if (PrintInlining) callee->method()->pretty_print();
  }

  // Save dependency information in the scopeDesc recorder.
  theCompiler->rec->add_dependant(callee->key());

  Node* next = start->next();
  assert(!next, "can't insert code (?)");

  // set callee's starting point for code generation
  gen = callee->gen();
  gen->setCurrent(start);
  callee->genCode();

  // make sure caller appends new code after inlined return of callee
  gen = sender->gen();
  gen->setCurrent(callee->returnPoint());
  assert(gen->current()->next() == NULL, "shouldn't have successor");
  return callee->result;
}

Expr* Inliner::picPredict() {
  // check PICs for information
  const int bci = sender->bci();
  RScope* rscope = sender->rscope;

  if (!rscope->hasSubScopes(bci)) {
    // note: not fully implemented yet -- if no subScopes, should check to see
    // if send should be made unlikely (e.g. sends in prim. failure branches)
    // fix this
    return _info->rcvr;
  }

  // l is the list of receivers predicted by the PIC
  GrowableArray<RScope*>* predictedReceivers = sender->rscope->subScopes(sender->bci());

  // check special cases: never executed or uninlinable/megamorphic
  if (predictedReceivers->length() == 1) {
    if (predictedReceivers->first()->isUntakenScope()) {
      // send was never executed
      return picPredictUnlikely(_info, (RUntakenScope*)predictedReceivers->first());
    } else if (predictedReceivers->first()->isUninlinableScope()) {
      if (CompilerDebug)
        cout(PrintInlining)->print("%*s*PIC-predicting %s as uninlinable/megamorphic\n", depth, "", _info->sel->as_string());
      _info->uninlinable = true;	// prevent static type prediction
      return _info->rcvr;
    }
  }

  // check special case: perfect information (from dataflow information)
  if (!_info->rcvr->containsUnknown()) {
    return _info->rcvr;	// already know type exactly, don't use PIC
  }

  // extract klasses from PIC
  GrowableArray<Expr*> klasses(5);
  MergeExpr* allKlasses = new MergeExpr(_info->rcvr->preg(), NULL);
  for (int i = 0; i < predictedReceivers->length(); i++) {
    RScope* r = predictedReceivers->at(i);
    Expr* expr = r->receiverExpr(_info->rcvr->preg());
    if (expr->isUnknownExpr()) {
      // untaken real send (from PIC) (usually the "otherwise" branch of predicted sends)
      // since prediction was always correct, make sure unknown case is unlikely
      assert(((UnknownExpr*)expr)->isUnlikely(), "performance bug: should make unknown unlikely");
      continue;
    }
    klasses.append(expr);
    allKlasses = (MergeExpr*)allKlasses->mergeWith(expr, NULL);
  }

  // check if PIC info is better than static type info; discard all static info
  // that's not in the PIC
  int npic = klasses.length();
  int nstatic = _info->rcvr->nklasses();
  if (npic != 0 && _info->rcvr->isMergeExpr()) {
    Expr* newRcvr = _info->rcvr;
    for (int i = ((MergeExpr*)_info->rcvr)->exprs->length() - 1; i >= 0; i--) {
      Expr* e = ((MergeExpr*)_info->rcvr)->exprs->at(i);
      if (e->isUnknownExpr()) continue;
      if (!allKlasses->findKlass(e->klass())) {
        if (PrintInlining) {
          std->print("%*s*discarding static type info for send %s (not found in PIC): ",
            depth, "", _info->sel->as_string());
          e->print();
        }
        newRcvr = newRcvr->copyWithout(e);
      }
    }
    _info->rcvr = newRcvr;
  }

  if (CompilerDebug)
    cout(PrintInlining)->print("%*s*PIC-type-predicting %s (%ld klasses): ", depth, "",
    _info->sel->as_string(), npic);

  // iterate through PIC _info and add it to the receiver type (_info->rcvr)
  for (i = 0; i < klasses.length(); i++) {
    Expr* expr = klasses.at(i);
    // use the PIC information for this case
    if (CompilerDebug) {
#ifndef PRODUCT // otherwise we get a syntax error (C++ compiler bug?) - gri 11/28/01
      expr->klass()->klass_part()->print_name_on(cout(PrintInlining));
      cout(PrintInlining)->print(" ");
#endif
    }
    Expr* alreadyThere = _info->rcvr->findKlass(expr->klass());
    // add klass only if not already present (for splitting -- adding klass
    // expr with node == NULL destroys splitting opportunity)
    if (alreadyThere) {
      // generalize constant to klass
      if (alreadyThere->isConstantExpr()) {
        _info->rcvr = _info->rcvr->mergeWith(expr, NULL);
      }
    } else {
      _info->predicted = true;
      _info->rcvr = _info->rcvr->mergeWith(expr, NULL);
      if (expr->hasConstant() && klasses.length() == 1) {
        // check to see if single predicted receiver is true or false;
        // if so, add other boolean to prediction.  Reduces the number
        // of uncommon branches; not doing so appears to be overly
        // aggressive (as observed experimentally)
        oop c = expr->constant();
        PReg* p = _info->rcvr->preg();
        if (c == trueObj && !_info->rcvr->findKlass(falseObj->klass())) {
          Expr* f = new ConstantExpr(falseObj, p, NULL);
          _info->rcvr = _info->rcvr->mergeWith(f, NULL);
        } else if (c == falseObj && !_info->rcvr->findKlass(trueObj->klass())) {
          Expr* t = new ConstantExpr(trueObj, p, NULL);
          _info->rcvr = _info->rcvr->mergeWith(t, NULL);
        }
      }
    }
  } // for
  if (CompilerDebug) cout(PrintInlining)->print("\n");

  // mark unknown branch as unlikely
  UnknownExpr* u = _info->rcvr->findUnknown();
  const bool canBeUnlikely = theCompiler->useUncommonTraps;
  if (u && canBeUnlikely && !rscope->isNotUncommonAt(bci)) {
    _info->rcvr = _info->rcvr->makeUnknownUnlikely(sender);
  }

  assert(_info->rcvr->preg(), "should have a preg");
  return _info->rcvr;
}

Expr* Inliner::picPredictUnlikely(SendInfo* info, RUntakenScope* uscope) {
  if (!theCompiler->useUncommonTraps) return info->rcvr;

  bool makeUncommon = uscope->isUnlikely();
  if (!makeUncommon && info->inPrimFailure) {
    // this send was never executed in the recompilee
    // only make the send unlikely if it had a chance to execute
    // (If the send isn't a prim failure, don't trust the info --
    // it's unlikely that the method just stops executing in the middle.
    // What probably happened is that recompilation occurred before the
    // rest of the method got a chance to execute (e.g. recursion), or it
    // always quit via NLR.  In any case, the compiler can't handle this
    // yet - need to treat it specially similar to endsDead.)
    makeUncommon = false;
  }
  if (false && CompilerDebug) {
    cout(PrintInlining)->print("%*s*%sPIC-type-predicting %s as never executed\n",
      depth, "", makeUncommon ? "" : "NOT ",
      info->sel->copy_null_terminated());
  }
  if (makeUncommon) {
    return new UnknownExpr(info->rcvr->preg(), NULL, true);
  } else {
    return info->rcvr;
  }
}

Expr* Inliner::typePredict() {
  // NB: all non-predicted cases exit this function early
  Expr* r = _info->rcvr;
  if (!(r->isUnknownExpr() ||
    r->isMergeExpr() &&
    ((MergeExpr*)r)->exprs->length() == 1 &&
    ((MergeExpr*)r)->exprs->at(0)->isUnknownExpr())) {
      // r already has a type (e.g. something predicted via PICs)
      // trust that information more than the static type prediction
      // NB: UnknownExprs are sometimes merged into a MergeExpr, that's why the above
      // test looks a bit more complicated
      return _info->rcvr;
  }

  // perform static type prediction
  if (InliningPolicy::isPredictedSmiSelector(_info->sel)) {
    r = r->mergeWith(new KlassExpr(smiKlassObj, r->preg(), NULL), NULL);
  } else if (InliningPolicy::isPredictedArraySelector(_info->sel)) {
    // don't know what to predict -- objArray? byteArray?
    if (TypePredictArrays) {
      r = r->mergeWith(new KlassExpr(Universe::objArrayKlassObj(), r->preg(), NULL), NULL);
    } else {
      return r;
    }
  } else if (InliningPolicy::isPredictedBoolSelector(_info->sel)) {
    r = r->mergeWith(new ConstantExpr(trueObj,  r->preg(), NULL), NULL);
    r = r->mergeWith(new ConstantExpr(falseObj, r->preg(), NULL), NULL);
  } else {
    return r;
  }

  // receiver was type-predicted
  if (theCompiler->useUncommonTraps) {
    // make unknon case unlikely
    r = r->makeUnknownUnlikely(sender);
  }
  return r;
}

extern bool SuperSendsAreAlwaysInlined = true;	// remove when removing super hack

InlinedScope* Inliner::tryLookup(Expr* rcvr) {
  // try to lookup the send to receiver rcvr and determine if it should be inlined;
  // return new InlinedScope if ok, NULL if lookup error or non-inlinable
  // NB: _info->rcvr is the overall receiver (e.g. a merge expr), rcvr is the particular branch
  // we're looking at right now
  assert(rcvr->hasKlass(), "should know klass");

  const klassOop  klass = (kind == SuperSend) ? sender->methodHolder() : rcvr->klass();
  if (klass == NULL) {
    _info->uninlinable = true;
    assert(kind == SuperSend, "shouldn't happen for normal sends");
    return notify("super send in Object");
  }

  const symbolOop selector = _info->sel;
  const methodOop method = (kind == SuperSend) ?
    lookupCache::compile_time_super_lookup (klass, selector) :
  lookupCache::compile_time_normal_lookup(klass, selector);
  lastLookupFailed = method == NULL;

  if (lastLookupFailed) {
    // nothing found statically (i.e., lookup error)
    _info->uninlinable = true;		// fix this -- probably wrong for merge exprs.
    return notify("lookup failed");
  }

  // Construct a lookup key
  LookupKey* key = kind == SuperSend
    ? LookupKey::allocate(rcvr->klass(), method)
    : LookupKey::allocate(rcvr->klass(), selector);

  if (CompilerDebug) cout(PrintInlining)->print("%*s found %s --> %#x\n", sender->depth, "", key->print_string(), PrintHexAddresses ? method : 0);

  // NB: use rcvr->klass() (not klass) for the scope -- klass may be the method holder (for super sends)
  // was bug -- Urs 3/16/96
  InlinedScope* callee = makeScope(rcvr, rcvr->klass(), key, method);
  assert(kind != SuperSend || callee != NULL, "Super send must be inlined");

  _msg = inliningPolicy.shouldInline(sender, callee);
  if (kind == SuperSend) {
    // for now, always inline super sends because run-time system can't handle them yet
    // fix this later -- Urs 12/95
    SuperSendsAreAlwaysInlined = true;
    assert(!_info->predicted, "shouldn't PIC-predict super sends");
    _msg = NULL;
  } else {
    if (_msg == NULL && sender->gen()->in_prim_failure_block()) {
      _msg = checkSendInPrimFailure();
    }
  }
  if (_msg) return notify(_msg);		// shouldn't inline this call

  return callee;
}

char* Inliner::checkSendInPrimFailure() {
  // The current send could be inlined, but it is in a primitive failure block.  Decide
  // if it really should be inlined (return NULL if so, msg if not).
  RScope* rs = sender->rscope;
  if (rs->isNullScope() || rs->isInterpretedScope()) {
    // not compiled -- don't inline, failure probably never happens
    // but make sure we'll detect if it does happen often
    if (UseRecompilation) _info->counting = true;
    return "send in primitive failure (null/interpreted sender)";
  }
  RScope* callee = rs->subScope(sender->bci(), _info->key);
  if (callee->isUntakenScope()) {
    // never executed; shouldn't even generate code for failure!
    // (note: if failure block has several sends, this can happen, but in the standard
    // system it's probably a performance bug)
    if (WizardMode) warning("probably should have made primitive failure uncommon");
    return "untaken send in primitive failure";
  }
  if (callee->isInlinedScope()) {
    // was inlined in previous versions, so I guess it's ok to inline it again
    return NULL;
  }
  if (callee->isNullScope()) {
    // no info; conservative thing is not to inline
    return "untaken send in primitive failure (NullScope)";
  }
  // ok, inline this send
  return NULL;
}

RScope* Inliner::makeBlockRScope(const Expr* rcvr, LookupKey* key, const methodOop method) {
  // create an InlinedScope for this block method
  if (!TypeFeedback) return new RNullScope;
  if (!rcvr->preg()->isBlockPReg()) {
    return new RNullScope;	  // block parent is in a different nmethod -- won't inline
  }

  // first check if block was inlined in previous nmethod (or comes from inlining database)
  const int senderBCI = sender->bci();
  GrowableArray<RScope*>* subScopes = sender->rscope->subScopes(senderBCI);
  if (subScopes->length() == 1 && subScopes->first()->method() == method) {
    RScope* rs = subScopes->first();
    assert(rs->method() == method, "mismatched block methods");
    return rs;
  }

  // must compute rscope differently -- primitiveValue has no inline cache, so must
  // get callee nmethod or method manually
  InlinedScope* parent = rcvr->preg()->scope();
  RScope* rparent = parent->rscope;
  const int level = rparent->isNullScope() ? 1 : ((RNonDummyScope*)rparent)->level();
  if (rparent->isCompiled()) {
    // get type feedback info from compiled block nmethod
    nmethod* parentNM = rparent->get_nmethod();
    // search for corresponding noninlined block nmethod
    int blockIndex = 0;
    for (blockIndex = parentNM->number_of_noninlined_blocks(); blockIndex >= 1; blockIndex--) {
      if (parentNM->noninlined_block_method_at(blockIndex) == method &&
        checkSenderPath(parent, parentNM->noninlined_block_scope_at(blockIndex)->parent())) {
          break;	  // found it
      }
    }
    if (blockIndex >= 1) {
      // try to use non-inlined block nmethod (if it was compiled)
      jumpTableEntry* jte = parentNM->noninlined_block_jumpEntry_at(blockIndex);
      if (jte->is_nmethod_stub()) {
        // use non-inlined block nmethod
        nmethod* blockNM = jte->method();
        assert(blockNM->method() == method, "method mismatch");
        return new RInlinedScope(NULL, senderBCI, blockNM, blockNM->scopes()->root(), level);
      } else {
        // block nmethod hasn't been compiled yet -- use interpreted method
        return new RInterpretedScope(NULL, senderBCI, key, method, level, true);
      }
    } else {
      // block was inlined in previous nmethod, but not found in rscope
      assert(sender->rscope->isNullScope() || sender->rscope->isLite(), "should have found subscope");
      return new RInterpretedScope(NULL, senderBCI, key, method, level, true);
    }
  } else {
    // get info from interpreted block method
    return new RInterpretedScope(NULL, senderBCI, key, method, level, true);
  }
}


bool Inliner::checkSenderPath(Scope* here, ScopeDesc* there) const {
  // return true if sender paths of here and there match
  // NB: I believe the code below isn't totally correct, in the sense that it
  // will return ok == true if the sender paths match up to the point where
  // one path crosses an nmethod boundary -- but there could be a difference
  // further up.
  // However, this imprecision isn't dangerous -- we'll just use type information
  // that may not be accurate.  But since the sender paths agreed up to that
  // point, it is likely the types are at least similar.
  // Fix this later.  -Urs 8/96
  while (here && !there->isTop()) {
    InlinedScope* sen = here->sender();
    if (sen == NULL) break;
    if (sen->bci() != there->senderBCI()) return false;
    here  = here->sender();
    there = there->sender();
  }
  return true;
}


InlinedScope* Inliner::makeScope(const Expr* rcvr, const klassOop klass, const LookupKey* key, const methodOop method) {
  // create an InlinedScope for this method/receiver
  SendInfo* calleeInfo = new SendInfo(*_info);
  calleeInfo->key = (LookupKey*) key;
  const klassOop methodHolder = klass->klass_part()->lookup_method_holder_for(method);

  if (method->is_blockMethod()) {
    RScope* rs = makeBlockRScope(rcvr, calleeInfo->key, method);
# ifdef ASSERT
    bool isNullRScope = rs->isNullScope(); 	// for conditional breakpoints (no type feedback info)
# endif
    if (rcvr->preg()->isBlockPReg()) {
      InlinedScope* parent = rcvr->preg()->scope();
      calleeInfo->rcvr = parent->self();
      callee = BlockScope::new_BlockScope(method, methodHolder, parent, sender, rs, calleeInfo);
      callee->set_self(parent->self());
    } else {
      // block parent is in a different nmethod -- don't inline
      return notify("block parent in different nmethod");
    }
  } else {
    // normal method
    RScope* rs = sender->rscope->subScope(sender->bci(), calleeInfo->key);
# ifdef ASSERT
    bool isNullRScope = rs->isNullScope(); 	// for conditional breakpoints (no type feedback info)
# endif
    callee = MethodScope::new_MethodScope(method, methodHolder, sender, rs, calleeInfo);
    callee->set_self(rcvr->asReceiver());
  }

  return callee;
}

InlinedScope* Inliner::notify(const char* msg) {
  if (CompilerDebug) {
    cout(PrintInlining)->print("%*s*cannot inline %s, cost = %ld (%s)\n", depth, "", _info->sel->as_string(),
      inliningPolicy.calleeCost, msg);
  }
  _msg = (char*)msg;
  return NULL;	// cheap trick to make notify more convenient (can say "return notify(...)")
}

void Inliner::print() {
  std->print("((Inliner*)%#x)\n", PrintHexAddresses ? this : 0);
}

Expr* Inliner::inlineBlockInvocation(SendInfo* info) {
  initialize(info, NormalSend);
  Expr* blockExpr = info->rcvr;
  assert(blockExpr->preg()->isBlockPReg(), "must be a BlockPR");
  const BlockPReg* block = (BlockPReg*)blockExpr->preg();
  const methodOop method = block->closure()->method();
  const InlinedScope* parent = block->closure()->parent_scope();
  // should decide here whether to actually inline -- fix this
  if (CompilerDebug) cout(PrintInlining)->print("%*s*inlining block invocation\n", sender->depth, "");

  // Construct a fake lookupKey
  LookupKey* key = LookupKey::allocate(parent->selfKlass(), method);

  makeScope(blockExpr, parent->selfKlass(), key, method);
  if (callee) {
    Expr* r = doInline(sender->current());
    return makeResult(r);
  } else {
    return NULL;	// couldn't inline the block
  }
}

# endif
