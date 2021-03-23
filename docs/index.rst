Aarith: An Arbitrary Precision Number Library
=============================================

.. image:: https://github.com/keszocze/aarith/actions/workflows/linux.yml/badge.svg?branch=master
    :target:https://github.com/keszocze/aarith/actions/workflows/linux.yml
    :alt: Linux Build Status

.. image:: https://readthedocs.org/projects/aarith/badge/?version=latest
    :target: https://aarith.readthedocs.io/en/latest/?badge=latest
    :alt: Documentation Status

Aarith is a header-only, arbitrary precision number library for C++. It is intended to be used as a drop-in replacement
of the native data types.

Aarith currently supports

* IEEE 754 like floating-point numbers of arbitrary bit-width for both, the exponent and the mantissa
* Two's complement integers of arbitrary bit-width (signed and unsigned)



.. toctree::
   :maxdepth: 1
   :caption: General

   installation
   tutorial
   usecases
   philosophy
   literature


.. toctree::
    :maxdepth: 1
    :caption: Integers

    api/integer

.. toctree::
    :maxdepth: 1
    :caption: Floating-Point Numbers

    api/float/class
    api/float/operations
    api/float/comparisons
    api/float/utilities



Publication
-----------


If you use Aarith (e.g., in your publication), please cite

Oliver Keszocze, Marcel Brand, Christian Heidorn, und Jürgen Teich. „Aarith: An Arbitrary Precision Number Library“,
In: ACM/SIGAPP Symposium On Applied Computing. March 2021.

Bibtex:

.. code-block:: tex

    @inproceedings{Keszocze2021,
      title = {Aarith: {{An Arbitrary Precision Number Library}}},
      booktitle = {ACM/SIGAPP Symposium On Applied Computing},
      author = {Keszocze, Oliver and Brand, Marcel and Heidorn, Christian and Teich, Jürgen},
      date = {2021-03},
      location = {{Virtual Event, South Korea}},
      series = {{{SAC}}'21}
    }




.. toctree::
    :maxdepth: 1
    :caption: Core & Utilities

    api/core/word_array
    api/core/stringnum
    api/core/traits
    api/core/bitcast

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`

