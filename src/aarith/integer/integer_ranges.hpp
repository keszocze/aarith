#pragma once

#include <aarith/integer/integers.hpp>

namespace aarith {

// template <typename Integer> class integer_stride
//{
//
//    Integer start_;
//    Integer end_;
//    Integer increment;
//
//    template <typename Int> class integer_iter
//    {
//
//        std::optional<Int> current;
//        std::optional<Int> end;
//        Int increment;
//
//    public:
//        using value_type = Int;
//        using reference = Int;
//        using iterator_category = std::input_iterator_tag;
//        using pointer = Int*;
//        using difference_type = void;
//
//        integer_iter<Integer>()
//            : current(std::nullopt)
//            , end(std::nullopt)
//            , increment(Int::zero())
//        {
//        }
//
//        integer_iter<Integer>(const Int start, const Int end, const Int increment)
//            : current(start)
//            , end(end)
//            , increment(increment)
//        {
//        }
//
//        Int operator*() const
//        {
//            return *current;
//        }
//        Int* operator->() const
//        {
//            return &(*current);
//        }
//
//        integer_iter& operator++()
//        { // preincrement
//            if (current == end)
//            {
//                current = std::nullopt;
//            }
//            else if (current)
//            {
//                const Int val = *current;
//                const Int end_val = *end;
//                const Int dist = distance(val, end_val);
//
//                std::cout << "val: " << val << "\tend_val: " << end_val << "\tdist: " << dist
//                          << "\n";
//
//                if (dist >= increment)
//                {
//                    current = add(val, increment);
//                }
//                else
//                {
//                    current = std::nullopt;
//                }
//            }
//
//            return *this;
//        }
//
//        friend bool operator==(integer_iter<Int> const& lhs, integer_iter<Int> const& rhs)
//        {
//            return lhs.current == rhs.current;
//        }
//        friend bool operator!=(integer_iter<Int> const& lhs, integer_iter<Int> const& rhs)
//        {
//            return !(lhs == rhs);
//        }
//    };
//
// public:
//    integer_stride(const Integer& start, const Integer& end,
//                   const Integer& increment = Integer::one())
//        : start_(start)
//        , end_(end)
//        , increment(end < start ? -increment : increment)
//    {
//        if (increment <= Integer::zero())
//        {
//            throw std::invalid_argument("Using a negative or zero increment is not supported");
//        }
//    }
//
//    [[nodiscard]] integer_iter<Integer> begin() const
//    {
//        return integer_iter<Integer>(start_, end_, increment);
//    }
//
//    [[nodiscard]] integer_iter<Integer> end() const
//    {
//        return integer_iter<Integer>();
//    }
//
//
//};

template <typename Integer> class integer_range
{
    Integer start_;
    Integer end_;
    Integer stride_;

    template <typename I> class integer_iter
    {

        std::optional<I> current;
        const I start;
        const I end;
        const I stride;

    public:
        using value_type = I;
        using reference = I;
        using iterator_category = std::input_iterator_tag;
        using pointer = I*;
        using difference_type = void;

        integer_iter<I>(const I current, const I start, const I end, const I stride)
            : current(current)
            , start(start)
            , end(end)
            , stride(stride)
        {
        }

        integer_iter<I>(const I start, const I end, const I stride)
            : current(std::nullopt)
            , start(start)
            , end(end)
            , stride(stride)
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
                current = add(*current, stride);
            }

            return *this;
        }

        friend bool operator==(integer_iter<I> const& lhs, integer_iter<I> const& rhs)
        {
            return (lhs.current ==
                    rhs.current); // && (lhs.end == rhs.end) && (lhs.stride == rhs.stride);
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
        return integer_iter<Integer>(start_, start_, end_, stride_);
    }

    [[nodiscard]] integer_iter<Integer> cend() const
    {
        return integer_iter<Integer>(start_, end_, stride_);
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