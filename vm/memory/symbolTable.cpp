/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.21 $ */
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
# include "incls/_symbolTable.cpp.incl"

# define FOR_ALL_ENTRIES(entry) \
  for (entry = firstBucket(); entry <= lastBucket(); entry ++)

# define FOR_ALL_SYMBOL_ADDR(bucket, var, code)                         \
    { if (bucket->is_symbol()) {                                        \
         var = (symbolOop*) bucket; code;                               \
      } else {                                                          \
        for (symbolTableLink* l = bucket->get_link(); l; l = l->next) { \
          var = &l->symbol; code;                                       \
        }                                                               \
      }                                                                 \
    }

int hash(char* name, int len) {
  // hash on at most 32 characters, evenly spaced
  int increment;
 
  if (len < 32) {
    increment = 1;
  } else {
    increment = len >> 5;
  }
 
  // hashpjw from Dragon book (ASU p. 436), except increment differently
 
  assert(BitsPerByte * BytesPerWord == 32, "assumes 32-bit words");
  long unsigned h = 0;
  long unsigned g;
  char* s = name;
  char* end = s + len;
  for (; s < end; s = s + increment) {
    h = (h << 4) + (long unsigned) *s;
    if (g = h & 0xf0000000) h ^= g | (g >> 24);
  }
  return h;
}

symbolTable::symbolTable() {
  for (int i = 0; i < symbol_table_size; i ++) buckets[i].clear();
  free_list = first_free_link = end_block = NULL;
}

symbolOop symbolTable::basic_add(char *name, int len, int hashValue) {
  symbolKlass* sk = (symbolKlass*) Universe::symbolKlassObj()->klass_part();
  symbolOop str =   sk->allocateSymbol(name, len);
  basic_add(str, hashValue);
  return str;
}

bool symbolTable::is_present(symbolOop sym) {
  char* name = (char*) sym->bytes();
  int   len  = sym->length();
  int hashValue = hash(name, len);
  symbolTableEntry* bucket = bucketFor(hashValue);
  if (bucket->is_empty()) return false;
  if (bucket->is_symbol())
    return bucket->get_symbol()->equals(name, len);
  for (symbolTableLink* l = bucket->get_link(); l; l = l->next)
    if (l->symbol->equals(name, len)) return true;
  return false;
}

symbolOop symbolTable::lookup(char* name, int len) {
  int hashValue = hash(name, len);
  symbolTableEntry* bucket = bucketFor(hashValue);
  if (!bucket->is_empty()) {
    if (bucket->is_symbol()) {
      if (bucket->get_symbol()->equals(name, len)) return bucket->get_symbol();
    } else {
      for (symbolTableLink* l = bucket->get_link(); l; l = l->next)
	if (l->symbol->equals(name, len)) return l->symbol;
    }
  }
  return basic_add(name, len, hashValue);
}

void symbolTable::add(symbolOop s) {
  assert(s->is_symbol(),
         "adding something that's not a symbol to the symbol table");
  assert(s->is_old(), "all symbols should be tenured");
  int hashValue = hash((char*) s->bytes(), s->length());
  basic_add(s, hashValue);
}

void symbolTable::add_symbol(symbolOop s) {
  basic_add(s, hash((char*) s->bytes(), s->length()));
}

symbolOop symbolTable::basic_add(symbolOop s, int hashValue) {
  assert(s->is_symbol(),
         "adding something that's not a symbol to the symbol table");
  assert(s->is_old(), "all symbols should be tenured");

  // Add the indentity hash for the new symbol
  s->hash_value();
  assert(!s->mark()->has_valid_hash(), "should not have a hash yet");
  s->set_mark(s->mark()->set_hash(s->hash_value()));
  assert(s->mark()->has_valid_hash(), "should have a hash now");

  symbolTableEntry* bucket = bucketFor(hashValue);

  if (bucket->is_empty()) {
    bucket->set_symbol(s);
  } else {
    symbolTableLink*  old_link;
    if (bucket->is_symbol()) {
      old_link = Universe::symbol_table->new_link(bucket->get_symbol());
    } else {
      old_link = bucket->get_link();
    }
    bucket->set_link(Universe::symbol_table->new_link(s, old_link));
  }
  return s;
}

void symbolTable::switch_pointers(oop from, oop to) {
  if (! from->is_symbol()) return;
  assert(to->is_symbol(),
         "cannot replace a symbol with a non-symbol");

  symbolTableEntry* e;
  FOR_ALL_ENTRIES(e) {
    symbolOop* addr;
    FOR_ALL_SYMBOL_ADDR(e, addr, SWITCH_POINTERS_TEMPLATE(addr));
  }
}

void symbolTable::follow_used_symbols() {
  // throw out unreachable symbols
  symbolTableEntry* e;
  FOR_ALL_ENTRIES(e) {
    // If we have a one element list; preserve the symbol but remove the chain
    // This moving around cannot take place after follow_root has been called
    // since follow_root reverse pointers.
    if (e->get_link() && !e->get_link()->next) {
      symbolTableLink* old = e->get_link();
      e->set_symbol(old->symbol);
      delete_link(old);
    }

    if (e->is_symbol()) {
      if (e->get_symbol()->is_gc_marked()) 
        MarkSweep::follow_root((oop*) e);
      else
        e->clear(); // unreachable; clear entry
    } else {
      symbolTableLink** p    = (symbolTableLink**) e;
      symbolTableLink*  link = e->get_link();
      while (link) {
        if (link->symbol->is_gc_marked()) {
          MarkSweep::follow_root((oop*) &link->symbol);
	  p    = &link->next;
          link = link->next;
        } else {
          // unreachable; remove from table
          symbolTableLink* old = link;
          *p   = link->next;
	  link = link->next;
          old->next = NULL;
          delete_link(old);
        }
      }
    }
  }
}

void symbolTableEntry::deallocate() {
  if(!is_symbol() && get_link()) 
    Universe::symbol_table->delete_link(get_link());
}

bool symbolTableEntry::verify(int i) {
  bool flag = true;
  if (is_symbol()) {
    if (!get_symbol()->is_symbol()) {
      error("entry %#lx in symbol table isn't a symbol", get_symbol());
      flag = false;
    }
  } else {
    if (get_link()) flag = get_link()->verify(i);
  }
  return flag;
}

void symbolTable::verify() {
  for (int i = 0; i < symbol_table_size; i ++)
    if (!buckets[i].verify(i))
      lprintf("\tof bucket %ld of symbol table\n", long(i));
}


void symbolTable::relocate() {
  symbolTableEntry* e;
  FOR_ALL_ENTRIES(e) {
    symbolOop* addr;
    FOR_ALL_SYMBOL_ADDR(e, addr, RELOCATE_TEMPLATE(addr));
  }
}

bool symbolTableLink::verify(int i) {
  bool flag = true;
  for (symbolTableLink* l = this; l; l = l->next) {
    if (! l->symbol->is_symbol()) {
      error("entry %#lx in symbol table isn't a symbol", l->symbol);
      flag = false;
    } else if (hash((char*) l->symbol->bytes(), l->symbol->length()) 
               % symbol_table_size != i) {
      error("entry %#lx in symbol table has wrong hash value", l->symbol);
      flag = false;
    } else if (!l->symbol->is_old()) {
      error("entry %#lx in symbol table isn't tenured", l->symbol);
      flag = false;
    }
  }
  return flag;
}

int symbolTableEntry::length() {
  if (is_symbol()) return 1;
  if (!get_link()) return 0;
  int count = 0;
  for (symbolTableLink* l = get_link(); l; l = l->next) count ++;
  return count;
}

symbolTableLink* symbolTable::new_link(symbolOop s, symbolTableLink* n) {
  symbolTableLink* res;
  if (free_list) {
    res = free_list;
    free_list = free_list->next;
  } else {
    const int block_size = 500;
    if (first_free_link == end_block) {
      first_free_link = NEW_C_HEAP_ARRAY(symbolTableLink, block_size);
      end_block = first_free_link + block_size;
    }
    res = first_free_link++;
  } 
  res->symbol = s;
  res->next   = n;
  return res;
}

void symbolTable::delete_link(symbolTableLink* l) {
  // Add the link to the freelist
  symbolTableLink* end = l;
  while(end->next) end = end->next;
  end->next = free_list;
  free_list = l;
}

// much of this comes from the print_histogram routine in mapTable.c,
// so if bug fixes are made here, also make them in mapTable.cpp.
void symbolTable::print_histogram() {
  const int results_length = 100;
  int results[results_length];
  
  // initialize results to zero
  for (int j = 0; j < results_length; j++) {
    results[j] = 0;
  }

  int total = 0;
  int min_symbols = 0;
  int max_symbols = 0;
  int out_of_range = 0;
  for (int i = 0; i < symbol_table_size; i++) {
    symbolTableEntry curr = buckets[i];
    int counter = curr.length();
    total += counter;
    if (counter < results_length) {
      results[counter]++;
    } else {
      out_of_range++;
    }
    min_symbols = min(min_symbols, counter);
    max_symbols = max(max_symbols, counter);
  }
  lprintf("Symbol Table:\n");
  lprintf("%8s %5d\n", "Total  ", total);
  lprintf("%8s %5d\n", "Minimum", min_symbols);
  lprintf("%8s %5d\n", "Maximum", max_symbols);
  lprintf("%8s %3.2f\n", "Average",
	  ((float) total / (float) symbol_table_size));
  lprintf("%s\n", "Histogram:");
  lprintf(" %s %29s\n", "Length", "Number chains that length");
  for (i = 0; i < results_length; i++) {
    if (results[i] > 0) {
      lprintf("%6d %10d\n", i, results[i]);
    }
  }
  int line_length = 70;    
  lprintf("%s %30s\n", " Length", "Number chains that length");
  for (i = 0; i < results_length; i++) {
    if (results[i] > 0) {
      lprintf("%4d", i);
      for (j = 0; (j < results[i]) && (j < line_length);  j++) {
        lprintf("%1s", "*");
      }
      if (j == line_length) {
        lprintf("%1s", "+");
      }
      lprintf("\n");
    }
  }  
  lprintf(" %s %d: %d\n", "Number chains longer than",
	  results_length, out_of_range);
}
