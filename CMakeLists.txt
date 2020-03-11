cmake_minimum_required(VERSION 3.13)
project(aarith)

include(TargetWarnings.cmake)
enable_testing()

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(tests)

add_executable(aarith_main main.cpp)
target_link_libraries(aarith_main aarith::Library)


file(GLOB_RECURSE ALL_SOURCE_FILES src/*.hpp tests/*.hpp tests/*.cpp)

add_custom_target(clangformat
        COMMAND clang-format
        -style=file
        -i
        ${ALL_SOURCE_FILES} EXCLUDE_FROM_ALL)



