#include <aarith/integer.hpp>
#include <mpir.h>

#include <chrono>
#include <iostream>
#include <string>
using namespace aarith;
using aint = integer<1024>;
using auint = uinteger<1024>;

static constexpr aint aone{aint::one()};

static constexpr size_t n_iter = 64;

void check_add()
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, result_mpir, aarith_in_mpir;
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);
    mpz_init_set_str(result_mpir, "0", 2);
    mpz_init_set_str(aarith_in_mpir, "0", 2);

    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {

                const auto result=expanding_add(a,b);
                mpz_add(result_mpir, a_mpir, b_mpir);
                mpz_init_set_str(aarith_in_mpir, to_binary(result).c_str(), 2);


                if (mpz_cmp(result_mpir, aarith_in_mpir) != 0)
                {

                    gmp_printf("%Zd\n", a_mpir);
                    gmp_printf("%Zd\n", b_mpir);
                    gmp_printf("%Zd\n", result_mpir);

                    std::cout << "\n" << to_decimal(a) << "\n" << to_decimal(b) << "\n" <<to_decimal(result) << "\n";
                    exit(1);
                }
                b = b + aone;
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            a = a + aone;
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

void check_mul()
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, result_mpir, aarith_in_mpir;
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);
    mpz_init_set_str(result_mpir, "0", 2);
    mpz_init_set_str(aarith_in_mpir, "0", 2);

    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {

                const auto result= booth_expanding_mul(a, b);
                mpz_mul(result_mpir, a_mpir, b_mpir);
                mpz_init_set_str(aarith_in_mpir, to_binary(result).c_str(), 2);


                if (mpz_cmp(result_mpir, aarith_in_mpir) != 0)
                {


                    gmp_printf("%Zd\n", a_mpir);
                    gmp_printf("%Zd\n", b_mpir);
                    gmp_printf("%Zd\n", result_mpir);

                    std::cout << "\n" << to_decimal(a) << "\n" << to_decimal(b) << "\n" <<to_decimal(result) << "\n";
                    std::cout << "i: " << i << "\tj: " << j << "\n";
                    exit(1);
                }
                else {std::cout << "nice\n";}
                b = b + aone;
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            a = a + aone;
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

void check_div()
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, result_mpir, aarith_in_mpir;
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);
    mpz_init_set_str(result_mpir, "0", 2);
    mpz_init_set_str(aarith_in_mpir, "0", 2);

    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {

                const auto result=div(a,b);
                mpz_div(result_mpir, a_mpir, b_mpir);
                mpz_init_set_str(aarith_in_mpir, to_binary(result).c_str(), 2);


                if (mpz_cmp(result_mpir, aarith_in_mpir) != 0)
                {
                    gmp_printf("%Zd\n", a_mpir);
                    gmp_printf("%Zd\n", b_mpir);
                    gmp_printf("%Zd\n", result_mpir);

                    std::cout << "\n" << to_decimal(a) << "\n" << to_decimal(b) << "\n" <<to_decimal(result) << "\n";
                    exit(1);
                }
                b = b + aone;
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            a = a + aone;
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

void check_sub()
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, result_mpir, aarith_in_mpir;
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);
    mpz_init_set_str(result_mpir, "0", 2);
    mpz_init_set_str(aarith_in_mpir, "0", 2);

    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {

                const auto result=expanding_sub(a,b);
                mpz_sub(result_mpir, a_mpir, b_mpir);
                mpz_init_set_str(aarith_in_mpir, to_binary(result).c_str(), 2);


                if (mpz_cmp(result_mpir, aarith_in_mpir) != 0)
                {
                    gmp_printf("%Zd\n", a_mpir);
                    gmp_printf("%Zd\n", b_mpir);
                    gmp_printf("%Zd\n", result_mpir);

                    std::cout << "\n" << to_decimal(a) << "\n" << to_decimal(b) << "\n" <<to_decimal(result) << "\n";
                    exit(1);
                }
                b = b + aone;
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            a = a + aone;
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}


void check_mod()
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, result_mpir, aarith_in_mpir;
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);
    mpz_init_set_str(result_mpir, "0", 2);
    mpz_init_set_str(aarith_in_mpir, "0", 2);

    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {

                const auto result=remainder(a,b);
                mpz_mod(result_mpir, a_mpir, b_mpir);
                mpz_init_set_str(aarith_in_mpir, to_binary(result).c_str(), 2);


                if (mpz_cmp(result_mpir, aarith_in_mpir) != 0)
                {
                    gmp_printf("%Zd\n", a_mpir);
                    gmp_printf("%Zd\n", b_mpir);
                    gmp_printf("%Zd\n", result_mpir);

                    std::cout << "\n" << to_decimal(a) << "\n" << to_decimal(b) << "\n" <<to_decimal(result) << "\n";
                    exit(1);
                }
                b = b + aone;
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            a = a + aone;
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}


int main()
{
    check_add();
    check_sub();
    check_div();
    check_mod();
    check_mul();
}