/*
	Core of assembler
	Functions used to transform CASM to semi-binary
*/

#include <cstdint>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>

using integer = std::uint16_t;

struct line {
	integer corresponding_line;
	std::string label;
	std::string instruction;
	std::string parameters;
	std::string comment;
};

struct semi_assembled_line {
	std::string instruction;
	integer parameter;
};

// Convert cardiac assembly from text to semi-binary
// Return an array of <address, data>
std::vector<std::pair<integer, integer>> assemble(std::vector<std::string> const& asm, bool verbose = false);

// Extract data from raw string
// Will throw if syntax error
line parse_line(std::string const& str);

// Evaluate mathematical expressions in parameters
// and replace label with their value
semi_assembled_line evaluate_line(line const& l, std::unordered_map<std::string, integer> const& labels_map);

//helper functions
integer evaluate_parameter(std::string const& p, std::unordered_map<std::string, integer> const& map);
integer convert_operand(std::string const& op, std::unordered_map<std::string, integer> const& labels_map, line const& l);

// Check if a line doesn't contain any error
// Will throw if it is the case
void check_line(line const& l, std::string const& str);

// Each of the following function check
// for specific errors
void check_label(line const& l, std::string const& original_line);
void check_instruction(line const& l, std::string const& original_line);
void check_parameters(line const& l, std::string const& original_line);
void check_comment(line const& l, std::string const& original_line);