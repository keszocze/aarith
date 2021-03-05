#pragma once

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <aarith/fixed_point.hpp>
#include <aarith/integer.hpp>

//
// This file is for prototypes and forward declarations of the posit
// implementation. Listing all functions and methods here makes it easier for
// them to call each other.
//
// Eventually we will have to look at how this can be circumvented with
// modules. But for now the focus is understanding posit arithmetic.
//

namespace aarith {

/**
 * @brief Fallback word type used as template parameters.
 *
 * Many of the types defines in this file use an aarith::uinteger or similar
 * containers which require a word type parameter. In most cases we just use the
 * default type defined here.
 */
using DefaultWordType = uint64_t;

//
// Rounding Mode Enum
//

/**
 * @brief Enum that encodes information how the result of a posit arithmetic
 * operation was rounded.
 *
 * @note Users might be tempted to misinterpret this enum as a boolean that
 * represents "has been rounded" or similar. To discourage this misuse, values
 * of the individual enum entries have been chosen such that each enum entry
 * evaluates to true if implicitly converted to bool.
 */
enum class rounding_event
{
    NOT_ROUNDED = 1,
    ROUNDED_DOWN = 2,
    ROUNDED_UP = 3
};

//
// Rounding Mode Operators
//
// For implementation, look at posit/rounding_event_operators.hpp
//

/**
 * @brief Output a string representation of r to os.
 *
 * @param os The stream to write to.
 * @param r The enum to write.
 * @return A reference to os.
 */
template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const rounding_event& r);

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
template <size_t N, size_t ES, typename WT = DefaultWordType> class posit
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
    [[nodiscard]] static constexpr posit from(const storage_type& bits);

    /**
     * @brief Construct this posit with given bits.
     *
     * Argument bits is not interpreted as an integer, rather it is used as
     * the underlying bit pattern of the returned posit.
     *
     * @param n Bits used to initialize the underlying type.
     */
    // TODO (Schärtl) das funktioniert aber nur gut, so lange N <= 64!
    [[nodiscard]] static constexpr posit from(const WT bits);

    //
    // Constructors
    //

    /**
     * @brief Construct posit initialized to zero.
     */
    constexpr posit();

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(int8_t value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(int16_t value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(int32_t value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(int64_t value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    template <size_t ValueWidth, typename ValueWordType>
    explicit constexpr posit(const integer<ValueWidth, ValueWordType>& value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(uint8_t value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(uint16_t value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(uint32_t value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(uint64_t value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    template <size_t ValueWidth, typename ValueWordType>
    explicit constexpr posit(const uinteger<ValueWidth, ValueWordType>& value);

    /**
     * @brief Construct posit from value.
     *
     * Depending on the parameters N and WT of the given posit type, the
     * returned posit might not represent value exactly.
     *
     * @praram value The value to represent with the constructed posit.
     */
    explicit constexpr posit(double value);

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

    //
    // Conversion Operators
    //

    /**
     * @brief Convert to integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned integer type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an integer.
     */
    [[nodiscard]] explicit constexpr operator int8_t() const;

    /**
     * @brief Convert to integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned integer type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an integer.
     */
    [[nodiscard]] explicit constexpr operator int16_t() const;

    /**
     * @brief Convert to integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned integer type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an integer.
     */
    [[nodiscard]] explicit constexpr operator int32_t() const;

    /**
     * @brief Convert to integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned integer type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an integer.
     */
    [[nodiscard]] explicit constexpr operator int64_t() const;

    /**
     * @brief Convert to unsigned integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned uinteger type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an unsigned integer.
     */
    [[nodiscard]] explicit constexpr operator uint8_t() const;

    /**
     * @brief Convert to unsigned integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned uinteger type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an unsigned integer.
     */
    [[nodiscard]] explicit constexpr operator uint16_t() const;

    /**
     * @brief Convert to unsigned integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned uinteger type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an unsigned integer.
     */
    [[nodiscard]] explicit constexpr operator uint32_t() const;

    /**
     * @brief Convert to unsigned integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned uinteger type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an unsigned integer.
     */
    [[nodiscard]] explicit constexpr operator uint64_t() const;

    /**
     * @brief Convert to unsigned integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned uinteger type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an unsigned integer.
     */
    template <size_t TargetWidth, typename TargetWordType>
    [[nodiscard]] explicit constexpr operator uinteger<TargetWidth, TargetWordType>() const;

    /**
     * @brief Convert to integer.
     *
     * @note It is the callers responsibility to ensure that the value represented
     * by this posit actually fits in the returned integer type. If it does not,
     * under or overflow will occur.
     *
     * @throws nar_error If this posits represents NaR. NaR cannot be
     * converted to an integer.
     */
    template <size_t TargetWidth, typename TargetWordType>
    [[nodiscard]] explicit constexpr operator integer<TargetWidth, TargetWordType>() const;

    /**
     * @brief Convert to IEEE double precision.
     *
     * @note It is the callers responsibility to ensure that the value
     * represented by this posit actually fits in the returned type. If it
     * does not, under or overflow will occur.
     */
    [[nodiscard]] explicit constexpr operator double() const;

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
     * returns false.
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
    posit& operator+=(const posit& rhs);

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
     * If this posit represents NaR, the result is also NaR. Also, as posits
     * only have one unique representation of zero, calling this operator on a
     * posit that represents zero again returns zero.
     */
    [[nodiscard]] constexpr posit operator-() const;

    /**
     * @brief Posit subtraction.
     *
     * If either operand is NaR, the result is also NaR. If neither operands
     * are NaR, the result is guaranteed not to be NaR.
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
    posit& operator-=(const posit& rhs);

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
     *
     * If either operand is NaR, the result is also NaR. If neither operands
     * are NaR, the result is guaranteed not to be NaR.
     */
    [[nodiscard]] constexpr posit operator*(const posit& rhs) const;

    /**
     * @brief Set this to this multiplied with rhs.
     */
    [[nodiscard]] constexpr posit& operator*=(const posit& rhs);

    /**
     * @brief Return this divided by other.
     *
     * If either operand is NaR, the result is also NaR. If neither operands
     * are NaR, the result is guaranteed not to be NaR.
     */
    [[nodiscard]] constexpr posit operator/(const posit& rhs) const;

    /**
     * @brief Set this to this divided by other.
     */
    [[nodiscard]] constexpr posit& operator/=(const posit& rhs);

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
    [[nodiscard]] constexpr storage_type msb() const;

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
    [[nodiscard]] const storage_type& get_bits() const;

    /**
     * @brief Return a rational representation.
     *
     * If the given posit represents NaR, this method returns the fraction
     * 0 / 0. If the given posit is not NaR, the denominator will never
     * be zero.
     *
     * @tparam IN The width of the returned fraction parts. If IN is chosen
     * too small, overflow and underflow might corrupt the result.
     *
     * @return A tuple [p, q] to represent the fraction p / q.
     */
    template <size_t IN> [[nodiscard]] std::tuple<integer<IN>, integer<IN>> as_fraction() const;

protected:
    /**
     * The underlying bitstring that makes up the posit.
     */
    storage_type bits;

    /**
     * @brief Internal helper for converting integers to double.
     *
     * @return Argument n converted to double.
     */
    template <size_t IN, typename IWT>
    [[nodiscard]] static constexpr double to_double(const uinteger<IN, IWT>& n);

    /**
     * @brief Internal helper for converting integers to double.
     *
     * @return Argument n converted to double.
     */
    template <size_t IN, typename IWT>
    [[nodiscard]] static constexpr double to_double(const integer<IN, IWT>& n);

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
 * Return the fraction of the given posit. The fraction is the posit_fraction part
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
 * @brief Return absolute value of a given posit.
 *
 * If p is NaR, the result is also NaR. If p is zero, the result is also zero
 * as there is only one representation of zero.
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

/**
 * @brief The result of an arithmetic operation.
 *
 * Operations such as add() and mul() return posit_result. It combines the
 * actual numeric result with information about rounding that occurred. Keeping
 * track of rounding can be useful when experimenting with posit arithmetic.
 *
 * @tparam N The total width in bits of the posit result.
 * @tparam ES The maximum width in bits of the exponent of the posit result.
 * @tparam WS The underlying word type used to store the posit result.
 */
template <size_t N, size_t ES, typename WT>
using posit_result = std::tuple<posit<N, ES, WT>, rounding_event>;

/**
 * @brief Sum of two posits.
 *
 * If either operand is NaR, the result is also NaR. If neither operands are
 * NaR, the result is guaranteed not to be NaR.
 *
 * @param lhs First posit to add.
 * @param rhs Second posit to add.
 * @return The sum of lhs and rhs. Also returns rounding information to
 * indicates whether rounding occurred.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit_result<N, ES, WT> add(const posit<N, ES, WT>& lhs,
                                                    const posit<N, ES, WT>& rhs);

/**
 * @brief Product of two posits.
 *
 * If either operand is NaR, the result is also NaR. If neither operands are
 * NaR, the result is guaranteed not to be NaR.
 *
 * @param lhs First posit to multiply.
 * @param rhs Second posit to multiply.
 * @return The product of lhs and rhs. Also returns rounding information to
 * indicates whether rounding occurred.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit_result<N, ES, WT> mul(const posit<N, ES, WT>& lhs,
                                                    const posit<N, ES, WT>& rhs);

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
// For implementation, look at posit/posit_string_utils.hpp
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
// Define custom exception types for use with posit operations
//
// For implementation, look at posit/errors.hpp
//

/**
 * @brief Base class for posit errors.
 *
 * While our posit implementation will try its best not to throw
 * exceptions and instead return reasonable results, sometimes there
 * is no way but to stop execution with an error.
 *
 * All exceptions thrown by the posit implementation inherit from
 * posit_error, but posit_error is never thrown directly.
 */
class posit_error : std::runtime_error
{
public:
    /**
     * @brief Construct a new error.
     *
     * @param whatarg A short explanation of what caused this error.
     */
    explicit posit_error(const std::string& whatarg);
};

/**
 * @brief Error thrown when trying to do illegal operations on NaR values.
 *
 * While most operations handle NaR values just fine, some do not. In
 * particular, it is not possible to convert a posit representing NaR to a
 * integer as there is no "error state" for integers. In those cases,
 * nar_error will be thrown to signal that an illegal operation was attempted.
 */
class nar_error : posit_error
{
public:
    /**
     * @brief Construct a new error.
     *
     * @param whatarg A short explanation of what caused this error.
     */
    explicit nar_error(const std::string& whatarg);
};

//
// Fractional Class
//
// For implementation, look at posit/posit_fraction.hpp
//

/**
 * @brief Class to handle posit_fraction values.
 *
 * This represents an unsigned fixed point fraction as used by posits. Unlike
 * posits, this class uses an explicit hidden bit.
 *
 * @tparam N The width of associated posits
 * @tparam ES The exponent size of associated posits
 * @tparam WT The word type of associated posits
 */
template <size_t N, size_t ES, typename WT = DefaultWordType> class posit_fraction
{
public:
    /**
     * @brief Size of the integer part.
     */
    static constexpr size_t IntegerSize = N;

    /**
     * @brief Size of the posit_fraction part.
     *
     * The biggest representable integer by a posit<N, ES> type
     * type is
     *
     *   max := (2^(2^ES))^(N - 2) = 2^((2^ES) * (N -2)
     *
     * which means to represent that value we need at last
     *
     *   s := ⌈ ld max ⌉ = ⌈ 2ᵉˢ (N - 2) ⌉
     *
     * bits to represent max. As such we pick this weird looking
     * fraction size. It means that we can easily use this fraction
     * class when converting from integer to posit.
     */
    static constexpr size_t FractionSize = (1 << ES) * 2 * N;

    /**
     * The index of the hidden bit in scratch memory, that is the bit that
     * represents the value 2^0 = 1.
     */
    static constexpr size_t HiddenBitIndex = FractionSize;

    /**
     * Size of the underlying scratch memory.
     */
    static constexpr size_t ScratchSize = IntegerSize + FractionSize;

    /**
     * @return A fraction that represents the number zero.
     */
    [[nodiscard]] static constexpr posit_fraction zero();

    /**
     * @brief Construct a new zero fraction.
     *
     * The returned fraction has all bits set to zero. Meaning the
     * constructed posit_fraction represents zero.
     */
    constexpr posit_fraction();

    /**
     * @brief Copy constructor.
     */
    constexpr posit_fraction(const posit_fraction& other);

    /**
     * @brief Construct posit_fraction from posit.
     *
     * Extract the posit_fraction part from p and construct this posit_fraction to
     * represent that number.
     *
     * @param p The posit to import the fraction from.
     */
    constexpr posit_fraction(const posit<N, ES, WT>& p);

    /**
     * @brief Construct posit_fraction from integer fraction.
     *
     * If argument frac is 00110010, then the posit_fraction is constructed to
     * 0.00110010, that is the argument frac is taken as-is. The integer
     * part of the posit_fraction is initalized to zero.
     *
     * @param frac The fraction to import.
     */
    constexpr posit_fraction(const uinteger<FractionSize, WT>& frac);

    /**
     * @brief Assignment operator.
     */
    posit_fraction& operator=(const posit_fraction<N, ES, WT>& other);

    /**
     * @brief Check for equality.
     *
     * Two posit_fraction objects are equal if their integer and posit_fraction part
     * are identical. The truncated flag is not taken into account.
     *
     * @return Whether this and other are equal.
     */
    [[nodiscard]] constexpr bool operator==(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @brief Check for not equal.
     *
     * Two posit_fraction objects are equal if their integer and posit_fraction part
     * are identical. The truncated flag is not taken into account.
     *
     * @return Whether this and other are not equal.
     */
    [[nodiscard]] constexpr bool operator!=(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @return Whether this is less than other. The truncated flag is not
     * taken into account.
     */
    [[nodiscard]] constexpr bool operator<(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @return Whether this is less than or equal to other. The truncated flag
     * is not taken into account.
     */
    [[nodiscard]] constexpr bool operator<=(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @return Whether this is greater than other. The truncated flag is not
     * taken into account.
     */
    [[nodiscard]] constexpr bool operator>(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @return Whether this is greater than or equal to other. The truncated flag
     * is not taken into account.
     */
    [[nodiscard]] constexpr bool operator>=(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @brief Addition of two posit_fraction values.
     *
     * If the result is too large or too small to fit in the underlying
     * storage, the overflowed/underflowed bits are discarded.
     *
     * The returned posit_fraction has the truncated flag set only if any of the
     * two operands also have the truncated flag set.
     *
     * @return The sum of this and other.
     */
    [[nodiscard]] constexpr posit_fraction operator+(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @brief Subtraction of two posit_fraction values.
     *
     * If the result is too large or too small to fit in the underlying
     * storage, the overflowed/underflowed bits are discarded.
     *
     * The returned posit_fraction has the truncated flag set only if any of the
     * two operands also have the truncated flag set.
     *
     * @return other subtracted from this.
     */
    [[nodiscard]] constexpr posit_fraction operator-(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @brief Multiplication of two posit_fraction values.
     *
     * If the result is too large or too small to fit in the underlying
     * storage, the overflowed/underflowed bits are discarded.
     *
     * @return this multiplied with other.
     */
    [[nodiscard]] constexpr posit_fraction operator*(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @brief Division of two posit_fraction values.
     *
     * If the result is too large or too small to fit in the underlying
     * storage, the overflowed/underflowed bits are discarded.
     *
     * @return this divided by other.
     */
    [[nodiscard]] constexpr posit_fraction operator/(const posit_fraction<N, ES, WT>& other) const;

    /**
     * @param shift The number of places to shift.
     * @return This posit_fraction shifted to the left.
     */
    [[nodiscard]] constexpr posit_fraction operator<<(const size_t shift) const;

    /**
     * @param shift The number of places to shift.
     * @return This posit_fraction shifted to the left.
     */
    [[nodiscard]] constexpr posit_fraction operator<<(const uinteger<N, WT>& shift) const;

    /**
     * @param shift The number of places to shift.
     * @return This posit_fraction shifted to the right.
     */
    [[nodiscard]] constexpr posit_fraction operator>>(const size_t shift) const;

    /**
     * @param shift The number of places to shift.
     * @return This posit_fraction shifted to the right.
     */
    [[nodiscard]] constexpr posit_fraction operator>>(const uinteger<N, WT>& shift) const;

    /**
     * @brief Return whether this posit_fraction has truncated precision.
     *
     * During left or right shifts, bits set to one can be truncated.  If that
     * happens, the internal truncated flag of the given posit_fraction object is
     * set to true and remains true until destruction.
     *
     * @return Truncated flag.
     */
    [[nodiscard]] constexpr bool has_been_truncated() const;

    void set_not_truncated();

    /**
     * @return Whether this object equals zero.
     */
    [[nodiscard]] constexpr bool is_zero() const;

    /**
     * @return A posit_fraction with the underlying bits incremented by one.
     */
    [[nodiscard]] constexpr posit_fraction incremented() const;

    /**
     * @brief Return the integer part of this posit_fraction value.
     *
     * If this posit_fraction represents 00001010.11110000, then this method
     * returns 00001010.
     */
    [[nodiscard]] constexpr uinteger<IntegerSize, WT> integer_bits() const;

    /**
     * @brief Return the integer part of this posit_fraction value.
     *
     * If this posit_fraction represents 00001010.11110000, then this method
     * returns 11110000.
     */
    [[nodiscard]] constexpr uinteger<FractionSize, WT> fraction_bits() const;

    /**
     * @brief Return the underlying scratch memory.
     */
    [[nodiscard]] constexpr uinteger<ScratchSize, WT> scratch_bits() const;

    /**
     * @brief Overload for writing posit_fraction values to a stream.
     */
    template <size_t SN, size_t SES, typename SWT>
    friend std::ostream& operator<<(std::ostream& os, const posit_fraction<SN, SES, SWT>& f);

protected:
    /**
     * The actual posit_fraction value as an unsigned integer. We use an unsigned
     * integer (instead of, say, word array) to use the built-in arithmetic
     * operations.
     */
    uinteger<ScratchSize, WT> bits;

    /**
     * Keep track whether shifting left or right truncated any one bits away.
     * During construction from posit or default constructor, truncated is set
     * to false.  Any shift operations that truncate '1' bits set this flag to
     * one.
     */
    bool truncated;

    /**
     * @brief Import integer fraction.
     *
     * If fraction_bits are set to 01100100, then after the call the fraction
     * is set to 0.01100100.
     */
    template <size_t IN, typename IWT>
    void import_fraction_bits(const uinteger<IN, IWT>& fraction_bits);
};

//
// Positparam Class
//
// For implementation, look at posit/posit_parameters.hpp
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
template <size_t N, size_t ES, typename WT = DefaultWordType> class posit_parameters
{
public:
    using scale_type = integer<N, WT>;
    using fraction_type = posit_fraction<N, ES, WT>;

    /**
     * @return Parameter object that represents zero.
     */
    [[nodiscard]] static constexpr posit_parameters<N, ES, WT> zero();

    /**
     * @return Parameter object that represents NaR.
     */
    [[nodiscard]] static constexpr posit_parameters<N, ES, WT> nar();

    /**
     * @return Parameter object that represents the max posit for the given
     * type.
     */
    [[nodiscard]] static constexpr posit_parameters<N, ES, WT> max();

    /**
     * @return Parameter object that represent the smallest positive value.
     */
    [[nodiscard]] static constexpr posit_parameters<N, ES, WT> minpos();

    /**
     * @brief Construct from posit.
     *
     * Construct this object to represent the value of p. Conversion from
     * posit to posit_parameters is referred to normalization in some posit
     * literature.
     *
     * @param p The posit to convert.
     */
    constexpr posit_parameters(const posit<N, ES, WT>& p);

    /**
     * @brief Construct a new parameters object with given bits.
     *
     * Values are imported and then converted to standard form internally.
     */
    [[nodiscard]] static constexpr posit_parameters
    from(bool is_nar_arg, bool is_zero_arg, bool sign_bit_arg, const integer<N, WT>& scale_arg,
         const posit_fraction<N, ES, WT>& fraction_arg);

    /**
     * @brief Construct from signed integer.
     *
     * Construct this object to represent the given value.
     *
     * @param value The integer to convert.
     */
    template <size_t ValueWidth, typename ValueWordType>
    constexpr posit_parameters(const integer<ValueWidth, ValueWordType>& value);

    /**
     * @brief Copy constructor.
     */
    constexpr posit_parameters(const posit_parameters& other);

    /**
     * @brief Assignment operator.
     */
    posit_parameters& operator=(const posit_parameters& other);

    /**
     * @brief Destructor.
     */
    ~posit_parameters();

    /**
     * @brief Convert back to posit.
     *
     * Convert the underlying parameters back to a posit. Conversion from
     * posit_parameters to posit is reference to as denormaliziation in some posit
     * literature.
     *
     * If there exists not posit to represent this posit_parameters exactly,
     * rounding is applied.
     */
    [[nodiscard]] explicit constexpr operator posit<N, ES, WT>() const;

    /**
     * @brief Convert to integer.
     *
     * Convert the underlying parameters back to an integer representation.
     */
    template <size_t TargetWidth, typename TargetWordType>
    [[nodiscard]] explicit constexpr operator integer<TargetWidth, TargetWordType>() const;

    /**
     * @brief Check for equality.
     *
     * @return Whether this and other are equal.
     */
    [[nodiscard]] constexpr bool operator==(const posit_parameters& other) const;

    /**
     * @brief Check for not equal.
     *
     * @return Whether this and other are not equal.
     */
    [[nodiscard]] constexpr bool operator!=(const posit_parameters& other) const;

    /**
     * @brief Addition of two posit in parameter representation.
     *
     * @return The sum of this and other.
     */
    [[nodiscard]] constexpr posit_parameters operator+(const posit_parameters& rhs) const;

    /**
     * @brief Multiplication of two posit in parameter representation.
     *
     * @return The product of this and other.
     */
    [[nodiscard]] constexpr posit_parameters operator*(const posit_parameters& rhs) const;

    /**
     * @brief Division of two posit in parameter representation.
     *
     * @return This divided by rhs.
     */
    [[nodiscard]] constexpr posit_parameters operator/(const posit_parameters& rhs) const;

    /**
     * @brief Overload for writing posit_fraction values to a stream.
     */
    template <size_t SN, size_t SES, typename SWT>
    friend std::ostream& operator<<(std::ostream& os, const posit_parameters<SN, SES, SWT>& p);

    /**
     * @return The underlying scale value.
     */
    [[nodiscard]] constexpr integer<N, WT> get_scale() const;

    /**
     * @return The underlying fraction.
     */
    [[nodiscard]] constexpr posit_fraction<N, ES, WT> get_fraction() const;

    /**
     * @brief Return absolute value of a given parameterized posit.
     *
     * If p is NaR, the result is also NaR. If p is zero, the result is also zero
     * as there is only one representation of zero.
     */
    [[nodiscard]] constexpr posit_parameters absolute_value() const;

    /**
     * @return The parameterized posit converted back to posit encoding. Also
     * returns rounding information to indicates whether rounding occurred.
     */
    [[nodiscard]] constexpr std::tuple<posit<N, ES, WT>, rounding_event> to_posit() const;

protected:
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
    posit_fraction<N, ES, WT> fraction;

    /**
     * @brief Protected constructor for constructing empty parameter objects.
     *
     * The constructed object has all flags set to false.
     */
    constexpr posit_parameters();

    /**
     * @brief Return p, q in order of scale.
     * @return A tuple that contains p, q ordered by scale.
     */
    static std::tuple<posit_parameters*, posit_parameters*> ordered(posit_parameters* p,
                                                                    posit_parameters* q);

    /**
     * @brief Match scale for addition.
     *
     * Ensures that both p and q have the same scale value. During matching,
     * the object with smaller scale will have its fraction shifted to match
     * the new scale.
     */
    static void match_scale_of(posit_parameters& p, posit_parameters& q);

    /**
     * @brief Add up two posit_parameters.
     *
     * @param dst Where to write the result to.
     * @param lhs First operand of the addition.
     * @Param rhs Second operand of the addition.
     */
    static void sum_fractions(posit_parameters& dst, const posit_parameters<N, ES, WT>& lhs,
                              const posit_parameters<N, ES, WT>& rhs);

    /**
     * @brief Add up two fractions.
     *
     * If overflow occurs, the scale of dest is updated accordingly.
     *
     * @param dst Where to write the result to.
     * @param lhs First operand of the addition.
     * @Param rhs Second operand of the addition.
     */
    static void add_fractions(posit_parameters& dst, const posit_fraction<N, ES, WT>& lfrac,
                              const posit_fraction<N, ES, WT>& rfrac);

    /**
     * @brief Subtract one fraction from the other.
     *
     * If underflow occurs, the scale of dest is updated accordingly.
     *
     * @param dst Where to write the result to.
     * @param lhs First operand of the addition.
     * @Param rhs Second operand of the addition. Will be subtracted from lhs.
     */
    static void sub_fractions(posit_parameters& dst, const posit_fraction<N, ES, WT>& lfrac,
                              const posit_fraction<N, ES, WT>& rfrac);

    /**
     * @brief Normalize the parameterized posit.
     *
     * There are different parameter representations that are equivalent. For
     * example, 1.0 × 2⁴ is the same as 0.1 × 2⁵. Calling ensure_standard_form
     * changes this parameters object to a standard form.
     *
     * Converting to a single standardized form is not really necessary, but it
     * allows us to ensure certain preconditions and that makes implementation
     * easier.
     */
    void ensure_standard_form();
};

//
// Additional operators for posit_parameters class.
//
// For implementations, look at posit/posit_parameters_operations.hpp
//

/**
 * @brief Return absolute value of a given parameterized posit.
 *
 * If p is NaR, the result is also NaR. If p is zero, the result is also zero
 * as there is only one representation of zero.
 *
 * @note This function just calls the absolute_value method on argument p.
 * We provide this function to match the interface of other types (e.g. integer,
 * posit and so on).
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit_parameters<N, ES, WT> abs(const posit_parameters<N, ES, WT>& p);

//
// Quire Helpers for Calculating Sizes.
//
// They are separate from the quire class because to instantiate a quire class
// we need to run these functions first.
//
// For implementation, look at posit/quire_sizes.hpp.
//

/**
 * @brief Return width of integer part in quire.
 *
 * Computes the width of the integer part in a quire for posits with given
 * width and exponent size.
 *
 * @param posit_width Width of the associated posit type.
 * @param posit_exponent_size Exponent size of the associated posit type.
 */
[[nodiscard]] constexpr size_t quire_integer_width(size_t posit_width, size_t posit_exponent_size);

/**
 * @brief Return width of fraction part in quire.
 *
 * Computes the width of the fraction part in a quire for posits with given
 * width and exponent size.
 *
 * @param posit_width Width of the associated posit type.
 * @param posit_exponent_size Exponent size of the associated posit type.
 */
[[nodiscard]] constexpr size_t quire_fraction_width(size_t posit_width, size_t posit_exponent_size);

/**
 * @brief Return width of carry guard part in quire.
 *
 * Computes the width of the carry guard in a quire for posits with given
 * width and exponent size.
 *
 * @param posit_width Width of the associated posit type.
 * @param posit_exponent_size Exponent size of the associated posit type.
 */
[[nodiscard]] constexpr size_t quire_carry_width(size_t posit_width, size_t posit_exponent_size);

/**
 * @brief Return width of quire.
 *
 * Computes the width of the quire for posits with given width and exponent
 * size. Includes sign bit, carry guard, integer and fractional parts.
 *
 * @param posit_width Width of the associated posit type.
 * @param posit_exponent_size Exponent size of the associated posit type.
 */
[[nodiscard]] constexpr size_t quire_width(size_t posit_width, size_t posit_exponent_size);

//
// Quire Implementation.
//
// For implementation, look at posit/quire.hpp.
//

/**
 * @brief Quire for use with posit arithmetic.
 *
 * A quire is a fixed point number that can be used to compute posit
 * arithmetic without loss of precision up to a certain amount of operations.
 *
 * @tparam N The width of associated posits.
 * @tparam ES The exponent size of associated posits.
 * @tparam WT The word type of associated posits.
 */
template <size_t N, size_t ES, typename WT = DefaultWordType> class quire
{
public:
    /**
     * @brief Width of the integer part in the underlying fixed point number.
     */
    static constexpr size_t IntegerSize = quire_integer_width(N, ES);

    /**
     * @brief Width of the fraction part in the underlying fixed point number.
     */
    static constexpr size_t FractionSize = quire_fraction_width(N, ES);

    /**
     * @brief Width of the carry guard in the underlying fixed point number.
     *
     * The carry guard consists of at least 30 bits that are here to prevent
     * rounding errors.
     */
    static constexpr size_t CarrySize = quire_carry_width(N, ES);

    /**
     * @brief Total width of the underlying fixed point number, including the
     * sign bit.
     */
    static constexpr size_t QuireSize = quire_width(N, ES);

    /**
     * @brief The underlying type used to store the current value of the
     * quire.
     */
    using storage_type = fixed_point<1 + CarrySize + IntegerSize, FractionSize>;

    /**
     * @return A quire that represents the number zero.
     */
    [[nodiscard]] static constexpr quire zero();

    /**
     * @return A quire that represents the number one.
     */
    [[nodiscard]] static constexpr quire one();

    /**
     * @return A quire that represents NaR.
     */
    [[nodiscard]] static constexpr quire nar();

    /**
     * @brief Construct quire from fixed point value.
     *
     * @param bits The value to import as-is.
     */
    [[nodiscard]] static constexpr quire from(const storage_type& bits);

    /**
     * @brief Construct zero quire.
     *
     * Construct this quire to represent the number zero.
     */
    constexpr quire();

    /**
     * @brief Copy constructor.
     *
     * Construct object to be identical to other.
     */
    constexpr quire(const quire& other);

    /**
     * @brief Construct from posit.
     *
     * Construct a quire to match the value given by posit p.
     */
    constexpr quire(const posit<N, ES, WT>& p);

    /**
     * @brief Default destructor.
     */
    ~quire() = default;

    /**
     * @brief Assignment operator.
     */
    quire& operator=(const quire& other);

    /**
     * @brief Add rhs to this.
     */
    quire& operator+=(const quire& rhs);

    /**
     * @brief Subtract rhs from this.
     */
    quire& operator-=(const quire& rhs);

    /**
     * @brief Set this to this multiplied with rhs.
     */
    quire& operator*=(const quire& rhs);

    /**
     * @brief Set this to this divided by other.
     */
    quire& operator/=(const quire& rhs);

    /**
     * @brief Conversion to posit.
     *
     * Converts the quire back to a posit value.
     */
    [[nodiscard]] constexpr operator posit<N, ES, WT>() const;

    /**
     * @return Whether this quire represents the number zero.
     */
    [[nodiscard]] constexpr bool is_zero() const;

    /**
     * @return Whether this quire represents NaR.
     */
    [[nodiscard]] constexpr bool is_nar() const;

    /**
     * @return The underlying fixed point storage.
     */
    [[nodiscard]] constexpr const storage_type& get_bits() const;

protected:
    storage_type value;
};

//
// Quire Operators.
//
// Outside the class for easy automatic conversion of arguments.
//
// For implementation, look at posit/quire_operators.hpp
//

/**
 * @return Whether lhs and rhs are equal.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator==(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs);

/**
 * @true Whether lhs and rhs are not equal.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator!=(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs);

/**
 * @return Whether lhs is greater than rhs.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator>(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs);

/**
 * @return Whether lhs is greater or equal to rhs.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator>=(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs);

/**
 * @return Whether lhs is less than rhs.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator<(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs);

/**
 * @return Whether lhs is less or equal to rhs.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator<=(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs);

/**
 * @brief Quire Addition.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> operator+(const quire<N, ES, WT>& lhs,
                                                   const quire<N, ES, WT>& rhs);

/**
 * @brief Unary minus.
 *
 * If this quire represents NaR, the result is also NaR.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> operator-(const quire<N, ES, WT>& q);

/**
 * @brief Quire Subtraction.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> operator-(const quire<N, ES, WT>& lhs,
                                                   const quire<N, ES, WT>& rhs);

/**
 * @brief Quire Multiplication.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> operator*(const quire<N, ES, WT>& lhs,
                                                   const quire<N, ES, WT>& rhs);

/**
 * @brief Quire Division.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> operator/(const quire<N, ES, WT>& lhs,
                                                   const quire<N, ES, WT>& rhs);

/**
 * @brief Output a string representation of p to os.
 */
template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const quire<N, ES, WT>& q);

//
// String utilities for the quire class.
//
// For implementation, look at posit/quire_string_utils.hpp
//

/**
 * @brief Convert quire to printable bitstring.
 *
 * This function prints the underlying bitstring as a std::string of zeroes
 * and ones.
 *
 * @param q The quire from which to print the bitstring.
 */
template <size_t N, size_t ES, typename WT> std::string to_binary(const quire<N, ES, WT>& q);

//
// Interval Bound Enum
//

/**
 * @brief Enum that encodes the kind of bound, either open or closed.
 *
 * These bounds are used in our implementation of valids. Using an enum here
 * makes for easier to understand code, but in a hardware implementation,
 * this enum would be encoded with the u-bit of the valid.
 */
enum class interval_bound
{
    OPEN = 1,
    CLOSED = 2,
};

//
// Interval Bound Operators
//
// For implementation, look at posit/interval_bound_operators.hpp
//

/**
 * @brief Output a string representation of u to os.
 *
 * @param os The stream to write to.
 * @param u The enum to write.
 * @return A reference to os.
 */
std::ostream& operator<<(std::ostream& os, const interval_bound& u);

//
// Additional Interval Bound Operations
//
// For implementation, look at posit/interval_bound_operations.hpp
//

/**
 * @brief Negate the interval bound.
 *
 * Negating a closed bound returns an open bound and negating an open bound
 * returns a closed bound.
 */
[[nodiscard]] constexpr interval_bound negate(interval_bound u);

/**
 * @brief Return whether this interval bound is open.
 */
[[nodiscard]] constexpr bool is_open(interval_bound u);

/**
 * @brief Return whether this interval bound is closed.
 */
[[nodiscard]] constexpr bool is_closed(interval_bound u);

//
// Valid Class.
//
// For implementation, look at posit/valid.hpp
//

/**
 * @brief Valid Intervals.
 *
 * Valids represent a similar concept to SORNs or Unum Type-II numbers.  A
 * given valid consists of two posit endpoint with one uncertainty bit (u-bit)
 * each.  If the u-bit is set, the given bound is open, if it is not set, the
 * bound is closed.
 */
template <size_t N, size_t ES, typename WT = DefaultWordType> class valid
{
public:
    /**
     * @brief Underlying posit type.
     */
    using posit_type = posit<N, ES, WT>;

    /**
     * @brief Construct a new valid with given endpoints.
     *
     * Arguments are imported as-is into a new valid.
     */
    [[nodiscard]] static constexpr valid from(const posit_type& start_value,
                                              interval_bound start_bound,
                                              const posit_type& end_value,
                                              interval_bound end_bound);

    /**
     * @return Representation of the real number zero.
     */
    [[nodiscard]] static constexpr valid zero();

    /**
     * @return Representation of the real number one.
     */
    [[nodiscard]] static constexpr valid one();

    /**
     * @return Representation of the empty set.
     */
    [[nodiscard]] static constexpr valid empty();

    /**
     * @brief Return the real set.
     *
     * The real set is the valid that contains any real value, that is
     * it represents the interval (-∞, ∞).
     *
     * @return Representation of the real set.
     */
    [[nodiscard]] static constexpr valid all_reals();

    /**
     * @brief Return the full set.
     *
     * The full set is the valid that contains any possible tile on the number
     * circle.
     *
     * @return Representation of the full set.
     */
    [[nodiscard]] static constexpr valid full();

    /**
     * @return Representation of the NaR error state.
     */
    [[nodiscard]] static constexpr valid nar();

    /**
     * @return Representation of (maxpos, ∞).
     */
    [[nodiscard]] static constexpr valid max();

    /**
     * @return Representation of (-∞, -maxpos).
     */
    [[nodiscard]] static constexpr valid min();

    /**
     * @brief Default constructor.
     *
     * Constructs this valid to represent the concrete value 0.
     */
    constexpr valid();

    /**
     * @brief Copy constructor.
     */
    constexpr valid(const valid& other);

    /**
     * @brief Construct valid to represent given value exactly.
     */
    constexpr valid(const posit<N, ES, WT>& exact_value);

    /**
     * @brief Construct valid to represent given closed interval.
     *
     * @param start Start of the interval, inclusive.
     * @param end End of the interval, inclusive.
     */
    constexpr valid(const posit<N, ES, WT>& start, const posit<N, ES, WT>& end);

    /**
     * @brief Destructor.
     *
     * Here to fulfill the rule of three.
     */
    ~valid();

    /**
     * @brief Assign this valid to hold the value of other.
     *
     * @param other The value to change to.
     */
    valid& operator=(const valid& other);

    /**
     * @brief Compare this and other for equality.
     *
     * Two valids are equal if they represent the same set.
     */
    [[nodiscard]] constexpr bool operator==(const valid& other) const;

    /**
     * @brief Compare this and other for inequality.
     */
    [[nodiscard]] constexpr bool operator!=(const valid& other) const;

    /**
     * @brief Return whether this is less than other.
     *
     * Given two valids u, v, u < v means that every tile t in u is smaller
     * than any other tile s in v.
     */
    [[nodiscard]] constexpr bool operator<(const valid& other) const;

    /**
     * @brief Return whether this is less than or equal to other.
     */
    [[nodiscard]] constexpr bool operator<=(const valid& other) const;

    /**
     * @brief Return whether this is greater than other.
     */
    [[nodiscard]] constexpr bool operator>(const valid& other) const;

    /**
     * @brief Return whether this is greater than or equal to other.
     */
    [[nodiscard]] constexpr bool operator>=(const valid& other) const;

    /**
     * @brief Unary addition.
     *
     * @return A copy of argument other.
     */
    [[nodiscard]] constexpr valid operator+() const;

    /**
     * @brief Sum of two valids.
     *
     * @param rhs The valid to add to this valid.
     * @return The sum of this and rhs.
     */
    [[nodiscard]] constexpr valid operator+(const valid& other) const;

    /**
     * @brief Unary minus.
     *
     * Negating a valid {p, q} means returning valid {-q, p}. Note that
     * negation is not inversion! Negation is equivalent to multiplying the
     * valid with (-1) while inversion is a set operation.
     *
     * @return The negation of this valid.
     */
    [[nodiscard]] constexpr valid operator-() const;

    /**
     * @brief Valid subtraction.
     *
     * @param rhs The valid to add to this valid.
     * @return The sum of this and rhs.
     */
    [[nodiscard]] constexpr valid operator-(const valid& other) const;

    /**
     * @brief Return the product of this multiplied with rhs.
     */
    [[nodiscard]] constexpr valid operator*(const valid& other) const;

    /**
     * @brief Return this divided by other.
     */
    [[nodiscard]] constexpr valid operator/(const valid& other) const;

    /**
     * @return Whether this valid represents the number zero.
     */
    [[nodiscard]] constexpr bool is_zero() const;

    /**
     * @return Whether this valid represents the empty set.
     */
    [[nodiscard]] constexpr bool is_empty() const;

    /**
     * @return Whether this valid represents the real set (-∞, ∞).
     */
    [[nodiscard]] constexpr bool is_all_reals() const;

    /**
     * @return Whether this valid represents the full set of all tiles.
     */
    [[nodiscard]] constexpr bool is_full() const;

    /**
     * @return Whether this valid represents NaR.
     */
    [[nodiscard]] constexpr bool is_nar() const;

    /**
     * @return Whether this valid represents a posit exactly.
     */
    [[nodiscard]] constexpr bool is_exact_real() const;

    /**
     * @return Whether "value" is part of the interval represented by this valid.
     */
    [[nodiscard]] constexpr bool contains(const posit_type& value) const;

    /**
     * @brief Check if valid is regular.
     *
     * A valid is regular if it is a a proper interval on the reals with
     * starting point is less than the end point.
     *
     * In particular, neither NaR nor the empty set are regular.
     */
    [[nodiscard]] constexpr bool is_regular() const;

    /**
     * @brief Check if valid is regular.
     *
     * A valid is irregular exactly if it is not regular.
     */
    [[nodiscard]] constexpr bool is_irregular() const;

    /**
     * @brief Invert the valid.
     *
     * Inverting a valid v means returning a valid u that contains exactly
     * those tiles that v does not contain.
     *
     * @return The reverse set.
     */
    [[nodiscard]] constexpr valid inverse() const;

    /**
     * @brief Return the intervals start value.
     *
     * Given valid v = {p, q} in interval notation, this method returns p.
     */
    [[nodiscard]] const posit_type& get_start_value() const;

    /**
     * @brief Return the intervals end value.
     *
     * Given valid v = {p, q} in interval notation, this method returns q.
     */
    [[nodiscard]] const posit_type& get_end_value() const;

    /**
     * @brief Return the intervals start bound. Either open or closed.
     */
    [[nodiscard]] const interval_bound& get_start_bound() const;

    /**
     * @brief Return the intervals end bound. Either open or closed.
     */
    [[nodiscard]] const interval_bound& get_end_bound() const;

    /**
     * @brief Return exact posit value.
     *
     * If this valid represents some real exactly, this method returns p.  If
     * this valid does not represent a real exactly, this method throws an
     * exception. You should probably use is_exact_real() before calling this
     * method to ensure this is the case.
     *
     * Note that when this valid represents NaR exactly, this method still
     * throws an exception as NaR is not a real.
     *
     * @return The posit real value this valid represents exactly.
     * @throws std::logic_error If this valid does not represent a real exactly.
     */
    [[nodiscard]] constexpr posit_type as_exact_real() const;

    /**
     * @brief Return this valid represented in interval notation.
     *
     * Tile notation looks like {p, q} where p and q are the start or end
     * tiles and the curly braces are either replace with either closed "[]" or
     * open "()" bounds.
     */
    [[nodiscard]] std::string in_interval_notation() const;

    /**
     * @brief Return this valid represented in tile notation.
     *
     * Tile notation looks like {p; q} where p and q are the start or
     * end tiles.
     *
     * If endpoints are printed as-is (e.g. "0"), that endpoint is a closed
     * endpoint, that is the u-bit is not set. If endpoints are printed with a
     * little ᵘ (e.g. "0ᵘ"), that endpoint is an open endpoint, that is the
     * u-bit is set.
     */
    [[nodiscard]] std::string in_tile_notation() const;

protected:
    struct tile_ref
    {
        const posit_type& value;
        const interval_bound& bound;
    };

    class group_result
    {
    public:
        posit_type product;
        rounding_event rounding;
        interval_bound lhs_bound;
        interval_bound rhs_bound;

        constexpr group_result();
        constexpr group_result(const posit_type& p, rounding_event r, interval_bound lu,
                               interval_bound ru);
    };

    /**
     * @brief Start p of interval {p, q}.
     */
    posit_type start_value;

    /**
     * @brief The kind of start bound, either open or closed.
     */
    interval_bound start_bound;

    /**
     * @brief End q of interval {p, q}.
     */
    posit_type end_value;

    /**
     * @brief The kind of end bound, either open or closed.
     */
    interval_bound end_bound;

    /**
     * @brief Convert endpoint to tile notation.
     *
     * Helper used in valid::in_tile_notation() method.
     */
    [[nodiscard]] static std::string in_tile_notation(const posit_type& p, const interval_bound& u);

    [[nodiscard]] constexpr static group_result tile_mul(const tile_ref& lhs, const tile_ref& rhs);

    [[nodiscard]] const tile_ref start() const;
    [[nodiscard]] const tile_ref end() const;

    [[nodiscard]] static interval_bound merge_bounds_from(const group_result& group);
    [[nodiscard]] static bool lt_left(const group_result& lhs, const group_result& rhs);
    [[nodiscard]] static bool lt_right(const group_result& lhs, const group_result& rhs);
    [[nodiscard]] static bool lt(const group_result& lhs, const group_result& rhs, bool left);
};

//
// Additional Valid Operators.
//
// For implementations, look at posit/valid_operators.hpp.
//

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const valid<N, ES, WT>& v);

//
// Functions for iterating over unum types. Useful for testing and
// experiments.
//
// For implementations, look at posit/iter_tools.hpp
//

/**
 * @brief Run function on each posit for a given type.
 *
 * Runs function operation on every posit of PositType in some unspecified
 * order.
 *
 * @tparam PositType Instance of a posit type, e.g. posit<8, 2>.
 * @param operation The function to call on each posit.
 */
template <typename PositType>
inline void for_each_posit(const std::function<void(const PositType&)>& operation);

/**
 * @brief Run function on each valid for a given type.
 *
 * Runs function operation on every valid of ValidType in some unspecified
 * order.
 *
 * @note This function is exponential in valid width N.
 *
 * @tparam ValidType Instance of a valid type, e.g. valid<8, 2>.
 * @param operation The function to call on each valid.
 */
template <typename ValidType>
inline void for_each_valid(const std::function<void(const ValidType&)>& operation);

/**
 * @brief Run function on each regular valid for a given type.
 *
 * Runs function operation on every regular valid of ValidType in some
 * unspecified order. In particular, operation is never called on irregular
 * valids.
 *
 * @note This function is exponential in valid width N.
 *
 * @tparam ValidType Instance of a valid type, e.g. valid<8, 2>.
 * @param operation The function to call on each valid.
 */
template <typename ValidType>
inline void for_each_regular_valid(const std::function<void(const ValidType&)>& operation);

} // namespace aarith

//
// Now we include the headers. Because we have prototypes for all methods and
// functions, these imports can be in any order and reference each other as
// they wish.
//

#include <aarith/posit/errors.hpp>
#include <aarith/posit/interval_bound_operations.hpp>
#include <aarith/posit/interval_bound_operators.hpp>
#include <aarith/posit/iter_tools.hpp>
#include <aarith/posit/posit.hpp>
#include <aarith/posit/posit_casts.hpp>
#include <aarith/posit/posit_fraction.hpp>
#include <aarith/posit/posit_operations.hpp>
#include <aarith/posit/posit_operators.hpp>
#include <aarith/posit/posit_parameters.hpp>
#include <aarith/posit/posit_parameters_operations.hpp>
#include <aarith/posit/posit_string_utils.hpp>
#include <aarith/posit/posit_types.hpp>
#include <aarith/posit/quire.hpp>
#include <aarith/posit/quire_operators.hpp>
#include <aarith/posit/quire_sizes.hpp>
#include <aarith/posit/quire_string_utils.hpp>
#include <aarith/posit/quire_types.hpp>
#include <aarith/posit/rounding_event_operators.hpp>
#include <aarith/posit/valid.hpp>
#include <aarith/posit/valid_operators.hpp>
#include <aarith/posit/valid_types.hpp>
