cmake_minimum_required(VERSION 3.16)


add_executable(harmony-default-test harmony_default_test.cpp)
	target_link_libraries(harmony-default-test harmony)


add_test(
    NAME harmony-default-HarmonyOptimizer_optimize
    COMMAND $<TARGET_FILE:harmony-default-test> HarmonyOptimizer_optimize
    )
    set_property(
        TEST harmony-default-HarmonyOptimizer_optimize
        PROPERTY LABELS lib harmony harmony-default HarmonyOptimizer_optimize
        )

add_test(
    NAME harmony-default-HarmonySearchStrategy_detection_best_worst
    COMMAND $<TARGET_FILE:harmony-default-test> HarmonySearchStrategy_detection_best_worst
    )
    set_property(
        TEST harmony-default-HarmonySearchStrategy_detection_best_worst
        PROPERTY LABELS lib harmony harmony-default HarmonySearchStrategy_detection_best_worst
        )

add_test(
    NAME harmony-default-HarmonySearchStrategy_trade_harmony
    COMMAND $<TARGET_FILE:harmony-default-test> HarmonySearchStrategy_trade_harmony
    )
    set_property(
        TEST harmony-default-HarmonySearchStrategy_trade_harmony
        PROPERTY LABELS lib harmony harmony-default HarmonySearchStrategy_trade_harmony
        )

add_test(
    NAME harmony-default-HarmonySearchStrategy_gen_rng_vals
    COMMAND $<TARGET_FILE:harmony-default-test> HarmonySearchStrategy_gen_rng_vals
    )
    set_property(
        TEST harmony-default-HarmonySearchStrategy_gen_rng_vals
        PROPERTY LABELS lib harmony harmony-default HarmonySearchStrategy_gen_rng_vals
        )

add_test(
    NAME harmony-default-HarmonySearchStrategy_generate_harmony_function
    COMMAND $<TARGET_FILE:harmony-default-test> HarmonySearchStrategy_generate_harmony_function
    )
    set_property(
        TEST harmony-default-HarmonySearchStrategy_generate_harmony_function
        PROPERTY LABELS lib harmony harmony-default HarmonySearchStrategy_generate_harmony_function
        )