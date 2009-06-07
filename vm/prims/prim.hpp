/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.52 $ */
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

// The file defines the interface to the internal primitives.

// Primitives are divided into four categories:
//                                   (canScavenge, NLR,   canBeConstantFolded)
// 1) Allocation primitives.         (true,        false, false)
// 2) Pure primitives.               (false,       false, true)
// 3) Function primitives            (false,       false, false)
// 4) Primitives who might scavenge, (true,        false, false)
//    gc, or call delta.


// Warning: Don't change the element order of this enum without
//          adjusting the DeltaPrimitiveGenerator accordingly!

enum PrimitiveGroup {
  NormalPrimitive,

  IntComparisonPrimitive,	// Integer comparison prim.
  IntArithmeticPrimitive,	// Integer arithmetic.

  FloatComparisonPrimitive,	// FP comparison prim.
  FloatArithmeticPrimitive,	// FP arithmetic.

  ObjArrayPrimitive,		// access/size
  ByteArrayPrimitive,		// access/size
  DoubleByteArrayPrimitive,	// access/size
  
  BlockPrimitive		// block-related prims (creation, invocation, contexts)
};

class InterpretedPrim_Cache: public ValueObj {
 private:
  u_char* hp() const { return (u_char*)this; }
 public:
  symbolOop       name() const;
  int             number_of_parameters() const;
  primitive_desc* pdesc() const;
  bool            has_receiver() const;
  bool            has_failure_code() const;
};

// The primitive_desc structure exposes all properties of a primitive.
// Primitives are like procedures (no dispatching is necessary) and
// invoked by providing a number of parameters. 

class primitive_desc {
 public:
  char*  _name;		// name of the primitive
  fntype _fn;		// primitive entry point
  int    _flags;	// see unpacking below
  char** _types;	// provides the parameter and return types
			// [0]                       -> contains the return type
		  	// [1..number_of_parameters] -> contains types for the parameters
  char** _errors;       // provides a null terminated list of errors for the primitive
                        // excluding {FirstArgumenthasWrongType, SecondArgumenthasWrongType ...}
 public:
  // returns the name of the primitive.
  char* name() const { return _name; }

  // returns the primitive entry point.
  fntype fn() const { return _fn; }
  
  // flags
  int number_of_parameters()	const { return get_unsigned_bitfield(_flags, 0, 8); }
	  // # of parameters (self or arguments), excluding failure block (if any); i.e., # of args
	  // that are actually passed to C
  PrimitiveGroup group()	const { return (PrimitiveGroup) get_unsigned_bitfield(_flags, 8, 8); }
  bool is_special_prim()	const { return group() != NormalPrimitive; }
  bool can_scavenge()		const { return isSet(_flags, 16);  }   // can it trigger a scavenge/GC?
  bool can_perform_NLR()	const { return isSet(_flags, 17);  }   // can it do an NLR or process abort?
  bool can_fail()		const { return isSet(_flags, 18);  }
  	// can_fail: can primitive fail with arguments of correct type?  (NB: even if !can_fail(), 
	// primitive will fail if argument types are wrong)
  bool can_invoke_delta()	const { return can_perform_NLR();  }   // can it call other Delta code?
  bool can_be_constant_folded()	const { return isSet(_flags, 19);  }   // is it side-effect free? (so it can be const-folded if args are const)
  bool has_receiver()		const { return isSet(_flags, 20);  }   // does it require a receiver? ({{self prim...}})
  bool is_internal()		const { return isSet(_flags, 21);  }   // true for VM-internal primitives
  bool needs_delta_fp_code()    const { return !isSet(_flags, 22); }   // must caller set up lastDeltaFP?
  bool can_walk_stack()		const;				       // can it trigger a stack print? (debug/interrupt point)

  // Returns the name of the primitive as a symbol
  symbolOop selector() const;

  // Evaluates the primitive with the given parameters
  oop eval(oop* parameters);
 
  //protected:		// NB: can't use protected:, or else Microsoft Compiler (2.1) refuses to initialize prim array
  // Type information of the primitive
  char* parameter_type(int index)	const; // 0 <= index < number_of_parameters()
  char* return_type()			const;

#ifdef DELTA_COMPILER
  // Type information for compiler
  // - returns NULL if type is unknown or too complicated
 public:
  Expr* parameter_klass(int index, PReg* p, Node* n) const; // 0 <= index < number_of_parameters()
  Expr* return_klass(PReg* p, Node* n) const; 
 //protected:
  Expr* convertToKlass(char* type, PReg* p, Node* n) const; 
#endif

 public:
  // Error information
  char* error(int index) const { return _errors[index]; }

  // Comparison operation
  int compare(char* str, int len);

  // Miscellaneous operations
  void print();
  void verify();
  void error(char* msg);
};


class primitives : AllStatic {
 public:
  static void print_table();

  static primitive_desc* lookup(symbolOop selector)	{ return lookup((char*) selector->bytes(), selector->length()); }
  static primitive_desc* lookup(fntype fn);

  static void lookup_and_patch(); // routine called by interpreter

  // For degbugging/profiling
  static void clear_counters();
  static void print_counters();

  static void initialize();
  static void patch(char* name, char* entry_point);

 private:
  static primitive_desc* lookup(char* selector, int len);
  static primitive_desc* lookup(char* selector)	{ return lookup(selector, strlen(selector)); }
  static primitive_desc* verified_lookup(char* selector); // Fails if the primitive could not be found

  // Primitives used by the compiler are looked up at startup
  static primitive_desc* _new0;
  static primitive_desc* _new1;
  static primitive_desc* _new2;
  static primitive_desc* _new3;
  static primitive_desc* _new4;
  static primitive_desc* _new5;
  static primitive_desc* _new6;
  static primitive_desc* _new7;
  static primitive_desc* _new8;
  static primitive_desc* _new9;

  static primitive_desc* _equal;
  static primitive_desc* _not_equal;

  static primitive_desc* _block_allocate;
  static primitive_desc* _block_allocate0;
  static primitive_desc* _block_allocate1;
  static primitive_desc* _block_allocate2;

  static primitive_desc* _context_allocate;
  static primitive_desc* _context_allocate0;
  static primitive_desc* _context_allocate1;
  static primitive_desc* _context_allocate2;

 public:
  static primitive_desc* new0() { return _new0; }
  static primitive_desc* new1() { return _new1; }
  static primitive_desc* new2() { return _new2; }
  static primitive_desc* new3() { return _new3; }
  static primitive_desc* new4() { return _new4; }
  static primitive_desc* new5() { return _new5; }
  static primitive_desc* new6() { return _new6; }
  static primitive_desc* new7() { return _new7; }
  static primitive_desc* new8() { return _new8; }
  static primitive_desc* new9() { return _new9; }

  static primitive_desc* equal()     { return _equal;     }
  static primitive_desc* not_equal() { return _not_equal; }

  static primitive_desc* block_allocate()    { return _block_allocate;  }
  static primitive_desc* block_allocate0()   { return _block_allocate0; }
  static primitive_desc* block_allocate1()   { return _block_allocate1; }
  static primitive_desc* block_allocate2()   { return _block_allocate2; }

  static primitive_desc* context_allocate()  { return _context_allocate;  }
  static primitive_desc* context_allocate0() { return _context_allocate0; }
  static primitive_desc* context_allocate1() { return _context_allocate1; }
  static primitive_desc* context_allocate2() { return _context_allocate2; }
};
