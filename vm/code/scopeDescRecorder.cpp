/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.53 $ */
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

# include "incls/_precompiled.incl"

#ifdef DELTA_COMPILER

# include "incls/_scopeDescRecorder.cpp.incl"

// Todo list 
// - Insert Logical addresses
//   If there is only on physical address they should be stored a today.

# define INITIAL_ARG_SIZE		 5
# define INITIAL_TEMP_SIZE		 5
# define INITIAL_CONTEXT_TEMP_SIZE	 5
# define INITIAL_EXPR_STACK_SIZE	10

# define INITIAL_OOPS_SIZE                100
# define INITIAL_VALUES_SIZE              100
# define INITIAL_DEPENDANTS_SIZE           20
# define INITIAL_CONTEXT_SCOPE_ARRAY_SIZE  10

const u_char nameDescHeaderByte::code_width        = 2;
const u_char nameDescHeaderByte::index_width       = 5;
const u_char nameDescHeaderByte::is_last_bit_num   = code_width+index_width;
const u_char nameDescHeaderByte::max_code          = nthMask(code_width);

const u_char nameDescHeaderByte::max_index         = nthMask(index_width) - 3;
const u_char nameDescHeaderByte::no_index          = nthMask(index_width) - 2;
const u_char nameDescHeaderByte::termination_index = nthMask(index_width) - 1;
const u_char nameDescHeaderByte::illegal_index     = nthMask(index_width) - 0;

// 2 bits: scopeDesc type
// 1 bit:  is lite scope desc
// 1 bit:  has arguments
// 1 bit:  has temporaries
// 1 bit:  has expression stack
// 1 bit:  has context temporaries
// 1 bit:  has context
const u_char scopeDescHeaderByte::code_width            = 2;
const u_char scopeDescHeaderByte::max_code              = nthMask(code_width);
const u_char scopeDescHeaderByte::lite_bit_num          = code_width;
const u_char scopeDescHeaderByte::args_bit_num          = code_width+1;
const u_char scopeDescHeaderByte::temps_bit_num         = code_width+2;
const u_char scopeDescHeaderByte::context_temps_bit_num = code_width+3;
const u_char scopeDescHeaderByte::expr_stack_bit_num    = code_width+4;
const u_char scopeDescHeaderByte::context_bit_num       = code_width+5;


// A LogicalAddress describes a source level location.
// Since backend optimizations reshuffles code a source level location
// may change its physical location.
class LogicalAddress : public ResourceObj {
 private:
  NameNode*       _physical_address;
  int             _pc_offset;
  LogicalAddress* _next;
  int             _offset;

  NameNode*       physical_address() const { return _physical_address; }
  int             pc_offset()        const { return _pc_offset;        }
  LogicalAddress* next()             const { return _next;             }
 public:
  LogicalAddress(NameNode* physical_address, int pc_offset = 0);

  void append(NameNode* physical_address, int pc_offset);
  NameNode* physical_address_at(int pc_offset);
  void generate(ScopeDescRecorder* rec);

  int length(); 
};


LogicalAddress::LogicalAddress(NameNode* physical_address, int pc_offset) {
  _physical_address = physical_address;
  _pc_offset        = pc_offset;
  _next             = NULL;
  _offset           = -1; // Illegal value
}

void LogicalAddress::append(NameNode* physical_address, int pc_offset) {
  if (next()) {
    // hopefully these lists are not getting too long...
    next()->append(physical_address, pc_offset);
  } else {
    assert(_pc_offset <= pc_offset, "checking progress");
   _next = new LogicalAddress(physical_address, pc_offset);
  }
}

NameNode* LogicalAddress::physical_address_at(int pc_offset) {
  LogicalAddress* current = this;
  while (current->next() && current->next()->pc_offset() > pc_offset) {
    current = current->next();
  }
  return current->physical_address();
}

int LogicalAddress::length() {
  LogicalAddress* current = this;
  int result = 1;
  while (current->next()) {
    current = current->next();
    result++;
  }
  return result;
}

void LogicalAddress::generate(ScopeDescRecorder* rec) {
  // emit: 
  //  [first ]
  //  [next  ], offset
  // where last element has the is_last bit set.
  LogicalAddress* n = next();
  physical_address()->generate(rec, n == NULL);

  int last_pc_offset = 0;
  while (n != NULL) {
    LogicalAddress* current = n;
    n = n->next();
    current->physical_address()->generate(rec, n == NULL);
    assert(last_pc_offset <= current->pc_offset(), "checking progress");
    rec->genValue(current->pc_offset() - last_pc_offset);
    last_pc_offset = current->pc_offset();
  }
}

class Array: public ResourceObj {
 private:
  int index;
  int size;

  int offset;

  int* values;
 public:
  Array(int size);

  int length() { return index; }
  void extend(int newSize);
  int insertIfAbsent(int value);  // returns index for value
  void copy_to(int*& addr);
};

class ByteArray : public ResourceObj {
private:
  u_char* array;
  int  top;
  int  max;

  void extend();

 public:
  int size()      { return top; }
  u_char* start() { return array; }

  ByteArray(int size);

  void appendByte(u_char p) {
    if (top + (int) sizeof(u_char) > max) extend();
    array[top++] = p;
  }

  void appendHalf(int16 p);
#ifdef UNUSED
  void appendWord(int p);
#endif

  void putByteAt(u_char p, int offset) {
    assert( offset < max, "index out of bound");
    array[offset] = p;
  }

 void putHalfAt(int16 p, int offset);

  // Cut off some of the generated code.
  void setTop(int offset) {
    assert( this->top >= offset, "A smaller top is expected");
    this->top = offset;
  }

  void alignToWord();
  void copy_to(int*& addr);
};

NameNode* newValueName(oop value) {
  if (value->is_block()) {
    fatal("should never be a block");
    return NULL;
  } else {
    return new ValueName(value);
  }
}

bool NameNode::genHeaderByte(ScopeDescRecorder* rec, u_char code, bool is_last, int index) {
  // Since id is most likely to be 0, the info part of the header byte
  // indicates if is is non zero.
  // Experiments show id is zero in at least 90% of the generated nameDescs.
  // returns true if index could be inlined in headerByte.
  nameDescHeaderByte b;
  bool   can_inline  = index <= b.max_index;
  u_char coded_index = can_inline ? index : b.no_index;
  b.pack(code, is_last, coded_index);
  rec->codes->appendByte( b.value());
  return can_inline;
} 

inline int ScopeDescRecorder::getValueIndex(int v) {
  // if v fits into 7 bits inline the value instead of creating index
  if ( 0 <= v && v <= MAX_INLINE_VALUE) return v;
  return MAX_INLINE_VALUE + 1 + values->insertIfAbsent(v);
}

inline int ScopeDescRecorder::getOopIndex(oop o) {
  return o == 0 ? 0 : oops->insertIfAbsent((int)o) + 1;
}

void ScopeDescRecorder::emit_illegal_node(bool is_last) {
  nameDescHeaderByte b;
  b.pack_illegal(is_last);
  codes->appendByte(b.value());
}

void ScopeDescRecorder::emit_termination_node() {
  nameDescHeaderByte b;
  b.pack_termination(true);
  codes->appendByte(b.value());
}

void IllegalName::generate(ScopeDescRecorder* rec, bool is_last) {
  rec->emit_illegal_node(is_last);
}

// Please encapsulate iterator.
static ScopeDesc*     _sd;
static nmethodScopes* _scopes;
static ScopeDesc*     _getNextScopeDesc() {
  _sd = _scopes->getNext(_sd);
  if (!_sd) fatal("out of scopeDescs");
  return _sd;
}

Array::Array(int sz) {
  size   = sz;
  index  = 0;
  values = NEW_RESOURCE_ARRAY(int, sz);
}

int Array::insertIfAbsent(int value){
  for (int i = 0; i < index; i ++)
    if (values[i] == value)
      return i;
  if (index == size)
    extend(size*2);
  values[index] = value;
  return index++;
}

void Array::extend(int newSize) {
  int* newValues = NEW_RESOURCE_ARRAY(int, newSize);
  for(int i=0;i < index; i++)
    newValues[i] = values[i];
  values = newValues;
  size   = newSize;
}

void Array::copy_to(int*& addr) {
  for(int i=0;i < length(); i++) {
    *addr++ = values[i];
  }
}

ByteArray::ByteArray(int size) {
  array = NEW_RESOURCE_ARRAY(u_char, size); 
  max   = size;
  top   = 0;
}

void ByteArray::extend() {
  int  newMax = max*2;
  u_char* newArray = NEW_RESOURCE_ARRAY(u_char, newMax);
  for(int i=0;i < top; i++)
    newArray[i] = array[i];
  array = newArray;
  max  = newMax;
}

void ByteArray::appendHalf(int16 p) {
  if (top + (int) sizeof(int16) > max) extend();
  // Saving the half as two bytes to avoid alignment problem.
  array[top++] = p >> BYTE_WIDTH;
  array[top++] = (u_char)lowerBits(p, 8);
}

void ByteArray::putHalfAt(int16 p, int offset) {
  // Saving the half as two bytes to avoid alignment problem.
  array[offset  ] = p >> BYTE_WIDTH;
  array[offset+1] = (u_char)lowerBits(p, 8);
}

#ifdef UNUSED
void ByteArray::appendWord(int p) {
  if (top+sizeof(int) > max) extend();
  assert( size() % sizeof(int) == 0, "Not word aligned");
  int* s = (int*) &array[top];
  *s = p;
  top += sizeof(int);
}
#endif

void ByteArray::alignToWord() {
  int fill_size = (sizeof(int) - (size()%sizeof(int))) % sizeof(int);
  for(int i = 0; i < fill_size; i++)
    appendByte(0); 
}

void ByteArray::copy_to(int*& addr) {
  int* fromAddr = (int*) start();
  int len = size()/sizeof(int);
  for(int i=0;i < len; i++) {
    *addr++ = *fromAddr++;
  }
}

// Class hierarchy for nodes generating scopeDescs.
// ScopeDescNode
//  - MethodScopeNode
//  - TopLevelBlockScopeNode
//  - BlockScopeNode

# define INVALID_OFFSET -1

class ScopeDescNode: public ResourceObj {
 public:
  methodOop       method;
  bool            allocates_compiled_context;
  int             scopeID;
  bool            lite;
  int             senderBCI;
  bool            visible;

  GrowableArray<LogicalAddress*>* arg_list;
  GrowableArray<LogicalAddress*>* temp_list;
  GrowableArray<LogicalAddress*>* context_temp_list;
  GrowableArray<LogicalAddress*>* expr_stack_list;

  int offset; // byte offset to the encoded scopeDesc
              // Initial value is  INVALID_OFFSET
  
  bool usedInPcs;
  
 public:
  bool has_args()          const { return !lite && !arg_list->isEmpty();          }
  bool has_temps()         const { return !lite && !temp_list->isEmpty();         }
  bool has_context_temps() const { return !lite && !context_temp_list->isEmpty(); }
  bool has_expr_stack()    const { return !lite && !expr_stack_list->isEmpty();   }
  bool has_context()       const { return allocates_compiled_context; }

  bool has_nameDescs()     const { return has_args()
                                       || has_temps()
                                       || has_context_temps()
                                       || has_expr_stack(); }

  ScopeInfo scopesHead;
  ScopeInfo scopesTail;
  ScopeInfo next;

  ScopeDescNode(methodOop method, bool allocates_compiled_context, int scopeID, bool lite, int senderBCI, bool visible);

  void addNested(ScopeInfo scope);

  virtual u_char code() = 0;

  virtual void generate(ScopeDescRecorder* rec, int senderScopeOffset, bool bigHeader);
  void generateBody(ScopeDescRecorder* rec, int senderScopeOffset);
  void generateNameDescs(ScopeDescRecorder* rec);

  void generate_solid(GrowableArray<LogicalAddress*>* list, ScopeDescRecorder* rec);
  void generate_sparse(GrowableArray<LogicalAddress*>* list, ScopeDescRecorder* rec);

  bool computeVisibility();

  ScopeInfo find_scope(int scope_id);

  virtual void verify(ScopeDesc* sd);
  void verifyBody();
};

ScopeInfo ScopeDescNode::find_scope(int scope_id) {
  if (scopeID == scope_id) return this;
  for(ScopeInfo p = scopesHead; p  != NULL; p = p->next) {
    ScopeInfo result = p->find_scope(scope_id);
    if (result) return result;
  }
  return NULL;
}

void ScopeDescNode::generate_solid(GrowableArray<LogicalAddress*>* list, ScopeDescRecorder* rec) {
  // Dump all the elements
  for(int i = 0; i < list->length(); i++) {
    assert(list->at(i), "must be a solid array"); 
    list->at(i)->generate(rec);
  }
  // Terminate the list
  rec->emit_termination_node();
}

void ScopeDescNode::generate_sparse(GrowableArray<LogicalAddress*>* list, ScopeDescRecorder* rec) {
  // Dump all the elements
  for(int i = 0; i < list->length(); i++) {
    if (list->at(i)) {
      list->at(i)->generate(rec);
      rec->genValue(i);
    }
  }
  // Terminate the list
  rec->emit_termination_node();
}

struct PcDescNode: public ResourceObj {
  int     pcOffset;
  ScopeInfo scope;
  int     bci;
};

class PcDescInfoClass : public ResourceObj {
 protected:
  PcDescNode*   nodes;
  int          end;
  int          size;
 public:
  PcDescInfoClass(int size);
  int length() { return end; }
  void extend(int newSize);
  void add(int pcOffset, ScopeInfo scope, int bci);
  void mark_scopes();
  void copy_to(int*& addr);
};

ScopeDescNode::ScopeDescNode(methodOop method, bool allocates_compiled_context, int scopeID, bool lite, int senderBCI, bool visible) {
  this->scopeID                    = scopeID;
  this->method                     = method;
  this->lite                       = lite;
  this->senderBCI                  = senderBCI;
  this->visible                    = visible;
  this->allocates_compiled_context = allocates_compiled_context;

  arg_list          = new GrowableArray<LogicalAddress*>(INITIAL_ARG_SIZE);
  temp_list         = new GrowableArray<LogicalAddress*>(INITIAL_TEMP_SIZE);
  context_temp_list = new GrowableArray<LogicalAddress*>(INITIAL_CONTEXT_TEMP_SIZE);
  expr_stack_list   = new GrowableArray<LogicalAddress*>(INITIAL_EXPR_STACK_SIZE);

  offset     = INVALID_OFFSET;
  scopesHead = NULL;
  scopesTail = NULL;
  usedInPcs  = false;
}

void ScopeDescNode::addNested(ScopeInfo scope) {
  scope->next = NULL;
  if (scopesHead == NULL) {
    scopesHead = scopesTail = scope; 
  } else {
    scopesTail->next = scope;
    scopesTail = scope;
  }
}

void ScopeDescNode::generate(ScopeDescRecorder* rec, int senderScopeOffset, bool bigHeader) {
  offset = rec->codes->size();

  rec->genScopeDescHeader(code(), lite, has_args(), has_temps(), has_context_temps(), has_expr_stack(), has_context(), bigHeader);
  if (offset != 0) {
    // Save the sender
    rec->genValue(offset - senderScopeOffset);
    rec->genValue(senderBCI);
  }
  rec->genOop(method);
  rec->genValue(scopeID);
}

void ScopeDescNode::generateNameDescs(ScopeDescRecorder* rec) {
  assert(has_nameDescs(), "must have nameDescs");
  if (has_args())          generate_solid(arg_list, rec);
  if (has_temps())         generate_solid(temp_list, rec);
  if (has_context_temps()) generate_solid(context_temp_list, rec);
  if (has_expr_stack())    generate_sparse(expr_stack_list, rec);
}

void ScopeDescNode::generateBody(ScopeDescRecorder* rec, int senderScopeOffset) {
  if (has_nameDescs()) {
    generateNameDescs(rec);
    if (!rec->updateScopeDescHeader(offset, rec->codes->size())) {
      // the offsets in the encoded scopeDesc header could not be
      // written in the pre-allocated two bytes.
      // Make the header large enough for extended index.
      rec->codes->setTop(offset);
      generate(rec, senderScopeOffset, true);
      generateNameDescs(rec);
      rec->updateExtScopeDescHeader(offset, rec->codes->size());
   }
  }
  
  for(ScopeInfo p = scopesHead; p  != NULL; p = p->next) {
    if (p->visible) {
      p->generate(rec, offset, false);
      p->generateBody(rec, offset);
    }
  }
}

void ScopeDescNode::verify(ScopeDesc* sd) {
  if (senderBCI != IllegalBCI && senderBCI != sd->senderBCI()) fatal("senderBCI is wrong");
}

void ScopeDescNode::verifyBody() {
  for(ScopeInfo p = scopesHead; p  != NULL; p = p->next) {
    if (p->visible) {
      p->verify(_getNextScopeDesc());
      p->verifyBody();
    }
  }
}

bool ScopeDescNode::computeVisibility() {
  visible = false;
  for(ScopeInfo p = scopesHead; p  != NULL; p = p->next) {
    visible = p->computeVisibility() || visible;
  }
  visible = visible || (usedInPcs && GenerateLiteScopeDescs) || !lite;
  return visible;
}


class MethodScopeNode: public ScopeDescNode {
 public:
  LookupKey*      key;
  LogicalAddress* receiver_location;

  u_char code() { return METHOD_CODE; }

  MethodScopeNode(LookupKey*      key, 
                  methodOop       method, 
		  LogicalAddress* receiver_location,
		  bool            allocates_compiled_context,
		  bool            lite,
		  int             scopeID,
		  int             senderBCI,
		  bool            visible)
    : ScopeDescNode(method, allocates_compiled_context, scopeID, lite, senderBCI, visible) {
      this->key               = key;
      this->receiver_location = receiver_location;
  }

  void generate(ScopeDescRecorder* rec, int senderScopeOffset, bool bigHeader);

  void verify(ScopeDesc* sd);
};


void MethodScopeNode::generate(ScopeDescRecorder* rec,
			       int senderScopeOffset, bool bigHeader) {
  ScopeDescNode::generate(rec,senderScopeOffset, bigHeader);
  rec->genOop(key->klass());
  rec->genOop(key->selector_or_method());
  receiver_location->generate(rec);
}

void MethodScopeNode::verify(ScopeDesc* sd) {
  ScopeDescNode::verify(sd);
  if (!sd->isMethodScope()) fatal("MethodScope expected");
}

class TopLevelBlockScopeNode: public ScopeDescNode {
 public:
  LogicalAddress* receiver_location;
  klassOop        receiver_klass;

  u_char code() { return TOPLEVELBLOCK_CODE; }

  TopLevelBlockScopeNode(methodOop  method, LogicalAddress* receiver_location, klassOop receiver_klass, bool allocates_compiled_context)
  : ScopeDescNode(method, allocates_compiled_context, false, 0, NULL, true) {
    this->receiver_location = receiver_location;
    this->receiver_klass    = receiver_klass;
  }

  void generate(ScopeDescRecorder* rec, int senderScopeOffset, bool bigHeader) {
    ScopeDescNode::generate(rec, senderScopeOffset, bigHeader);
    receiver_location->generate(rec);
    rec->genOop(receiver_klass);
  }

  void verify(ScopeDesc* sd) {
    ScopeDescNode::verify(sd);
    if (!sd->isTopLevelBlockScope()) fatal("TopLevelBlockScope expected");
  }  
};

class BlockScopeNode: public ScopeDescNode {
 public:
  ScopeInfo parent;

  BlockScopeNode(methodOop method,
		 ScopeInfo parent,
		 bool      allocates_compiled_context,
		 bool      lite,
		 int       scopeID,
		 int       senderBCI, 
		 bool      visible) 
    : ScopeDescNode(method, allocates_compiled_context, scopeID, lite, senderBCI, visible) {
    this->parent = parent;
  }

  u_char code() { return BLOCK_CODE; }

  void generate(ScopeDescRecorder* rec, int senderScopeOffset, bool bigHeader);

  void verify(ScopeDesc* sd);
};

void BlockScopeNode::generate(ScopeDescRecorder* rec, int senderScopeOffset, bool bigHeader) {
  ScopeDescNode::generate(rec, senderScopeOffset, bigHeader);
  rec->genValue(offset - parent->offset);
}

void BlockScopeNode::verify(ScopeDesc* sd) {
  ScopeDescNode::verify(sd);
} 

class NonInlinedBlockScopeNode: public ResourceObj {
 public:
  int                       offset;
  NonInlinedBlockScopeNode* next;
  methodOop                 method;
  ScopeInfo                 parent;

  NonInlinedBlockScopeNode(methodOop method, ScopeInfo parent)  {
    this->method = method;
    this->parent = parent;
    this->offset = INVALID_OFFSET;
    this->next   = NULL;
  }
  u_char code() { return NONINLINED_BLOCK_CODE; }
  void generate(ScopeDescRecorder* rec);
};

void NonInlinedBlockScopeNode::generate(ScopeDescRecorder* rec) {
  offset = rec->codes->size();
  rec->genScopeDescHeader(code(), false, false, false, false, false, false, false);
  rec->genOop(method);
  rec->genValue(offset - parent->offset);
//  if (WizardMode) lprintf("generating NonInlinedBlockScopeNode at %d\n", offset);
}

void ScopeDescRecorder::generate() {
  assert(root, "root scope must be present");
  // Generate the bytecodes for the ScopeDescs and their NameDescs.

  generateDependencies();

  pcs->mark_scopes();
  (void) root->computeVisibility();
  root->generate(this, 0, false);
  root->generateBody(this, 0);
  
  for(NonInlinedBlockScopeNode* p = nonInlinedBlockScopesHead; p != NULL; p = p->next) {
    p->generate(this);
  }

  codes->alignToWord();
  _hasCodeBeenGenerated = true;
}

void ScopeDescRecorder::generateDependencies() {
  int end_marker = 0;
  for (int index = 0; index < dependants->length(); index++) {
    int i = oops->insertIfAbsent((int)dependants->at(index));
    if (i > end_marker)  end_marker = i;
  }
  dependants_end = end_marker;
}

ScopeInfo ScopeDescRecorder::addScope(ScopeInfo scope, ScopeInfo senderScope){
  if (root == NULL) {
    assert( senderScope == NULL, "Root scope must be the first");
    root = scope;
  } else {
    assert( senderScope != NULL, "Sender scope must be present"); 
    senderScope->addNested(scope);
  }
  return scope;
}

NonInlinedBlockScopeNode* ScopeDescRecorder::addNonInlinedBlockScope(NonInlinedBlockScopeNode* scope) {
  scope->next = NULL;
  if (nonInlinedBlockScopesHead == NULL) {
    nonInlinedBlockScopesHead = nonInlinedBlockScopesTail = scope; 
  } else {
    nonInlinedBlockScopesTail->next = scope;
    nonInlinedBlockScopesTail = scope;
  }
  return scope;
}

int ScopeDescRecorder::offset(ScopeInfo scope) {
  assert(scope->offset != INVALID_OFFSET, "uninitialized offset");
  return scope->offset;
}

int ScopeDescRecorder::offset_for_noninlined_scope_node(NonInlinedBlockScopeNode* scope) {
  assert(scope->offset != INVALID_OFFSET, "uninitialized offset");
  return scope->offset;
}

ScopeInfo ScopeDescRecorder::addMethodScope(LookupKey*      key, 
					    methodOop       method,
                                            LogicalAddress* receiver_location,
					    bool            allocates_compiled_context,
					    bool            lite,
					    int             scopeID,
					    ScopeInfo       senderScope, 
					    int             senderBCI,
					    bool            visible) {
  return addScope(
    new MethodScopeNode(key, method, receiver_location, allocates_compiled_context, lite, scopeID, senderBCI, visible),		      
    senderScope);
}


ScopeInfo ScopeDescRecorder::addBlockScope(methodOop       method,
                                           ScopeInfo       parent,
					   bool            allocates_compiled_context,
					   bool            lite,
					   int             scopeID,
					   ScopeInfo       senderScope, 
					   int             senderBCI,
					   bool            visible) {
  return addScope(
    new BlockScopeNode(method, parent, allocates_compiled_context, lite, scopeID, senderBCI, visible),
    senderScope);
}

ScopeInfo ScopeDescRecorder::addTopLevelBlockScope(methodOop       method, 
                                                   LogicalAddress* receiver_location,
						   klassOop        receiver_klass,
                                                   bool            allocates_compiled_context) {
  return addScope(new TopLevelBlockScopeNode(method, receiver_location, receiver_klass, allocates_compiled_context), NULL);
}

NonInlinedBlockScopeNode* ScopeDescRecorder::addNonInlinedBlockScope(methodOop method, ScopeInfo parent) {
  
  return addNonInlinedBlockScope(new NonInlinedBlockScopeNode(method, parent));
}


void ScopeDescRecorder::addArgument(ScopeInfo scope, int index, LogicalAddress* location){
  assert(!scope->lite, "cannot add slot to lite scopeDesc");
  scope->arg_list->at_put_grow(index, location);
}

void ScopeDescRecorder::addTemporary(ScopeInfo scope, int index, LogicalAddress* location){
  assert(!scope->lite, "cannot add slot to lite scopeDesc");
  scope->temp_list->at_put_grow(index, location);
}

void ScopeDescRecorder::addExprStack(ScopeInfo scope, int index, LogicalAddress* location) {
  assert(!scope->lite, "cannot add expression to lite scopeDesc");
  scope->expr_stack_list->at_put_grow(index, location);
}

void ScopeDescRecorder::addContextTemporary(ScopeInfo scope, int index, LogicalAddress* location) {
  assert(!scope->lite, "cannot add expression to lite scopeDesc");
  scope->context_temp_list->at_put_grow(index, location);
}

LogicalAddress* ScopeDescRecorder::createLogicalAddress(NameNode* initial_value) {
  return new LogicalAddress(initial_value);
}

void ScopeDescRecorder::changeLogicalAddress(LogicalAddress* location, NameNode* new_value, int pc_offset) {
  location->append(new_value, pc_offset);
}

void ScopeDescRecorder::genScopeDescHeader(u_char code, 
					   bool lite,
					   bool args,
					   bool temps,
					   bool context_temps,
					   bool expr_stack,
					   bool has_context,
					   bool bigHeader) {
  scopeDescHeaderByte b;
  b.pack(code, lite, args, temps, context_temps, expr_stack, has_context);
  codes->appendByte(b.value());
  if (b.has_nameDescs()) {
    codes->appendByte(0); // placeholder for next offset
    if (bigHeader) {
      codes->appendHalf(0);
    }
  }
}

int ScopeDescRecorder::updateScopeDescHeader(int offset, int next) {
  int nextIndex = getValueIndex(next - offset);
  if (nextIndex < EXTENDED_INDEX) {
    codes->putByteAt(nextIndex, offset+1);
    return true;
  } else {
    return false;
  }
}

void ScopeDescRecorder::updateExtScopeDescHeader(int offset, int next) {
  int nextIndex = getValueIndex(next - offset);
  codes->putByteAt(EXTENDED_INDEX, offset+1);
  codes->putHalfAt(nextIndex,      offset+2);
}

inline void ScopeDescRecorder::genIndex(int index) {
  if (index < EXTENDED_INDEX) {
    // generate 1 byte indexing the oop.
    codes->appendByte(index);
  } else {
    codes->appendByte(EXTENDED_INDEX);
    codes->appendHalf(index);
  }
}

void ScopeDescRecorder::genValue(int v) {
  genIndex(getValueIndex(v));
}

void ScopeDescRecorder::genOop(oop o) {
  genIndex(getOopIndex(o));
}

PcDescInfoClass::PcDescInfoClass(int sz) {
  nodes = NEW_RESOURCE_ARRAY(PcDescNode, sz); 
  end   = 0;
  size  = sz;
}

void PcDescInfoClass::extend(int newSize) {
  PcDescNode* newNodes = NEW_RESOURCE_ARRAY(PcDescNode, newSize);
  for(int i=0;i < end; i++)
    newNodes[i] = nodes[i];
  nodes = newNodes;
  size  = newSize;
}

void PcDescInfoClass::add(int pcOffset, ScopeInfo scope, int bci) {
  if (scope->lite && !GenerateLiteScopeDescs) return;
  if (end == size) extend(size*2);

  // After Robert's jmp elimination, instructions can be eliminated
  // We can therefore remove pc descs describing the eliminated code.
  while (end > 0 && pcOffset < nodes[end-1].pcOffset) {
    end--;
  }

  if (CompressPcDescs && end > 0) {
    // skip if the previous had the same scope and bci.
    if (scope == nodes[end-1].scope && bci == nodes[end-1].bci) return;
    // overwrite if the previous had the same pcOffset.
    if (pcOffset == nodes[end-1].pcOffset) {
      end--;
    }
  }

  nodes[end].pcOffset = pcOffset;
  nodes[end].scope    = scope;
  nodes[end].bci      = bci;
  end++;
}

void PcDescInfoClass::mark_scopes() {
  for(int i=0;i < end; i++) {
    if (nodes[i].scope) nodes[i].scope->usedInPcs = true;
  }
}

void PcDescInfoClass::copy_to(int*& addr) {
  for(int i = 0; i < end; i++) {
    PcDesc* pc = (PcDesc*) addr;
    pc->pc       = nodes[i].pcOffset;
    pc->scope    = nodes[i].scope ? nodes[i].scope->offset : IllegalBCI;
    pc->byteCode = nodes[i].bci;
    addr += sizeof(PcDesc)/sizeof(int);
  }
}

void LocationName::generate(ScopeDescRecorder* rec, bool is_last) {
  Location converted_location = rec->convert_location(l);
  int index = rec->getValueIndex(converted_location._loc);
  if (!genHeaderByte(rec, LOCATION_CODE, is_last, index)) rec->genIndex(index);
}

void ValueName::generate(ScopeDescRecorder* rec, bool is_last) {
  int index = rec->getOopIndex(value);
  if (!genHeaderByte(rec, VALUE_CODE, is_last, index)) rec->genIndex(index);
}

void MemoizedName::generate(ScopeDescRecorder* rec, bool is_last) {
  Location converted_location  = rec->convert_location(loc);
  int index = rec->getValueIndex(converted_location._loc);
  if (!genHeaderByte(rec, MEMOIZEDBLOCK_CODE, is_last, index)) rec->genIndex(index);
  rec->genOop(oop(block_method));
  rec->genValue(parent_scope == NULL ? 0 : parent_scope->offset); // Lars, please check this (gri 2/2/96)
}

void BlockValueName::generate(ScopeDescRecorder* rec, bool is_last) {
  int index = rec->getOopIndex(oop(block_method));
  if (!genHeaderByte(rec, BLOCKVALUE_CODE, is_last, index)) rec->genIndex(index);
  rec->genValue(parent_scope == NULL ? 0 : parent_scope->offset); // Lars, please check this (gri 2/2/96)
}

Location ScopeDescRecorder::convert_location(Location loc) {
  if (!loc.isContextLocation()) return loc;
  int scope_id = loc.scopeID();

  // Find the ScopeInfo with the right scope_id
  ScopeInfo scope = theCompiler->scopes->at(scope_id)->scopeInfo();
  assert(scope, "scope must exist");
  if (scope->offset == INVALID_OFFSET) {
    std->print_cr(loc.name());
    theCompiler->print_code(false);
    fatal("compiler error: context location appears outside its scope");    // Urs 5/96
  }
  return Location::runtimeContextLocation(loc.contextNo(), loc.tempNo(), scope->offset);
}


int ScopeDescRecorder::size() {
  return   sizeof(nmethodScopes)
         + codes->size()
         + oops->length()   * sizeof(oop)
         + values->length() * sizeof(int)
         + pcs->length()    * sizeof(PcDesc);
}

ScopeDescRecorder::ScopeDescRecorder(int byte_size, int pcDesc_size) {
  // size is the initial size of the byte array.
  root   = NULL;
  oops   = new Array(INITIAL_OOPS_SIZE);
  values = new Array(INITIAL_VALUES_SIZE);
  codes  = new ByteArray(byte_size);
  pcs    = new PcDescInfoClass(pcDesc_size);

  dependants = new GrowableArray<klassOop>(INITIAL_DEPENDANTS_SIZE);

  _hasCodeBeenGenerated = false;

  nonInlinedBlockScopesHead = NULL;
  nonInlinedBlockScopesTail = NULL;
}

void ScopeDescRecorder::copyTo(nmethod* nm) {
  nmethodScopes* d = (nmethodScopes*) nm->scopes();

  // Copy the body part of the nmethodScopes
  int* start = (int*)(d+1);
  int* p     = start;

  d->set_nmethod_offset((char*) d - (char*) nm);

  codes->copy_to( p);

  d->set_oops_offset((char*) p - (char*) start);
  oops->copy_to( p);

  d->set_value_offset((char*) p - (char*) start);
  values->copy_to( p);

  d->set_pcs_offset((char*) p - (char*) start);
  pcs->copy_to( p);

  d->set_length((char*) p - (char*) start);

  d->set_dependants_end(dependants_end);

  assert( (char*) d + size() == (char*) p, "wrong size of nmethodScopes");
}

void ScopeDescRecorder::addPcDesc(int pcOffset, ScopeInfo scope, int bci) {
  assert( scope, "scope must be specified in addPcDesc");
  assert( root, "root must be present");
  pcs->add(pcOffset, scope, bci);
}

void ScopeDescRecorder::add_dependant(LookupKey* key) {
  // make this nmethod dependent on the receiver klass of the lookup key.
  dependants->append(key->klass());
}


void ScopeDescRecorder::verify(nmethodScopes* scopes) {
  // Initialize iterator
  _scopes = scopes;
  _sd     = NULL;

  assert( root, "root must be present to verify");
  root->verify(_getNextScopeDesc());
  root->verifyBody();
}

#endif
