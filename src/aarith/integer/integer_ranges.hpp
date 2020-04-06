#pragma once

#include <aarith/integer/integers.hpp>

namespace aarith {
template <typename Integer> using integer_range = std::pair<Integer, Integer>;

template <typename Integer> class integer_iter
{

    std::optional<Integer> current;
    std::optional<Integer> end;

public:
    using value_type = Integer;
    using refernce = Integer;
    using iterator_category = std::input_iterator_tag;
    using pointer = Integer*;
    using difference_type = void;

    // TODO make the default thingie static;

    integer_iter<Integer>()
        : current(std::nullopt)
        , end(std::nullopt)
    {
    }

    integer_iter<Integer>(const Integer start, const Integer end)
        : current(start)
        , end(end)
    {
    }

    Integer operator*() const
    {
        return *current;
    }
    Integer* operator->() const
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
            current = add(*current, Integer::one());
        }

        //        std::cout << current << "\n";
        return *this;
    }

    friend bool operator==(integer_iter<Integer> const& lhs, integer_iter<Integer> const& rhs)
    {
        return lhs.current == rhs.current;
    }
    friend bool operator!=(integer_iter<Integer> const& lhs, integer_iter<Integer> const& rhs)
    {
        return !(lhs == rhs);
    }
};

template <typename Integer> integer_iter<Integer> begin(integer_range<Integer> r)
{
    return integer_iter<Integer>(r.first, r.second);
}
template <typename Integer> integer_iter<Integer> end([[maybe_unused]] integer_range<Integer> r)
{
    return integer_iter<Integer>();
}

} // namespace aarith