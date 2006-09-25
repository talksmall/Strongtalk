/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.17 $ */
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
# include "incls/_stackChunk.cpp.incl"

GrowableArray<const compiledVFrame*>*  frames;
GrowableArray<contextOop>*       contexts;

bool StackChunkBuilder::_is_deoptimizing = false;
int* StackChunkBuilder::frame_pointer    = NULL;

StackChunkBuilder::StackChunkBuilder(int* fp, int size) {
  number_of_vframes = 0;
  number_of_locals  = 0;
  array             = new GrowableArray<oop>(size);
  frame_pointer     = fp;
}

StackChunkBuilder::~StackChunkBuilder() {
}

void StackChunkBuilder::append(deltaVFrame* f) {
  number_of_vframes++;

  // Append the frame information to the array
  methodOop method = f->method();
  array->push(f->receiver());
  array->push(f->method());
  array->push(as_smiOop(f->bci()));
 
  // push locals
  int number_of_temps = f->method()->number_of_stack_temporaries();
  GrowableArray<oop>* stack = f->expression_stack();

  // push number of locals
  int locals = number_of_temps + stack->length();
  array->push(as_smiOop(locals));
  number_of_locals += locals;

  // push context and temporaries
  // if a context is present store the canoniocal form as temporary 0.
  contextOop con = f->canonical_context();
  if (con) {
    array->push(con);
#ifdef ASSERT
    method->verify_context(con);
#endif

    // If we have pending nlrs (suspended in unwind protect) we have to update
    // the nlr targets if we match deoptimized frames
    if (!method->is_blockMethod()) {
      con->set_home_fp(frame_pointer);
      if (f->is_compiled_frame()) {
        Processes::update_nlr_targets((compiledVFrame*) f, con);
      }
    }
  }
  for (int index = con ? 1 : 0; index < number_of_temps; index++) 
    array->push(f->temp_at(index));
 
  // push expression stack
  for (index = stack->length() - 1; index >= 0; index--) {
    array->push(stack->at(index));
  }
}

objArrayOop StackChunkBuilder::as_objArray() {
  int length = header_size() + array->length();
  objArrayOop result = oopFactory::new_objArray(length);
  result->obj_at_put(1, as_smiOop(number_of_vframes));
  result->obj_at_put(2, as_smiOop(number_of_locals));
  for (int index = 0; index < array->length(); index++) 
    result->obj_at_put(index + header_size() + 1, array->at(index));
  return result;
}

void StackChunkBuilder::context_at_put(const compiledVFrame* frame, contextOop con) {
  // Returns if no StackChunkBuilder is in use
  if (!is_deoptimizing()) {
    con->kill(); 
    return;
  }

#ifdef ASSERT
  for (int index = 0; index < frames->length(); index++) {
    assert(!frames->at(index)->equal(frame), "should not be present");
  }
#endif
  con->set_home_fp(frame_pointer);
  frames->push(frame);
  contexts->push(con);
}

contextOop StackChunkBuilder::context_at(const compiledVFrame* frame) {
  // Returns if no StackChunkBuilder is in use
  if (!is_deoptimizing()) return NULL;

  // See if it's stored
  for (int index = 0; index < frames->length(); index++) {
    if (frames->at(index)->equal(frame)) {
      return contexts->at(index);
    }
  }
  return NULL;
}

void StackChunkBuilder::begin_deoptimization() {
  assert(!is_deoptimizing(), "just checking");
  _is_deoptimizing  = true;
  frames            = new GrowableArray<const compiledVFrame*>(100);
  contexts          = new GrowableArray<contextOop>(100);
}

void StackChunkBuilder::end_deoptimization() {
  assert(is_deoptimizing(), "just checking");
  _is_deoptimizing  = false;
  frames   = NULL;
  contexts = NULL;

}

