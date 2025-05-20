find_package(Catch2 CONFIG REQUIRED)

add_executable(test-gks
    test.cpp
    error.h
    error.cpp
    gkcl.cpp
    gkop.cpp
    wsop.cpp
    wsac.cpp
    )
target_link_libraries(test-gks PUBLIC gks Catch2::Catch2WithMain)
target_folder(test-gks "Tests")

add_custom_command(TARGET test-gks POST_BUILD COMMAND test-gks)
