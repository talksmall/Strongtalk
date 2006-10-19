/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.23 $ */
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

// ICs describe the interface to a send in an nmethod.
// A IC can either describe:
//  - an inline cache for a Delta send (isDeltaSend() == true), or
//  - a primitive call                  (isPrimCall() == true).
//
//
// The memory layout of a Delta send (inline cache) is:
//  (offsets from return address)
// -5: call { _icLookupStub     (if not filled)
//          | nmethod address   (if filled with compiled    method)
//          | pic entry point}  (if filled with PIC/MIC)
//   				<--- CompiledIC* pointers point here
//  0: test reg,		<--- dummy test instruction used to hold ic info
//  1: ic info
//  5: ...			<--- first instruction after move
//
//
// The memory layout of a primitive call is (offsets from return address):
//
// -5: call ...
//   				<--- PrimitiveIC* pointers point here
//  0: test reg,		<--- dummy test instruction used to hold ic info (only if NLR is possible)
//  1: ic info
//  5: ...			<--- first instruction after move
//
//
// IC info layout (see als IC_Info for layout constants)
//
// [NLR offset|flags]		32bit immediate of the dummy test instruction
//  31.......8|7...0
//
// The NLR offset is a signed int, the NLR target destination is computed
// from the (call's) return address + NLR offset.
//
// Calling nmethods through a jump table will cost 10% of the total execution speed
// (data provided by Urs 4-21-95)


class AbstractCompiledIC: public NativeCall {
 public:
   char* NLR_testcode() const		{ return ic_info_at(next_instruction_address())->NLR_target(); }
   
   // returns the beginning of the inline cache. 
   char* begin_addr() const		{ return instruction_address(); }
};


// Flags

const int dirty_send_bit_no		= 0;
const int optimized_bit_no		= 1;
const int uninlinable_bit_no		= 2;
const int super_send_bit_no		= 3;
const int megamorphic_bit_no		= 4;
const int receiver_static_bit_no	= 5;


// CompiledIC isn't a real object; the 'this' pointer points into the compiled code
// (more precisely, it's the same as the return address of the callee)

class CompiledIC: public AbstractCompiledIC {
 protected:
  int compiler_info() const		{ return ic_info_at(next_instruction_address())->flags(); }
  void set_compiler_info(int info)	{ ic_info_at(next_instruction_address())->set_flags(info); }

 public:
  // lookup routines for empty inline cache
  static char* normalLookupRoutine();
  static char* superLookupRoutine();

  // conversion (machine PC to CompiledIC*)
  friend CompiledIC* CompiledIC_from_return_addr(char* return_addr);

  friend CompiledIC* CompiledIC_from_relocInfo(char* displacement_address);

  // Accessors

  // isDirty() --> has had misses
  bool isDirty() const			{ return isSet(compiler_info(), dirty_send_bit_no); }
  void setDirty()			{ set_compiler_info(addNth(compiler_info(), dirty_send_bit_no)); }

  // isOptimized() --> nmethods called from here should be optimized
  bool isOptimized() const		{ return isSet(compiler_info(), optimized_bit_no); }	
  void setOptimized()			{ set_compiler_info(addNth(compiler_info(), optimized_bit_no)); }
  void resetOptimized()			{ set_compiler_info(subNth(compiler_info(), optimized_bit_no)); }

  // isUninlinable() --> compiler says don't try to inline this send
  bool isUninlinable() const		{ return isSet(compiler_info(), uninlinable_bit_no); }
  
  // isSuperSend() --> send is a super send
  bool isSuperSend() const		{ return isSet(compiler_info(), super_send_bit_no); }

  // isMegamorphic() --> send is megamorphic
  bool isMegamorphic() const		{ return isSet(compiler_info(), megamorphic_bit_no); }	
  void setMegamorphic()			{ set_compiler_info(addNth(compiler_info(), megamorphic_bit_no)); }

  // isReceiverStatic() --> receiver klass is known statically (connect to verifiedEntryPoint)
  bool isReceiverStatic() const		{ return isSet(compiler_info(), receiver_static_bit_no); }	
  void setReceiverStatic()		{ set_compiler_info(addNth(compiler_info(), receiver_static_bit_no)); }

  bool wasNeverExecuted() const;

  InterpretedIC* inlineCache() const;	// corresponding source-level inline cache (NULL if none, e.g. perform)
  PIC*	pic() const;			// NULL if 0 or 1 targets
    
  // returns the first address after this primitive ic
  char* end_addr()			{ return ic_info_at(next_instruction_address())->next_instruction_address(); }

  // sets the destination of the call instruction
  void set_call_destination(char* entry_point);

  // Does a lookup in the receiver and patches the inline cache
  char* normalLookup(oop receiver);
  char* superLookup (oop receiver);

  // Returns the class that holds the current method
  klassOop sending_method_holder();

  // replace appropriate target (with key nm->key) by nm
  void replace(nmethod* nm);	
  
 public:
  symbolOop	selector() const;
  bool 		is_empty() const;
  bool 		is_monomorphic() const;
  bool 		is_polymorphic() const;
  bool 		is_megamorphic() const;
  
  nmethod* 	target() const;  	// directly called nmethod or NULL if none/PIC
  klassOop	targetKlass() const;    // klass of compiled or interpreted target;
					// can only call if single target
  int 		ntargets() const;	// number of targets in inline cache or PIC
  klassOop 	get_klass(int i) const; // receiver klass of ith target (i=0..ntargets()-1)
  
  // returns the lookup key for PIC index
  LookupKey* 	key(int which, bool is_normal_send) const;
  void 		reset_jump_addr();
  void 		link(PIC* s);
  void 		clear();		// clear inline cache
  void 		cleanup();		// cleanup inline cache

  bool verify();
  void print();
};

inline CompiledIC* CompiledIC_from_return_addr(char* return_addr)
{ 
    return (CompiledIC*)nativeCall_from_return_address(return_addr);
}

inline CompiledIC* CompiledIC_from_relocInfo(char* displacement_address)
{
    return (CompiledIC*)nativeCall_from_relocInfo(displacement_address);
}

class PrimitiveIC: public AbstractCompiledIC {
 public:
  // returns the primitive descriptor (based on the destination of the call).
  primitive_desc* primitive();

  // conversion (machine PC to PrimitiveIC*)
  friend PrimitiveIC* PrimitiveIC_from_return_addr(char* return_addr);

  friend PrimitiveIC* PrimitiveIC_from_relocInfo(char* displacement_address);

  // returns the first address after this primitive ic.
  char* end_addr();

  void print();
};

inline PrimitiveIC* PrimitiveIC_from_return_addr(char* return_addr)
{ 
    return (PrimitiveIC*)nativeCall_from_return_address(return_addr);
}

  inline PrimitiveIC* PrimitiveIC_from_relocInfo(char* displacement_address)
{
    return (PrimitiveIC*)nativeCall_from_relocInfo(displacement_address);
}

#endif
