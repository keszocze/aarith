#pragma once

/**
 * support.hpp
 *
 * Inline helpers for use in posit experiments.
 */

#include <cmath>
#include <fstream>
#include <functional>
#include <ostream>
#include <string>

#include <aarith/float.hpp>
#include <aarith/integer.hpp>

/**
 * @brief Run function on each float.
 *
 * @tparam FloatType The aarith floating point type to iterate on.
 * @param operation The function to call on each float.
 */
template <typename FloatType>
inline void for_each_float(const std::function<void(const FloatType&)>& operation)
{
    using namespace aarith;

    constexpr size_t M = FloatType::mantissa_width();
    constexpr size_t E = FloatType::exponent_width();
    constexpr size_t N = 1 + M + E;

    using Int = uinteger<N>;
    Int bits = Int::zero();

    do
    {
        const word_array<N> current_bits = bits;
        const FloatType current = FloatType(current_bits);
        operation(current);
    } while (++bits);
}

/**
 * @brief Compute decimal accuracy for expected value x and actual value y.
 *
 * Based on "Beating Floating Point at its Own Game: Posit Arithmetic", 2017,
 * pp. 78.
 */
template <typename X, typename Y> inline double decimal_accuracy(const X& x, const Y& y)
{
    const double xd = static_cast<double>(x);
    const double yd = static_cast<double>(y);

    return fabs(log10(xd / yd));
}

struct color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    color()
        : red(0)
        , green(0)
        , blue(0)
    {
    }

    color(uint8_t ared, uint8_t agreen, uint8_t ablue)
        : red(ared)
        , green(agreen)
        , blue(ablue)
    {
    }

    static color black()
    {
        return color(0, 0, 0);
    }

    static color white()
    {
        return color(255, 255, 255);
    }
};

template <size_t Width, size_t Height> class bitmap
{
public:
    bitmap()
    {
        const size_t npixels = Width * Height;

        for (size_t i = 0; i < npixels; ++i)
        {
            this->pixels.emplace_back(0, 0, 0);
        }
    }

    color& at(const size_t x, const size_t y)
    {
        return this->pixels.at(index_for(x, y));
    }

    const color& at(const size_t x, const size_t y) const
    {
        return this->pixels.at(index_for(x, y));
    }

    void to_ppm(std::ostream& os) const
    {
        os << "P3"
           << "\n";
        os << Width << " " << Height << "\n";
        os << "255"
           << "\n";

        for (size_t y = 0; y < Height; ++y)
        {
            for (size_t x = 0; x < Width; ++x)
            {
                const color& mypixel = this->at(x, y);

                os << static_cast<int>(mypixel.red) << " ";
                os << static_cast<int>(mypixel.green) << " ";
                os << static_cast<int>(mypixel.blue) << "\n";
            }
        }
    }

    void to_ppm_file(const std::string& filename) const
    {
        std::ofstream out(filename);
        out.exceptions(std::ios_base::failbit);
        this->to_ppm(out);
    }

private:
    std::vector<color> pixels;

    size_t index_for(const size_t x, const size_t y) const
    {
        return x + y * Width;
    }
};
