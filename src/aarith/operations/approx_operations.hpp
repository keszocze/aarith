#pragma once

#include <cstdint>
#include <iostream>

namespace aarith {

template <class UInteger> 
auto generate_bitmask(const uint32_t bits) 
-> UInteger
{
    UInteger mask;

    constexpr typename UInteger::word_type full_mask = static_cast<typename UInteger::word_type>(-1);
    const auto full_mask_words = bits/static_cast<uint32_t>(UInteger::word_width());
    const auto remaining_bits = bits - full_mask_words * static_cast<uint32_t>(UInteger::word_width());

    const typename UInteger::word_type last_word_mask = (static_cast<typename UInteger::word_type>(1) << bits) - 1;

    auto counter = 0U;
    while(counter < full_mask_words){
        mask.set_word(counter, full_mask);
        ++counter;
    }
    
    if(remaining_bits > 0)
    {
        mask.set_word(counter, last_word_mask);
    }

    return mask;
}

template <class UInteger> 
auto approx_uint_bitmasking_add(const UInteger& opd1, const UInteger& opd2, const uint32_t bits) 
-> UInteger
{
    UInteger sum, mask;

    mask = generate_bitmask<UInteger>(UInteger::width()-bits);
    mask = ~mask;

    UInteger opd1_masked = opd1 & mask;
    UInteger opd2_masked = opd2 & mask;
    
    sum = exact_uint_add(opd1_masked, opd2_masked);

    return sum;
}


} // namespace aarith
