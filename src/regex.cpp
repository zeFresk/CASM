/*
	definitions of regexes, should save compile time
*/

#include "regex.h"

namespace _impl {

	std::regex separate_line{ separate_line_str, flags };

	std::regex parameters_regex{ parameters_str, flags };

	std::regex double_regex{ double_digits_only, flags };

	std::regex triple_regex{ triple_digits_only, flags };

	std::regex just_a_number{ just_a_number_str, flags };

	std::regex is_expression{ is_expression_str, flags };

	std::regex evaluate_regex{ evaluate_regex_str, flags };

}