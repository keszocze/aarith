/**
 * Evaluating Posit Accuracy
 *
 * Based on previous work "Beating FLatoing Point at its Own Game: Posit
 * Arithmetic", 2017, pp. 79.
 */

#include <aarith/float.hpp>
#include <aarith/posit.hpp>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>

#include "support.hpp"

using namespace aarith;

template <typename X, typename Y> static double decimal_accuracy(const X& x, const Y& y)
{
    const double xd = static_cast<double>(x);
    const double yd = static_cast<double>(y);

    const double err = fabs(log10(xd / yd));

    if (std::isnan(err) || std::isinf(err))
    {
        return 0.0;
    }

    return err;
}

/**
 * Run the reciprocal test for posits.
 *
 * That is, given posit p, compute 1 / p and check whether the result had to
 * be rounded.
 */
template <typename Posit> static void reciprocal_posit_for()
{
    constexpr size_t N = Posit::width();
    constexpr size_t ES = Posit::exponent_size();

    std::cout << "====== Reciprocal (Posit) ======" << std::endl;
    std::cout << "N=" << N << std::endl;
    std::cout << "ES=" << ES << std::endl;

    uint64_t ok = 0;
    uint64_t bad = 0;
    double total_error = 0.0;

    for_each_posit<Posit>([&](const Posit& p) {
        // When evaluating 1 / p, there is no point in looking at the cases
        // where p is NaR or zero as the result will again be NaR.

        if (p.is_nar() || p.is_zero())
        {
            return;
        }

        // Check if the result was rounded.

        const auto [quot, r] = div(Posit::one(), p);

        if (is_not_rounded(r))
        {
            ok += 1;
        }
        else
        {
            bad += 1;
        }

        // Compute achived decimal accuracy. We use a high precision float
        // enviornment as comparison.

        const double x = static_cast<double>(p);
        const double expected = 1.0 / x;
        const double error = decimal_accuracy(quot, expected);
        total_error += error;
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    std::cout << "=== Summary ===" << std::endl;
    std::cout << "ok=" << ok << " p=" << ok_percent << std::endl;
    std::cout << "bad=" << bad << " p=" << bad_percent << std::endl;
    std::cout << "mean_error=" << mean_error << std::endl;
}

template <typename Float> static void reciprocal_float_for()
{
    constexpr size_t E = Float::exponent_width();
    constexpr size_t M = Float::mantissa_width();
    constexpr size_t N = 1 + E + M;

    std::cout << "====== Reciprocal (Float) ======" << std::endl;
    std::cout << "N=" << N << std::endl;
    std::cout << "E=" << E << std::endl;
    std::cout << "M=" << M << std::endl;

    uint64_t ok = 0;
    uint64_t bad = 0;
    double total_error = 0.0;

    for_each_float<Float>([&](const Float& f) {
        // When evaluating 1 / x, there is no point in looking at the cases
        // where x is NaN, Infty or zero.

        if (f.is_nan() || f.is_inf() || f.is_zero())
        {
            return;
        }

        const Float quot = Float::one() / f;

        // Compute achived decimal accuracy. We use a high precision float
        // enviornment as comparison.

        const double x = static_cast<double>(f);
        const double expected = 1.0 / x;
        const double error = decimal_accuracy(quot, expected);
        total_error += error;

        if (error == 0)
        {
            ok += 1;
        }
        else
        {
            bad += 1;
        }
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    std::cout << "=== Summary ===" << std::endl;
    std::cout << "ok=" << ok << " p=" << ok_percent << std::endl;
    std::cout << "bad=" << bad << " p=" << bad_percent << std::endl;
    std::cout << "mean_error=" << mean_error << std::endl;
}

int main()
{
    reciprocal_posit_for<posit<8, 1>>();
    reciprocal_posit_for<posit16>();

    reciprocal_float_for<floating_point<4, 3>>();
    reciprocal_float_for<half_precision>();
}
