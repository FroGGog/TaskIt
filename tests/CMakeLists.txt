add_executable(core_test
    test_core.cpp
)

target_link_libraries(core_test
    PRIVATE
        gtest_main
        core
)

target_compile_features(core_test PRIVATE cxx_std_17)

include(GoogleTest)

gtest_discover_tests(core_test)