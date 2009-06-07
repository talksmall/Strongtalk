/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.19 $ */
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

# include "incls/_nameDesc.cpp.incl"

void LocationNameDesc::print() {
  std->print("@%s (%d)", location().name(), offset);
}

void ValueNameDesc::print() {
  std->print("=");
  value()->print_value();
  std->print(" (%d)", offset);
}

void BlockValueNameDesc::print() {
  std->print("[=");
  block_method()->print_value();
  std->print("]");
  std->print(" (%d)", offset);
}

void MemoizedBlockNameDesc::print() {
  std->print("[@%s =", location().name());
  block_method()->print_value();
  std->print("]");
  std->print(" (%d)", offset);
}

void IllegalNameDesc::print() {
  std->print("###illegal###");
  std->print(" (%d)", offset);
}

bool LocationNameDesc::equal(NameDesc* other) const {
  if (other->isLocation()) {
    return location() == ((LocationNameDesc*) other)->location();
  }
  return false;
}

bool ValueNameDesc::equal(NameDesc* other) const {
  if (other->isValue()) {
    return value() == ((ValueNameDesc*) other)->value();
  }
  return false;
}

bool BlockValueNameDesc::equal(NameDesc* other) const {
  if (other->isBlockValue()) {
    return block_method() == ((BlockValueNameDesc*) other)->block_method()
        && parent_scope()->s_equivalent(((BlockValueNameDesc*) other)->parent_scope());
  }
  return false;
}

bool MemoizedBlockNameDesc::equal(NameDesc* other) const {
  if (other->isMemoizedBlock()) {
    return location()     == ((MemoizedBlockNameDesc*) other)->location()
        && block_method() == ((MemoizedBlockNameDesc*) other)->block_method()
        && parent_scope()->s_equivalent(((MemoizedBlockNameDesc*) other)->parent_scope());
  }
  return false;
}

bool IllegalNameDesc::equal(NameDesc* other) const {
  return other->isIllegal();
}

extern "C" blockClosureOop allocateBlock(smiOop nofArgs);

oop BlockValueNameDesc::value(const frame* fr) const {
  // create a block closure
  if (MaterializeEliminatedBlocks || StackChunkBuilder::is_deoptimizing()) {
    blockClosureOop blk = blockClosureOop(
	 allocateBlock(as_smiOop(block_method()->number_of_arguments())));
    blk->set_method(block_method());

    compiledVFrame* vf = compiledVFrame::new_vframe(fr, parent_scope(), 0);
    blk->set_lexical_scope(vf->canonical_context());
    return blk;
  } else {
    ResourceMark rm;
    stringStream st(50);
    st.print("eliminated [] in ");
    block_method()->home()->selector()->print_symbol_on(&st);
    return oopFactory::new_symbol(st.as_string());
  }
}

oop MemoizedBlockNameDesc::value(const frame* fr) const {
  // check if the block has been created
  compiledVFrame* vf = fr 
                     ? compiledVFrame::new_vframe(fr, parent_scope(), 0)
		     : NULL;

  oop stored_value = compiledVFrame::resolve_location(location(), vf);
  if (stored_value != uncreatedBlockValue()) {
    return stored_value;
  }
  
  // otherwise do the same as for a BlockValueNameDesc
  if (MaterializeEliminatedBlocks || StackChunkBuilder::is_deoptimizing()) {
    blockClosureOop blk = blockClosureOop(
	 allocateBlock(as_smiOop(block_method()->number_of_arguments())));
    blk->set_method(block_method());

    blk->set_lexical_scope(vf->canonical_context());
    return blk;
  } else {
    ResourceMark rm;
    stringStream st(50);
    st.print("memoized [] in ");
    block_method()->home()->selector()->print_symbol_on(&st);
    return oopFactory::new_symbol(st.as_string());
  }
}

#endif
