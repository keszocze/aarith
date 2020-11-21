#pragma once


#include "util.hpp"


#include <aarith/integer.hpp>

namespace dsp_packing {

static constexpr size_t APortWidth = 27;
static constexpr size_t DPortWidth = 27;
static constexpr size_t BPortWidth = 18;
static constexpr size_t CPortWidth = 48;
static constexpr size_t PinPortWidth = 48;
static constexpr size_t PPortWidth = 48;

using APort = aarith::integer<APortWidth>;
using DPort = aarith::integer<DPortWidth>;
using BPort = aarith::integer<BPortWidth>;
using CPort = aarith::integer<CPortWidth>;
using PinPort = aarith::integer<PinPortWidth>;
using PPort = aarith::integer<PPortWidth>;



/**
 * @brief Struct storing the inputs to the DSP
 */
struct DSPInput
{
    APort A;                //! First of the summands
    DPort D;                //! Second of the summands
    BPort B;               //! The other half of the multiplication
    CPort C = CPort::zero(); //! The optional carry
    PinPort Pin = PinPort::zero(); //! The optional chained input
};



/**
 * @brief Models the DSP's behaviour
 * @param A The first summand
 * @param D The second summand
 * @param B The other part of the product
 * @param C The carry
 * @return ((A+D)*B)+C
 */
[[nodiscard]] PPort dsp(const APort& A, const DPort& D, const BPort& B,
                         const CPort C = CPort::zero(), const PinPort Pin = PinPort::zero() )
{

    const APort sum = A + D;


    const APort B_{B};


    // TODO ask Akif about preferred automatic width extension
    const PPort prod = expanding_mul(sum, B_);


    const PPort result = prod + C;

    // TODO ask Akif how this P_in input actually works

    return result;
}

/**
 * @brief Helper function calling the DSP with a given input struct
 * @param input The input parameters to the DSP packed in a strut
 * @return ((A+D)*B)+C
 */
[[nodiscard]] PPort dsp(const DSPInput& input)
{
    return dsp(input.A, input.D, input.B, input.C, input.Pin);
}




}

