/*
	Every regex are stocked here
*/
#ifndef regex_h
#define regex_h

#include <regex>

namespace _impl {

	// https://regex101.com/r/eeEgLy/9
	constexpr auto separate_line_str = R"(^\s*(?:(?:(\w*?)\s*:)?\s*(?:([a-zA-Z]+?|\.\w+)\s+(\w+|\[.*?\])\s*)?)?(?:;\s*(.*)\s*|()\s*)$)";
	extern std::regex separate_line;

	// https://regex101.com/r/RHHBET/1
	constexpr auto parameters_str = R"(^(:?\D\w*|0?\d{2}\D*|\[\s*\w*\s*[+-]\s*\w*\s*\])$)";
	extern std::regex parameters_regex;

	// For the moment all the parameters can be the same
	#define inp_params parameters_regex
	#define out_params parameters_regex
	#define lda_params parameters_regex
	#define ldi_params parameters_regex
	#define sta_params parameters_regex
	#define sti_params parameters_regex
	#define add_params parameters_regex
	#define sub_params parameters_regex
	#define jaz_params parameters_regex
	#define hrs_params parameters_regex
	#define word_params parameters_regex

	constexpr auto double_digits_only = R"(^(?:0?\d{2}\D*)$)";
	extern std::regex double_regex;

	#define at_params double_regex

	// Not used for the moment
	//constexpr auto triple_digits_only = R"(^(?:0?\d{3}\D*)$)"; 
	//extern std::regex triple_regex;

	constexpr auto is_expression_str = R"(^\[\s*\w*\s*[+-]\s*\w*\s*\]$)";
	extern std::regex is_expression;

	constexpr auto evaluate_regex_str = R"(^\[\s*(\w*)\s*([+-])\s*(\w*)\s*\]$)";
	extern std::regex evaluate_regex;
}

#endif
