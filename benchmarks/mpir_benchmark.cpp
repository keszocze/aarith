#include <benchmark/benchmark.h>

#include <aarith/integer.hpp>
#include <mpir.h>

#include <chrono>
#include <iostream>
#include <string>
using namespace aarith;
using aint = integer<1024>;
using auint = uinteger<1024>;

static constexpr aint aone{aint::one()};

static constexpr size_t n_iter = 100;

void aarith_add(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                const auto result = expanding_add(a, b);
                benchmark::DoNotOptimize(result);
                b = b + aone;
            }
            a = a + aone;
        }
    }
}

void mpir_add(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                mpz_add(tmp_mpir, a_mpir, b_mpir);
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

void aarith_sub(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                const auto result = expanding_sub(a, b);
                benchmark::DoNotOptimize(result);
                b = b + aone;
            }
            a = a + aone;
        }
    }
}

void mpir_sub(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                mpz_sub(tmp_mpir, a_mpir, b_mpir);
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

void aarith_booth(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                benchmark::DoNotOptimize(booth_expanding_mul(a, b));
                b = b + aone;
            }
            a = a + aone;
        }
    }
}

void aarith_booth_inplace(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                benchmark::DoNotOptimize(inplace_expanding_mul(a, b));
                b = b + aone;
            }
            a = a + aone;
        }
    }
}

void aarith_naive(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                benchmark::DoNotOptimize(naive_expanding_mul(a, b));
                b = b + aone;
            }
            a = a + aone;
        }
    }
}

void mpir_mul(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                mpz_mul(tmp_mpir, a_mpir, b_mpir);
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

void aarith_div(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                benchmark::DoNotOptimize(div(a, b));
                b = b + aone;
            }
            a = a + aone;
        }
    }
}

void mpir_div(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                mpz_div(tmp_mpir, a_mpir, b_mpir);
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

void aarith_mod(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                benchmark::DoNotOptimize(remainder(a, b));
                b = b + aone;
            }
            a = a + aone;
        }
    }
}

void mpir_mod(benchmark::State& state)
{
    aint a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir,tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                mpz_mod(tmp_mpir, a_mpir, b_mpir);
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

int main(int argc, char** argv)
{
    benchmark::RegisterBenchmark("aarith_add", &aarith_add)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("mpir_add", &mpir_add)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("aarith_sub", &aarith_sub)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("mpir_sub", &mpir_sub)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("aarith_booth_mul", &aarith_booth)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("aarith_booth_mul_inplace", &aarith_booth_inplace)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("aarith_naive_mul", &aarith_naive)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("mpir_mul", &mpir_mul)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("aarith_div", &aarith_div)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("mpir_div", &mpir_div)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("aarith_mod", &aarith_mod)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("mpir_mod", &mpir_mod)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
