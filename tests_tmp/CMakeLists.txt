cmake_minimum_required(VERSION 3.13)
project(aarith)

add_executable(normfloat_test normfloat.cpp)
target_compile_options(normfloat_test PUBLIC -g) 
target_link_libraries(normfloat_test aarith::Library) 
