/* Copyright 1994, 1995, LongView Technologies, L.L.C. $Revision: 1.10 $ */
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

// IC_Shape describes the (logical) shape in which an IC at a particular
// call site apears; i.e. how many receivers have been registered in that
// IC.
//
// Note: IC_Shape is the LOGICAL shape and it does not necessarily
// correspond with the physical implementation of a call site; e.g.
// a call site may contain a pic but nevertheless hold only one
// receiver and therefore be monomorphic!

enum IC_Shape {
  anamorphic,	// send has never been executed => no type information	(size = 0)
  monomorphic,	// only one receiver type available			(size = 1)
  polymorphic,	// more than one receiver type available		(size > 1)
  megamorphic	// many receiver types, only last one is available	(size = 1)
};

  
// IC_Iterator is the abstract superclass of all IC iterators in the system.
// It SHOULD BE USED whenever iteration over an inline cache is required.

class IC_Iterator: public PrintableResourceObj {
 public:
  // IC information
  virtual int		number_of_targets() const	= 0;
  virtual IC_Shape	shape() const			= 0;
  virtual symbolOop	selector() const		= 0;

  virtual InterpretedIC* interpreted_ic() const		{ ShouldNotCallThis(); return NULL; }
  virtual CompiledIC*    compiled_ic() const		{ ShouldNotCallThis(); return NULL; }

  virtual bool		is_interpreted_ic() const	{ return false; }	// is sender interpreted?
  virtual bool		is_compiled_ic() const		{ return false; }	// is sender compiled?
  virtual bool		is_super_send() const		= 0;			// is super send?

  // Iterating through entries
  virtual void		init_iteration()		= 0;
  virtual void		advance()			= 0;
  virtual bool		at_end() const			= 0;

  // Accessing entries
  virtual klassOop	klass() const			= 0;

  virtual bool		is_interpreted() const		= 0;	// is current target interpreted?
  virtual bool		is_compiled() const		= 0;	// is current target compiled?

  virtual methodOop	interpreted_method() const	= 0;	// target methodOop (always non-NULL)
  virtual nmethod*	compiled_method() const		= 0;	// target nmethod; NULL if interpreted

  // methods for direct access to ith element (will set iteration state to i)
  void      goto_elem(int i);
  methodOop interpreted_method(int i);
  nmethod*  compiled_method(int i);
  klassOop  klass(int i);

}; 


// IC is the implementation independent representation for ICs. It allows manipulation
// of ICs without dealing with concrete interpreted or compiled ICs and it SHOULD BE
// USED instead whenever possible.
//
// Class hierarchy:
//
// IC ---- uses ---->	IC_Iterator
//			  InterpretedIC_Iterator --- uses --->	InterpretedIC
//			  CompiledIC_Iterator    --- uses --->	CompiledIC & PIC_Iterator
//
// (Note: One could also imagine IC being the abstract super class of InterpretedIC
// and CompiledIC, which in turn are using InterpretedIC_Iterator and CompiledIC_Iterator.
// However, right now, CompiledIC shares a common super class with PrimitiveIC, and
// furthermore, the real ICs are composed out of at least 2 classes, one for the
// monormorphic case and one for the polymorphic case. Having IC based on the
// iterator seems to simplify this.)

class IC: public PrintableResourceObj {
 private:
  const IC_Iterator*	_iter;

 public:
  IC(IC_Iterator* iter) : _iter(iter)		{ }
  IC(CompiledIC* ic);
  IC(InterpretedIC* ic);

  IC_Iterator*	iterator() const		{ return (IC_Iterator*)_iter; }

  // IC information
  int		number_of_targets() const	{ return _iter->number_of_targets(); }
  IC_Shape	shape() const			{ return _iter->shape(); }
  symbolOop	selector() const		{ return _iter->selector(); }
  
  InterpretedIC* interpreted_ic() const		{ return _iter->interpreted_ic(); }
  CompiledIC*	compiled_ic() const		{ return _iter->compiled_ic(); }

  bool		is_interpreted_ic() const	{ return _iter->is_interpreted_ic(); }
  bool		is_compiled_ic() const		{ return _iter->is_compiled_ic(); }
  bool		is_super_send() const		{ return _iter->is_super_send(); }

  GrowableArray<klassOop>* receiver_klasses() const;

  // IC manipulation
  void		replace(nmethod* nm);		// replace entry matching nm's key with nm

  // Debugging
  void		print();
};