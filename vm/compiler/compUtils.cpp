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

# include "incls/_precompiled.incl"

#ifdef DELTA_COMPILER

# include "incls/_compUtils.cpp.incl"

PerformanceDebugger::PerformanceDebugger(Compiler* c) {
  this->c = c; compileReported = false;
  blocks = new GrowableArray<BlockPReg*>(5);
  reports = new GrowableArray<char*>(5);
  str = NULL;
  notInlinedBecauseNmethodTooBig = NULL;
}

void PerformanceDebugger::start_report() {
  str = new stringStream(256);
  report_compile();
}

void PerformanceDebugger::stop_report() {
  char* report = str->as_string();
  for (int i = reports->length() - 1; i >= 0; i--) {
    if (strcmp(reports->at(i), report) == 0) return;  // already printed identical msg
  }
  std->print(report);
  reports->append(report);
  str = NULL;
}

void PerformanceDebugger::report_compile() {
  if (!compileReported) {
    compileReported = true;
    std->print("\n*while compiling nmethod for %s:\n", c->key->print_string());
  }
}

// always put one of these in your top_level reporting method
// see PerformanceDebugger::report_context for an example
class Reporter {
  PerformanceDebugger* d;
public:
  Reporter(PerformanceDebugger* d) { this->d = d; d->start_report(); }
  ~Reporter() { d->stop_report(); }
};

void PerformanceDebugger::finish_reporting() {
  // output messages about non-inlined sends
  if (DebugPerformance && notInlinedBecauseNmethodTooBig) {
    Reporter r(this);
    str->print("  did not inline the following sends because the nmethod was getting too big:");
    int len = notInlinedBecauseNmethodTooBig->length();
    for (int i = 0; i < min(9, len); i++) {
      if (i % 3 == 0) str->print("\n    ");
      InlinedScope* s = notInlinedBecauseNmethodTooBig->at(i);
      str->print("%s  ", s->key()->print_string());
    }
    if (i < len) str->print("\n    (%d more sends omitted)\n", len);
    str->put('\n');
  }
}

void PerformanceDebugger::report_context(InlinedScope* s) {
  if (!DebugPerformance) return;
  Reporter r(this);
  GrowableArray<Expr*>* temps = s->contextTemporaries();
  const int len = temps->length();
  int nused = 0;
  for (int i = 0; i < len; i++) {
    PReg* r = temps->at(i)->preg();
    if (r->uplevelR() || r->uplevelW() || (r->isBlockPReg() && !r->isUnused())) nused++;
  }
  if (nused == 0) {
    str->print("  could not eliminate context of scope %s (fixable compiler restriction; should be eliminated)\n", s->key()->print_string());
  } else {
    str->print("  could not eliminate context of scope %s; temp(s) still used: ", s->key()->print_string());
    for (int j = 0; j < len; j++) {
      PReg* r = temps->at(j)->preg();
      if (r->uplevelR() || r->uplevelW()) {
	str->print("%d ", j);
      } else if (r->isBlockPReg() && !r->isUnused()) {
	str->print("%d (non-inlined block)", j);
      }
    }
    str->print("\n");
  } 
}

void PerformanceDebugger::report_toobig(InlinedScope* s) {
  if (!DebugPerformance) return;
  report_compile();
  if (!notInlinedBecauseNmethodTooBig) notInlinedBecauseNmethodTooBig = new GrowableArray<InlinedScope*>(20);
  notInlinedBecauseNmethodTooBig->append(s);
}

void PerformanceDebugger::report_uncommon(bool reoptimizing) {
  if (!DebugPerformance) return;
  Reporter r(this);
  if (reoptimizing) {
    str->print(" -- reoptimizing previously compiled 'uncommon' version of nmethod\n");
  } else {
    str->print(" -- creating 'uncommon' version of nmethod\n");
  }
}

void PerformanceDebugger::report_prim_failure(primitive_desc* pd) {
  // suppress methods for uncommon compiles -- too many (and not interesting)
  if (!DebugPerformance || theCompiler->is_uncommon_compile()) return;
  Reporter r(this);
  str->print(" primitive failure of %s not uncommon\n", pd->name());
}

void PerformanceDebugger::report_block(Node* n, BlockPReg* blk, char* what) {
  if (!DebugPerformance) return;
  if (blocks->contains(blk)) return;
  if (blk->method()->is_clean_block()) return;
  Reporter r(this);
  str->print(" could not eliminate block in ");
  blk->method()->home()->selector()->print_symbol_on(str);
  str->print(" because it is %s in scope %s at bytecode %d", 
             what, n->scope()->key()->print_string(), n->bci());
  InterpretedIC* ic = n->scope()->method()->ic_at(n->bci());
  if (ic) str->print(" (send of %s)", ic->selector()->copy_null_terminated());
  str->print("\n");
  blocks->append(blk);
}

#endif
