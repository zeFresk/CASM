#include "instructions.h"

#include <cassert>

opcode instruction_opcode(std::string const& instruction_name)
{
	assert(is_valid_instruction(instruction_name) && "Invalid instruction : can't retrieve opcode");
	
	return _impl::Instructions::get()[instruction_name].id;
}


bool is_valid_instruction_parameter(std::string const& instruction_name, std::string const& parameter)
{
	assert(is_valid_instruction(instruction_name) && "Invalid instruction : can't check parameters");
	
	auto checker = _impl::Instructions::get()[instruction_name].reg_params;
	return std::regex_match(parameter, checker);
}


bool is_valid_instruction(std::string const& instruction_name)
{
	return _impl::Instructions::get().find(instruction_name) != _impl::Instructions::get().end();
}

namespace _impl {

	Instructions::Instructions()
	{
		create_map();
	}

	void Instructions::create_map()
	{
		instructions_["INP"] = { 0, inp_params };
		instructions_["OUT"] = { 1, out_params };
		instructions_["LDA"] = { 2, lda_params };
		instructions_["LDI"] = { 3, lda_params };
		instructions_["STA"] = { 4, sta_params };
		instructions_["STI"] = { 5, sti_params };
		instructions_["ADD"] = { 6, add_params };
		instructions_["SUB"] = { 7, sub_params };
		instructions_["JAZ"] = { 8, jaz_params };
		instructions_["HRS"] = { 9, hrs_params };
		instructions_[".AT"] = { special_op, at_params };
		instructions_[".WORD"] = { 0, word_params };
		instructions_[".START"] = { special_op, at_params };
	}

	Instructions::mapped_type& Instructions::operator[](Instructions::key_type const& key) { return instructions_[key]; }
	Instructions::mapped_type& Instructions::at(Instructions::key_type const& key) { return instructions_.at(key); }
	Instructions::mapped_type const& Instructions::at(Instructions::key_type const& key) const { return instructions_.at(key); }

	// Redirecting to unordered_map iterator related functions
	Instructions::iterator Instructions::begin() noexcept { return instructions_.begin(); }
	Instructions::const_iterator Instructions::begin() const noexcept { return instructions_.begin(); }
	Instructions::const_iterator Instructions::cbegin() const noexcept { return instructions_.cbegin(); }

	Instructions::iterator Instructions::end() noexcept { return instructions_.end(); }
	Instructions::const_iterator Instructions::end() const noexcept { return instructions_.end(); }
	Instructions::const_iterator Instructions::cend() const noexcept { return instructions_.cend(); }

	// Redirecting to unordered_map find
	Instructions::iterator Instructions::find(const Instructions::key_type& key) { return instructions_.find(key); }
	Instructions::const_iterator Instructions::find(const Instructions::key_type& key) const { return instructions_.find(key); }

}
