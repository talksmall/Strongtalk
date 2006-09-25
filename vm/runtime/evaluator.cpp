/* Copyright 1994, 1995 LongView Technologies L.L.C. $Revision: 1.45 $ */
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
# include "incls/_evaluator.cpp.incl"

// The single_step_handler is called from single_step_stub
// when a single step has taken place
// return the distance between the current and the next_bci
extern "C" void single_step_handler() {
  ResourceMark rm;
  evaluator::single_step(DeltaProcess::active()->last_Delta_fp());
}

int* saved_frame;

bool patch_last_delta_frame(int* fr, int* dist) {
  // change the current to next bci;
  frame v(NULL, fr, NULL);

  methodOop method = methodOopDesc::methodOop_from_hcode(v.hp());

  // The interpreter is in the middle of executing a byte code 
  // and the hp pointer points to the current bci and NOT the next bci.
  int bci = method->next_bci_from(v.hp());

  // in case of single step we ignore the case with
  // an empty inline cache since the send is reexecuted.
  InterpretedIC* ic = method->ic_at(bci);
  if (ic && ic->is_empty()) return false;

  *dist = method->next_bci(bci) - bci;
  v.set_hp(v.hp() + *dist);
  return true;
}

void restore_hp(int* fr, int dist) {
  frame v(NULL, fr, NULL);
  v.set_hp(v.hp() - dist);
}

static bool is_aborting = false;

void evaluator::single_step(int* fr) {
  int dist;
  if (!patch_last_delta_frame(fr, &dist)) return;
  
  deltaVFrame* df = DeltaProcess::active()->last_delta_vframe();
  assert(df, "delta frame must be present");

  { // Always show code at single step
    FlagSetting(ActivationShowCode, true);
    df->print_activation(1);
  }
  saved_frame = fr;
  read_eval_loop();

  restore_hp(fr, dist);

  if (is_aborting) {
    is_aborting = false;
    ErrorHandler::abort_current_process();
  }
}

bool evaluator::get_line(char* line) {
  int end = 0;
  int c;
  while (((c = getchar()) != EOF) && (c != '\n'))
    line[end++] = c;
  while ((end > 0) && ((line[end-1] == ' ') || (line[end-1] == '\t')))
    end--;
  line[end] = '\0';
  return c != EOF;
}


class TokenStream : public StackObj {
 private:
  GrowableArray<char*>* tokens;
  int pos;
  void tokenize(char* str);
  bool match(char* str) { return strcmp(current(), str) == 0; }
 public:
  TokenStream(char* line) {
    tokens = new GrowableArray<char*>(10);
    tokenize(line);
    pos = 0;
  }
  char* current() { return tokens->at(pos); }
  void  advance() { pos++; }
  bool  eos()     { return pos >= tokens->length(); }

  // testers
  bool is_hat()     { return match("^"); }
  bool is_step()    { return match("s") || match("step");    }
  bool is_next()    { return match("n") || match("next");    }
  bool is_end()     { return match("e") || match("end");     }
  bool is_cont()    { return match("c") || match("cont");    }
  bool is_stack()   { return               match("stack");   }
  bool is_abort()   { return               match("abort");   }
  bool is_genesis() { return               match("genesis"); }
  bool is_top()     { return               match("top");     }
  bool is_show()    { return               match("show");    }
  bool is_break()   { return               match("break");   }
  bool is_events()  { return               match("events");  }
  bool is_status()  { return               match("status");  }
  bool is_help()    { return match("?") || match("help");    }
  bool is_quit()    { return match("q") || match("quit");    }
  bool is_plus()    { return match("+"); }
  bool is_minus()   { return match("-"); }

  bool is_smi(oop* addr);
  bool is_table_entry(oop* addr);
  bool is_object_search(oop* addr);
  bool is_name(oop* addr);
  bool is_symbol(oop* addr);

  bool is_unary();
  bool is_binary();
  bool is_keyword();
};


static char* seps = " \t\n";

void TokenStream::tokenize(char* str) {
  char* token = strtok(str, seps);
  while( token != NULL ) {
    tokens->push(token);
    token = strtok(NULL, seps);
  }
}


bool TokenStream::is_smi(oop* addr) {
  int value;
  unsigned int length;
  if (sscanf(current(), "%d%n", &value, &length) == 1 && strlen(current()) == length) {
    *addr = as_smiOop(value);
    return true;
  }
  return false;
}


bool TokenStream::is_table_entry(oop* addr) {
  int value;
  unsigned int length;
  if (sscanf(current(), "!%d%n", &value, &length) == 1 && strlen(current()) == length) {
    if(!objectIDTable::is_index_ok(value)) {
      std->print_cr("Could not find index %d in object table.", value);
      return true;
    }
    *addr = objectIDTable::at(value);
    return true;
  }
  return false;
}


bool TokenStream::is_object_search(oop* addr) {
  int address;
  oop obj;
  unsigned int length;
  if (sscanf(current(), "0x%x%n", &address, &length) == 1 && strlen(current()) == length) {
    if (obj = oop(Universe::object_start((oop*) address))) {
      *addr = obj;
      return true;
    }
  }
  return false;
}

bool TokenStream::is_name(oop* addr) {
  char name[200];
  oop   obj;
  unsigned int length; 
  if (sscanf(current(), "%[a-zA-Z]%n", name, &length) == 1 && strlen(current()) == length) {
    if (obj = Universe::find_global(name)) { *addr = obj ; return true; }
  }
  return false;
}

bool TokenStream::is_symbol(oop* addr) {
  char name[200];
  unsigned int length; 
  if (sscanf(current(), "#%[a-zA-Z0-9_]%n", name, &length) == 1 && strlen(current()) == length) {
    *addr = oopFactory::new_symbol(name);
    return true;
  }
  return false;
}

bool TokenStream::is_unary() {
  char name[40];
  unsigned int length;
  return sscanf(current(), "%[a-zA-Z]%n", name, &length) == 1 && strlen(current()) == length;
}

bool TokenStream::is_binary() { return !is_unary() && !is_keyword();}

bool TokenStream::is_keyword() {
  char name[40];
  unsigned int length;
  return sscanf(current(), "%[a-zA-Z]:%n", name, &length) == 1 && strlen(current()) == length;
}

bool evaluator::get_oop(TokenStream* st, oop* addr) {
  if (st->is_smi(addr))           { st->advance(); return true; }
  if (st->is_table_entry(addr))   { st->advance(); return true; }
  if (st->is_object_search(addr)) { st->advance(); return true; }
  if (st->is_name(addr))          { st->advance(); return true; }
  if (st->is_symbol(addr))        { st->advance(); return true; }
  std->print_cr("Error: could not oop'ify [%s]", st->current());
  return false; 
}


bool validate_lookup(oop receiver, symbolOop selector) {
 LookupKey key(receiver->klass(), selector);
 if (lookupCache::lookup(&key).is_empty()) {
   std->print_cr("Lookup error");
   key.print_on(std);
   std->cr();
   return false;
 }
 return true;
}

void evaluator::eval_message(TokenStream* st) {
  oop receiver;
  oop result = nilObj;
  symbolOop selector;

  if (st->eos()) return;
  if (!get_oop(st, &receiver)) return;
  if (st->eos()) {
    receiver->print();
  } else if (st->is_unary()) {
    symbolOop selector = oopFactory::new_symbol(st->current());
    if (!validate_lookup(receiver, selector)) return;
    result = Delta::call(receiver, selector);
  } else if (st->is_binary()) {
    selector = oopFactory::new_symbol(st->current());
    if (!validate_lookup(receiver, selector)) return;
    oop argument;
    st->advance();
    if (!get_oop(st, &argument)) return;
    result = Delta::call(receiver, selector, argument);
  } else if (st->is_keyword()) {
    char name[100];
    oop  arguments[10];
    int  nofArgs = 0;
    name[0] = '\0';
    while (!st->eos()) {
      strcat(name, st->current());
      st->advance();
      oop arg;
      if (!get_oop(st, &arg)) return;
      arguments[nofArgs++] = arg;
    }
    selector = oopFactory::new_symbol(name);
    if (!validate_lookup(receiver, selector)) return;
    static DeltaCallCache cache;
    result = Delta::call_generic(&cache, receiver, selector, nofArgs, arguments);
  }
  result->print_value();
  std->cr();
}

void evaluator::top_command(TokenStream* st) {
  int number_of_frames_to_show = 10;
  st->advance();
  if (!st->eos()) {
    oop value;
    if (st->is_smi(&value)) {
      number_of_frames_to_show = smiOop(value)->value();
    }
    st->advance();
    if (!st->eos()) {
      std->print_cr("warning: garbage at end");
    }
  }
  DeltaProcess::active()->trace_top(1, number_of_frames_to_show);
};

void evaluator::change_debug_flag(TokenStream* st, bool value) {
  st->advance();
  if (!st->eos()) {
    st->current();
    bool r = value;
    if (!debugFlags::boolAtPut(st->current(), &r)) { 
      std->print_cr("boolean flag %s not found", st->current());
    }
    st->advance();
    if (!st->eos()) {
      std->print_cr("warning: garbage at end");
    }
  } else {
    std->print_cr("boolean flag expected");
  }
}

void evaluator::show_command(TokenStream* st) {
  int start_frame              = 1;
  int number_of_frames_to_show = 1;

  st->advance();
  if (!st->eos()) {
    oop value;
    if (st->is_smi(&value)) {
      start_frame = smiOop(value)->value();
    }
    st->advance();
    if (!st->eos()) {
      if (st->is_smi(&value)) {
        number_of_frames_to_show = smiOop(value)->value();
      }
      st->advance();
      if (!st->eos()) {
        std->print_cr("warning: garbage at end");
      }
    }    
  }
  DeltaProcess::active()->trace_top(start_frame, number_of_frames_to_show);               
};

bool evaluator::process_line() {
  char line[200];
  if (!get_line(line)) return false;

  TokenStream st(line);
  if (st.eos()) return true;

  if (st.is_hat()) {
    st.advance();
    dispatchTable::reset();
    eval_message(&st);
    return true;
  } else {
    if (st.is_help())    { print_help();                                     return true;  }
    if (st.is_step())    { dispatchTable::intercept_for_step();              return false; }
    if (st.is_next())    { dispatchTable::intercept_for_next(saved_frame);   return false; }
    if (st.is_end())     { dispatchTable::intercept_for_return(saved_frame); return false; }
    if (st.is_cont())    { dispatchTable::reset();                           return false; }
    if (st.is_stack())   { DeltaProcess::active()->trace_stack();            return true;  }
    if (st.is_quit())    { os::fatalExit(0);                                 return true;  }
    if (st.is_break())   { fatal("evaluator break");                         return true;  }
    if (st.is_events())  { eventLog->print();                                return true;  }
    if (st.is_top())     { top_command(&st);                                 return true;  }
    if (st.is_show())    { show_command(&st);                                return true;  }
    if (st.is_plus())    { change_debug_flag(&st, true);                     return true;  }
    if (st.is_minus())   { change_debug_flag(&st, false);                    return true;  }
    if (st.is_status())  { print_status();                                   return true;  }
    if (st.is_abort())   {
      if (DeltaProcess::active()->is_scheduler()) {
        std->print_cr("You cannot abort in the scheduler");
	std->print_cr("Try another command");
      } else {
        dispatchTable::reset(); 
        is_aborting = true; 
        return false; 
      }
    }
    if (st.is_genesis()) { 
      dispatchTable::reset(); 
      VM_Genesis op;
      VMProcess::execute(&op);
      return false; 
    }
    oop receiver;
    if (get_oop(&st, &receiver)) {
      st.advance();
      if (!st.eos()) {
        std->print_cr("warning: garbage at end");
      }
      receiver->print();
      std->cr();
      return true;
    }
  }
  print_mini_help();
  return true;
}


void evaluator::read_eval_loop() {
  do {
    std->print("Eval> ");
  } while (process_line());
}


void evaluator::print_mini_help() {
  std->print_cr("Use '?' for help ('c' to continue)");
}

class ProcessStatusClosure : public ProcessClosure {
 private:
  int index;
 public:
   ProcessStatusClosure() { index = 1; }

  void do_process(DeltaProcess* p) {
    std->print(" %d:%s ", index++, DeltaProcess::active() == p ? "*" : " ");
    p->print();
  }
};

void evaluator::print_status() {
  std->print_cr("Processes:");
  ProcessStatusClosure iter;
  Processes::process_iterate(&iter);
}

void evaluator::print_help() {
  std->cr();
  std->print_cr("<command>  ::= 'q'     | 'quit'    -> quits the system");
  std->print_cr("             | 's'     | 'step'    -> single step byte code");
  std->print_cr("             | 'n'     | 'next'    -> single step statement");
  std->print_cr("             | 'e'     | 'end'     -> single step to end of method");
  std->print_cr("             | 'c'     | 'cont'    -> continue execution");
  std->print_cr("                       | 'abort'   -> aborts the current process");
  std->print_cr("                       | 'genesis' -> aborts all processes and restarts the scheduler");
  std->print_cr("                       | 'break'   -> provokes fatal() to get into C++ debugger");
  std->print_cr("                       | 'event'   -> prints the event log");
  std->print_cr("                       | 'stack'   -> prints the stack of current process");
  std->print_cr("                       | 'status'  -> prints the status all processes");
  std->print_cr("                       | 'top' <n> -> prints the top of current process");
  std->print_cr("                       | 'show' <s> <n> -> prints some activation");
  std->print_cr("             | '?'     | 'help'    -> prints this help\n");
  std->print_cr("             | '^' <expr>          -> evaluates the expression");
  std->print_cr("             | '-' name            -> turns off debug flag");
  std->print_cr("             | '+' name            -> turns on debug flag");
  std->print_cr("             | <object>            -> prints this object\n");
  std->cr();
  std->print_cr("<expr>     ::= <unary>  | <binary>  | <keyword>\n");
  std->print_cr("<object>   ::= <number>            -> smi(number)");
  std->print_cr("             | !<number>           -> objectTable[number]");
  std->print_cr("             | 0x<hex_number>      -> object_start(number)");
  std->print_cr("             | name                -> Smalltalk at: #name");
  std->print_cr("             | #name               -> new_symbol(name)");
  std->cr();
}