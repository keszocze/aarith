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

    for (auto [a, counter_a] = std::tuple<T, count_type>{std::numeric_limits<T>::min(),
                                                         std::numeric_limits<T>::min()};
         counter_a <= std::numeric_limits<T>::max(); ++a, ++counter_a)
    {
        for (auto [b, counter_b] = std::tuple<T, count_type>{std::numeric_limits<T>::min(),
                                                             std::numeric_limits<T>::min()};
             counter_b <= std::numeric_limits<T>::max(); ++counter_b, ++b)
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

template <template <size_t> typename I, size_t W> void bench_aarith_int(std::string name)
{
    using namespace std;
    using namespace chrono;

    using Integer = I<W>;
    using count_type = I<W + 1>;
    constexpr count_type max_val{std::numeric_limits<Integer>::max()};

    const steady_clock::time_point start = std::chrono::steady_clock::now();

    Integer a = std::numeric_limits<Integer>::min();
    count_type counter_a = std::numeric_limits<Integer>::min();

    std::cout << counter_a << "\t" << std::numeric_limits<Integer>::min() << "/" << max_val << " " << (counter_a < max_val) << "\n";

    while (counter_a <= max_val)
    {

        Integer b = std::numeric_limits<Integer>::min();
        count_type counter_b = std::numeric_limits<Integer>::min();

        while (counter_b <= max_val)
        {
            const I<W> sum = add(a, b);
            const I<W> diff = sub(a, b);
            const I<W> prod = mul(a, b);
            const I<W> fun = add(mul(add(a, b), b), a);

            //            std::cout << a << "\t" << b << "\n";

            counter_b = add(counter_b, I<W + 1>::one());
            b = add(b, Integer::one());
        }

        counter_a = add(counter_a, I<W + 1>::one());
        a = add(a, Integer::one());
    }

    const steady_clock::time_point end = std::chrono::steady_clock::now();
    const steady_clock::duration duration = end - start;
    const auto dur = duration_cast<std::chrono::milliseconds>(duration).count();

    std::cout << name << "<" << W << ">;" << dur << "\n";
}

int main()
{

    using namespace std;
    using namespace aarith;

    //    bench_int<int8_t>("int8");
    //    bench_int<uint8_t>("uint8");
    //    bench_int<int16_t>("int16");
    //    bench_int<uint16_t>("uint16");
    //
    bench_aarith_int<integer, 8>("integer");
        bench_aarith_int<uinteger, 8>("uinteger");
    bench_aarith_int<integer, 16>("integer");
    //    bench_aarith_int<uinteger, 16>("uinteger");

    return 0;
}