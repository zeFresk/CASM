#include <gtest/gtest.h>

#include <regex>

#include "regex.h"

class RegexLineTests : public ::testing::Test {};
using _impl::separate_line;


/*
	Big MACRO generating a test for the separate_line regex
	Well... It's dirty but it simplify test-making...
*/
#define REGEX_FULL_LINE_TEST(TEST_NAME, LINE_STR, LINE_LABEL, LINE_INSTRUCTION, LINE_PARAMETER, LINE_COMMENT) TEST_F(RegexLineTests, TEST_NAME) {\
	std::smatch m;\
	std::string const str{LINE_STR};\
	bool v = std::regex_match(str, m, separate_line);\
	EXPECT_TRUE(v) << LINE_STR << " isn't detected as correct.";\
	\
	EXPECT_EQ(m[1].str(), LINE_LABEL) << "Can't extract label from " << LINE_STR;\
	EXPECT_EQ(m[2].str(), LINE_INSTRUCTION) << "Can't extract instruction from " << LINE_STR;\
	EXPECT_EQ(m[3].str(), LINE_PARAMETER) << "Can't extract paramater from " << LINE_STR;\
	EXPECT_EQ(m[4].str(), LINE_COMMENT) << "Can't extract comment from " << LINE_STR;\
	}

//	Check if lines containing everything possible are correctly splitted
REGEX_FULL_LINE_TEST(full_simple, "label: lda var ;comment", "label", "lda", "var", "comment")

//	Check if lines with spaces are correctly splitted
REGEX_FULL_LINE_TEST(full_spaced, "  label  :   lda  var        ;           comment", "label", "lda", "var", "comment")

// Check if lines containing expressions are correctly splitted
REGEX_FULL_LINE_TEST(full_expression_paramater, "label: lda [var + 3] ;comment", "label", "lda", "[var + 3]", "comment")

//	Check if instructions starting with a dot are correctly splitted
REGEX_FULL_LINE_TEST(full_dot_instructions, "label: .word var ;comment", "label", ".word", "var", "comment")

//	Check if integer value as parameter work is correctly extracted
REGEX_FULL_LINE_TEST(full_int_parameter, "label: lda 032 ;comment", "label", "lda", "032", "comment")