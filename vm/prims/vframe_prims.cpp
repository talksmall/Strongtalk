/* Copyright 1996 LongView Technologies L.L.C. $Revision: 1.3 $ */
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
# include "incls/_vframe_prims.cpp.incl"

TRACE_FUNC(TraceVframePrims, "vframe")

int vframeOopPrimitives::number_of_calls;

#define ASSERT_RECEIVER assert(receiver->is_vframe(), "receiver must be vframe")

PRIM_DECL_1(vframeOopPrimitives::process, oop receiver) {
  PROLOGUE_1("process", receiver);
  ASSERT_RECEIVER;
  return vframeOop(receiver)->process();
}

PRIM_DECL_1(vframeOopPrimitives::index, oop receiver) {
  PROLOGUE_1("index", receiver);
  ASSERT_RECEIVER;
  return as_smiOop(vframeOop(receiver)->index());
}

PRIM_DECL_1(vframeOopPrimitives::time_stamp, oop receiver) {
  PROLOGUE_1("time_stamp", receiver);
  ASSERT_RECEIVER;
  return as_smiOop(vframeOop(receiver)->time_stamp());
}


PRIM_DECL_1(vframeOopPrimitives::is_smalltalk_activation, oop receiver) {
  PROLOGUE_1("is_smalltalk_activation", receiver);
  ASSERT_RECEIVER;

  ResourceMark rm;

  vframe* vf = vframeOop(receiver)->get_vframe();

  if (vf == NULL)
    return markSymbol(vmSymbols::activation_is_invalid());
    
  return vf->is_delta_frame() ? trueObj : falseObj;
}

PRIM_DECL_1(vframeOopPrimitives::byte_code_index, oop receiver) {
  PROLOGUE_1("byte_code_index", receiver);
  ASSERT_RECEIVER;

  ResourceMark rm;

  vframe* vf = vframeOop(receiver)->get_vframe();

  if (vf == NULL)
    return markSymbol(vmSymbols::activation_is_invalid());
    
  if (!vf->is_delta_frame())
    return markSymbol(vmSymbols::external_activation());
  
  return as_smiOop(((deltaVFrame*) vf)->bci());
}

PRIM_DECL_1(vframeOopPrimitives::expression_stack, oop receiver) {
  PROLOGUE_1("expression_stack", receiver);
  ASSERT_RECEIVER;

  ResourceMark rm;

  vframe* vf = vframeOop(receiver)->get_vframe();

  if (vf == NULL)
    return markSymbol(vmSymbols::activation_is_invalid());
    
  if (!vf->is_delta_frame())
    return markSymbol(vmSymbols::external_activation());

  BlockScavenge bs;

  GrowableArray<oop>* stack = ((deltaVFrame*) vf)->expression_stack();

  return oopFactory::new_objArray(stack);
}

PRIM_DECL_1(vframeOopPrimitives::method, oop receiver) {
  PROLOGUE_1("method", receiver);
  ASSERT_RECEIVER;

  ResourceMark rm;

  vframe* vf = vframeOop(receiver)->get_vframe();

  if (vf == NULL)
    return markSymbol(vmSymbols::activation_is_invalid());
    
  if (!vf->is_delta_frame())
    return markSymbol(vmSymbols::external_activation());

  return ((deltaVFrame*) vf)->method();
}

PRIM_DECL_1(vframeOopPrimitives::receiver, oop recv) {
  PROLOGUE_1("receiver", recv);

  assert(recv->is_vframe(), "receiver must be vframe")

  ResourceMark rm;

  vframe* vf = vframeOop(recv)->get_vframe();

  if (vf == NULL)
    return markSymbol(vmSymbols::activation_is_invalid());
    
  if (!vf->is_delta_frame())
    return markSymbol(vmSymbols::external_activation());

  return ((deltaVFrame*) vf)->receiver();
}

PRIM_DECL_1(vframeOopPrimitives::temporaries, oop receiver) {
  PROLOGUE_1("temporaries", receiver);
  ASSERT_RECEIVER;

  return receiver;
}

PRIM_DECL_1(vframeOopPrimitives::arguments, oop receiver) {
  PROLOGUE_1("arguments", receiver);
  ASSERT_RECEIVER;

  ResourceMark rm;

  vframe* vf = vframeOop(receiver)->get_vframe();

  if (vf == NULL)
    return markSymbol(vmSymbols::activation_is_invalid());
    
  if (!vf->is_delta_frame())
    return markSymbol(vmSymbols::external_activation());

  BlockScavenge bs;

  GrowableArray<oop>* stack = ((deltaVFrame*) vf)->arguments();

  return oopFactory::new_objArray(stack);
}

class vframeStream: public byteArrayPrettyPrintStream {
  void begin_highlight() { set_highlight(true); print_char(27); }
  void end_highlight()   { set_highlight(false); print_char(27); }
};

PRIM_DECL_1(vframeOopPrimitives::pretty_print, oop receiver) {
  PROLOGUE_1("receiver", receiver);
  ASSERT_RECEIVER;

  ResourceMark rm;
  BlockScavenge bs;

  vframe* vf = vframeOop(receiver)->get_vframe();

  if (vf == NULL)
    return markSymbol(vmSymbols::activation_is_invalid());

  if (!vf->is_delta_frame())
    return markSymbol(vmSymbols::external_activation());

  byteArrayPrettyPrintStream* stream = new vframeStream;
  prettyPrinter::print_body((deltaVFrame*) vf, stream);

  return stream->asByteArray();
}
