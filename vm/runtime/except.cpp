#include "incls\_precompiled.incl"

void trace(vframe* from_frame, int start_frame, int number_of_frames){
  FlagSetting fs(ActivationShowCode, true);

  std->print_cr("- Stack trace (%d, %d)", start_frame, number_of_frames);
  int  vframe_no = 1;

  for (vframe* f = from_frame; f; f = f->sender() ) {
    if (vframe_no >= start_frame) {
      if (f->is_delta_frame()) {
        ((deltaVFrame*) f)->print_activation(vframe_no);
      } else f->print();
      if (vframe_no - start_frame + 1 >= number_of_frames) return;
    }
    vframe_no++;
  }
}

void handle_exception(void* fp, void* sp, void* pc) {
  frame f((oop*)sp, (int*)fp, (char*)pc);
  lprintf("ebp: 0x%x, esp: 0x%x, pc: 0x%x\n", fp, sp, pc);
  if (f.is_compiled_frame()) {
    ResourceMark mark;

    // Find the nmethod containing the pc
    compiledVFrame* vf = (compiledVFrame*) vframe::new_vframe(&f);
    assert(vf->is_compiled_frame(), "must be compiled frame");
    nmethod* nm = vf->code();
    lprintf("Found nmethod: 0x%x\n", nm);
    nm->print_value_on(std);

    std->print("\n @%d called from %#x",
               vf->scope()->offset(),
               f.pc() - Assembler::sizeOfCall);
    std->cr();
    
    trace(vf, 0, 10);
  } else if (f.is_interpreted_frame()) {
    ResourceMark mark;
    vframe *vf = vframe::new_vframe(&f);

    trace(vf, 0, 10);
  } else {
    lprintf("Not in compiled code.");
  }
}

/*
  Set up a new exception handler to catch unhandled exceptions BEFORE the debugger.
  This is to allow the dumping of ST-level info about the exception location to aid
  in debugging of the exception.
 */
void except_init() {
  os::add_exception_handler(handle_exception);
}