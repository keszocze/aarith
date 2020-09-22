add_aarith_test(integer8-correctness FILES check_integer8_operations.cpp int_correctness_check_fun.hpp)
add_aarith_test(integer10-correctness FILES check_integer10_operations.cpp int_correctness_check_fun.hpp)
add_aarith_test(integer12-correctness FILES check_integer12_operations.cpp int_correctness_check_fun.hpp)
add_aarith_test(integer14-correctness FILES check_integer14_operations.cpp int_correctness_check_fun.hpp)
add_aarith_test(integer16-correctness FILES check_integer16_operations.cpp int_correctness_check_fun.hpp)

add_aarith_test(uinteger32-add-correctness FILES check_uinteger32_add.cpp int_correctness_check_fun.hpp)
add_aarith_test(uinteger32-sub-correctness FILES check_uinteger32_sub.cpp int_correctness_check_fun.hpp)
add_aarith_test(uinteger32-mul-correctness FILES check_uinteger32_mul.cpp int_correctness_check_fun.hpp)
add_aarith_test(uinteger32-karazuba-correctness FILES check_uinteger32_karazuba.cpp int_correctness_check_fun.hpp)
add_aarith_test(uinteger32-div-correctness FILES check_uinteger32_div.cpp int_correctness_check_fun.hpp)
add_aarith_test(uinteger32-mod-correctness FILES check_uinteger32_mod.cpp int_correctness_check_fun.hpp)

add_aarith_test(integer32-add-correctness FILES check_integer32_add.cpp int_correctness_check_fun.hpp)
add_aarith_test(integer32-sub-correctness FILES check_integer32_sub.cpp int_correctness_check_fun.hpp)
add_aarith_test(integer32-mul-correctness FILES check_integer32_mul.cpp int_correctness_check_fun.hpp)
add_aarith_test(integer32-div-correctness FILES check_integer32_div.cpp int_correctness_check_fun.hpp)
add_aarith_test(integer32-mod-correctness FILES check_integer32_mod.cpp int_correctness_check_fun.hpp)


if(MPIR_FOUND)
    add_aarith_test("check-against-mpir" FILES check_against_mpir.cpp INCLUDES ${MPIR_INCLUDE_DIR} LIBS ${MPIR_LIBRARIES})
endif()