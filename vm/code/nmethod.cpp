/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.114 $ */
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

# include "incls/_nmethod.cpp.incl"


void nmFlags::clear() {
  assert(sizeof(nmFlags) == sizeof(int), "using more than one word for nmFlags");
  *(int*)this = 0;
}


static int instruction_length;
static int location_length;
static int scope_length;
static int nof_noninlined_blocks;


nmethod* new_nmethod(Compiler* c) {
  // This grossness is brought to you by the great way in which C++
  // handles non-standard allocation...
  instruction_length    = roundTo(c->code()->code_size(),         oopSize);
  location_length       = roundTo(c->code()->reloc_size(),        oopSize);
  scope_length          = roundTo(c->scopeDescRecorder()->size(), oopSize);
  nof_noninlined_blocks = c->number_of_noninlined_blocks();
  nmethod* nm = new nmethod(c);
  if (c->is_method_compile()) {
    Universe::code->addToCodeTable(nm);
  }
  return nm;
}


void* nmethod::operator new(size_t size) {
  Unused(size);
  assert(sizeof(nmethod) % oopSize == 0, "nmethod size must be multiple of a word");
  int nmethod_size = sizeof(nmethod) 
                   + instruction_length
                   + location_length
                   + scope_length
                   + roundTo((nof_noninlined_blocks) * sizeof(uint16), oopSize);
  void *p = Universe::code->allocate(nmethod_size);
  if (!p) fatal("out of space in code cache");
  return p;
}

void nmethod::initForTesting(int size, LookupKey* key) {
  this->key.initialize(key->klass(), key->selector_or_method());
  _instsLen = size;
  _locsLen = 0;
  _scopeLen = 0;
  _number_of_noninlined_blocks = 0;
  main_id = Universe::code->jump_table()->allocate(1);
  promoted_id = NULL;

  _invocation_count		= 0;
  _uncommon_trap_counter	= 0;
  _number_of_links		= 0;
  _special_handler_call_offset	= 0;
  _entry_point_offset    	= 0;
  _verified_entry_point_offset	= 0;
  _number_of_float_temporaries	= 0;
  _float_section_size		= 0;
  _float_section_start_offset	= 0;

  flags.clear();
  flags.state = zombie;
}

nmethod::nmethod(Compiler* c) : key(c->key->klass(), c->key->selector_or_method()) {
#ifdef ASSERT
  // lookupCache::verify();
#endif
  // Initializing the chunks sizes
  assert(instruction_length <= 10 * MaxNmInstrSize, "too many instructions");
  _instsLen	= instruction_length;
  _locsLen	= location_length; 
  _scopeLen	= scope_length;

  main_id	= c->main_jumpTable_id;
  promoted_id	= c->promoted_jumpTable_id;

  _number_of_noninlined_blocks = nof_noninlined_blocks;

  _invocation_count		= 0;
  _uncommon_trap_counter	= 0;
  _number_of_links		= 0;

  _special_handler_call_offset	= theCompiler->special_handler_call_offset();
  _entry_point_offset    	= theCompiler->entry_point_offset();
  _verified_entry_point_offset	= theCompiler->verified_entry_point_offset();
  assert(_entry_point_offset          % oopSize == 0, "entry point is not aligned");
  assert(_verified_entry_point_offset % oopSize == 0, "verified entry point is not aligned");
  assert(0 <= _special_handler_call_offset && _special_handler_call_offset < instruction_length, "bad special handler call offset");
  assert(0 <= _entry_point_offset          && _entry_point_offset          < instruction_length, "bad entry point offset");
  assert(0 <= _verified_entry_point_offset && _verified_entry_point_offset < instruction_length, "bad verified entry point offset");

  _number_of_float_temporaries	= theCompiler->totalNofFloatTemporaries();
  _float_section_size		= theCompiler->float_section_size();
  _float_section_start_offset	= theCompiler->float_section_start_offset();
  
  flags.clear();
  flags.isUncommonRecompiled = c->is_uncommon_compile();
      
  flags.level    = c->level();
  flags.version  = c->version();
  flags.is_block = c->is_block_compile() ? 1 : 0;

  flags.state    = alive;

  if (UseNMethodAging) makeYoung();

  // assert(c->frameSize() >= 0, "frame size cannot be negative");
  // frame_size = c->frameSize();

  // Fill in instructions and locations.
  c->code()->copyTo(this);
  // Fill in scope information
  c->scopeDescRecorder()->copyTo(this);
  // Fill in noninlined block scope table
  c->copy_noninlined_block_info(this);

  flushICacheRange(insts(), instsEnd());
  flushICache();

#ifdef ASSERT
  check_store();
#endif

  // Set the jumptable entry for the nmethod
  if (c->is_method_compile()) {
    Universe::code->jump_table()->at(main_id)->set_destination(entryPoint());
  } else {
    if (has_noninlined_blocks()) {
      Universe::code->jump_table()->at(promoted_id)->set_destination(entryPoint());
    }
  }
  if (this == (nmethod*)catchThisOne) warning("caught nmethod");
#ifdef ASSERT
  // turned off because they're very slow  -Urs 4/96
  // lookupCache::verify();
  verify_expression_stacks();
#endif
}


nmethod* nmethod::parent() {
  if (is_block()) {
    int index;
    return Universe::code->jump_table()->at(main_id)->parent_nmethod(index);
  }
  return NULL;
}


nmethod* nmethod::outermost() {
  nmethod* p = parent();
  if (p == NULL) return this;
  return p->outermost();
}


int nmethod::level() const {
  assert(flags.level >= 0 && flags.level <= MaxRecompilationLevels,
	 "invalid level");
  return flags.level;
}


jumpTableEntry* nmethod::jump_table_entry() const {
  return Universe::code->jump_table()->at(main_id);
}


void nmethod::setVersion(int v) {
  assert(v > 0 && v <= MaxVersions, "bad version");
  flags.version = v;
}


ScopeDesc* nmethod::containingScopeDesc(char* pc) const {
  PcDesc* pcd = containingPcDesc(pc);
  if (!pcd) return NULL;
  return pcd->containingDesc(this);
}


void nmethod::check_store() {
  // Make sure all oops in the compiled code are tenured
  relocIterator iter(this);
  while (iter.next()) {
    if (iter.type() == relocInfo::oop_type) {
      oop obj = *iter.oop_addr();
      if (obj->is_mem() && obj->is_new()) {
        fatal("must be tenured oop in compiled code");
      }
    }
  }
}


void nmethod::fix_relocation_at_move(int delta) {
  relocIterator iter(this);
  while (iter.next()) {
    if (iter.is_position_dependent()) {
      if (iter.type() == relocInfo::internal_word_type) {
        *iter.word_addr() -= delta;
      } else {
        *iter.word_addr() += delta;
      }
    }
  }
}

int nmethod::all_uncommon_trap_counter = 0;

methodOop nmethod::method() const {
  ResourceMark rm;
  return scopes()->root()->method();
}


klassOop nmethod::receiver_klass() const {
  ResourceMark rm;
  return scopes()->root()->selfKlass();
}


void nmethod::moveTo(void* p, int size) {
# ifdef NOT_IMPLEMENTED
  nmethod* to = (nmethod*)p;
  if (this == to) return;
  if (PrintCodeCompaction) {
    printf("*moving nmethod %#lx (", this);
    key.print();
    printf(") to %#lx\n", to);
    fflush(stdout);
  }

  assert(iabs((char*)to - (char*)this) >= sizeof(NCodeBase),
	 "nmethods overlap too much");
  assert(sizeof(NCodeBase) % oopSize == 0, "should be word-aligned");
  copy_oops((oop*)this, (oop*)to, sizeof(NCodeBase) / oopSize);
  		// init to's vtable
  int delta = (char*) to - (char*) this;
      
  for (relocInfo* q = locs(), *pend = locsEnd(); q < pend; q++) {
    bool needShift;		// speed optimization - q->shift() is slow
    if (q->isIC()) {
      IC* sd = q->asIC(this);
      sd->shift(delta, this);
      needShift = true;
    } else {
      needShift = true;
    }      
    if (needShift) {
      q->shift(this, delta);
    }
  }

  assert(size % oopSize == 0, "not a multiple of oopSize");
  copy_oops_overlapping((oop*) this, (oop*) to, size / oopSize);
  flushICacheRange(to->insts(), to->instsEnd());
# endif
}


void nmethod::clear_inline_caches() {
  // Iterate over all inline caches and flush them
  relocIterator iter(this);
  while (iter.next()) {
    if (iter.type() == relocInfo::ic_type) {
      iter.ic()->clear();
    }
  }
}


void nmethod::cleanup_inline_caches() {
  // Ignore zombies
  if (isZombie()) return;

  // Iterate over all inline caches and flush them
  relocIterator iter(this);
  while (iter.next()) {
    if (iter.type() == relocInfo::ic_type) {
      iter.ic()->cleanup();
    }
  }
}


void nmethod::makeOld() {
  LOG_EVENT1("marking nmethod %#x as old", this);
  flags.isYoung = 0;
}


void nmethod::forwardLinkedSends(nmethod* to) {
  // the to nmethod is about to replace the receiver; replace receiver in
  // all inline caches
  Unimplemented();
}


void nmethod::unlink() {
  LOG_EVENT1("unlinking nmethod %#lx", this);

  if (is_method()) {
    // Remove from lookupCache.
    lookupCache::flush(&key);
    // Remove the nmethod from the code table.if it's still there (another nmethod with the
    // same key may have been added in the meantime).
    if (Universe::code->methodTable->is_present(this)) Universe::code->methodTable->remove(this);
  }

  // Now clear all inline caches filled with this nmethod
  // (*not* done by clear_inline_caches() -- that clears the inline caches *in* this nmethod).
  // Right now, can't do this -- don't know who is calling this nmethod.
}


void nmethod::makeZombie(bool clearInlineCaches) {
  // mark this nmethod as zombie (it is almost dead and can be flushed as
  // soon as it is no longer on the stack)
  if (isZombie()) return;

  // overwrite call to recompiler by call to zombie handler
  LOG_EVENT2("%s nmethod 0x%x becomes zombie", (is_method() ? "normal" : "block"), this);
  NativeCall* call = nativeCall_at(specialHandlerCall());

  // Fix this:
  //   CODE NEEDED UNTIL Lars IMPLEMENTS CORRECT DEOPTIMIZATION FOR BLOCK NMETHODS
  if (is_block() && !MakeBlockMethodZombies) return;

  if (is_method()) {
    call->set_destination(StubRoutines::zombie_nmethod_entry());
  } else {
    call->set_destination(StubRoutines::zombie_block_nmethod_entry());
  }

  // WARNING: INTEL SPECIFIC CODE PROVIDED BY ROBERT
  // at verified entry point: overwrite "push ebp, mov ebp esp" instructions
  // belonging to activation frame construction with jump to zombie handler
  // call (note that this code can be savely overwritten, since there's no
  // relocation info nor oops associated with it).
  const char* enter = "\x55\x8b\xec";
  char* p = verifiedEntryPoint();
  guarantee(p[0] == enter[0] && p[1] == enter[1] && p[2] == enter[2], "not \"push ebp, mov ebp esp\" - check this");
  // overwrite with "nop, jmp specialHandlerCall" (nop first so it can be replaced by int3 for debugging)
  const char nop = '\x90';
  const char jmp = '\xeb'; // short jump with 8bit signed offset
  int offset = specialHandlerCall() - &p[3];
  guarantee(-128 <= offset && offset < 128, "offset too big for short jump");
  p[0] = nop;
  p[1] = jmp;
  p[2] = char(offset);

  if (TraceZombieCreation) {
    std->print_cr("%s nmethod 0x%x becomes zombie", (is_method() ? "normal" : "block"), this);
    if (WizardMode) {
      std->print_cr("entry code sequence:");
      char* beg = (char*)min(int(specialHandlerCall()), int(entryPoint()), int(verifiedEntryPoint()));
      char* end = (char*)max(int(specialHandlerCall()), int(entryPoint()), int(verifiedEntryPoint()));
      Disassembler::decode(beg, end + 10);
    }
  }

  // Update flags
  flags.state = zombie;
  flags.isToBeRecompiled = 0;
  assert(isZombie(), "just checking");
 
  // Make sure the entry is gone from the lookup cache & inline caches
  lookupCache::flush(&key);
  if (clearInlineCaches) clear_inline_caches();

  // Remove from nmethod tables
  unlink();
}


bool nmethod::has_noninlined_blocks() const {
  return number_of_noninlined_blocks() > 0;
}


int nmethod::number_of_noninlined_blocks() const {
  return _number_of_noninlined_blocks;
}


methodOop nmethod::noninlined_block_method_at(int noninlined_block_index) const {
  ResourceMark rm;
  return noninlined_block_scope_at(noninlined_block_index)->method();
}


inline void nmethod::validate_noninlined_block_scope_index(int index) const {
  assert(index > 0, 
         "noninlined_block_index must be positive");
  assert(index <= number_of_noninlined_blocks(), 
         "noninlined_block_index must be within boundary");
}


NonInlinedBlockScopeDesc* nmethod::noninlined_block_scope_at(int noninlined_block_index) const {
  validate_noninlined_block_scope_index(noninlined_block_index);
  int offset = noninlined_block_offsets()[noninlined_block_index-1];
  return scopes()->noninlined_block_scope_at(offset);
}


void nmethod::noninlined_block_at_put(int noninlined_block_index, int offset) const {
  validate_noninlined_block_scope_index(noninlined_block_index);
  noninlined_block_offsets()[noninlined_block_index-1] = offset;
}

jumpTableEntry* nmethod::noninlined_block_jumpEntry_at(int noninlined_block_index) const {
  validate_noninlined_block_scope_index(noninlined_block_index);
  jumpTableID id = is_block() ? promoted_id : main_id;
  return Universe::code->jump_table()->at(id.sub(noninlined_block_index));
}

void nmethod::flush() {
  // completely deallocate this method
  EventMarker em("flushing nmethod %#lx %s", this, "");
  if (PrintMethodFlushing) {
    std->print_cr("*flushing nmethod %#lx", this);
  }
  if (isZombie()) {
    clear_inline_caches();    // make sure they're cleared (may not have been done by makeZombie)
  } else {
    makeZombie(true);
  }
  unlink();
  Universe::code->free(this);
}


bool nmethod::depends_on_invalid_klass() {
  // Check receiver class
  if (receiver_klass()->is_invalid())
    return true;

  // Check dependents
  nmethodScopes* ns = scopes();
  for (int index = ns->dependent_length()-1; index >= 0; index--) {
    if (ns->dependant_at(index)->is_invalid()) 
      return true;
  }

  // We do not depend on an invalid class
  return false;
}


void nmethod::add_family(GrowableArray<nmethod*>* result) {
  // Add myself
  result->append(this);
  // Find the major for all my sub jumpTable entries
  int major = is_method() ? main_id.major() : promoted_id.major();
  // Add all filled jumpTable entries to the family
  for (int minor = 1; minor <= number_of_noninlined_blocks(); minor++) {
    jumpTableEntry* entry = Universe::code->jump_table()->at(jumpTableID(major, minor));
    nmethod* bm = entry->block_nmethod();
    if (bm) bm->add_family(result);
  }
}


GrowableArray<nmethod*>* nmethod::invalidation_family() {
  GrowableArray<nmethod*>* result = new GrowableArray<nmethod*>(10);
  add_family(result); // Call the recusive function
  return result;
}


PcDesc* nmethod::containingPcDescOrNULL(char* pc, PcDesc* st) const {
  // returns PcDesc that is closest one before or == to pc, or NULL if
  // no stored pcDesc exists 
  // called a lot, so watch out for performance bugs
  assert(contains(pc), "nmethod must contain pc into frame");
  int offset = pc - insts();
  PcDesc* start = st ? st : pcs();
  PcDesc* end = pcsEnd() - 1;

  // Skim the cream if only one pcDesc is present.
  if (start == end) return start; 

  assert(start <= end, "no PcDescs to search");

  // binary search to find approx. location
  PcDesc* middle;
  int l = 0, h = end - start;
  do {
    // avoid pointer arithmetic -- gcc uses a division for PcDesc* - PcDesc*
    int m = l + (h - l) / 2;
    middle = &start[m];
    if (middle->pc < offset) {
      l = m + 1;
    } else {
      h = m - 1;
    }
  } while (middle->pc != offset && l < h);

  // may not have found exact offset, so search for closest match
  while (middle->pc <= offset && middle < end  ) middle++;
  while (middle->pc >  offset && middle > start) middle--;
  
  assert(start <= middle && middle <= end, "should have found a pcDesc");
# ifdef ASSERT
    PcDesc* d = st ? st : pcs();
    PcDesc* closest = d;
    for (; d <= end; d ++) {
      if (d->pc <= offset && (closest == NULL || closest->pc <= d->pc)) {
	closest = d;
      }
    }
    assert(closest == middle, "found different pcDesc");
# endif

  if (middle->pc > offset) {
    assert( middle == start, "should be the first PcDesc");
    // in prologue; caller has to deal with this
    return NULL;
  }
  return middle;
}


// Create a static PcDesc for prologue PcDesc's
static PcDesc prologue_pd(0, 0, PrologueBCI);

PcDesc* nmethod::containingPcDesc(char* pc, PcDesc* start) const {
  // returns PcDesc that is closest one before or == to pc
  PcDesc* p = containingPcDescOrNULL(pc, start);
  // in prologue; there is no PcDesc stored in the nmethod (to save space),
  // so we have to create one
  return p ? p : &prologue_pd;
}


int nmethod::estimatedInvocationCount() const {
  Unimplemented();
  return 0;
}


static int cmp_addrs(const void* p1,  const void* p2) {
  char** r1 = (char**) p1;
  char** r2 = (char**) p2;
  return *r1 - *r2;
}


int nmethod::ncallers() const {
  return number_of_links();
}


// Memory operations: return true if need to inval cache

void nmethod::relocate() {
  key.relocate();
  scopes()->relocate();
  OopNCode::relocate();
}


bool nmethod::switch_pointers(oop from, oop to,
			      GrowableArray<nmethod*>* nmethods_to_invalidate) {
  key.switch_pointers(from, to);
  scopes()->switch_pointers(from, to, nmethods_to_invalidate);
  check_store();
  return OopNCode::switch_pointers(from, to, nmethods_to_invalidate);
}


void nmethod::oops_do(void f(oop*)) {
  // LookupKey
  key.oops_do(f);

  // Compiled code
  relocIterator iter(this);
  while (iter.next()) {
    if (iter.type() == relocInfo::oop_type) {
      f(iter.oop_addr());
    }
   }

  // Debugging information
  scopes()->oops_do(f);
}


void nmethod::verify() {
  ResourceMark rm;

  OopNCode::verify2("nmethod");

  // Make sure all entry points are aligned
  // The interpreter counts on it for InterpreterPICs

  if (!oop(insts())->is_smi())
    error("nmethod at %#lx has unaligned instruction start", this);

  if (!oop(entryPoint())->is_smi())
    error("nmethod at %#lx has unaligned entryPoint", this);
    
  if (!oop(verifiedEntryPoint())->is_smi())
    error("nmethod at %#lx has unaligned verifiedEntryPoint", this);

  if (!Universe::code->contains(this))
    error("nmethod at %#lx not in zone", this);
      
  scopes()->verify();

  for (PcDesc* p = pcs(); p < pcsEnd(); p++) {
    if (! p->verify(this)) {
      std->print_cr("\t\tin nmethod at %#lx (pcs)", this);
    }
  }
  
  if (findNMethod((char*) instsEnd() - oopSize) != this) {
    error("findNMethod did not find this nmethod (%#lx)", this);
  }

  verify_expression_stacks();
}


void nmethod::verify_expression_stacks_at(char* pc) {
  PcDesc* pd = containingPcDesc(pc);
  if (!pd) fatal("PcDesc not found");

  ScopeDesc* sd  = scopes()->at(pd->scope, pc);
  int        bci = pd->byteCode;
  while (sd) {
    sd->verify_expression_stack(bci);
    ScopeDesc* next = sd->sender();
    if (next) bci = sd->senderBCI();
    sd  = next;
  }
}


void nmethod::verify_expression_stacks() {
  relocIterator iter(this);
  while (iter.next()) {
    switch (iter.type()) {
      case relocInfo::ic_type:
	verify_expression_stacks_at(iter.ic()->begin_addr());
	break;
      case relocInfo::prim_type:
	if (iter.primIC()->primitive()->can_walk_stack()) {
	  verify_expression_stacks_at(iter.primIC()->begin_addr());
	}
	break;
    }
  }
}


void nmethod::CompiledICs_do(void f(CompiledIC*)) {
  relocIterator iter(this);
  while (iter.next())
    if (iter.type() == relocInfo::ic_type)
      f(iter.ic());
}


void nmethod::PrimitiveICs_do(void f(PrimitiveIC*)) {
  relocIterator iter(this);
  while (iter.next())
    if (iter.type() == relocInfo::prim_type)
      f(iter.primIC());
}


// Printing operations

void nmethod::print() {
  ResourceMark rm;
  printIndent();
  std->print("((nmethod*)%#lx) ", this);
  std->print(" for method %#lx ", method());
  key.print();
  std->print(" { ");
  if (isYoung()) std->print("YOUNG ");
  if (version()) std->print("v%d ", version());
  if (level()) std->print("l%d ", level());
  if (isZombie()) std->print("zombie ");
  if (isToBeRecompiled()) std->print("TBR ");
  if (isUncommonRecompiled()) std->print("UNCOMMON ");
  std->print_cr("}:");
  Indent ++;   
      
  printIndent();
  std->print_cr("instructions (%ld bytes): [%#lx..%#lx]", size(), insts(), instsEnd());
  printIndent();
  std->print_cr("((nmethod*)%#lx)->printCode()", this);
  // don't print code/locs/pcs by default -- too much output   -Urs 1/95
  // printCode();
  scopes()->print();
  // printLocs();
  // printPcs();
  Indent --;
}


void nmethod::printCode() {
  ResourceMark m;
  Disassembler().decode(this);
}


void nmethod::printLocs() {
  ResourceMark m;	// in case methods get printed via the debugger
  printIndent();
  std->print_cr("locations:");
  Indent ++;
  relocIterator iter(this);
  int last_offset = 0;
  for (relocInfo* l = locs(); l < locsEnd(); l++) {
    iter.next();
    last_offset = l->print(this, last_offset);
    if (iter.type() == relocInfo::uncommon_type && iter.wasUncommonTrapExecuted()) std->print(" (taken)");
    std->cr();
  }
  Indent --;
}


void nmethod::printPcs() {
  ResourceMark m;	// in case methods get printed via debugger
  printIndent();
  lprintf("pc-bytecode offsets:\n");
  Indent ++;
  for (PcDesc* p = pcs(); p < pcsEnd(); p ++) p->print(this);
  Indent --;
}


void nmethod::print_value_on(outputStream* st) {
  st->print("nmethod");
  if (WizardMode) st->print(" (0x%lx)", this);
  st->print(":");
  method()->print_value_for(receiver_klass(), st);
}


static ScopeDesc* print_scope_node(nmethodScopes* scopes, ScopeDesc* sd, int level, outputStream* st, bool with_debug_info) {
  // indent
  st->fill_to(2 + level*2);

  // print scope
  sd->print_value_on(st);
  st->cr();
  if (with_debug_info) sd->print(4 + level*2, UseNewBackend);

  // print sons
  ScopeDesc* son = scopes->getNext(sd);
  while (son && son->sender_scope_offset() == sd->offset()) {
    son = print_scope_node(scopes, son, level+1, st, with_debug_info);
  }
  return son;
}


void nmethod::print_inlining(outputStream* st, bool with_debug_info) {
  // Takes advantage of the fact that the scope tree is stored in a depth first traversal order.
  ResourceMark rm;
  if (st == NULL) st = std;
  st->print_cr("nmethod inlining structure");
  ScopeDesc* result = print_scope_node(scopes(), scopes()->root(), 0, st, with_debug_info);
  if (result != NULL) warning("print_inlining returned prematurely");
}


nmethod* nmethodContaining(char* pc, char* likelyEntryPoint) {
  assert(Universe::code->contains(pc), "should contain address");
  if (likelyEntryPoint && Universe::code->contains(likelyEntryPoint)) {
    nmethod* result = nmethod_from_insts(likelyEntryPoint);
    if (result->contains(pc)) return result;
  }
  return findNMethod(pc);
}


nmethod* findNMethod(void* start) {
  nmethod* m = Universe::code->findNMethod(start);
  assert(m->encompasses(start), "returned wrong nmethod");
  return m;
}


nmethod* findNMethod_maybe(void* start) {
  nmethod* m = Universe::code->findNMethod_maybe(start);
  assert(!m || m->encompasses(start), "returned wrong nmethod");
  return m;
}


inline bool includes(void* p, void* from, void* to) {
  return from <= p && p < to;
}


bool nmethod::encompasses(void* p) const {
  return includes(p, (void*)this, pcsEnd());
}


#ifdef DEBUG_LATER
PcDesc* nmethod::correspondingPC(ScopeDesc* sd, int bci) {
  // find the starting PC of this scope
  assert(scopes()->includes(sd), "scope not in this nmethod");  
  int scope = scopes()->offsetTo(sd);
  for (PcDesc* p = pcs(), *end = pcsEnd(); p < end; p ++) {
    if (p->scope == scope && p->byteCode == bci) break;
  }
  if (p < end ) {
    return p;
  } else {
    // no PC corresponding to this scope
    return NULL;
  }
}
#endif


CompiledIC* nmethod::IC_at(char* p) const {
  relocIterator iter(this);
  while (iter.next())
    if (iter.type() == relocInfo::ic_type)
      if (iter.ic()->begin_addr() == p) return iter.ic();
  return NULL;
}


PrimitiveIC* nmethod::primitiveIC_at(char* p) const {
  relocIterator iter(this);
  while (iter.next())
    if (iter.type() == relocInfo::prim_type)
      if (iter.primIC()->begin_addr() == p) return iter.primIC();
  return NULL;
}


oop* nmethod::embeddedOop_at(char* p) const {
  relocIterator iter(this);
  while (iter.next())
    if (iter.type() == relocInfo::oop_type)
      if (iter.oop_addr() == (oop*) p) return iter.oop_addr();
  return NULL;
}


bool nmethod::in_delta_code_at(char* pc) const {
  PcDesc* pd = containingPcDescOrNULL(pc);
  if (pd == NULL) return false;
  return !(pd->byteCode == PrologueBCI || pd->byteCode == EpilogueBCI);
}


// Support for preemption:

void nmethod::overwrite_for_trapping(nmethod_patch* data) {
  relocIterator iter(this);
  while (iter.next()) {
    switch(iter.type()) {
      case relocInfo::ic_type:           break;
      case relocInfo::prim_type:         break;
      case relocInfo::runtime_call_type: break;
      case relocInfo::uncommon_type:     break;
    }
  }
}


void nmethod::restore_from_patch(nmethod_patch* data) {
  Unimplemented();
}


void nmethod::print_inlining_database() {
  print_inlining_database_on(std);
}


void nmethod::print_inlining_database_on(outputStream* st) {
  // WARNING: this method is for debugging only -- it's not used to actually file out the DB
  ResourceMark rm;
  RScope* root = RNonDummyScope::constructRScopes(this, false);
  GrowableArray<PcDesc*>* uncommon = uncommonBranchList();
  root->print_inlining_database_on(st, uncommon);
}


static int compare_pcDescs(PcDesc** a, PcDesc** b) {
  // to sort by ascending scope and ascending bci
  int diff = (*a)->scope - (*b)->scope;
  return diff ? diff : (*a)->byteCode - (*b)->byteCode;
}


GrowableArray<PcDesc*>* nmethod::uncommonBranchList() {
  // return a list of *all* uncommon branches (taken or not) of nm, sorted by scope and bci
  // (for inlining DB)
  GrowableArray<PcDesc*>* uncommon = new GrowableArray<PcDesc*>(20);
  relocIterator iter(this);
  while (iter.next()) {
    if (iter.type() == relocInfo::uncommon_type) {
      uncommon->append(containingPcDesc((char*)iter.word_addr()));
    }
  }

  uncommon->sort(&compare_pcDescs);
  return uncommon;
}


inline void nmethod::decay_invocation_count(double decay_factor) {
  double new_count = (double) invocation_count() / decay_factor;
  set_invocation_count((int) new_count);
}


// Perform a sweeper task
void nmethod::sweeper_step(double decay_factor) {
  // Ignore zombies
  if (isZombie()) return;
  decay_invocation_count(decay_factor);
  _uncommon_trap_counter = int(_uncommon_trap_counter / decay_factor);
  cleanup_inline_caches();
  incrementAge();
}


bool nmethod::isYoung()	{ 
  if (!UseNMethodAging) return false;
  if (!flags.isYoung) return false; 
  // flags.isYoung == 1, but maybe it has become old in the meantime
  if (age() >= NMethodAgeLimit ||
      invocation_count() >= InvocationCounterLimit) {
    makeOld();
  }
  return flags.isYoung;
}


void nmethod_init() {
  // make sure you didn't forget to adjust the filler fields
  assert(sizeof(nmFlags) <= 4, "nmFlags occupies more than a word");
}

# endif
