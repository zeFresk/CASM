#include "assembler.h"
#include "tools.h"

#include <algorithm>
#include <cctype> // toupper

std::vector<std::pair<integer, integer>> assemble(std::vector<std::string> const& asm, bool verbose = false)
{
	//Parsing a first time to separate labels, instructions, parameters and comments
	std::vector<line> parsed_asm;
	parsed.reserve(asm.size());

	integer i = 1;
	for (auto &e : asm) // parsing everything once
	{
			parsed_asm.push_back(std::move(parse_line(i,e)));
			++i;
	}

	//make instruction case-insensitive
	for (auto &e : parsed_asm)
		std::transform(std::begin(e.instruction), std::end(e.instruction), std::begin(e.instruction), [](unsigned char c) {return std::toupper(c); });


}

line parse_line(std::string const& str)
{
	if (str == "") // empty
		return { id, "", "", "", "" };


}