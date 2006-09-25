/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.13 $ */
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

// bootstrap parses and allocates memOops from a text file

class bootstrap: CHeapObj {
 private:
  // Tables
  oop* oop_table;
  int number_of_oops;
  int max_number_of_oops;

  int position;

  bool _new_format;

  void initialize_tables(int initial_table_size);

  void add(oop obj);
  oop  at(int index);

  // File
  char* file_name;
  FILE* stream;
  int   file_size;
  char get_char();
  int  get_integer();

  // Error handline
  bool _has_error;

  // Parsing
  void open_file();
  void parse_file();
  oop  get_object();
  void close_file();
  void insert_symbol(memOop obj);
 public:
  bootstrap(char *name);
  ~bootstrap();

  bool has_error() { return _has_error; }

  void read_mark(markOop* mark_addr);
  void read_oop(oop* oop_addr) { *oop_addr = get_object(); }

  char       read_byte()       { return get_char(); }
  doubleByte read_doubleByte() { return (doubleByte) get_integer(); }
  long       read_long()       { return get_integer(); } 
  double     read_double();

  bool new_format() const { return _new_format; }

  bool is_byte() { return getc(stream) == '4'; }
};
