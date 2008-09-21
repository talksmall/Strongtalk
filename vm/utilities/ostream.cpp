/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.16 $ */
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
# include "incls/_ostream.cpp.incl"

outputStream::outputStream(int width) {
  _width       = width;
  _position    = 0;
  _indentation = 0;
}

void outputStream::basic_print(const char* str) {
  int len = strlen(str);
  for (int i = 0; i < len; i++) put(str[i]);
}

#define BUFLEN 2000   /* max size of output of individual print() methods */

void outputStream::print(const char* format, ...) {
  char buffer[BUFLEN];
  va_list ap;
  va_start(ap, format);
  if (_vsnprintf(buffer, BUFLEN, format, ap) < 0) {
    warning("increase BUFLEN in ostream.cpp -- output truncated");
    buffer[BUFLEN] = 0;
  }
  va_end(ap);
  basic_print(buffer);
}

void outputStream::print_cr(const char* format, ...) {
  char buffer[BUFLEN];
  va_list ap;
  va_start(ap, format);
  if (_vsnprintf(buffer, BUFLEN, format, ap) < 0) {
    warning("increase BUFLEN in ostream.cpp -- output truncated");
    buffer[BUFLEN] = 0;
  }
  va_end(ap);
  basic_print(buffer);
  cr();
}

void outputStream::vprint(const char *format, va_list argptr) {
  char buffer[BUFLEN];
  if (_vsnprintf(buffer, BUFLEN, format, argptr) < 0) {
    warning("increase BUFLEN in ostream.cpp -- output truncated");
    buffer[BUFLEN] = 0;
  }
  basic_print(buffer);
}

void outputStream::vprint_cr(const char* format, va_list argptr) {
  vprint(format, argptr);
  cr();
}

void outputStream::fill_to(int col) {
  while (position() < col) sp();
}

void outputStream::put(char c) {
  lputc(c);
  _position++;
}

void outputStream::out_hex(int addr) {
  Unimplemented();
}

void outputStream::sp() {
  put(' ');
}

void outputStream::cr() {
  put('\n');
  _position = 0;
}

void outputStream::indent() {
 while (_position < _indentation) sp();
}

stringStream::stringStream(int initial_size) : outputStream() {
  buffer_length = initial_size;
  buffer        = NEW_RESOURCE_ARRAY(char, buffer_length);
  buffer_pos    = 0;  
}

void stringStream::put(char c) {
  if (buffer_pos >= buffer_length) {
    // grow string buffer
    char* oldbuf = buffer;
    buffer = NEW_RESOURCE_ARRAY(char, buffer_length * 2);
    strncpy(buffer, oldbuf, buffer_length);
    buffer_length *= 2;
  }
  buffer[buffer_pos++] = c;
  _position++;
}

char* stringStream::as_string() {
  char* copy = NEW_RESOURCE_ARRAY(char, buffer_pos + 1);
  memcpy(copy, buffer, buffer_pos);
  copy[buffer_pos] = '\0';
  return copy;
}

byteArrayOop stringStream::as_byteArray() {
  byteArrayOop a = oopFactory::new_byteArray(buffer_pos);
  for (int i = 0; i < buffer_pos; i++) {
    a->byte_at_put(i+1, buffer[i]);
  }
  return a;
}

fileStream::fileStream(char* file_name) {
  _file = fopen(file_name, "w");
}

void fileStream::put(char c) {
  fputc(c, _file);
  _position++;
}

fileStream::~fileStream() {
  fclose(_file);
  _file = NULL;
}

#ifdef std
#undef std
#endif

outputStream* _std;
outputStream* _err;

void ostream_init() {
  if (_std) return;
  _std = new(true) outputStream();   // NB: this stream is allocated on the C heap
  _err = _std;
}

outputStream* getErr() {
	ostream_init();
	return _err;
}

outputStream* getStd() {
	ostream_init();
	return _std;
}
