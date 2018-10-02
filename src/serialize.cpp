#include "serialize.h"

#include <iostream>
#include <iomanip>

#include <boost/endian/conversion.hpp>

void serialize_as_txt(std::vector<std::pair<integer, integer>> const& in, integer start, std::ostream& out)
{
	out << start << "\n";
	for (auto &e : in)
	{
		out << std::setfill('0') << std::setw(2) << e.first;
		out << ": ";
		out << std::setfill('0') << std::setw(3) << e.second << "\n";
	}
}


/*
	Optimization can be done here !
*/
void serialize_as_binary(std::vector<std::pair<integer, integer>> const & in, integer start, std::ostream & out)
{
	integer start_tmp = boost::endian::native_to_little(start);
	out.write(reinterpret_cast<const char*>(&start_tmp), sizeof(integer));
	for (auto &e : in)
	{
		integer tmp_l_value; // we need a lvalue to serialize

		tmp_l_value = boost::endian::native_to_little(e.first);
		out.write(reinterpret_cast<const char*>(&tmp_l_value), sizeof(integer));

		tmp_l_value = boost::endian::native_to_little(e.second);
		out.write(reinterpret_cast<const char*>(&tmp_l_value), sizeof(integer));
	}
}