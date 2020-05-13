cmake_minimum_required(VERSION 3.13)
project(aarith)
option(BUILD_BENCHMARKS "build benchmarks" OFF)
option(BUILD_EXPERIMENTS "build experiments" OFF)



include(TargetWarnings.cmake)
enable_testing()

add_subdirectory(src)
add_subdirectory(lib/catch2)
add_subdirectory(tests)

add_executable(aarith_main main.cpp)
target_link_libraries(aarith_main aarith::Library)


file(GLOB_RECURSE ALL_SOURCE_FILES
        src/*.hpp
        tests/*.hpp
        tests/*.cpp
        benchmarks/*.hpp
        benchmarks/*.cpp
        experiments/*.hpp
        experiments/*.cpp)

add_custom_target(clangformat
#        COMMAND echo ${ALL_SOURCE_FILES}
        COMMAND clang-format -style=file -i ${ALL_SOURCE_FILES}
        )


if(BUILD_BENCHMARKS)
    add_subdirectory(lib/benchmark)
    add_subdirectory(benchmarks)
endif()

if(BUILD_EXPERIMENTS)
    add_subdirectory(experiments)
endif()
