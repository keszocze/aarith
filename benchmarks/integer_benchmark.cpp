#include <aarith/integer.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

template <typename T> void bench_int(std::string name)
{
    using namespace std;
    using namespace chrono;

    using count_type = int64_t;

    const steady_clock::time_point start = std::chrono::steady_clock::now();

    constexpr count_type max_val = std::numeric_limits<T>::max();
    constexpr count_type min_val = std::numeric_limits<T>::min();

    for (auto [a, counter_a] = std::tuple<T, count_type>{min_val, min_val}; counter_a <= max_val;
         ++a, ++counter_a)
    {
        for (auto [b, counter_b] = std::tuple<T, count_type>{min_val, min_val};
             counter_b <= max_val; ++counter_b, ++b)
        {
            const T sum = a + b;
            const T diff = a - b;
            const T prod = a * b;
            const T fun = ((a + b) * b) + a;
        }
    }

    const steady_clock::time_point end = std::chrono::steady_clock::now();
    const steady_clock::duration duration = end - start;
    const auto dur = duration_cast<std::chrono::milliseconds>(duration).count();

    std::cout << name << ";" << dur << "\n";
}

template <template <size_t, typename> typename I, size_t W, typename WordType = uint64_t>
void bench_aarith_int(std::string name)
{
    using namespace std;
    using namespace chrono;

    using Integer = I<W, WordType>;
    using count_type = I<W + 1, WordType>;
    constexpr count_type max_val{std::numeric_limits<Integer>::max()};
    constexpr count_type big_one = count_type::one();
    constexpr I small_one = Integer::one();

    const steady_clock::time_point start = std::chrono::steady_clock::now();

    Integer a = std::numeric_limits<Integer>::min();
    count_type counter_a = std::numeric_limits<Integer>::min();

    //    std::cout << counter_a << "\t" << std::numeric_limits<Integer>::min() << "/" << max_val <<
    //    " "
    //              << (counter_a < max_val) << "\n";

    while (counter_a <= max_val)
    {

        Integer b = std::numeric_limits<Integer>::min();
        count_type counter_b = std::numeric_limits<Integer>::min();

        while (counter_b <= max_val)
        {
            const Integer sum = add(a, b);
            const Integer diff = sub(a, b);
            const Integer prod = mul(a, b);
            const Integer fun = add(mul(add(a, b), b), a);

            counter_b = add(counter_b, big_one);
            b = add(b, small_one);
        }

        counter_a = add(counter_a, big_one);
        a = add(a, small_one);
    }

    const steady_clock::time_point end = std::chrono::steady_clock::now();
    const steady_clock::duration duration = end - start;
    const auto dur = duration_cast<std::chrono::milliseconds>(duration).count();

    std::cout << name << "<" << W << ">;" << dur << "\n";
}

int main(int argc, char* argv[])
{
    using namespace std;
    using namespace aarith;

    if (argc == 1 || (argc == 2 && std::string{argv[1]} == std::string{"small"}))
    {
        bench_int<int8_t>("int8");
        bench_int<uint8_t>("uint8");
        bench_int<int16_t>("int16");
        bench_int<uint16_t>("uint16");

        bench_aarith_int<integer, 8>("integer");
        bench_aarith_int<uinteger, 8>("uinteger");
        bench_aarith_int<integer, 16>("integer");
        bench_aarith_int<uinteger, 16>("uinteger");

        bench_int<int8_t>("int8");
        bench_aarith_int<uinteger, 16>("uinteger");
        bench_int<uint8_t>("uint8");
        bench_aarith_int<integer, 16>("integer");
        bench_int<int16_t>("int16");
        bench_int<uint16_t>("uint16");
        bench_aarith_int<integer, 8>("integer");
        bench_aarith_int<uinteger, 8>("uinteger");
    }
    else if (argc == 2 && std::string{argv[1]} == std::string{"large"})
    {
        bench_int<int32_t>("int32");
        bench_int<uint32_t>("uint32");

        bench_aarith_int<integer, 32>("integer");
        bench_aarith_int<uinteger, 32>("uinteger");

        bench_int<int32_t>("int32");
        bench_aarith_int<integer, 32>("integer");
        bench_int<uint32_t>("uint32");
        bench_aarith_int<uinteger, 32>("uinteger");
    }

    return 0;
}