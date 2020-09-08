#!/bin/ruby

internal_types=["uint8_t", "int8_t", "uint16_t", "int16_t"]
aarith_signed_types=["aarith::integer<8>", "aarith::integer<10>", "aarith::integer<12>", "aarith::integer<14>",  "aarith::integer<16>"]
aarith_unsigned_types=["aarith::uinteger<8>", "aarith::uinteger<10>",  "aarith::uinteger<12>", "aarith::uinteger<14>", "aarith::uinteger<16>"]
aarith_types = aarith_signed_types + aarith_unsigned_types
types = internal_types + aarith_types

operations=["Add", "Sub", "Mul", "Div"]

#types.each { |t|
aarith_signed_types.each {|t|
    #operations.each { |op|
    ["Mul"].each { |op|
        puts "benchmark::RegisterBenchmark(\"#{op}<#{t}>\", &generic_aarithmetic<#{op}<#{t}>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();"
    }
}

aarith_unsigned_types.each { |t|
    puts "benchmark::RegisterBenchmark(\"NaiveMul<#{t}>\", &generic_aarithmetic<NaiveMul<#{t}>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();"
}

types.each { |t|
# puts "benchmark::RegisterBenchmark(\"Scalar Product<#{t}>\", &scalar_product<#{t}>) ->Range(1, std::numeric_limits<#{t}>::max()) ->Repetitions(5)->DisplayAggregatesOnly();"
}