#pragma once

#include <aarith/integer.hpp>
#include <cstdint>
#include <tuple>

//
// This file is for prototypes and forward declarations of the posit
// implementation. Listing all functions and methods here makes it easier for
// them to call each other.
//
// Eventually we will have to look at how this can be circumvented with
// modules. But for now the focus is understanding posit arithmetic.
//

namespace aarith {

//
// Posit Class and Methods.
//
// For implementations, look at posit/posit.hpp.
//

/**
 * @brief A posit that represents a real number.
 *
 * Represents a posit number that can represent real values. Based on
 * the description by John L. Gustafson ("Posit Arithmetic", 10
 * October 2017.)
 *
 * @tparam N The total width in bits of the given posit.
 * @tparam ES The maximum width in bits of the exponent for a given posit.
 * @tparam WS The underlying word type used to store the posit.
 */
template <size_t N, size_t ES, typename WT = uint64_t> class posit
{
public:
    /**
     * Underlying type used to store the actual bits of a
     * given posit.
     */
    using storage_type = uinteger<N, WT>;

    /**
     * Type used to store the fragments of actual bits of a given posit.  This
     * information is required by some cast operators.
     */
    using word_type = WT;

    /**
     * Type returned when the useed constant is requested. As useed can get
     * quite large, we do not use standard C types such as size_t here.
     */
    using useed_type = uinteger<((1 << (1 << ES)) + 1), WT>;

    //
    // Factory Methods
    //

    /**
     * @brief Construct a new posit with given bits.
     *
     * Argument bits is not interpreted as an integer, rather it is used as
     * the underlying bit pattern of the returned posit.
     */
    static constexpr posit from_bits(const storage_type& bits);

    //
    // Constructors
    //

    /**
     * @brief Construct this posit initialized to zero.
     */
    constexpr posit();

    /**
     * @brief Construct this posit to be a clone of other.
     *
     * @param other The posit to clone.
     */
    constexpr posit(const posit& other);

    /**
     * @brief Construct this posit to be a clone of other.
     *
     * @param other The posit to clone.
     */
    constexpr posit(const posit&& other);

    /**
     * @brief Assign this posit to hold the value of other.
     *
     * @param The value to change to.
     */
    posit& operator=(const posit& other);

    /**
     * @brief Assign this posit to hold the value of other.
     *
     * @param The value to change to.
     */
    posit& operator=(const posit&& other);

    /**
     * @brief Construct this posit with given bits.
     *
     * @param n Bits used to initialize the underlying type.
     */
    // TODO (Schärtl) das funktioniert aber nur gut, so lange N <= 64!
    constexpr explicit posit(WT n);

    //
    // Comparison Operators
    //

    /**
     * @brief Compare this and other for equality.
     *
     * Two posits are equal if they share the same exact bit patterns.
     * In particular, NaR compares equal to itself.
     *
     * @return true if this and other represent the same value. Otherwise
     * returns true.
     */
    [[nodiscard]] constexpr bool operator==(const posit& other) const;

    /**
     * @brief Compare this and other for inequality.
     *
     * Two posits are not equal if they have different bit patterns.
     *
     * @return true if this and other represent different values. Otherwise
     * returns false.
     */
    [[nodiscard]] constexpr bool operator!=(const posit& other) const;

    /**
     * @brief Return whether this is less than other.
     *
     * If this or other represent NaR, this method returns false.
     */
    [[nodiscard]] constexpr bool operator<(const posit& other) const;

    /**
     * @brief Return whether this is less than or equal to other.
     *
     * If either this or other represent NaR, this method returns
     * false. If both this and other represent NaR, this method
     * returns true.
     */
    [[nodiscard]] constexpr bool operator<=(const posit& other) const;

    /**
     * @brief Return whether this is greater than other.
     *
     * If this or other represent NaR, this method returns false.
     */
    [[nodiscard]] constexpr bool operator>(const posit& other) const;

    /**
     * @brief Return whether this is greater than or equal to other.
     *
     * If either this or other represent NaR, this method returns
     * false. If both this and other represent NaR, this method
     * returns true.
     */
    [[nodiscard]] constexpr bool operator>=(const posit& other) const;

    //
    // Arithmetic Operators
    //

    /**
     * @brief Unary plus.
     *
     * Returns the posit as-is. Sometimes it can be useful to be explicit
     * about the sign of variables or literals. In these cases, an unary plus
     * is useful.
     *
     * If this posit represents NaR, the result is also NaR.
     */
    [[nodiscard]] constexpr posit& operator+() const;

    /**
     * @brief Sum of two posits.
     *
     * If either operand is NaR, the result is also NaR. If neither operands
     * are NaR, the result is guaranteed not to be NaR.
     *
     * @param rhs The posit to add to this posit.
     * @return The sum of this and rhs.
     */
    [[nodiscard]] constexpr posit operator+(const posit& rhs) const;

    /**
     * @brief Add rhs to this.
     *
     * @param rhs The posit to add to this posit.
     */
    [[nodiscard]] constexpr posit& operator+=(const posit& rhs);

    /**
     * @brief Pre-Increment.
     *
     * @return A reference to the incremented value.
     */
    [[nodiscard]] constexpr posit& operator++();

    /**
     * @brief Post-Increment.
     *
     * @return A copy of the value before it was incremented.
     */
    [[nodiscard]] constexpr posit operator++(int);

    /**
     * @brief Unary minus.
     *
     * If this posit represents NaR, the result is also NaR.
     */
    [[nodiscard]] constexpr posit operator-() const;

    /**
     * @brief Sum of two posits.
     *
     * If either operand is NaR, the result is also NaR. If neither operands
     * are NaR, the result is guaranteed not to be NaR.
     *
     * As posits only have one unique representation of zero, calling this
     * operator on a posit that represents zero again returns zero.
     *
     * @param rhs The posit to add to this posit.
     * @return The sum of this and rhs.
     */
    [[nodiscard]] constexpr posit operator-(const posit& rhs) const;

    /**
     * @brief Subtract rhs from this.
     *
     * @param rhs The posit to subtract.
     */
    [[nodiscard]] constexpr posit& operator-=(const posit& rhs);

    /**
     * @brief Pre-Decrement
     *
     * @return A reference to the decremented value.
     */
    [[nodiscard]] constexpr posit& operator--();

    /**
     * @brief Post-Decrement.
     *
     * @return A copy of the value before it was decremented.
     */
    [[nodiscard]] constexpr posit operator--(int);

    /**
     * @brief Return the product of this multiplied with rhs.
     */
    [[nodiscard]] constexpr posit operator*(const posit& rhs) const;

    /**
     * @brief Set this to this multiplied with rhs.
     */
    [[nodiscard]] constexpr posit& operator*=(const posit& rhs);

    //
    // Constants
    //

    /**
     * @brief Return width of the given posit type.
     */
    [[nodiscard]] static constexpr size_t width();

    /**
     * @brief Return the maximum exponent size of the given posit type.
     */
    [[nodiscard]] static constexpr size_t exponent_size();

    /**
     * Return the minimum value of this posit type. The minimum is the
     * negative posit that has the greatest magnitude.
     *
     * @return The smallest representable posit that represents a real number.
     */
    [[nodiscard]] static constexpr posit min();

    /**
     * Return the smallest representable positive value that is not zero.
     *
     * @return Smallest positive non-zero posit that represents a real number.
     */
    [[nodiscard]] static constexpr posit minpos();

    /**
     * Return the maximum value of this posit type. The maximum is the
     * positive posit that has the greatest magnitude.
     *
     * @return The biggest representable posit that represents a real number.
     */
    [[nodiscard]] static constexpr posit max();

    /**
     * @return Representation of the real number zero.
     */
    [[nodiscard]] static constexpr posit zero();

    /**
     * @return Representation of the real number one.
     */
    [[nodiscard]] static constexpr posit one();

    /**
     * @return Representation of NaR.
     */
    [[nodiscard]] static constexpr posit nar();

    //
    // Posit-Specific Constants
    //

    /**
     * @brief Return the npat for this type as defined in unum literature.
     *
     * @return The number of possible bit patterns this type can take.
     */
    [[nodiscard]] static constexpr uinteger<N + 1, WT> npat();

    /**
     * @brief Return the useed for this type as used in official unum
     * literature.
     *
     * @ The useed value, which is 2 to the power of 2 to the power of the
     * exponent size.
     */
    [[nodiscard]] static constexpr useed_type useed();

    //
    // Getters
    //

    /**
     * @return Whether this posit represents a negative real number.
     */
    [[nodiscard]] constexpr bool is_negative() const;

    /**
     * @return Whether this posit represents the number zero.
     */
    [[nodiscard]] constexpr bool is_zero() const;

    /**
     * @return Whether this posit represents NaR.
     */
    [[nodiscard]] constexpr bool is_nar() const;

    /**
     * @return The sign bit. The returned integer is either set to 0 or 1.
     */
    [[nodiscard]] constexpr storage_type get_sign_bit() const;

    /**
     * @brief Compute the bitwise increment.
     *
     * This method does not return this + one, rather it returns a posit that
     * has the bitstring of this incremented by one. Note that the result may
     * exhibit overflow.
     *
     * Getting the increment of a given posit can be useful when computing
     * rounding.
     *
     * @return The posit bits plus one
     */
    [[nodiscard]] constexpr posit incremented() const;

    /**
     * @brief Compute the bitwise decrement.
     *
     * This method does not return this - one, rather it returns a posit that
     * has the bitstring of this decremented by one. Note that the result may
     * exhibit underflow.
     *
     * Getting the decrement of a given posit can be useful when computing
     * rounding.
     *
     * @return The posit bits minus one
     */
    [[nodiscard]] constexpr posit decremented() const;

    /**
     * @brief Compute the bitwise increment that represents a real value.
     *
     * This method does not return this + one, rather it returns a posit that
     * has the bitstring of this incremented by one. If incrementing would lead
     * to NaR, this function returns the original posit.
     *
     * Getting the increment of a given posit can be useful when computing
     * rounding.
     *
     * @return The posit bits plus one
     */
    [[nodiscard]] constexpr posit incremented_real() const;

    /**
     * @brief Compute the bitwise decrement that represents a real value.
     *
     * This method does not return this - one, rather it returns a posit that
     * has the bitstring of this decremented by one. If decrementing would lead
     * to NaR, this function returns the original posit.
     *
     * Getting the decrement of a given posit can be useful when computing
     * rounding.
     *
     * @return The posit bits plus one
     */
    [[nodiscard]] constexpr posit decremented_real() const;

    /**
     * @return The underlying storage.
     */
    [[nodiscard]] constexpr storage_type get_bits() const;

private:
    /**
     * The underlying bitstring that makes up the posit.
     */
    storage_type bits;

    /**
     * Statically assert that the created instance of a given posit
     * conforms to the requirements for legal posits.
     *
     * We call this method in the constructor so it is guaranteed
     * to get generated on template instantiation.
     */
    constexpr void static_assert_template_parameters() const;
};

//
// Additional operators for posit Class.
//
// For implementations, look at posit/posit_operators.hpp.
//

/**
 * @brief Output a string representation of p to os.
 *
 * @param os The stream to write to.
 * @param p The posit to write.
 * @return A reference to os.
 */
template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const posit<N, ES, WT>& p);

//
// Additional operations for posit Class.
//
// For implementations, look at posit/posit_operations.hpp.
//

/**
 * @brief Compute number of regime bits.
 *
 * @return Number of regime bits in p.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr size_t get_num_regime_bits(const posit<N, ES, WT>& p);

/**
 * @brief Compute number of exponent bits.
 *
 * @return The number of exponent bits in the given posit.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr size_t get_num_exponent_bits(const posit<N, ES, WT>& p);

/**
 * @biref Compute number of fraction bits.
 *
 * @return The number of fraction bits in the given posit.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr size_t get_num_fraction_bits(const posit<N, ES, WT>& p);

/**
 * @brief Get the sign bit.
 *
 * @return The sign bit as-is, that is either 0 or 1.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr integer<N, WT> get_sign_value(const posit<N, ES, WT>& p);

/**
 * @brief Return regime value.
 *
 * The regime value is used in determining the real value a given posit
 * represents.
 *
 * @return The regime value of the given posit.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr integer<N, WT> get_regime_value(const posit<N, ES, WT>& p);

/**
 * @brief Return exponent value.
 *
 * @return The exponent value of the given posit.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr uinteger<N, WT> get_exponent_value(const posit<N, ES, WT>& p);

/**
 * Return the fraction of the given posit. The fraction is the fractional part
 * of the posit interpreted as an unsigned integer.
 *
 * @return The fraction of the given posit.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr uinteger<N, WT> get_fraction_value(const posit<N, ES, WT>& p);

/**
 * Evaluate the given posit to compute the represented real value.  Uses
 * double precision IEEE floats for computation.
 *
 * The special value posit::nar gets translated to NaN
 * by this function.
 *
 * @return The real value of p, represented as a double precision float.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr double to_double(const posit<N, ES, WT>& p);

/**
 * @brief Convert an IEEE double to posit.
 *
 * Depending on the specific value and the width of posit and exponent, loss
 * of information can occur. Do not expect the returned posit to match
 * argument x exactly.
 *
 * @nparam N The width of the returned posit.
 * @nparam ES The exponent size of the returned posit.
 * @napram WT The underlying word type of the returned posit.
 */
template <size_t N, size_t ES, typename WT = uint64_t>
[[nodiscard]] constexpr posit<N, ES, WT> from_double(const double x);

/**
 * @brief Return absolute value of a given posit.
 *
 * If p is NaR, the result is also NaR. If p is
 * zero, the result is also zero as there is only one representation of zero.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> abs(const posit<N, ES, WT>& p);

/**
 * @brief Return the global exponent that unifies regime and exponent.
 *
 * Posit values are a fraction m raised to some power 2^k where k is made up
 * of useed^R times 2^E. This function returns k.  Knowing k can be useful to
 * estimate the value of a given posit without evaluating the fraction.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr integer<N> get_scale_value(const posit<N, ES, WT>& p);

/**
 * @brief Dump posit information to stream.
 *
 * This function dumps various interesting parameters to a stream. It is
 * useful for debugging. After it has dumped the posit information, std::endl
 * is also written to the stream.
 *
 * @param os the stream to dump to
 * @param p The posit to dump
 */
template <size_t N, size_t ES, typename WT>
void dump_meta(std::ostream& os, const posit<N, ES, WT>& p);

/**
 * @brief Dump posit information to a string.
 *
 * This function dumps various interesting parameters to a string. It is
 * useful for debugging. The returned string contains a newline character.
 *
 * @param p The posit to dump
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] std::string dump_string(const posit<N, ES, WT>& p);

//
// Casts for posit Class.
//
// For implementation, look at posit/posit_casts.hpp
//

/**
 * @brief Convert posit to a posit type with more bits.
 *
 * All posits representable in an <N, ES> environment are also perfectly
 * representable in an <M, ES>, M > N environment.
 *
 * @tparam DestinationPosit The posit type to convert to. You must ensure that
 * the exponent size of DestinationPosit and SourceExponentSize match.
 * @tparam SourceWidth Width of the posit to be converted.
 * @tparam SourceExponentSize Exponent size of the posit to be converted.
 * @tparam SourceWordType Word type of the posit to be converted.
 * @param p The posit to convert.
 */
template <typename DestinationPosit, size_t SourceWidth, size_t SourceExponentSize,
          typename SourceWordType>
constexpr DestinationPosit
width_cast(const posit<SourceWidth, SourceExponentSize, SourceWordType>& p);

//
// String Utilities for posit Class.
//
// For implementation, look at posit/string_utils.hpp
//

/**
 * @brief Convert posit to printable bitstring.
 *
 * This function prints the underlying bitstring as a std::string of zeroes
 * and ones.
 *
 * @param p The posit from which to print the bitstring.
 */
template <size_t N, size_t ES, typename WT> std::string to_binary(const posit<N, ES, WT>& p);

/**
 * @brief Convert posit to printable bitstring.
 *
 * This function prints the underlying bitstring as a std::string of zeroes
 * and ones.
 *
 * If argument delim is not nullptr, delim is printed between sign and regime
 * bits, between regime and exponent bits and between exponent bits and
 * fraction bits. The returned string never contains any leading or trailing
 * delimiters.
 *
 * Note that this function really only makes sense if p is a positive posit.
 * Negative posits needs to be decoded (twos complement) to infer their
 * meaning.
 *
 * @param p The posit from which to print the bitstring.
 */
template <size_t N, size_t ES, typename WT>
std::string to_binary(const posit<N, ES, WT>& p, const char* delim);

//
// Fractional Class
//
// For implementation, look at posit/fractional.hpp
//

/**
 * @brief Class to handle fractional values.
 *
 * This represents an unsigned fixed point fraction as used by posits. Unlike
 * posits, this class uses an explicit hidden bit.
 *
 * @tparam N The width of associated posits
 * @tparam ES The exponent size of associated posits
 * @tparam WT The word type of associated posits
 */
template <size_t N, size_t ES, typename WT> class fractional
{
public:
    /**
     * Size of the integer part.
     */
    static constexpr size_t IntegerSize = N;

    /**
     * Size of the fractional part.
     */
    static constexpr size_t FractionSize = 2 * N;

    /**
     * @return A fraction that represents the number zero.
     */
    [[nodiscard]] static constexpr fractional zero();

    /**
     * @brief Construct a new zero fraction.
     *
     * The returned fraction has all bits set to zero. Meaning the
     * constructed fractional represents zero.
     */
    constexpr fractional();

    /**
     * @brief Copy constructor.
     */
    constexpr fractional(const fractional& other);

    /**
     * @brief Construct fractional from posit.
     *
     * Extract the fractional part from p and construct this fractional to
     * represent that number.
     *
     * @param p The posit to import the fraction from.
     */
    constexpr fractional(const posit<N, ES, WT>& p);

    /**
     * @brief Assignment operator.
     */
    fractional& operator=(const fractional<N, ES, WT>& other);

    /**
     * @brief Check for equality.
     *
     * Two fractional objects are equal if their integer and fractional part
     * are identical. The truncated flag is not taken into account.
     *
     * @return Whether this and other are equal.
     */
    [[nodiscard]] constexpr bool operator==(const fractional<N, ES, WT>& other) const;

    /**
     * @brief Check for not equal.
     *
     * Two fractional objects are equal if their integer and fractional part
     * are identical. The truncated flag is not taken into account.
     *
     * @return Whether this and other are not equal.
     */
    [[nodiscard]] constexpr bool operator!=(const fractional<N, ES, WT>& other) const;

    /**
     * @return Whether this is less than other. The truncated flag is not
     * taken into account.
     */
    [[nodiscard]] constexpr bool operator<(const fractional<N, ES, WT>& other) const;

    /**
     * @return Whether this is less than or equal to other. The truncated flag
     * is not taken into account.
     */
    [[nodiscard]] constexpr bool operator<=(const fractional<N, ES, WT>& other) const;

    /**
     * @return Whether this is greater than other. The truncated flag is not
     * taken into account.
     */
    [[nodiscard]] constexpr bool operator>(const fractional<N, ES, WT>& other) const;

    /**
     * @return Whether this is greater than or equal to other. The truncated flag
     * is not taken into account.
     */
    [[nodiscard]] constexpr bool operator>=(const fractional<N, ES, WT>& other) const;

    /**
     * @brief Addition of two fractional values.
     *
     * If the result is too large or too small to fit in the underlying
     * storage, the overflowed/underflowed bits are discarded.
     *
     * The returned fractional has the truncated flag set only if any of the
     * two operands also have the truncated flag set.
     *
     * @return The sum of this and other.
     */
    [[nodiscard]] constexpr fractional operator+(const fractional<N, ES, WT>& other) const;

    /**
     * @brief Subtraction of two fractional values.
     *
     * If the result is too large or too small to fit in the underlying
     * storage, the overflowed/underflowed bits are discarded.

     * The returned fractional has the truncated flag set only if any of the
     * two operands also have the truncated flag set.
     *
     * @return other subtracted from this.
     */
    [[nodiscard]] constexpr fractional operator-(const fractional<N, ES, WT>& other) const;

    /**
     * @param shift The number of places to shift.
     * @return This fractional shifted to the left.
     */
    [[nodiscard]] constexpr fractional operator<<(const size_t shift) const;

    /**
     * @param shift The number of places to shift.
     * @return This fractional shifted to the left.
     */
    [[nodiscard]] constexpr fractional operator<<(const uinteger<N, WT>& shift) const;

    /**
     * @param shift The number of places to shift.
     * @return This fractional shifted to the right.
     */
    [[nodiscard]] constexpr fractional operator>>(const size_t shift) const;

    /**
     * @param shift The number of places to shift.
     * @return This fractional shifted to the right.
     */
    [[nodiscard]] constexpr fractional operator>>(const uinteger<N, WT>& shift) const;

    /**
     * @brief Return whether this fractional has truncated precision.
     *
     * During left or right shifts, bits set to one can be truncated.  If that
     * happens, the internal truncated flag of the given fractional object is
     * set to true and remains true until destruction.
     *
     * @return Truncated flag.
     */
    [[nodiscard]] constexpr bool has_been_truncated() const;

    /**
     * @return Whether this object equals zero.
     */
    [[nodiscard]] constexpr bool is_zero() const;

    /**
     * @return A fractional with the underlying bits incremented by one.
     */
    [[nodiscard]] constexpr fractional incremented() const;

    /**
     * @brief Return the integer part of this fractional value.
     *
     * If this fractional represents 00001010.11110000, then this function
     * returns 00001010.
     */
    [[nodiscard]] constexpr uinteger<IntegerSize, WT> integer_bits() const;

    /**
     * @brief Return the integer part of this fractional value.
     *
     * If this fractional represents 00001010.11110000, then this function
     * returns 11110000.
     */
    [[nodiscard]] constexpr uinteger<FractionSize, WT> fraction_bits() const;

    /**
     * @brief Overload for writing fractional values to a stream.
     */
    template <size_t SN, size_t SES, typename SWT>
    friend std::ostream& operator<<(std::ostream& os, const fractional<SN, SES, SWT>& f);

private:
    /**
     * The index of the hidden bit, that is the bit that represents the value
     * 2^0 = 1.
     */
    static constexpr size_t HiddenBitIndex = 2 * N;

    /**
     * Size of the underlying scratch memory.
     */
    static constexpr size_t ScratchSize = IntegerSize + FractionSize;

    /**
     * The actual fractional value as an unsigned integer. We use an unsigned
     * integer (instead of, say, word array) to use the built-in arithmetic
     * operations.
     */
    uinteger<ScratchSize, WT> bits;

    /**
     * Keep track whether shifting left or right truncated any one bits away.
     * During construction from posit or default constructor, truncated is set
     * to false.  Any shift operations that truncate '1' bits set this flag to
     * one. It cannot be reset and in particular assignments copy the state of
     * the truncated flag.
     */
    bool truncated;
};

//
// Positparam Class
//
// For implementation, look at posit/positparams.hpp
//

/**
 * @brief Parameterized representation of a given posit.
 *
 * While posits use a more involved encoding, they actually represent a
 * product of fraction * 2^scale. This type represents posits in this format
 * rather than the original encoding. Translating posits to a positparam
 * representation is an intermediate step when doing arithmetic.
 *
 * @tparam N The width of associated posits
 * @tparam ES The exponent size of associated posits
 * @tparam WT The word type of associated posits
 */
template <size_t N, size_t ES, typename WT> class positparams
{
public:
    /**
     * @brief Construct from posit.
     *
     * Construct this object to represent the value of p. Conversion from
     * posit to positparams is referred to normalization in some posit
     * literature.
     *
     * @param p The posit to convert.
     */
    constexpr positparams(const posit<N, ES, WT>& p);

    /**
     * @brief Copy constructor.
     */
    constexpr positparams(const positparams& other);

    /**
     * @brief Assignment operator.
     */
    positparams& operator=(const positparams& other);

    /**
     * @brief Destructor.
     */
    ~positparams();

    /**
     * @brief Convert back to posit.
     *
     * Convert the underlying parameters back to a posit. Conversion from
     * positparams to posit is reference to as denormaliziation in some posit
     * literature.
     *
     * If there exists not posit to represent this positparams exactly,
     * rounding is applied.
     */
    [[nodiscard]] explicit constexpr operator posit<N, ES, WT>() const;

    /**
     * @brief Check for equality.
     *
     * @return Whether this and other are equal.
     */
    [[nodiscard]] constexpr bool operator==(const positparams& other) const;

    /**
     * @brief Check for not equal.
     *
     * @return Whether this and other are not equal.
     */
    [[nodiscard]] constexpr bool operator!=(const positparams& other) const;

    /**
     * @brief Addition of two posit in parameter representation.
     *
     * @return The sum of this and other.
     */
    [[nodiscard]] constexpr positparams operator+(const positparams& rhs) const;

    /**
     * @brief Overload for writing fractional values to a stream.
     */
    template <size_t SN, size_t SES, typename SWT>
    friend std::ostream& operator<<(std::ostream& os, const positparams<SN, SES, SWT>& p);

private:
    /**
     * Whether this posit represents NaR.
     */
    bool is_nar;

    /**
     * Whether this posit represents zero. Zero is often considered a special
     * case in posit operations. Here, we make that explicit.
     */
    bool is_zero;

    /**
     * Sign bit of the posit. If the sign bit is true, the represented posit
     * is negative. Otherwise it is positive.
     */
    bool sign_bit;

    /**
     * Scale parameter. Can be both positive (for big numbers) and negative
     * (for numbers close to zero).
     */
    integer<N, WT> scale;

    /**
     * Fraction parameter.
     */
    fractional<N, ES, WT> fraction;

    /**
     * @brief Private constructor for constructing empty parameter objects.
     *
     * The constructed object has all flags set to false.
     */
    constexpr positparams();

    /**
     * @return Return parameter object that represents zero.
     */
    [[nodiscard]] static constexpr positparams<N, ES, WT> zero();

    /**
     * @return Return parameter object that represents NaR.
     */
    [[nodiscard]] static constexpr positparams<N, ES, WT> nar();

    /**
     * @brief Return p, q in order of scale.
     * @return A tuple that contains p, q ordered by scale.
     */
    static std::tuple<positparams*, positparams*> ordered(positparams* p, positparams* q);

    /**
     * @brief Match scale for addition.
     *
     * Ensures that both p and q have the same scale value. During matching,
     * the object with smaller scale will have its fraction shifted to match
     * the new scale.
     */
    static void match_scale_of(positparams& p, positparams& q);

    /**
     * @brief Add up two positparams.
     *
     * @param dst Where to write the result to.
     * @param lhs First operand of the addition.
     * @Param rhs Second operand of the addition.
     */
    static void sum_fractions(positparams& dst, const positparams<N, ES, WT>& lhs,
                              const positparams<N, ES, WT>& rhs);

    /**
     * @brief Add up two fractions.
     *
     * If overflow occurs, the scale of dest is updated accordingly.
     *
     * @param dst Where to write the result to.
     * @param lhs First operand of the addition.
     * @Param rhs Second operand of the addition.
     */
    static void add_fractions(positparams& dst, const fractional<N, ES, WT>& lfrac,
                              const fractional<N, ES, WT>& rfrac);

    /**
     * @brief Subtract one fraction from the other.
     *
     * If underflow occurs, the scale of dest is updated accordingly.
     *
     * @param dst Where to write the result to.
     * @param lhs First operand of the addition.
     * @Param rhs Second operand of the addition. Will be subtracted from lhs.
     */
    static void sub_fractions(positparams& dst, const fractional<N, ES, WT>& lfrac,
                              const fractional<N, ES, WT>& rfrac);
};

} // namespace aarith

//
// Now we include the headers. Because we have prototypes for all methods and
// functions, these imports can be in any order and reference each other as
// they wish.
//

#include <aarith/posit/fractional.hpp>
#include <aarith/posit/posit.hpp>
#include <aarith/posit/posit_casts.hpp>
#include <aarith/posit/posit_operations.hpp>
#include <aarith/posit/posit_operators.hpp>
#include <aarith/posit/posit_types.hpp>
#include <aarith/posit/positparams.hpp>
#include <aarith/posit/string_utils.hpp>
