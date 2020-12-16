#pragma once

#include <aarith/integer.hpp>
#include <cstdint>

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
    constexpr posit& operator=(const posit& other);

    /**
     * @brief Assign this posit to hold the value of other.
     *
     * @param The value to change to.
     */
    constexpr posit& operator=(const posit&& other);

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
     * In particular, complex infinite compares equal to itself.
     *
     * @return true if this and other represent the same value. Otherwise
     * returns true.
     */
    constexpr bool operator==(const posit& other) const;

    /**
     * @brief Compare this and other for inequality.
     *
     * Two posits are not equal if they have different bit patterns.
     *
     * @return true if this and other represent different values. Otherwise
     * returns false.
     */
    constexpr bool operator!=(const posit& other) const;

    /**
     * @brief Return whether this is less than other.
     *
     * If this or other represent complex infinity, this method returns false.
     */
    constexpr bool operator<(const posit& other) const;

    /**
     * @brief Return whether this is less than or equal to other.
     *
     * If either this or other represent complex infinity, this method returns
     * false. If both this and other represent complex infinity, this method
     * returns true.
     */
    constexpr bool operator<=(const posit& other) const;

    /**
     * @brief Return whether this is greater than other.
     *
     * If this or other represent complex infinity, this method returns false.
     */
    constexpr bool operator>(const posit& other) const;

    /**
     * @brief Return whether this is greater than or equal to other.
     *
     * If either this or other represent complex infinity, this method returns
     * false. If both this and other represent complex infinity, this method
     * returns true.
     */
    constexpr bool operator>=(const posit& other) const;

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
     * If this posit represents complex infinity, the result is also complex
     * infinity.
     */
    constexpr posit& operator+() const;

    /**
     * @brief Sum of two posits.
     *
     * If either operand is complex infinity, the result is also complex
     * infinity. If neither operands are complex infinity, the result is
     * guaranteed not to be complex infinity.
     *
     * @param rhs The posit to add to this posit.
     * @return The sum of this and rhs.
     */
    posit operator+(const posit& rhs) const;

    /**
     * @brief Add rhs to this.
     *
     * @param rhs The posit to add to this posit.
     */
    posit& operator+=(const posit& rhs);

    /**
     * @brief Pre-Increment.
     *
     * @return A reference to the incremented value.
     */
    posit& operator++();

    /**
     * @brief Post-Increment.
     *
     * @return A copy of the value before it was incremented.
     */
    posit operator++(int);

    /**
     * @brief Unary minus.
     *
     * If this posit represents complex infinity, the result is also complex
     * infinity.
     */
    constexpr posit operator-() const;

    /**
     * @brief Sum of two posits.
     *
     * If either operand is complex infinity, the result is also complex
     * infinity. If neither operands are complex infinity, the result is
     * guaranteed not to be complex infinity.
     *
     * As posits only have one unique representation of zero, calling this
     * operator on a posit that represents zero again returns zero.
     *
     * @param rhs The posit to add to this posit.
     * @return The sum of this and rhs.
     */
    constexpr posit operator-(const posit& rhs) const;

    /**
     * @brief Subtract rhs from this.
     *
     * @param rhs The posit to subtract.
     */
    posit& operator-=(const posit& rhs);

    /**
     * @brief Pre-Decrement
     *
     * @return A reference to the decremented value.
     */
    posit& operator--();

    /**
     * @brief Post-Decrement.
     *
     * @return A copy of the value before it was decremented.
     */
    posit operator--(int);

    /**
     * @brief Return the product of this multipled with rhs.
     */
    posit operator*(const posit& rhs) const;

    //
    // Constants
    //

    /**
     * @brief Return width of the given posit type.
     */
    [[nodiscard]] static constexpr size_t n();

    /**
     * @brief Return the maximum exponent size of the given posit type.
     */
    [[nodiscard]] static constexpr size_t es();

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
     * @return Representation of complex infinity.
     */
    [[nodiscard]] static constexpr posit complex_infinity();

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
     * @return The sign bit. The returned integer is either set to 0 or 1.
     */
    [[nodiscard]] constexpr storage_type get_sign_bit() const;

    /**
     * @return The underlying storage.
     */
    [[nodiscard]] constexpr storage_type get_bits() const;

    // TODO (Schärtl): make private
    storage_type bits;

private:
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
 * The special value posit::complex_infinity gets translated to NaN
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
 * If p is complex infinity, the result is also complex infinity. If p is
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
constexpr integer<N> get_global_exponent(const posit<N, ES, WT>& p);

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
// Binprod Class
//
// For implementation, look at posit/binprod.hpp
//

/**
 * @brief Helper class that represents a product x * 2^m.
 *
 * One way to do arithmetic on posit values is to first convert them to
 * binprod, do the arithmetic and then convert them back to posit.
 *
 * @tparam N Determines the width of the underlying storage type. If used
 * together with posits<PN, PES, PWT> N should match PN.
 */
template <size_t N> class binprod
{
public:
    /**
     * Underlying storage type for scratch calculations. To avoid loss of
     * precision during arithmetic operations, it is twice is large as N.
     */
    // TODO (Schärtl): Find a real bound for N or maybe find a better way of
    // doing arithmetic all-together.
    using storage_type = integer<2 * N>;

    /**
     * Fractional part x of product x * 2^m.
     */
    storage_type x;

    /**
     * Exponent part m of product x * 2^m.
     */
    storage_type m;

    /**
     * @brief Construct new binprod with given parameters.
     *
     * @param x The fractional part to copy.
     * @param m The exponent part to copy.
     */
    constexpr binprod(const storage_type& x, const storage_type& m);

    /**
     * @brief Construct a new binprod from given posit.
     *
     * @nparam PES The exponent size of the provided posit.
     * @nparam WT The underlying word type of the provided posit.
     * @param p The posit to convert to binprod.
     */
    template <size_t PES, typename PWT> constexpr binprod(const posit<N, PES, PWT>& p);

    /**
     * @brief Convert value back to posit.
     *
     * @nparam PN Width of the returned posit.
     * @nparam PES Exponent size of the returned posit.
     * @nparam PWT Underlying storage type of the returned posit.
     */
    template <size_t PN, size_t PES, typename PWT>
    [[nodiscard]] posit<PN, PES, PWT> to_posit() const;

private:
    /**
     * Binary product x * 2^m can be constructed from parameters h, f, k, e as
     * explained in "Provably Correct Posit Arithmetic with Fixed-Point Big
     * Integer", Shin Yee Chung, 2018. This tuple represents storage for these
     * parameters.
     */
    using param_type = std::tuple<storage_type, storage_type, storage_type, storage_type>;

    /**
     * @brief Convert integer to ssize_t.
     *
     * Helper function only used here.
     */
    // TODO (Schärtl): Either move something like this to the integer directory
    // or find the real way to do it. Maybe file an issue.
    template <size_t M> [[nodiscard]] static constexpr ssize_t to_ssize_t(const integer<M>& n);

    /**
     * @brief Return the decoded parameters (h, f, k, e).
     *
     * Based on "Provably Correct Posit Arithmetic with Fixed-Point Big
     * Integer", Shin Yee Chung, 2018.
     */
    template <size_t PN, size_t PES, typename PWT>
    [[nodiscard]] constexpr param_type decode() const;

    /**
     * @brief Fill in the decoded regime.
     *
     * @nparam nregime The number of regime bits. Will be filled in by call to
     * this method.
     * @nparam first_regime Value of the first bit of the regime, either 1 (true)
     * or 0 (false). Will be filled in by call to this method.
     */
    template <size_t PN, size_t PES, typename PWT>
    void decode_regime(ssize_t& nregime, bool& first_regime) const;

    /**
     * @brief Assert that this binprod represents a non-negative value.
     *
     * Many internal methods only work on positive values. These methods
     * should call ensure_positive to check whether the value they are dealing
     * with is actually positive.
     */
    void ensure_positive() const;
};

//
// Additional operators for binprod Class.
//
// For implementations, look at posit/binprod_operators.hpp.
//

/**
 * @brief Return the sum of two binary products.
 */
template <size_t N> constexpr binprod<N> operator+(const binprod<N>& lhs, const binprod<N>& rhs);

/**
 * @brief Return the product of two binary products.
 */
template <size_t N> constexpr binprod<N> operator*(const binprod<N>& lhs, const binprod<N>& rhs);

} // namespace aarith

#include <aarith/posit/binprod.hpp>
#include <aarith/posit/binprod_operators.hpp>
#include <aarith/posit/posit.hpp>
#include <aarith/posit/posit_operators.hpp>
#include <aarith/posit/posit_types.hpp>
#include <aarith/posit/string_utils.hpp>
