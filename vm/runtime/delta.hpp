/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.5 $ */
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

// A DeltaCallCache caches methods for Delta calls.
// They are primarily used to speed up performs.
//
// %todo:
//   clear all DeltaCallCache at garbage collection

class DeltaCallCache: public AllStatic {
 private:
  static DeltaCallCache* _root;	// root of all DeltaCallCaches
  DeltaCallCache*        _link;	// all DeltaCallCaches are linked

  // cache
  LookupKey    _key;
  LookupResult _result;

 public:
  DeltaCallCache();
  void clear();			// clears the cache
  static void clearAll();	// clears all DeltaCallCaches (called by GC, etc.)


  bool match(klassOop klass, symbolOop selector) {
    return oop(selector) == _key.selector_or_method() 
        && klass         == _key.klass();
  }


  LookupResult lookup(klassOop klass, symbolOop selector) {
    if (!match(klass, selector)) {
      _result = interpreter_normal_lookup(klass, selector);
      if (!_result.is_empty()) {
        _key.initialize(klass, selector);
      }
    }
    return _result;
  }

  LookupResult result() { return _result; }
};


// Delta provides the following entry points for calling Delta methods

class Delta: public AllStatic {
 public:
  // The following functions return a marked oop if the selector is not a symbolOop
  static oop call_generic(DeltaCallCache* ic, oop receiver, oop selector, int nofArgs, oop* args);

  static oop call(oop receiver, oop selector);
  static oop call(oop receiver, oop selector, oop arg1);
  static oop call(oop receiver, oop selector, oop arg1, oop arg2);
  static oop call(oop receiver, oop selector, oop arg1, oop arg2, oop arg3);
  static oop call(oop receiver, oop selector, objArrayOop args);

  static oop does_not_understand(oop receiver, symbolOop selector, int nofArgs, oop* argArray);
};
