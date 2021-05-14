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

/**
 * Represents a single color with 8 bit for each channel.
 * Used together with class bitmap for drawing images.
 */
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

/**
 * A bitmap in memory. Can be exported in PPM format.  This class is
 * useful for drawing pretty pictures and visualizations.
 */
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

/**
 * Define an eight bit float type. It is honestly kind of useless.
 */
using quarter_precision = aarith::floating_point<4, 3>;

/**
 * Define floating point types that are easier to work with.
 */
using float8 = quarter_precision;
using float16 = aarith::half_precision;
using float32 = aarith::single_precision;
using float64 = aarith::double_precision;

/**
 * Implement sqrt for aarith floating points. Converts to double
 * and then does math with the libc sqrt implementation. Because
 * it does math in 64 bit, calling sqrt w/ floating points of greater
 * width will introduce error that is hard to track.
 */
template <size_t E, size_t M, typename WT>
inline aarith::floating_point<E, M, WT> sqrt(const aarith::floating_point<E, M, WT>& x)
{
    using Float = aarith::floating_point<E, M, WT>;
    return Float(sqrt(double(x)));
}

template <size_t E, size_t M, typename WT, size_t IN, typename IWT>
inline aarith::floating_point<E, M, WT> pow(const aarith::floating_point<E, M, WT>& x,
                                            const aarith::integer<IN, IWT>& k)
{
    using Int = aarith::integer<IN, IWT>;
    using Float = aarith::floating_point<E, M, WT>;

    Float power = Float::one();

    const auto niterations = expanding_abs(k);

    for (auto i = niterations.zero(); i < niterations; ++i)
    {
        if (k.is_negative())
        {
            power = power / x;
        }
        else
        {
            power = power * x;
        }
    }

    return power;
}
