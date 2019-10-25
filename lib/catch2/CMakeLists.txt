add_library(catch2-main
  catch.hpp
  catch-main.cpp
)
target_include_directories(catch2-main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_all_warnings(catch2-main INTERFACE)
target_warnings_are_errors(catch2-main INTERFACE)
target_compile_features(catch2-main PRIVATE cxx_std_17)
add_library(Catch2::WithMain ALIAS catch2-main)
