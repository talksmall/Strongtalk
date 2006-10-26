/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.13 $ */
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

// This file contains the interface to the vm pretty printer.
//    prettyPrintStream describes the output media.
//    prettyPrinter     defined the interface for printing.

// WARNING WARNING WARNING WARNING WARNING WARNING
// The implementation is NOT production quality due
// to time constraints but since it is only intended 
// for vm debugging I hope we can live  with it!!!!!
// -- Lars 3/24-95

class prettyPrintStream : public PrintableResourceObj {
 protected:
  int indentation;
  int pos;
  bool in_hl;
  void set_highlight(bool value) { in_hl = value; }
 public:
  bool in_highlight() { return in_hl; }

  // Creation
  prettyPrintStream() {
    indentation = pos = 0;
    set_highlight(false);
  }

  // Indentation
  virtual void indent() = 0;
  void inc_indent() { indentation++; }
  void dec_indent() { indentation--; }
  int position ()   { return pos; }

  // Printing
  virtual void print(char* str)   = 0;
  virtual void print_char(char c) = 0;
  virtual void newline()          = 0;
  void dec_newline() { dec_indent(); newline(); }
  void inc_newline() { inc_indent(); newline(); }
  virtual void space()            = 0;
  virtual void begin_highlight()  = 0;
  virtual void end_highlight()    = 0;

  // Layout
  virtual int width()                    = 0;
  virtual int remaining()                = 0;
  virtual int width_of_string(char* str) = 0;
  virtual int width_of_char(char c)      = 0;
  virtual int width_of_space()           = 0;
  virtual int infinity()                 = 0;

  // VM printing
  void print();
};

// Default pretty-printer stream
class defaultPrettyPrintStream : public prettyPrintStream {
 public:
  defaultPrettyPrintStream() : prettyPrintStream() {}
  void indent();
  void print(char* str);
  void print_char(char c);
  int width_of_string(char* str);
  void space();
  void newline();
  void begin_highlight();
  void end_highlight();

  int width()               { return 100;           }
  int remaining()           { return width() - pos; }
  int width_of_char(char c) { return 1;             }
  int width_of_space()      { return 1;             }
  int infinity()            { return width();       }
};

class byteArrayPrettyPrintStream: public defaultPrettyPrintStream {
 private:
  GrowableArray<int>* _buffer;

 public:
  byteArrayPrettyPrintStream();
  void newline();
  void print_char(char c);
  byteArrayOop asByteArray();
};

// Pretty printing is done by first constructing a pseudo abstract syntax tree
// based on the byte codes of the method and then printing the AST.
// A simple ad-hoc strategy is used for splitting!

class prettyPrinter : AllStatic {
 public:
  // Pretty prints the method with the bci highlighted.
  // If output is NULL a default stream is used.

  static void print(int index, 
                    deltaVFrame* fr,
                    prettyPrintStream* output = NULL);

  static void print_body(
                    deltaVFrame* fr,
                    prettyPrintStream* output = NULL);

  static void print(methodOop          method, 
                    klassOop           klass  = NULL,
                    int                bci    = -1,
                    prettyPrintStream* output = NULL);

  // Pretty prints the method with the bci highlighted into a byteArray.
  static byteArrayOop print_in_byteArray(methodOop method,
  					 klassOop  klass  = NULL,
  					 int       bci    = -1);
};
