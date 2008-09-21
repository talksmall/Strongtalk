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

// Bit-format of markOop:
//  sentinel:1 near_death:1 tagged_contents:1 age:7 hash:20 tag:2 = 32 bits
//  - sentinel is needed during pointer reversal in garbage collection 
//    to distinguish markOops from roots of oops. markOops are used termination
//    elements in the pointer list.
//    During normal execution the sentinel is 1 but the but may be used for
//    marking during special vm operations like dependency checking.
//  - near_death is set by the memory system iff weak pointers keep the object alive.
//  - tagged_contents indicates the is no untagged data in the object.
//  - age contains the age of the object when residing in the new generation.
//    (during garbage collection, age is used to store the size of the object).
//    PLEASE DO NOT CHANGE THE AGE FIELD since the garbage collector relies on the size.
//  - hash contains the identity hash value.
//  - tag contains the special mark tag

class markOopDesc: public oopDesc {
 private:
  unsigned int value()  const { return (unsigned int) this; }
  friend   int assign_hash(markOop& m);

  enum { no_hash    = 0, 
         first_hash = 1};

  enum { sentinel_bits         = 1,
         near_death_bits       = 1,
         tagged_contents_bits  = 1,
         age_bits              = 7,
         hash_bits             = BitsPerWord - sentinel_bits - near_death_bits - tagged_contents_bits - age_bits - Tag_Size };

  enum { hash_shift            = Tag_Size,
         age_shift             = hash_bits             + hash_shift,
         tagged_contents_shift = age_bits              + age_shift,
         near_death_shift      = tagged_contents_bits  + tagged_contents_shift,
         sentinel_shift        = near_death_bits       + near_death_shift };

  enum { hash_mask                     = nthMask(hash_bits),
         hash_mask_in_place            = hash_mask << hash_shift,
         age_mask                      = nthMask(age_bits),
         age_mask_in_place             = age_mask << age_shift,
         tagged_contents_mask          = nthMask(tagged_contents_bits),
         tagged_contents_mask_in_place = tagged_contents_mask << tagged_contents_shift,
         near_death_mask               = nthMask(near_death_bits),
         near_death_mask_in_place      = near_death_mask << near_death_shift,
         sentinel_mask                 = nthMask(sentinel_bits),
	 sentinel_mask_in_place        = sentinel_mask << sentinel_shift };


  enum { no_hash_in_place           = no_hash     << hash_shift,
         first_hash_in_place        = first_hash  << hash_shift,
         untagged_contents_in_place = 1           << tagged_contents_shift };

  enum { sentinel_is_place = 1 << sentinel_shift } ;
 public:
  enum { max_age = age_mask };
  // accessors
  bool    has_sentinel()      const { return maskBits(value(), sentinel_mask_in_place) != 0; }
  markOop set_sentinel()      const { return markOop( sentinel_is_place | value()); } 
  markOop clear_sentinel()    const { return markOop(~sentinel_is_place & value()); }

  bool has_tagged_contents()  const { return maskBits(value(), tagged_contents_mask_in_place) != 0; }

  bool    is_near_death()     const { return maskBits(value(), near_death_mask_in_place) != 0; }
  markOop set_near_death()    const { return markOop( near_death_mask_in_place | value()); } 
  markOop clear_near_death()  const { return markOop(~near_death_mask_in_place & value()); }

  // klass invalidation (via sentinel bit)
  bool    is_klass_invalid()  const { return !has_sentinel();  }
  markOop set_klass_invalid() const { return clear_sentinel(); }
  markOop clear_klass_invalid() const { return set_sentinel(); }

  // tells if context has forward reference to an unoptimized context(via sentinel bit)
  static int context_forward_bit_mask() { return near_death_mask_in_place; }
  bool    has_context_forward() const { return is_near_death();  }
  markOop set_context_forward() const { return set_near_death(); }

  // notification queue check
  bool    is_queued()    const { return !has_sentinel();  }
  markOop set_queued()   const { return clear_sentinel(); }
  markOop clear_queued() const { return set_sentinel();   }

  // age operations
  int        age()            const { return maskBits(value(), age_mask_in_place)  >> age_shift; }
  markOop    set_age(int v)   const {
    assert((v & ~age_mask) == 0, "shouldn't overflow field");
    return markOop((value() & ~age_mask_in_place) | ((v & age_mask) << age_shift));
  }
  markOop    incr_age()       const { return age() == max_age ? markOop(this) : set_age(age() + 1); }

  // hash operations
  int        hash()           const { return maskBits(value(), hash_mask_in_place) >> hash_shift; }
  markOop    set_hash(int v)  const {
    if ((v & hash_mask) == 0) v = first_hash; // avoid no_hash
    markOop val = markOop((value() & ~hash_mask_in_place) | ((v & hash_mask) << hash_shift));
    assert(val->hash() != no_hash, "should have hash now");
    return val;
  }
  bool       has_valid_hash() const { return hash() != no_hash; }

  // markOop prototypes
  static markOop tagged_prototype() {
    return markOop(sentinel_is_place | no_hash_in_place | Mark_Tag);
  }
  static markOop untagged_prototype() {
    return markOop(sentinel_is_place | untagged_contents_in_place | no_hash_in_place | Mark_Tag);
  }

  // badOop
  static markOop bad() {
    return markOop(sentinel_is_place | first_hash_in_place | Mark_Tag);
  }

  friend int hash_markOop(markOop& m) {
    int v = m->hash();
    return v == no_hash ? assign_hash(m) : v;
  }

  // printing
  void print_on(outputStream* st);

  static int masked_hash(int v) { return v & hash_mask; }
};

# define badOop markOopDesc::bad()

// tells whether p is a root to an oop or a markOop
// used during pointer reversal during GC.
//inline bool is_oop_root(oop* p) { return !markOop(p)->has_sentinel(); }
inline bool is_oop_root(oop* p) { return !markOop(p)->is_mark(); }
