/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.4 $ */
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
# include "incls/_snapshot.cpp.incl"

void SnapshotDesc::read_header() {
  read_revision();
  read_sizes();
}

void SnapshotDesc::write_header() {
  write_revision();
  write_sizes();
}

void SnapshotDesc::read_sizes() {

}

void SnapshotDesc::write_sizes() {

}  

static const char* revision_format = "Delta snapshot revision: %d.%d\n";

void SnapshotDesc::read_revision() {
  fprintf(file, revision_format, 
          Universe::major_version(), Universe::snapshot_version());
}

void SnapshotDesc::write_revision() {
  int major, snap;
  if (fscanf(file, revision_format, &major, &snap) != 2)
    error("reading revision");
  if (Universe::major_version() != major)
    error("major revision number conflict");
  if (Universe::snapshot_version() != snap)
    error("snapshot revision number conflict");
}

class ReadClosure : public OopClosure {
  void do_oop(oop* o) {
    
  }
  SnapshotDesc* s;
  public:
    ReadClosure(SnapshotDesc* s) { this->s = s; }
};

class WriteClosure : public OopClosure {
  void do_oop(oop* o) {
    fprintf(s->file, "0x%lx\n", o);
  }
  SnapshotDesc* s;
  public:
    WriteClosure(SnapshotDesc* s) { this->s = s; }
};

void SnapshotDesc::read_roots() {
  ReadClosure blk(this);
  Universe::root_iterate(&blk);
}

void SnapshotDesc::write_roots() {
  WriteClosure blk(this);
  Universe::root_iterate(&blk);
}

void SnapshotDesc::read_spaces() {

}

void SnapshotDesc::write_spaces() {

}

void SnapshotDesc::read_zone() {
  // Not implemented yet
}

void SnapshotDesc::write_zone() {
  // Not implemented yet
}

void SnapshotDesc::read_from(char* name) {
  read_header();
  read_spaces();
  read_roots();
  read_zone();
}

void SnapshotDesc::write_on(char* name) {
  write_header();
  write_spaces();
  write_roots();
  write_zone();
}

void SnapshotDesc::error(char* msg) {
  fatal(msg);
}

symbolOop SnapshotDesc::error_symbol() {
  return NULL;
}
