Installation
============

Foo bar blubb, der Witterauf weiß, wie's geht!

Single header file
------------------

Single header file is generated from the sources.
Can be downloaded from releases ...
Just copy the header file into your project and include it.
Done!

With CMake and `add_subdirectory`
---------------------------------

Check out the `aarith` repository into your project (possibly as a submodule) and type in

.. code-block:: CMake

   add_subdirectory(aarith)

You can then use the library by linking against `aarith::aarith`.

With CMake and installation
---------------------------

Clone the `aarith` repository and install it.
You can then use it by typing into your `CMakeLists.txt`

.. code-block:: CMake

   find_package(aarith [REQUIRED])

You can then use the library by linking against `aarith::aarith`.

