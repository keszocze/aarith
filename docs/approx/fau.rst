The FAU Adder
=============

When computing with unsigned integers
-------------------------------------

The FAU adder is an adder that speed up the computation of the sum by splitting the carry chain.
In order to keep the error small, some bits of the least-significant part are used in a carry prediction
unit.

See :ref:`the uses cases <fau_adder>` and [Echavarria2016]_ for details.

**Header** ``aaarith/integer/integer_approx_operations.hpp``

.. doxygenfunction:: FAUadder

.. doxygenfunction:: FAUsubtractor


When computing with floating_points
-----------------------------------

The FAU adder can also be used when computing with :ref:`floating_point <floating_point>` numbers. Here, the FAU adder
ist used to approximately compute the mantissa.

**Header** ``aaarith/float/float_approx_operations.hpp``

.. doxygenfunction:: FAU_add

.. doxygenfunction:: FAU_sub