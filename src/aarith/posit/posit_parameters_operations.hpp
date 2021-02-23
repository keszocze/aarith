#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit_parameters<N, ES, WT> abs(const posit_parameters<N, ES, WT>& p)
{
    return p.absolute_value();
}

} // namespace aarith
