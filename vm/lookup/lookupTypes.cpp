/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.5 $ */
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
# include "incls/_lookupTypes.cpp.incl"

# include <string.h>

#ifdef junk

void printLookupType(LookupType l) {
  lprintf(lookupTypeName(l)); 
}

static void addFlag(bool& flag, char* name, const char* add) {
  if (!flag) strcat(name, " { ");
  flag = true;
  strcat(name, add);
}

char* lookupTypeName(LookupType l) {
  char* name = NEW_RESOURCE_ARRAY(char, 80);
  switch (withoutExtraBits(l)) {
   case NormalLookupType:
    strcpy(name, "NormalLookup");
    break;
   case SelfLookupType:
    strcpy(name, "SelfLookup");
    break;
   case SuperLookupType:
    strcpy(name, "SuperLookup");
    break;
   default:
    fatal("Unknown lookupType");
   }
  bool hasFlag = false;
  switch (countType(l)) {
   case NonCounting: break;
   case Counting:    addFlag(hasFlag, name, "counting "); break;
   case Comparing:   addFlag(hasFlag, name, "comparing "); break;
   default: 	     fatal1("invalid count type %ld", countType(l));
  }
  if (isSet(l, DirtySendBit)) addFlag(hasFlag, name, "dirty ");
  if (isSet(l, OptimizedSendBit)) addFlag(hasFlag, name, "optimized ");
  if (isSet(l, UninlinableSendBit)) addFlag(hasFlag, name, "uninlinable ");
  if (hasFlag) strcat(name, "}");
    
  return name;
}

void kinds_init() {
  assert((LookupTypeMask >> LookupTypeSize) == 0, "wrong LookupTypeSize");
  assert((CountTypeMask >> CountTypeSize) == 0, "wrong CountTypeSize");
}  
#endif