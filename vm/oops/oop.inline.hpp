/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.37 $ */
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


inline klassOop oopDesc::klass() const {
  if (is_mem()) return memOop(this)->klass_field();
  assert(is_smi(), "tag must be smi");
  return smiKlassObj;
}

inline Klass* oopDesc::blueprint() const { return klass()->klass_part(); }

inline smi oopDesc::identity_hash() {
  if (is_smi()) return smiOop(this)->identity_hash();
  assert(is_mem(), "tag must be mem");
  return memOop(this)->identity_hash();
}

inline oop oopDesc::scavenge() {
  return is_mem() ? memOop(this)->scavenge() : this;
}

inline oop oopDesc::relocate() {
  // FIX LATER
  return this;
}

// generation testers
inline bool oopDesc::is_old() const { return is_mem() && memOop(this)->is_old(); }
inline bool oopDesc::is_new() const { return is_mem() && memOop(this)->is_new(); }

inline generation* oopDesc::my_generation() {
  return Universe::generation_containing(this);
}

// type test operations
inline bool oopDesc::is_double()           const { return is_mem() && memOop(this)->klass_field() == doubleKlassObj;       }
inline bool oopDesc::is_block()            const { return is_mem() && memOop(this)->blueprint()->oop_is_block();           }
inline bool oopDesc::is_byteArray()        const { return is_mem() && memOop(this)->blueprint()->oop_is_byteArray();       }
inline bool oopDesc::is_doubleByteArray()  const { return is_mem() && memOop(this)->blueprint()->oop_is_doubleByteArray(); }
inline bool oopDesc::is_doubleValueArray() const { return is_mem() && memOop(this)->blueprint()->oop_is_doubleValueArray();}
inline bool oopDesc::is_symbol()           const { return is_mem() && memOop(this)->blueprint()->oop_is_symbol();          }
inline bool oopDesc::is_objArray()         const { return is_mem() && memOop(this)->blueprint()->oop_is_objArray();        }
inline bool oopDesc::is_weakArray()        const { return is_mem() && memOop(this)->blueprint()->oop_is_weakArray();       }
inline bool oopDesc::is_association()      const { return is_mem() && memOop(this)->blueprint()->oop_is_association();     }
inline bool oopDesc::is_context()          const { return is_mem() && memOop(this)->blueprint()->oop_is_context();         }
inline bool oopDesc::is_klass()            const { return is_mem() && memOop(this)->blueprint()->oop_is_klass();           }
inline bool oopDesc::is_proxy()            const { return is_mem() && memOop(this)->blueprint()->oop_is_proxy();           }
inline bool oopDesc::is_mixin()            const { return is_mem() && memOop(this)->blueprint()->oop_is_mixin();           }
inline bool oopDesc::is_process()          const { return is_mem() && memOop(this)->blueprint()->oop_is_process();         }
inline bool oopDesc::is_vframe()           const { return is_mem() && memOop(this)->blueprint()->oop_is_vframe();          }
inline bool oopDesc::is_method()           const { return is_mem() && memOop(this)->blueprint()->oop_is_method();          }
inline bool oopDesc::is_indexable()        const { return is_mem() && memOop(this)->blueprint()->oop_is_indexable();       }

// Primitives
inline bool oopDesc::verify() {
  return blueprint()->oop_verify(this);
}

inline oop oopDesc::primitive_allocate(bool allow_scavenge) {
  return blueprint()->oop_primitive_allocate(this, allow_scavenge);
}

inline oop oopDesc::primitive_allocate_size(int size) {
  return blueprint()->oop_primitive_allocate_size(this, size);
}

inline oop oopDesc::shallow_copy(bool tenured) {
  return blueprint()->oop_shallow_copy(this, tenured);
}
