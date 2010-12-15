/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.68 $ */
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



// This file contains some global types, plus many partial definitions
// to reduce include file dependencies.  (If foo.h contains a bar* and
// struct bar is defined here, foo.h need not depend on bar.h.)

#define SYSTEM_NAME "strongtalk"

class bootstrap;

// most portable way of dealing with bools is to treat them as ints;
// the new basic bool type isn't goodness.
typedef int _bool_type_;
#ifdef bool
#undef bool
#endif
#define bool _bool_type_

typedef int smi;

#ifdef WIN32
typedef          long  int32_t;
typedef unsigned long  uint32_t;
typedef unsigned char  uint8_t;
#else
#include <inttypes.h>
#endif

typedef unsigned char  u_char;
typedef unsigned short doubleByte;
typedef unsigned short uint16;
typedef unsigned short u_short;
typedef          short int16;
typedef unsigned long  uint32;

class  universe;
class  space;
class    newSpace;
class    oldSpace;
class  generation;
class    newGeneration;
class    oldGeneration;
class  rSet;
class  symbolTable;
class  klassTable;
class  ageTable;

class ReservedSpace;
class VirtualSpace;

class  ResourceObj;
class    PrintableResourceObj;

class  CHeapObj;
class    PrintableCHeapObj;

class  StackObj;
class    PrintableStackObj;

class ostream;
class stringStream;

class Thread;
class Event;

class  primitive_desc;
class  DLLCache;
class  CompiledDLL_Cache;

class scopeNode;

class  frame;
class  vframe;
class    deltaVFrame;
class      compiledVFrame;
class      interpretedVFrame;
class    cVFrame;
class      cChunk;
class  Stack;

class   Process;
class     VMProcess;
class     DeltaProcess;
class   Processes;

class   VM_Operation;

class   spaceSizes;

class  EventBuffer;
class  varDesc;

class  LookupKey;
class  LookupTable;

class InterpretedIC;

class IC_Iterator;
class   InterpretedIC_Iterator;
class   CompiledIC_Iterator;

class InterpretedPrim_Cache;
class InterpretedDLL_Cache;

class   Heap;

// type of runtime functions
typedef oop     (* fntype) (...);
typedef void    (*oopsDoFn)(oop* p);
typedef void    (*doFn)();

class ResourceArea;
class ResourceMark;
class Resources;

template <class T> class Closure;
class ObjectClosure;
class ObjectLayoutClosure;
class FrameClosure;
class FrameLayoutClosure;
class ProcessClosure;
class ObjectFilterClosure;
class OopClosure;
class klassOopClosure;
class outputStream;

#ifdef DELTA_COMPILER

  class Compiler;
  class OldAssembler;
  class Assembler;
  class Register;
  class Inliner;
  class CompiledLoop;
  class LoopRegCandidate;
  class HoistedTypeTest;
  class IntervalInfo;

  class	zone;
  class jumpTable;
  class jumpTableEntry;
  class codeTable;
  class	Heap;
  class relocInfo;
  class PcDesc;
  class	ScopeDesc;
  class NonInlinedBlockScopeDesc;
  class NameDesc;

  class nmethodScopes;
  class PcDesc;
  class ScopeDescRecorder;
  class NameNode;
  typedef class ScopeDescNode    *ScopeInfo;
  typedef class PcDescInfoClass  *PcDescInfo;
  class LogicalAddress;
  class ScopeDescRecorder;
  class NonInlinedBlockScopeNode;

  class	NCodeBase;
  class	  OopNCode;
  class	    nmethod;
  class	    PIC;
  class	CompiledIC;
  class PrimitiveIC;

  class nmethod_patch;

  class AbstractBB;
  class   BB;
  class     BranchBB;
  class     MergeBB;
  class BBIterator;
  
  class Node;
  class   LoopHeaderNode;
  class   NonTrivialNode;
  class	    BranchNode;
  class	      TArithRRNode;
  class	    LoadOffsetNode;
  class     CallNode;
  class       PrimNode;
  class   TrivialNode;
  class     MergeNode;
  
  class PReg;
  class   BlockPReg;
  class   ConstPReg;
  class   NoPReg;
  class   SAPReg;
  class     SplitPReg;

  class IRGenerator;

  class Compiler;
  class Scope;
  class   InlinedScope;

  class Expr;
  class   KlassExpr;
  class   ConstantExpr;
  class   MergeExpr;
  class   UnknownExpr;
  class ExprStack;
  
  class DefUse;
  class   Use;
  class   Def;
  class PRegBBIndex;
  class DUInfo;
  class BitVector;
  class LongRegisterMask;
  class CPInfo;
  class RegCandidate;
  class RegisterEqClass;
  
  class RUncommonBranch;
  class RScope;
  class   RDeltaScope;
  class   RPICScope;
  class     RUntakenScope;
  class RDatabaseScope;

  class Recompilee;
  class   CompiledRecompilee;
  class   InterpretedRecompilee;
  class RFrame;
  class   CompiledRFrame;
  class   InterpretedRFrame;
  class RecompilationPolicy;

  class SendInfo;
  class HoistedTypeTest;

class StackChunkBuilder;

#endif
