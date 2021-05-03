#include <aarith/float.hpp>
#include <aarith/float/numeric_limits.hpp>

#include <catch.hpp>
#include <iostream>

using namespace aarith; // NOLINT




TEMPLATE_TEST_CASE_SIG("Testing numeric_limits for aarith floating_points",
                       "[utility][floating_point]", ((typename F, typename AF, size_t N), F, AF, N),
                       (float, single_precision, 3), (double, double_precision, 3))
{
    /*
     * Note that the tmeplate paramter N (set to 3 in both cases) is necessary as otherwise Catch2
     * complains (and failts to compile) that some wrapper call is ambiguous. Simply ignore it.
     */


    using fbase = std::numeric_limits<F>;
    using abase = std::numeric_limits<AF>;

    // these variables are to check for constexpr'nes
    constexpr bool is_specialized = abase::is_specialized;
    constexpr bool is_signed = abase::is_signed;
    constexpr bool is_integer = abase::is_integer;
    constexpr bool is_exact = abase::is_exact;
    constexpr bool has_infinity = abase::has_infinity;
    constexpr bool has_quiet_NaN = abase::has_quiet_NaN;
    constexpr bool has_signaling_NaN = abase::has_signaling_NaN;
    constexpr bool is_bounded = abase::is_bounded;
    constexpr std::float_denorm_style has_denorm = abase::has_denorm;
    constexpr bool has_denorm_loss = abase::has_denorm_loss;
    constexpr std::float_round_style round_style = abase::round_style;
    constexpr bool is_iec559 = abase::is_iec559;
    constexpr bool is_modulo = abase::is_modulo;
    constexpr int radix = abase::radix;
    constexpr int digits = abase::digits;
    constexpr int digits10 = abase::digits10;
    constexpr int max_digits10 = abase::max_digits10;
    constexpr int min_exponent = abase::min_exponent;
    constexpr int max_exponent = abase::max_exponent;
    constexpr int min_exponent10 = abase::min_exponent10;
    constexpr int max_exponent10 = abase::max_exponent10;
    constexpr bool tinyness_before = abase::tinyness_before;
    constexpr AF min_ = abase::min();
    constexpr AF lowest_ = abase::lowest();
    constexpr AF max_ = abase::max();
    constexpr AF round_error_ = abase::round_error();
    constexpr AF infinity_ = abase::infinity();
    constexpr AF sNaN = abase::signaling_NaN();
    constexpr AF qNaN = abase::quiet_NaN();

    CHECK(is_specialized == fbase::is_specialized);
    CHECK(is_signed == fbase::is_signed);
    CHECK(is_integer == fbase::is_integer);
    CHECK(is_exact == fbase::is_exact);
    CHECK(has_infinity == fbase::has_infinity);
    CHECK(has_quiet_NaN == fbase::has_quiet_NaN);
    CHECK(has_signaling_NaN == fbase::has_signaling_NaN);
    CHECK(is_bounded == fbase::is_bounded);
    CHECK(has_denorm == fbase::has_denorm);
    CHECK(has_denorm_loss == fbase::has_denorm_loss);
    CHECK(round_style == fbase::round_style);
    CHECK(is_modulo == fbase::is_modulo);
    CHECK(radix == fbase::radix);
    CHECK(digits == fbase::digits);
    CHECK(digits10 == fbase::digits10);
    CHECK(max_digits10 == fbase::max_digits10);
    CHECK(min_exponent10 == fbase::min_exponent10);
    CHECK(min_exponent == fbase::min_exponent);
    CHECK(max_exponent == fbase::max_exponent);
    CHECK(max_exponent10 == fbase::max_exponent10);
    CHECK(tinyness_before == fbase::tinyness_before);

    CHECK(static_cast<F>(min_) == fbase::min());
    CHECK(min_ == AF{fbase ::min()});

    CHECK(static_cast<F>(max_) == fbase::max());
    CHECK(max_ == AF{fbase::max()});

    CHECK(static_cast<F>(lowest_) == fbase::lowest());
    CHECK(lowest_ == AF{fbase ::lowest()});

    CHECK(static_cast<F>(round_error_) == fbase::round_error());
    CHECK(round_error_ == AF{fbase ::round_error()});

    CHECK(static_cast<F>(infinity_) == fbase::infinity());
    CHECK(infinity_ == AF{fbase ::infinity()});

    // manually check some values that differ from the native float/double values
    CHECK((sNaN.is_nan() && sNaN.is_sNaN()));
    CHECK((qNaN.is_nan() && qNaN.is_qNaN()));
    CHECK_FALSE(is_iec559);
}
