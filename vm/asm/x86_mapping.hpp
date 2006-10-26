/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.27 $ */
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

// Register usage
const int nofArgRegisters	= 0;			// max. number of arguments (excl. receiver) passed in registers
const int nofLocalRegisters	= 3;			// max. number of temporaries allocated in registers


// Temporaries on the stack
const int first_temp_offset	= -1;			// offset of first temporary relative to ebp if there are no floats
const int first_float_offset	= -4;			// offset of first float relative to 8byte aligned ebp value (= base)


// Mapping specifies the x86 architecture specific constants and
// code sequences that are valid machine-independently.

class Mapping: AllStatic {
 private:
  static Location _localRegisters[nofLocalRegisters+1]; // the list of local registers
  static int      _localRegisterIndex[nofRegisters+1]; 	// the inverse of localRegisters[]

 public:
  // initialization
  static void initialize();

  // register allocation
  static Location localRegister(int i);      		// the i.th local register (i = 0 .. nofLocalRegisters-1)
  static int localRegisterIndex(Location l); 		// the index of local register l (localRegisterIndex(localRegister(i)) = i)

  // parameter passing
  static Location incomingArg(int i, int nofArgs);	// incoming argument (excluding receiver; i >= 0, 0 = first arg)
  static Location outgoingArg(int i, int nofArgs);	// outgoing argument (excluding receiver; i >= 0, 0 = first arg)

  // stack allocation
  static Location localTemporary(int i); 		// the i.th local temporary (i >= 0)
  static int localTemporaryIndex(Location l);		// the index of the local temporary l (localTemporaryIndex(localTemporary(i)) = i)
  static Location floatTemporary(int scope_id, int i);	// the i.th float temporary within a scope (i >= 0)
  
  // context temporaries
  static int contextOffset(int tempNo);			// the byte offset of temp from the contextOop
  static Location contextTemporary(int contextNo, int i, int scope_id);	     // the i.th context temporary (i >= 0)
  static Location* new_contextTemporary(int contextNo, int i, int scope_id); // ditto, but allocated in resource area

  // conversion functions
  static Location asLocation(Register reg)		{ return Location::registerLocation(reg.number()); }
  static Register asRegister(Location loc)		{ return Register(loc.number(), ' '); }

  // predicates
  static bool isTemporaryRegister(const Location loc)	{ return false; }	// fix this
  static bool isLocalRegister(const Location loc)	{ return _localRegisterIndex[loc.number()] != -1; }
  static bool isTrashedRegister(const Location loc)	{ return true; }	// fix this

  static bool isNormalTemporary(Location loc);
  static bool isFloatTemporary(Location loc);

  // helper functions for code generation
  //
  // needsStoreCheck determines whether a store check is needed when storing into a context location
  // (e.g., no storeCheck is needed when initializing individual context fields because there's one
  // store check after context creation).
  static void load(Location src, Register dst);
  static void store(Register src, Location dst, Register temp1, Register temp2, bool needsStoreCheck);
  static void storeO(oop obj, Location dst, Register temp1, Register temp2, bool needsStoreCheck);

  // helper functions for float code
  static void fload(Location src, Register base);
  static void fstore(Location dst, Register base);
};


// calls
const Register self_reg		= eax;			// incoming receiver location (in prologue of callee)
const Register receiver_reg	= eax;			// outgoing receiver location (before call)
const Register result_reg	= eax;			// outgoing result location (before exit)
const Register frame_reg	= ebp;			// activation frame pointer

const Location selfLoc		= Mapping::asLocation(self_reg);
const Location receiverLoc 	= Mapping::asLocation(receiver_reg);
const Location resultLoc	= Mapping::asLocation(result_reg);
const Location frameLoc		= Mapping::asLocation(frame_reg);


// non-local returns (make sure to adjust the corresponding constants in interpreter_asm.asm when changing these)
const Register NLR_result_reg	= eax;			// result being returned
const Register NLR_home_reg	= edi;			// frame ptr of home frame (stack)
const Register NLR_homeId_reg	= esi;			// scope id of home scope (inlining)

const Location NLRResultLoc	= Mapping::asLocation(NLR_result_reg);
const Location NLRHomeLoc	= Mapping::asLocation(NLR_home_reg);
const Location NLRHomeIdLoc	= Mapping::asLocation(NLR_homeId_reg);


// temporaries for local code generation (within one Node only)
// note: these locations must not intersect with any location used
// for non-local returns!
const Register temp1		= ebx;
const Register temp2		= ecx;
const Register temp3		= edx;


# endif
