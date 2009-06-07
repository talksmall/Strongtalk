/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.4 $ */
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
# include "incls/_disclaimer.cpp.incl"


char* Disclaimer::_msg =
"\
Smalltalk Virtual Machine by LongView Technologies LLC		\n\
---------------------------------------------------------	\n\
(c) 1994-1997 Sun Microsystems, Inc. ALL RIGHTS RESERVED.	\n\
Use and distribution of this software is subject to the		\n\
terms of the attached license.					\n\
---------------------------------------------------------	\n\
";
/*
"\n\
Strongtalk (TM) Virtual Machine -- proprietary information of		\n\
LongView Technologies L.L.C. (c) 1994 - 1996.				\n\
									\n\
LongView Technologies L.L.C. (\"LongView\") grants to you the non-	\n\
exclusive, non-assignable right to use the Strongtalk (TM) Virtual	\n\
Machine as well as the associated Strongtalk components .bst file	\n\
and source database (\"this software\") on one computer system solely	\n\
for evaluation purposes. This software shall not be copied or used	\n\
for any other purpose. You may not decompile, dissassemble, or		\n\
reverse	engineer this software. You may not disclose the results of	\n\
any benchmark testing of this software to any third party without	\n\
LongView's prior written permission. In addition to the restrictions	\n\
set forth in this notice, any use of this software is subject to the	\n\
terms of the Nondisclosure Agreement, if any. All rights not expressly	\n\
granted are reserved by LongView.					\n\
									\n\
This software is provided \"as is\", without any warranty of any kind.	\n\
To the maximum extent permitted by law, LongView disclaims all		\n\
warranties, either express or implied, including but not limited	\n\
to implied warranties of merchantability, fitness for a particular	\n\
purpose and nonfringement. The entire risk arising out of the use	\n\
of this software remains with you.					\n\
									\n\
To the maximum extent permitted by law, in no event shall LongView	\n\
be liable for any damages whatsoever arising out of the use of this	\n\
software, even if LongView has been advised of the possibility of	\n\
such damages.								\n\
\n";
*/

void Disclaimer::print_disclaimer(outputStream* s) {
  s->print(_msg);
}


void Disclaimer::initialize() {
  // Computes a checksum over the disclaimer msg - simple check
  // to detect hack attacks aiming at modifying the disclaimer.
  const int xsum = -2020472186; // change this when changing _msg
  int s = 0;
  for (char* p = _msg; *p != '\x0'; p++)
    s = ((s << 3) | (s >> 29) & 0x7) ^ *p;
  if (s != xsum) {
    if (is_product()) {
      // VM has been hacked => terminate immediately.
      exit(0);
    } else {
      // notify programmer
      warning("Disclaimer::initialize: xsum = %d but %d expected", s, xsum);
    }
  }
}


void disclaimer_init() {
  Disclaimer::initialize();
}
