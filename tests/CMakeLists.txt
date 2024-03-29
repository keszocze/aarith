set(CMAKE_CXX_CLANG_TIDY "")


add_aarith_test(core-functional-style FILES core/functional-style-test.cpp)
add_aarith_test(core-number-util FILES core/number_utils-test.cpp)
add_aarith_test(word_array-random-generation FILES core/word_array-generation-test.cpp)
add_aarith_test(word_array-bit-operations FILES core/bit_operations-test.cpp)
add_aarith_test(word_array-extraction FILES core/word_array-extraction-test.cpp)
add_aarith_test(word_array-utility FILES core/word_array-utility-test.cpp)
add_aarith_test(word_array-construction FILES core/word_array_constructor.cpp)


add_aarith_test(uint-general FILES integer/uint-test.cpp)
add_aarith_test(uint-random-generation FILES integer/uint-random-generation-test.cpp)
add_aarith_test(uint-operations FILES integer/uint-operations-test.cpp)
add_aarith_test(uint-anytime FILES integer/uint-anytime-test.cpp)
add_aarith_test(uint-comparisons FILES integer/uint-comparisons-test.cpp)
add_aarith_test(uint-extraction FILES integer/uint-extraction-test.cpp)

add_aarith_test(string_utils FILES integer/string_utils-test.cpp)
add_aarith_test(integer-general FILES integer/integer-test.cpp)
add_aarith_test(integer-operations FILES integer/integer-operations-test.cpp)
add_aarith_test(integer-shift-operations FILES integer/integer-shift-operations-test.cpp)
add_aarith_test(integer-comparisons FILES integer/integer-comparisons-test.cpp)
add_aarith_test(integer-ranges FILES integer/ranges_test.cpp)
add_aarith_test(integer-random-generation FILES integer/integer-random-generation-test.cpp)
add_aarith_test(integer-cast FILES integer/integer-casts.cpp)

add_aarith_test(float-anytime-operations FILES float/anytime_operations-float-test.cpp)
add_aarith_test(float FILES float/float-test.cpp  float/float_general_operations.cpp)
add_aarith_test(float-casts FILES float/float_casts.cpp)
add_aarith_test(float-string_utils FILES float/integer_string_utils.cpp)
add_aarith_test(float-comparison FILES float/float_comparisons.cpp)
add_aarith_test(float-total_ordering FILES float/total_ordering.cpp)
add_aarith_test(float-sign_bit_operations FILES float/sign_bit_operations.cpp)
add_aarith_test(float-NaN-payloads FILES float/nan_payload.cpp)
add_aarith_test(float-random-generation FILES float/float-random-generation-test.cpp)
add_aarith_test(float-division FILES float/float_division.cpp)
add_aarith_test(float-multiplication FILES float/float_mul.cpp)
add_aarith_test(float-addition FILES float/float_addition.cpp)
add_aarith_test(float-subtraction FILES float/float_subtraction.cpp)
add_aarith_test(float-classify-methods FILES float/classify-methods.cpp)
add_aarith_test(float-numeric_limits FILES float/float_numeric_limits.cpp)

add_aarith_test(fau-adder FILES uint-approx-test.cpp)



if(MPIR_FOUND)
  add_aarith_test(uint-check-against-mpir FILES integer/uint-arithmetic-mpir-test.cpp LIBS ${MPIR_LIBRARIES} INCLUDES ${MPIR_INCLUDE_DIR})
  message(STATUS "MPIR found: Building MPIR tests")
else()
  message(STATUS "Could not find MPIR: not building MPIR tests")
endif()


if (USE_FULL_RANGE_IN_TESTS)
  add_compile_definitions(AARITH_USE_FULL_RANGE)
endif()


## coverage

set(BUILD_COVERAGE_DATA TRUE)
if(CMAKE_COMPILER_IS_GNUCXX)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
      set(BUILD_COVERAGE_DATA FALSE)
    endif()
  endif()
else()
  set(BUILD_COVERAGE_DATA FALSE)
endif()

if(BUILD_COVERAGE_DATA)
  target_compile_options(uint-general-test PRIVATE --coverage)
  target_link_options(uint-general-test PRIVATE --coverage)
endif()

## integration tests
