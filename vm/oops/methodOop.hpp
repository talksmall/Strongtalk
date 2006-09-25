/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.89 $ */
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

// A methodOop is a method with byte codes.

const int method_size_mask_bitno  =  2;
const int method_size_mask_size   = 18;

const int method_args_mask_bitno  =  method_size_mask_bitno + method_size_mask_size;
const int method_args_mask_size   =  4;

const int method_flags_mask_bitno =  method_args_mask_bitno + method_args_mask_size;
const int method_flags_mask_size  =  8;


class methodOopDesc : public memOopDesc {
 private:
  objArrayOop _debugInfo;
  oop         _selector_or_method;	// selector for normal methods, enclosing method for blocks
  int         _counters;		// invocation counter and sharing counter
  smiOop      _size_and_flags;
  // [flags (8 bits),  nofArgs (4 bits), size in oops (18 bits), tag (2 bits)]
  methodOopDesc* addr() const		{ return (methodOopDesc*)memOopDesc::addr(); }

  // returns the header size of a methodOop
  static int header_size()		{ return sizeof(methodOopDesc)/oopSize; }

 public:
  // offsets for code generation
  static int selector_or_method_byte_offset()	{ return int(&(((methodOopDesc*)NULL)->_selector_or_method)) - Mem_Tag; }
  static int counters_byte_offset()		{ return int(&(((methodOopDesc*)NULL)->_counters)) - Mem_Tag; }
  static int codes_byte_offset()		{ return sizeof(methodOopDesc) - Mem_Tag; }

  smiOop size_and_flags() const { return addr()->_size_and_flags; }	
  void set_size_and_flags(int size, int nofArgs, int flags) {
    addr()->_size_and_flags = (smiOop)
      ((flags << method_flags_mask_bitno) + (nofArgs << method_args_mask_bitno) + (size << method_size_mask_bitno));
  }

  int flags() const { 
    return get_unsigned_bitfield((int)size_and_flags(), method_flags_mask_bitno, method_flags_mask_size);
  }

  void set_flags(int flags) {
    set_size_and_flags(size_of_codes(), nofArgs(), flags);
  }

  int nofArgs() const {			 // number of arguments (excluding receiver)
    return get_unsigned_bitfield((int)size_and_flags(), method_args_mask_bitno, method_args_mask_size); }

 public:
  friend methodOop as_methodOop(void* p) { return methodOop(as_memOop(p)); }

  void bootstrap_object(bootstrap* st);

  // Tester
  bool is_blockMethod() const		{ return !selector_or_method()->is_symbol(); }

  objArrayOop debugInfo() const		{ return addr()->_debugInfo; }
  void set_debugInfo(objArrayOop d)	{ addr()->_debugInfo = d; }

  symbolOop selector() const;
  methodOop parent() const;		// returns the enclosing block or method (for blocks), or NULL
  methodOop home() const;		// returns the enclosing method (for blocks), or itself

  oop  selector_or_method() const	{ return addr()->_selector_or_method; }
  void set_selector_or_method(oop value){ addr()->_selector_or_method = value; }

  // returns the enclosing method's selector (block methods only)
  symbolOop enclosing_method_selector() const;

  // returns the source if the debugInfo is present, otherwise NULL.
  byteArrayOop source();

  // returns the tempInfo if the debugInfo is present, otherwise NULL.
  objArrayOop tempInfo();

  // Generates an array with fileout information.
  // The array contains pair of information:
  // (true,  SmallInteger) means character
  // (false, oop)          means oop
  objArrayOop fileout_body();

  // Accessor operaions on counters.
  //   invocation counter - tells how many times this methodOop has been executed (may decay over time)
  //   sharing counter    - tells how many callers this methodOop has.

  enum {
    _short_size              = 16,
    _invocation_count_offset = _short_size,
    _invocation_count_width  = _short_size,
    _invocation_count_max    = (1 << _short_size) - 1,

    _sharing_count_offset    = 0,
    _sharing_count_width     = _short_size,
    _sharing_count_max       = (1 << _short_size) - 1
  };

  int  counters() const			{ return addr()->_counters; }
  void set_counters(int inv, int share)	{ addr()->_counters = (inv << _invocation_count_offset) | share; }

  // Invocation counter
  int invocation_count() const		{ return get_unsigned_bitfield(counters(), _invocation_count_offset, _invocation_count_width); }
  void set_invocation_count(int value)	{ set_counters(value, sharing_count()); }

  void decay_invocation_count(double decay_factor);
  
  // Sharing counter (number of callers)
  int sharing_count() const		{ return get_unsigned_bitfield(counters(), _sharing_count_offset, _sharing_count_width); }
  void set_sharing_count(int value)	{ set_counters(invocation_count(), value); }
  void inc_sharing_count();
  void dec_sharing_count();

  bool was_never_executed();		// was method never executed? (count = 0, empty inline caches)

  int size_of_codes() const {		// size of byte codes in words
    return get_unsigned_bitfield((int) size_and_flags(), method_size_mask_bitno, method_size_mask_size); 
  }

  void set_size_of_code(int size) {
    set_size_and_flags(size, nofArgs(), flags());
  }

  // Returns a pointer to the hybrid code at 'offset'
  u_char* codes(int offset = 1) const {
     return (u_char*) addr() + sizeof(methodOopDesc) + offset - 1;
  }

  u_char* codes_end() const {
     return codes() + size_of_codes() * oopSize;
  }

  // find methodOop given an hcode pointer 
  static methodOop methodOop_from_hcode(u_char* hp);

  u_char byte_at(int offset) const		{ return *codes(offset); }
  void byte_at_put(int offset,u_char c)		{ *codes(offset) = c; }

  long word_at(int offset) const		{ return * (long*) codes(offset); }
  void word_at_put(int offset,unsigned long w)	{ * (long*) codes(offset) = w; }

  oop  oop_at(int offset) const			{ return * (oop*) codes(offset); }
  void oop_at_put(int offset,oop obj)		{ * (oop*) codes(offset) = obj; }

  // Returns the next byte code index based on hp.
  int  next_bci_from(u_char* hp) const;

  // Returns the current byte code index based on hp (points to the next byte code)
  int  bci_from(u_char* hp) const;

  int  number_of_arguments() const;

  // Returns the number of temporaries allocated by the interpreter
  // (excluding receiver & float temporaries, which may come afterwards).
  int number_of_stack_temporaries() const;

  // Method with hardwired floating-point operations
  bool has_float_temporaries() const		{ return *codes(1) == Bytecodes::float_allocate; }
  int  number_of_float_temporaries() const	{ return has_float_temporaries() ? *codes(3) : 0; }
  int  float_expression_stack_size() const	{ return has_float_temporaries() ? *codes(4) : 0; }
  int  total_number_of_floats() const		{ return number_of_float_temporaries() + float_expression_stack_size(); }

  // Stack frame layout if there's a float section (offset & size in oops relative to ebp)
  int  float_offset(int float_no) const;
  int  float_section_start_offset() const	{ return frame_temp_offset - number_of_stack_temporaries(); }
  int  float_section_size() const		{ return total_number_of_floats()*floatSize/oopSize; }

  // Testers
  bool is_accessMethod() const			{ return *codes() == Bytecodes::return_instVar; }
  bool is_primitiveMethod() const;

  // For predicted sends (smi +, -, *, etc.)
  bool is_special_primitiveMethod() const	{ return *codes(1) == Bytecodes::special_primitive_send_1_hint; }
  Bytecodes::Code special_primitive_code() const;


  // Information needed by the optimizing compiler
  //
  // Incoming_Info describes the possible oop kinds that a methodOop gets 'from' the
  // outside (via the closure) when invoked. Currently, the incoming oop is stored in
  // the recv and temp0 stack location for blocks, and in the recv location only for
  // methods (ordinary send). This should change at some point and the incoming oop
  // should be stored in the recv location only (allows better stack usage).
  //
  // Temporaries_Info is used to compute the number of stack-allocated temporaries. If
  // more than two temporaries are needed, the very first bytecode is an allocate temp
  // byte code (bytecode invariant).

  enum Flags {
    // general flags
    containsNLRFlag	= 0,
    allocatesContextFlag= 1,
    mustBeCustomizedFlag= 2,
    isCustomizedFlag	= 3,

    // method specific flags (overlapping with block specific flags)
    methodInfoFlags	= isCustomizedFlag + 1,
    methodInfoSize	= 2,

    // block specific flags (overlapping with method specific flags)
    blockInfoFlags	= methodInfoFlags,
    blockInfoSize	= methodInfoSize
  };

  // Flags for inlining
  enum Method_Inlining_Info {
    never_inline  = 0,
    normal_inline = 1,
    always_inline = 2,
  } method_inlining_info() const;

  void set_method_inlining_info(Method_Inlining_Info info);

  enum Block_Info {
    expects_nil		= 0,			// 'clean' block
    expects_self	= 1, 			// 'copying' block
    expects_parameter	= 2,			// 'copying' block
    expects_context	= 3			// 'full' block
  } block_info() const;

  // Tells if an activation of this method has a context stored as temp 0.
  bool activation_has_context() const { 
    return allocatesInterpretedContext()
        || (is_blockMethod() && expectsContext());
  }

  // Tells if bci is a context allocation
  bool in_context_allocation(int bci) const;

  bool containsNLR() const			{ return isSet(flags(), containsNLRFlag); }
  bool allocatesInterpretedContext() const	{ return isSet(flags(), allocatesContextFlag); }
  bool mustBeCustomizedToClass() const		{ return isSet(flags(), mustBeCustomizedFlag); }
  bool expectsContext()	const			{ return block_info() == expects_context; }
  bool hasNestedBlocks() const;
  bool is_clean_block() const			{ return block_info() == expects_nil; }
  bool is_copying_block() const			{ return block_info() == expects_self || block_info() == expects_parameter; }
  bool is_full_block() const			{ return block_info() == expects_context; }

  // Method customization
  bool has_instVar_access() const		{ return true; } // for now - conservative - FIX THIS
  bool has_classVar_access() const		{ return true; } // for now - conservative - FIX THIS
  bool has_inlineCache() const			{ return true; } // for now - conservative - FIX THIS
  bool is_customized() const		        { return isSet(flags(), isCustomizedFlag); }			
  bool should_be_customized() const             { return has_instVar_access() || has_classVar_access() || has_inlineCache(); }

  // Returns a deep copy of the methodOop
  methodOop copy_for_customization() const;

  // Customize method to klass
  void customize_for(klassOop klass, mixinOop mixin);
  void uncustomize_for(mixinOop mixin);

  // Uplevel accesses via contexts
  int lexicalDistance(int contextNo);	// for uplevel accesses; see comment in .c file
  int contextNo(int lexicalDistance);	// inverse of lexicalDistance()

  // Computes the number of interpreter contexts from here up to the home method
  int context_chain_length() const;

  // Clears all the inline caches in the method.
  void clear_inline_caches();

  // Cleanup all inline caches
  void cleanup_inline_caches();

  // Computes the estimated cost of a method by summing 
  // cost of all byte codes (see code_cost in methodOop.cpp).
  int estimated_inline_cost(klassOop receiverKlass);

  // Finds the bci based on the next bci
  // Returns -1 if the search failed.
  int find_bci_from(int nbci) const;

  // Returns the next bci
  int next_bci(int bci) const;

  // Returns the end bci (excluding the padding)
  int end_bci() const;

  // Returns the inline cache associated with the send at bci.
  InterpretedIC* ic_at(int bci) const;

  // Returns an array of byte code indecies contributing to the expression stack
  GrowableArray<int>* expression_stack_mapping(int bci);

  // For debugging only
  void print_codes();
  void pretty_print();

  // Printing support
  void print_value_for(klassOop receiver_klass, outputStream* st = NULL);

  // Inlining database support
  void print_inlining_database_on(outputStream* st);
  int  bci_for_block_method(methodOop inner);
  methodOop block_method_at(int bci);

  // Returns the numbers of temporaries allocated in a context.
  // self_in_context tells if self is copied into context.
  // Note: This function is extremely slow, so please use it for
  //       verify and debug code only.
  int number_of_context_temporaries(bool* self_in_context = NULL);

  // Checks if the context matches this method
  void verify_context(contextOop con);

  // Query primitives
  objArrayOop referenced_instance_variable_names(mixinOop mixin) const;
  objArrayOop referenced_class_variable_names() const;
  objArrayOop referenced_global_names() const;
  objArrayOop senders() const;

  friend methodKlass;
};
