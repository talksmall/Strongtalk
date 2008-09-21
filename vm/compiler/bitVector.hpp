/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.9 $ */
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

// General bit vectors; space is allocated for maxLength bits, but only
// the portion up to length bits is used.
// SimpleBitVector: simple version that fits into a word (for speed)

# ifdef DELTA_COMPILER

  typedef void (*intDoFn)(int i);
  
  // a pseudo-class -- int(this) actually holds the bits, so there's no
  // allocation
  class SimpleBitVector : public ValueObj {
    int bits;
   public:
    SimpleBitVector(int b = 0) { bits = b; } 

    SimpleBitVector allocate(int l) {
      assert(l >= 0 && l < BitsPerWord, "need longer bit vector");
      return SimpleBitVector(addNth(bits, l));
    }

    SimpleBitVector deallocate(int l) {
      assert(l >= 0 && l < BitsPerWord, "need longer bit vector");
      return SimpleBitVector(subNth(bits, l));
    }

    bool isAllocated(int l) {
      assert(l >= 0 && l < BitsPerWord, "need longer bit vector");
      return isSet(bits, l);
    }
    bool isEmpty() { return bits == 0; }
  };

  class BitVector:  public PrintableResourceObj {
   protected:
    int maxLength;		// max # bits
    int length;			// number of bits, not words
    int* bits;			// array containing the bits
    
    int  indexFromNumber(int i) { return i >> LogBitsPerWord; }
    int offsetFromNumber(int i) { return lowerBits(i, LogBitsPerWord); }
    
    bool getBitInWord(int i, int o) { return isSet(bits[i], o); }
    void setBitInWord(int i, int o) {       setNth(bits[i], o); }
    void clearBitInWord(int i, int o) {   clearNth(bits[i], o); }
    
    int bitsLength(int l) { return indexFromNumber(l - 1) + 1; }
    
    int* createBitString(int l) {
      int blen = bitsLength(l);
      int* bs = NEW_RESOURCE_ARRAY( int, blen);
      set_words(bs, blen, 0);
      return bs; }
    int* copyBitString(int len) {
      assert(len >= maxLength, "can't shorten");
      int blen = bitsLength(len);
      int* bs = NEW_RESOURCE_ARRAY( int, blen);
      int blength = bitsLength(maxLength);
      copy_words(bits, bs, blength);
      if (blength < blen) set_words(bs + blength, blen - blength, 0);
      return bs; }
    
  public:
    BitVector(int l) {
      assert(l > 0, "should have some length");
      length = maxLength = l; bits = createBitString(l); }
    
  protected:
    BitVector(int l, int ml, int* bs) {
      maxLength = ml; length = l; bits = bs; }
    
  public:
    BitVector* copy(int len) {
      return new BitVector(length, len, copyBitString(len)); }
    
    bool includes(int i) {
      assert(this, "shouldn't be a null pointer");
      assert(i >= 0 && i < length, "not in range");
      bool b = getBitInWord(indexFromNumber(i), offsetFromNumber(i));
      return b; }
    void add(int i) {
      assert(this, "shouldn't be a null pointer");
      assert(i >= 0 && i < length, "not in range");
      setBitInWord(indexFromNumber(i), offsetFromNumber(i)); }
    void addFromTo(int first, int last);	// set bits [first..last]
    void remove(int i) {
      assert(this, "shouldn't be a null pointer");
      assert(i >= 0 && i < length, "not in range");
      clearBitInWord(indexFromNumber(i), offsetFromNumber(i)); }
    void removeFromTo(int first, int last);	// clear bits [first..last]

    // union/intersect return true if receiver has changed
    bool unionWith(BitVector* other);		// this |= other
    bool intersectWith(BitVector* other);	// this &= other
    bool isDisjointFrom(BitVector* other);	// (this & other) == {}

    void doForAllOnes(intDoFn f);  // call f for all 1 bits

    void setLength(int l) 	{ assert(l < maxLength, "too big"); length = l; }
    void clear() 		{ set_words(bits, bitsLength(length), 0); }
    
    void print_short();
    void print();

    friend class LongRegisterMask;
    friend int findFirstUnused(LongRegisterMask** strings, int len, int start);
  };
  
# endif
