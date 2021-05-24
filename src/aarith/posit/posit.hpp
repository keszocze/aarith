#pragma once

#include <aarith/posit.hpp>
#include <cassert>
#include <cstdint>
#include <cstdlib>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>
posit<N, ES, WT>::from(const posit<N, ES, WT>::storage_type& bits)
{
    posit p;
    p.bits = bits;

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::from(const WT first_word)
{
    const storage_type bits(first_word);
    return posit<N, ES, WT>::from(bits);
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit()
    : bits(0)
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(int8_t value)
    : posit(static_cast<int64_t>(value))
{
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(int16_t value)
    : posit(static_cast<int64_t>(value))
{
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(int32_t value)
    : posit(static_cast<int64_t>(value))
{
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(int64_t value)
    : posit(integer<64>(value))
{
}

template <size_t N, size_t ES, typename WT>
template <size_t ValueWidth, typename ValueWordType>
constexpr posit<N, ES, WT>::posit(const integer<ValueWidth, ValueWordType>& value)
{
    const posit_parameters<N, ES, WT> parameters(value);
    *this = posit(parameters);
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(uint8_t value)
    : posit(static_cast<uint64_t>(value))
{
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(uint16_t value)
    : posit(static_cast<uint64_t>(value))
{
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(uint32_t value)
    : posit(static_cast<uint64_t>(value))
{
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(uint64_t value)
    : posit(uinteger<64>(value))
{
}

template <size_t N, size_t ES, typename WT>
template <size_t ValueWidth, typename ValueWordType>
constexpr posit<N, ES, WT>::posit(const uinteger<ValueWidth, ValueWordType>& value)
    : posit(integer<ValueWidth + 1, ValueWordType>(value))
{
}

template <size_t N, size_t ES, typename WT> constexpr posit<N, ES, WT>::posit(double x)
{
    // first consider the two possible special cases, zero NaR

    if (x == 0.0)
    {
        *this = zero();
        return;
    }

    if (std::isnan(x) || std::isinf(x))
    {
        *this = nar();
        return;
    }

    // prepare some types and constants for later

    constexpr storage_type zero = storage_type::zero();
    constexpr storage_type one = storage_type::one();
    constexpr storage_type two = one + one;

    const double useed = std::pow(2, std::pow(2, ES));

    // in this function we build up the underlying bitstring "p" bit by bit
    // (as explained in Posit Arithmetic, Gustafson, October 2017, pp. 25)

    storage_type p;
    double y = std::abs(x);
    size_t i = 0;

    // (1) First divide by useed or multiply by useed until x is in the
    // interval [1, useed).

    if (y >= 1.0)
    {
        // north east

        p = one;

        i = 2;

        while (y >= useed && i < N)
        {
            p = (p << 1) + one;
            y = y / useed;

            i += 1;
        }

        p = p << 1;
        i += 1;
    }
    else
    {
        // south east

        p = zero;
        i = 1;

        while (y < 1.0 && i <= N)
        {
            y = y * useed;
            i += 1;
        }

        if (i >= N)
        {
            p = two;
            i = N + 1;
        }
        else
        {
            p = one;
            i += 1;
        }
    }

    // (2) Then the value is repeatedly divided by 2 until it is in the
    // interval [1, 2) and that determines the exponent.

    double e = std::pow(2, ES - 1);

    while (e > 1.0 / 2.0 && i <= N)
    {
        p = p << 1;

        if (y >= std::pow(2.0, e))
        {
            y /= std::pow(2.0, e);
            p = p + one;
        }

        e = e / 2.0;
        i += 1;
    }

    y = y - 1.0;

    // (3) The fraction always has a leading 1 to the left of the binary
    // point, eliminating the need to handle subnormal exception values that
    // have a 0 bit to the left of the binary point.

    while (y > 0.0 && i <= N)
    {
        y = y * 2.0;

        p = (p << 1);

        if (std::floor(y))
        {
            p = p + one;
        }

        y -= std::floor(y);
        i += 1;
    }

    p = p << N + 1 - i;
    i += 1;

    // (4) Round to nearest even and fill up the last bits.

    const auto isb = p.lsb();
    p = p >> one;

    if (isb)
    {
        if (y == 1 || y == 0)
        {
            if (p.lsb())
            {
                p = p + one;
            }
        }
        else
        {
            p = p + one;
        }
    }

    // (5) If we were converting a negative number, we now need
    // to take the 2s complement.

    if (x < 0.0)
    {
        p = negate(p);
    }

    // (6) Now that we have the bitstring, we can construct the underlying
    // bitstring

    bits = p;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(const posit& other)
    : bits(other.bits)
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(const posit&& other)
    : bits(std::move(other.bits))
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator=(const posit& other)
{
    bits = other.bits;
    return *this;
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator=(const posit&& other)
{
    bits = std::move(other.bits);
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator int8_t() const
{
    const int64_t converted = int64_t(*this);
    return static_cast<int8_t>(converted);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator int16_t() const
{
    const int64_t converted = int64_t(*this);
    return static_cast<int16_t>(converted);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator int32_t() const
{
    const int64_t converted = int64_t(*this);
    return static_cast<int32_t>(converted);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator int64_t() const
{
    // TODO (Schärtl): Find a better way for this...

    const integer<64> signed64 = integer<64>(*this);
    const uinteger<64> unsigned64 = signed64;
    const uint64_t unsigned_stdint = uint64_t(unsigned64);
    const int64_t signed_stdint = static_cast<int64_t>(unsigned_stdint);

    return signed_stdint;
}

template <size_t N, size_t ES, typename WT>
template <size_t TargetWidth, typename TargetWordType>
[[nodiscard]] constexpr posit<N, ES, WT>::operator integer<TargetWidth, TargetWordType>() const
{
    const posit_parameters<N, ES, WT> parameterized(*this);
    return integer<TargetWidth, TargetWordType>(parameterized);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator uint8_t() const
{
    const uint64_t converted = uint64_t(*this);
    return static_cast<uint8_t>(converted);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator uint16_t() const
{
    const uint64_t converted = uint64_t(*this);
    return static_cast<uint16_t>(converted);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator uint32_t() const
{
    const uint64_t converted = uint64_t(*this);
    return static_cast<uint32_t>(converted);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator uint64_t() const
{
    const uinteger<64> aarith_integer = uinteger<64>(*this);
    return uint64_t(aarith_integer);
}

template <size_t N, size_t ES, typename WT>
template <size_t TargetWidth, typename TargetWordType>
[[nodiscard]] constexpr posit<N, ES, WT>::operator uinteger<TargetWidth, TargetWordType>() const
{
    // convert uinteger to integer (w/ additional sign bit), run the standard
    // conversion and then clip away the sign bit

    const auto sresult = integer<TargetWidth + 1, TargetWordType>(*this);
    return width_cast<TargetWidth>(sresult);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator float() const
{
    const double ieee64 = static_cast<double>(*this);
    return static_cast<float>(ieee64);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>::operator double() const
{
    if (is_zero())
    {
        return 0.0;
    }

    if (is_nar())
    {
        return NAN;
    }

    const double sign = is_negative() ? -1.0 : 1.0;
    const double useed = std::pow(2.0, std::pow(2.0, ES));
    const double k = to_double(get_regime_value(*this));
    const double e = to_double(get_exponent_value(*this));

    const double F = to_double(get_fraction_value(*this));
    const double nF = get_num_fraction_bits(*this);
    const double f = (nF == 0.0) ? 1.0 : 1.0 + F / std::pow(2.0, nF);

    return sign * std::pow(useed, k) * std::pow(2, e) * f;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::operator==(const posit& other) const
{
    return bits == other.bits;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::operator!=(const posit& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::incremented() const
{
    const auto bits = get_bits();
    return from(bits + bits.one());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::decremented() const
{
    const auto bits = get_bits();
    return from(bits - bits.one());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::incremented_real() const
{
    if (is_nar())
    {
        return *this;
    }

    const auto increment = incremented();

    if (increment.is_nar())
    {
        return *this;
    }

    return increment;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::decremented_real() const
{
    if (is_nar())
    {
        return *this;
    }

    const auto decrement = decremented();

    if (decrement.is_nar())
    {
        return *this;
    }

    return decrement;
}

template <size_t N, size_t ES, typename WT>
constexpr bool posit<N, ES, WT>::operator<(const posit& other) const
{
    // special case

    if (is_nar() || other.is_nar())
    {
        return false;
    }

    // regular cases

    auto mybits = get_bits();
    auto otherbits = other.get_bits();

    if (mybits.msb() && otherbits.msb())
    {
        mybits = negate(mybits);
        otherbits = negate(otherbits);

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

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::operator<=(const posit& other) const
{
    return (*this == other) || (*this < other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::operator>(const posit& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::operator>=(const posit& other) const
{
    return (*this == other) || (*this > other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator+() const
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>
posit<N, ES, WT>::operator+(const posit<N, ES, WT>& rhs) const
{
    const auto [sum, rbit] = add(*this, rhs);
    return sum;
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator+=(const posit<N, ES, WT>& rhs)
{

    posit<N, ES, WT> sum = *this + rhs;
    this->bits = sum.get_bits();
    return *this;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator++()
{
    // ++x
    *this += this->one();
    return *this;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT> posit<N, ES, WT>::operator++(int)
{
    // x++
    auto copy = *this;
    *this += this->one();
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::operator-() const
{
    if (is_zero())
    {
        return *this;
    }

    if (is_nar())
    {
        return *this;
    }

    const auto bits = negate(this->get_bits());
    return posit<N, ES, WT>::from(bits);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>
posit<N, ES, WT>::operator-(const posit<N, ES, WT>& rhs) const
{
    return *this + (-rhs);
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator-=(const posit<N, ES, WT>& rhs)
{
    posit<N, ES, WT> sum = *this - rhs;
    this->bits = sum.get_bits();
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator--()
{
    // --x
    *this -= one();
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::operator--(int)
{
    // x--
    auto copy = *this;
    *this -= this->one();
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>
posit<N, ES, WT>::operator*(const posit<N, ES, WT>& rhs) const
{
    const auto [product, rbit] = mul(*this, rhs);
    return product;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator*=(const posit<N, ES, WT>& rhs)
{
    const auto product = *this * rhs;
    bits = product.get_bits();
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>
posit<N, ES, WT>::operator/(const posit<N, ES, WT>& rhs) const
{
    const auto [quot, rbit] = div(*this, rhs);
    return quot;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator/=(const posit<N, ES, WT>& rhs)
{
    const auto product = *this / rhs;
    bits = product.get_bits();
    return *this;
}

template <size_t N, size_t ES, typename WT> [[nodiscard]] constexpr size_t posit<N, ES, WT>::width()
{
    return N;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr size_t posit<N, ES, WT>::exponent_size()
{
    return ES;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::min()
{
    // the min value is represented by 10..01

    posit p;

    p.bits.set_bit(N - 1, true);
    p.bits.set_bit(0, true);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::minpos()
{
    // minpos is presented by 0..01

    posit p;

    p.bits.set_bit(0, true);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::max()
{
    // the maximum value is represented by 01..1

    posit p;

    p.bits = p.bits.all_ones();
    p.bits.set_bit(N - 1, false);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::zero()
{
    // zero is represented by all bits set to zero

    return posit();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::one()
{
    // one is represented by 010...0

    posit p;

    p.bits.set_bit(N - 2, true);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::nar()
{
    // NaR is sign bit set to one and all
    // other bits set to zero

    posit p;
    p.bits.set_bit(N - 1, true);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::pi()
{
    // This sucks because for big posit types (N >> 32), we will get
    // increasingly inaccurate results.
    return posit(M_PI);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::tau()
{
    return posit(2) * posit::pi();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr uinteger<N + 1, WT> posit<N, ES, WT>::npat()
{
    // to support arbitrary template parameters, we need to be able to
    // return arbitrary large sizes; so instead of size_t we use uinteger
    // as a return type here

    using SizeType = uinteger<N + 1, WT>;
    const SizeType one = SizeType::one();

    return one << N;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename posit<N, ES, WT>::useed_type posit<N, ES, WT>::useed()
{
    return useed_type(1 << (1 << ES));
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::is_non_negative() const
{
    return !is_nar() && !msb();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::is_negative() const
{
    return !is_nar() && msb();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::is_zero() const
{
    return (*this == zero());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::is_nar() const
{
    return (*this == nar());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename posit<N, ES, WT>::storage_type posit<N, ES, WT>::msb() const
{
    return storage_type(bits.msb());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename posit<N, ES, WT>::storage_type& posit<N, ES, WT>::get_bits() const
{
    return bits;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const integer<N, WT> posit<N, ES, WT>::get_signed_bits() const
{
    const integer<N, WT> signed_bits = this->bits;
    return signed_bits;
}

template <size_t N, size_t ES, typename WT>
template <size_t IN>
[[nodiscard]] auto posit<N, ES, WT>::as_fraction() const -> std::tuple<integer<IN>, integer<IN>>
{
    using Int = integer<IN>;
    using UInt = uinteger<IN>;

    //
    // Start by handling the two special cases, zero and NaR.
    //

    if (is_zero())
    {
        return {Int::zero(), Int::one()};
    }

    if (is_nar())
    {
        return {Int::zero(), Int::zero()};
    }

    //
    // Parameterize the posit. This is similar to the double conversion
    // operator code.
    //

    static constexpr UInt es = UInt(ES);

    const Int sign = is_negative() ? -Int::one() : Int::one();
    const Int useed = UInt::one() << (UInt::one() << es);
    const Int k = get_regime_value(*this);
    const Int e = get_exponent_value(*this);

    const Int F = get_fraction_value(*this);
    const Int nF = Int(get_num_fraction_bits(*this));

    const Int scale = get_scale_value(*this);

    //
    // Set fraction parts p, q. This is quite tricky, so we split it up in
    // four distinct cases.
    //

    Int p, q;

    if (scale >= scale.zero())
    {
        if (F.is_zero())
        {
            // We are looking at an integer value greater than zero.

            p = sign * Int(pow(useed, k)) * Int(UInt::one() << UInt(e));
            q = Int::one();
        }
        else
        {
            // We are looking at a non-negative value that is not accurately
            // representable by an integer.

            q = UInt::one() << UInt(nF);
            const Int p_base = sign * Int(pow(useed, k)) * Int(UInt::one() << UInt(e));
            p = p_base * q + p_base * F;
        }
    }
    else
    {
        if (F.is_zero())
        {
            // We are looking at a negative value that is perfectly representable by
            // a fraction -1 / n where n is a positive integer.

            p = sign * Int::one();
            q = UInt::one() << UInt(abs(scale));
        }
        else
        {
            // We are looking at a negative value w/ an unrounded nominator
            // greater than 1.

            const Int m = Int::one() << expanding_abs(scale);
            const Int n = Int::one() << expanding_abs(F);
            const Int o = m * n;

            p = sign * (n + n.one());
            q = o;
        }
    }

    //
    // We have computed p, q. Now simplify the fraction and return it.
    //

    const Int divisor = gcd(p, q);
    const Int pret = sign * abs(p / divisor);
    const Int qret = abs(q / divisor);

    return {pret, qret};
}

template <size_t N, size_t ES, typename WT>
template <size_t IN, typename IWT>
[[nodiscard]] constexpr double posit<N, ES, WT>::to_double(const uinteger<IN, IWT>& n)
{
    const uint64_t n64 = narrow_cast<uint64_t>(n);
    return static_cast<double>(n64);
}

template <size_t N, size_t ES, typename WT>
template <size_t IN, typename IWT>
[[nodiscard]] constexpr double posit<N, ES, WT>::to_double(const integer<IN, IWT>& n)
{
    const int64_t i64 = narrow_cast<int64_t>(n);
    return static_cast<double>(i64);
}

template <size_t N, size_t ES, typename WT>
constexpr void posit<N, ES, WT>::static_assert_template_parameters() const
{
    static_assert(N >= 2, "number of bits needs to be at least 2");
    static_assert(ES <= 5, "exponent unreasonably large");
}

} // namespace aarith
