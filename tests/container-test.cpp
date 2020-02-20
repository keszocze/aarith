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
                const auto result = map(w, [](uint64_t a) { return a + 1; });

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
                const auto result = map(w, [](uint64_t a) { return a << 2; });

                CHECK(result.word(0) == 12);
                CHECK(result.word(1) == 8);
                CHECK(result.word(2) == 4);
                CHECK(result.word(3) == 0);
            }
        }

        AND_GIVEN("Another word container v")
        {
            const word_container<256> v{8, 16, 32, 64};
            WHEN("Performign the zip_with operation")
            {
                //            std::cout << group_digits(to_binary(v), 64) << "\n";
            }


        }
    }
}
