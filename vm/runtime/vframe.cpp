/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.100 $ */
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
# include "incls/_vframe.cpp.incl"

// Ideas:
//   Maybe cache methodOop in deltaVFrame for faster argument access. (Lars 8/10/95)

// ------------- vframe --------------

bool vframe::equal(const vframe* f) const {
  return _fr.fp() == f->_fr.fp();
}

oop vframe::callee_argument_at(int index) const {
  getErr()->print_cr("vframe::callee_argument_at should be specialized for all vframes calling deltaVFrames");
  fatal("aborting");
  return NULL;
}

vframe* vframe::new_vframe(frame* f) {
  if (f->is_interpreted_frame()) return new interpretedVFrame(f);
  if (f->is_entry_frame())       return new cChunk(f);
  if (f->is_deoptimized_frame()) return new deoptimizedVFrame(f);
#ifdef DELTA_COMPILER
  if (f->is_compiled_frame()) {
    nmethod* nm = f->code();
    assert(nm, "nmethod not found");

    // NB: pc points *after* the current instruction (e.g., call), so must adjust it
    // to get the right bci; -1 will do portably    -Urs 2/96
    char* pc = f->pc() - 1;
    PcDesc*    pd = nm->containingPcDesc(pc);
    assert(pd, "PcDesc not found");

    ScopeDesc* sd = nm->scopes()->at(pd->scope, pc);
    assert(sd, "ScopeDesc not found");
    return compiledVFrame::new_vframe(f, sd, pd->byteCode);
  }
#endif
  return new cVFrame(f);
}

vframe* vframe::sender() const {
  assert(is_top(), "just checking");
  frame s = _fr.sender();
  if (s.is_first_frame()) return NULL;
  return vframe::new_vframe(&s);
}

vframe* vframe::top() const {
  vframe* vf = (vframe*) this;
  while (!vf->is_top()) vf = vf->sender();
  return vf;
}

void vframe::print() {
  if (WizardMode  || ActivationShowFrame) {
    _fr.print();
  }
}

void vframe::print_value() const { 
  ((vframe*)this)->print();
}

// ------------- deltaVFrame --------------

GrowableArray<oop>* deltaVFrame::arguments() const {
  int nargs = method()->number_of_arguments();
  GrowableArray<oop>* result = new GrowableArray<oop>(nargs);
  vframe* s = sender();
  for (int index = 0; index < nargs; index++) {
   result->push(argument_at(index)); 
  }
  return result;
}

oop deltaVFrame::argument_at(int index) const {
  return sender()->callee_argument_at(method()->number_of_arguments() - (index+1));
}

oop deltaVFrame::callee_argument_at(int index) const {
  return expression_at(index);
}

void deltaVFrame::print() {
  lprintf("Delta frame: "); 
  vframe::print();
}

void deltaVFrame::print_activation(int index) const {
  ((vframe*)this)->vframe::print();
  prettyPrinter::print(index, (deltaVFrame*) this);
}


deltaVFrame* deltaVFrame::sender_delta_frame() const {
  vframe* f = sender();
  while (f != NULL) {
    if (f->is_delta_frame()) return (deltaVFrame*) f;
    f = f->sender();
  }
  return NULL;
}

void deltaVFrame::verify() const {
  // Verify the frame
  fr().verify();
  // Make sure we do not have any context objects in the argument list
  int number_of_arguments = method()->number_of_arguments();
  for (int index = 0; index < number_of_arguments; index++) {
    oop argument = argument_at(index);
    if (argument->is_context()) {
      std->print_cr("Argument is a context");
      print_activation(0);
      warning("verify failed");
    }
  }
}
// ------------- interpretedVFrame --------------

bool interpretedVFrame::has_interpreter_context() const {
  return method()->activation_has_context();
}

oop interpretedVFrame::temp_at(int offset) const {
  assert(offset > 0 || !has_interpreter_context(),
         "you cannot use temp(0) when a context is present");
  assert(offset < method()->number_of_stack_temporaries(), "checking bounds");
  return _fr.temp(offset);
}

oop interpretedVFrame::context_temp_at(int offset) const {
  assert(has_interpreter_context(), "must have context when using context_temp_at");
  return interpreter_context()->obj_at(offset);
}

contextOop interpretedVFrame::interpreter_context() const {
  if (!has_interpreter_context()) return NULL;
  contextOop result = contextOop(_fr.temp(0));
  assert(method()->in_context_allocation(bci()) || result->is_context(), "context type check");
  return result;
}

contextOop interpretedVFrame::canonical_context() const { 
  return interpreter_context();
}

oop interpretedVFrame::expression_at(int index) const {
 return *expression_addr(index);
}

oop* interpretedVFrame::expression_addr(int offset) const {
  return (oop*) &((oop*) _fr.sp())[offset]; 
}

GrowableArray<oop>* interpretedVFrame::expression_stack() const {
  int last_temp_number = method()->number_of_stack_temporaries() - 1;
  int size = _fr.temp_addr(last_temp_number) - expression_addr(0);
  assert(size >= 0, "expr stack size must be non-negative");
  GrowableArray<oop>* result = new GrowableArray<oop>(size);
  for (int index = 0; index < size; index++) {
    oop value = expression_at(index);
    assert(!value->is_context(), "checking for contextOop on expression stack");
    result->push(expression_at(index));
  }
#ifdef ASSERT
  int computed_size = method()->expression_stack_mapping(bci())->length();
  if (size != computed_size) {
    warning("Expression stack size  @%d is %d but computed to %d", bci(), size, computed_size);
    std->print_cr("[expression stack:"); 
    for (int index = 0; index < result->length(); index++) {
      std->print(" - ");
      result->at(index)->print_value_on(std);
      std->cr();
    }
    std->print_cr("]"); 
    method()->pretty_print();
    method()->print_codes();
  }
#endif
  return result;
}
 
u_char* interpretedVFrame::hp() const { 
  return _fr.hp();
}

void interpretedVFrame::set_hp(u_char* p) {
  _fr.set_hp(p);
}

oop interpretedVFrame::receiver() const {
  // In case of a block invocation the receiver might be a contextOop
  // due to an interpreter optimization (ask Robert for details).
  // To provide clean semantics in this case nilObj is returned.
  oop r = _fr.receiver();
  assert(!r->is_context() || method()->is_blockMethod(), "check: context implies block method");
  return r->is_context() ? nilObj : r;
}

void interpretedVFrame::set_receiver(oop obj) {
  _fr.set_receiver(obj);
}

void interpretedVFrame::temp_at_put(int offset, oop obj) { 
  _fr.set_temp(offset, obj);
}

void interpretedVFrame::expression_at_put(int offset, oop obj) {
  // FIX LATER p.set_expr(offset) = obj;
  Unimplemented();
}

bool interpretedVFrame::equal(const vframe* f) const {
  if (!f->is_interpreted_frame()) return false;
  return vframe::equal(f);
}

int interpretedVFrame::bci() const {
  return method()->bci_from(hp());
}

methodOop interpretedVFrame::method() const {
  memOop m = as_memOop(Universe::object_start((oop*) (hp() - 1)));
  assert(m->is_method(), "must be method");
  return methodOop(m);
}

deltaVFrame* interpretedVFrame::parent() const {
  methodOop m = method();
  
  // Return NULL if method is outer.
  if (!m->is_blockMethod()) return NULL;

  contextOop target = interpreter_context();
  if (!target) return NULL; // Return NULL if no context is present.
  
  // Walk the stack and find the vframe with outer as context.
  // NB: the parent may still be alive even though it cannot be
  //     found on this stack. It might reside on another stack.

  for (vframe* p = sender(); p; p = p->sender()) {
    if (p->is_interpreted_frame())
      if (((interpretedVFrame*)p)->interpreter_context() == target)
        return (deltaVFrame*) p;
  }

  warning("parent frame is not found on same stack");

  return NULL;
}

void interpretedVFrame::verify() const {
  deltaVFrame::verify();
  methodOop m = method();
  if(m->activation_has_context()) {
    if (!m->in_context_allocation(bci())) {
      contextOop con = interpreter_context();
      if (!con->is_context()) warning("expecting context");
      if (!m->is_blockMethod()) {
        if(con->parent_fp() == NULL) warning("expecting frame in context");
      }
      m->verify_context(con);
    }
  }
}

# ifdef DELTA_COMPILER

// ------------- compiledVFrame --------------

compiledVFrame* compiledVFrame::new_vframe(const frame* fr, ScopeDesc* sd, int bci) {
  if (sd->isMethodScope())
    return new compiledMethodVFrame(fr, sd, bci);
  if (sd->isTopLevelBlockScope())
    return new compiledTopLevelBlockVFrame(fr, sd, bci);
  if (sd->isBlockScope()) 
    return new compiledBlockVFrame(fr, sd, bci);
  fatal("unknown scope desc");
  return NULL;
}

void compiledVFrame::rewind_bci() {
  int new_bci = method()->find_bci_from(_bci);
  assert(new_bci >= 0, "must be real bci");
  std->print_cr("%d -> %d", _bci, new_bci);
  _bci = new_bci;
}

compiledVFrame::compiledVFrame(const frame* fr, ScopeDesc* sd, int bci) : deltaVFrame(fr) {
  this->sd   = sd;
  this->_bci = bci;
}

vframe* compiledVFrame::sender() const {
  if (sd->isTop()) return vframe::sender();
  return compiledVFrame::new_vframe(&_fr, sd->sender(), sd->senderBCI());
}

oop compiledVFrame::temp_at(int offset) const {
  assert(offset > 0 || !method()->activation_has_context(),
         "you cannot use temp(0) when a context is present");
  assert(offset < method()->number_of_stack_temporaries(), "checking bounds");
  return resolve_name(scope()->temporary(offset), this);
}

class ContextTempFindClosure : public NameDescClosure {
 public:
  NameDesc* result;
  int i;
  ContextTempFindClosure(int index) {
    i = index;
    result = NULL; }
  void context_temp (int no, NameDesc* a, char* pc) {
    if (no == i) result = a;
  }
};

oop compiledVFrame::context_temp_at(int offset) const {
  assert(method()->activation_has_context(),
         "you cannot use context_temp_at() when methodOop does not allocate context");
  ContextTempFindClosure blk(offset);
  sd->iterate(&blk);
  assert(blk.result, "must find result");
  return resolve_name(blk.result, this);
}

oop compiledVFrame::expression_at(int index) const {
  GrowableArray<DeferredExpression*>* stack = deferred_expression_stack();
  if (stack->length() <= index) {
    // Hack for Robert 1/15/96, probably wrong expression stack
    return oopFactory::new_symbol("invalid stack element");
  }
  return stack->at(index)->value();
}

class CollectContextInfoClosure : public NameDescClosure {
 public:
  GrowableArray<NameDesc*>* result;
  CollectContextInfoClosure() {
    result = new GrowableArray<NameDesc*>(10);
  }
  void context_temp(int no, NameDesc* a, char* pc) { 
    result->append(a);
  }
};

extern "C" contextOop allocateContext(smiOop nofVars);

contextOop compiledVFrame::compiled_context() const {
  if (!method()->activation_has_context()) return NULL;
  
  // Hack suggested by Urs
  // if temp 0 contains nil the compiler has optimized away the
  // contextOop.
  // A better solution would be adding has_compiled_context to scopeDesc.
  oop con = resolve_name(scope()->temporary(0), this); 
  if (con == nilObj) return NULL;

  assert(con->is_context(), "context type check");
  return contextOop(con);
}

GrowableArray<DeferredExpression*>* compiledVFrame::deferred_expression_stack() const {
  GrowableArray<int>* mapping = method()->expression_stack_mapping(bci());
  GrowableArray<DeferredExpression*>* result = new GrowableArray<DeferredExpression*>(mapping->length());
  for (int index = 0; index < mapping->length(); index++) {
    NameDesc* nd    = sd->exprStackElem(mapping->at(index));
    result->push(new DeferredExpression(this, nd));
  }
  return result;
}

GrowableArray<oop>* compiledVFrame::expression_stack() const {
  GrowableArray<int>* mapping = method()->expression_stack_mapping(bci());
  GrowableArray<oop>* result = new GrowableArray<oop>(mapping->length());
  for (int index = 0; index < mapping->length(); index++) {
    NameDesc* nd    = sd->exprStackElem(mapping->at(index));
    oop       value = resolve_name(nd, this);
    result->push(value);
  }
#ifdef ASSERT
  int computed_size = method()->expression_stack_mapping(bci())->length();
  if (result->length() != computed_size) {
    warning("Expression stack size  @%d is %d but computed to %d", bci(), result->length(), computed_size);
    std->print_cr("[expression stack:"); 
    for (int index = 0; index < result->length(); index++) {
      std->print(" - ");
      result->at(index)->print_value_on(std);
      std->cr();
    }
    std->print_cr("]"); 
    method()->pretty_print();
    method()->print_codes();
  }
#endif
  return result;
}

bool compiledVFrame::equal(const vframe* f) const {
  if (!f->is_compiled_frame()) return false;
  return vframe::equal(f)
      && scope()->is_equal(((compiledVFrame*)f)->scope());
}

int compiledVFrame::bci() const {
  return _bci;
}

methodOop compiledVFrame::method() const {
  return sd->method();
}

nmethod* compiledVFrame::code() const {
  return _fr.code();
}

oop compiledVFrame::resolve_location(Location loc, const compiledVFrame* vf, contextOop con) {

  // Context location
  if (loc.isStackLocation()) {
    return vf
	 ? oop(vf->fr().at(loc.offset()))
	 : filler_oop();
  }

  // Context location
  if (loc.isContextLocation()) {
    if (vf) {
      ScopeDesc* scope = vf->code()->scopes()->at(loc.scopeOffs(), vf->fr().pc());
      assert(scope->allocates_compiled_context(), "must have context");
      assert(scope->compiled_context()->isLocation(), "context must be a location");
      contextOop context = contextOop(resolve_location(scope->compiled_context()->location(), vf));
      return context->obj_at(loc.tempNo());
    }

    if (con) {
      return con->obj_at(loc.tempNo());
    }

    return filler_oop();
  }

  // Register location
  if (loc.isRegisterLocation()) {
    return SavedRegisters::fetch(loc.number(), vf->fr().fp());
  }

  ShouldNotReachHere();
  return NULL;
}

oop compiledVFrame::resolve_name(NameDesc* nd, const compiledVFrame* vf, contextOop con) {
  // takes a NameDesc & looks up the oop it describes
  // (on the stack, in contexts, etc.)

  if (nd->isLocation()) {
    // describes a location where we can find the oop.
    Location loc = nd->location();
    return resolve_location(loc, vf, con);
  }

  if (nd->isValue()) {
    // an oop constant.
    return nd->value();
  }

  if (nd->isBlockValue()) {
    // a blockClosureOop that has been completely optimized away.
    frame f = vf->fr();
    oop result = nd->value(&f);
    return result;
  }

  if (nd->isMemoizedBlock()) {
    // a blockClosureOop that has been partially optimized away.
    // if the block doesn't exist yet, we have to create one and store it in the location.
    frame f = vf->fr();
    oop result = nd->value(&f);
    return result;
  }

  if (nd->isIllegal()) {
    // This should never happen.
    // For now, a fake value is returned to avoid crashes when tracing the stack.
    // Lars 7/3/95
    if (UseNewBackend) {
      // This is a hack - we should introduce a special
      // nameDesc instead - gri 8-5-96
      return nilObj;
    }
    warning("Compiler Bug: Illegal name desc found in nmethod 0x%lx @ %d",
            vf->fr().code(), vf->scope()->offset());
    return oopFactory::new_symbol("illegal nameDesc");
  }

  ShouldNotReachHere();
  return NULL;
}

oop compiledVFrame::filler_oop() {
  return nilObj;
  // This is useful for debugging
  // return oopFactory::new_symbol("compiledVFrame::filler_oop");
}

int compiledVFrame::bci_for(ScopeDesc* d) const {
  ScopeDesc* s = sd;
  int        b = bci();
  while (!s->is_equal(d)) {
    b = s->senderBCI();
    assert(s->sender(), "make sure we have a sender");
    s = s->sender();
  }
  return b;
}

#define CHECK(n)  if (n->isIllegal()) ok = false

class VerifyNDClosure : public NameDescClosure {
 public:
  bool ok;
  VerifyNDClosure() { ok = true; }
  void arg	    (int no, NameDesc* a, char* pc) { CHECK(a); }
  void temp	    (int no, NameDesc* a, char* pc) { CHECK(a); }
  void context_temp (int no, NameDesc* a, char* pc) { CHECK(a); }
};

void compiledVFrame::verify_debug_info() const {
  // verify that all source-visible names / expr stack entries have valid nameDescs
  ResourceMark rm;
  bool ok = true;
  VerifyNDClosure blk;
  sd->iterate(&blk);
  CHECK(sd->self());

  if (!ok || !blk.ok) {
    print_activation(0);
    error("illegal nameDescs in vframe");
  }
#undef CHECK
}

class Indenting : public ValueObj {
public:
  Indenting() {_std->inc();}
  ~Indenting() {_std->dec();}
};

void traceFrame(const compiledVFrame* vf, contextOop con) {
    if (TraceCanonicalContext) {
      FlagSetting flag(TraceCanonicalContext, false);
      _std->cr();
      _std->indent();
      _std->print_cr("context(0x%x), vframe(0x%x), block? %d", con, vf,
        vf ? vf->method()->is_blockMethod() : false);
      if (vf) {
        vf->print_activation(0);
        vf->method()->print_codes();
      }
    }
}

contextOop compiledVFrame::compute_canonical_parent_context(ScopeDesc* scope, const compiledVFrame* vf, contextOop con) {
  compiledVFrame* parent_vf = (!vf || !vf->parent() || !vf->parent()->is_compiled_frame())
                                  ? NULL
                                  : (compiledVFrame*) vf->parent();
    return compute_canonical_context(scope->parent(true), parent_vf, con);
}

contextOop compiledVFrame::compute_canonical_context(ScopeDesc* scope, const compiledVFrame* vf, contextOop con) {
  // Computes the canonical contextOop for a scope desc.
  // 
  // Recipe:
  // 1. Search the stack builder context cache (in case we're during deoptimizing)
  // 2. Check the forward reference in contextOop.
  // 3. Allocate a fresh contextOop.

  // Playing rules for contextOops
  // - If there exists a compiled context there is an 1-to-1 mapping to an interpreted context
  Indenting in;
  traceFrame(vf, con);

  if (!scope->allocates_interpreted_context()) {
    // This scope does not allocate an interpreter contextOop
    if (scope->isMethodScope()) return NULL;

    if (!scope->method()->expectsContext() && 
         scope->method()->parent()->allocatesInterpretedContext()) {
      warning("May be allocating context when unneeded");
    }
    return compute_canonical_parent_context(scope, vf, con);
  }

  contextOop result;

  // step 1.
  if (vf) {
    if ((result = StackChunkBuilder::context_at(vf)) != NULL) {
      assert(result->unoptimized_context() == NULL, "cannot have deoptimized context");
      return result;
    }
  }

  // step 2
  if (con && con->unoptimized_context()) {
    result = con->unoptimized_context(); // shouldn't this go in the SCB context cache?
    assert(result->unoptimized_context() == NULL, "cannot have deoptimized context");
    return result;
  }

  contextOop comp_context = con;
  assert(comp_context == NULL || comp_context->is_context(), "must be context");
  
  // step 3
  if (!MaterializeEliminatedBlocks && !StackChunkBuilder::is_deoptimizing()) {
    // don't create a context (for better compiler debugging)
    // not quite kosher since the return type is expected to be a contextOop,
    // but for stack printing code it shouldn't matter
    ResourceMark rm;
    stringStream st(50);
    st.print("eliminated context in ");
    scope->selector()->print_symbol_on(&st);
    return (contextOop)oopFactory::new_symbol(st.as_string());	  // unsafe cast
  }

  // collect all NameDescs
  CollectContextInfoClosure blk;
  scope->iterate(&blk);

  // allocate the new context
  result = contextKlass::allocate_context(blk.result->length());

  // fill in the meat
  for(int index = 0; index < blk.result->length(); index++) {
    NameDesc* nd = blk.result->at(index);
    result->obj_at_put(index, resolve_name(nd, vf, comp_context));
  }

  if (vf) {
    StackChunkBuilder::context_at_put(vf, result);
  } else {
    // kill the home field
    result->kill();
  }

  assert(result->unoptimized_context() == NULL, "cannot have deoptimized context");

  // Set parent scope if needed
  if (!scope->isMethodScope()) {
    contextOop parent = compute_canonical_parent_context(scope, vf, con ? con->outer_context() : NULL);
    result->set_parent(parent);
  }

  if (StackChunkBuilder::is_deoptimizing() && comp_context) {
    // Save the unoptimized context as a forward reference in the
    // compiled context.
    comp_context->set_unoptimized_context(result);
  }

  if (TraceDeoptimization && comp_context) {
    std->print(" - ");
    comp_context->print_value();
    std->print_cr(" -> ");
    result->print_value();
    std->cr();
  }
 
  assert(result->unoptimized_context() == NULL, "cannot have deoptimized context");
  return result;
}

void compiledVFrame::verify() const {
  deltaVFrame::verify();
  contextOop con = compiled_context();
  if (con) {
    if(con->mark()->has_context_forward())
      warning("context has forwarder");
  }
}
// ------------- compiledMethodVFrame --------------

compiledMethodVFrame::compiledMethodVFrame(const frame* fr, ScopeDesc* sd, int bci)
: compiledVFrame(fr, sd, bci) {}


bool compiledMethodVFrame::is_top() const {
  return sd->isTop();
}

oop compiledMethodVFrame::receiver() const {
  return resolve_name(sd->self(), this);
}

contextOop compiledMethodVFrame::canonical_context() const {
  assert(!method()->is_blockMethod(), "check methodOop type");
  contextOop conIn = compiled_context();
  contextOop conOut = compute_canonical_context(scope(), this, conIn);
  if (TraceCanonicalContext) {
    _std->print("context in(0x%x), vf(0x%x), context out(0x%x), block? %d", conIn, this, conOut, method()->is_blockMethod());
  }
  return conOut;
}

// ------------- compiledBlockVFrame --------------

compiledBlockVFrame::compiledBlockVFrame(const frame* fr, ScopeDesc* sd, int bci)
: compiledVFrame(fr, sd, bci) {}

bool compiledBlockVFrame::is_top() const {
  return sd->isTop();
}

oop compiledBlockVFrame::receiver() const {
  NameDesc* nd = sd->self();
  if (nd) {
    return resolve_name(nd, this);
  } else {
    fatal("self is unknown");	// can't handle this yet -- fix this
    return NULL;
  }
}

deltaVFrame* compiledBlockVFrame::parent() const {
  ScopeDesc* ps = parent_scope();
  int parent_bci = bci_for(ps);
  return compiledVFrame::new_vframe(&_fr, ps, parent_bci);
}

contextOop compiledBlockVFrame::canonical_context() const {
  assert(method()->is_blockMethod(), "must be block method");
  contextOop conIn = compiled_context();
  contextOop conOut = compute_canonical_context(scope(), this, conIn);
  if (TraceCanonicalContext) {
    _std->print("context in(0x%x), vf(0x%x), context out(0x%x), block? %d", conIn, this, conOut, method()->is_blockMethod());
  }
  return conOut;
}

ScopeDesc* compiledBlockVFrame::parent_scope() const {
  ScopeDesc* result = scope()->parent();
  assert(result, "parent should be within same nmethod");
  return result;
}

// ------------- compiledTopLevelBlockVFrame --------------

compiledTopLevelBlockVFrame::compiledTopLevelBlockVFrame(const frame* fr, ScopeDesc* sd, int bci)
: compiledVFrame(fr, sd, bci) {}

oop compiledTopLevelBlockVFrame::receiver() const {
  return resolve_name(sd->self(), this);
}

deltaVFrame* compiledTopLevelBlockVFrame::parent() const {
  methodOop m = method();
  if (!m->expectsContext()) return NULL;
  
  contextOop parent_context = m->allocatesInterpretedContext() 
                          ? compiled_context()->outer_context()
			  : compiled_context();

  // If the context is killed return NULL
  if (!parent_context || parent_context->is_dead()) return NULL;

  // Now we have to search for the parent on the stack.
  ScopeDesc* ps = parent_scope();
  nmethod* parent_nmethod = ps->scopes()->my_nmethod();

  frame v = fr().sender();
  do {
    if (v.is_compiled_frame()) {
      if (v.code() == parent_nmethod) {
	compiledVFrame* result = (compiledVFrame*) vframe::new_vframe(&v);
        // Run throuch the scopes and find a matching one
        while (result) {
	  assert(result->is_compiled_frame(), "must be compiled frame");
          if (result->scope()->is_equal(ps)) {
            if (result->compiled_context() == parent_context)
              return result;
          }
          result = result->is_top() ? NULL : (compiledVFrame*) result->sender();
        }
      }
    }
    v = v.sender();
  } while (!v.is_first_frame());
  return NULL;
}

contextOop compiledTopLevelBlockVFrame::canonical_context() const {
  assert(method()->is_blockMethod(), "must be block method");
  contextOop conIn = compiled_context();
  contextOop conOut = compute_canonical_context(scope(), this, conIn);
  if (TraceCanonicalContext) {
    _std->print("context in(0x%x), vf(0x%x), context out(0x%x), block? %d", conIn, this, conOut, method()->is_blockMethod());
  }
  return conOut;
}

ScopeDesc* compiledTopLevelBlockVFrame::parent_scope() const {
  ScopeDesc* result = scope()->parent(true);
  assert(result, "parent scope must be present");
  return result;
}

// ------------- deoptimizedVFrame --------------

objArrayOop deoptimizedVFrame::retrieve_frame_array() const {
  objArrayOop array = _fr.frame_array();
  assert(array->is_objArray(), "expecting objArray");
  return array; 
}

oop deoptimizedVFrame::obj_at(int index) const {
  return frame_array->obj_at(offset + index);
}

bool deoptimizedVFrame::is_top() const {
  return offset + end_of_expressions() > frame_array->length();
}

int deoptimizedVFrame::end_of_expressions() const {
  smiOop n = smiOop(obj_at(locals_size_offset));
  assert(n->is_smi(), "expecting smi");
  return first_temp_offset + n->value();
}

deoptimizedVFrame::deoptimizedVFrame(const frame* fr) 
 : deltaVFrame(fr) {
  // the first frame in the array is located at position 3 (after #frames, #locals)
  this->offset      = StackChunkBuilder::first_frame_index;
  this->frame_array = retrieve_frame_array();
}

deoptimizedVFrame::deoptimizedVFrame(const frame* fr, int offset) 
 : deltaVFrame(fr) {
  this->offset      = offset;
  this->frame_array = retrieve_frame_array();
}

oop deoptimizedVFrame::receiver() const {
  return obj_at(receiver_offset);
}

methodOop deoptimizedVFrame::method() const {
  methodOop m = methodOop(obj_at(method_offset));
  assert(m->is_method(), "expecting method");
  return m;
}

int deoptimizedVFrame::bci() const {
  smiOop b = smiOop(obj_at(bci_offset));
  assert(b->is_smi(), "expecting smi");
  return b->value();
}

vframe* deoptimizedVFrame::sender() const {
  return is_top() ? vframe::sender()
                  : new deoptimizedVFrame(&_fr, offset + end_of_expressions());
}

bool deoptimizedVFrame::equal(const vframe* f) const {
  if (!f->is_deoptimized_frame()) return false;
  return vframe::equal(f) 
      && offset == ((deoptimizedVFrame*)f)->offset;
}

oop deoptimizedVFrame::temp_at(int offset) const {
  return obj_at(first_temp_offset + offset);
}

oop deoptimizedVFrame::expression_at(int index) const {
  return obj_at(end_of_expressions() - 1 - index);
}

oop deoptimizedVFrame::context_temp_at(int offset) const {
  assert(deoptimized_context(), "must have context when using context_temp_at");
  return deoptimized_context()->obj_at(offset);
}

contextOop deoptimizedVFrame::deoptimized_context() const {
  if (!method()->activation_has_context()) return NULL;
  contextOop result = contextOop(temp_at(0));
  assert(result->is_context(), "context type check");
  return result;
}

contextOop deoptimizedVFrame::canonical_context() const {
  return deoptimized_context();
}

GrowableArray<oop>* deoptimizedVFrame::expression_stack() const {
  int locals   = end_of_expressions() - first_temp_offset;
  int temps    = method()->number_of_stack_temporaries();
  int exp_size = locals - temps;

  GrowableArray<oop>* array = new GrowableArray<oop>(exp_size);
  for (int index = 0 ; index < exp_size; index++) {
    array->push(expression_at(index));
  }

#ifdef ASSERT
  int computed_size = method()->expression_stack_mapping(bci())->length();
  if (exp_size != computed_size)
    warning("Expression stack size is %d but computed to %d", exp_size, computed_size);
#endif

  return array;
}

#endif

// ------------- cVFrame --------------

void cVFrame::print() {
  vframe::print();
  std->print_cr("C frame");
}

void cVFrame::print_value() const {
  ((vframe*)this)->print();
}

// ------------- cChunk --------------

vframe* cChunk::sender() const {
  return cVFrame::sender();
}
 
void cChunk::print_value() const { 
  ((cChunk*)this)->print();
}

void cChunk::print() {
  vframe::print();
  std->print_cr("C Chunk inbetween Delta");
  std->print_cr("C     link 0x%lx", _fr.link());
}

oop cChunk::callee_argument_at(int index) const {
  return oop(_fr.sp()[index]); 
}

