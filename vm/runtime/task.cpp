/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.5 $ */
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
# include "incls/_task.cpp.incl"

const int max_tasks = 10;
      int num_tasks = 0;

PeriodicTask* tasks[max_tasks];


bool pending_tasks(int delay_time) {
  bool result = false;
  for(int index = 0; index < num_tasks; index++) {
    result = tasks[index]->is_pending(delay_time) || result;
  }
  return result;
}

void real_time_tick(int delay_time) {
  // Do not perform any tasks before the bootstrapping is done
  if (bootstrapping) return;

  if (pending_tasks(delay_time)) {
    ThreadCritical tc;
    if (!Process::external_suspend_current()) return;

    for(int index = 0; index < num_tasks; index++) {
      PeriodicTask* task = tasks[index];
      if (task->counter >= task->interval) {
        task->task();
        task->counter = 0;
      }
    }
    Process::external_resume_current();
  }
}

PeriodicTask::PeriodicTask(int interval_time) {
  counter  = 0;
  interval = interval_time;
}

PeriodicTask::~PeriodicTask() {
  if (is_enrolled())
    deroll();
}

bool PeriodicTask::is_enrolled() const {
  for(int index = 0; index < num_tasks; index++) 
    if (tasks[index] == this) return true;
  return false;
}

void PeriodicTask::enroll() {
  if (num_tasks == max_tasks)
    fatal("Overflow in PeriodicTask table");
  tasks[num_tasks++] = this;
}

void PeriodicTask::deroll() {
  int index;
  for(index = 0; index < num_tasks && tasks[index] != this; index++);
  if (index == max_tasks) return;
  num_tasks--;
  for (; index < num_tasks; index++) {
    tasks[index] = tasks[index+1];
  }
}
