#include <benchmark/benchmark.h>

#include <aarith/integer.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

template <typename Op> void generic_aarithmetic(benchmark::State& state)
{
    using namespace std;
    // TODO WTF?! why is there this double nesting?
    using namespace aarith::aarith::arithmetic_operators;
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
        using namespace aarith::aarith::arithmetic_operators;
        return a + b;
    }
};

template <typename I> class Sub
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace aarith::aarith::arithmetic_operators;
        return a + b;
    }
};

template <typename I> class Div
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace aarith::aarith::arithmetic_operators;
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
        using namespace aarith::aarith::arithmetic_operators;
        return a * b;
    }
};

template <typename I> void scalar_product(benchmark::State& state)
{
    using namespace aarith::aarith::arithmetic_operators;
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

    benchmark::RegisterBenchmark("Add<uint8_t>", &generic_aarithmetic<Add<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<uint8_t>", &generic_aarithmetic<Sub<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<uint8_t>", &generic_aarithmetic<Mul<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<uint8_t>", &generic_aarithmetic<Div<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<int8_t>", &generic_aarithmetic<Add<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<int8_t>", &generic_aarithmetic<Sub<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<int8_t>", &generic_aarithmetic<Mul<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<int8_t>", &generic_aarithmetic<Div<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<uint16_t>", &generic_aarithmetic<Add<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<uint16_t>", &generic_aarithmetic<Sub<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<uint16_t>", &generic_aarithmetic<Mul<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<uint16_t>", &generic_aarithmetic<Div<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<int16_t>", &generic_aarithmetic<Add<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<int16_t>", &generic_aarithmetic<Sub<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<int16_t>", &generic_aarithmetic<Mul<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<int16_t>", &generic_aarithmetic<Div<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<aarith::uinteger<8>>",
                                 &generic_aarithmetic<Add<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<aarith::uinteger<8>>",
                                 &generic_aarithmetic<Sub<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::uinteger<8>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<aarith::uinteger<8>>",
                                 &generic_aarithmetic<Div<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<aarith::integer<8>>",
                                 &generic_aarithmetic<Add<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<aarith::integer<8>>",
                                 &generic_aarithmetic<Sub<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::integer<8>>",
                                 &generic_aarithmetic<Mul<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<aarith::integer<8>>",
                                 &generic_aarithmetic<Div<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<aarith::uinteger<12>>",
                                 &generic_aarithmetic<Add<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<aarith::uinteger<12>>",
                                 &generic_aarithmetic<Sub<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::uinteger<12>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<aarith::uinteger<12>>",
                                 &generic_aarithmetic<Div<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<aarith::integer<12>>",
                                 &generic_aarithmetic<Add<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<aarith::integer<12>>",
                                 &generic_aarithmetic<Sub<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::integer<12>>",
                                 &generic_aarithmetic<Mul<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<aarith::integer<12>>",
                                 &generic_aarithmetic<Div<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<aarith::uinteger<14>>",
                                 &generic_aarithmetic<Add<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<aarith::uinteger<14>>",
                                 &generic_aarithmetic<Sub<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::uinteger<14>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<aarith::uinteger<14>>",
                                 &generic_aarithmetic<Div<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<aarith::integer<14>>",
                                 &generic_aarithmetic<Add<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<aarith::integer<14>>",
                                 &generic_aarithmetic<Sub<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::integer<14>>",
                                 &generic_aarithmetic<Mul<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<aarith::integer<14>>",
                                 &generic_aarithmetic<Div<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<aarith::uinteger<16>>",
                                 &generic_aarithmetic<Add<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<aarith::uinteger<16>>",
                                 &generic_aarithmetic<Sub<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::uinteger<16>>",
                                 &generic_aarithmetic<Mul<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<aarith::uinteger<16>>",
                                 &generic_aarithmetic<Div<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Add<aarith::integer<16>>",
                                 &generic_aarithmetic<Add<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Sub<aarith::integer<16>>",
                                 &generic_aarithmetic<Sub<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mul<aarith::integer<16>>",
                                 &generic_aarithmetic<Mul<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Div<aarith::integer<16>>",
                                 &generic_aarithmetic<Div<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Scalar Product<uint8_t>", &scalar_product<uint8_t>)
        ->Range(1, std::numeric_limits<uint8_t>::max())
        ->Repetitions(5)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Scalar Product<int8_t>", &scalar_product<int8_t>)
        ->Range(1, std::numeric_limits<int8_t>::max())
        ->Repetitions(5)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Scalar Product<uint16_t>", &scalar_product<uint16_t>)
        ->Range(1, std::numeric_limits<uint16_t>::max())
        ->Repetitions(5)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Scalar Product<int16_t>", &scalar_product<int16_t>)
        ->Range(1, std::numeric_limits<int16_t>::max())
        ->Repetitions(5)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Scalar Product<aarith::uinteger<8>>",
                                 &scalar_product<aarith::uinteger<8>>)
        ->Range(1, std::numeric_limits<int8_t>::max())
        ->Repetitions(5)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Scalar Product<aarith::integer<8>>",
                                 &scalar_product<aarith::integer<8>>)
        ->Range(1, std::numeric_limits<int8_t>::max())
        ->Repetitions(5)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Scalar Product<aarith::uinteger<16>>",
                                 &scalar_product<aarith::uinteger<16>>)
        ->Range(1, std::numeric_limits<int16_t>::max())
        ->Repetitions(5)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Scalar Product<aarith::integer<16>>",
                                 &scalar_product<aarith::integer<16>>)
        ->Range(1, std::numeric_limits<int16_t>::max())
        ->Repetitions(5)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
