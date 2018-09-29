/*
	Every regex are stocked here
*/

#include <regex>

namespace _impl {

	std::regex_constants::syntax_option_type flags = std::regex_constants::ECMAScript;

	// https://regex101.com/r/eeEgLy/5
	constexpr auto separate_line_str = R"(^\s*(\w*?):?\s?([A-Z]+?|\.\w+)\s+(\w+|\[.*?\])\s*(?:;\s*(.*)\s*|()\s*)$)";
	extern std::regex separate_line;

	// https://regex101.com/r/RHHBET/1
	constexpr auto parameters_str = R"(^(:?\D\w*|0?\d{2}\D*|\[\s*\w*\s*[+-]\s*\w*\s*\])$)";
	extern std::regex parameters_regex;

	// For the moment all the parameters can be the same
	using inp_params = parameters_regex;
	using out_params = parameters_regex;
	using lda_params = parameters_regex;
	using ldi_params = parameters_regex;
	using sta_params = parameters_regex;
	using sti_params = parameters_regex;
	using add_params = parameters_regex;
	using sub_params = parameters_regex;
	using jaz_params = parameters_regex;
	using hrs_params = parameters_regex;

	constexpr auto double_digits_only = R"(^(?:0?\d{2}\D*)$)";
	extern std::regex double_regex;

	using at_params = double_regex;

	constexpr auto triple_digits_only = R"(^(?:0?\d{3}\D*)$)";
	extern std::regex triple_regex;

	using word_params = triple_regex;

	constexpr auto just_a_number_str = R"(^\d+$)";
	extern std::regex just_a_number;

	constexpr auto is_expression_str = R"(^\[\s*\w*\s*[+-]\s*\w*\s*\]$)";
	extern std::regex is_expression;

	constexpr auto evaluate_regex_str = R"(^\[\s*(\w*)\s*([+-])\s*(\w*)\s*\]$)";
	extern std::regex evaluate_regex;
}