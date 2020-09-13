#include <benchmark/benchmark.h>

#include <aarith/integer.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

using namespace std;
using namespace aarith;
using namespace integer_operators;

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
        using namespace integer_operators;
        return a + b;
    }
};

template <typename I> class Sub
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace integer_operators;
        return a + b;
    }
};

template <typename I> class Div
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace integer_operators;
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

template <typename I> class Mod
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace integer_operators;
        if (b != static_cast<I>(0))
        {
            return a % b;
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
        using namespace integer_operators;
        return a * b;
    }
};

template <typename I> class Karazuba
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        using namespace integer_operators;
        return karazuba(a,b);
    }
};


template <typename I> class BoothMul
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        return booth_mul(a, b);
    }
};

template <typename I> class InPlaceMul
{
public:
    using Type = I;
    static I compute(const I a, const I b)
    {
        return booth_inplace_mul(a, b);
    }
};

template <typename I> void scalar_product(benchmark::State& state)
{
    using namespace integer_operators;
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

benchmark::RegisterBenchmark("Adduint8t", &generic_aarithmetic<Add<uint8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subuint8t", &generic_aarithmetic<Sub<uint8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Muluint8t", &generic_aarithmetic<Mul<uint8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divuint8t", &generic_aarithmetic<Div<uint8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Moduint8t", &generic_aarithmetic<Mod<uint8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addint8t", &generic_aarithmetic<Add<int8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subint8t", &generic_aarithmetic<Sub<int8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulint8t", &generic_aarithmetic<Mul<int8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divint8t", &generic_aarithmetic<Div<int8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modint8t", &generic_aarithmetic<Mod<int8_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Adduint16t", &generic_aarithmetic<Add<uint16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subuint16t", &generic_aarithmetic<Sub<uint16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Muluint16t", &generic_aarithmetic<Mul<uint16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divuint16t", &generic_aarithmetic<Div<uint16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Moduint16t", &generic_aarithmetic<Mod<uint16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addint16t", &generic_aarithmetic<Add<int16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subint16t", &generic_aarithmetic<Sub<int16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulint16t", &generic_aarithmetic<Mul<int16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divint16t", &generic_aarithmetic<Div<int16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modint16t", &generic_aarithmetic<Mod<int16_t>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithinteger8", &generic_aarithmetic<Add<aarith::integer<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithinteger8", &generic_aarithmetic<Sub<aarith::integer<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithinteger8", &generic_aarithmetic<Mul<aarith::integer<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithinteger8", &generic_aarithmetic<Div<aarith::integer<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithinteger8", &generic_aarithmetic<Mod<aarith::integer<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithinteger10", &generic_aarithmetic<Add<aarith::integer<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithinteger10", &generic_aarithmetic<Sub<aarith::integer<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithinteger10", &generic_aarithmetic<Mul<aarith::integer<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithinteger10", &generic_aarithmetic<Div<aarith::integer<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithinteger10", &generic_aarithmetic<Mod<aarith::integer<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithinteger12", &generic_aarithmetic<Add<aarith::integer<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithinteger12", &generic_aarithmetic<Sub<aarith::integer<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithinteger12", &generic_aarithmetic<Mul<aarith::integer<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithinteger12", &generic_aarithmetic<Div<aarith::integer<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithinteger12", &generic_aarithmetic<Mod<aarith::integer<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithinteger14", &generic_aarithmetic<Add<aarith::integer<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithinteger14", &generic_aarithmetic<Sub<aarith::integer<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithinteger14", &generic_aarithmetic<Mul<aarith::integer<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithinteger14", &generic_aarithmetic<Div<aarith::integer<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithinteger14", &generic_aarithmetic<Mod<aarith::integer<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithinteger16", &generic_aarithmetic<Add<aarith::integer<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithinteger16", &generic_aarithmetic<Sub<aarith::integer<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithinteger16", &generic_aarithmetic<Mul<aarith::integer<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithinteger16", &generic_aarithmetic<Div<aarith::integer<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithinteger16", &generic_aarithmetic<Mod<aarith::integer<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithuinteger8", &generic_aarithmetic<Add<aarith::uinteger<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithuinteger8", &generic_aarithmetic<Sub<aarith::uinteger<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithuinteger8", &generic_aarithmetic<Mul<aarith::uinteger<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithuinteger8", &generic_aarithmetic<Div<aarith::uinteger<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithuinteger8", &generic_aarithmetic<Mod<aarith::uinteger<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithuinteger10", &generic_aarithmetic<Add<aarith::uinteger<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithuinteger10", &generic_aarithmetic<Sub<aarith::uinteger<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithuinteger10", &generic_aarithmetic<Mul<aarith::uinteger<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithuinteger10", &generic_aarithmetic<Div<aarith::uinteger<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithuinteger10", &generic_aarithmetic<Mod<aarith::uinteger<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithuinteger12", &generic_aarithmetic<Add<aarith::uinteger<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithuinteger12", &generic_aarithmetic<Sub<aarith::uinteger<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithuinteger12", &generic_aarithmetic<Mul<aarith::uinteger<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithuinteger12", &generic_aarithmetic<Div<aarith::uinteger<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithuinteger12", &generic_aarithmetic<Mod<aarith::uinteger<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithuinteger14", &generic_aarithmetic<Add<aarith::uinteger<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithuinteger14", &generic_aarithmetic<Sub<aarith::uinteger<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithuinteger14", &generic_aarithmetic<Mul<aarith::uinteger<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithuinteger14", &generic_aarithmetic<Div<aarith::uinteger<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithuinteger14", &generic_aarithmetic<Mod<aarith::uinteger<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Addaarithuinteger16", &generic_aarithmetic<Add<aarith::uinteger<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Subaarithuinteger16", &generic_aarithmetic<Sub<aarith::uinteger<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Mulaarithuinteger16", &generic_aarithmetic<Mul<aarith::uinteger<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Divaarithuinteger16", &generic_aarithmetic<Div<aarith::uinteger<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Modaarithuinteger16", &generic_aarithmetic<Mod<aarith::uinteger<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("BoothMulaarithinteger8", &generic_aarithmetic<BoothMul<aarith::integer<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("InPlaceMulaarithinteger8", &generic_aarithmetic<InPlaceMul<aarith::integer<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("BoothMulaarithinteger10", &generic_aarithmetic<BoothMul<aarith::integer<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("InPlaceMulaarithinteger10", &generic_aarithmetic<InPlaceMul<aarith::integer<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("BoothMulaarithinteger12", &generic_aarithmetic<BoothMul<aarith::integer<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("InPlaceMulaarithinteger12", &generic_aarithmetic<InPlaceMul<aarith::integer<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("BoothMulaarithinteger14", &generic_aarithmetic<BoothMul<aarith::integer<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("InPlaceMulaarithinteger14", &generic_aarithmetic<InPlaceMul<aarith::integer<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("BoothMulaarithinteger16", &generic_aarithmetic<BoothMul<aarith::integer<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("InPlaceMulaarithinteger16", &generic_aarithmetic<InPlaceMul<aarith::integer<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Karazubaaarithuinteger8", &generic_aarithmetic<Karazuba<aarith::uinteger<8>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Karazubaaarithuinteger10", &generic_aarithmetic<Karazuba<aarith::uinteger<10>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Karazubaaarithuinteger12", &generic_aarithmetic<Karazuba<aarith::uinteger<12>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Karazubaaarithuinteger14", &generic_aarithmetic<Karazuba<aarith::uinteger<14>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();
benchmark::RegisterBenchmark("Karazubaaarithuinteger16", &generic_aarithmetic<Karazuba<aarith::uinteger<16>>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();


    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
