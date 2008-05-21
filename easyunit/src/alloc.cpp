#include "alloc.h"

using namespace easyunit;

void* CHeap::operator new(size_t size) {
  return (void *) malloc(size);
}
void CHeap::operator delete(void* p) {
  free(p);
}
