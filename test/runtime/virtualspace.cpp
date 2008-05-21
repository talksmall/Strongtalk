# include "incls/_precompiled.incl"
# include "incls/_virtualspace.cpp.incl"
#include "easyunit/test.h"

using namespace easyunit;

DECLARE(ReservedSpace)
void checkAlignment(int size, int expected_alignment) {
  char message[100];
  int adjustment = ReservedSpace::page_align_size(size);
  sprintf(message, "adjustment for %d: %d", size, adjustment);
  ASSERT_EQUALS_M(expected_alignment, adjustment, message);
}
END_DECLARE

SETUP(ReservedSpace) {}

TEARDOWN(ReservedSpace){}

TESTF(ReservedSpace, page_align_size)
{
  // at least one page
  checkAlignment(0, os::vm_page_size());
  checkAlignment(1, os::vm_page_size());
  checkAlignment(os::vm_page_size() - 1, os::vm_page_size());
  checkAlignment(os::vm_page_size(), os::vm_page_size());

  // in chunks of one page in size
  checkAlignment(os::vm_page_size() + 1, 2 * os::vm_page_size());
  checkAlignment(2 * os::vm_page_size() - 1, 2 * os::vm_page_size());
  checkAlignment(2 * os::vm_page_size(), 2 * os::vm_page_size());
  checkAlignment(2 * os::vm_page_size() + 1, 3 * os::vm_page_size());
}
