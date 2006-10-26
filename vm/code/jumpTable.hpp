/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.17 $ */
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

// The jumpTable constitutes the interface between interpreter code and optimized code.
// This indirection makes it possible to invalidate optimized code without 
//  - keeping track of dependencies between sends in methodsOops and optimized code, or 
//  - traverse all methodOops. 
// If optimized code has become invalid the jump table entry is simply snapped.
// In addition the jumpTable serves as a dispatch table for block closures for optimized code.

// %implementation note:
//   make jumpTable growable

#ifdef DELTA_COMPILER

class jumpTableEntry;
class jumpTable;

class jumpTableID : ValueObj {
  u_short _major;
  u_short _minor;
  friend class jumpTable;

  enum { max_value = nthMask(16) };

 public:
  jumpTableID()                             : _major(max_value), _minor(max_value) {}
  jumpTableID(u_short major)                : _major(major),     _minor(max_value) {}
  jumpTableID(u_short major, u_short minor) : _major(major),     _minor(minor)     {}
  bool has_minor() const 		{ return _minor != max_value; }
  bool is_block() const 		{ return _minor > 0;          }
  bool is_valid() const 		{ return _major != max_value; }
  u_short major() const 		{ return _major; }
  u_short minor() const 		{ return _minor; }
  jumpTableID sub(u_short minor) const 	{ return jumpTableID(_major, minor); }
};


class jumpTable : public ValueObj {
 protected:
  int firstFree;	// index of first free elem
  static char*           allocate_jump_entries(int size);
  static jumpTableEntry* jump_entry_for_at(char* entries, int index);
  jumpTableEntry*        major_at(u_short index);
 public:
  char* entries;
  int length;		// max. number of IDs
  int usedIDs;		// # of used ID

 public:
  jumpTable();
  ~jumpTable();

  void  init();

  // Allocates a block of adjacent jump table entries.
  jumpTableID allocate(int number_of_entries);

  // returns the jumptable entry for id
  jumpTableEntry* at(jumpTableID id);

  int newID();	    	   // return a new ID
  int peekID();    	   // return value which would be returned by newID,
    	    	    	   // but don't actually allocate the ID
  void freeID(int index); // index is unused again

  void verify();
  void print();

  // compilation of blocks
  static char*	  compile_new_block(blockClosureOop blk);     // create nmethod, return entry point
  static nmethod* compile_block(blockClosureOop blk);	      // (re)compile block nmethod

  friend class jumpTableEntry;
};


// implementation note: jumpTableEntry should be an abstract class with two
// subclasses for nmethod and block entries, but these classes are combined
// in order to save space (no vtbl pointer needed)
class jumpTableEntry : public ValueObj {
 private:
  char* jump_inst_addr() const 		{ assert(oop(this)->is_smi(), "misaligned"); return (char*) this; }
  char* state_addr() const		{ return ((char*) this) + sizeof(char) + sizeof(int); }
  char  state() const			{ return *state_addr(); }
  void fill_entry(char instr, char* dest, char state);
  void initialize_as_unused(int index);
  void initialize_as_link(char* link);
  void initialize_nmethod_stub(char* dest);
  void initialize_block_closure_stub();
  inline jumpTableEntry* previous_stub() const;
  inline jumpTableEntry* next_stub() const;
  jumpTableEntry* parent_entry(int& index) const;
  void report_verify_error(char* message);
 public:
   // testing operations	    LARS: please add comments explaining what the 4 cases are  -Urs 4/96
  bool is_nmethod_stub() const;
  bool is_block_closure_stub() const;
  bool is_link() const;
  bool is_unused() const;

  // entry point
  char* entry_point() const 		{ return jump_inst_addr(); }

  // destination
  char** destination_addr() const; 	// the address of the destination
  char*  destination() const;		// current destination
  void   set_destination(char* dest); 	// sets the destination

  // operations for link stubs
  char* link() const;

  // operations for unused
  int next_free() const;

  // operations for nmethod stubs (is_nmethod_stub() == true)
  nmethod* method() const;        // NULL if not pointing to a method

  // operations for block stubs (is_block_closure_stub() == true)
  bool      block_has_nmethod() const;
  nmethod*  block_nmethod() const;  	// block nmethod (or NULL if not compiled yet)
  methodOop block_method() const;  	// block method
  // nmethod creating the blockClosureOops pointing to this entry
  // index is set to the distance |parent_entry - this|
  nmethod*  parent_nmethod(int& index) const; 

  // printing
  void print();
  void verify();

  // size of jump table entry
  static int size() 			{ return 8; }

  friend class jumpTable;
};

#endif
