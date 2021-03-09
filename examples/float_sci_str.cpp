#include <aarith/float.hpp>
#include <aarith/float/string_utils.hpp>
#include <bitset>
#include <cmath>

using namespace aarith;
using namespace std;

template <typename F, typename FP> void print_nums(const FP value)
{

    using int_storage = typename float_extraction_helper::bit_cast_to_type_trait<F>::type;
    constexpr size_t int_width = float_extraction_helper::bit_cast_to_type_trait<F>::width;

    auto large_val = floating_point<11, 52>(as_word_array<11, 52>(value));

    cout << to_binary(value) << "\n";
    cout << to_compute_string(value) << "\n";
    cout << to_binary(large_val) << "\n";
    cout << to_compute_string(large_val) << "\n";
    cout << static_cast<F>(value) << "\n";
    auto tmp = bit_cast<int_storage>(static_cast<F>(value));
    std::bitset<int_width> as_bitset(tmp);
    cout << as_bitset << "\n";
}

template <class F, class FP> void compare_to_float(F base)
{
    F ref = base;
    FP sgl;
    do
    {
        sgl = FP(ref);
        std::cout << "float:\n"
                  << ref << "\n"
                  << to_binary(FP(ref)) << "\n"
                  << "nfloat - single precision:\n"
                  << to_sci_string(sgl) << "\n\n";

        ref *= base;
    } while (!sgl.is_inf());

    ref = base;
    do
    {
        sgl = FP(ref);
        std::cout << "float:\n"
                  << ref << "\n"
                  << to_binary(FP(ref)) << "\n"
                  << "nfloat - single precision:\n"
                  << to_sci_string(sgl) << "\n\n";

        ref /= base;
    } while (!sgl.is_zero());
}

int main()
{
    using nsingle = floating_point<8, 23>;
    using nbetweensd = floating_point<10, 42>;
    using ndouble = floating_point<11, 52>;
    using nddouble = floating_point<22, 104>;
    const float base = 3.1415f;

    auto single_bit = uinteger<23>::all_zeroes();
    single_bit.set_bit(0, true);

    nsingle foo(false, uinteger<10>::one(), single_bit);

    //    print_nums<double>(foo);

    //    auto large_val = width_cast<11, 52>(foo);
    //
    //    cout << to_binary(foo) << "\n";
    //    cout << to_compute_string(foo) << "\n";
    //    cout << to_binary(large_val) << "\n";
    //    cout << to_compute_string(large_val) << "\n";
    //
    //    nsingle v(false, uinteger<8>{0b01111110}, uinteger<23>{0b10000000000000000000000});
    //    std::cout << to_binary(v) << "\t" << to_compute_string(v) << "\n";
    //
    //

    ndouble test(
        word_array<64>{0b0111111101100010001101001001001011100010000111111101001011001000});
    std::cout << as_word_array(test) << "\n";
    floating_point<11 + 80, 52> test_{test};
    std::cout << "(" << test_.is_negative() << ", " << to_binary(test.get_exponent()) << ", "
              << to_binary(test_.get_full_mantissa()) << ")"
              << "\n";
    // compare_to_float<float, nsingle>(base);
    // compare_to_float<float, nbetweensd>(base);
    // compare_to_float<double, ndouble>(static_cast<double>(base));
    // compare_to_float<double, nddouble>(static_cast<double>(base));

    return 0;
}
