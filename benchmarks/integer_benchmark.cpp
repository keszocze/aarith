#include <benchmark/benchmark.h>

#include <aarith/integer.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

using namespace std;
using namespace aarith;
using namespace aarith::arithmetic_operators;

template <typename Op> void generic_aarithmetic(benchmark::State& state)
{
    using namespace std;
    using I = typename Op::Type;

    I one;

    if constexpr (aarith::is_integral_v<I>)
    {
        one = I::one();
    }
    else
    {
        one = static_cast<I>(1);
    }

    for (auto _ : state)
    {
        for (I a = std::numeric_limits<I>::min(); a != std::numeric_limits<I>::max(); a = a + one)
        {

            for (I b = std::numeric_limits<I>::min(); b != std::numeric_limits<I>::max();
                 b = b + one)
            {
                benchmark::DoNotOptimize(Op::compute(a, b));
            }

            // extra run for the largest b
            benchmark::DoNotOptimize(Op::compute(a, std::numeric_limits<I>::max()));
        }

        for (I b = std::numeric_limits<I>::min(); b != std::numeric_limits<I>::max(); b = b + one)
        {
            benchmark::DoNotOptimize(Op::compute(std::numeric_limits<I>::max(), b));
        }

        benchmark::DoNotOptimize(
            Op::compute(std::numeric_limits<I>::max(), std::numeric_limits<I>::max()));
    }
}
template <typename I> class Add
{
public:
    using Type = I;

    static I compute(const I a, const I b)
    {
        using namespace aarith::arithmetic_operators;
        return a + b;
    }
};

template <typename I> class Sub
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace aarith::arithmetic_operators;
        return a + b;
    }
};

template <typename I> class Div
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace aarith::arithmetic_operators;
        if (b != static_cast<I>(0))
        {
            return a / b;
        }
        else
        {
            return 0;
        }
    }
};

template <typename I> class Mul
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace aarith::arithmetic_operators;
        return a * b;
    }
};

template <typename I> class NaiveMul
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        return naive_mul(a, b);
    }
};

template <typename I> void scalar_product(benchmark::State& state)
{
    using namespace aarith::arithmetic_operators;
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<I> v1(state.range(0));
        std::vector<I> v2(state.range(0));

        I fst = std::numeric_limits<I>::min();
        I lst = std::numeric_limits<I>::max();

        I one;

        if constexpr (aarith::is_integral_v<I>)
        {
            one = I::one();
        }
        else
        {
            one = static_cast<I>(1);
        }

        for (size_t i = 0; i < state.range(0); ++i)
        {
            v1.push_back(fst);
            v2.push_back(lst);

            fst = fst + one;
            lst = lst - one;
        }
        state.ResumeTiming();

        I result = static_cast<I>(0);
        auto iter1 = std::begin(v1);
        auto iter2 = std::begin(v2);

        for (; iter1 != std::end(v1); ++iter1, ++iter2)
        {
            benchmark::DoNotOptimize(result = result + (*iter1 * *iter2));
        }
    }
}

int main(int argc, char** argv)
{

    // the code below has been generated using a ruby script...

    benchmark::RegisterBenchmark("Mul<aarith::uinteger<8>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::uinteger<10>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::uinteger<12>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::uinteger<14>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::uinteger<16>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("NaiveMul<aarith::uinteger<8>>",
                                 &generic_aarithmetic<NaiveMul<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("NaiveMul<aarith::uinteger<10>>",
                                 &generic_aarithmetic<NaiveMul<aarith::integer<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("NaiveMul<aarith::uinteger<12>>",
                                 &generic_aarithmetic<NaiveMul<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("NaiveMul<aarith::uinteger<14>>",
                                 &generic_aarithmetic<NaiveMul<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("NaiveMul<aarith::uinteger<16>>",
                                 &generic_aarithmetic<NaiveMul<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
