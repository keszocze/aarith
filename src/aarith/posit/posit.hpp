#pragma once

#include <aarith/core/word_array.hpp>
#include <aarith/integer.hpp>
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

    constexpr posit(const posit& other)
        : bits(other.bits)
    {
        static_assert_template_parameters();
    }

    constexpr posit(const posit&& other)
        : bits(other.bits)
    {
        static_assert_template_parameters();
    }

    constexpr posit& operator=(const posit& other)
    {
        bits = other.bits;
        return *this;
    }

    constexpr posit& operator=(const posit&& other)
    {
        bits = other.bits;
        return *this;
    }

    /**
     * Construct this posit with given bits.
     *
     * @param n Bits used to initalize the underlying type.
     */
    constexpr explicit posit(WordType n)
        : bits(n)
    {
    }

    //
    // Comparison Operators
    //

    constexpr bool operator==(const posit& other) const
    {
        return bits == other.bits;
    }

    constexpr bool operator!=(const posit& other) const
    {
        return !(*this == other);
    }

    //
    // Constants
    //

    [[nodiscard]] static constexpr posit min()
    {
        // the minum value is represented by 10..01

        posit p;

        p.bits.set_bit(NBits - 1, true);
        p.bits.set_bit(0, true);

        return p;
    }

    [[nodiscard]] static constexpr posit minpos()
    {
        // minpos is presented by 0..01

        posit p;

        p.bits.set_bit(0, true);

        return p;
    }

    [[nodiscard]] static constexpr posit max()
    {
        // the maximum value is represented by 01..1

        posit p;

        p.bits = p.bits.all_ones();
        p.bits.set_bit(NBits - 1, false);

        return p;
    }

    [[nodiscard]] static constexpr posit zero()
    {
        // zero is represented by all bits set to zero

        return posit();
    }

    [[nodiscard]] static constexpr posit one()
    {
        // one is represented by 010...0

        posit p;

        p.bits.set_bit(NBits - 2, true);

        return p;
    }

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
     * Return the number of possible bit patterns this type can
     * take.
     */
    [[nodiscard]] static constexpr size_t npat()
    {
        // TODO
        return 0;
    }

    [[nodiscard]] static constexpr size_t useed()
    {
        // TODO
        return 0;
    }

    //
    // Getters
    //

    [[nodiscard]] constexpr bool is_negative() const
    {
        return get_sign_bit() == 1;
    }

    [[nodiscard]] constexpr storage_type get_sign_bit() const
    {
        return storage_type(bits.msb());
    }

    [[nodiscard]] constexpr storage_type get_bits() const
    {
        return bits;
    }

private:
    storage_type bits;

    constexpr void static_assert_template_parameters() const
    {
        static_assert(NBits >= 2, "number of bits needs to be at least 2");
    }
};
} // namespace aarith
