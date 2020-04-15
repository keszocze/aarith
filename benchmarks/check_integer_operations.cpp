#include <aarith/integer.hpp>

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

template <template <size_t, typename> class I, size_t W, typename T, typename WordType = uint64_t>
void check_addition(const std::string op_name)
{
    using Integer = I<W, WordType>;
    using count_type = I<W + 1, WordType>;
    constexpr count_type max_val{std::numeric_limits<Integer>::max()};
    constexpr count_type big_one = count_type::one();
    constexpr I small_one = Integer::one();

    Integer a = std::numeric_limits<Integer>::min();
    count_type counter_a = std::numeric_limits<Integer>::min();
    T base_a = std::numeric_limits<T>::min();

    std::string filename{""};

    if constexpr (::aarith::is_unsigned_v<Integer>)
    {
        filename += "uinteger";
    }
    else
    {
        filename += "integer";
    }
    filename += std::to_string(W) +"_" + op_name + ".log";

//    std::cout << filename << "\n";

    std::ofstream file;

    file.open(filename);

    while (counter_a <= max_val)
    {

        Integer b = std::numeric_limits<Integer>::min();
        count_type counter_b = std::numeric_limits<Integer>::min();
        T base_b = std::numeric_limits<T>::min();

        while (counter_b <= max_val)
        {

            T result_base = base_a + base_b;
            Integer result_int = add(a, b);

            if (Integer{result_base} != result_int)
            {

                file << int64_t{base_a} << ";" << int64_t{base_b} << ";" << int64_t{result_base}
                     << ";" << a << ";" << b << ";" << result_int << "\n";
                return;
            }

            counter_b = add(counter_b, big_one);
            b = add(b, small_one);
            ++base_b;
        }

        counter_a = add(counter_a, big_one);
        a = add(a, small_one);
        ++base_a;
    }

    file.close();
}

int main(int argc, char* argv[])
{
    using namespace std;
    using namespace aarith;

    if (argc == 1 || (argc == 2 && std::string{argv[1]} == std::string{"small"}))
    {
        check_addition<uinteger, 8, uint8_t>("addition");
        check_addition<integer, 8, int8_t>("addition");
        check_addition<uinteger, 16, uint16_t>("addition");
        check_addition<integer, 16, int16_t>("addition");
    }
    else if (argc == 2 && std::string{argv[1]} == std::string{"large"})
    {
    }

    return 0;
}