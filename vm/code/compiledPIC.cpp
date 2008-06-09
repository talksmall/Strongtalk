/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.37 $ */
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

# include "incls/_compiledPIC.cpp.incl"


// A PIC implements a Polymorphic Inline Cache for compiled code.
// The layout comes in 3 variants: a) PICs that contain only m methodOop
// entries, b) PICs that contain both, n nmethod and m methodOop entries
// and c) MICs (Megamorphic Inline Caches) which keep only the selector
// so lookup is fast.The 3 formats can be distinguished by looking at
// the first instruction.
//
// Layout a)		(methodOop entries only)
//
//  0			: call PIC stub routine
//  5			: m methodOop entries a 8 bytes each
//  5 + m*8		: <end of PIC>
//
// i.th methodOop entry (0 <= i < m; m > 0)
//
//  5 + i*8		: klass(i)
//  9 + i*8		: methodOop(i)
//
//
// Layout b)		(nmethod & methodOop entries)
//
//  0			: test al, Mem_Tag
//  2			: jz smi_nmethod/jz methodOop call stub/jz cache_miss
//  8			: mov edx, [eax.klass] (always here to simplify iteration, even if n = 0)
// 11			: n nmethod entries a 12 bytes each
// 11 + n*12		: call PIC stub routine (m > 0)/jmp cache_miss (m = 0)
// 16 + n*12		: m methodOop entries a 8 bytes each
// 16 + n*12 + m*8	: <end of PIC>
//
// i.th nmethod entry (0 <= i < n; n >= 0):
//
// 11 + i*12		: cmp edx, klass(i)
// 11 + i*12 + 6	: je nmethod(i)
//
// i.th methodOop (0 <= i < m; m >= 0):
//
// 16 + n*12 + i*8	: klass
// 16 + n*12 + i*8 + 4	: methodOop
//
//
// Layout c)		(megamorphic inline cache, selector only)
//
//  0			: call MIC stub routine
//  5			: selector
//  9			: <end of MIC>
//
// The PIC stub routine interprets the remaining entries of the PIC; there
// are different stub routines for different m (starting point for interpretation
// is the return address). Entries for smis are treated especially in the sense
// that an initial check for them is always there.
//
// NB: The reason for interpreting the interpreter entries is space, a smallest
// native code implementation for these cases requires 18 bytes per entry (cmp,
// je and load of methodOop). Furthermore, in each case it would be necessary
// to call a stub routine to setup an interpreter frame anyway.
//
// The MIC stub routine interprets the one entry in the MIC. In case of a miss,
// no new PIC/MIC is generated but the current entries are updated. The reason
// for having a specialized PIC allocated in the MIC case is only so the selector
// can be stored. It is used for fast lookup (usually the selector is recomputed
// via debug-info from the corresponding interpreted method).


// Opcodes for code pattern generation/parsing
static const char   test_opcode = '\xa8';
static const char   call_opcode = '\xe8';
static const char   jmp_opcode  = '\xe9';
static const uint16 jz_opcode   = 0x840f;
static const uint16 mov_opcode  = 0x508b;
static const uint16 cmp_opcode  = 0xfa81;	static const int cmp_opcode_size = sizeof(uint16);


// Helper routines for code pattern generation/parsing
static inline void  put_byte(char*& p, u_char b){ *p++ = b; }
static inline void  put_shrt(char*& p, uint16 s){ *(uint16*)p = s; p += sizeof(uint16); }
static inline void  put_word(char*& p, int w)	{ *(int*)p = w; p += sizeof(int); }
static inline void  put_disp(char*& p, char* d)	{ put_word(p, (int)(d - p - sizeof(int))); }

static inline int   get_shrt(char* p)		{ return *(uint16*)p; }
static inline char* get_disp(char* p)		{ return *(int*)p + p + sizeof(int); }


// Structure for storing the entries of a PIC
class PIC_contents {
public:
  // smi case
  char*		smi_nmethod;
  methodOop	smi_methodOop;

  // nmethod entries
  klassOop	nmethod_klasses[PIC::max_nof_entries];
  char*		nmethods[PIC::max_nof_entries];
  int n;	// nmethods index

  // methodOop entries
  klassOop	methodOop_klasses[PIC::max_nof_entries];
  methodOop	methodOops[PIC::max_nof_entries];
  int m;	// methodOops index

  void append_nmethod_entry(klassOop klass, char* entry);
  void append_method(klassOop klass, methodOop method);

  int number_of_compiled_targets() const	{ return (smi_nmethod   ? 1 : 0) + n; }
  int number_of_interpreted_targets() const	{ return (smi_methodOop ? 1 : 0) + m; }
  int number_of_targets() const			{ return number_of_compiled_targets() + number_of_interpreted_targets(); }

  bool has_smi_case() const			{ return (smi_methodOop != NULL) || (smi_nmethod != NULL); }
  bool has_nmethods() const			{ return (n > 0) || (smi_nmethod != NULL); }


  int code_size() const {
    int methodOop_size = number_of_interpreted_targets() * PIC::PIC_methodOop_entry_size;
    if (has_nmethods()) {
      return PIC::PIC_methodOop_entry_offset + n * PIC::PIC_nmethod_entry_size + methodOop_size;
    } else {
      return PIC::PIC_methodOop_only_offset + methodOop_size;
    }
  }


  PIC_contents() {
    smi_nmethod   = NULL;
    smi_methodOop = NULL;
    n = 0;
    m = 0;
  }
};


void PIC_contents::append_nmethod_entry(klassOop klass, char* entry) {
  // add new entry
  if (klass == smiKlassObj) {
    assert(!has_smi_case(), "cannot overwrite smi case");
    smi_nmethod = entry;
  } else {
    nmethod_klasses[n] = klass;
    nmethods[n] = entry;
    n++;
  }
}


void PIC_contents::append_method(klassOop klass, methodOop method) {
  // add new entry
  assert(method->is_method(), "must be methodOop");
  if (klass == smiKlassObj) {
    assert(!has_smi_case(), "cannot overwrite smi case");
    smi_methodOop = method;
  } else {
    methodOop_klasses[m] = klass;
    methodOops[m] = method;
    m++;
  }
}


// Implementation of PIC_Iterators

PIC_Iterator::PIC_Iterator(PIC* pic) {
  _pic   = pic;
  _pos   = pic->entry();
  // determine initial state
  if (pic->is_megamorphic()) {
    // MIC -> do not use cached information
    assert(get_disp(_pos + 1) == StubRoutines::megamorphic_ic_entry(), "MIC stub expected");
    _state = at_the_end;
  } else if (*_pos == call_opcode) {
    // PIC without nmethods
    _state = at_methodOop;
    _methodOop_counter = PIC::nof_entries(get_disp(_pos + 1));
    _pos += PIC::PIC_methodOop_only_offset;
  } else {
    // nmethods -> handle smis first
    char* dest = get_disp(_pos + PIC::PIC_smi_nmethod_offset);
    if (dest == CompiledIC::normalLookupRoutine() || _pic->contains(dest)) {
      // no smis or smi case is treated in methodOop section
      _state = at_nmethod;
      _pos += PIC::PIC_nmethod_entry_offset;
    } else {
      // smi entry is treated here
      _state = at_smi_nmethod;
    }
  }
}


void PIC_Iterator::computeNextState() {
  if (get_shrt(_pos) == cmp_opcode) {
    // same state
  } else if (*_pos == call_opcode) {
    _state = at_methodOop;
    _methodOop_counter = PIC::nof_entries(get_disp(_pos + 1));
    _pos += PIC::PIC_methodOop_entry_offset - PIC::PIC_nmethod_entry_offset;
  } else {
    assert(*_pos == jmp_opcode, "jump to lookup routine expected");
    _state = at_the_end;
  }
}


void PIC_Iterator::advance() {
  switch (_state) {
    case at_smi_nmethod:
      assert(_pos == _pic->entry(), "must be at beginning");
      _pos += PIC::PIC_nmethod_entry_offset;
      _state = at_nmethod;
      computeNextState();
      break;
    case at_nmethod:
      _pos += PIC::PIC_nmethod_entry_size;
      computeNextState();
      break;
    case at_methodOop:
      if (--_methodOop_counter > 0) {
        _pos += PIC::PIC_methodOop_entry_size;
      } else {
        _state = at_the_end;
      }
      break;
    case at_the_end:
      ShouldNotCallThis();
    default:
      ShouldNotReachHere();
  }
}


klassOop* PIC_Iterator::klass_addr() const {
  int offs;
  switch (state()) {
    case at_smi_nmethod: ShouldNotCallThis();			// no klass stored -> no klass address available
    case at_nmethod    : offs = PIC::PIC_nmethod_klass_offset;	break;
    case at_methodOop  : offs = PIC::PIC_methodOop_klass_offset;break;
    case at_the_end    : ShouldNotCallThis();			// no klass stored -> no klass address available
    default            : ShouldNotReachHere();
  }
  return (klassOop*)(_pos + offs);
}


int* PIC_Iterator::nmethod_disp_addr() const {
  int offs;
  switch (state()) {
    case at_smi_nmethod: offs = PIC::PIC_smi_nmethod_offset;	break;
    case at_nmethod    : offs = PIC::PIC_nmethod_offset;	break;
    case at_methodOop  : ShouldNotCallThis();			// no nmethod stored -> no nmethod address available
    case at_the_end    : ShouldNotCallThis();			// no nmethod stored -> no nmethod address available
    default            : ShouldNotReachHere();
  }
  return (int*)(_pos + offs);
}


methodOop* PIC_Iterator::methodOop_addr() const {
  int offs;
  switch (state()) {
    case at_smi_nmethod: ShouldNotCallThis();			// no methodOop stored -> no methodOop address available
    case at_nmethod    : ShouldNotCallThis();			// no methodOop stored -> no methodOop address available
    case at_methodOop  : offs = PIC::PIC_methodOop_offset;	break;
    case at_the_end    : ShouldNotCallThis();
    default            : ShouldNotReachHere();
  }
  return (methodOop*)(_pos + offs);
}


void PIC_Iterator::print() {
  lprintf("a PIC_Iterator\n");
}


// Implementation of PICs

bool PIC::in_heap(char* addr) {
  return Universe::code->picHeap->contains(addr);
}


PIC* PIC::find(char* addr) {
  if (Universe::code->picHeap->contains(addr)) {
    PIC* result = (PIC*)Universe::code->picHeap->findStartOfBlock(addr);
    return result;
  }
  return NULL;
}


// Accessing PIC entries
klassOop PIC_Iterator::get_klass() const { 
  return state() == at_smi_nmethod ? smiKlassObj : *klass_addr(); 
}


char* PIC_Iterator::get_call_addr() const { 
  int* a = nmethod_disp_addr(); 
  return (char*)a + sizeof(int) + *a; 
}


bool PIC_Iterator::is_compiled() const {
  switch (state()) {
    case at_smi_nmethod: return true;
    case at_nmethod    : return true;
    case at_methodOop  : return false;
    case at_the_end    : ShouldNotCallThis();
    default            : ShouldNotReachHere();
  }
  return false;
}


bool PIC_Iterator::is_interpreted() const { 
  return !is_compiled();
}


nmethod* PIC_Iterator::compiled_method() const { 
  if (!is_compiled()) return NULL;  
  return findNMethod(get_call_addr() - sizeof(nmethod)); 
}


methodOop PIC_Iterator::interpreted_method() const { 
  if (is_interpreted()) {
    return *methodOop_addr(); 
  } else {
    return compiled_method()->method();
  }
}


// Modifying PIC entries
void PIC_Iterator::set_klass(klassOop klass) { 
  assert(state() != at_smi_nmethod, "cannot be set"); 
  *klass_addr() = klass; 
}


void PIC_Iterator::set_nmethod(nmethod* nm) { 
  assert(get_klass() == nm->key.klass(), "mismatched receiver klass");
  int* a = nmethod_disp_addr(); 
  *a = nm->verifiedEntryPoint() - (char*)a - sizeof(int); 
}


void PIC_Iterator::set_methodOop(methodOop method) { 
  *methodOop_addr() = method; 
}


symbolOop* PIC::MIC_selector_address() const {
  assert(is_megamorphic(), "not a MIC");
  return (symbolOop*)(entry() + MIC_selector_offset);
}


PIC* PIC::replace(nmethod* nm) {
  // nothing to do in megamorphic case
  if (is_megamorphic()) return this;

  LOG_EVENT3("compiled PIC at 0x%x: new nmethod 0x%x for klass 0x%x replaces old entry", this, nm, nm->key.klass());

  { // do the replace without creating a new PIC if possible
    PIC_Iterator it(this);
    while (it.get_klass() != nm->key.klass()) it.advance();
    assert(!it.at_end(), "unexpected end during replace");
    if (it.is_compiled()) {
      it.set_nmethod(nm);
      return this;
    }
  }

  { // Create a new PIC 
    PIC_contents contents;
    PIC_Iterator it(this);
    while (!it.at_end()) {
      klassOop receiver_klass = it.get_klass();
      if (receiver_klass == nm->key.klass()) {
        contents.append_nmethod_entry(nm->key.klass(), nm->verifiedEntryPoint());
      } else {
        if (it.is_interpreted()) {
          contents.append_method(it.get_klass(), it.interpreted_method());
        } else {
          contents.append_nmethod_entry(it.get_klass(), it.get_call_addr());
        }
      }
      it.advance();
    }
    int allocated_code_size = contents.code_size();
    return new (allocated_code_size) PIC(_ic, &contents, allocated_code_size);
  }
}


PIC* PIC::cleanup(nmethod** nm) {
  // nothing to do in megamorphic case
  if (is_megamorphic()) return this;

  bool pic_layout_has_changed = false;

  // Iterate over the PIC and
  //  - patch the PIC if possible
  //  - check if the layout has changed
  //  - collect the PIC information.

  PIC_contents contents;
  PIC_Iterator it(this);
  while (!it.at_end()) {
    klassOop receiver_klass = it.get_klass();
    if (it.is_interpreted()) {
      // Interpreted methodOop
      if(compiled_ic()->isSuperSend()) {
        contents.append_method(it.get_klass(), it.interpreted_method());
      } else {
        LookupKey key(it.get_klass(), it.interpreted_method()->selector());
        LookupResult result = lookupCache::lookup(&key);
        if (result.matches(it.interpreted_method())) {
          contents.append_method(it.get_klass(), it.interpreted_method());
        } else {
          if (result.is_method()) {
            contents.append_method(it.get_klass(), result.method());
            it.set_methodOop(result.method());
          } else if (result.is_entry()) {
            contents.append_nmethod_entry(it.get_klass(), result.get_nmethod()->verifiedEntryPoint());
            pic_layout_has_changed = true;
          } else {
            pic_layout_has_changed = true;
          }
        }
      }
    } else {
      // Compiled nmethod
      nmethod* nm = it.compiled_method();
      LookupResult result = lookupCache::lookup(&nm->key);
      if (result.matches(nm)) {
        contents.append_nmethod_entry(it.get_klass(), it.get_call_addr());
      } else {
        if (result.is_method()) {
          contents.append_method(it.get_klass(), result.method());
          pic_layout_has_changed = true;
        } else if (result.is_entry()) {
          contents.append_nmethod_entry(it.get_klass(), result.get_nmethod()->verifiedEntryPoint());
          it.set_nmethod(result.get_nmethod());
        } else {
          pic_layout_has_changed = true;
        }
      }
    }
    it.advance();
  }

  *nm = NULL;
  if (pic_layout_has_changed) {

    if (contents.number_of_targets() == 0) {
      // no targets left
      return NULL;
    }

    if (contents.number_of_targets() == 1 && contents.has_nmethods()) {
      // 1 nmethod target
      *nm = findNMethod(contents.has_smi_case() ? contents.smi_nmethod : contents.nmethods[0]);
      assert(*nm, "nmethod must be present");
      return NULL;
    }

    int allocated_code_size = contents.code_size();
    return new (allocated_code_size) PIC(_ic, &contents, allocated_code_size);
  }

  // The layout has not changed so return the patched self
  return this;
}


int PIC::nof_entries(char* pic_stub) {
  int i = 1;
  while (true) {
    if (pic_stub == StubRoutines::PIC_stub_entry(i)) return i;
    i++;
  }
  ShouldNotReachHere();
  return 0;
}


int PIC::code_for_methodOops_only(char* entry, PIC_contents* c) {
  char* p = entry;
  put_byte(p, call_opcode);
  if (c->smi_methodOop == NULL) {
    // no smi methodOop
    put_disp(p, StubRoutines::PIC_stub_entry(c->m));
    assert(entry + PIC_methodOop_only_offset == p, "constant inconsistent");
  } else {
    // handle smi methodOop first
    put_disp(p, StubRoutines::PIC_stub_entry(1 + c->m));
    put_word(p, int(smiKlassObj));
    put_word(p, int(c->smi_methodOop));
    assert(entry + PIC_methodOop_only_offset + PIC_methodOop_entry_size == p, "constant value inconsistent with code pattern");
  }
  char* p1 = p;
  for (int i = 0; i < c->m; i++) {
    assert(c->methodOop_klasses[i] != smiKlassObj, "should not be smiKlassObj");
    put_word(p, int(c->methodOop_klasses[i]));
    put_word(p, int(c->methodOops[i]));
  }
  assert(p1 + c->m * PIC_methodOop_entry_size == p, "constant value inconsistent with code pattern");
  return p - entry;
}


int PIC::code_for_polymorphic_case(char* entry, PIC_contents* c) {
  if (c->has_nmethods()) {
    // nmethods & methodOops
    // test al, Mem_Tag
    char* p     = entry;
    char* fixup = NULL;
    put_byte(p, test_opcode);
    put_byte(p, Mem_Tag);
    // jz ...
    put_shrt(p, jz_opcode);
    if (c->smi_nmethod != NULL) {
      assert(c->smi_methodOop == NULL, "can only have one method for smis");
      put_disp(p, c->smi_nmethod);
    } else if (c->smi_methodOop != NULL) {
      // smi method is methodOop -> handle it in methodOop section
      fixup = p;
      put_disp(p, 0);
    } else {
      // no smi entries
      put_disp(p, CompiledIC::normalLookupRoutine()); // Fix this for super sends!
    }
    // always load klass to simplify decoding/iteration of PIC
    // mov edx, [eax.klass]
    put_shrt(p, mov_opcode);
    put_byte(p, memOopDesc::klass_byte_offset());
    assert(entry + PIC_nmethod_entry_offset == p, "constant value inconsistent with code pattern");
    // handle nmethods
    for (int i = 0; i < c->n; i++) {
      // cmp edx, klass(i)
      assert(c->nmethod_klasses[i] != smiKlassObj, "should not be smiKlassObj");
      put_shrt(p, cmp_opcode);
      assert(entry + PIC_nmethod_entry_offset + i*PIC_nmethod_entry_size + PIC_nmethod_klass_offset == p, "constant value inconsistent with code pattern");
      put_word(p, int(c->nmethod_klasses[i]));
      // je nmethod(j)
      put_shrt(p, jz_opcode);
      assert(entry + PIC_nmethod_entry_offset + i*PIC_nmethod_entry_size + PIC_nmethod_offset == p, "constant value inconsistent with code pattern");
      put_disp(p, c->nmethods[i]);
    }
    assert(entry + PIC_nmethod_entry_offset + c->n*PIC_nmethod_entry_size == p, "constant value inconsistent with code pattern");
    if (c->smi_methodOop != NULL || c->m > 0) {
      // handle methodOops
      if (fixup != NULL) put_disp(fixup, p);
      p += code_for_methodOops_only(p, c);
    } else {
      // jmp cache_miss
      put_byte(p, jmp_opcode);
      put_disp(p, CompiledIC::normalLookupRoutine());
    }
    return p - entry;
  } else {
    // no nmethods -> call PIC stub routine directly
    return code_for_methodOops_only(entry, c);
  }
}


int PIC::code_for_megamorphic_case(char* entry) {
  char* p = entry;
  put_byte(p, call_opcode);
  put_disp(p, StubRoutines::megamorphic_ic_entry());
  assert(entry + MIC_selector_offset == p, "layout constant inconsistent with code pattern");
  put_word(p, int(selector()));	// used for fast lookup
  assert(entry + MIC_code_size == p, "layout constant inconsistent with code pattern");
  return p - entry;
}


void PIC::shrink_and_generate(PIC* pic, klassOop klass, void* method) {
  Unimplemented();
}


void* PIC::operator new(size_t size, int code_size){
  return Universe::code->picHeap->allocate(size + code_size);
}


void PIC::operator delete(void* p) {
  Universe::code->picHeap->deallocate(p, 0);
}


PIC* PIC::allocate(CompiledIC* ic, klassOop klass, LookupResult result) {
  assert(!result.is_empty(), "lookup result cannot be empty");

  PIC_contents contents;

  // Always add the new entry first
  if (result.is_entry()) {
    contents.append_nmethod_entry(klass, result.get_nmethod()->verifiedEntryPoint());
  } else {
    contents.append_method(klass, result.method());
  }

  PIC*     old_pic       = ic->pic();
  nmethod* old_nmethod   = ic->target();
  bool     switch_to_MIC = false;

  // 3 possible cases:
  //
  // 1. IC is empty but lookup result returns methodOop -> needs 1-element PIC to call methodOop
  // 2. IC contains 1 nmethod -> generate 2-element PIC
  // 3. IC contains pic -> grow PIC or switch to MIC

  if (old_pic != NULL) {
    // ic contains pic
    assert(old_nmethod == NULL, "just checking");
    assert(!old_pic->is_megamorphic(), "MICs should not change anymore");
    if (old_pic->number_of_targets() >= max_nof_entries) {
      if (UseMICs) {
        // switch to MIC, keep only no lookup result
        switch_to_MIC = true;
      } else {
        ic->resetOptimized();	// make sure it doesn't force the creation of nmethods
        return NULL;
      }
    } else {
      // append old PIC entries
      PIC_Iterator it(old_pic);
      while (!it.at_end()) {
        if (it.is_interpreted()) {
          contents.append_method(it.get_klass(), it.interpreted_method());
        } else {
          contents.append_nmethod_entry(it.get_klass(), it.get_call_addr());
        }
        it.advance();
      }
    }
  } else if (old_nmethod != NULL) {
    // ic contains 1 nmethod
    contents.append_nmethod_entry(ic->get_klass(0), old_nmethod->verifiedEntryPoint());
  } else {
    // empty ic -> nothing to do
    assert(ic->is_empty(), "just checking");
  }

  assert(switch_to_MIC ||
    contents.number_of_interpreted_targets() > 0 ||
    contents.number_of_compiled_targets() > 1,
    "no PIC required for only 1 compiled target");

  PIC* new_pic = NULL;
  if (switch_to_MIC) {
    new_pic = new (MIC_code_size) PIC(ic);
  } else {
    int allocated_code_size = contents.code_size();
    new_pic = new (allocated_code_size) PIC(ic, &contents, allocated_code_size);
  }

#ifdef ASSERT
  new_pic->verify();
#endif

  return new_pic;
}



PIC::PIC(CompiledIC* ic, PIC_contents* contents, int allocated_code_size) {
  assert(contents->number_of_targets() >= 1, "at least one entry needed for non-megamorphic case");
  _ic			= ic;
  _number_of_targets	= contents->number_of_targets();
  _code_size		= code_for_polymorphic_case(entry(), contents);
  assert(code_size() == allocated_code_size, "Please adjust PIC_contents::code_size()");
}


PIC::PIC(CompiledIC* ic) {
  _ic			= ic;
  _number_of_targets	= 0; // indicates megamorphic case
  _code_size		= code_for_megamorphic_case(entry());
  assert(code_size() == MIC_code_size, "Please adjust PIC_contents::code_size()");
}


GrowableArray<klassOop>* PIC::klasses() const {
  GrowableArray<klassOop>* k = new GrowableArray<klassOop>(2);
  PIC_Iterator it((PIC*)this);
  while(!it.at_end()) {
    k->append(it.get_klass());
    it.advance();
  }
  return k;
}


void PIC::oops_do(void f(oop*)) {
  if (is_megamorphic()) {
    // cannot use PIC_Iterator (0 entries) -> deal with MIC directly
    f((oop*)MIC_selector_address());
  } else {
    PIC_Iterator it(this);
    while(!it.at_end()) {
      switch(it.state()) {
        case PIC_Iterator::at_methodOop: f((oop*) it.methodOop_addr());  // fall through
        case PIC_Iterator::at_nmethod  : f((oop*) it.klass_addr());
      }
      it.advance();
    }
  }
}


void PIC::print() {
  lprintf("\tPIC with %d entr%s\n", number_of_targets(), number_of_targets() == 1 ? "y" : "ies");
  lprintf("\t- selector    : ");
  selector()->print_symbol_on();
  lprintf("\n");

  // Disassembler::decode(entry(), entry() + code_size());

  int i = 1;
  PIC_Iterator it(this);
  while (!it.at_end()) {
    lprintf("\t- %d. klass    : ", i);
    it.get_klass()->print_value();
    lprintf("\n");
    switch (it.state()) {
      case PIC_Iterator::at_smi_nmethod: // fall through
      case PIC_Iterator::at_nmethod    : printf("\t-    nmethod  : %#x (entry %#x)\n", 
                                           (int)it.compiled_method(), (int)it.get_call_addr()); break;
      case PIC_Iterator::at_methodOop  : printf("\t-    methodOop: %s\n", it.interpreted_method()->print_value_string()); break;
      default: ShouldNotReachHere();
    }
    i++;
    it.advance();
  }
}


void PIC::verify() {
  // check for multiple entries for same class
  ResourceMark rm;
  GrowableArray<klassOop>* k = klasses();
  for (int i = 0; i < k->length() - 1; i++) {
    for (int j = i + 1; j < k->length(); j++) {
      if (k->at(i) == k->at(j)) {
        std->print("The class ");
        k->at(i)->klass_part()->print_name_on(std);
        std->print_cr("is twice in PIC 0x%lx", this);
        warning("PIC verify error");
      }
    }
  }
}


// Implementation of CompiledIC_Iterator

CompiledIC_Iterator::CompiledIC_Iterator(CompiledIC* ic) {
  _ic = ic;
  init_iteration();
}


void CompiledIC_Iterator::init_iteration() {
  _picit = NULL;
  _index = 0;
  if (_ic->is_empty()) {
    _number_of_targets = 0;
    _shape = anamorphic;
  } else if (_ic->is_monomorphic()) {
    _number_of_targets = 1;
    _shape = monomorphic;
    PIC* pic = _ic->pic();
    if (pic) _picit = new PIC_Iterator(pic);  // calls an interpreted method
  } else if (_ic->is_polymorphic()) {
    PIC* pic = _ic->pic();
    _number_of_targets = pic->number_of_targets();
    _shape = polymorphic;
    _picit = new PIC_Iterator(pic);
  } else if (_ic->is_megamorphic()) {
    _number_of_targets = 0;
    _shape = megamorphic;
  } else {
    ShouldNotReachHere();
  }
}


void CompiledIC_Iterator::advance() {
  assert(!at_end(), "iterated over the end");
  if (_picit != NULL) {
    _picit->advance();
  }
  _index++;
}


klassOop CompiledIC_Iterator::klass() const {
  assert(!at_end(), "iterated over the end");
  if (_picit != NULL) {
    return _picit->get_klass();
  } else {
    return _ic->get_klass(0);
  }
}


bool CompiledIC_Iterator::is_interpreted() const {
  assert(!at_end(), "iterated over the end");
  if (_picit != NULL) {
    return _picit->is_interpreted();
  } else {
    return false;
  }
}


bool CompiledIC_Iterator::is_compiled() const {
  assert(!at_end(), "iterated over the end");
  if (_picit != NULL) {
    return _picit->is_compiled();
  } else {
    return true;
  }
}


bool CompiledIC_Iterator::is_super_send() const {
  extern bool SuperSendsAreAlwaysInlined;
  assert(SuperSendsAreAlwaysInlined, "fix this");
  return false;		// for now, super sends are always inlined
}


methodOop CompiledIC_Iterator::interpreted_method() const {
  assert(!at_end(), "iterated over the end");
  if (_picit != NULL) {
    return _picit->interpreted_method();
  } else {
    return compiled_method()->method();
  }
}


nmethod* CompiledIC_Iterator::compiled_method() const {
  assert(!at_end(), "iterated over the end");
  if (_picit != NULL) {
    return _picit->compiled_method();
  } else {
    assert(number_of_targets() == 1, "must be monomorphic");
    return _ic->target();
  }
}


void CompiledIC_Iterator::print() {
  lprintf("CompiledIC_Iterator for ((CompiledIC*)%#x) (%s)\n", _ic, selector()->as_string());
}

#endif
