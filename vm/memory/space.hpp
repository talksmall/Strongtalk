/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.23 $ */
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


// A Water mark points into a space and is used during scavenge to 
// keep track of progress.
class NewWaterMark : ValueObj {
 public:
  oop* _point;

};

inline bool operator==(const NewWaterMark& x, const NewWaterMark& y) {
  return x._point == y._point; 
}

inline bool operator!=(const NewWaterMark& x, const NewWaterMark& y) {
  return !(x == y);
}

class OldWaterMark : ValueObj {
 public:
  oldSpace* _space;
  oop*      _point;
  oop* pseudo_allocate(int size);
};

inline bool operator==(const OldWaterMark& x, const OldWaterMark& y) {
  return (x._space == y._space) && (x._point == y._point); 
}
inline bool operator!=(const OldWaterMark& x, const OldWaterMark& y) {
  return !(x == y); 
}

class space: public CHeapObj {
 private:
  // instance variable
  char* _name;

 public:
  // invariant: bottom() and end() are on page_size boundaries
  // and:       bottom() <= top() <= end()
  virtual oop* bottom() = 0;
  virtual oop* top()    = 0;
  virtual oop* end()    = 0;

  char* name() const        { return _name; }
  void set_name(char* name) { _name = name; }

 protected:
  virtual void set_bottom(oop* value) = 0;
  virtual void set_top(oop* value)    = 0;
  virtual void set_end(oop* value)    = 0;

 public:
  void initialize(char* name, oop* bottom, oop* end);
  void clear();

 public:
  bool is_empty()     { return used() == 0; }
  int  capacity()     { return byte_size(bottom(), end()); }
  int  used()         { return byte_size(bottom(), top()); }
  int  free()         { return byte_size(top(),    end()); }
    
  // MarkSweep support
  // phase2
  void prepare_for_compaction(OldWaterMark* mark);
  // phase3
  void compact(OldWaterMark* mark);

 protected:
  // operations
  virtual void verify() = 0;

  // for debugging & fun
  void oops_do(oopsDoFn f);
     
 public:
  void switch_pointers(oop from, oop to);
  void print();
  void object_iterate(ObjectClosure* blk);
};

class newSpace: public space {
 public:
  newSpace* next_space;

 public:
  oop* object_start(oop* p);
  void verify();

  void object_iterate_from(NewWaterMark* mark, ObjectClosure* blk);

  NewWaterMark top_mark() {
    NewWaterMark m;
    m._point = top();
    return m;
  }
};

extern "C" oop* eden_bottom;
extern "C" oop* eden_top;
extern "C" oop* eden_end;

class edenSpace : public newSpace {
 public:
  oop* bottom() { return eden_bottom; }
  oop* top()    { return eden_top; }
  oop* end()    { return eden_end; }

  bool contains(void* p) { return (oop*) p >= eden_bottom && (oop*) p < eden_top; }

 protected:
  void set_bottom(oop* value) { eden_bottom = value; }
  void set_top(oop* value)    { eden_top    = value; }
  void set_end(oop* value)    { eden_end    = value; }

 public:
  // constructor
  edenSpace();

  // allocation
  oop* allocate(int size) {
    oop* oops     = eden_top;
    oop* oops_end = oops + size;
    if (oops_end <= eden_end) {
      eden_top = oops_end;
      return oops;
    } else {
      return NULL;
    }
  }
};

class survivorSpace : public newSpace {
 private:
  oop* _bottom;
  oop* _top;
  oop* _end;

 public:
  oop* bottom() { return _bottom; }
  oop* top()   { return _top; }
  oop* end()   { return _end; }

  bool contains(void* p) { return (oop*) p >= _bottom && (oop*) p < _top; }

 protected:
  void set_bottom(oop* value) { _bottom = value; }
  void set_top(oop* value)    { _top    = value; }
  void set_end(oop* value)    { _end    = value; }

 public:
  // constructor
  survivorSpace();

  // allocation
  oop* allocate(int size) {
    oop* oops     = _top;
    oop* oops_end = oops + size;
    if (oops_end <= _end) {
      _top = oops_end;
      return oops;
    } else {
      return NULL;
    }
  }

  // allocation test
  bool would_fit(int size) { return _top + size < _end; }

  // Scavenge support
  void scavenge_contents_from(NewWaterMark* mark);
};
class OldSpaceMark;
class oldSpace: public space {
 friend class space;
 friend class OldSpaceMark;

 private:
  oop* _bottom;
  oop* _top;
  oop* _end;
 public:
  oop* bottom() { return _bottom; }
  oop* top()    { return _top; }
  oop* end()    { return _end; }

  bool contains(void* p) { return (oop*) p >= _bottom && (oop*) p < _top; }

 protected:
  void set_bottom(oop* value) { _bottom = value; }
  void set_top(oop* value)    { _top    = value; }
  void set_end(oop* value)    { _end    = value; }

 public: 
  void initialize_threshold();
  inline void update_offsets(oop* begin, oop* end) {
    if (end >= next_offset_treshold) update_offset_array(begin, end);
  }
  public: 
  oldSpace* next_space;

  oop* object_start(oop* p);

  void update_offset_array(oop* p, oop* p_end);

  int expand(int size);
  oop* expand_and_allocate(int size);
  int shrink(int size);

  // Keeps offset for retrieving object start given a card_page
  u_char* offset_array;
  oop*    next_offset_treshold;
  int     next_offset_index;

  oop* allocate(int size, bool allow_expansion=true) {
    oop* p  = _top;
    oop* p1 = p + size;
    if (p1 < _end) {
      _top = p1;
      update_offsets(p, p1);
      return p;
    } else {
      if (!allow_expansion) return NULL;
      return expand_and_allocate(size);
    }
  }

  // constructors
  // allocates object space too; sets size to amount allocated, 0 if none
  oldSpace(char* nm, int &size);

  void scavenge_contents_from(OldWaterMark* mark);
  void object_iterate_from(OldWaterMark* mark, ObjectClosure* blk);

  void scavenge_recorded_stores();

  void verify();

  OldWaterMark top_mark() {
    OldWaterMark m;
    m._space = this;
    m._point = _top;
    return m;
  }

  OldWaterMark bottom_mark() {
    OldWaterMark m;
    m._space = this;
    m._point = _bottom;
    return m;
  }
};

// This is primarily intended for testing to allow
// temporary allocations to be reset easily.
class OldSpaceMark: public ValueObj {
private:
  oop*      oldTop;
public:
  oldSpace* theSpace;
  OldSpaceMark(oldSpace* aSpace): theSpace(aSpace) {
    oldTop = theSpace->top();
  }
  ~OldSpaceMark() {
    theSpace->set_top(oldTop);
  }
};