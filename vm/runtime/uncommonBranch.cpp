/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.24 $ */
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
# include "incls/_uncommonBranch.cpp.incl"

// Tells whether the call has been executed before
// %note:
//    This function is highly INTEL specific.

bool patch_uncommon_call(frame* f) {
  // patch the call site:
  //  from: call _unused_uncommon_trap
  //  to:   call _used_uncommon_trap

  int* next_inst = (int*) f->pc();
  int* dest_addr = next_inst-1;
  int  dest      = *dest_addr + (int) next_inst;

  // return true if the call has been executed before
  if (dest == (int)StubRoutines::used_uncommon_trap_entry())
    return true;

  assert(dest == (int)StubRoutines::unused_uncommon_trap_entry(), "Make sure we are patching the right call");

  // patch with used_uncommon_trap
  *dest_addr = ((int)StubRoutines::used_uncommon_trap_entry()) - ((int) next_inst);

  assert(*dest_addr + (int) next_inst == (int)StubRoutines::used_uncommon_trap_entry(), "Check the patch");

  // return false since the call is patched
  return false;
}


// Tells whether the frame is a candidate for deoptimization by
// checking if the frame uses contextOops with forward pointers.
static bool has_invalid_context(frame* f) {
  // Return false if we're not in compiled code
  if (!f->is_compiled_frame()) return false;

  // Iterate over the vframes and check the compiled_context
  compiledVFrame* vf = (compiledVFrame*) vframe::new_vframe(f);
  assert(vf->is_compiled_frame(), "should be compiled vframe");
  while (true) {
    contextOop con = vf->compiled_context();

    // std->print_cr("checking context fp = 0x%lx, pc = 0x%lx", f->fp(), f->pc());
    if (con) con->print();

    if (con && con->unoptimized_context()) return true;
    if (vf->is_top()) break;
    vf = (compiledVFrame*) vf->sender();
    assert(vf->is_compiled_frame(), "should be compiled vframe");
  }
  return false;
}


class FrameAndContextElement : public ResourceObj {
 public:
  frame      fr;
  contextOop con;

  FrameAndContextElement(frame* f, contextOop c) {
    fr  = *f;
    con = c;
  }
};


void collect_compiled_contexts_for(frame* f, GrowableArray<FrameAndContextElement*>* elements) {
  // Return false if we're not in compiled code
  if (!f->is_compiled_frame()) return;

  // Iterate over the vframes and check the compiled_context
  compiledVFrame* vf = (compiledVFrame*) vframe::new_vframe(f);
  assert(vf->is_compiled_frame(), "should be compiled vframe");
  while (true) {
    contextOop con = vf->compiled_context();
    if (con) {
      elements->append(new FrameAndContextElement(f, con));
    }
    if (vf->is_top()) break;
    vf = (compiledVFrame*) vf->sender();
    assert(vf->is_compiled_frame(), "should be compiled vframe");
  }
}


class EnableDeoptimization: StackObj {
 public:
  EnableDeoptimization()  { StackChunkBuilder::begin_deoptimization(); }
  ~EnableDeoptimization() { StackChunkBuilder::end_deoptimization();   }
};


void uncommon_trap() {
  /*
  if (UseNewBackend) {
    warning("uncommon traps not supported yet for new backend");
    Unimplemented();
  }
  */

  ResourceMark rm;
  // Find the frame that caused the uncommon trap
  DeltaProcess* process = DeltaProcess::active();

#ifdef ASSERT
  process->verify();
#endif

  FlagSetting fl(processSemaphore, true);

  process->enter_uncommon();
  frame f = process->last_frame();

  // Patch the call destination if necessary
  bool used = patch_uncommon_call(&f);

  // Find the nmethod containing the uncommon trap
  compiledVFrame* vf = (compiledVFrame*) vframe::new_vframe(&f);
  assert(vf->is_compiled_frame(), "must be compiled frame");
  nmethod* nm = vf->code();

  nm->inc_uncommon_trap_counter();

  LOG_EVENT3("Uncommon trap in 0x%lx@%d #%d", nm, vf->scope()->offset(), nm->uncommon_trap_counter());

  /* For Debugging inserted by Lars Bak 5-13-96
  if (nm->is_block()) { 
    PrintUncommonBranches = true;
    TraceDeoptimization = true;
  } else {
    PrintUncommonBranches = false;
    TraceDeoptimization = false;
  }
  */

  if (PrintUncommonBranches)  {
    std->print("%s trap in ", used ? "Uncommon" : "New uncommon");
    nm->print_value_on(std);
    std->print(" #%d", nm->uncommon_trap_counter());
    if (WizardMode) std->print(" @%d called from %#x", vf->scope()->offset(), f.pc() - Assembler::sizeOfCall);
    std->cr();
    if (TraceDeoptimization) vf->print_activation(0);
    process->trace_top(0, 3);
  }

  // if counter is high enough, recompile the nmethod
  if (RecompilationPolicy::shouldRecompileAfterUncommonTrap(nm)) {
    if (nm->isZombie()) nm->resurrect();
    Recompilation recomp(vf->receiver(), nm, true);
    VMProcess::execute(&recomp);
    nm->makeZombie(false); // only allow the old method to be release AFTER recompilation
  }

  {
    FinalResourceMark rm;
    EnableDeoptimization ed; // Wrapper that enables canonicalization when deoptimizing.

    DeltaProcess::deoptimize_redo_last_send();
    process->deoptimize_stretch(&f, &f);
    if (!nm->is_method()) {
      // This is a top level block nmethod so we have to make sure all frames on the stack
      // referring the context chain are deoptimized.
      // Frames on other stacks might be candidates for deoptimization but are ignored for now.  fix this later
      //
      // Recipe:
      //   walk the stack and collect a work list of {frame, compiled_context} pairs.
      //   iterate over the work list until no deoptimized contextOops are present.
      GrowableArray<FrameAndContextElement*>* elements  = new GrowableArray<FrameAndContextElement*>(10);
      for (frame current_frame = f.sender(); !current_frame.is_first_frame(); current_frame = current_frame.sender()) {
        collect_compiled_contexts_for(&current_frame, elements);
      }
      
      bool done = false;

      while (!done) {
        done = true;      
        for (int index = 0; index < elements->length() && done; index++) {
          FrameAndContextElement* e = elements->at(index);
	  if (e && e->con->unoptimized_context()) {
	    process->deoptimize_stretch(&e->fr, &e->fr);

            for (int j = 0; j < elements->length(); j++) {
              if (elements->at(j) && elements->at(j)->fr.fp() == e->fr.fp())
	        elements->at_put(j, NULL);
	    }
            done = false;
	  }
        } 
      }
    }
  }
  process->exit_uncommon();
}

#endif
