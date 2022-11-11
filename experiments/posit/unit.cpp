/**
 * Evaluating Unit Distribution
 *
 * That is: What is the ratio of values that fall into [-1,1]?
 */

#include <cstdint>
#include <string>

#include <aarith/float.hpp>
#include <aarith/posit.hpp>

#include "support.hpp"

using namespace aarith;

class result
{
public:
    uint64_t unit;
    uint64_t bigger;
    uint64_t special;

    result()
        : unit(0)
        , bigger(0)
        , special(0)
    {
    }

    double unit_ratio() const
    {
        return this->unit / this->float_sum();
    }

    double bigger_ratio() const
    {
        return this->bigger / this->float_sum();
    }

    double special_ratio() const
    {
        return this->special / this->float_sum();
    }

private:
    uint64_t sum() const
    {
        return this->unit + this->bigger + this->special;
    }

    double float_sum() const
    {
        return static_cast<double>(this->sum());
    }
};

template <size_t E, size_t M, typename WT = uint64_t> static result result_for_float()
{
    using Float = floating_point<E, M, WT>;
    result res;

    for_each_float<Float>([&](const Float& x) {
        const auto one = Float::one();

        if (x.is_nan() || x.is_inf())
        {
            res.special += 1;
        }
        else if (x >= -one && x <= one)
        {
            res.unit += 1;
        }
        else
        {
            res.bigger += 1;
        }
    });

    return res;
}

template <size_t N, size_t ES, typename WT = uint64_t> static result result_for_posit()
{
    using Posit = posit<N, ES, WT>;
    result res;

    for_each_posit<Posit>([&](const Posit& p) {
        const auto one = Posit::one();

        if (p.is_nar())
        {
            res.special += 1;
        }
        else if (p >= -one && p <= one)
        {
            res.unit += 1;
        }
        else
        {
            res.bigger += 1;
        }
    });

    return res;
}

template <typename Posit> static void report_for_posit(const std::string& name)
{
    constexpr size_t N = Posit::width();
    constexpr size_t ES = Posit::exponent_size();

    std::cerr << "computing ratios for " << name << std::endl;
    const auto res = result_for_posit<N, ES>();

    std::cout << name << ";";
    std::cout << res.unit_ratio() << ";";
    std::cout << res.bigger_ratio() << ";";
    std::cout << res.special_ratio() << "\n";
}

template <typename Float> static void report_for_float(const std::string& name)
{
    constexpr size_t E = Float::exponent_width();
    constexpr size_t M = Float::mantissa_width();

    std::cerr << "computing ratios for " << name << std::endl;
    const auto res = result_for_float<E, M>();

    std::cout << name << ";";
    std::cout << res.unit_ratio() << ";";
    std::cout << res.bigger_ratio() << ";";
    std::cout << res.special_ratio() << "\n";
}

int main()
{
    std::cout << "type;unit;bigger;special\n";

    report_for_posit<posit8>("posit8");
    report_for_posit<posit16>("posit16");
    report_for_posit<posit32>("posit32");

    report_for_float<quarter_precision>("quarter");
    report_for_float<half_precision>("half");
    report_for_float<single_precision>("single");
}
