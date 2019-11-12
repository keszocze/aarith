add_executable(uint-test
  uint-test.cpp
  exact_operations-test.cpp
  comparisons-test.cpp
)
target_link_libraries(uint-test Catch2::WithMain aarith::Library)
add_test(NAME uint-test COMMAND uint-test)

if(CMAKE_COMPILER_IS_GNUCXX)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
        message(FATAL_ERROR "Clang version must be 3.0.0 or greater; not building coverage reports")
    endif()
  endif()

  target_compile_options(uint-test PRIVATE --coverage)
  target_link_options(uint-test PRIVATE --coverage)

  add_custom_command(TARGET uint-test
    COMMAND ./uint-test > /dev/null
    COMMAND mkdir -p coverage/uint-test
    # Giving the following path manually is not a clean solution, but CMake does not give
    # us the path to the intermediate build directory as a variable.
    COMMAND cd coverage/uint-test && gcov ../../CMakeFiles/uint-test.dir/uint-test.cpp.gcno > gcov.log
  )
else()
  message(FATAL_ERROR "Compiler is not GNU gcc; not building coverage reports")
endif()

add_executable(uint-anytime-test uint-anytime-test.cpp)
target_link_libraries(uint-anytime-test Catch2::WithMain aarith::Library)
add_test(NAME uint-anytime-test COMMAND uint-anytime-test)
