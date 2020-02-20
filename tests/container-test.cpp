#include <aarith/operations/uinteger_comparisons.hpp>
#include <aarith/types/uinteger.hpp>
#include <aarith/utilities/string_utils.hpp>
#include <catch.hpp>
#include <iostream>
#include <sstream>

using namespace aarith;

SCENARIO("Performing common functional operations", "[word_container]")
{
    GIVEN("A word container w ")
    {

        const word_container<256> w{0, 1, 2, 3};

        WHEN("Mapping the values by adding one")
        {
            THEN("All individual words should be increased by one")
            {
                const auto result = map(w, [](word_container<256>::word_type a) { return a + 1; });

                for (size_t i = 0; i < result.word_count(); ++i)
                {
                    CHECK(result.word(i) == result.word_count() - i);
                }
            }
        }

        WHEN("Mapping the values by left shifting by two bits")
        {
            THEN("All individual words should be increased by one")
            {
                const auto result = map(w, [](word_container<256>::word_type a) { return a << 2; });

                CHECK(result.word(0) == 12);
                CHECK(result.word(1) == 8);
                CHECK(result.word(2) == 4);
                CHECK(result.word(3) == 0);
            }
        }

        WHEN("Reducing the word_container")
        {
            THEN("Summing up the individual words should work fine")
            {
                const auto f = [](const word_container<265>::word_type a, uint64_t w) {
                    return a + w;
                };
                const auto result = reduce(w, f, 0);
                CHECK(result == 6U);
            }
        }

        AND_GIVEN("Another word container v of same length")
        {
            const word_container<256> v{8, 16, 32, 64};
            WHEN("Performing the zip_with operation")
            {

                THEN("Element-wise addition should work as intended")
                {

                    const auto f = [](word_container<256>::word_type a,
                                      word_container<256>::word_type b) { return a + b; };

                    const auto result = zip_with(w, v, f);

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

                    const auto f = [](word_container<256>::word_type a,
                                      word_container<256>::word_type b,
                                      uint64_t m) { return a + b + m; };

                    for (const auto n : ns)
                    {
                        const auto result = zip_reduce(w, v, f, n);

                        CHECK(result == (1U + 2U + 3U + 8U + 16U + 32U + 64U + n));
                    }
                }
            }
        }

        AND_GIVEN("Another word container v of different length")
        {
            const word_container<192> v{8, 16, 64};
            WHEN("Performing the zip_with operation")
            {

                THEN("Element-wise addition should work as intended")
                {

                    // "cheat" by knowing the underlying word type
                    const auto f = [](const uint64_t a,const uint64_t b) { return a + b; };

                    const word_container<192> result_w_v = zip_with(w, v, f);
                    const word_container<192> result_v_w = zip_with(v, w, f);

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
            WHEN("Performing the zip_reduce operation")
            {

                THEN("Element-wise addition should work as intended")
                {

                    const std::vector<uint64_t> ns{8U, 15U, 5743U, 0U};

                    const auto f = [](word_container<256>::word_type a,
                                      word_container<256>::word_type b,
                                      uint64_t m) { return a + b + m; };

                    for (const auto n : ns)
                    {
                        const auto result = zip_reduce(w, v, f, n);
                        
                        CHECK(result == (1U + 2U + 3U + 8U + 16U +  64U + n));
                    }
                }
            }
        }
    }
}
