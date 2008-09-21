/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.12 $ */
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
# include "incls/_timer.cpp.incl"

void timer::start() { 
  os::updateTimes();
  userTime   = os::userTime();
  systemTime = os::systemTime();
}

void timer::stop() {
  os::updateTimes();
  userTime   = os::userTime()   - userTime;
  systemTime = os::systemTime() - systemTime;
}

double timer::seconds() {
  return userTime;
}

void timer::print() {
  std->print_cr("%3.3f", userTime);
}

void elapsedTimer::start() {
  counter = os::elapsed_counter();
}

void elapsedTimer::stop() {
  counter = os::elapsed_counter() - counter;
}

double elapsedTimer::seconds() {
  double count = counter.as_double();
  double freq  = os::elapsed_frequency().as_double();
  return count/freq;
}

void elapsedTimer::print() {
  std->print_cr("%3.3f", seconds());
}

TimeStamp::TimeStamp() : counter(0,0) {
}

void TimeStamp::update() {
  counter = os::elapsed_counter();
}

double TimeStamp::seconds() {
  long_int new_count = os::elapsed_counter();
  double count = (new_count - counter).as_double();
  double freq  = os::elapsed_frequency().as_double();
  return count/freq;
}

TraceTime::TraceTime(const char* title, bool doit) {
  active = doit;
  if (active) {
    std->print("[%s", title);
    t.start();
  }
}

TraceTime::~TraceTime() {
  if (active) {
    t.stop();
    std->print_cr(", %3.3f secs]", t.seconds());
  }
}

