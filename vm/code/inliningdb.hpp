/* Copyright 1996, LongView Technologies L.L.C. $Revision: 1.8 $ */
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

// Interface to the inlining database

class InliningDatabaseKey;

class InliningDatabase : AllStatic {
 private:
  // Helper functions to compute file path when filing out.
  static char* selector_string(symbolOop selector);
  static char* method_string(methodOop method);
  static char* klass_string(klassOop klass);

  // Read the scope structure from a stream
  static RScope* file_in_from(FILE* stream);

  // Helper functions when iterating over zone.
  static int      local_number_of_nmethods_written;
  static void     local_file_out_all(nmethod* nm);
  static klassOop local_klass;
  static void     local_file_out_klass(nmethod* nm);

  // Returns the file name for the two keys and creates the necessary directories 
  static char* compute_file_name(LookupKey* outer, LookupKey* inner, bool create_directories);

  // Returns the file name for the index file
  static char* index_file_name();

  static char* _directory;

  static outputStream* index_st;
  static InliningDatabaseKey* table;
  static unsigned int table_size;      // Size of table power of 2
  static unsigned int table_size_mask; // nthMask(table_size)
  static unsigned int table_no;        // Number of elements in the table
 public:
  // Accessor for the root of the database
  static char* default_directory();
  static void  set_directory(char* dir);
  static char* directory();

  // Writes the inlining structure for all compiled code.
  // Returns the number of written inlining structures. 
  static bool file_out_all();

  // Writes the inlining structure for compiled method.
  // Returns whether the information was written.
  static bool file_out(nmethod* nm, outputStream* index_st = NULL);

  // Writes the inlining structure for all compiled methods with a
  // specific receiver klass, returns the number of written structures. 
  static int file_out(klassOop klass);

  // Reads the inlining structure from file_name.
  // Returns NULL if the attempt failed.
  static RScope* file_in(char* file_name);

  // Reads the inlining structure for receiver_class/selector.
  // Returns NULL if the attempt failed.
  static RScope* file_in(LookupKey* outer, LookupKey* inner = NULL);

  // Converts a string into a mangled name that is a valid filename
  // on the running platform.
  static char* mangle_name(char *str);

  // Converts a mangled string back to the orignal sting. 
  static char* unmangle_name(char* str);

  // the lookup table
  static void    reset_lookup_table();
  static void    add_lookup_entry(LookupKey* outer,  LookupKey* inner = NULL);
  static bool    lookup(LookupKey* outer, LookupKey* inner = NULL);
  static RScope* lookup_and_remove(LookupKey* outer, LookupKey* inner = NULL);
  static RScope* select_and_remove(bool* end_of_table); // For background compilation

  // Index file
  static void load_index_file();

  // Iterates through all oops stored in the inlining database
  static void oops_do(void f(oop*));
 private:
  static void allocate_table(unsigned int size);
  static inline unsigned int index_for(LookupKey* outer, LookupKey* inner);
  static inline unsigned int next_index(unsigned int index);
};