/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.43 $ */
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

// A blockClosureOop is the oop of a block closure, i.e., the object created
// when a block literal like [3 + 4] is evaluated.  Blocks can be "clean"
// (have no free variables, like [3 + 4]), in which case their _lexical_scope
// pointer contains nil or self, or they can be full blocks (like [x + 4])
// in which case their _lexical_scope is used to resolve accesses
// to free variables.

class blockClosureOopDesc: public memOopDesc {
 private:
  oop _methodOrJumpAddr;		// block method (if interpreted), jumpTable stub address (if compiled)
  contextOop     _lexical_scope;	// lexical context or nil (if no free variables)
  blockClosureOop addr() const 		{ return blockClosureOop(memOopDesc::addr()); }
 public:
  // field offsets for code generation
  static int method_or_entry_byte_offset() { return (2 * oopSize) - Mem_Tag; }
  static int context_byte_offset()	   { return (3 * oopSize) - Mem_Tag; }

  friend blockClosureOop as_blockClosureOop(void* p) { return blockClosureOop(as_memOop(p)); }

  static blockClosureOop create_clean_block(int nofArgs, char* entry_point);	// create a clean block

  inline bool isCompiledBlock() const { return !oop(addr()->_methodOrJumpAddr)->is_mem(); }
  void set_method(methodOop m)        { STORE_OOP(&addr()->_methodOrJumpAddr, m); }
  void set_jumpAddr(void* jmp_addr)   {
    assert(!oop(jmp_addr)->is_mem(), "not properly aligned");
    addr()->_methodOrJumpAddr = (oop)jmp_addr;
  }
  methodOop method() const;

  #ifdef DELTA_COMPILER
  jumpTableEntry* jump_table_entry() const;
  #endif

  // returns the number of arguments for the method oop belonging to this closure
  int number_of_arguments();

  // sizing
  static int header_size() 		{ return sizeof(blockClosureOopDesc)/oopSize; }
  static int object_size() 		{ return header_size(); }

  void set_lexical_scope(contextOop l)	{ STORE_OOP(&addr()->_lexical_scope, l); }
  contextOop lexical_scope() const	{ return addr()->_lexical_scope; }

  bool is_pure() const;

  // deoptimization
  void deoptimize();

  char* name() const 			{ return "blockClosure"; }
  void verify();

  friend blockClosureKlass;
};

// Contexts contain the heap-allocated local variables of a method, i.e., the locals
// and arguments that are uplevel-accessed by blocks.  They are variable-length, what's
// shown below is just the common prefix which is followed by the words containing the
// actual data.

class contextOopDesc: public memOopDesc {
 private:
  smiOop _parent;
  //
  // %note: Robert please describe the parent states in excruciating details.
  //        The description below is far from complete (Lars, 1/9/96).
  //
  // Contains either:
  //  - the frame   	(if the activation creating the block is alive and a first-level block)
  //  - smiOop_zero 	(when the activation creating the block is dead)
  //  - outer context 	(if the corresponding method is a block method??)
  // The transition from frame to smiOop_zero happens when the block is zapped
  // by the epilog code of the method or a non local return.
  // NOTE: the frame is needed in case of a non local return.
  contextOop addr() const 		{ return contextOop(memOopDesc::addr()); }
 public:
  friend contextOop as_contextOop(void* p) { return contextOop(as_memOop(p)); }

  void set_parent(oop h) 		{ STORE_OOP(&addr()->_parent, h); }
  oop  parent() const 			{ return addr()->_parent; }

  // Test operations on home
  bool is_dead() const;
  bool has_parent_fp() const;
  bool has_outer_context() const;

  int* parent_fp() const		{ return has_parent_fp() ? (int*) parent() : NULL; }
  void set_home_fp(int* fp) {
    assert(oop(fp)->is_smi(), "checking alignment");
    set_parent(oop(fp));
  } 

  // Returns the outer context if any
  contextOop outer_context() const;     // NULL if is_dead or has_frame

  // Sets the home to smiOop_zero
  void kill()				{ set_parent(smiOop_zero); }

  static int header_size() 		{ return sizeof(contextOopDesc)/oopSize; }
  int object_size() 			{ return header_size() + length(); }

  oop* obj_addr_at(int index)           { return oops(header_size() + index); }
  oop  obj_at(int index) 		{ return raw_at(header_size() + index); }
  void obj_at_put(int index, oop value) { raw_at_put(header_size() + index, value); }
  int length() 				{ return mark()->hash() - 1; }

  // constants for code generation
  static int parent_word_offset() 	{ return 2; }	// word offset of parent context
  static int temp0_word_offset()	{ return 3; }	// word offset of first context temp
  static int parent_byte_offset()	{ return byteOffset(parent_word_offset()); }
  static int temp0_byte_offset() 	{ return byteOffset(temp0_word_offset()); }

  // Accessors for storing and reading the forward reference
  // to the unoptimized context (Used during deoptimization).
  void   set_unoptimized_context(contextOop con);
  contextOop unoptimized_context();

  // Returns the length of the context chain.
  int chain_length() const;

  // Print the contents of home
  void print_home_on(outputStream* st);

  friend contextKlass;
};

