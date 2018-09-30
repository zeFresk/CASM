/*
	Enhanced assembler for cardiac system.
	Designed to be practical and simpler to use.
	by zeFresk
*/

#include "opt.h"
#include "tools.h"
#include "assembler.h"
#include "serialize.h"
#include "asm_error.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	try {
		auto opts = parse_args(argc, argv);
		check_args(opts);
		if (opts.assemble)
		{
			for (std::size_t i = 0; i < opts.in.size(); ++i)
			{
				// Opening output file for writing
				// We have to be carefull because of nofile option
				std::ostream* out;
				std::ofstream out_file;
				std::string filename;

				if (opts.no_file) // output is set to stdout
				{
					out = &std::cout;
				}
				else // We write in a file
				{
					std::string base_filename = (i >= opts.out.size()) ? get_filename(opts.in[i]) : opts.out[i]; // user hasn't provided a name ?
					filename = base_filename + (opts.binary_file ? ".cbin" : ".txt"); // is output a binary file ?

					out_file.open(filename, (opts.binary_file ? std::ofstream::binary : std::ofstream::out) | std::ofstream::trunc);
					out = &out_file;
				}

				// Extracting assembler from input file
				auto input_data = load_from_file(opts.in[i]);
				v_log(opts.verbose, "Assembling ", opts.in[i], "\n");
				integer start_id;
				decltype(assemble(input_data, start_id, opts.verbose)) out_data;

				try {
					out_data = assemble(input_data, start_id, opts.verbose);
				}
				catch (base_asm_error const& err) {
					throw base_asm_error{ std::string{"in "} +filename + std::string{": "} + std::string{err.what()} }; // Stop program and forward error
				}

				// Saving assembly
				if (opts.binary_file) //We want the file to be a binary
				{
					serialize_as_binary(out_data, start_id, *out);
				}
				else
				{
					serialize_as_txt(out_data, start_id, *out);
				}
			}
		}
	}
	catch (assembly_failed const& err){
		std::cout << ">>> " << err.what() << std::endl;
		std::cout << "Exiting...";
	}
	catch (std::exception const& e){
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}