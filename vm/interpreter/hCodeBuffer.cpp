/* Copyright (c) 2010, Stephen Rees 
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

# include "incls/_precompiled.incl"
# include "incls/_hCodeBuffer.cpp.incl"
void HCodeBuffer::align() {
  while(!isAligned())
    _bytes->append(0xFF);
}
bool HCodeBuffer::isAligned() {
  return (_bytes->length() % oopSize) == 0;
}
void HCodeBuffer::pushByte(unsigned char op) {
  if (isAligned())
    _oops->append(as_smiOop(0));
  _bytes->append(op);
}
void HCodeBuffer::pushOop(oop arg) {
  align();
  _bytes->append(0);
  _bytes->append(0);
  _bytes->append(0);
  _bytes->append(0);

  _oops->append(arg);
}
byteArrayOop HCodeBuffer::bytes() {
  BlockScavenge bs;
  align();
  Klass* klass = Universe::byteArrayKlassObj()->klass_part();
  byteArrayOop result = byteArrayOop(klass->allocateObjectSize(byteLength()));

  for (int index = 0; index < byteLength(); index++)
    result->byte_at_put(index + 1, (unsigned char) _bytes->at(index));
  
  return result;
}
objArrayOop HCodeBuffer::oops() {
  BlockScavenge bs;
  Klass* klass = Universe::objArrayKlassObj()->klass_part();
  objArrayOop result = objArrayOop(klass->allocateObjectSize(oopLength()));

  for (int index = 0; index < oopLength(); index++)
    result->obj_at_put(index + 1, _oops->at(index));

  return result;
}
