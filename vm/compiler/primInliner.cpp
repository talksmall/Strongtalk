/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.100 $ */
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
# include "incls/_primInliner.cpp.incl"

inline bool equal(char* s, char* t) { return strcmp(s, t) == 0; }


void PrimInliner::assert_failure_block() {
  assert(_failure_block != NULL, "primitive must have a failure block");
}


void PrimInliner::assert_no_failure_block() {
  assert(_failure_block == NULL, "primitive doesn't have a failure block");
}


void PrimInliner::assert_receiver() {
  assert(parameter(0) == _scope->self(), "first parameter must be self");
}


int PrimInliner::log2(int x) const {
  int i = -1;
  int p =  1;
  while (p != 0 && p <= x) {
    // p = 2^(i+1) && p <= x (i.e., 2^(i+1) <= x)
    i++; p *= 2;
  }
  // p = 2^(i+1) && x < p (i.e., 2^i <= x < 2^(i+1))
  // (if p = 0 then overflow occured and i = 31)
  return i;
}


Expr* PrimInliner::tryConstantFold() {
  // Returns the result if the primitive call has been constant folded
  // successfully; returns NULL otherwise.
  // Note: The result may be a marked oop - which has to be unmarked
  // before using it - and which indicates that the primitive will fail
  // always.
  if (!_pdesc->can_be_constant_folded()) {
    // check for Symbol>>at: before declaring failure
    if ((equal(_pdesc->name(), "primitiveIndexedByteAt:ifFail:") ||
         equal(_pdesc->name(), "primitiveIndexedByteCharacterAt:ifFail:")) &&
        parameter(0)->hasKlass() && parameter(0)->klass() == Universe::symbolKlassObj()) {
      // the at: primitive can be constant-folded for symbols
      // what if the receiver is a constant string? unfortunately, Smalltalk has
      // "assignable constants" like Fortran...
    } else {
      return NULL;
    }
  }
  // get parameters
  int i = number_of_parameters();
  oop* args = NEW_RESOURCE_ARRAY(oop, i);
  while (i > 0) {
    i--;
    Expr* arg = parameter(i);
    if (!arg->isConstantExpr()) return NULL;
    args[i] = arg->constant();
  }
  // all parameters are constants, so call primitive
  oop res = _pdesc->eval(args);
  if (res->is_mark()) {
    // primitive failed
    return primitiveFailure(unmarkSymbol(res));
  } else if (res->is_mem() && !res->is_old()) {
    // must tenure result because nmethods aren't scavenged
    if (res->is_double()) {
      res = oopFactory::clone_double_to_oldspace(doubleOop(res));
    } else {
      // don't know how to tenure non-doubles
      warning("const folding: primitive %s is returning non-tenured object", _pdesc->name());
      return NULL;
    }
  }
  ConstPReg* constResult = new_ConstPReg(_scope, res);
  SAPReg* result = new SAPReg(_scope);
  _gen->append(NodeFactory::new_AssignNode(constResult, result));
  if (CompilerDebug) cout(PrintInlining)->print("%*sconstant-folding %s --> %#x\n", _scope->depth + 2, "", _pdesc->name(), res);
  assert(!constResult->constant->is_mark(), "result must not be marked");
  return new ConstantExpr(res, constResult, _gen->current());
}


Expr* PrimInliner::tryTypeCheck() {
  // Check if we have enough type information to prove that the primitive is going to fail;
  // if so, directly compile failure block (important for mixed-mode arithmetic).
  // Returns the failure result if the primitive call has been proven
  // to fail; returns NULL otherwise.
  // Should extend code to do general type compatibility test (including MergeExprs, e.g. for 
  // booleans) -- fix this later.  -Urs 11/95
  int num = number_of_parameters();
  for (int i = 0; i < num; i++) {
    Expr* a = parameter(i);
    if (a->hasKlass()) {
      Expr* primArgType = _pdesc->parameter_klass(i, a->preg(), NULL);
      if (primArgType && primArgType->hasKlass() && (a->klass() != primArgType->klass())) {
        // types differ -> primitive must fail
	return primitiveFailure(failureSymbolForArg(i));
      }
    }
  }
  return NULL;
}


symbolOop PrimInliner::failureSymbolForArg(int i) {
  assert(i >= 0 && i < number_of_parameters(), "bad index");
  switch (i) {
    case  0: return vmSymbols::receiver_has_wrong_type();
    case  1: return vmSymbols::first_argument_has_wrong_type();
    case  2: return vmSymbols::second_argument_has_wrong_type();
    case  3: return vmSymbols::third_argument_has_wrong_type();
    case  4: return vmSymbols::fourth_argument_has_wrong_type();
    case  5: return vmSymbols::fifth_argument_has_wrong_type();
    case  6: return vmSymbols::sixth_argument_has_wrong_type();
    case  7: return vmSymbols::seventh_argument_has_wrong_type();
    case  8: return vmSymbols::eighth_argument_has_wrong_type();
    case  9: return vmSymbols::ninth_argument_has_wrong_type();
    case 10: return vmSymbols::tenth_argument_has_wrong_type();
    default: return vmSymbols::argument_has_wrong_type();
  }
}


// Helper class to find the first assignment to a temporary in a failure
// block and the remaining method interval (without the assignment).
class AssignmentFinder: public SpecializedMethodClosure {
 public:
  int             tempNo;	// the temporary to which the assignment took place
  MethodInterval* block;	// the block over which to iterate
  MethodInterval* interval; 	// the rest of the block without the assignment

  AssignmentFinder(MethodInterval* b) : SpecializedMethodClosure() {
    block = b;
    interval = NULL;
    MethodIterator iter(block, this);
  }
  void instruction()		{ abort(); }
  void store_temporary(int no)	{ 
    tempNo = no; 
    interval = MethodIterator::factory->new_MethodInterval(method(), NULL, next_bci(), block->end_bci()); 
  }
};


Expr* PrimInliner::primitiveFailure(symbolOop failureCode) {
  // compile a failing primitive
  if (CompilerDebug) cout(PrintInlining)->print("%*sprimitive %s will fail: %s\n", _scope->depth + 2, "", _pdesc->name(), failureCode->as_string());
  if (_failure_block == NULL) {
    error("\nPotential compiler bug: compiler believes primitive %s will fail\nwith error %s, but primitive has no failure block.\n",
          _pdesc->name(), failureCode->as_string());
    return NULL;
  }
  // The byte code compiler stores the primitive result in a temporary (= the parameter of
  // the inlined failure block).  Thus, we store res in the corresponding temp Expr so that
  // its value isn't forgotten (the default Expr for temps is UnknownExpr) and restore the
  // temp's original value afterwards. This is safe because within the failure block that
  // temporary is treated as a parameter and therefore is read-only.
  AssignmentFinder closure(_failure_block);
  assert(closure.interval != NULL, "no assignment found");
  Expr* old_temp = _scope->temporary(closure.tempNo);	// save temp
  Expr* res = new ConstantExpr(failureCode, new_ConstPReg(_scope, failureCode), _gen->current());
  _scope->set_temporary(closure.tempNo, res);
  closure.set_prim_failure(false);		// allow inlining sends in failure branch
  _gen->generate_subinterval(closure.interval, true);
  _scope->set_temporary(closure.tempNo, old_temp);
  return _gen->exprStack()->pop();		// restore temp
}

Expr* PrimInliner::merge_failure_block(Node* ok_exit, Expr* ok_result, Node* failure_exit, Expr* failure_code, bool ok_result_is_read_only) {
  assert(failure_code->hasKlass() && failure_code->klass() == Universe::symbolKlassObj(), "failure code must be a symbol");
  _cannotFail = false;
  // push failure code
  _gen->setCurrent(failure_exit);
  _gen->exprStack()->push(failure_code, _scope, failure_exit->bci());
  // code for failure block
  _gen->generate_subinterval(_failure_block, true);
  Expr* failure_block_result = _gen->exprStack()->pop();
  if (failure_block_result->isNoResultExpr()) {
    // failure doesn't reach here (e.g., uncommon branch)
    assert(!_gen->current() || _gen->current() == NodeBuilder::EndOfCode, "shouldn't reach here");
    _gen->setCurrent(ok_exit);
    return ok_result;
  } else {
    if (ok_result_is_read_only) {
      // cannot assign to ok_result directly => introduce extra PReg & extra assignment
      // (was bug detected with new backend: assignment to ok_result is not legal if it
      // refers to parameter/receiver of the method calling the primitive - gri 6/29/96)
      PReg* resPReg = new SAPReg(_scope);
      Node* node    = NodeFactory::new_AssignNode(ok_result->preg(), resPReg);
      Expr* resExpr = ok_result->shallowCopy(resPReg, node);
      ok_exit->append(node);
      ok_exit = node;
      ok_result = resExpr;
    }
    _gen->append(NodeFactory::new_AssignNode(failure_block_result->preg(), ok_result->preg()));
    // merge after failure block
    MergeNode* prim_exit = NodeFactory::new_MergeNode(_failure_block->end_bci());
    _gen->append(prim_exit);
    ok_exit->append(prim_exit);
    _gen->setCurrent(prim_exit);
    return new MergeExpr(ok_result, failure_block_result, ok_result->preg(), prim_exit);
  }
}

class PrimSendFinder : public SpecializedMethodClosure {
public:
  RScope* rs;
  bool was_executed;

  PrimSendFinder(RScope* rs)			{ this->rs = rs; was_executed = false; rs->extend(); }
  virtual void normal_send(InterpretedIC* ic) 	{ check_send(ic); }
  virtual void self_send  (InterpretedIC* ic) 	{ check_send(ic); }
  virtual void super_send (InterpretedIC* ic) 	{ check_send(ic); }
  void check_send(InterpretedIC* ic) {
    GrowableArray<RScope*>* sub = rs->subScopes(bci());
    if (sub->length() == 1 && sub->first()->isUntakenScope()) {
      // this send was never taken
    } else {
      was_executed = true;			// this send was taken in recompilee
    }
  }
};


inline bool PrimInliner::basic_shouldUseUncommonTrap() const {
  ResourceMark rm;
  if (!theCompiler->useUncommonTraps) return false; 
  // first check if recompilee has a taken uncommon branch 
  // NOTE: the uncommon branch should be at _failure_block->begin_bci(), not _bci, but
  // deoptimization currently breaks if that's so -- please fix this, Lars
  // (search for new_UncommonNode below when fixing it)
  if (_scope->rscope->isNotUncommonAt(_bci)) return false;
  //if (_scope->rscope->isNotUncommonAt(_failure_block->begin_bci())) return false;
  if (_scope->rscope->isUncommonAt(_bci)) return true;
  //if (_scope->rscope->isUncommonAt(_failure_block->begin_bci())) return true;
  // now check if recompilee had code for the failure block, and any send in it was taken
  if (!_scope->rscope->get_nmethod()) {
    return true;      // if it's not compiled, assume that using uncommon branch is ok
  }
#ifdef broken
  // This code doesn't really do the right thing; what's needed is something that
  // can determine whether the failure was taken in the previous version of compiled code.
  // If there's a send that's easy (ic dirty flag), but if everything in the failure block
  // was inlined...?  Could put a "removable trap" there to determine this.  fix this later
  // -Urs 8/96
  PrimSendFinder p(_scope->rscope);
  MethodIterator it(_failure_block, &p);
  return !p.was_executed;
#endif
  // for now, try the aggressive approach: no proof that it was taken, so guess that it's uncommon
  return true;
}

bool PrimInliner::shouldUseUncommonTrap() {
  // remember result of call in _usingUncommonTrap
  _usingUncommonTrap = basic_shouldUseUncommonTrap(); 
  return _usingUncommonTrap;
}

Expr* PrimInliner::smi_ArithmeticOp(ArithOpCode op, Expr* arg1, Expr* arg2) {
  assert_failure_block();
  assert_receiver();
  
  // parameters & result registers
  bool       intArg1      = arg1->is_smi();
  bool       intArg2      = arg2->is_smi();
  bool       intBoth      = intArg1 && intArg2;			// if true, tag error cannot occur
  SAPReg*    resPReg      = new SAPReg(_scope);			// holds the result if primitive didn't fail
  SAPReg*    errPReg      = new SAPReg(_scope);			// holds the error message if primitive failed
  MergeNode* failureStart = NodeFactory::new_MergeNode(_failure_block->begin_bci());

  // n1: operation & treatment of tag error
  Node* n1;
  AssignNode*  n1Err;
  ConstantExpr* n1Expr = NULL;
  if (intBoth) {
    // tag error cannot occur
    n1 = NodeFactory::new_ArithRRNode(resPReg, arg1->preg(), op, arg2->preg());
  } else {
    // tag error can occur
    n1 = NodeFactory::new_TArithRRNode(resPReg, arg1->preg(), op, arg2->preg(), intArg1, intArg2);
    if (shouldUseUncommonTrap()) {
      // simply jump to uncommon branch code
      n1->append(1, failureStart);
    } else {
      ConstPReg* n1PReg = new_ConstPReg(_scope, vmSymbols::first_argument_has_wrong_type());
      n1Err = NodeFactory::new_AssignNode(n1PReg, errPReg);
      n1Expr = new ConstantExpr(n1PReg->constant, errPReg, n1Err);
      n1->append(1, n1Err);
      n1Err->append(failureStart);
    }
  }
  _gen->append(n1);
  Expr* result = new KlassExpr(smiKlassObj, resPReg, n1);

  // n2: overflow check & treatment of overflow
  const bool taken_is_uncommon = true;
  BranchNode* n2 = NodeFactory::new_BranchNode(VSBranchOp, taken_is_uncommon);
  AssignNode* n2Err;
  ConstantExpr* n2Expr = NULL;
  if (shouldUseUncommonTrap()) {
    // simply jump to uncommon branch code
    n2->append(1, failureStart);
  } else {
    ConstPReg* n2PReg = new_ConstPReg(_scope, vmSymbols::smi_overflow());
    n2Err = NodeFactory::new_AssignNode(n2PReg, errPReg);
    n2Expr = new ConstantExpr(n2PReg->constant, errPReg, n2Err);
    n2->append(1, n2Err);
    n2Err->append(failureStart);
  }
  _gen->append(n2);

  // continuation
  if (shouldUseUncommonTrap()) {
    // uncommon branch instead of failure code
    failureStart->append(NodeFactory::new_UncommonNode(_gen->copyCurrentExprStack(), _bci /*_failure_block->begin_bci()*/));
  } else {
    assert(n2Expr != NULL, "no error message defined");
    Expr* error;
    if (n1Expr != NULL) {
      error = new MergeExpr(n1Expr, n2Expr, errPReg, failureStart);
    } else {
      error = n2Expr;
    }
    result = merge_failure_block(n2, result, failureStart, error, false);
  }
  return result;
}


Expr* PrimInliner::smi_BitOp(ArithOpCode op, Expr* arg1, Expr* arg2) {
  assert_failure_block();
  assert_receiver();

  // parameters & result registers
  bool    intArg1 = arg1->is_smi();
  bool    intArg2 = arg2->is_smi();
  bool    intBoth = intArg1 && intArg2;	// if true, tag error cannot occur
  SAPReg* resPReg = new SAPReg(_scope);	// holds the result if primitive didn't fail
  SAPReg* errPReg = new SAPReg(_scope);	// holds the error message if primitive failed

  // n1: operation & treatment of tag error
  Node*  n1;
  AssignNode* n1Err;
  ConstantExpr* n1Expr;
  if (intBoth) {
    // tag error cannot occur
    n1 = NodeFactory::new_ArithRRNode(resPReg, arg1->preg(), op, arg2->preg());
  } else {
    // tag error can occur
    n1 = NodeFactory::new_TArithRRNode(resPReg, arg1->preg(), op, arg2->preg(), intArg1, intArg2);
    ConstPReg* n1PReg = new_ConstPReg(_scope, vmSymbols::first_argument_has_wrong_type());
    n1Err = NodeFactory::new_AssignNode(n1PReg, errPReg);
    n1Expr = new ConstantExpr(n1PReg->constant, errPReg, n1Err);
    n1->append(1, n1Err);
  }
  _gen->append(n1);

  // continuation
  Expr* result = new KlassExpr(smiKlassObj, resPReg, n1);
  if (!intBoth) {
    // failure can occur
    if (shouldUseUncommonTrap()) {
      // simply do an uncommon trap
      n1Err->append(NodeFactory::new_UncommonNode(_gen->copyCurrentExprStack(), _bci /*_failure_block->begin_bci()*/));
    } else {
      // treat failure case
      result = merge_failure_block(n1, result, n1Err, n1Expr, false);
    }
  }
  return result;
}


Expr* PrimInliner::smi_Div(Expr* x, Expr* y) {
  if (y->preg()->isConstPReg()) {
    assert(y->is_smi(), "type check should have failed");
    int d = smiOop(((ConstPReg*)y->preg())->constant)->value();
    if (is_power_of_2(d)) {
      // replace it with shift
      ConstPReg* preg = new_ConstPReg(_scope, as_smiOop(-log2(d)));
      return smi_BitOp(tShiftArithOp, x, new ConstantExpr(preg->constant, preg, _gen->_current));
    }
  }
  // otherwise leave it alone
  return NULL;
}


Expr* PrimInliner::smi_Mod(Expr* x, Expr* y) {
  if (y->preg()->isConstPReg()) {
    assert(y->is_smi(), "type check should have failed");
    int d = smiOop(((ConstPReg*)y->preg())->constant)->value();
    if (is_power_of_2(d)) {
      // replace it with mask
      ConstPReg* preg = new_ConstPReg(_scope, as_smiOop(d-1));
      return smi_BitOp(tAndArithOp, x, new ConstantExpr(preg->constant, preg, _gen->_current));
    }
  }
  // otherwise leave it alone
  return NULL;
}


Expr* PrimInliner::smi_Shift(Expr* arg1, Expr* arg2) {
  if (parameter(1)->preg()->isConstPReg()) {
    // inline if the shift count is a constant
    assert(arg2->is_smi(), "type check should have failed");
    return smi_BitOp(tShiftArithOp, arg1, arg2);
  }
  // otherwise leave it alone
  return NULL;
}


static BranchOpCode not(BranchOpCode cond) {
  switch (cond) {
    case EQBranchOp : return NEBranchOp;
    case NEBranchOp : return EQBranchOp;
    case LTBranchOp : return GEBranchOp;
    case LEBranchOp : return GTBranchOp;
    case GTBranchOp : return LEBranchOp;
    case GEBranchOp : return LTBranchOp;
    case LTUBranchOp: return GEUBranchOp;
    case LEUBranchOp: return GTUBranchOp;
    case GTUBranchOp: return LEUBranchOp;
    case GEUBranchOp: return LTUBranchOp;
    case VSBranchOp : return VCBranchOp;
    case VCBranchOp : return VSBranchOp;
  }
  ShouldNotReachHere();
  return EQBranchOp;
}


Expr* PrimInliner::generate_cond(BranchOpCode cond, NodeBuilder* gen, PReg* resPReg) {
// generate code loading true or false depending on current condition code
// Note: condition is negated in order to provoke a certain code order
//       when compiling whileTrue loops - gri 6/26/96

  // n2: conditional branch
  BranchNode* n2 = NodeFactory::new_BranchNode(not(cond));
  gen->append(n2);

  // tAsgn: true branch
  ConstPReg* tPReg = new_ConstPReg(gen->scope(), trueObj);
  AssignNode* tAsgn = NodeFactory::new_AssignNode(tPReg, resPReg);
  ConstantExpr* tExpr = new ConstantExpr(trueObj, resPReg, tAsgn);
  n2->append(0, tAsgn);

  // fAsgn: false branch
  ConstPReg* fPReg = new_ConstPReg(gen->scope(), falseObj);
  AssignNode* fAsgn = NodeFactory::new_AssignNode(fPReg, resPReg);
  ConstantExpr* fExpr = new ConstantExpr(falseObj, resPReg, fAsgn);
  n2->append(1, fAsgn);

  // ftm: false & true branch merger
  MergeNode* ftm = NodeFactory::new_MergeNode(fAsgn, tAsgn);
  Expr* result = new MergeExpr(fExpr, tExpr, resPReg, ftm);
  gen->setCurrent(ftm);
  return result;
}


Expr* PrimInliner::smi_Comparison(BranchOpCode cond, Expr* arg1, Expr* arg2) {
  assert_failure_block();
  assert_receiver();

  // parameters & result registers
  bool    intArg1 = arg1->is_smi();
  bool    intArg2 = arg2->is_smi();
  bool    intBoth = intArg1 && intArg2;	// if true, tag error cannot occur
  SAPReg* resPReg = new SAPReg(_scope);	// holds the result if primitive didn't fail
  SAPReg* errPReg = new SAPReg(_scope);	// holds the error message if primitive failed

  // n1: comparison & treatment of tag error
  Node* n1;
  ConstPReg* n1PReg;
  AssignNode* n1Err;
  ConstantExpr* n1Expr;
  if (intBoth) {
    // tag error cannot occur
    n1 = NodeFactory::new_ArithRRNode(new NoPReg(_scope), arg1->preg(), tCmpArithOp, arg2->preg());
  } else {
    // tag error can occur
    n1 = NodeFactory::new_TArithRRNode(new NoPReg(_scope), arg1->preg(), tCmpArithOp, arg2->preg(), intArg1, intArg2);
    n1PReg = new_ConstPReg(_scope, vmSymbols::first_argument_has_wrong_type());
    n1Err = NodeFactory::new_AssignNode(n1PReg, errPReg);
    n1Expr = new ConstantExpr(n1PReg->constant, errPReg, n1Err);
    n1->append(1, n1Err);
  }
  _gen->append(n1);

  Expr* result = generate_cond(cond, _gen, resPReg);

  // continuation
  if (!intBoth) {
    // failure can occur
    if (shouldUseUncommonTrap()) {
      // simply do an uncommon trap
      n1Err->append(NodeFactory::new_UncommonNode(_gen->copyCurrentExprStack(), _bci /*_failure_block->begin_bci()*/));
    } else {
      // treat failure case
      result = merge_failure_block(result->node(), result, n1Err, n1Expr, false);
    }
  }
  return result;
}


Expr* PrimInliner::array_size() {
  assert(_failure_block == NULL, "primitive must have no failure block");
  assert_receiver();

  // parameters & result registers
  Expr*  array = parameter(0);
  Klass* klass = array->klass()->klass_part();
  int    lenOffs = klass->non_indexable_size();

  // get size
  SAPReg* res = new SAPReg(_scope);
  _gen->append(NodeFactory::new_LoadOffsetNode(res, array->preg(), lenOffs, true));
  return new KlassExpr(smiKlassObj, res, _gen->_current);
}


Expr* PrimInliner::array_at_ifFail(ArrayAtNode::AccessType access_type) {
  assert_failure_block();
  assert_receiver();

  // parameters & result registers
  Expr*   array    = parameter(0);
  Expr*   index    = parameter(1);
  SAPReg* resPReg  = new SAPReg(_scope);	// holds the result if primitive didn't fail
  SAPReg* errPReg  = new SAPReg(_scope);	// holds the error message if primitive failed
  Klass*  klass    = array->klass()->klass_part();
  int     lenOffs  = klass->non_indexable_size();
  int     arrOffs  = lenOffs + 1;
  
  // at node
  ArrayAtNode* at = NodeFactory::new_ArrayAtNode(
    access_type, array->preg(), index->preg(), index->is_smi(), resPReg, errPReg, arrOffs, lenOffs
  );
  _gen->append(at);

  // continuation
  Expr* resExpr;
  switch (access_type) {
    case ArrayAtNode::byte_at	    : // fall through
    case ArrayAtNode::double_byte_at: resExpr = new KlassExpr(Universe::smiKlassObj(), resPReg, at);		break;
    case ArrayAtNode::character_at  : resExpr = new KlassExpr(Universe::characterKlassObj(), resPReg, at);	break;
    case ArrayAtNode::object_at	    : resExpr = new UnknownExpr(resPReg, at);					break;
    default			    : ShouldNotReachHere();
  }
  if (at->canFail()) {
    if (shouldUseUncommonTrap()) {
      // uncommon branch instead of failure code
      at->append(1, NodeFactory::new_UncommonNode(_gen->copyCurrentExprStack(), _bci /*_failure_block->begin_bci()*/));
    } else {
      // append failure code
      NopNode* err = NodeFactory::new_NopNode();
      at->append(1, err);
      Expr* errExpr = new KlassExpr(Universe::symbolKlassObj(), errPReg, at);
      resExpr = merge_failure_block(at, resExpr, err, errExpr, false);
    }
  }
  return resExpr;
}


Expr* PrimInliner::array_at_put_ifFail(ArrayAtPutNode::AccessType access_type) {
  assert_failure_block();
  assert_receiver();

  // parameters & result registers
  Expr*   array      = parameter(0);
  Expr*   index      = parameter(1);
  Expr*   element    = parameter(2);
  SAPReg* resPReg    = new SAPReg(_scope);	// holds the result if primitive didn't fail
  SAPReg* errPReg    = new SAPReg(_scope);	// holds the error message if primitive failed
  Klass*  klass      = array->klass()->klass_part();
  int     lenOffs    = klass->non_indexable_size();
  int     arrOffs    = lenOffs + 1;
  bool    storeCheck = (access_type == ArrayAtPutNode::object_at_put) && element->needsStoreCheck();
  
  if (access_type == ArrayAtPutNode::object_at_put) {
    // make sure blocks stored into array are created
    _gen->materialize(element->preg(), NULL);
  }

  // atPut node
  ArrayAtPutNode* atPut = NodeFactory::new_ArrayAtPutNode(
    access_type, array->preg(), index->preg(), index->is_smi(), element->preg(), element->is_smi(), NULL, errPReg, arrOffs, lenOffs, storeCheck
  );
  _gen->append(atPut);

  // continuation
  Expr* resExpr = array;
  if (atPut->canFail()) {
    if (shouldUseUncommonTrap()) {
      // uncommon branch instead of failure code
      atPut->append(1, NodeFactory::new_UncommonNode(_gen->copyCurrentExprStack(), _bci /*_failure_block->begin_bci()*/));
    } else {
      // append failure code
      NopNode* err = NodeFactory::new_NopNode();
      atPut->append(1, err);
      Expr* errExpr = new KlassExpr(Universe::symbolKlassObj(), errPReg, atPut);
      resExpr = merge_failure_block(atPut, resExpr, err, errExpr, true);
    }
  }
  return resExpr;
}


Expr* PrimInliner::obj_new() {
  // replace generic allocation primitive by size-specific primitive, if possible
  Expr* rcvr = parameter(0);
  if (!rcvr->isConstantExpr() || !rcvr->constant()->is_klass()) return NULL;
  Klass* klass = klassOop(rcvr->constant())->klass_part();	// class being instantiated
  if (klass->oop_is_indexable()) return NULL;			// would fail (extremely unlikely)
  int size = klass->non_indexable_size();			// size in words

  if (klass->can_inline_allocation()) {
    // These special compiler primitives only work for memOop klasses
    int number_of_instance_variables = size - memOopDesc::header_size();
    switch (number_of_instance_variables) {
      case 0: _pdesc = primitives::new0(); break;
      case 1: _pdesc = primitives::new1(); break;
      case 2: _pdesc = primitives::new2(); break;
      case 3: _pdesc = primitives::new3(); break;
      case 4: _pdesc = primitives::new4(); break;
      case 5: _pdesc = primitives::new5(); break;
      case 6: _pdesc = primitives::new6(); break;
      case 7: _pdesc = primitives::new7(); break;
      case 8: _pdesc = primitives::new8(); break;
      case 9: _pdesc = primitives::new9(); break;
      default:  ; // use generic primitives
    }
  }
  Expr* u = genCall(false);
  return new KlassExpr(klass->as_klassOop(), u->preg(), u->node());
}


Expr* PrimInliner::obj_shallowCopy() {
  // temporary hack, fix when prims have type info
  // Fix this Robert, 10/24-95, Lars
  Expr* u = genCall(false);
  assert(u->isUnknownExpr(), "oops");
  return new KlassExpr(parameter(0)->klass(), u->preg(), u->node());
}


Expr* PrimInliner::obj_equal() {
  assert_no_failure_block();

  // parameters & result registers
  PReg* arg1 = parameter(0)->preg();
  PReg* arg2 = parameter(1)->preg();
  // comparison
  _gen->append(NodeFactory::new_ArithRRNode(new NoPReg(_scope), arg1, CmpArithOp, arg2));
  SAPReg* resPReg = new SAPReg(_scope);
  return generate_cond(EQBranchOp, _gen, resPReg);
}


Expr* PrimInliner::obj_class(bool has_receiver) {
  assert_no_failure_block();
  if (has_receiver) assert_receiver();

  Expr*   obj     = parameter(0);
  if (obj->hasKlass()) {
    // constant-fold it
    klassOop k = obj->klass();
    return new ConstantExpr(k, new_ConstPReg(_scope, k), NULL);
  } else {
    SAPReg* resPReg = new SAPReg(_scope);
    InlinedPrimitiveNode* n = NodeFactory::new_InlinedPrimitiveNode(
      InlinedPrimitiveNode::obj_klass, resPReg, NULL, obj->preg()
    );
    _gen->append(n);
    // don't know exactly what it is - just use PIC info
    return new UnknownExpr(resPReg, n);
  }
}


Expr* PrimInliner::obj_hash(bool has_receiver) {
  assert_no_failure_block();
  if (has_receiver) assert_receiver();

  Expr* obj = parameter(0);
  if (obj->is_smi()) {
    // hash value = self (no code necessary)
    return obj;
  } else {
    return NULL;
    //
    // code in x86_node.cpp not yet implemented - fix this at some point
    //
    // has value taken from header field
    SAPReg* resPReg = new SAPReg(_scope);
    InlinedPrimitiveNode* n = NodeFactory::new_InlinedPrimitiveNode(
      InlinedPrimitiveNode::obj_hash, resPReg, NULL, obj->preg()
    );
    _gen->append(n);
    return new KlassExpr(Universe::smiKlassObj(), resPReg, n);
  }
}


Expr* PrimInliner::proxy_byte_at() {
  assert_failure_block();
  assert_receiver();

  // parameters & result registers
  Expr*   proxy     = parameter(0);
  Expr*   index     = parameter(1);
  SAPReg* resPReg   = new SAPReg(_scope);	// holds the result if primitive didn't fail
  SAPReg* errPReg   = new SAPReg(_scope);	// holds the error message if primitive failed
  
  // at node
  InlinedPrimitiveNode* at = NodeFactory::new_InlinedPrimitiveNode(
    InlinedPrimitiveNode::proxy_byte_at, resPReg, errPReg, proxy->preg(), index->preg(), index->is_smi()
  );
  _gen->append(at);

  // continuation
  Expr* resExpr = new KlassExpr(Universe::smiKlassObj(), resPReg, at);
  if (at->canFail()) {
    if (shouldUseUncommonTrap()) {
      // uncommon branch instead of failure code
      at->append(1, NodeFactory::new_UncommonNode(_gen->copyCurrentExprStack(), _bci /*_failure_block->begin_bci()*/));
    } else {
      // append failure code
      NopNode* err = NodeFactory::new_NopNode();
      at->append(1, err);
      Expr* errExpr = new KlassExpr(Universe::symbolKlassObj(), errPReg, at);
      resExpr = merge_failure_block(at, resExpr, err, errExpr, false);
    }
  }
  return resExpr;
}


Expr* PrimInliner::proxy_byte_at_put() {
  assert_failure_block();
  assert_receiver();

  // parameters & result registers
  Expr*   proxy     = parameter(0);
  Expr*   index     = parameter(1);
  Expr*   value     = parameter(2);
  SAPReg* errPReg   = new SAPReg(_scope);	// holds the error message if primitive failed
  
  // atPut node
  InlinedPrimitiveNode* atPut = NodeFactory::new_InlinedPrimitiveNode(
    InlinedPrimitiveNode::proxy_byte_at_put, NULL, errPReg, proxy->preg(), index->preg(), index->is_smi(), value->preg(), value->is_smi()
  );
  _gen->append(atPut);

  // continuation
  Expr* resExpr = proxy;
  if (atPut->canFail()) {
    if (shouldUseUncommonTrap()) {
      // uncommon branch instead of failure code
      atPut->append(1, NodeFactory::new_UncommonNode(_gen->copyCurrentExprStack(), _bci /*_failure_block->begin_bci()*/));
    } else {
      // append failure code
      NopNode* err = NodeFactory::new_NopNode();
      atPut->append(1, err);
      Expr* errExpr = new KlassExpr(Universe::symbolKlassObj(), errPReg, atPut);
      resExpr = merge_failure_block(atPut, resExpr, err, errExpr, true);
    }
  }
  return resExpr;
}


Expr* PrimInliner::block_primitiveValue() {
  PReg* r = parameter(0)->preg();
  if (r->isBlockPReg()) {
    // we know the identity of the block -- inline it if possible
    Inliner* inliner = _gen->inliner();
    SendInfo* info = new SendInfo(_scope, parameter(0), _pdesc->selector());
    Expr* res = inliner->inlineBlockInvocation(info);
    if (res) return res;
  } 
  // could at least inline block invocation -- fix this later
  return NULL;
}


Expr* PrimInliner::tryInline() {
  // Returns the failure result or the result of the primitive (if the
  // primitive can't fail) if the primitive has been inlined; returns
  // NULL otherwise. If the primitive has been inlined but can't fail,
  // the bci in the MethodDecoder is set to the first instruction after
  // the failure block.
  // NB: The comparisons below should be replaced with pointer comparisons
  // comparing with the appropriate vmSymbol. Should fix this at some point.
  char* name  = _pdesc->name();
  Expr* res = NULL;
  switch (_pdesc->group()) {
    case IntArithmeticPrimitive:
      if (number_of_parameters() == 2) {
        Expr* x = parameter(0);
        Expr* y = parameter(1);
        if (equal(name, "primitiveAdd:ifFail:"))			{ res = smi_ArithmeticOp(tAddArithOp, x, y);	break; }
        if (equal(name, "primitiveSubtract:ifFail:"))			{ res = smi_ArithmeticOp(tSubArithOp, x, y);	break; }
        if (equal(name, "primitiveMultiply:ifFail:"))			{ res = smi_ArithmeticOp(tMulArithOp, x, y);	break; }
        if (equal(name, "primitiveDiv:ifFail:"))			{ res = smi_Div(x, y);				break; }
        if (equal(name, "primitiveMod:ifFail:"))			{ res = smi_Mod(x, y);				break; }
        if (equal(name, "primitiveBitAnd:ifFail:"))			{ res = smi_BitOp(tAndArithOp, x, y);		break; }
        if (equal(name, "primitiveBitOr:ifFail:"))			{ res = smi_BitOp(tOrArithOp , x, y);		break; }
        if (equal(name, "primitiveBitXor:ifFail:"))			{ res = smi_BitOp(tXOrArithOp, x, y);		break; }
        if (equal(name, "primitiveRawBitShift:ifFail:"))		{ res = smi_Shift(x, y);			break; }
      }
      break;
    case IntComparisonPrimitive:
      if (number_of_parameters() == 2) {
        Expr* x = parameter(0);
        Expr* y = parameter(1);
        if (equal(name, "primitiveSmallIntegerEqual:ifFail:"))		{ res = smi_Comparison(EQBranchOp, x, y);	break; }
        if (equal(name, "primitiveSmallIntegerNotEqual:ifFail:"))	{ res = smi_Comparison(NEBranchOp, x, y);	break; }
        if (equal(name, "primitiveLessThan:ifFail:"))			{ res = smi_Comparison(LTBranchOp, x, y);	break; }
        if (equal(name, "primitiveLessThanOrEqual:ifFail:"))		{ res = smi_Comparison(LEBranchOp, x, y);	break; }
        if (equal(name, "primitiveGreaterThan:ifFail:"))		{ res = smi_Comparison(GTBranchOp, x, y);	break; }
        if (equal(name, "primitiveGreaterThanOrEqual:ifFail:"))		{ res = smi_Comparison(GEBranchOp, x, y);	break; }
      }
      break;
    case FloatArithmeticPrimitive:
      break;
    case FloatComparisonPrimitive:
      break;
    case ObjArrayPrimitive:
      if (equal(name, "primitiveIndexedObjectSize"))			{ res = array_size();						break; }
      if (equal(name, "primitiveIndexedObjectAt:ifFail:"))		{ res = array_at_ifFail(ArrayAtNode::object_at);		break; }
      if (equal(name, "primitiveIndexedObjectAt:put:ifFail:"))		{ res = array_at_put_ifFail(ArrayAtPutNode::object_at_put);	break; }
      break;
    case ByteArrayPrimitive:
      if (equal(name, "primitiveIndexedByteSize"))			{ res = array_size();						break; }
      if (equal(name, "primitiveIndexedByteAt:ifFail:"))		{ res = array_at_ifFail(ArrayAtNode::byte_at);			break; }
      if (equal(name, "primitiveIndexedByteAt:put:ifFail:"))		{ res = array_at_put_ifFail(ArrayAtPutNode::byte_at_put);	break; }
      break;
    case DoubleByteArrayPrimitive:
      if (equal(name, "primitiveIndexedDoubleByteSize"))		{ res = array_size();						break; }
      if (equal(name, "primitiveIndexedDoubleByteAt:ifFail:"))		{ res = array_at_ifFail(ArrayAtNode::double_byte_at);		break; }
      if (equal(name, "primitiveIndexedDoubleByteCharacterAt:ifFail:"))	{ res = array_at_ifFail(ArrayAtNode::character_at);		break; }
      if (equal(name, "primitiveIndexedDoubleByteAt:put:ifFail:"))	{ res = array_at_put_ifFail(ArrayAtPutNode::double_byte_at_put);break; }
      break;
    case BlockPrimitive:
      if (strncmp(name, "primitiveValue", 14) == 0) 			{ res = block_primitiveValue();		break; }
      break;
    case NormalPrimitive:
      if (strncmp(name, "primitiveNew", 12) == 0) 			{ res = obj_new();			break; }
      if (equal(name, "primitiveShallowCopyIfFail:ifFail:"))		{ res = obj_shallowCopy();		break; }
      if (equal(name, "primitiveEqual:"))				{ res = obj_equal();			break; }
      if (equal(name, "primitiveClass"))				{ res = obj_class(true);		break; }
      if (equal(name, "primitiveClassOf:"))				{ res = obj_class(false);		break; }
      if (equal(name, "primitiveHash"))					{ res = obj_hash(true);			break; }
      if (equal(name, "primitiveHashOf:"))				{ res = obj_hash(false);		break; }
      if (equal(name, "primitiveProxyByteAt:ifFail:"))			{ res = proxy_byte_at();		break; }
      if (equal(name, "primitiveProxyByteAt:put:ifFail:"))		{ res = proxy_byte_at_put();		break; }
      break;
   default:
      fatal1("bad primitive group %d", _pdesc->group());
      break;
  }
 
  if (CompilerDebug) {
    cout(PrintInlining && (res != NULL))->print("%*sinlining %s %s\n", _scope->depth + 2, "", _pdesc->name(),
						_usingUncommonTrap ? "(unc. failure)" : (_cannotFail ? "(cannot fail)" :  ""));
  }
  if (!_usingUncommonTrap && !_cannotFail) theCompiler->reporter->report_prim_failure(_pdesc);
  return res;
}


PrimInliner::PrimInliner(NodeBuilder* gen, primitive_desc* pdesc, MethodInterval* failure_block) {
  assert(pdesc, "must have a primitive desc to inline");
  _gen		= gen;
  _pdesc	= pdesc;
  _failure_block= failure_block;

  _scope	= gen->_scope;
  _bci		= gen->bci();	  // bci of primitive call
  _exprStack	= gen->_exprStack;
  _cannotFail   = true;
  _params	= new GrowableArray<Expr*>(number_of_parameters(), number_of_parameters(), NULL);
  _usingUncommonTrap = false;

  // get parameters
  int i = number_of_parameters();
  int first = _exprStack->length() - i;
  while (i-- > 0) {
    _params->at_put(i, _exprStack->at(first + i));
  }

  assert(_pdesc->can_fail() == (failure_block != NULL), "primitive desc & primitive usage inconsistent");
}


void PrimInliner::generate() {
  Expr* res = NULL;
  if (InlinePrims) { 
    if (ConstantFoldPrims) { res = tryConstantFold(); }
    if (res == NULL) 	   { res = tryTypeCheck(); }
    if (res == NULL) 	   { res = tryInline(); }
  }
  if (res == NULL) {
    // primitive has not been constant-folded nor inlined
    // -> must call primitive at run-time
    res = genCall(_pdesc->can_fail());
  }
  assert(res, "must have result expr");
  _exprStack->pop(_pdesc->number_of_parameters());	// pop parameters
  _exprStack->push2nd(res, _scope, _bci);		// push primitive result
}


Expr* PrimInliner::genCall(bool canFail) {
  // standard primitive call
  //
  // Note: If a primitive fails, it will return a marked symbol. One has to make sure that
  //       this marked symbol is *never* stored on the stack but only kept in registers
  //       (otherwise GC won't work correctly). Here this is established by using the dst()
  //       of a pcall only which is pre-allocated to the result_reg.

  MergeNode*		nlrTestPoint	= _pdesc->can_perform_NLR() ? _scope->nlrTestPoint() : NULL;
  GrowableArray<PReg*>*	args		= _gen->pass_arguments(NULL, _pdesc->number_of_parameters());
  GrowableArray<PReg*>*	exprStack	= _pdesc->can_walk_stack() ? _gen->copyCurrentExprStack() : NULL;
  PrimNode*		pcall		= NodeFactory::new_PrimNode(_pdesc, nlrTestPoint, args, exprStack);

  _gen->append(pcall);

  BranchNode* branch = NULL;
  if (_failure_block != NULL && canFail) {
    // primitive with failure block; failed if Mark_Tag_Bit is set in result -> add a branch node here
    _gen->append(NodeFactory::new_ArithRCNode(new NoPReg(_scope), pcall->dest(), TestArithOp, Mark_Tag_Bit)); 
    branch = NodeFactory::new_BranchNode(NEBranchOp);
    _gen->append(branch);
  }

  // primitive didn't fail (with or without failure block) -> assign to resPReg & determine its expression
  SAPReg* resPReg = new SAPReg(_scope);
  _gen->append(NodeFactory::new_AssignNode(pcall->dst(), resPReg));
  Node* ok_exit = _gen->_current;
  Expr* resExpr = _pdesc->return_klass(resPReg, ok_exit);
  if (resExpr == NULL) resExpr = new UnknownExpr(resPReg, ok_exit);

  if (branch != NULL) {
    // add failure block if primitive can fail -> reset Mark_Tag_Bit first
    SAPReg* errPReg = new SAPReg(_scope);
    ArithRCNode* failure_exit = NodeFactory::new_ArithRCNode(errPReg, pcall->dst(), AndArithOp, ~Mark_Tag_Bit);
    branch->append(1, failure_exit);
    resExpr = merge_failure_block(ok_exit, resExpr, failure_exit, new KlassExpr(Universe::symbolKlassObj(), errPReg, failure_exit), false);
  }

  return resExpr;
}


void PrimInliner::print() {
  std->print("a PrimInliner\n");
}


# endif
