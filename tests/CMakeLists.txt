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




add_aarith_test(uint FILES integer/uint-test.cpp)
add_aarith_test(uint-exact-operations FILES integer/uint-operations-test.cpp)
add_aarith_test(float-exact-operations-float FILES float/exact_operations-float-test.cpp)
add_aarith_test(uint-comparisons FILES integer/uint-comparisons-test.cpp)
add_aarith_test(string_utils FILES integer/string_utils-test.cpp)
add_aarith_test(uint-anytime FILES integer/uint-anytime-test.cpp)
add_aarith_test(util FILES integer/string_utils-test.cpp)
add_aarith_test(bit-operations FILES core/bit_operations-test.cpp)
add_aarith_test(integer FILES integer/integer-test.cpp integer/integer-operations-test.cpp integer/integer-comparisons-test.cpp)
add_aarith_test(container FILES core/functional-style-test.cpp)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/")
find_package(MPIR)
if(MPIR_FOUND)
  add_aarith_test(uint-random FILES integer/uint-random-test.cpp LIBS ${MPIR_LIBRARIES} INCLUDES ${MPIR_INCLUDE_DIR})
else()
  message(WARNING "Could not find MPIR; not building coverage test")
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
  target_compile_options(uint-test PRIVATE --coverage)
  target_link_options(uint-test PRIVATE --coverage)
endif()
