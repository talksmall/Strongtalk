/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.4 */
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

// ReservedSpace is a data strucure for reserving a contiguous chunk of memory.

class ReservedSpace : public ValueObj {
 private:
  char* _base;
  int   _size;
 public:
  ReservedSpace(int size);
  ReservedSpace(char* base, int size) {
    _base = base;
    _size = size;
  }

  // Accessors
  char* base() { return _base; }
  int   size() { return _size; }

  bool is_reserved() { return _base != NULL; }

  // Splitting
  ReservedSpace first_part(int partition_size);
  ReservedSpace last_part(int  partition_size);

  // Alignment
  static int page_align_size(int size);
  static int align_size(int size, int page_size);
};

// VirtualSpace is data structure for reserving a contiguous chunk of memory and 
// then commit to the reserved chunk bit by bit.
// Perfect for implementing growable stack without relocation.

class VirtualSpace : public ValueObj {
 private:
  // Reserved area
  char* _low_boundary;
  char* _high_boundary;

  // Committed area
  char* _low;
  char* _high;

  // Grow direction
  bool  _low_to_high;

  VirtualSpace* next;
  friend class VirtualSpaces;

 public:
  char* low()  const { return _low; }
  char* high() const { return _high; }

  char* low_boundary()  const { return _low_boundary; }
  char* high_boundary() const { return _high_boundary; }

 public:
  VirtualSpace(int reserved_size,      int committed_size, bool low_to_high = true);
  VirtualSpace(ReservedSpace reserved, int committed_size, bool low_to_high = true);
  VirtualSpace();

  void initialize(ReservedSpace reserved, int committed_size, bool low_to_high = true);
  
  ~VirtualSpace();

  // testers
  int committed_size()   const;
  int reserved_size()    const;
  int uncommitted_size() const;
  bool contains(void* p) const;
  bool low_to_high()     const;

  // operations
  void expand(int size);
  void shrink(int size);
  void release();

  // debugging
  void print();

  // page faults
  virtual void page_fault() {}
};

class VirtualSpaces : AllStatic {
 private:
  static VirtualSpace* head;
  static void add(VirtualSpace* sp);
  static void remove(VirtualSpace* sp);
  friend class VirtualSpace;
 public:
  static int committed_size();
  static int reserved_size();
  static int uncommitted_size();
  static void print();
  static void test();
};
