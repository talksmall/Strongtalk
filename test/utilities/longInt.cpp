# include "incls/_precompiled.incl"
# include "incls/_longInt.cpp.incl"
#include "test.h"

using namespace easyunit;
 
TEST(longint, as_double)
{
	long_int value(1.0);
	double result = value.as_double();
	ASSERT_EQUALS_DELTA_V(1.0, result, 0.0001);
}

TEST(longint, asDoubleFromInt)
{
	long_int value(1000, 0);
	ASSERT_EQUALS_DELTA_V(1000.0, value.as_double(), 0.0001);
}

TEST(longint, asDoubleFromLongInt)
{
	long_int value(3, 1);
	double expected = (((int64_t)1)<<32) + 3;
	ASSERT_EQUALS_DELTA_V(expected, value.as_double(), 0.0001);
}

TEST(longint, equality)
{
	long_int lhs(5, 10);
	long_int rhs(5, 10);
	long_int different(0,0);
	ASSERT_TRUE(lhs == rhs);
	ASSERT_TRUE(!(different == rhs));
	ASSERT_TRUE(!(lhs == different));
}

TEST(longint, inequality)
{
	long_int lhs(5, 10);
	long_int rhs(5, 10);
	long_int different(0,0);
	ASSERT_TRUE(different != rhs);
	ASSERT_TRUE(lhs != different);
	ASSERT_TRUE(!(lhs != rhs));
}

TEST(longint, subtraction)
{
	long_int minuend(5, 10);
	long_int subtrahend(2, 3);
	long_int expected(3, 7);
	ASSERT_TRUE(expected == minuend - subtrahend);
}

TEST(longint, addition)
{
	long_int augend(5, 10);
	long_int addend(2, 3);
	long_int expected(7, 13);
	ASSERT_TRUE(expected == augend + addend);
}
