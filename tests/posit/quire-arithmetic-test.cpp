#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"

TEMPLATE_TEST_CASE_SIG("quire arithmetic with NaR", "[quire][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;

    constexpr auto pnar = Posit::nar();
    constexpr auto pzero = Posit::zero();
    constexpr auto pone = Posit::one();

    const auto qnar = quire(pnar);
    const auto qzero = quire(pzero);
    const auto qone = quire(pone);

    CHECK(qnar + qzero == qnar);
    CHECK(qnar + qone == qnar);
    CHECK(qnar + qnar == qnar);

    CHECK(qnar - qzero == qnar);
    CHECK(qnar - qone == qnar);
    CHECK(qnar - qnar == qnar);

    CHECK(qnar * qzero == qnar);
    CHECK(qnar * qone == qnar);
    CHECK(qnar * qnar == qnar);

    CHECK(qnar / qzero == qnar);
    CHECK(qnar / qone == qnar);
    CHECK(qnar / qnar == qnar);
}

TEMPLATE_TEST_CASE_SIG("quire arithmetic with zero", "[quire][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;

    constexpr auto pnar = Posit::nar();
    constexpr auto pzero = Posit::zero();
    constexpr auto pone = Posit::one();

    const auto qnar = quire(pnar);
    const auto qzero = quire(pzero);
    const auto qone = quire(pone);

    CHECK(qzero + qzero == qzero);
    CHECK(qzero + qone == qone);
    CHECK(qzero + qnar == qnar);

    CHECK(qzero - qzero == qzero);
    CHECK(qzero - qone == -qone);
    CHECK(qzero - qnar == qnar);

    CHECK(qzero * qzero == qzero);
    CHECK(qzero * qone == qzero);
    CHECK(qzero * qnar == qnar);

    CHECK(qzero / qzero == qnar);
    CHECK(qzero / qone == qzero);
    CHECK(qzero / qnar == qnar);
}

TEMPLATE_TEST_CASE_SIG("quire arithmetic with one", "[quire][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;

    constexpr auto pnar = Posit::nar();
    constexpr auto pzero = Posit::zero();
    constexpr auto pone = Posit::one();

    const auto qnar = quire(pnar);
    const auto qzero = quire(pzero);
    const auto qone = quire(pone);

    CHECK(qone + qzero == qone);
    CHECK(qone + qnar == qnar);

    CHECK(qone - qzero == qone);
    CHECK(qone - qone == qzero);
    CHECK(qone - qnar == qnar);

    CHECK(qone * qzero == qzero);
    CHECK(qone * qone == qone);
    CHECK(qone * qnar == qnar);

    CHECK(qone / qzero == qnar);
    CHECK(qone / qone == qone);
    CHECK(qone / qnar == qnar);
}
