/*
	definitions of regexes, should save compile time
*/

#include "regexs.h"

namespace _impl {

	std::regex separate_line{ separate_line_str};

	std::regex parameters_regex{ parameters_str};

	std::regex double_regex{ double_digits_only };

	//std::regex triple_regex{ triple_digits_only}; // not used

	std::regex is_expression{ is_expression_str };

	std::regex evaluate_regex{ evaluate_regex_str };

}
