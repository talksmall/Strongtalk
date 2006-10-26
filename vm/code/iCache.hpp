/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.3 $ */
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

#ifdef DELTA_COMPILER

// Handle "self-modifying" code on processors with separate I-caches.
// To minimize the performance penalty of the flushes, the VM always
// does a sequence of selective flushes followed by a flushICache().
// If the CPU supports line-by-line flushes, implement flushICacheWord,
// otherwise flushICache.

#define HAVE_LINE_FLUSH		/* x86: don't need any flushing */


// For processors with a small I-cache / without selective cache invalidation,
// define flushICache to flush the entire I-cache.  Otherwise, make it a no-op.
# ifdef HAVE_LINE_FLUSH
  inline void flushICache() {}
# else
  void flushICache();
# endif

// For processors with selective cache invalidation, define the following two
// routines:
# ifdef HAVE_LINE_FLUSH
  void flushICacheWord(void* addr);		// flush one word (instruction)
  void flushICacheRange(void* start, void* end);// flush range [start, end)
# else
  inline void flushICacheWord(void* addr) { Unused(addr); }
  inline void flushICacheRange(void* start, void* end) {
    Unused(start); Unused(end); }
# endif

#endif
