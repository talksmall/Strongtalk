#include "incls/_precompiled.incl"
#include "incls/_hCodeBuffer.cpp.incl"
#include "test.h"

using namespace easyunit;

DECLARE(HCodeBufferTests)
  HCodeBuffer *code;
  HeapResourceMark *rm;
  char msg[200];
  void checkByteLength(int expected, char* message) {
    sprintf(msg, "Wrong byte length for %s, expected: %d, but was: %d", message, expected, code->byteLength());
    ASSERT_EQUALS_M(expected, code->byteLength(), msg);
  }
  void checkOopLength(int expected, char* message) {
    sprintf(msg, "Wrong oop length for %s, expected: %d, but was: %d", message, expected, code->oopLength());
    ASSERT_EQUALS_M(expected, code->oopLength(), msg);
  }

#define checkByte(expected, actual)\
    sprintf(msg, "Expected: %d, but was: %d", expected, actual);\
    ASSERT_EQUALS_M(expected, actual, msg)

#define checkOop(expected, actual)\
    sprintf(msg, "Expected: %d, but was: %d", expected, actual);\
    ASSERT_EQUALS_M(int(expected), int(actual), msg)

END_DECLARE

SETUP(HCodeBufferTests) {
  rm = new HeapResourceMark();
  code = new HCodeBuffer();
}

TEARDOWN(HCodeBufferTests){
  delete rm;
  rm = NULL;
  code = NULL;
}

TESTF(HCodeBufferTests, pushingByteShouldAddByteToBytesArray) {
  BlockScavenge bs;
  klassOop messageClass = klassOop(Universe::find_global("Message"));
  symbolOop errorSelector = symbolOop(oopFactory::new_symbol("value"));
  symbolOop selector = symbolOop(oopFactory::new_symbol("receiver:selector:arguments:"));
  symbolOop dnuSelector = symbolOop(oopFactory::new_symbol("doesNotUnderstand:"));
  objArrayOop args = objArrayOop(oopFactory::new_objArray(0));

  code->pushByte(Bytecodes::push_literal);
  code->pushOop(messageClass);
  checkByteLength(8, "Message");
  checkOopLength(2, "Message");
  code->pushByte(Bytecodes::push_self);
  checkByteLength(9, "self");
  code->pushByte(Bytecodes::push_literal);
  code->pushOop(errorSelector);
  checkByteLength(16, "selector");
  checkOopLength(4, "selector");
  code->pushByte(Bytecodes::push_literal);
  code->pushOop(args);
  checkByteLength(24, "args");
  checkOopLength(6, "args");
  code->pushByte(Bytecodes::interpreted_send_n);
  code->pushByte(2);
  code->pushOop(selector);
  code->pushOop(as_smiOop(0));
  checkByteLength(36, "constructor");
  checkOopLength(9, "constructor");
  code->pushByte(Bytecodes::interpreted_send_self);
  code->pushOop(dnuSelector);
  code->pushOop(as_smiOop(0));
  checkByteLength(48, "DNU");
  checkOopLength(12, "DNU");
  code->pushByte(Bytecodes::return_tos_pop_0);
  checkByteLength(49, "return");
}
TESTF(HCodeBufferTests, pushingBytesShouldAddCorrectBytesToByteArray) {
  code->pushByte(1);
  code->pushByte(2);
  code->pushByte(3);
  code->pushByte(4);

  checkByte(1, code->bytes()->byte_at(1));
  checkByte(2, code->bytes()->byte_at(2));
  checkByte(3, code->bytes()->byte_at(3));
  checkByte(4, code->bytes()->byte_at(4));
}
TESTF(HCodeBufferTests, pushingOopShouldPadByteArray) {
  code->pushByte(1);
  code->pushOop(as_smiOop(2));

  checkByte(1,    code->bytes()->byte_at(1));
  checkByte(0xFF, code->bytes()->byte_at(2));
  checkByte(0xFF, code->bytes()->byte_at(3));
  checkByte(0xFF, code->bytes()->byte_at(4));

  checkByte(0, code->bytes()->byte_at(5));
  checkByte(0, code->bytes()->byte_at(6));
  checkByte(0, code->bytes()->byte_at(7));
  checkByte(0, code->bytes()->byte_at(8));
}
TESTF(HCodeBufferTests, pushingOopShouldAddToOopArray) {
  BlockScavenge bs;
  code->pushOop(as_smiOop(2));
  checkOop(as_smiOop(2), code->oops()->obj_at(1));
}
TESTF(HCodeBufferTests, pushingByteShouldAddZeroToOopArray) {
  BlockScavenge bs;
  code->pushByte(1);
  checkOop(as_smiOop(0), code->oops()->obj_at(1));
}
