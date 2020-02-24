function(add_aarith_test testname filename)
  SET(targetname "${testname}-test")
  add_executable(${targetname} ${filename})
  target_link_libraries(${targetname} Catch2::WithMain aarith::Library)
  add_test(NAME ${targetname} COMMAND ${targetname})
endfunction(add_aarith_test)

add_aarith_test(uint uint-test.cpp)
add_aarith_test(exact-operations exact_operations-test.cpp)
add_aarith_test(exact-operations-float-test exact_operations-float-test.cpp)





add_aarith_test(comparisons-test comparisons-test.cpp)

add_aarith_test(string_utils-test string_utils-test.cpp)

add_aarith_test(uint-anytime-test uint-anytime-test.cpp)


add_aarith_test(util-test string_utils-test.cpp)


add_aarith_test(bit-operations-test bit_operations-test.cpp)


add_aarith_test(sint-test sint-test.cpp sint-operations-test.cpp sint-comparisons-test.cpp)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/")
find_package(MPIR)
if(MPIR_FOUND)
  add_executable(uint-random-test uint-random-test.cpp)
  target_link_libraries(uint-random-test PRIVATE aarith::Library Catch2::WithMain ${MPIR_LIBRARIES})
  target_include_directories(uint-random-test PRIVATE ${MPIR_INCLUDE_DIR})
  add_test(NAME uint-random-test COMMAND uint-random-test)
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
