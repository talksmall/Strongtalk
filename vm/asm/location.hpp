/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.18 $ */
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

# ifdef DELTA_COMPILER

// Locations serve as abstractions for physical addresses. For each
// physical location (register, stack position or context temporary),
// there is a corresponding location and vice versa.

enum Mode {
// mode\bits		3...................31	describes
//			3..9	10..16	17..31
   specialLoc,	//	--------id------------	sentinel values/global locations
   registerLoc,	//	--------regLoc--------	register locations
   stackLoc,	//	--------offset--------	stack locations
   contextLoc1,	//	ctxtNo	offset	scID	context locations during compilation (scope ID identifies InlinedScope)
   contextLoc2,	//	ctxtNo	offset	scOffs	context locations (scOffs is scope offset within encoded scopes)
   floatLoc,	//	0	floatNo	scopeN  float locations
};


class Location: public ResourceObj /* but usually used as ValueObj */ {
 private:
  int _loc;	// location encoding

  // field layout of _loc (no local const definitions allowed in C++)
  enum {
    _f1Pos	= 3,			_f1Len	= 7,	_f1Mask	= (1<<_f1Len) - 1,
    _f2Pos	= _f1Pos + _f1Len,	_f2Len	= 7,	_f2Mask	= (1<<_f2Len) - 1,
    _f3Pos	= _f2Pos + _f2Len,	_f3Len	= 15,	_f3Mask	= (1<<_f3Len) - 1,
    _fPos	= _f1Pos,		_fLen	= 29,	_fMask	= (1<<_fLen ) - 1,
  };

  void overflow(Mode mode, int f1, int f2, int f3);				// handle field overflow if possible

 public:
  // default constructor - for better debugging
  Location()									{ specialLocation(0); }
  Location(int l)								{ _loc = l; }
  Location(Mode mode, int f);
  Location(Mode mode, int f1, int f2, int f3);

  // factory
  static Location specialLocation(int id)					{ return Location(specialLoc, id); }
  static Location registerLocation(int number)					{ return Location(registerLoc, number); }
  static Location stackLocation(int offset)					{ return Location(stackLoc, offset); }
  static Location compiledContextLocation(int contextNo, int tempNo, int id)	{ return Location(contextLoc1, contextNo, tempNo, id); }
  static Location runtimeContextLocation(int contextNo, int tempNo, int offs)	{ return Location(contextLoc2, contextNo, tempNo, offs); }
  static Location floatLocation(int scopeNo, int tempNo)			{ return Location(floatLoc, 0, tempNo, scopeNo); }

  // attributes
  Mode mode() const { return (Mode)(_loc & ((1<<_f1Pos) - 1)); }

  int id() const {
    assert(mode() == specialLoc, "not a special location");
    return (_loc>>_fPos) & _fMask;
  }

  int number() const {
    assert(mode() == registerLoc, "not a register location");
    return (_loc>>_fPos) & _fMask;
  }

  int offset() const {
    assert(mode() == stackLoc, "not a stack location");
    int t = _loc>>_fPos;
    return _loc < 0 ? (t | ~_fMask) : t;
  }

  int contextNo() const {
    assert(mode() == contextLoc1 || mode() == contextLoc2, "not a context location");
    return (_loc>>_f1Pos) & _f1Mask;
  }

  int tempNo() const {
    assert(mode() == contextLoc1 || mode() == contextLoc2, "not a context location");
    return (_loc>>_f2Pos) & _f2Mask;
  }

  int scopeID() const {
    assert(mode() == contextLoc1, "not a compiled context location");
    return (_loc>>_f3Pos) & _f3Mask;
  }

  int scopeOffs() const {
    assert(mode() == contextLoc2, "not a runtime context location");
    return (_loc>>_f3Pos) & _f3Mask;
  }

  int floatNo() const {
    assert(mode() == floatLoc, "not a float location");
    return (_loc>>_f2Pos) & _f2Mask;
  }

  int scopeNo() const {
    assert(mode() == floatLoc, "not a float location");
    return (_loc>>_f3Pos) & _f3Mask;
  }

  // helper functions
  char* name() const;

  // predicates
  bool isSpecialLocation() const	{ return mode() == specialLoc; }
  bool isRegisterLocation() const	{ return mode() == registerLoc; }
  bool isStackLocation() const		{ return mode() == stackLoc; }
  bool isContextLocation() const	{ return mode() == contextLoc1 || mode() == contextLoc2; }
  bool isFloatLocation() const		{ return mode() == floatLoc; }
  bool isTopOfStack() const;
  bool equals(Location y) const		{ return _loc == y._loc; }

  friend bool operator == (Location x, Location y) {return x._loc == y._loc; }
  friend bool operator != (Location x, Location y) {return x._loc != y._loc; }
  
  bool isTemporaryRegister() const;
  bool isTrashedRegister() const;
  bool isLocalRegister() const;

  // For packing and unpacking scope information
  friend class LocationName;
  friend class MemoizedName;
  friend class nmethodScopes;
  friend class DebugInfoWriter;
};


// special locations
const int nofSpecialLocations	= 6;
const Location illegalLocation	= Location::specialLocation(0);
const Location unAllocated	= Location::specialLocation(1);
const Location noRegister	= Location::specialLocation(2);
const Location topOfStack	= Location::specialLocation(3);
const Location resultOfNLR	= Location::specialLocation(4);
const Location topOfFloatStack	= Location::specialLocation(5);	// only used if UseFPUStack is true


// An IntFreeList maintains a list of 'available' integers in the range [0, n[
// where n is the maximum number of integers ever allocated. An IntFreeList may
// be used to allocate/release stack locations.

class IntFreeList : public PrintableResourceObj {
 protected:
  int			_first;	// the first available integer
  GrowableArray<int>*	_list;	// the list

  void grow();

 public:
  IntFreeList(int size);

  int	allocate();		// returns a new integer, grows the list if necessary
  int	allocated();		// returns the number of allocated integers
  void	release(int i);		// marks the integer i as 'available' again
  int	length();		// the maximum number of integers ever allocated
  void	print();
};


# endif
