cmake_minimum_required(VERSION 3.16)

# if you want to add sub directory.
# add_subdirectory(sample_name)


# if you want to add test and test executable.
add_executable(common-result-test common_result_test.cpp)
	target_link_libraries(common-result-test common)

# if you want to add config for test.
# add_test(
#    NAME test-name
#    COMMAND $<TARGET_FILE:sample-test-name> commad_line_option
#    )
#    set_property(
#        TEST test-name
#        PROPERTY LABELS label_name1 label_name2 ...
#        )

add_test(
    NAME common-result-test-TraitResult_tprint_update_curve
    COMMAND $<TARGET_FILE:common-result-test> TraitResult_tprint_update_curve
    )
    set_property(
        TEST common-result-test-TraitResult_tprint_update_curve
        PROPERTY LABELS lib common common-result TraitResult_tprint_update_curve
        )

add_test(
    NAME common-result-test-TraitResult_gprint_update_curve
    COMMAND $<TARGET_FILE:common-result-test> TraitResult_gprint_update_curve
    )
    set_property(
        TEST common-result-test-TraitResult_gprint_update_curve
        PROPERTY LABELS lib common common-result TraitResult_gprint_update_curve
        )

add_test(
    NAME common-result-test-TraitResult_tprint_update_variable
    COMMAND $<TARGET_FILE:common-result-test> TraitResult_tprint_update_variable
    )
    set_property(
        TEST common-result-test-TraitResult_tprint_update_variable
        PROPERTY LABELS lib common common-result TraitResult_tprint_update_variable
        )

add_test(
    NAME common-result-test-TraitResult_tprint_variable
    COMMAND $<TARGET_FILE:common-result-test> TraitResult_tprint_variable
    )
    set_property(
        TEST common-result-test-TraitResult_tprint_variable
        PROPERTY LABELS lib common common-result TraitResult_tprint_variable
        )

add_test(
    NAME common-result-test-TraitResult_tprint_init_variable
    COMMAND $<TARGET_FILE:common-result-test> TraitResult_tprint_init_variable
    )
    set_property(
        TEST common-result-test-TraitResult_tprint_init_variable
        PROPERTY LABELS lib common common-result TraitResult_tprint_init_variable
        )

add_test(
    NAME common-result-test-tprint_all_result
    COMMAND $<TARGET_FILE:common-result-test> tprint_all_result
    )
    set_property(
        TEST common-result-test-tprint_all_result
        PROPERTY LABELS lib common common-result tprint_all_result
        )

add_test(
    NAME common-result-test-tprint_all_variables
    COMMAND $<TARGET_FILE:common-result-test> tprint_all_variables
    )
    set_property(
        TEST common-result-test-tprint_all_variables
        PROPERTY LABELS lib common common-result tprint_all_variables
        )