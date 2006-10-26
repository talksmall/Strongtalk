/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.21 $ */
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
# include "incls/_blockOop.cpp.incl"

methodOop blockClosureOopDesc::method() const { 
  oop m = addr()->_methodOrJumpAddr;

# ifdef DELTA_COMPILER
  if (isCompiledBlock()) {
    jumpTableEntry* e = (jumpTableEntry*) m;
    assert(e->is_block_closure_stub(), "must be block stub");
    return e->block_method();
  }
# endif

  return methodOop(m); 
}

int blockClosureOopDesc::number_of_arguments() {
  return ((blockClosureKlass*)klass())->number_of_arguments();
}

bool blockClosureOopDesc::is_pure() const {
  return !lexical_scope()->is_context();
}

void blockClosureOopDesc::verify() {
  memOopDesc::verify();
  oop m = addr()->_methodOrJumpAddr;
# ifdef DELTA_COMPILER
  if (isCompiledBlock()) {
    jumpTableEntry* e = (jumpTableEntry*) m;
    e->verify();
    if (!e->is_block_closure_stub()) error("stub %#x of block %#x isn't a closure stub", e, this);
  } else {
    m->verify();
  }
# endif
}

blockClosureOop blockClosureOopDesc::create_clean_block(int nofArgs, char* entry_point) {
  blockClosureOop blk = allocateTenuredBlock(as_smiOop(nofArgs));
  blk->set_lexical_scope((contextOop)nilObj);
  blk->set_jumpAddr(entry_point);
  return blk;
}

void blockClosureOopDesc::deoptimize() {
  if (!isCompiledBlock()) return; // do nothing if unoptimized

  contextOop con = lexical_scope();
  if (con == nilObj) return;     // do nothing if lexical scope is nil

  int index;
  nmethod* nm = jump_table_entry()->parent_nmethod(index); 
  NonInlinedBlockScopeDesc* scope = nm->noninlined_block_scope_at(index);

  LOG_EVENT1("Deoptimized context in blockClosure -> switch to methodOop 0x%lx", nm);
  assert(nm, "nmethod must be present");

  assert(!StackChunkBuilder::is_deoptimizing(), "you cannot be in deoptimization mode");
  StackChunkBuilder::begin_deoptimization();

  // Patch the block closure to unoptimized form
  set_method(scope->method());
  con = compiledVFrame::compute_canonical_context(scope->parent(), NULL, con);
  set_lexical_scope(con);

  StackChunkBuilder::end_deoptimization();
}

// -------------- contextOop --------------------
bool contextOopDesc::is_dead() const { 
  // assert(!mark()->has_context_forward(), "checking if context is deoptimized");
  return parent() == oop(smiOop_zero) || parent() == nilObj;
}

bool contextOopDesc::has_parent_fp() const {
  // assert(!mark()->has_context_forward(), "checking if context is deoptimized");
  return parent()->is_smi() && !is_dead();
}

bool contextOopDesc::has_outer_context() const {
  // assert(!mark()->has_context_forward(), "checking if context is deoptimized");
  return parent()->is_context();
}

contextOop contextOopDesc::outer_context() const {
  if (has_outer_context()) {
    contextOop con = contextOop(parent());
    assert(con->is_context(), "must be context");
    return con;
  }
  return NULL;
}

void contextOopDesc::set_unoptimized_context(contextOop con) {
  assert(!mark()->has_context_forward(), "checking if context is deoptimized");
  assert(this != con, "Checking for forward cycle");
  set_parent(con);
  set_mark(mark()->set_context_forward());
}

contextOop contextOopDesc::unoptimized_context() {
  if (mark()->has_context_forward()) {
    contextOop con =  contextOop(parent());
    assert(con->is_context(), "must be context");
    return con;
  }
  return NULL;
}

int contextOopDesc::chain_length() const {
  int size = 1;
#ifdef ASSERT
  GrowableArray<contextOop>* path = new GrowableArray<contextOop>(10);
  for (contextOop cc = contextOop(this); cc->has_outer_context(); cc = cc->outer_context()) {
    assert(path->find(cc) < 0, "cycle has been detected in a context chain")
    path->append(cc);
  }
#endif
  for (contextOop con = contextOop(this); con->has_outer_context(); con = con->outer_context()) {
    size++;
  }
  return size;
}

void contextOopDesc::print_home_on(outputStream* st) {
  if (mark()->has_context_forward()) {
    st->print("deoptimized to (");
    unoptimized_context()->print_value();
    st->print(")");
  } else if (has_parent_fp()) {
    st->print("frame 0x%lx", parent_fp());
  } else if (has_outer_context()) {
    st->print("outer context ");
    outer_context()->print_value_on(st);
  } else {
    assert(is_dead(), "context must be dead");
    st->print("dead");
  }
}