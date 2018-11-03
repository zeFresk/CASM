#ifndef bounded_integer_h
#define bounded_integer_h

#include "CMake_config.h" // We need to now internal type

#include <cstdint>
#include <iosfwd>
#include <limits>


//###############################
//###### Types definitions ######
//###############################
class bounded_integer_b10;

// We need to define which type to use internally
#if BITS == 64
	using internal_t = std::int64_t;
#else
	using internal_t = std::int32_t;
#endif

using bint = bounded_integer_b10;

// return a^b
internal_t pow(internal_t a, std::size_t b);

// core class which will automatically check for overflow/underflow and serialize to the minimal size possible
class bounded_integer_b10
{
	private:
		internal_t value_;
		std::size_t nb_digits_;
		internal_t maximum_; // could be calculated each time but we will gain time here
		bool check_;
		internal_t minimum_;

		// Check for overflow/underflow
		bool OOB() const noexcept;

		// throw if OOB
		void OOB_throw() const;


	public:

		using underlying_t = internal_t;
		// ### Constructors ###
		
		// constructor : - nb_digits10 is the number of digits for this integer in base 10
		//               - check_for_overflow : if set an exception will be thrown if an overflow or underflow occurs.
		//               - minimum_value : minimum value before underflow
		bounded_integer_b10(internal_t value = 0, std::size_t nb_digits10 = 3, bool check_for_overflow = true, internal_t minimum_value = 0);

		// we want to be able to affect directly after definition
		bounded_integer_b10& operator=(internal_t rhs);

		// return number of digits of this bounded integer
		std::size_t digits() const;
		// Return underkying value, be cautious with this !
		internal_t value() const;

		// copy digits, oob checks settings, min and max from other bounded_integer
		void copy_properties(bounded_integer_b10 const& other);

		// all others constructors are trivials
		~bounded_integer_b10()=default;
		bounded_integer_b10(bounded_integer_b10 const&)=default;
		bounded_integer_b10(bounded_integer_b10 &&)=default;
		bounded_integer_b10& operator=(bounded_integer_b10 const&)=default;
		bounded_integer_b10& operator=(bounded_integer_b10 &&)=default;

		// ### mathematics operators ###

		// pre-incrementation
		bounded_integer_b10& operator++();
		// post-incrementation
		bounded_integer_b10 operator++(int);
		// pre-decrementation
		bounded_integer_b10& operator--();
		// post-decrementation
		bounded_integer_b10 operator--(int);

		// binary +
		bounded_integer_b10& operator+=(bounded_integer_b10 const& rhs);
		// binary -
		bounded_integer_b10& operator-=(bounded_integer_b10 const& rhs);
		// binary *
		bounded_integer_b10& operator*=(bounded_integer_b10 const& rhs);
		
		// + - and * are outside of the class
		
		// comparison operators are outside too
		friend bool operator==(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs);
		friend bool operator<(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs);

		// serialization functions, defined in serialize.cpp
		friend void serialize_as_binary(std::ostream& out_buffer, bounded_integer_b10 const& binteger);
		friend void serialize_as_txt(std::ostream& out_buffer, bounded_integer_b10 const& binteger);

};
// ### free operators ###

// +-*
bounded_integer_b10 operator+(bounded_integer_b10 lhs, bounded_integer_b10 const& rhs);
bounded_integer_b10 operator-(bounded_integer_b10 lhs, bounded_integer_b10 const& rhs);
bounded_integer_b10 operator*(bounded_integer_b10 lhs, bounded_integer_b10 const& rhs);

// comparison
bool operator!=(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs);
bool operator>(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs);
bool operator<=(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs);
bool operator>=(bounded_integer_b10 const& lhs, bounded_integer_b10 const& rhs);

// magic value
const bint error_value{std::numeric_limits<internal_t>::min(), std::numeric_limits<internal_t>::digits10, false, std::numeric_limits<internal_t>::min()};

#endif
