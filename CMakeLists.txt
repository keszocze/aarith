cmake_minimum_required(VERSION 3.13)
project(aarith)
option(BUILD_BENCHMARKS "build benchmarks" OFF)
option(BUILD_EXPERIMENTS "build experiments" OFF)
option(BUILD_CORRECTNESS_EXPERIMENTS "build correctness experiments" OFF)
option(BUILD_TESTS "build tests" ON)
option(BUILD_EXAMPLES "build examples" ON)
option(BUILD_DOCUMENTATION "Build documentation" OFF)
option(USE_CLANGTIDY "Use clang-tidy" ON)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release")
endif()

if (USE_CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib")
find_package(MPIR)
find_package(MPFR)

include(cmake/TargetWarnings.cmake)
include(cmake/Tests.cmake)
include(cmake/Experiments.cmake)
include(cmake/Benchmarks.cmake)
enable_testing()

add_subdirectory(src)

if (BUILD_TESTS)
    add_subdirectory(lib/catch2)
    add_subdirectory(tests)
    option(USE_FULL_RANGE_IN_TESTS "test large set of (u)integer types" OFF)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

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

if(BUILD_CORRECTNESS_EXPERIMENTS)
    add_subdirectory(tests/correctness)
endif()

if (BUILD_DOCUMENTATION)
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    add_subdirectory(docs)
endif()
