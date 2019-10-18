cmake_minimum_required(VERSION 3.14)
project(aarith)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/types)
add_subdirectory(lib)
add_subdirectory(tests)

add_executable(aarith main.cpp)
target_link_libraries(aarith types)