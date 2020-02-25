#include <aarith/integer.hpp>
#include <aarith/operations/approx_operations.hpp>

using namespace aarith;

template <class base_type, size_t N> auto eval_fau_adder()
{

    static_assert(std::is_same_v<base_type, uint8_t> || std::is_same_v<base_type, uint16_t> ||
                  std::is_same_v<base_type, uint32_t> || std::is_same_v<base_type, uint64_t>);

    constexpr size_t max_val = std::numeric_limits<base_type>::max();
    constexpr size_t digits = std::numeric_limits<base_type>::digits;

    static_assert(N < digits);

    uinteger<digits + 2> max_diff;

    for (size_t a_ = 0U; a_ <= max_val; ++a_)
    {
        const uinteger<digits> a{a_};
        for (size_t b_ = 0U; b_ <= max_val; ++b_)
        {
            const uinteger<digits> b{b_};
            const auto result_correct = expanding_add(a, b);
            const auto result_fau = FAUadder<digits, N>(a, b);

            uinteger<digits + 2> difference;
            if (result_fau < result_correct)
            {
                difference = expanding_sub(result_correct, result_fau);
            }
            else
            {
                difference = expanding_sub(result_fau, result_correct);
            }

            max_diff = std::max(max_diff, difference);
        }
    }

    return max_diff;
}
int main()
{
    const auto res = eval_fau_adder<uint8_t, 4>();
    std::cout << res << "\n";
}