#include "opt.h"
#include "CMake_config.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Make sure args are valids
// else throw an exception
void check_args(options const& opt)
{
	if (opt.in.empty() && opt.assemble)
	{
		throw po::error("Error: No input file provided, try --help.");
	}
}

void version()
{
	std::cout << "CAssembler (x" << BITS <<" bits) version " << MAJOR_VERSION << "." << MINOR_VERSION << "\n"
		<< "Compiled with " << COMPILER_ID << " v" << COMPILER_VERSION << "\n"
		/*<< "CXX_FLAGS=" << CXX_FLAGS */<< std::endl;
}

options parse_args(int argc, char* argv[])
{
	options ret; //to fill and return
	ret.assemble = true;
	std::string config_file;

	// Declare a group of options that will be 
	// allowed only on command line
	po::options_description gen("Generic options");
	gen.add_options()
		("version", "print this program version along with usefull informations")
		("help", "produce help message")
		("config,c", po::value<std::string>(&config_file),"path to configuration file")
		;

	// Declare a group of options that will be 
	// allowed both on command line and in
	// config file
	po::options_description config("Configuration");
	config.add_options()
		("verbose,v","print more information during assembly process")
		("human-readable,h", "print output as text rather than binary")
		("file,f", po::value<std::vector<std::string>>(&ret.out)->composing(), "output file(s), default is ${input_filename}.cbin")
		("disable-check", "disable out of range checks during assembly.")
		("address-size,d", po::value<std::size_t>(&ret.address_digits)->default_value(3), "number of digits allowed per address (base 10)")
		("no-file", "print output binary instead of saving it")
		;

	// Hidden options, will be allowed both on command line and
	// in config file, but will not be shown to the user.
	po::options_description hidden("Hidden options");
	hidden.add_options()
		("input-file", po::value<std::vector<std::string>>(&ret.in)->composing(), "input file(s)")
		;


	po::options_description cmdline_options;
	cmdline_options.add(gen).add(config).add(hidden);

	po::options_description config_file_options;
	config_file_options.add(config).add(hidden);

	po::options_description visible("Allowed options");
	visible.add(gen).add(config);

	//set first parameter to be input file
	po::positional_options_description p;
	p.add("input-file", -1);

	//parsing args
	po::variables_map vm;
	store(po::command_line_parser(argc, argv).
		options(cmdline_options).positional(p).run(), vm);
	notify(vm);

	if (vm.count("config")) {
		std::ifstream ifs(config_file);
		if (!ifs)
		{
			throw std::runtime_error("Error: unable to open config file");
		}
		else
		{
			//parsing config file
			store(parse_config_file(ifs, config_file_options), vm);
			notify(vm);
		}
	}

	if (vm.count("help")) {
		ret.assemble = false; //no assembly to do
		std::cout << visible << std::endl;
		return ret;
	}

	if (vm.count("version")) {
		ret.assemble = false; //no assembly to do
		version();
		return ret;
	}

	ret.no_file = vm.count("no-file");

	ret.verbose = vm.count("verbose");

	ret.check = !(vm.count("disable-check"));

	ret.binary_file = !vm.count("human-readable");


	return ret;
}
