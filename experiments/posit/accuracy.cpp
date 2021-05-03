/**
 * Evaluating Posit Accuracy
 *
 * Based on previous work "Beating Floating Point at its Own Game: Posit
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

/**
 * @brief Configuration
 *
 * Set this to true to only print human-readable overviews. Set it to false to
 * generate CSV for use in other tools.
 */
static constexpr bool PRINT_STATS = false;

template <typename Posit> static void reciprocal_posit_for()
{
    constexpr size_t N = Posit::width();
    constexpr size_t ES = Posit::exponent_size();

    if constexpr (PRINT_STATS)
    {
        std::cout << "====== Reciprocal (Posit) ======" << std::endl;
        std::cout << "N=" << N << std::endl;
        std::cout << "ES=" << ES << std::endl;
    }

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

        // Compute achieved decimal accuracy. We use a high precision float
        // environment as comparison.

        const double x = static_cast<double>(p);
        const double expected = 1.0 / x;
        const double error = decimal_accuracy(quot, expected);
        total_error += error;

        if (!PRINT_STATS)
        {
            std::cout << error << "\n";
        }
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    if constexpr (PRINT_STATS)
    {
        std::cout << "ok=" << ok << " p=" << ok_percent << std::endl;
        std::cout << "bad=" << bad << " p=" << bad_percent << std::endl;
        std::cout << "mean_error=" << mean_error << std::endl;
        std::cout << std::endl;
    }
}

template <typename Posit> static void square_posit_for()
{
    constexpr size_t N = Posit::width();
    constexpr size_t ES = Posit::exponent_size();

    if constexpr (PRINT_STATS)
    {
        std::cout << "====== Square (Posit) ======" << std::endl;
        std::cout << "N=" << N << std::endl;
        std::cout << "ES=" << ES << std::endl;
    }

    uint64_t ok = 0;
    uint64_t bad = 0;
    double total_error = 0.0;

    for_each_posit<Posit>([&](const Posit& p) {
        // When evaluating p * p, there is no point in looking at the case
        // where p is NaR as the result will again be NaR.

        if (p.is_nar())
        {
            return;
        }

        // Check if the result was rounded.

        const auto [square, r] = mul(p, p);

        if (is_not_rounded(r))
        {
            ok += 1;
        }
        else
        {
            bad += 1;
        }

        // Compute achieved decimal accuracy. We use a high precision float
        // environment as comparison.

        const double x = static_cast<double>(p);
        const double expected = x * x;
        const double error = decimal_accuracy(square, expected);
        total_error += error;

        if (!PRINT_STATS)
        {
            std::cout << error << "\n";
        }
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    if constexpr (PRINT_STATS)
    {
        std::cout << "ok=" << ok << " p=" << ok_percent << std::endl;
        std::cout << "bad=" << bad << " p=" << bad_percent << std::endl;
        std::cout << "mean_error=" << mean_error << std::endl;
        std::cout << std::endl;
    }
}

template <typename Float> static void reciprocal_float_for()
{
    constexpr size_t E = Float::exponent_width();
    constexpr size_t M = Float::mantissa_width();
    constexpr size_t N = 1 + E + M;

    if constexpr (PRINT_STATS)
    {
        std::cout << "====== Reciprocal (Float) ======" << std::endl;
        std::cout << "N=" << N << std::endl;
        std::cout << "E=" << E << std::endl;
        std::cout << "M=" << M << std::endl;
    }

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

        // Compute achieved decimal accuracy. We use a high precision float
        // environment as comparison.

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

        if (!PRINT_STATS)
        {
            std::cout << error << "\n";
        }
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    if constexpr (PRINT_STATS)
    {
        std::cout << "ok=" << ok << " p=" << ok_percent << std::endl;
        std::cout << "bad=" << bad << " p=" << bad_percent << std::endl;
        std::cout << "mean_error=" << mean_error << std::endl;
        std::cout << std::endl;
    }
}

template <typename Float> static void square_float_for()
{
    constexpr size_t E = Float::exponent_width();
    constexpr size_t M = Float::mantissa_width();
    constexpr size_t N = 1 + E + M;

    if constexpr (PRINT_STATS)
    {
        std::cout << "====== Square (Float) ======" << std::endl;
        std::cout << "N=" << N << std::endl;
        std::cout << "E=" << E << std::endl;
        std::cout << "M=" << M << std::endl;
    }

    uint64_t ok = 0;
    uint64_t bad = 0;
    double total_error = 0.0;

    for_each_float<Float>([&](const Float& f) {
        // When evaluating x * x, there is no point in looking at the cases
        // where x is NaN or Infty.

        if (f.is_nan() || f.is_inf())
        {
            return;
        }

        const Float square = f * f;

        // Compute achieved decimal accuracy. We use a high precision float
        // environment as comparison.

        const double x = static_cast<double>(f);
        const double expected = x * x;
        const double error = decimal_accuracy(square, expected);
        total_error += error;

        if (error == 0)
        {
            ok += 1;
        }
        else
        {
            bad += 1;
        }

        if (!PRINT_STATS)
        {
            std::cout << error << "\n";
        }
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    if constexpr (PRINT_STATS)
    {
        std::cout << "ok=" << ok << " p=" << ok_percent << std::endl;
        std::cout << "bad=" << bad << " p=" << bad_percent << std::endl;
        std::cout << "mean_error=" << mean_error << std::endl;
        std::cout << std::endl;
    }
}

int main()
{
    using P8 = posit<8, 1>;
    using P16 = posit16;

    using F8 = floating_point<4, 3>;
    using F16 = half_precision;

    // reciprocal_posit_for<P8>();
    // reciprocal_posit_for<P16>();
    // reciprocal_float_for<F8>();
    // reciprocal_float_for<F16>();
    // square_posit_for<P8>();
    // square_posit_for<P16>();
    // square_float_for<F8>();
    // square_float_for<F16>();
}
