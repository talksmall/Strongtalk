/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.34 $ */
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

// ScopeDescRecorder provides the interface to generate scopeDescs for
// optimized methods (nmethods).
// To retrieve the generated information, use nmethodScopes

class Array;
class ByteArray;
class NonInlinedBlockScopeNode;

// Interface to generate scope information for an nmethod
class ScopeDescRecorder: public ResourceObj {
 private:
  bool          _hasCodeBeenGenerated;
  Array*        oops;
  Array*        values;
  ByteArray*    codes;
  PcDescInfo    pcs;

  GrowableArray<klassOop>* dependants;
  int                  dependants_end;

  NonInlinedBlockScopeNode* nonInlinedBlockScopesHead;
  NonInlinedBlockScopeNode* nonInlinedBlockScopesTail;

 public:
  ScopeInfo root;
  
  // Returns the offset for a scopeDesc after generation of the scopeDesc info.
  int offset(ScopeInfo scope);
  int offset_for_noninlined_scope_node(NonInlinedBlockScopeNode* scope);

  ScopeDescRecorder(int scopeSize, // estimated size of scopes (in bytes)
		    int npcDesc);  // estimated number of PcDescs

  // Adds a method scope
  ScopeInfo addMethodScope(LookupKey*      key,                        // lookup key
			   methodOop       method,                     // result of the lookup
			   LogicalAddress* receiver_location,          // location of receiver
			   bool            allocates_compiled_context, // tells whether the code allocates a context
                           bool            lite         = false,
			   int             scopeID      = 0,
			   ScopeInfo       senderScope  = NULL, 
			   int             senderBCI    = IllegalBCI,
			   bool            visible      = false);


  // Adds an inlined block scope
  ScopeInfo addBlockScope(methodOop       method,                     // block method
			  ScopeInfo       parent,                     // parent scope
			  bool            allocates_compiled_context, // tells whether the code allocates a context
                          bool            lite             = false,
			  int             scopeID          = 0,
			  ScopeInfo       senderScope      = NULL, 
			  int             senderBCI        = IllegalBCI,
			  bool            visible          = false);

  // Adds a top level block scope
  ScopeInfo addTopLevelBlockScope(methodOop       method,                     // block method
				  LogicalAddress* receiver_location,          // location of receiver
				  klassOop        receiver_klass,             // receiver klass
				  bool            allocates_compiled_context);// tells whether the code allocates a context

  // Adds an noninlined block scope
  // Used for retrieving information about block closure stubs
  NonInlinedBlockScopeNode* addNonInlinedBlockScope(methodOop block_method, ScopeInfo parent);

  // Interface for adding name nodes.
  void addTemporary        (ScopeInfo scope, int index, LogicalAddress* location); // all entries [0..max(index)] must be filled.
  void addContextTemporary (ScopeInfo scope, int index, LogicalAddress* location); // all entries [0..max(index)] must be filled.
  void addExprStack        (ScopeInfo scope, int bci,   LogicalAddress* location); // sparse array = some entries may be left empty.

  LogicalAddress* createLogicalAddress(NameNode* initial_value);
  void            changeLogicalAddress(LogicalAddress* location, NameNode* new_value, int pc_offset);

  // Providing the locations of the arguments is superfluous but is convenient for
  // verification. 
  //  - for top level scopes the argument locations are fixed (on the stack provided by the caller).
  //  - for inlined scopes the expression stack of the caller describes the argument locations.
  // %implementation-note:
  // For now the argument locations are saved since the function to compute expression stack has
  // not been implemented.
  void addArgument(ScopeInfo scope, int index, LogicalAddress* location);

  // Interface for creating the pc-offset <-> (ScopeDesc, bci) mapping.
  void addPcDesc(int pcOffset, ScopeInfo scope, int bci);
  void addIllegalPcDesc(int pcOffset);

  // Dependencies
  void add_dependant(LookupKey* key);

  // Returns the size of the generated scopeDescs.
  int size();

  // Copy the generated scopeDescs to 'addr'
  void copyTo(nmethod* nm);

  void verify(nmethodScopes* scopes);

  // Generates the the scopeDesc information.
  void generate();

 private:
  // Called before storring a location to resolve the context scope index into an offset
  Location convert_location(Location loc);

  ScopeInfo addScope(ScopeInfo scope, ScopeInfo senderScope);
  NonInlinedBlockScopeNode* addNonInlinedBlockScope(NonInlinedBlockScopeNode* scope);

  void genScopeDescHeader(u_char code, 
                          bool   lite,
                          bool   args,
                          bool   temps,
                          bool   context_temps,
                          bool   expr_stack,
                          bool   has_context,
                          bool   bigHeader);

  // Generate the collected dependecies 
  void generateDependencies();

  void emit_termination_node();
  void emit_illegal_node(bool is_last);

  // Returns true if was possible to save exprOffset and nextOffset in the
  // two pre-allocated bytes.
  int  updateScopeDescHeader(int offset, int next);

  void  updateExtScopeDescHeader(int offset, int next);

  inline int getValueIndex(int v);
  inline int getOopIndex(oop o);
  
  inline void genIndex(int index);
  void genValue(int v);
  void genOop(oop o);

  // Make the private stuff reachable from the internal nodes
  friend class NameNode;
  friend class LocationName;
  friend class ValueName;
  friend class MemoizedName;
  friend class BlockValueName;
  friend class IllegalName;

  friend class ScopeDescNode;
  friend class MethodScopeNode;
  friend class BlockScopeNode;
  friend class TopLevelBlockScopeNode;
  friend class NonInlinedBlockScopeNode;

  friend class LogicalAddress;
  friend class NameList;
};

// NameNode
//  - LocationName
//  - ValueName
//  - MemoizedName
//  - BlockValueName
//  - IllegalName
// this hierarchy is parallel to the NameDesc hierarchy in nameDesc.hpp

class NameNode: public ResourceObj {		// abstract superclass of all NameNodes
 public:
  bool genHeaderByte(ScopeDescRecorder* rec, u_char code, bool is_last, int index);

  virtual bool hasLocation() 	{ return false; }
  virtual bool isIllegal()  	{ return false; }
  virtual Location location()	{ ShouldNotCallThis(); return unAllocated; }
  virtual void generate(ScopeDescRecorder* rec, bool is_last) = 0;
};

// a LocationName describes a location; i.e., the corresponding source name (e.g., method temporary)
// lives in this location for its entire lifetime
class LocationName: public NameNode {
 private:
  Location l;

  void generate(ScopeDescRecorder* rec, bool is_last);

 public:
  LocationName(Location ln) { l = ln; }
  bool hasLocation() 	{ return true; }
  Location location()	{ return l; }
};

// a ValueName is a constant; i.e., the corresponding source name is a compile-time constant
// (maybe because it is a source constant, or because its computation has been constant-folded)
class ValueName: public NameNode {
 private:
  oop value;
  
  void generate(ScopeDescRecorder* rec, bool is_last);

 public:
  ValueName(oop val) {
    value = val;
    assert(!val->is_block(), "should use BlockValueName");
  }
};

// a BlockValueName describes a block closure that has been completely optimized away; i.e., no
// closure will ever be created at runtime during normal execution of the program
class BlockValueName: public NameNode {
 private:
  methodOop block_method;   // The block method
  ScopeInfo parent_scope; // The scope where to find the context

  void generate(ScopeDescRecorder* rec, bool is_last);

 public:
  BlockValueName(methodOop block_method, ScopeInfo parent_scope) {
    this->block_method = block_method;
    this->parent_scope = parent_scope;
  }
};

// a MemoizedName describes a block closure that has been partially optimized away; i.e., a
// closure may or may not be created at runtime. The closure's location is initialized to a special
// value, and the actual closure is created on demand after testing if it has been created already.
// If we ever look at this name during debugging and the block doesn't exist yet, we have to create
// one and store it in the location.
class MemoizedName: public NameNode {
 private:
  Location  loc;
  methodOop block_method;
  ScopeInfo parent_scope;

  void generate(ScopeDescRecorder* rec, bool is_last);

 public:
  MemoizedName(Location loc, methodOop block_method, ScopeInfo parent_scope) {
    this->loc          = loc;
    this->block_method = block_method;
    this->parent_scope = parent_scope;
  }
  bool hasLocation() 	{ return true; }
  Location location()	{ return loc; }
};

// newValueName creates a ValueName or BlockValueName (if value is a block)
NameNode* newValueName(oop value);  	

// an IllegalName marks a name that cannot be inspected at runtime because it is never visible
// at any interrupt point (i.e., it is live only between two interrupt points)
// mainly exists for compiler/runtime system debugging
class IllegalName: public NameNode {
 private:
  bool isIllegal()  { return true; }
  void generate(ScopeDescRecorder* rec,  bool is_last);
};

enum {
  LOCATION_CODE,
  VALUE_CODE,
  BLOCKVALUE_CODE,
  MEMOIZEDBLOCK_CODE
};

// helper data structures used during packing and unpacking
class nameDescHeaderByte : public ValueObj {
 private:
  u_char byte; 
  static const u_char code_width;
  static const u_char index_width;
  static const u_char is_last_bit_num;
  static const u_char max_code;

  u_char raw_index() { return lowerBits(byte >> code_width, index_width); }

 public:
  static const u_char max_index;
  static const u_char no_index;
  static const u_char termination_index;
  static const u_char illegal_index;

  u_char value() { return byte; }

  u_char code()  {
    return lowerBits(byte, code_width); 
  }

  u_char index()          {
    assert(has_index(), "must have valid index");
    return raw_index();
  }

  bool is_illegal()     { return raw_index() == illegal_index;     }
  bool is_termination() { return raw_index() == termination_index; }
  bool is_last()        { return isSet(byte, is_last_bit_num);     }
  bool has_index()      { return raw_index() <= max_index;         }
  
  void pack(u_char code, bool is_last, u_char i) {
    assert( code <= max_code,  "code to high");
    assert( i    <= no_index,  "index to high");
    byte = addBits(i << code_width, code);
    if (is_last) byte = setNth(byte, is_last_bit_num);
  }

  void pack_illegal(bool is_last) {
    byte = addBits(illegal_index << code_width, 0);
    if (is_last) byte = setNth(byte, is_last_bit_num);
  }

  void pack_termination(bool is_last) {
    byte = addBits(termination_index << code_width, 0);
    if (is_last) byte = setNth(byte, is_last_bit_num);
  }

  void unpack(u_char value) { byte = value; }
};

class scopeDescHeaderByte : public ValueObj {
 private:
  u_char byte;
  static const u_char code_width;
  static const u_char max_code;
  static const u_char lite_bit_num;
  static const u_char args_bit_num;
  static const u_char temps_bit_num;
  static const u_char context_temps_bit_num;
  static const u_char expr_stack_bit_num;
  static const u_char context_bit_num;
 public:
  u_char value()                { return byte; }
  u_char code()                 { return lowerBits(byte, code_width);        }
  bool   is_lite()              { return isSet(byte, lite_bit_num);          }
  bool   has_args()             { return isSet(byte, args_bit_num);          }
  bool   has_temps()            { return isSet(byte, temps_bit_num);         }
  bool   has_context_temps()    { return isSet(byte, context_temps_bit_num); }
  bool   has_expr_stack()       { return isSet(byte, expr_stack_bit_num);    }
  bool   has_compiled_context() { return isSet(byte, context_bit_num);       }
 
  bool   has_nameDescs()     { return has_args()
                                   || has_temps()
                                   || has_context_temps()
                                   || has_expr_stack(); }

  void pack(u_char code, bool lite, bool args, bool temps, bool context_temps, bool expr_stack, bool has_compiled_context) {
    assert( code <= max_code, "code to high");
    byte = code;
    if (lite)                   byte = setNth(byte, lite_bit_num);
    if (args)                   byte = setNth(byte, args_bit_num);
    if (temps)                  byte = setNth(byte, temps_bit_num);
    if (context_temps)          byte = setNth(byte, context_temps_bit_num);
    if (expr_stack)             byte = setNth(byte, expr_stack_bit_num);
    if (has_compiled_context)   byte = setNth(byte, context_bit_num);
  }

  void unpack(u_char value) { byte = value; }
};

# define BYTE_WIDTH       8 
# define EXTENDED_INDEX   nthMask(BYTE_WIDTH)
# define MAX_INLINE_VALUE nthMask(BYTE_WIDTH-1)

enum {
  METHOD_CODE,
  BLOCK_CODE,
  TOPLEVELBLOCK_CODE,
  NONINLINED_BLOCK_CODE
};

#endif
