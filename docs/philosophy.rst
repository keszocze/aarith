Aarith's Philosophy
===================

Aarith is build with the following points in mind:

* Aarith is should easy to use:
    * e.g. no functions such as ``u_add_32(num_a, num_b)``, just plain ``num_a + num_b``
    * easily define new number formats, e.g. ``using bfloat16 = floating_point<8, 7, uint64_t>;`` (see [Burgess2019]_)
* Aarith should allow for easy access to the individual bits of the stored number:
    * for the design of new hardware units (e.g., implementing arithmetic units such as the FAU Adder [Echavarria2016]_, :ref:`see example <fau_adder>`
    * for debugging unexpected results
* Aarith should be publicly available

Easy to use
-----------
We want Aarith to blend in with ``C++``. That is, we want it to provide the usual operations, such as ``+`` or ``<<``.

The following program computes the sum of ``1.0`` and ``2.0`` using 200 digits using the  `MPFR <http://www.mpfr.org>`_
library.

.. code-block:: C++

    #include <stdio.h>

    #include <gmp.h>
    #include <mpfr.h>

    int main (void)
    {
      unsigned int i;
      mpfr_t s, t, u;

      mpfr_init2 (t, 200);
      mpfr_set_d (t, 1.0, GMP_RNDD);
      mpfr_init2 (s, 200);
      mpfr_set_d (s, 2.0, GMP_RNDD);
      mpfr_init2 (u, 200);
      mpfr_add (s, s, u, GMP_RNDD);
      printf ("Sum is ");
      mpfr_out_str (stdout, 10, 0, s, GMP_RNDD);
      putchar ('\n');
      mpfr_clear (s);
      mpfr_clear (t);
      mpfr_clear (u);
      return 0;
    }

The equivalent program using Aarith looks like this (the parameters for the exponent and mantissa width, i.e. ``E`` and
``M``, need to be chosen large enough to fit a 200 digits number):

.. code-block:: C++

    #include <aarith/float.hpp>
    #include <aarith/integer.hpp>

    int main()
    {
        floating_point<E,M> x_{1.0F}, y_{2.0F};
        std::cout << "Sum is " << (x + y) << "\n";

    }

No Surprises!
-------------

Aarith performs very little implicit type conversions. Most of the constructors are ``explicit``. Especially,
Aarith does not use ``typedef``'s involving native data types. This is motivated by the following situation.
Consider the following program:

.. code-block:: c++

    uint8_t u8 = 42; uinteger<8> aarith_u8{u8};
    std::cout << "uint8_t=" << u8
        << " (as int=" << int{u8} << ")"
        << " aarith::uinteger<8>="
        << aarith_u8 << "\n";

Running this program gives the following output:

.. code-block:: sh

    $ ./output_example
    uint8_t=* (as int=42) aarith::uinteger<8>=42

The asterisk ``*`` most likely was not was the user was expecting to see. Such a conversion is `never` carried
out by Aarith.

Speed
-----

Aarith is *not* extensively optimized for speed! There are other libraries for that. If raw speed is your
goal, try `MPFR <http://www.mpfr.org>`_ and `MPIR <https://mpir.org>`_.
