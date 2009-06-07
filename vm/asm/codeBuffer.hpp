/* Copyright 1994 - 1996, LongView Technologies L.L.C. $Revision: 1.1 $ */
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

#ifdef DELTA_COMPILER

class  MacroAssembler;
extern MacroAssembler* theMacroAssm;


class CodeBuffer: public PrintableResourceObj {
 private:
  char*		instsStart;
  char*		instsEnd;
  char*		instsOverflow;
  
  relocInfo*	locsStart;
  relocInfo*	locsEnd;
  relocInfo*	locsOverflow;
  int		last_reloc_offset;

  char*		_decode_begin;

  char*		decode_begin();

 public:
  CodeBuffer(char* code_start, int code_size);
  CodeBuffer(int instsSize, int locsSize);
  
  char* code_begin() const		{ return instsStart; };
  char* code_end()   const		{ return instsEnd; }
  char* code_limit() const		{ return instsOverflow; };

  int   code_size()  const		{ return instsEnd - instsStart; }
  int   reloc_size() const		{ return (locsEnd - locsStart) * sizeof(relocInfo); }

  void  set_code_end(char* end);
  void  relocate(char* at, relocInfo::relocType rtype);

  void  decode();
  void  decode_all();

  void  copyTo(nmethod* nm);

  void  print();
};

#endif
