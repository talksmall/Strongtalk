/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.50 $ */
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

// A frame represents a physical stack frame (an activation).  Frames can be
// C or Delta frames, and the Delta frames can be interpreted or compiled.
// In contrast, vframes represent source-level activations, so that one (Delta) frame 
// can correspond to multiple deltaVFrames because of inlining.

// Layout of interpreter frame:
//    [locals + expr         ] * <- sp
//    [old frame pointer     ]   <- fp
//    [return pc             ]
//    [previous locals + expr]   <- sender sp

// Layout of deoptimized frame:
//    [&unpack_unoptimized_frame ]		// patched return address
//    [scrap area                ] * <- sp
//    [sender sp                 ]
//    [frame array               ]              // objArrayOop holding the deoptimized frames
//    [old frame                 ]   <- fp	// old frame may skip real frames deoptimized away.
//    [return pc                 ]

const int frame_temp_offset          = -3; // For interpreter frames only
const int frame_hp_offset            = -2; // For interpreter frames only
const int frame_receiver_offset      = -1; // For interpreter frames only
const int frame_next_Delta_fp_offset = -1; // For entry frames only; see call_delta in interpreter_asm.asm
const int frame_next_Delta_sp_offset = -2; // For entry frames only; see call_delta in interpreter_asm.asm
const int frame_link_offset          =  0;
const int frame_return_addr_offset   =  1;
const int frame_arg_offset           =  2;
const int frame_sender_sp_offset     =  2;

const int frame_real_sender_sp_offset   = -2; // For deoptimized frames only
const int frame_frame_array_offset      = -1; // For deoptimized frames only

const int interpreted_frame_float_magic_offset = frame_temp_offset - 1;
const int compiled_frame_magic_oop_offset      = -1;
const int minimum_size_for_deoptimized_frame   = 4;
    
class frame : ValueObj {
 private:
  oop*  _sp; // stack pointer
  int*  _fp; // frame pointer
  char* _pc; // program counter

 public:
  // Constructors
  frame() {}
  frame(oop* sp, int* fp, char* pc) { 
    _sp = sp; _fp = fp; _pc = pc;
  }

  frame(oop* sp, int* fp) { 
    _sp = sp;
    _fp = fp;
    _pc = (char*) sp[-1];
  }

  // accessors for the instance variables
  oop*  sp() const 			{ return _sp; }
  int*  fp() const 			{ return _fp; } // should return void**
  char* pc() const 			{ return _pc; }

  // patching operations
  void patch_pc(char* pc); // patch the return address of the frame below.
  void patch_fp(int*  fp); // patch the link of the frame below.

  int*   addr_at(int index) const 	{ return &fp()[index];    } // should return void**
  int    at(int index) const      	{ return *addr_at(index); } // should really return void*

 private:
  int**  link_addr() const        	{ return (int**) addr_at(frame_link_offset); }
  char** return_addr_addr() const 	{ return (char**) addr_at(frame_return_addr_offset); }

  // support for interpreter frames
  oop*   receiver_addr() const    	{ return (oop*) addr_at(frame_receiver_offset); }
  u_char** hp_addr() const          	{ return (u_char**) addr_at(frame_hp_offset); }
  oop*   arg_addr(int off) const  	{ return (oop*) addr_at(frame_arg_offset + off); }

 public:
  // returns the stack pointer of the calling frame
  oop* sender_sp() const 		{ return (oop*)  addr_at(frame_sender_sp_offset); }

  // Link
  int*     link() const            	{ return *link_addr(); }
  void set_link(int* addr) 		{ *link_addr() = addr; }

  // Return address
  char*    return_addr() const          { return *return_addr_addr(); }
  void set_return_addr(char* addr) 	{ *return_addr_addr() = addr; }

  // Receiver
  oop      receiver() const		{ return *receiver_addr(); }
  void set_receiver(oop recv)		{ *receiver_addr() = recv; }

  // Temporaries
  oop       temp(int offset) const	{ return *temp_addr(offset); }
  void  set_temp(int offset, oop obj)	{ *temp_addr(offset) = obj; }
  oop* temp_addr(int offset) const 	{ return (oop*) addr_at(frame_temp_offset - offset); }

  // Arguments
  oop      arg(int offset) const	{ return *arg_addr(offset); }
  void set_arg(int offset, oop obj)	{ *arg_addr(offset) = obj; }

  // Expressions
  oop      expr(int index) const	{ return ((oop*)sp())[index]; }

  // Hybrid Code Pointer (interpreted frames only); corresponds to "current PC", not return address
  u_char*   hp() const;
  void  set_hp(u_char* hp);

  // Returns the method for a valid hp() or NULL if frame not set up yet (interpreted frames only) 
  // Used by the profiler which means we must check for
  // valid frame before using the hp value.
  methodOop method() const;

  // compiled code (compiled frames only)
  nmethod* code() const;

 private:
  // Float support
  inline bool has_interpreted_float_marker() const;
  bool oop_iterate_interpreted_float_frame(OopClosure* blk);
  bool follow_roots_interpreted_float_frame();

  inline bool has_compiled_float_marker() const;
  bool oop_iterate_compiled_float_frame(OopClosure* blk);
  bool follow_roots_compiled_float_frame();
 public:

  // Accessors for (deoptimized frames only)
  objArrayOop* frame_array_addr() const;  
  oop**        real_sender_sp_addr() const;
 
  objArrayOop frame_array() const;
  void  set_frame_array(objArrayOop a) { *frame_array_addr() = a; }

  oop*  real_sender_sp() const         { return *real_sender_sp_addr(); }
  void  set_real_sender_sp(oop* addr)  { *real_sender_sp_addr() = addr;   }

  // returns the frame size in oops
  int frame_size() const 		{ return sender_sp() - sp(); }

  // returns the the sending frame
  frame sender() const;
  // returns the the sending Delta frame, skipping any intermediate C frames 
  // NB: receiver must not be first frame
  frame delta_sender() const;

  // tells whether there is another chunk of Delta stack above (entry frames only)
  bool has_next_Delta_fp() const;
  // returns the next C entry frame (entry frames only)
  int* next_Delta_fp()     const;
  oop* next_Delta_sp()     const;

  bool is_first_frame() const;			// oldest frame? (has no sender)
  bool is_first_delta_frame() const;		// same for Delta frame

  // testers
  bool is_interpreted_frame() const;
  bool is_compiled_frame()    const;
  bool is_delta_frame()       const	{ return is_interpreted_frame() || is_compiled_frame(); }

  bool should_be_deoptimized() const;
  bool is_entry_frame()        const;		// Delta frame called from C?
  bool is_deoptimized_frame()  const;

  // inline caches
  IC_Iterator* sender_ic_iterator() const;	// sending IC (NULL if entry frame or if a perform rather than a send)
  IC_Iterator* current_ic_iterator() const;	// current IC (will break if not at a send or perform)
  InterpretedIC* current_interpretedIC() const;	// current IC in this frame; NULL if !is_interpreted_frame 
  CompiledIC* current_compiledIC() const;	// current IC in this frame; NULL if !is_compiled_frame

  // Iterators
  void oop_iterate(OopClosure* blk);
  void layout_iterate(FrameLayoutClosure* blk);

  // For debugging
 private:
  char* print_name() const;

 public:
  void verify() const;
  void print() const;

  // Prints the frame in a format useful when debugging deoptimization.
  void print_for_deoptimization(outputStream* st);

  // Garbage collection operations
  void follow_roots();
  void convert_hcode_pointer();
  void restore_hcode_pointer();

  // Returns the size of a number of interpreter frames in words.
  // This is used during deoptimization.
  static int interpreter_stack_size(int number_of_frames, int number_of_temporaries_and_locals) {
    return number_of_frames * interpreter_frame_size(0) 
         + number_of_temporaries_and_locals;
  }

  // Returns the word size of an interpreter frame
  static int interpreter_frame_size(int locals) {
    return frame_return_addr_offset - frame_temp_offset + locals;
  }
};
