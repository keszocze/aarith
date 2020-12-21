#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>


using aarith::normalized_float;

using F = normalized_float<8, 23>; // NOLINT

template <size_t iterations> F heron_exact(const F& num)
{

    using aarith::anytime_add;
    using aarith::anytime_div;

    const F one{1.0f}; // NOLINT
    const F two{2.0f}; // NOLINT
    F result = add(num, one);
    result = div(result, two);

    for (size_t i = 0; i < iterations; ++i)
    {
        const F quot = div(num, result);
        result = add(result, quot);
        result = div(result, two);
    }

    return result;
}

template <size_t iterations, size_t C> F heron_anytime(const F& num)
{

    using aarith::anytime_add;
    using aarith::anytime_div;

    const F one{1.0f}; // NOLINT
    const F two{2.0f}; // NOLINT
    F result = anytime_add(num, one, C + 1);
    result = anytime_div(result, two, C + 1);

    for (size_t i = 0; i < iterations; ++i)
    {
        const F quot = anytime_div(num, result, C + 1);
        result = anytime_add(result, quot, C + 1);
        result = anytime_div(result, two, C + 1);
    }

    return result;
}

template <int start, size_t iters, size_t heron_iter, size_t C> void compare_heron()
{

    auto t1 = std::chrono::high_resolution_clock::now();

    F a{static_cast<float>(start)};

    const F delta{0.014324f / 2.0f}; // NOLINT

    double max_abs_diff{0};
    double max_abs_diff_float{0};

    for ([[maybe_unused]] size_t i = 0; i < iters; ++i)
    {
        const F res_anytime = heron_anytime<heron_iter, C>(a);
        const F res_exact = heron_exact<heron_iter>(a);

        const double abs_diff = static_cast<double>(abs(sub(res_exact, res_anytime)));

        const double res_anytime_float = static_cast<double>(res_anytime);
        const double a_float = static_cast<double>(a);
        const double res_float = static_cast<double>(sqrtf(a_float)); // NOLINT
        const double abs_diff_correct = std::abs(res_anytime_float - res_float);

        max_abs_diff = std::max(max_abs_diff, abs_diff);
        max_abs_diff_float = std::max(max_abs_diff_float, abs_diff_correct);
        //        std::cout << "value iter: " << i << "\n";
        //        std::cout << "sqrt_e(" << static_cast<float>(a) << ") = " <<
        //        static_cast<float>(res_exact)
        //                  << "\n";
        //        std::cout << "sqrt_a(" << static_cast<float>(a) << ") = " <<
        //        static_cast<float>(res_anytime)
        //                  << "\n";
        //        std::cout << "sqrt_f(" << a_float << ") = " << res_float << "\n";
        //        std::cout << "diff: " << abs_diff << "\n";
        //        std::cout << "diff_float: " << abs_diff_correct << "\n\n";

        a = add(a, delta);
    }
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << std::fixed << std::setprecision(30) << heron_iter << ";" << C << ";" // NOLINT
              << static_cast<double>(max_abs_diff) << ";" << max_abs_diff_float << ";" << duration
              << "\n";
}

int main()
{

    // change to 'true' for the full range of experiments
    // greatly increases compilation time
    static constexpr bool full_experiments = false;

    constexpr size_t value_iter = 8800;
    constexpr int starting_value = 65;

    compare_heron<starting_value, value_iter, 1, 22>(); // NOLINT
    compare_heron<starting_value, value_iter, 2, 22>(); // NOLINT

    if constexpr (full_experiments) // NOLINT
    {
        compare_heron<starting_value, value_iter, 3, 22>();    // NOLINT
        compare_heron<starting_value, value_iter, 4, 22>();    // NOLINT
        compare_heron<starting_value, value_iter, 5, 22>();    // NOLINT
        compare_heron<starting_value, value_iter, 6, 22>();    // NOLINT
        compare_heron<starting_value, value_iter, 7, 22>();    // NOLINT
        compare_heron<starting_value, value_iter, 10, 22>();   // NOLINT
        compare_heron<starting_value, value_iter, 100, 22>();  // NOLINT
        compare_heron<starting_value, value_iter, 1000, 22>(); // NOLINT
        compare_heron<starting_value, value_iter, 2000, 22>(); // NOLINT
    }

    constexpr size_t heron_iter = 5;
    compare_heron<starting_value, value_iter, heron_iter, 6>();

    if constexpr (full_experiments) // NOLINT
    {
        compare_heron<starting_value, value_iter, heron_iter, 7>();  // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 8>();  // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 9>();  // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 10>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 11>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 12>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 13>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 14>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 15>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 16>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 17>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 18>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 19>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 20>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 21>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 22>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 23>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter, 24>(); // NOLINT
    }

    constexpr size_t heron_iter_15 = 15;
    compare_heron<starting_value, value_iter, heron_iter_15, 6>(); // NOLINT
    compare_heron<starting_value, value_iter, heron_iter_15, 7>(); // NOLINT

    if constexpr (full_experiments) // NOLINT
    {
        compare_heron<starting_value, value_iter, heron_iter_15, 8>();  // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 9>();  // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 10>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 11>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 12>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 13>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 14>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 15>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 16>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 17>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 18>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 19>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 20>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 21>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 22>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 23>(); // NOLINT
        compare_heron<starting_value, value_iter, heron_iter_15, 24>(); // NOLINT
    }

    return 0;
}
