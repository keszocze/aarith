add_executable(uint-test
  uint-test.cpp
  exact_operations-test.cpp
  comparisons-test.cpp
  string_utils-test.cpp
)
target_link_libraries(uint-test Catch2::WithMain aarith::Library)
add_test(NAME uint-test COMMAND uint-test)

add_executable(uint-anytime-test uint-anytime-test.cpp)
target_link_libraries(uint-anytime-test Catch2::WithMain aarith::Library)
add_test(NAME uint-anytime-test COMMAND uint-anytime-test)

add_executable(bit-operations-test bit_operations-test.cpp)
target_link_libraries(bit-operations-test Catch2::WithMain aarith::Library)
add_test(NAME bit-operations-test COMMAND bit-operations-test)


add_executable(sint-test sint-test.cpp sint-operations-test.cpp sint-comparisons-test.cpp)
target_link_libraries(sint-test Catch2::WithMain aarith::Library)
add_test(NAME sint-test COMMAND sint-test)

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
