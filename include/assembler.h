/*
	Core of assembler
	Functions used to transform CASM to semi-binary
*/

#include <cstdint>
#include <vector>
#include <utility>
#include <string>

using integer = std::uint16_t;

struct line {
	integer corresponding_line;
	std::string label;
	std::string instruction;
	std::string parameters;
	std::string comment;
};

// Convert cardiac assembly from text to semi-binary
// Return an array of <address, data>
std::vector<std::pair<integer, integer>> assemble(std::vector<std::string> const& asm, bool verbose = false);

// Extract data from raw string
// Will throw if syntax error
line parse_line(std::string const& str);

// Check if a line doesn't contain any error
// Will throw if it is the case
void check_line(line const& l, std::string const& str);

// Each of the following function check
// for specific errors
void check_label(line const& l, std::string const& original_line);
void check_instruction(line const& l, std::string const& original_line);
void check_parameters(line const& l, std::string const& original_line);
void check_comment(line const& l, std::string const& original_line);