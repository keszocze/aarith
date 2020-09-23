




add_aarith_benchmark(integer-timing FILES integer_benchmark.cpp)
add_aarith_benchmark(fau_adder-timing FILES fau_adder_benchmark.cpp)


if(MPIR_FOUND)
  message(STATUS "MPIR found: Building MPIR benchmarks")
  add_aarith_benchmark(mpir-comparison FILES mpir_benchmark.cpp LIBS ${MPIR_LIBRARIES} INCLUDES ${MPIR_INCLUDE_DIR})
else()
    message(STATUS "Could not find MPIR: Not building MPIR benchmarks")
endif()


