/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.35 */
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
# include "incls/_vmOperations.cpp.incl"

void VM_Operation::evaluate() {
  EventMarker  em("VM operation %s", name());
  ResourceMark rm;
  doit();
}

VM_Genesis::VM_Genesis() {}

void VM_Genesis::doit() {
  ErrorHandler::genesis();
}

void VM_Scavenge::doit() {
  // For debugging gc-problems
  if (false) { 
    ResourceMark rm;
    FlagSetting fs(PrintLongFrames, true);
    frame f = calling_process()->last_frame();
    f.print();
  }

  if (PrintStackAtScavenge) {
    std->print_cr("*** BEFORE ***");
    Processes::print();
  }
  Universe::scavenge(addr);
  if (PrintStackAtScavenge) {
    std->print_cr("*** AFTER ***");
    Processes::print();
    std->print_cr("******");
  }
} 			
 			
void VM_GarbageCollect::doit() {
  *addr = MarkSweep::collect(*addr); 
}

void VM_DeoptimizeStacks::doit() {
  Processes::deoptimize_all();
  CompiledCodeOnly = false;
}

void VM_TerminateProcess::doit() {
  VMProcess::terminate(target);
  delete calling_process();
  set_calling_process(NULL);
}

# ifdef DELTA_COMPILER

void VM_OptimizeMethod::doit() {
  if (_method->is_blockMethod()) {
    //Compiler c(closure, scope);
    //return c.compile();  
    compiler_warning("can't recompile block yet");
    _nm = NULL;
    return;
  }
  Compiler c(&_key, _method);
  _nm = c.compile();
}

void VM_OptimizeRScope::doit() {
  Compiler c(_scope);
  _nm = c.compile();
}

void VM_OptimizeBlockMethod::doit() {
  Compiler c(closure, scope);
  nm = c.compile();
}

# endif
