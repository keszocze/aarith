add_library(aarith INTERFACE)
target_sources(aarith INTERFACE
        aarith/types/uinteger.hpp
        aarith/types/sinteger.hpp
  aarith/types/word_container.hpp
  aarith/types/traits.hpp
  aarith/operations/exact_operations.hpp
  aarith/operations/approx_operations.hpp
        aarith/operations/sinteger_operations.hpp
  aarith/operations/comparisons.hpp
  aarith/utilities/bit_operations.hpp
  aarith/utilities/string_utils.hpp

)
target_include_directories(aarith INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(aarith INTERFACE cxx_std_17)
add_library(aarith::Library ALIAS aarith)
