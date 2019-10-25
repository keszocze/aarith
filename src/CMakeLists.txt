add_library(aarith INTERFACE)
target_sources(aarith INTERFACE
  types/integer.hpp
  operations/exact_operations.hpp
)
target_include_directories(aarith INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(aarith INTERFACE cxx_std_17)
add_library(aarith::Library ALIAS aarith)