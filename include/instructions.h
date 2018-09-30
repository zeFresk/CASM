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

	using map_type = std::unordered_map<std::string, parameter_data>;

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