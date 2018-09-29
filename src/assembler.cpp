#include "assembler.h"
#include "tools.h"
#include "asm_error.h"
#include "regex.h"
#include "instructions.h"

#include <algorithm>
#include <cctype> // toupper
#include <regex>
#include <cassert>
#include <iostream>

using namespace _impl;

std::vector<std::pair<integer, integer>> assemble(std::vector<std::string> const& asm, bool verbose = false)
{
	//Parsing a first time to separate labels, instructions, parameters and comments
	std::vector<line> parsed_asm;
	parsed.reserve(asm.size());

	std::size_t errors = 0

	integer i = 1;
	for (auto &e : asm) // parsing everything once
	{
		try {
			parsed_asm.push_back(std::move(parse_line(i, e)));
		}
		catch (base_asm_error const& err)
		{
			++errors;
			std::cout << "! in line " << err.what() << std::endl;
		}
		++i
	}

	//make instruction case-insensitive
	for (auto &e : parsed_asm)
		std::transform(std::begin(e.instruction), std::end(e.instruction), std::begin(e.instruction), [](unsigned char c) {return std::toupper(c); });

	//check for more complicated errors
	for (auto &e : parsed_asm)
	{
		try {
			check_line(e.corresponding_line,e, asm[e.corresponding_line]);
		}
		catch (base_asm_error const& err) {
			++errors;
			std::cout << "! in line " << err.what() << std::endl;
		}
	}

	if (errors > 0)
		throw assembly_failed{ errors };

}

line parse_line(integer id, std::string const& str)
{
	line ret{ id, "", "", "", "" };
	if (str == "") // empty
		return { id, "", "", "", "" };

	std::smatch match;
	if (!std::regex_match(str, match, separate_line)) // Error : the line isn't empty and it doesn't match normal syntax
	{
		throw syntax_error{ id,str };
	}
	else // no basic syntax error
	{
		//storing matches
		assert(match.size() == 5);
		ret.corresponding_line = id;
		ret.label = match[1].str();
		ret.instruction = match[2].str();
		ret.parameters = match[3].str();
		ret.comment = match[4].str();
	}
	return ret;
}

void check_line(line const& l, std::string const& str)
{
	check_label(l.label, str);
	check_instruction(l.instruction, str);
	check_parameters(l.parameters, str);
	check_comment(l.comment, str)
}

void check_label(line const& l, std::string const& original_line)
{
	// nothing to check in labels for the moment
}

void check_instruction(line const& l, std::string const& original_line)
{
	if (std::find(std::begin(instructions), std::end(instructions), l.instruction) == std::end(instructions)) // instruction not found
		throw syntax_error{ l.corresponding_line, original_line, "error: unknown instruction [" + l.instruction + std::string{"]"} };
}

void check_parameters(line const& l, std::string const& original_line)
{
	if (!std::regex_match(l.parameters, instructions[l.instruction])) // parameters is incorrect
		throw syntax_error{ l.corresponding_line, original_line, "error: parameters are incorrect [" + l.parameters + std::string{"]"} };
}

void check_comment(line const& l, std::string const& original_line)
{
	//nothing to check for the moment
}