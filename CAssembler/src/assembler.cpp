#include "assembler.h"
#include "tools.h"
#include "asm_error.h"
#include "regexs.h"
#include "instructions.h"
#include "parsing.h"

#include <algorithm>
#include <cctype> // toupper
#include <regex>
#include <cassert>
#include <iostream>
#include <map>
#include <unordered_map>



//###################################################################
//################## Assembly main functions#########################
//###################################################################

std::vector<std::pair<integer, integer>> assemble(std::vector<std::string> const& raw_asm, integer& strt, bool verbose)
{
	//Parsing a first time to separate labels, instructions, parameters and comments
	v_log(verbose, "->Parsing\n");

	// variable which will contains the total number of build errors
	std::size_t errors = 0;

	auto splitted_asm = split_and_parse(raw_asm, errors);
	
	//make instruction case-insensitive by setting them in uppercase
	for (auto &e : splitted_asm)
		std::transform(std::begin(e.instruction), std::end(e.instruction), std::begin(e.instruction), [](unsigned char c) {return std::toupper(c); });

	v_log(verbose, "->Checking for advanced syntax error\n");
	//check for more complicated errors and warnings
	advanced_checks(splitted_asm, errors);
	
	v_log(verbose, "->Creating structure\n");
	// first we associate each line to its address
	auto ordered_map = associate_lines_to_addresses(splitted_asm, strt, errors);

	v_log(verbose, "->Associating labels\n");
	// then we associate each label to the line it represents
	auto labels_map = associate_labels(ordered_map, errors);
	// delete lines with no instructions i.e label-only lines
	ordered_map.erase(std::remove_if(std::begin(ordered_map), std::end(ordered_map), [](std::pair<integer, splitted_raw_line> const& p){return p.second.instruction == "";}), std::end(ordered_map));
	
	// evaluating parameters
	v_log(verbose, "->Resolving and evaluating parameters\n");
	auto parameters_resolved_map = resolve_parameters(ordered_map, labels_map, errors);
	
	// Stop here if at least one error occured
	if (errors > 0)
		throw assembly_failed{ errors };

	// assembling
	v_log(verbose, "->Assembling\n");
	auto assembled_data = assemble_resolved_lines(parameters_resolved_map);
	
	return assembled_data;

}

//###################################################################
//################### Assembly sub-functions ########################
//###################################################################

std::vector<splitted_raw_line> split_and_parse(std::vector<std::string> const& raw_lines, std::size_t& nb_errors)
{
	std::vector<splitted_raw_line> splitted_code;
	splitted_code.reserve(raw_lines.size()); // to speed up push_back
	integer i = 0;

	for (auto &e : raw_lines) // parse every line of code
	{
		try {
			splitted_code.push_back(parse_line(i, e));
		}
		catch (base_asm_error const& err)
		{
			++nb_errors;
			std::cout << "! in line " << err.what() << std::endl;
		}
		++i;
	}
	return splitted_code;
}

void advanced_checks(std::vector<splitted_raw_line> const& splitted_asm, std::size_t& nb_errors)
{
	for (auto &e : splitted_asm)
	{
		try {
			check_line(e, e.original_line);
		}
		catch (base_asm_error const& err) {
			++nb_errors;
			std::cout << "! in line " << err.what() << std::endl;
		}
	}
}

std::vector<std::pair<integer, splitted_raw_line>> associate_lines_to_addresses(std::vector<splitted_raw_line> const& splitted_asm, integer& strt, std::size_t& nb_errors)
{
	std::vector<std::pair<integer, splitted_raw_line>> ordered_map; // associate each line to its future address

	integer start_id = _impl::special_id; // magic value
	integer current_id = 20; // default starting id will be twenty if not specified

	for (auto &e : splitted_asm)
	{
		try {
			if (e.instruction == ".AT") // ordering instruction
			{
				current_id = static_cast<integer>(std::stoul(e.parameters));
			}
			else if (e.instruction == ".START") // start instruction
			{
				if (start_id != _impl::special_id) // start already set ???
					throw asm_logic_error{ e.original_line_number, e.original_line, "error: two \".start\" found" };

				current_id = static_cast<integer>(std::stoul(e.parameters));
				start_id = current_id;
			}
			else if (e.instruction != "" || e.label != "")
			{
				ordered_map.push_back({current_id, e}); // copy the current line at the right place
				if (e.instruction != "") // We don't want to increase id if the line contained only a label 
					++current_id;
			}

			if (current_id >= 100) //overflow
				throw asm_overflow{ e.original_line_number, e.original_line, std::string{ "error: overflow id=" } +std::to_string(current_id) };
		}
		catch (base_asm_error const& err) {
			++nb_errors;
			std::cout << "! in line " << err.what() << std::endl;
		}
	}

	// Checking if one .start has been defined
	try {
		if (start_id == _impl::special_id) // no start defined
			throw asm_logic_error{ 0, "", "error: no .start found" };
		strt = start_id;
	}
	catch (base_asm_error const& err) {
		++nb_errors;
		std::cout << "! in line " << err.what() << std::endl;
	}

	return ordered_map;
}

std::unordered_map<std::string, integer> associate_labels(std::vector<std::pair<integer, splitted_raw_line>> const& splitted_asm, std::size_t& nb_errors)
{
	std::unordered_map<std::string, integer> labels_map;
	integer id;
	splitted_raw_line line;

	for (auto &pair : splitted_asm)
	{
		try {
			std::tie(id, line) = pair;

			if (line.label != "") // this line has a label
			{
				if (labels_map.find(line.label) != std::end(labels_map)) // label already defined
					throw asm_logic_error{ line.original_line_number, line.original_line, std::string{"error: label already defined ["} + line.label + std::string{"]"} };
				

				labels_map[line.label] = id;
			}
		}
		catch (base_asm_error const& err) {
				++nb_errors;
				std::cout << "! in line " << err.what() << std::endl;
			}
	}
	return labels_map;
}


std::map<integer, half_resolved_line> resolve_parameters(std::vector<std::pair<integer, splitted_raw_line>> const& ordered_lines_map, std::unordered_map<std::string, integer> const& labels_map, std::size_t& nb_errors)
{
	std::map<integer, half_resolved_line> semi_assembled_map;

	for (auto &e : ordered_lines_map)
	{
		try {
			semi_assembled_map[e.first] = evaluate_line(e.second, labels_map);
		}
		catch (base_asm_error const& err) {
			++nb_errors;
			std::cout << "! in line " << err.what() << std::endl;
		}
	}
	return semi_assembled_map;
}

std::vector<std::pair<integer,integer>> assemble_resolved_lines(std::map<integer, half_resolved_line> const& parameters_resolved_map)
{
	std::vector<std::pair<integer, integer>> ret{ parameters_resolved_map.size() };

	std::size_t j = 0;
	for (auto &e : parameters_resolved_map)
	{
		ret[j] = { e.first, (_impl::Instructions::get().at(e.second.instruction)).id * 100 + e.second.parameter };
		++j;
	}
	return ret;
}

//###################################################################
//################# Line specialized functions ######################
//###################################################################
half_resolved_line evaluate_line(splitted_raw_line const& l, std::unordered_map<std::string, integer> const& labels_map)
{
	half_resolved_line ret;
	ret.instruction = l.instruction;

	if (is_strictly_numeric(l.parameters)) // parameters contains just a number
	{
		ret.parameter = static_cast<integer>(std::stoul(l.parameters));
	}
	else if (resolve_parameter(l.parameters, labels_map) != _impl::special_id) // parameters contains only a label which exists in BDD
	{
		ret.parameter = resolve_parameter(l.parameters, labels_map);
	}
	else if (is_expression(l.parameters)) // parameters is an expression
	{
		auto splitted_expression = split_expression(l.parameters);
		ret.parameter = compute_expression(splitted_expression, labels_map, l);	
	}
	else // parameter is an unknown label
	{ 
		throw asm_logic_error{ l.original_line_number, l.parameters, "error: unknown label" }; 
	}

	return ret;
}

integer compute_expression(expression const& exp, std::unordered_map<std::string, integer> const& labels_map, splitted_raw_line const& l)
{
	integer lhs = resolve_operand(exp.lhs, labels_map, l);
	integer rhs = resolve_operand(exp.rhs, labels_map, l);

	if (exp.operand == std::string{ "-" }) // sub
	{
		return static_cast<integer>(lhs - rhs);
	}
	else // add
	{
		return static_cast<integer>(lhs + rhs);
	}
}


integer resolve_operand(std::string const& op, std::unordered_map<std::string, integer> const& labels_map, splitted_raw_line const& l)
{
	integer ret;

	if (is_strictly_numeric(op)) // lhs is just a number
	{
		ret = static_cast<integer>(std::stoul(op));
	}
	else // lhs must be a label
	{
		ret = resolve_parameter(op, labels_map);
		if (ret == _impl::special_id) // lhs isn't a number or a known label
			asm_logic_error{ l.original_line_number, l.parameters, std::string{ "error: Unknown label [" } + op + std::string{ "]" } };
	}

	return ret;
}

integer resolve_parameter(std::string const& p, std::unordered_map<std::string, integer> const& map)
{
	integer ret = _impl::special_id;

	if (map.find(p) != std::end(map)) // parameters contains only a label which exists in BDD
	{
		ret = map.at(p);
	}
	
	return ret;
}

splitted_raw_line parse_line(integer id, std::string const& str)
{
	splitted_raw_line ret{ id, str, "", "", "", "" };
	if (str == std::string{""}) // empty
		return { id, str, "", "", "", "" };

	std::smatch match;
	if (!is_valid_line(str)) // Error : the line isn't empty and it doesn't match normal syntax
	{
		throw syntax_error{ id,str };
	}
	else // no basic syntax error
	{
		auto splitted = split_line(str);

		ret.original_line_number = id;
		ret.original_line = str;
		ret.label = splitted.label;
		ret.instruction = splitted.instruction;
		ret.parameters = splitted.parameters;
		ret.comment = splitted.comment;
	}
	return ret;
}


//###################################################################
//############### Specialized Checking functions ####################
//###################################################################

void check_line(splitted_raw_line const& l, std::string const& str)
{
	check_label(l, str);
	check_instruction(l, str);
	check_parameters(l, str);
	check_comment(l, str);
}

void check_label(splitted_raw_line const&, std::string const&)
{
	// nothing to check in labels for the moment
}

void check_instruction(splitted_raw_line const& l, std::string const& original_line)
{
	if (l.instruction != std::string{ "" }) {
		if ((_impl::Instructions::get()).find(l.instruction) == std::end((_impl::Instructions::get()))) // instruction not found
			throw syntax_error{ l.original_line_number, original_line, std::string{"error: unknown instruction ["} +l.instruction + std::string{"]"} };
	}
}

void check_parameters(splitted_raw_line const& l, std::string const& original_line)
{
	if (l.parameters != std::string{ "" }) {
		if (!std::regex_match(l.parameters, static_cast<std::regex&>((_impl::Instructions::get()).at(l.instruction).reg_params))) // parameters is incorrect
			throw syntax_error{ l.original_line_number, original_line, std::string{"error: parameters are incorrect ["} +l.parameters + std::string{"]"} };
	}
}

void check_comment(splitted_raw_line const&, std::string const&)
{
	//nothing to check for the moment
}
