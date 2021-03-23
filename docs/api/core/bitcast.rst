Bit Cast
========

**Header** ``aarith/core/bit_cast.hpp``

Unfortunately, there is no easy way to completely re-interpret bits as a different type. There is
`the usual <https://en.wikipedia.org/wiki/Type_punning#Use_of_union>`_ trick
of using an union. This is undefined behavior. This is why we opted to use the solution
`from this talk <https://www.youtube.com/watch?v=_qzMpk-22cc>`_.

In the near future, when C++20 is more widely availabe, we will switch to using
`std::bit_cast <https://en.cppreference.com/w/cpp/numeric/bit_cast>`_




.. doxygenfile:: bit_cast.hpp