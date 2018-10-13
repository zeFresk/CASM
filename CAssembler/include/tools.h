/*
Collection of usefull standalone functions
*/

#ifndef tools_h
#define tools_h

#include <vector>
#include <string>
#include <iostream>

// Retrieve filename without extension
// If no extension return as it is
// Undefined behaviour if str is empty
std::string get_filename(std::string const& str);

// Extract data from a file efficiently
// Return and array of lines
// Throw if unable to open file
std::vector<std::string> load_from_file(std::string const& filename);

template <typename T>
void unused(T const&) {}

// Print everything concatened if verbose is true
// At least one argument beside the first one must be passed
template <typename... Args>
void v_log(bool verbose, Args&& ...args)
{
	if (verbose)
	{
		int dummy[sizeof...(args)] = { (std::cout << args, 0)... };
		unused(dummy); // dummy var is necessary but we don't want any warning
	}
}

#endif
