#pragma once

#include <aarith/integer/integers.hpp>

namespace aarith {

template <typename Integer> class integer_range
{
    Integer start_;
    Integer end_;
    Integer stride_;

    template <typename I> class integer_iter
    {

        std::optional<I> current;
        const integer_range<I> range;

    public:
        using value_type = I;
        using reference = I;
        using iterator_category = std::input_iterator_tag;
        using pointer = I*;
        using difference_type = void;

        integer_iter<I>(const I current, const integer_range<I> range)
            : current(current)
            , range(range)
        {
        }

        integer_iter<I>(const integer_range<I> range)
            : current(std::nullopt)
            , range(range)
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
            if (current == range.end_)
            {
                current = std::nullopt;
            }
            else if (current)
            {
                current = add(*current, range.stride_);
            }

            return *this;
        }

        friend bool operator==(integer_iter<I> const& lhs, integer_iter<I> const& rhs)
        {
            return (lhs.current == rhs.current) && (lhs.range == rhs.range);
        }
        friend bool operator!=(integer_iter<I> const& lhs, integer_iter<I> const& rhs)
        {
            return !(lhs == rhs);
        }
    };

public:
    integer_range(const Integer start, const Integer end, const Integer stride = Integer::one())
        : start_(start)
        , end_(end)
        , stride_(stride)
    {
        static_assert(is_integral_v<Integer>);

        if (stride_ <= Integer::zero())
        {
            throw std::invalid_argument("Stride must be positive");
        }
    }

    [[nodiscard]] integer_iter<Integer> begin() const
    {
        return cbegin();
    }

    [[nodiscard]] integer_iter<Integer> end() const
    {
        return cend();
    }

    [[nodiscard]] integer_iter<Integer> cbegin() const
    {
        return integer_iter<Integer>(start_, *this);
    }

    [[nodiscard]] integer_iter<Integer> cend() const
    {
        return integer_iter<Integer>(*this);
    }

    friend bool operator==(integer_range<Integer> const& lhs, integer_range<Integer> const& rhs)
    {
        return (lhs.start_ == rhs.start_) && (lhs.end_ == rhs.end_) && (lhs.stride_ == rhs.stride_);
    }

    friend bool operator!=(integer_range<Integer> const& lhs, integer_range<Integer> const& rhs)
    {
        return !(lhs == rhs);
    }
};

} // namespace aarith