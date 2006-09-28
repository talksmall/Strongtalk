/* Copyright 1994 - 1996, LongView Technologies, L.L.C. $Revision: 1.4 $ */
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


// The base class for differnt kinds of native instruction abstractions.
// Provides the primitive operations to manipulate code relative to this.

class NativeInstruction: ValueObj {
 protected:
  char*	addr_at(int offset) const		{ return (char*)this + offset; }

  char  char_at(int offset) const		{ return *addr_at(offset); }
  int   long_at(int offset) const		{ return *(int*)addr_at(offset); }
  oop   oop_at (int offset) const		{ return *(oop*)addr_at(offset); }

  void  set_char_at(int offset, char c)		{ *addr_at(offset) = c; }
  void  set_long_at(int offset, int  i)		{ *(int*)addr_at(offset) = i; }
  void  set_oop_at (int offset, oop  o)		{ *(oop*)addr_at(offset) = o; }
};


// An abstraction for accessing/manipulating native call imm32 instructions.
// (used to manipulate inline caches, primitive & dll calls, etc.)

class NativeCall: public NativeInstruction {
 public:
  enum Intel_specific_constants {
    instruction_code		= 0xE8,
    instruction_size		=    5,
    instruction_offset		=   -5,
    displacement_offset		=   -4,
    return_address_offset	=    0,
  };

  char* instruction_address() const		{ return addr_at(instruction_offset); }
  char* next_instruction_address() const	{ return addr_at(return_address_offset); }
  int   displacement() const			{ return long_at(displacement_offset); }
  char* return_address() const			{ return addr_at(return_address_offset); }
  char* destination() const			{ return return_address() + displacement(); }
  void  set_destination(char* dest)		{ set_long_at(displacement_offset, dest - return_address()); }

  void  verify();
  void  print();
  
  // Creation
  friend NativeCall* nativeCall_at(char* address);
  friend NativeCall* nativeCall_from_return_address(char* return_address);
  friend NativeCall* nativeCall_from_relocInfo(char* displacement_address);
};


// An abstraction for accessing/manipulating native mov reg, imm32 instructions.
// (used to manipulate inlined 32bit data dll calls, etc.)

class NativeMov: public NativeInstruction {
 public:
  enum Intel_specific_constants {
    instruction_code		= 0xB8,
    instruction_size		=    5,
    instruction_offset		=    0,
    data_offset			=    1,
    next_instruction_offset	=    5,
    register_mask		= 0x07,
  };

  char* instruction_address() const		{ return addr_at(instruction_offset); }
  char* next_instruction_address() const	{ return addr_at(next_instruction_offset); }
  int   data() const				{ return long_at(data_offset); }
  void  set_data(int x)				{ set_long_at(data_offset, x); }

  void  verify();
  void  print();
  
  // Creation
  friend NativeMov* nativeMov_at(char* address);
};



// An abstraction for accessing/manipulating native test eax, imm32 instructions.
// (used to manipulate inlined 32bit data for NLRs, dll calls, etc.)

class NativeTest: public NativeInstruction {
 public:
  enum Intel_specific_constants {
    instruction_code		= 0xA9,
    instruction_size		=    5,
    instruction_offset		=    0,
    data_offset			=    1,
    next_instruction_offset	=    5,
  };

  char* instruction_address() const		{ return addr_at(instruction_offset); }
  char* next_instruction_address() const	{ return addr_at(next_instruction_offset); }
  int   data() const				{ return long_at(data_offset); }
  void  set_data(int x)				{ set_long_at(data_offset, x); }

  void  verify();
  void  print();
  
  // Creation
  friend NativeTest* nativeTest_at(char* address);

};


// An abstraction for accessing/manipulating native test eax, imm32 instructions that serve as IC info.

class IC_Info: public NativeTest {
 public:
  enum IC_Info_specific_constants {
    info_offset		= data_offset,
    number_of_flags	= 8,
    flags_mask		= (1 << number_of_flags) - 1,
  };

  char* NLR_target() const			{ return instruction_address() + (data() >> number_of_flags); }
  int	flags() const				{ return data() & flags_mask; }
  void  set_flags(int flags)			{ set_data((data() & ~flags_mask) | (flags & flags_mask)); }

  // Creation
  friend IC_Info* ic_info_at(char* address);
};


