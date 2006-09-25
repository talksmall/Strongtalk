/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.7 $ */
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
# include "incls/_eventlog.cpp.incl"

EventLog* eventLog;

void eventlog_init() { eventLog = new EventLog; }

static char* noEvent = "no event";

void EventLog::init() {
  buf = next = NEW_C_HEAP_ARRAY( EL_Event, EventLogLength);
  bufEnd = buf + EventLogLength;
  for (EL_Event* e = buf; e < bufEnd; e++) e->name = noEvent;
}

EventLog::EventLog() {
  nesting = 0;
  init();
}

void EventLog::resize() {
  EL_Event* oldBuf = buf;
  EL_Event* oldEnd = bufEnd;
  EL_Event* oldNext = next;
  init();
  // copy events
  for (EL_Event* e = nextEvent(oldNext, oldBuf, oldEnd); e != oldNext;
       e = nextEvent(e, oldBuf, oldEnd), next = nextEvent(next, buf, bufEnd)) {
    *next = *e;
  }
  FreeHeap( oldBuf);
}

void EventLog::printPartial(int n) {
  EL_Event* e = next;
  // find starting point
  if (n >= EventLogLength) n = EventLogLength - 1;
  for (int i = 0; i < n; i++, e = prevEvent(e, buf, bufEnd)) ;
  
  // skip empty entries
  i = 0;
  for (; e != next && e->name == noEvent; i++, e = nextEvent(e, buf, bufEnd)) ;

  int indent = 0;
  for (; i < n && e != next; i++, e = nextEvent(e, buf, bufEnd)) {
    char* s;
    switch (e->status) {
     case starting: s = "[ "; break;
     case ending:   s = "] "; indent--; break;
     case atomic:   s = "- "; break;
    }
    lprintf("%*.s%s", 2*indent, " ", s);
    lprintf(e->name, e->args[0], e->args[1], e->args[2]);
    lprintf("\n");
    if (e->status == starting) indent++;
  }
  if (indent != nesting)
    lprintf("Actual event nesting is %ld greater than shown.\n",
	   nesting - indent);
}

