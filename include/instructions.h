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


	extern std::unordered_map<std::string, parameter_data> instructions;

}

#endif