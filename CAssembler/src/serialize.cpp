#include "serialize.h"
#include "bounded_integer.h"
#include "assembler.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cassert>

#include <boost/endian/conversion.hpp>


void serialize_as_binary(std::ostream& out_buffer, bounded_integer_b10 const& bint)
{
	if (bint.maximum_ < std::numeric_limits<std::uint8_t>::max()) // value can be serialized as 8 bits  value
	{
		uint8_t endian_corrected = boost::endian::native_to_little(static_cast<uint8_t>(bint.value_));
		out_buffer.write(reinterpret_cast<const char*>(&endian_corrected), sizeof(uint8_t));
	}
	else if (bint.maximum_ < std::numeric_limits<std::uint16_t>::max()) // value can be serialized as 16 bits  value
	{
		uint16_t endian_corrected = boost::endian::native_to_little(static_cast<uint16_t>(bint.value_));
		out_buffer.write(reinterpret_cast<const char*>(&endian_corrected), sizeof(uint16_t));
	}
	else if (bint.maximum_ < std::numeric_limits<std::uint32_t>::max()) // value can be serialized as 32 bits  value
	{
		uint32_t endian_corrected = boost::endian::native_to_little(static_cast<uint32_t>(bint.value_));
		out_buffer.write(reinterpret_cast<const char*>(&endian_corrected), sizeof(uint32_t));
	}
	else if (bint.maximum_ < std::numeric_limits<std::uint64_t>::max()) // value can be serialized as 64 bits  value
	{
		uint64_t endian_corrected = boost::endian::native_to_little(static_cast<uint64_t>(bint.value_));
		out_buffer.write(reinterpret_cast<const char*>(&endian_corrected), sizeof(uint64_t));
	}
	else // WTF ? > 64 bits ???
	{
		throw std::out_of_range{"can't serialize value as binary : too big"};
	}
}


void serialize_as_txt(std::ostream& out_buffer, bounded_integer_b10 const& bint)
{
	out_buffer << std::setfill('0') << std::setw(bint.nb_digits_) << bint.value_;
}

void serialize_as_txt(assembled_data const& in, bint const& start, std::ostream& out)
{
	serialize_as_txt(out, start);
	out << "\n";
	for (auto &e : in)
	{
		serialize_as_txt(out, e.first);
		out << ": ";
		serialize_as_txt(out, e.second);
		out << "\n";
	}
}


/*
	Optimization can be done here !
*/
void serialize_as_binary(assembled_data const & in, bint const& start, std::ostream & out)
{
	if (in[0].first.digits() >= 256 && in[0].second.digits() >= 256) //overflow ?
		throw std::overflow_error{"can't serialize more than 255 digits"};

	// header
	const char header[] = "CB"; //header of the file
	uint8_t address_digits = boost::endian::native_to_little(static_cast<uint8_t>(in[0].first.digits()));

	out.write(header, sizeof(header));
	out.write(reinterpret_cast<char*>(&address_digits), sizeof(address_digits));
	serialize_as_binary(out, start);

	// body
	for (auto &e : in)
	{
		serialize_as_binary(out, e.first); // address serialization
		serialize_as_binary(out, e.second); // instruction serialization
	}
}
