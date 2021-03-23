#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include <aarith/core.hpp>
#include <aarith/core/core_string_utils.hpp>

using namespace aarith;


//SCENARIO("Creating word arrays from WordTypes larger than the storage word type", "[word_array]")
//{
//
//        using I = aarith::word_array<32,uint16_t>;
//
//        const I val{std::numeric_limits<int32_t>::max()};
//
//        std::cout << val << " " << to_binary(val) << "\n";
//
//}