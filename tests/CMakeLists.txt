add_executable(uint-test uint-test.cpp)
target_link_libraries(uint-test Catch2::WithMain aarith::Types)
add_test(NAME uint-test COMMAND uint-test)