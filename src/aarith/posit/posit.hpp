#pragma once

#include <aarith/core/word_array.hpp>
#include <aarith/integer.hpp>
#include <cassert>
#include <cstdint>
#include <cstdlib>

namespace aarith {
/**
 * Represents a posit number that can represent real values. Based on
 * the description by John L. Gustafson ("Posit Arithmetic", 10
 * October 2017.)
 *
 * @tparam NBits The total width in bits of the given Posit.
 * @tparam ExponentSize The maximum width in bits of the exponent for
 * a given Posit.
 */
template <size_t NBits, size_t ExponentSize, class WordType = uint64_t> class posit
{
public:
    using storage_type = uinteger<NBits, WordType>;

    //
    // Constructors
    //

    /**
     * Construct this posit initalized to zero.
     */
    constexpr posit()
        : bits(0)
    {
        static_assert_template_parameters();
    }

    /**
     * Construct this posit to be a clone of other.
     *
     * @param other The posit to clone.
     */
    constexpr posit(const posit& other)
        : bits(other.bits)
    {
        static_assert_template_parameters();
    }

    /**
     * Construct this posit to be a clone of other.
     *
     * @param other The posit to clone.
     */
    constexpr posit(const posit&& other)
        : bits(std::move(other.bits))
    {
        static_assert_template_parameters();
    }

    /**
     * Assign this posit to hold the value of other.
     *
     * @param The value to change to.
     */
    constexpr posit& operator=(const posit& other)
    {
        bits = other.bits;
        return *this;
    }

    /**
     * Assign this posit to hold the value of other.
     *
     * @param The value to change to.
     */
    constexpr posit& operator=(const posit&& other)
    {
        bits = std::move(other.bits);
        return *this;
    }

    /**
     * Construct this posit with given bits.
     *
     * @param n Bits used to initialize the underlying type.
     */
    constexpr explicit posit(WordType n)
        : bits(n)
    {
    }

    //
    // Comparison Operators
    //

    /**
     * @brief Compare this and other for equality.
     *
     * @return true if this and other represent the same value. Otherwise
     * returns true.
     */
    constexpr bool operator==(const posit& other) const
    {
        return bits == other.bits;
    }

    /**
     * @brief Compare this and other for inequality.
     *
     * @return true if this and other represent different values. Otherwise
     * returns false.
     */
    constexpr bool operator!=(const posit& other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Return whether this is less than other.
     *
     * If this or other represent complex infinity, this method returns false.
     */
    constexpr bool operator<(const posit& other) const
    {
        // special case complex infinity

        const auto inf = complex_infinity();

        if (*this == inf || other == inf)
        {
            return false;
        }

        // regular cases

        auto mybits = get_bits();
        auto otherbits = other.get_bits();

        if (mybits.msb() && otherbits.msb())
        {
            mybits = twos_complement(mybits);
            otherbits = twos_complement(otherbits);

            return mybits > otherbits;
        }
        else if ((!mybits.msb()) && (!otherbits.msb()))
        {
            return mybits < otherbits;
        }
        else if (mybits.msb() && (!otherbits.msb()))
        {
            return true;
        }
        else
        {
            assert((!mybits.msb()) && otherbits.msb());
            return false;
        }
    }

    /**
     * @brief Return whether this is less than or equal to other.
     *
     * If this or other represent complex infinity, this method returns false.
     */
    constexpr bool operator<=(const posit& other) const
    {
        return (*this < other) || (*this == other);
    }

    /**
     * @brief Return whether this is greater than other.
     *
     * If this or other represent complex infinity, this method returns false.
     */
    constexpr bool operator>(const posit& other) const
    {
        return other < *this;
    }

    /**
     * @brief Return whether this is greater than or equal to other.
     *
     * If this or other represent complex infinity, this method returns false.
     */
    constexpr bool operator>=(const posit& other) const
    {
        return (*this > other) || (*this == other);
    }

    //
    // Constants
    //

    /**
     * Return the minimum value of this posit type. The minimum is the
     * negative posit that has the greatest magnitude.
     *
     * @return The smallest representable posit that represents a real number.
     */
    [[nodiscard]] static constexpr posit min()
    {
        // the min value is represented by 10..01

        posit p;

        p.bits.set_bit(NBits - 1, true);
        p.bits.set_bit(0, true);

        return p;
    }

    /**
     * Return the smallest representable positive value that is not zero.
     *
     * @return Smallest positive non-zero posit that represents a real number.
     */
    [[nodiscard]] static constexpr posit minpos()
    {
        // minpos is presented by 0..01

        posit p;

        p.bits.set_bit(0, true);

        return p;
    }

    /**
     * Return the maximum value of this posit type. The maximum is the
     * positive posit that has the greatest magnitude.
     *
     * @return The biggest representable posit that represents a real number.
     */
    [[nodiscard]] static constexpr posit max()
    {
        // the maximum value is represented by 01..1

        posit p;

        p.bits = p.bits.all_ones();
        p.bits.set_bit(NBits - 1, false);

        return p;
    }

    /**
     * @return Representation of the real number zero.
     */
    [[nodiscard]] static constexpr posit zero()
    {
        // zero is represented by all bits set to zero

        return posit();
    }

    /**
     * @return Representation of the real number one.
     */
    [[nodiscard]] static constexpr posit one()
    {
        // one is represented by 010...0

        posit p;

        p.bits.set_bit(NBits - 2, true);

        return p;
    }

    /**
     * @return Representation of complex infinity.
     */
    [[nodiscard]] static constexpr posit complex_infinity()
    {
        // complex infinity is sign bit set to one and all
        // other bits set to zero

        posit p;
        p.bits.set_bit(NBits - 1, true);

        return p;
    }

    //
    // Posit-Specific Constants
    //

    /**
     * @brief Return the npat for this type as defined in unum literature.
     *
     * @return The number of possible bit patterns this type can take.
     */
    [[nodiscard]] static constexpr uinteger<NBits + 1, WordType> npat()
    {
        // to support arbitrary template parameters, we need to be able to
        // return arbitrary large sizes; so instead of size_t we use uinteger
        // as a return type here

        using SizeType = uinteger<NBits + 1, WordType>;
        const SizeType one = SizeType::one();

        return one << NBits;
    }

    /**
     * @brief Return the useed for this type as used in official unum
     * literature.
     *
     * @ The useed value, which is 2 to the power of 2 to the power of the
     * exponent size.
     */
    [[nodiscard]] static constexpr size_t useed()
    {
        throw std::logic_error("not implemented");
        return 0;
    }

    //
    // Getters
    //

    /**
     * @return Whether this posit represents a negative real number.
     */
    [[nodiscard]] constexpr bool is_negative() const
    {
        if (*this == complex_infinity())
        {
            return false;
        }

        return get_sign_bit() == storage_type(1);
    }

    /**
     * @return The sign bit. The returned integer is either set to 0 or 1.
     */
    [[nodiscard]] constexpr storage_type get_sign_bit() const
    {
        return storage_type(bits.msb());
    }

    /**
     * @return The underlying storage.
     */
    [[nodiscard]] constexpr storage_type get_bits() const
    {
        return bits;
    }

private:
    storage_type bits;

    /**
     * Statically assert that the created instance of a given posit
     * conforms to the requirements for legal posits.
     *
     * We call this method in the constructor so it is guaranteed
     * to get generated on template instantiation.
     */
    constexpr void static_assert_template_parameters() const
    {
        static_assert(NBits >= 2, "number of bits needs to be at least 2");
    }
};
} // namespace aarith
