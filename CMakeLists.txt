cmake_minimum_required(VERSION 3.13)
project(aarith)

include(TargetWarnings.cmake)
enable_testing()

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(tests)

#add_executable(aarith main.cpp)
#target_link_libraries(aarith aarith::Types)