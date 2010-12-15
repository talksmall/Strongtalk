/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.33 $ */
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

# include "incls/_jumpTable.cpp.incl"

static const char halt_instruction = '\xF4';
static const char jump_instruction = '\xE9';


char* jumpTable::allocate_jump_entries(int size) {
//  return AllocateHeap(size * jumpTableEntry::size(), "jump table");
  return os::exec_memory(size * jumpTableEntry::size()); //, "jump table");
}

jumpTableEntry* jumpTable::jump_entry_for_at(char* entries, int index) {
  return (jumpTableEntry*) &entries[index * jumpTableEntry::size()];
}

jumpTable::jumpTable() {
  length  = Universe::current_sizes.jump_table_size;
  assert(length < 32 * K, "must change code to handle >32K entries");
  entries = allocate_jump_entries(length);
  init();
}

jumpTableID jumpTable::allocate(int number_of_entries) {
  int id                = newID();
  jumpTableEntry* entry = major_at(id);

  assert(entry->is_unused(), "cannot allocate used jump entry");
  if (number_of_entries == 1) {
    entry->initialize_nmethod_stub(NULL);
    return jumpTableID(id);
  } else {
    char* new_block = allocate_jump_entries(number_of_entries);
    // Initialize the first entry as a nmethod stub
    jump_entry_for_at(new_block, 0)->initialize_nmethod_stub(NULL);
    // initialize the rest as block closure stubs
    for(int i = 1; i < number_of_entries; i++) {
      jump_entry_for_at(new_block, i)->initialize_block_closure_stub();
    }
    entry->initialize_as_link(new_block);
    return jumpTableID(id,0);
  }
}

jumpTable::~jumpTable() { free(entries); }

jumpTableEntry* jumpTable::major_at(u_short index) { 
  return jump_entry_for_at(entries, index);
}

jumpTableEntry* jumpTable::at(jumpTableID id) {
  assert(id.is_valid(), "invalid ID");
  if (!id.has_minor()) return major_at(id.major());
  return jump_entry_for_at(major_at(id._major)->link(), id._minor);
}

void jumpTable::init() {
  // free list: firstFree keeps first free index, entries[firstFree] keeps index
  // of next free element, etc.
  firstFree = usedIDs = 0;
  for (int i = 0; i < length; i++)
    major_at(i)->initialize_as_unused(i+1);
}

int jumpTable::newID() {
  int id = firstFree;
  if (id >= length - 2) fatal("grow not implemented");
  firstFree = major_at(firstFree)->next_free();
  usedIDs++;
  return id;
}

int jumpTable::peekID() {
  return firstFree;
}

void jumpTable::freeID(int index) {
  assert(index >= 0 && index < length && index != firstFree, "invalid ID");
  if (major_at(index)->is_link()) {
    // free the chunk 
    free(major_at(index)->link());
  }
  major_at(index)->initialize_as_unused(firstFree);
  firstFree = index;
  usedIDs--;
  assert(usedIDs >= 0, "freed to many IDs");
}

void jumpTable::print() {
  std->print_cr("jumpTable %#lx: capacity %ld (%ld used)", this, length, usedIDs);
  for (int i = 0; i < length; i++) {
    if (!major_at(i)->is_unused()) {
      std->print(" %3d: ", i);
      major_at(i)->print();
    }
  }
}

char* jumpTable::compile_new_block(blockClosureOop blk) {
  // Called from the compile_block stub routine (see StubRoutines)
  BlockScavenge bs;
  ResourceMark rm;
  nmethod* nm = compile_block(blk);

  // return the entry point for the new nmethod.
  return nm->entryPoint();
}

nmethod* jumpTable::compile_block(blockClosureOop closure) {
  // compute the scope for noninlined block
  int index;
  nmethod* parent = closure->jump_table_entry()->parent_nmethod(index); 
  NonInlinedBlockScopeDesc* scope = parent->noninlined_block_scope_at(index);

  // save it in case it gets flushed during allocation!
  jumpTableEntry* jumpEntry = closure->jump_table_entry();

  // compile the top-level block nmethod
  VM_OptimizeBlockMethod op(closure, scope);
  VMProcess::execute(&op);
  nmethod* nm = op.method();

  // patch the jump entry with the entry point of the compiled nmethod
  jumpEntry->set_destination(nm->entryPoint());

  jumpTableEntry* b = nm->jump_table_entry();
  assert(jumpEntry == b, "jump table discrepancy");
  return nm;
}

static const char nmethod_entry        = 0;
static const char block_closure_entry  = 1;
static const char link_entry           = 2;
static const char unused_entry         = 3;

inline jumpTableEntry* jumpTableEntry::previous_stub() const {
  return (jumpTableEntry*) (((char*) this) - size());
}

inline jumpTableEntry* jumpTableEntry::next_stub() const {
  return (jumpTableEntry*) (((char*) this) + size());
}
void jumpTableEntry::fill_entry(char instr, char* dest, char state) {
  *jump_inst_addr()   = instr;
  *destination_addr() = dest;
  *state_addr()       = state;
}

void jumpTableEntry::initialize_as_unused(int index) {
  fill_entry(halt_instruction, (char*) index, unused_entry);
}

void jumpTableEntry::initialize_as_link(char* link) {
  fill_entry(halt_instruction, link, link_entry);
}

void jumpTableEntry::initialize_nmethod_stub(char* dest) {
  fill_entry(jump_instruction, dest - (int) state_addr(), nmethod_entry);
}

void jumpTableEntry::initialize_block_closure_stub() {
  fill_entry(jump_instruction,
	     StubRoutines::compile_block_entry() - (int) state_addr(),
             block_closure_entry);
}

bool jumpTableEntry::is_nmethod_stub() const { 
  return state() == nmethod_entry;
}

bool jumpTableEntry::is_block_closure_stub() const {
  return state() == block_closure_entry;
}

bool jumpTableEntry::is_unused() const { 
  return state() == unused_entry;
}

bool jumpTableEntry::is_link() const { 
  return state() == link_entry;
}

char* jumpTableEntry::link() const {
  return *destination_addr();
}

char** jumpTableEntry::destination_addr() const { 
  return (char**) (((char*) this) + sizeof(char));
}

char* jumpTableEntry::destination() const {
  return *destination_addr() + (int) state_addr();
}

void jumpTableEntry::set_destination(char* dest) {
  *destination_addr() = dest - (int) state_addr();
}

int jumpTableEntry::next_free() const {
  assert(is_unused(), "must be a unused entry");
  return (int) *destination_addr();
}

nmethod* jumpTableEntry::method() const {
  assert(is_nmethod_stub(), "must be a nmethod_stub");
  return Universe::code->findNMethod(destination());
}

bool jumpTableEntry::block_has_nmethod() const {
  assert(is_block_closure_stub(), "must be a block_closure_stub");
  return destination() != StubRoutines::compile_block_entry();
}

nmethod* jumpTableEntry::block_nmethod() const {
  assert(is_block_closure_stub(), "must be a block_closure_stub");
  if (block_has_nmethod()) {
    return Universe::code->findNMethod(destination());
  } else {
    return NULL;
  }
}

methodOop jumpTableEntry::block_method() const {
  assert(is_block_closure_stub(), "must be a block_closure_stub");
  if (block_has_nmethod()) {
    nmethod* nm = Universe::code->findNMethod(destination());
    assert(nm != NULL, "nmethod must exists");
    return nm->method();
  } else {
    int index;
    jumpTableEntry* pe = parent_entry(index);
    // find methodOop inside the nmethod:
    return pe->method()->noninlined_block_method_at(index);
  }
}

jumpTableEntry* jumpTableEntry::parent_entry(int& index) const {
  assert(is_block_closure_stub(), "must be a block_closure_stub");
  // search back in the jump table to find the nmethod stub
  // responsible for this block closure stub.
  jumpTableEntry* result = (jumpTableEntry*)this;
  index = 0;
  do {
    index++;
    result = result->previous_stub();
  }
  while(result->is_block_closure_stub());
  return result;
}

nmethod* jumpTableEntry::parent_nmethod(int& index) const {
  return parent_entry(index)->method();
}

void jumpTableEntry::print() {
  if (is_unused()) { 
    std->print_cr("Unused {next = %d}", (int) destination());
    return;
  }
  if (is_nmethod_stub()) {
    std->print("Nmethod stub ");
    Disassembler::decode(jump_inst_addr(), state_addr());
    nmethod* nm = method();
    if (nm) {
      nm->key.print();
    } else {
      std->print_cr("{not pointing to nmethod}");
    }
    return;
  }

  if (is_block_closure_stub()) {
    std->print("Block closure stub");
    Disassembler::decode(jump_inst_addr(), state_addr());
    nmethod* nm = block_nmethod();
    if (nm) {
      nm->key.print();
    } else {
      std->print_cr("{not compiled yet}");
    }
    return;
  }

  if (is_link()) {
    std->print_cr("Link for:");
    jumpTable::jump_entry_for_at(link(), 0)->print();
    return;
  }

  fatal("unknown jump table entry");
}

void jumpTableEntry::report_verify_error(char* message) {
  error("jumpTableEntry %#x: %s", this, message);
}

void jumpTableEntry::verify() {
  if (is_unused()) {
    // Nothing to check in an unused entry
    return;
  }

  if (is_nmethod_stub()) {
    // Check nmethod
    char* addr = destination();
    if (!Universe::code->contains(addr)) report_verify_error("nmethod not in zone");
    if (method()->entryPoint() != addr)  report_verify_error("destination doesn't point to beginning of nmethod");
    return;
  }

  if (is_link()) {
    // Verify the elements in the list {nmethod} {block_closure}+
    jumpTableEntry* head = jumpTable::jump_entry_for_at(link(), 0);
    if (!head->is_nmethod_stub()) report_verify_error("must be nmethod stub");
    head->verify();
    nmethod* nm = method();
    if (!nm->has_noninlined_blocks()) report_verify_error("nmethod must have noninlined blocks");
    for (int index = 1; index <= nm->number_of_noninlined_blocks(); index++) {
       jumpTableEntry* son = jumpTable::jump_entry_for_at(link(), index);
       if (!son->is_block_closure_stub()) report_verify_error("must be block closure stub");
       son->verify();
    }
    return;
  }

  if (is_block_closure_stub()) {
    char* addr = destination();
    if (!Universe::code->contains(addr)) {
      if (addr != StubRoutines::compile_block_entry())
        report_verify_error("destination points neither into zone nor to compile stub");
    } else {
      nmethod* nm = block_nmethod();
      if (nm->entryPoint() != addr) 
        report_verify_error("destination doesn't point to beginning of nmethod");
    }
    return;
  }

  report_verify_error("invalid state");
}

void jumpTable::verify() {
/*
  ResourceMark rm;
  int prev = -1;
  bool* check = NEW_RESOURCE_ARRAY(bool, n);
  for (int i = 0; i < n; i++) check[i] = false;
  for (int id = firstFree, j = 0; j < n - usedIDs; id = data[id], j++) {
    if (id < 0 || id >= n) {
      error("jumpTable: invalid ID %ld in free list (#%ld)\n", id, j);
    }
    if (check[id]) {
      error("jumpTable: loop with ID %ld in free list (#%ld)\n", id, j);
    }
    check[id] = true;
    prev = id;
  }
  if (id != n)
    error("jumpTable: wrong free list length, last = %ld, prev = %ld\n",
	   id, prev);
*/
}

#endif
