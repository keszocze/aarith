#include <aarith/integer.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

template <template <size_t, typename> class I, size_t W, typename WordType, typename T>
void check_addition()
{
    using Integer = I<W, WordType>;
    using count_type = I<W + 1, WordType>;
    constexpr count_type max_val{std::numeric_limits<Integer>::max()};
    constexpr count_type big_one = count_type::one();
    constexpr I small_one = Integer::one();

    Integer a = std::numeric_limits<Integer>::min();
    count_type counter_a = std::numeric_limits<Integer>::min();
    T base_a = std::numeric_limits<T>::min();

    while (counter_a <= max_val)
    {

        Integer b = std::numeric_limits<Integer>::min();
        count_type counter_b = std::numeric_limits<Integer>::min();
        T base_b = std::numeric_limits<T>::min();

        while (counter_b <= max_val)
        {

            T result_base = base_a + base_b;
            Integer result_int = add(a, b);
            std::cout << int64_t{base_a} << " + " << int64_t{base_b} << " = "
                      << int64_t{result_base} << "\n";
            std::cout << a << " + " << b << " = " << result_int << "\n";

            if (Integer{result_base} != result_int)
            {
                std::cout << "KAPOTT!!!!\n";
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
}

int main(int argc, char* argv[])
{
    using namespace std;
    using namespace aarith;

    if (argc == 1 || (argc == 2 && std::string{argv[1]} == std::string{"small"}))
    {
        check_addition<uinteger, 8, uint64_t, uint8_t>();
        check_addition<uinteger, 8, uint8_t, uint8_t>();
        check_addition<integer, 8, uint64_t, int8_t>();
    }
    else if (argc == 2 && std::string{argv[1]} == std::string{"large"})
    {
    }

    return 0;
}