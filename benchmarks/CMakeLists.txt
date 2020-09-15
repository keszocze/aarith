

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
    target_link_libraries(${targetname} PRIVATE benchmark::benchmark)
    target_include_directories(${targetname} PRIVATE ${PROJECT_SOURCE_DIR}/lib/benchmark/include)

endfunction(add_aarith_benchmark)


add_aarith_benchmark(integer-timing FILES integer_benchmark.cpp)
add_aarith_benchmark(fau_adder-timing FILES fau_adder_benchmark.cpp)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_MODULE_PATH}")
#find_package(MPFR)

#if (MPFR_FOUND)
  #message(STATUS "MPFR found, building benchmarks")
  #message(STATUS "LIB: ${MPFR_LIBRARIES}")
  #message(STATUS "INC: ${MPFR_INCLUDE_DIR}")

  #add_executable(mpfr-comparison
    #mpfr_benchmark.cpp
  #)
  #target_include_directories(mpfr-comparison PRIVATE ${MPFR_INCLUDE_DIR})
  #target_link_libraries(mpfr-comparison PRIVATE ${MPFR_LIBRARY_RELEASE})
#else()
  #message(STATUS "MPFR not found, not building MPFR benchmarks")
#endif()

if(MPIR_FOUND)
  add_executable(mpir-comparison   mpir_benchmark.cpp  )
  target_compile_options(mpir-comparison PRIVATE "-O3")
  #target_include_directories(mpfr-comparison PRIVATE ${MPFR_INCLUDE_DIR})
  target_include_directories(mpir-comparison PRIVATE ${PROJECT_SOURCE_DIR}/lib/benchmark/include ${MPIR_INCLUDE_DIR})
  target_link_libraries(mpir-comparison PRIVATE aarith::Library benchmark::benchmark ${MPIR_LIBRARIES})
  message(STATUS "MPIR found: Building MPIR benchmarks")
else()
    message(STATUS "Could not find MPIR: Not building MPIR benchmarks")
endif()


