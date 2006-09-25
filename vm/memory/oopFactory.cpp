/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.20 $ */
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

# include "incls/_precompiled.incl"
# include "incls/_oopFactory.cpp.incl"

byteArrayOop oopFactory::new_byteArray(int size) {
  byteArrayKlass* bk =
    (byteArrayKlass*) Universe::byteArrayKlassObj()->klass_part();
  return byteArrayOop(bk->allocateObjectSize(size));
}

byteArrayOop oopFactory::new_byteArray(char* name) {
  int len = strlen(name);
  byteArrayOop result = new_byteArray(len);
  for (int index = 0; index < len; index++) {
    result->byte_at_put(index+1, name[index]);
  }
  return result;
}

objArrayOop oopFactory::new_objArray(int size) {
  objArrayKlass* ok =
    (objArrayKlass*) Universe::objArrayKlassObj()->klass_part();
  return objArrayOop(ok->allocateObjectSize(size));
}

objArrayOop oopFactory::new_objArray(GrowableArray<oop>* array) {
  BlockScavenge bs;
  FlagSetting(processSemaphore, true);
  int size = array->length();
  objArrayKlass* ok =
    (objArrayKlass*) Universe::objArrayKlassObj()->klass_part();

  objArrayOop result = objArrayOop(ok->allocateObjectSize(size));
  for (int index = 1; index <= size; index++) {
    result->obj_at_put(index, array->at(index-1));
  }
  return result;
}

doubleOop oopFactory::new_double(double value) {
  doubleOop d = as_doubleOop(Universe::allocate(sizeof(doubleOopDesc)/oopSize));
  d->init_untagged_contents_mark();
  d->set_klass_field(doubleKlassObj);
  d->set_value(value);
  return d;
}

doubleOop oopFactory::clone_double_to_oldspace(doubleOop value) {
  doubleOop d = as_doubleOop(Universe::allocate_tenured(sizeof(doubleOopDesc)/oopSize));
  d->init_untagged_contents_mark();
  d->set_klass_field(doubleKlassObj);
  d->set_value(value->value());
  return d;
}

symbolOop oopFactory::new_symbol(char* name,int len) {
  return Universe::symbol_table->lookup(name, len);
}

symbolOop oopFactory::new_symbol(char* name) {
  return new_symbol(name, strlen(name));
}

symbolOop oopFactory::new_symbol(byteArrayOop b) {
  return new_symbol(b->chars(), b->length());
}

associationOop oopFactory::new_association(symbolOop key, oop value, bool is_constant) {
  associationOop as = associationOop(Universe::associationKlassObj()->klass_part()->allocateObject());
  assert(as->is_association(), "type check");
  as->set_key(key);
  as->set_value(value);
  as->set_is_constant(is_constant);
  return as;
}

vframeOop oopFactory::new_vframe(processOop process, int index) {
  BlockScavenge bs;
  vframeKlass* vk =(vframeKlass*) Universe::vframeKlassObj()->klass_part();

  vframeOop result = vframeOop(vk->allocateObject());

  result->set_process(process);
  result->set_index(index);
  result->set_time_stamp(process->process()->time_stamp());

  return result;
}
