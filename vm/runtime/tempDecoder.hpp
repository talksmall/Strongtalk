/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.8 $ */
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

// TempDecoder decodes the method annotation describing
// the names of parameters and temporaries.

class TempDecoder {
 private:
   int num_of_params;
   bool is_heap_parameter(byteArrayOop name, objArrayOop tempInfo);
 public:
  virtual void decode(methodOop method, int bci = 0);

  // arguments are numbered from 1 to n
  virtual void parameter        (byteArrayOop name, int index) {}
  virtual void stack_temp       (byteArrayOop name, int no)    {}
  virtual void stack_float_temp (byteArrayOop name, int fno)   {}
  virtual void heap_temp        (byteArrayOop name, int no)    {}
  virtual void heap_parameter   (byteArrayOop name, int no)    {}

  virtual void no_debug_info() {}      
};

class TempPrinter : public TempDecoder {
 private:
   
 public:
  void decode(methodOop method, int bci = 0);

  // arguments are numbered from 1 to n
  void parameter        (byteArrayOop name, int index);
  void stack_temp       (byteArrayOop name, int no);
  void stack_float_temp (byteArrayOop name, int fno);
  void heap_temp        (byteArrayOop name, int no);
  void heap_parameter   (byteArrayOop name, int no);

  void no_debug_info();
};

// Returns the name of parameter number if found, NULL otherwise.
byteArrayOop find_parameter_name(methodOop method, int no);

// Returns the name of temp at offset if found, NULL otherwise.
byteArrayOop find_stack_temp(methodOop method, int bci, int no);
byteArrayOop find_heap_temp(methodOop method, int bci, int no);
byteArrayOop find_stack_float_temp(methodOop method, int bci, int fno);
