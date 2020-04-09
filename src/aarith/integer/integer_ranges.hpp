#pragma once

#include <optional>

#include <aarith/integer/integers.hpp>

namespace aarith {

template <typename Integer> class integer_range
{
    const Integer start_;
    const Integer end_;
    const Integer stride_;

    template <typename I, bool IsForwardIterator = true> class integer_iter
    {

        std::optional<I> current;
        const integer_range<I> range;

    public:
        using value_type = I;
        using reference = I;
        using iterator_category = std::input_iterator_tag;
        using pointer = I*;
        using difference_type = void;

        integer_iter<I,IsForwardIterator>(const I current, const integer_range<I> range)
            : current(current)
            , range(range)
        {
        }

        integer_iter<I, IsForwardIterator>(const integer_range<I> range)
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
                const I curr = *current;
                const I stride = range.stride_;
                const I dist = sub(range.end_, curr);

                if (dist >= stride)
                {
                    current = add(*current, range.stride_);
                }
                else
                {
                    current = std::nullopt;
                }
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
        if (start_ > end_)
        {
            return cend();
        }
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