//
// Created by keszocze on 18.10.19.
//

#pragma once

#include <cstdint>
#include <type_traits>
#include <array>

template <class T>
constexpr size_t size_in__words(const size_t w) {
    const u_int word_size = sizeof(T)*8;
    return (w / word_size)  + (w % word_size ? 1  : 0);
}


template <size_t Width> class integer {
private:

    static_assert(Width > 0, "Need at least 1 bit");

    static constexpr size_t  array_size = size_in__words<uint64_t>(Width);

    std::array<uint64_t, array_size> value;

public:

    template <class T>
    integer(T n) {

        static_assert(std::is_integral<T>::value, "Only integral values are supported");
        static_assert(sizeof(T)*8<=64,"Only up to 64 bit integers are supported");
        static_assert(sizeof(T)*8 <= Width, "Data type can not fit provided number");

        value[0]=n;

    }

};


