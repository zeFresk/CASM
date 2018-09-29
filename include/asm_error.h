/*
	Set of different exceptions to throw if an error is detected during assembly phase
*/

#include <stdexcept>
#include <string>

// Base class for all errors 
class base_asm_error : public std::logic_error {
public:
	base_asm_error(const std::string& xwhat) : std::logic_error(xwhat) {}
};

class assembly_failed : public base_asm_error {
	assembly_failed(std::size_t nb_errors) :
		base_asm_error{ std::string{"Assembly failed with "} +std::to_string(nb_errors) + std::string{" errors.\n"} }
	{}
};

class asm_error : public base_asm_error {
	asm_error(std::size_t line, std::string const& line_str, std::string const& msg = "") : 
		base_asm_error{ std::to_string(line) + std::string{": "} + msg + std::string{"\n>>>>"} + line_str + std::string{"\n"} }
	{}
};

class syntax_error : public asm_error {
	syntax_error(std::size_t line, std::string const& line_str, std::string const& msg = "error: syntax error") :
		asm_error{line, line_str, msg}
	{}
};