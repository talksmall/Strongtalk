/* Copyright 1994 - 1996 LongView Technologies L.L.C. $Revision: 1.24 $ */
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

// A PIC implements a Polymorphic Inline Cache for compiled code.
// It may be megamorphic, in which case it may cache only the last
// method and do a lookup whenever there is a cache miss.

class PIC_Iterator;
class PIC_contents;

class PIC {
 public:
  enum Consts {
    max_nof_entries		=  4,			// the maximal number of PIC entries

    // PIC layout constants
    PIC_methodOop_only_offset	=  5,
    PIC_smi_nmethod_offset	=  4,

    PIC_nmethod_entry_offset  	= 11,
    PIC_nmethod_entry_size	= 12,
    PIC_nmethod_klass_offset	=  2,
    PIC_nmethod_offset		=  8,

    PIC_methodOop_entry_offset	= 16,
    PIC_methodOop_entry_size	=  8,
    PIC_methodOop_klass_offset	=  0,
    PIC_methodOop_offset	=  4,

    // MIC layout constants
    MIC_selector_offset		=  5,
    MIC_code_size		=  9,
  };

 private:
  CompiledIC*	_ic;				// the ic linked to this PIC
  short		_code_size;			// size of code in bytes
  short		_number_of_targets;		// the total number of PIC entries, 0 indicates a MIC

  static int	nof_entries(char* pic_stub);	// the no. of methodOop entries for a given stub routine

  int		code_for_methodOops_only (char* entry, PIC_contents* c);
  int		code_for_polymorphic_case(char* entry, PIC_contents* c);
  int		code_for_megamorphic_case(char* entry);

  void          shrink_and_generate(PIC* pic, klassOop klass, void* method);

  bool		contains(char* addr)		{ return entry() <= addr && addr < entry() + code_size(); }

  // Creation/access of PICs
  PIC(CompiledIC* ic, PIC_contents* contents, int allocated_code_size);	// creation of PICs
  PIC(CompiledIC* ic);							// creation of MICs

 public:
  void* operator new(size_t size, int code_size);

  // Deallocates this pic from the pic heap
  void operator delete (void* p);
  void operator delete (void* p, int) {};

  // Allocates and returns a new ready to execute pic.
  static PIC*	allocate(CompiledIC* ic, klassOop klass, LookupResult result);

  // Tells whether addr inside the PIC area
  static bool	in_heap(char* addr);

  // Returns the PIC containing addr, NULL otherwise
  static PIC*	find(char* addr);

  // Returns the code size of the PIC
  int		code_size() const		{ return _code_size; }            


  // Retrieving PIC information
  CompiledIC*   compiled_ic() const		{ return _ic;				}
  int           number_of_targets() const	{ return _number_of_targets;		}
  symbolOop	selector() const		{ return compiled_ic()->selector();	}
  char*		entry() const			{ return (char*) (this+1);		}
  bool		is_monomorphic() const		{ return number_of_targets() == 1;	}
  bool		is_polymorphic() const		{ return number_of_targets() > 1;	}
  bool		is_megamorphic() const		{ return number_of_targets() == 0;	}

  // For MICs (megamorphic PICs) only
  symbolOop*	MIC_selector_address() const;	// the address of the selector in the MIC cache

  // replace appropriate target (with key nm->key) by nm.
  // this is returned if we could patch the current PIC.
  // a new PIC is returned if we could not patch this PIC.
  PIC* replace(nmethod* nm);

  // Cleans up the pic and returns:
  //  1) A PIC			(still polymorphic or megamorphic)
  //  2) A nmethod		(now   monomorphic)
  //  3) nothing		(now   anamorphic)
  PIC* cleanup(nmethod** nm);

  GrowableArray<klassOop>*	klasses() const;

  // Iterate over all oops in the pic
  void oops_do(void f(oop*));

  // printing operation
  void print();

  // verify operation
  void verify();

  friend class PIC_Iterator;
};


// A PIC_Iterator is used get information out of a PIC. It is used to
// display PICs visually, to generate new PICs out of old ones and for
// GC purposes. In case of a megamorphic IC (MIC), the PIC_Iterator will
// be at_end after setup (no entries).

class PIC_Iterator: public PrintableResourceObj {
 public:
  enum State {	at_smi_nmethod,
  		at_nmethod,
  		at_methodOop,
  		at_the_end
  };

 private:
  PIC*		_pic;				// the PIC over which is iterated
  char*		_pos;				// the current iterator position
  enum State	_state;				// current iterator state
  int		_methodOop_counter;		// remaining no. of methodOop entries

  int*		nmethod_disp_addr() const;	// valid if state() in {at_smi_nmethod, at_nmethod}
  void		computeNextState();

 public:
  PIC_Iterator(PIC* pic);

  // Iterating through PIC entries
  void		advance();
  State		state() const			{ return _state; }
  bool		at_end() const			{ return state() == at_the_end; }

  // Accessing PIC entries
  klassOop	get_klass() const;
  char*		get_call_addr() const;
  bool		is_interpreted() const;
  bool		is_compiled() const;

  methodOop	interpreted_method() const;
  nmethod*	compiled_method() const;

  // Modifying PIC entries
  void		set_klass(klassOop klass);
  void		set_nmethod(nmethod* nm);
  void		set_methodOop(methodOop method);

  // Must be public for oops_do in CompiledPIC
  methodOop*	methodOop_addr() const;		// valid if state() is at_PIC_methodOop
  klassOop*	klass_addr() const;		// valid if state() in {at_nmethod, at_methodOop}

  // Debugging
  void		print();
};


class CompiledIC_Iterator: public IC_Iterator {
 private:
  CompiledIC*	_ic;
  PIC_Iterator*	_picit;

  int		_number_of_targets;		// the no. of IC entries
  IC_Shape	_shape;				// shape of inline cache
  int		_index;				// the next entry no.
  
 public:
  CompiledIC_Iterator(CompiledIC* ic);

  // IC information
  bool		is_compiled_ic() const		{ return true; }
  bool		is_super_send() const;
  int		number_of_targets() const	{ return _number_of_targets; }
  IC_Shape	shape() const			{ return _shape; }
  symbolOop	selector() const		{ return _ic->selector(); }
  CompiledIC*   compiled_ic() const		{ return _ic; }

  // Iterating through entries
  void		init_iteration();
  void		advance();			// advance iterator to next target
  bool		at_end() const			{ return _index >= number_of_targets(); }

  // Accessing entries
  klassOop	klass() const;			// receiver klass of current target

  bool		is_interpreted() const;		// is current target interpreted?
  bool		is_compiled() const;

  methodOop	interpreted_method() const;	// current target method (whether compiled or not)
  nmethod*	compiled_method() const;	// current compiled target or NULL if interpreted

  // Debugging
  void		print();
};


#endif
