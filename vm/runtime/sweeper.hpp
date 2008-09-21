/* Copyright 1994-1996 LongView Technologies L.L.C. $Revision: 1.9 $ */
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

// This is where the dirty work takes places.

// A Sweeper is an incremental cleaner for
//   Decaying invocation counters and cleanup inline caches in methodOops (MethodSweeper).

class Sweeper : public CHeapObj {
 // Static part
 private:
  static Sweeper* _head;
  static Sweeper* head()	{ return _head; }
  static int sweep_seconds;
  static bool _is_running;
  static methodOop _active_method;
  static nmethod*  _active_nmethod;
 public:
  static bool register_active_frame(frame fr);
  static void clear_active_frame();

  static void print_all();
  static void step_all();
  static void add(Sweeper* sweeper);

  static methodOop active_method()  { return _active_method;  }
  static nmethod*  active_nmethod() { return _active_nmethod; }

  // Tells is the sweeper is running
  static bool is_running() { return _is_running; }
 protected:
  Sweeper*  _next;
  Sweeper*  next() const      { return _next; }
  int       _sweep_start;     // time of last activation
  bool      _is_active;	      // are we waiting to do something?
  
  bool is_active() const      { return _is_active; }
  void set_active(bool value) { _is_active = value; }

  virtual void  step();
  virtual void  activate();
  virtual void  deactivate();
  virtual int   interval() const = 0;
  virtual char* name()     const = 0;
 public:
  Sweeper();
  virtual void task() = 0;
  void print() const;
};

// Sweeps through the heap for cleaning blockOops  
class HeapSweeper : public Sweeper {
 private:
  OldWaterMark mark;
 private:
  void  task();
  void  activate();
  int   interval() const { return HeapSweeperInterval; }
  char* name()     const { return "HeapSweeper"; }
};

class CodeSweeper : public Sweeper {
 protected:
  int CodeSweeperInterval;    // time interval (sec) between starting zone sweep; computed from half-life time
  double decayFactor;	      // decay factor for invocation counts
  int oldHalfLifeTime;	      // old half-life time (to dectect changes in half-life setting)
  int fractionPerTask;	      // a task invocation does (1 / fractionPerTask) of the entire work

  void  updateInterval();      // check for change in half-life setting
 public:
  CodeSweeper() { oldHalfLifeTime = -1; CodeSweeperInterval = 1; decayFactor = 1; }
  int   interval() const;
};

// Sweeps through the methods and decay counters and cleanup inline caches.
// Traverses all methodOops by traversing the system dictionary.
class MethodSweeper : public CodeSweeper {
 private:
  int index; // next index in systemDictionary to process
 private:
  methodOop excluded_method()                { return Universe::sweeper_method();    }
  void set_excluded_method(methodOop method) { Universe::set_sweeper_method(method); }

  void  task();
  void  method_task(methodOop method);
  int   method_dict_task(objArrayOop methods);
  int   klass_task(klassOop klass);
  void  activate();
  char* name()     const { return "MethodSweeper"; }
  friend class Recompilation;
};

extern MethodSweeper* methodSweeper;	  // single instance 

class ZoneSweeper : public CodeSweeper {
 private:
  nmethod* _excluded_nmethod; 
  nmethod* next;
 private:
  nmethod* excluded_nmethod() { return _excluded_nmethod; }
  void set_excluded_nmethod(nmethod* nm) { _excluded_nmethod = nm; }

  void  nmethod_task(nmethod* nm);
  void  task();
  void  activate();
  char* name()     const { return "ZoneSweeper"; }
};
