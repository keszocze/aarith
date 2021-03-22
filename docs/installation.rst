Installation
============

Copy the contents of this repository into a ``<destination>`` folder and include Aarith in your CMake build using:

.. code-block:: cmake

    add_subdirectory(<destination>)

You can then link your target ``<targetname>`` against Aarith with:

.. code-block:: cmake

    target_link_libraries(<targetname> PUBLIC aarith::Library)



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
The documentation is [available online](add link!). If you want to build it locally, you need Python,
`Sphinx <https://www.sphinx-doc.org/en/master/>`_, the
`readthedocs <https://readthedocs.io>`_ Theme, `breathe <https://breathe.readthedocs.io/en/latest/>`_
and `Doxygen <https://www.doxygen.nl/index.html>`_.
