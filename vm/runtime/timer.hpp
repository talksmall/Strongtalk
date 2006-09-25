/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.11 $ */
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


// Timers for simple measurements

class timer {
 private:
  double userTime;
  double systemTime;
 public:
  void start();
  void stop();
  void print();
  double seconds();
};


class elapsedTimer {
 private:
  long_int counter;
 public:
  elapsedTimer() : counter(0,0) {}
  void start();
  void stop();
  void print();
  double seconds();
};

// TimeStamp is used based on elapsed time and 
class TimeStamp {
 private:
  long_int counter;

 public:
  // Constructor
  TimeStamp();

  // update to current elapsed time
  void update();

  // returns seconds since updated
  double seconds();
};

// TraceTime is used for tracing the execution time of a block
// Usage:
//  { TraceTime t("block time")
//    some_code();
//  }
//

class TraceTime {
 private:
  bool   active;
  elapsedTimer t;
 public:
  TraceTime(char* title, bool doit = true);
  ~TraceTime();
};

