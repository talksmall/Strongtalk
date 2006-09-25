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
# include "incls/_error.cpp.incl"

void report_vm_state() {
#ifdef DEBUG
  static bool recursive_error = false;
  if (!recursive_error) {
    recursive_error = true;
#ifdef DELTA_COMPILER
    if (theCompiler) {
      std->print("\nThe error happened while compiling ");
      theCompiler->print_key(std);
      std->cr();
      if (CompilerDebug) {
	print_cout();
      } else {
	std->print("(No compiler debug output available -- run with +CompilerDebug to get it)");
      }
      std->cr();
    }
#endif // DELTA_COMPILER

    std->print("\nLast 10 internal VM events:\n");
    eventLog->printPartial(10);
    recursive_error = false;
  }
#endif // DEBUG
}

void report_error(char* title, char* format, ...) {
  char buffer[2048];
  va_list ap;
  va_start(ap, format);
  vsprintf(buffer, format, ap);
  va_end(ap);

  std->cr();
  std->print_cr("[A Runtime Error Occurred]");
  std->print_raw(buffer);

  if (!bootstrapping) report_vm_state();

  if (ShowMessageBoxOnError) {
    strcat(buffer, "\n\nDo you want to debug the problem?");
    if (!os::message_box(title, buffer)) os::fatalExit(-1);
  }
}

void report_assertion_failure(char* code_str, char* file_name, int line_no, char* message) {
  report_error("Assertion Failure", "assert(%s, \"%s\")\n%s, %d",
	        code_str, message, file_name, line_no);
}

void report_fatal(char* file_name, int line_no, char* format, ...) {
  char buffer[2048];
  va_list ap;
  va_start(ap, format);
  vsprintf(buffer, format, ap);
  va_end(ap);
  report_error("Fatal Error", "Fatal: %s\n%s, %d", buffer, file_name, line_no);
}

void report_should_not_call(char* file_name, int line_no) {
  report_error("Should Not Call This Error", "ShouldNotCall()\n%s, %d",
	       file_name, line_no);
}

void report_should_not_reach_here(char* file_name, int line_no) {
  report_error("Should Not Reach Here Error", "ShouldNotReachHere()\n%s, %d",
	       file_name, line_no);
}

void report_subclass_responsibility(char* file_name, int line_no) {
  report_error("Subclass Responsibility Error", "SubclassResponsibility()\n%s, %d",
	       file_name, line_no);
}

void report_unimplemented(char* file_name, int line_no) {
  report_error("Unimplemented Error", "Unimplemented()\n%s, %d", file_name, line_no);
}

