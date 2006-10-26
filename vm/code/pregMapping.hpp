/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.24 $ */
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

#ifdef DELTA_COMPILER

// A PRegClosure is used when iterating over a PRegMapping.

class PRegClosure: public PrintableResourceObj {
 public:
  virtual void preg_do(PReg* preg) {}		// called for each PReg in the mapping
};


// A PRegMapping holds the mapping of life PRegs to (register and/or stack) locations.
// Within one PRegMapping, at any time a particular PReg is mapped to at most one register
// and one stack location, but different PRegs may be mapped to the same location. Because
// of register moving/spilling, PReg locations may change automatically.
//
// Note: The _NLRinProgress flag indicates that the current mapping also reserves the
//       3 special registers used during NLRs. This is needed because these registers
//       are not explicitly visible in the intermediate data structure (no PRegs) but
//       have to be preserved anyway (e.g. code generation for conformance mappings).

class PRegMapping: public PrintableResourceObj {
 private:
  MacroAssembler*	_assm;			// the low_level assembler (for spill code generation, etc.)
  bool                  _NLRinProgress;		// indicates that a NLR is in progress (see also Note above)
  Locations*		_locs;			// the locations freelist
  GrowableArray<PReg*>*	_pregs;			// the pregs, a NULL entry means the slot is not used
  GrowableArray<int>*	_regLocs;		// the register to which a preg is mapped or -1
  GrowableArray<int>*	_stkLocs;		// the stack location to which a preg is mapped or -1
  GrowableArray<int>*	_tmpLocs;		// a list of temporary locations used by instances of Temporary
  						// (these locations will be freed when the mapping is copied)

  // Helper routines
  int  size() const				{ return _pregs->length(); }
  bool used(int i) const			{ return _pregs->at(i) != NULL; }
  int  regLoc(int i) const			{ return _regLocs->at(i); }
  int  stkLoc(int i) const			{ return _stkLocs->at(i); }
  bool hasRegLoc(int i) const			{ return _locs->isLocation(regLoc(i)); }
  bool hasStkLoc(int i) const			{ return _locs->isLocation(stkLoc(i)); }
  int  location(int i) const			{ int rloc = regLoc(i); return rloc >= 0 ? rloc : stkLoc(i); }
  void set_entry(int i, PReg* preg, int rloc, int sloc);
  int  index(PReg* preg);
  int  freeSlot();
  void print(int i);
  void destroy();				// destroys mapping to make sure it is not accidentally used afterwards

  // Register allocation/spilling
  int  spillablePRegIndex();			// returns the _pregs/_mappings index of a PReg mapped to a non-locked register
  void ensureOneFreeRegister();			// ensures at least one free register in locations - spill a register if necessary
  void spillRegister(int loc);			// spills register loc to a free stack location
  void saveRegister(int loc);
  
  // Helpers for class Temporary
  int  allocateTemporary(Register hint = noreg);
  void releaseTemporary(int regLoc);
  void releaseAllTemporaries();

  // make conformant implementations
  void old_makeConformant(PRegMapping* with);
  void new_makeConformant(PRegMapping* with);

 public:
  // Creation
  PRegMapping(MacroAssembler* assm, int nofArgs, int nofRegs, int nofTemps);
  PRegMapping(PRegMapping* m);

  MacroAssembler* assembler() const		{ return _assm; }

  // Testers
  bool isInjective();
  bool isConformant(PRegMapping* with);
  bool isDefined(PReg* preg)			{ return index(preg) >= 0; }
  bool inRegister(PReg* preg)			{ int i = index(preg); return used(i) && hasRegLoc(i); }
  bool onStack(PReg* preg)			{ int i = index(preg); return used(i) && hasStkLoc(i); }
  
  // Definition
  void mapToArgument(PReg* preg, int argNo);
  void mapToRegister(PReg* preg, Register reg);
  void mapToTemporary(PReg* preg, int tempNo);

  void kill(PReg* preg);
  void killDeadsAt(Node* node, PReg* exception = NULL);
  void killAll(PReg* exception = NULL);
  void cleanupContextReferences();

  // Expressions
  Register def(PReg* preg, Register hint = noreg);	// defines a new value for preg (uses hint if given)
  Register use(PReg* preg, Register hint);		// uses the value of preg (uses hint if given)
  Register use(PReg* preg);				// deals also with constants (code originally in CodeGenerator)

  // Assignments
  void move(PReg* dst, PReg* src);

  // Calls
  void saveRegisters(PReg* exception = NULL);
  void killRegisters(PReg* exception = NULL);
  void killRegister (PReg* preg);

  // Non-local returns
  bool NLRinProgress() const			{ return _NLRinProgress; }
  void acquireNLRRegisters();
  void releaseNLRRegisters();

  // Labels
  void makeInjective();
  void makeConformant(PRegMapping* with);

  // Iteration/Debug info
  void iterate(PRegClosure* closure);
  Location locationFor(PReg* preg);

  // Space usage
  int  nofPRegs();
  int  maxNofStackTmps();

  // Debugging
  void my_print();
  void print();
  void verify();

  friend class Temporary;
};


// A PRegLocker is used to lock certain PRegs for the existence of the scope of
// a C++ function activation. A PReg that is locked in a PRegLocker is kept in
// the same register once it has been mapped to a register location by a PRegMapping.
// NOTE: PRegLockers MUST only be created/destructed in a stack-fashioned manner.

class PRegLocker: StackObj {
 private:
  static PRegLocker*	_top;			// the topmost PRegLocker
  PRegLocker*		_prev;			// the previous PRegLocker
  PReg*			_pregs[3];		// the locked PRregs

  void lock(PReg* r0, PReg* r1, PReg* r2)	{ _prev = _top; _top = this; _pregs[0] = r0; _pregs[1] = r1; _pregs[2] = r2; }
  bool holds(PReg* preg) const;			// returns true if preg belongs to the locked PRegs

 public:
  PRegLocker(PReg* r0);
  PRegLocker(PReg* r0, PReg* r1);
  PRegLocker(PReg* r0, PReg* r1, PReg* r2);
  ~PRegLocker()					{ _top = _prev; }

  
  static bool locks(PReg* preg);		// returns true if preg is locked in any PRegLocker instance
  static void initialize()			{ _top = NULL; }
  friend class PRegMapping;
};


// A Temporary is a freely usable register allocated for the time the Temporary
// is alive. Temporaries must be created/destructed in a stack-fashioned manner.

class Temporary: StackObj {
 private:
  PRegMapping*	_mapping;
  int		_regLoc;

 public:
  Temporary(PRegMapping* mapping, Register hint = noreg);
  Temporary(PRegMapping* mapping, PReg* preg);	// keep a (modifiable) copy of the preg value in temporary register
  ~Temporary();

  Register reg() const				{ return _mapping->_locs->locationAsRegister(_regLoc); }
};


#endif