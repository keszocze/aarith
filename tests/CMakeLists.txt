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
