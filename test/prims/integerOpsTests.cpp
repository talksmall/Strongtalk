# include "incls/_precompiled.incl"
# include "incls/_integerOps.cpp.incl"
#include "test.h"

using namespace easyunit;

DECLARE(IntegerOpsTest)
ResourceMark rm;
Integer *x, *y, *z;

#define ASSERT_EQUALS_M2(expected, actual, prefix)\
  ASSERT_EQUALS_M(expected, actual, report(prefix, expected, actual))
#define ASSERT_EQUALS_MH(expected, actual, prefix)\
  ASSERT_EQUALS_M(expected, actual, reportHex(prefix, expected, actual))
#define ASSERT_EQUALS_MS(expected, actual, prefix)\
  ASSERT_TRUE_M(!strcmp(expected, actual), report(prefix, expected, actual))
#define CHECK_SIZE(op, first, second, expected)\
  IntegerOps::string_to_Integer(first, 16, *x);\
  IntegerOps::string_to_Integer(second, 16, *y);\
  ASSERT_EQUALS_M2(expected, IntegerOps::op(*x, *y), "Wrong size")
#define CHECK_AND_SIZE(first, second, expected)\
  CHECK_SIZE(and_result_size_in_bytes, first, second, expected)
#define CHECK_OR_SIZE(first, second, expected)\
  CHECK_SIZE(or_result_size_in_bytes, first, second, expected)
#define CHECK_XOR_SIZE(first, second, expected)\
  CHECK_SIZE(xor_result_size_in_bytes, first, second, expected)
#define CHECK_OP(op, first, second, expected)\
  IntegerOps::string_to_Integer(first, 16, *x);\
  IntegerOps::string_to_Integer(second, 16, *y);\
  IntegerOps::op(*x, *y, *z);\
  char result[100];\
  IntegerOps::Integer_to_string(*z, 16, result);\
  ASSERT_TRUE_M(z->is_valid(), "Not a valid Integer");\
  ASSERT_EQUALS_MS(expected, result , "Wrong result")
#define CHECK_AND(first, second, expected)\
  CHECK_OP(and, first, second, expected)
#define CHECK_OR(first, second, expected)\
  CHECK_OP(or, first, second, expected)
#define CHECK_XOR(first, second, expected)\
  CHECK_OP(xor, first, second, expected)
#define CHECK_ASH_SIZE(first, second, expected)\
  IntegerOps::string_to_Integer(first, 16, *x);\
  ASSERT_EQUALS_M2(expected, IntegerOps::ash_result_size_in_bytes(*x, second) , "Wrong result")
#define CHECK_ASH(first, second, expected)\
  IntegerOps::string_to_Integer(first, 16, *x);\
  IntegerOps::ash(*x, second, *z);\
  char result[100];\
  IntegerOps::Integer_to_string(*z, 16, result);\
  ASSERT_TRUE_M(z->is_valid(), "Not a valid Integer");\
  ASSERT_EQUALS_MS(expected, result , "Wrong result")

char message[100];
char* reportHex(char*prefix, int expected, int actual) {
  sprintf(message, "%s. Expected: 0x%x, but was: 0x%x", prefix, expected, actual);
  return message;
}
char* report(char*prefix, int expected, int actual) {
  sprintf(message, "%s. Expected: %d, but was: %d", prefix, expected, actual);
  return message;
}
char* report(char*prefix, char* expected, char* actual) {
  sprintf(message, "%s. Expected: %s, but was: %s", prefix, expected, actual);
  return message;
}
END_DECLARE

SETUP(IntegerOpsTest) {
  x = (Integer*)NEW_RESOURCE_ARRAY(Digit, 5);
  y = (Integer*)NEW_RESOURCE_ARRAY(Digit, 5);
  z = (Integer*)NEW_RESOURCE_ARRAY(Digit, 5);
}
TEARDOWN(IntegerOpsTest){
}

TESTF(IntegerOpsTest, largeIntegerDivShouldReturnZeroWhenYLargerThanX) {
  IntegerOps::int_to_Integer(1, *x);
  IntegerOps::int_to_Integer(2, *y);
  IntegerOps::div(*x, *y, *z);
  
  bool ok;
  int result = z->as_int(ok);
  ASSERT_TRUE_M(ok, "invalid Integer");
  ASSERT_EQUALS_M(0, result, "wrong result");
}
TESTF(IntegerOpsTest, largeIntegerDivShouldReturnZeroWhenAbsYLargerThanX) {
  IntegerOps::int_to_Integer(1, *x);
  IntegerOps::int_to_Integer(-2, *y);
  IntegerOps::div(*x, *y, *z);
  
  bool ok;
  int result = z->as_int(ok);
  ASSERT_TRUE_M(ok, "invalid Integer");
  ASSERT_EQUALS_M2(-1, result, "wrong result");
}
TESTF(IntegerOpsTest, largeIntegerDivShouldReturnMinus1WhenYLargerThanAbsX) {
  IntegerOps::int_to_Integer(-1, *x);
  IntegerOps::int_to_Integer(2, *y);
  IntegerOps::div(*x, *y, *z);
  
  bool ok;
  int result = z->as_int(ok);
  ASSERT_TRUE_M(ok, "invalid Integer");
  ASSERT_EQUALS_M2(-1, result, "wrong result");
}
TESTF(IntegerOpsTest, largeIntegerDivShouldReturnZeroWhenAbsYLargerThanAbsX) {
  IntegerOps::int_to_Integer(-1, *x);
  IntegerOps::int_to_Integer(-2, *y);
  IntegerOps::div(*x, *y, *z);
  
  bool ok;
  int result = z->as_int(ok);
  ASSERT_TRUE_M(ok, "invalid Integer");
  ASSERT_EQUALS_M(0, result, "wrong result");
}
TESTF(IntegerOpsTest, largeIntegerDivShouldReturnM1WhenAbsYEqualsX) {
  IntegerOps::int_to_Integer(2, *x);
  IntegerOps::int_to_Integer(-2, *y);
  IntegerOps::div(*x, *y, *z);
  
  bool ok;
  int result = z->as_int(ok);
  ASSERT_TRUE_M(ok, "invalid Integer");
  ASSERT_EQUALS_M(-1, result, "wrong result");
}
TESTF(IntegerOpsTest, xpyShouldHandleAllBitsPlus1) {
  Digit x = 0xffffffff;
  Digit y = 1;
  Digit c = 0;
  ASSERT_EQUALS_M(0, IntegerOps::xpy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(1, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xpyShouldHandle1PlusAllBits) {
  Digit x = 1;
  Digit y = 0xffffffff;
  Digit c = 0;
  ASSERT_EQUALS_M(0, IntegerOps::xpy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(1, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xpyShouldHandleAllBitsPlusAllBitsPlusAllBits) {
  Digit x = 0xffffffff;
  Digit y = 0xffffffff;
  Digit c = 0xffffffff;
  ASSERT_EQUALS_M(0xfffffffd, IntegerOps::xpy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(2, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xmyShouldHandle0Minus1) {
  Digit x = 0;
  Digit y = 1;
  Digit c = 0;
  ASSERT_EQUALS_M(0xffffffff, IntegerOps::xmy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(1, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xmyShouldHandle0Minus0) {
  Digit x = 0;
  Digit y = 0;
  Digit c = 0;
  ASSERT_EQUALS_M(0, IntegerOps::xmy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(0, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xmyShouldHandle0Minus0Carry1) {
  Digit x = 0;
  Digit y = 0;
  Digit c = 1;
  ASSERT_EQUALS_M(0xffffffff, IntegerOps::xmy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(1, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xmyShouldHandle0MinusAllOnesCarry1) {
  Digit x = 0;
  Digit y = 0xffffffff;
  Digit c = 1;
  ASSERT_EQUALS_M(0, IntegerOps::xmy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(1, c, "Wrong carry");
}
TESTF(IntegerOpsTest, axpyShouldHandleAllOnes) {
  Digit a = 0xffffffff;
  Digit x = 0xffffffff;
  Digit y = 0xffffffff;
  Digit c = 0xffffffff;
  ASSERT_EQUALS_M(0xffffffff, IntegerOps::axpy(a, x, y, c), "Wrong total");
  ASSERT_EQUALS_M(0xffffffff, c, "Wrong carry");
}
TESTF(IntegerOpsTest, axpyShouldHandleAllZeroes) {
  Digit a = 0;
  Digit x = 0;
  Digit y = 0;
  Digit c = 0;
  ASSERT_EQUALS_M(0, IntegerOps::axpy(a, x, y, c), "Wrong total");
  ASSERT_EQUALS_M(0, c, "Wrong carry");
}
TESTF(IntegerOpsTest, axpyWithPieces) {
  Digit a = 0xffff;
  Digit x = 0x10000;
  Digit y = 0xff00;
  Digit c = 0x00ff;
  ASSERT_EQUALS_M(0xffffffff, IntegerOps::axpy(a, x, y, c), "Wrong total");
  ASSERT_EQUALS_M(0, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xdyWithZeroShouldBeZero) {
  Digit x = 0;
  Digit y = 1;
  Digit c = 0;
  ASSERT_EQUALS_M(0, IntegerOps::xdy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(0, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xdyByTwoShouldHaveZeroCarry) {
  Digit x = 0;
  Digit y = 2;
  Digit c = 1;
  ASSERT_EQUALS_M(0x80000000, IntegerOps::xdy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(0, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xdyByTwoShouldHaveCarryWhenOdd) {
  Digit x = 1;
  Digit y = 2;
  Digit c = 1;
  ASSERT_EQUALS_M(0x80000000, IntegerOps::xdy(x, y, c), "Wrong total");
  ASSERT_EQUALS_M(1, c, "Wrong carry");
}
TESTF(IntegerOpsTest, xdyWithAllOnes) {
  Digit x = 0xffffffff;
  Digit y = 0x80000000;
  Digit c = 1;
  Digit result = IntegerOps::xdy(x, y, c);
  ASSERT_EQUALS_M(3, result, report("Wrong total", 3, result));
  ASSERT_EQUALS_M(0x7fffffff, c, "Wrong carry");
}
TESTF(IntegerOpsTest, qr_decompositionSimple) {
  IntegerOps::unsigned_int_to_Integer(0xffffffff, *x);
  IntegerOps::unsigned_int_to_Integer(1, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0, result[0], "Wrong remainder.");
  ASSERT_EQUALS_M(0xffffffff, result[1], "Wrong quotient.");
}
TESTF(IntegerOpsTest, qr_decompositionSimpleWithRemainder) {
  IntegerOps::unsigned_int_to_Integer(0xffffffff, *x);
  IntegerOps::unsigned_int_to_Integer(0x80000000, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0x7fffffff, result[0], report("Wrong remainder", 0x7fffffff, result[0]));
  ASSERT_EQUALS_M(1, result[1], report("Wrong quotient", 1, result[1]));
}
TESTF(IntegerOpsTest, qr_decompositionTwoDigitNoRemainder) {
  IntegerOps::string_to_Integer("FFFFFFFF80000000", 16, *x);
  IntegerOps::unsigned_int_to_Integer(0x80000000, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0, result[0], report("Wrong remainder", 0, result[0]));
  ASSERT_EQUALS_M(0xffffffff, result[1], report("Wrong quotient word 1", 0xffffffff, result[1]));
  ASSERT_EQUALS_M(1, result[2], report("Wrong quotient word 2", 1, result[2]));
}
TESTF(IntegerOpsTest, qr_decompositionTwoDigitWithRemainder) {
  IntegerOps::string_to_Integer("FFFFFFFFFFFFFFFF", 16, *x);
  IntegerOps::unsigned_int_to_Integer(0x80000000, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0x7fffffff, result[0], report("Wrong remainder", 0x7fffffff, result[0]));
  ASSERT_EQUALS_M(0xffffffff, result[1], report("Wrong quotient word 1", 0xffffffff, result[1]));
  ASSERT_EQUALS_M(1, result[2], report("Wrong quotient word 2", 1, result[2]));
}
TESTF(IntegerOpsTest, qr_decompositionThreeDigitWithRemainder) {
  IntegerOps::string_to_Integer("FFFFFFFFFFFFFFFFFFFFFFFF", 16, *x);
  IntegerOps::string_to_Integer("800000000000", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0xffffffff, result[0], report("Wrong remainder word 1", 0xffffffff, result[0]));
  ASSERT_EQUALS_M(0x7fff, result[1], report("Wrong remainder word 2", 0x7fff, result[1]));
  ASSERT_EQUALS_M(0xffffffff, result[2], report("Wrong quotient word 1", 0xffffffff, result[2]));
  ASSERT_EQUALS_M(0x1ffff, result[3], report("Wrong quotient word 2", 0x1ffff, result[3]));
}
TESTF(IntegerOpsTest, qr_decompositionTwoDigitDivisorNoRemainder) {
  IntegerOps::string_to_Integer("800000000000000000000000", 16, *x);
  IntegerOps::string_to_Integer("8000000000000000", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0, result[0], report("Wrong remainder low word", 0, result[0]));
  ASSERT_EQUALS_M(0, result[1], report("Wrong remainder high word", 0, result[1]));
  ASSERT_EQUALS_M(0, result[2], report("Wrong quotient low word", 0, result[2]));
  ASSERT_EQUALS_M(1, result[3], report("Wrong quotient high word", 1, result[3]));
}
TESTF(IntegerOpsTest, qr_decompositionScaledTwoDigitDivisorNoRemainder) {
  IntegerOps::string_to_Integer("800000000000000000000000", 16, *x);
  IntegerOps::string_to_Integer("0800000000000000", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0, result[0], report("Wrong remainder low word", 0, result[0]));
  ASSERT_EQUALS_M(0, result[1], report("Wrong remainder high word", 0, result[1]));
  ASSERT_EQUALS_M(0, result[2], report("Wrong quotient low word", 0, result[2]));
  ASSERT_EQUALS_M(0x10, result[3], report("Wrong quotient high word", 0x10, result[3]));
}
TESTF(IntegerOpsTest, qr_decompositionTwoDigitDivisorSmallRemainder) {
  IntegerOps::string_to_Integer("800000000000000000000001", 16, *x);
  IntegerOps::string_to_Integer("8000000000000000", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(1, result[0], report("Wrong remainder low word", 1, result[0]));
  ASSERT_EQUALS_M(0, result[1], report("Wrong remainder high word", 0, result[1]));
  ASSERT_EQUALS_M(0, result[2], report("Wrong quotient low word", 0, result[2]));
  ASSERT_EQUALS_M(1, result[3], report("Wrong quotient high word", 1, result[3]));
}
TESTF(IntegerOpsTest, qr_decompositionTwoDigitDivisorSmallAllOnesRemainder) {
  IntegerOps::string_to_Integer("8000000000000000FFFFFFFF", 16, *x);
  IntegerOps::string_to_Integer("8000000000000000", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0xffffffff, result[0], report("Wrong remainder low word", 0xffffffff, result[0]));
  ASSERT_EQUALS_M(0, result[1], report("Wrong remainder high word", 0, result[1]));
  ASSERT_EQUALS_M(0, result[2], report("Wrong quotient low word", 0, result[2]));
  ASSERT_EQUALS_M(1, result[3], report("Wrong quotient high word", 1, result[3]));
}
TESTF(IntegerOpsTest, qr_decompositionTwoDigitDivisorTwoDigitRemainder) {
  IntegerOps::string_to_Integer("8000000000007FFFFFFFFFFF", 16, *x);
  IntegerOps::string_to_Integer("8000000000000000", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0xffffffff, result[0], report("Wrong remainder low word", 0xffffffff, result[0]));
  ASSERT_EQUALS_M(0x7fff, result[1], report("Wrong remainder high word", 0x7fff, result[1]));
  ASSERT_EQUALS_M(0, result[2], report("Wrong quotient low word", 0, result[2]));
  ASSERT_EQUALS_M(1, result[3], report("Wrong quotient high word", 1, result[3]));
}
TESTF(IntegerOpsTest, qr_decompositionTwoNonZeroDigitDivisorTwoDigitRemainder) {
  IntegerOps::string_to_Integer("FFFFFFFFFFFFFFF1FFFFFFFF", 16, *x);
  IntegerOps::string_to_Integer("0FFFFFFFFFFFFFFF", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0xffffffff, result[0], report("Wrong remainder low word", 0xffffffff, result[0]));
  ASSERT_EQUALS_M(0x1, result[1], report("Wrong remainder high word", 0x1, result[1]));
  ASSERT_EQUALS_M(0, result[2], report("Wrong quotient low word", 0, result[2]));
  ASSERT_EQUALS_M(0x10, result[3], report("Wrong quotient high word", 0x10, result[3]));
}
TESTF(IntegerOpsTest, qr_decompositionFirstDigitsMatch) {
  IntegerOps::string_to_Integer("FFFFFFFF0000000000000000", 16, *x);
  IntegerOps::string_to_Integer("FFFFFFFF00000000", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0x00000000, result[0], report("Wrong remainder low word", 0x00000000, result[0]));
  ASSERT_EQUALS_M(0x0, result[1], report("Wrong remainder high word", 0x0, result[1]));
  ASSERT_EQUALS_M(0, result[2], report("Wrong quotient low word", 0, result[2]));
  ASSERT_EQUALS_M(0x1, result[3], report("Wrong quotient high word", 0x1, result[3]));
}
TESTF(IntegerOpsTest, qr_decompositionComplexCase) {
  IntegerOps::string_to_Integer("121FA00AD77D7422347E9A0F6729E011", 16, *x);
  IntegerOps::string_to_Integer("FEDCBA9876543210", 16, *y);

  Digit* result = IntegerOps::qr_decomposition(*x, *y);
  ASSERT_EQUALS_M(0x11111111, result[0], report("Wrong remainder low word", 0x11111111, result[0]));
  ASSERT_EQUALS_M(0x11111111, result[1], report("Wrong remainder high word", 0x11111111, result[1]));
  ASSERT_EQUALS_M(0x9ABCDEF0, result[2], report("Wrong quotient low word", 0x9ABCDEF0, result[2]));
  ASSERT_EQUALS_M(0x12345678, result[3], report("Wrong quotient high word", 0x12345678, result[3]));
}
TESTF(IntegerOpsTest, unsignedQuo) {
  IntegerOps::string_to_Integer("121FA00AD77D7422347E9A0F6729E011", 16, *x);
  IntegerOps::string_to_Integer("FEDCBA9876543210", 16, *y);

  IntegerOps::unsigned_quo(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("123456789abcdef0", result, "Wrong quotient");
}
TESTF(IntegerOpsTest, unsignedRem) {
  IntegerOps::string_to_Integer("121FA00AD77D7422347E9A0F6729E011", 16, *x);
  IntegerOps::string_to_Integer("FEDCBA9876543210", 16, *y);

  IntegerOps::unsigned_rem(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("1111111111111111", result, "Wrong remainder");
}
TESTF(IntegerOpsTest, unsignedRemWhenXIsShortThanY) {
  IntegerOps::string_to_Integer("12345678", 16, *x);
  IntegerOps::string_to_Integer("FEDCBA9876543210", 16, *y);

  IntegerOps::unsigned_rem(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("12345678", result, "Wrong remainder");
}
TESTF(IntegerOpsTest, unsignedRemWithNoRemainder) {
  IntegerOps::string_to_Integer("12345678", 16, *x);
  IntegerOps::string_to_Integer("12345678", 16, *y);

  IntegerOps::unsigned_rem(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_TRUE_M(z->is_zero(), "Remainder should be zero");
}
TESTF(IntegerOpsTest, divWithLargeDividendNegDivisorAndRemainder) {
  IntegerOps::string_to_Integer("1234567809abcdef", 16, *x);
  IntegerOps::string_to_Integer("-12345678", 16, *y);

  IntegerOps::div(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("-100000001", result, "Wrong quotient");
}
TESTF(IntegerOpsTest, modWithLargeNegDividendAndRemainder) {
  IntegerOps::string_to_Integer("-1234567809abcdef", 16, *x);
  IntegerOps::string_to_Integer("12345678", 16, *y);

  IntegerOps::mod(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("8888889", result, "Wrong remainder");
}
TESTF(IntegerOpsTest, modWithLargeNegDividendNegDivisorAndRemainder) {
  IntegerOps::string_to_Integer("-1234567809abcdef", 16, *x);
  IntegerOps::string_to_Integer("-12345678", 16, *y);

  IntegerOps::mod(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("-9abcdef", result, "Wrong remainder");
}
TESTF(IntegerOpsTest, modWithLargeDividendNegDivisorAndRemainder) {
  IntegerOps::string_to_Integer("1234567809abcdef", 16, *x);
  IntegerOps::string_to_Integer("-12345678", 16, *y);

  IntegerOps::mod(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("-8888889", result, "Wrong remainder");
}
TESTF(IntegerOpsTest, modWithLargeDividendNegDivisorAndNoRemainder) {
  IntegerOps::string_to_Integer("1234567800000000", 16, *x);
  IntegerOps::string_to_Integer("-12345678", 16, *y);

  IntegerOps::mod(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("0", result, "Wrong remainder");
}
TESTF(IntegerOpsTest, modWithLongerDivisor) {
  IntegerOps::string_to_Integer("12345678", 16, *x);
  IntegerOps::string_to_Integer("-100000000", 16, *y);

  ASSERT_TRUE_M(y->is_negative(), "Should be negative");
  IntegerOps::mod(*x, *y, *z);
  char result[100];
  IntegerOps::Integer_to_string(*z, 16, result);

  ASSERT_EQUALS_MS("-edcba988", result, "Wrong remainder");
}
TESTF(IntegerOpsTest, divResultSizeInBytesWithTwoPositiveIntegers) {
  IntegerOps::string_to_Integer("123456781234567812345678", 16, *x);
  IntegerOps::string_to_Integer("12345678", 16, *y);

  int result = IntegerOps::div_result_size_in_bytes(*x, *y);

  ASSERT_EQUALS_M2(sizeof(int) + (3 * sizeof(Digit)), result, "Wrong size");
}
TESTF(IntegerOpsTest, divResultSizeInBytesWithTwoNegativeIntegers) {
  IntegerOps::string_to_Integer("-123456781234567812345678", 16, *x);
  IntegerOps::string_to_Integer("-12345678", 16, *y);

  int result = IntegerOps::div_result_size_in_bytes(*x, *y);

  ASSERT_EQUALS_M2(sizeof(int) + (3 * sizeof(Digit)), result, "Wrong size");
}
TESTF(IntegerOpsTest, divResultSizeInBytesWithPosDividendAndNegDivisor) {
  IntegerOps::string_to_Integer("123456781234567812345678", 16, *x);
  IntegerOps::string_to_Integer("-12345678", 16, *y);

  int result = IntegerOps::div_result_size_in_bytes(*x, *y);

  ASSERT_EQUALS_M2(sizeof(int) + (4 * sizeof(Digit)), result, "Wrong size");
}
TESTF(IntegerOpsTest, modResultSizeInBytesWithTwoPositiveIntegers) {
  IntegerOps::string_to_Integer("123456781234567812345678", 16, *x);
  IntegerOps::string_to_Integer("12345678", 16, *y);

  int result = IntegerOps::mod_result_size_in_bytes(*x, *y);

  ASSERT_EQUALS_M2(sizeof(int) + (1 * sizeof(Digit)), result, "Wrong size");
}
TESTF(IntegerOpsTest, modResultSizeInBytesWithTwoNegativeIntegers) {
  IntegerOps::string_to_Integer("-123456781234567812345678", 16, *x);
  IntegerOps::string_to_Integer("-1234567812345678", 16, *y);

  int result = IntegerOps::mod_result_size_in_bytes(*x, *y);

  ASSERT_EQUALS_M2(sizeof(int) + (2 * sizeof(Digit)), result, "Wrong size");
}
TESTF(IntegerOpsTest, orWithTwoPositive) {
  CHECK_OR("1", "FFFFFFFF", "ffffffff");
}
TESTF(IntegerOpsTest, orWithFirstZero) {
  CHECK_OR("0", "FFFFFFFF", "ffffffff");
}
TESTF(IntegerOpsTest, orWithSecondNegative) {
  CHECK_OR("1", "-1", "-1");
}
TESTF(IntegerOpsTest, orWithSecondNegativeAndTwoDigits) {
  CHECK_OR("1000000001", "-100000001", "-100000001");
}
TESTF(IntegerOpsTest, orWithFirstLongerSecondNegative) {
  CHECK_OR("123456789a", "-1", "-1");
}
TESTF(IntegerOpsTest, orWithSecondLongerAndNegative) {
  CHECK_OR("1", "-100000001", "-100000001");
}
TESTF(IntegerOpsTest, orWithFirstZeroAndSecondNegative) {
  CHECK_OR("0", "-100000001", "-100000001");
}
TESTF(IntegerOpsTest, orWithSecondZero) {
  CHECK_OR("FFFFFFFF", "0", "ffffffff");
}
TESTF(IntegerOpsTest, orWithFirstNegative) {
  CHECK_OR("-1", "1", "-1");
}
TESTF(IntegerOpsTest, orWithFirstNegativeAndTwoDigits) {
  CHECK_OR("-100000001", "1000000001", "-100000001");
}
TESTF(IntegerOpsTest, orWithFirstNegativeAndLonger) {
  CHECK_OR("-100000002", "1", "-100000001");
}
TESTF(IntegerOpsTest, orWithBothNegative) {
  CHECK_OR("-ffff0001", "-00010000", "-1");
}
TESTF(IntegerOpsTest, orWithBothNegativeAndTwoDigits) {
  CHECK_OR("-ffffffffffff0001", "-ffffffff00010000", "-ffffffff00000001");
}
TESTF(IntegerOpsTest, orWithBothNegativeAndTooManyDigits) {
  CHECK_OR("-1000000000000", "-ffff000010000000", "-10000000");
}
TESTF(IntegerOpsTest, orWithBothZero) {
  CHECK_OR("0", "0", "0");
}
TESTF(IntegerOpsTest, xorWithBothZero) {
  CHECK_XOR("0", "0", "0");
}
TESTF(IntegerOpsTest, xorWithBothPositiveAndDifferent) {
  CHECK_XOR("2", "1", "3");
}
TESTF(IntegerOpsTest, xorWithBothPositiveAndTheSame) {
  CHECK_XOR("2", "2", "0");
}
TESTF(IntegerOpsTest, xorWithFirstNegative) {
  CHECK_XOR("-1", "1", "-2");
}
TESTF(IntegerOpsTest, xorWithFirstNegativeAndLonger) {
  CHECK_XOR("-1000000000", "1", "-fffffffff");
}
TESTF(IntegerOpsTest, xorWithFirstNegativeAndShorter) {
  CHECK_XOR("-1", "1000000000", "-1000000001");
}
TESTF(IntegerOpsTest, xorWithSecondNegative) {
  CHECK_XOR("1", "-1", "-2");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeAndLonger) {
  CHECK_XOR("1", "-1000000000", "-fffffffff");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeAndThreeDigits) {
  CHECK_XOR("1", "-100000000000000000", "-fffffffffffffffff");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeAndShorter) {
  CHECK_XOR("1000000000", "-1", "-1000000001");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeAndShorterWithCarry) {
  CHECK_XOR("1fffffffe", "-2", "-200000000");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeAndBothTwoDigits) {
  CHECK_XOR("effffffff", "-100000001", "-1000000000");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeAndSecondDigitsZero) {
  CHECK_XOR("200000000", "-100000000", "-300000000");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeAndOverflow) {
  CHECK_XOR("ffff0000", "-100010000", "-200000000");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeWithAllDigits) {
  CHECK_XOR("123456789", "-876543210", "-955115587");
}
TESTF(IntegerOpsTest, xorWithSecondNegativeFirstThreeDigits) {
  CHECK_XOR("10000000000000000", "-1", "-10000000000000001");
}
TESTF(IntegerOpsTest, xorWithTwoNegative) {
  CHECK_XOR("-2", "-1", "1");
}
TESTF(IntegerOpsTest, xorWithTwoNegativeOneDigitComplex) {
  CHECK_XOR("-f0f0f0f0", "-f0f0f0f", "ffffffe1");
}
TESTF(IntegerOpsTest, xorWithTwoNegativeTwoDigits) {
  CHECK_XOR("-1234567890ABCDEF", "-FEDCBA0987654321", "ece8ec7117ce8ece");
}
TESTF(IntegerOpsTest, xorWithTwoNegativeFirstThreeDigits) {
  CHECK_XOR("-123456780000000000000000", "-1", "12345677ffffffffffffffff");
}
TESTF(IntegerOpsTest, xorWithTwoNegativeSecondThreeDigits) {
  CHECK_XOR("-1", "-123456780000000000000000", "12345677ffffffffffffffff");
}
TESTF(IntegerOpsTest, xorWithTwoNegativeTwoDigitUnderflow) {
  CHECK_XOR("-100000000", "-ffffffff", "1");
}
TESTF(IntegerOpsTest, xorWithTwoNegativeFirstLonger) {
  CHECK_XOR("-1234567890ABCDEF", "-87654321", "1234567817ce8ece");
}
TESTF(IntegerOpsTest, andWithTwoPositive) {
  CHECK_AND("FFFFFFFF", "FFFFFFFF", "ffffffff");
}
TESTF(IntegerOpsTest, andWithFirstPositive) {
  CHECK_AND("1", "-1", "1");
}
TESTF(IntegerOpsTest, andWithSecondPositive) {
  CHECK_AND("-1", "1", "1");
}
TESTF(IntegerOpsTest, andWithFirstPositiveTwoDigit) {
  CHECK_AND("FFFFFFFFF", "-1", "fffffffff");
}
TESTF(IntegerOpsTest, andWithFirstPositiveTwoDigit2) {
  CHECK_AND("FFFFFFFFF", "-10", "ffffffff0");
}
TESTF(IntegerOpsTest, andWithSecondPositiveTwoDigit) {
  CHECK_AND("-10", "FFFFFFFFF", "ffffffff0");
}
TESTF(IntegerOpsTest, andWithBothNegative) {
  CHECK_AND("-1", "-1", "-1");
}
TESTF(IntegerOpsTest, andWithBothNegativeAndTwoDigits) {
  CHECK_AND("-1ffffffff", "-100000001", "-1ffffffff");
}
TESTF(IntegerOpsTest, andWithBothNegativeAndFirstLonger) {
  CHECK_AND("-1ffffffff", "-1", "-1ffffffff");
}
TESTF(IntegerOpsTest, andWithBothNegativeAndFirstThreeLong) {
  CHECK_AND("-10000000000000000", "-1", "-10000000000000000");
}
TESTF(IntegerOpsTest, andWithBothNegativeAndSecondLonger) {
  CHECK_AND("-1", "-100000001", "-100000001");
}
TESTF(IntegerOpsTest, andWithFirstZero) {
  CHECK_AND("0", "-1", "0");
}
TESTF(IntegerOpsTest, andWithSecondZero) {
  CHECK_AND("-1", "0", "0");
}
TESTF(IntegerOpsTest, andResultSizeInBytesWithTwoPositive) {
  CHECK_AND_SIZE("123456781234567812345678",
                 "1234567812345678",
                 sizeof(int) + (2 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, andResultSizeInBytesWithSecondNegativeAndShorter) {
  CHECK_AND_SIZE("123456781234567812345678",
                 "-1234567812345678",
                 sizeof(int) + (3 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, andResultSizeInBytesWithFirstLongerAndNegative) {
  CHECK_AND_SIZE("-123456781234567812345678",
                 "1234567812345678",
                 sizeof(int) + (2 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, andResultSizeInBytesWithFirstShorterAndPositive) {
  CHECK_AND_SIZE("1234567812345678",
                 "-123456781234567812345678",
                 sizeof(int) + (2 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, andResultSizeInBytesWithFirstZero) {
  CHECK_AND_SIZE("0", "-123456781234567812345678", sizeof(int) + (0 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, andResultSizeInBytesWithSecondZero) {
  CHECK_AND_SIZE("-123456781234567812345678", "0", sizeof(int) + (0 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, orResultSizeInBytesWithFirstLongerAndNegative) {
  CHECK_OR_SIZE("-123456781234567812345678",
                 "1234567812345678",
                 sizeof(int) + (3 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, orResultSizeInBytesWithFirstShorterAndNegative) {
  CHECK_OR_SIZE("-12345678",
                 "1234567812345678",
                 sizeof(int) + (1 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, orResultSizeInBytesWithFirstShorterAndBothNegative) {
  CHECK_OR_SIZE("-12345678",
                 "-1234567812345678",
                 sizeof(int) + (1 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, orResultSizeInBytesWithSecondShorterAndBothNegative) {
  CHECK_OR_SIZE("-1234567812345678",
                 "-12345678",
                 sizeof(int) + (1 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, orResultSizeInBytesWithSecondShorterAndNegative) {
  CHECK_OR_SIZE("1234567812345678",
                 "-12345678",
                 sizeof(int) + (1 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, xorResultSizeInBytesWithFirstLongerAndNegative) {
  CHECK_XOR_SIZE("-123456781234567812345678",
                 "1234567812345678",
                 sizeof(int) + (4 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, xorResultSizeInBytesWithBothPositive) {
  CHECK_XOR_SIZE("123456781234567812345678",
                 "1234567812345678",
                 sizeof(int) + (3 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, ashResultSizeInBytesWhenNoShift) {
  CHECK_ASH_SIZE("2",
                 0,
                 sizeof(int) + (1 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, ashResultSizeInBytesWhenPositive) {
  CHECK_ASH_SIZE("2",
                 1,
                 sizeof(int) + (1 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, ashResultSizeInBytesWhenPositiveWithUnderflow) {
  CHECK_ASH_SIZE("1",
                 -1,
                 sizeof(int) + (1 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, ashResultSizeInBytesWhenNegativeWithUnderflow) {
  CHECK_ASH_SIZE("-1",
                 -1,
                 sizeof(int) + (1 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, ashResultSizeInBytesWhenPositiveWithOverflow) {
  CHECK_ASH_SIZE("2",
                 63,
                 sizeof(int) + (3 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, ashResultSizeInBytesWhenZero) {
  CHECK_ASH_SIZE("0",
                 32,
                 sizeof(int) + (0 * sizeof(Digit)));
}
TESTF(IntegerOpsTest, ashDigitShiftSimple) {
  CHECK_ASH("2", 1, "4");
}
TESTF(IntegerOpsTest, ashDigitShift) {
  CHECK_ASH("1", 32, "100000000");
}
TESTF(IntegerOpsTest, ashDigitShiftWithNegative) {
  CHECK_ASH("-1", 32, "-100000000");
}
TESTF(IntegerOpsTest, ashTwoAndAHalfDigitShift) {
  CHECK_ASH("1", 80, "100000000000000000000");
}
TESTF(IntegerOpsTest, ashSimpleShift) {
  CHECK_ASH("1", 16, "10000");
}
TESTF(IntegerOpsTest, ashOverFlow) {
  CHECK_ASH("ffffffff", 16, "ffffffff0000");
}
TESTF(IntegerOpsTest, ashTwoDigitOverFlow) {
  CHECK_ASH("ffffffffffffffff", 16, "ffffffffffffffff0000");
}
TESTF(IntegerOpsTest, ashNegativeBitShift) {
  CHECK_ASH("2", -1, "1");
}
TESTF(IntegerOpsTest, ashNegativeBitShiftWithUnderflow) {
  CHECK_ASH("100000000", -1, "80000000");
}
TESTF(IntegerOpsTest, ashNegativeBitShiftWithTwoDigitUnderflow) {
  CHECK_ASH("1", -64, "0");
}
TESTF(IntegerOpsTest, ashNegativeBitShiftNegativeValueWithUnderflow) {
  CHECK_ASH("-100000000", -1, "-80000000");
}
TESTF(IntegerOpsTest, ashNegativeDigitShift) {
  CHECK_ASH("100000000", -32, "1");
}
TESTF(IntegerOpsTest, ashNegativeHalfDigitShift) {
  CHECK_ASH("ffffffff0000", -16, "ffffffff");
}
TESTF(IntegerOpsTest, ashNegativeUnderflowShouldResultInNegativeOne) {
  CHECK_ASH("-1", -1, "-1");
}
TESTF(IntegerOpsTest, hashShouldXorDigits) {
  IntegerOps::string_to_Integer("-12345678ffffffff", 16, *x);

  int result = IntegerOps::hash(*x);

  ASSERT_EQUALS_M2((0x12345678 ^ 0xffffffff ^ -2) >> 2, result, "Wrong hash");
}
