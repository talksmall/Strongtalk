/* Copyright 1996 LongView Technologies L.L.C. $Revision: 1.1 $ */
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

# ifdef DELTA_COMPILER
# include "incls/_mapConformance.cpp.incl"

void Variable::print() {
  if (in_register()) {
    std->print("R%d", offset());
  } else if (on_stack()) {
    std->print("S[%d]", offset());
  } else if (is_unused()) {
    std->print("UN");
  } else if (is_top_of_stack()){
    std->print("tos");
  } else {
    fatal("invalid variable");
  }
}


Variable MapConformance::pop_temporary() {
  Variable result = Variable::top_of_stack();
  if (!_free_register.is_unused()) {
    result = _free_register;
    _free_register = Variable::unused();
  }
  return result;
}

void MapConformance::push_temporary(Variable var) {
  if (var.in_register()) {
    _free_register = var;
  }
}

void MapConformance::push(Variable src, int n) {
  for (int index = 0; index < n; index++) {
    push(src);
  }
}
class MappingEntry : public ValueObj {
 private:
  Variable _reg;
  Variable _stack;
 public:
  MappingEntry(Variable reg, Variable stack) {
    _reg   = reg;
    _stack = stack;
  }
  Variable reg()   { return _reg;  }
  Variable stack() { return _stack; }

  bool has_reg()   { return !reg().is_unused(); }
  bool has_stack() { return !stack().is_unused(); }

  void set_reg(Variable r)   { _reg = r; }
  void set_stack(Variable s) { _stack = s; }

  void print();
};

void MappingEntry::print() {
  if (!reg().is_unused())   reg().print();
  if (!stack().is_unused()) stack().print();
}

bool operator == (MappingEntry x, MappingEntry y) {
  return x.reg() == y.reg() && x.stack() == y.stack();
}

class MappingTask : public ResourceObj {
 private:
  MappingTask* _next;         // next task with same source
  MappingTask* _parent;       // parent chain for recursion
  bool         _is_processed; 
  char*        _what_happend; // what happend to this task
  bool         _uses_top_of_stack;
  Variable     _variable_to_free;
 public:
  MappingTask(Variable src_register, Variable src_stack, Variable dst_register, Variable dst_stack) 
  : src(src_register, src_stack), dst(dst_register, dst_stack) {
    _next         = NULL;
    _is_processed = false;
    _what_happend = "Nothing";
    _parent       = NULL;
    _uses_top_of_stack = false;
    _variable_to_free = Variable::unused();
  }

  bool is_processed() const { return _is_processed; }
  void set_processed(char* reason) {
    _is_processed = true;
    _what_happend = reason;
  }

  void append(MappingTask* son) {
    assert(!is_processed(),      "should be un touched");
    assert(!son->is_processed(), "should be un touched");
    son->set_next(next());
    set_next(son);
    son->set_processed("Linked");
  }

  MappingTask* next() const { return _next; }
  void set_next(MappingTask* n) { _next = n; }

  MappingTask* parent() const { return _parent; }
  void set_parent(MappingTask* p) { _parent = p; }

  Variable variable_to_free() const { return _variable_to_free; }
  void set_variable_to_free(Variable var) { _variable_to_free = var; }

  bool uses_top_of_stack() const { return _uses_top_of_stack; }
  void set_uses_top_of_stack(bool b) { _uses_top_of_stack = b; }

  MappingEntry src;
  MappingEntry dst;

  void process_task(MapConformance* mc, MappingTask* p);
  void generate_code(MapConformance* mc);

  bool target_includes(Variable var);
  bool is_dependent(MapConformance* mc, MappingTask* task);
  bool in_parent_chain(MappingTask* task);

  int  number_of_targets();

  void print(int index);
};

int MappingTask::number_of_targets() {
  int result = 0;
  for (MappingTask* current = this; current; current = current->next()) {
    if (current->dst.has_reg())   result++;
    if (current->dst.has_stack()) result++;
  }
  return result;
}


bool MappingTask::in_parent_chain(MappingTask* task) {
  for (MappingTask* current = this; current; current = current->parent()) {
    if (current == task) return true;
  }
  return false;
}

bool MappingTask::target_includes(Variable var) {
  for (MappingTask* current = this; current; current = current->next()) {
    if (current->dst.reg() == var || current->dst.stack() == var)
      return true;
  }
  return false;
}


bool MappingTask::is_dependent(MapConformance* mc, MappingTask* task) {
  // Do we have to process task before this?
  // => do task->results overlap with src?
  if (this == task) return false;

  bool is_reg_dependent   = false;
  bool is_stack_dependent = false;

  if (task->src.has_reg()) {
    is_reg_dependent = target_includes(task->src.reg());
  }

  if (task->src.has_stack()) {
    is_stack_dependent = target_includes(task->src.stack());
  }

  if (is_reg_dependent || is_stack_dependent) {
    if (in_parent_chain(task)) {
      // A....task......this
      if (task->src.has_reg()) {
        if (task->src.has_stack()) {
          task->src.set_stack(Variable::unused());
        }
        Variable temp = mc->pop_temporary();
	if (temp.is_top_of_stack()) {
	  mc->push(task->src.reg(), number_of_targets());
          task->src.set_reg(temp);
	  task->set_uses_top_of_stack(true);
	} else {
	  mc->move(task->src.reg(), temp);
          task->src.set_reg(temp);
	  task->set_variable_to_free(temp);
	}
      } else {
        Variable temp = mc->pop_temporary();
	if (temp.is_top_of_stack()) {
	  mc->push(task->src.stack(), number_of_targets());
          task->src.set_reg(temp);
	  task->set_uses_top_of_stack(true);
	} else {
	  mc->move(task->src.stack(), temp);
          task->src.set_reg(temp);
	  task->src.set_stack(Variable::unused());
	  task->set_variable_to_free(temp);
	}
      }
      return false;
    }
  }

  return is_reg_dependent || is_stack_dependent;
}

void MappingTask::process_task(MapConformance* mc, MappingTask* p) {
  if (is_processed()) return;

  //Is anybody dependent on source?
  set_parent(p);
  for (int index = 0; index < mc->mappings->length(); index++) {
    MappingTask* task = mc->mappings->at(index);
    if (!task->is_processed() && is_dependent(mc, task)) {
      task->process_task(mc, this);
    }
  }
  set_parent(NULL);

  // Generate code for task
  generate_code(mc);
  mc->push_temporary(variable_to_free());
  set_processed("Code has been generated");
}

void MappingTask::generate_code(MapConformance* mc) {
  bool uses_temp = false;

  if (uses_top_of_stack()) {
    //Use source register for moves
    for (MappingTask* current = this; current; current = current->next()) {
      if(current->dst.has_reg()) {
	mc->pop(current->dst.reg()); 
      }
      if (dst.has_stack() && !(src.stack() == dst.stack())) {
        mc->pop(current->dst.stack());
      }
    }
  } else if (src.has_reg()) {
    //Use source register for moves
    for (MappingTask* current = this; current; current = current->next()) {
      if(current->dst.has_reg()) {
	mc->move(src.reg(), current->dst.reg()); 
      }
      if (current->dst.has_stack() && !(src.stack() == current->dst.stack())) {
        mc->move(src.reg(), current->dst.stack());
      }
    }
  } else {
    //Use register in target or free register
    Variable temp;
    for (MappingTask* current = this; current; current = current->next()) {
      if(current->dst.has_reg()) temp = current->dst.reg();
    }
    if (temp.is_unused()) {
      uses_temp = true;
      temp = mc->pop_temporary();
    }
    if (temp.in_register()) {
      //We found a temporary register
      mc->move(src.stack(), temp);
      for (MappingTask* current = this; current; current = current->next()) {
        if(current->dst.has_reg() && !(current->dst.reg() == temp)) {
	  mc->move(temp, current->dst.reg()); 
        }
        if (current->dst.has_stack() && !(src.stack() == current->dst.stack())) {
          mc->move(temp, current->dst.stack());
        }
      }
      if (uses_temp) {
        mc->push_temporary(temp);
      }
    } else {
      for (MappingTask* current = this; current; current = current->next()) {
        if(current->dst.has_reg()) {
	  mc->push(src.stack());
	  mc->pop(current->dst.reg()); 
        }
        if (current->dst.has_stack() && !(src.stack() == current->dst.stack())) {
	  mc->push(src.stack());
	  mc->pop(current->dst.stack()); 
        }
      }
    }
  }
}

void MappingTask::print(int index) {
  std->print("  %2d: ", index);
  src.print();
  std->print(" -> ");
  dst.print();
  if (_what_happend != NULL) {
    std->set_indentation(35);
    std->indent();
    std->print("%s", _what_happend);
  }
  std->cr();
}

MapConformance::MapConformance() {
  mappings       = new GrowableArray<MappingTask*>(20);
  used_variables = NULL;
  _free_register.set_unused();
}

void MapConformance::append_mapping(Variable src_register, Variable src_stack, Variable dst_register, Variable dst_stack) {
  mappings->append(new MappingTask(src_register, src_stack, dst_register, dst_stack));
}

void MapConformance::generate(Variable free_register1, Variable free_register2) {
 this->_free_register  = free_register1;
 // There is max. 2 used variables per mapping.
 this->used_variables = NEW_RESOURCE_ARRAY(Variable, mappings->length() * 2);
 this->number_of_used_variables = 0;

 simplify();
 process_tasks();

 this->_free_register.set_unused();
 this->used_variables = NULL;
}

void MapConformance::move(Variable src, Variable dst) {
  std->print("	move  ");
  src.print();
  std->print(",	");
  dst.print();
  std->cr();
}

void MapConformance::push(Variable src) {
  std->print("	push  ");
  src.print();
  std->cr();
}

void MapConformance::pop(Variable dst) {
  std->print("	pop  ");
  dst.print();
  std->cr();
}

void MapConformance::print() {
  std->print_cr("MapConformance");
  for (int index = 0; index < mappings->length(); index++) {
    mappings->at(index)->print(index);
  }
}

bool MapConformance::reduce_noop_task(MappingTask* task) {
  // A noop task if destination is a subset of source
  bool result = true;

  if (task->dst.has_reg() && task->src.reg() != task->dst.reg()) {
    result = false;
  }

  if (task->dst.has_stack() && task->src.stack() != task->dst.stack()) {
    result = false;
  }

  if (result) task->set_processed("Noop");
  return result;
}

void MapConformance::simplify() {
  // Links tasks with identical source.
  for (int x = 0; x < mappings->length(); x++) {
    MappingTask* x_task = mappings->at(x);
    if (!x_task->is_processed() && !reduce_noop_task(x_task)) {
      for (int y = x + 1; y < mappings->length(); y++) {
        MappingTask* y_task = mappings->at(y);
        if (x_task->src == y_task->src) x_task->append(y_task);
      }
    }
  }
}

void MapConformance::process_tasks() {
  for (int index = 0; index < mappings->length(); index++) {
    mappings->at(index)->process_task(this, NULL);
  }
}

#endif // DELTA_COMPILER
