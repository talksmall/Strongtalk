/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.12 $ */
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

// A NameDesc describes the source-level value of a name in some Delta scope
// (e.g. an argument, local, or expression stack entry).  NameDescs are usually
// stored in the zone as part of the debugging information.

class NameDesc: public PrintableResourceObj {
  // are ResourceObj because some are created on-the-fly
 public:
  virtual bool isLocation() const 	{ return false; }
  virtual bool isValue() const 		{ return false; }
  virtual bool isBlockValue() const 	{ return false; }
  virtual bool isMemoizedBlock() const	{ return false; }
  virtual bool isIllegal() const 	{ return false; }

  virtual bool hasLocation() const 	{ return false; }
  virtual Location location() const	{ SubclassResponsibility(); return unAllocated; }
  virtual oop value(const frame* f = NULL) const { SubclassResponsibility(); return NULL; } 
  
  virtual bool verify()			{ return true; }
  virtual void print()			= 0;

  virtual bool equal(NameDesc* other) const { return false; }

  int offset;
};

// something stored at a location
struct LocationNameDesc: public NameDesc {
  Location _loc;

  LocationNameDesc(Location loc) {
    _loc = loc; 
  }

  bool isLocation() const 	{ return true; }
  Location location() const 	{ return _loc; }
  bool hasLocation() const  	{ return true; }

  bool equal(NameDesc* other) const; 
  
  void print();
};

// a run-time constant
struct ValueNameDesc: public NameDesc {
  oop _v;

  ValueNameDesc(oop v) {
    _v = v;
  }
  
  bool isValue() const { return true; }
  
  oop value(const frame* f = NULL) const { return _v; }

  bool equal(NameDesc* other) const; 

  void print();
};

// a block closure "constant", i.e., a block that has been optimized away
struct BlockValueNameDesc: public NameDesc {
  methodOop  _block_method;
  ScopeDesc* _parent_scope;
  
  BlockValueNameDesc(methodOop block_method, ScopeDesc* parent_scope) {
    _block_method = block_method;
    _parent_scope = parent_scope;
  }

  bool isBlockValue() const 	{ return true; }
  
  // Returns a blockClosureOop
  // There are two cases:
  // 1. During deoptmization, 
  //      where the contextOop referred by the block must be canonicalized
  //      to preserve language semantics.
  // 2. Normal operation (use during stack tracing etc.), 
  //      where contextOop canonicalization is not needed.     
  oop value(const frame* f = NULL) const;

  methodOop block_method() const  { return _block_method; }
  ScopeDesc* parent_scope() const { return _parent_scope; }

  bool equal(NameDesc* other) const; 
  
  void print();
};

// a block closure that may or may not be created at runtime, so location l 
// contains either the real block or a dummy block
struct MemoizedBlockNameDesc: public NameDesc {
  Location   _loc;
  methodOop  _block_method;
  ScopeDesc* _parent_scope;

  MemoizedBlockNameDesc(Location loc, methodOop block_method, ScopeDesc* parent_scope) {
    _loc          = loc;
    _block_method = block_method;
    _parent_scope = parent_scope;
  }

  bool isMemoizedBlock() const 		{ return true; }
  
  Location location() const 		{ return _loc; }
  methodOop block_method() const        { return _block_method; }
  ScopeDesc* parent_scope() const       { return _parent_scope; }
  static oop uncreatedBlockValue()	{ return smiOop_zero; }

  bool hasLocation() const  		{ return true; }
  oop value(const frame* f = NULL) const;

  bool equal(NameDesc* other) const; 
  
  void print();
};

struct IllegalNameDesc: public NameDesc {

  IllegalNameDesc() {}
  bool isIllegal() const 	{ return true; }

  bool equal(NameDesc* other) const; 
  
  void print();
};

#endif
