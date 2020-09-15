function(add_aarith_test testname)

  SET(targetname "${testname}-test")


  set (multiValueArgs FILES)
  cmake_parse_arguments(
          AARITH_TEST # prefix
          "" # no flags
          "" # no single value options
          "FILES;LIBS;INCLUDES" # multi value options
          ${ARGN}
  )

  add_executable(${targetname} ${AARITH_TEST_FILES})
  target_link_libraries(${targetname} PRIVATE Catch2::WithMain aarith::Library ${AARITH_TEST_LIBS})
  add_test(NAME ${targetname} COMMAND ${targetname})

  if (DEFINED AARITH_TEST_INCLUDES)
    target_include_directories(${targetname} PRIVATE ${AARITH_TEST_INCLUDES})
  endif()

endfunction(add_aarith_test)




add_aarith_test(uint-general FILES integer/uint-test.cpp)
add_aarith_test(uint-operations FILES integer/uint-operations-test.cpp)
add_aarith_test(uint-anytime FILES integer/uint-anytime-test.cpp)
add_aarith_test(uint-comparisons FILES integer/uint-comparisons-test.cpp)
add_aarith_test(float-operations FILES float/exact_operations-float-test.cpp)
add_aarith_test(string_utils FILES integer/string_utils-test.cpp)
add_aarith_test(util FILES integer/string_utils-test.cpp)
add_aarith_test(bit-operations FILES core/bit_operations-test.cpp)
add_aarith_test(integer FILES integer/integer-test.cpp integer/integer-operations-test.cpp integer/integer-comparisons-test.cpp)
add_aarith_test(integer-ranges FILES integer/ranges_test.cpp)
add_aarith_test(functional-style FILES core/functional-style-test.cpp)
add_aarith_test(number-util FILES core/number_utils-test.cpp)
add_aarith_test(integer-cast FILES integer/integer-casts.cpp)

add_aarith_test(fau FILES uint-approx-test.cpp)



if(MPIR_FOUND)
  add_aarith_test(uint-random FILES integer/uint-random-test.cpp LIBS ${MPIR_LIBRARIES} INCLUDES ${MPIR_INCLUDE_DIR})
  message(STATUS "MPIR found: Building MPIR tests")
else()
  message(STATUS "Could not find MPIR: not building MPIR tests")
endif()


## coverage

set(BUILD_COVERAGE_DATA TRUE)
if(CMAKE_COMPILER_IS_GNUCXX)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
      set(BUILD_COVERAGE_DATA FALSE)
    endif()
  endif()
else()
  set(BUILD_COVERAGE_DATA FALSE)
endif()

if(BUILD_COVERAGE_DATA)
  target_compile_options(uint-general-test PRIVATE --coverage)
  target_link_options(uint-general-test PRIVATE --coverage)
endif()

## integration tests
