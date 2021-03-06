/*
	All the functions used to save as file
	(take care of endianness
*/

#ifndef serialize_h
#define serilize_h

#include "assembler.h"
#include "bounded_integer.h"

#include <iostream>

/*
	Format of a txt file:

	START
	ADDRESS: DATA
	ADDRESS: DATA
	...

	Everything is a number
*/

// Serialize an array of address|instruction to a text file.
void serialize_as_txt(assembled_data const& in, bint const& start, std::ostream& out);

/*
	Format of a cardiac binary :
	START|ADDRESS|DATA|ADDRESS|DATA|ADDRESS...

	Everything is encoded on sizeof(integer) and in little-endian
*/
void serialize_as_binary(assembled_data const& in, bint const& start, std::ostream& out);

#endif
