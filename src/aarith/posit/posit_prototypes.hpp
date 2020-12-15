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
 * Represents a posit number that can represent real values. Based on
 * the description by John L. Gustafson ("Posit Arithmetic", 10
 * October 2017.)
 *
 * @tparam N The total width in bits of the given posit.
 * @tparam ES The maximum width in bits of the exponent for a given posit.
 */
template <size_t N, size_t ES, class WordType = uint64_t> class posit
{
public:
    using storage_type = uinteger<N, WordType>;
    using useed_type = uinteger<((1 << (1 << ES)) + 1), WordType>;

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
     * Construct this posit initialized to zero.
     */
    constexpr posit();

    /**
     * Construct this posit to be a clone of other.
     *
     * @param other The posit to clone.
     */
    constexpr posit(const posit& other);

    /**
     * Construct this posit to be a clone of other.
     *
     * @param other The posit to clone.
     */
    constexpr posit(const posit&& other);

    /**
     * Assign this posit to hold the value of other.
     *
     * @param The value to change to.
     */
    constexpr posit& operator=(const posit& other);

    /**
     * Assign this posit to hold the value of other.
     *
     * @param The value to change to.
     */
    constexpr posit& operator=(const posit&& other);

    /**
     * Construct this posit with given bits.
     *
     * @param n Bits used to initialize the underlying type.
     */
    // TODO (Schärtl) das funktioniert aber nur gut, so lange N <= 64!
    constexpr explicit posit(WordType n);

    //
    // Comparison Operators
    //

    /**
     * @brief Compare this and other for equality.
     *
     * @return true if this and other represent the same value. Otherwise
     * returns true.
     */
    constexpr bool operator==(const posit& other) const;

    /**
     * @brief Compare this and other for inequality.
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

    constexpr posit& operator+() const;

    posit operator+(const posit& rhs) const;

    posit& operator+=(const posit& rhs);

    posit& operator++();

    posit operator++(int);

    constexpr posit operator-() const;

    constexpr posit operator-(const posit& rhs) const;

    posit& operator-=(const posit& rhs);

    posit& operator--();

    posit operator--(int);

    //
    // Constants
    //

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
    [[nodiscard]] static constexpr uinteger<N + 1, WordType> npat();

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

template <size_t N, size_t ES, class WT>
std::ostream& operator<<(std::ostream& os, const posit<N, ES, WT>& p);

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr size_t get_num_regime_bits(const posit<N, ES, WT>& p);

/**
 * @return The number of exponent bits in the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr size_t get_num_exponent_bits(const posit<N, ES, WT>& p);

/**
 * @return The number of fraction bits in the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr size_t get_num_fraction_bits(const posit<N, ES, WT>& p);

/**
 * @return The sign value of the given posit.
 *
 * Returns either 0 or 1.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr integer<N, WT> get_sign_value(const posit<N, ES, WT>& p);

/**
 * @return The regime value of the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr integer<N, WT> get_regime_value(const posit<N, ES, WT>& p);

/**
 * @return The exponent value of the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr uinteger<N, WT> get_exponent_value(const posit<N, ES, WT>& p);

/**
 * Return the fraction of the given posit. The fraction is the fractional part
 * of the posit interpreted as an unsigned integer.
 *
 * @return The fraction of the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr uinteger<N, WT> get_fraction_value(const posit<N, ES, WT>& p);

/**
 * Evaluate the given posit to compute the represented real value.  Uses
 * double precision IEEE floats for computation.
 *
 * The special value posit::complex_infinity gets translated to NaN
 * by this function.
 *
 * @ The real value of p, represented as a double precision float.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr double to_double(const posit<N, ES, WT>& p);

template <size_t N, size_t ES, class WT = uint64_t>
[[nodiscard]] constexpr posit<N, ES, WT> from_double(const double x);

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr posit<N, ES, WT> abs(const posit<N, ES, WT>& p);

/**
 * @brief Return the global exponent that unifies regime and exponent.
 *
 * Posit values are a fraction m raised to some power 2^k where
 * k is made up of useed^R times 2^E. This function returns k.
 */
template <size_t N, size_t ES, class WT>
constexpr integer<N> get_global_exponent(const posit<N, ES, WT>& p);

//
// String Utilities for posit Class.
//
// For implementation, look at posit/string_utils.hpp
//

template <size_t N, size_t ES, class WT> std::string to_binary(const posit<N, ES, WT>& p);

template <size_t N, size_t ES, class WT>
std::string to_binary(const posit<N, ES, WT>& p, const char* delim);

//
// Binprod Class
//
// For implementation, look at posit/binprod.hpp
//

template <size_t N> class binprod
{
public:
    using storage_type = integer<2 * N>;

    storage_type x;
    storage_type m;

    constexpr binprod(const storage_type& x, const storage_type& m);

    template <size_t PES, class PWT> constexpr binprod(const posit<N, PES, PWT>& p);

    template <size_t PN, size_t PES, typename PWT>
    [[nodiscard]] posit<PN, PES, PWT> to_posit() const;

private:
    using param_type = std::tuple<storage_type, storage_type, storage_type, storage_type>;

    /**
     * @biref Computes k mod n.
     *
     * Unlike the C mod operation, this operation always returns non-negative
     * results, which is closer to what is commonly used in mathematics.
     */
    template <size_t M, class WT>
    [[nodiscard]] static constexpr integer<M, WT> absmod(const integer<M, WT>& k,
                                                         const integer<M, WT>& n);

    template <size_t M> [[nodiscard]] static constexpr ssize_t to_ssize_t(const integer<M>& n);

    /**
     * @brief Return the decoded parameters (h, f, k, e).
     *
     * Based on "Provably Correct Posit Arithmetic with Fixed-Point Big
     * Integer", Shin Yee Chung, 2018.
     */
    template <size_t PN, size_t PES, class PWT> [[nodiscard]] constexpr param_type decode() const;

    /**
     * @brief Fill in the decoded regime.
     */
    template <size_t PN, size_t PES, class PWT>
    void decode_regime(ssize_t& nregime, bool& first_regime) const;

    void ensure_positive() const;
};

//
// Additional operators for binprod Class.
//
// For implementations, look at posit/binprod_operators.hpp.
//

template <size_t N> constexpr binprod<N> operator+(const binprod<N>& lhs, const binprod<N>& rhs);

} // namespace aarith

#include <aarith/posit/binprod.hpp>
#include <aarith/posit/binprod_operators.hpp>
#include <aarith/posit/posit.hpp>
#include <aarith/posit/posit_operators.hpp>
#include <aarith/posit/string_utils.hpp>
