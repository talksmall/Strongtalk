/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.73 $ */
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
# include "incls/_frame.cpp.incl"

u_char* frame::hp() const { 
  // Lars, please check -- assertion fails
  //assert(is_interpreted_frame(), "must be interpreted");
  return *hp_addr(); 
}

void  frame::set_hp(u_char* hp) { 
  //assert(is_interpreted_frame(), "must be interpreted");
  *hp_addr() = hp; 
}

void frame::patch_pc(char* pc) {
  char** pc_addr = (char**) sp() - 1;
  *pc_addr = pc;
}

objArrayOop* frame::frame_array_addr() const {
  assert(frame_size() >= minimum_size_for_deoptimized_frame, "Compiler frame is too small for deoptimization");
  //assert(is_deoptimized_frame(), "must be deoptimized frame");
  return (objArrayOop*) addr_at(frame_frame_array_offset);
}

objArrayOop frame::frame_array() const {
  objArrayOop result = *frame_array_addr();
  assert(result->is_objArray(), "must be objArray");
  return result;
}

oop** frame::real_sender_sp_addr() const {
  //assert(is_deoptimized_frame(), "must be deoptimized frame");
  return (oop**) addr_at(frame_real_sender_sp_offset);
}

void frame::patch_fp(int* fp) {
  frame previous(NULL, ((int*) sp()) - frame_sender_sp_offset, NULL);
  previous.set_link(fp);
}

methodOop frame::method() const {
  assert(is_interpreted_frame(), "must be interpreter frame");
  // First we will check the interpreter frame is valid by checking the frame size.
  // The interpreter guarantees hp is valid if the frame is at least 4 in size. 
  // (return address, link, receiver, hcode pointer)
  if (frame_size() < minimum_size_for_deoptimized_frame) return NULL;

  u_char* h = hp();
  if (!Universe::old_gen.contains(h)) return NULL; 
  memOop obj = as_memOop(Universe::object_start((oop*) h));  
  return obj->is_method() ? methodOop(obj) : NULL;
}

nmethod* frame::code() const {
  assert(is_compiled_frame(), "no code");
  return findNMethod(pc());
}

bool frame::is_interpreted_frame() const  {
  return Interpreter::contains(pc());
}

bool frame::is_compiled_frame() const {
#ifdef DELTA_COMPILER
  return Universe::code->contains(pc());
#else
  return false;
#endif
}

extern "C" void unpack_unoptimized_frames();

bool frame::is_deoptimized_frame()  const {
  return pc() == (char*) &unpack_unoptimized_frames;
}

IC_Iterator* frame::sender_ic_iterator() const {
  return is_entry_frame() ?  NULL : sender().current_ic_iterator();
}

IC_Iterator* frame::current_ic_iterator() const {

  if (is_interpreted_frame()) {
    InterpretedIC* ic = current_interpretedIC();
    if (ic && !Bytecodes::is_send_code(ic->send_code())) return NULL;
    return ic ? new InterpretedIC_Iterator(ic) : NULL;
  }

  if (is_compiled_frame()) {
    CompiledIC* ic = current_compiledIC();
    return  ic->inlineCache() 
          ? new CompiledIC_Iterator(ic)
	  : NULL; // a perform, not a send
  }

  // entry or deoptimized frame
  return NULL;
}

InterpretedIC* frame::current_interpretedIC() const {

  if (is_interpreted_frame()) {
    methodOop m = method();
    int bci = m->bci_from(hp());
    u_char* codeptr = m->codes(bci);
    if (Bytecodes::is_send_code(Bytecodes::Code(*codeptr))) {
      InterpretedIC* ic = as_InterpretedIC((char*)hp());
      assert(ic->send_code_addr() == codeptr, "found wrong ic");
      return ic;
    } else {
      return NULL;	  // perform, dll call, etc.
    }
  }

  return NULL;	  // doesn't have InterpretedIC
}

CompiledIC* frame::current_compiledIC() const {
  return is_compiled_frame()
       ? CompiledIC_from_return_addr(pc())  // may fail if current frame isn't at a send -- caller must know
       : NULL;
}

extern "C" void return_from_Delta();

bool frame::is_entry_frame() const {
  return pc() == (char*) &return_from_Delta;
}

bool frame::has_next_Delta_fp() const {
  return at(frame_next_Delta_fp_offset) != NULL;
}

int* frame::next_Delta_fp() const	{
  return (int*) at(frame_next_Delta_fp_offset);
}

oop* frame::next_Delta_sp() const	{
  return (oop*) at(frame_next_Delta_sp_offset);
}

bool frame::is_first_frame() const {
  return is_entry_frame() && !has_next_Delta_fp();
}

bool frame::is_first_delta_frame() const {
  // last Delta frame isn't necessarily is_first_frame(), so check is a bit more complicated
  // [I don't understand why, but the first Delta frame of a process isn't an entry frame  -Urs 2/96]
  frame s;
  for (s = sender(); !(s.is_delta_frame() || s.is_first_frame()); s = s.sender()) ;
  return s.is_first_frame();
}

char* frame::print_name() const {
  if (is_interpreted_frame()) return "interpreted";
  if (is_compiled_frame())    return "compiled";
  if (is_deoptimized_frame()) return "deoptimized";
  return "C";
}

void frame::print() const {
  std->print("[%s frame: fp = %#lx, sp = %#lx, pc = %#lx", print_name(), fp(), sp(), pc());
  if (is_compiled_frame()) {
    std->print(", nm = %#x", findNMethod(pc()));
  } else if (is_interpreted_frame()) {
    std->print(", hp = %#x, method = %#x", hp(), method());
  }
  std->print_cr("]");

  if (PrintLongFrames) {
    for (oop* p = sp(); p < (oop*)fp(); p++)
      std->print_cr("  - 0x%lx: 0x%lx", p, *p);
  }
}


static void print_context_chain(contextOop con, outputStream* st) {
  if (con) {
    // Print out the contexts chain
    st->print("    context ");
    con->print_value_on(st);
    while (con->has_outer_context()) {
      con = con->outer_context();
      st->print(" -> ");
      con->print_value_on(st);
    }
    st->cr();
  }
}

void frame::print_for_deoptimization(outputStream* st) {
 ResourceMark rm;
 st->print(" - ");
 if (is_interpreted_frame()) {
   st->print("I ");
   interpretedVFrame* vf = (interpretedVFrame*) vframe::new_vframe(this);
   vf->method()->print_value_on(st);
   if (ActivationShowBCI) {
     st->print(" bci=%d ", vf->bci());
   }
   std->print_cr(" @ 0x%lx", fp());
   print_context_chain(vf->interpreter_context(), st);
   if (ActivationShowExpressionStack) {
     GrowableArray<oop>* stack = vf->expression_stack();
     for (int index = 0; index < stack->length(); index++) {
       st->print("    %3d: ", index);
       stack->at(index)->print_value_on(st);
       st->cr();
     }
   }
   return;
 } 

 if (is_compiled_frame()) {
   st->print("C ");
   compiledVFrame* vf = (compiledVFrame*) vframe::new_vframe(this);
   assert(vf->is_compiled_frame(), "should be compiled vframe");
   vf->code()->print_value_on(st);
   std->print_cr(" @ 0x%lx", fp());

   while (true) {
     st->print("    ");
     vf->method()->print_value_on(st);
     std->print_cr(" @ %d", vf->scope()->offset());
     print_context_chain(vf->compiled_context(), st);
     if (vf->is_top()) break;
     vf = (compiledVFrame*) vf->sender();
     assert(vf->is_compiled_frame(), "should be compiled vframe");
   }
   return;
 }

 if (is_deoptimized_frame()) {
   st->print("D "); 
   frame_array()->print_value();
   std->print_cr(" @ 0x%lx", fp());

   deoptimizedVFrame* vf = (deoptimizedVFrame*) vframe::new_vframe(this);
   assert(vf->is_deoptimized_frame(), "should be deoptimized vframe");
   while (true) {
     st->print("    ");
     vf->method()->print_value_on(st);
     std->cr();
     print_context_chain(vf->deoptimized_context(), st);
     if (vf->is_top()) break;
     vf = (deoptimizedVFrame*) vf->sender();
     assert(vf->is_deoptimized_frame(), "should be deoptimized vframe");
   }
   return;
 }

 st->print("E foreign frame @ 0x%lx", fp());
}

void frame::layout_iterate(FrameLayoutClosure* blk) {
  if (is_interpreted_frame()){ 
    oop* eos = temp_addr(0);
    for (oop* p = sp(); p <= eos; p++) 
      blk->do_stack(eos-p, p);
    blk->do_hp(hp_addr());
    blk->do_receiver(receiver_addr());
    blk->do_link(link_addr());
    blk->do_return_addr(return_addr_addr());
  }
}

bool frame::has_interpreted_float_marker() const {
  return oop(at(interpreted_frame_float_magic_offset)) == Floats::magic_value();
}

bool frame::has_compiled_float_marker() const {
  return oop(at(compiled_frame_magic_oop_offset)) == Floats::magic_value();
}

bool frame::oop_iterate_interpreted_float_frame(OopClosure* blk) {
  methodOop m =  methodOopDesc::methodOop_from_hcode(hp());
  // Return if this activation has no floats (the marker is conservative)
  if (!m->has_float_temporaries()) return false;
        
  // Iterator from stack pointer to end of float section
  oop* end = (oop*) addr_at(m->float_section_start_offset() - m->float_section_size());
  for (oop* p = sp(); p <= end; p++) {
    blk->do_oop(p);
  }

  // Skip the float section and magic_value

  // Iterate from just before the float section to the first temp
  for (oop* q = (oop*) addr_at(m->float_section_start_offset() + 2); q <= temp_addr(0); q++) {
    blk->do_oop(q);
  }

  // The receiver
  blk->do_oop(receiver_addr());

  return true;
}

bool frame::oop_iterate_compiled_float_frame(OopClosure* blk) {
  warning("oop_iterate_compiled_float_frame not implemented");
  return false;
}

void frame::oop_iterate(OopClosure* blk) {
  if (is_interpreted_frame()) {
    if (has_interpreted_float_marker() && oop_iterate_interpreted_float_frame(blk)) return;
 
    // lprintf("Frame: fp = %#lx, sp = %#lx]\n", fp(), sp());
    for (oop* p = sp(); p <= temp_addr(0); p++) {
      // lprintf("\t[%#lx]: ", p);
      // (*p)->short_print();
      // lprintf("\n");
      blk->do_oop(p);
    }
    // lprintf("\t{%#lx}: ", receiver_addr());
    // (*receiver_addr())->short_print();
    // lprintf("\n");
    blk->do_oop(receiver_addr());
    return;
  }
  
  if (is_compiled_frame()) {
    if (has_compiled_float_marker() && oop_iterate_compiled_float_frame(blk)) return;

     // All oops are [sp..fp[
    for (oop* p = sp(); p < (oop*)fp(); p++) {
      blk->do_oop(p);
    }
    return;
  }

  if (is_entry_frame()) {
    // All oops are [sp..fp[
    for (oop* p = sp(); p < (oop*)fp(); p++) {
      blk->do_oop(p);
    }
    return;
  }
  
  if (is_deoptimized_frame()) {
    // Expression stack
    oop* end = (oop*)fp() + frame_real_sender_sp_offset;
    // All oops are [sp..end[
    for (oop* p = sp(); p < end; p++) {
      blk->do_oop(p);
    }
    blk->do_oop((oop*)frame_array_addr());
    return;
  }
}

bool frame::follow_roots_interpreted_float_frame() {
  methodOop m = methodOop(hp());
  assert(m->is_method(), "must be method");
  // Return if this activation has no floats (the marker is conservative)
  if (!m->has_float_temporaries()) return false;

  // Iterator from stack pointer to end of float section
  oop* end = (oop*) addr_at(m->float_section_start_offset() - m->float_section_size());
  for (oop* p = sp(); p <= end; p++) {
    MarkSweep::follow_root(p);
  }

  // Skip the float section and magic_value

  // Iterate from just before the float section to the first temp
  for (oop* q = (oop*) addr_at(m->float_section_start_offset() + 2); q <= temp_addr(0); q++) {
    MarkSweep::follow_root(q);
  }

  // The receiver
  MarkSweep::follow_root(receiver_addr());

  return true;
}

bool frame::follow_roots_compiled_float_frame() {
  warning("follow_roots_compiled_float_frame not implemented");
  return true;
}

void frame::follow_roots() {
  if (is_interpreted_frame()) {
    if (has_interpreted_float_marker() && follow_roots_interpreted_float_frame()) return;

    // Follow the roots of the frame
    for (oop* p = sp(); p <= temp_addr(0); p++) {
      MarkSweep::follow_root(p);
    }
    MarkSweep::follow_root((oop*)hp_addr());
    MarkSweep::follow_root(receiver_addr());
    return;
  } 
  
  if (is_compiled_frame()) {
    if (has_compiled_float_marker() && follow_roots_compiled_float_frame()) return;

    for (oop* p = sp(); p < (oop*)fp(); p++) MarkSweep::follow_root(p);
    return;
  }
    
  if (is_entry_frame()) {
    for (oop* p = sp(); p < (oop*)fp(); p++) MarkSweep::follow_root(p);
    return;
  }

  if (is_deoptimized_frame()) {
    // Expression stack
    oop* end = (oop*)fp() + frame_real_sender_sp_offset;
    for (oop* p = sp(); p < end; p++) MarkSweep::follow_root(p);
    MarkSweep::follow_root((oop*)frame_array_addr());
    return;
  }
}

void frame::convert_hcode_pointer() {
  if (!is_interpreted_frame()) return;
  // Adjust hcode pointer to object start
  u_char* h   = hp();
  u_char* obj = (u_char*) as_memOop(Universe::object_start((oop*) h));
  set_hp(obj);
  // Save the offset
  MarkSweep::add_hcode_offset(h - obj);
  // if (WizardMode) lprintf("[0x%lx+%d]\n", obj, h - obj);
}

void frame::restore_hcode_pointer() {
  if (!is_interpreted_frame()) return;
  // Readjust hcode pointer
  u_char* obj  = hp();
  int   offset = MarkSweep::next_hcode_offset();
  // if (WizardMode) lprintf("[0x%lx+%d]\n", obj, offset);
  set_hp(obj + offset);
}

class VerifyOopClosure : public OopClosure {
 public:
  frame* fr;
  void do_oop(oop* o) {
    oop obj = *o;
    if (!obj->verify()) {
      lprintf("Verify failed in frame:\n");
      fr->print();
    }
  }
};

void frame::verify() const {
  if (fp() == NULL) fatal("fp cannot be NULL");
  if (sp() == NULL) fatal("fp cannot be NULL");
  VerifyOopClosure blk;
  blk.fr = (frame*) this;
  ((frame*) this)->oop_iterate(&blk);
}

frame frame::sender() const {
  frame result;
  if (is_entry_frame()) {
    // Delta frame called from C; skip all C frames and return top C
    // frame of that chunk as the sender
    assert(has_next_Delta_fp(), "next Delta fp must be non zero");
    assert(next_Delta_fp() > _fp, "must be above this frame on stack");
    result = frame(next_Delta_sp(), next_Delta_fp());
  } else if (is_deoptimized_frame()) {
     result = frame(real_sender_sp(), link(), return_addr());
  } else {
    result = frame(sender_sp(), link(), return_addr());
  }
  return result;
}

frame frame::delta_sender() const {
  for (frame s = sender(); !s.is_delta_frame(); s = s.sender()) ;
  return s;
}

bool frame::should_be_deoptimized() const {
  if (!is_compiled_frame()) return false;
  nmethod* nm = code();
  if (TraceApplyChange) {
    std->print("checking (%s) ", nm->is_marked_for_deoptimization() ? "true" : "false"); 
    nm->print_value_on(std);
    std->cr();
  }
  return nm->is_marked_for_deoptimization();
}
