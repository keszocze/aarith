add_library(aarith INTERFACE)
target_sources(aarith INTERFACE
  aarith/types/integer.hpp
  aarith/types/traits.hpp
  aarith/operations/exact_operations.hpp
  aarith/utilities/bit_operations.hpp
)
target_include_directories(aarith INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(aarith INTERFACE cxx_std_17)
add_library(aarith::Library ALIAS aarith)