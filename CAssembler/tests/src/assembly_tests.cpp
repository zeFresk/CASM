#include <gtest/gtest.h>

#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

#include "assembler.h"
#include "tools.h"
#include "serialize.h"

class AssemblyTests : public ::testing::Test {};

/*
	To simplify testing the assembly source code for each test will be located in tests/asm/TEST_NAME.casm
	and the expected output will be in tests/asm/TEST_NAME_expected.txt
*/

/*##
####  Tests that *FORMARTED* assembly is the same as expected
##*/
std::vector<std::string> load_from_stream(std::istream& is)
{
	std::vector<std::string> ret;
	std::string tmp;
	while (std::getline(is, tmp)) {
		ret.push_back(std::move(tmp));
	}
	return ret;
}


/*
	Ugly macro, to use simply create the two files mentioned above and use the macro
*/
#define ASSEMBLY_FMT_TEST_FILE(TEST_NAME) TEST_F(AssemblyTests, TEST_NAME) {\
	auto asm_str = load_from_file(std::string{"asm/"} + #TEST_NAME + std::string{ ".casm" });\
	bint start_id{0, 2, true};\
	auto out = assemble(asm_str, start_id);\
	\
	std::stringstream out_stream;\
	serialize_as_txt(out, start_id, out_stream);\
	auto fmt_out = load_from_stream(out_stream);\
	\
	auto expected_out = load_from_file(std::string{"asm/"} + #TEST_NAME + std::string{ "_expected.txt" });\
	EXPECT_TRUE(std::equal(std::begin(fmt_out), std::end(fmt_out), std::begin(expected_out))) << "Assembly output for " << #TEST_NAME << " doesn't match expected output.";\
}\

// Check if most basic example is correctly assembled (no complex instructions other than .start)
ASSEMBLY_FMT_TEST_FILE(most_basic)

// Check if all basic instructions work
ASSEMBLY_FMT_TEST_FILE(all_basic_instructions)

// Check if it works with variables
ASSEMBLY_FMT_TEST_FILE(basic_variables)

// Check if ptr variables work
ASSEMBLY_FMT_TEST_FILE(ptr_variables)

// Check if label without instructions but pointing on line below works
ASSEMBLY_FMT_TEST_FILE(no_instructions_labels)
