#!/bin/ruby

types=["uint8_t", "int8_t", "uint16_t", "int16_t", "aarith::uinteger<8>", "aarith::integer<8>", "aarith::uinteger<12>", "aarith::integer<12>", "aarith::uinteger<14>", "aarith::integer<14>", "aarith::uinteger<16>", "aarith::integer<16>"]

arith=["Add", "Sub", "Mul", "Div"]

types.each { |t|
    arith.each { |op|
        puts "benchmark::RegisterBenchmark(\"#{op}<#{t}>\", &generic_aarithmetic<#{op}<#{t}>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();"
    }
}

types.each { |t|
 puts "benchmark::RegisterBenchmark(\"Scalar Product<#{t}>\", &scalar_product<#{t}>) ->Range(1, std::numeric_limits<#{t}>::max()) ->Repetitions(5)->DisplayAggregatesOnly();"
}