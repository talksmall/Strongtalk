/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.12 $ */
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

// Macroes for the primitive implementation

#define PRIM_NOT_IMPLEMENTED \
   return markSymbol(vmSymbols::not_yet_implemented());


// Macros to disable certain primitives in the final product.
// Note: Some primitives cannot fail (and in the compiled code
//       there might be no check for failure), thus they cannot
//       return a markOop but must return a normal oop.

#ifdef PRODUCT
  #define PRINT_WARNING                     warning("warning: disabled primitive called");
  #define PRIMITIVE_FAILS_IN_PRODUCT        return markSymbol(vmSymbols::primitive_disabled_in_product());
  #define PRIMITIVE_RETURNS_TRUE_IN_PRODUCT PRINT_WARNING return Universe::trueObj();
  #define PRIMITIVE_RETURNS_ZERO_IN_PRODUCT PRINT_WARNING return as_smiOop(0);
#else
  #define PRIMITIVE_FAILS_IN_PRODUCT
  #define PRIMITIVE_RETURNS_TRUE_IN_PRODUCT
  #define PRIMITIVE_RETURNS_ZERO_IN_PRODUCT
#endif


// Macros to do primitive tracing

#ifdef ASSERT
#define TRACE_FUNC(flag,label) \
  static void Trace(char* name, \
                    char* s1 = NULL, oop a1 = NULL, \
                    char* s2 = NULL, oop a2 = NULL, \
                    char* s3 = NULL, oop a3 = NULL, \
                    char* s4 = NULL, oop a4 = NULL, \
                    char* s5 = NULL, oop a5 = NULL, \
                    char* s6 = NULL, oop a6 = NULL, \
                    char* s7 = NULL, oop a7 = NULL) { \
  if (!flag) return; \
  lprintf("{%s::%s", label, name); \
  if (s1) { lprintf(" %s=", s1); a1->print_value(); } \
  if (s2) { lprintf(" %s=", s2); a2->print_value(); } \
  if (s3) { lprintf(" %s=", s3); a3->print_value(); } \
  if (s4) { lprintf(" %s=", s4); a4->print_value(); } \
  if (s5) { lprintf(" %s=", s5); a5->print_value(); } \
  if (s6) { lprintf(" %s=", s6); a6->print_value(); } \
  if (s7) { lprintf(" %s=", s7); a7->print_value(); } \
  lprintf("}\n"); \
}
#define TRACE_0(name) \
  inc_calls();  \
  Trace(name);

#define TRACE_1(name, a1) \
  inc_calls();      \
  Trace(name, XSTR(a1), a1);

#define TRACE_2(name, a1, a2) \
  inc_calls();          \
  Trace(name, XSTR(a1), a1, XSTR(a2), a2);

#define TRACE_3(name, a1, a2, a3) \
  inc_calls();              \
  Trace(name, XSTR(a1), a1, XSTR(a2), a2, XSTR(a3), a3);

#define TRACE_4(name, a1, a2, a3, a4) \
  inc_calls();                  \
  Trace(name, XSTR(a1), a1, XSTR(a2), a2, XSTR(a3), a3, XSTR(a4), a4);

#define TRACE_5(name, a1, a2, a3, a4, a5) \
  inc_calls();                      \
  Trace(name, XSTR(a1), a1, XSTR(a2), a2, XSTR(a3), a3, XSTR(a4), a4, XSTR(a5), a5);

#define TRACE_6(name, a1, a2, a3, a4, a5, a6) \
  inc_calls();                          \
  Trace(name, XSTR(a1), a1, XSTR(a2), a2, XSTR(a3), a3, XSTR(a4), a4, XSTR(a5), a5, XSTR(a6), a6);

#define TRACE_7(name, a1, a2, a3, a4, a5, a6, a7) \
  inc_calls();                              \
  Trace(name, XSTR(a1), a1, XSTR(a2), a2, XSTR(a3), a3, XSTR(a4), a4, XSTR(a5), a5, XSTR(a6), a6, XSTR(a7), a7);
#else
#define TRACE_FUNC(flag, label)
#define TRACE_0(name)
#define TRACE_1(name, a1)
#define TRACE_2(name, a1, a2)
#define TRACE_3(name, a1, a2, a3)
#define TRACE_4(name, a1, a2, a3, a4)
#define TRACE_5(name, a1, a2, a3, a4, a5)
#define TRACE_6(name, a1, a2, a3, a4, a5, a6)
#define TRACE_7(name, a1, a2, a3, a4, a5, a6, a7)
#endif


#define PROLOGUE_0(name)                             TRACE_0(name)
#define PROLOGUE_1(name, a1)                         TRACE_1(name, a1)
#define PROLOGUE_2(name, a1, a2)                     TRACE_2(name, a1, a2)
#define PROLOGUE_3(name, a1, a2, a3)                 TRACE_3(name, a1, a2, a3)
#define PROLOGUE_4(name, a1, a2, a3, a4)             TRACE_4(name, a1, a2, a3, a4)
#define PROLOGUE_5(name, a1, a2, a3, a4, a5)         TRACE_5(name, a1, a2, a3, a4, a5)
#define PROLOGUE_6(name, a1, a2, a3, a4, a5, a6)     TRACE_6(name, a1, a2, a3, a4, a5, a6)
#define PROLOGUE_7(name, a1, a2, a3, a4, a5, a6, a7) TRACE_7(name, a1, a2, a3, a4, a5, a6, a7)
