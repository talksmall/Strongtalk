/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.12 $ */
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

// Locations manages register and stack locations. All locations are numbered. A fixed range
// of indices >= 0 is assigned to arguments & registers and a growable range of indices >= 0
// is assigned to stack locations.
//
// Note: Do not confuse Locations with the (old) class Location, which should disappear
//       as soon as the new backend is up and running.

class Locations: public PrintableResourceObj {
 private:
  int			_nofArguments;		// the number of arguments
  int			_nofRegisters;		// the maximum number of available registers
  GrowableArray<int>*	_freeList;		// the list of free locations
  int			_firstFreeRegister;	// the index of the first free register in _freeList
  int			_firstFreeStackTmp;	// the index of the first free stack temporary in _freeList

  int  argumentsBeg() const			{ return 0; }
  int  argumentsEnd() const			{ return _nofArguments; }
  int  registersBeg() const			{ return _nofArguments; }
  int  registersEnd() const			{ return _nofArguments + _nofRegisters; }
  int  stackTmpsBeg() const			{ return _nofArguments + _nofRegisters; }
  int  stackTmpsEnd() const			{ return _freeList->length(); }
  int  locationsBeg() const			{ return 0; }
  int  locationsEnd() const			{ return _freeList->length(); }

 public:
  enum {
    noLocation			= -1,		// isLocation(noLocation) return false
    maxNofUsableRegisters	= 6,		// the maximum number of usable registers (<= nofRegisters)
    sentinel			= 999999999	// simply much bigger than _freeList can ever get
  };

  Locations(int nofArgs, int nofRegs, int nofInitialStackTmps);	// nofRegisters <= maxNofUsableRegisters
  Locations(Locations* l);			// to copy locations

  void extendTo(int nofStackTmps);

  // Location management
  int  allocateRegister();			// allocates a new register (fatal if !freeRegisters())
  int  allocateStackTmp();			// allocates a new stack location
  void allocate(int i);				// allocates location i, i must have been unallocated
  void use(int i);				// uses location i once again, i must be allocated already
  void release(int i);				// releases a register or stack location

  // Testers
  int  nofUses(int i) const;			// the number of times the location has been use'd (including allocation)
  int  nofTotalUses() const;			// the number of total uses of all locations (for verification purposes)
  int  nofArguments() const			{ return _nofArguments; }
  int  nofRegisters() const			{ return _nofRegisters; }
  int  nofStackTmps() const			{ return stackTmpsEnd() - stackTmpsBeg(); }
  int  nofFreeRegisters() const;		// the number of available registers

  bool freeRegisters() const			{ return _firstFreeRegister != sentinel; }
  bool isLocation(int i) const			{ return locationsBeg() <= i && i < locationsEnd(); }
  bool isArgument(int i) const			{ return argumentsBeg() <= i && i < argumentsEnd(); }
  bool isRegister(int i) const			{ return registersBeg() <= i && i < registersEnd(); }
  bool isStackTmp(int i) const			{ return stackTmpsBeg() <= i && i < stackTmpsEnd(); }
  bool isStackLoc(int i) const			{ return isArgument(i) || isStackTmp(i); }

  // Machine-dependent mapping of Registers/locations
  int  freeRegisterMask() const;		// bit i corresponds to register i; bit set <==> register is free
  int  usedRegisterMask() const;		// bit i corresponds to register i; bit set <==> register is used

  int  argumentAsLocation(int argNo) const;	// the location encoding for argument argNo
  int  registerAsLocation(Register reg) const;	// the location encoding for register reg
  int  temporaryAsLocation(int tempNo) const;	// the location encoding for temporary tempNo

  Register locationAsRegister  (int loc) const;	// the register corresponding to loc
  int      locationAsRegisterNo(int loc) const  { return locationAsRegister(loc).number(); }
  int      locationAsWordOffset(int loc) const;	// the (ebp) word offset corresponding to loc
  int      locationAsByteOffset(int loc) const	{ return locationAsWordOffset(loc) * oopSize; }
  Address  locationAsAddress   (int loc) const	{ return Address(ebp, locationAsByteOffset(loc)); }

  // Debugging
  void print();
  void verify();
};

#endif
