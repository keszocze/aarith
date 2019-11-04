#pragma once

#include <cstdint>

namespace aarith {

template <class UInteger> 
auto approx_uint_bitmasking_add(const UInteger& opd1, const UInteger& opd2, const uint32_t bits) 
-> UInteger
{
    UInteger sum, mask;

    typename UInteger::word_type max_word = 0;
    for(auto counter = 0U; counter < sizeof(typename UInteger::word_type); ++counter){
        max_word <<= 8;
        max_word |= 255;
    }
    
    const auto full_mask_words = bits/static_cast<uint32_t>(UInteger::word_width());
    
    auto counter = 0U;
    for(; counter < full_mask_words; ++counter){
      mask.set_word(counter, max_word);
    }
    
    const auto remaining_bits = bits - full_mask_words * static_cast<uint32_t>(UInteger::word_width());

    mask = mask << remaining_bits;

    UInteger opd1_masked = opd1 & mask;
    UInteger opd2_masked = opd2 & mask;
    
    sum = exact_uint_add(opd1_masked, opd2_masked);

    return sum;
}


} // namespace aarith
