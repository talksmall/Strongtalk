# include "incls/_precompiled.incl"
# include "incls/_behavior_prims.cpp.incl"
#include "handle.hpp"
#include "test.h"

using namespace easyunit;

extern "C" int expansion_count;
DECLARE(OldSpaceTests)
END_DECLARE

SETUP(OldSpaceTests) {
}

TEARDOWN(OldSpaceTests){
  MarkSweep::collect();
}

TESTF(OldSpaceTests, expandShouldIncreaseExpansionCount) {
  int expansions = expansion_count;
  oldSpace* theSpace = Universe::old_gen.top_mark()._space;
  theSpace->expand(ObjectHeapExpandSize * K);
  ASSERT_EQUALS(expansions + 1, expansion_count);
}

TESTF(OldSpaceTests, oldSpaceMarkShouldRestoreTop) {
  oldSpace* theSpace = Universe::old_gen.top_mark()._space;
  int freeSpace = theSpace->free();
  oop* oldTop = theSpace->top();
  {
    OldSpaceMark mark(theSpace);
    ASSERT_TRUE(theSpace->allocate(100, false) != NULL);
    ASSERT_EQUALS(freeSpace - (100 * oopSize), theSpace->free());
  }
  ASSERT_EQUALS(freeSpace, theSpace->free());
  ASSERT_EQUALS((char*)oldTop, (char*)theSpace->top());
}

TESTF(OldSpaceTests, expandAndAllocateShouldLeaveFreeSpaceUnchanged) {
  HandleMark mark;
  Handle byteArrayClass(Universe::find_global("ByteArray"));

  oldSpace* theSpace = Universe::old_gen.top_mark()._space;
  int freeSpace = theSpace->free();
  {
    OldSpaceMark mark(theSpace);
    oop* result = theSpace->expand_and_allocate(ObjectHeapExpandSize * K);
    ASSERT_EQUALS(freeSpace, theSpace->free());
  }
}
