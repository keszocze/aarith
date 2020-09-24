#include <benchmark/benchmark.h>

#include <aarith/integer.hpp>
#include <mpir.h>

#include <chrono>
#include <iostream>
#include <string>
using namespace aarith;

template <size_t W> class Init;

template <size_t W> class Init
{
public:
    using Type = integer<W>;

    static constexpr Type val_a()
    {
        return Type{-50};
    }

    static constexpr Type val_b()
    {
        return mul(div(Type::max(), Type{3}), Type{2});
    }

    static constexpr size_t n_iter()
    {
        return 100;
    }
};

template <typename F> void aarith_generic_bench(benchmark::State& state)
{
    using I = typename F::Type;
    using S = Init<I::width()>;

    I a{S::val_a()};
    I b{S::val_b()};

    constexpr size_t n_iter = S::n_iter();
    constexpr I aone{I::one()};

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter + 1; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                const auto result = F::compute(a, b);
                benchmark::DoNotOptimize(result);
                b = b + aone;
            }
            a = a + aone;
        }
    }
}

template <size_t W> class Add
{
public:
    using Type = integer<W>;

    static Type compute(const Type& a, const Type& b)
    {
        return expanding_add(a, b);
    }
};

template <size_t W> class Sub
{
public:
    using Type = integer<W>;

    static Type compute(const Type& a, const Type& b)
    {
        return expanding_sub(a, b);
    }
};

template <size_t W> class NaiveMul
{
public:
    using Type = integer<W>;

    static Type compute(const Type& a, const Type& b)
    {
        return naive_expanding_mul(a, b);
    }
};

template <size_t W> class BoothMul
{
public:
    using Type = integer<W>;

    static Type compute(const Type& a, const Type& b)
    {
        return booth_expanding_mul(a, b);
    }
};

template <size_t W> class Div
{
public:
    using Type = integer<W>;

    static Type compute(const Type& a, const Type& b)
    {
        if (b.is_zero())
        {
            return Type::zero();
        }
        return div(a, b);
    }
};

template <size_t W> class Mod
{
public:
    using Type = integer<W>;

    static Type compute(const Type& a, const Type& b)
    {
        if (b.is_zero())
        {
            return Type::zero();
        }
        return remainder(a, b);
    }
};

template <size_t W> void mpir_add(benchmark::State& state)
{

    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};

    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_decimal(a).c_str(), 10);
    mpz_init_set_str(b_mpir, to_decimal(b).c_str(), 10);

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

template <size_t W> void mpir_sub(benchmark::State& state)
{
    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};
    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_decimal(a).c_str(), 10);
    mpz_init_set_str(b_mpir, to_decimal(b).c_str(), 10);

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

template <size_t W> void mpir_mul(benchmark::State& state)
{
    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};
    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_decimal(a).c_str(), 10);
    mpz_init_set_str(b_mpir, to_decimal(b).c_str(), 10);

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

template <size_t W> void mpir_div(benchmark::State& state)
{
    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};

    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_decimal(a).c_str(), 10);
    mpz_init_set_str(b_mpir, to_decimal(b).c_str(), 10);

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                mpz_tdiv_q(tmp_mpir, a_mpir, b_mpir);
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

template <size_t W> void mpir_mod(benchmark::State& state)
{
    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};

    mpz_t a_mpir, b_mpir, tmp_mpir;
    mpz_init_set_str(tmp_mpir, "0", 2);
    mpz_init_set_str(a_mpir, to_decimal(a).c_str(), 10);
    mpz_init_set_str(b_mpir, to_decimal(b).c_str(), 10);

    for (auto _ : state)
    {
        for (size_t i = 0; i < n_iter; ++i)
        {
            for (size_t j = 0; j < n_iter; ++j)
            {
                mpz_tdiv_r(tmp_mpir, a_mpir, b_mpir);
                mpz_add_ui(b_mpir, b_mpir, 1);
            }
            mpz_add_ui(a_mpir, a_mpir, 1);
        }
    }
}

int main(int argc, char** argv)
{

benchmark::RegisterBenchmark("AarithAdd8", &aarith_generic_bench<Add<8>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub8", &aarith_generic_bench<Sub<8>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul8", &aarith_generic_bench<NaiveMul<8>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul8", &aarith_generic_bench<BoothMul<8>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv8", &aarith_generic_bench<Div<8>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod8", &aarith_generic_bench<Mod<8>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd16", &aarith_generic_bench<Add<16>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub16", &aarith_generic_bench<Sub<16>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul16", &aarith_generic_bench<NaiveMul<16>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul16", &aarith_generic_bench<BoothMul<16>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv16", &aarith_generic_bench<Div<16>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod16", &aarith_generic_bench<Mod<16>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd32", &aarith_generic_bench<Add<32>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub32", &aarith_generic_bench<Sub<32>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul32", &aarith_generic_bench<NaiveMul<32>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul32", &aarith_generic_bench<BoothMul<32>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv32", &aarith_generic_bench<Div<32>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod32", &aarith_generic_bench<Mod<32>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd64", &aarith_generic_bench<Add<64>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub64", &aarith_generic_bench<Sub<64>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul64", &aarith_generic_bench<NaiveMul<64>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul64", &aarith_generic_bench<BoothMul<64>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv64", &aarith_generic_bench<Div<64>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod64", &aarith_generic_bench<Mod<64>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd128", &aarith_generic_bench<Add<128>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub128", &aarith_generic_bench<Sub<128>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul128", &aarith_generic_bench<NaiveMul<128>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul128", &aarith_generic_bench<BoothMul<128>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv128", &aarith_generic_bench<Div<128>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod128", &aarith_generic_bench<Mod<128>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd256", &aarith_generic_bench<Add<256>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub256", &aarith_generic_bench<Sub<256>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul256", &aarith_generic_bench<NaiveMul<256>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul256", &aarith_generic_bench<BoothMul<256>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv256", &aarith_generic_bench<Div<256>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod256", &aarith_generic_bench<Mod<256>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd512", &aarith_generic_bench<Add<512>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub512", &aarith_generic_bench<Sub<512>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul512", &aarith_generic_bench<NaiveMul<512>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul512", &aarith_generic_bench<BoothMul<512>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv512", &aarith_generic_bench<Div<512>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod512", &aarith_generic_bench<Mod<512>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd1024", &aarith_generic_bench<Add<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub1024", &aarith_generic_bench<Sub<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul1024", &aarith_generic_bench<NaiveMul<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul1024", &aarith_generic_bench<BoothMul<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv1024", &aarith_generic_bench<Div<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod1024", &aarith_generic_bench<Mod<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd2048", &aarith_generic_bench<Add<2048>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub2048", &aarith_generic_bench<Sub<2048>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul2048", &aarith_generic_bench<NaiveMul<2048>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul2048", &aarith_generic_bench<BoothMul<2048>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv2048", &aarith_generic_bench<Div<2048>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod2048", &aarith_generic_bench<Mod<2048>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithAdd4096", &aarith_generic_bench<Add<4096>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub4096", &aarith_generic_bench<Sub<4096>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul4096", &aarith_generic_bench<NaiveMul<4096>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul4096", &aarith_generic_bench<BoothMul<4096>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv4096", &aarith_generic_bench<Div<4096>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod4096", &aarith_generic_bench<Mod<4096>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd8", &mpir_add<8>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub8", &mpir_sub<8>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul8", &mpir_mul<8>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv8", &mpir_div<8>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod8", &mpir_mod<8>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd16", &mpir_add<16>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub16", &mpir_sub<16>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul16", &mpir_mul<16>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv16", &mpir_div<16>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod16", &mpir_mod<16>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd32", &mpir_add<32>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub32", &mpir_sub<32>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul32", &mpir_mul<32>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv32", &mpir_div<32>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod32", &mpir_mod<32>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd64", &mpir_add<64>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub64", &mpir_sub<64>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul64", &mpir_mul<64>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv64", &mpir_div<64>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod64", &mpir_mod<64>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd128", &mpir_add<128>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub128", &mpir_sub<128>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul128", &mpir_mul<128>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv128", &mpir_div<128>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod128", &mpir_mod<128>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd256", &mpir_add<256>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub256", &mpir_sub<256>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul256", &mpir_mul<256>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv256", &mpir_div<256>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod256", &mpir_mod<256>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd512", &mpir_add<512>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub512", &mpir_sub<512>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul512", &mpir_mul<512>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv512", &mpir_div<512>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod512", &mpir_mod<512>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd1024", &mpir_add<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub1024", &mpir_sub<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul1024", &mpir_mul<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv1024", &mpir_div<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod1024", &mpir_mod<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd2048", &mpir_add<2048>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub2048", &mpir_sub<2048>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul2048", &mpir_mul<2048>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv2048", &mpir_div<2048>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod2048", &mpir_mod<2048>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRadd4096", &mpir_add<4096>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub4096", &mpir_sub<4096>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul4096", &mpir_mul<4096>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv4096", &mpir_div<4096>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod4096", &mpir_mod<4096>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
