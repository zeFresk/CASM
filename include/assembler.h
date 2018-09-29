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

line parse_line(std::string const& str);