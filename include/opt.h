#ifndef opt_h
#define opt_h

#include <string>
#include <vector>

struct options
{
	bool binary_file;
	bool no_file;
	bool verbose;
	bool assemble; //do we need to assemble ?
	std::vector<std::string> out;
	std::vector<std::string> in;
};

options parse_args(int argc, char* argv[]);
void check_args(options const& opt);

#endif