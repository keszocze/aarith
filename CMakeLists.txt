cmake_minimum_required(VERSION 3.13)
project(aarith)

include(TargetWarnings.cmake)
enable_testing()

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(tests)

add_executable(aarith_main main.cpp)
target_link_libraries(aarith_main aarith::Library)





option(BUILD_EXPERIMENTS "build experiments" ON)
if(BUILD_EXPERIMENTS)
    add_subdirectory(experiments)
endif()