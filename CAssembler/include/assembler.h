/*
	Core of assembler
	Functions used to transform CASM to semi-binary
*/
#ifndef assembler_h
#define assembler_h

#include "bounded_integer.h"

#include <cstdint>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>
#include <map>

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
	bint parameter;
};


// #########################
// ######## Aliases ########
// #########################

using address = bint;
using instruction = bint;
using parameter = bint;

using assembled_data = std::vector<std::pair<address, instruction>>;
using splitted_lines = std::vector<splitted_raw_line>;
using ordered_lines_map_t = std::vector<std::pair<address, splitted_raw_line>>;
using labels_map_t = std::unordered_map<std::string, address>;
using half_assembled_lines = std::map<address, half_resolved_line>;

// #########################
// ## Assembler functions ##
// #########################

// Transform code as string to a binary form
// Return an array of <address, data>
// start_id will be overwrite to the value representing the starting point of the executable
// An exception will be thrown, if at least one error occurs during assembly
assembled_data assemble(std::vector<std::string> const& asm_data, address& start_id, bool verbose = false);

// Split all lines of code and check for errors
// nb_errors value will increase by number of errors which happened during this stage
splitted_lines split_and_parse(std::vector<std::string> const& raw_lines, std::size_t& nb_errors);

// Associate each line of code to its final address in the future binary
// nb_errors value will increase by number of errors which happened during this stage
// strt will be replaced with entry point of the future program
ordered_lines_map_t associate_lines_to_addresses(std::vector<splitted_raw_line> const& splitted_asm, address& strt, std::size_t& nb_errors);

// Associate each label to its corresponding line number in a map
labels_map_t associate_labels(ordered_lines_map_t const& splitted_asm, std::size_t& nb_errors);

// Resolve every parameter whereas it is an expression, a label or a number and returns a map of asl lines containing only instructions and resolved parameter
half_assembled_lines resolve_parameters(ordered_lines_map_t const& ordered_lines_map, labels_map_t const& labels_map, std::size_t& nb_errors);

// Fully assemble every line to its final asm form. The line must not require any interpretation anymore.
assembled_data assemble_resolved_lines(half_assembled_lines const& parameters_resolved_map);

// Check for more complicated syntax errors and warnings
// nb_errors value will increase by number of errors which happened during this stage
void advanced_checks(splitted_lines const& splitted_asm, std::size_t& nb_errors);

// Extract data from raw string
// Will throw if syntax error
splitted_raw_line parse_line(std::size_t id, std::string const& str);

// Evaluate mathematical expressions in parameters
// and replace label with their value
half_resolved_line evaluate_line(splitted_raw_line const& l, labels_map_t const& labels_map);

//helper functions

// resolve a given parameter label
// if the label doesn't exists returns a special value
parameter resolve_parameter(std::string const& p, labels_map_t const& map);

// resolve an operand as if it was a parameter, converts it if numeric or resolve it if label
// if the operand is a label AND the label doesn't exists throw an exception
bint resolve_operand(std::string const& op,labels_map_t const& labels_map,  splitted_raw_line const& l);

// compute a pre-splitted expression given all the labels known
// will throw if the expression contains unknown labels
bint compute_expression(expression const& exp, labels_map_t const& labels_map, splitted_raw_line const& l);

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
