function(add_aarith_experiment testname)

    SET(targetname "${testname}-experiment")


    set (multiValueArgs FILES)
    cmake_parse_arguments(
            AARITH_EXP # prefix
            "" # no flags
            "" # no single value options
            "FILES;LIBS;INCLUDES" # multi value options
            ${ARGN}
    )

    add_executable(${targetname} ${AARITH_EXP_FILES})
    target_link_libraries(${targetname} PRIVATE aarith::Library ${AARITH_EXP_LIBS})


    if (DEFINED AARITH_EXP_INCLUDES)
        target_include_directories(${targetname} PRIVATE ${AARITH_EXP_INCLUDES})
    endif()

    target_compile_options(${targetname} PRIVATE "-O3")

endfunction(add_aarith_experiment)


add_aarith_experiment(integer8-correctness FILES check_integer8_operations.cpp int_correctness_check_fun.hpp)



add_aarith_experiment(integer16-correctness FILES check_integer16_operations.cpp int_correctness_check_fun.hpp)

add_aarith_experiment(uinteger32-add-correctness FILES check_uinteger32_add.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(uinteger32-sub-correctness FILES check_uinteger32_sub.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(uinteger32-mul-correctness FILES check_uinteger32_mul.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(uinteger32-karazuba-correctness FILES check_uinteger32_karazuba.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(uinteger32-div-correctness FILES check_uinteger32_div.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(uinteger32-mod-correctness FILES check_uinteger32_mod.cpp int_correctness_check_fun.hpp)

add_aarith_experiment(integer32-add-correctness FILES check_integer32_add.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(integer32-sub-correctness FILES check_integer32_sub.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(integer32-mul-correctness FILES check_integer32_mul.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(integer32-div-correctness FILES check_integer32_div.cpp int_correctness_check_fun.hpp)
add_aarith_experiment(integer32-mod-correctness FILES check_integer32_mod.cpp int_correctness_check_fun.hpp)

add_aarith_experiment("fau-adder" FILES eval_fau_adder.cpp)
add_aarith_experiment("check-against-mpir" FILES check_against_mpir.cpp INCLUDES ${MPIR_INCLUDE_DIR} LIBS ${MPIR_LIBRARIES})
