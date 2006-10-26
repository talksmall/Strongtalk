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

#ifdef DELTA_COMPILER

# define OOPNCODE_FROM(fieldName, p)					      \
    ((OopNCode*)((char*)p - (char*)&((OopNCode*)NULL)->fieldName))

// NCodeBase is the superclass of all things containing native code.

class NCodeBase : public PrintableCHeapObj {
 protected:
  int _instsLen;			// instruction length in bytes
 public:
  void* operator new(size_t size) {
    Unused(size); SubclassResponsibility(); return NULL; }
  
  virtual char* insts() const = 0;	// beginning of instructions part
  virtual int size() const = 0;		// size in bytes
  int instsLen() const 			{ return _instsLen; }
  char* instsEnd() const 		{ return insts() + instsLen(); }
  bool  contains(void* p) const 	{ return (void*)insts() <= p && p < (void*)instsEnd(); }

  virtual bool isNMethod() const  	{ return false; }
  virtual bool isPIC() const  		{ return false; }
  //virtual bool isCountStub() const  	{ return false; }
  //virtual bool isAgingStub() const  	{ return false; }
  
  virtual void moveTo(void* to, int size) = 0; // (possibly overlapping) copy
  virtual void relocate() {};
  virtual void verify() = 0;
 protected:
  void verify2(char* name);
};


// OopNCode is the base class of all code containing oop references embedded
// in the code (e.g. "load constant" instructions).  

class OopNCode : public NCodeBase {
 protected:
  void	check_store(oop x, char *bound) {
    if (Universe::new_gen.is_new(x, bound)) remember(); }
  int _locsLen;				// relocation info length (bytes)
 public:
  
  relocInfo* locs() const	    	{ return (relocInfo*) instsEnd();}
  int locsLen() const	    		{ return _locsLen;}
  relocInfo* locsEnd() const       	{ return (relocInfo*) ((char*)locs() + _locsLen);}
    
  virtual bool isNMethod() const 	{ return false; }
  
  // Memory operations: return true if need to invalidate instruction cache
  virtual bool switch_pointers(oop from, oop to,
			       GrowableArray<nmethod*>* nmethods_to_invalidate);

  void relocate();
  void remember();
  virtual void verify();
};

NCodeBase* findThing(void* addr);   // returns NULL if addr not in a zone

#endif
