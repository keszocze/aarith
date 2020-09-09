add_library(aarith INTERFACE)
target_sources(aarith INTERFACE
        aarith/core.hpp
        aarith/integer.hpp
        aarith/fixed_point.hpp
        aarith/float.hpp
        aarith/float/approx_operations.hpp
        aarith/operations/approx_operations.hpp
)
target_include_directories(aarith INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(aarith INTERFACE cxx_std_17)
add_library(aarith::Library ALIAS aarith)
