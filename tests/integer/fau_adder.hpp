#pragma once

#include <aarith/integer.hpp>
#include <aarith/operations/approx_operations.hpp>
#include <tuple>

namespace aarith {

template <size_t width, size_t lsp_width, size_t shared_bits = 0>
std::tuple<uinteger<2 * width>, uinteger<2 * width>, uinteger<2 * width>>
eval_fau_adder()
{
    using namespace aarith;
    using I = uinteger<width>;
    using V = uinteger<width + 1>;
    using W = uinteger<2 * width>;

    constexpr I min = std::numeric_limits<I>::min();
    constexpr I max = std::numeric_limits<I>::max();

    W AER = W::zero();
    W MED = W::zero();
    V ME = V::zero();

    for (const I a : integer_range<I>(min, max))
    {
        for (const I b : integer_range<I>(min, max))
        {

            const V approx_result = FAUadder<width, lsp_width, shared_bits>(a, b);
            const V correct_result = expanding_add(a, b);

            W diff = sub(correct_result, approx_result);
            MED = add(MED, diff);
            if (diff > ME)
            {
                ME = diff;
            }

            if (approx_result > correct_result)
            {
                std::cout
                    << "The FAU approximation should have been smaller than the correct result!\n";
            }

            if (approx_result != correct_result)
            {
                AER = add(AER, W::one());
            }
        }
    }

    return std::make_tuple(AER, MED, ME);
}

} // namespace aarith