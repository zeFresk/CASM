#include "serialize.h"

#include <boost/endian/conversion.hpp>

void serialize_as_txt(std::vector<std::pair<integer, integer>> const& in, integer start, std::ostream& out)
{
	out << start << "\n";
	for (auto &e : in)
		out << e.first << ": " << e.second << "\n";
}


/*
	Optimization can be done here !
*/
void serialize_as_binary(std::vector<std::pair<integer, integer>> const & in, integer start, std::ostream & out)
{
	out.write(reinterpret_cast<const char*>(boost::endian::native_to_little(start)), sizeof(integer));
	for (auto &e : in)
	{
		integer tmp_l_value; // we need a lvalue to serialize

		tmp_l_value = boost::endian::native_to_little(e.first);
		out.write(reinterpret_cast<const char*>(&tmp_l_value), sizeof(integer));

		tmp_l_value = boost::endian::native_to_little(e.second);
		out.write(reinterpret_cast<const char*>(&tmp_l_value), sizeof(integer));
	}
}