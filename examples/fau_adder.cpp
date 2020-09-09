#include <aarith/integer_no_operators.hpp>
#include <aarith/operations/approx_operations.hpp>

using namespace aarith;

template <class base_type, size_t N, size_t P> void eval_fau_adder()
{

    static_assert(std::is_same_v<base_type, uint8_t> || std::is_same_v<base_type, uint16_t> ||
                  std::is_same_v<base_type, uint32_t> || std::is_same_v<base_type, uint64_t>);

    constexpr size_t max_val = std::numeric_limits<base_type>::max();
    constexpr size_t digits = std::numeric_limits<base_type>::digits;

    static_assert(N < digits);

    std::cout << "Evaluating the FAU adder for " << digits << " bits using " << N << " lower bits and " << P << " visibility bits\n";
    std::cout <<" \texpecting maximum error of\t" << ((1 << (N-P))-1) << "\n";

    uinteger<digits + 2> max_diff;

    for (size_t a_ = 0U; a_ <= max_val; ++a_)
    {
        const uinteger<digits> a{a_};
        for (size_t b_ = 0U; b_ <= max_val; ++b_)
        {
            const uinteger<digits> b{b_};
            const auto result_correct = expanding_add(a, b);
            const auto result_fau = FAUadder<digits, N, P>(a, b);

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

    std::cout << "\tactual maximum error\t\t" << max_diff << "\n";
}
int main()
{
    eval_fau_adder<uint8_t, 4,0>();
    eval_fau_adder<uint8_t, 4,1>();
    eval_fau_adder<uint8_t, 4,2>();
    eval_fau_adder<uint8_t, 4,3>();


    eval_fau_adder<uint8_t, 6,0>();
    eval_fau_adder<uint8_t, 6,1>();
    eval_fau_adder<uint8_t, 6,2>();
    eval_fau_adder<uint8_t, 6,3>();


//    eval_fau_adder<uint16_t, 4,0>();
//    eval_fau_adder<uint16_t, 4,1>();
//    eval_fau_adder<uint16_t, 4,2>();
//    eval_fau_adder<uint16_t, 4,3>();
}