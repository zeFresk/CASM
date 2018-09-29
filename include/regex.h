/*
	Every regex are stocked here
*/

#include <regex>

std::regex_constants::syntax_option_type flags = std::regex_constants::ECMAScript;

// https://regex101.com/r/eeEgLy/1/
auto separate_line_str = R"((\w*?):?\s?([A-Z]+?|\.\w+)\s+(\w+|\[.*?\])\s*;?\s*(\w*)$)";
extern std::regex separate_line;