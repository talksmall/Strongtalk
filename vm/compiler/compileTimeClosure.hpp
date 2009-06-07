/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.9 $ */
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

# ifdef DELTA_COMPILER

// A CompileTimeClosure represents a block closure at compile time. It holds the noninlined
// block method and a unique identification of the compiled & customized home method
// (the non-block method lexically enclosing the block). CompileTimeClosures are needed to
// distinguish multiple versions of the same source block at compile time.  Multiple
// versions can exist when the block's home method is inlined several times.
//
// Note: Having a closure stub doesn't mean that there exists a blockClosureOop at
// run-time.  If the block is inlined, none of this ever matters.  However, if a 
// block belonging to a compiled method can potentially be created at run-time, it 
// must be different from other versions of the same method (i.e., it must be customized to 
// the version of its home method).  This customization is achieved by putting different
// code addresses in the blockClosureOops so they invoke different nmethods.

class CompileTimeClosure: public PrintableResourceObj {
 protected:
  InlinedScope*		_parent_scope;	// scope to which the closure belongs
  methodOop		_method;	// block method
  PReg*			_context;	// parent context
  int			_nofArgs;	// number of arguments for the block
  jumpTableID           _id;            // unique identification of this closure within the parent nmethod.
  NonInlinedBlockScopeNode* _noninlined_block_scope;	// an NonInlinedScopeDesc

 public:
  CompileTimeClosure(InlinedScope* s, methodOop method, PReg* context, int nofArgs) { 
    _parent_scope = s;
    _method       = method;
    _context      = context;
    _nofArgs      = nofArgs;
    _noninlined_block_scope = NULL;
  }

  InlinedScope*			parent_scope() const		{ return _parent_scope; }
  methodOop			method() const			{ return _method; }
  PReg*				context() const			{ return _context; }
  int				nofArgs() const			{ return _nofArgs; }
  jumpTableID                   id() const			{ return _id; }
  NonInlinedBlockScopeNode*	noninlined_block_scope();
  void                  	generateDebugInfo();

  void	set_id(jumpTableID id) { _id = id; }	// sets the indices for computing the jump table entry
  char*	jump_table_entry();			// returns the code entry point for the jump table entry

  void print();
  virtual bool verify() const;
};

# endif



