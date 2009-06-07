/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.4 $ */
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

/* 
 **  CONC(a,b)  Concatenates a and b (which are not macro-expanded)
 **  CONC3(a,b,c) Concatentates a, b, and c (which are not macro-expanded)
 **  STR(a) Makes a string of the argument (which is not macro-expanded)
 **  STR(a) Makes a character of the argument (which is not macro-expanded)
 **  XCONC(a,b) Concatenates the macro expansions of a and b
 **  XSTR(b) Makes a string of the macro expansion of a
 **  XCHAR(b) Makes a character of the macro expansion of a
 */

# define CONC(a,b)      a##b
# define CONC3(a,b,c)   a##b##c
# define STR(a)         #a
# define CHAR(a)        ((#a)[0])
  
# define XCONC(a,b)     CONC(a,b)
# define XSTR(a)        STR(a)
# define XCHAR(a)       CHAR(a)


// ------------------------ Type and value checking macros -------------------- 

// Macro to verify the type of an oop, and create a requalified oop.
// E.g. CHECKOOPTYPE(host, is_byteArray, byteArrayOop, host1) expands to:
//      if(!host->is_byteArray()) return prim_error(BADTYPEERROR);
//      byteArrayOop host1 = byteArrayOop(host);
// Warning: does not wrap in '{' and '}'!
# define CHECKOOPTYPE(ref, typePredicate, newType, newRef)                    \
    if (!ref->typePredicate())                                                \
      return prim_error(BADTYPEERROR);                                        \
    newType newRef = newType(ref);


// Check that ref is a smiOop, and set up variable to hold value. 
// Warning: does not wrap in '{' and '}'!
# define CHECKOOPSMI(ref, val)                                                \
    if (!ref->is_smi())                                                       \
      return prim_error(BADTYPEERROR);                                        \
    smi val = smiOop(ref)->value();


// Check that ref is either trueObj or falseObj. Set up boolean var corresp.
// Warning: does not wrap in '{' and '}'!
# define CHECKOOPBOOL(ref, val)                                               \
    bool val;                                                                 \
    if (ref == falseObj)                                              \
      val = false;                                                            \
    else if (ref == trueObj)                                          \
      val = true;                                                             \
    else                                                                      \
      return prim_error(BADTYPEERROR);


