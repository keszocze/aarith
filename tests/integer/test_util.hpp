#pragma once

#define AARITH_INT_TEST_SIGNATURE ((size_t W, typename WordType), W, WordType)
#define AARITH_INT_TEST_TEMPLATE_PARAM_RANGE                                                       \
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

