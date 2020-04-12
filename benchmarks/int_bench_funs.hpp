#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

#include <aarith/integer.hpp>

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
    count_type max_val = std::numeric_limits<count_type>::min();

    const steady_clock::time_point start = std::chrono::steady_clock::now();

    Integer a = std::numeric_limits<Integer>::min();
    count_type counter_a = std::numeric_limits<count_type>::min();


    while(counter_a <= max_val)
    {

        Integer b = std::numeric_limits<Integer>::min();
        count_type counter_b = std::numeric_limits<count_type>::min();


        while (counter_b <= max_val)
        {
            const I<W> sum = add(a, b);
            const I<W> diff = sub(a, b);
            const I<W> prod = mul(a, b);
            const I<W> fun = add(mul(add(a, b), b), a);

            counter_b = add(counter_b, I<W + 1>::one());
            b = add(b, Integer::one());
        }

        counter_a = add(counter_a, I<W + 1>::one());
        a = add(a,Integer::one());
    }

    const steady_clock::time_point end = std::chrono::steady_clock::now();
    const steady_clock::duration duration = end - start;
    const auto dur = duration_cast<std::chrono::milliseconds>(duration).count();

    std::cout << name << "<" << W << ">;" << dur << "\n";
}
