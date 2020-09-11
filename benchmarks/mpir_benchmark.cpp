#include <benchmark/benchmark.h>

#include <aarith/integer.hpp>
#include <mpir.h>

#include <chrono>
#include <iostream>
#include <string>

void aarith_add(benchmark::State& state)
{
    aarith::integer<1024> a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < 32; ++i)
        {
            for (size_t j = 0; j < 32; ++j)
            {
                benchmark::DoNotOptimize(aarith::integer_operators::operator+(a, b));
                b = b + aarith::integer<1024>::one();
            }
            a = a + aarith::integer<1024>::one();
        }
    }
}

void mpir_add(benchmark::State& state)
{
    aarith::integer<1024> a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir;
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < 32; ++i)
        {
            for (size_t j = 0; j < 32; ++j)
            {
                mpz_add(a_mpir, a_mpir, b_mpir);
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

void aarith_mul(benchmark::State& state)
{
    aarith::integer<1024> a(1), b(1);
    a = a << 1022;
    b = b << 511;

    for (auto _ : state)
    {
        for (size_t i = 0; i < 32; ++i)
        {
            for (size_t j = 0; j < 32; ++j)
            {
                benchmark::DoNotOptimize(aarith::expanding_mul(a, b));
                b = b + aarith::integer<1024>::one();
            }
            a = a + aarith::integer<1024>::one();
        }
    }
}

void mpir_mul(benchmark::State& state)
{
    aarith::integer<1024> a(1), b(1);
    a = a << 1022;
    b = b << 511;

    mpz_t a_mpir, b_mpir;
    mpz_init_set_str(a_mpir, to_binary(a).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(b).c_str(), 2);

    for (auto _ : state)
    {
        for (size_t i = 0; i < 32; ++i)
        {
            for (size_t j = 0; j < 32; ++j)
            {
                mpz_mul(a_mpir, a_mpir, b_mpir);
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
    benchmark::RegisterBenchmark("aarith_mul", &aarith_mul)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("mpir_mul", &mpir_mul)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
