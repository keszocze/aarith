#include <aarith/float.hpp>
#include <aarith/float/float_string_utils.hpp>
#include <bitset>
#include <cmath>
#include <sstream>

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
    using ss = std::stringstream;
    auto comparisons_inc = 0UL;
    auto errors_inc = 0UL;
    auto comparisons_dec = 0UL;
    auto errors_dec = 0UL;

    do
    {
        comparisons_inc++;
        std::cout << "\r" << comparisons_inc << "        ";
        sgl = FP(ref);

        ss reference, afloat;
        reference << scientific << ref;
        afloat << to_sci_string(sgl);

        if(reference.str() != afloat.str())
        {
            std::cout << "float:\n"
                      << reference.str() << "\n"
                      << to_binary(FP(ref)) << "\n"
                      << "aarith float:\n"
                      << afloat.str() << "\n\n";
            errors_inc++;
        }
        ref *= base;
    } while (!sgl.is_inf());
    std::cout << "\n";

    ref = base;
    do
    {
        comparisons_dec++;
        std::cout << "\r" << comparisons_dec << "        ";
        sgl = FP(ref);

        ss reference, afloat;
        reference << scientific << ref;
        afloat << to_sci_string(sgl);

        if(reference.str() != afloat.str())
        {
            std::cout << "float:\n"
                      << reference.str() << "\n"
                      << to_binary(FP(ref)) << "\n"
                      << "aarith float:\n"
                      << afloat.str() << "\n\n";
            errors_dec++;
        }

        ref /= base;
    } while (!sgl.is_zero());
    std::cout << "\n";

    std::cout << "Base: " << base << "\n"
              << "Errors (inc): " << errors_inc << "/" << comparisons_inc << "\n"
              << "Errors (dec): " << errors_dec << "/" << comparisons_dec << "\n";
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

    single_precision a(word_array<32>{0b11110111100110110011101101010111});
    single_precision b(word_array<32>{0b00000000001101010101101111001000});
    float a_ = static_cast<float>(a);
    float b_ = static_cast<float>(b);
    cout << a << "\t" << a_ << "\n";
    cout << b << "\t" << b_ << "\n";

    cout << "float, nsingle:\n";
    compare_to_float<float, nsingle>(3.14);
    cout << "float, nbetweensd:\n";
    compare_to_float<float, nbetweensd>(3.14);
    cout << "float, ndouble:\n";
    compare_to_float<float, ndouble>(3.14);
    cout << "float, nddouble:\n";
    compare_to_float<float, nddouble>(3.14);
    cout << "double, ndouble:\n";
    compare_to_float<double, ndouble>(3.14);
    cout << "double, nddouble:\n";
    compare_to_float<double, nddouble>(3.14);

    return 0;
}
