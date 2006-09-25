/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.29 */
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

// The following classes are used for operations
// initiated by a delta process but must
// take place in the vmProcess.

class VM_Operation : public PrintableStackObj {
 private:
   DeltaProcess* _calling_process;
 public:
   void set_calling_process(DeltaProcess* p) { _calling_process = p; }
   DeltaProcess* calling_process() const { return _calling_process; }

   virtual bool is_scavenge()        const { return false; }
   virtual bool is_garbage_collect() const { return false; }
   virtual bool is_single_step()     const { return false; }

   void evaluate();
   // Evaluate is called in the vmProcess
   virtual void doit() = 0;
   void print() { std->print("%s", name()); }

   virtual char* name() { return "vanilla"; }
};

class VM_Scavenge : public VM_Operation {
 private:
  oop* addr;
 public:
  bool is_scavenge() const 	{ return true; }
  VM_Scavenge(oop* addr) 	{ this->addr = addr; }
  void doit();

  char* name() { return "scavenge"; }
};

class VM_Genesis : public VM_Operation {
 public:
  VM_Genesis();
  void doit();

  char* name() { return "genesis"; }
};

class VM_GarbageCollect : public VM_Operation {
 private:
  oop* addr;
 public:
  bool is_garbage_collect() const 	{ return true; }
  VM_GarbageCollect(oop* addr) 		{ this->addr = addr; }
  void doit();

  char* name() { return "terminate process"; }
};

class VM_TerminateProcess : public VM_Operation {
 private:
  DeltaProcess* target;
 public:
  VM_TerminateProcess(DeltaProcess* target) { this->target = target; }
  void doit();

  char* name() { return "terminate process"; }
};


class VM_DeoptimizeStacks : public VM_Operation {
 public:
  void doit();

  char* name() { return "deoptimize stacks"; }
};

# ifdef DELTA_COMPILER

class VM_OptimizeMethod : public VM_Operation {
 private:
  LookupKey _key;
  methodOop _method;
  nmethod*  _nm;
 public:
  VM_OptimizeMethod(LookupKey* key, methodOop method) : _key(key) {
    _method = method;
  }
  nmethod* result() const 	{ return _nm; }
  void doit();

  char* name() { return "optimize method"; }
};

class VM_OptimizeRScope : public VM_Operation {
 private:
  RScope*   _scope;
  nmethod*  _nm;
 public:
  VM_OptimizeRScope(RScope* scope) {
    _scope = scope;
  }
  nmethod* result() const 	{ return _nm; }
  void doit();

  char* name() { return "optimize rscope"; }
};

class VM_OptimizeBlockMethod : public VM_Operation {
 private:
  blockClosureOop           closure;
  NonInlinedBlockScopeDesc* scope;
  nmethod*                  nm;
 public:
  VM_OptimizeBlockMethod(blockClosureOop closure, NonInlinedBlockScopeDesc* scope) {
    this->closure   = closure;
    this->scope     = scope;
  }
  void doit();
  nmethod* method() const 	{ return nm; }

  char* name() { return "optimize block method"; }
};

# endif
