find_package(Catch2 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

include(GoogleTest)

add_executable(test-gks
    test.cpp
    error.h
    error.cpp
    gkcl.cpp
    gkop_test.cpp
    wsop.cpp
    wsac.cpp
    )
target_link_libraries(test-gks PUBLIC gks Catch2::Catch2 GTest::gtest_main)
target_folder(test-gks "Tests")
gtest_discover_tests(test-gks)
