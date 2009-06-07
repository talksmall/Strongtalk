/* Copyright 1994 - 1996, LongView Technologies, L.L.C. $Revision: 1.12 $ */
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

// The InterpretedIC describes the contents of an inline cache in the
// byte codes of a method. The inline cache holds a combination of
// selector/method and 0/class oops and is read and modified during
// execution.
//
//
// An InterpretedIC can have four different states:
//
// 1) empty					{selector,         0} 
// 2) filled with a call to interpreted code	{method,           klass}
// 3) filled with an interpreter PIC		{selector,         objArrayObj}
// 4) filled with a call to compiled code	{jump table entry, klass}
//
//
// Layout:
//
//    send byte code			<-----	send_code_addr()
//    ...
//    halt byte codes for alignment
//    ...
// 0: selector/method/jump table entry	<-----	first_word_addr()
// 4: 0/klass/objArray			<-----	second_word_addr()
// 8: ...

class InterpretedIC: ValueObj {
 public:
  enum {
    size               = 8,			// inline cache size in words
    first_word_offset  = 0,			// layout info: first word
    second_word_offset = 4,			// layout info: second word
  } InterpretedICConstants;

  // Conversion (Bytecode* -> InterpretedIC*)
  friend InterpretedIC* as_InterpretedIC(char* address_of_next_instr);

  // find send bytecode, given address of selector; return NULL/IllegalBCI if not in a send
  static u_char* findStartOfSend(u_char* selector_addr);
  static int findStartOfSend(methodOop m, int bci);

 private:
  // field access
  char*   addr_at(int offset) const		{ return (char*)this + offset; }
  oop*    first_word_addr() const		{ return (oop*)addr_at(first_word_offset); }
  oop*    second_word_addr() const		{ return (oop*)addr_at(second_word_offset); }
  void    set(Bytecodes::Code send_code, oop first_word, oop second_word);

 public:
  // Raw inline cache access
  u_char* send_code_addr() const		{ return findStartOfSend((u_char*)this); }
  Bytecodes::Code send_code() const		{ return Bytecodes::Code(*send_code_addr()); }
  oop             first_word() const		{ return *first_word_addr(); }
  oop             second_word() const		{ return *second_word_addr(); }
  
  // Returns the polymorphic inline cache array. Assert fails if no pic is present.
  objArrayOop pic_array();

  // Inline cache information
  bool		  is_empty() const		{ return second_word() == NULL; }
  symbolOop	  selector() const;		// the selector
  jumpTableEntry* jump_table_entry() const;	// only legal to call if compiled send

  int             nof_arguments() const;        // the number of arguments
  Bytecodes::SendType     send_type() const;	// the send type
  Bytecodes::ArgumentSpec argument_spec() const;// the argument spec

  // Manipulation
  void clear();					// clears the inline cache
  void cleanup();                               // cleanup the inline cache
  void clear_without_deallocation_pic();        // clears the inline cache without deallocating the pic
  void replace(nmethod* nm);			// replaces the appropriate target with a nm
  void replace(LookupResult result, klassOop receiver_klass); // replaces the inline cache with a lookup result

  // Debugging
  void print();

  // Cache miss
  static oop* inline_cache_miss();		// the inline cache miss handler

 private:
  // helpers for inline_cache_miss
  static void update_inline_cache(InterpretedIC* ic, frame* f, Bytecodes::Code send_code, klassOop klass, LookupResult result);
  static oop does_not_understand(oop receiver, InterpretedIC* ic, frame* f);
  static void trace_inline_cache_miss(InterpretedIC* ic, klassOop klass, LookupResult result);
};

inline InterpretedIC* as_InterpretedIC(char* address_of_next_instr)
{
	return (InterpretedIC*)(address_of_next_instr - InterpretedIC::size);
}

// Interpreter_PICs handles the allocation and deallocation of interpreter PICs.

static const int size_of_smallest_interpreterPIC = 2;
static const int size_of_largest_interpreterPIC  = 5;
static const int number_of_interpreterPIC_sizes  = size_of_largest_interpreterPIC - size_of_smallest_interpreterPIC + 1;


// An InterpretedIC_Iterator is used to iterate through the entries of
// an inline cache in a methodOop. Whenever possible, one should use this
// abstraction instead of the (raw) InterpretedIC.

class InterpretedIC_Iterator: public IC_Iterator {
 private:
  InterpretedIC* _ic;				// the inline cache
  objArrayOop	_pic;				// the PIC if there is one

  // state machine
  int		_number_of_targets;		// the no. of IC entries
  IC_Shape	_info;				// send site information
  int		_index;				// the current entry no.
  klassOop	_klass;				// the current klass
  methodOop	_method;			// the current method
  nmethod*	_nm;				// current nmethod (NULL if none)

  void set_method(oop m);			// set _method and _nmethod
  void set_klass(oop k);			// don't assign to _klass directly

 public:
  InterpretedIC_Iterator(InterpretedIC* ic);

  // IC information
  int		number_of_targets() const	{ return _number_of_targets; }
  IC_Shape	shape() const			{ return _info; }
  symbolOop	selector() const		{ return _ic->selector(); }
  bool		is_interpreted_ic() const	{ return true; }
  bool		is_super_send() const;
  InterpretedIC* interpreted_ic() const		{ return _ic; }


  // Iterating through entries
  void		init_iteration();
  void		advance();
  bool		at_end() const			{ return _index >= number_of_targets(); }

  // Accessing entries
  klassOop	klass() const			{ return _klass; }

  // answer whether current target method is compiled or interpreted
  bool		is_interpreted() const		{ return _nm == NULL; }
  bool		is_compiled() const		{ return _nm != NULL; }

  methodOop	interpreted_method() const;
  nmethod*	compiled_method() const;

  // Debugging
  void		print();
};
