/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.59 $ */
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

# include "incls/_scopeDesc.cpp.incl"

char* ScopeDesc::invalid_pc = (char*) -1;

int compareBCI(int bci1, int bci2) {
  assert(bci1 != IllegalBCI && bci2 != IllegalBCI, "can't compare");
  return bci1 - bci2;
}

ScopeDesc::ScopeDesc(const nmethodScopes* scopes, int offset, char* pc) {
  _scopes     = scopes;
  _offset     = offset;
  _pc         = pc;

  _name_desc_offset = offset;

  scopeDescHeaderByte b;
  b.unpack(_scopes->get_next_char(_name_desc_offset));

  _lite              = b.is_lite();
  _has_temps         = b.has_temps();
  _has_context_temps = b.has_context_temps();
  _has_expr_stack    = b.has_expr_stack();

  assert(offset != 0 || !is_lite(), "Root scopeDesc cannot be lite");

  if (b.has_nameDescs()) {
    _next = _scopes->unpackValueAt(_name_desc_offset) + _offset;  
  } else {
    _next = -1;
  }

  if (_offset == 0) {
    _senderScopeOffset   = 0;
    _senderByteCodeIndex = IllegalBCI;
  } else {
    _senderScopeOffset   = _scopes->unpackValueAt(_name_desc_offset);  
    _senderByteCodeIndex = _scopes->unpackValueAt(_name_desc_offset);
  }
  _method  = methodOop(_scopes->unpackOopAt(_name_desc_offset));
  assert(_method->is_method(), "expecting a method");
  _allocates_compiled_context = b.has_compiled_context();
  _scopeID = _scopes->unpackValueAt(_name_desc_offset);  
}


ScopeDesc* ScopeDesc::home(bool cross_nmethod_boundary ) const {
  ScopeDesc* p = (ScopeDesc*)this;
  for ( ; p && !p->isMethodScope(); p = p->parent(cross_nmethod_boundary)) ;
  return p;
}


NameDesc* ScopeDesc::temporary(int index, bool canFail) {
  int pos = _name_desc_offset;
  NameDesc* result = NULL;
  if (_has_temps) {
    NameDesc* current = nameDescAt(pos);
    int i = 0;
    while (current != NULL) {
      if (i == index) { result = current; break; }
      current = nameDescAt(pos);
      i++;
    }
  }
  if (!result && !canFail) fatal1("couldn't find temporary %d", index);
  return result;
}


NameDesc* ScopeDesc::contextTemporary(int index, bool canFail) {
  int pos = _name_desc_offset;
  NameDesc* result = NULL;
  if (_has_temps) {
    NameDesc* current = nameDescAt(pos);
    while(current) {
      current = nameDescAt(pos);
    }
  }
  if (_has_context_temps) {
    NameDesc* current = nameDescAt(pos);
    int i = 0;
    while(current) {
      if (i == index) { result = current; break; }
      current = nameDescAt(pos);
      i++;
    }
  }
  if (!result && !canFail) fatal1("couldn't find context temporary %d", index);
  return result;
}


NameDesc* ScopeDesc::exprStackElem(int bci, bool includeTrivial) {
  int pos = _name_desc_offset;
  if (_has_temps) {
    NameDesc* current = nameDescAt(pos);
    while(current) {
      current = nameDescAt(pos);
    }
  }
  if (_has_context_temps) {
    NameDesc* current = nameDescAt(pos);
    while(current) {
      current = nameDescAt(pos);
    }
  }
  if (_has_expr_stack) {
    NameDesc* current = nameDescAt(pos);
    while(current) {
      int the_bci = valueAt(pos);
      if (bci == the_bci) return current;
      current = nameDescAt(pos);
    }
  }
  return NULL;
}


void ScopeDesc::iterate(NameDescClosure* blk) {
  int pos = _name_desc_offset;
  if (_has_temps) {
    NameDesc* current = nameDescAt(pos);
    int       number  = 0;
    while(current) {
      blk->temp(number++, current, pc());
      current = nameDescAt(pos);
    }
  }
  if (_has_context_temps) {
    NameDesc* current = nameDescAt(pos);
    int       number  = 0;
    while(current) {
      blk->context_temp(number++, current, pc());
      current = nameDescAt(pos);
    }
  }
  if (_has_expr_stack) {
    NameDesc* current = nameDescAt(pos);
    while(current) {
      blk->stack_expr(valueAt(pos), current, pc());
      current = nameDescAt(pos);
    }
  }
}


// Wrapper class for NameDescClosure, allows iteration over
// string of NameDescs with different pc information.

class IterationHelper: public UnpackClosure {
 protected:
  int              _no;
  NameDescClosure* _blk;
  bool             _is_used;

  void use()       { _is_used = true; }

 public:
  void init(int no, NameDescClosure* blk) {
    _no      = no;
    _blk     = blk;
    _is_used = false;
  }

  bool is_used() const { return _is_used; }
};

class IH_arg         : public IterationHelper { void nameDescAt(NameDesc* nd, char* pc) { use(); _blk->arg         (_no, nd, pc); } };
class IH_temp        : public IterationHelper { void nameDescAt(NameDesc* nd, char* pc) { use(); _blk->temp        (_no, nd, pc); } };
class IH_context_temp: public IterationHelper { void nameDescAt(NameDesc* nd, char* pc) { use(); _blk->context_temp(_no, nd, pc); } };
class IH_stack_expr  : public IterationHelper { void nameDescAt(NameDesc* nd, char* pc) { use(); _blk->stack_expr  (_no, nd, pc); } };



void ScopeDesc::iterate_all(NameDescClosure* blk) {
  int pos = _name_desc_offset;
  if (_has_temps) {
    int no = 0;
    IH_temp helper;
    do {
      helper.init(no++, blk);
      _scopes->iterate(pos, &helper);
    } while (helper.is_used());
  }
  if (_has_context_temps) {
    int no = 0;
    IH_context_temp helper;
    do {
      helper.init(no++, blk);
      _scopes->iterate(pos, &helper);
    } while (helper.is_used());
  }
  if (_has_expr_stack) {
    int no = 0;
    IH_stack_expr helper;
    do {
      helper.init(no++, blk);
      _scopes->iterate(pos, &helper);
      if (helper.is_used()) valueAt(pos); // get bci (i.e., print-out is showing expr. index and not bci)
    } while (helper.is_used());
  }
}


bool ScopeDesc::allocates_interpreted_context() const {
  return method()->allocatesInterpretedContext();
}


NameDesc* ScopeDesc::compiled_context() {
  assert(allocates_compiled_context(), "must allocate a context");
  const int temporary_index_for_context = 0;
  return temporary(temporary_index_for_context);
}


bool ScopeDesc::s_equivalent(ScopeDesc* s) const {
  return method() == s->method()
     && (_senderByteCodeIndex == s->_senderByteCodeIndex ||
	 _senderByteCodeIndex < 0 || s->_senderByteCodeIndex < 0);
         // don't check senderByteCodeIndex for pseudo BCIs
}


bool ScopeDesc::l_equivalent(LookupKey* l) const {
  return selector() == l->selector();
}


ScopeDesc* ScopeDesc::sender() const {
  return _senderScopeOffset ? _scopes->at(_offset - _senderScopeOffset, pc()) : NULL; 
}


NameDesc* ScopeDesc::nameDescAt(int& offset) const {
  return _scopes->unpackNameDescAt(offset, pc());
}


int ScopeDesc::valueAt(int& offset) const {
  return _scopes->unpackValueAt(offset);
}


bool ScopeDesc::verify() {
  // verifies mostly structure, not contents
  bool ok = true;

  // don't do a full verify of parent/sender -- too many redundant verifies
  ScopeDesc* s = sender();
  if (s && !s->shallow_verify()) {
    std->print("invalid sender %#lx of ScopeDesc %#lx", s, this);
    ok = false;
  }
  ScopeDesc* p = parent();
  if (p && !p->shallow_verify()) {
    std->print("invalid parent %#lx of ScopeDesc %#lx", p, this);
    ok = false;
  }
  return ok;
}


// verify expression stack at a call or primitive call
void ScopeDesc::verify_expression_stack(int bci) {
  GrowableArray<int>* mapping = method()->expression_stack_mapping(bci);
  for (int index = 0; index < mapping->length(); index++) {
    NameDesc* nd    = exprStackElem(mapping->at(index));
    if (nd == NULL) { warning("expression not found in nmethod"); continue; }
    // Fix this Lars (add parameter for checking registers
    // if (nd->hasLocation() && nd->location().isRegisterLocation()) {
    //   print(); nd->print(); method()->print_codes();
    //   error("expr stack element is in register at call at bci %d", bci);
    // }
  }
}


class PrintNameDescClosure: public NameDescClosure {
 private:
  int   _indent;
  char* _pc0;

  void print(char* title, int no, NameDesc* nd, char* pc) {
    std->fill_to(_indent);
    if (UseNewBackend) {
      std->print("%5d: ", pc - _pc0);
    }
    std->print("%s[%d]\t", title, no); nd->print(); std->cr();
  }

 public:
  PrintNameDescClosure(int indent, char* pc0)      { _indent = indent; _pc0 = pc0; }

  void arg         (int no, NameDesc* a, char* pc) { print("arg   ", no, a, pc); }
  void temp        (int no, NameDesc* t, char* pc) { print("temp  ", no, t, pc); }
  void context_temp(int no, NameDesc* c, char* pc) { print("c_temp", no, c, pc); }
  void stack_expr  (int no, NameDesc* e, char* pc) { print("expr  ", no, e, pc); }
};


void ScopeDesc::print(int indent, bool all_pcs) {
  std->fill_to(indent);
  printName();
  std->print("ScopeDesc @%d%s: ", offset(), is_lite() ? ", lite" : "");
  std->print(" (ID %ld) ", scopeID());
  method()->selector()->print_symbol_on();
  std->print(" %#x", method());
  std->cr();
  ScopeDesc* s = sender();
  if (s != NULL) {
    std->fill_to(indent);
    std->print("sender: (%d) @ %ld", s->offset(), long(senderBCI()));
  }
  ScopeDesc* p = parent();
  if (p != NULL) {
    if (s != NULL) {
      std->print("; ");
    } else {
      std->fill_to(indent);
    }
    std->print("parent: (%d)", p->offset());
  }
  if (s != NULL || p != NULL) {
    std->cr();
  }
  std->fill_to(indent);
  printSelf();
  PrintNameDescClosure blk(indent+2, _scopes->my_nmethod()->insts());
  if (all_pcs) {
    iterate_all(&blk);
  } else {
    iterate(&blk);
  }
}


void ScopeDesc::print_value_on(outputStream* st) const {
  // print offset
  if (WizardMode) 
    st->print(" [%d]", offset());
}


bool MethodScopeDesc::s_equivalent(ScopeDesc* s) const {
  return s->isMethodScope() 
      && ScopeDesc::s_equivalent(s)
      && key()->equal(((MethodScopeDesc*)s)->key());
}


bool MethodScopeDesc::l_equivalent(LookupKey* l) const {
  return ScopeDesc::l_equivalent(l) && selfKlass() == l->klass();
}


MethodScopeDesc::MethodScopeDesc(const nmethodScopes* scopes, int offset, char* pc)
  : ScopeDesc(scopes, offset, pc), _key() {
  oop k = _scopes->unpackOopAt(_name_desc_offset);
  oop s = _scopes->unpackOopAt(_name_desc_offset);
  _key.initialize((klassOop) k, s);
  _self_name = _scopes->unpackNameDescAt(_name_desc_offset, pc);
  if (_next == -1) _next = _name_desc_offset;
}


void MethodScopeDesc::printName() {
  std->print("Method");
}


void MethodScopeDesc::printSelf() {
  printIndent();
  std->print("self: ");
  self()->print();
  std->cr();
}


void MethodScopeDesc::print_value_on(outputStream* st) const {
  key()->print_on(st);
  ScopeDesc::print_value_on(st);
}


void BlockScopeDesc::printSelf() {
  ScopeDesc::printSelf();
  std->cr();
}


BlockScopeDesc::BlockScopeDesc(const nmethodScopes* scopes, int offset, char* pc)
  : ScopeDesc(scopes, offset, pc) {
  _parentScopeOffset = _scopes->unpackValueAt(_name_desc_offset);
  if (_next == -1) _next = _name_desc_offset;
}


bool BlockScopeDesc::s_equivalent(ScopeDesc* s) const {
  return s->isBlockScope() && ScopeDesc::s_equivalent(s);
}


void BlockScopeDesc::printName() {
  std->print("Block");
}


klassOop BlockScopeDesc::selfKlass() const {
  ScopeDesc* p = parent();
  return p ? p->selfKlass() : NULL;
}


NameDesc* BlockScopeDesc::self() const {
  ScopeDesc* p = parent();
  return p ? p->self() : NULL;
}


ScopeDesc* BlockScopeDesc::parent(bool cross_nmethod_boundary) const {
  return _parentScopeOffset ? _scopes->at(_offset - _parentScopeOffset, pc()) : NULL; 
}


void BlockScopeDesc::print_value_on(outputStream* st) const {
  st->print("block {parent %d}", _offset - _parentScopeOffset);
  ScopeDesc::print_value_on(st);
}


LookupKey* BlockScopeDesc::key() const {
  return LookupKey::allocate(selfKlass(), method());
}


LookupKey* TopLevelBlockScopeDesc::key() const {
  return LookupKey::allocate(selfKlass(), method());
}


NonInlinedBlockScopeDesc::NonInlinedBlockScopeDesc(const nmethodScopes* scopes, int offset) {
  _offset            = offset;
  _scopes            = scopes;

  scopeDescHeaderByte b;
  b.unpack(_scopes->get_next_char(offset));
  _method            = methodOop(scopes->unpackOopAt(offset));
  _parentScopeOffset = scopes->unpackValueAt(offset);
}


void NonInlinedBlockScopeDesc::print() {
  std->print("NonInlinedBlockScopeDesc\n");
  std->print(" - method: "); 
  method()->print_value();
  std->cr();
  std->print(" - parent offset: %d\n", _parentScopeOffset);
}


ScopeDesc* NonInlinedBlockScopeDesc::parent() const { 
  return _parentScopeOffset ? _scopes->at(_offset - _parentScopeOffset, ScopeDesc::invalid_pc) : NULL;
}


TopLevelBlockScopeDesc::TopLevelBlockScopeDesc(const nmethodScopes* scopes, int offset, char* pc)
  : ScopeDesc(scopes, offset, pc) {
  _self_name  = _scopes->unpackNameDescAt(_name_desc_offset, pc);
  _self_klass = klassOop(scopes->unpackOopAt(_name_desc_offset));
  if (_next == -1) _next = _name_desc_offset;
}


void TopLevelBlockScopeDesc::printSelf() {
  ScopeDesc::printSelf(); 
  std->print("self: ");
  self()->print();
  std->cr();
}


ScopeDesc* TopLevelBlockScopeDesc::parent(bool cross_nmethod_boundary) const {
  if (!cross_nmethod_boundary) return NULL;
  nmethod* nm = _scopes->my_nmethod();
  int index;
  nmethod* parent = nm->jump_table_entry()->parent_nmethod(index); 
  NonInlinedBlockScopeDesc* scope = parent->noninlined_block_scope_at(index);
  return scope->parent();
}


bool TopLevelBlockScopeDesc::s_equivalent(ScopeDesc* s) const {
  // programming can tranform a nested block to a top-level block
  return s->isBlockScope() && ScopeDesc::s_equivalent(s);
}


void TopLevelBlockScopeDesc::printName() {
  std->print("TopLevelBlock");
}


void TopLevelBlockScopeDesc::print_value_on(outputStream* st) const {
  st->print("top block");
  ScopeDesc::print_value_on(st);
}


Expr* ScopeDesc::selfExpr(PReg* p) const {
  klassOop self_klass = selfKlass();
  if (self_klass == trueObj->klass())  return new ConstantExpr(trueObj,  p, NULL);
  if (self_klass == falseObj->klass()) return new ConstantExpr(falseObj, p, NULL);
  if (self_klass == nilObj->klass())   return new ConstantExpr(nilObj,   p, NULL);
  return new KlassExpr(self_klass, p, NULL);
}


#endif

