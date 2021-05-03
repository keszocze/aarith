[![Linux CI](https://github.com/keszocze/aarith/actions/workflows/linux.yml/badge.svg?branch=master)](https://github.com/keszocze/aarith/actions/workflows/linux.yml)
[![Documentation Status](https://readthedocs.org/projects/aarith/badge/?version=latest)](https://aarith.readthedocs.io/en/latest/?badge=latest)

# Aarith: An Arbitrary Precision Number Library


Aarith is a header-only, arbitrary precision number library for C++. It is intended to be used as a drop-in replacement
of the native data types.

Aarith currently supports

* IEEE 754 like floating-point numbers of arbitrary bit-width for both, the exponent and the mantissa
* Two's complement integers of arbitrary bit-width (signed and unsigned)

[Read the full documentation](https://aarith.readthedocs.io)

## Installation

Copy the contents of this repository into a `<destination>` folder and include Aarith in your CMake build using:
```cmake
add_subdirectory(<destination>)
```
You can then link your target `<targetname>` against Aarith with:
```cmake
target_link_libraries(<targetname> PUBLIC aarith::Library)
```


## Usage Example
Aarith is intended to be used as a drop-in replacement for the native data types. You only need to include
the headers and can start using Aarith immediately:

```c++
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
```
This gives the expected output of

```shell
$ ./arithmetic_example
a + b = 30
a_+ b_ = 30
x * y = 7.5
x_* y_ = 7.5
```


To make usage of Aarith more convenient, the following type aliases are shipped with Aarith:

```c++
using half_precision = floating_point<5, 10, uint64_t>;
using single_precision = floating_point<8, 23, uint64_t>;
using double_precision = floating_point<11, 52, uint64_t>;
using quadruple_precision = floating_point<15, 112, uint64_t>;
using bfloat16 = floating_point<8, 7, uint64_t>;
using tensorfloat32 = floating_point<8, 10, uint64_t>;
```

Further examples for how to use aarith can be found in the `examples` and `experiments` folders
(the tests can also give a good idea of how to use aarith).

We also refer the interested user to the publication listed below.

## License

Aarith *does* use a license! At some point in time, we will even tell you which one

## Publication

If you use Aarith (e.g., in your publication), please cite

Oliver Keszocze, Marcel Brand, Christian Heidorn, und Jürgen Teich. „Aarith: An Arbitrary Precision Number Library“,
In: ACM/SIGAPP Symposium On Applied Computing (SAC'21). March 2021.

Bibtex:

```bibtex
@inproceedings{Keszocze2021,
  title = {Aarith: {{An Arbitrary Precision Number Library}}},
  booktitle = {ACM/SIGAPP Symposium On Applied Computing},
  author = {Keszocze, Oliver and Brand, Marcel and Heidorn, Christian and Teich, Jürgen},
  date = {2021-03},
  location = {{Virtual Event, South Korea}},
  series = {{{SAC}}'21}
}
```