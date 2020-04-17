function(add_aarith_benchmark testname)

    SET(targetname "${testname}-benchmark")


    set (multiValueArgs FILES)
    cmake_parse_arguments(
            AARITH_BENCH # prefix
            "" # no flags
            "" # no single value options
            "FILES;LIBS;INCLUDES" # multi value options
            ${ARGN}
    )

    add_executable(${targetname} ${AARITH_BENCH_FILES})
    target_link_libraries(${targetname} PRIVATE aarith::Library ${AARITH_BENCH_LIBS})


    if (DEFINED AARITH_BENCH_INCLUDES)
        target_include_directories(${targetname} PRIVATE ${AARITH_BENCH_INCLUDES})
    endif()

    target_compile_options(${targetname} PRIVATE "-O3")

endfunction(add_aarith_benchmark)


add_aarith_benchmark(integer-timing FILES integer_benchmark.cpp)

