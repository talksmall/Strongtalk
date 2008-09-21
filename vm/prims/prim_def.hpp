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

// Macros for the primitive definitions.
// NB: Borland C++ requires reversed arguments.

#define PRIM_API __stdcall

#define PRIM_DECL_0(name)                                           oop PRIM_API name()
#define PRIM_DECL_1(name, a1)                                       oop PRIM_API name( a1)
#define PRIM_DECL_2(name, a1, a2)                                   oop PRIM_API name( a2, a1)
#define PRIM_DECL_3(name, a1, a2, a3)                               oop PRIM_API name( a3, a2, a1)
#define PRIM_DECL_4(name, a1, a2, a3, a4)                           oop PRIM_API name( a4, a3, a2, a1)
#define PRIM_DECL_5(name, a1, a2, a3, a4, a5)                       oop PRIM_API name( a5, a4, a3, a2, a1)
#define PRIM_DECL_6(name, a1, a2, a3, a4, a5, a6)                   oop PRIM_API name( a6, a5, a4, a3, a2, a1)
#define PRIM_DECL_7(name, a1, a2, a3, a4, a5, a6, a7)               oop PRIM_API name( a7, a6, a5, a4, a3, a2, a1)
#define PRIM_DECL_8(name, a1, a2, a3, a4, a5, a6, a7, a8)           oop PRIM_API name( a8, a7, a6, a5, a4, a3, a2, a1)
#define PRIM_DECL_9(name, a1, a2, a3, a4, a5, a6, a7, a8, a9)       oop PRIM_API name( a9, a8, a7, a6, a5, a4, a3, a2, a1)
#define PRIM_DECL_10(name, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) oop PRIM_API name(a10, a9, a8, a7, a6, a5, a4, a3, a2, a1)


