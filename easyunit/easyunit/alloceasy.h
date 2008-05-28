#ifndef _EASYUNIT_ALLOC_
#define _EASYUNIT_ALLOC_
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

namespace easyunit {

  class CHeap {
   public:
    void* operator new(size_t size);
    void  operator delete(void* p);
  };

}
#endif
