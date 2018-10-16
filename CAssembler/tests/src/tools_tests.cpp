#include <gtest/gtest.h>

#include "tools.h"

class ToolsTests : public ::testing::Test {};

TEST_F(ToolsTests, get_filename_works)
{
	const std::string simple_filename = "test.txt";
	EXPECT_EQ(get_filename(simple_filename), "test") << "Can't resolve basic filename.";

	const std::string complex_filename = "test.av.tct";
	EXPECT_EQ(get_filename(complex_filename), "test.av") << "Can't resolve complex filename.";

	const std::string spaced_filename = "test spaced.tct";
	EXPECT_EQ(get_filename(spaced_filename), "test spaced") << "Can't resolve filename containg space.";
}
