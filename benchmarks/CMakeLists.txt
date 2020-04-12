add_executable(int-benchmark int8_bench.cpp)
target_link_libraries(int-benchmark PRIVATE aarith::Library)