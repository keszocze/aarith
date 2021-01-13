#include <catch.hpp>

#include <aarith/posit.hpp>

SCENARIO("Valid Tile Comparisons")
{
    using namespace aarith;

    GIVEN("All valid tiles for <4, 1> posits")
    {
        using Posit = posit<4, 1>;
        using Tile = valid_tile<4, 1>;

        // Test cases based on Posit Arithmetic, Gustafson, October 2017,
        // pp. 19. While the following formulation feels a bit awkward, it is
        // a very valuable test to have.

        // First we get all <4, 1> posits.

        const Posit p16 = Posit::from(0b0111U);
        const Posit p4 = Posit::from(0b0110U);
        const Posit p2 = Posit::from(0b0101U);
        const Posit p1 = Posit::from(0b0100U);
        const Posit p1d2 = Posit::from(0b0011);
        const Posit p1d4 = Posit::from(0b0010);
        const Posit p1d16 = Posit::from(0b0001);
        const Posit p0 = Posit::from(0b0000);
        const Posit pn1d16 = Posit::from(0b1111);
        const Posit pn1d4 = Posit::from(0b1110);
        const Posit pn1d2 = Posit::from(0b1101);
        const Posit pn1 = Posit::from(0b1100);
        const Posit pn2 = Posit::from(0b1011);
        const Posit pn4 = Posit::from(0b1010);
        const Posit pn16 = Posit::from(0b1001);
        const Posit pnar = Posit::from(0b1000);

        // Now we construct the tiles that represent an exact posit.

        const Tile t16 = Tile::from(p16, false);
        const Tile t4 = Tile::from(p4, false);
        const Tile t2 = Tile::from(p2, false);
        const Tile t1 = Tile::from(p1, false);
        const Tile t1d2 = Tile::from(p1d2, false);
        const Tile t1d4 = Tile::from(p1d4, false);
        const Tile t1d16 = Tile::from(p1d16, false);
        const Tile t0 = Tile::from(p0, false);
        const Tile tn1d16 = Tile::from(pn1d16, false);
        const Tile tn1d4 = Tile::from(pn1d4, false);
        const Tile tn1d2 = Tile::from(pn1d2, false);
        const Tile tn1 = Tile::from(pn1, false);
        const Tile tn2 = Tile::from(pn2, false);
        const Tile tn4 = Tile::from(pn4, false);
        const Tile tn16 = Tile::from(pn16, false);
        const Tile tnar = Tile::from(pnar, false);

        // And here we construct the tiles that are ranges

        const Tile u16 = Tile::from(p16, true);
        const Tile u4 = Tile::from(p4, true);
        const Tile u2 = Tile::from(p2, true);
        const Tile u1 = Tile::from(p1, true);
        const Tile u1d2 = Tile::from(p1d2, true);
        const Tile u1d4 = Tile::from(p1d4, true);
        const Tile u1d16 = Tile::from(p1d16, true);
        const Tile u0 = Tile::from(p0, true);
        const Tile un1d16 = Tile::from(pn1d16, true);
        const Tile un1d4 = Tile::from(pn1d4, true);
        const Tile un1d2 = Tile::from(pn1d2, true);
        const Tile un1 = Tile::from(pn1, true);
        const Tile un2 = Tile::from(pn2, true);
        const Tile un4 = Tile::from(pn4, true);
        const Tile un16 = Tile::from(pn16, true);
        const Tile unar = Tile::from(pnar, true);

        THEN("Assert that they compare as expected")
        {
            REQUIRE(t16 > t4);
            REQUIRE(t16 > t2);
            REQUIRE(t16 > t1);
            REQUIRE(t16 > t1d2);
            REQUIRE(t16 > t1d4);
            REQUIRE(t16 > t1d16);
            REQUIRE(t16 > t0);
            REQUIRE(t16 > tn1d16);
            REQUIRE(t16 > tn1d4);
            REQUIRE(t16 > tn1d2);
            REQUIRE(t16 > tn1);
            REQUIRE(t16 > tn2);
            REQUIRE(t16 > tn4);
            REQUIRE(t16 > tn16);
            REQUIRE(t16 < u16);
            REQUIRE(t16 > u4);
            REQUIRE(t16 > u2);
            REQUIRE(t16 > u1);
            REQUIRE(t16 > u1d2);
            REQUIRE(t16 > u1d4);
            REQUIRE(t16 > u1d16);
            REQUIRE(t16 > u0);
            REQUIRE(t16 > un1d16);
            REQUIRE(t16 > un1d4);
            REQUIRE(t16 > un1d2);
            REQUIRE(t16 > un1);
            REQUIRE(t16 > un2);
            REQUIRE(t16 > un4);
            REQUIRE(t16 > un16);

            REQUIRE(t4 > t2);
            REQUIRE(t4 > t1);
            REQUIRE(t4 > t1d2);
            REQUIRE(t4 > t1d4);
            REQUIRE(t4 > t1d16);
            REQUIRE(t4 > t0);
            REQUIRE(t4 > tn1d16);
            REQUIRE(t4 > tn1d4);
            REQUIRE(t4 > tn1d2);
            REQUIRE(t4 > tn1);
            REQUIRE(t4 > tn2);
            REQUIRE(t4 > tn4);
            REQUIRE(t4 > tn16);
            REQUIRE(t4 < u16);
            REQUIRE(t4 < u4);
            REQUIRE(t4 > u2);
            REQUIRE(t4 > u1);
            REQUIRE(t4 > u1d2);
            REQUIRE(t4 > u1d4);
            REQUIRE(t4 > u1d16);
            REQUIRE(t4 > u0);
            REQUIRE(t4 > un1d16);
            REQUIRE(t4 > un1d4);
            REQUIRE(t4 > un1d2);
            REQUIRE(t4 > un1);
            REQUIRE(t4 > un2);
            REQUIRE(t4 > un4);
            REQUIRE(t4 > un16);

            REQUIRE(t2 > t1);
            REQUIRE(t2 > t1d2);
            REQUIRE(t2 > t1d4);
            REQUIRE(t2 > t1d16);
            REQUIRE(t2 > t0);
            REQUIRE(t2 > tn1d16);
            REQUIRE(t2 > tn1d4);
            REQUIRE(t2 > tn1d2);
            REQUIRE(t2 > tn1);
            REQUIRE(t2 > tn2);
            REQUIRE(t2 > tn4);
            REQUIRE(t2 > tn16);
            REQUIRE(t2 < u16);
            REQUIRE(t2 < u4);
            REQUIRE(t2 < u2);
            REQUIRE(t2 > u1);
            REQUIRE(t2 > u1d2);
            REQUIRE(t2 > u1d4);
            REQUIRE(t2 > u1d16);
            REQUIRE(t2 > u0);
            REQUIRE(t2 > un1d16);
            REQUIRE(t2 > un1d4);
            REQUIRE(t2 > un1d2);
            REQUIRE(t2 > un1);
            REQUIRE(t2 > un2);
            REQUIRE(t2 > un4);
            REQUIRE(t2 > un16);

            REQUIRE(t1 > t1d2);
            REQUIRE(t1 > t1d4);
            REQUIRE(t1 > t1d16);
            REQUIRE(t1 > t0);
            REQUIRE(t1 > tn1d16);
            REQUIRE(t1 > tn1d4);
            REQUIRE(t1 > tn1d2);
            REQUIRE(t1 > tn1);
            REQUIRE(t1 > tn2);
            REQUIRE(t1 > tn4);
            REQUIRE(t1 > tn16);
            REQUIRE(t1 < u16);
            REQUIRE(t1 < u4);
            REQUIRE(t1 < u2);
            REQUIRE(t1 < u1);
            REQUIRE(t1 > u1d2);
            REQUIRE(t1 > u1d4);
            REQUIRE(t1 > u1d16);
            REQUIRE(t1 > u0);
            REQUIRE(t1 > un1d16);
            REQUIRE(t1 > un1d4);
            REQUIRE(t1 > un1d2);
            REQUIRE(t1 > un1);
            REQUIRE(t1 > un2);
            REQUIRE(t1 > un4);
            REQUIRE(t1 > un16);

            REQUIRE(t1d2 > t1d4);
            REQUIRE(t1d2 > t1d16);
            REQUIRE(t1d2 > t0);
            REQUIRE(t1d2 > tn1d16);
            REQUIRE(t1d2 > tn1d4);
            REQUIRE(t1d2 > tn1d2);
            REQUIRE(t1d2 > tn1);
            REQUIRE(t1d2 > tn2);
            REQUIRE(t1d2 > tn4);
            REQUIRE(t1d2 > tn16);
            REQUIRE(t1d2 < u16);
            REQUIRE(t1d2 < u4);
            REQUIRE(t1d2 < u2);
            REQUIRE(t1d2 < u1);
            REQUIRE(t1d2 < u1d2);
            REQUIRE(t1d2 > u1d4);
            REQUIRE(t1d2 > u1d16);
            REQUIRE(t1d2 > u0);
            REQUIRE(t1d2 > un1d16);
            REQUIRE(t1d2 > un1d4);
            REQUIRE(t1d2 > un1d2);
            REQUIRE(t1d2 > un1);
            REQUIRE(t1d2 > un2);
            REQUIRE(t1d2 > un4);
            REQUIRE(t1d2 > un16);

            REQUIRE(t1d4 > t1d16);
            REQUIRE(t1d4 > t0);
            REQUIRE(t1d4 > tn1d16);
            REQUIRE(t1d4 > tn1d4);
            REQUIRE(t1d4 > tn1d2);
            REQUIRE(t1d4 > tn1);
            REQUIRE(t1d4 > tn2);
            REQUIRE(t1d4 > tn4);
            REQUIRE(t1d4 > tn16);
            REQUIRE(t1d4 < u16);
            REQUIRE(t1d4 < u4);
            REQUIRE(t1d4 < u2);
            REQUIRE(t1d4 < u1);
            REQUIRE(t1d4 < u1d2);
            REQUIRE(t1d4 < u1d4);
            REQUIRE(t1d4 > u1d16);
            REQUIRE(t1d4 > u0);
            REQUIRE(t1d4 > un1d16);
            REQUIRE(t1d4 > un1d4);
            REQUIRE(t1d4 > un1d2);
            REQUIRE(t1d4 > un1);
            REQUIRE(t1d4 > un2);
            REQUIRE(t1d4 > un4);
            REQUIRE(t1d4 > un16);

            REQUIRE(t1d16 > t0);
            REQUIRE(t1d16 > tn1d16);
            REQUIRE(t1d16 > tn1d4);
            REQUIRE(t1d16 > tn1d2);
            REQUIRE(t1d16 > tn1);
            REQUIRE(t1d16 > tn2);
            REQUIRE(t1d16 > tn4);
            REQUIRE(t1d16 > tn16);
            REQUIRE(t1d16 < u16);
            REQUIRE(t1d16 < u4);
            REQUIRE(t1d16 < u2);
            REQUIRE(t1d16 < u1);
            REQUIRE(t1d16 < u1d2);
            REQUIRE(t1d16 < u1d4);
            REQUIRE(t1d16 < u1d16);
            REQUIRE(t1d16 > u0);
            REQUIRE(t1d16 > un1d16);
            REQUIRE(t1d16 > un1d4);
            REQUIRE(t1d16 > un1d2);
            REQUIRE(t1d16 > un1);
            REQUIRE(t1d16 > un2);
            REQUIRE(t1d16 > un4);
            REQUIRE(t1d16 > un16);

            REQUIRE(t0 > tn1d16);
            REQUIRE(t0 > tn1d4);
            REQUIRE(t0 > tn1d2);
            REQUIRE(t0 > tn1);
            REQUIRE(t0 > tn2);
            REQUIRE(t0 > tn4);
            REQUIRE(t0 > tn16);
            REQUIRE(t0 < u16);
            REQUIRE(t0 < u4);
            REQUIRE(t0 < u2);
            REQUIRE(t0 < u1);
            REQUIRE(t0 < u1d2);
            REQUIRE(t0 < u1d4);
            REQUIRE(t0 < u1d16);
            REQUIRE(t0 > un1d16);
            REQUIRE(t0 > un1d4);
            REQUIRE(t0 > un1d2);
            REQUIRE(t0 > un1);
            REQUIRE(t0 > un2);
            REQUIRE(t0 > un4);
            REQUIRE(t0 > un16);

            REQUIRE(tn1d16 > tn1d4);
            REQUIRE(tn1d16 > tn1d2);
            REQUIRE(tn1d16 > tn1);
            REQUIRE(tn1d16 > tn2);
            REQUIRE(tn1d16 > tn4);
            REQUIRE(tn1d16 > tn16);
            REQUIRE(tn1d16 < u16);
            REQUIRE(tn1d16 < u4);
            REQUIRE(tn1d16 < u2);
            REQUIRE(tn1d16 < u1);
            REQUIRE(tn1d16 < u1d2);
            REQUIRE(tn1d16 < u1d4);
            REQUIRE(tn1d16 < u1d16);
            REQUIRE(tn1d16 < u0);
            REQUIRE(tn1d16 > un1d16);
            REQUIRE(tn1d16 > un1d4);
            REQUIRE(tn1d16 > un1d2);
            REQUIRE(tn1d16 > un1);
            REQUIRE(tn1d16 > un2);
            REQUIRE(tn1d16 > un4);
            REQUIRE(tn1d16 > un16);

            REQUIRE(tn1d4 > tn1d2);
            REQUIRE(tn1d4 > tn1);
            REQUIRE(tn1d4 > tn2);
            REQUIRE(tn1d4 > tn4);
            REQUIRE(tn1d4 > tn16);
            REQUIRE(tn1d4 < u16);
            REQUIRE(tn1d4 < u4);
            REQUIRE(tn1d4 < u2);
            REQUIRE(tn1d4 < u1);
            REQUIRE(tn1d4 < u1d2);
            REQUIRE(tn1d4 < u1d4);
            REQUIRE(tn1d4 < u1d16);
            REQUIRE(tn1d4 < u0);
            REQUIRE(tn1d4 < un1d16);
            REQUIRE(tn1d4 > un1d4);
            REQUIRE(tn1d4 > un1d2);
            REQUIRE(tn1d4 > un1);
            REQUIRE(tn1d4 > un2);
            REQUIRE(tn1d4 > un4);
            REQUIRE(tn1d4 > un16);

            REQUIRE(tn1d2 > tn1);
            REQUIRE(tn1d2 > tn2);
            REQUIRE(tn1d2 > tn4);
            REQUIRE(tn1d2 > tn16);
            REQUIRE(tn1d2 < u16);
            REQUIRE(tn1d2 < u4);
            REQUIRE(tn1d2 < u2);
            REQUIRE(tn1d2 < u1);
            REQUIRE(tn1d2 < u1d2);
            REQUIRE(tn1d2 < u1d4);
            REQUIRE(tn1d2 < u1d16);
            REQUIRE(tn1d2 < u0);
            REQUIRE(tn1d2 < un1d16);
            REQUIRE(tn1d2 < un1d4);
            REQUIRE(tn1d2 > un1d2);
            REQUIRE(tn1d2 > un1);
            REQUIRE(tn1d2 > un2);
            REQUIRE(tn1d2 > un4);
            REQUIRE(tn1d2 > un16);

            REQUIRE(tn1 > tn2);
            REQUIRE(tn1 > tn4);
            REQUIRE(tn1 > tn16);
            REQUIRE(tn1 < u16);
            REQUIRE(tn1 < u4);
            REQUIRE(tn1 < u2);
            REQUIRE(tn1 < u1);
            REQUIRE(tn1 < u1d2);
            REQUIRE(tn1 < u1d4);
            REQUIRE(tn1 < u1d16);
            REQUIRE(tn1 < u0);
            REQUIRE(tn1 < un1d16);
            REQUIRE(tn1 < un1d4);
            REQUIRE(tn1 < un1d2);
            REQUIRE(tn1 > un1);
            REQUIRE(tn1 > un2);
            REQUIRE(tn1 > un4);
            REQUIRE(tn1 > un16);

            REQUIRE(tn2 > tn4);
            REQUIRE(tn2 > tn16);
            REQUIRE(tn2 < u16);
            REQUIRE(tn2 < u4);
            REQUIRE(tn2 < u2);
            REQUIRE(tn2 < u1);
            REQUIRE(tn2 < u1d2);
            REQUIRE(tn2 < u1d4);
            REQUIRE(tn2 < u1d16);
            REQUIRE(tn2 < u0);
            REQUIRE(tn2 < un1d16);
            REQUIRE(tn2 < un1d4);
            REQUIRE(tn2 < un1d2);
            REQUIRE(tn2 < un1);
            REQUIRE(tn2 > un2);
            REQUIRE(tn2 > un4);
            REQUIRE(tn2 > un16);

            REQUIRE(tn4 > tn16);
            REQUIRE(tn4 < u16);
            REQUIRE(tn4 < u4);
            REQUIRE(tn4 < u2);
            REQUIRE(tn4 < u1);
            REQUIRE(tn4 < u1d2);
            REQUIRE(tn4 < u1d4);
            REQUIRE(tn4 < u1d16);
            REQUIRE(tn4 < u0);
            REQUIRE(tn4 < un1d16);
            REQUIRE(tn4 < un1d4);
            REQUIRE(tn4 < un1d2);
            REQUIRE(tn4 < un1);
            REQUIRE(tn4 < un2);
            REQUIRE(tn4 > un4);
            REQUIRE(tn4 > un16);

            REQUIRE(tn16 < u16);
            REQUIRE(tn16 < u4);
            REQUIRE(tn16 < u2);
            REQUIRE(tn16 < u1);
            REQUIRE(tn16 < u1d2);
            REQUIRE(tn16 < u1d4);
            REQUIRE(tn16 < u1d16);
            REQUIRE(tn16 < u0);
            REQUIRE(tn16 < un1d16);
            REQUIRE(tn16 < un1d4);
            REQUIRE(tn16 < un1d2);
            REQUIRE(tn16 < un1);
            REQUIRE(tn16 < un2);
            REQUIRE(tn16 < un4);
            REQUIRE(tn16 > un16);

            REQUIRE(u16 > u2);
            REQUIRE(u16 > u1);
            REQUIRE(u16 > u1d2);
            REQUIRE(u16 > u1d4);
            REQUIRE(u16 > u1d16);
            REQUIRE(u16 > u0);
            REQUIRE(u16 > un1d16);
            REQUIRE(u16 > un1d4);
            REQUIRE(u16 > un1d2);
            REQUIRE(u16 > un1);
            REQUIRE(u16 > un2);
            REQUIRE(u16 > un4);
            REQUIRE(u16 > un16);

            REQUIRE(u4 > u2);
            REQUIRE(u4 > u1);
            REQUIRE(u4 > u1d2);
            REQUIRE(u4 > u1d4);
            REQUIRE(u4 > u1d16);
            REQUIRE(u4 > u0);
            REQUIRE(u4 > un1d16);
            REQUIRE(u4 > un1d4);
            REQUIRE(u4 > un1d2);
            REQUIRE(u4 > un1);
            REQUIRE(u4 > un2);
            REQUIRE(u4 > un4);
            REQUIRE(u4 > un16);

            REQUIRE(u2 > u1);
            REQUIRE(u2 > u1d2);
            REQUIRE(u2 > u1d4);
            REQUIRE(u2 > u1d16);
            REQUIRE(u2 > u0);
            REQUIRE(u2 > un1d16);
            REQUIRE(u2 > un1d4);
            REQUIRE(u2 > un1d2);
            REQUIRE(u2 > un1);
            REQUIRE(u2 > un2);
            REQUIRE(u2 > un4);
            REQUIRE(u2 > un16);

            REQUIRE(u1 > u1d2);
            REQUIRE(u1 > u1d4);
            REQUIRE(u1 > u1d16);
            REQUIRE(u1 > u0);
            REQUIRE(u1 > un1d16);
            REQUIRE(u1 > un1d4);
            REQUIRE(u1 > un1d2);
            REQUIRE(u1 > un1);
            REQUIRE(u1 > un2);
            REQUIRE(u1 > un4);
            REQUIRE(u1 > un16);

            REQUIRE(u1d2 > u1d4);
            REQUIRE(u1d2 > u1d16);
            REQUIRE(u1d2 > u0);
            REQUIRE(u1d2 > un1d16);
            REQUIRE(u1d2 > un1d4);
            REQUIRE(u1d2 > un1d2);
            REQUIRE(u1d2 > un1);
            REQUIRE(u1d2 > un2);
            REQUIRE(u1d2 > un4);
            REQUIRE(u1d2 > un16);

            REQUIRE(u1d4 > u1d16);
            REQUIRE(u1d4 > u0);
            REQUIRE(u1d4 > un1d16);
            REQUIRE(u1d4 > un1d4);
            REQUIRE(u1d4 > un1d2);
            REQUIRE(u1d4 > un1);
            REQUIRE(u1d4 > un2);
            REQUIRE(u1d4 > un4);
            REQUIRE(u1d4 > un16);

            REQUIRE(u1d16 > u0);
            REQUIRE(u1d16 > un1d16);
            REQUIRE(u1d16 > un1d4);
            REQUIRE(u1d16 > un1d2);
            REQUIRE(u1d16 > un1);
            REQUIRE(u1d16 > un2);
            REQUIRE(u1d16 > un4);
            REQUIRE(u1d16 > un16);

            REQUIRE(u0 > un1d16);
            REQUIRE(u0 > un1d4);
            REQUIRE(u0 > un1d2);
            REQUIRE(u0 > un1);
            REQUIRE(u0 > un2);
            REQUIRE(u0 > un4);
            REQUIRE(u0 > un16);

            REQUIRE(un1d16 > un1d4);
            REQUIRE(un1d16 > un1d2);
            REQUIRE(un1d16 > un1);
            REQUIRE(un1d16 > un2);
            REQUIRE(un1d16 > un4);
            REQUIRE(un1d16 > un16);

            REQUIRE(un1d4 > un1d2);
            REQUIRE(un1d4 > un1);
            REQUIRE(un1d4 > un2);
            REQUIRE(un1d4 > un4);
            REQUIRE(un1d4 > un16);

            REQUIRE(un1d2 > un1);
            REQUIRE(un1d2 > un2);
            REQUIRE(un1d2 > un4);
            REQUIRE(un1d2 > un16);

            REQUIRE(un1 > un2);
            REQUIRE(un1 > un4);
            REQUIRE(un1 > un16);

            REQUIRE(un2 > un4);
            REQUIRE(un2 > un16);

            REQUIRE(un4 > un16);
        }
    }
}
