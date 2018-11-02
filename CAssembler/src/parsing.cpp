#include "parsing.h"
#include "regexs.h"

#include <cctype>
#include <regex>
#include <cassert>

bool is_strictly_numeric(std::string const& str)
{
	for (char const& c : str)
	{
		if (!std::isdigit(c)) // at least one character is not a digit
			return false;
	}
	return true;
}

bool is_expression(std::string const& str)
{
	return std::regex_match(str, _impl::is_expression); // Maybe a speed-up is possible by switching to homemade function ?
}

bool is_valid_line(std::string const& str)
{
	return std::regex_match(str, _impl::separate_line); // Really hard to make a manual check
}

expression split_expression(std::string const& str)
{
	assert(is_expression(str) && "string is not an expression");

	// [0] = full match
	// [1] = lhs
	// [2] = operand
	// [3] = rhs
	std::smatch matches;

	std::regex_match(str, matches, _impl::evaluate_regex);
	return {matches[1].str(), matches[2].str(), matches[3].str()};
}

raw_line split_line(std::string const& str)
{
	assert(is_valid_line(str) && "string is not a valid line");

	// [0] = full match
	// [1] = label
	// [2] = instruction
	// [3] = parameters
	// [4] = comment
	std::smatch matches;

	std::regex_match(str, matches, _impl::separate_line);
	return {matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()};
}
