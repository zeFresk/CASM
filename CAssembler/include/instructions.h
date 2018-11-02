/*
	All existing instructions and their parameters
*/
#ifndef instruction_h
#define instructions_h

#include "regexs.h"
#include "assembler.h"

#include <unordered_map>
#include <string>
#include <limits>
#include <regex>
#include <cstdint>

using opcode = std::uint8_t;

// Functions we will use

// retrieve opcode given instruction name
// instruction must be correct !
opcode instruction_opcode(std::string const& instruction_name);

// tells if parameter is compatible with given instruction
bool is_valid_instruction_parameter(std::string const& instruction_name, std::string const& parameter);

// tells if instruction is a valid one
bool is_valid_instruction(std::string const& instruction_name);


// We will not have to go here !
namespace _impl {

	constexpr opcode special_op = std::numeric_limits<opcode>::max();
	constexpr integer special_id = std::numeric_limits<integer>::max();

	struct parameter_data {
		opcode id;
		std::regex reg_params;
	};

	using map_type = std::unordered_map<std::string, parameter_data>;

	// Singleton which map every instruction to its id and correct parameters
	class Instructions
	{
	public:
		// Return all instructions and initialize them if not already done
		static Instructions& get()
		{
			static Instructions instance;
			return instance;
		}

		// typedefs
		using mapped_type = map_type::mapped_type;
		using key_type = map_type::key_type;
		using iterator = map_type::iterator;
		using const_iterator = map_type::const_iterator;

		// Redirecting to unordered_map access functions
		mapped_type& operator[](key_type const& key);
		mapped_type& at(key_type const& key);
		mapped_type const& at(key_type const& key) const;

		// Redirecting to unordered_map iterator related functions
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;

		iterator end() noexcept;
		const_iterator end() const noexcept;
		const_iterator cend() const noexcept;

		// Redirecting to unordered_map find
		iterator find(const key_type& key);
		const_iterator find(const key_type& key) const;

	private:
		// real map of instructions
		std::unordered_map<std::string, parameter_data> instructions_;

		Instructions();

		// Create the inside map
		void create_map();

		~Instructions() = default;
		Instructions(const Instructions&) = delete;
		Instructions& operator=(const Instructions&) = delete;
	};

}

#endif
