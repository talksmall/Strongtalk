/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.28 $ */
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
// These hold enough information to read or write a value embedded
// in the instructions of an nmethod.  They're used to update:
//   1) embedded oops     (isOop()          == true)
//   2) inline caches     (isIC()           == true)
//   3) primitive caches  (isPrimitive()    == true)
//   4) uncommom traps    (isUncommonTrap() == true)
//   5) runtime calls     (isRuntimeCall()  == true)
//   6) internal word ref (isInternalWord() == true) 
//   7) external word ref (isExternalWord() == true)
// when objects move (GC) or if code moves (compacting the code heap)
//
// A relocInfo is represented in 16 bits:
//   3 bits indicating the relocation type
//  13 bits indicating the byte offset from the previous relocInfo address

const int reloc_type_width   =  3;
const int reloc_offset_width = 13;

class relocInfo : ValueObj {
 protected:
  unsigned short _value;
 public:

  // constructor
  relocInfo(int type, int offset);

  enum relocType {
    none	       = -1,
    oop_type           =  0,	// embedded oop (non-smi)
    ic_type            =  1,	// inline cache
    prim_type          =  2,	// primitive call
    runtime_call_type  =  3, 	// Relative reference to external segment
    external_word_type =  4, 	// Absolute reference to external segment
    internal_word_type =  5, 	// Absolute reference to local    segment
    uncommon_type      =  6,	// uncommon branch
    dll_type           =  7  	// DLL call
  };

  // accessors
  int offset() const { 
    return get_unsigned_bitfield((int)_value, 0, reloc_offset_width);
  }
  int type() const { 
    return get_unsigned_bitfield((int)_value, reloc_offset_width, reloc_type_width);
  }

  // marks inline cache?
  bool isIC()           const { return type() == ic_type; }
  // marks primitive call?
  bool isPrimitive()    const { return type() == prim_type; }
  // marks uncommon trap?
  bool isUncommonTrap() const { return type() == uncommon_type; }
  // is oop encoded in instruction?
  bool isOop()          const { return type() == oop_type;  }
  // is runtime call?
  bool isRuntimeCall()  const { return type() == runtime_call_type; }
  // is internal word reference?
  bool isInternalWord() const { return type() == internal_word_type; }
  // is external word reference?
  bool isExternalWord() const { return type() == internal_word_type; }
  // is external word reference?
  bool isDLL()          const { return type() == dll_type; }

  // marks any call?
  bool isCall()         const { return !isOop(); }

  // If the offset is 0 the relocInfo is invalid.
  // Only used for padding (relocInfo array is oop aligned in nmethod).
  bool isValid()        const { return offset() != 0; } 
  
  // prints the relocation with retrieved information from the nmethod.
  int print(nmethod* c, int last_offset);
};

// A relocIterator iterates through the relocation information of a nmethod.
// Usage:
//    relocIterator iter(nm);
//    while (iter.next()) {
//      switch (iter.type()) {
//       case relocInfo::oop_type:
//       case relocInfo::ic_type:
//       case relocInfo::prim_type:
//       case relocInfo::uncommon_type:
//       case relocInfo::runtime_call_type:
//       case relocInfo::internal_word_type: 
//       case relocInfo::external_word_type: 
//      }
//    }

class relocIterator : StackObj {
 private:
   char*      addr;	  // this is the first instance variable comment  -Lars
   relocInfo* current;	  // this is the 2nd instance variable comment  -Lars
   relocInfo* end;
 public:

  // constructor
  relocIterator(const nmethod* nm);

  // get next reloc info, return !eos
  bool next() {
    current++;
    if (current == end) return false;
    addr += current->offset();
    return current->isValid();
  }

  // accessors
  int type() const { return current->type(); }

  bool is_call() const { return current->isCall(); }

  int* word_addr() const { return (int*) addr; }

  oop* oop_addr() const {
    assert(type() == relocInfo::oop_type, "must be oop");
    return (oop*) addr;
  }

  CompiledIC* ic() const {
    assert(type() == relocInfo::ic_type, "must be inline cache");
    return CompiledIC_from_relocInfo(addr);
  }

  PrimitiveIC* primIC() const {
    assert(type() == relocInfo::prim_type, "must be inline cache");
    return PrimitiveIC_from_relocInfo(addr);
  }

  /*
  CompiledDLL_Cache* DLLIC() const {
    assert(type() == relocInfo::dll_type, "must be inline cache");
    return compiledDLL_Cache_from_relocInfo(addr);
  }
  */

  char* call_end() const {
    assert(type() != relocInfo::oop_type, "must be call");
    return addr + 4;	// INTEL-SPECIFIC
  }

  char* callDestination() const {
    assert(type() != relocInfo::oop_type, "must be call");
    return *(char**)addr + int(addr) + 4;	// INTEL-SPECIFIC
  }

  // for uncommon traps only: was it ever executed?
  bool wasUncommonTrapExecuted() const;

  // is current reloc position dependent
  bool is_position_dependent() const;
};

#endif
