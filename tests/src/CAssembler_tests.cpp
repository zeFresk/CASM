#include <gtest/gtest.h>

#include "tools.h"

class ToolsTests : public ::testing::Test {};

TEST_F(ToolsTests, get_filename_works)
{
	const std::string simple_filename = "test.txt";
	EXPECT_EQ(get_filename(simple_filename), "test");
}
