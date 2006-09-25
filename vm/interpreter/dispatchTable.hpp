/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.11 $ */
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

// The dispatchTable controls the dispatch of byte codes

class dispatchTable: AllStatic {
 private:
  enum Mode {
    normal_mode = 0,
    step_mode   = 1,
    next_mode   = 2,
    return_mode = 3
  };

  static Mode mode;
  static void patch_with_sst_stub();

 public:
  // the dispatch table
  static u_char** table();

  // initializes the dispatch table to the original state.
  static void reset();

  // intercepts all relevant entries to enable single step.
  static void intercept_for_step();
  static void intercept_for_next(int* fr);
  static void intercept_for_return(int* fr);

  // answers whether the dispatch table is in single step mode.
  static bool in_normal_mode()  { return mode == normal_mode; }
  static bool in_step_mode()    { return mode == step_mode; }
  static bool in_next_mode()    { return mode == next_mode; }
  static bool in_return_mode()  { return mode == return_mode; }
};
