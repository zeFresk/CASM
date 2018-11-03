#include "bounded_integer.h"

#include <gtest/gtest.h>

class BintTests : public ::testing::Test {};

// tests ++, += and +
TEST_F(BintTests, plus)
{
	bint a{41, 2, false};
	bint b = a;
	b++;
	EXPECT_EQ(b.value(), 42) << "Error in post-incrementation.";
	++a;
	EXPECT_EQ(a.value(), 42) << "Error un pre-incrementation.";
	a += b;
	EXPECT_EQ(a.value(), 84) << "Error in addition.";
}

//tests --, -= and -
TEST_F(BintTests, minus)
{
	bint a{43, 2, false};
	bint b = a;
	b--;
	EXPECT_EQ(b.value(), 42) << "Error in post-decrementation.";
	--a;
	EXPECT_EQ(a.value(), 42) << "Error un pre-decrementation.";
	a -= b;
	EXPECT_EQ(a.value(), 0) << "Error in substraction.";
}

// tests exception are thrown in case they should
TEST_F(BintTests, OOB_plus)
{
	bint a{1, 1, true};
	bint b{25, 2, true};

	try {
		a += b;
		EXPECT_TRUE(false) << "No exception thrown when overflow !";
	}
	catch (...)
	{
		EXPECT_TRUE(true);
	}
}

// tests boundaries are excluded from OOB
TEST_F(BintTests, OOB_minus)
{
	bint a{1, 1, true};
	bint b{25, 2, true};

	try {
		a -= b;
		EXPECT_TRUE(false) << "No exception thrown when underflow !";
	}
	catch (...)
	{
		EXPECT_TRUE(true);
	}
}

TEST_F(BintTests, OOB_boundaries)
{
	try {
		bint a{99, 2, true};
		EXPECT_TRUE(true);
	}
	catch (...)
	{
		EXPECT_TRUE(false) << "99 is overflowing 2 digits bint.";
	}
	try {
		bint a{0, 2, true};
		EXPECT_TRUE(true);
	}
	catch (...)
	{
		EXPECT_TRUE(false) << "0 is underflowing 2 digits bint.";
	}
}
