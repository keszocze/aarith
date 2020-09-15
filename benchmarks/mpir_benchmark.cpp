#include <benchmark/benchmark.h>

#include <aarith/integer.hpp>
#include <mpir.h>

#include <chrono>
#include <iostream>
#include <string>
using namespace aarith;

template <size_t W> class Init;

template <> class Init<1024>
{
public:
    using Type = integer<1024>;

    static constexpr Type val_a()
    {
        return Type::one() << 1022;
    }

    static constexpr Type val_b()
    {
        return Type::one() << 511;
    }

    static constexpr size_t n_iter() {
        return 10000;
    }
};

template <size_t W> class Init {
public:
    using Type = integer<W>;

    static constexpr Type val_a()
    {
        return Type::min();
    }

    static constexpr Type val_b()
    {
        return Type::min();
    }

    static constexpr size_t n_iter() {
        return size_t{1} << W;
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
        for (size_t i = 0; i < n_iter; ++i)
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
        if (b.is_zero()) {
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
        if (b.is_zero()) {
            return Type::zero();
        }
        return remainder(a, b);
    }
};


template<size_t W>
void mpir_add(benchmark::State& state)
{


    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};

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

template<size_t W>
void mpir_sub(benchmark::State& state)
{
    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};
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

template<size_t W>
void mpir_mul(benchmark::State& state)
{
    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};
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


template<size_t W>
void mpir_div(benchmark::State& state)
{
    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};

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


template<size_t W>
void mpir_mod(benchmark::State& state)
{
    integer<W> a{Init<W>::val_a()};
    integer<W> b{Init<W>::val_b()};

    using S = Init<W>;

    constexpr size_t n_iter{S::n_iter()};

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
                mpz_mod(tmp_mpir, a_mpir, b_mpir);
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
benchmark::RegisterBenchmark("AarithAdd1024", &aarith_generic_bench<Add<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithSub1024", &aarith_generic_bench<Sub<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithNaiveMul1024", &aarith_generic_bench<NaiveMul<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithBoothMul1024", &aarith_generic_bench<BoothMul<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithDiv1024", &aarith_generic_bench<Div<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("AarithMod1024", &aarith_generic_bench<Mod<1024>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
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
benchmark::RegisterBenchmark("MPIRadd1024", &mpir_add<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRsub1024", &mpir_sub<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmul1024", &mpir_mul<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRdiv1024", &mpir_div<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("MPIRmod1024", &mpir_mod<1024>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();


    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
