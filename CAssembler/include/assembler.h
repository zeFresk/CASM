/*
	Core of assembler
	Functions used to transform CASM to semi-binary
*/
#ifndef assembler_h
#define assembler_h

#include <cstdint>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>
#include <map>

using integer = std::uint16_t;

// forward declarations
struct expression;

// #########################
// #### Data structures ####
// #########################

// non proccesed asm line but splitted according to asm standard
struct splitted_raw_line {
	// number of the line in the raw asm which corresponds to this splitted one
	std::size_t original_line_number;
	// original line of code as string
	std::string original_line;

	// raw string corresponding to each category, "" if none, a line can be composed of nothing except a comment
	std::string label;
	std::string instruction;
	std::string parameters;
	std::string comment;
};

struct half_resolved_line {
	std::string instruction;
	integer parameter;
};

// #########################
// ## Assembler functions ##
// #########################

// Transform code as string to a binary form
// Return an array of <address, data>
// start_id will be overwrite to the value representing the starting point of the executable
// An exception will be thrown, if at least one error occurs during assembly
std::vector<std::pair<integer, integer>> assemble(std::vector<std::string> const& asm_data, integer& start_id, bool verbose = false);

// Split all lines of code and check for errors
// nb_errors value will increase by number of errors which happened during this stage
std::vector<splitted_raw_line> split_and_parse(std::vector<std::string> const& raw_lines, std::size_t& nb_errors);

// Associate each line of code to its final address in the future binary
// nb_errors value will increase by number of errors which happened during this stage
// strt will be replaced with entry point of the future program
std::vector<std::pair<integer, splitted_raw_line>> associate_lines_to_addresses(std::vector<splitted_raw_line> const& splitted_asm, integer& strt, std::size_t& nb_errors);

// Associate each label to its corresponding line number in a map
std::unordered_map<std::string, integer> associate_labels(std::vector<std::pair<integer, splitted_raw_line>> const& splitted_asm, std::size_t& nb_errors);

// Resolve every parameter whereas it is an expression, a label or a number and returns a map of asl lines containing only instructions and resolved parameter
std::map<integer, half_resolved_line> resolve_parameters(std::vector<std::pair<integer, splitted_raw_line>> const& ordered_lines_map, std::unordered_map<std::string, integer> const& labels_map, std::size_t& nb_errors);

// Fully assemble every line to its final asm form. The line must not require any interpretation anymore.
std::vector<std::pair<integer,integer>> assemble_resolved_lines(std::map<integer, half_resolved_line> const& parameters_resolved_map);

// Check for more complicated syntax errors and warnings
// nb_errors value will increase by number of errors which happened during this stage
void advanced_checks(std::vector<splitted_raw_line> const& splitted_asm, std::size_t& nb_errors);

// Extract data from raw string
// Will throw if syntax error
splitted_raw_line parse_line(integer id, std::string const& str);

// Evaluate mathematical expressions in parameters
// and replace label with their value
half_resolved_line evaluate_line( splitted_raw_line const& l, std::unordered_map<std::string, integer> const& labels_map);

//helper functions

// resolve a given parameter label
// if the label doesn't exists returns a special value
integer resolve_parameter(std::string const& p, std::unordered_map<std::string, integer> const& map);

// resolve an operand as if it was a parameter, converts it if numeric or resolve it if label
// if the operand is a label AND the label doesn't exists throw an exception
integer resolve_operand(std::string const& op, std::unordered_map<std::string, integer> const& labels_map,  splitted_raw_line const& l);

// compute a pre-splitted expression given all the labels known
// will throw if the expression contains unknown labels
integer compute_expression(expression const& exp, std::unordered_map<std::string, integer> const& labels_map, splitted_raw_line const& l);

//###################################################################
//############### Specialized Checking functions ####################
//###################################################################

// Check if a line doesn't contain any error
// Will throw if it is the case
void check_line( splitted_raw_line const& l, std::string const& str);

// Each of the following function check
// for specific errors
void check_label( splitted_raw_line const& l, std::string const& original_line);
void check_instruction( splitted_raw_line const& l, std::string const& original_line);
void check_parameters( splitted_raw_line const& l, std::string const& original_line);
void check_comment( splitted_raw_line const& l, std::string const& original_line);

#endif
