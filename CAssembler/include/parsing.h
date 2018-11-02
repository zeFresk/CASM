#ifndef parsing_h
#define parsing_h

#include <string>

//##########################
//### Booleans functions ###
//##########################

// Tells if a given string composed only of a number
bool is_strictly_numeric(std::string const& str);

// Tells if a given string is a cardiac expression
bool is_expression(std::string const& str);

// Tells if a given string is a valid asm line
bool is_valid_line(std::string const& str);

//##########################
//## Extraction functions ##
//##########################

// data structure which contains raw expression but splitted correctly
struct expression {
	std::string lhs;
	std::string operand;
	std::string rhs;
};

// data structure which contains a line after it has been splitted
struct raw_line {
	std::string label;
	std::string instruction;
	std::string parameters;
	std::string comment;
};

// split an expression according into its components
expression split_expression(std::string const& str);

raw_line split_line(std::string const& str);


#endif
