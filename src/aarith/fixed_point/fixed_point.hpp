#pragma once

namespace aarith {
template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
class fixed
{

public:
    using int_type = B<I + F, WordType>;

    static_assert(::aarith::is_integral_v<int_type>);
    static_assert(I + F > 0, "At least one bit is needed to construct a fixed point number");

private:
    /*
     * We store the value of the fixed point in a member variable instead of directly inheriting
     * from the aarith integer types as per the Liskov substition principle, fixed points could
     * otherwise be used in situations an integer was required.
     */
    int_type data;

public:
    /**
     * @brief Creates a fixed point number with value zero
     */
    fixed() = default;

    [[nodiscard]] static constexpr size_t width()
    {
        return I + F;
    }

    [[nodiscard]] static constexpr size_t int_width()
    {
        return I;
    }

    [[nodiscard]] static constexpr size_t frac_width()
    {
        return F;
    }

    /**
     * @brief Creates a fixed point number from a standard data type integral number
     * @tparam Integer The integer type used for creation of the fixed point number
     * @param i The number to be stored in the fixed point number
     */
    template <typename Integer> explicit fixed(const Integer i)
    {

        if constexpr (std::is_integral_v<Integer>)
        {
            static_assert(sizeof(Integer) * 8 <= I);

            data = int_type{i};
            data = data << F;
        }
        else if constexpr (::aarith::is_integral_v<Integer>)
        {
            static_assert(int_width() >= i.width());
            static_assert(width() >= i.width());
            data = i;
            data = data << F;
        }
    }

    template <size_t W>[[nodiscard]] static fixed from_bitstring(const word_array<W>& w)
    {
        static_assert(width() >= W);
        fixed result;
        result.data = w;
        return result;
    }

    /**
     * @brief Returns the underlying aarith integer that stores the bits of the fixed point number.
     *
     * Note that basically is a cast that drops the information where the decimal point is within
     * the bits.
     *
     * @return Aarith integer storing the data of this fixed point number.
     */
    [[nodiscard]] constexpr int_type bits() const
    {
        return data;
    }

    /**
     * @brief Extracts the integer from the fixed point number
     *
     * @return Integer part of the fixed point number represented as an aarith integer
     */
    [[nodiscard]] constexpr B<I, WordType> integer_part() const
    {
        return bit_range<(I + F) - 1, F>(data);
    }

    /**
     * @brief Extracts the fractional from the fixed point number
     *
     * @note This method returns an unsigned aarith integer as it makes no sense to keep the sign
     * for a part of the bitstring that does not store sign information anyway
     *
     * @return Fractional part of the fixed point number represented as an aarith integer
     */
    [[nodiscard]] constexpr uinteger<F, WordType> fractional_part() const
    {
        return bit_range<F - 1, 0>(data);
    }
};

// template <size_t TargetI, size_t TargetF, size_t I, size_t F, template <size_t, class> B,
//          typename WordType = uint46_t>
// width_cast(fixed<I, F, B, WordType> a)->fixed<TargetI, TargetF, B, WordType>
//{
//}
//
// template <size_t I1, size_t F1, size_t I2, size_t F2, template <size_t, class> B,
//          typename WordType = uint64_t>
// expanding_add(fixed<I1, F1, B, WordType> a, fixed<I2, F2, B, WordType> b)
//    ->fixed<std::max(I1, I2) + 1, std::max(F1, F2), B, WordType>
//{
//    // TODO größen angleichen
//    auto tmp_result = add(a_expanded, b_expanded);
//    // TODO reinterpret Funktion bauen (oder doch per Konstruktor? mittels "bool raw=false" extra
//    // param?)
//    return reinterpret<std::max(I1, I2) + 1, std::max(F1, F2)>(tmp_result);
//}

template <size_t I, size_t F, typename WordType = uint64_t>
using fixed_point = fixed<I, F, integer, WordType>;

template <size_t I, size_t F, typename WordType = uint64_t>
using ufixed_point = fixed<I, F, uinteger, WordType>;

template <size_t I, size_t F, template <size_t, class> typename B, typename WordType>
class is_fixed_point<fixed<I, F, B, WordType>>
{
public:
    static constexpr bool value = true;
};

} // namespace aarith