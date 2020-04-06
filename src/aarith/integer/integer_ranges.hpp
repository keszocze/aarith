#pragma once

#include <aarith/integer/integers.hpp>

namespace aarith {

template <typename Integer> class integer_range
{
    Integer start_;
    Integer end_;

    template <typename I> class integer_iter
    {

        std::optional<I> current;
        std::optional<I> end;

    public:
        using value_type = I;
        using refernce = I;
        using iterator_category = std::input_iterator_tag;
        using pointer = I*;
        using difference_type = void;

        // TODO make the default thingie static;

        integer_iter<I>()
            : current(std::nullopt)
            , end(std::nullopt)
        {
        }

        integer_iter<I>(const I start, const I end)
            : current(start)
            , end(end)
        {
        }

        I operator*() const
        {
            return *current;
        }
        I* operator->() const
        {
            return &(*current);
        }

        integer_iter& operator++()
        { // preincrement
            if (current == end)
            {
                current = std::nullopt;
            }
            else if (current)
            {
                current = add(*current, I::one());
            }

            //        std::cout << current << "\n";
            return *this;
        }

        friend bool operator==(integer_iter<I> const& lhs, integer_iter<I> const& rhs)
        {
            return lhs.current == rhs.current;
        }
        friend bool operator!=(integer_iter<I> const& lhs, integer_iter<I> const& rhs)
        {
            return !(lhs == rhs);
        }
    };

public:
    integer_range(const Integer start, const Integer end)
        : start_(start)
        , end_(end)
    {
        static_assert(is_integral_v<Integer>);
        if (end < start)
        {
            throw std::invalid_argument("The end of the range must not be smaller than the start");
        }
    }

    [[nodiscard]] integer_iter<Integer> begin() const
    {
        return integer_iter<Integer>(start_, end_);
    }

    [[nodiscard]] integer_iter<Integer> end() const
    {
        return integer_iter<Integer>();
    }
};

} // namespace aarith