add_executable(int-benchmark integer_benchmark.cpp)
target_link_libraries(int-benchmark PRIVATE aarith::Library)
target_compile_options(int-benchmark PRIVATE "-O3")