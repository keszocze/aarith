add_library(catch-main catch-main.cpp)
target_link_libraries(catch-main catch2)

add_executable(uint-test uint-test.cpp)
target_link_libraries(uint-test catch2 catch-main aarith::Types)
target_all_warnings(uint-test)
target_warnings_are_errors(uint-test)
add_test(NAME uint-test COMMAND uint-test)