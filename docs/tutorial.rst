Quick Start
===========


Aarith is intended to be used as a drop-in replacement for the native data types. You only need to include
the headers and can start using Aarith immediately:

.. code-block:: c++

    #include <aarith/float.hpp>
    #include <aarith/integer.hpp>

    int main()
    {
        using namespace aarith;
        uint64_t a = 10, b = 20;
        uinteger<64> a_ = 10, b_ = 20;
        std::cout << "a + b = " << (a + b) << "\n";
        std::cout << "a_+ b_ = " << (a_ + b_) << "\n";

        float x=3.0F, y=2.5F;
        floating_point<8,23> x_{3.0F}, y_{2.5F};
        std::cout << "x + y = " << (x * y) << "\n";
        std::cout << "x_+ y_ = " << (x_ * y_) << "\n";
    }

This gives the expected output of

.. code-block:: sh

    $ ./arithmetic_example
    a + b = 30
    a_+ b_ = 30
    x * y = 7.5
    x_* y_ = 7.5


.. hint::
    To make usage of Aarith more convenient, the following type aliases are shipped with Aarith:

    .. code-block:: c++

        using half_precision = floating_point<5, 10, uint64_t>;
        using single_precision = floating_point<8, 23, uint64_t>;
        using double_precison = floating_point<11, 52, uint64_t>;
        using quadruple_precision = floating_point<15, 112, uint64_t>;
        using bfloat16 = floating_point<8, 7, uint64_t>;
        using tensorfloat32 = floating_point<8, 10, uint64_t>;


Further examples for how to use aarith can be found in the `examples` and `experiments` folders
(the tests can also give a good idea of how to use aarith).

We also refer the interested user to [Keszocze2021]_.

