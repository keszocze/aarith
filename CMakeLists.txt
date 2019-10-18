cmake_minimum_required(VERSION 3.13.4)
project(aarith)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(tests)


add_compile_options(-W -Wall -Wextra -Werror)

add_executable(aarith main.cpp)
target_link_libraries(aarith types)