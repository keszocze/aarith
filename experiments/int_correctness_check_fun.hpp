#pragma once

#include <aarith/integer.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

auto native_add = [](const auto a, const auto b) { return a + b; };
auto native_sub = [](const auto a, const auto b) { return a - b; };
auto native_mul = [](const auto a, const auto b) { return a * b; };
auto native_div = [](const auto a, const auto b) { return a / b; };
auto native_mod = [](const auto a, const auto b) { return a % b; };

template <size_t W, typename T> constexpr T width_min()
{

    if constexpr (::aarith::is_unsigned_int<T>)
    {
        return T{0};
    }
    else
    {
        static_assert(W <= 8 * sizeof(T));

        T val{1};
        val <<= T{W - 1};

        return -val;
    }
}

template <typename T, typename B> std::string description(const std::string op_name)
{
    std::string filename_{""};

    if constexpr (::aarith::is_unsigned_v<T>)
    {
        filename_ += "uinteger";
    }
    else
    {
        filename_ += "integer";
    }
    filename_ +=
        std::to_string(T::width()) + "_word" + std::to_string(8*sizeof(B)) + "_" + op_name;

    return filename_;
}

template <typename T, typename B> std::string filename(const std::string op_name)
{
    std::string filename_{description<T, B>(op_name)};

    filename_ += ".log";

    return filename_;
}

template <template <size_t, typename> class I, size_t W, typename NativeType, bool AvoidZero,
          typename Op, typename OpNative, typename WordType = uint64_t>
void check_int_operation(const std::string op_name, const Op& fun, OpNative& fun_native)
{

    static_assert(8 * sizeof(NativeType) >= W,
                  "The native type must be able to store the values of the (u)integer");
    static_assert(
        sizeof(NativeType) <= sizeof(WordType),
        "The native type must be small enough that the I(NativeType) constructor is applicable");
    static_assert(W < 64, "Only up to (u)integer<63> is supported");

    using Integer = I<W, WordType>;
    using count_type = I<W + 1, WordType>;

    constexpr count_type max_val{std::numeric_limits<Integer>::max()};
    constexpr count_type big_one = count_type::one();
    constexpr I small_one = Integer::one();

    Integer a = std::numeric_limits<Integer>::min();
    count_type counter_a = std::numeric_limits<Integer>::min();

    // TODO compute this correctly!
    constexpr NativeType native_min = width_min<W, NativeType>();
    NativeType native_a = native_min;

    std::ofstream file;
    file.open(filename<Integer, NativeType>(op_name));

    //    std::cout << description<Integer>(op_name) << "\t" << int64_t{native_min} << "\n";
    //    return;

    bool error = false;

    while (counter_a <= max_val)
    {

        Integer b = std::numeric_limits<Integer>::min();
        count_type counter_b = std::numeric_limits<Integer>::min();

        NativeType native_b = native_min;

        while (counter_b <= max_val)
        {

            if constexpr (AvoidZero)
            {
                if (native_b != 0)
                {
                    NativeType result_base = fun_native(native_a, native_b);
                    Integer result_int = fun(a, b);

                    if (Integer{result_base} != result_int)
                    {
                        error = true;

                        file << int64_t{native_a} << ";" << int64_t{native_b} << ";"
                             << int64_t{result_base} << ";" << a << ";" << b << ";" << result_int
                             << "\n";
                    }
                }
            }
            else
            {
                NativeType result_base = fun_native(native_a, native_b);
                Integer result_int = fun(a, b);

                std::cout << int64_t{result_base} << ";" << result_int << "\n";

                if (Integer{result_base} != result_int)
                {
                    error = true;

                    file << int64_t{native_a} << ";" << int64_t{native_b} << ";"
                         << int64_t{result_base} << ";" << a << ";" << b << ";" << result_int
                         << "\n";
                }
            }

            counter_b = add(counter_b, big_one);
            b = add(b, small_one);
            ++native_b;
        }

        counter_a = add(counter_a, big_one);
        a = add(a, small_one);
        ++native_a;
    }

    file.close();

    if (error)
    {
        std::cout << "ERROR in " << description<Integer, NativeType>(op_name) << "\n";
    }
}
