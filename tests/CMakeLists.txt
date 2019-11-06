add_executable(uint-test
  uint-test.cpp
  exact_operations-test.cpp
  comparisons-test.cpp
)
target_link_libraries(uint-test Catch2::WithMain aarith::Library)
add_test(NAME uint-test COMMAND uint-test)