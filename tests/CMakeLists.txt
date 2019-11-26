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
