add_library(aarith INTERFACE)
target_sources(aarith INTERFACE
        aarith/integer/uinteger.hpp
        aarith/integer/sinteger.hpp
        aarith/core/word_container.hpp
        aarith/float/normfloat.hpp
        aarith/core/traits.hpp
        aarith/integer/random.hpp
        aarith/integer/uinteger_operations.hpp
        aarith/float/float_operations.hpp
        aarith/float/float_comparisons.hpp
        aarith/float/approx_operations.hpp
        aarith/operations/approx_operations.hpp
        aarith/integer/sinteger_operations.hpp
        aarith/integer/uinteger_comparisons.hpp
)

target_include_directories(aarith INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(aarith INTERFACE cxx_std_17)
add_library(aarith::Library ALIAS aarith)

