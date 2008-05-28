/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.11 $ */
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

// Output streams for printing

class outputStream : public ResourceObj {
 protected:
   int  _indentation;	    // current indentation
   int  _width;		    // width of the page
   int  _position;	    // position on the current line

   void basic_print(const char* str);

 public:
   // creation
   outputStream(int width = 80);

   // indentation
   void indent();
   void inc() { _indentation++; };
   void dec() { _indentation--; };
   int  indentation() const    { return _indentation; }
   void set_indentation(int i) { _indentation = i;    }
   void fill_to(int col);

   // sizing
   int width()    const { return _width;    }
   int position() const { return _position; }

   // printing
   void print(const char* format, ...);
   void print_cr(const char* format, ...);

   void vprint(const char *format, va_list argptr);
   void vprint_cr(const char* format, va_list argptr);

   void print_raw(const char* str) { basic_print(str); }
   void out_hex(int addr);

   virtual void put(char c);
   virtual void sp();
   virtual void cr();

   void dec_cr() { dec(); cr(); }
   void inc_cr() { inc(); cr(); }
};

// Standard output
extern outputStream* _std;

outputStream* getErr();
outputStream* getStd();

#ifdef std
#undef std
#endif

#define std getStd()
//#define err getErr()

// Standard error
extern outputStream* _err;

class stringStream : public outputStream {
 protected:
  char* buffer;
  int   buffer_pos;
  int   buffer_length;
 public:
  stringStream(int initial_size = 1 * K);
  void put(char c);
  char* as_string();

  // Conversion into Delta object
  byteArrayOop as_byteArray();
};

class fileStream : public outputStream {
 protected:
  FILE* _file;
 public:
  fileStream(char* file_name);
  ~fileStream();
  int is_open() const { return _file != NULL; }
  void put(char c);
};




