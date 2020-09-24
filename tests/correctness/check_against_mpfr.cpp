#include <aarith/float.hpp>
#include <gmp.h>
#include <mpfr.h>

#include <chrono>
#include <iostream>
#include <string>
using namespace aarith;

static constexpr size_t n_iter = 5;

template <typename F> void check_add()
{

    mpfr_t a_mpfr, b_mpfr, result_mpfr;

    mpfr_init2 (a_mpfr, 32);
    mpfr_init2 (b_mpfr, 32);
    mpfr_init2 (result_mpfr, 32);


    F a_native = 4.0f;
    F b_native = 4.0f;


    mpfr_set_flt(a_mpfr, a_native, MPFR_RNDD);
    mpfr_set_flt(b_mpfr, b_native, MPFR_RNDD);

    mpfr_out_str(stdout,10, 0, a_mpfr,MPFR_RNDD );


    mpfr_clear(a_mpfr);
    mpfr_clear(b_mpfr);
    mpfr_clear(result_mpfr);

    mpfr_free_cache ();


//    using I = integer<W>;
//    static constexpr I aone{I::one()};
//    I a{-50};
//    I b = mul(div(I::max(), I{3}), I{2});
//
//    mpz_t a_mpir, b_mpir, result_mpir, aarith_in_mpir;
//    mpz_init_set_str(a_mpir, to_decimal(a).c_str(), 10);
//    mpz_init_set_str(b_mpir, to_decimal(b).c_str(), 10);
//    mpz_init_set_str(result_mpir, "0", 2);
//    mpz_init_set_str(aarith_in_mpir, "0", 2);
//
//    std::cout << "Checking addition for a bit-width of " << W << "..." << std::flush;
//
//    {
//        for (size_t i = 0; i < n_iter + 1; ++i)
//        {
//            for (size_t j = 0; j < n_iter; ++j)
//            {
//
//                const auto result = expanding_add(a, b);
//                mpz_add(result_mpir, a_mpir, b_mpir);
//                mpz_init_set_str(aarith_in_mpir, to_decimal(result).c_str(), 10);
//
//                if (mpz_cmp(result_mpir, aarith_in_mpir) != 0)
//                {
//                    std::cout << "Addition failed:\n";
//                    std::cout << "MPIR:\n";
//                    gmp_printf("\ta: %Zd\n", a_mpir);
//                    gmp_printf("\tb: %Zd\n", b_mpir);
//                    gmp_printf("\ta+b: %Zd\n", result_mpir);
//
//                    std::cout << "\naarith:\n"
//                              << "\ta: " << to_decimal(a) << "\n"
//                              << "\tb: " << to_decimal(b) << "\n"
//                              << "\ta+b: " << to_decimal(result) << "\n";
//                    exit(1);
//                }
//                b = b + aone;
//                mpz_add_ui(b_mpir, b_mpir, 1);
//            }
//            a = a + aone;
//            mpz_add_ui(a_mpir, a_mpir, 1);
//        }
//    }
//
//    std::cout << "\t done." << std::endl;
}

template <size_t W> void check_div()
{
    using I = integer<W>;
    static constexpr I aone{I::one()};
    I a{-50};
    I b = mul(div(I::max(), I{3}), I{2});

    mpz_t a_mpir, b_mpir, result_mpir, aarith_in_mpir;
    mpz_init_set_str(a_mpir, to_decimal(a).c_str(), 10);
    mpz_init_set_str(b_mpir, to_decimal(b).c_str(), 10);
    mpz_init_set_str(result_mpir, "0", 2);
    mpz_init_set_str(aarith_in_mpir, "0", 2);

    std::cout << "Checking division for a bit-width of " << W << "..." << std::flush;
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {

                {
                    const auto result = div(a, b);
                    mpz_tdiv_q(result_mpir, a_mpir, b_mpir);
                    mpz_init_set_str(aarith_in_mpir, to_decimal(result).c_str(), 10);

                    if (mpz_cmp(result_mpir, aarith_in_mpir) != 0)
                    {
                        std::cout << "Division failed:\n";
                        std::cout << "MPIR:\n";
                        gmp_printf("\ta: %Zd\n", a_mpir);
                        gmp_printf("\tb: %Zd\n", b_mpir);
                        gmp_printf("\ta/b: %Zd\n", result_mpir);

                        std::cout << "\naarith:\n"
                                  << "\ta: " << to_decimal(a) << "\n"
                                  << "\tb: " << to_decimal(b) << "\n"
                                  << "\ta/b: " << to_decimal(result) << "\n";
                        exit(1);
                    }
                }
                {
                    const auto result = div(b, a);
                    mpz_tdiv_q(result_mpir, b_mpir, a_mpir);
                    mpz_init_set_str(aarith_in_mpir, to_decimal(result).c_str(), 10);

                    if (mpz_cmp(result_mpir, aarith_in_mpir) != 0)
                    {
                        std::cout << "Division failed:\n";
                        std::cout << "MPIR:\n";
                        gmp_printf("\ta: %Zd\n", a_mpir);
                        gmp_printf("\tb: %Zd\n", b_mpir);
                        gmp_printf("\tb/a: %Zd\n", result_mpir);

                        std::cout << "\naarith:\n"
                                  << "\ta: " << to_decimal(a) << "\n"
                                  << "\tb: " << to_decimal(b) << "\n"
                                  << "\tb/a: " << to_decimal(result) << "\n";
                        exit(1);
                    }
                }
                b = b + aone;
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            a = a + aone;
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
    std::cout << "\t done." << std::endl;
}

int main()
{



    check_add<float>();
//    check_add<double>();
//    check_add<512>();
//    check_add<1024>();
//
//    check_sub<128>();
//    check_sub<256>();
//    check_sub<512>();
//    check_sub<1024>();
//
//    check_mul<128>();
//    check_mul<256>();
//    check_mul<512>();
//    check_mul<1024>();
//
//    check_div<128>();
//    check_div<256>();
//    check_div<512>();
//    check_div<1024>();
//
//    check_mod<128>();
//    check_mod<256>();
//    check_mod<512>();
//    check_mod<1024>();
    return 0;
}