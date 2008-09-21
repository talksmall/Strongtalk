/* Copyright 1996, LongView Technologies L.L.C. $Revision: 1.2 $ */
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

// Keeps the sliding average of where the system spends the time
class SlidingSystemAverage : AllStatic {
 public:

  enum {
    // initial value
    nowhere             =  0,

    // ticks without last_delta_fp set 
    in_compiled_code    =  1, 
    in_interpreted_code =  2,
    in_pic_code         =  3,
    in_stub_code        =  4,

    // ticks with last_delta_fp set
    in_compiler         =  5,
    in_garbage_collect  =  6, // sacvenge or GC
    is_idle             =  7, // hanging in primitiveProcessSchedulerWait:
    in_vm               =  8, // somewhere else in the vm (primitive / dll)
    number_of_cases     =  9,

    buffer_size         = 256
  };


  // Resets the buffer
  static void reset();

  // Add tick to buffer
  static void add(char type);

  // Returns the update statistics array
  static unsigned int* update();

 private:
  static char         buffer[buffer_size];    // Buffer for the ticks
  static unsigned int stat[number_of_cases];  // Results from buffer
  static unsigned int pos;                    // Current pos in buffer
};
