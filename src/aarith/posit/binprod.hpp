#pragma once

#include <aarith/posit.hpp>
#include <cstdlib>
#include <stdexcept>
#include <tuple>

namespace aarith {

template <size_t N>
constexpr binprod<N>::binprod(const storage_type& x, const storage_type& m)
    : x(x)
    , m(m)
{
}

template <size_t N>
template <size_t PES, typename PWT>
constexpr binprod<N>::binprod(const posit<N, PES, PWT>& p)
{
    // based on "Provably Correct Posit Arithmetic with Fixed-Point Big
    // Integer", Shin Yee Chung, 2018, p. 3

    if (p.is_zero())
    {
        x = x.zero();
        m = m.one();

        return;
    }

    if (p.is_nar())
    {
        throw std::invalid_argument("p has to represent a real value");
    }

    // compute fraction part

    constexpr auto one = storage_type::one();
    constexpr auto two = one + one;

    const auto s = storage_type(get_sign_value(p));
    const auto h = storage_type(get_num_fraction_bits(p));
    const auto f = storage_type(get_fraction_value(p));

    x = pow(-one, s) * (pow(two, h) + f);

    // compute scale

    constexpr auto es = storage_type(PES);
    const auto k = storage_type(get_regime_value(p));
    const auto e = storage_type(get_exponent_value(p));

    m = pow(two, es) * k + e - h;
}

template <size_t N>
template <size_t PN, size_t PES, typename PWT>
[[nodiscard]] posit<PN, PES, PWT> binprod<N>::to_posit() const
{
    static_assert(PN == N);

    // based on "Provably Correct Posit Arithmetic with Fixed-Point Big
    // Integer", Shin Yee Chung, 2018, p. 4

    using Posit = posit<PN, PES, PWT>;
    // using PositPlusOne = posit<PN + 1, PES, PWT>;
    using Storage = uinteger<PN>;

    //
    // handling zero
    //

    if (x.is_zero())
    {
        return Posit::zero();
    }

    //
    // negative values are handled by looking at their absolute value
    // and then applying the twos complement at the end
    //

    if (x < x.zero())
    {

        const binprod absprod(-x, m);
        return -absprod.to_posit<PN, PES, PWT>();
    }

    ensure_positive();

    //
    // handling limits
    //

    constexpr auto max = binprod(Posit::max()).magnitude();
    const auto my_magnitude = magnitude();

    if (my_magnitude > max)
    {
        std::cout << "[falling back to max maginitude]" << std::endl;
        return Posit::max();
    }

    //
    // converting to posit, one bit at a time
    //

    const auto [h, f, k, e] = decode<PN, PES, PWT>();

    Storage bits;
    ssize_t i = PN - 2;

    //
    // (1) write regime bits; because the regime uses a run length
    // encoding we first have to calculate the number of regime bits and
    // then fill them in
    //

    {
        ssize_t nregime = 0;
        bool first_regime = false;

        decode_regime<PN, PES, PWT>(nregime, first_regime);

        for (ssize_t ridx = 0; ridx < nregime && i >= 0; ++ridx, --i)
        {
            const bool last_regime_bit = (ridx == (nregime - 1));

            if (last_regime_bit)
            {
                bits.set_bit(i, !first_regime);
            }
            else
            {
                bits.set_bit(i, first_regime);
            }
        }
    }

    //
    // (2) write exponent bits; here we can just copy the decoded exponent
    // integer
    //

    {
        const ssize_t nexp = std::min(static_cast<ssize_t>(PES), i + 1);

        for (ssize_t eidx = nexp - 1; eidx >= 0 && i >= 0; --eidx, --i)
        {
            const auto exponent_bit = e.bit(eidx);
            bits.set_bit(i, exponent_bit);
        }
    }

    //
    // (3) write fraction bits; again, we can just copy the decoded
    // integer into the bitstring
    //

    {
        const ssize_t nfrac = static_cast<size_t>(uinteger<16 * N>(h));

        for (ssize_t fidx = nfrac - 1; fidx >= 0 && i >= 0; --fidx, --i)
        {
            const auto fraction_bit = f.bit(fidx);
            bits.set_bit(i, fraction_bit);
        }
    }

    //
    // (4) now for the hard part, rounding...
    //

    {
        // const Posit p = Posit::from_bits(bits);
        // const PositPlusOne X = width_cast<PositPlusOne>(p);
        // const PositPlusOne u = width_cast<PositPlusOne>(X.decremented());
        // const PositPlusOne v = width_cast<PositPlusOne>(X.incremented());

        //(void) u;
        //(void) v;
    }

    //
    // (5) construct posit from bits
    //

    return Posit::from_bits(bits);
}

template <size_t N>
template <size_t M>
[[nodiscard]] constexpr ssize_t binprod<N>::to_ssize_t(const integer<M>& n)
{
    const uinteger<M> ucopy(n);

    if (n >= n.zero())
    {
        return static_cast<size_t>(ucopy);
    }
    else
    {
        const auto tc = twos_complement(ucopy);
        return (-1) * static_cast<ssize_t>(static_cast<size_t>(tc));
    }
}

template <size_t N>
template <size_t PN, size_t PES, typename PWT>
[[nodiscard]] constexpr typename binprod<N>::param_type binprod<N>::decode() const
{
    ensure_positive();

    constexpr auto one = storage_type::one();
    constexpr auto two = one + one;
    constexpr auto es = storage_type(PES);
    const auto h = ilog(abs(x), two);
    const auto f = abs(x) - pow(two, h);
    const auto e = absmod(m + h, pow(two, es));

    // this differs from how k is defined in the paper; I'm not sure if
    // the notation in the paper is awkward or if it is in fact an error
    // in the original publication

    const auto k = (m + h - e) / pow(two, es);

    assert(h >= h.zero());
    assert(f >= f.zero());
    assert(e >= e.zero());

    return {h, f, k, e};
}

template <size_t N>
template <size_t PN, size_t PES, typename PWT>
void binprod<N>::decode_regime(ssize_t& nregime, bool& first_regime) const
{
    ensure_positive();

    const auto [dh, df, dk, de] = decode<PN, PES, PWT>();
    const ssize_t k = binprod::to_ssize_t<2 * N>(dk);

    if (dk < dk.zero())
    {
        nregime = std::abs(k) + 1;
        first_regime = false;
    }
    else
    {
        nregime = k + 2;
        first_regime = true;
    }
}

template <size_t N> constexpr typename binprod<N>::storage_type binprod<N>::magnitude() const
{
    constexpr auto two = storage_type::one() + storage_type::one();
    return x * pow(two, m);
}

template <size_t N> void binprod<N>::ensure_positive() const
{
    if (x < x.zero())
    {
        throw std::logic_error("internal binprod methods only support positive values");
    }
}

} // namespace aarith
