#include <benchmark/benchmark.h>

#include <aarith/integer.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

using namespace std;
using namespace aarith;
using namespace integer_operators;

namespace aarith::helpers {

template <typename Op> void ::aarith::helpers::generic_arithmetic(benchmark::State& state)
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

    static I compute(const I& a, const I& b)
    {
        using namespace integer_operators;
        return a + b;
    }
};

template <typename I> class Sub
{
public:
    using Type = I;
    static I compute(const I& a, const I& b)
    {
        using namespace integer_operators;
        return a + b;
    }
};

template <typename I> class Div
{
public:
    using Type = I;
    static I compute(const I& a, const I& b)
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
    static I compute(const I& a, const I& b)
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

template <typename I> class NaiveMul
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
    static I compute(const I& a, const I& b)
    {
        using namespace integer_operators;
        return karazuba(a, b);
    }
};

template <typename I> class BoothMul
{
public:
    using Type = I;
    static I compute(const I& a, const I& b)
    {
        return booth_mul(a, b);
    }
};

template <typename I> class InPlaceMul
{
public:
    using Type = I;
    static I compute(const I& a, const I& b)
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

        for (; iter1 != std::end(v1); ++iter1)
        {
            benchmark::DoNotOptimize(result = result + (*iter1 * *iter2));
            ++iter2;
        }
    }
}
} // namespace aarith::helpers

int main(int argc, char** argv)
{

    // the code below has been generated using a ruby script...

    benchmark::RegisterBenchmark("Adduint8t", &::aarith::helpers::generic_arithmetic<Add<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subuint8t", &::aarith::helpers::generic_arithmetic<Sub<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Muluint8t", &::aarith::helpers::generic_arithmetic<NaiveMul<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divuint8t", &::aarith::helpers::generic_arithmetic<Div<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Moduint8t", &::aarith::helpers::generic_arithmetic<Mod<uint8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addint8t", &::aarith::helpers::generic_arithmetic<Add<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subint8t", &::aarith::helpers::generic_arithmetic<Sub<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulint8t", &::aarith::helpers::generic_arithmetic<NaiveMul<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divint8t", &::aarith::helpers::generic_arithmetic<Div<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modint8t", &::aarith::helpers::generic_arithmetic<Mod<int8_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Adduint16t", &::aarith::helpers::generic_arithmetic<Add<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subuint16t", &::aarith::helpers::generic_arithmetic<Sub<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Muluint16t", &::aarith::helpers::generic_arithmetic<NaiveMul<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divuint16t", &::aarith::helpers::generic_arithmetic<Div<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Moduint16t", &::aarith::helpers::generic_arithmetic<Mod<uint16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addint16t", &::aarith::helpers::generic_arithmetic<Add<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subint16t", &::aarith::helpers::generic_arithmetic<Sub<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulint16t", &::aarith::helpers::generic_arithmetic<NaiveMul<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divint16t", &::aarith::helpers::generic_arithmetic<Div<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modint16t", &::aarith::helpers::generic_arithmetic<Mod<int16_t>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithinteger8", &::aarith::helpers::generic_arithmetic<Add<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithinteger8", &::aarith::helpers::generic_arithmetic<Sub<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithinteger8",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithinteger8", &::aarith::helpers::generic_arithmetic<Div<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithinteger8", &::aarith::helpers::generic_arithmetic<Mod<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithinteger10",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::integer<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithinteger10",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::integer<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithinteger10",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::integer<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithinteger10",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::integer<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithinteger10",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::integer<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithinteger12",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithinteger12",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithinteger12",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithinteger12",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithinteger12",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithinteger14",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithinteger14",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithinteger14",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithinteger14",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithinteger14",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithinteger16",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithinteger16",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithinteger16",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithinteger16",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithinteger16",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithuinteger8",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithuinteger8",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithuinteger8",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithuinteger8",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithuinteger8",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithuinteger10",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::uinteger<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithuinteger10",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::uinteger<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithuinteger10",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::uinteger<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithuinteger10",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::uinteger<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithuinteger10",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::uinteger<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithuinteger12",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithuinteger12",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithuinteger12",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithuinteger12",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithuinteger12",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithuinteger14",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithuinteger14",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithuinteger14",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithuinteger14",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithuinteger14",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Addaarithuinteger16",
                                 &::aarith::helpers::generic_arithmetic<Add<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Subaarithuinteger16",
                                 &::aarith::helpers::generic_arithmetic<Sub<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Mulaarithuinteger16",
                                 &::aarith::helpers::generic_arithmetic<NaiveMul<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Divaarithuinteger16",
                                 &::aarith::helpers::generic_arithmetic<Div<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Modaarithuinteger16",
                                 &::aarith::helpers::generic_arithmetic<Mod<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("BoothMulaarithinteger8",
                                 &::aarith::helpers::generic_arithmetic<BoothMul<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("InPlaceMulaarithinteger8",
                                 &::aarith::helpers::generic_arithmetic<InPlaceMul<aarith::integer<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("BoothMulaarithinteger10",
                                 &::aarith::helpers::generic_arithmetic<BoothMul<aarith::integer<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("InPlaceMulaarithinteger10",
                                 &::aarith::helpers::generic_arithmetic<InPlaceMul<aarith::integer<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("BoothMulaarithinteger12",
                                 &::aarith::helpers::generic_arithmetic<BoothMul<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("InPlaceMulaarithinteger12",
                                 &::aarith::helpers::generic_arithmetic<InPlaceMul<aarith::integer<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("BoothMulaarithinteger14",
                                 &::aarith::helpers::generic_arithmetic<BoothMul<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("InPlaceMulaarithinteger14",
                                 &::aarith::helpers::generic_arithmetic<InPlaceMul<aarith::integer<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("BoothMulaarithinteger16",
                                 &::aarith::helpers::generic_arithmetic<BoothMul<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("InPlaceMulaarithinteger16",
                                 &::aarith::helpers::generic_arithmetic<InPlaceMul<aarith::integer<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Karazubaaarithuinteger8",
                                 &::aarith::helpers::generic_arithmetic<Karazuba<aarith::uinteger<8>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Karazubaaarithuinteger10",
                                 &::aarith::helpers::generic_arithmetic<Karazuba<aarith::uinteger<10>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Karazubaaarithuinteger12",
                                 &::aarith::helpers::generic_arithmetic<Karazuba<aarith::uinteger<12>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Karazubaaarithuinteger14",
                                 &::aarith::helpers::generic_arithmetic<Karazuba<aarith::uinteger<14>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();
    benchmark::RegisterBenchmark("Karazubaaarithuinteger16",
                                 &::aarith::helpers::generic_arithmetic<Karazuba<aarith::uinteger<16>>>)
        ->Unit(benchmark::kMillisecond)
        ->Repetitions(5)
        ->DisplayAggregatesOnly();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
