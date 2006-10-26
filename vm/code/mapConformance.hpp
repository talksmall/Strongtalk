/* Copyright 1996 LongView Technologies L.L.C. $Revision: 1.1 $ */
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

// Compiler/New Backend/Value Conformance
//
// Used by the new backed to make mappings conform before merging code pieces. 
//
// Usage:
//   MapConformance mp(1);
//   mp.append_mapping(...);
//   mp.generate();
//

// Variable is an abstraction describing a register or stack location
// Format as requested by Robert 9/6/96
// 30 bits signed offset
// 2  bits describes type

#ifdef DELTA_COMPILER

class Variable : ValueObj {
 private:
  int _value;

  enum {
   special_type = 0,
   reg_type     = 1,
   stack_type   = 2
  };

  int type()   const { return _value & 0x3; }
  int offset() const { return _value >> 2;  }
  int value()  const { return _value; }

  static Variable new_variable(int type, int offset) {
    Variable result;
    result._value = (offset << 2) | type;
    return result;
  }

 public:
  Variable() { _value = 0; }

  // Generators
  static Variable new_register(int offset) { return new_variable(reg_type,     offset); }
  static Variable new_stack(int offset)    { return new_variable(stack_type,   offset); }
  static Variable unused()                 { return new_variable(special_type, 0);      }
  static Variable top_of_stack()           { return new_variable(special_type, 1);      }

  // Testing
  bool in_register() const { return type() == reg_type;    }
  bool on_stack()    const { return type() == stack_type;  }

  bool is_unused()       const { return type() == special_type && offset() == 0; }
  bool is_top_of_stack() const { return type() == special_type && offset() == 1; }

  // Accessors
  int register_number() const { return offset(); }
  int stack_offset()    const { return offset(); }
   
  void set_unused() { _value = 0; }

  // Prints the variable.
  void print();

  // Comparison
  friend bool operator == (Variable x, Variable y) { return x.value() == y.value(); }
  friend bool operator != (Variable x, Variable y) { return x.value() != y.value(); }
};


class MappingTask;

class MapConformance : public ResourceObj {
 private:
  Variable                     _free_register;
  GrowableArray<MappingTask*>* mappings;
  Variable*                    used_variables;
  int                          number_of_used_variables;

  bool reduce_noop_task(MappingTask* task);
  void simplify();
  void process_tasks();

  Variable pop_temporary();
  void push_temporary(Variable var);

  void push(Variable src, int n);

  friend class MappingTask;
 public:
  // Constructor
  MapConformance();

  // Appends mapping
  void append_mapping(Variable src_register, Variable src_stack, Variable dst_register, Variable dst_stack);

  // Generates the move operations. free_register is a register that can be used during code generation.
  void generate(Variable free_register1, Variable free_register2);

  // Callback for the generated move operations. Default behavior will print a line per move.
  virtual void move(Variable src, Variable dst);
  virtual void push(Variable src);
  virtual void pop(Variable dst);

  // Print the status of the conformance
  void print();
};

#endif
