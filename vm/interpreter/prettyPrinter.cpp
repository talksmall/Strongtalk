/* Copyright 1994, LongView Technologies L.L.C. $Revision: 1.73 $ */
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
# include "incls/_prettyPrinter.cpp.incl"

// ToDo list for pretty printer
// - convert the stream stuff to ostream.
// - improve the namedesc printout for temps etc.

class cascadeSendNode;
class statement;
class codeNode;

class astNode : public ResourceObj {
 protected:
  int        bci;
  scopeNode* scope;
 public:
  astNode(int bci, scopeNode* scope) { 
    this->bci   = bci;
    this->scope = scope;
  }
  int        this_bci()   { return bci;   }
  scopeNode* this_scope() { return scope; }

  virtual bool is_message()           { return false; }
  virtual bool is_statement()         { return false; }
  virtual bool is_code()              { return false; }
  virtual bool is_cascade()           { return false; }

  virtual void add(astNode* statement) {
    fatal("subclass should implement add");
  }

  void top_level_print(prettyPrintStream* output);

  virtual bool print(prettyPrintStream* output);
  virtual int  width(prettyPrintStream* output) { return output->infinity(); }

  virtual symbolOop selector() { return NULL; }
  virtual bool should_wrap_argument(astNode* argument) { return false; };
  virtual astNode*  argument_at(int i) { return NULL; }
};


class PrintWrapper {
 private:
  astNode* t;
  bool hit;
  prettyPrintStream* output;
 public:
  PrintWrapper(astNode* t, prettyPrintStream* output);
  ~PrintWrapper();
};


# define HIGHLIGHT PrintWrapper pw(this, output);

# include <ctype.h>

bool should_wrap(int type, astNode* arg); 


static bool print_selector_with_arguments(prettyPrintStream*       output, 
                                          symbolOop                selector,
                                          GrowableArray<astNode*>* arguments,
                                          bool                     split) {
  if (selector->is_binary()) {
    // binary send
    output->print(selector->as_string());
    output->space();
    astNode* arg = arguments->at(0);
    bool wrap = should_wrap(1, arg);
    if (wrap) output->print("(");
    bool result = arg->print(output);
    if (wrap) output->print(")");
    return result;
  }

  int arg = arguments->length();

  if (arg == 0) {
    output->print(selector->as_string());
    return false;
  }

  for (int index = 1; index <= selector->length(); index++) {
    int c = selector->byte_at(index);
    output->print_char(c);
    if (c == ':') {
      output->space();
      astNode* a = arguments->at(--arg);
      bool wrap = should_wrap(2, a);
      if (wrap) output->print("(");
      a->print(output); 
      if (wrap) output->print(")");
         
      if (index < selector->length()) {
      	if (split) output->newline();
        else       output->space();
      }
    }
  }
  return split;
}


static astNode* get_literal_node(oop obj, int bci, scopeNode* scope);

class PrintTemps : public TempDecoder {
 public:
  GrowableArray<astNode*>* elements;
  scopeNode* scope;
  void decode(methodOop method, scopeNode *scope) {
    this->scope = scope;
    elements = new GrowableArray<astNode*>(10);
    TempDecoder::decode(method);
 }
 void stack_temp(byteArrayOop name, int offset);
 void heap_temp(byteArrayOop name, int offset);
};


class PrintParams : public TempDecoder {
  scopeNode* scope;
 public:
  GrowableArray<astNode*>* elements;
  void decode(methodOop method, scopeNode* scope) {
    this->scope = scope;
    elements = new GrowableArray<astNode*>(10);
    TempDecoder::decode(method);
 }
 void parameter(byteArrayOop name, int index);
};


class leafNode : public astNode {
 public:
   leafNode(int bci, scopeNode* scope) : astNode(bci, scope) {}
   bool print(prettyPrintStream* output) { 
     HIGHLIGHT
     astNode::print(output);
     output->print(string());
     return false;
   }
   int  width(prettyPrintStream* output) { return output->width_of_string(string()); }
   virtual char* string() = 0;
};


class paramNode : public leafNode {
 private:
   int   no;
   char* str;
 public:
   paramNode(int bci, scopeNode* scope, int no);
   char* string()  { return str;  }
   bool is_param() { return true; }
};


class nameValueNode : public astNode {
  char* name;
  char* value;
 public:
   nameValueNode(char* name, char* value) : astNode(0, 0) {
     this->name  = name;
     this->value = value;
   }
   bool print(prettyPrintStream* output) { 
     output->print(name);
     output->print(" \"");
     output->print(value);
     output->print("\"");
     return false;
   }
   int  width(prettyPrintStream* output) { 
     return output->width_of_string(name) + 3 + output->width_of_string(value);
   }
};


class nameNode : public leafNode {
 private:
   char* str;
 public:
   nameNode(char* str) : leafNode(0,0) {
     this->str = str;
   }
   char* string()  { return str;  }
};


class listNode : public astNode {
 protected:
   GrowableArray<astNode*>* elements;
   char* begin_sym;
   char* end_sym;
 public:
  listNode(char* begin_sym, char* end_sym) : astNode(0,0) {
    this->begin_sym = begin_sym;
    this->end_sym   = end_sym;
    elements  = new GrowableArray<astNode*>(10);
  }

  void add(astNode* element) {
    elements->push(element);
  }

  bool print(prettyPrintStream* output) {
    if (begin_sym) {
      output->print(begin_sym);
      output->space();
    }

    for (int index = 0; index < elements->length(); index++) {
      elements->at(index)->print(output);
      output->space();
    }

    if (end_sym) 
      output->print(end_sym);
    return false;
  }

  int width(prettyPrintStream* output) {
    int w = 0;
    if (begin_sym)
      w += output->width_of_string(begin_sym) + output->width_of_space();

    for (int index = 0; index < elements->length(); index++) {
      w += elements->at(index)->width(output) + output->width_of_space();
    }

    if (end_sym)
      w += output->width_of_string(end_sym);
    return w;
  }

};


class scopeNode : public astNode {
 protected:
   methodOop   _method;
   klassOop     klass;
   int          in;
   int         _hot_bci;
   int          frame_index;
   deltaVFrame*_fr;
   ScopeDesc*  _sd;
   scopeNode*   parentScope;
   scopeNode*   innerScope;
 public:
   scopeNode(deltaVFrame* fr, int index, scopeNode* scope = NULL) 
  : astNode(0, NULL) {
    frame_index = index;
    _method     = fr->method();
    klass       = fr->receiver()->klass();
    _hot_bci    = scope ? -1 : fr->bci();
    _fr         = fr;
    in = 0;
    _sd = _fr->is_compiled_frame() 
          ? ((compiledVFrame*) _fr)->scope()
	  : NULL;
    innerScope = scope;
    initParent();
  }

  scopeNode(methodOop method, klassOop klass, int hot_bci, scopeNode* scope = NULL)
  : astNode(0, NULL) {
    _method        = method;
    this->klass    = klass;
    this->_hot_bci = hot_bci;
    _fr = NULL;
    in = 0;
    _sd = NULL;
    innerScope = scope;
    initParent();
  }
  void initParent() {
    parentScope = _method->is_blockMethod()
      ? new scopeNode(_method->parent(), klass, -1, this) 
      : NULL;
  }
  deltaVFrame* fr() { return _fr; }
  ScopeDesc* sd() const { return _sd; }

  methodOop method() { return _method; }

  int hot_bci() { return _hot_bci; } 

  klassOop get_klass() { return klass; }

  bool is_block_method() { return method()->is_blockMethod(); }

  void context_allocated()    { assert(is_context_allocated(), "just checking");  }
  bool is_context_allocated() { return method()->allocatesInterpretedContext(); }
  scopeNode* scopeFor(methodOop method) {
    return innerScope && innerScope->isFor(method) ? innerScope : NULL;
  }
  bool isFor(methodOop method) {
    return _method == method;
  }
  scopeNode* homeScope() { return parentScope ? parentScope->homeScope() : this; }
  char* param_string(int index, bool in_block = false) {
    byteArrayOop res = find_parameter_name(method(), index);
    if (in_block) {
      if (!res) return create_name(":p", index);
      char* buffer = NEW_RESOURCE_ARRAY(char, res->length() + 2);
      buffer[0] = ':';
      res->copy_null_terminated(&buffer[1], res->length());
      return buffer;
    } else {
      return res ? res->as_string() : create_name("p", index);
    }
  }

  char* stack_temp_string(int bci, int no) {
    byteArrayOop res = find_stack_temp(method(), bci, no);
    return res ? res->as_string() : create_name("t", no);
  }

  char* stack_float_temp_string(int bci, int fno) {
    byteArrayOop res = find_stack_float_temp(method(), bci, fno);
    return res ? res->as_string() : create_name("f", fno);
  }

  char* create_name(char* prefix, int no) {
    char* str = NEW_RESOURCE_ARRAY(char, 7);
    sprintf(str, "%s_%d", prefix, no);
    return str;
  }

  char* heap_temp_string(int bci, int no, int context_level) {
    if (is_block_method()) {
      int level = context_level;
      if (is_context_allocated()) {
        level--;
	if (context_level == 0) {
          byteArrayOop res = find_heap_temp(method(), bci, no);
          return res ? res->as_string() : create_name("ht", no);
	}
      }
      scopeNode* n = new scopeNode(method()->parent(), klass, 0);
      return n->heap_temp_string(bci, no, level);
    } else {
      byteArrayOop res = find_heap_temp(method(), bci, no);
      return res ? res->as_string() : create_name("ht", no);
    }
  }

  char* inst_var_string(int offset) {
    if (klass) {
      symbolOop name = klass->klass_part()->inst_var_name_at(offset);
      if (name) return name->as_string();
    }
    char* str = NEW_RESOURCE_ARRAY(char, 15);
    sprintf(str, "i_%d", offset);
    return str;
  }

  astNode* temps() {
    PrintTemps p;
    p.decode(method(), this);
    if (p.elements->length() == 0) return NULL;
    listNode* l = new listNode("|", "|");
    for(int index = 0; index < p.elements->length(); index++)
      l->add(p.elements->at(index));
    return l;
  }

  astNode* params() {
    PrintParams p;
    p.decode(method(), this);
    if (p.elements->length() == 0) return NULL;

    listNode* l = new listNode(NULL, "|");
    for(int index = 0; index < p.elements->length(); index++) {
      l->add(p.elements->at(index));
    }
    return l;
  }

  astNode* heap_temp_at(int no) {
    char* name = heap_temp_string(0, no, 0);
    if (!fr()) return new nameNode(name);
    char* value = fr()->context_temp_at(no)->print_value_string();
    return new nameValueNode(name, value);
  }

  astNode* stack_temp_at(int no) {
    byteArrayOop res = find_stack_temp(method(), 0, no);
    char* name =  res ? res->as_string() : create_name("t", no);
    if (!fr()) return new nameNode(name);
    char* value = fr()->temp_at(no)->print_value_string();
    return new nameValueNode(name, value);
  }

  astNode* parameter_at(int index, bool in_block = false) {
    char* name = param_string(index, in_block);
    if (!fr()) return new nameNode(name);
    char* value = fr()->argument_at(index)->print_value_string();
    return new nameValueNode(name, value);
  }

  void print_frame_header(prettyPrintStream* output) {
    char num[10];
    output->newline();
    output->print("#");
    sprintf(num, "%d", frame_index);
    output->print(num);
    output->print(", receiver \"");
    output->print(fr()->receiver()->print_value_string());
    output->print("\"");

    if (!method()->is_blockMethod()) {
      klassOop method_holder = fr()->receiver()->klass()->klass_part()->lookup_method_holder_for(method());
      if (method_holder && (method_holder != fr()->receiver()->klass())) {
        output->print(", method holder \"");
        output->print(method_holder->print_value_string());
        output->print("\"");
      }
    }
    output->newline();

    print_method_header(output);    // print method header first (in case something crashes later)

    output->newline();

    if (ActivationShowBCI) {
      // Print current bci
      output->print(" [bci = ");  
      sprintf(num, "%d", _hot_bci);
      output->print(num);
      output->print("]");  
      output->newline();
    }
    
    if (ActivationShowExpressionStack) {
      // expression stack:
      GrowableArray<oop>* stack = fr()->expression_stack();
      if (stack->length() > 0) {
        output->print(" [expression stack:"); 
        output->newline();
        GrowableArray<oop>* stack = fr()->expression_stack();
        for (int index = 0; index < stack->length(); index++) {
          output->print(" - ");
          stack->at(index)->print_value();
	  output->newline();
        }
        output->print(" ]"); 
        output->newline();
      }
    }

    if (ActivationShowContext) {
      // Print the context if present
      contextOop con = NULL;
      if (fr()->is_interpreted_frame()) {
        con = ((interpretedVFrame*) fr())->interpreter_context();
      } else if (fr()->is_deoptimized_frame()) {
        con = ((deoptimizedVFrame*) fr())->deoptimized_context();
      } else if (fr()->is_compiled_frame()) {
        con = ((compiledVFrame*) fr())->compiled_context();
      }
      if (con) con->print();
    }
  }

  void print_header(prettyPrintStream* output) {
    if (fr()) {
      print_frame_header(output);
    } else {
      print_method_header(output);
    }
  } 

  void print_method_header(prettyPrintStream* output) {
    GrowableArray<astNode*>* arguments = new GrowableArray<astNode*>(10);
    for (int i = method()->number_of_arguments()-1; i >= 0; i--) {
      arguments->push(parameter_at(i));
    }
    print_selector_with_arguments(output, method()->selector(), arguments, false);
  }
};


paramNode::paramNode(int bci, scopeNode* scope, int no) : leafNode(bci, scope) {
  this->no  = no;
  this->str = scope->param_string(no);
}


PrintWrapper::PrintWrapper(astNode* t, prettyPrintStream* output) {
  this->t      = t;
  this->output = output;
  this->hit    = false;
  if(!t->this_scope()) return;
  if (t->this_bci() == t->this_scope()->hot_bci() && !output->in_highlight()) {
     output->begin_highlight();
     hit = true;
  }
}


PrintWrapper::~PrintWrapper() {
  if (hit) output->end_highlight();
}


bool astNode::print(prettyPrintStream* output) {
  if (ActivationShowNameDescs) {
    if (scope && scope->sd()) {
      NameDesc* nd = scope->sd()->exprStackElem(bci);
      if (nd) nd->print();
    }
  }
  return false;
}


void astNode::top_level_print(prettyPrintStream* output) {
  scope->print_header(output);
  print(output);
}


class codeNode : public astNode {
 protected:
   GrowableArray<astNode*>* statements;
 public:
  codeNode(int bci, scopeNode* scope) : astNode(bci, scope) {
    statements  = new GrowableArray<astNode*>(10);
  }

  void add(astNode* statement) {
    statements->push(statement);
  }

  bool print(prettyPrintStream* output);
  bool is_code() { return true; }

  statement* last_statement() {
    if (statements->length() > 0) {
      assert(statements->at(statements->length() - 1)->is_statement(), "must be statement");
      return (statement*) statements->at(statements->length() - 1);
    }
    return NULL;
  }

  int width(prettyPrintStream* output) {
    int len = statements->length();
    if (len == 0) return 0;
    if (len == 1) return statements->at(0)->width(output);
    return output->infinity();
  }
};


class inlinedBlockNode : public codeNode {
 public:
  inlinedBlockNode(int bci, scopeNode* scope) : codeNode(bci, scope) {}

  void add(astNode* statement) {
    statements->push(statement);
  }

  bool print(prettyPrintStream* output) {
    bool split = output->remaining() < width(output);
    output->print("[");
    if (split) output->inc_newline();
    codeNode::print(output);
    if (split) output->dec_indent();
    output->print("]"); 
    return split;
  }

  int width(prettyPrintStream* output) {
    return output->width_of_string("[") 
	   + codeNode::width(output)
	   + output->width_of_string("]");
  }
};


class statement : public astNode {
 private:
  bool has_return;
  astNode* stat;
  char* hat() { return "^"; }
 public:

  bool is_statement() { return true; }

  statement(int bci, scopeNode* scope, astNode* stat, bool has_return) : astNode(bci, scope) {
    this->stat       = stat;
    this->has_return = has_return;
  }

  void set_return() { has_return = true; }
  bool has_hat() { return has_return; }

  bool print(prettyPrintStream* output) {
    HIGHLIGHT
    if (has_return) output->print(hat());
    return stat->print(output);
  }
  int width(prettyPrintStream* output) {
    return   (has_return ? output->width_of_string(hat()) : 0)
	   + stat->width(output);
  }
};


bool codeNode::print(prettyPrintStream* output) {
  bool first = true;
  for (int index = 0; index < statements->length(); index++) {
    astNode* s = statements->at(index);
    if (!first) {
      output->print(".");
      output->newline();
    }
    s->print(output);
    if (s->is_statement() && ((statement*)s)->has_hat()) break;
    first = false;
  }
  return true;
}


class methodNode : public codeNode {
 public:
  methodNode(int bci, scopeNode* scope) : codeNode(bci, scope) {}

  bool print(prettyPrintStream* output) {
    output->inc_indent();
    output->indent();

    astNode* l = scope->temps();
    if (l) {
       l->print(output);
       output->newline();
    }
    codeNode::print(output);
    output->dec_newline();
    return true;
  }
};


class blockNode : public codeNode {
 private:
  int numOfArgs;
 public:
  blockNode(int bci, scopeNode* scope, int numOfArgs)
  : codeNode(bci, scope) {
    this->numOfArgs = numOfArgs;
  }

  bool print(prettyPrintStream* output) {
    HIGHLIGHT
    bool split = false;
    output->print("[");
    astNode* p = scope->params();
    if (p) { 
      if (split = output->remaining() < p->width(output) + output->width_of_space()) {
        output->inc_newline();
        p->print(output);
        output->newline();
      }
      if (!split) {
        output->space();
        p->print(output);
      }
    }
   
    astNode* l = scope->temps();
    if (l) {
      if (!split) {
        if(output->remaining() >= l->width(output) + output->width_of_space()) {
	   output->space();
           l->print(output);
	} else {
          output->inc_newline();
	  split = true;
	}
      }
      if (split) {
        l->print(output);
        output->newline();
      }
    }

    if (!split) {
      if(output->remaining() >= codeNode::width(output)) {
        if (l || p) output->space();
        codeNode::print(output);
      } else {
         output->inc_newline();
	 split = true;
      }
    }
    if (split) {
      codeNode::print(output);
    }
    output->print("]");
    if (split) output->dec_indent();
    return split;
  }

  int width(prettyPrintStream* output) {
    int w = output->width_of_string("[") + output->width_of_string("]");
    astNode* p = scope->params();
    if (p) w += p->width(output) + 2 * output->width_of_space();
    astNode* t = scope->temps();
    if (t) w += t->width(output) + output->width_of_space();
    w += codeNode::width(output);
    return w;
  }

};


class assignment : public astNode {
 private:
   astNode* variable;
   astNode* e;
   char* sym() { return ":="; }
 public:
   assignment(int bci, scopeNode* scope, astNode* variable, astNode* e) : astNode(bci, scope) {
     this->variable = variable;
     this->e        = e;
   }
   bool print(prettyPrintStream* output) {
     HIGHLIGHT
     bool split = output->remaining() < width(output);
     variable->print(output);
     output->space(); output->print(sym()); 
     astNode::print(output);
     if (split) output->inc_newline();
     else       output->space();
     e->print(output);
     if (split) output->dec_indent();
     return split;
   }
   int width(prettyPrintStream* output) {
      return   variable->width(output) 
             + 2 * output->width_of_space()
	     + output->width_of_string(sym());
   }
};


class messageNode : public astNode {
 private:
   astNode*                 _receiver;
   symbolOop                selector;
   GrowableArray<astNode*>* arguments;
   bool                     _is_prim;
 public:
   messageNode(int bci, scopeNode* scope, symbolOop selector, bool is_prim = false) : astNode(bci, scope) {
     this->selector = selector;
     arguments = new GrowableArray<astNode*>(10);
     _receiver = NULL;
     _is_prim  = is_prim;
   }

   bool is_unary()   { return selector->is_unary();   }
   bool is_binary()  { return selector->is_binary();  }
   bool is_keyword() { return selector->is_keyword(); }

   bool should_wrap_receiver() {
      if (receiver() && receiver()->is_message()) {
	if (((messageNode*)_receiver)->is_keyword())               return true;
	if (((messageNode*)_receiver)->is_binary() && is_binary()) return true;
      }
      return false; 
   }

   bool should_wrap_argument(astNode* arg) {
     if (arg->is_message()) {
	if (((messageNode*)arg)->is_keyword())               return true;
	if (((messageNode*)arg)->is_binary() && is_binary()) return true;
     }
     return false;
   }

   bool print_receiver(prettyPrintStream* output) {
     bool wrap = should_wrap_receiver();
     if (wrap) output->print("(");
     _receiver->print(output);
     if (wrap) output->print(")");
     output->print(" ");
     return false;
   }

   bool real_print(prettyPrintStream* output) {
     HIGHLIGHT
     bool split;
     if (_is_prim) output->print("{{");
     if (valid_receiver()) print_receiver(output);
     if (output->remaining() < width_send(output)) {
       output->inc_newline();
       astNode::print(output);
       split = print_selector_with_arguments(output, selector, arguments, true);
       if (_is_prim) output->print("}}");
       output->dec_indent();
       return split;
     }
     split = print_selector_with_arguments(output, selector, arguments, false);
     if (_is_prim) output->print("}}");
     return split;
   }

   bool print(prettyPrintStream* output) {
     if(receiver() && receiver()->is_cascade()) return receiver()->print(output);
     return real_print(output);
   }

   int width_receiver(prettyPrintStream* output) {
     int w  = receiver() ? receiver()->width(output) + output->width_of_space() : 0;
     if (should_wrap_receiver()) {
       w += output->width_of_string("(") + output->width_of_string(")");
     }
     return w;
   }

   int width_send(prettyPrintStream* output) {
     int arg = selector->number_of_arguments();
     int w = output->width_of_string(selector->as_string())
           + arg * output->width_of_space();
     for (int index = 0; index < arguments->length(); index++)
       w += arguments->at(index)->width(output);
     return w;
   }

   int real_width(prettyPrintStream* output) {
     return (receiver() ? width_receiver(output) + output->width_of_space() : 0)
            + width_send(output);
   }

   int width(prettyPrintStream* output)  {
     if(receiver() && receiver()->is_cascade()) return receiver()->width(output);
     return real_width(output);
   }

   void add_param(astNode* p)    { arguments->push(p); }
   void set_receiver(astNode* p);

   astNode* receiver() { return _receiver; }

   bool valid_receiver() {
     return receiver() ? !receiver()->is_cascade() : false;
   }

   bool is_message() { return true; }
};


class selfNode : public leafNode {
 public:
   selfNode(int bci, scopeNode* scope) : leafNode(bci, scope) {}
   char* string() { return "self"; }
};


class ignoreReceiver : public leafNode {
 public:
   ignoreReceiver(int bci, scopeNode* scope) : leafNode(bci, scope) {}
   char* string() { return ""; }
};


class superNode : public leafNode {
 public:
   superNode(int bci, scopeNode* scope) : leafNode(bci, scope) {}
   char* string() { return "super"; }
};


class literalNode : public leafNode {
 private:
   char* str;
 public:
   literalNode(int bci, scopeNode* scope, char* str) : leafNode(bci, scope) {
     this->str = str;
   }
   char* string() { return str; }
};


class symbolNode : public astNode {
 private:
  symbolOop value;
  bool      is_outer;
  char*     str;
 public:
  symbolNode(int bci, scopeNode* scope, symbolOop value, bool is_outer = true)
  : astNode(bci, scope) {
    this->value    = value;
    this->is_outer = is_outer;
    this->str      = value->as_string();
  }
  bool print(prettyPrintStream* output) {
    astNode::print(output);
    if (is_outer) output->print("#");
    output->print(str);
    return false;
  }
  int  width(prettyPrintStream* output) {
    return (is_outer ? output->width_of_string("#") : 0)
           + output->width_of_string(str);
  }
};


class doubleByteArrayNode : public astNode {
 private:
  doubleByteArrayOop value;
  char*              str;
 public:
  doubleByteArrayNode(int bci, scopeNode* scope, doubleByteArrayOop value)
  : astNode(bci, scope) {
    this->value    = value;
    this->str      = value->as_string();
  }
  bool print(prettyPrintStream* output) {
    astNode::print(output);
    output->print("'");
    output->print(str);
    output->print("'");
    return false;
  }
  int  width(prettyPrintStream* output) {
    return 2 * output->width_of_string("'")
           + output->width_of_string(str);
  }
};


class byteArrayNode : public astNode {
 private:
  byteArrayOop value;
  char*        str;
 public:
  byteArrayNode(int bci, scopeNode* scope, byteArrayOop value)
  : astNode(bci, scope) {
    this->value    = value;
    this->str      = value->as_string();
  }
  bool print(prettyPrintStream* output) {
    HIGHLIGHT
    astNode::print(output);
    output->print("'");
    output->print(str);
    output->print("'");
    return false;
  }
  int  width(prettyPrintStream* output) {
    return 2 * output->width_of_string("'")
           + output->width_of_string(str);
  }
};


class smiNode : public leafNode {
 private:
  int   value;
  char* str;
 public:
  smiNode(int bci, scopeNode* scope, int value)
  : leafNode(bci, scope) {
    this->value    = value;
    this->str      = NEW_RESOURCE_ARRAY(char, 10);
    sprintf(this->str, "%d", value);
  }
  char* string() { return str; }
};

class doubleNode : public leafNode {
 private:
  double   value;
  char*    str;
 public:
  doubleNode(int bci, scopeNode* scope, double value)
  : leafNode(bci, scope) {
    this->value = value;
    this->str   = NEW_RESOURCE_ARRAY(char, 30);
    sprintf(this->str, "%1.10gd", value);
  }
  char* string() { return str; }
};


class characterNode : public leafNode {
 private:
  oop    value;
  char*  str;
 public:
  characterNode(int bci, scopeNode* scope, oop value)
  : leafNode(bci, scope) {
    this->value = value;
    this->str   = NEW_RESOURCE_ARRAY(char, 3);
    if (value->is_mem()) {
      oop ch = memOop(value)->instVarAt(2);
      if(ch->is_smi()) {
        sprintf(this->str, "$%c", smiOop(ch)->value());
	return;
      }
    }
    this->str = "$%c";
  }
  char* string() { return str; } 
};


class objArrayNode : public astNode {
 private:
  objArrayOop              value;
  bool                     is_outer;
  GrowableArray<astNode*>* elements;
 public:
  objArrayNode(int bci, scopeNode* scope, objArrayOop value, bool is_outer = true)
  : astNode(bci, scope) {
    this->value    = value;
    this->is_outer = is_outer;
    this->elements = new GrowableArray<astNode*>(10);
    for (int index = 1; index <= value->length(); index++)
      elements->push(get_literal_node(value->obj_at(index), bci, scope));
  }
  bool print(prettyPrintStream* output) {
    HIGHLIGHT
    astNode::print(output);
    output->print("#(");
    for (int index = 0; index < elements->length(); index++) {
      elements->at(index)->print(output);
      if (index < elements->length() -1) output->space();
    }
    output->print(")");
    return false;
  }
  int  width(prettyPrintStream* output) {
    int w = output->width_of_string("#(") + output->width_of_string(")");
    for (int index = 0; index < elements->length(); index++) {
      w += elements->at(index)->width(output);
      if (index < elements->length() -1) 
        w += output->width_of_space();
    }
    return 0;
  }
};


class dllNode : public astNode {
 private:
   astNode*                 dll_name;
   astNode*                 func_name;
   GrowableArray<astNode*>* arguments;
   astNode*                 proxy;
 public:
  dllNode(int bci, scopeNode* scope, symbolOop dll_name, symbolOop func_name)
  : astNode(bci, scope) {
     this->dll_name   = new symbolNode(bci,scope,dll_name,false);
     this->func_name  = new symbolNode(bci,scope,func_name,false);
     arguments        = new GrowableArray<astNode*>(10);
     proxy            = NULL;
   }

   void add_param(astNode* param) { arguments->push(param); }

   void set_proxy(astNode* p) { proxy = p; }

   bool print(prettyPrintStream* output) {
     HIGHLIGHT
     astNode::print(output);
     output->print("{{<");
     dll_name->print(output);
     output->space();
     proxy->print(output);
     output->space();
     func_name->print(output);
     output->print(">");
     for (int index = arguments->length() - 1; index >= 0 ; index--) {
       arguments->at(index)->print(output);
       if (index < arguments->length() -1) output->print(", ");
     }
     output->print("}}");
     return false;
   }

   int width(prettyPrintStream* output) {
     return output->width_of_string("<")
          + output->width_of_string("Printing dll call")
          + output->width_of_string(">");
   }
};


class stackTempNode : public leafNode {
 private:
   int offset;
   char* str;
 public:
   stackTempNode(int bci, scopeNode* scope, int offset) : leafNode(bci, scope) {
     this->offset        = offset;
     this->str           = scope->stack_temp_string(this_bci(), offset);
   }
   char* string() { return str; }
};


class heapTempNode : public leafNode {
 private:
   int offset;
   int context_level;
   char* str;
 public:
   heapTempNode(int bci, scopeNode* scope, int offset, int context_level) : leafNode(bci, scope) {
     this->offset        = offset;
     this->context_level = context_level;
     this->str           = scope->heap_temp_string(this_bci(), offset, context_level);
   }
   char* string() { return str; }
};

class floatNode : public leafNode {
 private:
  char* str;
 public:
  floatNode(int no, int bci, scopeNode* scope)
  : leafNode(bci, scope) {
    this->str = scope->stack_temp_string(this_bci(), no);
  }
  char* string() { return str; }
};


class instVarNode : public leafNode {
 private:
   oop   obj;
   char* str;
 public:
   instVarNode(int bci, scopeNode* scope, oop obj) : leafNode(bci, scope) {
     this->obj = obj;
     if (obj->is_smi()) {
       str = scope->inst_var_string(smiOop(obj)->value());
     } else {
       str = symbolOop(obj)->as_string();
     }
   }
   char* string() { return str; }
};


class classVarNode : public leafNode {
 private:
   oop   obj;
   char* str;
 public:
   classVarNode(int bci, scopeNode* scope, oop obj) : leafNode(bci, scope) {
     this->obj = obj;
     if (obj->is_association()) {
       str = associationOop(obj)->key()->as_string();
     } else {
       str = symbolOop(obj)->as_string();
     }
   }
   char* string() { return str; }
};


class primitiveResultNode : public leafNode {
 public:
   bool is_primitive_result()  { return true; }
   primitiveResultNode(int bci, scopeNode* scope) 
   : leafNode(bci, scope) { }
   char* string() { return "`primitive result`"; }
};


class assocNode : public leafNode {
 private:
   associationOop assoc;
   char* str;
 public:
   assocNode(int bci, scopeNode* scope, associationOop assoc)
   : leafNode(bci, scope) {
     this->assoc = assoc;
     this->str   = assoc->key()->as_string();
   }
   char* string() { return str; }
};


class cascadeSendNode : public astNode {
 private:
   GrowableArray<messageNode*>* messages;
   astNode* receiver;
 public:
   cascadeSendNode(int bci, scopeNode* scope, messageNode* first) : astNode(bci, scope) {
     messages = new GrowableArray<messageNode*>(10);
     receiver = first->receiver();
     first->set_receiver(this);
   }

   bool is_cascade() { return true; }

   void add_message(messageNode* m) {
     messages->push(m);
   }

   bool print(prettyPrintStream* output) {
     receiver->print(output);
     output->inc_newline();
     for (int i = 0; i < messages->length(); i++) {
       if (i == 0) output->print("  ");
       else        output->print("; ");
       messages->at(i)->real_print(output);
       if (i != messages->length() - 1) output->newline();
     }
     output->dec_indent();
     return true;
   }
   int width(prettyPrintStream* output) {
    return 0;
  }
};


void messageNode::set_receiver(astNode* p) { 
  if (p->is_cascade()) {
    ((cascadeSendNode*)p)->add_message(this);
  }
  _receiver = p;
}


static astNode* get_literal_node(oop obj, int bci, scopeNode* scope) {
  if (obj == trueObj)            return new literalNode(bci, scope, "true");
  if (obj == falseObj)           return new literalNode(bci, scope, "false");
  if (obj == nilObj)             return new literalNode(bci, scope, "nil");
  if (obj->is_doubleByteArray()) return new doubleByteArrayNode(bci, scope, doubleByteArrayOop(obj));
  if (obj->is_symbol())          return new symbolNode(bci, scope, symbolOop(obj));
  if (obj->is_byteArray())       return new byteArrayNode(bci, scope, byteArrayOop(obj));
  if (obj->is_smi())             return new smiNode(bci, scope, smiOop(obj)->value());
  if (obj->is_double())          return new doubleNode(bci, scope, doubleOop(obj)->value());
  if (obj->is_objArray())        return new objArrayNode(bci, scope, objArrayOop(obj));
  return new characterNode(bci, scope, obj);
}

void prettyPrintStream::print() { std->print("pretty-printer stream"); }

void defaultPrettyPrintStream::indent() {
  for (int i = 0; i < indentation; i++) {
	space(); space();
  }
}
void defaultPrettyPrintStream::print(char* str)  {
  for (int i = 0; str[i]; i++)
    print_char(str[i]);
}
void defaultPrettyPrintStream::print_char(char c)  {
  std->print("%c", c);
  pos += width_of_char(c);
}
int defaultPrettyPrintStream::width_of_string(char* str) {
  int w = 0;
  for (int i = 0; str[i]; i++)
    w += width_of_char(str[i]);
  return w;
}
void defaultPrettyPrintStream::space()  { print_char(' '); }
void defaultPrettyPrintStream::newline() { 
  print_char('\n');
  pos = 0;
  indent();
}
void defaultPrettyPrintStream::begin_highlight() { set_highlight(true);  print("{"); }
void defaultPrettyPrintStream::end_highlight()   { set_highlight(false); print("}"); }

void byteArrayPrettyPrintStream::newline() { 
  print_char('\r');
  pos = 0;
  indent();
}

// Forward declaration
astNode* generateForBlock(methodOop method, klassOop klass, int bci, int numOfArgs);
astNode* generate(scopeNode* scope);

class MethodPrettyPrinter : public MethodClosure {
 private:
  GrowableArray<astNode*>* _stack; // expression stack
  scopeNode*               _scope; // debugging scope

  // the receiver is on the stack
  void normal_send(symbolOop selector, bool is_prim = false);
  // the receiver is provided 
  void special_send(astNode* receiver, symbolOop selector, bool is_prim = false);

  astNode* generateInlineBlock(MethodInterval* interval, bool produces_result, astNode* push_elem = NULL);
  void store(astNode* node) { _push(new assignment(bci(), scope(), node, _pop())); }

 public:
  void       _push(astNode* node) { _stack->push(node);      }
  astNode*   _pop()               { return _stack->pop();    }
  astNode*   _top()               { return _stack->top();    }
  int        _size()       const  { return _stack->length(); }
  scopeNode* scope()       const  { return _scope;           }

  // Constructor
  MethodPrettyPrinter(scopeNode* scope);

  // node call backs
  void if_node(IfNode* node);
  void cond_node(CondNode* node);
  void while_node(WhileNode* node);
  void primitive_call_node(PrimitiveCallNode* node);
  void dll_call_node(DLLCallNode* node);

  // complicated call backs
  void pop();
  void method_return(int nofArgs);
  void nonlocal_return(int nofArgs);

  void allocate_closure(AllocationType type, int nofArgs, methodOop meth) {
    if (type == tos_as_scope) _pop();
    scopeNode* methodScope = _scope->scopeFor(meth);
    if (methodScope)
      _push(generate(methodScope));
    else
      _push(generateForBlock(meth, scope()->get_klass(), -1, nofArgs));
  }
  void allocate_context(int nofTemps, bool forMethod) { 
    scope()->context_allocated();
  }

  // sends
  void normal_send(InterpretedIC* ic)		{ normal_send(ic->selector());					}
  void self_send(InterpretedIC* ic)   		{ special_send(new selfNode(bci(), scope()), ic->selector());	}
  void super_send(InterpretedIC* ic)  		{ special_send(new superNode(bci(), scope()), ic->selector());	}
  void double_equal()				{ normal_send(vmSymbols::double_equal());			}
  void double_not_equal()			{ normal_send(vmSymbols::double_tilde());			}

  // simple call backs
  void push_self()				{ _push(new selfNode(bci(), scope()));				}
  void push_tos()				{ _push(_top());                                        	}
  void push_literal(oop obj)			{ _push(get_literal_node(obj, bci(), scope()));         	}
  void push_argument(int no)			{ _push(new paramNode(bci(), scope(), no));             	}
  void push_temporary(int no)			{ _push(new stackTempNode(bci(), scope(), no));         	}
  void push_temporary(int no, int context)	{ _push(new heapTempNode(bci(), scope(), no, context)); 	}
  void push_instVar(int offset)			{ _push(new instVarNode(bci(), scope(), as_smiOop(offset)));  	}
  void push_instVar_name(symbolOop name)	{ _push(new instVarNode(bci(), scope(), name));       		}
  void push_classVar(associationOop assoc)	{ _push(new classVarNode(bci(), scope(), assoc));       	}
  void push_classVar_name(symbolOop name)	{ _push(new classVarNode(bci(), scope(), name));       		}
  void push_global(associationOop obj)		{ _push(new assocNode(bci(), scope(), obj));            	}
  void store_temporary(int no)			{ store(new stackTempNode(bci(), scope(), no));         	}
  void store_temporary(int no, int context)	{ store(new heapTempNode(bci(), scope(), no, context)); 	}
  void store_instVar(int offset)		{ store(new instVarNode(bci(), scope(), as_smiOop(offset)));	}
  void store_instVar_name(symbolOop name)	{ store(new instVarNode(bci(), scope(), name));       		}
  void store_classVar(associationOop assoc)	{ store(new classVarNode(bci(), scope(), assoc));       	}
  void store_classVar_name(symbolOop name)	{ store(new classVarNode(bci(), scope(), name));       		}
  void store_global(associationOop obj)		{ store(new assocNode(bci(), scope(), obj));            	}

  // call backs to ignore
  void allocate_temporaries(int nofTemps)				{}
  void set_self_via_context()						{}
  void copy_self_into_context()						{}
  void copy_argument_into_context(int argNo, int no)			{}
  void zap_scope()							{}
  void predict_prim_call(primitive_desc* pdesc, int failure_start)	{}

  void float_allocate(int nofFloatTemps, int nofFloatExprs)		{}
  void float_floatify(Floats::Function f, int tof)			{ 
    normal_send(Floats::selector_for(f));
    pop();
  }
  void float_move(int tof, int from) {
    _push(new floatNode(from, bci(), scope()));
    store(new floatNode(tof, bci(), scope()));
    pop();
  } 
  void float_set(int tof, doubleOop value)				{}
  void float_nullary(Floats::Function f, int tof)			{}
  void float_unary(Floats::Function f, int tof) {
    _push(new floatNode(tof, bci(), scope()));
    normal_send(Floats::selector_for(f));
    pop();
  }
  void float_binary(Floats::Function f, int tof) { 
    _push(new floatNode(tof-1, bci(), scope()));
    _push(new floatNode(tof,   bci(), scope()));
    normal_send(Floats::selector_for(f));
    pop();
  }

  void float_unaryToOop(Floats::Function f, int tof) {
    _push(new floatNode(tof, bci(), scope()));
    normal_send(Floats::selector_for(f));
  }

  void float_binaryToOop(Floats::Function f, int tof) {
    _push(new floatNode(tof,   bci(), scope()));
    _push(new floatNode(tof-1, bci(), scope()));
    normal_send(Floats::selector_for(f));
  }
};


MethodPrettyPrinter::MethodPrettyPrinter(scopeNode* scope) {
  _stack = new GrowableArray<astNode*>(10);
  _scope = scope;
  if (scope->method()->is_blockMethod())
    _push(new blockNode(1, scope, scope->method()->number_of_arguments()));
  else
    _push(new methodNode(1, scope));
}


void MethodPrettyPrinter::normal_send(symbolOop selector, bool is_prim) {
  int nargs = selector->number_of_arguments();
  messageNode* msg = new messageNode(bci(), scope(), selector, is_prim);

  GrowableArray<astNode*>* arguments = new GrowableArray<astNode*>(10);
  for (int index = 0; index < nargs; index++)
    arguments->push(_pop());
    
  for (index = 0; index < nargs; index++)
    msg->add_param(arguments->at(index));

  msg->set_receiver(_pop());

  _push(msg);
}


void MethodPrettyPrinter::special_send(astNode* receiver, symbolOop selector, bool is_prim) {
  int nargs = selector->number_of_arguments();
  messageNode* msg = new messageNode(bci(), scope(), selector, is_prim);

  GrowableArray<astNode*>* arguments = new GrowableArray<astNode*>(10);
  for (int index = 0; index < nargs; index++)
    arguments->push(_pop());
    
  for (index = 0; index < nargs; index++)
    msg->add_param(arguments->at(index));

  msg->set_receiver(receiver);

  _push(msg);
}


void MethodPrettyPrinter::pop() {
  // This has to be on a statement boundary.
  astNode* expr = _pop();
  astNode* code = _top();

  if (code->is_code()) {
    code->add(new statement(bci(), scope(), expr, false));
    return;
  } 
  
  if (expr->is_message()) {
    // This must be a cascade send
    //    - exp
    //    - receiver for the cascade send
      
    // Replace the receiver with a cascade code
    if (!_top()->is_cascade()) {
      _pop();
      _push(new cascadeSendNode(bci(), scope(), (messageNode*) expr));
    }
  } else {
    _pop();
    _push(expr);
  }
}


void MethodPrettyPrinter::method_return(int nofArgs)  {
  // This has to be on a statement boundary.
  if (_size() == 1) {
    // Make the last statement in the method a nlr.
    astNode* code = _top();
    ((codeNode*) code)->last_statement()->set_return();
  } else {
    astNode* expr = _pop();
    astNode* code = _top();
    if (code->is_code()) {
      code->add(new statement(bci(), scope(), expr, !scope()->is_block_method()));
    } else {
      fatal1("Stack size = %d", _size());
    }
  }
}


void MethodPrettyPrinter::nonlocal_return(int nofArgs)  {
  // This has to be on a statement boundary.
  astNode* expr = _pop();
  astNode* code = _top();
  if (code->is_code()) {
    ((codeNode*) code)->add(new statement(bci(), scope(), expr, true));
  } else {
    fatal1("Stack size = %d", _size());
  }
}


class StackChecker {
  public:
   MethodPrettyPrinter* pp;
   int size; 
   char* name;
   int offset;

  StackChecker(char* name, MethodPrettyPrinter* pp, int offset = 0) {
    this->pp     = pp;
    this->size   = pp->_size();
    this->name   = name;
    this->offset = offset;
  }

  ~StackChecker() {
    if (pp->_size() != size + offset) {
      std->print_cr("StackTracer found misaligned stack");
      std->print_cr("Expecting %d but found %d", size + offset, pp->_size());
      fatal("aborting");
    }
  }
};


astNode* MethodPrettyPrinter::generateInlineBlock(MethodInterval* interval, bool produces_result, astNode* push_elem) {
  inlinedBlockNode* block = new inlinedBlockNode(bci(), scope());
  _push(block);
  if (push_elem) _push(push_elem);
  MethodIterator mi(interval, this);
  if (produces_result) pop(); // discard the result value
  return _pop();
}


void MethodPrettyPrinter::if_node(IfNode* node) {
  _push(generateInlineBlock(node->then_code(), node->produces_result()));
  if (node->else_code()) {
    _push(generateInlineBlock(node->else_code(), node->produces_result()));
  }
  normal_send(node->selector());
  if (!node->produces_result()) pop();
}


void MethodPrettyPrinter::cond_node(CondNode* node) {
  _push(generateInlineBlock(node->expr_code(), true));
  normal_send(node->selector());
}


void MethodPrettyPrinter::while_node(WhileNode* node) {
  _push(generateInlineBlock(node->expr_code(), true));
  if (node->body_code())
    _push(generateInlineBlock(node->body_code(), false));
  normal_send(node->selector());
  pop();
}


void MethodPrettyPrinter::primitive_call_node(PrimitiveCallNode* node) {
  if (node->failure_code())
    _push(generateInlineBlock(node->failure_code(), true, new primitiveResultNode(bci(), scope())));
  if (node->has_receiver())
    normal_send(node->name(), true);
  else
    special_send(new ignoreReceiver(bci(), scope()), node->name(), true);
}


void MethodPrettyPrinter::dll_call_node(DLLCallNode* node) {
  dllNode* msg = new dllNode(bci(), scope(), node->dll_name(), node->function_name());

  int nargs = node->nofArgs();

  GrowableArray<astNode*>* arguments = new GrowableArray<astNode*>(10);
  for (int index = 0; index < nargs; index++)
    arguments->push(_pop());
    
  for (index = 0; index < nargs; index++)
    msg->add_param(arguments->at(index));

  msg->set_proxy(_pop());

  _push(msg);
}


astNode* generate(scopeNode* scope) {
  MethodPrettyPrinter blk(scope);
  MethodIterator mi(scope->method(), &blk);
  assert(blk._size() == 1, "just checking");
  return blk._top();
}


astNode* generateForActivation(deltaVFrame* fr, int index) {
  scopeNode* scope = new scopeNode(fr, index);
  scopeNode* toGenerate = scope->homeScope();
  return generate(toGenerate);
}


astNode* generateForMethod(methodOop method, klassOop klass, int bci) {
  return generate(new scopeNode(method, klass, bci));
}


astNode* generateForBlock(methodOop  method, klassOop klass, int bci, int numOfArgs) {
  return generate(new scopeNode(method, klass, bci));
}


void prettyPrinter::print(methodOop method, klassOop klass, int bci, prettyPrintStream* output) {
  ResourceMark rm;
  if (!output) output = new defaultPrettyPrintStream;
  astNode* root = generateForMethod(method, klass, bci);
  root->top_level_print(output);
  output->newline();
}

void prettyPrinter::print(int index, deltaVFrame* fr, prettyPrintStream* output) {
  ResourceMark rm;
  if (!output) output = new defaultPrettyPrintStream;

  if (ActivationShowCode) {
    astNode* root = generateForActivation(fr, index);
    root->top_level_print(output);
  } else {
    scopeNode* scope = new scopeNode(fr, index);
    scope->print_header(output);
  }
  output->newline();
}

void prettyPrinter::print_body(deltaVFrame* fr, prettyPrintStream* output) {
  ResourceMark rm;
  if (!output) output = new defaultPrettyPrintStream;

  astNode* root = generateForActivation(fr, 0);
  root->print(output);
}

byteArrayPrettyPrintStream::byteArrayPrettyPrintStream() : defaultPrettyPrintStream() {
  _buffer = new GrowableArray<int>(200);
}

void byteArrayPrettyPrintStream::print_char(char c)  {
  _buffer->append((int)c);
  pos += width_of_char(c);
}

byteArrayOop byteArrayPrettyPrintStream::asByteArray() {
  int l = _buffer->length();
  byteArrayOop a = oopFactory::new_byteArray(l);
  for (int i = 0; i < l; i++) {
    a->byte_at_put(i+1, (char)_buffer->at(i));
  }
  return a;
}

byteArrayOop prettyPrinter::print_in_byteArray(methodOop method, klassOop klass, int bci) {  
  ResourceMark rm;
  byteArrayPrettyPrintStream* stream = new byteArrayPrettyPrintStream;
  prettyPrinter::print(method, klass, bci, stream);
  return stream->asByteArray();
}


bool should_wrap(int type, astNode* arg) {
  if (!arg->is_message()) return false;
  messageNode* msg = (messageNode*) arg;
  switch (type) {
   case 0: return false;
   case 1: return !msg->is_unary();
   case 2: return msg->is_keyword();
  }
  return false;
}


void PrintParams::parameter(byteArrayOop name, int index) {
  elements->push(scope->parameter_at(index, true));
}


void PrintTemps::stack_temp(byteArrayOop name, int offset) {
  elements->push(scope->stack_temp_at(offset)); 
}


void PrintTemps::heap_temp(byteArrayOop name, int offset)  {
  elements->push(scope->heap_temp_at(offset));
}
