/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.53 $ */
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
# include "incls/_bootstrap.cpp.incl"

#include <io.h>

bootstrap::bootstrap(char* name) {
  file_name = name;
  _has_error = false;
  open_file();
  if (!has_error()) {
    initialize_tables(file_size > 0 ? (file_size / 32) : (10 * K));
    parse_file();
    close_file();
    Universe  ::cleanup_after_bootstrap();
  }
}

void bootstrap::initialize_tables(int initial_table_size) {
  oop_table          = NEW_C_HEAP_ARRAY(oop, initial_table_size);
  max_number_of_oops = initial_table_size;
  number_of_oops     = 0;
  position           = 0;
}

bootstrap::~bootstrap() {
  FreeHeap(oop_table);
}

oop bootstrap::at(int index) {
  if (index < 0 || index > number_of_oops)
    error("bootstrap oop table overflow");
  return oop_table[index];
}

void bootstrap::add(oop obj) {
  if (number_of_oops >= max_number_of_oops) {
    int new_size = max_number_of_oops  * 2;
    printf("Expanding boot table to %d\n", new_size);
    oop* new_oop_table = NEW_C_HEAP_ARRAY(oop, new_size);
    for(int index = 0; index < max_number_of_oops; index++)
      new_oop_table[index] = oop_table[index];
   
    max_number_of_oops = new_size;
    FreeHeap(oop_table);
    oop_table = new_oop_table;
  }
  oop_table[number_of_oops++] = obj;
}

void bootstrap::open_file() {
  stream = fopen(file_name, "rb");
  if (stream == NULL) {
    _has_error = true;
    lprintf("\nCould not open file (%s) for reading!\n", file_name);
    exit(-1);
  }
  int no    = _fileno(stream);
  file_size = _filelength(no);
}

char bootstrap::get_char() {
  position++;
  return getc(stream);
}

int bootstrap::get_integer() {
  position++;
  int result = getc(stream);
  if (result == EOF) fatal("end of file");
  assert(result >= 0, "must be positive");
  if (result < 128 ) return result;
  return get_integer() * 128 + result - 128;
}

void bootstrap::parse_file() {
  // Check version number
  int version_number = get_integer();

  if (version_number > 100) {
    _new_format = true;
    lprintf(", n");
    version_number -= 100;
  } else {
    _new_format = false;
  }

  if (Bytecodes::version() == version_number) {

  } else {
    lprintf("\n");
    lprintf("Bytecode version conflict\n");
    lprintf(" excpected: %d\n", Bytecodes::version());
    lprintf(" received:  %d\n", version_number);
    exit(-1);
  }

  Universe::_systemDictionaryObj         = objArrayOop(get_object());
  nilObj                                 = memOop(get_object());
  trueObj                                = memOop(get_object());
  falseObj                               = memOop(get_object());
  smiKlassObj                            = klassOop(get_object());
  Universe::_memOopKlassObj              = klassOop(get_object());
  Universe::_objArrayKlassObj            = klassOop(get_object());
  Universe::_byteArrayKlassObj           = klassOop(get_object());
  symbolKlassObj                         = klassOop(get_object());
  doubleKlassObj                         = klassOop(get_object());
  Universe::_methodKlassObj              = klassOop(get_object());
  Universe::_associationKlassObj         = klassOop(get_object());
  zeroArgumentBlockKlassObj              = klassOop(get_object());
  oneArgumentBlockKlassObj               = klassOop(get_object());
  twoArgumentBlockKlassObj               = klassOop(get_object());
  threeArgumentBlockKlassObj             = klassOop(get_object());
  fourArgumentBlockKlassObj              = klassOop(get_object());
  fiveArgumentBlockKlassObj              = klassOop(get_object());
  sixArgumentBlockKlassObj               = klassOop(get_object());
  sevenArgumentBlockKlassObj             = klassOop(get_object());
  eightArgumentBlockKlassObj             = klassOop(get_object());
  nineArgumentBlockKlassObj              = klassOop(get_object());
  contextKlassObj                        = klassOop(get_object());
  Universe::_asciiCharacters             = objArrayOop(get_object());
  Universe::_vframeKlassObj              = HasActivationClass ? klassOop(get_object()) : klassOop(nilObj);
}

void bootstrap::insert_symbol(memOop obj) {
  if (Universe::symbol_table->is_present(symbolOop(obj))) {
    lprintf("Symbol ");
    symbolOop(obj)->print_value();
    lprintf(" already present in symbol_table!\n");
  } else {
    Universe::symbol_table->add_symbol(symbolOop(obj));
  }
}

# define KLASS_CASE(func)  func(klassOop(m)->klass_part()); klassOop(m)->bootstrap_object(this); break;
# define OBJECT_CASE(cast) cast(m)->bootstrap_object(this); break;
# define OBJECT_ERROR(str) fatal(str); break;
# define SYMBOL_CASE(cast) cast(m)->bootstrap_object(this); insert_symbol(cast(m)); break;


oop bootstrap::get_object() {
  char type = get_char();
  
  if (type == '0') {
    int v = get_integer();
    // if (TraceBootstrap) std->print_cr("i %d", v);
    return as_smiOop(v);
  }
  if (type == '-') { 
    int v = get_integer();
    // if (TraceBootstrap) std->print_cr("i %d", -v);
    return as_smiOop(-v);
  }
  if (type == '3') {
    int v = get_integer();
    // if (TraceBootstrap) std->print_cr("r %d", v);
    return at(v);
  }

  int    size = get_integer();
  memOop m    = as_memOop(Universe::allocate_tenured(size));

  // Clear eventual padding area for byteArray, symbol, doubleByteArray.
  m->raw_at_put(size-1, smiOop_zero);

  // if (TraceBootstrap) lprintf("%c %d = 0x%lx\n", type, size, m);

  add(m);
  switch (type) {
    // Classes
    case 'A': KLASS_CASE(set_klassKlass_vtbl)
    case 'B': KLASS_CASE(set_smiKlass_vtbl)
    case 'C': KLASS_CASE(set_memOopKlass_vtbl)
    case 'D': KLASS_CASE(set_byteArrayKlass_vtbl)
    case 'E': KLASS_CASE(set_doubleByteArrayKlass_vtbl)
    case 'F': KLASS_CASE(set_objArrayKlass_vtbl)
    case 'G': KLASS_CASE(set_symbolKlass_vtbl)
    case 'H': KLASS_CASE(set_doubleKlass_vtbl)
    case 'I': KLASS_CASE(set_associationKlass_vtbl)
    case 'J': KLASS_CASE(set_methodKlass_vtbl)
    case 'K': KLASS_CASE(set_blockClosureKlass_vtbl)
    case 'L': KLASS_CASE(set_contextKlass_vtbl)
    case 'M': KLASS_CASE(set_proxyKlass_vtbl)
    case 'N': KLASS_CASE(set_mixinKlass_vtbl)
    case 'O': KLASS_CASE(set_weakArrayKlass_vtbl)
    case 'P': KLASS_CASE(set_processKlass_vtbl)
    case 'Q': KLASS_CASE(set_doubleValueArrayKlass_vtbl)
    case 'R': KLASS_CASE(set_vframeKlass_vtbl)
    // Objects
    case 'a': OBJECT_ERROR("klass")
    case 'b': OBJECT_ERROR("smi")
    case 'c': OBJECT_CASE(memOop);
    case 'd': OBJECT_CASE(byteArrayOop);
    case 'e': OBJECT_CASE(doubleByteArrayOop);
    case 'f': OBJECT_CASE(objArrayOop)
    case 'g': SYMBOL_CASE(symbolOop);
    case 'h': OBJECT_CASE(doubleOop);
    case 'i': OBJECT_CASE(associationOop);
    case 'j': OBJECT_CASE(methodOop);
    case 'k': OBJECT_ERROR("blockClosure")
    case 'l': OBJECT_ERROR("context")
    case 'm': OBJECT_ERROR("proxy")
    case 'n': OBJECT_CASE(mixinOop)
    case 'o': OBJECT_ERROR("weakArrayOop")
    case 'p': OBJECT_CASE(processOop)
    default: fatal("unknown object type");
  }

  return m;
}

void bootstrap::close_file() {
  fclose(stream);
}

void bootstrap::read_mark(markOop* mark_addr) {
  char type = get_char();
  markOop m;
  if      (type == '1') m = markOopDesc::untagged_prototype();
  else if (type == '2') m = markOopDesc::tagged_prototype();
  else fatal("expecting a markup");
  *mark_addr = m;
}

double bootstrap::read_double() {
  double value;
  unsigned char* str = (unsigned char*) &value;
  for (int index = 0; index < 8; index++) {
    position++;
    str[index] = getc(stream);
  }
  return value; 
}
