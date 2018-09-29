/*
	Enhanced assembler for cardiac system.
	Designed to be practical and simpler to use.
	by zeFresk
*/

#include "opt.h"

#include <iostream>
#include <fstream>
#include <boost/program_options/errors.hpp>

int main(int argc, char* argv[])
{
	try {
		auto opts = parse_args(argc, argv);
		check_args(opts);
		if (opts.assemble)
		{
			for (std::size_t i = 0; i < opts.in.size(); ++i)
			{
				std::ostream* out;
				std::ofstream out_file;
				if (opts.no_file)
				{
					out = &std::cout;
				}
				else if (i >= opts.out.size()) // no user provided output file
				{
					std::string filename = get_filename(opts.in[i]);
					out_file.open(filename + ".cbin", std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
					out = &out_file;
				}
				else if (i < opts.out.size()) // user provided a custom name for this file
				{
					out_file.open(opts.out[i], std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
					out = &out_file;
				}
				else { throw std::runtime_error{ "How ?" }; }
			}
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}