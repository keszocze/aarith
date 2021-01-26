#pragma once

#include <aarith/posit.hpp>

using namespace aarith;

static const size_t N = 3;
static const size_t ES = 1;

using Posit = posit<N, ES>;
using Tile = tile<N, ES>;
using Valid = valid<N, ES>;

// Posit pᵢ is the i-th posit if you start with p₀ = NaR and then
// progress counter-clockwise on the projective reals.

static const Posit p0 = Posit::from(0b100);
static const Posit p1 = Posit::from(0b101);
static const Posit p2 = Posit::from(0b110);
static const Posit p3 = Posit::from(0b111);
static const Posit p4 = Posit::from(0b000);
static const Posit p5 = Posit::from(0b001);
static const Posit p6 = Posit::from(0b010);
static const Posit p7 = Posit::from(0b011);

// Tile tᵢ is the tile with value pᵢ and u-bit set to 0. It is the
// tile that represents pᵢ exactly.

static const Tile t0 = Tile::from(p0, false);
static const Tile t1 = Tile::from(p1, false);
static const Tile t2 = Tile::from(p2, false);
static const Tile t3 = Tile::from(p3, false);
static const Tile t4 = Tile::from(p4, false);
static const Tile t5 = Tile::from(p5, false);
static const Tile t6 = Tile::from(p6, false);
static const Tile t7 = Tile::from(p7, false);

// Tile uᵢ is the tile with value pᵢ and u-bit set to 1. It is the
// tile that represents the open interval (pᵢ, incremented(pᵢ)).

static const Tile u0 = Tile::from(p0, true);
static const Tile u1 = Tile::from(p1, true);
static const Tile u2 = Tile::from(p2, true);
static const Tile u3 = Tile::from(p3, true);
static const Tile u4 = Tile::from(p4, true);
static const Tile u5 = Tile::from(p5, true);
static const Tile u6 = Tile::from(p6, true);
static const Tile u7 = Tile::from(p7, true);

static const std::vector<Tile> all_tiles = {t0, t1, t2, t3, t4, t5, t6, t7,
                                            u0, u1, u2, u3, u4, u5, u6, u7};

// The following valids should cover most use cases. We have open and closed
// bounds. Some intervals that cross infinity and some do not.

static const Valid v0 = Valid::from(t4, t6);
static const Valid v1 = Valid::from(t3, t4);
static const Valid v2 = Valid::from(t2, t6);
static const Valid v3 = Valid::from(t7, t1);
static const Valid v4 = Valid::from(t7, t1);
static const Valid v5 = Valid::from(u0, u1);
static const Valid v6 = Valid::from(u5, u6);
static const Valid v7 = Valid::from(t2, u4);
static const Valid v8 = Valid::from(u3, t6);
