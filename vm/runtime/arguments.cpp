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

# include "incls/_precompiled.incl"
# include "incls/_arguments.cpp.incl"
# include <ctype.h>

// Recipe for setting flags for the VM:
// 1. Read the file .deltarc
// 2. Process the arguments

//char* boot_filename = "delta.bst";
char* boot_filename = SYSTEM_NAME ".bst";
char* rc_filename   = "." SYSTEM_NAME "rc";

static void set_bool_flag(char* name, bool value) {
  bool s = value;
  if (!debugFlags::boolAtPut(name, &s))
    fprintf(stderr, "Boolean flag %s unknown.\n", name);
}

static void set_int_flag(char* name, int value) {
  int v = value;
  if (!debugFlags::intAtPut(name, &v))
    fprintf(stderr, "Integer flag %s unknown.\n", name);
}

static void process_token(char* token) {
       if (token[0] == '-') set_bool_flag(&token[1], false);
  else if (token[0] == '+') set_bool_flag(&token[1], true);
  else {
    char name[100];
    int  value;
    if (sscanf(token, "%[a-zA-Z]=%d", name, &value) == 2) {
      set_int_flag(name, value);
    }
  }
}

void process_settings_file(char* file_name, bool quiet) {
  FILE* stream = fopen(file_name, "rb");
  if (stream == NULL) {
    if (quiet) return;
    fprintf(stderr, "Could not open %s\n", file_name);
    exit(-1);
  }

  char token[1024];
  int  pos = 0;

  bool in_white_space = true;
  bool in_comment     = false;

  int c = getc(stream);
  while(c != EOF) {
    if (in_white_space) {
      if (in_comment) {
	if (c == '\n') in_comment = false;
      } else {
        if (c == '#') in_comment = true;
        else if (!isspace(c)) {
          in_white_space = false;
	  token[pos++] = c;
        }
      }
    } else {
      if (isspace(c)) {
        token[pos] = '\0';
        process_token(token);
	pos = 0;
	in_white_space = true;
      } else {
        token[pos++] = c;
      }
    }
    c = getc(stream);
  }
  if (pos>0) {
    token[pos] = '\0';
    process_token(token);
  }
  fclose(stream);
}

void print_credits() {
  // string minimaly encrypted to make it more difficult to
  // tamper with it by looking for it in the executable...
  const char credits[] = "\
\x11\x5a\x04\x1e\x7a\x76\x08\x38\x0b\x6e\x5b\x50\x1e\x04\x2e\x34\x6b\x21\x47\x2e\
\x73\x57\x41\x7b\x65\x18\x3c\x54\x03\x2e\x79\x0e\x1a\x49\x17\x1f\x2f\x44\x45\x73\
\x75\x21\x7e\x5c\x00\x11\x6a\x53\x5b\x3a\x43\x3e\x7e\x5b\x08\x29\x1a\x4c\x1d\x53\
\x13\x22\x73\x0e\x1a\x49\x1f\x1e\x2d\x5b\x05\x37\x0b\x6e\x49\x4b\x01\x45\x02\x7b\
\x4c\x3f\x5c\x21\x7e\x3c\
";
  int mask = 0xa729b65d;
  for (int i = 0; i < sizeof(credits) - 1; i++) {
    fputc((credits[i] ^ mask) & 0x7f, stdout);
    mask = (mask << 1) | (mask >> 31) & 1; // rotate mask
  }
}

void parse_arguments(int argc, char* argv[]) {
  bool parse_files = true;

  if (argc > 1 && strcmp(argv[1], "-t") == 0) {
    fprintf(stdout, "Timers turned off, flags file and -f arguments are ignored.\n");
    UseTimers   = false;
    EnableTasks = false;
    parse_files = false;
  } 

  if (parse_files) {
    process_settings_file(rc_filename, true);
  }

  for (int index = parse_files ? 1 : 2; index < argc; index++) {
    if (strcmp(argv[index], "-?") == 0) {
      debugFlags::printFlags();
      exit(0);
    } else if (strcmp(argv[index], "-credits") == 0) {
      print_credits();
      exit(0);
    } else if (strcmp(argv[index], "-b") == 0) {
      index++;
      if (index >= argc) {
        fprintf(stderr, "file name expected after '-b'\n");
	exit(-1);
      }
      boot_filename = argv[index];
    } else if (strcmp(argv[index], "-f") == 0) {
      index++;
      if (index >= argc) {
        fprintf(stderr, "file name expected after '-f'\n");
	exit(-1);
      }
      if (parse_files) {
        process_settings_file(argv[index]);
      }
    } else process_token(argv[index]);
  }
}
