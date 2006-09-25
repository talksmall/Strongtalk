/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.12 $ */
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

// The RecompilationPolicy selects which method (if any) should be recompiled.    

class RecompilationPolicy : public ResourceObj {
 protected:
   GrowableArray<RFrame*>* stack;
   char* msg;				// for (performance) debugging: reason for not going up, etc.

   RFrame* senderOf(RFrame* rf);	// return rf->sender() and update stack if necessary
   RFrame* parentOfBlock(blockClosureOop blk);	// block's parent frame (or NULL)
   RFrame* parentOf(RFrame* rf);	// same for rf->parent()
   RFrame* senderOrParentOf(RFrame* rf);// either sender or parent, depending on various factors
   RFrame* findTopInlinableFrame();
   void	   checkCurrent(RFrame*& current, RFrame*& prev, RFrame*& prevMethod);
   void    fixBlockParent(RFrame* rf);
   void    printStack();
 public:
   RecompilationPolicy(RFrame* first);

   Recompilee* findRecompilee(); 	// determine what to recompile
   void cleanupStaleInlineCaches();	// clean up inline caches of top methods

   static needRecompileCounter(Compiler* c);  // does this compilation (nmethod) need an invocation counter?
   static bool shouldRecompileAfterUncommonTrap(nmethod* nm);  
	  // nm encountered an uncommon case; should it be recompiled?
   static bool shouldRecompileUncommonNMethod(nmethod* nm);  
	  // nm is in uncommon mode; ok to recompile and reoptimize it?
   static char* shouldNotRecompileNMethod(nmethod* nm);
	  // is nm fit to be recompiled?  return NULL if yes, reason otherwise
   static int uncommonNMethodInvocationLimit(int version);  
	  // return invocation counter limit for an uncommon nmethod
   static int uncommonNMethodAgeLimit(int version);  
	  // return nmethod age limit for an uncommon nmethod
};


#endif
