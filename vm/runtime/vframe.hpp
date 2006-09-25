/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.45 $ */
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


// vframes are virtual stack frames representing source level activations.
// A deltaVFrame represents an activation of a Delta level method. A single
// frame may hold several source level activations in the case of 
// optimized code. The debugging stored with the optimized code enables
// us to unfold a frame as a stack of vframes.
// A cVFrame represents an activation of a non-Delta method.

// The vframe inheritance hierarchy:
// - vframe
//   - deltaVFrame
//     - interpretedVFrame
//     - compiledVFrame
//     - deoptimizedVFrame  ; special deltaVFrame for deoptimized off stack VFrames
//   - cVFrame
//     - cChunk             ; special frame created when entering Delta from C

class vframe: public PrintableResourceObj {
 private:
  // Interface for the accessing the callees argument.
  // Must be provided for all vframes calling deltaVFrames.
  virtual oop callee_argument_at(int index) const;

 protected:
  frame _fr;

  vframe(const frame* fr) { _fr = *fr; }	// for subclass use only

 public:
  // constructor: creates bottom (most recent) vframe from a frame
  static vframe* new_vframe(frame* f);

  // returns the frame
  frame fr() const { return _fr; }

  // returns the sender vframe
  virtual vframe* sender() const;

  // answers if the receiver is the top vframe in the frame, i.e., if the sender vframe 
  // is in the caller frame
  virtual bool is_top() const { return true; }

  // returns top vframe within same frame (see is_top())	
  virtual vframe* top() const;		

  // comparison operation
  virtual bool equal(const vframe* f) const;

  // type testing operations
  virtual bool is_c_frame()           const { return false; }
  virtual bool is_c_chunk()           const { return false; }
  virtual bool is_delta_frame()       const { return false; }
  virtual bool is_interpreted_frame() const { return false; }
  virtual bool is_compiled_frame()    const { return false; }
  virtual bool is_deoptimized_frame() const { return false; }

  // printing operations
  virtual void print_value() const;
  virtual void print();

  // verify operations
  virtual void verify() const {};

  // friends
  friend class interpretedVFrame;
  friend class compiledVFrame;
  friend class cVFrame;
  friend class deltaVFrame;
  friend class deoptimizedVFrame;
};

class deltaVFrame: public vframe {
 private:
   oop callee_argument_at(int index) const; // see vframe

 public:
  // constructor
  deltaVFrame(const frame* fr) : vframe(fr) {}

  bool is_delta_frame() const	{ return true; }

  // returns the receiver (block for block invoc.)
  virtual oop receiver() const 			= 0;

  // returns the active method
  virtual methodOop method() const   		= 0;

  // returns the current byte code index
  virtual int bci() const 			= 0;

  virtual oop temp_at(int offset) const         = 0;
  virtual oop expression_at(int index) const    = 0;
  virtual oop context_temp_at(int offset) const = 0;

  // Returns the interpreter contextOop for this vframe.
  // If the frame is optimized a converted context will be returned.
  // Returns NULL is canonical form has no context.
  // (Only used during deoptimization)
  virtual contextOop canonical_context() const = 0;

  // returns the expression stack
  virtual GrowableArray<oop>* expression_stack() const = 0;

  // returns the lexical scope of an activation
  // - method activations yield NULL.
  // - pure block activations yield NULL.
  // - non lifo block activations yield NULL.
  // - other block activations yield the parent activation
  //   if the parent frame resides on the same stack, NULL otherwise!
  virtual deltaVFrame* parent() const 		= 0;
  
  // returns the nearest delta vframe in the sender chain
  deltaVFrame* sender_delta_frame() const; 

  // returns the arguments
  GrowableArray<oop>* arguments() const;

  // arguments are numbered from 1 to n
  oop argument_at(int index) const;

  // printing operations
  void print();
  void print_activation(int index) const;

  // verify operations
  void verify() const;
  virtual void verify_debug_info() const {};
};

// Layout of an interpreter frame:
// sp-> [expressions ] *   ^
//      [temps       ] *   |
//      [hp          ]    -2
//      [receiver    ]    -1
// fp-> [link        ]     0
//      [return pc   ]    +1
//      [arguments   ]    +2

class interpretedVFrame: public deltaVFrame {
 public: 
  // Constructor
  interpretedVFrame(frame* fr) : deltaVFrame(fr) {};

  // Sets the receiver object
  void set_receiver(oop obj);

  // Accessors for HP
  u_char* hp() const;
  void  set_hp(u_char* p);

  // Sets temporaries
  void temp_at_put(int offset, oop obj);

  // Sets element on expression stack
  void expression_at_put(int index, oop obj);

  // Returns the contextOop for this interpreter frame
  // NULL is returned is no context exists.
  contextOop interpreter_context() const;

 private:
  static const int temp_offset;
  static const int hp_offset;
  static const int receiver_offset;
  static const int argument_offset;
  oop* expression_addr(int offset) const;
  bool has_interpreter_context() const;

 public:
  // Virtuals from vframe
  bool is_interpreted_frame() const	{ return true; }
  bool equal(const vframe* f) const;

 public:
  // Virtuals from deltaVFrame
  oop receiver() const;
  methodOop method() const;
  int bci() const;
  oop temp_at(int offset) const;
  oop expression_at(int index) const;
  oop context_temp_at(int offset) const;
  contextOop canonical_context() const;
  GrowableArray<oop>* expression_stack() const;
  deltaVFrame* parent() const;
  void verify() const;
};

#ifdef DELTA_COMPILER

class compiledVFrame: public deltaVFrame {
 public:
  // Constructors
  static compiledVFrame* new_vframe(const frame* fr, ScopeDesc* sd, int bci);
  compiledVFrame(const frame* fr, ScopeDesc* sd, int bci);

  // Returns the active nmethod
  nmethod*  code() const;

  // Returns the scopeDesc
  ScopeDesc* scope() const { return sd; }

  // Returns the contextOop for this interpreter frame
  // NULL is returned is no context exists.
  contextOop compiledVFrame::compiled_context() const;

  // Rewind the bci one step
  void rewind_bci(); 

  // Returns the scope for the parent.
  virtual ScopeDesc* parent_scope() const = 0;

 protected:
  ScopeDesc* sd;
  int        _bci;

  static contextOop compute_canonical_context(ScopeDesc* sd, const compiledVFrame* vf, contextOop con = NULL);
  static oop        resolve_name             (NameDesc* nd,  const compiledVFrame* vf, contextOop con = NULL);
  static oop        resolve_location         (Location loc,  const compiledVFrame* vf, contextOop con = NULL);

  // The filler_obj is used during deoptimization for values that couldn't be retrieved.
  // - stack temps if the frame is absent
  // - context variables if the frame or optimized context is absent.
  // In the ideal situation this should never be used but is works great as a defensive mechanism.
  static oop filler_oop();

  // Returns the bci for a scope desc. 
  // d must belong to the same nmethod and be in the sender chain.
  int  bci_for(ScopeDesc* d) const;

  friend struct MemoizedBlockNameDesc;
  friend class  blockClosureOopDesc;
 public:
  // Virtuals defined in vframe
  bool is_compiled_frame() const { return true; }
  vframe* sender() const;
  bool equal(const vframe* f) const;

 public:
  // Virtuals defined in deltaVFrame
  methodOop method() const;
  int  bci() const;
  oop temp_at(int offset) const;
  oop expression_at(int index) const;
  oop context_temp_at(int offset) const;
  GrowableArray<oop>* expression_stack() const;
  void verify() const;
  void verify_debug_info() const;
};

class compiledMethodVFrame : public compiledVFrame {
 public:
  // Constructor
  compiledMethodVFrame(const frame* fr, ScopeDesc* sd, int bci);

 public:
  // Virtuals defined in vframe
  bool is_top() const;

 public:
  // Virtuals defined in deltaVFrame
  deltaVFrame* parent() const { return NULL; }
  contextOop canonical_context() const;
  oop  receiver() const;
  // Virtuals defined in compiledVFrame
  ScopeDesc* parent_scope() const { return NULL; }
};

class compiledBlockVFrame : public compiledVFrame {
 public:
  // Constructor
  compiledBlockVFrame(const frame* fr, ScopeDesc* sd, int bci);

 public:
  // Virtuals defined in vframe
  bool is_top() const;

 public:
  // Virtuals defined in deltaVFrame
  deltaVFrame* parent() const;
  contextOop canonical_context() const;
  oop  receiver() const;
  // Virtuals defined in compiledVFrame
  ScopeDesc* parent_scope() const;
};

class compiledTopLevelBlockVFrame : public compiledVFrame {
 public:
  // Constructor
  compiledTopLevelBlockVFrame(const frame* fr, ScopeDesc* sd, int bci);

 public:
  // Virtuals defined in vframe
  bool is_top() const { return true; }

 public:
  // Virtuals defined in deltaVFrame
  oop  receiver() const;
  contextOop canonical_context() const;
  deltaVFrame* parent() const;
  // Virtuals defined in compiledVFrame
  ScopeDesc* parent_scope() const;
};

// A deoptimizedVFrame is represented by a frame and an offset
// into the packed array of frames.

class deoptimizedVFrame: public deltaVFrame {
 public:
  // Constructor
  deoptimizedVFrame(const frame* fr);
  deoptimizedVFrame(const frame* fr, int offset);

  // Returns the contextOop for this unoptimized frame
  // NULL is returned is no context exists.
  contextOop deoptimized_context() const;

 private:
  int offset;
  objArrayOop frame_array;

  // Reitrieves the frame array from the frame
  objArrayOop retrieve_frame_array() const;

  // Returns the oop at offset+index
  oop  obj_at(int index) const;
  int  end_of_expressions() const;

  enum {
    receiver_offset     = 0,
    method_offset       = 1,
    bci_offset          = 2,
    locals_size_offset  = 3,
    first_temp_offset   = 4,
  };

  friend class StackChunkBuilder;

 public:
  // Virtuals defined in vframe
  bool equal(const vframe* f) const;
  bool is_deoptimized_frame() const { return true; }
  vframe* sender() const;
  bool is_top() const;

 public:
  // Virtuals defined in deltaVFrame
  oop receiver() const;
  methodOop method() const;
  int bci() const;
  deltaVFrame* parent() const { return NULL; }
  oop temp_at(int offset) const;
  oop expression_at(int index) const;
  oop context_temp_at(int offset) const;
  GrowableArray<oop>* expression_stack() const;
  contextOop canonical_context() const;
};

#endif

class cVFrame: public vframe {
 public:
  // Constructor
  cVFrame(const frame* fr) : vframe(fr) {}

 public:
  // Virtuals defined in vframe
  bool is_c_frame() const { return true; }
  void print_value() const;
  void print();
};

class cChunk: public cVFrame {
 public:
  // Constructor
  cChunk(const frame* fr) : cVFrame(fr) {}

 public:
  // Virtuals defined in vframe
  bool is_c_chunk() const { return true; }
  vframe* sender() const;
  void print_value() const;
  void print();

 private:
  // Virtual defined in vframe
  oop callee_argument_at(int index) const;
};
