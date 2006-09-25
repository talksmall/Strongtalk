/* Copyright 1996 LongView Technologies L.L.C. $Revision: 1.2 $ */
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


// vframe objects are handles to process activations.

// memory layout:
//    [header      ]
//    [klass_field ]
//    [process     ]
//    [index       ]
//    [timestamp   ]

class vframeOopDesc: public memOopDesc {
 private:
  processOop _process;
  smiOop     _index;
  smiOop     _time_stamp;

 protected:
  vframeOopDesc* addr() const { return (vframeOopDesc*)memOopDesc::addr(); }

 public:
  // accessors
  processOop process() const 		{ return addr()->_process; }
  void set_process(processOop p)	{ STORE_OOP(&addr()->_process,p);}

  int index() const			{ return addr()->_index->value(); }
  void set_index(int i)			{ STORE_OOP(&addr()->_index, as_smiOop(i)); }

  int time_stamp() const		{ return addr()->_time_stamp->value(); }
  void set_time_stamp(int t)		{ STORE_OOP(&addr()->_time_stamp, as_smiOop(t)); }

  friend vframeOop as_vframeOop(void* p) { return vframeOop(as_memOop(p)); }

  // sizing
  static int header_size() { return sizeof(vframeOopDesc)/oopSize; }

  // get the corresponding vframe, returns NULL it fails.
  vframe* get_vframe();

 private:
   friend vframeKlass;
};


;
