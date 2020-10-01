#pragma once

#define AARITH_FLOAT_TEST_SIGNATURE ((size_t E, size_t M), E, M)
#define AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE                                               \
    ((size_t E, size_t M, typename Native), E, M, Native)

#define AARIHT_FLOAT_TEMPLATE_RANGE (5, 2), (6, 9), (8, 7), (8, 23), (11, 52), (15, 112)
#define AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE (8, 23), (11, 52)
#define AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE (8, 23, float), (11, 52, double)

#define AARITH_INT_TEST_SIGNATURE ((size_t W, typename WordType), W, WordType)
#define AARITH_INT_TEST_TEMPLATE_FULL_PARAM_RANGE                                                  \
    (5, uint8_t), (5, uint16_t), (5, uint32_t), (5, uint64_t), (8, uint8_t), (8, uint16_t),        \
        (8, uint32_t), (8, uint64_t), (16, uint8_t), (16, uint16_t), (16, uint32_t),               \
        (16, uint64_t), (20, uint8_t), (20, uint16_t), (20, uint32_t), (20, uint64_t),             \
        (32, uint8_t), (32, uint16_t), (32, uint32_t), (32, uint64_t), (50, uint8_t),              \
        (50, uint16_t), (50, uint32_t), (50, uint64_t), (64, uint8_t), (64, uint16_t),             \
        (64, uint32_t), (64, uint64_t), (65, uint8_t), (65, uint16_t), (65, uint32_t),             \
        (65, uint64_t), (128, uint8_t), (128, uint16_t), (128, uint32_t), (128, uint64_t),         \
        (150, uint8_t), (150, uint16_t), (150, uint32_t), (150, uint64_t), (1024, uint8_t),        \
        (1024, uint16_t), (1024, uint32_t), (1024, uint64_t), (1025, uint8_t), (1025, uint16_t),   \
        (1025, uint32_t)
#define AARITH_INT_TEST_TEMPLATE_LARGE_PARAM_RANGE                                                 \
    (1024, uint8_t), (1024, uint16_t), (1024, uint32_t), (1024, uint64_t), (1025, uint8_t),        \
        (1025, uint16_t), (1025, uint32_t), (1025, uint64_t), (5000, uint8_t), (5000, uint16_t),   \
        (5000, uint32_t), (5000, uint64_t)

#define AARITH_INT_TEST_TEMPLATE_NATIVE_SIZES_PARAM_RANGE                                          \
    (8, uint64_t), (16, uint64_t), (32, uint64_t), (64, uint64_t)

#define AARITH_FIXED_TEST_SIGNATURE                                                                \
    ((size_t I, size_t F, template <size_t, typename> class BaseInt, typename WordType), I, F, BaseInt,  \
     WordType)
#define AARITH_FIXED_TEST_PARAM_RANGE                                                              \
    (8, 3, integer, uint64_t), (10, 4, integer, uint32_t), (3, 18, integer, uint8_t),              \
        (35, 70, integer, uint64_t), (8, 3, uinteger, uint64_t), (10, 4, uinteger, uint32_t),      \
        (3, 18, uinteger, uint8_t), (35, 70, uinteger, uint64_t)

#define AARITH_INT_EXTENDED_TEST_SIGNATURE                                                         \
    ((template <size_t, typename> class Type, size_t W, typename WordType), Type, W, WordType)
#define AARITH_INT_EXTENDED_TEST_TEMPLATE_FULL_PARAM_RANGE                                         \
    (integer, 5, uint8_t), (integer, 5, uint16_t), (integer, 5, uint32_t), (integer, 5, uint64_t), \
        (integer, 8, uint8_t), (integer, 8, uint16_t), (integer, 8, uint32_t),                     \
        (integer, 8, uint64_t), (integer, 16, uint8_t), (integer, 16, uint16_t),                   \
        (integer, 16, uint32_t), (integer, 16, uint64_t), (integer, 20, uint8_t),                  \
        (integer, 20, uint16_t), (integer, 20, uint32_t), (integer, 20, uint64_t),                 \
        (integer, 32, uint8_t), (integer, 32, uint16_t), (integer, 32, uint32_t),                  \
        (integer, 32, uint64_t), (integer, 50, uint8_t), (integer, 50, uint16_t),                  \
        (integer, 50, uint32_t), (integer, 50, uint64_t), (integer, 64, uint8_t),                  \
        (integer, 64, uint16_t), (integer, 64, uint32_t), (integer, 64, uint64_t),                 \
        (integer, 65, uint8_t), (integer, 65, uint16_t), (integer, 65, uint32_t),                  \
        (integer, 65, uint64_t), (integer, 128, uint8_t), (integer, 128, uint16_t),                \
        (integer, 128, uint32_t), (integer, 128, uint64_t), (integer, 150, uint8_t),               \
        (integer, 150, uint16_t), (integer, 150, uint32_t), (integer, 150, uint64_t),              \
        (integer, 1024, uint8_t), (integer, 1024, uint16_t), (integer, 1024, uint32_t),            \
        (integer, 1024, uint64_t), (integer, 1025, uint8_t), (integer, 1025, uint16_t),            \
        (integer, 1025, uint32_t), (integer, 1025, uint64_t), (uinteger, 5, uint8_t),              \
        (uinteger, 5, uint16_t), (uinteger, 5, uint32_t), (uinteger, 5, uint64_t),                 \
        (uinteger, 8, uint8_t), (uinteger, 8, uint16_t), (uinteger, 8, uint32_t),                  \
        (uinteger, 8, uint64_t), (uinteger, 16, uint8_t), (uinteger, 16, uint16_t),                \
        (uinteger, 16, uint32_t), (uinteger, 16, uint64_t), (uinteger, 20, uint8_t),               \
        (uinteger, 20, uint16_t), (uinteger, 20, uint32_t), (uinteger, 20, uint64_t),              \
        (uinteger, 32, uint8_t), (uinteger, 32, uint16_t), (uinteger, 32, uint32_t),               \
        (uinteger, 32, uint64_t), (uinteger, 50, uint8_t), (uinteger, 50, uint16_t),               \
        (uinteger, 50, uint32_t), (uinteger, 50, uint64_t), (uinteger, 64, uint8_t),               \
        (uinteger, 64, uint16_t), (uinteger, 64, uint32_t), (uinteger, 64, uint64_t),              \
        (uinteger, 65, uint8_t), (uinteger, 65, uint16_t), (uinteger, 65, uint32_t),               \
        (uinteger, 65, uint64_t), (uinteger, 128, uint8_t), (uinteger, 128, uint16_t),             \
        (uinteger, 128, uint32_t), (uinteger, 128, uint64_t), (uinteger, 150, uint8_t),            \
        (uinteger, 150, uint16_t), (uinteger, 150, uint32_t), (uinteger, 150, uint64_t),           \
        (uinteger, 1024, uint8_t), (uinteger, 1024, uint16_t), (uinteger, 1024, uint32_t),         \
        (uinteger, 1024, uint64_t), (uinteger, 1025, uint8_t), (uinteger, 1025, uint16_t),         \
        (uinteger, 1025, uint32_t), (uinteger, 1025, uint64_t)

#define AARITH_INT_EXTENDED_TEST_TEMPLATE_LARGE_PARAM_RANGE                                        \
    (uinteger, 1024, uint8_t), (uinteger, 1024, uint16_t), (uinteger, 1024, uint32_t), uinteger,   \
        (1024, uint64_t), (uinteger, 1025, uint8_t), (uinteger, 1025, uint16_t),                   \
        (uinteger, 1025, uint32_t), (uinteger, 1025, uint64_t), (uinteger, 5000, uint8_t),         \
        (uinteger, 5000, uint16_t), (uinteger, 5000, uint32_t), (uinteger, 5000, uint64_t),        \
        (integer, 1024, uint8_t), (integer, 1024, uint16_t), (integer, 1024, uint32_t),            \
        (integer, 1024, uint64_t), (integer, 1025, uint8_t), (integer, 1025, uint16_t),            \
        (integer, 1025, uint32_t), (integer, 1025, uint64_t), (integer, 5000, uint8_t),            \
        (integer, 5000, uint16_t), (integer, 5000, uint32_t), (integer, 5000, uint64_t)

#define AARITH_INT_EXTENDED_TEST_TEMPLATE_NATIVE_SIZES_PARAM_RANGE                                 \
    (uinteger, 8, uint64_t), (uinteger, 16, uint64_t), (uinteger, 32, uint64_t),                   \
        (uinteger, 64, uint64_t), (integer, 8, uint64_t), (integer, 16, uint64_t),                 \
        (integer, 32, uint64_t), (integer, 64, uint64_t)

#ifdef AARITH_USE_FULL_RANGE
#define AARITH_INT_TEST_TEMPLATE_PARAM_RANGE AARITH_INT_TEST_TEMPLATE_FULL_PARAM_RANGE
#define AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE AARITH_INT_TEST_TEMPLATE_FULL_PARAM_RANGE
#define AARITH_INT_EXTENDED_TEST_TEMPLATE_PARAM_RANGE                                              \
    AARITH_INT_EXTENDED_TEST_TEMPLATE_FULL_PARAM_RANGE
#else
#ifdef AARITH_USE_LARGE_INTS
#define AARITH_INT_TEST_TEMPLATE_PARAM_RANGE AARITH_INT_TEST_TEMPLATE_LARGE_PARAM_RANGE
#define AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE AARITH_INT_TEST_TEMPLATE_LARGE_PARAM_RANGE
#define AARITH_INT_EXTENDED_TEST_TEMPLATE_PARAM_RANGE                                              \
    AARITH_INT_EXTENDED_TEST_TEMPLATE_FULL_PARAM_RANGE
#else
#define AARITH_INT_TEST_TEMPLATE_PARAM_RANGE AARITH_INT_TEST_TEMPLATE_NATIVE_SIZES_PARAM_RANGE
#define AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE                                                \
    AARITH_INT_TEST_TEMPLATE_NATIVE_SIZES_PARAM_RANGE
#define AARITH_INT_EXTENDED_TEST_TEMPLATE_PARAM_RANGE                                              \
    AARITH_INT_EXTENDED_TEST_TEMPLATE_FULL_PARAM_RANGE
#endif
#endif

#define AARITH_NATIVE_BITWIDTHS_SMALL 8, 16
#define AARITH_NATIVE_BITWIDTHS 8, 16, 32, 64
#define AARITH_NATIVE_TYPES_SMALL uint8_t, int8_t, uint16_t, int16_t
#define AARITH_NATIVE_TYPES uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t
