#include "int_bench_funs.hpp"

#include <aarith/integer.hpp>

int main()
{

    using namespace std;
    using namespace aarith;

    bench_int<int8_t>("int8");
    bench_int<uint8_t>("uint8");
//    bench_int<int16_t>("int16");
//    bench_int<uint16_t>("uint16");

    bench_aarith_int<integer,8>("integer");
    bench_aarith_int<uinteger,8>("uinteger");
    return 0;
}