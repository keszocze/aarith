#include <benchmark/benchmark.h>

#include "../tests/integer/fau_adder.hpp"
#include <aarith/integer.hpp>

template <size_t width, size_t lsp_width, size_t shared_bits = 0>
void wrapper(benchmark::State& state)
{
    using ResultType = aarith::uinteger<2 * width>;
    using T = std::tuple<ResultType, ResultType, ResultType>;

    T res;

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(res = aarith::eval_fau_adder<width, lsp_width, shared_bits>());
    }
}

int main(int argc, char** argv)
{

    constexpr size_t repetitions = 5;
    using namespace aarith;

    benchmark::RegisterBenchmark("FAU Adder n=8, m=4, p=1", &::wrapper<8, 4, 1>)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly()
        ->Repetitions(repetitions);
    benchmark::RegisterBenchmark("FAU Adder n=8, m=4, p=3", &::wrapper<8, 4, 3>)
        ->Unit(benchmark::kMillisecond)
        ->DisplayAggregatesOnly()
        ->Repetitions(repetitions);
    benchmark::RegisterBenchmark("FAU Adder n=16, m=8, p=1", &::wrapper<16, 8, 1>)
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("FAU Adder n=16, m=8, p=3", &::wrapper<16, 8, 3>)
        ->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("FAU Adder n=32, m=16, p=1", &::wrapper<32, 16, 1>)
        ->Unit(benchmark::kMillisecond);

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
