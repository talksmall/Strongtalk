/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.10 $ */
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

// Bogosity alert:  for non-ANSI C, `obj' and `t' must not be separated
//  by whitespace in either the parameters of the definition or
//  the arguments of the call.  

#define assert_type(obj,t,msg)                                               \
  assert(CONC(oop(obj)->is_,t()), msg)

#define assert_mark(obj,msg)                                                 \
  assert_type(obj,mark,msg)

#define assert_smi(obj,msg)                                                  \
  assert_type(obj,smi,msg)

#define assert_mem(obj,msg)                                                  \
  assert_type(obj,mem,msg)

#define assert_objArray(obj,msg)                                             \
  assert_type(obj,objArray,msg)

#define assert_byteArray(obj,msg)                                            \
  assert_type(obj,byteArray,msg)

#define assert_doubleByteArray(obj,msg)                                      \
  assert_type(obj,doubleByteArray,msg)

#define assert_doubleValueArray(obj,msg)                                     \
  assert_type(obj,doubleValueArray,msg)

#define assert_symbol(obj,msg)                                               \
  assert_type(obj,symbol,msg)

#define assert_double(obj,msg)                                               \
  assert_type(obj,double,msg)

#define assert_block(obj,msg)                                                \
  assert_type(obj,block,msg)

#define assert_klass(obj,msg)                                                \
  assert_type(obj,klass,msg)

#define assert_process(obj,msg)                                              \
  assert_type(obj,process,msg)

#define assert_oop_aligned(p) \
  assert((int)(p) % 4 == 0, "not word aligned")


#ifndef DEBUG
  #ifndef PRODUCT
    for now DEBUG should always be set in non-production versions  -Urs
  #endif
#endif
