#include <gtest/gtest.h>

#include <regex>

#include "regex.h"

class RegexLineTests : public ::testing::Test {};
using _impl::separate_line;

class RegexParametersTests : public ::testing::Test {};
using _impl::parameters_regex;

class ExpressionRegexTests : public ::testing::Test {};
using _impl::is_expression;
using _impl::evaluate_regex;


/*##
####  Tests differents valid lines and verify than data are coorectly extracted
##*/
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

// Check if a line containing only comment is correctly parsed
REGEX_FULL_LINE_TEST(partial_only_comments, ";comment", "", "", "", "comment")


/*##
####  Tests differents valid arguments
##*/
/*
	Smaller MACRO generating a test for checking if argument is correctly detected as valid.
*/
#define REGEX_TEST_VALID_PARAMETER(TEST_NAME, ARGUMENT) TEST_F(RegexParametersTests, TEST_NAME) {\
	std::string const str{ARGUMENT};\
	bool v = std::regex_match(str, parameters_regex);\
	EXPECT_TRUE(v) << ARGUMENT << " is not detected as a valid argument";\
	}

// Check if a simple variable as parameter is valid
REGEX_TEST_VALID_PARAMETER(basic_variable, "var")

// Check if a number as parameter is valid
REGEX_TEST_VALID_PARAMETER(dd_number, "42")

// Check if number starting with 0 is valid
REGEX_TEST_VALID_PARAMETER(dd_with_zero_number, "042")

// Check if expression is a valid parameter
REGEX_TEST_VALID_PARAMETER(basic_expression, "[var + 3]")

// Check if spaced expression is a valid parameter
REGEX_TEST_VALID_PARAMETER(spaced_expression, "[  var -   2     ]")

// Check if expression is a valid parameter
REGEX_TEST_VALID_PARAMETER(inverted_basic_expression, "[3 + var]")


/*##
####  Tests differents valid expressions
##*/
/*
	Another MACRO... Yeah I know...
*/
#define REGEX_TEST_VALID_EXPRESSION(TEST_NAME, EXPRESSION) TEST_F(ExpressionRegexTests, TEST_NAME) {\
	std::string const str{EXPRESSION};\
	bool v = std::regex_match(str, is_expression);\
	EXPECT_TRUE(v) << EXPRESSION << " is not detected as a valid expression";\
	}

REGEX_TEST_VALID_EXPRESSION(basic_expression, "[var + 3]")
REGEX_TEST_VALID_EXPRESSION(spaced_expression, "[  var -   2     ]")
REGEX_TEST_VALID_EXPRESSION(compressed_expression, "[var-2]")
REGEX_TEST_VALID_EXPRESSION(inverted_basic_expression, "[3 + var]")

/*##
####  Tests expression data extraction and evaluation
##*/
/*
	Well, at least it helps !
*/
#define REGEX_TEST_EXTRACTING_FROM_EXPRESSION(TEST_NAME, EXPRESSION, LHS, OPERATOR, RHS) TEST_F(ExpressionRegexTests, TEST_NAME) {\
	std::string const str{EXPRESSION};\
	std::smatch m;\
	bool v = std::regex_match(str, m, evaluate_regex);\
	EXPECT_TRUE(v) << EXPRESSION << " is not detected as valid expression to evaluate";\
	EXPECT_EQ(m[1].str(), LHS) << "lhs is not correctly extracted from " << EXPRESSION;\
	EXPECT_EQ(m[2].str(), OPERATOR) << "operator is not correctly extracted from " << EXPRESSION;\
	EXPECT_EQ(m[3].str(), RHS) << "rhs is not correctly extracted from " << EXPRESSION;\
	}

// Check if informations from basics expressions can be extracted correctly
REGEX_TEST_EXTRACTING_FROM_EXPRESSION(extraction_basic_expression, "[var + 3]", "var", "+", "3")
REGEX_TEST_EXTRACTING_FROM_EXPRESSION(extraction_ibasic_expression, "[3 + var]", "3", "+", "var")
REGEX_TEST_EXTRACTING_FROM_EXPRESSION(extraction_spaced_expression, "[       var   +     3     ]", "var", "+", "3")

// Check the same as below but with 2 variable
REGEX_TEST_EXTRACTING_FROM_EXPRESSION(extraction_double_var_expression, "[var + tmp]", "var", "+", "tmp")
REGEX_TEST_EXTRACTING_FROM_EXPRESSION(extraction_double_var_expression_spaced, "[      var  -   tmp        ]", "var", "-", "tmp")