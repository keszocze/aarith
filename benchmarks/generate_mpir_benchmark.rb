#!/bin/ruby



widths=["8","16","1024"]

aarith_operations=["Add", "Sub", "NaiveMul", "BoothMul", "Div", "Mod"]
mpir_operations=["add", "sub", "mul", "div", "mod"]


widths.each { |w|
    aarith_operations.each { |op|
        puts "benchmark::RegisterBenchmark(\"Aarith#{op}#{w}\", &aarith_generic_bench<#{op}<#{w}>>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();"
    }
}

widths.each { |w|
    mpir_operations.each { |op|
    puts "benchmark::RegisterBenchmark(\"MPIR#{op}#{w}\", &mpir_#{op}<#{w}>) ->Unit(benchmark::kMillisecond) ->Repetitions(5) ->DisplayAggregatesOnly();"
    }
}

