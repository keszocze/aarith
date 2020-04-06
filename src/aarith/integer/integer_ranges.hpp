#pragma once

#include <aarith/integer/integers.hpp>

namespace aarith {
template <typename Integer> using integer_range = std::pair<Integer, Integer>;

template <typename Integer> class integer_iter
{

    bool reached_end = false;
    Integer current;
    Integer end;

public:
    using value_type = Integer;
    using refernce = Integer;
    using iterator_category = std::input_iterator_tag;
    using pointer = Integer*;
    using difference_type = void;


    // TODO make the default thingie static;

    integer_iter<Integer>()
        : reached_end(true)
        , current(Integer::zero())
        , end(Integer::zero())
    {
    }

    integer_iter<Integer>(const Integer start, const Integer end)
        : reached_end(false)
        , current(start)
        , end(end)
    {
    }

    Integer operator*() const
    {
        return current;
    }
    Integer* operator->() const
    {
        return &current;
    }

    integer_iter& operator++()
    { // preincrement
        if (current == end)
        {
            reached_end = true;
        }
        else
        {
            current = add(current, Integer::one());
        }
        //        std::cout << current << "\n";
        return *this;
    }

    friend bool operator==(integer_iter<Integer> const& lhs, integer_iter<Integer> const& rhs)
    {
        // TODO hier auch die obere Grenze mit testen, damit man unterschiedliche Iteratoren haben kann
        //        std::cout << "testing for equality\n";
        const bool both_at_the_end = lhs.reached_end == rhs.reached_end && lhs.reached_end;
        const bool both_at_same_val = lhs.current == rhs.current;
        //        std::cout << "both at end: " << both_at_the_end << "\tboth same val: " <<
        //        both_at_same_val << "\n";
        return both_at_the_end || both_at_same_val;
    }
    friend bool operator!=(integer_iter<Integer> const& lhs, integer_iter<Integer> const& rhs)
    {
        //        std::cout << "testing for inequality by ";
        return !(lhs == rhs);
    }
};

template <typename Integer> integer_iter<Integer> begin(integer_range<Integer> r)
{
    //    std::cout << "generating begin() with " << r.first << " to " << r.second << "\n";
    return integer_iter<Integer>(r.first, r.second);
}
template <typename Integer> integer_iter<Integer> end([[maybe_unused]] integer_range<Integer> r)
{
    //    std::cout << "generating end() with empty sentinel value\n";
    return integer_iter<Integer>();
}

} // namespace aarith