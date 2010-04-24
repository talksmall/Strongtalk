/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.48 $ */
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
# include "incls/_shell.cpp.incl"

extern "C" void load_image()   {
    ResourceMark rm;
    stringStream st;
    st.print("Reading in %s", boot_filename);
    TraceTime t(st.as_string());
    bootstrap b(boot_filename);
    vmSymbols::initialize();
    bootstrapping = false;
}

int vmProcessMain(void* ignored) {
  Processes::start(new VMProcess);
  return 0;
}
void launchVMProcess() {
  int ignored;
  Thread* vmThread = os::create_thread(&vmProcessMain, NULL, &ignored);
  Event* waitForever = os::create_event(false);
  os::wait_for_event(waitForever);
}
int vm_main(int argc, char* argv[]) {
  parse_arguments(argc, argv);		// overrides default flag settings
  init_globals();

  load_image();

  if (UseInliningDatabase)
    InliningDatabase::load_index_file();

  launchVMProcess();
  return 0;
}
