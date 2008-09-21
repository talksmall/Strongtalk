/* Copyright 1996, LongView Technologies L.L.C. $Revision: 1.18 $ */
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

# include "incls/_inliningdb.cpp.incl"

char* InliningDatabase::_directory = NULL;

char* InliningDatabase::default_directory() {
  return ".\\Inlining";
}

void InliningDatabase::set_directory(char* dir) {
  _directory = dir;
}

char* InliningDatabase::directory() {
  return _directory == NULL 
       ? default_directory() 
       : _directory;
}

# include <ctype.h>

const char  quote = '_';
const char* quote_string = "_\\/:; *?~|><,+=@%&!-";

char* InliningDatabase::mangle_name(char *str) {
  char* result = NEW_RESOURCE_ARRAY(char, 100);
  int i = 0;
  int j = 0;
  while (str[i] != '\0') {
    int c = str[i];
    if (strchr(quote_string,c)) {
      result[j++] = quote;
      result[j++] = get_unsigned_bitfield(c, 6, 3) + '0';
      result[j++] = get_unsigned_bitfield(c, 3, 3) + '0';
      result[j++] = get_unsigned_bitfield(c, 0, 3) + '0';
    } else if (isupper(c)) {
      result[j++] = quote;
      result[j++] = c + ('a' - 'A');
    } else {
      result[j++] = c;
    } 
    i++;
  }
  result[j++] = '\0';
  return result;
}

char* InliningDatabase::unmangle_name(char* str) {
  char* result = NEW_RESOURCE_ARRAY(char, 100);
  int i = 0;
  int j = 0;
  while (str[i] != '\0') {
    int c = str[i];
    if (c == quote) {
      i++;
      assert(str[i] != '\0', "we cannot end with a quote");
      c = str[i];
      if (isdigit(c)) {
        int value = (c - '0') << 6;
        i++;
        assert(str[i] != '\0', "we cannot end with a quote");
        c = str[i];
	value += (c - '0') << 3;
        i++;
        assert(str[i] != '\0', "we cannot end with a quote");
        c = str[i];
	value += (c - '0');
        result[j++] = value;
      } else {
        result[j++] = c - ('a' - 'A');
      }
    } else {
      result[j++] = c;
    }
    i++;
  }
  result[j++] = '\0';
  return result;
}

char* InliningDatabase::selector_string(symbolOop selector) {
  stringStream st(100);
  selector->print_symbol_on(&st);
  return st.as_string();
}

char* InliningDatabase::method_string(methodOop method) {
  stringStream st(100);
  method->print_inlining_database_on(&st);
  return st.as_string();
}

char* InliningDatabase::klass_string(klassOop klass) {
  stringStream st(100);
  klass->klass_part()->print_name_on(&st);
  return st.as_string();
}

bool check_directory(char* dir_name) {
  return os::check_directory(dir_name);
}

char* InliningDatabase::compute_file_name(LookupKey* outer, LookupKey* inner, bool create_directories) {
  char* name = NEW_RESOURCE_ARRAY(char, 1024);

  // Outer key
  char* outer_klass_name    = mangle_name(klass_string(outer->klass()));
  char* outer_selector_name = mangle_name(selector_string(outer->selector()));

  if (create_directories) {
    if (!check_directory(directory())) return NULL;
  }

  strcpy(name, directory());
  strcat(name, "\\");
  strcat(name, outer_klass_name);

  if (create_directories) {
    if (!check_directory(name)) return NULL;
  }

  strcat(name, "\\");
  strcat(name, outer_selector_name);

  if (inner) {
    // Inner key
    char* inner_klass_name  = mangle_name(klass_string(inner->klass()));
    char* inner_method_name = mangle_name(method_string(inner->method()));

    if (create_directories) {
      if (!check_directory(name)) return NULL;
    }
    strcat(name, "\\");
    strcat(name, inner_klass_name);

    if (create_directories) {
      if (!check_directory(name)) return NULL;
    }
    strcat(name, "\\");
    strcat(name, inner_method_name);
  }
  strcat(name, ".txt");
  return name;
}

bool InliningDatabase::file_out(nmethod* nm, outputStream* index_st) {
  ResourceMark rm;

  LookupKey* outer_key = NULL;
  LookupKey* inner_key = NULL;

  if (nm->is_block()) {
    nmethod* outer = nm->outermost();
    if (outer->isZombie()) return false;
    outer_key = &outer->key;
    inner_key = &nm->key;
  } else {
    outer_key = &nm->key;
    inner_key = NULL;
  }


  // construct nmethod's RScope tree; we only want the inlined scopes, so use trusted = false
  RScope* rs = RNonDummyScope::constructRScopes(nm, false);
  // Ignore nmethods with small inlining trees
  if (rs->inlining_database_size() < InliningDatabasePruningLimit) return false;
  

  add_lookup_entry(outer_key, inner_key);

  if (index_st) {
    if (inner_key) {
      inner_key->print_inlining_database_on(index_st);
      index_st->cr();
    }
    outer_key->print_inlining_database_on(index_st);
    index_st->cr();
  }

  char* file_name = compute_file_name(outer_key, inner_key, true);
  if (file_name == NULL) return false;

  if (TraceInliningDatabase) {
    std->print_cr("Dumping %s", file_name);
  }
  fileStream out(file_name);
  if (out.is_open()) {
    GrowableArray<PcDesc*>* uncommon = nm->uncommonBranchList();
    // if (TraceInliningDatabase) rs->printTree(0,0);
    rs->print_inlining_database_on(&out, uncommon);
    return true;
  }
  return false;
}

char* find_type(char* line, bool* is_super, bool* is_block) {
  // Find "::", "^^" or "->"
  char* sub = strstr(line+1, "::");
  if (sub) {
    *is_super = false;
    *is_block = false;
    return sub;
  }

  sub = strstr(line+1, "^^");
  if (sub) {
    *is_super = true;
    *is_block = false;
    return sub;
  }

  sub = strstr(line+1, "->");
  if (sub) {
    *is_super = false;
    *is_block = true;
    return sub;
  }

  return NULL;
}

// Returns whether the key was succesfully scanned
bool scan_key(RScope* sender, char* line, klassOop* receiver_klass, methodOop* method) {
  bool is_super;
  bool is_block;

  char* sub = find_type(line, &is_super, &is_block);
  if (sub == NULL)
    return false;

  *sub = '\0';

  char* class_name = line;
  char* method_id  = sub + 2;
  
  bool class_side = false;
  char* class_start = strstr(class_name, " class");
  if (class_start != NULL) {
    *class_start = '\0';
    class_side = true;
  }

  klassOop rec = klassOop(Universe::find_global(class_name, true));
  if (rec == NULL || !rec->is_klass()) return false;
  if (class_side) rec = rec->klass();
  *receiver_klass = rec;
 
  GrowableArray<int>* bcis = new GrowableArray<int>(10);

  char* bcis_string = strstr(method_id, " ");

  if (bcis_string) {
    *bcis_string++ = '\0';
    while (*bcis_string != '\0') {
      int index;
      int bci;
      if (sscanf(bcis_string, "%d%n", &bci, &index) != 1) return 0;
      bcis->push(bci);
      bcis_string += index;
      if (*bcis_string == ' ') 
        bcis_string++;
    }
  }
  symbolOop selector = oopFactory::new_symbol(method_id);


  if (is_super) {
    assert(sender, "sender must be present");
    klassOop method_holder = sender->receiverKlass()->klass_part()->lookup_method_holder_for(sender->method());
    
    if (method_holder) {
      methodOop met = method_holder->klass_part()->superKlass()->klass_part()->lookup(selector);
      if (met) {
        *method = met;
	return true;
      }
    }
    return false;
  }

  methodOop met = rec->klass_part()->lookup(selector);
  if (met == NULL) return false;

  for (int index = 0; index < bcis->length(); index++) {
    int bci = bcis->at(index);
    met = met->block_method_at(bci);
    if (met == NULL) return false;
  }

  *method = met;
  return true;
}

// Returns the index where the scan terminated.
// index is 0 is the scan failed
int scan_prefix(char* line, int* bci, int* level) {
  int index;

  int l = 0;
  while (*line == ' ') {
    line++;
    l++;
  }
  if (sscanf(line, "%d %n", bci, &index) != 1) return 0;
  *level = l / 2;
  return l + index;
}

// Returns whether the uncommon word was succesfully scanned
bool scan_uncommon(char* line) {
  return strcmp(line, "uncommon") == 0;
}

static bool create_rscope(char* line, GrowableArray<RDatabaseScope*>* stack) {
  // remove the cr
  int len = strlen(line);
  if (len > 1 && line[len-1] == '\n') 
    line[len-1] = '\0';

  int       bci            = 0;
  int       level          = 0;
  methodOop method         = NULL;
  klassOop  receiver_klass = NULL;

  RScope* result = NULL;

  if (stack->isEmpty()) {
    // the root scope
    if (!scan_key(NULL, line, &receiver_klass, &method)) return false;
    stack->push(new RDatabaseScope(NULL, -1, receiver_klass, method, 0));
  } else {
    // sub scope
    int index = scan_prefix(line, &bci, &level);
    if (index <= 0) return false;

    while (stack->length() > level) 
      stack->pop();
    RDatabaseScope* sender = stack->top();
    if (scan_uncommon(&line[index])) {
      sender->mark_as_uncommon(bci);
    } else if (scan_key(sender, &line[index], &receiver_klass, &method)) {
      stack->push(new RDatabaseScope(sender, bci, receiver_klass, method, level));
    } else {
      return false;
    }
  }
  return true;
}

outputStream* InliningDatabase::index_st = NULL;

int      InliningDatabase::local_number_of_nmethods_written = 0;
klassOop InliningDatabase::local_klass                      = NULL;

void InliningDatabase::local_file_out_all(nmethod* nm) {
  if (nm->isZombie()) return;
  if (file_out(nm)) {
    local_number_of_nmethods_written++;
  }
}

char* InliningDatabase::index_file_name() {
  char* name = NEW_RESOURCE_ARRAY(char, 1024);
  if (!check_directory(directory())) return NULL;
  strcpy(name, directory());
  strcat(name, "\\index.txt");
  return name;
}

bool scan_key(char* line, LookupKey* key) {
  int len = strlen(line);
  if (len > 1 && line[len-1] == '\n') 
    line[len-1] = '\0';

  bool is_super;
  bool is_block;

  char* sub = find_type(line, &is_super, &is_block);
  if (sub == NULL)
    return false;

  *sub = '\0';

  char* class_name = line;
  char* method_id  = sub + 2;
  
  bool class_side = false;
  char* class_start = strstr(class_name, " class");
  if (class_start != NULL) {
    *class_start = '\0';
    class_side = true;
  }

  klassOop rec = klassOop(Universe::find_global(class_name, true));
  if (rec == NULL || !rec->is_klass()) return false;
  if (class_side) rec = rec->klass();


  GrowableArray<int>* bcis = new GrowableArray<int>(10);

  char* bcis_string = strstr(method_id, " ");

  if (bcis_string) {
    *bcis_string++ = '\0';
    while (*bcis_string != '\0') {
      int index;
      int bci;
      if (sscanf(bcis_string, "%d%n", &bci, &index) != 1) return 0;
      bcis->push(bci);
      bcis_string += index;
      if (*bcis_string == ' ') 
        bcis_string++;
    }
  }
  symbolOop selector = oopFactory::new_symbol(method_id);
 
  if (is_block) {
    methodOop met = rec->klass_part()->lookup(selector);
    if (met == NULL) return false;
    for (int index = 0; index < bcis->length(); index++) {
      int bci = bcis->at(index);
      met = met->block_method_at(bci);
      if (met == NULL) return false;
    }
    key->initialize(rec, met);
  } else {
    key->initialize(rec, selector);
  }
  return true;
}


void InliningDatabase::load_index_file() {
  ResourceMark rm;
  TraceTime t("Loading index for inlining database");

  // Open the file
  FILE* stream = fopen(index_file_name(), "rt");
  if (!stream) return;

  char line[1000];

  LookupKey first;
  LookupKey second;

  while (fgets( line, 1000, stream)) {
    if (scan_key(line, &first)) {
      if (first.is_block_type()) {
        if (fgets( line, 1000, stream)) {
          if (scan_key(line, &second)) {
	    // std->print("Block ");
            // first.print_on(std);
	    // std->print(" outer ");
            // second.print_on(std);
            // std->cr();
            add_lookup_entry(&second, &first);
	  } else {
            std->print_cr("Index file parsing block failed for %s", line);
	  }
	}
      } else {
	// std->print("Method ");
        // first.print_on(std);
        // std->cr();
        add_lookup_entry(&first);
      }
    } else {
      std->print_cr("Index file parsing failed for %s", line);
    }
  }
  fclose(stream);
}

void InliningDatabase::local_file_out_klass(nmethod* nm) {
  if (nm->isZombie()) return;
  if (nm->receiver_klass() == local_klass) {
    if (file_out(nm)) {
      local_number_of_nmethods_written++;
    }
  }
}

int InliningDatabase::file_out(klassOop klass) {
  local_number_of_nmethods_written = 0;
  local_klass                      = klass;
  Universe::code->nmethods_do(local_file_out_klass);
  return local_number_of_nmethods_written;
}

RScope* InliningDatabase::file_in_from(FILE* stream) {
  GrowableArray<RDatabaseScope*>* stack = new GrowableArray<RDatabaseScope*>(10);

  char line[1000];

  // Read the first top scope
  if (!fgets( line, 1000, stream)) return NULL;
  if (!create_rscope(line, stack)) return NULL;

  // Read the sub scopes
  while (fgets( line, 1000, stream)) {
    if (!create_rscope(line, stack)) return NULL;
  }

  // Return the top scope
  return stack->at(0);
}

RScope* InliningDatabase::file_in(char* file_path) {
  // Open the file
  FILE* stream = fopen(file_path, "rt");

  if (!stream) {
    return NULL;
  }
  RScope* result = file_in_from(stream);
  fclose(stream);
  return result;
}

RScope* InliningDatabase::file_in(LookupKey* outer, LookupKey* inner) {
  char* file_name = compute_file_name(outer, inner, false);
  if (file_name == NULL) {
    if (TraceInliningDatabase) {
      std->print("Failed opening file for ");
      if (inner) {
        inner->print();
        std->print(" ");
      }
      outer->print();
      std->cr();
    }
    return NULL;
  }
  RScope* result = file_in(file_name);
  
  if (TraceInliningDatabase && result == NULL) {
    std->print("Failed parsing file for ");
    if (inner) {
      inner->print();
      std->print(" ");
    }
    outer->print();
    std->cr();
  }
 
  return result;
}

class InliningDatabaseKey {
 public:
  LookupKey outer;
  LookupKey inner;

  bool is_empty()   const { return outer.selector_or_method() == smiOop_zero; }
  bool is_filled()  const { return smiOop(outer.klass())      != smiOop_zero; }
  bool is_deleted() const { return outer.selector_or_method() == smiOop_one;  }

  bool is_inner()   const { return inner.selector_or_method() != smiOop_zero; }
  bool is_outer()   const { return !is_inner(); }

  void clear() {
    outer.initialize(NULL, smiOop_zero);
    inner.initialize(NULL, smiOop_zero);
  }

  void set_deleted() {
    outer.initialize(NULL, smiOop_one);
  }

  bool equal(LookupKey* o, LookupKey* i) {
    return outer.equal(o) && (is_outer() || inner.equal(i));
  }

  void oops_do(void f(oop*)) {
    if (is_filled()) {
     outer.oops_do(f);
     if(is_inner()) 
       inner.oops_do(f);
    }
  }
};

InliningDatabaseKey*   InliningDatabase::table = NULL;
unsigned int InliningDatabase::table_size      = 0;
unsigned int InliningDatabase::table_size_mask = 0;
unsigned int InliningDatabase::table_no        = 0;

inline unsigned int InliningDatabase::index_for(LookupKey* outer, LookupKey* inner) {
  unsigned int hash = (unsigned int) outer->klass()->identity_hash() ^ (unsigned int) outer->selector()->identity_hash();
  if (inner) {
    hash ^= (unsigned int) inner->klass()->identity_hash() ^ (unsigned int) inner->selector()->identity_hash();
  }
  return hash & table_size_mask;
}

inline unsigned int InliningDatabase::next_index(unsigned int index) {
  return (index + 1) & table_size_mask;
}

void InliningDatabase::reset_lookup_table() {
  if (table) {
    FreeHeap(table);
    table           = NULL;
    table_size      = 0;
    table_size_mask = 0;
    table_no        = 0;
  }
}

RScope* InliningDatabase::select_and_remove(bool* end_of_table) {
  if (table_no == 0) return NULL;  // Skim the cream

  for (unsigned int index = 0 ; index < table_size; index++) {
    if (table[index].is_filled() && table[index].is_outer()) {
      RScope* result = file_in(&table[index].outer);
      table[index].set_deleted();
      table_no--;
      *end_of_table = false;
      return result;
    }
  }
  *end_of_table = true;
  return NULL;
}

void InliningDatabase::allocate_table(unsigned int size) {
  if (TraceInliningDatabase) {
    std->print_cr("InliningDatabase::allocate_table(%d)", size);
  }
  table_size      = size;
  table_size_mask = size - 1;
  table_no        = 0;
  table           = NEW_C_HEAP_ARRAY(InliningDatabaseKey, table_size);
  // clear the table
  for (unsigned int index = 0 ; index < table_size; index++) {
    table[index].clear();
  }
}

void InliningDatabase::add_lookup_entry(LookupKey* outer, LookupKey* inner) {
  if (table_no * 2 >= table_size) {
    if (table == NULL) {
      allocate_table(4 * K);
    } else {
      // Expand table
      InliningDatabaseKey* old_table      = table;
      unsigned int         old_table_size = table_size;
      allocate_table(table_size * 2);
      for (unsigned int index = 0 ; index < old_table_size; index++) {
        if (old_table[index].is_filled())
	  add_lookup_entry(&old_table[index].outer, &old_table[index].inner);
      }
      FreeHeap(old_table);
    }
  }
  assert(table_no * 2 < table_size, "just checking density");
  
  unsigned int index = index_for(outer, inner);

  while (table[index].is_filled()) {
    if (table[index].equal(outer, inner)) return;
    index = next_index(index);
  }

  table[index].outer = *outer;
  if (inner) {
    table[index].inner = *inner;
  }
  table_no++;

  if (TraceInliningDatabase) {
    std->print_cr("InliningDatabase::add_lookup_entry @ %d", index);
    if (inner) {
      inner->print();
      std->print(" ");
    }
    outer->print();
    std->cr();
  }
}

bool InliningDatabase::lookup(LookupKey* outer, LookupKey* inner) {
  if (table_no == 0) return NULL;  // Skim the cream

  unsigned int index = index_for(outer, inner);
  if (!table[index].is_filled()) return false; 
  while (!table[index].equal(outer, inner)) {
    index = next_index(index);
    if (table[index].is_empty()) return false;
  }
  return true;
}


RScope* InliningDatabase::lookup_and_remove(LookupKey* outer, LookupKey* inner) {
  if (table_no == 0) return NULL;  // Skim the cream

  unsigned int index = index_for(outer, inner);
  if (!table[index].is_filled()) return NULL; 
  while (!table[index].equal(outer, inner)) {
    index = next_index(index);
    if (table[index].is_empty()) return NULL;
  }
  table[index].set_deleted();
  table_no--;
  return file_in(outer, inner);
}

void InliningDatabase::oops_do(void f(oop*)) {
  for (unsigned int index = 0; index < table_size; index++) {
    table[index].oops_do(f);
  }
}

bool InliningDatabase::file_out_all() {
  ResourceMark rm;

  // The lookup table is used to create the index file.

  // Flush the lookup table
  reset_lookup_table();

  // File in the current index file.
  load_index_file();

  local_number_of_nmethods_written = 0;
  Universe::code->nmethods_do(local_file_out_all);

  fileStream index(index_file_name());

  // File out the index file.
  for (unsigned int i = 0 ; i < table_size; i++) {
    if (table[i].is_filled()) {
      if (table[i].is_inner()) {
        table[i].inner.print_inlining_database_on(&index);
        index.cr();
      }
      table[i].outer.print_inlining_database_on(&index);
      index.cr();
    }
  }

  // Flush the lookup table
  reset_lookup_table();

  return local_number_of_nmethods_written ? true : false;
}
