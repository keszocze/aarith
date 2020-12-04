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
        assert_template_parameters();
    }

    constexpr posit(const posit& other)
        : bits(other.bits)
    {
    }

    constexpr posit(const posit&& other)
        : bits(other.bits)
    {
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
    // Constants
    //

    [[nodiscard]] static constexpr posit min()
    {
        // TODO
    }

    [[nodiscard]] static constexpr posit max()
    {
        // TODO
    }

    [[nodiscard]] static constexpr posit zero()
    {
        // TODO
    }

    [[nodiscard]] static constexpr posit one()
    {
        // TODO
    }

    [[nodiscard]] static constexpr posit complex_infinity()
    {
        // TODO
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

    constexpr void assert_template_parameters() const
    {
        static_assert(NBits >= 2, "number of bits needs to be at least 2");
    }
};
} // namespace aarith
