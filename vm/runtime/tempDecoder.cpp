/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.18 $ */
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
# include "incls/_tempDecoder.cpp.incl"

#define NEXT                        \
  pos++;                            \
  if (pos > len) return;            \
  current = tempInfo->obj_at(pos)

void TempDecoder::decode(methodOop method, int bci) {
  // Format:
  //   name*                     parameters
  //   offset name*              stack-allocated temporaries
  //   offset name*              stack-allocated floats (offset is 0)
  //   offset name*              heap-allocated temporaries
  //   (begin end offset name+)* stack-allocated temporaries of inlined scopes

  objArrayOop tempInfo = method->tempInfo();
  if (tempInfo == NULL) {
    no_debug_info();
    return;
  }
  int len  = tempInfo->length();
  if (len == 0) return;

  int pos     = 1;
  oop current = tempInfo->obj_at(pos);

  { // scan parameters
    num_of_params = 0;
    while (current->is_byteArray()) {
      parameter(byteArrayOop(current), num_of_params++);
      NEXT;
    }
  }

  { // scan global stack temps
    assert_smi(current, "expecting smi");
    int offset = smiOop(current)->value();
    NEXT;
    while (current->is_byteArray()) {
      stack_temp(byteArrayOop(current), offset++);
      NEXT;
    }
  }

  { // scan global stack float temps
    assert_smi(current, "expecting smi");
    assert(smiOop(current)->value() == 0, "should be zero")
    int fno = smiOop(current)->value();
    NEXT;
    while (current->is_byteArray()) {
      stack_float_temp(byteArrayOop(current), fno++);
      NEXT;
    }
  }

  { // scan global heap temps
    assert_smi(current, "expecting smi");
    int offset = smiOop(current)->value();
    NEXT;
    while (current->is_byteArray()) {
      if (is_heap_parameter(byteArrayOop(current), tempInfo)) {
        heap_parameter(byteArrayOop(current), offset++);
      } else {
        heap_temp(byteArrayOop(current), offset++);
      }
      NEXT;
    } 
  }
  { // scan inlined temps
    while(1) {
      assert_smi(current, "expecting smi");
      int begin = smiOop(current)->value();
      NEXT;
      assert_smi(current, "expecting smi");
      int end = smiOop(current)->value();
      NEXT;
      // Oop temps
      assert_smi(current, "expecting smi");
      int offset = smiOop(current)->value();
      NEXT;
      while (current->is_byteArray()) {
        if ((begin <= bci) && (bci <= end)) {
          stack_temp(byteArrayOop(current), offset);
	}
	offset++;
        NEXT;
      }
      // Floats
      assert_smi(current, "expecting smi");
      offset = smiOop(current)->value();
      NEXT;
      while (current->is_byteArray()) {
        if ((begin <= bci) && (bci <= end)) {
          stack_float_temp(byteArrayOop(current), offset);
	}
	offset++;
        NEXT;
      }
    }
  }
}

bool TempDecoder::is_heap_parameter(byteArrayOop name, objArrayOop tempInfo) {
  assert(name->is_symbol(), "Must be symbol");
  for (int index = 1; index <= num_of_params; index++) {
    byteArrayOop par = byteArrayOop(tempInfo->obj_at(index));
     assert(par->is_symbol(), "Must be symbol");
    if (name == par) return true;
  }
  return false;
}

void TempPrinter::decode(methodOop method, int bci) {
  std->print_cr("TempDecoding:");
  TempDecoder::decode(method, bci);
}

void TempPrinter::parameter(byteArrayOop name, int index) {
  std->print_cr("  param:      %s@%d", name->as_string(), index);
}

void TempPrinter::stack_temp(byteArrayOop name, int no) {
  std->print_cr("  stack temp: %s@%d", name->as_string(), no);
}

void TempPrinter::stack_float_temp(byteArrayOop name, int fno) {
  std->print_cr("  stack float temp: %s@%d", name->as_string(), fno);
}

void TempPrinter::heap_temp(byteArrayOop name, int no){
  std->print_cr("  heap temp:  %s@%d", name->as_string(), no);
}

void TempPrinter::heap_parameter(byteArrayOop name, int no){
  std->print_cr("  heap param:  %s@%d", name->as_string(), no);
}

void TempPrinter::no_debug_info() {
  std->print_cr("method has no debug information");
}


class FindParam : public TempDecoder {
 private:
   int the_no;
 public:
  byteArrayOop result;

  void find(methodOop method, int no) {
    result = NULL;
    the_no = no;
    decode(method, 0);
  }
  void parameter(byteArrayOop name, int no) {
    if (the_no == no) result = name;
  }
};

byteArrayOop find_parameter_name(methodOop method, int no) {
  FindParam p;
  p.find(method, no);
  return p.result;
}

class FindStackTemp : public TempDecoder {
 private:
   int the_no;
 public:
  byteArrayOop result;

  void find(methodOop method, int bci, int no) {
    result = NULL;
    the_no = no;
    TempDecoder::decode(method, bci);
  }
  void stack_temp(byteArrayOop name, int no) { 
    if (the_no == no) result = name;
  }
};

class FindStackFloatTemp : public TempDecoder {
 private:
   int the_fno;
 public:
  byteArrayOop result;

  void find(methodOop method, int bci, int fno) {
    result = NULL;
    the_fno = fno;
    TempDecoder::decode(method, bci);
  }
  void stack_float_temp(byteArrayOop name, int fno) { 
    if (the_fno == fno) result = name;
  }
};

class FindHeapTemp : public TempDecoder {
 private:
   int the_no;
 public:
  byteArrayOop result;

  void find(methodOop method, int bci, int no) {
    result = NULL;
    the_no = no;
    TempDecoder::decode(method, bci);
  }
  void heap_temp(byteArrayOop name, int no) {
    if (the_no == no) result = name;
  }
  void heap_parameter(byteArrayOop name, int no) {
    if (the_no == no) result = name;
  }

};

byteArrayOop find_stack_temp(methodOop method, int bci, int no) {
  FindStackTemp p;
  p.find(method, bci, no);
  return p.result;
}

byteArrayOop find_heap_temp(methodOop method, int bci, int no) {
  FindHeapTemp p;
  p.find(method, bci, no);
  return p.result;
}

byteArrayOop find_stack_float_temp(methodOop method, int bci, int fno) {
  FindStackFloatTemp p;
  p.find(method, bci, fno);
  return p.result;
}