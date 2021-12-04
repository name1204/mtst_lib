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