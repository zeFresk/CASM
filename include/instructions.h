/*
	All existing instructions and their parameters
*/
#ifndef instruction_h
#define instructions_h

#include "regex.h"
#include "assembler.h"

#include <unordered_map>
#include <string>
#include <limits>

namespace _impl {

	constexpr integer special_id = std::numeric_limits<integer>::max();

	struct parameter_data {
		integer id;
		std::regex reg_params;
	};

	std::unordered_map<std::string, parameter_data> instructions = {
	{"INP", { 0, inp_params}},
	{"OUT", { 1, out_params}},
	{"LDA", { 2, lda_params}},
	{ "LDI",{ 3, lda_params } },
	{"STA", { 4, sta_params}},
	{ "STI", { 5, sti_params} },
	{ "ADD",{ 6, add_params }},
	{ "SUB",{ 7,  sub_params }},
	{ "JAZ",{ 8, jaz_params }},
	{ "HRS",{ 9, hrs_params }},
	{ ".AT", {special_id, at_params}},
	{ ".WORD",{ special_id, word_params } },
	{ ".START",{ special_id, at_params } },
	};

}

#endif