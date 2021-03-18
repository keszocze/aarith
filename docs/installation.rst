Installation
============

Foo bar blubb, der Witterauf weiß, wie's geht!

Single header file
------------------

Single header file is generated from the sources.
Can be downloaded from releases ...
Just copy the header file into your project and include it.
Done!

With CMake and ``add_subdirectory``
-----------------------------------

Check out the Aarith repository into your project (possibly as a submodule) and add the following line
to your project's ``CMakeLists.txt`` file.

.. code-block:: CMake

   add_subdirectory(aarith)

You can then use the library by linking against `aarith::Library`.

With CMake and installation
---------------------------

Clone the `aarith` repository and install it.
You can then use it by typing into your `CMakeLists.txt`

.. code-block:: CMake

   find_package(aarith [REQUIRED])

You can then use the library by linking against ``aarith::Library``.



Requirements/Dependencies
-------------------------

Aarith is intended to be used without introducing further dependencies. You can use it immediately without having to
further software.

Tests
^^^^^

If you want to run the tests, you need `catch2 <https://github.com/catchorg/Catch2>`_, the
`Google benchmark library <https://github.com/google/benchmark>`_.

If you want to run the tests against other number libraries, you need to install `MPFR <http://www.mpfr.org>`_ and
`MPIR <https://mpir.org>`_.

Documentation
^^^^^^^^^^^^^
The documentation is [available online](add link!). If you want to build it locally, you need Python **add rest**
