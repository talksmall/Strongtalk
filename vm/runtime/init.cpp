/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.32 $ */
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
# include "incls/_init.cpp.incl"

// Add new initialization/finalization methods to the macro below; your
// functions must be parameterless and must be named after the .c file
// they belong to (e.g. "void foo_init(); void foo_exit()" for foo.c).
// The INIT_DO macro defines the initialization sequence; you need not
// add a corresponding entry to EXIT_DO if you don't need finalization.

# define INIT_DO(template)				\
  template(ostream_init)				\
  template(os_init)					\
  template(except_init)					\
  template(prim_init)					\
  template(eventlog_init)				\
  template(integerOps_init)				\
  template(bytecodes_init)				\
 /* template(stubRoutines_init)	*/			\
  template(universe_init)				\
  template(generatedPrimitives_init_before_interpreter)	\
  template(interpreter_init)				\
  template(dispatchTable_init)				\
  template(disclaimer_init)				\
  template(costModel_init)				\
  template(sweeper_init)				\
  template(fprofiler_init)				\
  template(systemAverage_init)				\
  template(generatedPrimitives_init_after_interpreter)	\

  

# define COMPILER_INIT_DO(template)	\
  template(compiler_init)		\
  template(mapping_init)		\

/* compiler related */
/*  template(opcode_init) */

# define EXIT_DO(template)		\
  template(lprintf_exit)		\
  template(os_exit)

# define DEFINE_TEMPLATE(name)    void name();

# define CALL_TEMPLATE(name)	  name();

INIT_DO(DEFINE_TEMPLATE)
#ifdef DELTA_COMPILER
  COMPILER_INIT_DO(DEFINE_TEMPLATE)
#endif

EXIT_DO(DEFINE_TEMPLATE)

void init_globals() {
  ResourceMark rm;
  INIT_DO(CALL_TEMPLATE)
#ifdef DELTA_COMPILER
  COMPILER_INIT_DO(CALL_TEMPLATE)
#endif

  if (!UseTimers) {
    SweeperUseTimer = false;
  }
}

void exit_globals() {
  static bool destructorsCalled = false;
  if (!destructorsCalled) {
    destructorsCalled = true;
    EXIT_DO(CALL_TEMPLATE);
  }
}

