cmake_minimum_required(VERSION 3.16)


add_executable(harmony-default-test harmony_default.cpp)
	target_link_libraries(harmony-default-test harmony)


add_test(
    NAME harmony-default-HarmonySearchStrategy_ranked_harmony
    COMMAND $<TARGET_FILE:harmony-default-test> HarmonySearchStrategy_ranked_harmony
    )
    set_property(
        TEST harmony-default-HarmonySearchStrategy_ranked_harmony
        PROPERTY LABELS lib harmony harmony-default HarmonySearchStrategy_ranked_harmony
        )