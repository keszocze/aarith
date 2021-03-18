function(add_aarith_benchmark benchname)

    SET(targetname "${benchname}-benchmark")


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
    target_link_libraries(${targetname} PRIVATE benchmark::benchmark)
    target_include_directories(${targetname} PRIVATE ${PROJECT_SOURCE_DIR}/lib/benchmark/include)

endfunction(add_aarith_benchmark)