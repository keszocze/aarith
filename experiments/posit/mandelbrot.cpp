/**
 * Based on https://rosettacode.org/wiki/Mandelbrot_set#C.2B.2B and
 * https://github.com/dario-marvin/Mandelbrot/blob/master/Mandelbrot.cc
 */

#include <complex>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <aarith/float.hpp>
#include <aarith/posit.hpp>

#include "support.hpp"

template <typename Real> int mandelbrot(const Real& real, const Real& imag)
{
    int limit = 100;
    Real zReal = real;
    Real zImag = imag;

    for (int i = 0; i < limit; ++i)
    {
        Real r2 = zReal * zReal;
        Real i2 = zImag * zImag;

        if (r2 + i2 > Real(4.0))
        {
            return i;
        }

        zImag = Real(2.0) * zReal * zImag + imag;
        zReal = r2 - i2 + real;
    }

    return limit;
}

template <typename Real, size_t Width = 960, size_t Height = 640>
static void render_to_file(const std::string& filename, bool report_progress = true)
{
    using namespace aarith;

    const size_t max_iterations = 100;

    Real x_start = Real(-2.0);
    Real x_fin = Real(1.0);
    Real y_start = Real(-1.0);
    Real y_fin = Real(1.0);
    Real dx = (x_fin - x_start) / (Real(double(Width)) - Real(1.0));
    Real dy = (y_fin - y_start) / (Real(double(Height)) - Real(1.0));

    bitmap<Width, Height> image;

    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            const Real x = x_start + Real(double(j)) * dx; // current real value
            const Real y = y_fin - Real(double(i)) * dy;   // current imaginary value
            const int value = mandelbrot(x, y);

            const int capped = std::min(value, 100);
            const uint8_t channel = double((Real(double(capped)) / Real(100.0)) * Real(255.0));

            const color pixel = {channel, channel, channel};

            image.at(j, i) = pixel;
        }

        if (report_progress)
        {
            fprintf(stderr, "\33[2K\r");
            fprintf(stderr, "mandelbrot: %s: rendered i=%d/%zu", filename.c_str(), i + 1, Height);
        }
    }

    if (report_progress)
    {
        fputc('\n', stderr);
    }

    // write ppm to outfile

    if (report_progress)
    {
        fprintf(stderr, "mandelbrot: writing %s ", filename.c_str());
    }

    image.to_ppm_file(filename);

    if (report_progress)
    {
        fprintf(stderr, "done\n");
    }
}

template <typename Real, size_t Width = 960, size_t Height = 640>
static std::thread render_to_file_async(const std::string filename)
{
    return std::thread([=]() {
	render_to_file<Real, Width, Height>(filename, false);
    });
}

int main()
{
    using namespace aarith;

    std::vector<std::thread> tids;

    tids.push_back(render_to_file_async<aarith::posit<8, 0>>("mandelbrot-p8-0.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<8, 1>>("mandelbrot-p8-1.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<8, 2>>("mandelbrot-p8-2.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<8, 3>>("mandelbrot-p8-3.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<8, 4>>("mandelbrot-p8-4.ppm"));

    tids.push_back(render_to_file_async<aarith::posit<9, 0>>("mandelbrot-p9-0.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<9, 1>>("mandelbrot-p9-1.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<9, 2>>("mandelbrot-p9-2.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<9, 3>>("mandelbrot-p9-3.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<9, 4>>("mandelbrot-p9-4.ppm"));

    tids.push_back(render_to_file_async<aarith::posit<10, 0>>("mandelbrot-p10-0.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<10, 1>>("mandelbrot-p10-1.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<10, 2>>("mandelbrot-p10-2.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<10, 3>>("mandelbrot-p10-3.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<10, 4>>("mandelbrot-p10-4.ppm"));

    tids.push_back(render_to_file_async<aarith::posit<11, 0>>("mandelbrot-p11-0.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<11, 1>>("mandelbrot-p11-1.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<11, 2>>("mandelbrot-p11-2.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<11, 3>>("mandelbrot-p11-3.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<11, 4>>("mandelbrot-p11-4.ppm"));

    tids.push_back(render_to_file_async<aarith::posit<12, 0>>("mandelbrot-p12-0.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<12, 1>>("mandelbrot-p12-1.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<12, 2>>("mandelbrot-p12-2.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<12, 3>>("mandelbrot-p12-3.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<12, 4>>("mandelbrot-p12-4.ppm"));

    tids.push_back(render_to_file_async<aarith::posit<13, 0>>("mandelbrot-p13-0.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<13, 1>>("mandelbrot-p13-1.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<13, 2>>("mandelbrot-p13-2.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<13, 3>>("mandelbrot-p13-3.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<13, 4>>("mandelbrot-p13-4.ppm"));

    tids.push_back(render_to_file_async<aarith::posit<14, 0>>("mandelbrot-p14-0.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<14, 1>>("mandelbrot-p14-1.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<14, 2>>("mandelbrot-p14-2.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<14, 3>>("mandelbrot-p14-3.ppm"));
    tids.push_back(render_to_file_async<aarith::posit<14, 4>>("mandelbrot-p14-4.ppm"));

    // https://www.khronos.org/opengl/wiki/Small_Float_Formats
    tids.push_back(render_to_file_async<aarith::floating_point<5, 5>>("mandelbrot-f5-5.ppm"));
    tids.push_back(render_to_file_async<aarith::floating_point<5, 6>>("mandelbrot-f5-6.ppm"));
    tids.push_back(render_to_file_async<aarith::floating_point<5, 9>>("mandelbrot-f5-9.ppm"));
    tids.push_back(render_to_file_async<aarith::floating_point<5, 10>>("mandelbrot-f5-10.ppm"));

    // standard types for comparison
    tids.push_back(render_to_file_async<posit16>("mandelbrot-p16.ppm"));
    tids.push_back(render_to_file_async<float16>("mandelbrot-f16.ppm"));

    for (std::thread& tid : tids)
    {
        tid.join();
    }
}
