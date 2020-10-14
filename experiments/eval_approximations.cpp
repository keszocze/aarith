#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <aarith/integer.hpp>
#include <chrono>
#include <cmath>
#include <random>
#include <sstream>

using namespace aarith;

using F = normalized_float<8, 23>;

template <size_t A, typename WordType = uint64_t> void test_a()
{

    constexpr size_t N = 1;
    constexpr size_t NDenorm = 1;

    std::random_device rd;
    std::mt19937 mt(rd());

    using ExpInt = uinteger<8, WordType>;
    using MantInt = uinteger<A, WordType>;
    using F = normalized_float<8, A, WordType>;
    using Float = normalized_float<8, 23, WordType>;

    // do not generate special cases, i.e. inf, nan, denormalized numbers
    uniform_uinteger_distribution exp_dist =
        uniform_uinteger_distribution<8, WordType>(ExpInt::one(), ExpInt::max() - ExpInt::one());
    uniform_uinteger_distribution mant_dist = uniform_uinteger_distribution<A, WordType>();

    const auto gen = [&rd, &mt, &exp_dist, &mant_dist]() {
        std::vector<F> values{F::zero(),
                              F::neg_zero(),
                              F::one(),
                              F::neg_one(),
                              F::smallest_normalized(),
                              F::smallest_denormalized(),
                              F::neg_infinity(),
                              F::pos_infinity(),
                              F::NaN()};

        for (size_t i = 0; i < N; ++i)
        {
            ExpInt e = exp_dist(mt);
            MantInt m = mant_dist(mt);
            F pos(false, e, m);
            F neg(true, e, m);

            values.push_back(pos);
            values.push_back(neg);
        }

        for (size_t i = 0; i < NDenorm; ++i)
        {
            ExpInt e = ExpInt::zero();
            MantInt m = mant_dist(mt);
            F pos(false, e, m);
            F neg(true, e, m);

            values.push_back(pos);
            values.push_back(neg);
        }
        return values;
    };

    const std::vector<F> vals_a = gen();
    const std::vector<F> vals_b = gen();

    std::cout << "a;a float;b float;a aarith; b aarith;a in float width;b in float width;"
              << "a+b float;a-b float;a*b float;a+b float bits;a-b float bits;a*b float bits;"
              << "a+b aarith;a-b aarith;a*b aarith;a+b in float width;a-b in float width;a*b in "
                 "float width;"
              << "anytime a+b aarith;anytime a-b aarith;anytime a*b aarith;anytime a+b in float "
                 "width;anytime a-b in float width;anytime a*b in float width;"
              << "\n";
    for (const F& f : vals_a)
    {
        for (const F& g : vals_b)
        {

            float f_float = static_cast<float>(f);
            float g_float = static_cast<float>(g);

            float add_float = f_float + g_float;
            float sub_float = f_float - g_float;
            float mul_float = f_float * g_float;

            Float add_float_F{add_float};
            Float sub_float_F{sub_float};
            Float mul_float_F{mul_float};

            F add = f + g;
            F sub = f - g;
            F mul = f * g;

            F any_add = anytime_add(f, g, A + 1);
            F any_sub = anytime_sub(f, g, A + 1);
            F any_mul = anytime_mul(f, g, A + 1);

            std::cout << A << ";" << f_float << ";" << g_float << ";" << as_word_array<8, A>(f)
                      << ";" << as_word_array<8, A>(g) << ";";
            std::cout << as_word_array<8, 23>(f) << ";" << as_word_array<8, 23>(g) << ";";
            std::cout << add_float << ";" << sub_float << ";" << mul_float << ";";
            std::cout << as_word_array<8, 23>(add_float_F) << ";"
                      << as_word_array<8, 23>(sub_float_F) << ";"
                      << as_word_array<8, 23>(mul_float_F) << ";";
            std::cout << as_word_array<8, A>(add) << ";" << as_word_array<8, A>(sub) << ";"
                      << as_word_array<8, A>(mul) << ";" << as_word_array<8, 23>(add) << ";"
                      << as_word_array<8, 23>(sub) << ";" << as_word_array<8, 23>(mul) << ";";
            std::cout << as_word_array<8, A>(any_add) << ";" << as_word_array<8, A>(any_sub) << ";"
                      << as_word_array<8, A>(any_mul) << ";" << as_word_array<8, 23>(any_add) << ";"
                      << as_word_array<8, 23>(any_sub) << ";" << as_word_array<8, 23>(any_mul)
                      << ";";

            std::cout << "\n";
        }
    }
}

int main()
{

    test_a<23>();
    test_a<22>();
    test_a<21>();
    test_a<20>();
    test_a<19>();
    test_a<18>();
    test_a<17>();
    test_a<16>();
    test_a<15>();
    test_a<14>();
    test_a<13>();
    test_a<12>();
    test_a<11>();
    test_a<10>();
    test_a<9>();
    test_a<8>();
    test_a<7>();
    test_a<6>();
    test_a<5>();
    test_a<4>();
    test_a<3>();
    test_a<2>();
    test_a<1>();
    return EXIT_SUCCESS;
}
