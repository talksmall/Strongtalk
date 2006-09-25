/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.7 $ */
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

// The EL_Event log is used for debugging; it is a circular buffer containing
// the last N events.  An EL_Event is represented by an identifying string
// and up to EVENT_PARAMS parameters.

const int EVENT_PARAMS = 3;       // number of params per EL_Event

// helper macros

#define LOG_EVENT(name)           eventLog->log(name)
#define LOG_EVENT1(name,p1)       eventLog->log(name, (void*)(p1))
#define LOG_EVENT2(name,p1,p2)    eventLog->log(name, (void*)(p1), (void*)(p2))
#define LOG_EVENT3(name,p1,p2,p3) eventLog->log(name, (void*)(p1), (void*)(p2), (void*)(p3))

enum EL_EventStatus { starting, ending, atomic };

struct EL_Event /* no superclass - never allocated individually */ {       
  char* name;                     // in printf format
  EL_EventStatus status;          // for nested events
  void* args[EVENT_PARAMS];
};


struct EventLog : public CHeapObj {   
  EL_Event* buf;                  // event buffer
  EL_Event* bufEnd;
  EL_Event* next;                 // where the next entry will go
  int  nesting;         	  // current nesting depth

  EventLog();
  void   init();

  EL_Event* nextEvent(EL_Event* e, EL_Event* start, EL_Event* end) {
    if (e + 1 == end) return start; else return e + 1; }
  EL_Event* prevEvent(EL_Event* e, EL_Event* start, EL_Event* end) {
    if (e == start) return end - 1; else return e - 1; }
  void   inc()          { next = nextEvent(next, buf, bufEnd); }

  void   log(EL_Event* e)  { *next = *e; inc(); }
  void   log(char* name) {
    next->name = name; next->status = atomic;
    inc(); }
  void   log(char* name, void* p1) {
    next->name = name; next->status = atomic;
    next->args[0] = p1; inc(); }
  void   log(char* name, void* p1, void* p2) {
    next->name = name; next->status = atomic;
    next->args[0] = p1; next->args[1] = p2; inc(); }
  void   log(char* name, void* p1, void* p2, void* p3) {
    next->name = name; next->status = atomic;
    next->args[0] = p1; next->args[1] = p2; next->args[2] = p3; inc(); }
  
  void   resize();                // resize buffer
  
  void   print() { printPartial(bufEnd - buf); }
  void   printPartial(int n);
};

extern EventLog* eventLog;

class EventMarker : StackObj {    // for events which have a duration
 public:
  EL_Event event;
  EL_Event* here;

  EventMarker(char* n)                                  { init(n, 0, 0, 0); }
  EventMarker(char* n, void* p1)                        { init(n, p1, 0, 0); }
  EventMarker(char* n, void* p1, void* p2)              { init(n, p1, p2, 0); }
  EventMarker(char* n, void* p1, void* p2, void* p3)    { init(n, p1, p2, p3);}

  void init(char* n, void* p1, void* p2, void* p3) {
    here = eventLog->next;
    eventLog->log(n, p1, p2, p3);
    here->status = starting; 
    event = *here;
    eventLog->nesting++;
  }
  ~EventMarker() {
    eventLog->nesting--;
    // optimization to make log less verbose; this isn't totally failproof
    // but that's ok
    if (here == eventLog->next - 1) {
      *here = event;
      here->status = atomic;       // nothing happened inbetween
    } else {
      event.status = ending;
      eventLog->log(&event);
    }
  }
};
