/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.7 */
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

#include "incls/_precompiled.incl"
#include "incls/_virtualspace.cpp.incl"

ReservedSpace::ReservedSpace(int size) {
  assert((size % os::vm_page_size()) == 0, "size not page aligned");
  _base = os::reserve_memory(size);
  _size = size;
}

ReservedSpace ReservedSpace::first_part(int partition_size) {
  if (partition_size > size())
    fatal("partition failed");
  ReservedSpace result(base(), partition_size);
  return result;
}

ReservedSpace ReservedSpace::last_part(int  partition_size) {
  if (partition_size > size())
    fatal("partition failed");
  ReservedSpace result(base() + partition_size, size() - partition_size);
  return result;
}

int ReservedSpace::page_align_size(int size) {
  int alignment = os::vm_page_size();
  int adjust    = alignment - (size%alignment) % alignment;
  return size + adjust;
}

VirtualSpace::VirtualSpace(int reserved_size, int committed_size, bool low_to_high) {
  ReservedSpace rs(reserved_size);
  initialize(rs, committed_size, low_to_high);
}

VirtualSpace::VirtualSpace(ReservedSpace reserved, int committed_size, bool low_to_high) {
  initialize(reserved, committed_size, low_to_high);
}

VirtualSpace::VirtualSpace() {
  _low_boundary  = NULL;
  _high_boundary = NULL;
  _low_to_high   = true;
  _low           = NULL;
  _high          = NULL;
}

void VirtualSpace::initialize(ReservedSpace reserved, int committed_size, bool low_to_high) {
  _low_boundary  = reserved.base();
  _high_boundary = low_boundary() + reserved.size();

  _low_to_high = low_to_high;

  if (low_boundary() == NULL) fatal("os::reserve_memory failed");

  // initialize committed area
  _low  = low_to_high ? low_boundary() : high_boundary();
  _high = low();

  // commit to initial size
  expand(committed_size);

  VirtualSpaces::add(this);
}

VirtualSpace::~VirtualSpace() { 
  release();
}

void VirtualSpace::release() {
  os::release_memory(low_boundary(), reserved_size());
  _low_boundary  = NULL;
  _high_boundary = NULL;
  _low           = NULL;
  _high          = NULL;
  VirtualSpaces::remove(this);
}

int VirtualSpace::committed_size() const { 
  return high() - low();
}

int VirtualSpace::reserved_size() const {
  return high_boundary() - low_boundary();
}

int VirtualSpace::uncommitted_size()  const {
  return reserved_size() - committed_size();
}

bool VirtualSpace::contains(void* p) const {
  return low() <= (char*) p && (char*) p < high();
}

bool VirtualSpace::low_to_high() const {
  return _low_to_high;
}


void VirtualSpace::expand(int size) {
  assert(uncommitted_size() >= size, "not space enough");
  assert((size % os::vm_page_size()) == 0, "size not page aligned");
  if (low() == low_boundary()) {
    if (!os::commit_memory(high(), size)) fatal("os::commit_memory failed");
    _high += size;
  } else {
    _low -= size;
    if (!os::commit_memory(low(), size)) fatal("os::commit_memory failed");
  }
}

void VirtualSpace::shrink(int size) {
  assert(committed_size() >= size, "not space enough");
  assert((size % os::vm_page_size()) == 0, "size not page aligned");
  if (low() == low_boundary()) {
    _high -= size;
    if (!os::uncommit_memory(high(), size)) fatal("os::uncommit_memory failed");
  } else {
    if (!os::uncommit_memory(low(), size)) fatal("os::uncommit_memory failed");
    _low += size;
  }
}

void VirtualSpace::print() {  
  std->print_cr("Virtual space:");
  std->print_cr(" - committed: %d", committed_size());
  std->print_cr(" - reserved: %d",  reserved_size());
  std->print_cr(" - [low, high]: [0x%lx, 0x%lx]",  low(), high());
  std->print_cr(" - [low_b, high_b]: [0x%lx, 0x%lx]",  low_boundary(), high_boundary());
}


VirtualSpace* VirtualSpaces::head = NULL;

void VirtualSpaces::add(VirtualSpace* sp) {
  sp->next = head;
  head = sp;
}

void VirtualSpaces::remove(VirtualSpace* sp) {
  if (!head) return;
  if (head == sp)
    head = sp->next;
  else {
    for(VirtualSpace* p = head; p->next; p = p->next)
      if (p->next = sp)
        p->next = sp->next;
  }
}

int VirtualSpaces::committed_size() {
  int total = 0;
  for(VirtualSpace* p = head; p; p = p->next)
    total += p->committed_size();
  return total;
}

int VirtualSpaces::reserved_size() {
  int total = 0;
  for(VirtualSpace* p = head; p; p = p->next)
    total += p->reserved_size();
  return total;
}

int VirtualSpaces::uncommitted_size() {
  int total = 0;
  for(VirtualSpace* p = head; p; p = p->next)
    total += p->uncommitted_size();
  return total;
}

void VirtualSpaces::print() {
  std->print_cr("VirtualSpaces:");
  for(VirtualSpace* p = head; p; p = p->next)
    p->print();
}

void VirtualSpaces::test() {
  VirtualSpace space(128 *1024, 4 * 1024);
  space.print();
  space.expand(4 * 1024);
  space.print();
  space.expand(4 * 1024);
  space.print();
  space.shrink(4 * 1024);
  space.print();
  space.shrink(4 * 1024);
  space.print();
}
