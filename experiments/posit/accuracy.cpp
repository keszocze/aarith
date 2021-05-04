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
#include <fstream>
#include <iostream>
#include <ostream>

#include "support.hpp"

using namespace aarith;

/**
 * @brief Configuration
 *
 * Set this to true to only print human-readable overviews. Set it to false to
 * generate CSV for use in other tools.
 */
static constexpr bool PRINT_STATS = false;

template <typename Posit> static void reciprocal_posit_for(std::ostream &os)
{
    constexpr size_t N = Posit::width();
    constexpr size_t ES = Posit::exponent_size();

    if constexpr (PRINT_STATS)
    {
        os << "====== Reciprocal (Posit) ======" << std::endl;
        os << "N=" << N << std::endl;
        os << "ES=" << ES << std::endl;
    }

    if constexpr (!PRINT_STATS)
    {
        os << "i;x;actual;expected;error;\n";
    }

    uint64_t i = 0;
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
            os << i << ";";
            os << p << ";";
            os << quot << ";";
            os << expected << ";";
            os << error << ";\n";
        }

	i += 1;
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    if constexpr (PRINT_STATS)
    {
        os << "ok=" << ok << " p=" << ok_percent << std::endl;
        os << "bad=" << bad << " p=" << bad_percent << std::endl;
        os << "mean_error=" << mean_error << std::endl;
        os << std::endl;
    }
}

template <typename Posit> static void reciprocal_posit_for(const std::string &filepath)
{
    std::ofstream os(filepath);
    reciprocal_posit_for<Posit>(os);
}

template <typename Posit> static void square_posit_for(std::ostream &os)
{
    constexpr size_t N = Posit::width();
    constexpr size_t ES = Posit::exponent_size();

    if constexpr (PRINT_STATS)
    {
        os << "====== Square (Posit) ======" << std::endl;
        os << "N=" << N << std::endl;
        os << "ES=" << ES << std::endl;
    }

    if constexpr (!PRINT_STATS)
    {
        os << "i;x;actual;expected;error;\n";
    }

    uint64_t i = 0;
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
            os << i << ";";
            os << p << ";";
            os << square << ";";
            os << expected << ";";
            os << error << ";\n";
        }

	i += 1;
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    if constexpr (PRINT_STATS)
    {
        os << "ok=" << ok << " p=" << ok_percent << std::endl;
        os << "bad=" << bad << " p=" << bad_percent << std::endl;
        os << "mean_error=" << mean_error << std::endl;
        os << std::endl;
    }
}

template <typename Posit> static void square_posit_for(const std::string& filepath)
{
    std::ofstream os(filepath);
    square_posit_for<Posit>(os);
}

template <typename Float> static void reciprocal_float_for(std::ostream &os)
{
    constexpr size_t E = Float::exponent_width();
    constexpr size_t M = Float::mantissa_width();
    constexpr size_t N = 1 + E + M;

    if constexpr (PRINT_STATS)
    {
        os << "====== Reciprocal (Float) ======" << std::endl;
        os << "N=" << N << std::endl;
        os << "E=" << E << std::endl;
        os << "M=" << M << std::endl;
    }

    if constexpr (!PRINT_STATS)
    {
        os << "i;x;actual;expected;error;\n";
    }

    uint64_t i = 0;
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
            os << i << ";";
            os << f << ";";
            os << quot << ";";
            os << expected << ";";
            os << error << ";\n";
        }

	i += 1;
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    if constexpr (PRINT_STATS)
    {
        os << "ok=" << ok << " p=" << ok_percent << std::endl;
        os << "bad=" << bad << " p=" << bad_percent << std::endl;
        os << "mean_error=" << mean_error << std::endl;
        os << std::endl;
    }
}

template <typename Float> static void reciprocal_float_for(const std::string& filepath)
{
    std::ofstream os(filepath);
    reciprocal_float_for<Float>(os);
}

template <typename Float> static void square_float_for(std::ostream &os)
{
    constexpr size_t E = Float::exponent_width();
    constexpr size_t M = Float::mantissa_width();
    constexpr size_t N = 1 + E + M;

    if constexpr (PRINT_STATS)
    {
        os << "====== Square (Float) ======" << std::endl;
        os << "N=" << N << std::endl;
        os << "E=" << E << std::endl;
        os << "M=" << M << std::endl;
    }

    if constexpr (!PRINT_STATS)
    {
        os << "i;x;actual;expected;error;\n";
    }

    uint64_t i = 0;
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
            os << i << ";";
            os << f << ";";
            os << square << ";";
            os << expected << ";";
            os << error << ";\n";
        }

	i += 1;
    });

    const auto total = ok + bad;
    const double ok_percent = static_cast<double>(ok) / total;
    const double bad_percent = static_cast<double>(bad) / total;
    const double mean_error = total_error / total;

    if constexpr (PRINT_STATS)
    {
        os << "ok=" << ok << " p=" << ok_percent << std::endl;
        os << "bad=" << bad << " p=" << bad_percent << std::endl;
        os << "mean_error=" << mean_error << std::endl;
        os << std::endl;
    }
}

template <typename Float> static void square_float_for(const std::string &filepath)
{
    std::ofstream os(filepath);
    square_float_for<Float>(os);
}

int main()
{
    using P8 = posit<8, 1>;
    using P16 = posit16;

    using F8 = floating_point<4, 3>;
    using F16 = half_precision;

    std::ostream& os = std::cout;

    reciprocal_posit_for<P8>("posit8-reciprocal-accuracy.csv");
    reciprocal_posit_for<P16>("posit16-reciprocal-accuracy.csv");
    reciprocal_float_for<F8>("float8-reciprocal-accuracy.csv");
    reciprocal_float_for<F16>("float16-reciprocal-accuracy.csv");
    square_posit_for<P8>("posit8-square-accuracy.csv");
    square_posit_for<P16>("posit16-square-accuracy.csv");
    square_float_for<F8>("float8-square-accuracy.csv");
    square_float_for<F16>("float16-square-accuracy.csv");
}
