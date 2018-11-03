#include "bounded_integer.h"

#include <cassert>
#include <limits>
#include <stdexcept>
#include <iostream>

// Needed to speed up calculation
// raise a to power b (integers)
internal_t pow(internal_t a, std::size_t b)
{
	internal_t ret = 1;

	for (std::size_t i = 0; i < b; ++i)
		ret *= a;

	return ret;
}

// ##################################
// ####### Members defintions #######
// ##################################

//ctor
bounded_integer_b10::bounded_integer_b10(internal_t value, std::size_t nb_digits10, bool check_for_overflow, internal_t minimum_value) 
	: value_{value}, nb_digits_{nb_digits10}, maximum_{static_cast<internal_t>(pow(10, nb_digits10))}, check_{check_for_overflow}, minimum_{minimum_value}
{
	assert(! OOB() && "Value entered is out of range");
	assert(std::numeric_limits<internal_t>::digits10 < nb_digits10 && "bounded integer : can't store that much digits in an integer");

	OOB_throw();
}

// assignation
bounded_integer_b10& bounded_integer_b10::operator=(internal_t rhs)
{
	assert(! OOB() && "Value assigned is out of range");
	OOB_throw();

	value_ = rhs;
	return *this;
}

//OOB checks
bool bounded_integer_b10::OOB() const noexcept
{
	return (value_ > maximum_) || (value_ < minimum_);
}

void bounded_integer_b10::OOB_throw() const
{
	if (check_ && OOB())
		throw std::out_of_range{"bounded_integer : value out of range"};
}

// pre-incrementation
bounded_integer_b10& bounded_integer_b10::operator++()
{
	++value_;

	assert(! OOB() && "value overflow after incrementation");
	OOB_throw();

	return *this;
}

// access functions
std::size_t bounded_integer_b10::digits() const
{
	return nb_digits_;
}

//access to stored value
internal_t bounded_integer_b10::value() const
{
	return value_;
}

void bounded_integer_b10::copy_properties(bounded_integer_b10 const& other)
{
	nb_digits_ = other.nb_digits_;
	minimum_ = other.minimum_;
	maximum_ = other.maximum_;
	check_ = other.check_;
}

// post-incrementation
bounded_integer_b10 bounded_integer_b10::operator++(int)
{
	bounded_integer_b10 tmp(*this); //copy
	operator++();

	assert(! OOB() && "value overflow after incrementation");
	OOB_throw();

	return tmp;
}

// pre-decrementation
bounded_integer_b10& bounded_integer_b10::operator--()
{
	--value_;
	
	assert(! OOB() && "value overflow after decrementation");
	OOB_throw();

	return *this;
}

// post-decrementation
bounded_integer_b10 bounded_integer_b10::operator--(int)
{
	bounded_integer_b10 tmp(*this);
	operator--();

	assert(! OOB() && "value overflow after decrementation");
	OOB_throw();

	return tmp;
}



// binary +
bounded_integer_b10& bounded_integer_b10::operator+=(bounded_integer_b10 const& rhs)
{
	value_ += rhs.value_;

	assert(! OOB() && "value overflow after addition");
	OOB_throw();

	return *this;
}

// binary -
bounded_integer_b10& bounded_integer_b10::operator-=(bounded_integer_b10 const& rhs)
{
	value_ -= rhs.value_;

	assert(! OOB() && "value underflow after substraction");
	OOB_throw();

	return *this;
}

// binary *
bounded_integer_b10& bounded_integer_b10::operator*=(bounded_integer_b10 const& rhs)
{
	value_ *= rhs.value_;

	assert(! OOB() && "value overflow after multiplication");
	OOB_throw();

	return *this;
}

// comparison operators are outside the class
bool operator==(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs)
{
	return lhs.value_ == rhs.value_;
}

bool operator<(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs)
{
	return lhs.value_ < rhs.value_;
}

// ### free operators ###

// +-*
bounded_integer_b10 operator+(bounded_integer_b10 lhs, bounded_integer_b10 const& rhs)
{
	lhs += rhs;
	return lhs;
}

bounded_integer_b10 operator-(bounded_integer_b10 lhs, bounded_integer_b10 const& rhs)
{
	lhs -= rhs;
	return lhs;
}

bounded_integer_b10 operator*(bounded_integer_b10 lhs, bounded_integer_b10 const& rhs)
{
	lhs *= rhs;
	return lhs;
}

// comparison
bool operator!=(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs) { return ! (lhs == rhs); }
bool operator>(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs) { return rhs < lhs; }
bool operator<=(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs) { return ! (lhs > rhs); }
bool operator>=(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs) { return ! (lhs < rhs); }

// serialization defined in serialize.cpp
