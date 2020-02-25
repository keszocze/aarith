cmake_minimum_required(VERSION 3.13)
project(aarith)

add_executable(sqrt_example sqrt.cpp)
target_link_libraries(sqrt_example aarith::Library)

add_executable(fau_adder_evaluation fau_adder.cpp)
target_link_libraries(fau_adder_evaluation aarith::Library)

