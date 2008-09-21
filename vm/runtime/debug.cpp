/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.21 $ */
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

#include "incls/_precompiled.incl"
#include "incls/_debug.cpp.incl"


// definition of boolean flags

#define MATERIALIZE_BOOLEAN_FLAG(name,value,doc) \
  bool name = value; 

APPLY_TO_BOOLEAN_FLAGS(MATERIALIZE_BOOLEAN_FLAG,MATERIALIZE_BOOLEAN_FLAG)


// definition of integer flags

#define MATERIALIZE_INTEGER_FLAG(name,value,doc) \
  int name = value; 

APPLY_TO_INTEGER_FLAGS(MATERIALIZE_INTEGER_FLAG,MATERIALIZE_INTEGER_FLAG)


// boolean flag table

struct boolFlag {
  char* name;
  bool* value_ptr;
  bool  default_value;
  char* doc;
};


#ifdef PRODUCT
#define MATERIALIZE_BOOLEAN_FLAG_STRUCT_FOR_DEVELOP(name,value,doc) \
  // do nothing - flag doesn't show up in table in product version
#else
#define MATERIALIZE_BOOLEAN_FLAG_STRUCT_FOR_DEVELOP(name,value,doc) \
  { XSTR(name), &name, value, doc },
#endif

#define MATERIALIZE_BOOLEAN_FLAG_STRUCT_FOR_PRODUCT(name,value,doc) \
  { XSTR(name), &name, value, doc },

static boolFlag boolTable[] = {
  APPLY_TO_BOOLEAN_FLAGS(MATERIALIZE_BOOLEAN_FLAG_STRUCT_FOR_DEVELOP,MATERIALIZE_BOOLEAN_FLAG_STRUCT_FOR_PRODUCT) 
  {0, NULL, false, NULL } // indicates end of table
};


inline bool str_equal(char* s, char* q, int len) {
  // s is null terminated, q is not!
  if (strlen(s) != (unsigned int) len) return false;
  return strncmp(s, q, len) == 0;
}


bool debugFlags::boolAt(char* name, int len, bool* value) {
  for (boolFlag* current = &boolTable[0]; current->name; current++) {
    if (str_equal(current->name, name, len)) {
      *value = *current->value_ptr;
      return true;
    }
  }
  return false;
}


bool debugFlags::boolAtPut(char* name, int len, bool* value) {
  if (str_equal("TracePrims", name, len)) {
    TraceOopPrims             = *value;
    TraceDoublePrims          = *value;
    TraceByteArrayPrims       = *value;
    TraceDoubleByteArrayPrims = *value;
    TraceObjArrayPrims        = *value;
    TraceSmiPrims             = *value;
    TraceProxyPrims           = *value;
    TraceBehaviorPrims        = *value;
    TraceBlockPrims           = *value;
    TraceDebugPrims           = *value;
    TraceSystemPrims          = *value;
    TraceProcessPrims         = *value;
    TraceCallBackPrims        = *value;
    TraceMethodPrims          = *value;
    TraceMixinPrims           = *value;
    *value = !*value;
    return true;
  }
  for (boolFlag* current = &boolTable[0]; current->name; current++) {
    if (str_equal(current->name, name, len)) {
      bool old_value = *current->value_ptr;
      *current->value_ptr = *value; 
      *value = old_value;
      return true;
    }
  }
  return false;
}


// integer flag table

struct intFlag {
  char* name;
  int*  value_ptr;
  int   default_value;
  char* doc;
};


#ifdef PRODUCT
#define MATERIALIZE_INTEGER_FLAG_STRUCT_FOR_DEVELOP(name,value,doc) \
  // do nothing - flag doesn't show up in table in product version
#else
#define MATERIALIZE_INTEGER_FLAG_STRUCT_FOR_DEVELOP(name,value,doc) \
  { XSTR(name), &name, value, doc },
#endif

#define MATERIALIZE_INTEGER_FLAG_STRUCT_FOR_PRODUCT(name,value,doc) \
  { XSTR(name), &name, value, doc },

static intFlag intTable[] = {
  APPLY_TO_INTEGER_FLAGS(MATERIALIZE_INTEGER_FLAG_STRUCT_FOR_DEVELOP,MATERIALIZE_INTEGER_FLAG_STRUCT_FOR_PRODUCT) 
  {0, NULL, 0, NULL } // indicates end of table
};


bool debugFlags::intAt(char* name, int len, int* value) {
  for (intFlag* current = &intTable[0]; current->name; current++) {
    if (str_equal(current->name, name, len)) {
      *value = *current->value_ptr;
      return true;
    }
  }
  return false;
}


bool debugFlags::intAtPut(char* name, int len, int* value) {
  for (intFlag* current = &intTable[0]; current->name; current++) {
    if (str_equal(current->name, name, len)) {
      int old_value = *current->value_ptr;
      *current->value_ptr = *value; 
      *value = old_value;
      return true;
    }
  }
  return false;  
}


// printing

void debugFlags::printFlags() {
  for (boolFlag* b = &boolTable[0]; b->name; b++)
    lprintf("%30s = %s\n", b->name, *b->value_ptr ? "true" : "false");
  for (intFlag* i = &intTable[0]; i->name; i++)
    lprintf("%30s = %d\n", i->name, *i->value_ptr);
}


void debugFlags::print_on(outputStream* st) {
  // Boolean flags
  for (boolFlag* b = &boolTable[0]; b->name; b++)
    st->print_cr("%s%s",  *b->value_ptr ? "+" : "-");

  // Integer flags
  for (intFlag* i = &intTable[0]; i->name; i++)
    st->print_cr("%s=%d",  i->name, *b->value_ptr);
}


void print_diff_on(outputStream* st) {
  // Boolean flags
  for (boolFlag* b = &boolTable[0]; b->name; b++) {
    if (*b->value_ptr != b->default_value)
      st->print_cr("%s%s",  *b->value_ptr ? "+" : "-");
  }

  // Integer flags
  for (intFlag* i = &intTable[0]; i->name; i++) {
    if (*b->value_ptr != b->default_value)
      st->print_cr("%s=%d",  i->name, *b->value_ptr);
  }
}


bool BeingDebugged = false;
