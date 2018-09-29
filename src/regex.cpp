/*
	definitions of regexes, should save compile time
*/

#include "regex.h"

namespace _impl {

	std::regex separate_line{ separate_line_str, flags };

	std::regex parameters_regex{ parameters_str, flags };
}