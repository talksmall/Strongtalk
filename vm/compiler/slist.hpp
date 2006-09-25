/* Copyright 1994-96, LongView Technologies L.L.C. $Revision: 1.7 $ */
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

// Yet another version of lists, this one allowing efficient insert/delete in the middle.

# ifdef DELTA_COMPILER
  
class GenericSListElem : public ResourceObj  {
 protected:
  void* _data;
  GenericSListElem*  _next;
 public: 
  GenericSListElem(void* d, GenericSListElem* n = NULL) { _data = d; _next = n; }
  
  void* dataL() const 	      		{ return this ? _data : NULL; }
  void setDataL(void* d) 		{ _data = d; }
  
  GenericSListElem* nextL() const 	{ return _next; }
  void setNextL(GenericSListElem* n) 	{ _next = n; }

 protected:
  void insertAfterL(void* d)		{ _next = new GenericSListElem(d, _next); }
  
  friend class GenericSList;
};

typedef bool (*slistFindFn)(void* token, void* elem);

class GenericSList : public PrintableResourceObj {
 protected:
  GenericSListElem* _head;
  GenericSListElem* _tail;
  int _len;
 public:
  GenericSList() { _head = _tail = NULL; _len = 0; }
  
  void prependL(void* d) {
    _head = new GenericSListElem(d, _head);
    if (_tail == NULL) _tail = _head;
    _len++;
  }
  void appendL(void* d) {
    GenericSListElem* e = new GenericSListElem(d);
    if (_tail == NULL) {
      _head = _tail = e;
    } else {
      _tail->_next = e;
      _tail = e;
    }
    _len++;
  }
  
  GenericSListElem* headL() const 		{ return _head; }
  GenericSListElem* tailL() const 		{ return _tail; }
  
  void applyL(void f(void*));
  
  bool isEmpty() const 				{ return _head == NULL; }
  bool nonEmpty() const 			{ return ! isEmpty(); }
  int length() const				{ return _len; }
  bool includesL(void* p) const 		{ return findL(p) != NULL; }
  GenericSListElem* findL(void* p) const;
  GenericSListElem* findL(void* p, slistFindFn f) const;
  
  void* nthL(int i) const;
  
  void* firstL() const 				{ return headL()->dataL(); }
  void* secondL() const 			{ return headL()->nextL()->dataL(); }
  void* lastL() const 				{ return tailL()->dataL(); }
  
  void insertAfterL(GenericSListElem* e, void* d);
  void removeL(void* d);
  void removeAfterL(GenericSListElem* e);
  
 protected:
  void* removeHeadL() {
    assert(nonEmpty(), "removing from an empty list");
    void* d = firstL();
    _head = headL()->nextL();
    if (--_len == 0) _tail = _head;
    return d; }
  
 public:
  void pushL(void* d) 				{ prependL(d); }
  void* popL() 					{ return removeHeadL(); }
  
  void print();
  void print_short();
};


template <class T> class SListElem : public GenericSListElem {
 public:
  SListElem(T d, SListElem<T>* n = NULL) : GenericSListElem((void*) d, (GenericSListElem*) n) {}			      	      \

  T data() const		{ return (T)GenericSListElem::dataL(); }
  void setData(T d)		{ GenericSListElem::setDataL((void*) d); }
  SListElem<T>* next() const	{ return (SListElem<T>*) GenericSListElem::nextL(); }
  void setNext(SListElem<T>* n) { GenericSListElem::setNextL(n); }
  void insertAfter(T d)		{ GenericSListElem::insertAfterL((void*) d); }
};

template <class T> class SList : public GenericSList {
 public:
  SList() {}

  SListElem<T>* head() const	{ return (SListElem<T>*) GenericSList::headL(); }
  SListElem<T>* tail() const	{ return (SListElem<T>*) GenericSList::tailL(); }
  bool includes(T e) const	{ return GenericSList::includesL((void*)e); } 
  SListElem<T>* find(T e) const { return (SListElem<T>*)GenericSList::findL((void*)e); }
  SListElem<T>* find(void* token, slistFindFn f) const {
				  return (SListElem<T>*)GenericSList::findL(token, (slistFindFn)f); } 
									    
  T first() const 		{ return (T) GenericSList::firstL(); }
  T second() const 		{ return (T) GenericSList::secondL(); }
  T at(int i) const		{ return (T) GenericSList::nthL(i); }
  T last() const 		{ return (T) GenericSList::lastL(); }
									    
  void prepend(T d) 		{ GenericSList::prependL((void*) d); }
  void append(T d) 		{ GenericSList::appendL((void*) d); }
  void apply(void f(T))		{ GenericSList::applyL((void (*)(void*))f); }
  void insertAfter(SListElem<T>* e, T d) { GenericSList::insertAfterL((SListElem<T>*) e, (void*) d); }
  void removeAfter(SListElem<T>* e, T d) { GenericSList::removeAfterL((SListElem<T>*) e); }
  void remove(T d)		{ GenericSList::removeL((void*) d); }
  T removeHead()		{ return (T) GenericSList::removeHeadL(); }
  void push(T d) 		{ GenericSList::pushL((void*) d); }
  T pop() 			{ return (T) GenericSList::popL(); }
  void apply(Closure<T>* c)	{ 
    SListElem<T>* nexte;	// to permit removing during iteration
    for (SListElem<T>* e = head(); e; e = nexte) { nexte = e->next(); c->do_it(e->data()); }
  }
};

# endif
