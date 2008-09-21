/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.32 $ */
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

# include "incls/_nmethodScopes.cpp.incl"

ScopeDesc* nmethodScopes::at(int offset, char* pc) const {
  // Read the first byte and decode the ScopeDesc type at the location.
  assert(offset >= 0, "illegal desc offset");
  scopeDescHeaderByte b;
  b.unpack(peek_next_char(offset));
  switch (b.code()) {
    case METHOD_CODE          : return new MethodScopeDesc(       (nmethodScopes*) this, offset, pc);
    case TOPLEVELBLOCK_CODE   : return new TopLevelBlockScopeDesc((nmethodScopes*) this, offset, pc);
    case BLOCK_CODE           : return new BlockScopeDesc(        (nmethodScopes*) this, offset, pc);
    case NONINLINED_BLOCK_CODE: return NULL;
  }
  fatal("Unknown ScopeDesc code in nmethodScopes");
  return NULL;
}


NonInlinedBlockScopeDesc* nmethodScopes::noninlined_block_scope_at(int offset) const {
  // Read the first byte and decode the ScopeDesc type at the location.
  assert(offset > 0, "illegal desc offset");
  scopeDescHeaderByte b;
  b.unpack(peek_next_char(offset));
  if ( b.code() != NONINLINED_BLOCK_CODE) {
    fatal("Not an noninlined scope desc as expected");
  }
  return new NonInlinedBlockScopeDesc((nmethodScopes*)this, offset);
}


int16 nmethodScopes::get_next_half(int& offset) const {
  int16 v;
  v = get_next_char(offset) << BYTE_WIDTH;
  v = addBits(v, get_next_char(offset));
  return v;
}


inline u_char nmethodScopes::getIndexAt(int& offset) const { 
  return get_next_char(offset);
}


inline oop nmethodScopes::unpackOopFromIndex(u_char index, int& offset) const {
  if (index == 0)  return 0;
  if (index < EXTENDED_INDEX) return oop_at(index-1);
  return oop_at(get_next_half(offset)-1); 
}


inline int nmethodScopes::unpackValueFromIndex(u_char index, int& offset) const {
  if (index <= MAX_INLINE_VALUE) return index;
  if (index < EXTENDED_INDEX) return value_at(index-(MAX_INLINE_VALUE+1));
  return value_at(get_next_half(offset)-(MAX_INLINE_VALUE+1)); 
}


oop nmethodScopes::unpackOopAt(int& offset) const {
  u_char index = getIndexAt(offset);
  return unpackOopFromIndex(index, offset);
}


int nmethodScopes::unpackValueAt(int& offset) const {
  u_char index = getIndexAt(offset);
  return unpackValueFromIndex(index, offset);
}


NameDesc* nmethodScopes::unpackNameDescAt(int& offset, bool& is_last, char* pc) const {
  int startOffset = offset;
  nameDescHeaderByte b;
  b.unpack(get_next_char(offset));
  is_last = b.is_last();
  NameDesc* nd;
  if (b.is_illegal()) {
    nd = new IllegalNameDesc;
  } else if (b.is_termination()) {
    return NULL;
  } else {
    u_char index;
    index = b.has_index() ? b.index() : getIndexAt(offset);

    switch(b.code()) {
     case LOCATION_CODE: {
       Location l = Location(unpackValueFromIndex(index, offset));
       nd = new LocationNameDesc(l);
       break;
     }
     case VALUE_CODE: {
       oop v = unpackOopFromIndex(index, offset);
       nd = new ValueNameDesc(v);
       break;
     }
     case BLOCKVALUE_CODE: {
       oop blkMethod    = unpackOopFromIndex(index, offset);
       assert(blkMethod->is_method(), "must be a method");
       int parent_scope_offset = unpackValueAt(offset);
       ScopeDesc* parent_scope = at(parent_scope_offset, pc);
       nd = new BlockValueNameDesc(methodOop(blkMethod), parent_scope);
       break;
     }
     case MEMOIZEDBLOCK_CODE: {
       Location l   = Location(unpackValueFromIndex(index, offset));
       oop      blkMethod = unpackOopAt(offset);
       assert(blkMethod->is_method(), "must be a method");
       int parent_scope_offset = unpackValueAt(offset);
       ScopeDesc* parent_scope = at(parent_scope_offset, pc);
       nd = new MemoizedBlockNameDesc(l, methodOop(blkMethod), parent_scope);
       break;
     }
     default:
      fatal1("no such name desc (code %d)", b.code());
    }
  }
  nd->offset = startOffset;
  return nd;
}


void nmethodScopes::iterate(int& offset, UnpackClosure* closure) const {
  char* pc = my_nmethod()->insts();
  bool is_last;
  NameDesc* nd = unpackNameDescAt(offset, is_last, ScopeDesc::invalid_pc);
  if (nd == NULL) return;		// if at termination byte
  closure->nameDescAt(nd, pc);
  while (!is_last) {
    nd  = unpackNameDescAt(offset, is_last, ScopeDesc::invalid_pc);
    pc += unpackValueAt(offset);
    closure->nameDescAt(nd, pc);
  }
}


NameDesc* nmethodScopes::unpackNameDescAt(int& offset, char* pc) const {
  int pc_offset = pc - my_nmethod()->insts();
  int current_pc_offset = 0;
  bool is_last;
  NameDesc* result = unpackNameDescAt(offset, is_last, pc);
  if (result == NULL) return NULL;	// if at termination byte
  while (!is_last) {
    NameDesc* current  = unpackNameDescAt(offset, is_last, pc);
    current_pc_offset += unpackValueAt(offset);
    if (current_pc_offset <= pc_offset) {
      result = current;
    }
  }
  return result;
}


# define FOR_EACH_OOPADDR(VAR)						      \
    for (oop* VAR = oops(), *CONC(VAR, _end) = oops() + oops_size();	      \
         VAR < CONC(VAR, _end); VAR++)


void nmethodScopes::verify() {
  // Verify all oops
  FOR_EACH_OOPADDR(addr) {
    VERIFY_TEMPLATE(addr)
  }

  // Verify all scopedesc
  FOR_EACH_SCOPE(this, s) {
    if (!s->verify())
      lprintf("\t\tin nmethod at %#lx (scopes)\n", my_nmethod()); 
  }
}


void nmethodScopes::scavenge_contents() {
  FOR_EACH_OOPADDR(addr) {
    SCAVENGE_TEMPLATE(addr);
  }
}


void nmethodScopes::switch_pointers(oop from, oop to,
				    GrowableArray<nmethod*> *nmethods_to_invalidate) {
  Unused(to);
/*
  This is tricky!
  First, since some inlined methods are not included in scopes
  (those that generate no code such as asSmallInteger),
  you might think that this would
  not be needed, since memory is swept and dependencies flushed
  (see space::switch_pointers_in_region).

  But, when nmethods are converted on the stack, zombie nmethods are
  produced. These are obsolete nmethods that carry on the execution of
  active but no longer referenced methods on the stack.
  Since they have no dependencies, they are not found from the heap.
  That is why this code is needed anyway.

  Now, since this info describes the code, you cannot change it,
  instead, you invalidate the nmethod if it has a ref to from in its scopes.
  For example the method oops in the scope determine the activations
  that might be on the stack, and you can't change these, because
  activations are clones of methods. This may be the last reference to
  a currently executing method, must keep it around.

  However, you may be confused by the fact that locs (embedded literals)
  in the code in the method are changed. But, those are just object literals,
  maps of inlined stuff, and are different than the scope oops.

  Although they could possibly be the same, by invalidating any
  match (beyond just the method holder and method) we are safe.
*/

# ifdef NOT_IMPLEMENTED
  if (my_nmethod()->isInvalid()) return;
  
  FOR_EACH_OOPADDR(addr) {
    if (*addr == from) {
      nmethods_to_invalidate->append(my_nmethod());
	return;
      }
  }
# endif
}


void nmethodScopes::oops_do(void f(oop*)) {
  oop* end = oops() + oops_size();
  for (oop* p = oops(); p < end; p++) {
    f(p);
  }
}


void nmethodScopes::relocate() {
  FOR_EACH_OOPADDR(addr) {
    RELOCATE_TEMPLATE(addr);
  }
}


bool nmethodScopes::is_new() const {
  bool result = false;
  FOR_EACH_OOPADDR(addr) {
    if ((*addr)->is_new()) return true;
  }
  return false;
}


void nmethodScopes::print() {
  ResourceMark m;	// in case methods get printed via debugger
  printIndent();
  lprintf("scopes:\n");
  Indent ++;
  FOR_EACH_SCOPE(this, d)
    d->print();
  Indent --;
}


void nmethodScopes::print_partition() {
  int d_size = dependent_length() * sizeof(oop);
  int o_size = oops_size() * sizeof(oop) - d_size;
  int p_size = (int) pcsEnd() - (int) pcs();
  int v_size = value_size() * sizeof(int);
  int total  = v_size + p_size + o_size + d_size;
 
  std->print_cr("{deps %d%%, oops %d%%, bytes %d%%, pcs %d%%}",
                d_size * 100/ total,
                o_size * 100/ total,
                v_size * 100/ total,
                p_size * 100/ total);
}

#endif
