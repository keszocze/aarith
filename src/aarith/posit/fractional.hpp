#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT> fractional<N, ES, WT>::zero()
{
    return fractional<N, ES, WT>();
}

template <size_t N, size_t ES, typename WT>
constexpr fractional<N, ES, WT>::fractional()
    : truncated(false)
{
}

template <size_t N, size_t ES, typename WT>
constexpr fractional<N, ES, WT>::fractional(const fractional<N, ES, WT>& other)
    : bits(other.bits)
    , truncated(other.truncated)
{
}

template <size_t N, size_t ES, typename WT>
constexpr fractional<N, ES, WT>::fractional(const posit<N, ES, WT>& p)
{
    // get fraction bits

    uinteger<N, WT> extracted = get_fraction_value(p);

    // shift all the way to the left

    if (!extracted.is_zero())
    {
        const size_t nfrac = get_num_fraction_bits(p);
        extracted = extracted << (extracted.width() - nfrac);
    }

    // copy into our bits

    import_fraction_bits(extracted);

    // make hidden bit explicit

    bits.set_bit(HiddenBitIndex, true);

    // starting out fresh, truncated will be set to false
    // as we have done no shift operations so far

    truncated = false;
}

template <size_t N, size_t ES, typename WT>
constexpr fractional<N, ES, WT>::fractional(
    const uinteger<fractional<N, ES, WT>::FractionSize, WT>& frac)
{
    import_fraction_bits(frac);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool fractional<N, ES, WT>::has_been_truncated() const
{
    return truncated;
}

template <size_t N, size_t ES, typename WT> void fractional<N, ES, WT>::set_not_truncated()
{
    truncated = false;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool fractional<N, ES, WT>::is_zero() const
{
    return (*this == zero());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT> fractional<N, ES, WT>::incremented() const
{
    fractional<N, ES, WT> ret;
    ret.bits = bits + bits.one();

    return ret;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr uinteger<fractional<N, ES, WT>::IntegerSize, WT>
fractional<N, ES, WT>::integer_bits() const
{
    return width_cast<IntegerSize>(bits >> FractionSize);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr uinteger<fractional<N, ES, WT>::FractionSize, WT>
fractional<N, ES, WT>::fraction_bits() const
{
    return width_cast<FractionSize>(bits);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr uinteger<fractional<N, ES, WT>::ScratchSize, WT>
fractional<N, ES, WT>::scratch_bits() const
{
    return bits;
}

template <size_t N, size_t ES, typename WT>
fractional<N, ES, WT>& fractional<N, ES, WT>::operator=(const fractional<N, ES, WT>& other)
{
    bits = other.bits;
    truncated = other.truncated;

    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool
fractional<N, ES, WT>::operator==(const fractional<N, ES, WT>& other) const
{
    return bits == other.bits;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool
fractional<N, ES, WT>::operator!=(const fractional<N, ES, WT>& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool
fractional<N, ES, WT>::operator<(const fractional<N, ES, WT>& other) const
{
    return bits < other.bits;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool
fractional<N, ES, WT>::operator<=(const fractional<N, ES, WT>& other) const
{
    return (*this < other) || (*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool
fractional<N, ES, WT>::operator>(const fractional<N, ES, WT>& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool
fractional<N, ES, WT>::operator>=(const fractional<N, ES, WT>& other) const
{
    return (*this > other) || (*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT>
fractional<N, ES, WT>::operator+(const fractional& other) const
{
    fractional<N, ES, WT> f;

    f.bits = bits + other.bits;
    f.truncated = truncated || other.truncated;

    return f;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT>
fractional<N, ES, WT>::operator-(const fractional& other) const
{
    fractional<N, ES, WT> f;

    f.bits = bits - other.bits;
    f.truncated = truncated || other.truncated;

    return f;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT>
fractional<N, ES, WT>::operator*(const fractional& other) const
{
    //
    // Start by computing the product of the two fractions. We
    // need enough scratch space so we don't lose precision.
    //
    // When computing the product p = n * m of two unsigned
    // integers n and m of length l, the resulting p will require
    // at most
    //
    //   ⌈ log₂(p) ⌉ = 2l
    //
    // bits to represent the product.
    //

    constexpr size_t multiply_scratch_size = 2 * ScratchSize;

    const auto lhs = width_cast<multiply_scratch_size>(bits);
    const auto rhs = width_cast<multiply_scratch_size>(other.bits);

    const auto product = lhs * rhs;
    const auto truncated_product_bits = width_cast<FractionSize>(product);
    const auto scaled = product >> FractionSize;

    //
    // Now we can write the result back into a fractional value.
    //

    fractional<N, ES, WT> f;

    f.bits = width_cast<ScratchSize>(scaled);
    f.truncated = truncated_product_bits || truncated || other.truncated;

    return f;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT>
fractional<N, ES, WT>::operator<<(const size_t shift) const
{
    const auto truncated_bits = bits >> (N - shift);

    fractional<N, ES, WT> f;
    f.bits = bits << shift;
    f.truncated = truncated || truncated_bits;

    return f;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT>
fractional<N, ES, WT>::operator<<(const uinteger<N, WT>& shift) const
{
    constexpr auto n = uinteger<N, WT>(N);
    const auto truncated_bits = bits >> (n - shift);

    fractional<N, ES, WT> f;
    f.bits = bits << shift;
    f.truncated = truncated || truncated_bits;

    return f;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT>
fractional<N, ES, WT>::operator>>(const size_t shift) const
{
    const auto truncated_bits = bits << (N - shift);

    fractional<N, ES, WT> f;
    f.bits = bits >> shift;
    f.truncated = truncated || truncated_bits;

    return f;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr fractional<N, ES, WT>
fractional<N, ES, WT>::operator>>(const uinteger<N, WT>& shift) const
{
    constexpr auto n = uinteger<N, WT>(N);
    const auto truncated_bits = bits << (n - shift);

    fractional<N, ES, WT> f;
    f.bits = bits >> shift;
    f.truncated = truncated || truncated_bits;

    return f;
}

template <size_t SN, size_t SES, typename SWT>
std::ostream& operator<<(std::ostream& os, const fractional<SN, SES, SWT>& f)
{
    os << to_binary(f.integer_bits());
    os << ".";
    os << to_binary(f.fraction_bits());

    return os;
}

template <size_t N, size_t ES, typename WT>
template <size_t IN, typename IWT>
void fractional<N, ES, WT>::import_fraction_bits(const uinteger<IN, IWT>& fraction_bits)
{
    static_assert(FractionSize >= fraction_bits.width(), "unexpected displacement");

    constexpr size_t displacement = FractionSize - fraction_bits.width();
    const auto extended = width_cast<ScratchSize>(fraction_bits);

    bits = extended << displacement;
}

} // namespace aarith
