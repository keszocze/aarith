#include <aarith/core.hpp>
#include <aarith/integer/approx_operations.hpp>
#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>
#include <iostream>
#include <sstream>

using namespace aarith; // NOLINT

SCENARIO("Performing common functional operations", "[word_array][functional][utility]")
{
    static constexpr size_t full_width = 256;
    static constexpr size_t half_width = 128;
    GIVEN("A word container w ")
    {

        constexpr word_array<full_width> w{5, 1, 2, 3};

        WHEN("Mapping the values by adding one")
        {
            THEN("All individual words should be increased by one")
            {
                constexpr auto fun = [](word_array<full_width>::word_type a) { return a + 1; };
                constexpr auto result = map(w, fun);

                CHECK(result.word(0) == 4);
                CHECK(result.word(1) == 3);
                CHECK(result.word(2) == 2);
                CHECK(result.word(3) == 6);
            }
        }

        WHEN("Mapping the values by left shifting by two bits")
        {
            THEN("All individual words should be increased by one")
            {
                constexpr auto fun = [](word_array<full_width>::word_type a) { return a << 2U; };
                constexpr auto result = map(w, fun);

                CHECK(result.word(0) == 12);
                CHECK(result.word(1) == 8);
                CHECK(result.word(2) == 4);
                CHECK(result.word(3) == 20);
            }
        }

        WHEN("Reducing the word_array")
        {
            THEN("Summing up the individual words should work fine")
            {
                constexpr auto f = [](const word_array<265>::word_type a, uint64_t w) {
                    return a + w;
                };
                constexpr auto result = reduce(w, f, 0);
                CHECK(result == 11U);
            }
        }

        AND_GIVEN("Another word container v of same length")
        {
            constexpr word_array<full_width> v{8, 16, 32, 64};
            WHEN("Performing the zip_with operation")
            {

                THEN("Element-wise addition should work as intended")
                {

                    constexpr auto f = [](word_array<full_width>::word_type a,
                                          word_array<full_width>::word_type b) { return a + b; };

                    constexpr auto result = zip_with(w, v, f);

                    for (size_t i = 0; i < w.word_count(); ++i)
                    {
                        CHECK(result.word(i) == w.word(i) + v.word(i));
                    }
                }
                THEN("It should be capable of modelling additions")
                {
                    constexpr word_array<half_width> a{0, std::numeric_limits<uint64_t>::max()};
                    constexpr word_array<half_width> b{0, std::numeric_limits<uint64_t>::max()};

                    constexpr auto f = [carry = uint64_t(0)](uint64_t ain, uint64_t bin) mutable {
                        uint64_t partial_sum = ain + bin;
                        uint64_t new_carry = (partial_sum < ain || partial_sum < bin) ? 1U : 0U;

                        partial_sum = partial_sum + carry;
                        carry = new_carry || (partial_sum < ain || partial_sum < bin) ? 1U : 0U;

                        return partial_sum;
                    };

                    constexpr auto result = zip_with(a, b, f);

                    constexpr auto uinteger_result =
                        add(uinteger<half_width>(a), uinteger<half_width>(b));

                    CHECK(uinteger<192>(result) == uinteger_result);
                }
            }

            WHEN("Performing the zip_with_state operation")
            {
                THEN("It should be capable of modeling addition")
                {
                    constexpr word_array<half_width> a{0, std::numeric_limits<uint64_t>::max()};
                    constexpr word_array<half_width> b{0, std::numeric_limits<uint64_t>::max()};

                    constexpr auto f = [](uint64_t ain, uint64_t bin, uint64_t cin) {
                        uint64_t partial_sum = ain + bin;
                        uint64_t new_carry = (partial_sum < ain || partial_sum < bin) ? 1U : 0U;

                        partial_sum = partial_sum + cin;
                        new_carry =
                            new_carry | ((partial_sum < ain || partial_sum < bin) ? 1U : 0U);

                        return std::make_pair(partial_sum, new_carry);
                    };

                    constexpr auto result = zip_with_state(a, b, f);

                    constexpr auto uinteger_result =
                        add(uinteger<half_width>(a), uinteger<half_width>(b));

                    CHECK(uinteger<192>(result) == uinteger_result);
                }
            }
            WHEN("Performing the trivial approximate addition")
            {
                THEN("The result should work out well, filling additional slots with zeroes")
                {
                    const auto result = trivial_approx_add<256, 256, uint64_t>(w, v);

                    for (size_t i = 0; i < w.word_count(); ++i)
                    {
                        CHECK(result.word(i) == w.word(i) + v.word(i));
                    }
                }
            }
            WHEN("Performing the zip_reduce operation")
            {

                THEN("Element-wise addition should work as intended")
                {

                    const std::vector<uint64_t> ns{8U, 15U, 5743U, 0U};

                    constexpr auto f = [](word_array<full_width>::word_type a,
                                          word_array<full_width>::word_type b,
                                          uint64_t m) { return a + b + m; };

                    for (const auto n : ns)
                    {
                        const auto result = zip_reduce(w, v, f, n);

                        CHECK(result == (5U + 1U + 2U + 3U + 8U + 16U + 32U + 64U + n));
                    }
                }
            }
            WHEN("Performing the trivial approximate addition")
            {
                THEN("The result should work out well")
                {
                    constexpr uinteger<full_width> a{w};
                    constexpr uinteger<full_width> b{v};
                    const auto result = trivial_approx_add<full_width, full_width>(a, b);

                    for (size_t i = 0; i < w.word_count(); ++i)
                    {
                        CHECK(result.word(i) == w.word(i) + v.word(i));
                    }
                }
            }
        }

        AND_GIVEN("Another word container v of different length")
        {
            constexpr word_array<192> v{8, 16, 64};
            WHEN("Performing the zip_with operation")
            {

                THEN("Element-wise addition should work as intended")
                {

                    // "cheat" by knowing the underlying word type
                    const auto f = [](const uint64_t a, const uint64_t b) { return a + b; };

                    const word_array<192> result_w_v = zip_with(w, v, f);
                    const word_array<192> result_v_w = zip_with(v, w, f);

                    for (size_t i = 0; i < result_w_v.word_count(); ++i)
                    {
                        CHECK(result_w_v.word(i) == w.word(i) + v.word(i));
                    }
                    for (size_t i = 0; i < result_v_w.word_count(); ++i)
                    {
                        CHECK(result_v_w.word(i) == w.word(i) + v.word(i));
                    }
                }
            }
            WHEN("Performing the trivial approximate addition")
            {
                THEN("The result should work out well, filling additional slots with zeroes")
                {
                    constexpr uinteger<full_width> a{w};
                    constexpr uinteger<192> b{v};
                    const auto result = trivial_approx_add<256, 192>(a, b);

                    for (size_t i = 0; i < w.word_count() - 1; ++i)
                    {
                        CHECK(result.word(i) == w.word(i) + v.word(i));
                    }
                    CHECK(result.word(3) == w.word(3));
                }
            }
            WHEN("Performing the zip_reduce operation")
            {

                THEN("Element-wise addition should work as intended")
                {

                    const std::vector<uint64_t> ns{8U, 15U, 5743U, 0U};

                    const auto f = [](word_array<full_width>::word_type a,
                                      word_array<full_width>::word_type b,
                                      uint64_t m) { return a + b + m; };

                    for (const auto n : ns)
                    {
                        const auto result = zip_reduce(w, v, f, n);

                        CHECK(result == (1U + 2U + 3U + 8U + 16U + 64U + n));
                    }
                }
            }
        }
    }
}
