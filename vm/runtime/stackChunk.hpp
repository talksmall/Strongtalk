/* Copyright 1994, 1995, 1996 LongView Technologies L.L.C. $Revision: 1.6 $ */
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


// The StackChunkBuilder helps generate the canonical form
// of vframes.
// Used for: 
//  - stack deoptimization.
//  - save stacks in heap.

class StackChunkBuilder: public ResourceObj {
 private:
  // These numbers makes it possible to compute the
  // size of the corresponding deoptimized interpreter stack.
  int number_of_vframes;    // Number of vframes collected
  int number_of_locals;     // Sum of all temporaries and expressions in colleced vframes
  static int* frame_pointer;// Frame pointer of the resulting frame

  static int header_size() { return 2; }
  static bool _is_deoptimizing;

  GrowableArray<oop>* array;
 public:
  // Constructor
  StackChunkBuilder(int* fp, int size = 100);
  ~StackChunkBuilder();

  // Adds a frame
  void append(deltaVFrame* f);

  // Returns the packed frames as an object array.
  objArrayOop as_objArray();

  // Constants for the resulting object array
  enum {
    number_of_vframes_index = 1,
    number_of_locals_index  = 2,
    first_frame_index       = 3
  };

  static void begin_deoptimization();
  static void end_deoptimization();

  static bool is_deoptimizing() { return _is_deoptimizing; }

  // The context cache
  static void       context_at_put(const compiledVFrame* frame, contextOop con);
  static contextOop context_at(const compiledVFrame* frame);
};
