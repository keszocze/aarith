Two's Complement Integers
=========================

**Header** ``aarith/integer/integers.hpp``

The template classes ``integer`` and ``uinteger`` represent signed and unsigned integers of arbitrary,
but compile-time static, precision stored in two's complement format

The aarith integers exhibit the usual overflow/underflow behavior (i.e. modulo 2^n) which is ``not`` undefined behavior!

.. doxygenclass:: aarith::uinteger
   :members:


.. doxygenclass:: aarith::integer
   :members: