add_library(catch-main catch-main.cpp)
target_link_libraries(catch-main catch2)

add_executable(uint-test uint-test.cpp)
target_link_libraries(uint-test catch2 catch-main types)