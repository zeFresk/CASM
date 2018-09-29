#include "tools.h"

#include <algorithm>
#include <cassert>
#include <fstream>

std::string get_filename(std::string const& str)
{
	assert(!str.empty() && "Error: filename is empty");

	auto it = std::find(std::rbegin(str), std::rend(str), '.');
	if (it != std::rend(str)) // file contain an extension
	{
		return std::string{ std::begin(str), (it + 1).base() };
	}
	else
	{
		return str;
	}
}

std::vector<std::string> load_from_file(std::string const& filename)
{
	ifstream file{ filename };
	if (!file)
		throw std::runtime_error{ std::string{"Unable to open " + filename}.c_str() };

	std::vector<std::string> ret;
	ret.reserve(100); // to speed-up things
	std::string tmp;
	while (std::getline(file, tmp))
		ret.push_back(std::move(tmp));

	return ret;
}