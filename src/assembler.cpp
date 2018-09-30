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
#include <map>
#include <unordered_map>

using namespace _impl;

std::vector<std::pair<integer, integer>> assemble(std::vector<std::string> const& asm_data, bool verbose)
{
	//Parsing a first time to separate labels, instructions, parameters and comments
	v_log(verbose, "->Parsing");
	std::vector<line> parsed_asm;
	parsed_asm.reserve(asm_data.size());

	std::size_t errors = 0;

	integer i = 1;
	for (auto &e : asm_data) // parsing everything once
	{
		try {
			parsed_asm.push_back(std::move(parse_line(i, e)));
		}
		catch (base_asm_error const& err)
		{
			++errors;
			std::cout << "! in line " << err.what() << std::endl;
		}
		++i;
	}

	//make instruction case-insensitive
	for (auto &e : parsed_asm)
		std::transform(std::begin(e.instruction), std::end(e.instruction), std::begin(e.instruction), [](unsigned char c) {return std::toupper(c); });

	v_log(verbose, "->Checking for advanced syntax error");
	//check for more complicated errors
	for (auto &e : parsed_asm)
	{
		try {
			check_line(e, asm_data[e.corresponding_line]);
		}
		catch (base_asm_error const& err) {
			++errors;
			std::cout << "! in line " << err.what() << std::endl;
		}
	}

	if (errors > 0)
		throw assembly_failed{ errors };

	// ordering the lines
	v_log(verbose, "->Creating structure");
	std::unordered_map<std::string, integer> labels_map; // will contain each position associated with a label
	std::map<integer, line> lines_map; // will contain every line at the correct place
	integer start_id = special_id;

	integer current_id = 20;

	for (auto &e : parsed_asm)
	{
		if (e.instruction == ".AT") // ordering instruction
		{
			current_id = static_cast<integer>(std::stoul(e.parameters));
		}
		else if (e.instruction == ".START") // start instruction
		{
			if (start_id != special_id) // start already set ???
				throw asm_logic_error{ e.corresponding_line, asm_data[e.corresponding_line], "error: two \".start\" found" };

			current_id = static_cast<integer>(std::stoul(e.parameters));
			start_id = current_id;
		}
		else
		{
			if (e.label != "") // this line has a label
			{
				if (/*std::find(std::begin(labels_map), std::end(labels_map), e.label)*/labels_map.find(e.label) != std::end(labels_map)) // label already defined
					throw asm_logic_error{ e.corresponding_line, asm_data[e.corresponding_line], std::string{"error: label already defined ["} +e.label + std::string{"]"} };

				labels_map[e.label] = current_id;
			}
			lines_map[current_id] = e; // copy the current line at the right place
			++current_id;
		}

		if (current_id >= 100) //overflow
			throw asm_overflow{ e.corresponding_line, asm_data[e.corresponding_line], std::string{ "error: overflow id=" } + std::to_string(current_id) };
	}

	// evaluating parameters
	v_log(verbose, "->Resolving labels and evaluating parameters");
	std::map<integer, semi_assembled_line> semi_assembled_map;

	for (auto &e : lines_map)
	{
		semi_assembled_map[e.first] = evaluate_line(e.second, labels_map);
	}

	// assembling
	v_log(verbose, "->Assembling");
	std::vector<std::pair<integer, integer>> ret{ semi_assembled_map.size() };

	std::size_t j = 0;
	for (auto &e : semi_assembled_map)
	{
		ret[j] = { e.first, (Instructions::get().at(e.second.instruction)).id * 100 + e.second.parameter };
	}

	return ret;

}

semi_assembled_line evaluate_line(line const& l, std::unordered_map<std::string, integer> const& labels_map)
{
	semi_assembled_line ret;
	ret.instruction = l.instruction;

	if (std::regex_match(l.parameters, just_a_number)) // parameters contains just a number
	{
		ret.parameter = static_cast<integer>(std::stoul(l.parameters));
	}
	else if (evaluate_parameter(l.parameters, labels_map) != special_id) // parameters contains only a label which exists in BDD
	{
		ret.parameter = evaluate_parameter(l.parameters, labels_map);
	}
	else if (std::regex_match(l.parameters, is_expression)) // parameters is an expression
	{
		std::smatch match;
		std::regex_match(l.parameters, match, evaluate_regex); // 1 = lhs, 2 = op, 3 = rhs
		
		std::string lhs = match[1].str();
		std::string rhs = match[3].str();
		std::string op = match[2].str();

		integer left = convert_operand(lhs, labels_map, l);
		integer right = convert_operand(rhs, labels_map, l);

		if (op == std::string{ "-" }) // sub
		{
			ret.parameter = static_cast<integer>(left - right);
		}
		else // add
		{
			ret.parameter = static_cast<integer>(left + right);
		}
	}
	else { throw syntax_error{ l.corresponding_line, l.parameters }; }

	return ret;
}

integer convert_operand(std::string const& op, std::unordered_map<std::string, integer> const& labels_map, line const& l)
{
	integer ret;

	if (std::regex_match(op, just_a_number)) // lhs is just a number
	{
		ret = static_cast<integer>(std::stoul(op));
	}
	else // lhs must be a label
	{
		ret = evaluate_parameter(op, labels_map);
		if (ret == special_id) // lhs isn't a number or a known label
			asm_logic_error{ l.corresponding_line, l.parameters, std::string{ "error: Unknown label [" } + op + std::string{ "]" } };
	}

	return ret;
}

integer evaluate_parameter(std::string const& p, std::unordered_map<std::string, integer> const& map)
{
	integer ret = special_id;

	if (/*std::find(std::begin(map), std::end(map), p)*/map.find(p) != std::end(map)) // parameters contains only a label which exists in BDD
	{
		ret = map.at(p);
	}
	
	return ret;
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
	check_label(l, str);
	check_instruction(l, str);
	check_parameters(l, str);
	check_comment(l, str);
}

void check_label(line const&, std::string const&)
{
	// nothing to check in labels for the moment
}

void check_instruction(line const& l, std::string const& original_line)
{
	if (/*std::find(std::begin((Instructions::get())), std::end((Instructions::get())), l.instruction)*/(Instructions::get()).find(l.instruction) == std::end((Instructions::get()))) // instruction not found
		throw syntax_error{ l.corresponding_line, original_line, std::string{"error: unknown instruction ["} +l.instruction + std::string{"]"} };
}

void check_parameters(line const& l, std::string const& original_line)
{
	if (!std::regex_match(l.parameters, static_cast<std::regex&>((Instructions::get())[l.instruction].reg_params))) // parameters is incorrect
		throw syntax_error{ l.corresponding_line, original_line, std::string{"error: parameters are incorrect ["} +l.parameters + std::string{"]"} };
}

void check_comment(line const&, std::string const&)
{
	//nothing to check for the moment
}