/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.77 $ */
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
# include "incls/_nodeBuilder.cpp.incl"

// Class variables

Node* NodeBuilder::EndOfCode = (Node*)-1;


// Initialization

NodeBuilder::NodeBuilder() {
  _exprStack = NULL;
  _inliner   = NULL;
  _scope     = NULL;
  _current   = NULL;
}


void NodeBuilder::initialize(InlinedScope* scope) {
  _exprStack = new ExprStack(scope, 8);
  _inliner   = new Inliner(scope);
  _scope     = scope;
  _current   = NULL;
}


// Helper functions for node creation/concatenation

void NodeBuilder::append(Node* node) { 
  if (node->isExitNode()) warning("should use append_exit for consistency");
  if (aborting() || _current == EndOfCode) {
    if (node->nPredecessors() == 0) {
      // ignore this code 
      // (sometimes, the bytecode compiler generates dead code after returns)
    } else {
      // node is already connected to graph (merge node), so restart code generation 
      _current = node;
    }
  } else {
    _current = _current->append(node); 
  }
}


void NodeBuilder::append_exit(Node* node) { 
  assert(node->isExitNode(), "not an exit node");
  if (_current != EndOfCode) _current->append(node);
  _current = NULL;
}


void NodeBuilder::branch(MergeNode* target) {
  // connect current with target
  if (_current != NULL && _current != EndOfCode && _current != target) {
    _current->append(target);
  }
  _current = target;
}


void NodeBuilder::comment(char* s) {
  if (CompilerDebug) append(NodeFactory::new_CommentNode(s));
}


GrowableArray<PReg*>* NodeBuilder::copyCurrentExprStack() {
  int l = exprStack()->length();
  GrowableArray<PReg*>* es = new GrowableArray<PReg*>(l);
  for (int i = 0; i < l; i++) {
    es->push(exprStack()->at(i)->preg());
  }
  return es;
}


// Node creation

void NodeBuilder::abort() {
  MethodClosure::abort();
  _current = EndOfCode;
}

bool NodeBuilder::abortIfDead(Expr* e) {
  if (e->isNoResultExpr()) {
    // dead code
    abort();
    return true;
  } else {
    return false;
  }
}

// Caution: do not use MethodIterator directly, always go through this function.
// Otherwise, things will break in the presence of dead code.
// Also, after calling generate_subinterval, make sure you set current (since
// the subinterval may have ended dead, leaving current at EndOfCode).
void NodeBuilder::generate_subinterval(MethodInterval* m, bool produces_result) {
  assert(!aborting(), "shouldn't generate when already aborting");
  int savedLen = exprStack()->length();
  MethodIterator mi(m, this);
  if (aborting()) {
    // the subinterval ended with dead code
    Expr* res = exprStack()->isEmpty() ? NULL : exprStack()->top();
    assert(res == NULL || res->isNoResultExpr(), "expected no result");
    un_abort();	  // abort will be done by caller (if needed)
    int diff = exprStack()->length() - savedLen;
    if (produces_result) diff--;
    if (diff > 0) {
      // adjust expression stack top pop extra stuff
      exprStack()->pop(diff);
    }
    if (produces_result) {
      if (res == NULL) {
        // subinterval should have produced a result but didn't
        // This is legal: e.g., in "someCond or: [ otherCond ifTrue: [^ false ]. true]" the subinterval
        // of or: doesn't return a result if otherCond is known to be true.   So, just fix it here.
        // Urs 9/6/96
        res = new NoResultExpr();
        exprStack()->push(res, scope(), m->end_bci());
      } else {
        exprStack()->assign_top(res);
      }
    }
  }
}


void NodeBuilder::constant_if_node(IfNode* node, ConstantExpr* cond) {
  // if statement with constant condition
  oop c = cond->constant();
  int resultBCI = node->begin_bci();
  if (c == trueObj || c == falseObj) {
    if (node->is_ifTrue() == (c == trueObj)) {
      // then branch is always taken
      generate_subinterval(node->then_code(), node->produces_result());
    } else if (node->else_code() != NULL) {
      // else branch always taken
      generate_subinterval(node->else_code(), node->produces_result());
    } else {
      // branch is never executed -> ignore
    }
    if (node->produces_result()) {
      Expr* res = exprStack()->top();
      // materialize(res, NULL);
      // NB: need not materialize block because it isn't merged with else branch
      // (BlockPReg will be materialized when assigned)
      scope()->setExprForBCI(resultBCI, res);	  // for debugging info
      assert(!res->isNoResultExpr() || is_in_dead_code(), "no result should imply in_dead_code");
      abortIfDead(res);				  // constant if ends dead, so method ends dead
    } else {
      if (is_in_dead_code()) abort();		  // constant if ends dead, so method ends dead
    }
  } else {
    // non-boolean condition -> fails always
    append(NodeFactory::new_UncommonNode(copyCurrentExprStack(), resultBCI));
  }
}


TypeTestNode* NodeBuilder::makeTestNode(bool cond, PReg* r) {
  GrowableArray<klassOop>* list = new GrowableArray<klassOop>(2);
  if (cond) {
    list->append(trueObj->klass());
    list->append(falseObj->klass());
  } else {
    list->append(falseObj->klass());
    list->append(trueObj->klass());
  }
  TypeTestNode* test = NodeFactory::new_TypeTestNode(r, list, true);
  test->append(0, NodeFactory::new_UncommonNode(copyCurrentExprStack(), bci()));
  return test;
}


void NodeBuilder::if_node(IfNode* node) {
  Expr* cond = exprStack()->pop();
  int resultBCI = node->begin_bci();
  if (abortIfDead(cond)) {
    if (node->produces_result()) exprStack()->push(cond, scope(), resultBCI);
    return;
  }
  if (cond->isConstantExpr()) {
    constant_if_node(node, (ConstantExpr*)cond);
  } else {
    // non-constant condition
    TypeTestNode* test = makeTestNode(node->is_ifTrue(), cond->preg());
    append(test);
    if (node->else_code() != NULL) {
      // with else branch
      Expr*      ifResult   = NULL;
      Expr*      elseResult = NULL;
      SAPReg*    resultReg  = new SAPReg(scope());
      MergeNode* ifBranch   = NodeFactory::new_MergeNode(node->then_code()->begin_bci());
      MergeNode* elseBranch = NodeFactory::new_MergeNode(node->else_code()->begin_bci());
      MergeNode* endOfIf    = NodeFactory::new_MergeNode(node->end_bci());
      test->append(1, ifBranch);
      test->append(2, elseBranch);
      splitMergeExpr(cond, test);
      // then branch
      setCurrent(ifBranch);
      generate_subinterval(node->then_code(), node->produces_result());
      bool ifEndsDead = is_in_dead_code();
      if (node->produces_result()) {
        ifResult = exprStack()->pop();
        if (!ifResult->isNoResultExpr()) {
          // NB: must materialize block because it is merged with result of else branch
          // (was bug 7/4/96 -Urs)
          materialize(ifResult->preg(), NULL);
          append(NodeFactory::new_AssignNode(ifResult->preg(), resultReg));
        } else {
          assert(ifEndsDead, "oops");
        }
      }
      append(endOfIf);
      // else branch
      setCurrent(elseBranch);
      generate_subinterval(node->else_code(), node->produces_result());
      bool elseEndsDead = is_in_dead_code();
      if (node->produces_result()) {
        elseResult = exprStack()->pop();
        if (!elseResult->isNoResultExpr()) {
          materialize(elseResult->preg(), NULL);
          append(NodeFactory::new_AssignNode(elseResult->preg(), resultReg));
        } else {
          assert(elseEndsDead, "oops");
        }
      }
      append(endOfIf);
      setCurrent(endOfIf);
      // end
      if (ifEndsDead && elseEndsDead) {
        abort();      // both branches end dead, so containing interval ends dead
      } else {
        if (node->produces_result()) {
          exprStack()->push2nd(new MergeExpr(ifResult, elseResult, resultReg, current()), scope(), resultBCI);
        }
      }
    } else {
      // no else branch
      assert(!node->produces_result(), "inconsistency - else branch required");
      MergeNode* ifBranch   = NodeFactory::new_MergeNode(node->then_code()->begin_bci());
      MergeNode* endOfIf    = NodeFactory::new_MergeNode(node->end_bci());
      test->append(1, ifBranch);
      test->append(2, endOfIf);
      splitMergeExpr(cond, test);
      // then branch
      setCurrent(ifBranch);
      generate_subinterval(node->then_code(), false);
      append(endOfIf);
      setCurrent(endOfIf);
      if (node->produces_result()) {
        // materialize(exprStack()->top(), NULL);
        // noo need to materialize result (see commment in constant_if_code)
        scope()->setExprForBCI(resultBCI, exprStack()->top());	  // for debugging info
      }
    }
    comment("end of if");
  }
}


void NodeBuilder::cond_node(CondNode* node) {
  Expr* cond = exprStack()->pop();
  int resultBCI = node->begin_bci();
  if (abortIfDead(cond)) {
    exprStack()->push(cond, scope(), resultBCI);
    return;	    // condition ends dead, so rest of code does, too
  }
  if (cond->isConstantExpr()) {
    // constant condition
    oop c = cond->asConstantExpr()->constant();
    if (c == trueObj || c == falseObj) {
      if (node->is_and() && c == trueObj ||
        node->is_or()  && c == falseObj) {
          generate_subinterval(node->expr_code(), true);
          // result of and:/or: is result of 2nd expression
          Expr* res = exprStack()->top();
          scope()->setExprForBCI(resultBCI, res);	  // for debugging info
          if (abortIfDead(res)) return;		  // 2nd expr never returns, so rest is dead
      } else {
        // don't need to evaluate 2nd expression, result is cond
        exprStack()->push(cond, scope(), resultBCI);
      }
    } else {
      // non-boolean condition -> fails always
      append(NodeFactory::new_UncommonNode(copyCurrentExprStack(), resultBCI));
    }
  } else {
    // non-constant condition
    SAPReg* resultReg  = new SAPReg(scope());
    TypeTestNode* test = makeTestNode(node->is_and(), cond->preg());
    append(test);
    MergeNode* condExpr   = NodeFactory::new_MergeNode(node->expr_code()->begin_bci());
    MergeNode* otherwise  = NodeFactory::new_MergeNode(node->expr_code()->begin_bci());
    MergeNode* endOfCond  = NodeFactory::new_MergeNode(node->end_bci());
    test->append(1, condExpr);
    test->append(2, otherwise);
    setCurrent(otherwise);
    append(NodeFactory::new_AssignNode(cond->preg(), resultReg));
    append(endOfCond);
    splitMergeExpr(cond, test);	    // split on result of first expression
    // evaluate second conditional expression
    setCurrent(condExpr);
    generate_subinterval(node->expr_code(), true);
    Expr* condResult = exprStack()->pop();
    if (condResult->isNoResultExpr()) {
      exprStack()->push2nd(new NoResultExpr, scope(), resultBCI);	// dead code
      setCurrent(endOfCond);
    } else {
      append(NodeFactory::new_AssignNode(condResult->preg(), resultReg));
      append(endOfCond);
      setCurrent(endOfCond);
      // end
      // The result of the conditional is *one* branch of the first expression (cond) plus
      // the result of the second expression.  (Was splitting bug -- Urs 4/96)
      Expr* exclude = cond->findKlass(node->is_or() ? falseObj->klass() : trueObj->klass());
      Expr* first;
      if (exclude) {
        first = cond->copyWithout(exclude);
      } else {
        first = cond;
      }
      exprStack()->push2nd(new MergeExpr(first, condResult, resultReg, current()), scope(), resultBCI);
      comment("end of cond");
    }
  }
}


void NodeBuilder::while_node(WhileNode* node) {
  int loop_bci = node->body_code() != NULL ? node->body_code()->begin_bci() : node->expr_code()->begin_bci();
  CompiledLoop* wloop = _scope->addLoop();
  LoopHeaderNode* header = NodeFactory::new_LoopHeaderNode();
  append(header);
  wloop->set_startOfLoop(header);
  MergeNode* loop = NodeFactory::new_MergeNode(loop_bci);
  MergeNode* exit = NodeFactory::new_MergeNode(node->end_bci());
  MergeNode* entry= NULL;	// entry point into loop (start of condition)
  exit->isLoopEnd = true;
  if (node->body_code() != NULL) {
    // set up entry point
    entry = NodeFactory::new_MergeNode(node->expr_code()->begin_bci());
    append(entry);
    entry->isLoopStart = true;
  } else {
    // no body code -> start with conditional expression
    append(loop);
    loop->isLoopStart = true;
  }
  wloop->set_startOfCond(current());
  generate_subinterval(node->expr_code(), true);
  Expr* cond = exprStack()->pop();
  if (abortIfDead(cond)) {
    return;	  // condition ends dead --> loop is never executed
  }
  if (false && cond->isConstantExpr()) {
    wloop->set_endOfCond(current());
    // ^^^ TURNED OFF - doesn't work for endless loops yet - FIX THIS
    // constant condition
    oop c = cond->asConstantExpr()->constant();
    if (c == trueObj || c == falseObj) {
      if (node->is_whileTrue() == (c == trueObj)) {
        append(loop);
        setCurrent(exit);
      }
    } else {
      // non-boolean condition -> fails always
      append(NodeFactory::new_UncommonNode(copyCurrentExprStack(), bci()));
    }
  } else {
    // non-constant condition
    TypeTestNode* test = makeTestNode(node->is_whileTrue(), cond->preg());
    append(test);
    wloop->set_endOfCond(test);
    test->append(1, loop);
    test->append(2, exit);
    setCurrent(exit);
    splitMergeExpr(cond, test);
    comment("end of while");
  }

  if (node->body_code() != NULL) {
    // generate loop body 
    // NB: *must* be done after generating condition, because Node order must correspond to
    // bci order, otherwise copy propagation breaks  -Urs 10/95
    Node* curr = current();
    setCurrent(loop);
    generate_subinterval(node->body_code(), false);
    wloop->set_startOfBody(loop->next());   // don't use loop (MergeNode) as start since it's been 
    // created too early (before loop cond) --> node id range is off
    if (theCompiler->is_uncommon_compile()) {
      // Make sure the invocation counter is incremented at least once per iteration; otherwise uncommon 
      // nmethods containing loops but no sends won't be recompiled early enough. 
      append(NodeFactory::new_FixedCodeNode(FixedCodeNode::inc_counter));
    }
    wloop->set_endOfBody(current());
    append(entry);
    setCurrent(curr);
  }
  wloop->set_endOfLoop(exit);
}


void NodeBuilder::primitive_call_node(PrimitiveCallNode* node) {
  if (node->pdesc() == NULL) {
    error("calling unknown primitive");
    exprStack()->pop();
    return;
  } 
  PrimInliner* p = new PrimInliner(this, node->pdesc(), node->failure_code());
  p->generate();
}


void NodeBuilder::dll_call_node(DLLCallNode* node) {
  GrowableArray<PReg*>* args = pass_arguments(NULL, node->nofArgs());
  assert(node->failure_code() == NULL, "compiler cannot handle DLL calls with failure blocks yet");
  DLLNode* dcall = NodeFactory::new_DLLNode(node->dll_name(), node->function_name(), node->function(), node->async(), scope()->nlrTestPoint(), args, copyCurrentExprStack());
  append(dcall);
  exprStack()->pop(node->nofArgs());
  // a proxy object has been pushed before the arguments, assign result
  // to its _pointer field - the proxy is also the result of the dll call
  // (since the result is NOT a Delta pointer, no store check is needed)
  append(NodeFactory::new_StoreOffsetNode(dcall->dest(), exprStack()->top()->preg(), pointer_no, false));
}


void NodeBuilder::allocate_temporaries(int nofTemps) {
  methodOop m = _scope->method();
  assert(1 + nofTemps == m->number_of_stack_temporaries(), "no. of stack variables inconsistent");
  // temporaries are allocated in the beginning (InlinedScope::createTemporaries)
}


void NodeBuilder::push_self() {
  exprStack()->push(scope()->self(), scope(), scope()->bci());
}


void NodeBuilder::push_tos() {
  exprStack()->push(exprStack()->top(), scope(), scope()->bci());
}


void NodeBuilder::push_literal(oop obj) {
  exprStack()->push(new ConstantExpr(obj, new_ConstPReg(_scope, obj), NULL), scope(), scope()->bci());
}


void NodeBuilder::push_argument(int no) {
  // arguments are non-assignable, so no assignment to SAPReg necessary
  assert((0 <= no) && (no < _scope->nofArguments()), "illegal argument no");
  exprStack()->push(scope()->argument(no), scope(), scope()->bci());
}


void NodeBuilder::push_temporary(int no) {
  // must assign non-parameters to temporary because exprStack entries must be singly-assigned
  // and because source (i.e., temporary location) could be assigned between now and the send
  // that actually consumes the value
  assert((0 <= no) && (no < _scope->nofTemporaries()), "illegal temporary no");
  Expr* temp = _scope->temporary(no);
  PReg* src = temp->preg();
  SAPReg* dst = new SAPReg(_scope);
  append(NodeFactory::new_AssignNode(src, dst));
  exprStack()->push(temp, scope(), scope()->bci());
}


void NodeBuilder::access_temporary(int no, int context, bool push) {
  // generates code to access temporary no in (logical, i.e., interpreter) context
  // context numbering starts a 0
  //assert(_scope->allocatesInterpretedContext() == (_contextInitializer != NULL), "context must exist already if used (find_scope)");
  assert(context >= 0, "context must be >= 0");
  int nofIndirections;
  OutlinedScope* out = NULL;
  InlinedScope* s = _scope->find_scope(context, nofIndirections, out);
  if (nofIndirections < 0) {
    // the accessed variable is in the same stack frame, in scope s
    if (push) {
      Expr* contextTemp = s->contextTemporary(no);
      PReg* src = contextTemp->preg();
      Expr* res;
      if (src->isBlockPReg()) {
        // don't copy around blocks; see ContextInitializeNode et al.
        res = contextTemp;
      } else {
        SAPReg* dst = new SAPReg(_scope);
        append(NodeFactory::new_AssignNode(src, dst));
        res = s->contextTemporary(no)->shallowCopy(dst, _current);
      }
      exprStack()->push(res, scope(), scope()->bci());
    } else {
      // store
      PReg* src = exprStack()->top()->preg();
      materialize(src, NULL);
      PReg* dst = s->contextTemporary(no)->preg();
      append(NodeFactory::new_AssignNode(src, dst));
    }
  } else {
    // the accessed variable is in higher stack frames
    assert(out, "must be set");
    assert(out->scope()->allocates_compiled_context(), "must allocate context");
    NameDesc* nd = out->scope()->contextTemporary(no);
    Location loc = nd->location();	// location of temp in compiled context
    assert(loc.isContextLocation(), "must be in context");
    int tempNo = loc.tempNo();		// compiled offset
    if (tempNo != no) {
      compiler_warning("first time this happens: compiled context offset != interpreted context offset");
    }
    if (push) {
      SAPReg* dst = new SAPReg(_scope);
      append(NodeFactory::new_LoadUplevelNode(dst, s->context(), nofIndirections, contextOopDesc::temp0_word_offset() + tempNo, NULL));
      exprStack()->push(new UnknownExpr(dst, _current), scope(), scope()->bci());
    } else {
      // store
      Expr* srcExpr = exprStack()->top();
      PReg* src = srcExpr->preg();
      materialize(src, NULL);
      append(NodeFactory::new_StoreUplevelNode(src, s->context(), nofIndirections, contextOopDesc::temp0_word_offset() + tempNo, NULL, srcExpr->needsStoreCheck()));
    }
  }
}


void NodeBuilder::push_temporary(int no, int context) {
  access_temporary(no, context, true);
}


void NodeBuilder::push_instVar(int offset) {
  assert(offset >= 0, "offset must be positive");
  SAPReg* dst = new SAPReg(_scope);
  PReg* base = _scope->self()->preg();
  append(NodeFactory::new_LoadOffsetNode(dst, base, offset, false));
  exprStack()->push(new UnknownExpr(dst, _current), scope(), scope()->bci());
}


void NodeBuilder::push_global(associationOop associationObj) {
  SAPReg* dst = new SAPReg(_scope);
  if (associationObj->is_constant()) {
    // constant association -- can inline the constant
    oop c = associationObj->value();
    ConstPReg* r = new_ConstPReg(_scope, c);
    exprStack()->push(new ConstantExpr(c, r, NULL), _scope, scope()->bci());
  } else {
    // Removed by Lars Bak, 4-22-96
    // if (associationObj->value()->is_klass()) 
    //    compiler_warning("potential performance bug: non-constant association containing klassOop");
    ConstPReg* base = new_ConstPReg(_scope, associationObj);
    append(NodeFactory::new_LoadOffsetNode(dst, base, associationOopDesc::value_offset(), false));
    exprStack()->push(new UnknownExpr(dst, _current), _scope, scope()->bci());
  }
}


void NodeBuilder::store_temporary(int no) {
  PReg* src = exprStack()->top()->preg();
  // should check here whether src is memoized block pushed in preceding byte code;
  // if so, un-memoize it  -- fix this
  materialize(src, NULL);
  PReg* dst = _scope->temporary(no)->preg();
  if (src != dst) append(NodeFactory::new_AssignNode(src, dst));
}


void NodeBuilder::store_temporary(int no, int context) {
  access_temporary(no, context, false);
}


void NodeBuilder::store_instVar(int offset) {
  assert(offset >= 0, "offset must be positive");
  Expr* srcExpr = exprStack()->top();
  PReg* src = srcExpr->preg();
  materialize(src, NULL);
  PReg* base = _scope->self()->preg();
  append(NodeFactory::new_StoreOffsetNode(src, base, offset, srcExpr->needsStoreCheck()));
}


void NodeBuilder::store_global(associationOop associationObj) {
  Expr* srcExpr = exprStack()->top();
  PReg* src = srcExpr->preg();
  materialize(src, NULL);
  ConstPReg* base = new_ConstPReg(_scope, associationObj);
  append(NodeFactory::new_StoreOffsetNode(src, base, associationOopDesc::value_offset(), srcExpr->needsStoreCheck()));
}


void NodeBuilder::pop() {
  exprStack()->pop();
}


// always call materialize before storing or passing a run-time value that could be a block
void NodeBuilder::materialize(PReg* r, GrowableArray<BlockPReg*>* materialized) {
  // make sure the block (and all parent blocks / uplevel-accessed blocks) exist
  // materialized is a list of blocks already materialized (NULL if none)
  if (r->isBlockPReg() && (materialized == NULL || !materialized->contains((BlockPReg*)r))) {
    BlockPReg* blk = (BlockPReg*)r;
    append(NodeFactory::new_BlockMaterializeNode(blk, copyCurrentExprStack()));
    if (materialized == NULL) materialized = new GrowableArray<BlockPReg*>(5);
    materialized->append(blk);
    GrowableArray<PReg*>* reads = blk->uplevelRead();
    if (reads) {
      for (int i = reads->length() - 1; i >= 0; i--) materialize(reads->at(i), materialized);
    }
  }
}


GrowableArray<PReg*>* NodeBuilder::pass_arguments(PReg* receiver, int nofArgs) {
  // Generate code for argument passing (move all args into the right locations).
  // If the arguments are passed on the stack (including the receiver) they should
  // be assigned in the order of textual appearance. If the receiver is passed in
  // a register that should happen at the end to allow this register to be used
  // as long as possible.
  int nofFormals = (receiver == NULL) ? nofArgs : nofArgs + 1;
  GrowableArray<PReg*>* formals = new GrowableArray<PReg*>(nofFormals);
  // setup formal receiver and pass if passed on the stack
  SAPReg* formalReceiver;
  GrowableArray<BlockPReg*> blocks(5);
  if (receiver != NULL) {
    materialize(receiver, &blocks);
    formalReceiver = new SAPReg(_scope, receiverLoc, false, false, bci(), bci());
    formals->append(formalReceiver);
    if (receiverLoc.isStackLocation()) {
      // receiver is passed on stack, must happen before the arguments are passed
      append(NodeFactory::new_AssignNode(receiver, formalReceiver));
    }
  }
  // argument range
  const int first_arg = exprStack()->length() - nofArgs;
  const int limit_arg = exprStack()->length();
  int sp;
  // materialize blocks
  for (sp = first_arg; sp < limit_arg; sp++)  {
    PReg* actual = exprStack()->at(sp)->preg();
    materialize(actual, &blocks);
  }
  // pass arguments
  for (sp = first_arg; sp < limit_arg; sp++)  {
    PReg*   actual = exprStack()->at(sp)->preg();
    SAPReg* formal = new SAPReg(_scope, Mapping::outgoingArg(sp - first_arg, nofArgs), false, false, bci(), bci());
    formals->append(formal);
    append(NodeFactory::new_AssignNode(actual, formal));
  }
  // pass receiver if not passed already
  if ((receiver != NULL) && !receiverLoc.isStackLocation()) {
    append(NodeFactory::new_AssignNode(receiver, formalReceiver));
  }
  return formals;
}


void NodeBuilder::gen_normal_send(SendInfo* info, int nofArgs, SAPReg* result) {
  GrowableArray<PReg*>* args = pass_arguments(exprStack()->at(exprStack()->length() - nofArgs - 1)->preg(), nofArgs);
  SendNode* send = NodeFactory::new_SendNode(info->key, scope()->nlrTestPoint(), args, 
    copyCurrentExprStack(), false, info);
  append(send);
  append(NodeFactory::new_AssignNode(send->dest(), result));
}


void NodeBuilder::gen_self_send(SendInfo* info, int nofArgs, SAPReg* result) {
  GrowableArray<PReg*>* args = pass_arguments(_scope->self()->preg(), nofArgs);
  SendNode* send = NodeFactory::new_SendNode(info->key, scope()->nlrTestPoint(), args, 
    copyCurrentExprStack(), false, info);
  append(send);
  append(NodeFactory::new_AssignNode(send->dest(), result));
}


void NodeBuilder::gen_super_send(SendInfo* info, int nofArgs, SAPReg* result) {
  GrowableArray<PReg*>* args = pass_arguments(_scope->self()->preg(), nofArgs);
  SendNode* send = NodeFactory::new_SendNode(info->key, scope()->nlrTestPoint(), args, 
    copyCurrentExprStack(), true, info);
  append(send);
  append(NodeFactory::new_AssignNode(send->dest(), result));
}


void NodeBuilder::normal_send(InterpretedIC* ic) {
  int nofArgs = ic->selector()->number_of_arguments();
  LookupKey* key = LookupKey::allocate(NULL, ic->selector());
  Expr* rcvr = exprStack()->at(exprStack()->length() - nofArgs - 1);
  SendInfo* info = new SendInfo(_scope, key, rcvr);
  Expr* result = _inliner->inlineNormalSend(info);
  exprStack()->pop(nofArgs + 1); // pop arguments & receiver
  exprStack()->push(result, _scope, scope()->bci());
  abortIfDead(result);
}


void NodeBuilder::self_send(InterpretedIC* ic) {
  int nofArgs = ic->selector()->number_of_arguments();
  LookupKey* key = LookupKey::allocate(_scope->selfKlass(), ic->selector());
  SendInfo* info = new SendInfo(_scope, key, _scope->self());
  Expr* result = _inliner->inlineSelfSend(info);
  exprStack()->pop(nofArgs);	// receiver has not been pushed
  exprStack()->push(result, _scope, scope()->bci());
  abortIfDead(result);
}


void NodeBuilder::super_send(InterpretedIC* ic) {
  int nofArgs = ic->selector()->number_of_arguments();
  //LookupKey* key = ic->lookupKey(0);
  klassOop klass = _scope->selfKlass()->klass_part()->superKlass();
  LookupKey* key = LookupKey::allocate(klass, lookupCache::method_lookup(klass, ic->selector()));
  SendInfo* info = new SendInfo(_scope, key, _scope->self());
  Expr* result = _inliner->inlineSuperSend(info);
  exprStack()->pop(nofArgs);	// receiver has not been pushed
  exprStack()->push(result, _scope, scope()->bci());
  abortIfDead(result);
}


void NodeBuilder::double_equal() {
  PrimInliner* p = new PrimInliner(this, primitives::equal(), NULL);
  p->generate();
}


void NodeBuilder::double_not_equal() {
  PrimInliner* p = new PrimInliner(this, primitives::not_equal(), NULL);
  p->generate();
}


void NodeBuilder::method_return(int nofArgs) {
  // assign result & return
  Expr* result = exprStack()->pop();
  if (_current == EndOfCode) {
    // scope ends with dead code, i.e., there's no method return
    // (e.g., because there is a preceding NLR)
    _scope->addResult(new NoResultExpr);
    return;
  }

  if (result->isNoResultExpr()) {
    // scope will never return normally (i.e., is always left via a NLR)
    _scope->addResult(result);	// make sure scope->result != NULL
  } else {
    // return TOS
    PReg* src = result->preg();
    materialize(src, NULL);
    if (_scope->isTop()) {
      // real return from nmethod
      append(NodeFactory::new_AssignNode(src, _scope->resultPR));
      _scope->addResult(result->shallowCopy(_scope->resultPR, _current));
    } else {
      // inlined return
      if (UseNewBackend) {
        // The new backend doesn't support InlinedReturnNodes anymore but has
        // an explizit ContextZapNode that is used together with an AssignNode.
        //
        // Note: We don't know if context zapping is really needed before
        // the block optimizations, therefore we introduce it eagerly if
        // the scope seems to need it. However, the backend will check
        // again if it is still necessary (the optimizations can only
        // remove the need, not create it).

        // Turned off for now - problem with ScopeDesc - FIX THIS
        // if (_scope->needsContextZapping()) append(NodeFactory::new_ContextZapNode(_scope->context()));
        append(NodeFactory::new_AssignNode(src, _scope->resultPR));
      } else {
        if (_scope->needsContextZapping()) {
          // keep inlined return node (no context zap node yet)
          append(NodeFactory::new_InlinedReturnNode(bci(), src, _scope->resultPR));
        } else {
          // only assignment required
          append(NodeFactory::new_AssignNode(src, _scope->resultPR));
        }
      }
      _scope->addResult(result->shallowCopy(_scope->resultPR, _current));
    }
  }

  append(scope()->returnPoint());	// connect to return code
  // The byte code compiler might generate a pushNil to adjust the stack;
  // make sure that code is discarded.
  _current = EndOfCode;
}


void NodeBuilder::nonlocal_return(int nofArgs) {
  // assign result & return
  Expr* resultExpr = exprStack()->pop();
  PReg* src = resultExpr->preg();
  materialize(src, NULL);
  if (scope()->isTop()) {
    // real NLR from nmethod
    append(NodeFactory::new_AssignNode(src, scope()->resultPR));
    append(scope()->nlrPoint());
  } else {
    // NLR from inlined scope
    Scope* home = scope()->home();
    if (home->isInlinedScope()) {
      // the NLR target is in the same nmethod
      InlinedScope* h = (InlinedScope*)home;
      append(NodeFactory::new_AssignNode(src, h->resultPR));
      h->addResult(resultExpr->shallowCopy(h->resultPR, _current));
      // jump to home method's return point
      // BUG: should zap contexts along the way -- fix this
      append(h->returnPoint());
    } else {
      // NLR target is in a different nmethod -- unwind all sender frames and
      // perform a real NLR
      // BUG: should zap contexts along the way -- fix this

      // now assign to result register and jump to NLR setup code to set up remaining NLR regs
      // NB: each scope needs its own setup node because the home fp/id is different
      int endBCI = scope()->nlrPoint()->bci();
      bool haveSetupNode = scope()->nlrPoint()->next() != NULL;
      assert(!haveSetupNode || scope()->nlrPoint()->next()->isNLRSetupNode(), "expected setup node");
      PReg* res = haveSetupNode ? ((NonTrivialNode*)scope()->nlrPoint()->next())->src() : 
        new SAPReg(scope(), NLRResultLoc, true, true, bci(), endBCI);
      append(NodeFactory::new_AssignNode(src, res));
      append(scope()->nlrPoint());
      if (!haveSetupNode) {
        // lazily create setup code
        append_exit(NodeFactory::new_NLRSetupNode(res, endBCI));
      }
    }
  }
  // (the byte code compiler might generate a pushNil to adjust the stack)
  _current = EndOfCode;
}

GrowableArray<NonTrivialNode*>* NodeBuilder::nodesBetween(Node* from, Node* to) {
  // collect assignments along the path from --> to (for splitting)
  GrowableArray<NonTrivialNode*>* nodes = new GrowableArray<NonTrivialNode*>(5);
  Node* n = from;
  while (n != to) {
    if (!n->hasSingleSuccessor()) return NULL;   // can't copy both paths
    bool shouldCopy = n->shouldCopyWhenSplitting();
    bool ok = (n == from) ||
      shouldCopy ||
      n->isTrivial() ||
      n->isMergeNode();
    if (!ok) return NULL;			      // can't copy this node
    if (shouldCopy && n != from) nodes->append((NonTrivialNode*)n);
    n = n->next();
  }
  return nodes;
}

MergeNode* NodeBuilder::insertMergeBefore(Node* n) {
  // insert a MergeNode before n
  assert(n->hasSinglePredecessor(), "should have only one predecessor");
  MergeNode* merge = NodeFactory::new_MergeNode(n->bci());
  Node* prev = n->firstPrev();
  prev->moveNext(n, merge);
  merge->setPrev(prev);
  n->removePrev(prev);
  merge->append(n);
  return merge;
}


static int split_count = 0; // for conditional breakpoints (debugging)

void NodeBuilder::splitMergeExpr(Expr* expr, TypeTestNode* test) {
  // Split MergeExpressions (currently works only for booleans).
  // expr is the expression (generated somewhere upstream), test is the (only) node
  // testing this expression.  If there's nothing "complicated" inbetween, split the
  // merge to avoid the type test.
  split_count++;
  GrowableArray<Expr*>* exprsToSplit = splittablePaths(expr, test);
  if (!exprsToSplit) return;

  for (int i = exprsToSplit->length() - 1; i >= 0; i--) {
    Expr* e = exprsToSplit->at(i);
    Node* start = e->node();
    GrowableArray<NonTrivialNode*>* nodesToCopy = nodesBetween(start, test);
    assert(nodesToCopy != NULL, "should have worked");

    // find corresponding 'to' node in type test
    klassOop c = e->asConstantExpr()->klass();
    int j = test->classes()->length();
    while (j-- > 0 && test->classes()->at(j) != c) ;
    assert(j >= 0, "didn't find klass in type test");
    Node* to = test->next(j+1);	// +1 because next(i) is branch for class i-1
    if (CompilerDebug) cout(PrintSplitting)->print("%*s*splitting merge expr: from N%d to N%d (%dth split)\n", _scope->depth*2, "", start->id(), to->id(), split_count);
    if (!to->isMergeNode()) {
      to = insertMergeBefore(to);   	// insert a MergeNode inbetween
    }

    // disconnect defining node from its current successor
    start->removeNext(start->next());
    // append copies of all assignments
    Node* current = start;
#ifdef ASSERT
    bool found = nodesToCopy->length() == 0;	// hard to test if no nodes to copy, so assume it's ok
#endif
    for (int i = 0; i < nodesToCopy->length(); i++) {
      NonTrivialNode* orig = nodesToCopy->at(i);
      Node* copy = orig->copy(NULL, NULL);
      if (CompilerDebug) cout(PrintSplitting)->print("%*s*  (copying node N%d along the path -> N%d)\n", _scope->depth*2, "", orig->id(), copy->id());
      current = current->append(copy);
#ifdef ASSERT
      if (orig->dest() == test->src()) found = true;
#endif
    }
#ifdef ASSERT
    if (!found) {
      // no assignment to tested PReg found along path; must be earlier
      // check only leftmost path (must be assigned along any path)
      for (Node* n = start; n && !found; n = n->firstPrev()) {
        if (n->hasDest() && ((NonTrivialNode*)n)->dest() == test->src()) {
          found = true;
        }
      }
      assert(found, "assignment to tested PReg not found");
    }
#endif
    // connect to 'to' node
    current->append(to);
  }
}

GrowableArray<Expr*>* NodeBuilder::splittablePaths(const Expr* expr, const TypeTestNode* test) const {
  // test if expr can be split
  if (!Splitting) return NULL;
  if (!expr->isMergeExpr()) return NULL;
  MergeExpr* m = expr->asMergeExpr();
  if (!m->isSplittable()) return NULL;

  GrowableArray<Node*>* exprNodes = new GrowableArray<Node*>(10); // start nodes of all paths
  GrowableArray<Expr*>* okExprs   = new GrowableArray<Expr*>(10);  // those who are splittable

  // collect all paths that look splittable
  for (int i = m->exprs->length() - 1; i >= 0; i--) {
    for (Expr* x = m->exprs->at(i); x; x = x->next) {
      Node* start = x->node();
      exprNodes->append(start);
      if (!x->isConstantExpr()) continue;
      // check if there is a 'simple' path between defining node and type test node -> split if possible
      const Node* n = start;
      while (n != test) {
        if (!n->hasSingleSuccessor()) goto nextExpr;   // can't copy both paths
        bool ok = (n == start) ||
          n->isTrivial() ||
          n->isAssignNode() ||
          n->isMergeNode();
        if (!ok) goto nextExpr;			      // can't copy this node
        n = n->next();
      }
      // ok, the path from this node is splittable
      okExprs->append(x);
    }
nextExpr: ;
  }

  // check that no exprNode is along the path from any other exprNode to the test node
  // this should be #ifdef ASSERT but for now always check to make sure there are no lurking bugs  -Urs 4/27/96
  for (i = okExprs->length() - 1; i >= 0; i--) {
    Node* start = okExprs->at(i)->node()->next();
    for (Node* n = start; n != (Node*)test; n = n->next()) {
      if (exprNodes->contains(n)) {
        lprintf("error in splittable boolean expression:\n");
        m->print();
        okExprs->at(i)->print();
#ifdef DEBUG
        printNodes(okExprs->at(i)->node());
#endif
        for (int j = 0; j < exprNodes->length(); j++) { 
          exprNodes->at(j)->print();
          lprintf("\n");
        }
        fatal("compiler error");
      }
    }
  }

  return okExprs;
}

void NodeBuilder::allocate_closure(AllocationType type, int nofArgs, methodOop method) {
  PReg* context;
  if (type == tos_as_scope) {
    context = exprStack()->pop()->preg();
  } else {
    context = _scope->context();
  }
  CompileTimeClosure* closure = new CompileTimeClosure(_scope, method, context, nofArgs);
  BlockPReg* block = new BlockPReg(_scope, closure, bci(), EpilogueBCI);
  append(NodeFactory::new_BlockCreateNode(block, copyCurrentExprStack()));
  exprStack()->push(new BlockExpr(block, _current), _scope, scope()->bci());
}


static methodOopDesc::Block_Info incoming_info(methodOop m) {
  // this function is here for compatibility reasons only
  // should go away at some point: block_info replaced
  // incoming_info, but is only defined for blocks
  if (m->is_blockMethod()) {
    return m->block_info();
  } else {
    return methodOopDesc::expects_self;
  }
}


void NodeBuilder::allocate_context(int nofTemps, bool forMethod) {
  _scope->createContextTemporaries(nofTemps);
  assert(!scope()->contextInitializer(), "should not already have a contextInitializer");
  PReg* parent;	// previous context in the context chain
  if (forMethod) {
    // method, not block context points to current stack frame
    parent = new SAPReg(_scope, frameLoc, true, true, PrologueBCI, EpilogueBCI);
  } else {
    // context points to previous (incoming) context, if there
    assert(incoming_info(_scope->method()) != methodOopDesc::expects_self, "fix this");
    assert(incoming_info(_scope->method()) != methodOopDesc::expects_parameter, "fix this");
    if (_scope->isTop()) {
      parent =
        (incoming_info(_scope->method()) == methodOopDesc::expects_context)
        ? _scope->self()->preg()		// incoming context is passed in self's location (interpreter invariant); fix this to use different PReg
        : NULL;					// parent should never be used, set to 0 for debugging
      // (note: the interpreter sets parent always to self)
    } else {
      assert(_scope->isBlockScope(), "must be a block scope");
      // create new context PReg (_scope->context() is the context passed in by the caller)
      parent = _scope->context();
      _scope->setContext(new SAPReg(_scope, PrologueBCI, EpilogueBCI));
    }
  }
  ContextCreateNode* creator = NodeFactory::new_ContextCreateNode(parent, _scope->context(), nofTemps, copyCurrentExprStack());
  append(creator);
  // append context initializer and initialize with nil
  scope()->set_contextInitializer(NodeFactory::new_ContextInitNode(creator));
  append(scope()->contextInitializer());
  ConstantExpr* nil = new ConstantExpr(nilObj, new_ConstPReg(_scope, nilObj), NULL);
  for (int i = 0; i < nofTemps; i++) scope()->contextInitializer()->initialize(i, nil);
}


void NodeBuilder::removeContextCreation() {
  assert(scope()->contextInitializer() != NULL, "must have context");
  ContextCreateNode* creator = scope()->contextInitializer()->creator();
  creator->eliminate(creator->bb(), NULL, true, false);		// delete creator node
  scope()->contextInitializer()->notifyNoContext();		// let initializer know about it
}


void NodeBuilder::set_self_via_context() {
  if (Inline) {
    Scope* s = _scope->parent();
    if (s->isInlinedScope()) {
      // self doesn't need to be set -- can directly use the Expr of the enclosing scope
      assert(_scope->self() == ((InlinedScope*)s)->self(), "should have identical selves");
      return;
    }
  }

  // prevent multiple initializations (see BlockScope::initializeSelf)
  if (_scope->is_self_initialized()) return;
  _scope->set_self_initialized();

  // must load self at runtime; first compute home context no
  int contextNo = _scope->homeContext();
  if (_scope->allocatesInterpretedContext()) {
    // correct contextNo: the set_self_via_context works on the incoming context
    // -> subtract 1 (homeContext() already counts the context allocated in this
    // method (if there is one)
    assert(_scope->contextInitializer() != NULL, "should have a _contextInitializer already");
    contextNo--;
  }

  // do uplevel access and store into self
  // Note: this should use the same mechanism as access_temporary(...). However,
  // access_temporary relies on the fact that a possible local context is already
  // allocated. Thus, for the time being, explicitly generate
  // the uplevel access node. Note: the incoming context is in the recv location!
  const int self_no	= 0; // self is always the first entry in the top context, if there
  PReg* reg = _scope->self()->preg();
  append(NodeFactory::new_LoadUplevelNode(reg, reg, contextNo, contextOopDesc::temp0_word_offset() + self_no, NULL));
}


Expr* NodeBuilder::copy_into_context(Expr* e, int no) {
  if (e->isBlockExpr()) {
    // A block must be stored into a context (e.g. it's passed in as an arg and the arg
    // is uplevel-accessed).
    // Must materialize it here to make sure it's created; this BlockMaterializeNode should
    // be deleted if the context is eliminated, so register it with the context.
    BlockPReg* block = (BlockPReg*)e->preg();
    BlockMaterializeNode* n = NodeFactory::new_BlockMaterializeNode(block, copyCurrentExprStack());
    scope()->contextInitializer()->addBlockMaterializer(n);
    append(n);
    // Remember the context location so we can later retrieve the corresponding context.
    // (We're just using a Location as a convenient way to denote a particular location;
    // this doesn't have anything to do with register / context allocation per se, so don't
    // use the location for anything else.)
    Location* loc = Mapping::new_contextTemporary(no, no, scope()->scopeID());
    block->addContextCopy(loc);
    // don't copy the block so it can still be inlined (handled specially in context initializer if
    // context is created)
    return e;
  } else {
    return e->shallowCopy(new SAPReg(scope(), PrologueBCI, EpilogueBCI, true), NULL);
  }
}


void NodeBuilder::copy_self_into_context() {
  const int self_no = 0; // self is always the first temporary in a context, if there
  // caution: must create new expr/preg for self in context because the two locations must be different 
  Expr* self_expr_in_context = copy_into_context(scope()->self(), self_no);
  scope()->contextTemporariesAtPut(self_no, self_expr_in_context);
  scope()->contextInitializer()->initialize(self_no, scope()->self());
}


void NodeBuilder::copy_argument_into_context(int argNo, int no) {
  // caution: must create new expr/preg for arg in context because the two locations must be different 
  // i.e., arg is on stack, arg in context may be on heap
  Expr* arg_expr_in_context = copy_into_context(scope()->argument(argNo), no);
  scope()->contextTemporariesAtPut(no, arg_expr_in_context);
  scope()->contextInitializer()->initialize(no, scope()->argument(argNo));
}


void NodeBuilder::zap_scope() {
  assert(scope()->isMethodScope(), "blocks cannot be target of a NLR, no zap required");
  assert(scope()->contextInitializer() != NULL, "should have a context allocated");
  // no explicit node generated
}


void NodeBuilder::predict_prim_call(primitive_desc* pdesc, int failure_start) {
  // ignored
}


PReg* NodeBuilder::float_at(int fno) {
  if (UseFPUStack) {
    if (fno < scope()->nofFloatTemporaries()) {
      // fno refers to a float temporary
      return scope()->floatTemporary(fno)->preg();
    } else {
      // fno refers to a float on the float expression stack,
      // it must refer to the top since code is generated for
      // a stack machine
      return new SAPReg(_scope, topOfFloatStack, true, true, bci(), bci());
    }
  } else {
    // intermediate expressions are treated as temporaries
    return scope()->floatTemporary(fno)->preg();
  }
}


void NodeBuilder::float_allocate(int nofFloatTemps, int nofFloatExprs) {
  // nofFloats 64bit floats are allocated and initialized to NaN
  // in the following set of operations float(fno) (fno = float no.)
  // refers to one of these floats within the range [0..nofFloats[.
  //
  // Example code for a method containing the following code
  // (strictly using the stack-machine paradigm)
  //
  // | a <!Float>			; mapped to 0
  //   b <!Float>			; mapped to 1
  //   c <!Float>			; mapped to 2
  // |
  //
  // a := 3.0.
  // b := 4.0.
  // c := a + b.
  // ^ c
  //
  // float_allocate     5		; two more for expression stack temporaries (mapped to 3 & 4)
  // float_set          3, 3.0		; a := 3.0
  // float_move         0, 3
  // float_set          3, 4.0		; b := 4.0
  // float_move         1, 3
  // float_move         3, 0		; push a
  // float_move         4, 1		; push b
  // float_binaryOp     3, add		; add
  // float_move         2, 3		; store to c
  // float_move         3, 2		; push c
  // float_unaryOpToOop 3, oopify	; push c converted to oop
  // return_tos				; return tos
  int size = nofFloatTemps + nofFloatExprs;
  assert(size == method()->total_number_of_floats(), "inconsistency");
  // floatTemporaries are allocated in InlinedScope::genCode()
}


void NodeBuilder::float_floatify(Floats::Function f, int fno) {
  // top of stack must be a boxed float, it is unboxed and stored at float(fno).
  Expr* t = _exprStack->pop();
  if (t->hasKlass() && t->klass() == doubleKlassObj) {
    // no type test needed
  } else {
    // put in a type test - fix this!
  }
  append(NodeFactory::new_FloatUnaryArithNode(float_at(fno), t->preg(), f2FloatArithOp));
}


void NodeBuilder::float_move(int to, int from) {
  // float(to) := float(from)
  append(NodeFactory::new_AssignNode(float_at(from), float_at(to)));
}


void NodeBuilder::float_set(int to, doubleOop value) {
  // float(to) := value
  ConstPReg* val = new_ConstPReg(_scope, value);
  append(NodeFactory::new_AssignNode(val, float_at(to)));
}


void NodeBuilder::float_nullary(Floats::Function f, int to) {
  // float(to) := f()
  // f refers to one of the functions in Floats
  switch (f) {
    case Floats::zero	: float_set(to, oopFactory::new_double(0.0)); break;
    case Floats::one	: float_set(to, oopFactory::new_double(1.0)); break;
    default		: fatal1("bad float nullary code %d", f);
  }
}


void NodeBuilder::float_unary(Floats::Function f, int fno) {
  // float(fno) := f(float(fno))
  // f refers to one of the functions in Floats
  ArithOpCode op;
  switch (f) {
    case Floats::abs	: op = fAbsArithOp; break;
    case Floats::negated: op = fNegArithOp; break;
    case Floats::squared: op = fSqrArithOp; break;
    case Floats::sqrt	: Unimplemented();
    case Floats::sin	: Unimplemented();
    case Floats::cos	: Unimplemented();
    case Floats::tan	: Unimplemented();
    case Floats::exp	: Unimplemented();
    case Floats::ln	: Unimplemented();
    default		: fatal1("bad float unary code %d", f);
  }
  PReg* preg = float_at(fno);
  append(NodeFactory::new_FloatUnaryArithNode(preg, preg, op));
}


void NodeBuilder::float_binary(Floats::Function f, int fno) {
  // float(fno) := f(float(fno), float(fno+1))
  // f refers to one of the functions in Floats
  ArithOpCode op;
  switch (f) {
    case Floats::add	 : op = fAddArithOp; break;
    case Floats::subtract: op = fSubArithOp; break;
    case Floats::multiply: op = fMulArithOp; break;
    case Floats::divide	 : op = fDivArithOp; break;
    case Floats::modulo	 : op = fModArithOp; break;
    default		 : fatal1("bad float binary code %d", f);
  }
  PReg* op1 = float_at(fno);
  PReg* op2 = float_at(fno+1);
  append(NodeFactory::new_FloatArithRRNode(op1, op1, op, op2));
}


void NodeBuilder::float_unaryToOop(Floats::Function f, int fno) {
  // push f(float(fno)) on top of (oop) expression stack, result is an oop
  // f refers to one of the functions in Floats
  PReg* src = float_at(fno);
  SAPReg* res = new SAPReg(_scope);
  switch (f) {
    case Floats::is_zero: // fall through
    case Floats::is_not_zero:  
      { ConstPReg* zero = new_ConstPReg(_scope, oopFactory::new_double(0.0));
      NodeFactory::new_FloatArithRRNode(new NoPReg(_scope), src, fCmpArithOp, zero);
      BranchOpCode cond = f == Floats::is_zero ? EQBranchOp : NEBranchOp;
      _exprStack->push(PrimInliner::generate_cond(cond, this, res), scope(), scope()->bci());
      }
      break;
    case Floats::oopify: 
      { append(NodeFactory::new_FloatUnaryArithNode(res, src, f2OopArithOp));
      Expr* result = new KlassExpr(doubleKlassObj, res, current());
      _exprStack->push(result, scope(), scope()->bci());
      }
      break;
    default: fatal1("bad float unaryToOop code %d", f);
  }
}


void NodeBuilder::float_binaryToOop(Floats::Function f, int fno) {
  // push f(float(fno), float(fno+1)) on top of (oop) expression stack, result is an oop
  // f refers to one of the functions in Floats
  Assembler::Condition cc1;
  switch (f) {
    case Floats::is_equal	 : cc1 = Assembler::equal;		break;
    case Floats::is_not_equal	 : cc1 = Assembler::notEqual;		break;
    case Floats::is_less	 : cc1 = Assembler::less;		break;
    case Floats::is_less_equal	 : cc1 = Assembler::lessEqual;		break;
    case Floats::is_greater	 : cc1 = Assembler::greater;		break;
    case Floats::is_greater_equal: cc1 = Assembler::greaterEqual;	break;
    default			 : fatal1("bad float comparison code %d", f);
  }
  int mask;
  Assembler::Condition cond;
  MacroAssembler::fpu_mask_and_cond_for(cc1, mask, cond);
  PReg* op1 = float_at(fno);
  PReg* op2 = float_at(fno+1);
  SAPReg* fpu_status = new SAPReg(_scope, Mapping::asLocation(eax), false, false, bci(), bci());
  append(NodeFactory::new_FloatArithRRNode(fpu_status, op1, fCmpArithOp, op2));
  append(NodeFactory::new_ArithRCNode(new NoPReg(_scope), fpu_status, TestArithOp, mask));
  BranchOpCode cc2;
  switch (cond) {
    case Assembler::zero   : cc2 = EQBranchOp; break;
    case Assembler::notZero: cc2 = NEBranchOp; break;
    default                : ShouldNotReachHere();
  }
  SAPReg* res = new SAPReg(_scope);
  _exprStack->push(PrimInliner::generate_cond(cc2, this, res), scope(), scope()->bci());
}


# endif
