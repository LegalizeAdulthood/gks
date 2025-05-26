find_package(GTest CONFIG REQUIRED)

include(GoogleTest)

add_executable(test-gks
    test.cpp
    error.h
    error.cpp
    gkcl_test.cpp
    gkop_test.cpp
    wsop_test.cpp
    wsac_test.cpp
    TestGKSOpen.h TestGKSOpen.cpp
    TestWSOpen.h TestWSOpen.cpp
    )
target_link_libraries(test-gks PUBLIC gks GTest::gtest_main)
target_folder(test-gks "Tests")
gtest_discover_tests(test-gks)
