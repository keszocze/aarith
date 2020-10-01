function(add_aarith_test testname)

    SET(targetname "${testname}-test")


    set (multiValueArgs FILES)
    cmake_parse_arguments(
            AARITH_TEST # prefix
            "" # no flags
            "" # no single value options
            "FILES;LIBS;INCLUDES" # multi value options
            ${ARGN}
    )

    add_executable(${targetname} ${AARITH_TEST_FILES})
    target_link_libraries(${targetname} PRIVATE Catch2::WithMain aarith::Library ${AARITH_TEST_LIBS})
    add_test(NAME ${targetname} COMMAND ${targetname})

    if (DEFINED AARITH_TEST_INCLUDES)
        target_include_directories(${targetname} PRIVATE ${AARITH_TEST_INCLUDES})
    endif()

endfunction(add_aarith_test)
